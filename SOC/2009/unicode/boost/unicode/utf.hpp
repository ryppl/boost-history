#ifndef BOOST_UNICODE_UTF_HPP
#define BOOST_UNICODE_UTF_HPP

#include <boost/unicode/utf_codecs.hpp>
#include <boost/iterator/pipe_iterator.hpp>
#include <boost/iterator/consumer_iterator.hpp>

#include <boost/detail/unspecified.hpp>

namespace boost
{
namespace unicode
{

/** INTERNAL ONLY */
#define BOOST_UNICODE_ENCODER_DEF(Name)                                \
/** Eagerly evaluates unicode::Name##_encoder until the whole input
   range \c range has been treated, copying the result to \c out and
   returning the past-the-end output iterator */                       \
template<typename Range, typename OutputIterator>                      \
OutputIterator Name##_encode(const Range& range, OutputIterator out)   \
{                                                                      \
    return pipe(                                                       \
        range,                                                         \
        make_one_many_pipe(unicode::Name##_encoder()), out             \
    );                                                                 \
}                                                                      \
                                                                       \
/** Lazily evalutes unicode::Name##_encoder by returning a range adapter
   that wraps the range \c range and converts it step-by-step as
   the range is advanced */                                            \
template<typename Range>                                               \
iterator_range<typename boost::detail::unspecified<                    \
    pipe_iterator<                                                     \
        typename range_iterator<const Range>::type,                    \
        one_many_pipe<unicode::Name##_encoder>                         \
    >                                                                  \
>::type>                                                               \
Name##_encoded(const Range& range)                                     \
{                                                                      \
    return piped(range, make_one_many_pipe(unicode::Name##_encoder()));\
}                                                                      \
                                                                       \
/** Lazily evalutes unicode::Name##_encoder by returning a range adapter
   that wraps the range \c range and converts it step-by-step as
   the range is advanced */                                            \
template<typename Range>                                               \
iterator_range<typename boost::detail::unspecified<                    \
    pipe_iterator<                                                     \
        typename range_iterator<Range>::type,                          \
        one_many_pipe<unicode::Name##_encoder>                         \
    >                                                                  \
>::type>                                                               \
Name##_encoded(Range& range)                                           \
{                                                                      \
    return piped(range, make_one_many_pipe(unicode::Name##_encoder()));\
}                                                                      \
                                                                       \
/** Lazily evalutes unicode::Name##_encoder by returning an output
  iterator that wraps \c out and converts every pushed element. */     \
template<typename OutputIterator>                                      \
typename boost::detail::unspecified<pipe_output_iterator<              \
    OutputIterator,                                                    \
    unicode::Name##_encoder                                            \
> >::type Name##_encoded_out(OutputIterator out)                       \
{                                                                      \
	return piped_output(out, unicode::Name##_encoder());               \
}                                                                      \


/* */

/** INTERNAL ONLY */
#define BOOST_UNICODE_DECODER_DEF(Name)                                \
/** Eagerly evaluates unicode::Name##_decoder until the whole input
   range \c range has been treated, copying the result to \c out and
   returning the past-the-end output iterator */                       \
template<typename Range, typename OutputIterator>                      \
OutputIterator Name##_decode(const Range& range, OutputIterator out)   \
{                                                                      \
    return pipe(range, unicode::Name##_decoder(), out);                \
}                                                                      \
                                                                       \
/** Lazily evalutes unicode::Name##_decoder by returning a range adapter
   that wraps the range \c range and converts it step-by-step as
   the range is advanced */                                            \
template<typename Range>                                               \
iterator_range<typename boost::detail::unspecified<                    \
    pipe_iterator<                                                     \
        typename range_iterator<const Range>::type,                    \
        unicode::Name##_decoder                                        \
    >                                                                  \
>::type>                                                               \
Name##_decoded(const Range& range)                                     \
{                                                                      \
    return piped(range, unicode::Name##_decoder());                    \
}                                                                      \
                                                                       \
/** Lazily evalutes unicode::Name##_decoder by returning a range adapter
   that wraps the range \c range and converts it step-by-step as
   the range is advanced */                                            \
template<typename Range>                                               \
iterator_range<typename boost::detail::unspecified<                    \
    pipe_iterator<                                                     \
        typename range_iterator<Range>::type,                          \
        unicode::Name##_decoder                                        \
    >                                                                  \
>::type>                                                               \
Name##_decoded(Range& range)                                           \
{                                                                      \
    return piped(range, unicode::Name##_decoder());                    \
}                                                                      \
                                                                       \
/** Adapts the range of Name units \c range into a range of ranges of
Name units, each subrange being a decoded unit. */                     \
template<typename Range>                                               \
iterator_range<typename boost::detail::unspecified<                    \
    consumer_iterator<                                                 \
        typename range_iterator<const Range>::type,                    \
        pipe_consumer<unicode::Name##_decoder>                         \
    >                                                                  \
>::type> Name##_bounded(const Range& range)                            \
{                                                                      \
    return consumed(                                                   \
        range,                                                         \
        make_pipe_consumer(unicode::Name##_decoder())                  \
    );                                                                 \
}                                                                      \
                                                                       \
/** Adapts the range of Name units \c range into a range of ranges of
Name units, each subrange being a decoded unit. */                     \
template<typename Range>                                               \
iterator_range<typename boost::detail::unspecified<                    \
    consumer_iterator<                                                 \
        typename range_iterator<Range>::type,                          \
        pipe_consumer<unicode::Name##_decoder>                         \
    >                                                                  \
>::type> Name##_bounded(Range& range)                                  \
{                                                                      \
    return consumed(                                                   \
        range,                                                         \
        make_pipe_consumer(unicode::Name##_decoder())                  \
    );                                                                 \
}                                                                      \

BOOST_UNICODE_ENCODER_DEF(u16)
BOOST_UNICODE_DECODER_DEF(u16)

BOOST_UNICODE_ENCODER_DEF(u8)
BOOST_UNICODE_DECODER_DEF(u8)

BOOST_UNICODE_DECODER_DEF(utf)

BOOST_UNICODE_ENCODER_DEF(latin1);

} // namespace unicode
} // namespace boost

#endif
