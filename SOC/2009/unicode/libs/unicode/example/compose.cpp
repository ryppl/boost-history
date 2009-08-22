//[ compose
/*`
Example in development.
*/

#include <boost/unicode/compose.hpp>
#include <boost/foreach.hpp>
#include <boost/range/as_array.hpp>
#include <iostream>
#include <iterator>

#include <boost/unicode/cat.hpp>

#include <boost/range/adaptor/reversed.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/assign/list_of.hpp> 

namespace unicode = boost::unicode;
namespace ucd = unicode::ucd;
using boost::assign::list_of; 
using boost::char32;

int main()
{
    boost::char32 cp = 0x1E17;
    
    std::cout << "Decomposition of U+01E17 within the UCD: ";
    BOOST_FOREACH(boost::char32 c, ucd::get_decomposition(cp))
        std::cout << "0x" << std::hex << c << ' ';
    std::cout << std::endl;
    std::cout << "Decomposition type: " << as_string(ucd::get_decomposition_type(cp)) << std::endl;
    
    boost::char32 baz[] = { cp, 0x330 };
    std::cout << "Canonical decomposition: ";
    std::cout << unicode::composed(unicode::decomposed(baz)) << std::endl;
    std::cout << "reversed: " << boost::make_reversed_range(unicode::composed(unicode::decomposed(baz))) << std::endl;
    std::cout << unicode::normalized(baz) << std::endl;
    std::cout << "reversed: " << boost::make_reversed_range(unicode::normalized(baz));
    std::cout << std::endl << std::endl;
    
    std::cout << "Canonical decomposition of U+00A8: ";
    unicode::decompose(list_of(0xA8), std::ostream_iterator<boost::char32>(std::cout, " "));
    std::cout << std::endl;
    std::cout << "Compatibility decomposition of U+00A8: ";
    unicode::decompose(list_of(0xA8), std::ostream_iterator<boost::char32>(std::cout, " "), UINT_MAX);
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
    std::cout << std::endl << std::endl;
    
    //unicode::decomposed_concat(list_of<char32>(0x48)(0x65)(0x304)(0x301), list_of<char32>(0x330)(0x49), std::ostream_iterator<boost::char32>(std::cout, " "));
    std::cout << std::endl;
    
    boost::char32 tmp[] = {0x48, 0x1e17};
    boost::iterator_range<
        boost::pipe_iterator<
            boost::char32*,
            unicode::decomposer
        >
    > r = unicode::decomposed(tmp);
    
    std::cout << "Distance: " << std::distance(
        begin(r),
        prior(end(r))
    ) << std::endl;
    
    boost::char32 tmp2[] = {0x330, 0x49};
    unicode::composed_concat(list_of<char32>(0x48)(0x1e17), list_of<char32>(0x330)(0x49), std::ostream_iterator<boost::char32>(std::cout, " "));
    std::cout << std::endl;
    
    std::cout << unicode::composed_concated(tmp, tmp2) << std::endl;
}
//]
