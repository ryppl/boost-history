//[ compose
#include <boost/unicode/compose.hpp>
#include <iostream>
#include <iterator>

namespace unicode = boost::unicode;
namespace ucd = unicode::ucd;

int main()
{
    boost::char32 cp = 0x1E17;
    
    std::cout << "Decomposition of U+01E17 within the UCD: ";
    for(const boost::char32* p = ucd::get_decomposition(cp); *p; ++p)
        std::cout << "0x" << std::hex << *p << ' ';
    std::cout << std::endl;
    std::cout << "Decomposition type: " << as_string(ucd::get_decomposition_type(cp)) << std::endl;
    std::cout << std::endl;
    
    unicode::decomposer<> canon_decomp;
    unicode::decomposer<UINT_MAX> compat_decomp;
    
    std::cout << "Canonical decomposition: ";
    canon_decomp(cp, std::ostream_iterator<boost::char32>(std::cout, " "));
    std::cout << std::endl << std::endl;
    
    std::cout << "Canonical decomposition of U+00A8: ";
    canon_decomp(0xA8, std::ostream_iterator<boost::char32>(std::cout, " "));
    std::cout << std::endl;
    std::cout << "Compatibility decomposition of U+00A8: ";
    compat_decomp(0xA8, std::ostream_iterator<boost::char32>(std::cout, " "));
    std::cout << std::endl;
}
//]
