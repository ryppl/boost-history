//  Boost string_algo library default_search_algorithm.hpp header file  ---------------------------//

//  Copyright Stefan Mihaila 2010
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/ for updates, documentation, and revision history.

#ifndef BOOST_ALGORITHM_DEFAULT_SEARCH_ALGORITHM_HPP
#define BOOST_ALGORITHM_DEFAULT_SEARCH_ALGORITHM_HPP

//#include <boost/algorithm/string/string_search/knuth_morris_pratt.hpp>
#include <boost/algorithm/string/string_search/naive_search.hpp>

/*! \file
    Defines the string search algorithm that is to be used when the algorithm parameter
    is omitted in creating a finder.
 */

namespace boost { namespace algorithm {
    //! The default search algorithm used by find/replace functions.
    typedef boost::algorithm::naive_search default_finder_algorithm;

    //! The tag of the default search algorithm used by find/replace functions.
    //! Instances of this type can be passed to find/replace functions.
    struct default_finder_algorithm_tag { typedef boost::algorithm::naive_search type; };
} }

namespace boost
{
    using algorithm::default_finder_algorithm;
}

#endif
