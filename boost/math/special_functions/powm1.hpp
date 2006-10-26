//  (C) Copyright John Maddock 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MATH_POWM1
#define BOOST_MATH_POWM1

#include <boost/math/special_functions/log1p.hpp>
#include <boost/math/special_functions/expm1.hpp>
#include <boost/assert.hpp>

namespace boost{ namespace math{

template <class T>
inline T powm1(const T a, const T z)
{
   using namespace std;

   if((fabs(a) < 1) || (fabs(z) < 1))
   {
      T p = log(a) * z;
      if(fabs(p) < 2)
         return boost::math::expm1(p);
      // otherwise fall though:
   }
   return pow(a, z) - 1;
}

} // namespace math
} // namespace boost

#endif // BOOST_MATH_POWM1



