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
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/string_search/detail/rabin_karp.hpp>
#include <boost/algorithm/string/finder/detail/finder_typedefs.hpp>
#include <cassert>
#include <limits>
#include <boost/type_traits/is_same.hpp>

/*!
    \file
    Implements the Rabin-Karp string search algorithm
*/

namespace boost { namespace algorithm {

    //todo Make it work with case insensitive. Find a way to allow providing locales.

    //TODO: Implement a version that works with Input iterators
    //TODO: Make sure this only works with integral char types
    //! A generic implementation of the string search algorithm Rabin-Karp
    //! \warning This algorithm is approximate, meaning it can yield false positives (but not false negatives).
    //!         In order to turn it into an exact matching algorithm, an extra equality check
    //!         needs to be performed after a match has occurred.
    //! \warning This algorithm is limited to using integral character types with
    //!             the equality comparator only (this is because "rolling" hash
    //!             functions need to be computed on the pattern and portions of text)
    template <
        class HashType,
        HashType FirstBase, HashType FirstModulo,
        HashType SecondBase, HashType SecondModulo>
    struct rabin_karp_algorithm
    {

        template <class Finder, class Range1T, class Range2T, class ComparatorT, class AllocatorT>
        class algorithm;

        template <class Finder, class Range1T, class Range2T, class AllocatorT>
        class algorithm<Finder, Range1T, Range2T, boost::algorithm::is_equal, AllocatorT>
            : public ::boost::algorithm::detail::rabin_karp_algorithm<Finder,
                Range1T, Range2T, HashType, FirstBase, FirstModulo, SecondBase, SecondModulo>
        {
        public:
            std::string get_algorithm_name () const
            { return "Rabin-Karp (" + boost::lexical_cast<std::string>(sizeof(HashType)) + ")"; }
		protected:
            //construct the algorithm given iterator ranges for the substring and the string
            algorithm () {
                //todo add more assertions here
                BOOST_STATIC_ASSERT((
                    sizeof(boost::range_value<Range1T>::type)*2 <= sizeof(HashType)
                ));
                BOOST_STATIC_ASSERT(( boost::is_same<substring_char_type,string_char_type>::value ));
                assert_overflow(FirstBase, FirstModulo);
                assert_overflow(SecondBase, SecondModulo);
            }
        private:
            inline void assert_overflow(HashType B, HashType M)
            {
                //todo fix this
                //char_range_size = CHAR_MAX-CHAR_MIN+1
                /*static const boost::uintmax_t char_range_size =
                    BOOST_ALGORITHM_DETAIL_ASSERTED_ADD(BOOST_ALGORITHM_DETAIL_ASSERTED_SUBSTRACT(
                    (boost::uintmax_t)std::numeric_limits<substring_char_type>::max(),
                    (boost::uintmax_t)std::numeric_limits<substring_char_type>::min(), HashType
                    ), 1, HashType);
                //(M-1)*b + X + (M1-1)*X <= MAX(HashType)
                BOOST_ALGORITHM_DETAIL_ASSERTED_ADD(
                    BOOST_ALGORITHM_DETAIL_ASSERTED_ADD(
                        BOOST_ALGORITHM_DETAIL_ASSERTED_MULTIPLY(M-1, B, HashType), char_range_size, HashType),
                    BOOST_ALGORITHM_DETAIL_ASSERTED_MULTIPLY(M-1,char_range_size, HashType), HashType );*/
            }
        };
    };


    //todo try to find better pairs for better efficiency?
    //! An implementation of a 32bit version of Rabin-Karp
    typedef rabin_karp_algorithm<boost::uint_fast32_t,257,64433,277,57923> rabin_karp32;
    //1/150167080229379589 odds of collision. useful with wchar_t
    //! An implementation of a 64bit version of Rabin-Karp
    typedef rabin_karp_algorithm<boost::uint64_t,337515847,373587883,255150899,401959183> rabin_karp64;
    //! Instances of this type can be passed to find functions to require them to
    //!     use the predefined 32bit Rabin-Karp algorithm.
    struct rabin_karp32_tag { typedef boost::algorithm::rabin_karp32 type; };
    //! Instances of this type can be passed to find functions to require them to
    //!     use the predefined 64bit Rabin-Karp algorithm.
    struct rabin_karp64_tag { typedef boost::algorithm::rabin_karp64 type; };
} } // namespace algorithm, namespace boost

namespace boost
{
    using boost::algorithm::rabin_karp32;
    using boost::algorithm::rabin_karp64;
    using boost::algorithm::rabin_karp32_tag;
    using boost::algorithm::rabin_karp64_tag;
}

#endif