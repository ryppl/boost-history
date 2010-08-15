//  Boost string_algo library suffix_array.hpp header file  ---------------------------//

//  Copyright Stefan Mihaila 2010.
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/ for updates, documentation, and revision history.

#ifndef BOOST_ALGORITHM_SUFFIX_ARRAY_HPP
#define BOOST_ALGORITHM_SUFFIX_ARRAY_HPP

#include <iterator>
#include <vector>
#include <algorithm>
#include <string>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/range/algorithm/sort.hpp>
#include <boost/range/category.hpp>

#include <boost/algorithm/string/compare.hpp>
#include <boost/algorithm/string/finder/detail/finder_typedefs.hpp>
#include <boost/algorithm/string/finder/detail/string_search_ranges.hpp>

/*!
    \file
    Implements a string search algorithm based on the suffix array data structure
*/

namespace boost { namespace algorithm {
    //! An implementation of a string search algorithm using the suffix array data structure
    //! \warning This algorithm can only work with the equality comparator (boost::algorithm::is_equal)
    struct suffix_array_search
    {


        template <class Range1CharT, class Range2CharT, class ComparatorT, class AllocatorT>
        class algorithm;
        template <class Range1CharT, class Range2CharT, class AllocatorT>
        class algorithm<Range1CharT, Range2CharT, boost::algorithm::is_equal, AllocatorT>
        {
        private:
            typedef Range1CharT substring_char_type;
            typedef Range2CharT string_char_type;
            typedef boost::algorithm::is_equal comparator_type;
            typedef AllocatorT allocator_type;
        public:
            std::string get_algorithm_name () const { return "Suffix array"; }

            algorithm (comparator_type const &comp, allocator_type const &alloc)
                : comp_(comp), alloc_(alloc), found_matches_(false), pos_(alloc_), matches_(alloc_) { }
            

            // no precomputation done on substring
            template <class Range1T, class Range2T> void on_substring_change(
                BOOST_STRING_TYPENAME boost::algorithm::detail::string_search_ranges<Range1T, Range2T> const &)
            { on_substring_change(BOOST_STRING_TYPENAME boost::range_category<Range1T>::type()); }
            
            template <class Range1T, class Range2T>
            inline void on_string_change(BOOST_STRING_TYPENAME boost::algorithm::detail::string_search_ranges<Range1T, Range2T> const &ranges)
            { on_string_change(ranges, BOOST_STRING_TYPENAME boost::range_category<Range2T>::type()); }

            template <class Range1T, class Range2T>
            inline BOOST_STRING_TYPENAME boost::iterator_range<BOOST_STRING_TYPENAME boost::range_iterator<Range2T>::type>
                find(BOOST_STRING_TYPENAME boost::algorithm::detail::string_search_ranges<Range1T, Range2T> const &ranges)
            {
                BOOST_ALGORITHM_DETAIL_COMMON_FINDER_TYPEDEFS(Range1T, Range2T);

                string_range_type const &str = ranges.str;
                substring_range_type const &substr = ranges.substr;
                string_iterator_type start = ranges.offset;

                std::size_t start_offset = start - boost::begin(str),
                    substr_size = boost::end(substr) - boost::begin(substr),
                    str_size = boost::end(str) - boost::begin(str);

                if (found_matches_)
                    return find_first_match(str, substr, start_offset);
                
                if (boost::begin(str) == boost::end(str)) return string_range_type(boost::end(str), boost::end(str));
                
                std::size_t suffix_left, suffix_right;
                std::size_t /*firstsuffix_end = pos_[0] + substr_size, */lastsuffix_end = pos_.back()+substr_size;
                //the end position of the smallest lexicographic suffix
                //if (firstsuffix_end > str.size()) firstsuffix_end = str.size();
                //the end position of the biggest lexicographic suffix
                if (lastsuffix_end > str_size) lastsuffix_end = str_size;
                //the substring is smaller than the smallest lexicographic suffix, therefore no matches
                //if (std::lexicographical_compare(substr.begin(), substr.end(),str.begin()+pos[0],str.begin()+firstsuffix_end))
                
                // TODO Is this really correct? Just because it starts before other suffix it does not mean there are no matches
                if (suffix_less(substr, str, 0) ||
                    std::lexicographical_compare(str.begin()+pos_.back(),str.begin()+lastsuffix_end,substr.begin(),substr.end())
                    )
                {
                    found_matches_ = true;
                    matches_.clear();
                    return string_range_type( boost::end(str), boost::end(str) );
                }
                //the substring is bigger than the biggest lexicographic suffix, therefore no matches
                //else if ()
                //    return;
                //perform binary search in the array of suffixes, find a range of matching suffixes
                else
                {
                    std::size_t left = 0, right=str_size, middle, result = 0;
                    while (right > left) // nonsingular range [left,right)
                    {
                        middle = (left+right)/2;
                        //does the substring match the prefix of the suffix `middle'?
                        //if (std::equal(substr.begin(), substr.end(), str.begin()+pos[middle]))
                        if (suffix_equal(substr, str, middle))
                        { result = middle; break; }
                        //is the substring lexicographically less than the prefix of the suffix `middle'?
                        else if (suffix_less(substr, str, middle))
                        //else if (std::lexicographical_compare(substr.begin(), substr.end(),
                        //    str.begin()+pos[middle], str.end()))
                        { right = middle; }
                        //it must be greater.
                        else { left = middle+1; }
                    }
                    //empty interval, no matches
                    if (left == right) return string_range_type(boost::end(str), boost::end(str));
                    

                    //suffix_left = lower bound of the matches
                    if (result > 0)
                    {
                        suffix_left = result;
                        //while (suffix_left >= 1 && std::equal(substr.begin(), substr.end(), str.begin()+pos[suffix_left-1]))
                        while (suffix_left >= 1 && suffix_equal(substr, str, suffix_left-1))
                            --suffix_left;
                        //++suffix_left;
                    } else suffix_left = result;

                    //suffix_right = upper bound of the matches
                    suffix_right = result+1;
                    //while (suffix_right < str.size() && std::equal(substr.begin(), substr.end(), str.begin()+pos[suffix_right]))
                    while (suffix_right < str_size && suffix_equal(substr, str, suffix_right))
                        ++suffix_right;

                    //there are (suffix_right - suffix_left) matches, corresponding to the positions:
                    // { pos[k] | k \in [suffix_left,suffix_right) }
                    found_matches_ = true;
                    matches_.clear();
                    matches_.reserve(suffix_right - suffix_left);
                    for (std::size_t k = suffix_left; k < suffix_right; ++k)
                        matches_.push_back(pos_[k]);
                    boost::sort(matches_);
                    return find_first_match(str, substr, start_offset);
                }
            }
        private:
            template <class StrT, class SubstrT>
            inline StrT find_first_match (StrT const &str, SubstrT const &substr, std::size_t start_offset)
            {
                //string_range_type const &str = static_cast<Finder*>(this)->get_string_range();
                //substring_range_type const &substr = static_cast<Finder*>(this)->get_substring_range();

                BOOST_STRING_TYPENAME std::vector<std::size_t, allocator_type>::const_iterator first_match = 
                            std::lower_bound(matches_.begin(), matches_.end(), start_offset);
                if (first_match == matches_.end())
                    return StrT(
                        boost::end(str), boost::end(str));
                std::size_t substr_size = boost::end(substr) - boost::begin(substr);
                assert(*first_match + substr_size <= static_cast<std::size_t>(str.size()) );
                return StrT(
                    boost::begin(str) + (*first_match),
                    boost::begin(str) + (*first_match) + substr_size
                    );
            }
            void on_substring_change(std::random_access_iterator_tag)
            {
                found_matches_ = false;
            }

            template <class Range1T, class Range2T>
            void on_string_change(BOOST_STRING_TYPENAME boost::algorithm::detail::string_search_ranges<Range1T, Range2T> const &ranges,
                std::random_access_iterator_tag)
            {
                BOOST_ALGORITHM_DETAIL_COMMON_FINDER_TYPEDEFS(Range1T, Range2T);

                string_range_type const &str = ranges.str;

                // compute the suffix array
                std::size_t str_size = boost::end(str) - boost::begin(str);
                pos_.clear();
                pos_.reserve(str_size);
                std::generate_n(std::back_inserter(pos_), str_size, increasing_generator());
                std::sort(pos_.begin(), pos_.end(), suffix_array_sort_comparator<string_range_type>(str));

                found_matches_ = false;
            }

            template <class SubstrT, class StrT>
            inline bool suffix_less(SubstrT const &substr,
                StrT const &str, std::size_t suffix)
            {
                std::size_t start_offset = pos_[suffix],
                    end_offset = start_offset + (boost::end(substr) - boost::begin(substr)),
                    str_size = boost::end(str) - boost::begin(str);
                if (end_offset > str_size) end_offset = str_size;
                return std::lexicographical_compare(
                    boost::begin(substr), boost::end(substr),
                    boost::begin(str) + start_offset, boost::begin(str) + end_offset);
            }

            template <class SubstrT, class StrT>
            inline bool suffix_equal(SubstrT const &substr,
                StrT const &str, std::size_t suffix)
            {
                std::size_t start_offset = pos_[suffix],
                    end_offset = start_offset + (boost::end(substr) - boost::begin(substr)),
                    str_size = boost::end(str) - boost::begin(str);
                //the substr is longer than the suffix, they can't be equal.
                if (end_offset > str_size) return false;
                return std::equal(substr.begin(), substr.end(), boost::begin(str)+start_offset);
            }

            struct increasing_generator
            {
                increasing_generator () : idx_(0) { }
                inline std::size_t operator()() { return idx_++; }
                std::size_t idx_;
            };

            template <class Range2T>
            struct suffix_array_sort_comparator
            {
                suffix_array_sort_comparator (Range2T const &str) : str_(str) { }
                bool operator()(std::size_t lhs, std::size_t rhs)
                {
                    return std::lexicographical_compare(str_.begin()+lhs, str_.end(), str_.begin()+rhs, str_.end());
                }
                Range2T const &str_;
            };

            comparator_type comp_;
            allocator_type alloc_;
            bool found_matches_;
            std::vector<std::size_t, BOOST_STRING_TYPENAME allocator_type::template rebind<std::size_t>::other> pos_;
            std::vector<std::size_t, BOOST_STRING_TYPENAME allocator_type::template rebind<std::size_t>::other> matches_;


        };
    };
    //! Instances of this type can be passed to find functions to require them to
    //!     use the Suffix Array Search algorithm.
    struct suffix_array_search_tag { typedef boost::algorithm::suffix_array_search type; };
} }

namespace boost
{
    using boost::algorithm::suffix_array_search;
    using boost::algorithm::suffix_array_search_tag;
}

#endif
