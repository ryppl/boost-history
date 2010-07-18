#ifndef BOOST_ITERATOR_CONVERTER_CODECVT_FACET_HPP
#define BOOST_ITERATOR_CONVERTER_CODECVT_FACET_HPP

#include <locale>
#include <cstddef>

#include <boost/iterator/convert_iterator.hpp>
#include <boost/iterator/dummy_output_iterator.hpp>

#include <algorithm>

#include <map>
#include <boost/range/algorithm.hpp>
#include <boost/range/join.hpp>

namespace boost
{
    
template<typename InternT, typename P1, typename P2>
struct converter_codecvt_facet : std::codecvt<InternT, typename P1::output_type, std::mbstate_t>  
{
    typedef InternT intern_type;
    typedef typename P1::output_type extern_type;
    typedef std::mbstate_t state_type;
    
    BOOST_CONCEPT_ASSERT((ConverterConcept<P1>));
    BOOST_CONCEPT_ASSERT((ConverterConcept<P2>));
    
    BOOST_CONCEPT_ASSERT((Convertible<InternT, typename P1::input_type>));
    BOOST_CONCEPT_ASSERT((Convertible<typename P2::output_type, InternT>));
    
    explicit converter_codecvt_facet(const P1& p1_ = P1(), const P2& p2_ = P2(), std::size_t refs = 0)
        : std::codecvt<intern_type, extern_type, state_type>(refs), p1(p1_), p2(p2_)
    {
    }
    
private:
    struct state_t
    {
        intern_type pending_data[P2::max_output::value];
        size_t pending_size;
    };
    mutable std::map<state_type*, state_t> states;
    
    mutable P1 p1;
    mutable P2 p2;
    
protected:

    virtual std::codecvt_base::result do_in(
        state_type& state, 
        const extern_type* from,
        const extern_type* from_end, 
        const extern_type*& from_next,
        intern_type* to, 
        intern_type* to_end, 
        intern_type*& to_next
    ) const
    {
        state_t& st = states[&state];
        
        from_next = from;
        to_next = to;
        
        if(st.pending_size)
        {
            *to_next++ = st.pending_data[0];
            std::copy(st.pending_data + 1, st.pending_data + st.pending_size, st.pending_data);
            st.pending_size--;
            return std::codecvt_base::ok;
        }
        
        try
        {
            std::pair<const extern_type*, intern_type*> p = p2.ltr(from_next, from_end, st.pending_data);
            from_next = p.first;
            *to_next++ = st.pending_data[0];
            st.pending_size = p.second - st.pending_data;
            std::copy(st.pending_data + 1, st.pending_data + st.pending_size, st.pending_data);
            st.pending_size--;
        }
        catch(...)
        {
            return std::codecvt_base::partial;
        }
        return std::codecvt_base::ok;
    }

    virtual std::codecvt_base::result do_out(
        state_type& state,
        const intern_type* from,
        const intern_type* from_end,
        const intern_type*& from_next,
        extern_type* to,
        extern_type* to_end,
        extern_type*& to_next
    ) const
    {
        typedef const boost::iterator_range<const intern_type*> range_base;
        typedef boost::range_detail::join_iterator<const intern_type*, const intern_type*> iterator;
        
        state_t& st = states[&state];
        
        from_next = from;
        to_next = to;
        
        boost::joined_range<range_base, range_base> input = boost::join(
            range_base(st.pending_data,  st.pending_data + st.pending_size),
            range_base(from, from_end)
        );
        
        iterator from2 = input.begin();
        iterator from_next2 = from2;
        iterator from_end2 = input.end();
        
        while(from_next2 != from_end2)
        {
            try
            {
                std::pair<iterator, extern_type*> p = p1.ltr(from_next2, from_end2, to_next);
                from_next2 = p.first;
                to_next = p.second;
            }
            catch(...)
            {
                size_t written = from_next2 - from2;
                if(written >= st.pending_size)
                {
                    from_next += (from_next2 - from2) - st.pending_size;
                    st.pending_size = 0;
                }
                
                boost::copy(range_base(from_next, from_end), st.pending_data + st.pending_size);
                st.pending_size += (from_end - from_next);
                from_next = from_end;
                return std::codecvt_base::ok;
            }
        }
        
        size_t written = from_next2 - from2;
        if(written >= st.pending_size)
        {
            from_next += (from_next2 - from2) - st.pending_size;
            st.pending_size = 0;
        }
        return std::codecvt_base::ok;
    }

    virtual bool do_always_noconv() const throw()
    {
        return false;
    }

    virtual std::codecvt_base::result do_unshift(
        state_type& state,
        extern_type* to,
        extern_type* to_end,
        extern_type*& to_next
    ) const 
    {
        state_t& st = states[&state];
        
        to_next = to;
        const intern_type* from = st.pending_data;
        const intern_type* from_next = from;
        const intern_type* from_end = st.pending_data + st.pending_size;
        
        while(from_next != from_end)
        {
            try
            {
                std::pair<const intern_type*, extern_type*> p = p1.ltr(from_next, from_end, to_next);
                from_next = p.first;
                to_next = p.second;
            }
            catch(...)
            {
                return std::codecvt_base::error;
            }
        }
        
        st.pending_size = 0;
        return std::codecvt_base::ok;
    }

    virtual int do_encoding() const throw()
    {
        return 0;
    }

    virtual int do_length(
        state_type&,
        const extern_type* from,
        const extern_type* from_end, 
        std::size_t max_limit
        ) const
    {
        const extern_type* from_next = from;
        while(from_next != from_end && max_limit--)
        {
            try
            {
                std::pair<const extern_type*, dummy_output_iterator> p = p2.ltr(from_next, from_end, dummy_output_iterator());
                from_next = p.first;
            }
            catch(...)
            {
                break;
            }
        }
        return from_next - from;
    }

    virtual int do_max_length() const throw ()
    {
        return P1::max_output::value;
    }
};
    
} // namespace boost

#endif
