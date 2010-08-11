#ifndef BOOST_ALGORITHM_KNUTH_MORRIS_PRATT_HPP
#define BOOST_ALGORITHM_KNUTH_MORRIS_PRATT_HPP

#include <iterator>
#include <vector>
#include <memory>

#include <boost/range/iterator_range.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include <boost/algorithm/string/finder/detail/finder_typedefs.hpp>

/*!
    \file
    Implements the Knuth-Morris-Pratt string search algorithm
*/

namespace boost { namespace algorithm {
    //! An implementation of the string search algorithm Knuth-Morris-Pratt
    struct knuth_morris_pratt
    {

        template <class Finder,class RandomAccessRange1T,
        class RandomAccessRange2T,class ComparatorT,class AllocatorT>
        class algorithm
            : public boost::algorithm::detail::finder_typedefs<
            RandomAccessRange1T,RandomAccessRange2T,ComparatorT,AllocatorT>
        {
        public:
            std::string get_algorithm_name () const { return "Knuth-Morris-Pratt"; }
        protected:
            string_range_type find(string_iterator_type start)
            {
                return find(start, string_iterator_category());
            }

            void on_substring_change()
            {
                on_substring_change(substring_iterator_category());
            }

            void on_string_change()
            {
            }
        private:

            std::vector<std::size_t,
                typename AllocatorT::template rebind<std::size_t>::other > failure_func;

            string_range_type find(string_iterator_type start, std::random_access_iterator_tag)
            {
                string_range_type const &str = static_cast<Finder*>(this)->get_string_range();
                substring_range_type const &substr = static_cast<Finder*>(this)->get_substring_range();
                comparator_type const &comp = static_cast<Finder*>(this)->get_comparator();

                std::size_t str_idx = start - boost::begin(str),
                    str_size = boost::end(str) - boost::begin(str), substr_idx = 0,
                    substr_size = boost::end(substr) - boost::begin(substr);

                if (boost::begin(substr) == boost::end(substr))
                    return boost::iterator_range<string_iterator_type>(
                        start, start);

                std::size_t substr_last_index = substr_size-1;

                //substring bigger than string
                if (substr_size > str_size-str_idx)
                    return make_iterator_range(boost::end(str), boost::end(str));


                //TODO step by step run for substr_size=1.

                //while (idx < str_size)
                std::size_t compare_against = str_size - substr_size + 1;
                while (str_idx-substr_idx < compare_against)
                {
                    // Invariant: substring[0..substr_idx-1] == string[..str_idx-1] iff substr_idx>0
                    
                    //Slide the pattern to the right until we manage to find a match for the current char
                    while (substr_idx > 0 &&
                            !comp(*(boost::begin(str)+str_idx),*(boost::begin(substr)+substr_idx)))
                        substr_idx = failure_func[substr_idx-1];

                    // Invariant: Either substr_idx==0 or string[str_idx]==substr[substr_idx]

                    while (substr_idx == 0 && str_idx < compare_against
                            && !comp(*(boost::begin(str)+str_idx),*(boost::begin(substr)+0)))
                        ++str_idx;

                    //Invariant: string[str_idx]==substr[substr_idx] or (str_idx=0 and str_idx >= str_size)
                    if (substr_idx == substr_last_index && str_idx-substr_idx < compare_against)
                        return make_iterator_range(
                        //boost::begin(str)+(str_idx+1-substr_size),
                        boost::begin(str)+(str_idx-substr_last_index),
                        boost::begin(str)+(str_idx+1));

                    ++substr_idx; ++str_idx;
                }
                return boost::iterator_range<string_iterator_type>(
                    boost::end(str), boost::end(str));
            }
            
            void on_substring_change(std::random_access_iterator_tag)
            {
                substring_range_type const &substr = static_cast<Finder*>(this)->get_substring_range();
                comparator_type const &comp = static_cast<Finder*>(this)->get_comparator();

                //failure_func[i] = the size of the largest border (prefix that's also a suffix)
                //      of P[0..i], that's different from itself.
                //failure_func[i] = q <=> P[0..q-1] = P[..m-1] (first q chars equal to last q chars)

                //Invariant: 0 <= failure_func[i] <= i, i=0..m-1


                std::size_t i = 0, j = 1, substr_size = boost::end(substr) - boost::begin(substr);
                failure_func.clear();
                failure_func.reserve(substr_size);
                failure_func.push_back(0); // failure_func[0] = 0
                //std::size_t capacity = failure_func.capacity();
                while (j < substr_size)
                {
                    while (i > 0 && !comp(*(boost::begin(substr)+i), *(boost::begin(substr)+j)))
                        i = failure_func[i-1];
                    while (i == 0 && j < substr_size &&
                        !comp(*(boost::begin(substr)+0),*(boost::begin(substr)+j)))
                    {
                        //Invariant: i == 0 and substr[0] != substr[j], which means failure_func[j]=0
                        failure_func.push_back(0);
                        ++j;
                    }
                    //Invariant: j is out of bounds or P[i]==P[j], meaning failure_func[j]=i+1
                    if (j < substr_size) failure_func.push_back(i+1);
                    ++j; ++i;
                }
                //assert(failure_func.capacity() == capacity);
            }

        };
    };
    //! Instances of this type can be passed to find functions to require them to
    //!     use the Knuth-Morris-Pratt algorithm.
    struct knuth_morris_pratt_tag { typedef boost::algorithm::knuth_morris_pratt type; };
} }

namespace boost
{
    using boost::algorithm::knuth_morris_pratt;
    using boost::algorithm::knuth_morris_pratt_tag;

}

#endif