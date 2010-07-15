#ifndef BOOST_ALGORITHM_SUFFIX_ARRAY_HPP
#define BOOST_ALGORITHM_SUFFIX_ARRAY_HPP

#include <iterator>
#include <vector>
#include <algorithm>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/range/algorithm/sort.hpp>

namespace boost { namespace algorithm {
    struct suffix_array_search
    {
        typedef std::allocator<std::size_t> default_allocator_type;

        //! \TODO this currently only works for boost::algorithm::is_equal as comparator because we don't yet have a template
        //!         parameter for LessThanComparator. Maybe we should pass two comparators, give it some thought.
        template <class Finder,class RandomAccessIterator1T,
            class RandomAccessIterator2T,class Comparator,class Allocator>
        class algorithm
        {
        public:
            typedef RandomAccessIterator1T substring_iterator_type;
	        typedef RandomAccessIterator2T string_iterator_type;
            typedef typename
                std::iterator_traits<substring_iterator_type>::value_type substring_char_type;
            typedef typename std::iterator_traits<string_iterator_type>::value_type string_char_type;
            typedef typename boost::iterator_range<substring_iterator_type> substring_range_type;
            typedef typename boost::iterator_range<string_iterator_type> string_range_type;
            typedef Comparator comparator_type;
            typedef Allocator allocator_type;
        protected:
            algorithm () : found_matches_(false), pos_(), matches_() { }
            
            void on_substring_change()
            { on_substring_change(std::iterator_traits<substring_iterator_type>::iterator_category()); }
            
            void on_string_change()
            { on_string_change(std::iterator_traits<string_iterator_type>::iterator_category()); }

            string_range_type find (string_iterator_type start)
            {
                substring_range_type const &substr = static_cast<Finder*>(this)->get_substring_range();
                string_range_type const &str = static_cast<Finder*>(this)->get_string_range();
                comparator_type const &comp = static_cast<Finder*>(this)->get_comparator();

                std::size_t start_offset = start - boost::begin(str),
                    substr_size = boost::end(substr) - boost::begin(substr),
                    str_size = boost::end(str) - boost::begin(str);

                if (found_matches_)
                    return find_first_match(start_offset);
                
                if (boost::begin(str) == boost::end(str)) return string_range_type(boost::end(str), boost::end(str));
                
                std::size_t suffix_left, suffix_right;
                std::size_t firstsuffix_end = pos_[0] + substr_size, lastsuffix_end = pos_.back()+substr_size;
                //the end position of the smallest lexicographic suffix
                //if (firstsuffix_end > str.size()) firstsuffix_end = str.size();
                //the end position of the biggest lexicographic suffix
                if (lastsuffix_end > str_size) lastsuffix_end = str_size;
                //the substring is smaller than the smallest lexicographic suffix, therefore no matches
                //if (std::lexicographical_compare(substr.begin(), substr.end(),str.begin()+pos[0],str.begin()+firstsuffix_end))
                
                //! \TODO Is this really correct? Just because it starts before other suffix it does not mean there are no matches
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
                    return find_first_match(start_offset);
                }
            }
        private:
            inline string_range_type find_first_match (std::size_t start_offset)
            {
                string_range_type const &str = static_cast<Finder*>(this)->get_string_range();
                substring_range_type const &substr = static_cast<Finder*>(this)->get_substring_range();

                std::vector<std::size_t, Allocator>::const_iterator first_match = 
                            std::lower_bound(matches_.begin(), matches_.end(), start_offset);
                if (first_match == matches_.end())
                    return string_range_type(
                        boost::end(str), boost::end(str));
                std::size_t substr_size = boost::end(substr) - boost::begin(substr);
                assert(*first_match + substr_size <= static_cast<std::size_t>(str.size()) );
                return string_range_type(
                    boost::begin(str) + (*first_match),
                    boost::begin(str) + (*first_match) + substr_size
                    );
            }
            void on_substring_change(std::random_access_iterator_tag)
            {
                found_matches_ = false;
            }

            void on_string_change(std::random_access_iterator_tag)
            {
                string_range_type const &str = static_cast<Finder*>(this)->get_string_range();

                // compute the suffix array
                std::size_t str_size = boost::end(str) - boost::begin(str);
                pos_.clear();
                pos_.reserve(str_size);
                std::generate_n(std::back_inserter(pos_), str_size, increasing_generator());
                std::sort(pos_.begin(), pos_.end(), suffix_array_sort_comparator(str));

                found_matches_ = false;
            }

            inline bool suffix_less(substring_range_type const &substr,
                string_range_type const &str, std::size_t suffix)
            {
                std::size_t start_offset = pos_[suffix],
                    end_offset = start_offset + (boost::end(substr) - boost::begin(substr)),
                    str_size = boost::end(str) - boost::begin(str);
                if (end_offset > str_size) end_offset = str_size;
                return std::lexicographical_compare(
                    boost::begin(substr), boost::end(substr),
                    boost::begin(str) + start_offset, boost::begin(str) + end_offset);
            }

            inline bool suffix_equal(substring_range_type const &substr,
                string_range_type const &str, std::size_t suffix)
            {
                std::size_t start_offset = pos_[suffix],
                    end_offset = start_offset + (boost::end(substr) - boost::begin(substr)),
                    str_size = boost::end(str) - boost::begin(str);
                //the substr is longer than the suffix, they can't be equal.
                if (end_offset > str_size) return false;
                return std::equal(substr.begin(), substr.end(), boost::begin(str)+start_offset);
            }

            std::vector<std::size_t, Allocator> pos_;
            bool found_matches_;
            std::vector<std::size_t, Allocator> matches_;


            struct increasing_generator
            {
                increasing_generator () : idx_(0) { }
                inline std::size_t operator()() { return idx_++; }
                std::size_t idx_;
            };

            struct suffix_array_sort_comparator
            {
                suffix_array_sort_comparator (string_range_type const &str) : str_(str) { }
                bool operator()(std::size_t lhs, std::size_t rhs)
                {
                    return std::lexicographical_compare(str_.begin()+lhs, str_.end(), str_.begin()+rhs, str_.end());
                }
                string_range_type const &str_;
            };

        };
    };
} }

namespace boost { using boost::algorithm::suffix_array_search; }

#endif