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

#ifndef WEINBERG_ANGLE_H
#define WEINBERG_ANGLE_H

namespace flexiblesusy {

namespace weinberg_angle {

class Weinberg_angle {
public:
   Weinberg_angle();
   ~Weinberg_angle();

   void set_number_of_iterations(unsigned);
   void set_precision_goal(double);
   void set_alpha_em_drbar(double);
   void set_fermi_contant(double);
   void set_self_energy_z_at_mz(double);
   void set_self_energy_z_at_0(double);
   void set_self_energy_w_at_mw(double);

   double get_rho_hat() const;

   double calculate() const;

private:
   unsigned number_of_iterations; ///< maximum number of iterations
   double precision_goal;         ///< precision goal
   double alpha_em_drbar;         ///< alpha_em(MZ, DR-bar)
   double fermi_contant;          ///< fermi constant
   double self_energy_z_at_mz;    ///< self-energy Z at p = MZ
   double self_energy_z_at_0;     ///< self-energy Z at p = MZ
   double self_energy_w_at_mw;    ///< self-energy W at p = MW

   double rho_hat;
};

} // namespace weinberg_angle

} // namespace flexiblesusy

#endif
