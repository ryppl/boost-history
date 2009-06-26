#ifndef BOOST_UNICODE_GRAPHEMES_HPP
#define BOOST_UNICODE_GRAPHEMES_HPP

#include <boost/unicode/ucd/properties.hpp>
#include <boost/utility.hpp>

#include <boost/iterator/consumer_iterator.hpp>

namespace boost
{
namespace unicode
{
    
namespace detail
{
    extern bool grapheme_is_break
        [boost::unicode::ucd::grapheme_cluster_break::_count]
        [boost::unicode::ucd::grapheme_cluster_break::_count];
    
} // namespace detail

struct grapheme_boundary
{
    template<typename Iterator>
    bool operator()(Iterator begin, Iterator end, Iterator pos)
    {
        if(pos == begin || pos == end)
            return true;
            
        return detail::grapheme_is_break
            [ucd::get_grapheme_cluster_break(*boost::prior(pos))]
            [ucd::get_grapheme_cluster_break(*pos)];
    }
};

} // unicode

template<typename Range>
iterator_range<
    consumer_iterator<
        typename range_iterator<const Range>::type,
        boundary_consumer<unicode::grapheme_boundary>
    >
>
grapheme_bounded(const Range& range)
{
    return consumed(range, make_boundary_consumer(unicode::grapheme_boundary()));
}

} // boost

#endif
