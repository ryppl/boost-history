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

template<typename P>
struct one_many_pipe : P
{
	one_many_pipe(P p_) : P(p_)
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

template<typename P>
one_many_pipe<P> make_one_many_pipe(P p)
{
	return one_many_pipe<P>(p);
}

template<typename It, typename Pipe>
struct pipe_iterator
	: boost::iterator_facade<
		pipe_iterator<It, Pipe>,
		typename Pipe::output_type,
		std::bidirectional_iterator_tag,
		const typename Pipe::output_type
	>
{
	pipe_iterator(It begin_, It pos_, It end_, Pipe p_) : pos(pos_), begin(begin_), end(end_), index(0), p(p_)
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

template<typename It, typename P>
pipe_iterator<It, P> make_pipe_iterator(It begin, It pos, It end, P p)
{
	return pipe_iterator<It, P>(begin, pos, end, p);
}

template<typename Range, typename P>
std::pair<
	pipe_iterator<typename boost::range_iterator<const Range>::type, P>,
	pipe_iterator<typename boost::range_iterator<const Range>::type, P>
> make_pipe_range(const Range& range, P p)
{
	return std::make_pair(
		make_pipe_iterator(boost::begin(range), boost::begin(range), boost::end(range), p),
		make_pipe_iterator(boost::begin(range), boost::end(range), boost::end(range), p)
	);
}

template<typename It, typename Pipe>
struct pipe_output_iterator
{
	pipe_output_iterator(It pos_, Pipe p_) : pos(pos_), p(p_)
	{
	}
	
	It base() const
	{
		return pos;
	}
	
	pipe_output_iterator& operator*() const
	{
		return const_cast<pipe_output_iterator&>(*this);
	}
	
	pipe_output_iterator& operator++()
	{
		return *this;
	}
	
	pipe_output_iterator& operator++(int)
	{
		return *this;
	}
	
	void operator=(typename Pipe::output_type val) const
	{
		pos = p.ltr(&val, &val + 1, pos).second;
	}
	
private:	
	It pos;
	Pipe p;
};

template<typename OutputIterator, typename P>
pipe_output_iterator<OutputIterator, P> make_pipe_output_iterator(OutputIterator out, P p)
{
	return pipe_output_iterator<OutputIterator, P>(out, p);
}

} // namespace boost

#endif
