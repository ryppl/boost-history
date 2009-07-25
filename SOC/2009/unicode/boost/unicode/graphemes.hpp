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
    BOOST_UNICODE_DECL extern bool grapheme_is_break
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

/** INTERNAL ONLY */
#define BOOST_UNICODE_GRAPHEME_BOUNDED_DEF(Name)                       \
/** Adapts the range of X \c range into a range of ranges of X,
 * each subrange being a grapheme cluster. */                          \
template<typename Range>                                               \
iterator_range<                                                        \
    consumer_iterator<                                                 \
        typename range_iterator<const Range>::type,                    \
        piped_consumer<                                                \
            unicode::Name##_decoder,                                   \
            boundary_consumer<unicode::grapheme_boundary>              \
        >                                                              \
    >                                                                  \
>                                                                      \
Name##_grapheme_bounded(const Range& range)                            \
{                                                                      \
    return consumed(                                                   \
        range,                                                         \
        make_piped_consumer(                                           \
            unicode::Name##_decoder(),                                 \
            make_boundary_consumer(unicode::grapheme_boundary())       \
        )                                                              \
    );                                                                 \
}                                                                      \

BOOST_UNICODE_GRAPHEME_BOUNDED_DEF(u16)
BOOST_UNICODE_GRAPHEME_BOUNDED_DEF(u8)
BOOST_UNICODE_GRAPHEME_BOUNDED_DEF(utf)

} // unicode
} // boost

#endif
