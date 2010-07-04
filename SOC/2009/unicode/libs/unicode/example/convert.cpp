//[ convert
/*`
This example shows the possible ways to convert between UTF
encodings, by converting a UTF-x range into UTF-8 and printing it.
*/
#include <boost/unicode/utf.hpp>
#include <boost/foreach.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <iterator>

namespace unicode = boost::unicode;

int main()
{
    wchar_t foo[] = L"hello \u00E9 world";
    std::vector<boost::char32> bar;
    std::string baz;

//` Decoding and encoding eager, using two buffers:
    unicode::utf_decode(foo, std::back_inserter(bar));
    unicode::u8_encode(bar, std::back_inserter(baz));
    std::cout << baz << std::endl;
    
    bar.clear();
    baz.clear();
    
//` Decoding and encoding eager, using one buffer and a stream iterator:
    unicode::utf_decode(foo, std::back_inserter(bar));
    unicode::u8_encode(bar, std::ostream_iterator<char>(std::cout));
    std::cout << std::endl;
    
//` Decoding eager, stream iterator lazily encoded:
    unicode::utf_decode(foo,
        unicode::adaptors::u8_encode_output(
            std::ostream_iterator<char>(std::cout)
        )
    );
    std::cout << std::endl;

//` Decoding is lazy, encoding is eager, using one buffer:
    unicode::u8_encode(unicode::adaptors::utf_decode(foo), std::back_inserter(baz));
    std::cout << baz << std::endl;
    
//` Decoding is lazy, encoding is eager, using stream iterators:
    unicode::u8_encode(unicode::adaptors::utf_decode(foo), std::ostream_iterator<char>(std::cout));
    std::cout << std::endl;

//` Fully lazy (recommended unless you need to store the result in a specific container):
   std::cout << unicode::adaptors::u8_encode(unicode::adaptors::utf_decode(foo)) << std::endl;
    
}
//]
