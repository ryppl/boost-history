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
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <iostream>

namespace boost { namespace explore
{
    namespace detail
    {
        struct depth_guard
        {
            depth_guard(container_common_stream_state* state)
            : m_state(state), m_prev_level(state->level())
            {
                ++m_state->m_depth;
            }
            
            ~depth_guard()
            {
                --m_state->m_depth;
                m_state->set_level(m_prev_level);
            }
            
        private:
            container_common_stream_state* m_state;
            size_t m_prev_level;
        };
        
        // manipulator function wrapper for 1 char/wchar_t argument.  When streamed, will run manipulator
        // function with argument.
        template<typename T>
        struct manipfunc
        {
            manipfunc(void (*fun)(std::ios_base&, T), T val)
            : pfun(fun), arg(val)
            {
            }
            
            void (*pfun)(std::ios_base&, T);
            T arg;
        };

        // stream manipfunc
        template<typename Elem, typename Tr, typename T>
        std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, const manipfunc<T>& manip)
        {
            typedef typename boost::remove_const< 
                        typename boost::remove_reference<
                            typename boost::remove_pointer<T>::type 
                        >::type
                    >::type char_type;
            
            BOOST_STATIC_ASSERT(( boost::is_same<Elem, char_type>::value ));
            
            (*manip.pfun)(ostr, manip.arg);
            return ostr;
        }
        
        template<typename Elem, typename Tr>
        std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, const manipfunc<std::size_t>& manip)
        {   
            (*manip.pfun)(ostr, manip.arg);
            return ostr;
        }
        
        template<typename Elem, typename Tr>
        std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, const manipfunc<bool>& manip)
        {   
            (*manip.pfun)(ostr, manip.arg);
            return ostr;
        }

        struct handle_custom_stream
        {
            handle_custom_stream()
                : m_state(0)
            {
            }

            ~handle_custom_stream()
            {
                if( m_state && m_state->depth() > 0 ) // only needed if nested
                {
                    m_state->level_down();
                }
            }

            mutable container_common_stream_state* m_state;
        };

        template<typename Elem, typename Tr>
        std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, const handle_custom_stream& cs)
        {
            container_common_stream_state* state = explore::get_stream_state<container_common_stream_state>(ostr);
            cs.m_state = state;
            if( state->depth() > 0 ) // only needed if nested
            {
                state->level_up();
            }

            return ostr;
        }

        // function ptr for separator manipulator
        template<typename Elem>
        void separatorFn(std::ios_base& ostr, const Elem* sep)
        {
            explore::get_stream_state<container_stream_state<Elem> >(ostr)->set_separator(sep);
        }
        
        // function ptr for start manipulator
        template<typename Elem>
        void startFn(std::ios_base& ostr, const Elem* start)
        {
            explore::get_stream_state<container_stream_state<Elem> >(ostr)->set_start(start);
        }
        
        // function ptr for end manipulator
        template<typename Elem>
        void endFn(std::ios_base& ostr, const Elem* end)
        {
            explore::get_stream_state<container_stream_state<Elem> >(ostr)->set_end(end);
        }
        
        // function ptr for associative separator manipulator
        template<typename Elem>
        void assoc_item_separatorFn(std::ios_base& ostr, const Elem* sep)
        {
            explore::get_stream_state<container_stream_state<Elem> >(ostr)->set_assoc_item_separator(sep);
        }
        
        // function ptr for associative start manipulator
        template<typename Elem>
        void assoc_item_startFn(std::ios_base& ostr, const Elem* start)
        {
            explore::get_stream_state<container_stream_state<Elem> >(ostr)->set_assoc_item_start(start);
        }
        
        // function ptr for associative end manipulator
        template<typename Elem>
        void assoc_item_endFn(std::ios_base& ostr, const Elem* end)
        {
            explore::get_stream_state<container_stream_state<Elem> >(ostr)->set_assoc_item_end(end);
        }

        void levelFn(std::ios_base& ostr, std::size_t l)
        {
            explore::get_stream_state<container_common_stream_state>(ostr)->set_level(l);
        }

        // function ptr object for setrows
        void setrowsFn(std::ios_base& ostr, std::size_t sz)
        {
            explore::get_stream_state<container_common_stream_state>(ostr)->set_rows(sz);
        }
        
        // function ptr object for setrows
        void setitemwidthFn(std::ios_base& ostr, std::size_t sz)
        {
            explore::get_stream_state<container_common_stream_state>(ostr)->set_itemwidth(sz);
        }

        void quotestringsFn(std::ios_base& ostr, bool qs)
        {
            explore::get_stream_state<container_common_stream_state>(ostr)->set_quote_strings(qs);
        }
    }
    
    template<typename Elem>
    detail::manipfunc<const Elem*> separator(const Elem* sep)
    {
        return detail::manipfunc<const Elem*>(&detail::separatorFn, sep);
    }
    
    template<typename Elem, typename Tr>
    const std::basic_string<Elem>& get_separator(std::basic_ostream<Elem, Tr>& ostr)
    {
        return explore::get_stream_state<container_stream_state<Elem> >(ostr)->separator();
    }
    
    template<typename Elem>
    detail::manipfunc<const Elem*> start(const Elem* s)
    {
        return detail::manipfunc<const Elem*>(&detail::startFn, s);
    }
    
    template<typename Elem, typename Tr>
    const std::basic_string<Elem>& get_start(std::basic_ostream<Elem, Tr>& ostr)
    {
        return explore::get_stream_state<container_stream_state<Elem> >(ostr)->start();
    }
    
    template<typename Elem>
    detail::manipfunc<const Elem*> end(const Elem* e)
    {
        return detail::manipfunc<const Elem*>(&detail::endFn, e);
    }
    
    template<typename Elem, typename Tr>
    const std::basic_string<Elem>& get_end(std::basic_ostream<Elem, Tr>& ostr)
    {
        return explore::get_stream_state<container_stream_state<Elem> >(ostr)->end();
    }
    
    template<typename Elem>
    detail::manipfunc<const Elem*> assoc_item_separator(const Elem* sep)
    {
        return detail::manipfunc<const Elem*>(&detail::assoc_item_separatorFn, sep);
    }
    
    template<typename Elem, typename Tr>
    const std::basic_string<Elem>& get_assoc_item_separator(std::basic_ostream<Elem, Tr>& ostr)
    {
        return explore::get_stream_state<container_stream_state<Elem> >(ostr)->assoc_item_separator();
    }
    
    template<typename Elem>
    detail::manipfunc<const Elem*> assoc_item_start(const Elem* start)
    {
        return detail::manipfunc<const Elem*>(&detail::assoc_item_startFn, start);
    }
    
    template<typename Elem, typename Tr>
    const std::basic_string<Elem>& get_assoc_item_start(std::basic_ostream<Elem, Tr>& ostr)
    {
        return explore::get_stream_state<container_stream_state<Elem> >(ostr)->assoc_item_start();
    }
    
    template<typename Elem>
    detail::manipfunc<const Elem*> assoc_item_end(const Elem* end)
    {
        return detail::manipfunc<const Elem*>(&detail::assoc_item_endFn, end);
    }
    
    template<typename Elem, typename Tr>
    const std::basic_string<Elem>& get_assoc_item_end(std::basic_ostream<Elem, Tr>& ostr)
    {
        return explore::get_stream_state<container_stream_state<Elem> >(ostr)->assoc_item_end();
    }
    
    detail::manipfunc<std::size_t> level(std::size_t l)
    {
        return detail::manipfunc<std::size_t>(&detail::levelFn, l);
    }
    
    template<typename Elem, typename Tr>
    std::size_t get_level(std::basic_ostream<Elem, Tr>& ostr)
    {
        return explore::get_stream_state<container_common_stream_state>(ostr)->level();
    }

    detail::manipfunc<std::size_t> rows(std::size_t sz)
    {
        return detail::manipfunc<std::size_t>(detail::setrowsFn, sz);
    }
    
    template<typename Elem, typename Tr>
    std::size_t get_rows(std::basic_ostream<Elem, Tr>& ostr)
    {
        return explore::get_stream_state<container_common_stream_state>(ostr)->get_rows();
    }    
    
    detail::manipfunc<std::size_t> item_width(std::size_t sz)
    {
        return detail::manipfunc<std::size_t>(detail::setitemwidthFn, sz);
    }
    
    template<typename Elem, typename Tr>
    std::size_t get_item_width(std::basic_ostream<Elem, Tr>& ostr)
    {
        return explore::get_stream_state<container_common_stream_state>(ostr)->itemwidth();
    }
     
    detail::manipfunc<bool> quote_strings()
    {
        return detail::manipfunc<bool>(detail::quotestringsFn, true);
    }
    
    detail::manipfunc<bool> no_quote_strings()
    {
        return detail::manipfunc<bool>(detail::quotestringsFn, false);
    }
    
    template<typename Elem, typename Tr>
    bool get_quote_strings(std::basic_ostream<Elem, Tr>& ostr)
    {
        return explore::get_stream_state<container_common_stream_state>(ostr)->quote_strings();
    }
     
    template<typename Elem, typename Tr>
    std::basic_ostream<Elem, Tr>& format_normal(std::basic_ostream<Elem, Tr>& ostr)
    {
        get_stream_state<container_stream_state<Elem> >(ostr)->template init<Elem>();
        return ostr;
    }

    detail::handle_custom_stream custom()
    {
        return detail::handle_custom_stream();
    }
    
    namespace detail
    {
        struct standard_tag;
        struct assoc_tag;
        
        // begin_end manipulator
        template<typename Tag, typename T>
        struct begin_end_manipulator
        {
            begin_end_manipulator(T& startVal, T& endVal)
            :startVal_(startVal), endVal_(endVal)
            {
            }
            
            T startVal_;
            T endVal_;
        };
        
        
        template<typename Elem, typename Tr, typename T>
        std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, const begin_end_manipulator<standard_tag,T>& manip)
        {
            startFn(ostr, manip.startVal_);
            endFn(ostr, manip.endVal_);
            return ostr;
        }
        
        template<typename Elem, typename Tr, typename T>
        std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, const begin_end_manipulator<assoc_tag,T>& manip)
        {
            assoc_item_startFn(ostr, manip.startVal_);
            assoc_item_endFn(ostr, manip.endVal_);
            return ostr;
        }
    }
    
    template<typename Elem>
    detail::begin_end_manipulator<detail::standard_tag,const Elem*> begin_end(const Elem* start, const Elem* end)
    {
        // todo: just use delimiters function and fetch seperator?
        return detail::begin_end_manipulator<detail::standard_tag,const Elem*>(start, end);
    }
    
    template<typename Elem>
    detail::begin_end_manipulator<detail::assoc_tag,const Elem*> assoc_item_begin_end(const Elem* start, const Elem* end)
    {
        // todo: just use delimiters function and fetch seperator?
        return detail::begin_end_manipulator<detail::assoc_tag,const Elem*>(start, end);
    }
    
    namespace detail
    {
        template<typename Tag, typename T>
        struct delimiters_manipulator
        {
            delimiters_manipulator(T& startVal, T& seperatorVal, T& endVal)
            :startVal_(startVal), seperatorVal_(seperatorVal), endVal_(endVal)
            {
            }
            
            T startVal_;
            T seperatorVal_;
            T endVal_;
        };

        template<typename Elem, typename Tr, typename T>
        std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, const delimiters_manipulator<standard_tag,T>& manip)
        {
            startFn(ostr, manip.startVal_);
            separatorFn(ostr, manip.seperatorVal_);
            endFn(ostr, manip.endVal_);
            return ostr;
        }
        
        template<typename Elem, typename Tr, typename T>
        std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, const delimiters_manipulator<assoc_tag,T>& manip)
        {
            assoc_item_startFn(ostr, manip.startVal_);
            assoc_item_separatorFn(ostr, manip.seperatorVal_);
            assoc_item_endFn(ostr, manip.endVal_);
            return ostr;
        }
    }
    
    template<typename Elem>
    detail::delimiters_manipulator<detail::standard_tag,const Elem*> delimiters(const Elem* start, const Elem* seperator, const Elem* end)
    {
        return detail::delimiters_manipulator<detail::standard_tag,const Elem*>(start, seperator, end);
    }
    
    template<typename Elem>
    detail::delimiters_manipulator<detail::assoc_tag,const Elem*> assoc_item_delimiters(const Elem* start, const Elem* seperator, const Elem* end)
    {
        return detail::delimiters_manipulator<detail::assoc_tag,const Elem*>(start, seperator, end);
    }
}}

#endif
