#ifndef BOOST_ALGORITHM_RABIN_KARP_HPP
#define BOOST_ALGORITHM_RABIN_KARP_HPP

#include <boost/cstdint.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/algorithm/string/compare.hpp>
#include <boost/call_traits.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/mpl/void.hpp>
#include <cstdlib>
#include <boost/static_assert.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <string>
#include <boost/algorithm/string/finder.hpp>


#include <boost/algorithm/string/string_search/detail/rabin_karp.hpp>

namespace boost { namespace algorithm {

    //!\todo document the fact that it's approximate, how to make it deterministic
    //      the limited comparators
    //!\todo Make it work with case insensitive. Find a way to allow providing locales.

    //Note: this only works with comparator ==
    //! \todo Implement a version that works with Input iterators?
    //! \todo Make sure this only works with char and wchar_t
    template <
        //bool Heuristic = true,
        class HashType,
        HashType FirstBase, HashType FirstModulo,
        HashType SecondBase, HashType SecondModulo>
    struct rabin_karp_algorithm
    {
        typedef boost::mpl::void_ default_allocator_type;

        template <class Finder, class Iterator1T, class Iterator2T, class ComparatorT, class AllocatorT>
        class algorithm;

        template <class Finder, class Iterator1T, class Iterator2T, class AllocatorT>
        class algorithm<Finder, Iterator1T, Iterator2T, boost::algorithm::is_equal, AllocatorT>
            : public ::boost::algorithm::detail::rabin_karp_algorithm<Finder,
                Iterator1T, Iterator2T, HashType,
                FirstBase, FirstModulo, SecondBase, SecondModulo>
        {
		protected:
            //construct the algorithm given iterator ranges for the substring and the string
            algorithm () {
                //!\todo add more assertions here
                BOOST_STATIC_ASSERT(
                    sizeof(boost::iterator_value<Iterator1T>::type)*2 <= sizeof(HashType)
                );
            }
	
        };
    };

    //1/3732152659 odds of collision. useful with char
    typedef rabin_karp_algorithm<boost::uint32_t,257,64433,277,57923> rabin_karp32;
    //1/150167080229379589 odds of collision. useful with wchar_t
    typedef rabin_karp_algorithm<boost::uint64_t,337515847,373587883,255150899,401959183> rabin_karp64;

} } // namespace algorithm, namespace boost

namespace boost
{
    using boost::algorithm::rabin_karp32;
    using boost::algorithm::rabin_karp64;
    
    typedef boost::algorithm::finder_t<std::string, std::string,
        boost::algorithm::rabin_karp32> rabin_karp32_finder;
    typedef boost::algorithm::finder_t<std::wstring, std::wstring,
        boost::algorithm::rabin_karp32> wrabin_karp32_finder;

    typedef boost::algorithm::finder_t<std::string, std::string,
        boost::algorithm::rabin_karp64> rabin_karp64_finder;
    typedef boost::algorithm::finder_t<std::wstring, std::wstring,
        boost::algorithm::rabin_karp64> wrabin_karp64_finder;
}

#endif