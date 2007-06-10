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

#pragma comment(lib, "libgmp-3.lib")

template <typename I> void test()
{
	typedef boost::bigint_base<I> number;

	// char
	BOOST_CHECK(!number("-129").can_convert_to<char>());
	BOOST_CHECK( number("-128").can_convert_to<char>());
	BOOST_CHECK( number("-127").can_convert_to<char>());

	BOOST_CHECK( number("-1").can_convert_to<char>());
	BOOST_CHECK( number("0").can_convert_to<char>());
	BOOST_CHECK( number("1").can_convert_to<char>());

	BOOST_CHECK( number("127").can_convert_to<char>());
	BOOST_CHECK(!number("128").can_convert_to<char>());
	BOOST_CHECK(!number("129").can_convert_to<char>());

	// unsigned char
	BOOST_CHECK(!number("-12930").can_convert_to<unsigned char>());
	BOOST_CHECK(!number("-1").can_convert_to<unsigned char>());

	BOOST_CHECK( number("0").can_convert_to<unsigned char>());
	BOOST_CHECK( number("1").can_convert_to<unsigned char>());

	BOOST_CHECK( number("127").can_convert_to<unsigned char>());
	BOOST_CHECK( number("128").can_convert_to<unsigned char>());
	BOOST_CHECK( number("129").can_convert_to<unsigned char>());

	BOOST_CHECK( number("255").can_convert_to<unsigned char>());
	BOOST_CHECK(!number("256").can_convert_to<unsigned char>());

	// short
	BOOST_CHECK(!number("-32769").can_convert_to<short>());
	BOOST_CHECK( number("-32768").can_convert_to<short>());
	BOOST_CHECK( number("-32767").can_convert_to<short>());

	BOOST_CHECK( number("-1").can_convert_to<short>());
	BOOST_CHECK( number("0").can_convert_to<short>());
	BOOST_CHECK( number("1").can_convert_to<short>());

	BOOST_CHECK( number("32767").can_convert_to<short>());
	BOOST_CHECK(!number("32768").can_convert_to<short>());
	BOOST_CHECK(!number("32769").can_convert_to<short>());

	// unsigned short
	BOOST_CHECK(!number("-12930").can_convert_to<unsigned short>());
	BOOST_CHECK(!number("-1").can_convert_to<unsigned short>());

	BOOST_CHECK( number("0").can_convert_to<unsigned short>());
	BOOST_CHECK( number("1").can_convert_to<unsigned short>());

	BOOST_CHECK( number("32767").can_convert_to<unsigned short>());
	BOOST_CHECK( number("32768").can_convert_to<unsigned short>());
	BOOST_CHECK( number("32769").can_convert_to<unsigned short>());

	BOOST_CHECK( number("65535").can_convert_to<unsigned short>());
	BOOST_CHECK(!number("65536").can_convert_to<unsigned short>());

	// int
	BOOST_CHECK(!number("-80000001", 16).can_convert_to<int>());
	BOOST_CHECK( number("-80000000", 16).can_convert_to<int>());
	BOOST_CHECK( number("-7fffffff", 16).can_convert_to<int>());
	
	BOOST_CHECK( number("-1").can_convert_to<int>());
	BOOST_CHECK( number("0").can_convert_to<int>());
	BOOST_CHECK( number("1").can_convert_to<int>());

	BOOST_CHECK( number("7fffffff", 16).can_convert_to<int>());
	BOOST_CHECK(!number("80000000", 16).can_convert_to<int>());
	BOOST_CHECK(!number("80000001", 16).can_convert_to<int>());

	// unsigned int
	BOOST_CHECK(!number("-12930").can_convert_to<unsigned int>());
	BOOST_CHECK(!number("-1").can_convert_to<unsigned int>());

	BOOST_CHECK( number("0").can_convert_to<unsigned int>());
	BOOST_CHECK( number("1").can_convert_to<unsigned int>());

	BOOST_CHECK( number("7fffffff", 16).can_convert_to<unsigned int>());
	BOOST_CHECK( number("80000000", 16).can_convert_to<unsigned int>());
	BOOST_CHECK( number("80000001", 16).can_convert_to<unsigned int>());

	BOOST_CHECK( number("ffffffff", 16).can_convert_to<unsigned int>());
	BOOST_CHECK(!number("100000000", 16).can_convert_to<unsigned int>());

	// int64_t
	BOOST_CHECK(!number("-8000000000000001", 16).can_convert_to<boost::int64_t>());
	BOOST_CHECK( number("-8000000000000000", 16).can_convert_to<boost::int64_t>());
	BOOST_CHECK( number("-7fffffffffffffff", 16).can_convert_to<boost::int64_t>());
	
	BOOST_CHECK( number("-80000001", 16).can_convert_to<boost::int64_t>());
	BOOST_CHECK( number("-80000000", 16).can_convert_to<boost::int64_t>());
	BOOST_CHECK( number("-7fffffff", 16).can_convert_to<boost::int64_t>());

	BOOST_CHECK( number("-1").can_convert_to<boost::int64_t>());
	BOOST_CHECK( number("0").can_convert_to<boost::int64_t>());
	BOOST_CHECK( number("1").can_convert_to<boost::int64_t>());

	BOOST_CHECK( number("7fffffff", 16).can_convert_to<boost::int64_t>());
	BOOST_CHECK( number("80000000", 16).can_convert_to<boost::int64_t>());

	BOOST_CHECK( number("7fffffffffffffff", 16).can_convert_to<boost::int64_t>());
	BOOST_CHECK(!number("8000000000000000", 16).can_convert_to<boost::int64_t>());
	BOOST_CHECK(!number("8000000000000001", 16).can_convert_to<boost::int64_t>());

	// uint64_t
	BOOST_CHECK(!number("-12930").can_convert_to<boost::uint64_t>());
	BOOST_CHECK(!number("-1").can_convert_to<boost::uint64_t>());

	BOOST_CHECK( number("0").can_convert_to<boost::uint64_t>());
	BOOST_CHECK( number("1").can_convert_to<boost::uint64_t>());

	BOOST_CHECK( number("7fffffff", 16).can_convert_to<boost::uint64_t>());
	BOOST_CHECK( number("80000000", 16).can_convert_to<boost::uint64_t>());
	BOOST_CHECK( number("80000001", 16).can_convert_to<boost::uint64_t>());

	BOOST_CHECK( number("ffffffff", 16).can_convert_to<boost::uint64_t>());
	BOOST_CHECK( number("100000000", 16).can_convert_to<boost::uint64_t>());
	
	BOOST_CHECK( number("7fffffffffffffff", 16).can_convert_to<boost::uint64_t>());
	BOOST_CHECK( number("8000000000000000", 16).can_convert_to<boost::uint64_t>());
	BOOST_CHECK( number("8000000000000001", 16).can_convert_to<boost::uint64_t>());
	
	BOOST_CHECK( number("7fffffffffffffff", 16).can_convert_to<boost::uint64_t>());
	BOOST_CHECK( number("8000000000000000", 16).can_convert_to<boost::uint64_t>());
	BOOST_CHECK( number("8000000000000001", 16).can_convert_to<boost::uint64_t>());
	
	BOOST_CHECK( number("ffffffffffffffff", 16).can_convert_to<boost::uint64_t>());
	BOOST_CHECK(!number("10000000000000001", 16).can_convert_to<boost::uint64_t>());
}

int test_main(int argc, char* argv[])
{
  test<boost::detail::bigint_gmp_implementation>();

  return 0;
}
