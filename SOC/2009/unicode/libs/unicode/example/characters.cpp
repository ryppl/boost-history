//[ characters
/*`
This example shows the difference between code units, code points and
graphemes, the different levels at which to approach the concept of
character.
*/
#include <boost/range/algorithm.hpp>
#include <boost/range/as_literal.hpp>
#include <boost/foreach.hpp>

#include <boost/unicode/utf.hpp>
#include <boost/unicode/graphemes.hpp>

#include <iostream>

namespace unicode = boost::unicode;

int main()
{
    char foo_[] = "foo\r\n\xc4\x93\xcc\x81 \xe1\xb8\x97";
    
    // We don't use foo_ directly because we don't want the terminating 0 to be part of the range
    boost::iterator_range<char*> foo = boost::as_literal(foo_);

    std::cout << "Code units: " << boost::distance(foo) << std::endl;
    std::cout << "Code points: " << boost::distance(unicode::adaptors::u8_segment(foo)) << std::endl; /*< We could also have used `unicode::adaptors::u8_decode(foo)` here. >*/
    std::cout << "Graphemes: " << boost::distance(unicode::adaptors::u8_grapheme_segment(foo)) << std::endl; /*< We could also have used `unicode::adaptors::grapheme_segment(unicode::adaptors::u8_decode(foo))` here. >*/
    
    std::cout << std::endl;
    std::cout << "Code points boundaries:" << std::endl; /*< Take into account your terminal may be interpreting the carriage return when viewing the output and thus visually produce strange results. >*/
    BOOST_FOREACH(boost::iterator_range<char*> cp, unicode::adaptors::u8_segment(foo)) /*< In case the type of `cp` is too difficult to deduce, you could use `BOOST_FOREACH_AUTO` if your platform supports it or type erasure with `boost::lazy_range<char>` if you don't mind the overhead. >*/
        std::cout << '(' << cp << ')';
    std::cout << std::endl;
    
    std::cout << std::endl;
    std::cout << "Graphemes boundaries:" << std::endl;
    BOOST_FOREACH(boost::iterator_range<char*> grapheme, unicode::adaptors::u8_grapheme_segment(foo))
        std::cout << '(' << grapheme << ')';
    std::cout << std::endl;
}
//]
