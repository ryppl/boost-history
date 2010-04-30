
/*
    The Extended Integer (XInt) Library
    A fast, portable C++ library for multi-precision integer math
    Copyright 2010 by Chad Nelson

    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt

    See http://www.boost.org/libs/xint for library home page.
*/

/*! \file
    \brief Contains tests for the fixed_integer type's initializations.
*/

#include <boost/xint/integer.hpp>
#include <boost/xint/nothrow_integer.hpp>
#include <boost/xint/fixed_integer.hpp>

#include <iomanip>

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

using namespace std;
using boost::test_tools::output_test_stream;

namespace boost {
namespace xint {

BOOST_AUTO_TEST_CASE(test_fixed_internals) {
    const size_t bits = detail::bits_per_digit + (detail::bits_per_digit / 2);
    typedef fixed_integer<bits> T; // Digit-and-a-half
    T test;

    BOOST_CHECK_EQUAL(test._fixed_bits(), bits);
}

BOOST_AUTO_TEST_CASE(test_numeric_limits) {
    BOOST_CHECK_EQUAL(std::numeric_limits<fixed_integer<8> >::digits10,
        std::numeric_limits<boost::uint8_t>::digits10);
    BOOST_CHECK_EQUAL(std::numeric_limits<fixed_integer<16> >::digits10,
        std::numeric_limits<boost::uint16_t>::digits10);
    BOOST_CHECK_EQUAL(std::numeric_limits<fixed_integer<32> >::digits10,
        std::numeric_limits<boost::uint32_t>::digits10);

    const size_t maxbits = std::numeric_limits<boost::uintmax_t>::digits;
    BOOST_CHECK_EQUAL(std::numeric_limits<fixed_integer<maxbits> >::digits10,
        std::numeric_limits<boost::uintmax_t>::digits10);

    const size_t bits = detail::bits_per_digit + (detail::bits_per_digit / 2);
    typedef fixed_integer<bits> T; // Digit-and-a-half
    const size_t digits10 = std::numeric_limits<T>::digits10;

    T n(1);
    for (size_t i = 0; i < digits10; ++i) n *= 10;

    T n2(n * 10);
    integer check(n);
    check *= 10;

    BOOST_CHECK_NE(integer(n2), check); // n2 should be truncated to a smaller number
}

BOOST_AUTO_TEST_CASE(test_fixed_from_nan) {
    typedef fixed_integer<12> T;
    BOOST_CHECK_THROW(T i(nothrow_integer::nan()), exceptions::not_a_number);
}

BOOST_AUTO_TEST_CASE(test_fixed_from_string) {
    integer src("0x12345678901234567890", autobase);
    typedef fixed_integer<12> T;
    T i(src);
    BOOST_CHECK_EQUAL(i, 0x890);
    BOOST_CHECK(i._is_fixed());
}

BOOST_AUTO_TEST_CASE(test_fixed_from_integer) {
    const std::string original("0x123456789012345678901234567890");
    const std::string trimmed("0x6789012345678901234567890");
    integer src(original, autobase);

    typedef fixed_integer<100> T;
    T i(src);
    BOOST_CHECK(i._is_fixed());

    output_test_stream output;
    output << hex << showbase << i;
    BOOST_CHECK(output.is_equal(trimmed));

    BOOST_CHECK_EQUAL(i, T(trimmed, autobase));
    BOOST_CHECK_EQUAL(integer(i), integer(trimmed, autobase));
}

BOOST_AUTO_TEST_CASE(test_fixed_from_fixed) {
    const std::string original("0x123456789012345678901234567890");
    typedef fixed_integer<12> T12;
    typedef fixed_integer<100> T100;

    T100 original_int(original, autobase);
    T12 smaller(original_int);
    T100 larger(smaller);
    larger = T100(original, autobase);

    BOOST_CHECK_EQUAL(smaller, 0x890);
    BOOST_CHECK_EQUAL(larger, original_int);
}

BOOST_AUTO_TEST_CASE(test_integer_from_fixed) {
    fixed_integer<12> n(0x890);
    integer i(n);
    BOOST_CHECK_EQUAL(i, 0x890);
    BOOST_CHECK(!i._is_fixed());
}

BOOST_AUTO_TEST_CASE(test_unary_operators) {
    fixed_integer<8> zero(0), one(1);
    BOOST_CHECK_EQUAL(!zero, true);
    BOOST_CHECK_EQUAL(!one, false);

    BOOST_CHECK_EQUAL(+one, 1);
    BOOST_CHECK_EQUAL(-one, -1);

    const size_t bits = detail::bits_per_digit + (detail::bits_per_digit / 2);
    typedef fixed_integer<bits> T; // Digit-and-a-half
    boost::uintmax_t src=0x12345678;
    T t(src);
    BOOST_CHECK_EQUAL(~t, T(~src));
}

BOOST_AUTO_TEST_CASE(test_fixed_add_subtract) {
    const size_t bits = detail::bits_per_digit + (detail::bits_per_digit / 2);
    typedef fixed_integer<bits> T; // Digit-and-a-half

    T n1(detail::digit_mask), n2;
    n2 = n1;
    BOOST_CHECK_EQUAL(n1, detail::digit_mask);
    BOOST_CHECK_EQUAL(n2, n1);

    T n3 = n1 + n2;

    integer check = (integer(n1) + integer(n2)) & (pow2(bits) - 1);
    BOOST_CHECK_EQUAL(integer(n3), check);

    typedef fixed_integer<8> T8;
    T8 n4 = T8(-255) - 2;
    BOOST_CHECK_EQUAL(n4, -1);

    n4 = 255;
    n4 += 2;
    BOOST_CHECK_EQUAL(n4, 1);
}

} // namespace xint
} // namespace boost
