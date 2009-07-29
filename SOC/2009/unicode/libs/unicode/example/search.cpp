//[ search
/*`
Example in development.
*/

#include <boost/algorithm/string.hpp>
#include <boost/unicode/search.hpp>

#include <boost/foreach.hpp>
#include <boost/foreach_auto.hpp>
#include <iostream>

#include <boost/unicode/graphemes.hpp>
#include <boost/unicode/utf.hpp>

namespace unicode = boost::unicode;

int main()
{
    char foo[] = "foo\xcc\x82" "foo";
    //BOOST_AUTO(foo, unicode::utf_grapheme_bounded(foo_));
    
    char search[] = "foo";
    
    //BOOST_AUTO(s, unicode::utf_grapheme_bounded(boost::as_literal(search)));
    BOOST_AUTO(s, boost::as_literal(search));
    
    // Boost.StringAlgo
    BOOST_AUTO(finder,
        /*boost::algorithm::make_boundary_finder(*/
            boost::algorithm::first_finder(s)/*,
            unicode::utf_grapheme_boundary()
        )*/
    );
    
    BOOST_AUTO(f, unicode::make_boundary_finder(
        unicode::make_simple_finder(s),
        unicode::utf_grapheme_boundary()
    )); 
    
    BOOST_AUTO(range, f.ltr(boost::begin(foo), boost::end(foo)));
    //BOOST_AUTO(range, boost::algorithm::find(foo, finder));
    
    std::cout << "[" << std::distance(boost::begin(foo), range.begin()) << ", " << std::distance(boost::begin(foo), range.end()) << "] ";
        
    BOOST_FOREACH_AUTO(r, range)
        std::cout << r;
    std::cout << std::endl;
}
//]
