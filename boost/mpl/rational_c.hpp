#ifndef BOOST_MPL_RATIONAL_C_HPP_INCLUDED
#define BOOST_MPL_RATIONAL_C_HPP_INCLUDED
// include guard at start of file, so borland only loads once

// + file: boost/mpl/rational_c.hpp
// + last modified: 30/oct/03

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
//
// See http://www.boost.org/libs/mpl for documentation.

#include "boost/mpl/plus.hpp"
#include "boost/mpl/minus.hpp"
#include "boost/mpl/divides.hpp"
#include "boost/mpl/multiplies.hpp"
#include "boost/mpl/negate.hpp"
#include "boost/math/common_factor_ct.hpp"

#include "boost/config.hpp"

namespace boost { namespace mpl {

template<
      typename IntegerType
    , IntegerType N
    , IntegerType D = 1
    >
struct rational_c
{
    typedef IntegerType integer_type;
    typedef integral_c<IntegerType,N> numerator;
    typedef integral_c<IntegerType,D> denominator;

    typedef rational_c<IntegerType,N,D> type;
    rational_c() {}

  static double value()
  {
    return (IntegerType)numerator::value
      / (double)(IntegerType)denominator::value;
  }

};

//! simplify a rational by dividing numerator and denominator by gcd
template <typename T>
struct rational_simplify
{
  typedef typename T::integer_type integer_type;
  typedef typename T::numerator old_numerator;
  typedef typename T::denominator old_denominator;

  typedef boost::mpl::integral_c<
    integer_type,
    boost::math::static_gcd<
      old_numerator::value,
      old_denominator::value>::value
  > this_gcd;

  typedef boost::mpl::divides< old_numerator, this_gcd> numerator;
  typedef boost::mpl::divides< old_denominator, this_gcd> denominator;

  typedef boost::mpl::rational_c<
    typename T::integer_type
    , numerator::value
    , denominator::value
  > type;

};

//! negate a rational
template <typename T>
struct rational_negate
{
  typedef typename T::integer_type integer_type;
  typedef typename T::numerator old_numerator;
  typedef typename T::denominator old_denominator;

  typedef boost::mpl::negate< old_numerator> numerator;

  typedef boost::mpl::rational_c<
    typename T::integer_type
    , numerator::value
    , old_denominator::value
  > type;

};

template <typename T1, typename T2>
struct rational_plus_c
{
  // use T1 integer type
  typedef typename T1::integer_type integer_type;

  typedef typename T1::numerator t1_numerator;
  typedef typename T1::denominator t1_denominator;

  typedef typename T2::numerator t2_numerator;
  typedef typename T2::denominator t2_denominator;

  typedef boost::mpl::integral_c<
    integer_type,
    boost::math::static_lcm<
      t1_denominator::value,
      t2_denominator::value>::value
  > this_lcm;

  typedef typename boost::mpl::divides<this_lcm, t1_denominator>::type t1_factor;
  typedef typename boost::mpl::divides<this_lcm, t2_denominator>::type t2_factor;

  typedef typename boost::mpl::multiplies<t1_factor, t1_numerator>::type numerator_1;
  typedef typename boost::mpl::multiplies<t2_factor, t2_numerator>::type numerator_2;

  typedef typename boost::mpl::plus<numerator_1,numerator_2>::type numerator;

  typedef boost::mpl::rational_c<
    integer_type
    , numerator::value
    , this_lcm::value
  > type;
};

template <typename T1, typename T2>
struct rational_plus
  : rational_simplify< typename rational_plus_c<T1,T2>::type >
{};




template <typename T1, typename T2>
struct rational_minus_c
{
  // use T1 integer type
  typedef typename T1::integer_type integer_type;

  typedef typename T1::numerator t1_numerator;
  typedef typename T1::denominator t1_denominator;

  typedef typename T2::numerator t2_numerator;
  typedef typename T2::denominator t2_denominator;

  typedef boost::mpl::integral_c<
    integer_type,
    boost::math::static_lcm<
      t1_denominator::value,
      t2_denominator::value>::value
  > this_lcm;

  typedef typename boost::mpl::divides<this_lcm, t1_denominator>::type t1_factor;
  typedef typename boost::mpl::divides<this_lcm, t2_denominator>::type t2_factor;

  typedef typename boost::mpl::multiplies<t1_factor, t1_numerator>::type numerator_1;
  typedef typename boost::mpl::multiplies<t2_factor, t2_numerator>::type numerator_2;

  typedef typename boost::mpl::minus<numerator_1,numerator_2>::type numerator;

  typedef boost::mpl::rational_c<
    integer_type
    , numerator::value
    , this_lcm::value
  > type;
};

template <typename T1, typename T2>
struct rational_minus
  : rational_simplify< typename rational_minus_c<T1,T2>::type >
{};



template <typename T1, typename T2>
struct rational_multiplies_c
{
  // use T1 integer type
  typedef typename T1::integer_type integer_type;

  typedef typename T1::numerator t1_numerator;
  typedef typename T1::denominator t1_denominator;

  typedef typename T2::numerator t2_numerator;
  typedef typename T2::denominator t2_denominator;

  typedef typename boost::mpl::multiplies<t1_numerator, t2_numerator>::type
    numerator;
  typedef typename boost::mpl::multiplies<t1_denominator, t2_denominator>::type
    denominator;

  typedef boost::mpl::rational_c<
    integer_type
    , numerator::value
    , denominator::value
  > type;
};

template <typename T1, typename T2>
struct rational_multiplies
  : rational_simplify< typename rational_multiplies_c<T1,T2>::type >
{};

template <typename T1, typename T2>
struct rational_divides_c
{
  // use T1 integer type
  typedef typename T1::integer_type integer_type;

  typedef typename T1::numerator t1_numerator;
  typedef typename T1::denominator t1_denominator;

  typedef typename T2::numerator t2_numerator;
  typedef typename T2::denominator t2_denominator;

  typedef typename boost::mpl::divides<t1_numerator, t2_numerator>::type
    numerator;
  typedef typename boost::mpl::divides<t1_denominator, t2_denominator>::type
    denominator;

  typedef boost::mpl::rational_c<
    integer_type
    , numerator::value
    , denominator::value
  > type;
};

template <typename T1, typename T2>
struct rational_divides
  : rational_simplify< typename rational_divides_c<T1,T2>::type >
{};

} } // namespace

#endif  // BOOST_MPL_RATIONAL_C_HPP_INCLUDED
