//  Boost string_algo library finder_aliases.hpp header file  ---------------------------//

//  Copyright Stefan Mihaila 2010.
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/ for updates, documentation, and revision history.

#ifndef BOOST_STRING_FINDER_ALIASES_HPP
#define BOOST_STRING_FINDER_ALIASES_HPP

/*! \file
    Defines useful typedefs for a easier creation of \ref finder_t finders.
*/

#include <boost/algorithm/string/finder/finder.hpp>
#include <boost/algorithm/string/string_search.hpp>

namespace boost { namespace algorithm {

    //Naive Search

    //! A template class of \ref finder_t using std::strings as types for pattern and text
    //!     and naive search as the string search algorithm.
    typedef boost::algorithm::finder_t<std::string, std::string,
        boost::algorithm::naive_search> naive_search_finder;
    //! A template class of \ref finder_t using std::wstrings as types for pattern and text
    //!     and naive search as the string search algorithm.
    typedef boost::algorithm::finder_t<std::wstring, std::wstring,
        boost::algorithm::naive_search> wnaive_search_finder;

    
    //Knuth-Morris-Pratt
    //! A template class of \ref finder_t using std::strings as types for pattern and text
    //!     and Knuth-Morris-Pratt as the string search algorithm.
    typedef boost::algorithm::finder_t<std::string, std::string,
        boost::algorithm::knuth_morris_pratt> knuth_morris_pratt_finder;
    //! A template class of \ref finder_t using std::wstrings as types for pattern and text
    //!     and Knuth-Morris-Pratt as the string search algorithm.
    typedef boost::algorithm::finder_t<std::wstring, std::wstring,
        boost::algorithm::knuth_morris_pratt> wknuth_morris_pratt_finder;


    //Boyer Moore
    //! A template class of \ref finder_t using std::strings as types for pattern and text
    //!     and Boyer-Moore as the string search algorithm.
    typedef boost::algorithm::finder_t<std::string, std::string,
        boost::algorithm::boyer_moore> boyer_moore_finder;
    //! A template class of \ref finder_t using std::wstrings as types for pattern and text
    //!     and Boyer-Moore as the string search algorithm.
    typedef boost::algorithm::finder_t<std::wstring, std::wstring,
        boost::algorithm::boyer_moore> wboyer_moore_finder;


    //Rabin Karp
    //! A template class of \ref finder_t using std::strings as types for pattern and text
    //!     and 32bit Rabin-Karp as the string search algorithm.
    typedef boost::algorithm::finder_t<std::string, std::string,
        boost::algorithm::rabin_karp32> rabin_karp32_finder;
    //! A template class of \ref finder_t using std::wstrings as types for pattern and text
    //!     and 32bit Rabin-Karp as the string search algorithm.
    typedef boost::algorithm::finder_t<std::wstring, std::wstring,
        boost::algorithm::rabin_karp32> wrabin_karp32_finder;

    //! A template class of \ref finder_t using std::strings as types for pattern and text
    //!     and 64bit Rabin-Karp as the string search algorithm.
    typedef boost::algorithm::finder_t<std::string, std::string,
        boost::algorithm::rabin_karp64> rabin_karp64_finder;
    //! A template class of \ref finder_t using std::wstrings as types for pattern and text
    //!     and 64bit Rabin-Karp as the string search algorithm.
    typedef boost::algorithm::finder_t<std::wstring, std::wstring,
        boost::algorithm::rabin_karp64> wrabin_karp64_finder;


    //Suffix array
    //! A template class of \ref finder_t using std::strings as types for pattern and text
    //!     and suffix array search as the string search algorithm.
    typedef boost::algorithm::finder_t<std::string, std::string,
        boost::algorithm::suffix_array_search> suffix_array_finder;
    //! A template class of \ref finder_t using std::wstrings as types for pattern and text
    //!     and suffix array search as the string search algorithm.
    typedef boost::algorithm::finder_t<std::wstring, std::wstring,
        boost::algorithm::suffix_array_search> wsuffix_array_finder;

} } // namespace algorithm, namespace boost

namespace boost
{
    using algorithm::naive_search_finder; using algorithm::wnaive_search_finder;
    using algorithm::knuth_morris_pratt_finder; using algorithm::wknuth_morris_pratt_finder;
    using algorithm::boyer_moore_finder; using algorithm::wboyer_moore_finder;
    using algorithm::rabin_karp32_finder; using algorithm::wrabin_karp32_finder;
    using algorithm::rabin_karp64_finder; using algorithm::wrabin_karp64_finder;
    using algorithm::suffix_array_finder; using algorithm::wsuffix_array_finder;
}

#endif
