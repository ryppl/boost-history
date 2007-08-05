/* Boost number_ctors.cpp test file
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

#include <string>

#pragma comment(lib, "libgmp-3.lib")

// This macro is not quite good, but - it's ok for our needs
#define ARRAY_SIZE(array) sizeof(array) / sizeof(array[0])

template <typename I> void test()
{
	typedef boost::bigint_base<I> number;

	struct test_entry
	{
		int base; // negative base means that input is unusual, therefore backwards translation will fail
		const char* lhs;
		int value;
		const char* rhs;
	};

	test_entry entries[] =
	{
		// correct base 2
		{2, "0", 0, 0},
		{2, "101011010", 346, 0},
		{2, "101110000100010010110100001110000100110010100001000100110011", 0, "829870984705741107"},
		{-2, "-0", 0, 0},
		{2, "-101011010", -346, 0},
		{2, "-101110000100010010110100001110000100110010100001000100110011", 0, "-829870984705741107"},
		// correct base 9
		{9, "0", 0, 0},
		{9, "774", 634, 0},
		{9, "1231237872637126378", 0, "189269583091367543"},
		{-9, "-0", 0, 0},
		{9, "-774", -634, 0},
		{9, "-1231237872637126378", 0, "-189269583091367543"},
		// correct base 18
		{18, "0", 0, 0},
		{18, "a8", 188, 0},
		{18, "aabcd3478", 0, "116719998974"},
		{-18, "-0", 0, 0},
		{18, "-a8", -188, 0},
		{18, "-aabcd3478", 0, "-116719998974"},
		// correct base 27
		{27, "0", 0, 0},
		{27, "hdjjjk", 251228513, 0},
		{27, "2389aidahjdah786ad", 0, "4571349829653128989996813"},
		{-27, "-0", 0, 0},
		{27, "-hdjjjk", -251228513, 0},
		{27, "-2389aidahjdah786ad", 0, "-4571349829653128989996813"},
		// correct base 36
		{36, "0", 0, 0},
		{36, "sa3mx", 47500521, 0},
		{36, "39xmasdhaxnzxcjkcjgh7893", 0, "2043118606165143938951178056619542151"},
		{-36, "-0", 0, 0},
		{36, "-sa3mx", -47500521, 0},
		{36, "-39xmasdhaxnzxcjkcjgh7893", 0, "-2043118606165143938951178056619542151"},
		// spaces, uppercase letters
		{-36, "   \t-39xASKMXM44ksxjsDJgh7893", 0, "-2042964800170965112469457130317055623"},
		// terminating on the letter that does not belong to the set
		{-36, "sa3mx<3", 47500521, 0},
		{-10, "-3489x3e3", -3489, 0},
		{-16, "ABCDEFG", 11259375, 0},
		// leading zeroes
		{-36, "-0000sa3mx", -47500521, 0},
		// incorrect input
		{-10, "ABCDEF", 0, 0},
		{-10, "\xff", 0, 0},
		// handling non-ASCII symbols
		{-36, "sa3mx\xa3\xb3", 47500521, 0},

		// check full alphabet
		{36, "abcdefghijklmnopqrstuvwxyz1234567890", 0, "30483235087530204251026473460499750369628113087340027780"}
	};

	for (size_t i = 0; i < ARRAY_SIZE(entries); ++i)
	{
		const test_entry& e = entries[i];

		int base = abs(e.base);

		// test string ctors
		number ref = e.rhs ? number(e.rhs) : number(e.value);

		BOOST_CHECK_EQUAL(ref, number(e.lhs, base));
		BOOST_CHECK_EQUAL(ref, number(std::string(e.lhs), base));
		BOOST_CHECK_EQUAL(ref, number(std::wstring(e.lhs, e.lhs + strlen(e.lhs)).c_str(), base));
		BOOST_CHECK_EQUAL(ref, number(std::wstring(e.lhs, e.lhs + strlen(e.lhs)), base));

		if (e.base > 0)
		{
			// test conversion
			BOOST_CHECK_EQUAL(ref.str(base), std::string(e.lhs));
			BOOST_CHECK(ref.wstr(base) == std::wstring(e.lhs, e.lhs + strlen(e.lhs))); // bug in boost.test - no wstring support
		}
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
