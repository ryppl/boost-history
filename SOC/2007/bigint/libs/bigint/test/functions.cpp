/* Boost functions.cpp test file
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

#define ARRAY_SIZE(array) sizeof(array) / sizeof(array[0])

template <typename I> void test()
{
	typedef boost::bigint_base<I> number;

	// abs
	{
		BOOST_CHECK_EQUAL(abs(number(0)), 0);
		BOOST_CHECK_EQUAL(abs(number("23923")), number("23923"));
		BOOST_CHECK_EQUAL(abs(number("2234984058203945820345823904580234583923")), number("2234984058203945820345823904580234583923"));
		BOOST_CHECK_EQUAL(abs(number("-23923")), number("23923"));
		BOOST_CHECK_EQUAL(abs(number("-2234984058203945820345823904580234583923")), number("2234984058203945820345823904580234583923"));
	}

	// pow
	{
		// zero base
		BOOST_CHECK_EQUAL(pow(number(0), 239482394), 0);
		BOOST_CHECK_EQUAL(pow(number(0), boost::uint64_t(239482394) * boost::uint64_t(239482394)), 0);
		// zero power
		BOOST_CHECK_EQUAL(pow(number(0), 0), 1);
		BOOST_CHECK_EQUAL(pow(number("24898934"), 0), 1);
		BOOST_CHECK_EQUAL(pow(number("24898932398493444"), 0), 1);
		BOOST_CHECK_EQUAL(pow(number("-240590245924898934"), 0), 1);
		// base = one
		BOOST_CHECK_EQUAL(pow(number(1), 239482394), 1);
		BOOST_CHECK_EQUAL(pow(number(1), boost::uint64_t(239482394) * boost::uint64_t(239482394)), 1);
		// other cases
		BOOST_CHECK_EQUAL(pow(number(2), 20), 1048576);
		BOOST_CHECK_EQUAL(pow(number(2), 65), number("36893488147419103232"));
		BOOST_CHECK_EQUAL(pow(number(234895897), 3), number("12960635374738189889899273"));
		// negative base
		BOOST_CHECK_EQUAL(pow(number(-2), 20), 1048576);
		BOOST_CHECK_EQUAL(pow(number(-2), 21), -2097152);
		BOOST_CHECK_EQUAL(pow(number(-234895897), 2), number("55176082427434609"));
		BOOST_CHECK_EQUAL(pow(number(-234895897), 3), number("-12960635374738189889899273"));
	}

	// ldiv
	{
		struct test_entry
		{
			const char* lhs;
			const char* rhs;
			const char* result;
			const char* rem;
		};

		test_entry entries[] =
		{
			// zero
			{"0", "23904293402394023940", "0", "0"},
			// positive / larger positive
			{"239409034", "23489023049230492304230493049034", "0", "239409034"},
			{"-239409034", "23489023049230492304230493049034", "0", "-239409034"},
			{"239409034", "-23489023049230492304230493049034", "0", "239409034"},
			// ~ equal magnitudes
			{"39049204923049203490234", "39049204923049203490230", "1", "4"},
			{"-39049204923049203490234", "39049204923049203490230", "-1", "-4"},
			{"39049204923049203490234", "-39049204923049203490230", "-1", "4"},
			// some other numbers
			{"13940239402394034213904", "49850", "279643719205497175", "40154"},
			{"-9049594859482398349", "-3401", "2660862940159482", "-67"},
			// proper rounding for positive numbers
			{"3", "2", "1", "1"},
			{"9304", "3", "3101", "1"},
			{"394093", "11", "35826", "7"},
			// proper rounding for positive results (negative numbers)
			{"-3", "-2", "1", "-1"},
			{"-9304", "-3", "3101", "-1"},
			{"-394093", "-11", "35826", "-7"},
			// proper rounding for positive / negative (towards zero)
			{"-3", "2", "-1", "-1"},
			{"-9304", "3", "-3101", "-1"},
			{"-394093", "11", "-35826", "-7"},
			// proper rounding for negative / positive (towards zero)
			{"3", "-2", "-1", "1"},
			{"9304", "-3", "-3101", "1"},
			{"394093", "-11", "-35826", "7"},
		};


		for (size_t i = 0; i < ARRAY_SIZE(entries); ++i)
		{
			const test_entry& e = entries[i];

			number rem;

			BOOST_CHECK_EQUAL(div(number(e.lhs), number(e.rhs), rem), number(e.result));
			BOOST_CHECK_EQUAL(rem, number(e.rem));
		}
	}

	// sqrt
	{
		// zero
		BOOST_CHECK_EQUAL(sqrt(number(0)), 0);
		// perfect squares
		BOOST_CHECK_EQUAL(sqrt(number(1)), 1);
		BOOST_CHECK_EQUAL(sqrt(number(1048576)), 1024);
		BOOST_CHECK_EQUAL(sqrt(number("80626033399934169")), 283947237);
		// slightly more
		BOOST_CHECK_EQUAL(sqrt(number(2)), 1);
		BOOST_CHECK_EQUAL(sqrt(number(1048577)), 1024);
		BOOST_CHECK_EQUAL(sqrt(number("80626033399934170")), 283947237);
		// slightly less
		BOOST_CHECK_EQUAL(sqrt(number(1048575)), 1023);
		BOOST_CHECK_EQUAL(sqrt(number("80626033399934168")), 283947236);
		// random numbers
		BOOST_CHECK_EQUAL(sqrt(number("2138948239482394")), 46248764);
		BOOST_CHECK_EQUAL(sqrt(number("93458908293847283478234234")), number("9667414767860"));
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
