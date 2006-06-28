//  (C) Copyright John Maddock 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MATH_TOOLS_RATIONAL_HPP
#define BOOST_MATH_TOOLS_RATIONAL_HPP

namespace boost{ namespace math{ namespace tools{

template <class T, class U, class V>
V evaluate_rational(const T* num, const U* denom, V z, unsigned count)
{
   V s1, s2;
   if(z <= 100)
   {
      s1 = num[count-1];
      s2 = denom[count-1];
      for(int i = count - 2; i >= 0; --i)
      {
         s1 *= z;
         s2 *= z;
         s1 += num[i];
         s2 += denom[i];
      }
   }
   else
   {
      z = 1 / z;
      s1 = num[0];
      s2 = denom[0];
      for(unsigned i = 1; i < count; ++i)
      {
         s1 *= z;
         s2 *= z;
         s1 += num[i];
         s2 += denom[i];
      }
   }
   return s1 / s2;
}


}}} // namespaces

#endif // BOOST_MATH_TOOLS_RATIONAL_HPP



