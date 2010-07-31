#ifndef BOOST_UNICODE_GRAPHEMES_HPP
#define BOOST_UNICODE_GRAPHEMES_HPP

#include <boost/unicode/ucd/properties.hpp>
#include <boost/unicode/utf_codecs.hpp>

#include <boost/unicode/detail/boundary_segment_def.hpp>
#include <boost/iterator/segment_iterator.hpp>

#include <boost/utility.hpp>

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

BOOST_UNICODE_BOUNDARY_SEGMENT_DEF(u8, grapheme)
BOOST_UNICODE_BOUNDARY_SEGMENT_DEF(u16, grapheme)
BOOST_UNICODE_BOUNDARY_SEGMENT_DEF(utf, grapheme)

} // unicode
} // boost

#endif
