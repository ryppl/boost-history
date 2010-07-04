#ifndef BOOST_UNICODE_GRAPHEMES_HPP
#define BOOST_UNICODE_GRAPHEMES_HPP

#include <boost/unicode/ucd/properties.hpp>
#include <boost/utility.hpp>
#include <boost/detail/unspecified.hpp>
#include <boost/unicode/detail/cat.hpp>

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

typedef boost::detail::unspecified<
    boost::boundary_segmenter<grapheme_boundary>
>::type grapheme_segmenter;
BOOST_SEGMENTER_DEF(boost::unicode::grapheme_segmenter, grapheme_segment)

/** INTERNAL ONLY */
#define BOOST_UNICODE_GRAPHEME_DEF(codec)                              \
typedef boost::detail::unspecified<                                    \
    boost::multi_boundary<                                             \
        codec##_boundary,                                              \
        codec##_decoder,                                               \
        grapheme_boundary                                              \
    >                                                                  \
>::type codec##_grapheme_boundary;                                     \
typedef boost::detail::unspecified<                                    \
    boost::boundary_segmenter<codec##_grapheme_boundary>               \
>::type codec##_grapheme_segmenter;                                    \
BOOST_SEGMENTER_DEF(BOOST_UNICODE_CAT(boost::unicode, codec##_grapheme_segmenter), codec##_grapheme_segment)

BOOST_UNICODE_GRAPHEME_DEF(u8)
BOOST_UNICODE_GRAPHEME_DEF(u16)
BOOST_UNICODE_GRAPHEME_DEF(utf)

} // unicode
} // boost

#endif
