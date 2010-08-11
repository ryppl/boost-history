#ifndef BOOST_STRING_FINDER_ALIASES_HPP
#define BOOST_STRING_FINDER_ALIASES_HPP

#include <boost/algorithm/string/finder/finder.hpp>
#include <boost/algorithm/string/string_search.hpp>

namespace boost { namespace algorithm {
    //Naive Search
    typedef boost::algorithm::finder_t<std::string, std::string,
        boost::algorithm::naive_search> naive_search_finder;
    typedef boost::algorithm::finder_t<std::wstring, std::wstring,
        boost::algorithm::naive_search> wnaive_search_finder;

    //Knuth-Morris-Pratt
    typedef boost::algorithm::finder_t<std::string, std::string,
        boost::algorithm::knuth_morris_pratt> knuth_morris_pratt_finder;
    typedef boost::algorithm::finder_t<std::wstring, std::wstring,
        boost::algorithm::knuth_morris_pratt> wknuth_morris_pratt_finder;

    //Boyer Moore
    typedef boost::algorithm::finder_t<std::string, std::string,
        boost::algorithm::boyer_moore> boyer_moore_finder;
    typedef boost::algorithm::finder_t<std::wstring, std::wstring,
        boost::algorithm::boyer_moore> wboyer_moore_finder;

    //Rabin Karp
    typedef boost::algorithm::finder_t<std::string, std::string,
        boost::algorithm::rabin_karp32> rabin_karp32_finder;
    typedef boost::algorithm::finder_t<std::wstring, std::wstring,
        boost::algorithm::rabin_karp32> wrabin_karp32_finder;

    typedef boost::algorithm::finder_t<std::string, std::string,
        boost::algorithm::rabin_karp64> rabin_karp64_finder;
    typedef boost::algorithm::finder_t<std::wstring, std::wstring,
        boost::algorithm::rabin_karp64> wrabin_karp64_finder;

    //Suffix array
    typedef boost::algorithm::finder_t<std::string, std::string,
        boost::algorithm::suffix_array_search> suffix_array_finder;
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
