#ifndef BOOST_UNICODE_UTF_HPP
#define BOOST_UNICODE_UTF_HPP

#include <boost/unicode/utf_codecs.hpp>
#include <boost/iterator/convert_iterator.hpp>
#include <boost/iterator/segment_iterator.hpp>

#include <boost/unicode/detail/cat.hpp>

namespace boost
{
namespace unicode
{
/** INTERNAL ONLY */
#define BOOST_UNICODE_ENCODER_DEF(codec)                               \
BOOST_ONEMANYCONVERTER_DEF(BOOST_UNICODE_CAT(boost::unicode, codec##_encoder), codec##_encode)

/** INTERNAL ONLY */
#define BOOST_UNICODE_DECODER_DEF(codec)                               \
BOOST_CONVERTER_DEF(BOOST_UNICODE_CAT(boost::unicode, codec##_decoder), codec##_decode)\
typedef boost::detail::unspecified<                                    \
    boost::boundary_segmenter<codec##_boundary>                        \
>::type codec##_segmenter;                                             \
BOOST_SEGMENTER_DEF(BOOST_UNICODE_CAT(boost::unicode, codec##_segmenter), codec##_segment)

BOOST_UNICODE_ENCODER_DEF(u8)
BOOST_UNICODE_ENCODER_DEF(u16)
BOOST_UNICODE_ENCODER_DEF(latin1)

BOOST_UNICODE_DECODER_DEF(u8)
BOOST_UNICODE_DECODER_DEF(u16)
BOOST_UNICODE_DECODER_DEF(utf)

namespace adaptors
{
    template<typename ValueType, typename OutputIterator>
    boost::convert_output_iterator<
        OutputIterator,
        boost::unicode::utf_encoder<ValueType>
    > utf_encode_output(OutputIterator out)
    {
        return boost::adaptors::convert_output(out, utf_encoder<ValueType>());
    }
}

} // namespace unicode
} // namespace boost

#endif
