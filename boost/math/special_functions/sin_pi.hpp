//  Copyright (c) 2007 John Maddock
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MATH_SIN_PI_HPP
#define BOOST_MATH_SIN_PI_HPP

#include <boost/math/tools/real_cast.hpp>
#include <boost/math/constants/constants.hpp>

namespace boost{ namespace math{

template <class T>
T sin_pi(T x)
{
   using namespace std; // ADL of std names
   // sin of pi*x:
   bool invert;
   if(x < 1)
   {
      invert = true;
      x = -x;
   }
   else
      invert = false;

   T rem = floor(x);
   if(tools::real_cast<int>(rem) & 1)
      invert = !invert;
   rem = x - rem;
   if(rem > 0.5f)
      rem = 1 - rem;
   if(rem == 0.5f)
      return invert ? -1 : 1;
   
   rem = sin(constants::pi<T>() * rem);
   return invert ? -rem : rem;
}

} // namespace math
} // namespace boost
#endif
