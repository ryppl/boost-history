#ifndef BOOST_ITERATOR_PIPE_ITERATOR_FWD_HPP
#define BOOST_ITERATOR_PIPE_ITERATOR_FWD_HPP

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/has_xxx.hpp>

#include <boost/iterator/iterator_facade.hpp>
#include <vector>

#include <boost/iterator/pipe_concept.hpp>

namespace boost
{

namespace detail
{
    BOOST_MPL_HAS_XXX_TRAIT_DEF(max_output)

    template<typename P, typename Enable = void>
    struct pipe_output_storage;

    template<typename P>
    struct pipe_output_storage<P, typename ::boost::disable_if< has_max_output<P> >::type>
    {
        BOOST_CONCEPT_ASSERT((PipeConcept<P>));
private:
        typedef std::vector<typename P::output_type> Values;
public:
        typedef std::back_insert_iterator<Values> output_iterator;
        
        const typename P::output_type& operator[](size_t i) const
        {
            return values[i];
        }
        
        size_t last_index() const
        {
            return values.size() - 1;
        }
        
        output_iterator out()
        {
            values.clear();
            return std::back_inserter(values);
        }
        
        void update(output_iterator)
        {
        }
        
    private:
        Values values;
    };
    
    template<typename P>
    struct pipe_output_storage<P, typename boost::enable_if< has_max_output<P> >::type>
    {
        BOOST_CONCEPT_ASSERT((PipeConcept<P>));
private:
        typedef typename P::output_type Value;
public:
        typedef Value* output_iterator;
        
        const Value& operator[](size_t i) const
        {
            return values[i];
        }
        
        size_t last_index() const
        {
            return last;
        }
        
        output_iterator out()
        {
            return values;
        }
        
        void update(output_iterator u)
        {
            last = u - values - 1;
        }
        
    private:
        Value values[P::max_output::value];
        size_t last;
    };
}

/** Iterator adapter that wraps a range to make it appear like a converted
 * one, by converting it step-by-step as it is advanced. */ 
template<typename It, typename Pipe>
struct pipe_iterator
	: iterator_facade<
		pipe_iterator<It, Pipe>,
		typename Pipe::output_type,
		std::bidirectional_iterator_tag,
		const typename Pipe::output_type
	>
{
    BOOST_CONCEPT_ASSERT((InputIterator<It>));
    BOOST_CONCEPT_ASSERT((PipeConcept<Pipe>));
    
    BOOST_CONCEPT_ASSERT((Convertible<typename InputIterator<It>::value_type, typename Pipe::input_type>));
    
    pipe_iterator() {} // singular
    
	pipe_iterator(It begin_, It end_, It pos_, Pipe p_) : pos(pos_), begin(begin_), end(end_), index(0), p(p_)
	{
		if(pos != end)
        {
            std::pair<It, typename detail::pipe_output_storage<Pipe>::output_iterator> pair =
                p.ltr(pos, end, values.out());
            next_pos = pair.first;
            values.update(pair.second);
        }
	}
	
	It base() const
	{
        BOOST_CONCEPT_ASSERT((ForwardIterator<It>));
        
		return pos;
	}

private:
	typedef typename Pipe::output_type T;
	friend class boost::iterator_core_access;

	T dereference() const
	{
		return values[index];
	}
	
	void increment()
	{
		if(index != values.last_index())
		{
			index++;
		}
		else
		{			
			pos = next_pos;	
			if(pos != end)
            {
                std::pair<It, typename detail::pipe_output_storage<Pipe>::output_iterator> pair =
				    p.ltr(pos, end, values.out());
                next_pos = pair.first;
                values.update(pair.second);
            }
			index = 0;
		}
	}
	
	void decrement()
	{
        BOOST_CONCEPT_ASSERT((BidirectionalIterator<It>));
        
		if(index != 0)
		{
			index--;
		}
		else
		{
			next_pos = pos;	
            
            std::pair<It, typename detail::pipe_output_storage<Pipe>::output_iterator> pair =
                p.rtl(begin, pos, values.out());
            pos = pair.first;
            values.update(pair.second);
            
			index = values.last_index();
		}
	}
	
	bool equal(const pipe_iterator& other) const
	{
		return pos == other.pos && index == other.index;
	}
	
	It pos;
	It next_pos;
	
	It begin;
	It end;
	size_t index;
	
	Pipe p;
	
	detail::pipe_output_storage<Pipe> values;
};

/** Output Iterator adapter that wraps an output iterator to make one
 * that will convert its output before pushing it to the wrapped iterator. */
template<typename OutputIterator, typename OneManyPipe>
struct pipe_output_iterator
{
    BOOST_CONCEPT_ASSERT((OutputIteratorConcept<OutputIterator, typename OneManyPipe::output_type>));
    BOOST_CONCEPT_ASSERT((OneManyPipeConcept<OneManyPipe>));
    
    typedef void                                                difference_type;
    typedef void                                                value_type;
    typedef pipe_output_iterator<OutputIterator, OneManyPipe>*  pointer;
    typedef pipe_output_iterator<OutputIterator, OneManyPipe>&  reference;
    typedef std::output_iterator_tag                            iterator_category;

    pipe_output_iterator() {} // singular
    
	pipe_output_iterator(OutputIterator pos_, OneManyPipe p_) : pos(pos_), p(p_)
	{
	}
	
	OutputIterator base() const
	{
		return pos;
	}
	
	const pipe_output_iterator& operator*() const
	{
		return *this;
	}
	
	pipe_output_iterator& operator++()
	{
		return *this;
	}
	
	pipe_output_iterator& operator++(int)
	{
		return *this;
	}
	
    template<typename T>
	void operator=(T val) const
	{
		pos = p(val, pos);
	}
    
    bool operator==(const pipe_output_iterator& other) const
    {
        return pos == other.pos;
    }
    
    bool operator!=(const pipe_output_iterator& other) const
    {
        return pos != other.pos;
    }
	
private:	
	mutable OutputIterator pos;
	mutable OneManyPipe p;
};

} // namespace boost

#endif
