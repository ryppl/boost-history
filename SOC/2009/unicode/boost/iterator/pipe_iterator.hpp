#ifndef BOOST_ITERATOR_PIPE_ITERATOR_HPP
#define BOOST_ITERATOR_PIPE_ITERATOR_HPP

#include <boost/assert.hpp>
#include <utility>

#include <boost/range.hpp>
#include <boost/mpl/int.hpp>

#include <boost/concept/requires.hpp>
#include <boost/range/concepts.hpp>

#include <boost/iterator/pipe_iterator_fwd.hpp>

namespace boost
{

/** Model of \c \xmlonly<conceptname>Pipe</conceptname>\endxmlonly
 * constructed from a model of \c \xmlonly<conceptname>OneManyPipe</conceptname>\endxmlonly. */
template<typename OneManyPipe>
struct one_many_pipe : OneManyPipe
{
    BOOST_CONCEPT_ASSERT((OneManyPipeConcept<OneManyPipe>));
    
    one_many_pipe() {} // singular
    
	one_many_pipe(OneManyPipe p_) : OneManyPipe(p_)
	{
	}
	
	template<typename In, typename Out>
	std::pair<In, Out>
	ltr(In begin, In end, Out out)
	{
		BOOST_ASSERT(begin != end);
		
		out = OneManyPipe::operator()(*begin, out);
		return std::make_pair(++begin, out);
	}
	
	template<typename In, typename Out>
	std::pair<In, Out>
	rtl(In begin, In end, Out out)
	{
		BOOST_ASSERT(begin != end);
		
		out = OneManyPipe::operator()(*--end, out);
		return std::make_pair(end, out);
	}
};

template<typename OneManyPipe>
BOOST_CONCEPT_REQUIRES(
    ((OneManyPipeConcept<OneManyPipe>)),
    (one_many_pipe<OneManyPipe>)
) make_one_many_pipe(OneManyPipe p)
{
	return one_many_pipe<OneManyPipe>(p);
}

/** Model of \c \xmlonly<conceptname>OneManyPipe</conceptname>\endxmlonly
 * that casts its input to its template parameter and writes it to its output. */
template<typename T>
struct cast_pipe
{
    typedef T input_type;
    typedef T output_type;
    typedef mpl::int_<1> max_output;
    
    template<typename U, typename Out>
    Out operator()(U in, Out out)
    {
        *out++ = static_cast<output_type>(in);
        return out;
    }
};

template<typename It, typename Pipe>
BOOST_CONCEPT_REQUIRES(
    ((InputIterator<It>))
    ((PipeConcept<Pipe>))
    ((Convertible<typename InputIterator<It>::value_type, typename Pipe::input_type>)),
    (pipe_iterator<It, Pipe>)
) make_pipe_iterator(It begin, It end, It pos, Pipe p)
{
	return pipe_iterator<It, Pipe>(begin, end, pos, p);
}

template<typename Range, typename Pipe>
BOOST_CONCEPT_REQUIRES(
    ((SinglePassRangeConcept<Range>))
    ((PipeConcept<Pipe>))
    ((Convertible<typename range_value<Range>::type, typename Pipe::input_type>)),
    (iterator_range<
	    pipe_iterator<typename range_iterator<const Range>::type, Pipe>
    >)
) piped(const Range& range, Pipe p)
{
	return boost::make_iterator_range(
		make_pipe_iterator(boost::begin(range), boost::end(range), boost::begin(range), p),
		make_pipe_iterator(boost::begin(range), boost::end(range), boost::end(range), p)
	);
}

template<typename Range, typename Pipe>
BOOST_CONCEPT_REQUIRES(
    ((SinglePassRangeConcept<Range>))
    ((PipeConcept<Pipe>))
    ((Convertible<typename range_value<Range>::type, typename Pipe::input_type>)),
    (iterator_range<
	    pipe_iterator<typename range_iterator<Range>::type, Pipe>
    >)
) piped(Range& range, Pipe p)
{
	return boost::make_iterator_range(
		make_pipe_iterator(boost::begin(range), boost::end(range), boost::begin(range), p),
		make_pipe_iterator(boost::begin(range), boost::end(range), boost::end(range), p)
	);
}

template<typename Range, typename Pipe, typename OutputIterator>
BOOST_CONCEPT_REQUIRES(
    ((SinglePassRangeConcept<Range>))
    ((PipeConcept<Pipe>))
    ((Convertible<typename range_value<Range>::type, typename Pipe::input_type>))
    ((OutputIteratorConcept<OutputIterator, typename Pipe::output_type>)),
    (OutputIterator)
) pipe(const Range& range, Pipe pipe, OutputIterator out)
{
    typedef typename boost::range_iterator<const Range>::type Iterator;
    
    Iterator begin = boost::begin(range);
    Iterator end = boost::end(range);
    
    while(begin != end)
    {
        std::pair<Iterator, OutputIterator> p = pipe.ltr(begin, end, out);
        begin = p.first;
        out = p.second;
    }
    
    return out;
}

template<typename OutputIterator, typename OneManyPipe>
BOOST_CONCEPT_REQUIRES(
    ((OneManyPipeConcept<OneManyPipe>))
    ((OutputIteratorConcept<OutputIterator, typename OneManyPipe::output_type>)),
    (pipe_output_iterator<OutputIterator, OneManyPipe>)
) piped_output(OutputIterator out, OneManyPipe p)
{
	return pipe_output_iterator<OutputIterator, OneManyPipe>(out, p);
}

} // namespace boost

#endif
