#ifndef BOOST_ITERATOR_CONSUMER_ITERATOR_HPP
#define BOOST_ITERATOR_CONSUMER_ITERATOR_HPP

#include <boost/iterator/dummy_output_iterator.hpp>

#include <boost/concept/requires.hpp>
#include <boost/range/concepts.hpp>
#include <boost/iterator/pipe_concept.hpp>

#include <boost/iterator/consumer_iterator_fwd.hpp>

#include <boost/mpl/assert.hpp>
#include <boost/mpl/equal_to.hpp>


namespace boost
{

/** Model of \c \xmlonly<conceptname>Consumer</conceptname>\endxmlonly
 * constructed from a model of \c \xmlonly<conceptname>Pipe</conceptname>\endxmlonly. */    
template<typename Pipe>
struct pipe_consumer : private Pipe
{
    BOOST_CONCEPT_ASSERT((PipeConcept<Pipe>));
    
    typedef typename Pipe::input_type input_type;
    
    pipe_consumer() {} // singular
    
    pipe_consumer(Pipe p_) : Pipe(p_)
    {
    }
    
    template<typename In>
    In ltr(In begin, In end)
    {
        return Pipe::ltr(begin, end, dummy_output_iterator()).first;
    }
    
    template<typename In>
    In rtl(In begin, In end)
    {
        return Pipe::rtl(begin, end, dummy_output_iterator()).first;
    }
};

template<typename Pipe>
BOOST_CONCEPT_REQUIRES(
    ((PipeConcept<Pipe>)),
    (pipe_consumer<Pipe>)
) make_pipe_consumer(Pipe p)
{
    return pipe_consumer<Pipe>(p);
}

/** Model of \c \xmlonly<conceptname>Consumer</conceptname>\endxmlonly
 * constructed from a model of \c \xmlonly<conceptname>BoundaryChecker</conceptname>\endxmlonly. */    
template<typename BoundaryChecker>
struct boundary_consumer : private BoundaryChecker
{
    BOOST_CONCEPT_ASSERT((BoundaryCheckerConcept<BoundaryChecker>));
    
    typedef typename BoundaryChecker::input_type input_type;
    
    boundary_consumer() {} // singular
    
    boundary_consumer(BoundaryChecker b_) : BoundaryChecker(b_)
    {
    }
    
    template<typename In>
    In ltr(In begin, In end)
    {
        In pos = begin;
        do
            ++pos;
        while(pos != end && !BoundaryChecker::operator()(begin, end, pos));
        
        return pos;
    }
    
    template<typename In>
    In rtl(In begin, In end)
    {
        In pos = end;
        do
            --pos;
        while(pos != end && !BoundaryChecker::operator()(begin, end, pos));
        
        return pos;
    }
};

template<typename BoundaryChecker>
BOOST_CONCEPT_REQUIRES(
    ((BoundaryCheckerConcept<BoundaryChecker>)),
    (boundary_consumer<BoundaryChecker>)
) make_boundary_consumer(BoundaryChecker b)
{
    return boundary_consumer<BoundaryChecker>(b);
}

/** Model of \c \xmlonly<conceptname>BoundaryChecker</conceptname>\endxmlonly
 * constructed from two models of \c \xmlonly<conceptname>BoundaryChecker</conceptname>\endxmlonly
 * with a model of \c \xmlonly<conceptname>Pipe</conceptname>\endxmlonly applied in the middle. */    
template<typename B1, typename Pipe, typename B2>
struct multi_boundary
{
    BOOST_CONCEPT_ASSERT((BoundaryCheckerConcept<B1>));
    BOOST_CONCEPT_ASSERT((PipeConcept<Pipe>));
    BOOST_CONCEPT_ASSERT((BoundaryCheckerConcept<B2>));
    
    BOOST_CONCEPT_ASSERT((Convertible<typename B1::input_type, typename Pipe::input_type>));
    BOOST_CONCEPT_ASSERT((Convertible<typename Pipe::output_type, typename B2::input_type>));
    
    typedef typename B1::input_type input_type;
    
    multi_boundary() // singular
    {
    }
    
    multi_boundary(B1 b1_, Pipe p_, B2 b2_) : b1(b1_), p(p_), b2(b2_)
    {
    }
    
    template<typename In>
    bool operator()(In begin, In end, In pos)
    {
        return b1(begin, end, pos)
           &&  b2(
                make_pipe_iterator(begin, end, begin, p),
                make_pipe_iterator(begin, end, end, p),
                make_pipe_iterator(begin, end, pos, p)
               );
    }
    
private:
    B1 b1;
    Pipe p;
    B2 b2;
};

template<typename B1, typename Pipe, typename B2>
BOOST_CONCEPT_REQUIRES(
    ((BoundaryCheckerConcept<B1>))
    ((PipeConcept<Pipe>))
    ((BoundaryCheckerConcept<B2>))
    ((Convertible<typename B1::input_type, typename Pipe::input_type>))
    ((Convertible<typename Pipe::output_type, typename B2::input_type>)),
    (multi_boundary<B1, Pipe, B2>)
) make_multi_boundary(B1 b1, Pipe p, B2 b2)
{
    return multi_boundary<B1, Pipe, B2>(b1, p, b2);
}

/** Model of \c \xmlonly<conceptname>Consumer</conceptname>\endxmlonly
 * that adapts the elements another \c \xmlonly<conceptname>Consumer</conceptname>\endxmlonly
 * sees with a model of \c \xmlonly<conceptname>Pipe</conceptname>\endxmlonly, assuming its \c max_output is \c 1. */
template<typename Pipe, typename Consumer>
struct piped_consumer
{
    BOOST_CONCEPT_ASSERT((PipeConcept<Pipe>));
    BOOST_CONCEPT_ASSERT((ConsumerConcept<Consumer>));
    BOOST_CONCEPT_ASSERT((Convertible<typename Pipe::output_type, typename Consumer::input_type>));
    
    BOOST_MPL_ASSERT(( mpl::equal_to< typename Pipe::max_output, mpl::int_<1> > ));
    
    typedef typename Pipe::input_type input_type;
    
    piped_consumer() // singular
    {
    }
    
    piped_consumer(Pipe p_, Consumer c_) : p(p_), c(c_)
    {
    }
    
    template<typename In>
    In ltr(In begin, In end)
    {
        return c.ltr(
            make_pipe_iterator(begin, end, begin, p),
            make_pipe_iterator(begin, end, end, p)
        ).base();
    }
    
    template<typename In>
    In rtl(In begin, In end)
    {
        return c.rtl(
            make_pipe_iterator(begin, end, begin, p),
            make_pipe_iterator(begin, end, end, p)
        ).base();
    }
    
private:
    Pipe p;
    Consumer c;
};

template<typename Pipe, typename Consumer>
BOOST_CONCEPT_REQUIRES(
    ((PipeConcept<Pipe>))
    ((ConsumerConcept<Consumer>))
    ((Convertible<typename Pipe::output_type, typename Consumer::input_type>)),
    (piped_consumer<Pipe, Consumer>)   
) make_piped_consumer(Pipe p, Consumer c)
{
    return piped_consumer<Pipe, Consumer>(p, c);
}

template<typename It, typename Consumer>
BOOST_CONCEPT_REQUIRES(
    ((InputIterator<It>))
    ((ConsumerConcept<Consumer>))
    ((Convertible<typename InputIterator<It>::value_type, typename Consumer::input_type>)),
    (consumer_iterator<It, Consumer>)
) make_consumer_iterator(It begin, It end, It pos, Consumer c)
{
	return consumer_iterator<It, Consumer>(begin, end, pos, c);
}

template<typename Range, typename Consumer>
BOOST_CONCEPT_REQUIRES(
    ((SinglePassRangeConcept<Range>))
    ((ConsumerConcept<Consumer>))
    ((Convertible<typename range_value<const Range>::type, typename Consumer::input_type>)),
    (iterator_range<
	    consumer_iterator<typename range_iterator<const Range>::type, Consumer>
    >)
) consumed(const Range& range, Consumer c)
{
	return boost::make_iterator_range(
		make_consumer_iterator(boost::begin(range), boost::end(range), boost::begin(range), c),
		make_consumer_iterator(boost::begin(range), boost::end(range), boost::end(range), c)
	);
}

template<typename Range, typename Consumer>
BOOST_CONCEPT_REQUIRES(
    ((SinglePassRangeConcept<Range>))
    ((ConsumerConcept<Consumer>))
    ((Convertible<typename range_value<Range>::type, typename Consumer::input_type>)),
    (iterator_range<
	    consumer_iterator<typename range_iterator<Range>::type, Consumer>
    >)
) consumed(Range& range, Consumer c)
{
	return boost::make_iterator_range(
		make_consumer_iterator(boost::begin(range), boost::end(range), boost::begin(range), c),
		make_consumer_iterator(boost::begin(range), boost::end(range), boost::end(range), c)
	);
}
    
} // namespace boost

#endif
