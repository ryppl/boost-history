#define BOOST_TEST_MODULE UTF
#include <boost/test/included/unit_test.hpp>

#include <boost/unicode/utf.hpp>
#include <boost/assign/list_of.hpp>

#include "range_test.hpp"

namespace unicode = boost::unicode;
using boost::assign::list_of; 
using boost::char32;
using boost::char16;

#define CHECK_UTF(name, range1, range2) \
do { \
    using unicode::adaptors::u##name##_encode; using unicode::adaptors::u##name##_decode; \
    CHECK_BI_EQUALS(u##name##_encode(range1), range2); \
    CHECK_BI_EQUALS(range1, u##name##_decode(range2)); \
} while(0)

BOOST_AUTO_TEST_CASE( basic )
{
    char32 input[] = {'$', 0xA2, 0x20AC, 0x24B62};
    
    CHECK_UTF(8,
        input,
        list_of<char>('$')(0xC2)(0xA2)(0xE2)(0x82)(0xAC)(0xF0)(0xA4)(0xAD)(0xA2)
    );
    
    CHECK_UTF(16,
        input,
        list_of<char16>('$')(0xA2)(0x20AC)(0xD852)(0xDF62)
    );
}

BOOST_AUTO_TEST_CASE( error )
{
    // TODO
}
