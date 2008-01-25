//  (C) Copyright John Maddock 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_MATH_TOOLS_MINIMA_HPP
#define BOOST_MATH_TOOLS_MINIMA_HPP

#ifdef _MSC_VER
#pragma once
#endif

#include <utility>
#include <cmath>
#include <boost/math/tools/precision.hpp>
#include <boost/math/policies/policy.hpp>
#include <boost/cstdint.hpp>

namespace boost{ namespace math{ namespace tools{

template <class F, class T>
std::pair<T, T> brent_find_minima(F f, T vmin, T vmax, int bits, boost::uintmax_t& max_iter)
{
   BOOST_MATH_STD_USING
   using std::min;

   bits = min BOOST_PREVENT_MACRO_SUBSTITUTION(policies::digits<T, policies::policy<> >() / 2, bits);
   T tolerance = static_cast<T>(ldexp(1.0, 1-bits));
   T x;  // minima so far
   T w;  // second best point
   T v;  // previous value of w
   T u;  // most recent evaluation point
   T delta;  // The distance moved in the last step
   T delta2; // The distance moved in the step before last
   T fu, fv, fw, fx;  // function evaluations at u, v, w, x
   T mid; // midpoint of vmin and vmax
   T fract1, fract2;  // minimal relative movement in x

   static const T golden = 0.3819660f;  // golden ratio, don't need too much precision here!

   x = w = v = vmax;
   fw = fv = fx = f(x);
   delta2 = delta = 0;

   uintmax_t count = max_iter;

   do{
      // get midpoint
      mid = (vmin + vmax) / 2;
      // work out if we're done already:
      fract1 = tolerance * fabs(x) + tolerance / 4;
      fract2 = 2 * fract1;
      if(fabs(x - mid) <= (fract2 - (vmax - vmin) / 2))
         break;

      if(fabs(delta2) > fract1)
      {
         // try and construct a parabolic fit:
         T r = (x - w) * (fx - fv);
         T q = (x - v) * (fx - fw);
         T p = (x - v) * q - (x - w) * r;
         q = 2 * (q - r);
         if(q > 0)
            p = -p;
         q = fabs(q);
         T td = delta2;
         delta2 = delta;
         // determine whether a parabolic step is acceptible or not:
         if((fabs(p) >= fabs(q * td / 2)) || (p <= q * (vmin - x)) || (p >= q * (vmax - x)))
         {
            // nope, try golden section instead
            delta2 = (x >= mid) ? vmin - x : vmax - x;
            delta = golden * delta2;
         }
         else
         {
            // whew, parabolic fit:
            delta = p / q;
            u = x + delta;
            if(((u - vmin) < fract2) || ((vmax- u) < fract2))
               delta = (mid - x) < 0 ? -fabs(fract1) : fabs(fract1);
         }
      }
      else
      {
         // golden section:
         delta2 = (x >= mid) ? vmin - x : vmax - x;
         delta = golden * delta2;
      }
      // update current position:
      u = (fabs(delta) >= fract1) ? x + delta : (delta > 0 ? x + fabs(fract1) : x - fabs(fract1));
      fu = f(u);
      if(fu <= fx)
      {
         // good new point is an improvement!
         // update brackets:
         if(u >= x)
            vmin = x;
         else
            vmax = x;
         // update control points:
         v = w;
         w = x;
         x = u;
         fv = fw;
         fw = fx;
         fx = fu;
      }
      else
      {
         // Oh dear, point u is worse than what we have already,
         // even so it *must* be better than one of our endpoints:
         if(u < x)
            vmin = u;
         else
            vmax = u;
         if((fu <= fw) || (w == x))
         {
            // however it is at least second best:
            v = w;
            w = u;
            fv = fw;
            fw = fu;
         }
         else if((fu <= fv) || (v == x) || (v == w))
         {
            // third best:
            v = u;
            fv = fu;
         }
      }

   }while(--count);

   max_iter -= count;

   return std::make_pair(x, fx);
}

template <class F, class T>
inline std::pair<T, T> brent_find_minima(F f, T vmin, T vmax, int digits)
{
   boost::uintmax_t m = (std::numeric_limits<boost::uintmax_t>::max)();
   return brent_find_minima(f, vmin, vmax, digits, m);
}

}}} // namespaces

#endif




