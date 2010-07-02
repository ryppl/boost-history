#ifndef BOOST_UNICODE_GRAPHEMES_HPP
#define BOOST_UNICODE_GRAPHEMES_HPP

#include <boost/unicode/ucd/properties.hpp>
#include <boost/utility.hpp>
#include <boost/detail/unspecified.hpp>

#include <boost/iterator/segment_iterator.hpp>

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

/** Model of \c \xmlonly<conceptname>BoundaryChecker</conceptname>\endxmlonly that tells whether a position lies on a
 * grapheme cluster boundary within a range of code points. */
struct grapheme_boundary
{
    typedef char32 input_type;
    
    template<typename Iterator>
    bool operator()(Iterator begin, Iterator end, Iterator pos)
    {
        return detail::grapheme_is_break
            [ucd::get_grapheme_cluster_break(*boost::prior(pos))]
            [ucd::get_grapheme_cluster_break(*pos)];
    }
};

#ifdef BOOST_UNICODE_DOXYGEN_INVOKED
/** Adapts the range of code points \c range into a range of ranges of code points,
 * each subrange being a grapheme cluster. */
template<typename Range>
detail::unspecified<void> grapheme_bounded(Range&& range);
#else
template<typename Range>
iterator_range<typename boost::detail::unspecified<
    segment_iterator<
        typename range_iterator<const Range>::type,
        boundary_segmenter<unicode::grapheme_boundary>
    >
>::type>
grapheme_bounded(const Range& range)
{
    return segmented(range, make_boundary_segmenter(unicode::grapheme_boundary()));
}

template<typename Range>
iterator_range<typename boost::detail::unspecified<
    segment_iterator<
        typename range_iterator<Range>::type,
        boundary_segmenter<unicode::grapheme_boundary>
    >
>::type>
grapheme_bounded(Range& range)
{
    return segmented(range, make_boundary_segmenter(unicode::grapheme_boundary()));
}
#endif

#ifdef BOOST_UNICODE_DOXYGEN_INVOKED
/** INTERNAL ONLY */
#define BOOST_UNICODE_GRAPHEME_BOUNDED_DEF(Name)                       \
/** Adapts the range of Name units \c range into a range of ranges of
Name units, each subrange being a grapheme cluster. */                 \
template<typename Range>                                               \
detail::unspecified<void> Name##_grapheme_bounded(Range&& range);      \
                                                                       \
/** Model of \c \xmlonly<conceptname>BoundaryChecker</conceptname>\endxmlonly
that tells whether a position lies on a grapheme cluster boundary
within a range of Name units. */                                       \
typedef multi_boundary<                                                \
    Name##_boundary, Name##_decoder,                                   \
    grapheme_boundary                                                  \
> Name##_grapheme_boundary;            
#else
#define BOOST_UNICODE_GRAPHEME_BOUNDED_DEF(Name)                       \
template<typename Range>                                               \
iterator_range<typename boost::detail::unspecified<                    \
    segment_iterator<                                                  \
        typename range_iterator<const Range>::type,                    \
        converted_segmenter<                                           \
            unicode::Name##_decoder,                                   \
            boundary_segmenter<unicode::grapheme_boundary>             \
        >                                                              \
    >                                                                  \
>::type>                                                               \
Name##_grapheme_bounded(const Range& range)                            \
{                                                                      \
    return segmented(                                                  \
        range,                                                         \
        make_converted_segmenter(                                      \
            unicode::Name##_decoder(),                                 \
            make_boundary_segmenter(unicode::grapheme_boundary())      \
        )                                                              \
    );                                                                 \
}                                                                      \
                                                                       \
template<typename Range>                                               \
iterator_range<typename boost::detail::unspecified<                    \
    segment_iterator<                                                  \
        typename range_iterator<Range>::type,                          \
        converted_segmenter<                                           \
            unicode::Name##_decoder,                                   \
            boundary_segmenter<unicode::grapheme_boundary>             \
        >                                                              \
    >                                                                  \
>::type>                                                               \
Name##_grapheme_bounded(Range& range)                                  \
{                                                                      \
    return segmented(                                                  \
        range,                                                         \
        make_converted_segmenter(                                      \
            unicode::Name##_decoder(),                                 \
            make_boundary_segmenter(unicode::grapheme_boundary())      \
        )                                                              \
    );                                                                 \
}                                                                      \
                                                                       \
typedef multi_boundary<                                                \
    Name##_boundary, Name##_decoder,                                   \
    grapheme_boundary                                                  \
> Name##_grapheme_boundary;                                            
#endif

BOOST_UNICODE_GRAPHEME_BOUNDED_DEF(u16)
BOOST_UNICODE_GRAPHEME_BOUNDED_DEF(u8)
BOOST_UNICODE_GRAPHEME_BOUNDED_DEF(utf)

} // unicode
} // boost

#endif
