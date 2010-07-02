/* Boost test/integer.cpp
 * test int extension
 *
 * Copyright 2003 Guillaume Melquiond
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/numeric/interval.hpp>
#include <boost/numeric/interval/ext/integer.hpp>
#include <boost/numeric/interval/policies.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/limits.hpp>
#include <iostream>
#include "bugs.hpp"

typedef boost::numeric::interval<float> I;

template <class T, class U>
T force_to_nearest(U u)
{
   typedef boost::numeric::interval_lib::rounded_arith_std<T> controller;
   typename controller::rounding_mode m;
   controller::get_rounding_mode(m);
   controller::to_nearest();
   volatile T result = static_cast<T>(u);
   controller::set_rounding_mode(m);
   return result;
}

template <class I, class Int>
void test_construct_from(I, Int, const char* name, const char* iname)
{
   std::cout << "Checking contruction of " << name << " from " << iname << std::endl;
   typedef typename I::base_type value_type;
   Int i = (std::numeric_limits<Int>::max)() / 2;
   if((i & 1) == 0)
      --i;
   I inter(i);
   BOOST_CHECK(std::floor(inter.lower()) == std::ceil(inter.lower()));
   BOOST_CHECK(std::floor(inter.upper()) == std::ceil(inter.upper()));
   BOOST_CHECK(inter.lower() <= inter.upper());
   BOOST_CHECK_PREDICATE(std::less_equal<value_type>(), (inter.lower())(force_to_nearest<value_type>(i)));
   BOOST_CHECK_PREDICATE(std::greater_equal<value_type>(), (inter.upper())(force_to_nearest<value_type>(i)));
   Int a = static_cast<Int>(std::floor(inter.lower()));
   BOOST_CHECK_PREDICATE(std::less_equal<Int>(), (a)(i));
   a = static_cast<Int>(std::ceil(inter.upper()));
   BOOST_CHECK_PREDICATE(std::greater_equal<Int>(), (a)(i));
   BOOST_CHECK_PREDICATE(std::less_equal<value_type>(), (width(inter) / median(inter)) (std::numeric_limits<value_type>::epsilon()));
   i = (std::numeric_limits<Int>::min)() / 2;
   if((i & 1) == 0)
      ++i;
   inter = I(i);
   BOOST_CHECK(std::floor(inter.lower()) == std::ceil(inter.lower()));
   BOOST_CHECK(std::floor(inter.upper()) == std::ceil(inter.upper()));
   BOOST_CHECK(inter.lower() <= inter.upper());
   BOOST_CHECK_PREDICATE(std::less_equal<value_type>(), (inter.lower())(force_to_nearest<value_type>(i)));
   BOOST_CHECK_PREDICATE(std::greater_equal<value_type>(), (inter.upper())(force_to_nearest<value_type>(i)));
   a = static_cast<Int>(std::floor(inter.lower()));
   BOOST_CHECK_PREDICATE(std::less_equal<Int>(), (a)(i));
   a = static_cast<Int>(std::ceil(inter.upper()));
   BOOST_CHECK_PREDICATE(std::greater_equal<Int>(), (a)(i));
   if(median(inter) != 0)
      BOOST_CHECK_PREDICATE(std::less_equal<value_type>(), (width(inter) / median(inter)) (std::numeric_limits<value_type>::epsilon()));

   i = (std::numeric_limits<Int>::max)();
   if((i & 1) == 0)
      --i;
   inter = I(i);
   BOOST_CHECK(std::floor(inter.lower()) == std::ceil(inter.lower()));
   BOOST_CHECK(std::floor(inter.upper()) == std::ceil(inter.upper()));
   BOOST_CHECK(inter.lower() <= inter.upper());
   BOOST_CHECK_PREDICATE(std::less_equal<value_type>(), (inter.lower())(force_to_nearest<value_type>(i)));
   BOOST_CHECK_PREDICATE(std::greater_equal<value_type>(), (inter.upper())(force_to_nearest<value_type>(i)));
   i = (std::numeric_limits<Int>::min)();
   if((i & 1) == 0)
      ++i;
   inter = I(i);
   BOOST_CHECK(std::floor(inter.lower()) == std::ceil(inter.lower()));
   BOOST_CHECK(std::floor(inter.upper()) == std::ceil(inter.upper()));
   BOOST_CHECK(inter.lower() <= inter.upper());
   BOOST_CHECK_PREDICATE(std::less_equal<value_type>(), (inter.lower())(force_to_nearest<value_type>(i)));
   BOOST_CHECK_PREDICATE(std::greater_equal<value_type>(), (inter.upper())(force_to_nearest<value_type>(i)));
}

template <class I>
void test_construct(I inter, const char* name)
{
   test_construct_from(inter, 0, name, "int");
   test_construct_from(inter, 0u, name, "unsigned int");
   test_construct_from(inter, 0L, name, "long");
   test_construct_from(inter, 0uL, name, "unsigned long");
#ifdef BOOST_HAS_LONG_LONG
   test_construct_from(inter, 0LL, name, "long long");
   test_construct_from(inter, 0uLL, name, "unsigned long long");
#endif
}

int test_main(int, char *[]) {
  I x, y;
  x = 4 - (2 * y + 1) / 3;
# ifdef __BORLANDC__
  ::detail::ignore_warnings();
# endif

  using namespace boost::numeric;
  using namespace boost::numeric::interval_lib;

  test_construct(interval<float, policies<rounded_arith_std<float>, checking_no_empty<float> > >(), "interval<float, policies<rounded_arith_std<float>, checking_no_empty<float> > >");
  test_construct(interval<float, policies<rounded_arith_opp<float>, checking_no_empty<float> > >(), "interval<float, policies<rounded_arith_opp<float>, checking_no_empty<float> > >");
  test_construct(interval<double, policies<rounded_arith_std<double>, checking_no_empty<double> > >(), "interval<double, policies<rounded_arith_std<double>, checking_no_empty<double> > >");
  test_construct(interval<double, policies<rounded_arith_opp<double>, checking_no_empty<double> > >(), "interval<double, policies<rounded_arith_opp<double>, checking_no_empty<double> > >");
  test_construct(interval<long double, policies<rounded_arith_std<long double>, checking_no_empty<long double> > >(), "interval<long double, policies<rounded_arith_std<long double>, checking_no_empty<long double> > >");
  test_construct(interval<long double, policies<rounded_arith_opp<long double>, checking_no_empty<long double> > >(), "interval<long double, policies<rounded_arith_opp<long double>, checking_no_empty<long double> > >");
  return 0;
}