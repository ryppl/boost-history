//[ file
#include <iostream>

#include <boost/foreach.hpp>
#include <boost/foreach_auto.hpp>

#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm.hpp>

#include <boost/unicode/utf.hpp>
#include <boost/unicode/ucd/properties.hpp>
#include <boost/unicode/graphemes.hpp>
#include <boost/unicode/compose.hpp>

#include <boost/unicode/string_cp.hpp>

int main()
{
    namespace unicode = boost::unicode;
    namespace ucd = boost::unicode::ucd;
    using namespace boost;
    
    std::vector<boost::char32> v;
    
    /*v.push_back(122);
    v.push_back(27700);
    v.push_back(119070);
    v.push_back(123);*/
    
    v.push_back(1);
    v.push_back(0xE9);
    v.push_back(3);
    
    
    /*v.push_back(0x7a);
    v.push_back(0x6c34);
    v.push_back(0xd834);
    v.push_back(0xdd1e);
    v.push_back(0x7b);*/
    
    
    BOOST_FOREACH(char cp, make_reversed_range(unicode::u8_encoded(v)))
        std::cout << std::hex << (int)(unsigned char)cp << std::endl;
        
    std::vector<char> v2;
    copy(v, unicode::u8_encoded_out(std::back_inserter(v2)));
    
    BOOST_FOREACH(char32 cp, unicode::utf_decoded(v2))
        std::cout << std::hex << cp << std::endl;
        
    std::cout << std::endl;
    
    BOOST_AUTO(range, unicode::utf_bounded( unicode::u8_encoded(v) ) );
    BOOST_FOREACH_AUTO(code_points, range)
    {
        BOOST_FOREACH_AUTO(cu, code_points)
            std::cout << ' ' << std::hex << (int)(unsigned char)cu;
            
        std::cout << ',';
    }
    
    std::cout << "\n";
    
    char foo[] = "eoaéôn";
    BOOST_FOREACH_AUTO(subrange, unicode::utf_bounded(foo))
    {
        BOOST_FOREACH(unsigned char c, subrange)
            std::cout << c;

        std::cout << ' ';
    }
    std::cout << std::endl;
        
    //std::cout << "\n" << boost::unicode::ucd::get_name(0xE9) << std::endl;
    std::cout << boost::unicode::ucd::as_string(boost::unicode::ucd::get_block(0xE9)) << std::endl;
    
    boost::char32 grapheme_test[] = {
        'f', 'o', 'o', '\r', '\n', 0x113, 0x301, ' ', 0x1e17
    };
    
    BOOST_AUTO(grapheme_utf8_test, unicode::u8_encoded(grapheme_test));
    
    BOOST_FOREACH_AUTO(grapheme, unicode::utf_grapheme_bounded(grapheme_utf8_test))
    {
        std::cout << "(";
        BOOST_FOREACH(unsigned char c, grapheme)
            std::cout << c;
        std::cout << ")";
    }
    std::cout << std::endl << std::endl;
    
    for(const boost::char32* p = ucd::get_decomposition(0x1e17); *p; ++p)
        std::cout << "0x" << std::hex << *p << ' ';
    std::cout << std::endl;
    
    unicode::decomposer<> decomp;
    decomp(0x1e17, std::ostream_iterator<char32>(std::cout, " "));
    std::cout << std::endl << std::endl;
    
    unicode::decomposer<> decomp2;
    decomp2(0xa8, std::ostream_iterator<char32>(std::cout, " "));
    std::cout << std::endl;
    unicode::decomposer<static_pow<2, ucd::decomposition_type::compat>::value> decomp3;
    decomp3(0xa8, std::ostream_iterator<char32>(std::cout, " "));
    std::cout << std::endl;
    
    wchar_t baz[] = L"foo\u00E9";
    std::cout << unicode::u8_encoded(unicode::utf_decoded(baz)) << std::endl;
    
    std::cout << mpl::c_str<
        mpl::string<'foo ', unicode::string_cp<0xe9>::value, ' bar'>
    >::value << std::endl;
    
    std::cout << unicode::latin1_encoded(unicode::utf_decoded(mpl::c_str<
        mpl::u16string<'f', 'o', 'o', ' ', 0xe9, ' ', 'b', 'a', 'r'>
    >::value)) << std::endl;
}
//]
