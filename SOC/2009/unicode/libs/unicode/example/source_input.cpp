//[ source_input
/*`
This example shows how to input some non-ASCII unicode
characters into source files in different means, following the
[link unicode.appendices.appendix_source.portability_guidelines portability
guidelines], and then how to convert them to displayable UTF-8.

The following strings are considered:

* $ U+00A2 U+20AC U+024B62
* hello U+00E9 world
*/

#include <boost/unicode/string_cp.hpp>

#include <boost/unicode/utf.hpp>
#include <boost/unicode/static_utf.hpp>
#include <iostream>

namespace mpl = boost::mpl;
namespace unicode = boost::unicode;

//` Direct UTF-8:
char direct_utf8_1[] = "$¢€𤭢";
char direct_utf8_2[] = "hello é world";

//` UTF-8 as escape sequences:
char escape_utf8_1[] = "$\xC2\xA2\xE2\x82\xAC\xF0\xA4\xAD\xA2";
char escape_utf8_2[] = "hello \xC3\xA9 world";

//` Probably UTF-X (either 16 or 32) as Unicode escape sequences:
wchar_t utfx_1[] = L"$\u00A2\u20AC\U00024B62";
wchar_t utfx_2[] = L"hello \u00E9 world";

//` Compile-time direct UTF-8:
typedef mpl::string<'$', '¢', '€', '𤭢'> static_direct_utf8_1;
typedef mpl::string<'hell', 'o ', 'é', ' wor', 'ld'> static_direct_utf8_2;

//` Compile-time UTF-8 with Unicode code points:
typedef mpl::string<'$', unicode::string_cp<0xA2>::value, unicode::string_cp<0x20AC>::value, unicode::string_cp<0x24B62>::value> static_escape_utf8_1;
typedef mpl::string<'hell', 'o ', unicode::string_cp<0xE9>::value, ' wor', 'ld'> static_escape_utf8_2;

//` Compile-time UTF-16:
typedef mpl::u16string<'$', 0xA2, 0x20AC, 0xD852, 0xDF62> static_utf16_1;
typedef mpl::u16string<'h', 'e', 'l', 'l', 'o', ' ', 0xE9, ' ', 'w', 'o', 'r', 'l', 'd'> static_utf16_2;

//` Compile-time UTF-32:
typedef mpl::u32string<'$', 0xA2, 0x20AC, 0x24B64> static_utf32_1;
typedef mpl::u32string<'h', 'e', 'l', 'l', 'o', ' ', 0xE9, ' ', 'w', 'o', 'r', 'l', 'd'> static_utf32_2;

//` Let's display everything as UTF-8:
int main()
{
//` For strings that are already in UTF-8, nothing to do:
    std::cout << direct_utf8_1 << std::endl;
    std::cout << direct_utf8_2 << std::endl;
    std::cout << escape_utf8_1 << std::endl;
    std::cout << escape_utf8_2 << std::endl;

/*`
Wide strings are either UTF-16 or UTF-32, we convert them to UTF-32
then back to UTF-8:
*/
    std::cout << unicode::adaptors::u8_encode(unicode::adaptors::utf_decode(utfx_1)) << std::endl;
    std::cout << unicode::adaptors::u8_encode(unicode::adaptors::utf_decode(utfx_2)) << std::endl;

/*`
Compile-time UTF-8 strings just need to be converted to runtime ones:
*/    
    std::cout << mpl::c_str<static_direct_utf8_1>::value << std::endl;
    std::cout << mpl::c_str<static_direct_utf8_2>::value << std::endl;
    std::cout << mpl::c_str<static_escape_utf8_1>::value << std::endl;
    std::cout << mpl::c_str<static_escape_utf8_2>::value << std::endl;

/*`
Compile-time decoding of UTF-16 is unsupported at the moment, so we
cannot convert `static_utf16_1` and `static_utf16_2` to displayable
UTF-8.

Compile-time UTF-32 can be converted at compile-time to UTF-8 then
converted to runtime strings:
*/
    std::cout << mpl::c_str<unicode::static_u8_encode<static_utf32_1>::type>::value << std::endl;
    std::cout << mpl::c_str<unicode::static_u8_encode<static_utf32_2>::type>::value << std::endl;
}

//]
