#ifndef BOOST_UNICODE_COMBINING_HPP
#define BOOST_UNICODE_COMBINING_HPP

#include <boost/config.hpp>
#include <boost/iterator/consumer_iterator.hpp>
#include <boost/cuchar.hpp>
#include <algorithm>

#include <boost/throw_exception.hpp>
#include <stdexcept>
#ifndef BOOST_NO_STD_LOCALE
#include <sstream>
#include <ios>
#endif

#include <boost/type_traits.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost
{
namespace unicode
{

/** Maximum size of a combining character sequence in a stream-safe Unicode string */
typedef mpl::int_<31> combining_max;

namespace detail
{
    struct combining_pred
    {
        bool operator()(char32 lft, char32 rgt) const
        {
            return ucd::get_combining_class(lft) < ucd::get_combining_class(rgt);
        }
    };
    
    template<typename Size, typename Iterator, typename Comp>
    void stable_sort_bounded(Iterator begin, Iterator end, Comp comp = std::less<typename std::iterator_traits<Iterator>::value_type>())
    {
#if defined(__GLIBCPP__) || defined(__GLIBCXX__) || defined(__SGI_STL_PORT) || defined(_STLPORT_VERSION) 
        typename std::iterator_traits<Iterator>::value_type buf[Size::value];
        return std::__stable_sort_adaptive(begin, end, buf, Size::value, comp);
#else
        return std::stable_sort(begin, end, comp);
#endif
    }
    
    template<typename Iterator>
    void not_stream_safe(Iterator begin, Iterator end)
    {
#ifndef BOOST_NO_STD_LOCALE
        std::stringstream ss;
        ss << "Invalid Unicode stream-safe combining character sequence " << std::showbase << std::hex;
        for(Iterator it = begin; it != end; ++it)
            ss << *it << " ";
        ss << "encountered while trying to decompose UTF-32 sequence";
        std::out_of_range e(ss.str());
#else
        std::out_of_range e("Invalid Unicode stream-safe combining character sequence encountered while trying to decompose UTF-32 sequence");
#endif
        boost::throw_exception(e);
    }
    
    template<typename Iterator, typename OutputIterator>
    OutputIterator copy_bounded(Iterator begin, Iterator end, OutputIterator first, OutputIterator last)
    {
        for(Iterator it = begin; it != end; ++it)
        {
            if(first == last)
                not_stream_safe(begin, end);
            
            *first++ = *it;
        }
        return first;
    }

} // namespace detail

/** Model of \c \xmlonly<conceptname>Consumer</conceptname>\endxmlonly
 * that consumes combining character sequences. */
struct combiner
{
    typedef char32 input_type;
    typedef char32 output_type;
    
    template<typename Iterator>
    Iterator ltr(Iterator begin, Iterator end)
    {
        do
        {
            ++begin;
        }
        while(begin != end && ucd::get_combining_class(*begin) != 0);
        return begin;
    }
    
    template<typename Iterator>
    Iterator rtl(Iterator begin, Iterator end)
    {
        while(end != begin && ucd::get_combining_class(*--end) != 0);
        return end;
    }
};

struct combine_sorter
{
    typedef char32 input_type;
    typedef char32 output_type;
    typedef combining_max max_output;
    
    template<typename In, typename Out>
    std::pair<In, Out> ltr(In begin, In end, Out out)
    {
        return combine_sort_impl(
            *make_consumer_iterator(begin, end, begin, combiner()),
            out
        );
    }
    
    template<typename In, typename Out>
    std::pair<In, Out> rtl(In begin, In end, Out out)
    {
        std::pair<
            reverse_iterator<In>,
            Out
        > p = combine_sort_impl(
            make_reversed_range(
                *boost::prior(
                    make_consumer_iterator(begin, end, end, combiner())
                )
            ),
            out
        );
        
        return std::make_pair(p.first.base(), p.second);
    }
    
private:
    template<typename Range, typename Out>
    std::pair<typename range_iterator<const Range>::type, Out> combine_sort_impl(const Range& range, Out out)
    {
        return std::make_pair(boost::end(range), combine_sort_impl(boost::begin(range), boost::end(range), out));
    }

    template<typename In, typename Out>
    typename enable_if<
        is_base_of<
            std::random_access_iterator_tag,
            typename std::iterator_traits<Out>::iterator_category
        >,
        Out
    >::type
    combine_sort_impl(In begin, In end, Out out)
    {
        Out out_pos = detail::copy_bounded(begin, end, out, out + max_output::value);
        detail::stable_sort_bounded<max_output>(out, out_pos, detail::combining_pred());
        return out_pos;
    }
    
    template<typename In, typename Out>
    typename disable_if<
        is_base_of<
            std::random_access_iterator_tag,
            typename std::iterator_traits<Out>::iterator_category
        >,
        Out
    >::type
    combine_sort_impl(In begin, In end, Out out)
    {
        char32 buffer[max_output::value];
        char32* out_pos = detail::copy_bounded(begin, end, buffer, buffer + max_output::value);
        detail::stable_sort_bounded<max_output>(buffer, out_pos, detail::combining_pred());
        return std::copy(buffer, out_pos, out);
    }
};

#ifdef BOOST_UNICODE_DOXYGEN_INVOKED
/** Turns a range of code points into a range of subranges of code points,
 * each subrange being a combining character sequence. */
template<typename Range>
iterator_range<
    consumer_iterator<typename range_iterator<Range>::type, combiner>
>
combine_bounded(Range&& range);
#else
template<typename Range>
iterator_range<
    consumer_iterator<typename range_iterator<const Range>::type, combiner>
>
combine_bounded(const Range& range)
{
    return consumed(range, combiner());
}

template<typename Range>
iterator_range<
    consumer_iterator<typename range_iterator<Range>::type, combiner>
>
combine_bounded(Range& range)
{
    return consumed(range, combiner());
}
#endif
    
} // namespace unicode
} // namespace boost

#endif
