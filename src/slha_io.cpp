// ====================================================================
// This file is part of FlexibleSUSY.
//
// FlexibleSUSY is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published
// by the Free Software Foundation, either version 3 of the License,
// or (at your option) any later version.
//
// FlexibleSUSY is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with FlexibleSUSY.  If not, see
// <http://www.gnu.org/licenses/>.
// ====================================================================

#include "slha_io.hpp"
#include "logger.hpp"
#include "wrappers.hpp"
#include "lowe.h"
#include "linalg.h"

#include <fstream>

namespace flexiblesusy {

SLHA_io::SLHA_io()
   : data()
   , modsel()
{
}

/**
 * @brief opens SLHA input file and reads the content
 * @param file_name SLHA input file name
 */
void SLHA_io::read_from_file(const std::string& file_name)
{
   std::ifstream ifs(file_name);
   data.clear();
   data.read(ifs);
}

void SLHA_io::read_modsel()
{
   using namespace std::placeholders;
   SLHA_io::Tuple_processor modsel_processor
      = std::bind(&SLHA_io::process_modsel_tuple, std::ref(modsel), _1, _2);

   read_block("MODSEL", modsel_processor);
}

void SLHA_io::fill(QedQcd& oneset) const
{
   using namespace std::placeholders;
   SLHA_io::Tuple_processor sminputs_processor
      = std::bind(&SLHA_io::process_sminputs_tuple, std::ref(oneset), _1, _2);

   read_block("SMINPUTS", sminputs_processor);
}

void SLHA_io::read_block(const std::string& block_name, Tuple_processor processor) const
{
   if (data.find(block_name) == data.cend())
      return;

   for (SLHAea::Block::const_iterator line = data.at(block_name).cbegin(),
        end = data.at(block_name).cend(); line != end; ++line) {
      if (!line->is_data_line())
         continue;

      if (line->size() >= 2) {
         const int key = SLHAea::to<int>((*line)[0]);
         const double value = SLHAea::to<double>((*line)[1]);
         processor(key, value);
      } else {
         WARNING(block_name << " entry has not enough columns");
      }
   }
}

void SLHA_io::set_block(const std::ostringstream& lines, Position position)
{
   SLHAea::Block block;
   block.str(lines.str());
   data.erase(block.name());
   if (position == front)
      data.push_front(block);
   else
      data.push_back(block);
}

/**
 * This function treats a given scalar as 1x1 matrix.  Such a case is
 * not defined in the SLHA standard, but we still handle it to avoid
 * problems.
 */
void SLHA_io::set_block(const std::string& name, double value,
                        const std::string& symbol, double scale)
{
   std::ostringstream ss;
   ss << "Block " << name;
   if (scale != 0.)
      ss << " Q= " << FORMAT_NUMBER(scale);
   ss << '\n'
      << boost::format(mixing_matrix_formatter) % 1 % 1 % value % symbol;

   set_block(ss);
}

void SLHA_io::set_block(const std::string& name, const DoubleMatrix& matrix,
                        const std::string& symbol, double scale)
{
   std::ostringstream ss;
   ss << "Block " << name;
   if (scale != 0.)
      ss << " Q= " << FORMAT_NUMBER(scale);
   ss << '\n';

   for (int i = 1; i <= matrix.displayRows(); ++i)
      for (int k = 1; k <= matrix.displayCols(); ++k) {
         ss << boost::format(mixing_matrix_formatter) % i % k % matrix(i,k)
            % (symbol + "(" + std::to_string(i) + "," + std::to_string(k) + ")");
      }

   set_block(ss);
}

void SLHA_io::set_block(const std::string& name, const Eigen::MatrixXd& matrix,
                        const std::string& symbol, double scale)
{
   std::ostringstream ss;
   ss << "Block " << name;
   if (scale != 0.)
      ss << " Q= " << FORMAT_NUMBER(scale);
   ss << '\n';

   const int rows = matrix.rows();
   const int cols = matrix.cols();
   for (int i = 1; i <= rows; ++i)
      for (int k = 1; k <= cols; ++k) {
         ss << boost::format(mixing_matrix_formatter) % i % k % matrix(i-1,k-1)
            % (symbol + "(" + std::to_string(i) + "," + std::to_string(k) + ")");
      }

   set_block(ss);
}

void SLHA_io::set_block(const std::string& name, const ComplexMatrix& matrix,
                        const std::string& symbol, double scale)
{
   std::ostringstream ss;
   ss << "Block " << name;
   if (scale != 0.)
      ss << " Q= " << FORMAT_NUMBER(scale);
   ss << '\n';

   for (int i = 1; i <= matrix.displayRows(); ++i)
      for (int k = 1; k <= matrix.displayCols(); ++k) {
         ss << boost::format(mixing_matrix_formatter) % i % k
            % Re(matrix(i,k))
            % ("Re(" + symbol + "(" + std::to_string(i) + "," + std::to_string(k) + "))");
      }

   set_block(ss);
}

void SLHA_io::write_to_file(const std::string& file_name)
{
   std::ofstream ofs(file_name);
   write_to_stream(ofs);
}

void SLHA_io::write_to_stream(std::ostream& ostr)
{
   if (ostr.good())
      ostr << data;
}

/**
 * fill Modsel struct from given key - value pair
 *
 * @param modsel MODSEL data
 * @param key SLHA key in MODSEL
 * @param value value corresponding to key
 */
void SLHA_io::process_modsel_tuple(Modsel& modsel, int key, double value)
{
   switch (key) {
   case 1:
   case 3:
   case 4:
   case 5:
   case 6:
   case 11:
   case 21:
      WARNING("MODSEL key " << key << " currently not supported");
      break;
   case 12:
      modsel.parameter_output_scale = value;
      break;
   default:
      WARNING("Unrecognized key in MODSEL: " << key);
      break;
   }
}

/**
 * fill oneset from given key - value pair
 *
 * @param oneset low-energy data set
 * @param key SLHA key in SMINPUTS
 * @param value value corresponding to key
 */
void SLHA_io::process_sminputs_tuple(QedQcd& oneset, int key, double value)
{
   switch (key) {
   case 1:
      oneset.setAlpha(ALPHA, 1.0 / value);
      break;
   case 2:
      // Gmu cannot be set yet
      break;
   case 3:
      oneset.setAlpha(ALPHAS, value);
      break;
   case 4:
      // MZ cannot be set yet
      // oneset.setMu(value);
      break;
   case 5:
      oneset.setMass(mBottom, value);
      oneset.setMbMb(value);
      break;
   case 6:
      oneset.setPoleMt(value);
      break;
   case 7:
      oneset.setMass(mTau, value);
      oneset.setPoleMtau(value);
      break;
   case 8:
      break;
   case 11:
      oneset.setMass(mElectron, value);
      break;
   case 12:
      break;
   case 13:
      oneset.setMass(mMuon, value);
      break;
   case 14:
      break;
   case 21:
      oneset.setMass(mDown, value);
      break;
   case 22:
      oneset.setMass(mUp, value);
      break;
   case 23:
      oneset.setMass(mStrange, value);
      break;
   case 24:
      oneset.setMass(mCharm, value);
      break;
   default:
      WARNING("Unrecognized key in SMINPUTS: " << key);
      break;
   }
}

} // namespace flexiblesusy
