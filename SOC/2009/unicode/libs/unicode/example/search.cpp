//[ search
/*`
This example shows how to search a substring within a string,
at the grapheme level, using two methods.

In this example we're going to use =BOOST_AUTO= as the
return type of some of the functions is unspecified, but ideally
you should try to avoid that dependency by not naming the variables at all,
or rely on a type deduction system the library doesn't provide yet.
*/

#include <boost/algorithm/string.hpp>
#include <boost/unicode/search.hpp>

#include <boost/typeof/typeof.hpp>
#include <iostream>

#include <boost/unicode/graphemes.hpp>
#include <boost/unicode/utf.hpp>

namespace unicode = boost::unicode;

int main()
{
/*`
We define the string we're going to search into, "foo<combining circumflex accent>foo"
as well as it's version in terms of graphemes
*/
    char foo[] = "foo\xcc\x82" "foo";
    BOOST_AUTO(foo_bounded, unicode::adaptors::utf_grapheme_segment(boost::as_literal(foo)));
    
//` We do the same thing for the string we're going to look for, "foo"
    char search[] = "foo";
    BOOST_AUTO(search_bounded, unicode::adaptors::utf_grapheme_segment(boost::as_literal(search)));
    
//` We perform the search using the ranges of graphemes, i.e. the [conceptref Segmenter]-based approach:
    BOOST_AUTO(range_segmenter, boost::algorithm::find_first(foo_bounded, search_bounded));
    
//` We perform the search using the original range, but using an adapted Boost.StringAlgo Finder with the relevant [conceptref BoundaryChecker]:
    BOOST_AUTO(finder,
        boost::algorithm::make_boundary_finder(
            boost::algorithm::first_finder(search),
            unicode::utf_grapheme_boundary()
        )
    );
    boost::iterator_range<char*> range_boundary = boost::algorithm::find(foo, finder);
    
//` We now display the resulting matches, which should both be pointing to the second occurrence, with their positions within the original UTF-8 string:
    std::cout << "[" << std::distance(boost::begin(foo), range_segmenter.begin().base()) << ", " << std::distance(boost::begin(foo), range_segmenter.end().base()) << "] ";
    std::cout << range_segmenter << std::endl;
    
    std::cout << "[" << std::distance(boost::begin(foo), range_boundary.begin()) << ", " << std::distance(boost::begin(foo), range_boundary.end()) << "] ";
    std::cout << range_boundary << std::endl;
}
//]
