//  (C) Copyright John Maddock 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MATH_TOOLS_SOLVE_ROOT_HPP
#define BOOST_MATH_TOOLS_SOLVE_ROOT_HPP

#include <boost/math/tools/precision.hpp>
#include <boost/math/tools/error_handling.hpp>
#include <boost/math/special_functions/sign.hpp>
#include <boost/cstdint.hpp>
#include <limits>

namespace boost{ namespace math{ namespace tools{

template <class T>
class eps_tolerance
{
public:
   eps_tolerance(unsigned bits)
   {
      using namespace std;
      eps = (std::max)(T(ldexp(1.0F, 1-bits)), 2 * tools::epsilon<T>());
   }
   bool operator()(const T& a, const T& b)
   {
      using namespace std;
      return (fabs(a - b) / (std::min)(fabs(a), fabs(b))) <= eps;
   }
private:
   T eps;
};

namespace detail{

template <class F, class T>
void bracket(F f, T& a, T& b, T c, T& fa, T& fb, T& d, T& fd)
{
   //
   // Given a point c inside the existing enclosing interval
   // [a, b] sets a = c if f(c) == 0, otherwise finds the new 
   // enclosing interval: either [a, c] or [c, b] and sets
   // d and fd to the point that has just been removed from
   // the interval.  In other words d is the third best guess
   // to the root.
   //
   using namespace std;  // For ADL of std math functions
   T tol = tools::epsilon<T>() * 2;
   //
   // If the interval [a,b] is very small, or if c is too close 
   // to one end of the interval then we need to adjust the
   // location of c accordingly:
   //
   if((b - a) < 2 * tol * a)
   {
      c = a + (b - a) / 2;
   }
   else if(c <= a + fabs(a) * tol)
   {
      c = a * (1 + tol);
   }
   else if(c >= b - fabs(b) * tol)
   {
      c = b * (1 - tol);
   }
   //
   // OK, lets invoke f(c):
   //
   T fc = f(c);
   //
   // if we have a zero then we have an exact solution to the root:
   //
   if(fc == 0)
   {
      a = c;
      fa = 0;
      d = 0;
      fd = 0;
      return;
   }
   //
   // Non-zero fc, update the interval:
   //
   if(boost::math::sign(fa) * boost::math::sign(fc) < 0)
   {
      d = b;
      fd = fb;
      b = c;
      fb = fc;
   }
   else
   {
      d = a;
      fd = fa;
      a = c;
      fa= fc;
   }
}

template <class T>
T safe_div(T num, T denom, T r)
{
   //
   // return num / denom without overflow,
   // return r if overflow would occur.
   //
   using namespace std;  // For ADL of std math functions

   if(fabs(denom) < 1)
   {
      if(fabs(denom * tools::max_value<T>()) <= fabs(num))
         return r;
   }
   return num / denom;
}

template <class T>
T secant_interpolate(const T& a, const T& b, const T& fa, const T& fb)
{
   //
   // Performs standard secant interpolation of [a,b] given
   // function evaluations f(a) and f(b).  Performs a bisection
   // if secant interpolation would leave us very close to either
   // a or b.  Rationale: we only call this function when at least
   // one other form of interpolation has already failed, so we know
   // that the function is unlikely to be smooth with a root very
   // close to a or b.
   //
   using namespace std;  // For ADL of std math functions

   T tol = tools::epsilon<T>() * 5;
   T c = a - (fa / (fb - fa)) * (b - a);
   if((c <= a + fabs(a) * tol) || (c >= b - fabs(b) * tol))
      return (a + b) / 2;
   return c;
}

template <class T>
T quadratic_interpolate(T a, T b, T const& d, 
                           T fa, T fb, T const& fd, 
                           unsigned count)
{
   //
   // Performs quadratic interpolation to determine the next point,
   // takes count Newton steps to find the location of the
   // quadratic polynomial.
   //
   // Point d must lie outside of the interval [a,b], it is the third
   // best approximation to the root, after a and b.
   //
   // Note: this does not guarentee to find a root
   // inside [a, b], so we fall back to a secant step should
   // the result be out of range.
   //
   // Start by obtaining the coefficients of the quadratic polynomial:
   //
   T B = safe_div(fb - fa, b - a, tools::max_value<T>());
   T A = safe_div(fd - fb, d - b, tools::max_value<T>());
   A = safe_div(A - B, d - a, T(0));

   if(a == 0)
   {
      // failure to determine coefficients, try a secant step:
      return secant_interpolate(a, b, fa, fb);
   }
   //
   // Determine the starting point of the Newton steps:
   //
   T c;
   if(boost::math::sign(A) * boost::math::sign(fa) > 0)
   {
      c = a;
   }
   else
   {
      c = b;
   }
   //
   // Take the Newton steps:
   //
   for(unsigned i = 1; i <= count; ++i)
   {
      //c -= safe_div(B * c, (B + A * (2 * c - a - b)), 1 + c - a);
      c -= safe_div(fa+(B+A*(c-b))*(c-a), (B + A * (2 * c - a - b)), 1 + c - a);
   }
   if((c <= a) || (c >= b))
   {
      // Oops, failure, try a secant step:
      c = secant_interpolate(a, b, fa, fb);
   }
   return c;
}

template <class T>
T cubic_interpolate(const T& a, const T& b, const T& d, 
                    const T& e, const T& fa, const T& fb, 
                    const T& fd, const T& fe)
{
   //
   // Uses inverse cubic interpolation of f(x) at points 
   // [a,b,d,e] to obtain an approximate root of f(x).
   // Points d and e lie outside the interval [a,b]
   // and are the third and forth best approximations
   // to the root that we have found so far.
   //
   // Note: this does not guarentee to find a root
   // inside [a, b], so we fall back to quadratic
   // interpolation in case of an erroneous result.
   //
   T q11 = (d - e) * fd / (fe - fd);
   T q21 = (b - d) * fb / (fd - fb);
   T q31 = (a - b) * fa / (fb - fa);
   T d21 = (b - d) * fd / (fd - fb);
   T d31 = (a - b) * fb / (fb - fa);
   T q22 = (d21 - q11) * fb / (fe - fb);
   T q32 = (d31 - q21) * fa / (fd - fa);
   T d32 = (d31 - q21) * fd / (fd - fa);
   T q33 = (d32 - q22) * fa / (fe - fa);
   T c = q31 + q32 + q33 + a;

   if((c <= a) || (c >= b))
   {
      // Out of bounds step, fall back to quadratic interpolation:
      c = quadratic_interpolate(a, b, d, fa, fb, fd, 3);
   }

   return c;
}

} // namespace detail

template <class F, class T, class Tol>
std::pair<T, T> toms748_solve(F f, const T& ax, const T& bx, const T& fax, const T& fab, Tol tol, boost::uintmax_t& max_iter)
{
   //
   // Main entry point and logic for Toms Algorithm 748
   // root finder.
   //
   using namespace std;  // For ADL of std math functions

   boost::uintmax_t count;
   T a, b, fa, fb, c, u, fu, a0, b0, d, fd, e, fe;
   static const T mu = 0.5f;

   // initialise a, b and fa, fb:
   a = ax;
   b = bx;
   count = 0;
   if(a >= b)
      tools::domain_error(
         BOOST_CURRENT_FUNCTION, 
         "Parameters a and b out of order: a=%1%", a);
   fa = fax;
   fb = fab;
   if(boost::math::sign(fa) * boost::math::sign(fb) > 0)
      tools::domain_error(
         BOOST_CURRENT_FUNCTION, 
         "Parameters a and b do not bracket the root: a=%1%", a);
   // dummy value for fd, e and fe:
   fe = e = fd = 1e5F;

   if(fa != 0)
   {
      //
      // On the first step we take a secant step:
      //
      c = detail::secant_interpolate(a, b, fa, fb);
      detail::bracket(f, a, b, c, fa, fb, d, fd);

      if(fa != 0)
      {
         //
         // On the second step we take a quadratic interpolation:
         //
         c = detail::quadratic_interpolate(a, b, d, fa, fb, fd, 2);
         e = d;
         fe = fd;
         detail::bracket(f, a, b, c, fa, fb, d, fd);
      }
   }

   while((fa != 0) && !tol(a, b) && (++count != max_iter))
   {
      // save our brackets:
      a0 = a;
      b0 = b;
      //
      // Starting with the third step taken
      // we can use either quadratic or cubic interpolation.
      // Cubic interpolation requires that all four function values
      // fa, fb, fd, and fe are distinct, should that not be the case
      // then variable prof will get set to true, and we'll end up
      // taking a quadratic step instead.
      //
      bool prof = (fa == fb) || (fa == fd) || (fa == fe) || (fb == fd) || (fb == fe) || (fd == fe);
      if(prof)
      {
         c = detail::quadratic_interpolate(a, b, d, fa, fb, fd, 2);
      }
      else
      {
         c = detail::cubic_interpolate(a, b, d, e, fa, fb, fd, fe);
      }
      //
      // re-bracket, and check for termination:
      //
      e = d;
      fe = fd;
      detail::bracket(f, a, b, c, fa, fb, d, fd);
      if((fa == 0) || tol(a, b))
         break;
      //
      // Now another interpolated step:
      //
      prof = (fa == fb) || (fa == fd) || (fa == fe) || (fb == fd) || (fb == fe) || (fd == fe);
      if(prof)
      {
         c = detail::quadratic_interpolate(a, b, d, fa, fb, fd, 3);
      }
      else
      {
         c = detail::cubic_interpolate(a, b, d, e, fa, fb, fd, fe);
      }
      //
      // Bracket again, and check termination condition, update e:
      //
      detail::bracket(f, a, b, c, fa, fb, d, fd);
      if((fa == 0) || tol(a, b))
         break;
      //
      // Now we take a double-length secant step:
      //
      if(fabs(fa) < fabs(fb))
      {
         u = a;
         fu = fa;
      }
      else
      {
         u = b;
         fu = fb;
      }
      c = u - 2 * (fu / (fb - fa)) * (b - a);
      if(fabs(c - u) > (b - a) / 2)
      {
         c = a + (b - a) / 2;
      }
      //
      // Bracket again, and check termination condition:
      //
      e = d;
      fe = fd;
      detail::bracket(f, a, b, c, fa, fb, d, fd);
      if((fa == 0) || tol(a, b))
         break;
      //
      // And finally... check to see if an additional bisection step is 
      // to be taken, we do this if we're not converging fast enough:
      //
      if((b - a) < mu * (b0 - a0))
         continue;
      //
      // bracket again on a bisection:
      //
      e = d;
      fe = fd;
      detail::bracket(f, a, b, a + (b - a) / 2, fa, fb, d, fd);
   } // while loop

   max_iter = count;
   if(fa == 0)
   {
      b = a;
   }
   else if(fb == 0)
   {
      a = b;
   }
   return std::make_pair(a, b);
}

template <class F, class T, class Tol>
inline std::pair<T, T> toms748_solve(F f, const T& ax, const T& bx, Tol tol, boost::uintmax_t& max_iter)
{
   return toms748_solve(f, ax, bx, f(ax), f(bx), tol, max_iter);
}

template <class F, class T, class Tol>
std::pair<T, T> bracket_and_solve_root(F f, const T& guess, const T& factor, bool rising, Tol tol, boost::uintmax_t& max_iter)
{
   T a = guess;
   T b = a;
   T fa = f(a);
   T fb = fa;

   if((fa < 0) == rising)
   {
      //
      // Zero is to the right of b, so walk upwards
      // until we find it:
      //
      while(sign(fb) == sign(fa))
      {
         a = b;
         fa = fb;
         b *= factor;
         fb = f(b);
      }
   }
   else
   {
      //
      // Zero is to the left of a, so walk downwards
      // until we find it:
      //
      while(sign(fb) == sign(fa))
      {
         b = a;
         fb = fa;
         a /= factor;
         fa = f(a);
      }
   }
   return toms748_solve(f, a, b, fa, fb, tol, max_iter);
}

} // namespace tools
} // namespace math
} // namespace boost


#endif // BOOST_MATH_TOOLS_SOLVE_ROOT_HPP
