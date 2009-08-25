//
// stream_container.hpp - container streaming.
//
// Copyright (C) 2007, Jeffrey Faust
// Copyright (C) 2008-2009, Jared McIntyre
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MANIPULATOR_INCLUDED
#define MANIPULATOR_INCLUDED

#include <boost/explore/stream_value.hpp>
#include <boost/explore/stream_state.hpp>
#include <boost/explore/container_stream_state.hpp>

namespace boost { namespace explore
{
    namespace detail
    {
        template<typename Elem>
        struct depth_guard
        {
            depth_guard(container_stream_state<Elem>* state)
            : m_state(state)
            {
                ++m_state->m_depth;
            }
            
            ~depth_guard()
            {
                --m_state->m_depth;
            }
            
        private:
            container_stream_state<Elem>* m_state;
        };
        
        // manipulator function wrapper for 1 char/wchar_t argument.  When streamed, will run manipulator
        // function with argument.
        template<typename T>
        struct manipfunc
        {
            manipfunc(void (*fun)(std::ios_base&, T, std::size_t), T val, std::size_t d)
            : pfun(fun), arg(val), depth(d)
            {
            }
            
            void (*pfun)(std::ios_base&, T, std::size_t);
            T arg;
            std::size_t depth;
        };
        
        // stream manipfunc
        template<typename Elem, typename Tr, typename T>
        std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, const manipfunc<T>& manip)
        {
            (*manip.pfun)(ostr, manip.arg, manip.depth);
            return ostr;
        }
        
        // function ptr for separator manipulator
        template<typename Elem>
        void separatorFn(std::ios_base& ostr, const Elem* sep, std::size_t depth)
        {
            explore::get_stream_state<container_stream_state<Elem> >(ostr)->set_separator(sep, depth);
        }
        
        // function ptr for start manipulator
        template<typename Elem>
        void startFn(std::ios_base& ostr, const Elem* start, std::size_t depth)
        {
            explore::get_stream_state<container_stream_state<Elem> >(ostr)->set_start(start, depth);
        }
        
        // function ptr for end manipulator
        template<typename Elem>
        void endFn(std::ios_base& ostr, const Elem* end, std::size_t depth)
        {
            explore::get_stream_state<container_stream_state<Elem> >(ostr)->set_end(end, depth);
        }
        
        // function ptr for associative separator manipulator
        template<typename Elem>
        void assoc_separatorFn(std::ios_base& ostr, const Elem* sep, std::size_t depth)
        {
            explore::get_stream_state<container_stream_state<Elem> >(ostr)->set_assoc_separator(sep, depth);
        }
        
        // function ptr for associative start manipulator
        template<typename Elem>
        void assoc_startFn(std::ios_base& ostr, const Elem* start, std::size_t depth)
        {
            explore::get_stream_state<container_stream_state<Elem> >(ostr)->set_assoc_start(start, depth);
        }
        
        // function ptr for associative end manipulator
        template<typename Elem>
        void assoc_endFn(std::ios_base& ostr, const Elem* end, std::size_t depth)
        {
            explore::get_stream_state<container_stream_state<Elem> >(ostr)->set_assoc_end(end, depth);
        }
        
        // function ptr object for setrows
        //template<typename Elem>
        void setrowsFn(std::ios_base& ostr, std::size_t sz, std::size_t depth)
        {
            explore::get_stream_state<container_stream_state<char> >(ostr)->set_rows(sz, depth);
        }
        
        // function ptr object for setrows
        //template<typename Elem>
        void setitemwidthFn(std::ios_base& ostr, std::size_t sz, std::size_t depth)
        {
            explore::get_stream_state<container_stream_state<char> >(ostr)->set_itemwidth(sz, depth);
        }
    }
    
    // manipulator
    template<typename Elem>
    detail::manipfunc<const Elem*> separator(const Elem* sep, std::size_t depth = 0)
    {
        return detail::manipfunc<const Elem*>(&detail::separatorFn, sep, depth);
    }
    
    // manipulator
    template<typename Elem>
    detail::manipfunc<const Elem*> start(const Elem* Start, std::size_t depth = 0)
    {
        return detail::manipfunc<const Elem*>(&detail::startFn, Start, depth);
    }
    
    // manipulator
    template<typename Elem>
    detail::manipfunc<const Elem*> end(const Elem* end, std::size_t depth = 0)
    {
        return detail::manipfunc<const Elem*>(&detail::endFn, end, depth);
    }
    
    // manipulator
    template<typename Elem>
    detail::manipfunc<const Elem*> assoc_separator(const Elem* sep, std::size_t depth = 0)
    {
        return detail::manipfunc<const Elem*>(&detail::assoc_separatorFn, sep, depth);
    }
    
    // manipulator
    template<typename Elem>
    detail::manipfunc<const Elem*> assoc_start(const Elem* start, std::size_t depth = 0)
    {
        return detail::manipfunc<const Elem*>(&detail::assoc_startFn, start, depth);
    }
    
    // manipulator
    template<typename Elem>
    detail::manipfunc<const Elem*> assoc_end(const Elem* end, std::size_t depth = 0)
    {
        return detail::manipfunc<const Elem*>(&detail::assoc_endFn, end, depth);
    }
    
    detail::manipfunc<std::size_t> setrows(std::size_t sz, std::size_t depth = 0)
    {
        return detail::manipfunc<std::size_t>(detail::setrowsFn, sz, depth);
    }
     
    detail::manipfunc<std::size_t> setitemwidth(std::size_t sz, std::size_t depth = 0)
    {
        return detail::manipfunc<std::size_t>(detail::setitemwidthFn, sz, depth);
    }
     
    // manipulator
    template<typename Elem, typename Tr>
    std::basic_ostream<Elem, Tr>& format_normal(std::basic_ostream<Elem, Tr>& ostr)
    {
        get_stream_state<container_stream_state<Elem> >(ostr)->template init<Elem>();
        return ostr;
    }
    
    // manipulator
    template<typename Elem, typename Tr>
    std::basic_ostream<Elem, Tr>& format_html_list(std::basic_ostream<Elem, Tr>& ostr)
    {
        return ostr
        << start("<ul>\n   <li>")
        << separator("\n   <li>")
        << end("\n</ul>");
    }
    
    namespace detail
    {
        // begin_end manipulator
        template<typename T>
        struct begin_end_manipulator
        {
            begin_end_manipulator(T& startVal, T& endVal, std::size_t d)
            :startVal_(startVal), endVal_(endVal), depth_(d)
            {
            }
            
            T startVal_;
            T endVal_;
            std::size_t depth_;
        };
        
        template<typename Elem, typename Tr, typename T>
        std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, const begin_end_manipulator<T>& manip)
        {
            startFn(ostr, manip.startVal_, manip.depth_);
            endFn(ostr, manip.endVal_, manip.depth_);
            return ostr;
        }
    }
    
    template<typename Elem>
    detail::begin_end_manipulator<const Elem*> begin_end(const Elem* start, const Elem* end, std::size_t depth = 0)
    {
        // todo: just use delimiters function and fetch seperator?
        return detail::begin_end_manipulator<const Elem*>(start, end, depth);
    }
    
    
    // manipulator
    namespace detail
    {
        template<typename T>
        struct delimiters_manipulator
        {
            delimiters_manipulator(T& startVal, T& seperatorVal, T& endVal, std::size_t d)
            :startVal_(startVal), seperatorVal_(seperatorVal), endVal_(endVal), depth_(d)
            {
            }
            
            T startVal_;
            T seperatorVal_;
            T endVal_;
            std::size_t depth_;
        };
        
        template<typename Elem, typename Tr, typename T>
        std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, const delimiters_manipulator<T>& manip)
        {
            startFn(ostr, manip.startVal_, manip.depth_);
            separatorFn(ostr, manip.seperatorVal_, manip.depth_);
            endFn(ostr, manip.endVal_, manip.depth_);
            return ostr;
        }
    }
    
    template<typename Elem>
    detail::delimiters_manipulator<const Elem*> delimiters(const Elem* start, const Elem* seperator, const Elem* end, std::size_t depth = 0)
    {
        return detail::delimiters_manipulator<const Elem*>(start, seperator, end, depth);
    }
}}

#endif
