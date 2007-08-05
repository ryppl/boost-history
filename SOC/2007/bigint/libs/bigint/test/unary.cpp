/* Boost unary.cpp test file
 *
 * Copyright 2007 Arseny Kapoulkine
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#define BOOST_DISABLE_WIN32

#include <boost/test/included/test_exec_monitor.hpp>

#include <boost/bigint/bigint.hpp>

#ifdef BOOST_BIGINT_HAS_GMP_SUPPORT
#	include <boost/bigint/bigint_gmp.hpp>
#endif

#include <boost/bigint/bigint_default.hpp>
#include <boost/bigint/bigint_storage_vector.hpp>
#include <boost/bigint/bigint_storage_fixed.hpp>

#pragma comment(lib, "libgmp-3.lib")

template <typename I> void test()
{
	typedef boost::bigint_base<I> number;

	// ++
	{
		number a(4);

		BOOST_CHECK_EQUAL(++a, 5);
		BOOST_CHECK_EQUAL(a, 5);
		BOOST_CHECK_EQUAL(a++, 5);
		BOOST_CHECK_EQUAL(a, 6);

		number b("999999999999");
		BOOST_CHECK_EQUAL(++b, number("1000000000000"));
		BOOST_CHECK_EQUAL(b, number("1000000000000"));
		BOOST_CHECK_EQUAL(b++, number("1000000000000"));
		BOOST_CHECK_EQUAL(b, number("1000000000001"));

		number c(-2);
		BOOST_CHECK_EQUAL(++c, -1);
		BOOST_CHECK_EQUAL(c, -1);
		BOOST_CHECK_EQUAL(c++, -1);
		BOOST_CHECK_EQUAL(c, 0);
	}

	// --
	{
		number a(5);

		BOOST_CHECK_EQUAL(--a, 4);
		BOOST_CHECK_EQUAL(a, 4);
		BOOST_CHECK_EQUAL(a--, 4);
		BOOST_CHECK_EQUAL(a, 3);

		number b("999999999999");
		BOOST_CHECK_EQUAL(--b, number("999999999998"));
		BOOST_CHECK_EQUAL(b, number("999999999998"));
		BOOST_CHECK_EQUAL(b--, number("999999999998"));
		BOOST_CHECK_EQUAL(b, number("999999999997"));

		number c(1);
		BOOST_CHECK_EQUAL(--c, 0);
		BOOST_CHECK_EQUAL(c, 0);
		BOOST_CHECK_EQUAL(c--, 0);
		BOOST_CHECK_EQUAL(c, -1);
	}

	// +
	{
		BOOST_CHECK_EQUAL(+number("0"), number("0"));
		BOOST_CHECK_EQUAL(+number("3"), number("3"));
		BOOST_CHECK_EQUAL(+number("234905823945839453"), number("234905823945839453"));
		BOOST_CHECK_EQUAL(+number("-1234912384"), number("-1234912384"));
	}
	
	// -
	{
		BOOST_CHECK_EQUAL(-number("0"), number("0"));
		BOOST_CHECK_EQUAL(-number("3"), number("-3"));
		BOOST_CHECK_EQUAL(-number("234905823945839453"), number("-234905823945839453"));
		BOOST_CHECK_EQUAL(-number("-1234912384"), number("1234912384"));
	}

	// ~
	{
		BOOST_CHECK_EQUAL(~number("0"), number("-1"));
		BOOST_CHECK_EQUAL(~number("3"), number("-4"));
		BOOST_CHECK_EQUAL(~number("234905823945839453"), number("-234905823945839454"));
		BOOST_CHECK_EQUAL(~number("-1234912384"), number("1234912383"));
	}
	
	// safe bool conversion and operator!
	{
		BOOST_CHECK(!number("0"));
		BOOST_CHECK(number("3"));
		BOOST_CHECK(number("-234893"));
		BOOST_CHECK(number("-234893") != true); // this should always call operator!= for numbers, not test safe bool - check it
	}
}

int test_main(int argc, char* argv[])
{
#ifdef BOOST_BIGINT_HAS_GMP_SUPPORT
	test<boost::detail::bigint_gmp_implementation>();
#endif

	test<boost::detail::bigint_default_implementation<boost::detail::bigint_storage_vector, 8> >();
	test<boost::detail::bigint_default_implementation<boost::detail::bigint_storage_vector, 16> >();
	test<boost::detail::bigint_default_implementation<boost::detail::bigint_storage_vector, 32> >();

	test<boost::detail::bigint_default_implementation<boost::detail::bigint_storage_fixed<1024>::type, 8> >();
	test<boost::detail::bigint_default_implementation<boost::detail::bigint_storage_fixed<1024>::type, 16> >();
	test<boost::detail::bigint_default_implementation<boost::detail::bigint_storage_fixed<1024>::type, 32> >();

	return 0;
}
