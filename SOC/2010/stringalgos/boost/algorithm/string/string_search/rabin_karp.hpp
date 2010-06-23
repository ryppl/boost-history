#ifndef BOOST_ALGORITHM_RABIN_KARP_HPP
#define BOOST_ALGORITHM_RABIN_KARP_HPP

#include <boost/cstdint.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/algorithm/string/compare.hpp>
#include <boost/call_traits.hpp>
#include <boost/tuple/tuple.hpp>
#include <cstdlib>

namespace boost { namespace algorithm {

    //! \todo Find this in boost?
    struct void_type {};

    //Note: this only works with comparator ==
    //! \note Implement a version that works with Input iterators?
    /*template <
        //bool Heuristic = true,
        class HashType = boost::uint32_t,
        HashType FirstModulo = 1, HashType FirstBase = 2,
        HashType SecondBase = 3, HashType SecondModulo = 4>
    struct rabin_karp
    {
        template <class ForwardIterator1T,class ForwardIterator2T,class Comparator,class Allocator = boost::algorithm::void_type>
        class algorithm;

        template <class ForwardIterator1T,class ForwardIterator2T,class Allocator>
        class algorithm<ForwardIterator1T, ForwardIterator2T, boost::algorithm::is_equal, Allocator>
        {
		public:
			typedef ForwardIterator1T substring_iterator_type;
			typedef ForwardIterator2T string_iterator_type;
            typedef typename boost::iterator_range<substring_iterator_type> substring_range_type;
            typedef typename boost::iterator_range<string_iterator_type> string_range_type;
			typedef boost::algorithm::is_equal comparator_type;
			typedef Allocator allocator_type;
		protected:
            //construct the algorithm given iterator ranges for the substring and the string
			algorithm (typename boost::call_traits<comparator_type>::param_type comparator,
				typename boost::call_traits<allocator_type>::param_type allocator,
				typename boost::call_traits<substring_range_type>::param_type substring,
				typename boost::call_traits<string_range_type>::param_type string)
				: comparator_(comparator), allocator_(allocator), substring_(substring), string_(string) { }

            string_range_type find(string_iterator_type start)
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
            }

		private:
            bool equal () const
            {
                return first_substring_hash == first_string_hash_ && second_substring_hash_ == second_string_hash_;
            }
            template <class IteratorT>
            inline boost::tuple<HashType,HashType> compute_hashes (IteratorT const &start, IteratorT const &end)
            {
                HashType first = static_cast<HashType>(0), second = static_cast<HashType>(0);
                for (IteratorT it = start; it != end; ++it)
                {
                    first = (first*FirstBase + *it) % FirstModulo;
                    second= (second*SecondBase + *it) % SecondModulo;
                }
                return boost::make_tuple(first, second);
            }

            void roll_string_hash()
            {
                //! \todo impl
            }

            HashType first_substring_hash_, first_string_hash_, first_string_beginning_hash_;
            HashType second_substring_hash_, second_string_hash_ second_string_beginning_hash_;
            string_iterator_type computed_until_;
            std::size_t substring_size_, string_computed_start_offset_, string_computed_end_offset_;
            string_iterator_type string_computed_upto_;
			typename boost::call_traits<comparator_type>::param_type comparator_;
			typename boost::call_traits<allocator_type>::param_type allocator_;
			typename boost::call_traits<substring_range_type> >::param_type substring_;
			typename boost::call_traits<substring_range_type>::param_type string_;
        };
    };
    */

} // namespace algorithm
} // namespace boost

#endif