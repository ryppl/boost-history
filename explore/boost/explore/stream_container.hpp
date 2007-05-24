//
// stream_container.hpp - container streaming.
//
// Copyright (C) 2007, Jeffrey Faust
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef STREAM_CONTAINER_INCLUDED
#define STREAM_CONTAINER_INCLUDED

#include "stream_state.hpp"

#include <ostream>
#include <boost/functional/detail/container_fwd.hpp>
#include <boost/array.hpp>

namespace boost
{
    namespace detail 
    {
        template<typename charType>
        std::basic_string<charType> init_separator();

        template<>
        std::basic_string<char> init_separator<char>()
        {
            return ", ";
        }

        template<>
        std::basic_string<wchar_t> init_separator<wchar_t>()
        {
            return L", ";
        }

        template<typename charType>
        std::basic_string<charType> init_start();

        template<>
        std::basic_string<char> init_start<char>()
        {
            return "[";
        }

        template<>
        std::basic_string<wchar_t> init_start<wchar_t>()
        {
            return L"[";
        }

        template<typename charType>
        std::basic_string<charType> init_end();

        template<>
        std::basic_string<char> init_end<char>()
        {
            return "]";
        }

        template<>
        std::basic_string<wchar_t> init_end<wchar_t>()
        {
            return L"]";
        }
    }

    // A simple collection of additional stream state
    template<typename Elem, typename Tr>
    struct container_stream_state
    {
        typedef std::basic_string<Elem, Tr> str_typ;
        container_stream_state()
        {
            init<Elem>();
        }

        // is there an easier way to specialize between char and wchar_t?
        // Concern: this is only specialized for char and wchar_t streams.
        template<typename El>
        void init()
        {
            separator = detail::init_separator<El>();
            start = detail::init_start<El>();
            end = detail::init_end<El>();
        }

        str_typ separator;
        str_typ start;
        str_typ end;
    };

    // manipulator function wrapper for 1 char/wchar_t argument.  When streamed, will run manipulator
    // function with argument.
    template<typename Elem>
    struct manipfunc
    {
        manipfunc(void (*fun)(std::basic_ostream<Elem, std::char_traits<Elem> >&, const Elem*), const Elem* val)
            : pfun(fun), arg(val)
        {
        }

        void (*pfun)(std::basic_ostream<Elem, std::char_traits<Elem> >&, const Elem*);
        const Elem* arg;
    };

    // stream manipulator
    template<typename Elem, typename Tr, typename Arg>
    std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, const manipfunc<Arg>& manip)
    {
        (*manip.pfun)(ostr, manip.arg);
        return ostr;
    }

    struct stream_normal_value
    {
        template<typename Elem, typename Tr, typename T>
        void operator()(std::basic_ostream<Elem, Tr>& ostr, const T& val)
        {
            ostr << val;
        }
    };

    // stream value from associative container
    struct stream_map_value
    {
        template<typename Elem, typename Tr, typename T>
        void operator()(std::basic_ostream<Elem, Tr>& ostr, const T& val)
        {
            ostr << val.first << ":" << val.second;
        }
    };

    template<typename Elem, typename Tr, typename FwdIter, typename F>
    std::basic_ostream<Elem, Tr>& stream_container(std::basic_ostream<Elem, Tr>& ostr, FwdIter first, FwdIter last, F f)
    {
        // grab the extra data embedded in the stream object.
        container_stream_state<Elem, Tr>* state = explore::get_stream_state<container_stream_state<Elem, Tr> >(ostr);

        // starting delimiter
        ostr << state->start;

        while( first != last )
        {
            // value
            f(ostr, *first);
            if( ++first != last )
            {
                // separation delimiter
                ostr << state->separator;
            }
        }

        // ending delimiter
        return ostr << state->end;
    }

    template<typename Elem, typename Tr, typename FwdIter>
    std::basic_ostream<Elem, Tr>& stream_container(std::basic_ostream<Elem, Tr>& ostr, FwdIter first, FwdIter last)
    {
        // redirect with "normal" streaming.
        return stream_container(ostr, first, last, stream_normal_value());
    }

    // stream vector<T>
    template<typename Elem, typename Tr, typename T>
    std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, const std::vector<T>& v)
    {
        return stream_container(ostr, v.begin(), v.end());
    }

    // stream deque<T>
    template<typename Elem, typename Tr, typename T, typename Allocator>
    std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, const std::deque<T, Allocator>& d)
    {
        return stream_container(ostr, d.begin(), d.end());
    }

    // stream set<T>
    template<typename Elem, typename Tr, typename T, typename Compare, typename Alloc>
    std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, const std::set<T, Compare, Alloc>& s)
    {
        return stream_container(ostr, s.begin(), s.end());
    }

    // stream multiset<T>
    template<typename Elem, typename Tr, typename T, typename Compare, typename Allocator>
    std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, const std::multiset<T, Compare, Allocator>& s)
    {
        return stream_container(ostr, s.begin(), s.end());
    }

    // stream list<T>
    template<typename Elem, typename Tr, typename T, typename Allocator>
    std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, const std::list<T, Allocator>& l)
    {
        return stream_container(ostr, l.begin(), l.end());
    }

    // stream array<T>
    template<typename Elem, typename Tr, typename T, std::size_t N>
    std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, const boost::array<T, N>& a)
    {
        return stream_container(ostr, a.begin(), a.end());
    }

    // stream pair<T1, T2>
    template<typename Elem, typename Tr, typename T1, typename T2>
    std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, const std::pair<T1, T2>& p)
    {
        container_stream_state<Elem, Tr>* state = explore::get_stream_state<container_stream_state<Elem, Tr> >(ostr);
        return ostr << state->start << p.first << state->separator << p.second << state->end;
    }

    // stream map<K, T>
    template<typename Elem, typename Tr, typename K, typename T, typename Compare, typename Allocator>
    std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, const std::map<K, T, Compare, Allocator>& m)
    {
        return stream_container(ostr, m.begin(), m.end(), stream_map_value());
    }

    // stream multimap<K, T>
    template<typename Elem, typename Tr, typename K, typename T, typename Compare, typename Allocator>
    std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, const std::multimap<K, T, Compare, Allocator>& m)
    {
        return stream_container(ostr, m.begin(), m.end(), stream_map_value());
    }

    // stream c-array
    template<typename Elem, typename Tr, typename T, std::size_t size>
    std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, T (&a)[size])
    {
        return stream_container(ostr, &a[0], &a[size]);
    }

    // function ptr for separator manipulator
    template<typename Elem, typename Tr>
    void setSeparatorFn(std::basic_ostream<Elem, Tr>& ostr, const Elem* sep)
    {
        explore::get_stream_state<container_stream_state<Elem, Tr> >(ostr)->separator = sep;
    }

    // manipulator
    template<typename Elem>
    manipfunc<Elem> setSeparator(const Elem* sep)
    {
        return manipfunc<Elem>(&setSeparatorFn, sep);
    }

    // function ptr for start manipulator
    template<typename Elem, typename Tr>
    void setStartFn(std::basic_ostream<Elem, Tr>& ostr, const char* start)
    {
        explore::get_stream_state<container_stream_state<Elem, Tr> >(ostr)->start = start;
    }

    // manipulator
    template<typename Elem>
    manipfunc<Elem> setStart(const Elem* Start)
    {
        return manipfunc<Elem>(&setStartFn, Start);
    }

    // function ptr for end manipulator
    template<typename Elem, typename Tr>
    void setEndFn(std::basic_ostream<Elem, Tr>& ostr, const char* end)
    {
        explore::get_stream_state<container_stream_state<Elem, Tr> >(ostr)->end = end;
    }

    // manipulator
    template<typename Elem>
    manipfunc<Elem> setEnd(const Elem* end)
    {
        return manipfunc<Elem>(&setEndFn, end);
    }

    // manipulator
    template<typename Elem, typename Tr>
    std::basic_ostream<Elem, Tr>& format_normal(std::basic_ostream<Elem, Tr>& ostr)
    {
        return ostr << setStart("[") << setSeparator(", ") << setEnd("]");
    }

    // manipulator
    template<typename Elem, typename Tr>
    std::basic_ostream<Elem, Tr>& format_html_list(std::basic_ostream<Elem, Tr>& ostr)
    {
        return ostr
            << setStart("<ul>\n   <li>")
            << setSeparator("\n   <li>")
            << setEnd ("\n</ul>");
    }
}

#endif
