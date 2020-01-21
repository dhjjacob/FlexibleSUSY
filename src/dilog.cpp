// ====================================================================
// This file is part of GM2Calc.
//
// GM2Calc is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published
// by the Free Software Foundation, either version 3 of the License,
// or (at your option) any later version.
//
// GM2Calc is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GM2Calc.  If not, see
// <http://www.gnu.org/licenses/>.
// ====================================================================

#include "dilog.hpp"
#include <cmath>
#include <limits>

namespace flexiblesusy {

namespace {
   template <typename T>
   T sqr(T x) noexcept { return x*x; }

   template <typename T>
   std::complex<T> cadd(T a, const std::complex<T>& b) noexcept
   {
      return std::complex<T>(a + std::real(b), std::imag(b));
   }

   template <typename T>
   std::complex<T> cadd(const std::complex<T>& a, const std::complex<T>& b) noexcept
   {
      return std::complex<T>(std::real(a) + std::real(b),
                             std::imag(a) + std::imag(b));
   }

   template <typename T>
   std::complex<T> cmul(const std::complex<T>& a, T b) noexcept
   {
      return std::complex<T>(std::real(a) * b, std::imag(a) * b);
   }

   template <typename T>
   std::complex<T> cmul(const std::complex<T>& a, const std::complex<T>& b) noexcept
   {
      return std::complex<T>(
         std::real(a) * std::real(b) - std::imag(a) * std::imag(b),
         std::real(a) * std::imag(b) + std::imag(a) * std::real(b));
   }
} // namespace

/**
 * @brief Clausen function \f$\mathrm{Cl}_2(\theta) = \mathrm{Im}(\mathrm{Li}_2(e^{i\theta}))\f$
 * @param x real angle
 * @return \f$\mathrm{Cl}_2(\theta)\f$
 */
double clausen_2(double x) noexcept
{
   const double PI = 3.141592653589793;
   const std::complex<double> i(0.0, 1.0);

   while (x >= 2*PI) {
      x -= 2*PI;
   }

   while (x < 0.0) {
      x += 2*PI;
   }

   if (std::abs(x) < std::numeric_limits<double>::epsilon() ||
       std::abs(x - PI) < std::numeric_limits<double>::epsilon() ||
       std::abs(x - 2*PI) < std::numeric_limits<double>::epsilon()) {
      return 0.0;
   }

   return std::imag(dilog(std::exp(i*x)));
}

/**
 * @brief Clausen function \f$\mathrm{Cl}_2(\theta) = \mathrm{Im}(\mathrm{Li}_2(e^{i\theta}))\f$ with long double precision
 * @param x real angle
 * @return \f$\mathrm{Cl}_2(\theta)\f$
 */
long double clausen_2(long double x) noexcept
{
   const long double PI = 3.14159265358979323846264338327950288L;
   const std::complex<long double> i(0.0L, 1.0L);

   while (x >= 2*PI) {
      x -= 2*PI;
   }

   while (x < 0.0L) {
      x += 2*PI;
   }

   if (std::abs(x) < std::numeric_limits<long double>::epsilon() ||
       std::abs(x - PI) < std::numeric_limits<long double>::epsilon() ||
       std::abs(x - 2*PI) < std::numeric_limits<long double>::epsilon()) {
      return 0.0L;
   }

   return std::imag(dilog(std::exp(i*x)));
}

/**
 * @brief Real dilogarithm \f$\mathrm{Li}_2(x)\f$
 * @param x real argument
 * @note Implementation translated by R.Brun from CERNLIB DILOG function C332
 * @return \f$\mathrm{Li}_2(x)\f$
 *
 * Implemented as a truncated series expansion in terms of Chebyshev
 * polynomials, see [Yudell L. Luke: Mathematical functions and their
 * approximations, Academic Press Inc., New York 1975, p.67].
 */
double dilog(double x) noexcept {
   const double PI  = 3.141592653589793;
   const double HF  = 0.5;
   const double PI2 = PI*PI;
   const double PI3 = PI2/3;
   const double PI6 = PI2/6;
   const double PI12 = PI2/12;
   const double C[20] = {  0.42996693560813697, 0.40975987533077105,
     -0.01858843665014592, 0.00145751084062268,-0.00014304184442340,
      0.00001588415541880,-0.00000190784959387, 0.00000024195180854,
     -0.00000003193341274, 0.00000000434545063,-0.00000000060578480,
      0.00000000008612098,-0.00000000001244332, 0.00000000000182256,
     -0.00000000000027007, 0.00000000000004042,-0.00000000000000610,
      0.00000000000000093,-0.00000000000000014, 0.00000000000000002};

   double T,H,Y,S,A,ALFA,B1,B2,B0;

   if (x == 1) {
       H = PI6;
   } else if (x == -1) {
       H = -PI12;
   } else {
       T = -x;
       if (T <= -2) {
           Y = -1/(1+T);
           S = 1;
           B1= std::log(-T);
           B2= std::log(1+1/T);
           A = -PI3+HF*(B1*B1-B2*B2);
       } else if (T < -1) {
           Y = -1-T;
           S = -1;
           A = std::log(-T);
           A = -PI6+A*(A+std::log(1+1/T));
       } else if (T <= -0.5) {
           Y = -(1+T)/T;
           S = 1;
           A = std::log(-T);
           A = -PI6+A*(-HF*A+std::log(1+T));
       } else if (T < 0) {
           Y = -T/(1+T);
           S = -1;
           B1= std::log(1+T);
           A = HF*B1*B1;
       } else if (T <= 1) {
           Y = T;
           S = 1;
           A = 0;
       } else {
           Y = 1/T;
           S = -1;
           B1= std::log(T);
           A = PI6+HF*B1*B1;
       }
       H    = Y+Y-1;
       ALFA = H+H;
       B1   = 0;
       B2   = 0;
       for (int i = 19; i >= 0; i--) {
          B0 = C[i] + ALFA*B1-B2;
          B2 = B1;
          B1 = B0;
       }
       H = -(S*(B0-H*B2)+A);
    }
    return H;
}

/**
 * @brief Real dilogarithm \f$\mathrm{Li}_2(z)\f$ with long double precision
 * @param x real argument
 * @note Implementation translated by R.Brun from CERNLIB DILOG function C332
 * @return \f$\mathrm{Li}_2(z)\f$
 *
 * Implemented as a truncated series expansion in terms of Chebyshev
 * polynomials, see [Yudell L. Luke: Mathematical functions and their
 * approximations, Academic Press Inc., New York 1975, p.67].
 */
long double dilog(long double x) noexcept {
   const long double PI  = 3.14159265358979323846264338327950288L;
   const long double HF  = 0.5L;
   const long double PI2 = PI*PI;
   const long double PI3 = PI2/3;
   const long double PI6 = PI2/6;
   const long double PI12 = PI2/12;
   const long double C[24] = { 0.42996693560813697204L, 0.40975987533077105847L,
     -0.01858843665014591965L, 0.00145751084062267855L,-0.00014304184442340049L,
      0.00001588415541879553L,-0.00000190784959386583L, 0.00000024195180854165L,
     -0.00000003193341274252L, 0.00000000434545062677L,-0.00000000060578480118L,
      0.00000000008612097799L,-0.00000000001244331660L, 0.00000000000182255696L,
     -0.00000000000027006766L, 0.00000000000004042209L,-0.00000000000000610325L,
      0.00000000000000092863L,-0.00000000000000014226L, 0.00000000000000002193L,
     -0.00000000000000000340L, 0.00000000000000000053L,-0.00000000000000000008L,
      0.00000000000000000001L};

   long double T,H,Y,S,A,ALFA,B1,B2,B0;

   if (x == 1) {
       H = PI6;
   } else if (x == -1) {
       H = -PI12;
   } else {
       T = -x;
       if (T <= -2) {
           Y = -1/(1+T);
           S = 1;
           B1= std::log(-T);
           B2= std::log(1+1/T);
           A = -PI3+HF*(B1*B1-B2*B2);
       } else if (T < -1) {
           Y = -1-T;
           S = -1;
           A = std::log(-T);
           A = -PI6+A*(A+std::log(1+1/T));
       } else if (T <= -0.5L) {
           Y = -(1+T)/T;
           S = 1;
           A = std::log(-T);
           A = -PI6+A*(-HF*A+std::log(1+T));
       } else if (T < 0) {
           Y = -T/(1+T);
           S = -1;
           B1= std::log(1+T);
           A = HF*B1*B1;
       } else if (T <= 1) {
           Y = T;
           S = 1;
           A = 0;
       } else {
           Y = 1/T;
           S = -1;
           B1= std::log(T);
           A = PI6+HF*B1*B1;
       }
       H    = Y+Y-1;
       ALFA = H+H;
       B1   = 0;
       B2   = 0;
       for (int i = 23; i >= 0; i--) {
          B0 = C[i] + ALFA*B1-B2;
          B2 = B1;
          B1 = B0;
       }
       H = -(S*(B0-H*B2)+A);
    }
    return H;
}

/**
 * @brief Complex dilogarithm \f$\mathrm{Li}_2(z)\f$
 * @param z complex argument
 * @note Implementation translated from SPheno to C++
 * @return \f$\mathrm{Li}_2(z)\f$
 */
std::complex<double> dilog(const std::complex<double>& z) noexcept
{
   const double PI = 3.141592653589793;

   // bf[1..N-1] are the even Bernoulli numbers / (2 n + 1)!
   // generated by: Table[BernoulliB[2 n]/(2 n + 1)!, {n, 1, 9}]
   const double bf[10] = {
      - 1.0/4.0,
      + 1.0/36.0,
      - 1.0/3600.0,
      + 1.0/211680.0,
      - 1.0/10886400.0,
      + 1.0/526901760.0,
      - 4.064761645144226e-11,
      + 8.921691020456453e-13,
      - 1.993929586072108e-14,
      + 4.518980029619918e-16
   };

   const double rz = std::real(z);
   const double iz = std::imag(z);
   const double nz = sqr(rz) + sqr(iz);

   // special cases
   if (iz == 0.0) {
      if (rz <= 1.0) {
         return {dilog(rz), 0.0};
      }
      if (rz > 1.0) {
         return {dilog(rz), -PI*std::log(rz)};
      }
   } else if (nz < std::numeric_limits<double>::epsilon()) {
      return z;
   }

   std::complex<double> cy(0.0, 0.0), cz(0.0, 0.0);
   int jsgn, ipi12;

   // transformation to |z|<1, Re(z)<=0.5
   if (rz <= 0.5) {
      if (nz > 1.0) {
         cy = -0.5 * sqr(std::log(-z));
         cz = -std::log(1.0 - 1.0 / z);
         jsgn = -1;
         ipi12 = -2;
      } else { // nz <= 1
         cy = 0;
         cz = -std::log(1.0 - z);
         jsgn = 1;
         ipi12 = 0;
      }
   } else { // rz > 0.5
      if (nz <= 2*rz) {
         cz = -std::log(z);
         cy = cz * std::log(1.0 - z);
         jsgn = -1;
         ipi12 = 2;
      } else { // nz > 2*rz
         cy = -0.5 * sqr(std::log(-z));
         cz = -std::log(1.0 - 1.0 / z);
         jsgn = -1;
         ipi12 = -2;
      }
   }

   // the dilogarithm
   const std::complex<double> cz2(sqr(cz));
   const std::complex<double> sum =
      cadd(cz,
      cmul(cz2, cadd(bf[0],
      cmul(cz , cadd(bf[1],
      cmul(cz2, cadd(bf[2],
      cmul(cz2, cadd(bf[3],
      cmul(cz2, cadd(bf[4],
      cmul(cz2, cadd(bf[5],
      cmul(cz2, cadd(bf[6],
      cmul(cz2, cadd(bf[7],
      cmul(cz2, cadd(bf[8],
      cmul(cz2, bf[9]))))))))))))))))))));

   return double(jsgn) * sum + cy + ipi12 * PI * PI / 12.0;
}

/**
 * @brief Complex dilogarithm \f$\mathrm{Li}_2(z)\f$ with long double precision
 * @param z complex argument
 * @note Implementation translated from SPheno to C++
 * @return \f$\mathrm{Li}_2(z)\f$
 */
std::complex<long double> dilog(const std::complex<long double>& z) noexcept
{
   const long double PI = 3.14159265358979323846264338327950288L;

   // bf[1..N-1] are the even Bernoulli numbers / (2 n + 1)!
   // generated by: Table[BernoulliB[2 n]/(2 n + 1)!, {n, 1, 22}]
   const long double bf[] = {
      -1.0L/4.0L                                 ,
       1.0L/36.0L                                ,
      -1.0L/3600.0L                              ,
       1.0L/211680.0L                            ,
      -1.0L/10886400.0L                          ,
       1.0L/526901760.0L                         ,
      -4.06476164514422552680590938629196667e-11L,
       8.92169102045645255521798731675274885e-13L,
      -1.99392958607210756872364434779378971e-14L,
       4.51898002961991819165047655285559323e-16L,
      -1.03565176121812470144834115422186567e-17L,
       2.39521862102618674574028374300098038e-19L,
      -5.58178587432500933628307450562541991e-21L,
       1.30915075541832128581230739918659230e-22L,
      -3.08741980242674029324227976486646243e-24L,
       7.31597565270220342035790560925214859e-26L,
      -1.74084565723400074098905514775970255e-27L,
       4.15763564461389971961789962077522667e-29L,
      -9.96214848828462210319400670245583885e-31L,
       2.39403442489616530052116798789374956e-32L,
      -5.76834735536739008429179316187765424e-34L,
       1.39317947964700797782788660391154833e-35L,
      -3.37212196548508947046847363525493096e-37L
   };

   const long double rz = std::real(z);
   const long double iz = std::imag(z);
   const long double nz = sqr(rz) + sqr(iz);

   // special cases
   if (iz == 0.0L) {
      if (rz <= 1.0L) {
         return {dilog(rz), 0.0L};
      }
      if (rz > 1.0L) {
         return {dilog(rz), -PI*std::log(rz)};
      }
   } else if (nz < std::numeric_limits<long double>::epsilon()) {
      return z;
   }

   std::complex<long double> cy(0.0L, 0.0L), cz(0.0L, 0.0L);
   int jsgn, ipi12;

   // transformation to |z|<1, Re(z)<=0.5
   if (rz <= 0.5L) {
      if (nz > 1.0L) {
         cy = -0.5L * sqr(std::log(-z));
         cz = -std::log(1.0L - 1.0L/z);
         jsgn = -1;
         ipi12 = -2;
      } else { // nz <= 1.0L
         cy = 0;
         cz = -std::log(1.0L - z);
         jsgn = 1;
         ipi12 = 0;
      }
   } else { // rz > 0.5L
      if (nz <= 2*rz) {
         cz = -std::log(z);
         cy = cz * std::log(1.0L - z);
         jsgn = -1;
         ipi12 = 2;
      } else { // nz > 2*rz
         cy = -0.5L * sqr(std::log(-z));
         cz = -std::log(1.0L - 1.0L/z);
         jsgn = -1;
         ipi12 = -2;
      }
   }

   // the dilogarithm
   const std::complex<long double> cz2(sqr(cz));
   const std::complex<long double> sum =
      cadd(cz,
      cmul(cz2, cadd(bf[0],
      cmul(cz , cadd(bf[1],
      cmul(cz2, cadd(bf[2],
      cmul(cz2, cadd(bf[3],
      cmul(cz2, cadd(bf[4],
      cmul(cz2, cadd(bf[5],
      cmul(cz2, cadd(bf[6],
      cmul(cz2, cadd(bf[7],
      cmul(cz2, cadd(bf[8],
      cmul(cz2, cadd(bf[9],
      cmul(cz2, cadd(bf[10],
      cmul(cz2, cadd(bf[11],
      cmul(cz2, cadd(bf[12],
      cmul(cz2, cadd(bf[13],
      cmul(cz2, cadd(bf[14],
      cmul(cz2, cadd(bf[15],
      cmul(cz2, cadd(bf[16],
      cmul(cz2, cadd(bf[17],
      cmul(cz2, cadd(bf[18],
      cmul(cz2, cadd(bf[19],
      cmul(cz2, cadd(bf[20],
      cmul(cz2, cadd(bf[21],
      cmul(cz2, bf[22]))))))))))))))))))))))))))))))))))))))))))))));

   return static_cast<long double>(jsgn)*sum + cy + ipi12*PI*PI/12.0L;
}

} // namespace flexiblesusy
