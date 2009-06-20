#ifndef BOOST_CONSUMER_ITERATOR_HPP
#define BOOST_CONSUMER_ITERATOR_HPP

#include <boost/iterator/iterator_facade.hpp>
#include <boost/iterator/dummy_output_iterator.hpp>

#include <boost/range.hpp>

namespace boost
{
    
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
    
	consumer_iterator(It begin_, It pos_, It end_, Consumer p_) : pos(pos_), begin(begin_), end(end_), p(p_)
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

template<typename It, typename P>
consumer_iterator<It, P> make_consumer_iterator(It begin, It pos, It end, P p)
{
	return consumer_iterator<It, P>(begin, pos, end, p);
}

template<typename Range, typename P>
boost::iterator_range<
	consumer_iterator<typename boost::range_iterator<const Range>::type, P>
> consumed(const Range& range, P p)
{
	return boost::make_iterator_range(
		make_consumer_iterator(boost::begin(range), boost::begin(range), boost::end(range), p),
		make_consumer_iterator(boost::begin(range), boost::end(range), boost::end(range), p)
	);
}
    
} // namespace boost

#endif
