#ifndef BOOST_UNICODE_UTF_HPP
#define BOOST_UNICODE_UTF_HPP

#include <boost/unicode/utf_codecs.hpp>
#include <boost/iterator/pipe_iterator.hpp>
#include <boost/iterator/consumer_iterator.hpp>

namespace boost
{

template<typename Range>
boost::iterator_range<
    pipe_iterator<typename range_iterator<const Range>::type, one_many_pipe<unicode::u16_encoder> >
> u16_encoded(const Range& range)
{
	return piped(range, make_one_many_pipe(unicode::u16_encoder()));
}

template<typename Range>
boost::iterator_range<
	pipe_iterator<typename range_iterator<const Range>::type, unicode::u16_decoder>
> u16_decoded(const Range& range)
{
	return piped(range, unicode::u16_decoder());
}

template<typename OutputIterator>
pipe_output_iterator<
    OutputIterator,
    one_many_pipe<unicode::u16_encoder>
> u16_encoded_out(OutputIterator out)
{
	return piped_output(out, make_one_many_pipe(unicode::u16_encoder()));
}

template<typename Range, typename OutputIterator>
OutputIterator u16_encode(const Range& range, OutputIterator out)
{
    return pipe(range, make_one_many_pipe(unicode::u16_encoder()), out);
}

template<typename Range, typename OutputIterator>
OutputIterator u16_decode(const Range& range, OutputIterator out)
{
    return pipe(range, unicode::u16_decoder(), out);
}

template<typename Range>
boost::iterator_range<
    consumer_iterator<
        typename range_iterator<const Range>::type,
        pipe_consumer<unicode::u16_decoder>
    >
> u16_bounded(const Range& range)
{
    return consumed(range, make_pipe_consumer(unicode::u16_decoder()));
}

template<typename Range>
boost::iterator_range<
    pipe_iterator<typename range_iterator<const Range>::type, one_many_pipe<unicode::u8_encoder> >
> u8_encoded(const Range& range)
{
	return piped(range, make_one_many_pipe(unicode::u8_encoder()));
}

template<typename Range>
boost::iterator_range<
	pipe_iterator<typename range_iterator<const Range>::type, unicode::u8_decoder>
> u8_decoded(const Range& range)
{
	return piped(range, unicode::u8_decoder());
}

template<typename OutputIterator>
pipe_output_iterator<
    OutputIterator,
    one_many_pipe<unicode::u8_encoder>
> u8_encoded_out(OutputIterator out)
{
	return piped_output(out, make_one_many_pipe(unicode::u8_encoder()));
}

template<typename Range, typename OutputIterator>
OutputIterator u8_encode(const Range& range, OutputIterator out)
{
    return pipe(range, make_one_many_pipe(unicode::u8_encoder()), out);
}

template<typename Range, typename OutputIterator>
OutputIterator u8_decode(const Range& range, OutputIterator out)
{
    return pipe(range, unicode::u8_decoder(), out);
}

template<typename Range>
boost::iterator_range<
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
