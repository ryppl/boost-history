/* Boost can_convert_to.cpp test file
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

	// char
	BOOST_CHECK(!number("-129").template can_convert_to<char>());
	BOOST_CHECK( number("-128").template can_convert_to<char>());
	BOOST_CHECK( number("-127").template can_convert_to<char>());

	BOOST_CHECK( number("-1").template can_convert_to<char>());
	BOOST_CHECK( number("0").template can_convert_to<char>());
	BOOST_CHECK( number("1").template can_convert_to<char>());

	BOOST_CHECK( number("127").template can_convert_to<char>());
	BOOST_CHECK(!number("128").template can_convert_to<char>());
	BOOST_CHECK(!number("129").template can_convert_to<char>());

	// unsigned char
	BOOST_CHECK(!number("-12930").template can_convert_to<unsigned char>());
	BOOST_CHECK(!number("-1").template can_convert_to<unsigned char>());

	BOOST_CHECK( number("0").template can_convert_to<unsigned char>());
	BOOST_CHECK( number("1").template can_convert_to<unsigned char>());

	BOOST_CHECK( number("127").template can_convert_to<unsigned char>());
	BOOST_CHECK( number("128").template can_convert_to<unsigned char>());
	BOOST_CHECK( number("129").template can_convert_to<unsigned char>());

	BOOST_CHECK( number("255").template can_convert_to<unsigned char>());
	BOOST_CHECK(!number("256").template can_convert_to<unsigned char>());

	// short
	BOOST_CHECK(!number("-32769").template can_convert_to<short>());
	BOOST_CHECK( number("-32768").template can_convert_to<short>());
	BOOST_CHECK( number("-32767").template can_convert_to<short>());

	BOOST_CHECK( number("-1").template can_convert_to<short>());
	BOOST_CHECK( number("0").template can_convert_to<short>());
	BOOST_CHECK( number("1").template can_convert_to<short>());

	BOOST_CHECK( number("32767").template can_convert_to<short>());
	BOOST_CHECK(!number("32768").template can_convert_to<short>());
	BOOST_CHECK(!number("32769").template can_convert_to<short>());

	// unsigned short
	BOOST_CHECK(!number("-12930").template can_convert_to<unsigned short>());
	BOOST_CHECK(!number("-1").template can_convert_to<unsigned short>());

	BOOST_CHECK( number("0").template can_convert_to<unsigned short>());
	BOOST_CHECK( number("1").template can_convert_to<unsigned short>());

	BOOST_CHECK( number("32767").template can_convert_to<unsigned short>());
	BOOST_CHECK( number("32768").template can_convert_to<unsigned short>());
	BOOST_CHECK( number("32769").template can_convert_to<unsigned short>());

	BOOST_CHECK( number("65535").template can_convert_to<unsigned short>());
	BOOST_CHECK(!number("65536").template can_convert_to<unsigned short>());

	// int
	BOOST_CHECK(!number("-80000001", 16).template can_convert_to<int>());
	BOOST_CHECK( number("-80000000", 16).template can_convert_to<int>());
	BOOST_CHECK( number("-7fffffff", 16).template can_convert_to<int>());
	
	BOOST_CHECK( number("-1").template can_convert_to<int>());
	BOOST_CHECK( number("0").template can_convert_to<int>());
	BOOST_CHECK( number("1").template can_convert_to<int>());

	BOOST_CHECK( number("7fffffff", 16).template can_convert_to<int>());
	BOOST_CHECK(!number("80000000", 16).template can_convert_to<int>());
	BOOST_CHECK(!number("80000001", 16).template can_convert_to<int>());

	// unsigned int
	BOOST_CHECK(!number("-12930").template can_convert_to<unsigned int>());
	BOOST_CHECK(!number("-1").template can_convert_to<unsigned int>());

	BOOST_CHECK( number("0").template can_convert_to<unsigned int>());
	BOOST_CHECK( number("1").template can_convert_to<unsigned int>());

	BOOST_CHECK( number("7fffffff", 16).template can_convert_to<unsigned int>());
	BOOST_CHECK( number("80000000", 16).template can_convert_to<unsigned int>());
	BOOST_CHECK( number("80000001", 16).template can_convert_to<unsigned int>());

	BOOST_CHECK( number("ffffffff", 16).template can_convert_to<unsigned int>());
	BOOST_CHECK(!number("100000000", 16).template can_convert_to<unsigned int>());

	// int64_t
	BOOST_CHECK(!number("-8000000000000001", 16).template can_convert_to<boost::int64_t>());
	BOOST_CHECK( number("-8000000000000000", 16).template can_convert_to<boost::int64_t>());
	BOOST_CHECK( number("-7fffffffffffffff", 16).template can_convert_to<boost::int64_t>());
	
	BOOST_CHECK( number("-80000001", 16).template can_convert_to<boost::int64_t>());
	BOOST_CHECK( number("-80000000", 16).template can_convert_to<boost::int64_t>());
	BOOST_CHECK( number("-7fffffff", 16).template can_convert_to<boost::int64_t>());

	BOOST_CHECK( number("-1").template can_convert_to<boost::int64_t>());
	BOOST_CHECK( number("0").template can_convert_to<boost::int64_t>());
	BOOST_CHECK( number("1").template can_convert_to<boost::int64_t>());

	BOOST_CHECK( number("7fffffff", 16).template can_convert_to<boost::int64_t>());
	BOOST_CHECK( number("80000000", 16).template can_convert_to<boost::int64_t>());

	BOOST_CHECK( number("7fffffffffffffff", 16).template can_convert_to<boost::int64_t>());
	BOOST_CHECK(!number("8000000000000000", 16).template can_convert_to<boost::int64_t>());
	BOOST_CHECK(!number("8000000000000001", 16).template can_convert_to<boost::int64_t>());

	// uint64_t
	BOOST_CHECK(!number("-12930").template can_convert_to<boost::uint64_t>());
	BOOST_CHECK(!number("-1").template can_convert_to<boost::uint64_t>());

	BOOST_CHECK( number("0").template can_convert_to<boost::uint64_t>());
	BOOST_CHECK( number("1").template can_convert_to<boost::uint64_t>());

	BOOST_CHECK( number("7fffffff", 16).template can_convert_to<boost::uint64_t>());
	BOOST_CHECK( number("80000000", 16).template can_convert_to<boost::uint64_t>());
	BOOST_CHECK( number("80000001", 16).template can_convert_to<boost::uint64_t>());

	BOOST_CHECK( number("ffffffff", 16).template can_convert_to<boost::uint64_t>());
	BOOST_CHECK( number("100000000", 16).template can_convert_to<boost::uint64_t>());
	
	BOOST_CHECK( number("7fffffffffffffff", 16).template can_convert_to<boost::uint64_t>());
	BOOST_CHECK( number("8000000000000000", 16).template can_convert_to<boost::uint64_t>());
	BOOST_CHECK( number("8000000000000001", 16).template can_convert_to<boost::uint64_t>());
	
	BOOST_CHECK( number("7fffffffffffffff", 16).template can_convert_to<boost::uint64_t>());
	BOOST_CHECK( number("8000000000000000", 16).template can_convert_to<boost::uint64_t>());
	BOOST_CHECK( number("8000000000000001", 16).template can_convert_to<boost::uint64_t>());
	
	BOOST_CHECK( number("ffffffffffffffff", 16).template can_convert_to<boost::uint64_t>());
	BOOST_CHECK(!number("10000000000000001", 16).template can_convert_to<boost::uint64_t>());
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
