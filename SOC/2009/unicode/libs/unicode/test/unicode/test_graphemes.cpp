#define BOOST_TEST_MODULE Graphemes
#include <boost/test/included/unit_test.hpp>

#include <boost/unicode/graphemes.hpp>

bool is_boundary(boost::char32 lft, boost::char32 rgt)
{
    boost::char32 tmp[] = {lft, rgt};
    return boost::unicode::grapheme_boundary()(tmp, tmp+2, tmp+1);
}

BOOST_AUTO_TEST_CASE( grapheme_boundary )
{
    BOOST_CHECK(is_boundary('a', 'b'));
    
    BOOST_CHECK(!is_boundary('a', 0x308));
    BOOST_CHECK(is_boundary(0x308, 'a'));
    
    BOOST_CHECK(!is_boundary(0x308, 0x308));
    
    BOOST_CHECK(!is_boundary('\r', '\n'));
    
    
}


