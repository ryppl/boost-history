#ifndef BOOST_ITERATOR_CONVERTER_CODECVT_FACET_HPP
#define BOOST_ITERATOR_CONVERTER_CODECVT_FACET_HPP

#include <locale>
#include <cstddef>

#include <boost/iterator/converter_concept.hpp>
#include <boost/iterator/segmenter_concept.hpp>
#include <boost/iterator/dummy_output_iterator.hpp>

#include <algorithm>

#include <map>
#include <boost/range/algorithm.hpp>
#include <boost/range/join.hpp>

namespace boost
{

/** Builds a codecvt facet from two \c \xmlonly<conceptname>Converter</conceptname>s\endxmlonly
 * and two \c \xmlonly<conceptname>BoundaryChecker</conceptname>s\endxmlonly.
 * When writing to a file, \c P1 is applied for segments of data on which \c B1 is true at the beginning and at the end.
 * When reading a file, \c P2 is applied for segments of data on which \c B2 is true at the beginning and at the end. */
template<typename InternT, typename B1, typename P1, typename B2, typename P2>
struct converter_codecvt_facet : std::codecvt<InternT, typename P1::output_type, std::mbstate_t>  
{
    typedef InternT intern_type;
    typedef typename P1::output_type extern_type;
    typedef std::mbstate_t state_type;
    
    BOOST_CONCEPT_ASSERT((BoundaryCheckerConcept<B1>));
    BOOST_CONCEPT_ASSERT((ConverterConcept<P1>));
    BOOST_CONCEPT_ASSERT((BoundaryCheckerConcept<B2>));
    BOOST_CONCEPT_ASSERT((ConverterConcept<P2>));
    
    BOOST_CONCEPT_ASSERT((Convertible<InternT, typename P1::input_type>));
    BOOST_CONCEPT_ASSERT((Convertible<typename P2::output_type, InternT>));
    
    explicit converter_codecvt_facet(const B1& b1_ = B1(), const P1& p1_ = P1(), const B2& b2_ = B2(), const P2& p2_ = P2(), std::size_t refs = 0)
        : std::codecvt<intern_type, extern_type, state_type>(refs), b1(b1_), p1(p1_), b2(b2_), p2(p2_)
    {
    }
    
private:
    struct state_t
    {
        intern_type pending_data[P2::max_output::value];
        size_t pending_size;
    };
    mutable std::map<state_type*, state_t> states;
    
    mutable B1 b1;
    mutable P1 p1;
    
    mutable B2 b2;
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
            st.pending_size = p2.ltr(from_next, from_end, st.pending_data) - st.pending_data;
            *to_next++ = st.pending_data[0];
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
                to_next = p1.ltr(from_next2, from_end2, to_next);
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
                to_next = p1.ltr(from_next, from_end, to_next);
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
                p2.ltr(from_next, from_end, dummy_output_iterator());
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
