#ifndef BOOST_ITERATOR_CONSUMER_ITERATOR_FWD_HPP
#define BOOST_ITERATOR_CONSUMER_ITERATOR_FWD_HPP

#include <boost/iterator/iterator_facade.hpp>
#include <boost/range.hpp>

#include <boost/iterator/segmenter_concept.hpp>

namespace boost
{

/** Iterator adapter that wraps a range to make it appear like a range
 * of subranges, each subrange being a step of a \c Segmenter invocation. */    
template<typename It, typename Segmenter>
struct segment_iterator
	: iterator_facade<
		segment_iterator<It, Segmenter>,
		iterator_range<It>,
		std::bidirectional_iterator_tag,
		const iterator_range<It>
	>
{
    BOOST_CONCEPT_ASSERT((InputIterator<It>));
    BOOST_CONCEPT_ASSERT((SegmenterConcept<Segmenter>));
    BOOST_CONCEPT_ASSERT((Convertible<typename InputIterator<It>::value_type, typename Segmenter::input_type>));
    
    segment_iterator() {} // singular
    
	segment_iterator(It begin_, It end_, It pos_, Segmenter c_) : pos(pos_), begin(begin_), end(end_), p(c_)
	{
		if(pos != end)
        {
            next_pos = pos;
            p.ltr(next_pos, end);
        }
	}
	
	It base() const
	{
        BOOST_CONCEPT_ASSERT((ForwardIterator<It>));
        
		return pos;
	}

private:
	friend class boost::iterator_core_access;

	iterator_range<It> dereference() const
	{
		return make_iterator_range(pos, next_pos);
	}
	
	void increment()
	{
        pos = next_pos;	
		if(pos != end)
            p.ltr(next_pos, end);
	}
	
	void decrement()
	{
        BOOST_CONCEPT_ASSERT((BidirectionalIterator<It>));
        
        next_pos = pos;	
        p.rtl(begin, pos);
	}
	
	bool equal(const segment_iterator& other) const
	{
		return pos == other.pos;
	}
	
	It pos;
	It next_pos;
	
	It begin;
	It end;
	
	Segmenter p;
};

} // namespace boost

#endif
