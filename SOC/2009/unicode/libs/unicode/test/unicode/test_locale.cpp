#define BOOST_TEST_MODULE Locale
#include <boost/test/included/unit_test.hpp>

#include <boost/unicode/codecvt.hpp>
#include <boost/unicode/utf.hpp>

#include <boost/range/as_literal.hpp>
#include <iostream>

#include "range_test.hpp"

namespace unicode = boost::unicode;
using boost::char32;

BOOST_AUTO_TEST_CASE( locale_custom )
{
    std::locale old_locale;
    std::locale loc(old_locale, new unicode::utf_u8_codecvt());
    
    char input_utf8_[] = "hello \xc3\xa9 \xf0\xaa\x98\x80 world";
    boost::iterator_range<const char*> input_utf8 = boost::as_literal(input_utf8_);
    
    char32 output[] = {'h', 'e', 'l', 'l', 'o', ' ', 0xE9, ' ', 0x2A600, ' ', 'w', 'o', 'r', 'l', 'd'};
    
    std::cout << "------------ locale_decode custom ----------------\n" << std::endl;
    CHECK_EQUALS(unicode::adaptors::locale_decode(input_utf8, loc), output);
    std::cout << "------------ locale_encode custom ----------------\n" << std::endl;
    CHECK_EQUALS(unicode::adaptors::locale_encode(output, unicode::locale_encoder(unicode::utf_encoder<wchar_t>(), unicode::utf_locale_transcoder(loc))), input_utf8);
}

BOOST_AUTO_TEST_CASE( locale_custom_normalize )
{
    std::locale old_locale;
    std::locale loc(old_locale, new unicode::utf_u8_normalize_codecvt());
    
    char input_utf8_[] = "hello e\xcc\x81 \xf0\xaf\xa8\x9d world";
    boost::iterator_range<const char*> input_utf8 = boost::as_literal(input_utf8_);
    
    char32 output[] = {'h', 'e', 'l', 'l', 'o', ' ', 0xE9, ' ', 0x2A600, ' ', 'w', 'o', 'r', 'l', 'd'};
    
    std::cout << "------------ locale_decode custom normalize ----------------\n" << std::endl;
    CHECK_EQUALS(unicode::adaptors::locale_decode(input_utf8, loc), output);
}

BOOST_AUTO_TEST_CASE( locale_native )
{
#ifdef BOOST_WINDOWS
    char input_native_[] = "hello \u00E9 world";
#else
    char input_native_[] = "hello \u00E9 \U0002A600 world";
#endif
    boost::iterator_range<const char*> input_native = boost::as_literal(input_native_);
    
#ifdef BOOST_WINDOWS
    char32 output_native[] = {'h', 'e', 'l', 'l', 'o', ' ', 0xE9, ' ', 'w', 'o', 'r', 'l', 'd'};
#else
    char32 output_native[] = {'h', 'e', 'l', 'l', 'o', ' ', 0xE9, ' ', 0x2A600, ' ', 'w', 'o', 'r', 'l', 'd'};
#endif
    
    std::cout << "------------ locale_decode ----------------\n" << std::endl;
    CHECK_EQUALS(unicode::adaptors::locale_decode(input_native), output_native);
    std::cout << "------------ locale_encode ----------------\n" << std::endl;
    CHECK_EQUALS(unicode::adaptors::locale_encode(output_native), input_native);
}
