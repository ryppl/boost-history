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


#include <boost/algorithm/string/string_search/detail/rabin_karp.hpp>

namespace boost { namespace algorithm {

    //! \todo Find this in boost?
    //struct void_type {};

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
                BOOST_STATIC_ASSERT(
                    sizeof(boost::iterator_value<Iterator1T>::type)*2 <= sizeof(HashType)
                );
            }
            /*string_range_type find(string_iterator_type start)
            {
                if (start == boost::begin(string) && string_computed_start_offset_) on_string_change();

            }

            void on_substring_change()
            {
                first_substring_hash_ = static_cast<HashType>(0);
                second_substring_hash_ = static_cast<HashType>(0);
                substring_size_ = 0;
                for (substring_iterator_type it = boost::begin(substring_); it != boost::end(substring_); ++it)
                {
                    substring_size_++;
                    first_substring_hash_  = (first_substring_hash_*FirstBase + *it) % FirstModulo;
                    second_substring_hash_ = (second_substring_hash_*SecondBase + *it) % SecondModulo;
                }
                if (string_computed_start_offset_ > 0) on_string_change();
            }

            void on_string_change()
            {
                first_string_hash_ = static_cast<HashType>(0);
                second_string_hash_ = static_cast<HashType>(0);
                string_computed_start_offset_ = 0;
                string_computed_end_offset_ = 0;
                for (string_iterator_type it = boost::begin(string_);
                    it != boost::end(string_) && string_computed_end_offset_ < substring_size; ++it)
                {
                    ++string_computed_end_offset_;
                    first_string_hash_ = (first_string_hash_ * FirstBase + *it) % FirstModulo;
                    second_string_hash_ = (second_string_hash_ * SecondBase + *it) % SecondModulo;
                }

                //string_computed_upto = 
            }*/


			
        };
    };

    //1/3732152659 odds of collision. used with char
    typedef rabin_karp_algorithm<boost::uint32_t,257,64433,277,57923> rabin_karp32;
    //1/(N*M) odds of collision. used with wchar_t
    typedef rabin_karp_algorithm<boost::uint64_t,1,2,3,4> rabin_karp64;

} // namespace algorithm
} // namespace boost


#endif