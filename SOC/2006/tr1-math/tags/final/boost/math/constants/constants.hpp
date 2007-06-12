//  (C) Copyright John Maddock 2005-2006.
//
//  This code may be used under either of the following two licences:
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
//  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE. OF SUCH DAMAGE.
//
//  Or:
//
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MATH_CONSTANTS_CONSTANTS_INCLUDED
#define BOOST_MATH_CONSTANTS_CONSTANTS_INCLUDED

#include <boost/config.hpp>
#include <boost/lexical_cast.hpp>

namespace boost{ namespace math{ namespace constants{

#define BOOST_DEFINE_MATH_CONSTANT(name, x, y, exp)\
   template <class T> inline T name(BOOST_EXPLICIT_TEMPLATE_TYPE(T))\
   {\
      static const T result = ::boost::lexical_cast<T>(BOOST_STRINGIZE(BOOST_JOIN(BOOST_JOIN(x, y), BOOST_JOIN(e, exp))));\
      return result;\
   }\
   template <> inline float name<float>(BOOST_EXPLICIT_TEMPLATE_TYPE_SPEC(float))\
   { return BOOST_JOIN(BOOST_JOIN(x, BOOST_JOIN(e, exp)), F); }\
   template <> inline double name<double>(BOOST_EXPLICIT_TEMPLATE_TYPE_SPEC(double))\
   { return BOOST_JOIN(x, BOOST_JOIN(e, exp)); }\
   template <> inline long double name<long double>(BOOST_EXPLICIT_TEMPLATE_TYPE_SPEC(long double))\
   { return BOOST_JOIN(BOOST_JOIN(x, BOOST_JOIN(e, exp)), L); }

BOOST_DEFINE_MATH_CONSTANT(pi, 3.141592653589793238462643383279502884197169399375105820974944, 59230781640628620899862803482534211706798214808651328230664709384460955058223172535940812848111745028410270193852110555964462294895493038196, 0)
BOOST_DEFINE_MATH_CONSTANT(e, 2.7182818284590452353602874713526624977572470936999595749669676, 27724076630353547594571382178525166427427466391932003059921817413596629043572900334295260595630738132328627943490763233829880753195251019011, 0)
BOOST_DEFINE_MATH_CONSTANT(half, 0.5, 0, 0)
BOOST_DEFINE_MATH_CONSTANT(euler, 0.577215664901532860606512090082402431042159335939923598805, 76723488486, 0)

} } } // namespaces

#endif // BOOST_MATH_CONSTANTS_CONSTANTS_INCLUDED
