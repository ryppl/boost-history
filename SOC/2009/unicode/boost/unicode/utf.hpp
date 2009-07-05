#ifndef BOOST_UNICODE_UTF_HPP
#define BOOST_UNICODE_UTF_HPP

#include <boost/unicode/utf_codecs.hpp>
#include <boost/iterator/pipe_iterator.hpp>
#include <boost/iterator/consumer_iterator.hpp>

namespace boost
{

/** INTERNAL ONLY */
#define BOOST_UNICODE_ONE_MANY_PIPE_FUNC_DEF(Action)                   \
/** Eagerly evaluates unicode::Action##r until the whole input range
   \c range has been treated, copying the result to \c out and
   returning the past-the-end output iterator */                       \
template<typename Range, typename OutputIterator>                      \
OutputIterator Action(const Range& range, OutputIterator out)          \
{                                                                      \
    return pipe(range, make_one_many_pipe(unicode::Action##r()), out); \
}                                                                      \
                                                                       \
/** Lazily evalutes unicode::Action##r by returning a range adapter
   that wraps the range \c range and converts it step-by-step as
   the range is advanced */                                            \
template<typename Range>                                               \
iterator_range<                                                        \
    pipe_iterator<                                                     \
        typename range_iterator<const Range>::type,                    \
        one_many_pipe<unicode::Action##r>                              \
    >                                                                  \
>                                                                      \
Action##d(const Range& range)                                          \
{                                                                      \
    return piped(range, make_one_many_pipe(unicode::Action##r()));     \
}                                                                      \
                                                                       \
/** Lazily evalutes unicode::Action##r by returning an output iterator
  that wraps \c out and converts every pushed element. */              \
template<typename OutputIterator>                                      \
pipe_output_iterator<                                                  \
    OutputIterator,                                                    \
    unicode::Action##r                                                 \
> Action##d_out(OutputIterator out)                                    \
{                                                                      \
	return piped_output(out, unicode::Action##r());                    \
}                                                                      \

/** INTERNAL ONLY */
#define BOOST_UNICODE_PIPE_FUNC_DEF(Action)                            \
/** Eagerly evaluates unicode::Action##r until the whole input range
   \c range has been treated, copying the result to \c out and
   returning the past-the-end output iterator */                       \
template<typename Range, typename OutputIterator>                      \
OutputIterator Action(const Range& range, OutputIterator out)          \
{                                                                      \
    return pipe(range, unicode::Action##r(), out);                     \
}                                                                      \
                                                                       \
/** Lazily evalutes unicode::Action##r by returning a range adapter
   that wraps the range \c range and converts it step-by-step as
   the range is advanced */                                            \
template<typename Range>                                               \
iterator_range<                                                        \
    pipe_iterator<                                                     \
        typename range_iterator<const Range>::type,                    \
        unicode::Action##r                                             \
    >                                                                  \
>                                                                      \
Action##d(const Range& range)                                          \
{                                                                      \
    return piped(range, unicode::Action##r());                         \
}                                                                      \

BOOST_UNICODE_ONE_MANY_PIPE_FUNC_DEF(u16_encode)
BOOST_UNICODE_PIPE_FUNC_DEF(u16_decode)

BOOST_UNICODE_ONE_MANY_PIPE_FUNC_DEF(u8_encode)
BOOST_UNICODE_PIPE_FUNC_DEF(u8_decode)

/** Adapts the range of UTF-16 code units \c range into a range of ranges of UTF-16 code units,
 * each subrange being a code point. */
template<typename Range>
iterator_range<
    consumer_iterator<
        typename range_iterator<const Range>::type,
        pipe_consumer<unicode::u16_decoder>
    >
> u16_bounded(const Range& range)
{
    return consumed(range, make_pipe_consumer(unicode::u16_decoder()));
}

/** Adapts the range of UTF-8 code units \c range into a range of ranges of UTF-8 code units,
 * each subrange being a code point. */
template<typename Range>
iterator_range<
    consumer_iterator<
        typename range_iterator<const Range>::type,
        pipe_consumer<unicode::u8_decoder>
    >
> u8_bounded(const Range& range)
{
    return consumed(range, make_pipe_consumer(unicode::u8_decoder()));
}

} // namespace boost

#endif
