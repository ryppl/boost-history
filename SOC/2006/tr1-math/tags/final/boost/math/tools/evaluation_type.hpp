//  (C) Copyright John Maddock 2006.
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

#ifndef BOOST_MATH_TOOLS_EVALUATION_TYPE_HPP
#define BOOST_MATH_TOOLS_EVALUATION_TYPE_HPP

namespace boost{ namespace math{ namespace tools{

template <class T>
struct evaluation
{
   typedef T type;
};

#ifndef BOOST_NO_LIMITS_COMPILE_TIME_CONSTANTS
#define BOOST_MATH_FLT_DIGITS std::numeric_limits<float>::digits
#define BOOST_MATH_DBL_DIGITS std::numeric_limits<double>::digits
#define BOOST_MATH_LDBL_DIGITS std::numeric_limits<long double>::digits
#else
#define BOOST_MATH_FLT_DIGITS FLT_MANT_DIG
#define BOOST_MATH_DBL_DIGITS DBL_MANT_DIG
#define BOOST_MATH_LDBL_DIGITS LDBL_MANT_DIG
#endif

template<>
struct evaluation<float>
{
   typedef boost::mpl::if_c<
      BOOST_MATH_DBL_DIGITS >= 40,
      double, float>::type type;
};

//
// We don't promote to long double precision if the platform
// doesn't have long double versions of the C std lib functions.
//
// If we did promote in this case we could pass out-of-range values
// to the C std lib without realising that we were doing so, 
// as numeric_limits<long double> would give us no clue that we were
// about to go out of range for a double.
//
#if !defined(__CYGWIN__)
template<>
struct evaluation<double>
{
   typedef boost::mpl::if_c<
      BOOST_MATH_LDBL_DIGITS >= 64,
      long double,
      double>::type type;
};
#endif

}}} // namespaces

#endif // BOOST_MATH_TOOLS_EVALUATION_TYPE_HPP



