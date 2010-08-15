//  Boost string_algo library string_search_ranges.hpp header file  ---------------------------//

//  Copyright Stefan Mihaila 2010.
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/ for updates, documentation, and revision history.

#ifndef BOOST_ALGORITHM_DETAIL_STRING_SEARCH_RANGES_HPP
#define BOOST_ALGORITHM_DETAIL_STRING_SEARCH_RANGES_HPP

#include <boost/algorithm/string/finder/detail/finder_typedefs.hpp>
#include <boost/range/begin.hpp>

namespace boost { namespace algorithm { namespace detail {

    template <class Range1T, class Range2T>
    class string_search_ranges
    {
    private:
        BOOST_ALGORITHM_DETAIL_FINDER_TYPEDEFS(Range1T, Range2T);
    public:	
        boost::iterator_range<substring_iterator_type> substr;
        boost::iterator_range<string_iterator_type> str;
        string_iterator_type offset;
        string_search_ranges() { }
        string_search_ranges (substring_range_type const &substr, string_range_type const &str)
            : substr(substr), str(str), offset(boost::begin(str)) { }
    };

} } }

#endif // BOOST_ALGORITHM_DETAIL_STRING_SEARCH_RANGES_HPP
