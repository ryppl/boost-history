/* Boost comparison.cpp test file
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

// This macro is not quite good, but - it's ok for our needs
#define ARRAY_SIZE(array) sizeof(array) / sizeof(array[0])

template <typename I> void test()
{
	typedef boost::bigint_base<I> number;

	struct test_entry
	{
		const char* lhs;
		const char* op;
		const char* rhs;
		bool result;
	};

	// Some tests are a bit redundant, because "test that comparison operations are sane" was implemented too late
	test_entry entries[] =
	{
		// ordering

		// 0 and -0
		{"0", "<", "-0", false},
		{"0", "<=", "-0", true},
		{"0", ">", "-0", false},
		{"0", ">=", "-0", true},

		// 0 and other numbers
		{"0", ">", "-3", true},
		{"0", ">=", "-3", true},
		{"0", "<", "-3", false},
		{"0", "<=", "-3", false},

		{"0", ">", "-234892349823943243", true},
		{"0", ">=", "-234892349823943243", true},
		{"0", "<", "-234892349823943243", false},
		{"0", "<=", "-234892349823943243", false},

		{"0", ">", "5", false},
		{"0", ">=", "5", false},
		{"0", "<", "5", true},
		{"0", "<=", "5", true},

		{"0", ">", "534892349823943243", false},
		{"0", ">=", "534892349823943243", false},
		{"0", "<", "534892349823943243", true},
		{"0", "<=", "534892349823943243", true},

		// equal small numbers
		{"3", "<", "3", false},
		{"3", "<=", "3", true},
		{"3", ">", "3", false},
		{"3", ">=", "3", true},

		{"-3", "<", "-3", false},
		{"-3", "<=", "-3", true},
		{"-3", ">", "-3", false},
		{"-3", ">=", "-3", true},

		// equal big numbers
		{"32398042834928394234", "<", "32398042834928394234", false},
		{"32398042834928394234", "<=", "32398042834928394234", true},
		{"32398042834928394234", ">", "32398042834928394234", false},
		{"32398042834928394234", ">=", "32398042834928394234", true},
	
		{"-32398042834928394234", "<", "-32398042834928394234", false},
		{"-32398042834928394234", "<=", "-32398042834928394234", true},
		{"-32398042834928394234", ">", "-32398042834928394234", false},
		{"-32398042834928394234", ">=", "-32398042834928394234", true},

		// small numbers of the same sign
		{"15", "<", "17", true},
		{"15", "<=", "17", true},
		{"15", ">", "17", false},
		{"15", ">=", "17", false},
		
		{"20", "<", "17", false},
		{"20", "<=", "17", false},
		{"20", ">", "17", true},
		{"20", ">=", "17", true},

		{"-15", "<", "-17", false},
		{"-15", "<=", "-17", false},
		{"-15", ">", "-17", true},
		{"-15", ">=", "-17", true},
		
		{"-20", "<", "-17", true},
		{"-20", "<=", "-17", true},
		{"-20", ">", "-17", false},
		{"-20", ">=", "-17", false},

		// big numbers of the same sign, big difference
		{"2387238472384", "<", "5387238472384", true},
		{"2387238472384", "<=", "5387238472384", true},
		{"2387238472384", ">", "5387238472384", false},
		{"2387238472384", ">=", "5387238472384", false},
		
		{"9387238472384", "<", "5387238472384", false},
		{"9387238472384", "<=", "5387238472384", false},
		{"9387238472384", ">", "5387238472384", true},
		{"9387238472384", ">=", "5387238472384", true},
		
		{"-2387238472384", "<", "-5387238472384", false},
		{"-2387238472384", "<=", "-5387238472384", false},
		{"-2387238472384", ">", "-5387238472384", true},
		{"-2387238472384", ">=", "-5387238472384", true},
		
		{"-9387238472384", "<", "-5387238472384", true},
		{"-9387238472384", "<=", "-5387238472384", true},
		{"-9387238472384", ">", "-5387238472384", false},
		{"-9387238472384", ">=", "-5387238472384", false},

		// big numbers of the same sign, small difference
		{"348923849238492384", "<", "348923849238492385", true},
		{"348923849238492384", "<=", "348923849238492385", true},
		{"348923849238492384", ">", "348923849238492385", false},
		{"348923849238492384", ">=", "348923849238492385", false},
		
		{"348923849238492386", "<", "348923849238492385", false},
		{"348923849238492386", "<=", "348923849238492385", false},
		{"348923849238492386", ">", "348923849238492385", true},
		{"348923849238492386", ">=", "348923849238492385", true},
		
		{"-348923849238492384", "<", "-348923849238492385", false},
		{"-348923849238492384", "<=", "-348923849238492385", false},
		{"-348923849238492384", ">", "-348923849238492385", true},
		{"-348923849238492384", ">=", "-348923849238492385", true},
		
		{"-348923849238492386", "<", "-348923849238492385", true},
		{"-348923849238492386", "<=", "-348923849238492385", true},
		{"-348923849238492386", ">", "-348923849238492385", false},
		{"-348923849238492386", ">=", "-348923849238492385", false},

		// small numbers of different sign
		{"34", "<", "-5", false},
		{"34", "<=", "-5", false},
		{"34", ">", "-5", true},
		{"34", ">=", "-5", true},
		
		{"-34", "<", "5", true},
		{"-34", "<=", "5", true},
		{"-34", ">", "5", false},
		{"-34", ">=", "5", false},

		// big numbers of different sign
		{"12834823948239423489", "<", "-523849234", false},
		{"12834823948239423489", "<=", "-523849234", false},
		{"12834823948239423489", ">", "-523849234", true},
		{"12834823948239423489", ">=", "-523849234", true},
		
		{"-12834823948239423489", "<", "523849234", true},
		{"-12834823948239423489", "<=", "523849234", true},
		{"-12834823948239423489", ">", "523849234", false},
		{"-12834823948239423489", ">=", "523849234", false},

		// equality

		// 0 and -0
		{"0", "==", "-0", true},
		{"0", "!=", "-0", false},

		// 0 and other numbers
		{"0", "==", "4", false},
		{"0", "!=", "4", true},
		{"0", "==", "-4", false},
		{"0", "!=", "-4", true},

		{"0", "==", "24389283492834928349", false},
		{"0", "!=", "24389283492834928349", true},
		{"0", "==", "-24389283492834928349", false},
		{"0", "!=", "-24389283492834928349", true},

		// small numbers
		{"4", "==", "4", true},
		{"4", "!=", "4", false},
		{"4", "==", "-4", false},
		{"4", "!=", "-4", true},
		
		{"4", "==", "8", false},
		{"4", "!=", "8", true},
		{"4", "==", "-8", false},
		{"4", "!=", "-8", true},
		
		{"8", "==", "4", false},
		{"8", "!=", "4", true},
		{"8", "==", "-4", false},
		{"8", "!=", "-4", true},

		// big numbers vs small numbers
		{"249839482934823944", "==", "8", false},
		{"249839482934823944", "!=", "8", true},
		{"249839482934823944", "==", "-8", false},
		{"249839482934823944", "!=", "-8", true},
		
		{"8", "==", "249839482934823944", false},
		{"8", "!=", "249839482934823944", true},
		{"8", "==", "-249839482934823944", false},
		{"8", "!=", "-249839482934823944", true},

		// big numbers
		{"12349812340981230494", "==", "12349812340981230494", true},
		{"12349812340981230494", "!=", "12349812340981230494", false},
		{"12349812340981230494", "==", "-12349812340981230494", false},
		{"12349812340981230494", "!=", "-12349812340981230494", true},
		
		{"20349230492349234344", "==", "359685968958695434088", false},
		{"20349230492349234344", "!=", "359685968958695434088", true},
		{"20349230492349234344", "==", "-359685968958695434088", false},
		{"20349230492349234344", "!=", "-359685968958695434088", true},
		
		{"359685968958695434088", "==", "20349230492349234344", false},
		{"359685968958695434088", "!=", "20349230492349234344", true},
		{"359685968958695434088", "==", "-20349230492349234344", false},
		{"359685968958695434088", "!=", "-20349230492349234344", true}
	};

	for (size_t i = 0; i < ARRAY_SIZE(entries); ++i)
	{
		const test_entry& e = entries[i];

		number a(e.lhs), b(e.rhs);
		bool c = e.result;
		
		// test that comparison operations are sane

		// <
		BOOST_CHECK_EQUAL(a < b, !(a >= b));
		if (a < b) BOOST_CHECK(a <= b);

		// <=
		BOOST_CHECK_EQUAL(a <= b, !(a > b));
		BOOST_CHECK_EQUAL(a <= b, a < b || a == b);

		// >
		BOOST_CHECK_EQUAL(a > b, !(a <= b));
		if (a > b) BOOST_CHECK(a >= b);

		// >=
		BOOST_CHECK_EQUAL(a >= b, !(a < b));
		BOOST_CHECK_EQUAL(a >= b, a > b || a == b);

		// ==
		BOOST_CHECK_EQUAL(a == b, !(a != b));
		BOOST_CHECK_EQUAL(a == b, a <= b && a >= b);

		// !=
		BOOST_CHECK_EQUAL(a != b, !(a == b));
		BOOST_CHECK_EQUAL(a != b, a < b || a > b);

#define GEN_OP(_op, sop)	else if (!strcmp(e.op, sop)) \
						{ \
							BOOST_CHECK_EQUAL(a _op b, c); \
							\
							if (a.template can_convert_to<boost::int64_t>()) \
							{ \
								BOOST_CHECK_EQUAL(a.template to_number<boost::int64_t>() _op b, c); \
							} \
							\
							if (b.template can_convert_to<boost::int64_t>()) \
							{ \
								BOOST_CHECK_EQUAL(a _op b.template to_number<boost::int64_t>(), c); \
							} \
						}

		// comparison tests
		if (false) ;
		GEN_OP(<, "<")
		GEN_OP(<=, "<=")
		GEN_OP(>, ">")
		GEN_OP(>=, ">=")
		GEN_OP(==, "==")
		GEN_OP(!=, "!=")
		else BOOST_CHECK(false);

#undef GEN_OP
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
