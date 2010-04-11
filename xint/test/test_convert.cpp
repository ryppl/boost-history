
/*
    The Extended Integer (XInt) Library
    A fast, portable C++ library for multi-precision integer math
    Copyright 2010 by Chad Nelson

    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt

    This file contains the tests for the conversion functions.
*/

#include <boost/xint/xint.hpp>

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <vector>
#include <boost/assign/list_of.hpp>

using std::endl;

namespace boost {
namespace xint {

using namespace test;

BOOST_AUTO_TEST_CASE(testConvert) {
    // Come up with a sufficiently complex number to test with. It must be
    // greater than one digit's worth of data, and have at least one leading
    // zero in base 16.
    std::string nStr("2345678900987654321");
    integer n(sqr(integer(nStr)));

    // If it's zero, something went wrong that we might not catch below.
    BOOST_REQUIRE_NE(n, 0);

    // Make sure that the number is greater than one digit's worth of data, and
    // that it has at least one leading zero in base 16.
    detail::digit_t mask=(detail::doubledigit_t(0x0F) << (detail::bits_per_digit - 4));
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

    BOOST_REQUIRE(exceptions_allowed());

    // Test base numbers that shouldn't be allowed
    const std::vector<int> testBases=boost::assign::list_of(-1)(0)(1)(37);
    for (std::vector<int>::const_iterator b=testBases.begin();
        b!=testBases.end(); ++b)
            BOOST_CHECK_THROW(to_string(n, *b), invalid_base);

    boost::int32_t nSrc=1234567890;
    n=nSrc;
    boost::int32_t nTgt=to<boost::int32_t>(n);
    BOOST_CHECK_EQUAL(nTgt, nSrc);

    {
        token t=block_exceptions();
        BOOST_CHECK_NO_THROW(blockable::integer badConversion("abcdefg", 16));
    }

    {
        integer n1("8688527F70B1", 16);
        std::string nbin=to_binary(n1);
        integer n2=from_binary(nbin);
        BOOST_CHECK_EQUAL(n1, n2);
    }
}

} // namespace xint
} // namespace boost
