#ifndef BOOST_ALGORITHM_GENERATED_FINDERS_HPP
#define BOOST_ALGORITHM_GENERATED_FINDERS_HPP

//#include <boost/algorithm/string/finder/detail/last_finder_impl.hpp>
//#include <boost/algorithm/string/finder/detail/nth_finder_impl.hpp>
#include <boost/algorithm/string/finder/detail/string_search_ranges.hpp>
#include <boost/algorithm/string/finder/detail/finder_typedefs.hpp>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/as_literal.hpp>
#include <boost/range/value_type.hpp>
#include <boost/range/iterator_range.hpp>

#include <boost/iterator/iterator_traits.hpp>

#include <memory>

namespace boost { namespace algorithm {

    template <class Range1T, class AlgorithmT, class ComparatorT>
    class first_finder_t
    {
    public:
        first_finder_t (Range1T const *const substr, ComparatorT const &comparator=ComparatorT())
            : substring_range_(boost::as_literal(*substr)), algorithm_(comparator,std::allocator<std::size_t>()),
            first_call_(true) { }

        template <class Iterator2T>
        typename boost::iterator_range<Iterator2T>
            operator()(Iterator2T const &string_start, Iterator2T const &string_end)
        {
            typedef typename boost::iterator_range<Iterator2T> Range2T;

            typename boost::algorithm::detail::string_search_ranges<Range1T, Range2T> ranges;
            ranges.str = boost::make_iterator_range(string_start, string_end);
            ranges.substr = substring_range_;
            ranges.offset = string_start;
            if (first_call_) { algorithm_.on_substring_change(ranges); first_call_ = false; }
            algorithm_.on_string_change(ranges);
            return algorithm_.find(ranges);
        }
    private:
        typedef typename boost::range_value<Range1T>::type char_type;
        typedef typename boost::range_const_iterator<Range1T>::type substring_iterator_type;
        typedef typename AlgorithmT::template algorithm<char_type, char_type,
            ComparatorT, std::allocator<std::size_t> > algorithm_type;

        typename boost::iterator_range<substring_iterator_type> substring_range_;
        algorithm_type algorithm_;
        bool first_call_;
    };

    template <class Range1T, class AlgorithmT, class ComparatorT>
    class last_finder_t
    {
    public:
        last_finder_t (Range1T const *const substr, ComparatorT const &comparator=ComparatorT())
            : substring_range_(boost::as_literal(*substr)), algorithm_(comparator,std::allocator<std::size_t>()),
            first_call_bidi_(true), first_call_forw_(true) { }

        template <class Iterator2T>
        typename boost::iterator_range<Iterator2T>
            operator()(Iterator2T const &string_start, Iterator2T const &string_end)
        {
            return find(string_start, string_end,
                typename boost::range_category<Range1T>::type(),
                typename boost::iterator_category<Iterator2T>::type());
        }
    private:
        //implementation of last_finder_t for when bidirectional iterators are available
        template <class Iterator2T>
        typename boost::iterator_range<Iterator2T>
            find (Iterator2T const &string_start, Iterator2T const &string_end,
            std::bidirectional_iterator_tag, std::bidirectional_iterator_tag)
        {
            typedef typename boost::iterator_range<Iterator2T> Range2T;
            BOOST_ALGORITHM_DETAIL_COMMON_FINDER_TYPEDEFS(Range1T, Range2T);
            BOOST_ALGORITHM_DETAIL_UNCOMMON_FINDER_TYPEDEFS(Range1T, Range2T);

            typename boost::algorithm::detail::string_search_ranges<substring_reverse_range_type,
                string_reverse_range_type> ranges;
            ranges.substr = boost::make_iterator_range(
                substring_reverse_iterator_type(boost::end(substring_range_)),
                substring_reverse_iterator_type(boost::begin(substring_range_))
                );
            ranges.str = boost::make_iterator_range(
                string_reverse_iterator_type(string_end),
                string_reverse_iterator_type(string_start)
                );
            ranges.offset = string_reverse_iterator_type(string_end);
            if (first_call_bidi_)
            {
                algorithm_.on_substring_change(ranges);
                first_call_bidi_ = false;
                first_call_forw_ = true;
            }
            algorithm_.on_string_change(ranges);
            string_reverse_range_type &ret = algorithm_.find(ranges);

            //no match
            if (boost::begin(ret) == string_reverse_iterator_type(string_start))
                return boost::make_iterator_range(string_end, string_end);
            
            //found a match, convert into direct iterators
            return boost::make_iterator_range(boost::end(ret).base(), boost::begin(ret).base());
        }

        //implementation of last_finder_t when all we have are forward iterators
        template <class Iterator2T>
        typename boost::iterator_range<Iterator2T>
            find (Iterator2T const &string_start, Iterator2T const &string_end,
            std::forward_iterator_tag, std::forward_iterator_tag)
        {
            typedef typename boost::iterator_range<Iterator2T> Range2T;

            typename boost::algorithm::detail::string_search_ranges<Range1T, Range2T> ranges;
            ranges.str = boost::make_iterator_range(string_start, string_end);
            ranges.substr = substring_range_;
            ranges.offset = string_start;

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
        typedef typename boost::range_value<Range1T>::type char_type;
        typedef typename boost::range_const_iterator<Range1T>::type substring_iterator_type;
        typedef typename AlgorithmT::template algorithm<char_type, char_type,
            ComparatorT, std::allocator<std::size_t> > algorithm_type;

        typename boost::iterator_range<substring_iterator_type> substring_range_;
        algorithm_type algorithm_;
        bool first_call_bidi_, first_call_forw_;
    };
   
    template <class Range1T, class AlgorithmT, class ComparatorT>
    class nth_finder_t
    {
    public:
        nth_finder_t (Range1T const *const substr, ComparatorT const &comparator=ComparatorT(), int n=0)
            : substring_range_(boost::as_literal(*substr)), algorithm_(comparator,std::allocator<std::size_t>()), n_(n),
            first_call_forw_(true), first_call_bidi_(true) { }

        template <class Iterator2T>
        typename boost::iterator_range<Iterator2T>
            operator()(Iterator2T const &string_start, Iterator2T const &string_end)
        {
            if (n_ < 0)
                return find_backwards(string_start, string_end,
                    typename boost::range_category<Range1T>::type(), typename boost::iterator_category<Iterator2T>::type());
            else return find(string_start, string_end);
        }
    private:
        //find nth, for n>=0
        template <class Iterator2T>
        typename boost::iterator_range<Iterator2T>
            find(Iterator2T const &string_start, Iterator2T const &string_end)
        {
            typedef typename boost::iterator_range<Iterator2T> Range2T;

            boost::algorithm::detail::string_search_ranges<Range1T, Range2T> ranges;
            ranges.substr = substring_range_;
            ranges.str = boost::make_iterator_range(string_start, string_end);
            ranges.offset = string_start;

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
        typename boost::iterator_range<Iterator2T>
            find_backwards(Iterator2T const &string_start, Iterator2T const &string_end,
            std::bidirectional_iterator_tag, std::bidirectional_iterator_tag)
        {
            typedef typename boost::iterator_range<Iterator2T> Range2T;
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
                string_reverse_iterator_type(string_start)
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
                if (boost::begin(ret) == string_reverse_iterator_type(string_start))
                    return boost::make_iterator_range(string_end, string_end);
                else { ranges.offset = boost::begin(ret); ++ranges.offset; }
            }
            return boost::make_iterator_range(boost::end(ret).base(), boost::begin(ret).base());
        }

        typedef typename boost::range_value<Range1T>::type char_type;
        typedef typename boost::range_const_iterator<Range1T>::type substring_iterator_type;
        typedef typename AlgorithmT::template algorithm<char_type, char_type,
            ComparatorT, std::allocator<std::size_t> > algorithm_type;

        typename boost::iterator_range<substring_iterator_type> substring_range_;
        algorithm_type algorithm_;
        bool first_call_forw_, first_call_bidi_;
        int n_;
    };
} }

namespace boost
{
    using algorithm::first_finder_t;
    using algorithm::last_finder_t;
    using algorithm::nth_finder_t;
}

#endif // BOOST_ALGORITHM_GENERATED_FINDERS_HPP
