//-----------------------------------------------------------------------------
// boost mpl/test/rational_c.cpp source file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2003
// Hugo Duncan
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee,
// provided that the above copyright notice appears in all copies and
// that both the copyright notice and this permission notice appear in
// supporting documentation. No representations are made about the
// suitability of this software for any purpose. It is provided "as is"
// without express or implied warranty.

#include "boost/mpl/rational_c.hpp"
#include "boost/static_assert.hpp"

namespace mpl = boost::mpl;

#define RATIONAL_C_TEST(T)                                                \
    {                                                                     \
  typedef boost::mpl::rational_c<T,1,2> half;                             \
  { BOOST_STATIC_ASSERT(half::numerator::value == 1); }                   \
  { BOOST_STATIC_ASSERT(half::denominator::value == 2); }                 \
  typedef boost::mpl::rational_c<T,2,4> half_2;                           \
  typedef boost::mpl::rational_simplify<half_2> half_3;                   \
  { BOOST_STATIC_ASSERT(half_3::numerator::value == 1); }                 \
  { BOOST_STATIC_ASSERT(half_3::denominator::value == 2); }               \
  typedef boost::mpl::rational_c<T,9,15> three_fiths_3;                   \
  typedef boost::mpl::rational_simplify<three_fiths_3>::type three_fiths; \
  { BOOST_STATIC_ASSERT(three_fiths::numerator::value == 3); }            \
  { BOOST_STATIC_ASSERT(three_fiths::denominator::value == 5); }          \
  typedef boost::mpl::rational_plus<three_fiths,half>::type eleven_tenth; \
  { BOOST_STATIC_ASSERT(eleven_tenth::numerator::value == 11); }          \
  { BOOST_STATIC_ASSERT(eleven_tenth::denominator::value == 10); }        \
  typedef boost::mpl::rational_plus<half,half>::type one;                 \
  { BOOST_STATIC_ASSERT(one::numerator::value == 1); }                    \
  { BOOST_STATIC_ASSERT(one::denominator::value == 1); }                  \
  typedef boost::mpl::rational_c<T,1,8> eighth;                           \
  typedef boost::mpl::rational_minus<half,eighth>::type three_eighths;    \
  { BOOST_STATIC_ASSERT(three_eighths::numerator::value == 3); }          \
  { BOOST_STATIC_ASSERT(three_eighths::denominator::value == 8); }        \
  typedef boost::mpl::rational_multiplies<half,eighth>::type sixteenth;   \
  { BOOST_STATIC_ASSERT(sixteenth::numerator::value == 1); }              \
  { BOOST_STATIC_ASSERT(sixteenth::denominator::value == 16); }           \
  typedef boost::mpl::rational_divides<eighth,half>::type quarter;        \
  { BOOST_STATIC_ASSERT(quarter::numerator::value == 1); }                \
  { BOOST_STATIC_ASSERT(quarter::denominator::value == 4); }              \
    }
/**/

int main()
{
    RATIONAL_C_TEST(char)
    RATIONAL_C_TEST(short)
    RATIONAL_C_TEST(int)
    return 0;
}
