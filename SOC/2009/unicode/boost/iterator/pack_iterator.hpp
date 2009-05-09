#ifndef BOOST_PACK_ITERATOR_HPP
#define BOOST_BACK_ITERATOR_HPP

#include <boost/iterator/iterator_facade.hpp>
#include <iterator>
#include <vector>
#include <utility>
#include <boost/range.hpp>

#include <iostream>

namespace boost
{

template<typename P>
struct packer
{
	packer(P p_) : p(p_)
	{
	}
	
	typedef typename P::output_type output_type;
	
	template<typename Range, typename It>
	std::pair<
		typename boost::range_iterator<Range>::type,
		It
	>
	left(const Range& in, It out)
	{
		assert(!boost::empty(in));
		
		p(*boost::begin(in), out);
		return std::make_pair(++boost::begin(in), out);
	}
	
	template<typename Range, typename OutputIterator>
	std::pair<
		typename boost::range_iterator<Range>::type,
		OutputIterator
	>
	right(const Range& in, OutputIterator out)
	{
		assert(!boost::empty(in));
		
		p(*--boost::end(in), out);
		return std::make_pair(--boost::end(in), out);
	}
	
private:
	P p;
};

template<typename P>
packer<P> make_packer(P p)
{
	return packer<P>(p);
}

template<typename It, typename Packer>
struct pack_iterator
	: boost::iterator_facade<
		pack_iterator<It, Packer>,
		typename Packer::output_type,
		std::bidirectional_iterator_tag,
		const typename Packer::output_type
	>
{
	typedef boost::iterator_facade<
		pack_iterator<It, Packer>,
		typename Packer::output_type,
		std::bidirectional_iterator_tag,
		const typename Packer::output_type
	> base_type;
	
	pack_iterator(It begin_, It pos_, It end_, Packer p_) : pos(pos_), begin(begin_), end(end_), index(0), p(p_)
	{
		if(pos != end)	
			next_pos = p.left(std::make_pair(pos, end), std::back_inserter(values)).first;
	}
	
	It base() const
	{
		return pos;
	}
	
private:
	typedef typename Packer::output_type T;

	friend class boost::iterator_core_access;

	typename base_type::reference dereference() const
	{
		return values[index];
	}
	
	void increment()
	{
		if(index != values.size()-1)
		{
			index++;
		}
		else
		{
			values.clear();
			
			pos = next_pos;	
			if(pos != end)
				next_pos = p.left(std::make_pair(pos, end), std::back_inserter(values)).first;
			index = 0;
		}
	}
	
	void decrement()
	{
		if(index != 0)
		{
			index--;
		}
		else
		{	
			values.clear();
			
			next_pos = pos;	
			pos = p.right(std::make_pair(begin, pos), std::back_inserter(values)).first;
			index = values.size()-1;
		}
	}
	
	bool equal(const pack_iterator& other) const
	{
		return pos == other.pos && index == other.index;
	}
	
	It pos;
	It next_pos;
	
	It begin;
	It end;
	size_t index;
	
	Packer p;
	
	std::vector<T> values;
};

template<typename It, typename P>
pack_iterator<It, P> make_pack_iterator(It begin, It pos, It end, P p)
{
	return pack_iterator<It, P>(begin, pos, end, p);
}

template<typename Range, typename P>
std::pair<
	pack_iterator<typename boost::range_iterator<const Range>::type, P>,
	pack_iterator<typename boost::range_iterator<const Range>::type, P>
> make_pack_range(const Range& range, P p)
{
	return std::make_pair(
		make_pack_iterator(boost::begin(range), boost::begin(range), boost::end(range), p),
		make_pack_iterator(boost::begin(range), boost::end(range), boost::end(range), p)
	);
}

} // namespace boost

#endif
