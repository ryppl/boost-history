#ifndef BOOST_ITERATOR_PIPE_ITERATOR_HPP
#define BOOST_ITERATOR_PIPE_ITERATOR_HPP

#include <boost/assert.hpp>
#include <utility>

#include <boost/range.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/times.hpp>
#include <boost/tuple/tuple.hpp>

#include <boost/concept/requires.hpp>
#include <boost/range/concepts.hpp>

#include <boost/iterator/pipe_iterator_fwd.hpp>

#include <boost/type_traits.hpp>
#include <boost/mpl/logical.hpp>
#include <boost/utility/enable_if.hpp>

#include <boost/mpl/assert.hpp>
#include <boost/mpl/equal_to.hpp>

namespace boost
{

/** CRTP Utility to define a \c \xmlonly<conceptname>OneManyPipe</conceptname>\endxmlonly. */
template<typename OneManyPipe>
struct one_many_pipe
{
	template<typename In, typename Out>
	std::pair<In, Out>
	ltr(In begin, In end, Out out)
	{
		BOOST_ASSERT(begin != end);
		
		out = static_cast<OneManyPipe&>(*this)(*begin, out);
		return std::make_pair(++begin, out);
	}
	
	template<typename In, typename Out>
	std::pair<In, Out>
	rtl(In begin, In end, Out out)
	{
		BOOST_ASSERT(begin != end);
		
		out = static_cast<OneManyPipe&>(*this)(*--end, out);
		return std::make_pair(end, out);
	}
};

/* TODO: Make it work for types that don't expose max_output */
/** Model of \c \xmlonly<conceptname>Pipe</conceptname>\endxmlonly
 * constructed from two models of \c \xmlonly<conceptname>Pipe</conceptname>\endxmlonly
 * and that applies one after the other.
 * 
 * The second pipe must require less input than the output of the first per
 * step for it to work. */
template<typename P1, typename P2>
struct multi_pipe
{
    BOOST_CONCEPT_ASSERT((PipeConcept<P1>));
    BOOST_CONCEPT_ASSERT((PipeConcept<P2>));
    
    BOOST_CONCEPT_ASSERT((Convertible<typename P1::output_type, typename P2::input_type>));
    
    typedef typename P1::input_type input_type;
    typedef typename P2::output_type output_type;
    
    typedef typename mpl::times<
        typename P1::max_output,
        typename P2::max_output
    >::type max_output;
    
    multi_pipe() {}
    multi_pipe(P1 p1_, P2 p2_ = P2()) : p1(p1_), p2(p2_) {}
    
    template<typename In, typename Out>
    typename enable_if<
        mpl::and_<
            is_base_of<
                std::forward_iterator_tag,
                typename std::iterator_traits<Out>::iterator_category
            >,
            is_same<
                typename P1::output_type,
                typename P2::output_type
            >
        >,
        std::pair<In, Out>
    >::type
    ltr(In begin, In end, Out out)
    {
        Out b = out;
        
        std::pair<In, Out> pair = p1.ltr(begin, end, out);
        Out e = pair.second;
        
        do
        {
            tie(b, out) = p2.ltr(b, e, out);
        }
        while(b != e);
        
        return std::make_pair(pair.first, out);
    }
    
    template<typename In, typename Out>
    typename disable_if<
        mpl::and_<
            is_base_of<
                std::forward_iterator_tag,
                typename std::iterator_traits<Out>::iterator_category
            >,
            is_same<
                typename P1::output_type,
                typename P2::output_type
            >
        >,
        std::pair<In, Out>
    >::type
    ltr(In begin, In end, Out out)
    {
        typename P1::output_type buf[max_output::value];
        typename P1::output_type* b = buf;
        
        std::pair<In, typename P1::output_type*> pair = p1.ltr(begin, end, buf);
        typename P1::output_type* e = pair.second;
        
        do
        {
            tie(b, out) = p2.ltr(b, e, out);
        }
        while(b != e);
        
        return std::make_pair(pair.first, out);
    }
    
    template<typename In, typename Out>
    typename enable_if<
        mpl::and_<
            is_base_of<
                std::forward_iterator_tag,
                typename std::iterator_traits<Out>::iterator_category
            >,
            is_same<
                typename P1::output_type,
                typename P2::output_type
            >
        >,
        std::pair<In, Out>
    >::type rtl(In begin, In end, Out out)
    {
        Out b = out;
        
        std::pair<In, Out> pair = p1.rtl(begin, end, out);
        Out e = pair.second;
        
        do
        {
            tie(b, out) = p2.ltr(b, e, out);
        }
        while(b != e);
        
        return std::make_pair(pair.first, out);
    }
    
    template<typename In, typename Out>
    typename disable_if<
        mpl::and_<
            is_base_of<
                std::forward_iterator_tag,
                typename std::iterator_traits<Out>::iterator_category
            >,
            is_same<
                typename P1::output_type,
                typename P2::output_type
            >
        >,
        std::pair<In, Out>
    >::type rtl(In begin, In end, Out out)
    {
        typename P1::output_type buf[max_output::value];
        typename P1::output_type* b = buf;
        
        std::pair<In, typename P1::output_type*> pair = p1.rtl(begin, end, buf);
        typename P1::output_type* e = pair.second;
        
        do
        {
            tie(b, out) = p2.ltr(b, e, out);
        }
        while(b != e);
        
        return std::make_pair(pair.first, out);
    }
    
private:
    P1 p1;
    P2 p2;
};

template<typename P1, typename P2>
BOOST_CONCEPT_REQUIRES(
    ((PipeConcept<P1>))
    ((PipeConcept<P2>))
    ((Convertible<typename P1::output_type, typename P2::input_type>)),
    (multi_pipe<P1, P2>)
) make_multi_pipe(P1 p1, P2 p2)
{
    return multi_pipe<P1, P2>(p1, p2);
}

/** Model of \c \xmlonly<conceptname>Pipe</conceptname>\endxmlonly
 * that adapts the elements another \c \xmlonly<conceptname>Pipe</conceptname>\endxmlonly
 * sees with a model of \c \xmlonly<conceptname>Pipe</conceptname>\endxmlonly, assuming its \c max_output is \c 1. */
template<typename P1, typename P2>
struct piped_pipe : P2
{
    BOOST_CONCEPT_ASSERT((PipeConcept<P1>));
    BOOST_CONCEPT_ASSERT((PipeConcept<P2>));
    
    BOOST_CONCEPT_ASSERT((Convertible<typename P1::output_type, typename P2::input_type>));
    
    BOOST_MPL_ASSERT(( mpl::equal_to< typename P1::max_output, mpl::int_<1> > ));
    
    typedef typename P1::input_type input_type;
    typedef typename P2::output_type output_type;
    
    piped_pipe() {}
    piped_pipe(P1 p1_, P2 p2_ = P2()) : P2(p2_), p1(p1_) {}
    
    template<typename In, typename Out>
    std::pair<In, Out> ltr(In begin, In end, Out out)
    {
        std::pair<
            pipe_iterator<In, P1>,
            Out
        > pair = P2::ltr(
            make_pipe_iterator(begin, end, begin, p1),
            make_pipe_iterator(begin, end, end, p1),
            out
        );
        
        return std::make_pair(pair.first.base(), pair.second);
    }
    
    template<typename In, typename Out>
    std::pair<In, Out> rtl(In begin, In end, Out out)
    {
        std::pair<
            pipe_iterator<In, P1>,
            Out
        > pair = P2::rtl(
            make_pipe_iterator(begin, end, begin, p1),
            make_pipe_iterator(begin, end, end, p1),
            out
        );
        return std::make_pair(pair.first.base(), pair.second);
    }
    
private:
    P1 p1;
};

template<typename P1, typename P2>
BOOST_CONCEPT_REQUIRES(
    ((PipeConcept<P1>))
    ((PipeConcept<P2>))
    ((Convertible<typename P1::output_type, typename P2::input_type>)),
    (piped_pipe<P1, P2>)
) make_piped_pipe(P1 p1, P2 p2)
{
    return piped_pipe<P1, P2>(p1, p2);
}



/** Model of \c \xmlonly<conceptname>OneManyPipe</conceptname>\endxmlonly
 * that casts its input to its template parameter and writes it to its output. */
template<typename T>
struct cast_pipe : one_many_pipe< cast_pipe<T> >
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
