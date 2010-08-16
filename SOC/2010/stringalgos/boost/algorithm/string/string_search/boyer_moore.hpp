//  Boost string_algo library boyer_moore.hpp header file  ---------------------------//

//  Copyright Stefan Mihaila 2010
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/ for updates, documentation, and revision history.

#ifndef BOOST_ALGORITHM_BOYER_MOORE_HPP
#define BOOST_ALGORITHM_BOYER_MOORE_HPP

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/adaptor/reversed.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/distance.hpp>
#include <boost/range/category.hpp>

#include <boost/call_traits.hpp>

#include <boost/type_traits/is_same.hpp>

#include <boost/static_assert.hpp>

#include <map>
#include <string>
#include <locale>
#include <iterator>
#include <memory>
#include <utility>
#include <vector>

#include <boost/algorithm/string/config.hpp>
#include <boost/algorithm/string/finder/detail/finder_typedefs.hpp>
#include <boost/algorithm/string/finder/detail/string_search_ranges.hpp>

#include <boost/unordered_map.hpp>
#include <boost/functional/hash.hpp>

#include <boost/utility/enable_if.hpp>


/*!
    \file
    Implements the Boyer-Moore string search algorithm
*/

namespace boost { namespace algorithm {
    //! An implementation of the string search algorithm Boyer-Moore
    //! \warning This algorithm can only work with the equality comparator and in certain cases the inequality comparator
    //! \warning For charsets where tolower(a)==tolower(b) is not equivalent to a case-insensitive match,
    //!         you \b MUST \b NOT use the inequality comparator with this algorithm.
    //!         It is best to avoid case-insensitive matches using Boyer-Moore for anything but
    //!         CharT=char
    struct boyer_moore
    {

        template <class Range1CharT, class Range2CharT, class ComparatorT, class AllocatorT>
        class algorithm
        {
        private:
            typedef Range1CharT substring_char_type;
            typedef Range2CharT string_char_type;
            typedef ComparatorT comparator_type;
            typedef AllocatorT allocator_type;
        public:
            std::string get_algorithm_name () const { return "Boyer-Moore"; }
            algorithm (comparator_type const &comp, allocator_type const &alloc)
                : comp_(comp), alloc_(alloc), table1(alloc_), table2(alloc_)
            {
                BOOST_STATIC_ASSERT((boost::is_same<substring_char_type,string_char_type>::value));
            }

            template <class Range1T, class Range2T>
            inline BOOST_STRING_TYPENAME boost::iterator_range<BOOST_STRING_TYPENAME boost::range_iterator<Range2T>::type>
                find(BOOST_STRING_TYPENAME boost::algorithm::detail::string_search_ranges<Range1T, Range2T> const &ranges)
            {
                
                return find(ranges, BOOST_STRING_TYPENAME boost::range_category<Range2T>::type());
            }

            //Compute the two tables
            template <class Range1T, class Range2T>
            inline void on_substring_change(
                BOOST_STRING_TYPENAME boost::algorithm::detail::string_search_ranges<Range1T, Range2T> const &ranges)
            {
                on_substring_change(ranges.substr, BOOST_STRING_TYPENAME boost::range_category<Range1T>::type());
            }

            //No precomputation to be done on the string
            template <class T>
            inline void on_string_change(T const&) { }
        private:
            comparator_type comp_; allocator_type alloc_;

            //TODO Maybe optimize for sizeof(substring_char_type)==1, or substring_char_type=char?
            typedef BOOST_STRING_TYPENAME boost::unordered_map<substring_char_type, std::size_t,
                BOOST_STRING_TYPENAME boost::hash<substring_char_type>, comparator_type,
                BOOST_STRING_TYPENAME allocator_type::template
                rebind<substring_char_type>::other
            > table1_type;
            table1_type table1;

            typedef BOOST_STRING_TYPENAME std::vector<std::size_t,
                BOOST_STRING_TYPENAME allocator_type::template rebind<std::size_t>::other
            > table2_type;
            table2_type table2;

            std::size_t substr_size_;

            struct compute_first_table_functor
            {
                //Case 1: ComparatorT=boost::algorithm::is_equal
                void operator()(BOOST_STRING_TYPENAME boost::call_traits<substring_char_type>::param_type c)
                {
                    compute_first_table<comparator_type>(c);
                }
                compute_first_table_functor (algorithm &alg) : idx_(0), alg_(alg)
                { alg_.table1.clear(); }

                template <class ComparatorTT>
                void compute_first_table(BOOST_STRING_TYPENAME boost::call_traits<substring_char_type>::param_type c,
                    BOOST_STRING_TYPENAME boost::enable_if<
                    BOOST_STRING_TYPENAME boost::is_same<ComparatorTT, boost::algorithm::is_equal> >::type* =0)
                {
                    //alg_.table1.insert( std::make_pair(c, alg_.substr_size_ - 1 - idx_) );
                    if (idx_ != 0) alg_.table1.insert(std::make_pair(c, idx_));

                    ++idx_;
                }

                //Case 2: ComparatorT=boost::algorithm::is_iequal
                template <class ComparatorTT>
                void compute_first_table(BOOST_STRING_TYPENAME boost::call_traits<substring_char_type>::param_type c,
                    BOOST_STRING_TYPENAME boost::enable_if<
                    BOOST_STRING_TYPENAME boost::is_same<ComparatorTT, boost::algorithm::is_iequal> >::type* =0)
                {
                    //Case insensitive matches are supported properly for char (in case of Boyer-Moore)
                    BOOST_STATIC_ASSERT((boost::is_same<substring_char_type,char>::value));

                    //todo get locale from user?
                    if (idx_ != 0) alg_.table1.insert(std::make_pair(
                        std::tolower(c, std::locale()), idx_
                        ));
                    ++idx_;
                }
                std::size_t idx_;
                algorithm &alg_;
            };

            //precomputation on pattern=bidirectional range
            template <class RangeT>
            void on_substring_change(RangeT const &substr, std::random_access_iterator_tag)
            {
                
                substr_size_ = boost::distance(substr);

                //Compute the first table
                boost::for_each(substr | boost::adaptors::reversed,
                    compute_first_table_functor(*this));
                
                //Compute the second table
               
                //this is a table similar to the one in the KMP algorithm
                //failure_func[i]=k means k is the size of the biggest boundary of the string P[i..m-1]
                //(which is not the string itself)
                //i.e. P[i..i+k+1] = P[..m-1]
                std::vector<std::size_t, BOOST_STRING_TYPENAME allocator_type::template rebind<std::size_t>::other>
                    failure_func(substr_size_);


                //The second table: delta2(x) = m-1-i + min(d1(x),d2(x)) <= 2m-1-i
                table2.clear(); table2.reserve(substr_size_);
                for (unsigned int i = 0; i < substr_size_; ++i)
                    table2.push_back(2*substr_size_ - 1 - i); // initialize with m-1-i+m=2m-1-i (maximum)
                    //table2.push_back(substr_size_);

                if (substr_size_ < 2) return;

                BOOST_STRING_TYPENAME boost::range_iterator<RangeT>::type const &pattern = boost::begin(substr);


                //todo find a better solution for this
                for (unsigned int i = substr_size_-1;i--;)
                {
                    //pattern[i] != pattern[m-1]
                    if (!comp_(*(pattern+i),*(pattern+(substr_size_-1))))
                    {
                        table2[substr_size_-1] = substr_size_ - 1 - i;
                        break;
                    }
                }

                //feel free to refactor if you can find better names heh
                std::size_t i = substr_size_ - 1,j = substr_size_ - 2;
                //it's zero-initialized anyway
                ///failure_func[substr_size_-1] = 0; // the one-character string cannot have boundaries

                //Start computing failure_func, and at the same time use the obtained values
                //to compute the first part of the second table (d1)
                while (true) // the condition is j>=0, checked at the end.
                {
                    //Invariant: P[i+1..m-1] = P[j+1..m-1]
                    //try to align with pattern indexed by j (by sliding the pattern indexed by i)
                    while (i != substr_size_ - 1 && !comp_(*(pattern+i),*(pattern+j)))
                        i = substr_size_ - 1 - failure_func[i + 1];

                    //Invariant: Either i=m-1 or P[i..m-1] = P[j..m-1]
                    while (i == substr_size_-1 && j > 0 &&
                        !comp_(*(pattern+(substr_size_-1)),*(pattern+j)))
                    {
                        //couldn't align the given j with any i
                        failure_func[j] = 0;
                        --j;
                    }
                    //Invariant: either (j==0 and i=m-1) or P[i..m-1] = P[j..m-1]
                    if (j == 0 && i == substr_size_-1 &&
                        !comp_(*(pattern+0),*(pattern+(substr_size_-1))))
                    {
                        failure_func[0] = 0;
                    }
                    else failure_func[j] = substr_size_-i;

                    //we only use the newly computed value pi(j)=a if j and a are nonzero.
                    if (j != 0)
                    {
                        //Invariant: P[i..m-1] = P[j..m-1] (i and j are aligned)
                        /*
                        Let B_P(i) be the set of all boundary sizes of P[i..m-1]
                        B_P(i) = { k+1 | (k=-1) OR (P[i..i+k] = P[m-1-k..m-1]) }
                        (k+1 because P[i..i+k] contains k+1 characters)
                        (k=-1 because k+1=0 is always a valid boundary size -- the empty boundary)
                        The set B_P(j) is computed at this point, based on the assignment on
                        the failure function above.

                        We want to compute:
                        d1(x) = min { k | (k=m) OR ((P[x+1..m-1] = P[x+1-k..m-1-k]) AND P[i]!=P[i-k]) }
                         for x<m-1 and k>0
                        This is equivalent to:
                        d1(x) = min { k | (k=m) OR (m-1-x \in B_P(x+1-k) AND P[i]!=P[i-k]) }
                        
                        Whenever a mismatch occurs whilist matching this pattern against a text,
                        d1(x) tells us how much the pattern should shift in order for it to be able
                        to match.

                        */

                        //for all nonzero a \in B_P(j):
                        
                        //Using the definition of d1 above:
                        //a = m-1-x <=> x = m-1-a
                        //j = x+1-k <=> k = m-a-j
                        unsigned int a = failure_func[j];
                        while (a > 0 && !comp_(*(pattern+(substr_size_-1-a)), *(pattern+(j-1))))
                        //while (a > 0 && !compare_range_nth(comp, substr, substr_size_-1-a, substr, j-1))
                        {
                            assert(substr_size_-1-a >= substr_size_-a-j); // x >= k
                            if (table2[substr_size_-1-a] > 2*substr_size_-i-1 -a-j)
                                table2[substr_size_-1-a] = 2*substr_size_-i-1 -a-j;
                            //if (table2[substr_size_-1-a] > substr_size_-a-j)
                            //    table2[substr_size_-1-a] = substr_size_-a-j;

                            a = failure_func[substr_size_ - a]; // get the next boundary size in B_P(j)
                        }
                    }
                    if (j > 0) { --j; --i; }
                    else break;
                }
                //We're done computing the reverse KMP function and the first part of the second table
                //now for the second part:
                //d2(i) = min { k-1 | (k=m+1) OR (P[0..m-k] = P[k-1..m-1] AND i+2<=k<=m) }
                //equivalent to:
                //d2(i) = min { k-1 | (k=m+1) OR (m-k+1 \in B_P(0) AND i+2 <= k <= m) }

                //for (std::size_t i = 0; i < substr_size_; ++i)
                //    table2[i] = substr_size_;

                std::size_t boundary = failure_func[0];
                for (std::size_t i = 0; i < substr_size_-1 && boundary; ++i)
                {
                    while (boundary > 0 && i+1>substr_size_-boundary)
                        boundary = failure_func[substr_size_-boundary];
                    if (table2[i] > 2*substr_size_-i-1-boundary)
                        table2[i] = 2*substr_size_-i-1-boundary;
                    //if (table2[i] > substr_size_-boundary)
                    //    table2[i] = substr_size_-boundary;
                }
            }

            //finding in text=random access range
            template <class Range1T, class Range2T>
            inline BOOST_STRING_TYPENAME boost::iterator_range<BOOST_STRING_TYPENAME boost::range_iterator<Range2T>::type>
                find(BOOST_STRING_TYPENAME boost::algorithm::detail::string_search_ranges<Range1T, Range2T> const &ranges,
                std::random_access_iterator_tag)
            {
                BOOST_ALGORITHM_DETAIL_COMMON_FINDER_TYPEDEFS(Range1T, Range2T);

                string_range_type const &str = ranges.str;
                substring_range_type const &substr = ranges.substr;
                string_iterator_type start = ranges.offset;

                //string_range_type const &str = static_cast<Finder*>(this)->get_string_range();
                //substring_range_type const &substr = static_cast<Finder*>(this)->get_substring_range();
                //comparator_type const &comp = static_cast<Finder*>(this)->get_comparator();

                std::size_t str_idx, substr_idx, str_size;

                if (substr_size_ == 0)
                    return string_range_type(start, start);

                str_size = boost::end(str) - start;
                str_idx = substr_size_ - 1; substr_idx = substr_size_ - 1;


                while (str_idx < str_size)
                {
                    if (comp_(*(start+str_idx), *(boost::begin(substr)+substr_idx)))
                    {
                        if (substr_idx == 0)
                        {
                            return string_range_type(start+str_idx,start+str_idx+substr_size_);
                        }
                        else { assert(str_idx > 0); --substr_idx; --str_idx; }
                    }
                    else
                    {
                        BOOST_STRING_TYPENAME table1_type::const_iterator iter = table1_find<comparator_type>(start[str_idx]);
                        std::size_t step = substr_size_ - substr_idx;
                        if (iter == table1.end())
                        {
                            step = substr_size_;
                        }
                        else if (substr_size_ - 1 - iter->second < substr_idx)
                        {
                            step = iter->second;
                        }
                                                
                        if (step < table2[substr_idx]) str_idx += table2[substr_idx];
                        else str_idx += step;
                        //start from the end of the substring again.
                        substr_idx = substr_size_ - 1;
                        //fast loop. get rid of matches that fail on the first character.
                        //remove this if it shows no improvement whatsoever. might be useful after adding table2
                        /*while (str_idx < str_size && !comp(start[str_idx],boost::begin(substr)[substr_idx]))
                        {
                            table1_type::const_iterator iter = table1_find<comparator_type>(start[str_idx]);
                            if (iter == table1.end()) str_idx += substr_size_;
                            else str_idx += iter->second;
                        }*/
                    }
                }
                return string_range_type(boost::end(str), boost::end(str));
            }

            //Case 1: ComparatorT=boost::algorithm::is_equal
            template <class ComparatorTT>
            BOOST_STRING_TYPENAME table1_type::iterator table1_find (string_char_type const &chr,
                BOOST_STRING_TYPENAME boost::enable_if<
                BOOST_STRING_TYPENAME boost::is_same<ComparatorTT, boost::algorithm::is_equal> >::type* = 0)
            {
                return table1.find(chr);
            }

            //Case 2: ComparatorT=boost::algorithm::is_iequal
            template <class ComparatorTT>
            BOOST_STRING_TYPENAME table1_type::iterator table1_find (string_char_type const &chr,
                BOOST_STRING_TYPENAME boost::enable_if<
                BOOST_STRING_TYPENAME boost::is_same<ComparatorTT, boost::algorithm::is_iequal> >::type* = 0)
            {
                return table1.find(std::tolower(chr, std::locale()));
            }

        };
    };
    //! Instances of this type can be passed to find functions to require them to
    //!     use the Boyer-Moore algorithm.
    struct boyer_moore_tag { typedef boost::algorithm::boyer_moore type; };
} }

namespace boost
{
    using boost::algorithm::boyer_moore;
    using boost::algorithm::boyer_moore_tag;
}

#endif
