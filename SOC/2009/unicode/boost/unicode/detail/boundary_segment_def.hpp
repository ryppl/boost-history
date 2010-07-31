#ifndef BOOST_UNICODE_DETAIL_BOUNDARY_SEGMENT_DEF_HPP
#define BOOST_UNICODE_DETAIL_BOUNDARY_SEGMENT_DEF_HPP

#include <boost/unicode/detail/cat.hpp>

/** INTERNAL ONLY */
#define BOOST_UNICODE_BOUNDARY_SEGMENT_DEF(codec, boundary)            \
typedef boost::detail::unspecified<                                    \
    boost::multi_boundary<                                             \
        codec##_boundary,                                              \
        codec##_decoder,                                               \
        boundary##_boundary                                            \
    >                                                                  \
>::type codec##_##boundary##_boundary;                                 \
typedef boost::detail::unspecified<                                    \
    boost::boundary_segmenter<codec##_##boundary##_boundary>           \
>::type codec##_##boundary##_segmenter;                                \
BOOST_SEGMENTER_DEF(BOOST_UNICODE_CAT(boost::unicode, codec##_##boundary##_segmenter), codec##_##boundary##_segment)

#endif
