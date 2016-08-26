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

#include "gsl_vector.hpp"
#include "error.hpp"

#include <cmath>
#include <string>
#include <utility>

namespace flexiblesusy {

GSL_vector::GSL_vector()
   : vec(NULL)
{
}

GSL_vector::GSL_vector(std::size_t size)
{
   if (!size) {
      vec = NULL;
      return;
   }

   vec = gsl_vector_calloc(size);

   if (!vec)
      throw OutOfMemoryError(
         "Allocation of GSL_vector of size " + std::to_string(size)
         + " failed.");
}

GSL_vector::GSL_vector(const gsl_vector* v)
   : vec(NULL)
{
   assign(v);
}

GSL_vector::GSL_vector(const GSL_vector& other)
{
   vec = gsl_vector_alloc(other.size());

   if (!vec)
      throw OutOfMemoryError(
         "Allocation of GSL_vector of size " + std::to_string(other.size())
         + " failed.");

   gsl_vector_memcpy(vec, other.vec);
}

GSL_vector::GSL_vector(GSL_vector&& other) noexcept
{
   move_assign(std::move(other));
}

GSL_vector::GSL_vector(std::initializer_list<double> list)
{
   if (list.size() == 0) {
      vec = NULL;
      return;
   }

   vec = gsl_vector_alloc(list.size());

   if (!vec)
      throw OutOfMemoryError(
         "Allocation of GSL_vector of size " + std::to_string(list.size())
         + " failed.");

   std::copy(list.begin(), list.end(), gsl_vector_ptr(vec, 0));
}

GSL_vector::~GSL_vector()
{
   gsl_vector_free(vec);
}

void GSL_vector::assign(const gsl_vector* other)
{
   if (!other) {
      gsl_vector_free(vec);
      vec = NULL;
      return;
   }

   // avoid free and alloc if other has same size
   if (size() != other->size) {
      gsl_vector_free(vec);
      vec = gsl_vector_alloc(other->size);

      if (!vec)
         throw OutOfMemoryError(
            "Allocation of GSL_vector of size " + std::to_string(other->size)
            + " failed.");
   }

   gsl_vector_memcpy(vec, other);
}

bool GSL_vector::empty() const noexcept
{
   return size() == 0;
}

const GSL_vector& GSL_vector::operator=(const GSL_vector& rhs)
{
   if (this != &rhs)
      assign(rhs.vec);

   return *this;
}

GSL_vector& GSL_vector::operator=(GSL_vector&& rhs) noexcept
{
   if (this != &rhs) {
      gsl_vector_free(vec);
      move_assign(std::move(rhs));
   }

   return *this;
}

double& GSL_vector::operator[](std::size_t n)
{
   range_check(n);
   return *gsl_vector_ptr(vec, n);
}

double GSL_vector::operator[](std::size_t n) const
{
   range_check(n);
   return gsl_vector_get(vec, n);
}

std::size_t GSL_vector::size() const noexcept
{
   if (!vec) return 0;
   return vec->size;
}

const gsl_vector* GSL_vector::raw() const noexcept
{
   return vec;
}

gsl_vector* GSL_vector::raw() noexcept
{
   return vec;
}

void GSL_vector::set_all(double value) noexcept
{
   if (vec)
      gsl_vector_set_all(vec, value);
}

std::ostream& operator<<(std::ostream& ostr, const GSL_vector& vec)
{
   std::cout << "(";

   for (std::size_t i = 0; i < vec.size(); i++) {
      std::cout << vec[i];
      if (i < vec.size() - 1)
         std::cout << ", ";
   }

   std::cout << ")";
}

void GSL_vector::move_assign(GSL_vector&& other) noexcept
{
   vec = other.vec;
   other.vec = NULL;
}

void GSL_vector::range_check(std::size_t n) const
{
   if (!vec)
      throw OutOfBoundsError(
         "GSL_vector::operator[]: index " + std::to_string(n)
         + " out of range for vector of size 0.");

   if (n >= size())
      throw OutOfBoundsError(
         "GSL_vector::operator[]: index " + std::to_string(n)
         + " out of range for vector of size " + std::to_string(size()) + ".");
}

double* begin(GSL_vector& v)
{
   if (v.size())
      return gsl_vector_ptr(v.raw(), 0);

   return nullptr;
}

double* end(GSL_vector& v)
{
   if (v.size()) {
      double* last = gsl_vector_ptr(v.raw(), v.size() - 1);
      return ++last;
   }

   return nullptr;
}

const double* cbegin(const GSL_vector& v)
{
   if (v.size())
      return gsl_vector_const_ptr(v.raw(), 0);

   return nullptr;
}

const double* cend(const GSL_vector& v)
{
   if (v.size()) {
      const double* last = gsl_vector_const_ptr(v.raw(), v.size() - 1);
      return ++last;
   }

   return nullptr;
}

} // namespace flexiblesusy
