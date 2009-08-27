//[ compose
/*`
This example shows how to decompose, recompose, normalize strings and
how to maintain a normalized form while concatenating two strings.
*/

#include <boost/unicode/compose.hpp>
#include <boost/unicode/cat.hpp>

#include <boost/range/algorithm/copy.hpp>
#include <boost/assign/list_of.hpp> 

#include <iostream>
#include <iterator>

namespace unicode = boost::unicode;
namespace ucd = unicode::ucd;
using boost::assign::list_of; 
using boost::char32;

/*`
We're going to do a lot of copying to std::out with spaces to delimit
the elements in that example, so we instantiate the iterator once:
*/
static std::ostream_iterator<char32> output(std::cout, " ");

int main()
{
    char32 cp = 0x1E17;
    
    // We want all results in hexadecimal
    std::cout << std::hex;
    
    std::cout << "Decomposition of U+01E17 within the UCD: ";
    boost::copy(ucd::get_decomposition(cp), output);
    std::cout << std::endl;
    std::cout << "Decomposition type: " << as_string(ucd::get_decomposition_type(cp)) << std::endl;
    
    std::cout << "Canonical decomposition: ";
    unicode::decompose(list_of(cp), output);
    std::cout << std::endl;
    
    std::cout << "Canonical decomposition of U+00A8: ";
    unicode::decompose(list_of(0xA8), output);
    std::cout << std::endl;
    std::cout << "Compatibility decomposition of U+00A8: ";
    unicode::decompose(list_of(0xA8), output, UINT_MAX);
    std::cout << std::endl;
    std::cout << std::endl;
    
    char32 foo[] = { 0x113, 0x301 };
    std::cout << "Canonical composition of { ";
    boost::copy(foo, output);
    std::cout << "}: ";
    unicode::compose(foo, output);
    std::cout << std::endl;
    
    char32 foo2[] = { 0x65, 0x304, 0x301 };
    std::cout << "Canonical composition of { ";
    boost::copy(foo2, output);
    std::cout << "}: ";
    unicode::compose(foo2, output);
    std::cout << std::endl;
    
    char32 bar[] = { 0x20, 0x308 };
    std::cout << "Canonical composition of { ";
    boost::copy(bar, output);
    std::cout << "}: ";
    unicode::compose(bar, output);
    std::cout << std::endl << std::endl;
    
    char32 baz[] = { cp, 0x330 };
    std::cout << "Normalization C of { ";
    boost::copy(baz, output);
    std::cout << "}: ";
    unicode::normalize(baz, output);
    std::cout << std::endl;
    
    char32 cat_dec1[] = { 0x48, 0x65, 0x304, 0x301 };
    char32 cat_dec2[] = { 0x330, 0x49 };
    std::cout << "Concatenation of the two decomposed strings { ";
    boost::copy(cat_dec1, output);
    std::cout << "} and { ";
    boost::copy(cat_dec2, output);
    std::cout << "}: ";
    unicode::decomposed_concat(cat_dec1, cat_dec2, output);
    std::cout << std::endl;
    
    char32 cat_comp1[] = { 0x48, 0x1e17 };
    char32 cat_comp2[] = { 0x330, 0x49 };
    std::cout << "Concatenation of the two composed strings { ";
    boost::copy(cat_comp1, output);
    std::cout << "} and { ";
    boost::copy(cat_comp2, output);
    std::cout << "}: ";
    unicode::composed_concat(cat_comp1, cat_comp2, output);
    std::cout << std::endl;
}
//]
