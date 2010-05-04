
/*
    The Extended Integer (XInt) Library
    A fast, portable C++ library for multi-precision integer math
    Copyright 2010 by Chad Nelson

    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt

    This file contains the tests for the conversion functions.
*/

#include <boost/xint/integer.hpp>
#include <boost/xint/nothrow_integer.hpp>
#include <boost/xint/fixed_integer.hpp>

#ifdef BOOST_XINT_SINGLE_TEST_PROGRAM
    #define BOOST_TEST_DYN_LINK
#else
    #define BOOST_TEST_MAIN
#endif
#include <boost/test/unit_test.hpp>

#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <vector>
#include <boost/assign/list_of.hpp>

using std::endl;

namespace boost {
namespace xint {

BOOST_AUTO_TEST_CASE(test_string_conversions) {
    // Come up with a sufficiently complex number to test with. It must be
    // greater than one digit's worth of data, and have at least one leading
    // zero in base 16.
    std::string nStr("2345678900987654321");
    integer n(sqr(integer(nStr)));

    // If it's zero, something went wrong that we might not catch below.
    BOOST_REQUIRE_NE(n, 0);

    // Make sure that the number is greater than one digit's worth of data, and
    // that it has at least one leading zero in base 16.
    detail::digit_t mask=(detail::doubledigit_t(0x0F) << (detail::bits_per_digit
        - 4));
    BOOST_REQUIRE_GT(n._get_length(), size_t(1));
    BOOST_REQUIRE_EQUAL(n._get_digit(n._get_length()-1) & mask, 0);

    for (int base=2; base<=36; ++base) {
        std::string s=to_string(n, base);
        integer nn(s, base);
        if (nn != n) {
            std::ostringstream out;
            out << "testConvert 1, " << base << ": " << to_string(n, base)
                << ", " << to_string(nn, base) << endl;
            BOOST_ERROR(out.str());
        }

        // Shouldn't be any extraneous leading zeros
        if (s[0]=='0') {
            std::ostringstream out;
            out << "testConvert 2, " << base << ": " << s << endl;
            BOOST_ERROR(out.str());
        }

        s=to_string(-n, base);
        nn=integer(s, base);
        if (nn != -n) {
            std::ostringstream out;
            out << "testConvert 3, " << base << ": " << to_string(-n, base)
                << ", " << to_string(nn, base) << endl;
            BOOST_ERROR(out.str());
        }
    }

    // Test base numbers that shouldn't be allowed
    const std::vector<int> testBases=boost::assign::list_of(-1)(0)(1)(37);
    for (std::vector<int>::const_iterator b=testBases.begin();
        b!=testBases.end(); ++b)
            BOOST_CHECK_THROW(to_string(n, *b), exceptions::invalid_base);

    boost::int32_t nSrc = 1234567890;
    n=nSrc;
    boost::int32_t nTgt = to<boost::int32_t>(n);
    BOOST_CHECK_EQUAL(nTgt, nSrc);

    BOOST_CHECK_NO_THROW(nothrow_integer badConversion("abcdefg", 16));

    {
		nothrow_integer test1 = nothrow_integer::nan();
		std::ostringstream outstream;
		outstream << test1;
		BOOST_CHECK_EQUAL(outstream.str(), detail::nan_text);

		std::istringstream instream;
		instream.str(outstream.str());
		nothrow_integer test2;
		instream >> test2;
		BOOST_CHECK(test2.is_nan());
	}
}

BOOST_AUTO_TEST_CASE(test_binary_conversions) {
    integer test(0x12345678);
    binary_t testbin = to_binary(test, 4);
    int x = 8;
    for (binary_t::iterator i = testbin.begin(), ie = testbin.end(); i != ie;
        ++i) BOOST_CHECK_EQUAL(*i, x--);
    BOOST_CHECK_EQUAL(x, 0);
    integer test2 = from_binary(testbin, 4);
    BOOST_CHECK_EQUAL(test, test2);

    integer n1("3856431dee2035bc3e0e41328688527f70b1", 16);
    binary_t nbin = to_binary(n1);
    BOOST_CHECK_EQUAL(nbin[0], 0xb1);
    integer n2 = from_binary(nbin);
    BOOST_CHECK_EQUAL(n1, n2);

    fixed_integer<8> n3 = fixed_from_binary<8>(nbin);
    BOOST_CHECK_EQUAL(n3, 0xb1);

    nbin = to_binary(n1, 5);
    n2 = from_binary(nbin, 5);
    BOOST_CHECK_EQUAL(n1, n2);

    BOOST_CHECK_THROW(from_binary(nbin, std::numeric_limits<unsigned char>::
        digits + 1), exceptions::invalid_argument);
    BOOST_CHECK_THROW(to_binary(n1, std::numeric_limits<unsigned char>::
        digits + 1), exceptions::invalid_argument);
}

BOOST_AUTO_TEST_CASE(test_to) {
    {
        typedef int T;
        T t = 1;
        integer n = (std::numeric_limits<T>::max)();
        BOOST_CHECK_NO_THROW(t = to<T>(n));
        BOOST_CHECK_EQUAL(t, (std::numeric_limits<T>::max)());
        ++n;
        BOOST_CHECK_THROW(t = to<T>(n), exceptions::too_big);

        n = (std::numeric_limits<T>::min)();
        BOOST_CHECK_NO_THROW(t = to<T>(n));
        BOOST_CHECK_EQUAL(t, (std::numeric_limits<T>::min)());
        --n;
        BOOST_CHECK_THROW(t = to<T>(n), exceptions::too_big);
    }

    {
        typedef unsigned int T;
        T t = 1;
        integer n = (std::numeric_limits<T>::max)();
        BOOST_CHECK_NO_THROW(t = to<T>(n));
        BOOST_CHECK_EQUAL(t, (std::numeric_limits<T>::max)());
        ++n;
        BOOST_CHECK_THROW(t = to<T>(n), exceptions::too_big);

        n = (std::numeric_limits<T>::min)();
        BOOST_CHECK_NO_THROW(t = to<T>(n));
        BOOST_CHECK_EQUAL(t, (std::numeric_limits<T>::min)());
        --n;
        BOOST_CHECK_THROW(t = to<T>(n), exceptions::too_big);
    }
}

BOOST_AUTO_TEST_CASE(test_copy_nan) {
    nothrow_integer n(nothrow_integer::nan());
    BOOST_CHECK(n.is_nan());
    nothrow_integer n2(n);
    BOOST_CHECK(n2.is_nan());
    n2=n;
    BOOST_CHECK(n2.is_nan());
}

} // namespace xint
} // namespace boost
