#ifndef BOOST_UNICODE_GRAPHEMES_HPP
#define BOOST_UNICODE_GRAPHEMES_HPP

#include <boost/unicode/ucd/properties.hpp>
#include <boost/utility.hpp>

#include <boost/iterator/consumer_iterator.hpp>

#include <boost/unicode/utf_codecs.hpp>

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

/** Model of \c BoundaryChecker that tells whether a position lies on a
 * grapheme cluster boundary within a range of code points. */
struct grapheme_boundary
{
    template<typename Iterator>
    bool operator()(Iterator begin, Iterator end, Iterator pos)
    {
        return detail::grapheme_is_break
            [ucd::get_grapheme_cluster_break(*boost::prior(pos))]
            [ucd::get_grapheme_cluster_break(*pos)];
    }
};

} // unicode

/** Adapts the range of code points \c range into a range of ranges of code points,
 * each subrange being a grapheme cluster. */
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

/** Adapts the range of UTF-8 code units \c range into a range of ranges of UTF-8 code units,
 * each subrange being a grapheme cluster. */
template<typename Range>
iterator_range<
    consumer_iterator<
        typename range_iterator<const Range>::type,
        piped_consumer<
            unicode::u8_decoder,
            boundary_consumer<unicode::grapheme_boundary>
        >
    >
>
u8_grapheme_bounded(const Range& range)
{
    return consumed(range, make_piped_consumer(unicode::u8_decoder(), make_boundary_consumer(unicode::grapheme_boundary())));
}

/** Adapts the range of UTF-16 code units \c range into a range of ranges of UTF-16 code units,
 * each subrange being a grapheme cluster. */
template<typename Range>
iterator_range<
    consumer_iterator<
        typename range_iterator<const Range>::type,
        piped_consumer<
            unicode::u16_decoder,
            boundary_consumer<unicode::grapheme_boundary>
        >
    >
>
u16_grapheme_bounded(const Range& range)
{
    return consumed(range, make_piped_consumer(unicode::u16_decoder(), make_boundary_consumer(unicode::grapheme_boundary())));
}

} // boost

#endif
