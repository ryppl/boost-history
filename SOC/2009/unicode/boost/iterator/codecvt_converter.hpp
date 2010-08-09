#ifndef BOOST_ITERATOR_CODECVT_CONVERTER_HPP
#define BOOST_ITERATOR_CODECVT_CONVERTER_HPP

#include <algorithm>
#include <stdexcept>
#include <locale>

#include <boost/mpl/int.hpp>

namespace boost
{

namespace detail
{

template<typename Input, typename Output, typename Codecvt>
struct codecvt_converter
{
    typedef Input input_type;
    typedef Output output_type;
    
    // arbitrary max buffer size;
    typedef mpl::int_<64> max_output;
    
private:
    typedef std::codecvt_base::result (Codecvt::*CvtFunction)(
        std::mbstate_t&,
        const Input*, const Input*, const Input*&,
        Output*, Output*, Output*&
    ) const;
    
public:
    codecvt_converter(const Codecvt& codecvt_, const CvtFunction& f_) : codecvt(codecvt_), f(f_)
    {
    }
    
    template<typename In, typename Out>
    Out ltr(In& begin, In end, Out out)
    {
        In old_begin = begin;
        
        std::mbstate_t state;
        memset(&state, 0, sizeof state);
        
        Input  buffer_in [max_output::value];
        Output buffer_out[max_output::value];
        
        const Input* from_next = buffer_in;
        Output*      to_next   = buffer_out;
        
        buffer_in[0] = *begin++;
        for(;;) // until we have some output
        {
            for(size_t i=1; ; ++i) // until we get 'ok'
            {
                std::codecvt_base::result result = (codecvt.*f)(
                    state,
                    from_next, from_next + i, from_next,
                    to_next, to_next + max_output::value - (buffer_out - to_next), to_next
                );
                if(result == std::codecvt_base::error)
                    throw std::out_of_range("codecvt error");
                
                if(result == std::codecvt_base::ok)
                    break;
            
                if(begin == end)
                    throw std::out_of_range("unexpected end");
                const_cast<Input*>(from_next)[i] = *begin++;
            }
            
            if(to_next - buffer_out || begin == end)
                break;
                
            *const_cast<Input*>(from_next) = *begin++;
        }
        
        // make sure we empty the state
        Output* old_to_next;
        do
        {
            old_to_next = to_next;
            std::codecvt_base::result result = (codecvt.*f)(
                state,
                from_next, from_next, from_next,
                to_next, to_next + max_output::value - (buffer_out - to_next), to_next
            );
            
            if(result == std::codecvt_base::error)
                throw std::out_of_range("codecvt error");
                
            if(result == std::codecvt_base::partial)
                break;
        }
        while(to_next != old_to_next);
    
        // restore begin to the position given by the final 'from_next'
        std::advance(old_begin, from_next - buffer_in);
        begin = old_begin;
        return std::copy(buffer_out, to_next, out);
    }
    
    template<typename In, typename Out>
    Out rtl(In begin, In& end, Out out)
    {
        throw std::runtime_error("not implemented");
    }
    
private:
    const Codecvt& codecvt;
    CvtFunction f;
};

} // namespace detail

template<typename Input, typename Output>
struct codecvt_in_converter
  : detail::codecvt_converter<
        Input,
        Output,
        std::codecvt<Output, Input, std::mbstate_t>
    >
{
private:
    typedef std::codecvt<Output, Input, std::mbstate_t> Codecvt;
    
public:    
    codecvt_in_converter(const Codecvt& codecvt)
      : detail::codecvt_converter<Input, Output, Codecvt>(codecvt, &Codecvt::in)
    {
    }
};

template<typename Input, typename Output>
struct codecvt_out_converter
  : detail::codecvt_converter<
        Input,
        Output,
        std::codecvt<Input, Output, std::mbstate_t>
    >
{
private:
    typedef std::codecvt<Input, Output, std::mbstate_t> Codecvt;
    
public:    
    codecvt_out_converter(const Codecvt& codecvt)
      : detail::codecvt_converter<Input, Output, Codecvt>(codecvt, &Codecvt::out)
    {
    }
};

} // namespace booost

#endif
