//[ compose
/*`
Example in development.
*/

#include <boost/unicode/compose.hpp>
#include <boost/foreach.hpp>
#include <boost/range/as_array.hpp>
#include <iostream>
#include <iterator>

namespace unicode = boost::unicode;
namespace ucd = unicode::ucd;

namespace boost
{
    
template<typename T>
iterator_range<T*> list_of(T& t)
{
    return make_iterator_range(&t, &t+1);
}

template<typename T>
iterator_range<const T*> list_of(const T& t)
{
    return make_iterator_range(&t, &t+1);
}

}

int main()
{
    boost::char32 cp = 0x1E17;
    
    std::cout << "Decomposition of U+01E17 within the UCD: ";
    BOOST_FOREACH(boost::char32 c, ucd::get_decomposition(cp))
        std::cout << "0x" << std::hex << c << ' ';
    std::cout << std::endl;
    std::cout << "Decomposition type: " << as_string(ucd::get_decomposition_type(cp)) << std::endl;
    
    std::cout << "Canonical decomposition: ";
    unicode::decompose(boost::list_of(cp), std::ostream_iterator<boost::char32>(std::cout, " "));
    std::cout << std::endl << std::endl;
    
    std::cout << "Canonical decomposition of U+00A8: ";
    unicode::decompose(boost::list_of(0xA8), std::ostream_iterator<boost::char32>(std::cout, " "));
    std::cout << std::endl;
    std::cout << "Compatibility decomposition of U+00A8: ";
    unicode::decompose(boost::list_of(0xA8), std::ostream_iterator<boost::char32>(std::cout, " "), UINT_MAX);
    std::cout << std::endl;
    std::cout << std::endl;
    
    boost::char32 foo[] = { 0x113, 0x301 };
    std::cout << "Canonical composition of { " << boost::as_array(foo) << " }: ";
    unicode::compose(foo, std::ostream_iterator<boost::char32>(std::cout, " "));
    std::cout << std::endl;
    
    boost::char32 foo2[] = { 0x65, 0x304, 0x301 };
    std::cout << "Canonical composition of { " << boost::as_array(foo2) << " }: ";
    unicode::compose(foo2, std::ostream_iterator<boost::char32>(std::cout, " "));
    std::cout << std::endl;
    
    boost::char32 bar[] = { 0x20, 0x308 };
    std::cout << "Canonical composition of { " << boost::as_array(bar) << " }: ";
    unicode::compose(bar, std::ostream_iterator<boost::char32>(std::cout, " "));
    std::cout << std::endl;
}
//]
