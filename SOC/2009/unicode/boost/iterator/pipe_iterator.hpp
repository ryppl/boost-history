#ifndef BOOST_PIPE_ITERATOR_HPP
#define BOOST_PIPE_ITERATOR_HPP

#include <boost/iterator/iterator_facade.hpp>
#include <iterator>
#include <vector>
#include <utility>

#include <boost/range.hpp>
#include <boost/assert.hpp>

#include <boost/introspection/has_member_data.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost
{
namespace detail
{
    BOOST_HAS_STATIC_MEMBER_DATA(const int, max_output)

    template<typename P, typename Enable = void>
    struct pipe_output_storage;

    template<typename P>
    struct pipe_output_storage<P, typename ::boost::disable_if< has_static_member_data_max_output<P> >::type>
    {
private:
        typedef std::vector<typename P::output_type> Values;
public:
        typedef std::back_insert_iterator<Values> output_iterator;
        
        const typename P::output_value& operator[](size_t i) const
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
    struct pipe_output_storage<P, typename boost::enable_if< has_static_member_data_max_output<P> >::type>
    {
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
        Value values[P::max_output];
        size_t last;
    };
}

/** Model of \c Pipe constructed from a model of \c OneManyPipe */
template<typename OneManyPipe>
struct one_many_pipe : OneManyPipe
{
    one_many_pipe() {} // singular
    
	one_many_pipe(OneManyPipe p_) : OneManyPipe(p_)
	{
	}
	
	template<typename In, typename Out>
	std::pair<In, Out>
	ltr(In begin, In end, Out out)
	{
		BOOST_ASSERT(begin != end);
		
		out = (*this)(*begin, out);
		return std::make_pair(++begin, out);
	}
	
	template<typename In, typename Out>
	std::pair<In, Out>
	rtl(In begin, In end, Out out)
	{
		BOOST_ASSERT(begin != end);
		
		out = (*this)(*--end, out);
		return std::make_pair(end, out);
	}
};

template<typename OneManyPipe>
one_many_pipe<OneManyPipe> make_one_many_pipe(OneManyPipe p)
{
	return one_many_pipe<OneManyPipe>(p);
}

/** Iterator adapter that wraps a range to make it appear like a converted
 * one, by converting it step-by-step as it is advanced. */ 
template<typename It, typename Pipe>
struct pipe_iterator
	: boost::iterator_facade<
		pipe_iterator<It, Pipe>,
		typename Pipe::output_type,
		std::bidirectional_iterator_tag,
		const typename Pipe::output_type
	>
{
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

template<typename It, typename Pipe>
pipe_iterator<It, Pipe> make_pipe_iterator(It begin, It end, It pos, Pipe p)
{
	return pipe_iterator<It, Pipe>(begin, end, pos, p);
}

template<typename Range, typename Pipe>
boost::iterator_range<
	pipe_iterator<typename boost::range_iterator<const Range>::type, Pipe>
> piped(const Range& range, Pipe p)
{
	return boost::make_iterator_range(
		make_pipe_iterator(boost::begin(range), boost::end(range), boost::begin(range), p),
		make_pipe_iterator(boost::begin(range), boost::end(range), boost::end(range), p)
	);
}

template<typename Range, typename Pipe, typename OutputIterator>
OutputIterator pipe(const Range& range, Pipe pipe, OutputIterator out)
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

/** Output Iterator adapter that wraps an output iterator to make one
 * that will convert its output before pushing it to the wrapped iterator. */
template<typename It, typename OneManyPipe>
struct pipe_output_iterator
{
    typedef void                                   difference_type;
    typedef void                                   value_type;
    typedef pipe_output_iterator<It, OneManyPipe>*        pointer;
    typedef pipe_output_iterator<It, OneManyPipe>&        reference;
    typedef std::output_iterator_tag               iterator_category;

    pipe_output_iterator() {} // singular
    
	pipe_output_iterator(It pos_, OneManyPipe p_) : pos(pos_), p(p_)
	{
	}
	
	It base() const
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
	mutable It pos;
	mutable OneManyPipe p;
};

template<typename OutputIterator, typename OneManyPipe>
pipe_output_iterator<OutputIterator, OneManyPipe> piped_output(OutputIterator out, OneManyPipe p)
{
	return pipe_output_iterator<OutputIterator, OneManyPipe>(out, p);
}

} // namespace boost

#endif
