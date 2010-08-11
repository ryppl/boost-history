#ifndef BOOST_ALGORITHM_DEFAULT_SEARCH_ALGORITHM_HPP
#define BOOST_ALGORITHM_DEFAULT_SEARCH_ALGORITHM_HPP

#include <boost/algorithm/string/string_search/knuth_morris_pratt.hpp>

namespace boost { namespace algorithm {
    //! The default search algorithm used by find/replace functions.
    typedef boost::algorithm::knuth_morris_pratt default_finder_algorithm;

    //! The tag of the default search algorithm used by find/replace functions.
    //! Instances of this type can be passed to find/replace functions.
    struct default_finder_algorithm_tag { typedef boost::algorithm::knuth_morris_pratt type; };
} }

namespace boost
{
    using algorithm::default_finder_algorithm;
}

#endif
