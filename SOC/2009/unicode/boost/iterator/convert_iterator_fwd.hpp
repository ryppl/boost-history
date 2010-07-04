#ifndef BOOST_ITERATOR_PIPE_ITERATOR_FWD_HPP
#define BOOST_ITERATOR_PIPE_ITERATOR_FWD_HPP

#include <boost/iterator/iterator_facade.hpp>
#include <boost/iterator/detail/convert_output_storage.hpp>

#include <boost/compressed_pair.hpp>

namespace boost
{

/** Iterator adapter that wraps a range to make it appear like a converted
 * one, by converting it step-by-step as it is advanced. */ 
template<typename It, typename Converter>
struct convert_iterator
	: iterator_facade<
		convert_iterator<It, Converter>,
		typename Converter::output_type,
		std::bidirectional_iterator_tag,
		const typename Converter::output_type
	>
{
    BOOST_CONCEPT_ASSERT((InputIterator<It>));
    BOOST_CONCEPT_ASSERT((ConverterConcept<Converter>));
    
    BOOST_CONCEPT_ASSERT((Convertible<typename InputIterator<It>::value_type, typename Converter::input_type>));
    
    convert_iterator() {} // singular
    
	convert_iterator(It begin_, It end_, It pos_, Converter p_) : pos(pos_), begin(begin_), end(end_), index(0), p(p_)
	{
		if(pos != end)
        {
            std::pair<It, typename detail::convert_output_storage<Converter>::output_iterator> pair =
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
	typedef typename Converter::output_type T;
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
                std::pair<It, typename detail::convert_output_storage<Converter>::output_iterator> pair =
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
            
            std::pair<It, typename detail::convert_output_storage<Converter>::output_iterator> pair =
                p.rtl(begin, pos, values.out());
            pos = pair.first;
            values.update(pair.second);
            
			index = values.last_index();
		}
	}
	
	bool equal(const convert_iterator& other) const
	{
		return pos == other.pos && index == other.index;
	}
	
	It pos;
	It next_pos;
	
	It begin;
	It end;
	size_t index;
	
	Converter p;
	
	detail::convert_output_storage<Converter> values;
};

/** Output Iterator adapter that wraps an output iterator to make one
 * that will convert its output before pushing it to the wrapped iterator. */
template<typename OutputIterator, typename OneManyConverter>
struct convert_output_iterator : private boost::compressed_pair<OutputIterator, OneManyConverter>
{
    BOOST_CONCEPT_ASSERT((OutputIteratorConcept<OutputIterator, typename OneManyConverter::output_type>));
    BOOST_CONCEPT_ASSERT((OneManyConverterConcept<OneManyConverter>));
    
    typedef void                                                difference_type;
    typedef void                                                value_type;
    typedef convert_output_iterator<OutputIterator, OneManyConverter>*  pointer;
    typedef convert_output_iterator<OutputIterator, OneManyConverter>&  reference;
    typedef std::output_iterator_tag                            iterator_category;

    convert_output_iterator() {} // singular
    
	convert_output_iterator(OutputIterator pos_, OneManyConverter p_)
        : boost::compressed_pair<OutputIterator, OneManyConverter>(pos_, p_)
	{
	}
	
	OutputIterator base() const
	{
		return get_out();
	}
	
	const convert_output_iterator& operator*() const
	{
		return *this;
	}
	
	convert_output_iterator& operator++()
	{
		return *this;
	}
	
	convert_output_iterator& operator++(int)
	{
		return *this;
	}
	
	void operator=(const typename OneManyConverter::input_type& val) const
	{
		get_out() = get_conv()(val, get_out());
	}
    
    bool operator==(const convert_output_iterator& other) const
    {
        return get_out() == other.get_out();
    }
    
    bool operator!=(const convert_output_iterator& other) const
    {
        return get_out() != other.get_out();
    }
	
private:
    OutputIterator& get_out() const
    {
        return const_cast<convert_output_iterator&>(*this).first();
    }
    
    OneManyConverter& get_conv() const
    {
        return const_cast<convert_output_iterator&>(*this).second();
    }
};

} // namespace boost

#endif
