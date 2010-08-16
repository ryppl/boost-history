//  Boost string_algo library functor_finders.hpp header file  ---------------------------//

//  Copyright Stefan Mihaila 2010.
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/ for updates, documentation, and revision history.

#ifndef BOOST_ALGORITHM_GENERATED_FINDERS_HPP
#define BOOST_ALGORITHM_GENERATED_FINDERS_HPP

#include <boost/algorithm/string/compare.hpp>
#include <boost/algorithm/string/finder/detail/string_search_ranges.hpp>
#include <boost/algorithm/string/finder/detail/finder_typedefs.hpp>
#include <boost/algorithm/string/finder/detail/functor_finders.hpp>
#include <boost/algorithm/string/finder/default_search_algorithm.hpp>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/as_literal.hpp>
#include <boost/range/value_type.hpp>
#include <boost/range/iterator_range.hpp>

#include <boost/iterator/iterator_traits.hpp>

#include <memory>

/*! \file
    Defines a few useful finder types (\ref boost::algorithm::first_finder_t, \ref boost::algorithm::last_finder_t,
    \ref boost::algorithm::nth_finder_t).
    These finder objects are functors which get constructed with a pattern to search for, and can afterwards
    be called using a pair of iterators representing the text in which the search is to be performed.
*/

namespace boost { namespace algorithm {

    //! A generic "first" finder type.
    /*!
        Instances of this type can be called with a pair of iterators,
        representing the range of the text (the string in which to search).
        This functor returns the first occurrence of the pattern in the given text.

        \tparam Range1T A range representing the type of the substring (pattern)
        \tparam AlgorithmT The algorithm used to perform the searches
        \tparam ComparatorT Optional template parameter passed to the algorithm;
            Used for comparing individual characters for equality.
        \tparam AllocatorT Optional template parameter passed to the algorithm
            in the event that additional computation on the data has to be stored.
    */
    template <class Range1T, class AlgorithmT = boost::algorithm::default_finder_algorithm,
        class ComparatorT = boost::algorithm::is_equal, class AllocatorT = std::allocator<std::size_t> >
    class first_finder_t
    {
    public:

        //! Constructs a \ref first_finder_t.
        /*!
            \param substr A pointer to a range indicating the pattern that is to be searched.
            \param comparator An instance of \c ComparatorT
            \param allocator An instance of \c AllocatorT
        */
        first_finder_t (Range1T const *const substr, ComparatorT const &comparator=ComparatorT(),
                AllocatorT const &allocator=AllocatorT())
            : substring_range_(boost::as_literal(*substr)), algorithm_(comparator,allocator),
            first_call_(true) { }

        //! Searches for a pattern in the given text
        /*!
            Searches for the first occurrence of the pattern in [string_begin,string_end).
        */
        template <class Iterator2T>
        BOOST_STRING_TYPENAME boost::iterator_range<Iterator2T>
            operator()(Iterator2T const &string_begin, Iterator2T const &string_end)
        {
            typedef BOOST_STRING_TYPENAME boost::iterator_range<Iterator2T> Range2T;

            BOOST_STRING_TYPENAME boost::algorithm::detail::string_search_ranges<Range1T, Range2T> ranges;
            ranges.str = boost::make_iterator_range(string_begin, string_end);
            ranges.substr = substring_range_;
            ranges.offset = string_begin;
            if (first_call_) { algorithm_.on_substring_change(ranges); first_call_ = false; }
            algorithm_.on_string_change(ranges);
            return algorithm_.find(ranges);
        }
    private:
        typedef BOOST_STRING_TYPENAME boost::range_value<Range1T>::type char_type;
        typedef BOOST_STRING_TYPENAME boost::range_const_iterator<Range1T>::type substring_iterator_type;
        typedef BOOST_STRING_TYPENAME AlgorithmT::template algorithm<char_type, char_type,
            ComparatorT, std::allocator<std::size_t> > algorithm_type;

        BOOST_STRING_TYPENAME boost::iterator_range<substring_iterator_type> substring_range_;
        algorithm_type algorithm_;
        bool first_call_;
    };

    //! A generic "last" finder type.
    /*!
        Instances of this type can be called with a pair of iterators,
        representing the range of the text (the string in which to search).
        This functor returns the last occurrence of the pattern in the given text.

        \tparam Range1T A range representing the type of the substring (pattern)
        \tparam AlgorithmT The algorithm used to perform the searches
        \tparam ComparatorT Optional template parameter passed to the algorithm;
            Used for comparing individual characters for equality.
        \tparam AllocatorT Optional template parameter passed to the algorithm
            in the event that additional computation on the data has to be stored.
    */
    template <class Range1T, class AlgorithmT = boost::algorithm::default_finder_algorithm,
        class ComparatorT = boost::algorithm::is_equal, class AllocatorT = std::allocator<std::size_t> >
    class last_finder_t
    {
    public:
        //! Constructs a \ref last_finder_t.
        /*!
            \param substr A pointer to a range indicating the pattern that is to be searched.
            \param comparator An instance of \c ComparatorT
            \param allocator An instance of \c AllocatorT
        */
        last_finder_t (Range1T const *const substr, ComparatorT const &comparator=ComparatorT(),
                AllocatorT const &allocator=AllocatorT())
            : substring_range_(boost::as_literal(*substr)), algorithm_(comparator, allocator),
            first_call_bidi_(true), first_call_forw_(true) { }

        //! Searches for a pattern in the given text
        /*!
            Searches for the last occurrence of the pattern in [string_begin,string_end).
        */
        template <class Iterator2T>
        BOOST_STRING_TYPENAME boost::iterator_range<Iterator2T>
            operator()(Iterator2T const &string_begin, Iterator2T const &string_end)
        {
            return find(string_begin, string_end,
                BOOST_STRING_TYPENAME boost::range_category<Range1T>::type(),
                BOOST_STRING_TYPENAME boost::iterator_category<Iterator2T>::type());
        }
    private:
#       ifndef BOOST_ALGORITHM_DOXYGEN

        //implementation of last_finder_t for when bidirectional iterators are available
        template <class Iterator2T>
        BOOST_STRING_TYPENAME boost::iterator_range<Iterator2T>
            find (Iterator2T const &string_begin, Iterator2T const &string_end,
            std::bidirectional_iterator_tag, std::bidirectional_iterator_tag)
        {
            typedef BOOST_STRING_TYPENAME boost::iterator_range<Iterator2T> Range2T;
            BOOST_ALGORITHM_DETAIL_COMMON_FINDER_TYPEDEFS(Range1T, Range2T);
            BOOST_ALGORITHM_DETAIL_UNCOMMON_FINDER_TYPEDEFS(Range1T, Range2T);

            BOOST_STRING_TYPENAME boost::algorithm::detail::string_search_ranges<substring_reverse_range_type,
                string_reverse_range_type> ranges;
            ranges.substr = boost::make_iterator_range(
                substring_reverse_iterator_type(boost::end(substring_range_)),
                substring_reverse_iterator_type(boost::begin(substring_range_))
                );
            ranges.str = boost::make_iterator_range(
                string_reverse_iterator_type(string_end),
                string_reverse_iterator_type(string_begin)
                );
            ranges.offset = string_reverse_iterator_type(string_end);
            if (first_call_bidi_)
            {
                algorithm_.on_substring_change(ranges);
                first_call_bidi_ = false;
                first_call_forw_ = true;
            }
            algorithm_.on_string_change(ranges);
            string_reverse_range_type const &ret = algorithm_.find(ranges);

            //no match
            if (boost::begin(ret) == string_reverse_iterator_type(string_begin))
                return boost::make_iterator_range(string_end, string_end);
            
            //found a match, convert into direct iterators
            return boost::make_iterator_range(boost::end(ret).base(), boost::begin(ret).base());
        }

        //implementation of last_finder_t when all we have are forward iterators
        //todo test if this works properly
        template <class Iterator2T>
        BOOST_STRING_TYPENAME boost::iterator_range<Iterator2T>
            find (Iterator2T const &string_begin, Iterator2T const &string_end,
            std::forward_iterator_tag, std::forward_iterator_tag)
        {
            typedef BOOST_STRING_TYPENAME boost::iterator_range<Iterator2T> Range2T;

            BOOST_STRING_TYPENAME boost::algorithm::detail::string_search_ranges<Range1T, Range2T> ranges;
            ranges.str = boost::make_iterator_range(string_begin, string_end);
            ranges.substr = substring_range_;
            ranges.offset = string_begin;

            if (first_call_forw_)
            {
                algorithm_.on_substring_change(ranges);
                first_call_forw_ = false;
                first_call_bidi_ = true;
            }
            algorithm_.on_string_change(ranges);
            Range2T prev, crt = boost::make_iterator_range(string_end, string_end);
            for (;;)
            {
                prev = crt;
                crt = algorithm_.find(ranges);
                if (boost::begin(crt) == string_end) break;
                else { ranges.offset = boost::begin(crt); ++ranges.offset; }
            }
            return prev;
        }
        typedef BOOST_STRING_TYPENAME boost::range_value<Range1T>::type char_type;
        typedef BOOST_STRING_TYPENAME boost::range_const_iterator<Range1T>::type substring_iterator_type;
        typedef BOOST_STRING_TYPENAME AlgorithmT::template algorithm<char_type, char_type,
            ComparatorT, std::allocator<std::size_t> > algorithm_type;

        BOOST_STRING_TYPENAME boost::iterator_range<substring_iterator_type> substring_range_;
        algorithm_type algorithm_;
        bool first_call_bidi_, first_call_forw_;

#       endif /* !defined(BOOST_ALGORITHM_DOXYGEN) */
    };
   
    //! A generic "nth" finder type.
    /*!
        Instances of this type can be called with a pair of iterators,
        representing the range of the text (the string in which to search).
        This functor returns the N-th occurrence of the pattern in the given text
        For negative N, the matches are looked up starting from the back (assuming the pattern
        and text support bidirectional ranges). I.e. N = -1 finds the last match.

        \tparam Range1T A range representing the type of the substring (pattern)
        \tparam AlgorithmT The algorithm used to perform the searches
        \tparam ComparatorT Optional template parameter passed to the algorithm;
            Used for comparing individual characters for equality.
        \tparam AllocatorT Optional template parameter passed to the algorithm
            in the event that additional computation on the data has to be stored.
    */
    template <class Range1T, class AlgorithmT = boost::algorithm::default_finder_algorithm,
        class ComparatorT = boost::algorithm::is_equal, class AllocatorT = std::allocator<std::size_t> >
    class nth_finder_t
    {
    public:

        //! Constructs a \ref nth_finder_t.
        /*!
            \param substr A pointer to a range indicating the pattern that is to be searched.
            \param n The value of n (indicating which match we are interested in)
            \param comparator An instance of \c ComparatorT
            \param allocator An instance of \c AllocatorT
        */
        nth_finder_t (Range1T const *const substr, int n=0,
            ComparatorT const &comparator=ComparatorT(), AllocatorT const &allocator=AllocatorT())
            : substring_range_(boost::as_literal(*substr)), n_(n), algorithm_(comparator,allocator),
            first_call_forw_(true), first_call_bidi_(true) { }

        //! Searches for a pattern in the given text
        /*!
            Searches for the nth occurrence of the pattern in [string_begin,string_end).
        */
        template <class Iterator2T>
        BOOST_STRING_TYPENAME boost::iterator_range<Iterator2T>
            operator()(Iterator2T const &string_begin, Iterator2T const &string_end)
        {
            if (n_ < 0)
                return find_backwards(string_begin, string_end,
                    BOOST_STRING_TYPENAME boost::range_category<Range1T>::type(), BOOST_STRING_TYPENAME boost::iterator_category<Iterator2T>::type());
            else return find(string_begin, string_end);
        }

        //! Changes the value of n.
        /*!
            \warning Every time the sign of n changes (from n>=0 to n<0 or vice-versa), the information
            on the pattern is recomputed.
        */
        void set_n(int n) { n_ = n; }
    private:
#       ifndef BOOST_ALGORITHM_DOXYGEN
        //find nth, for n>=0
        template <class Iterator2T>
        BOOST_STRING_TYPENAME boost::iterator_range<Iterator2T>
            find(Iterator2T const &string_begin, Iterator2T const &string_end)
        {
            typedef BOOST_STRING_TYPENAME boost::iterator_range<Iterator2T> Range2T;

            boost::algorithm::detail::string_search_ranges<Range1T, Range2T> ranges;
            ranges.substr = substring_range_;
            ranges.str = boost::make_iterator_range(string_begin, string_end);
            ranges.offset = string_begin;

            if (first_call_forw_)
            {
                algorithm_.on_substring_change(ranges);
                first_call_forw_ = false;
                first_call_bidi_ = true;
            }
            algorithm_.on_string_change(ranges);

            Range2T ret;
            for (int n = 0; n <= n_; ++n)
            {
                ret = algorithm_.find(ranges);
                if (boost::begin(ret) == string_end)
                    return boost::make_iterator_range(string_end, string_end);
                else { ranges.offset=boost::begin(ret); ++ranges.offset; }
            }
            return ret;
        }

        //find nth, for n < 0
        template <class Iterator2T>
        BOOST_STRING_TYPENAME boost::iterator_range<Iterator2T>
            find_backwards(Iterator2T const &string_begin, Iterator2T const &string_end,
            std::bidirectional_iterator_tag, std::bidirectional_iterator_tag)
        {
            typedef BOOST_STRING_TYPENAME boost::iterator_range<Iterator2T> Range2T;
            BOOST_ALGORITHM_DETAIL_COMMON_FINDER_TYPEDEFS(Range1T, Range2T);
            BOOST_ALGORITHM_DETAIL_UNCOMMON_FINDER_TYPEDEFS(Range1T, Range2T);

            boost::algorithm::detail::string_search_ranges<substring_reverse_range_type,
                string_reverse_range_type> ranges;
            ranges.substr = boost::make_iterator_range(
                substring_reverse_iterator_type(boost::end(substring_range_)),
                substring_reverse_iterator_type(boost::begin(substring_range_))
                );
            ranges.str = boost::make_iterator_range(
                string_reverse_iterator_type(string_end),
                string_reverse_iterator_type(string_begin)
                );
            ranges.offset = string_reverse_iterator_type(string_end);

            if (first_call_bidi_)
            {
                algorithm_.on_substring_change(ranges);
                first_call_bidi_ = false;
                first_call_forw_ = true;
            }
            algorithm_.on_string_change(ranges);

            string_reverse_range_type ret;
            int n_2 = -n_ - 1;
            for (int n = 0; n <= n_2; ++n)
            {
                ret = algorithm_.find(ranges);
                if (boost::begin(ret) == string_reverse_iterator_type(string_begin))
                    return boost::make_iterator_range(string_end, string_end);
                else { ranges.offset = boost::begin(ret); ++ranges.offset; }
            }
            return boost::make_iterator_range(boost::end(ret).base(), boost::begin(ret).base());
        }

        typedef BOOST_STRING_TYPENAME boost::range_value<Range1T>::type char_type;
        typedef BOOST_STRING_TYPENAME boost::range_const_iterator<Range1T>::type substring_iterator_type;
        typedef BOOST_STRING_TYPENAME AlgorithmT::template algorithm<char_type, char_type,
            ComparatorT, std::allocator<std::size_t> > algorithm_type;

        BOOST_STRING_TYPENAME boost::iterator_range<substring_iterator_type> substring_range_;
        int n_;
        algorithm_type algorithm_;
        bool first_call_forw_, first_call_bidi_;

#       endif /* !defined(BOOST_ALGORITHM_DOXYGEN) */
    };
} }

namespace boost
{
    using algorithm::first_finder_t;
    using algorithm::last_finder_t;
    using algorithm::nth_finder_t;
}

#endif // BOOST_ALGORITHM_GENERATED_FINDERS_HPP
