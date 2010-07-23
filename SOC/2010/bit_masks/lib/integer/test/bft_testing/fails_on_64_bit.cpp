#include <boost/integer/endian.hpp>
#include <boost/integer/endian_binary_stream.hpp>
#include <boost/binary_stream.hpp>

#include <boost/integer/bitfield_tuple.hpp>
#include <boost/detail/lightweight_test.hpp>

// used for working with the special conversion tools for creating 
// specific endianed storage.
#include <boost/type_traits/is_convertible.hpp>

#include <boost/mpl/bitwise.hpp>
#include <boost/mpl/arithmetic.hpp>
#include <climits>

#include <boost/assert.hpp>
#include <iostream>
#include <typeinfo>
#include <cstdio>


struct red;
struct green;
struct blue;

using namespace boost;
using namespace boost::integer;
using namespace boost::bitfields;

typedef bitfield_tuple<
    storage< big32_t >,
    member<unsigned char, red, 5>,
    member<unsigned char, green, 6>,
    member<unsigned char, blue, 5>
>                               rgb565_t;



int main() {
    {
    rgb565_t rgb565;
    rgb565.get<red>() = 3;
    BOOST_TEST(rgb565.get<red>() == 3);
    rgb565.get<0>() = 4;
    BOOST_TEST(rgb565.get<0>() == 4);
    }
    return boost::report_errors();
}
