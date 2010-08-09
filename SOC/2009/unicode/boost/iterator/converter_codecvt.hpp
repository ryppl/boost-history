#ifndef BOOST_ITERATOR_CONVERTER_CODECVT_FACET_HPP
#define BOOST_ITERATOR_CONVERTER_CODECVT_FACET_HPP

#include <locale>
#include <cstddef>

#include <boost/iterator/converter_concept.hpp>
#include <boost/iterator/segmenter_concept.hpp>
#include <boost/iterator/dummy_output_iterator.hpp>

#include <algorithm>

#include <map>
#include <boost/range/join.hpp>
#include <boost/circular_buffer.hpp>

namespace boost
{

namespace detail
{
    template<typename A, typename B, typename T>
    struct other;
    
    template<typename A>
    struct other<A, A, A>
    {
        typedef A type;
    };
    template<typename A, typename B>
    struct other<A, B, A>
    {
        typedef B type;
    };
    template<typename A, typename B>
    struct other<A, B, B>
    {
        typedef A type;
    };
}

/** Builds a codecvt facet from two \c \xmlonly<conceptname>Converter</conceptname>s\endxmlonly
 * and two \c \xmlonly<conceptname>BoundaryChecker</conceptname>s\endxmlonly.
 * When writing to a file, \c P1 is applied for segments of data on which \c B1 is true at the beginning and at the end.
 * When reading a file, \c P2 is applied for segments of data on which \c B2 is true at the beginning and at the end. */
template<typename InternT, typename B1, typename P1, typename B2, typename P2>
struct converter_codecvt : std::codecvt<InternT, typename P1::output_type, std::mbstate_t>  
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
    
    typedef typename std::basic_ios<InternT>::pos_type pos_type;
    
    explicit converter_codecvt(pos_type file_size_ = (pos_type)-1, const B1& b1_ = B1(), const P1& p1_ = P1(), const B2& b2_ = B2(), const P2& p2_ = P2(), std::size_t refs = 0)
        : std::codecvt<intern_type, extern_type, state_type>(refs), file_size(file_size_), b1(b1_), p1(p1_), b2(b2_), p2(p2_)
    {
    }
    
private:
    pos_type file_size;

    template<typename T>
    struct state_t
    {
        boost::circular_buffer<T> pending_data;
        pos_type read_size;
        
        // size of storage is maximum size of input, which is not exposed
        // by Converters, so we just take an arbitrary max size
        state_t() : pending_data(64), read_size(0)
        {
        }
    };
    
    struct state_pair_t
    {
        state_t<extern_type> in;
        state_t<intern_type> out;
        
#ifdef BOOST_MSVC
        // MSVC only calls 'in' step-by-step, so that is enough storage
        intern_type pending_write_data[P2::max_output::value];
        size_t pending_write_index;
        size_t pending_write_size;
        
        state_pair_t() : pending_write_index(0), pending_write_size(0)
        {
        }
#endif

    };
    
    template<typename T>
    struct other : detail::other<intern_type, extern_type, T>
    {
    };
    
    mutable std::map<state_type*, state_pair_t> states;
    
    mutable B1 b1;
    mutable P1 p1;
    
    mutable B2 b2;
    mutable P2 p2;

    template<typename B, typename P, typename T>
    std::codecvt_base::result do_(
        B& b, P& p,
        state_t<T>& st,
        const T* from, const T* from_end, const T*& from_next,
        typename other<T>::type* to, typename other<T>::type* to_end, typename other<T>::type*& to_next
    ) const
    {
        typedef const boost::iterator_range<typename circular_buffer<T>::const_iterator> range_circular;
        typedef const boost::iterator_range<const T*> range_base;
        typedef boost::range_detail::join_iterator<typename circular_buffer<T>::const_iterator, const T*> iterator;
        
        from_next = from;
        to_next = to;
        
        // our real input is the concatenated pending data and the given input
        boost::joined_range<range_circular, range_base> input = boost::join(
            range_circular(st.pending_data.begin(), st.pending_data.end()),
            range_base(from, from_end)
        );
        
        iterator from2 = input.begin();
        iterator from_next2 = from2;
        iterator from_end2 = input.end();
        
        // while we have some input
        while(from_next2 != from_end2)
        {
            iterator from_boundary = from_next2;
            do
            {
                ++from_boundary;
            }
            while(from_boundary != from_end2 && !b(from_next2, from_end2, from_boundary));
         
            bool eof = st.read_size + pos_type(from_end2 - from_next2) == file_size
                    || from == from_end;
            
            // boundary not found and not end of file, we append the trailing data to 'pending'
            if(from_boundary == from_end2 && !eof)
            {
                std::copy(from_next, from_end, std::back_inserter(st.pending_data));
                from_next = from_end;
                return std::codecvt_base::ok;
            }
            
            size_t written = from_boundary - from_next2;
            try
            {
                to_next = p.ltr(from_next2, from_boundary, to_next);
            }
            catch(...)
            {
                return std::codecvt_base::error;
            }
            
            // erase the consumed pending data and update 'from_next'
            st.read_size += written;
            if(written >= st.pending_data.size())
            {
                from_next += written - st.pending_data.size();
                st.pending_data.clear();
            }
            else
            {
                st.pending_data.erase_begin(written);
            }
        }
        return std::codecvt_base::ok;
    }

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
#if BOOST_MSVC
        state_pair_t& st = states[&state];
        
        to_next = to;
        from_next = from;
        
        // MSVC only supports getting one result per call, so we use a hack
        if(st.pending_write_size)
        {
            *to_next++ = st.pending_write_data[st.pending_write_index];
            st.pending_write_index++;
            st.pending_write_size--;
            
            if(!st.pending_write_size)
                from_next++;
            
            return std::codecvt_base::ok;
        }
        
        intern_type* to_next2;
        std::codecvt_base::result result = do_(b2, p2, states[&state].in, from, from_end, from_next, st.pending_write_data, st.pending_write_data + st.pending_write_size, to_next2);
        st.pending_write_size = to_next2 - st.pending_write_data;
        
        if(st.pending_write_size)
        {
            *to_next++ = st.pending_write_data[0];
            st.pending_write_index = 1;
            st.pending_write_size--;
        }
        
        if(st.pending_write_size)
            from_next--;
        
        return result;
#else
        return do_(b2, p2, states[&state].in, from, from_end, from_next, to, to_end, to_next);
#endif
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
        return do_(b1, p1, states[&state].out, from, from_end, from_next, to, to_end, to_next);
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
        typedef typename circular_buffer<intern_type>::const_iterator iterator;
        state_t<intern_type>& st = states[&state].out;
        
        to_next = to;
        
        // we output the pending 'out' data
        iterator from = st.pending_data.begin();
        iterator from_next = from;
        iterator from_end = st.pending_data.end();
        
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
        st.pending_data.clear();
        return std::codecvt_base::ok;
    }

    virtual int do_encoding() const throw()
    {
        return 0;
    }

    // probably needs fixing, but no implementation uses this
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
        // maximum size of P2's input, which is not exposed
        // by Converters, so we just take an arbitrary max size
        return 64;
    }
};
    
} // namespace boost

#endif
