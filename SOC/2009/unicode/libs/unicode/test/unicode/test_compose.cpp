#define BOOST_TEST_MODULE Composition
#include <boost/test/included/unit_test.hpp>

#include <boost/unicode/compose.hpp>
#include <boost/assign/list_of.hpp>

#include <boost/iterator/dummy_output_iterator.hpp>

#include "range_test.hpp"

namespace unicode = boost::unicode;
using boost::assign::list_of; 
using boost::char32;

#define CHECK_COMP(range1, range2)                                     \
do {                                                                   \
    CHECK_BI_EQUALS(unicode::adaptors::decompose(range1), range2);     \
    CHECK_BI_EQUALS(range1, unicode::adaptors::compose(range2));       \
} while(0)

BOOST_AUTO_TEST_CASE( decomposition )
{
    // Recursive test
    CHECK_COMP(
        list_of<char32>(0x1e17),
        list_of<char32>(0x65)(0x304)(0x301)
    );
    
    // Canonical reordering test
    CHECK_BI_EQUALS(
        unicode::adaptors::decompose(list_of<char32>(0x1e17)(0x330)),
        list_of<char32>(0x65)(0x330)(0x304)(0x301)
    );
    
    // Compatibility decompositions test
    CHECK_COMP(
        list_of<char32>(0xA8),
        list_of<char32>(0xA8)
    );
    
    CHECK_BI_EQUALS(
        unicode::adaptors::decompose(list_of<char32>(0xA8), UINT_MAX),
        list_of<char32>(0x20)(0x308)
    );
}

BOOST_AUTO_TEST_CASE( hangul )
{
    // Hangul LV
    CHECK_COMP(
        list_of<char32>(0xAC00),
        list_of<char32>(0x1100)(0x1161)
    );
    
    // Hangul LVT
    CHECK_COMP(
        list_of<char32>(0xAC01),
        list_of<char32>(0x1100)(0x1161)(0x11a8)
    );
}

BOOST_AUTO_TEST_CASE( normalization )
{
    // Canonical reordering
    CHECK_BI_EQUALS(
        unicode::adaptors::normalize(list_of<char32>(0x1e17)(0x330)),
        list_of<char32>(0x1e1b)(0x304)(0x301)
    );
}

BOOST_AUTO_TEST_CASE( error )
{
    try
    {
        unicode::normalize(
            list_of<char32>(0x20)(0x330)(0x330)(0x330)(0x330)(0x330)(0x330)
            (0x330)(0x330)(0x330)(0x330)(0x330)(0x330)(0x330)(0x330)(0x330)
            (0x330)(0x330)(0x330)(0x330)(0x330)(0x330)(0x330)(0x330)(0x330)
            (0x330)(0x330)(0x330)(0x330)(0x330)(0x330)(0x330),
            boost::dummy_output_iterator()
        );
    }
    catch(const std::out_of_range&)
    {
        return;
    }
    
    BOOST_CHECK(false);
    return;
}
