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

#include <string>
#include <sstream>

#pragma comment(lib, "libgmp-3.lib")

template <typename T, typename Tr> std::basic_ios<T, Tr>& setfillch(std::basic_ios<T, Tr>& s)
{
	s.fill('_');
	return s;
}

template <typename T, typename Tr> std::basic_ios<T, Tr>& setusanumlocale(std::basic_ios<T, Tr>& s)
{
	std::locale c_loc;
	std::locale sys(c_loc, "LC_NUMERIC=ENG_USA;", std::locale::numeric);
	
	s.imbue(sys);

	return s;
}

template <typename I> void test()
{
	typedef boost::bigint_base<I> number;

#ifdef BOOST_NO_STD_WSTREAMBUF

#define CHECK_OSS(expr, result) do \
								{ \
									std::ostringstream oss; oss << expr; \
									BOOST_CHECK_EQUAL(oss.str(), result); \
									BOOST_CHECK(oss.good()); \
								} \
								while (0)

#define CHECK_ISS_MOD_STATE(input, mod, result, state) do \
								{ \
									number a(7); \
									std::istringstream iss(input); iss >> mod >> a; \
									BOOST_CHECK_EQUAL(a, number(result)); \
									if (!state) BOOST_CHECK(iss.bad()); \
								} while (0)

#else

#define CHECK_OSS(expr, result) do \
								{ \
									std::ostringstream oss; oss << expr; \
									BOOST_CHECK_EQUAL(oss.str(), result); \
									BOOST_CHECK(oss.good()); \
									\
									std::wostringstream woss; woss << expr; \
									const char* r = result; \
									BOOST_CHECK(woss.str() == std::wstring(r, r + strlen(r))); \
									BOOST_CHECK(woss.good()); \
								} \
								while (0)

#define CHECK_ISS_MOD_STATE(input, mod, result, state) do \
								{ \
									number a(7); \
									std::istringstream iss(input); iss >> mod >> a; \
									BOOST_CHECK_EQUAL(a, number(result)); \
									if (!state) BOOST_CHECK(iss.bad()); \
									\
									number b(7); \
									const char* i = input; \
									std::wistringstream wiss(std::wstring(i, i + strlen(i))); wiss >> mod >> b; \
									BOOST_CHECK_EQUAL(b, number(result)); \
									if (!state) BOOST_CHECK(wiss.bad()); \
								} while (0)

#endif

#define CHECK_ISS_STATE(input, result, state) CHECK_ISS_MOD_STATE(input, std::skipws, result, state)

#define CHECK_ISS_MOD(input, mod, result) CHECK_ISS_MOD_STATE(input, mod, result, true)

#define CHECK_ISS(input, result) CHECK_ISS_STATE(input, result, true)

	// stream output

	// simple
	CHECK_OSS(number(10), "10");
	CHECK_OSS(number(-10), "-10");

	// bases
	CHECK_OSS(std::oct << number(10), "12");
	CHECK_OSS(std::oct << number(-10), "-12");
	CHECK_OSS(std::hex << number(10), "a");
	CHECK_OSS(std::hex << number(-10), "-a");
	CHECK_OSS(std::hex << std::dec << number(10), "10");
	CHECK_OSS(std::hex << std::dec << number(-10), "-10");

	// showpos
	CHECK_OSS(std::showpos << number(-1), "-1");
	CHECK_OSS(std::showpos << number(0), "+0");
	CHECK_OSS(std::showpos << number(1), "+1");

	// showbase
	CHECK_OSS(std::showbase << std::oct << number(10), "012");
	CHECK_OSS(std::showbase << std::oct << number(-10), "-012");
	CHECK_OSS(std::showbase << std::hex << number(10), "0xa");
	CHECK_OSS(std::showbase << std::hex << number(-10), "-0xa");

	// uppercase
	CHECK_OSS(std::uppercase << std::hex << number(10), "A");
	CHECK_OSS(std::uppercase << std::hex << number(-10), "-A");
	CHECK_OSS(std::showbase << std::uppercase << std::hex << number(10), "0XA");
	CHECK_OSS(std::showbase << std::uppercase << std::hex << number(-10), "-0XA");

	// padding

	// right
	CHECK_OSS(std::setw(6) << number(10), "    10");
	CHECK_OSS(std::setw(6) << number(-10), "   -10");
	CHECK_OSS(std::setw(6) << std::showbase << std::hex << number(10), "   0xa");
	CHECK_OSS(std::setw(6) << std::showbase << std::oct << number(10), "   012");
	CHECK_OSS(std::setw(6) << std::showbase << std::hex << number(-10), "  -0xa");
	CHECK_OSS(std::setw(6) << std::showbase << std::oct << number(-10), "  -012");
	CHECK_OSS(std::setw(6) << std::showpos << std::showbase << std::hex << number(10), "  +0xa");
	CHECK_OSS(std::setw(6) << std::showpos << std::showbase << std::oct << number(10), "  +012");

	// left
	CHECK_OSS(std::left << std::setw(6) << number(10), "10    ");
	CHECK_OSS(std::left << std::setw(6) << number(-10), "-10   ");
	CHECK_OSS(std::left << std::setw(6) << std::showbase << std::hex << number(10), "0xa   ");
	CHECK_OSS(std::left << std::setw(6) << std::showbase << std::oct << number(10), "012   ");
	CHECK_OSS(std::left << std::setw(6) << std::showbase << std::hex << number(-10), "-0xa  ");
	CHECK_OSS(std::left << std::setw(6) << std::showbase << std::oct << number(-10), "-012  ");
	CHECK_OSS(std::left << std::setw(6) << std::showpos << std::showbase << std::hex << number(10), "+0xa  ");
	CHECK_OSS(std::left << std::setw(6) << std::showpos << std::showbase << std::oct << number(10), "+012  ");

	// internal
	CHECK_OSS(std::internal << std::setw(6) << number(10), "    10");
	CHECK_OSS(std::internal << std::setw(6) << number(-10), "-   10");
	CHECK_OSS(std::internal << std::setw(6) << std::showbase << std::hex << number(10), "0x   a");
	CHECK_OSS(std::internal << std::setw(6) << std::showbase << std::oct << number(10), "   012");
	CHECK_OSS(std::internal << std::setw(6) << std::showbase << std::hex << number(-10), "-0x  a");
	CHECK_OSS(std::internal << std::setw(6) << std::showbase << std::oct << number(-10), "-  012");
	CHECK_OSS(std::internal << std::setw(6) << std::showpos << std::showbase << std::hex << number(10), "+0x  a");
	CHECK_OSS(std::internal << std::setw(6) << std::showpos << std::showbase << std::oct << number(10), "+  012");

	// internal + fill
	CHECK_OSS(setfillch << std::internal << std::setw(6) << number(10), "____10");
	CHECK_OSS(setfillch << std::internal << std::setw(6) << number(-10), "-___10");
	CHECK_OSS(setfillch << std::internal << std::setw(6) << std::showbase << std::hex << number(10), "0x___a");
	CHECK_OSS(setfillch << std::internal << std::setw(6) << std::showbase << std::oct << number(10), "___012");
	CHECK_OSS(setfillch << std::internal << std::setw(6) << std::showbase << std::hex << number(-10), "-0x__a");
	CHECK_OSS(setfillch << std::internal << std::setw(6) << std::showbase << std::oct << number(-10), "-__012");
	CHECK_OSS(setfillch << std::internal << std::setw(6) << std::showpos << std::showbase << std::hex << number(10), "+0x__a");
	CHECK_OSS(setfillch << std::internal << std::setw(6) << std::showpos << std::showbase << std::oct << number(10), "+__012");

	// padding + locale (thousands separator)
	try
	{
		std::ostringstream temp;
		temp << setusanumlocale; // this will throw for gcc/win32

		CHECK_OSS(std::setw(10) << setusanumlocale << number(1234567), " 1,234,567");
		CHECK_OSS(std::setw(10) << setusanumlocale << number(-234567), "  -234,567");
		CHECK_OSS(std::setw(10) << setusanumlocale << std::showpos << number(234567), "  +234,567");
		CHECK_OSS(std::setw(10) << setusanumlocale << std::showbase << std::oct << number(234567), "  0712,107");
	}
	catch (const std::exception&)
	{
	}

	// stream input

	// simple
	CHECK_ISS("+123", "123");
	CHECK_ISS("123", "123");
	CHECK_ISS("-123", "-123");
	CHECK_ISS("+0", "0");
	CHECK_ISS("0", "0");
	CHECK_ISS("-0", "0");

	// spaces
	CHECK_ISS("  \t-13", "-13");
	CHECK_ISS_MOD("  \t-13", std::noskipws, "7");
	CHECK_ISS("- 13", "7");
	CHECK_ISS_MOD("- 13", std::noskipws, "7");

	// bases
	CHECK_ISS_MOD("13", std::oct, "11");
	CHECK_ISS_MOD("-13", std::oct, "-11");
	CHECK_ISS_MOD("ff", std::hex, "255");
	CHECK_ISS_MOD("-ff", std::hex, "-255");
	CHECK_ISS_MOD("11", std::oct >> std::dec, "11");
	CHECK_ISS_MOD("-11", std::oct >> std::dec, "-11");

	// separators
	try
	{
		std::ostringstream temp;
		temp << setusanumlocale; // this will throw for gcc/win32

		CHECK_ISS_MOD("1,234,567", setusanumlocale, "1234567");
		CHECK_ISS_MOD("-234,567", setusanumlocale, "-234567");
		CHECK_ISS_MOD("+234,567", setusanumlocale, "234567");
		CHECK_ISS_MOD("0712,107", std::oct >> setusanumlocale, "234567");
		CHECK_ISS_MOD("0x123,456", std::hex >> setusanumlocale, "1193046");

		// whole input is incorrect - should leave variable untouched (and set bad state)
		CHECK_ISS_MOD_STATE("-3,,,", setusanumlocale, "7", false);
		CHECK_ISS_MOD_STATE("-3,22", setusanumlocale, "7", false);
		CHECK_ISS_MOD_STATE("-3,33,222,222", setusanumlocale, "7", false);
	}
	catch (const std::exception&)
	{
	}
	
	// whole input is incorrect - should leave variable untouched (and set bad state)
	CHECK_ISS_STATE("JAKFDJLK", "7", false);
	CHECK_ISS_STATE("-.3", "7", false);

	// input is partially incorrect - should read only correct part
	{
		number a;
		std::istringstream iss("-384ABC");
		iss >> a;
		
		BOOST_CHECK_EQUAL(a, -384);
		BOOST_CHECK_EQUAL(iss.get(), 65);
	}

#ifndef BOOST_NO_STD_WSTREAMBUF
	{
		number b;
		std::wistringstream wiss(L"-384ABC");
		wiss >> b;
		
		BOOST_CHECK_EQUAL(b, -384);
		BOOST_CHECK_EQUAL(wiss.get(), 65);
	}
#endif

	{
		number a;
		std::istringstream iss("FFZYX");
		iss >> std::hex >> a;
		
		BOOST_CHECK_EQUAL(a, 255);
		BOOST_CHECK_EQUAL(iss.get(), 90);
	}

#ifndef BOOST_NO_STD_WSTREAMBUF
	{
		number b;
		std::wistringstream wiss(L"FFZYX");
		wiss >> std::hex >> b;
		
		BOOST_CHECK_EQUAL(b, 255);
		BOOST_CHECK_EQUAL(wiss.get(), 90);
	}
#endif
	
	{
		number a;
		std::istringstream iss("1238a");
		iss >> std::oct >> a;
		
		BOOST_CHECK_EQUAL(a, 83);
		BOOST_CHECK_EQUAL(iss.get(), 56);
	}

#ifndef BOOST_NO_STD_WSTREAMBUF
	{
		number b;
		std::wistringstream wiss(L"1238a");
		wiss >> std::oct >> b;
		
		BOOST_CHECK_EQUAL(b, 83);
		BOOST_CHECK_EQUAL(wiss.get(), 56);
	}
#endif

#undef CHECK_ISS
#undef CHECK_OSS

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
