#ifndef BOOST_CONSUMER_ITERATOR_HPP
#define BOOST_CONSUMER_ITERATOR_HPP

#include <boost/iterator/iterator_facade.hpp>
#include <boost/iterator/dummy_output_iterator.hpp>

#include <boost/range.hpp>

namespace boost
{

/** Model of \c Consumer constructed from a model of \c Pipe */    
template<typename Pipe>
struct pipe_consumer : private Pipe
{
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
pipe_consumer<Pipe> make_pipe_consumer(Pipe p)
{
    return pipe_consumer<Pipe>(p);
}

/** Model of \c Consumer constructed from a model of \c BoundaryChecker */    
template<typename BoundaryChecker>
struct boundary_consumer : private BoundaryChecker
{
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
boundary_consumer<BoundaryChecker> make_boundary_consumer(BoundaryChecker b)
{
    return boundary_consumer<BoundaryChecker>(b);
}

/** Model of \c BoundaryChecker constructed from two models of \c BoundaryChecker
 * with a model of \c Pipe applied in the middle. */    
template<typename B1, typename Pipe, typename B2>
struct multi_boundary
{
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
multi_boundary<B1, Pipe, B2> make_multi_boundary(B1 b1, Pipe p, B2 b2)
{
    return multi_boundary<B1, Pipe, B2>(b1, p, b2);
}

/** Model of \c Consumer that adapts the elements another \c Consumer
 * sees with a model of \c Pipe. */
template<typename Pipe, typename Consumer>
struct piped_consumer
{
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
piped_consumer<Pipe, Consumer> make_piped_consumer(Pipe p, Consumer c)
{
    return piped_consumer<Pipe, Consumer>(p, c);
}

/** Iterator adapter that wraps a range to make it appear like a range
 * of subranges, each subrange being a step of a \c Consumer invocation. */    
template<typename It, typename Consumer>
struct consumer_iterator
	: boost::iterator_facade<
		consumer_iterator<It, Consumer>,
		boost::iterator_range<It>,
		std::bidirectional_iterator_tag,
		const boost::iterator_range<It>
	>
{
    consumer_iterator() {} // singular
    
	consumer_iterator(It begin_, It end_, It pos_, Consumer c_) : pos(pos_), begin(begin_), end(end_), p(c_)
	{
		if(pos != end)
            next_pos = p.ltr(pos, end);
	}
	
	It base() const
	{
		return pos;
	}

private:
	friend class boost::iterator_core_access;

	boost::iterator_range<It> dereference() const
	{
		return boost::make_iterator_range(pos, next_pos);
	}
	
	void increment()
	{
        pos = next_pos;	
		if(pos != end)
            next_pos = p.ltr(pos, end);
	}
	
	void decrement()
	{
        next_pos = pos;	
            
        pos = p.rtl(begin, pos);
	}
	
	bool equal(const consumer_iterator& other) const
	{
		return pos == other.pos;
	}
	
	It pos;
	It next_pos;
	
	It begin;
	It end;
	
	Consumer p;
};

template<typename It, typename Consumer>
consumer_iterator<It, Consumer> make_consumer_iterator(It begin, It end, It pos, Consumer c)
{
	return consumer_iterator<It, Consumer>(begin, end, pos, c);
}

template<typename Range, typename Consumer>
boost::iterator_range<
	consumer_iterator<typename boost::range_iterator<const Range>::type, Consumer>
> consumed(const Range& range, Consumer c)
{
	return boost::make_iterator_range(
		make_consumer_iterator(boost::begin(range), boost::end(range), boost::begin(range), c),
		make_consumer_iterator(boost::begin(range), boost::end(range), boost::end(range), c)
	);
}
    
} // namespace boost

#endif
