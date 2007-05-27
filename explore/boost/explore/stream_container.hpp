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

// generate string init functions for both char and wchar_t types
#define BOOST_EXPLORE_INIT_STRING(name, str)                             \
template<typename charType> std::basic_string<charType> init_##name();   \
template<> std::basic_string<char> init_##name<char>() { return (str); } \
template<> std::basic_string<wchar_t> init_##name<wchar_t>() { return L##str; }

namespace explore
{
    namespace detail 
    {
        BOOST_EXPLORE_INIT_STRING(separator, ", ")
        BOOST_EXPLORE_INIT_STRING(start, "[")
        BOOST_EXPLORE_INIT_STRING(end, "]")
        BOOST_EXPLORE_INIT_STRING(assoc_separator, ":")
        BOOST_EXPLORE_INIT_STRING(assoc_start, "")
        BOOST_EXPLORE_INIT_STRING(assoc_end, "")

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

        // stream manipfunc
        template<typename Elem, typename Tr, typename Arg>
        std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, const manipfunc<Arg>& manip)
        {
            (*manip.pfun)(ostr, manip.arg);
            return ostr;
        }

        // function ptr for separator manipulator
        template<typename Elem, typename Tr>
        void separatorFn(std::basic_ostream<Elem, Tr>& ostr, const Elem* sep)
        {
            explore::get_stream_state<container_stream_state<Elem, Tr> >(ostr)->separator = sep;
        }

        // function ptr for start manipulator
        template<typename Elem, typename Tr>
        void startFn(std::basic_ostream<Elem, Tr>& ostr, const char* start)
        {
            explore::get_stream_state<container_stream_state<Elem, Tr> >(ostr)->start = start;
        }

        // function ptr for end manipulator
        template<typename Elem, typename Tr>
        void endFn(std::basic_ostream<Elem, Tr>& ostr, const char* end)
        {
            explore::get_stream_state<container_stream_state<Elem, Tr> >(ostr)->end = end;
        }

        // function ptr for associative separator manipulator
        template<typename Elem, typename Tr>
        void assoc_separatorFn(std::basic_ostream<Elem, Tr>& ostr, const Elem* sep)
        {
            explore::get_stream_state<container_stream_state<Elem, Tr> >(ostr)->assoc_separator = sep;
        }

        // function ptr for associative start manipulator
        template<typename Elem, typename Tr>
        void assoc_startFn(std::basic_ostream<Elem, Tr>& ostr, const Elem* start)
        {
            explore::get_stream_state<container_stream_state<Elem, Tr> >(ostr)->assoc_start = start;
        }

        // function ptr for associative end manipulator
        template<typename Elem, typename Tr>
        void assoc_endFn(std::basic_ostream<Elem, Tr>& ostr, const Elem* end)
        {
            explore::get_stream_state<container_stream_state<Elem, Tr> >(ostr)->assoc_end = end;
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
            assoc_separator = detail::init_assoc_separator<El>();
            assoc_start = detail::init_assoc_start<El>();
            assoc_end = detail::init_assoc_end<El>();
        }

        str_typ separator;
        str_typ start;
        str_typ end;
        str_typ assoc_separator;
        str_typ assoc_start;
        str_typ assoc_end;
    };

    struct stream_normal_value
    {
        template<typename Elem, typename Tr, typename T>
        void operator()(std::basic_ostream<Elem, Tr>& ostr, const T& val, container_stream_state<Elem, Tr>*)
        {
            ostr << val;
        }
    };

    // stream value from associative container
    struct stream_map_value
    {
        template<typename Elem, typename Tr, typename T>
        void operator()(std::basic_ostream<Elem, Tr>& ostr, const T& val, container_stream_state<Elem, Tr>* state)
        {
            ostr << state->assoc_start << val.first << state->assoc_separator << val.second << state->assoc_end;
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
            f(ostr, *first, state);
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
}

namespace std
{
    // stream vector<T>
    template<typename Elem, typename Tr, typename T>
    std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, const std::vector<T>& v)
    {
        return explore::stream_container(ostr, v.begin(), v.end());
    }

    // stream deque<T>
    template<typename Elem, typename Tr, typename T, typename Allocator>
    std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, const std::deque<T, Allocator>& d)
    {
        return explore::stream_container(ostr, d.begin(), d.end());
    }

    // stream set<T>
    template<typename Elem, typename Tr, typename T, typename Compare, typename Alloc>
    std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, const std::set<T, Compare, Alloc>& s)
    {
        return explore::stream_container(ostr, s.begin(), s.end());
    }

    // stream multiset<T>
    template<typename Elem, typename Tr, typename T, typename Compare, typename Allocator>
    std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, const std::multiset<T, Compare, Allocator>& s)
    {
        return explore::stream_container(ostr, s.begin(), s.end());
    }

    // stream list<T>
    template<typename Elem, typename Tr, typename T, typename Allocator>
    std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, const std::list<T, Allocator>& l)
    {
        return explore::stream_container(ostr, l.begin(), l.end());
    }

    // stream pair<T1, T2>
    template<typename Elem, typename Tr, typename T1, typename T2>
    std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, const std::pair<T1, T2>& p)
    {
        using namespace explore;
        container_stream_state<Elem, Tr>* state = get_stream_state<container_stream_state<Elem, Tr> >(ostr);
        return ostr << state->start << p.first << state->separator << p.second << state->end;
    }

    // stream map<K, T>
    template<typename Elem, typename Tr, typename K, typename T, typename Compare, typename Allocator>
    std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, const std::map<K, T, Compare, Allocator>& m)
    {
        using namespace explore;
        return stream_container(ostr, m.begin(), m.end(), stream_map_value());
    }

    // stream multimap<K, T>
    template<typename Elem, typename Tr, typename K, typename T, typename Compare, typename Allocator>
    std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, const std::multimap<K, T, Compare, Allocator>& m)
    {
        using namespace explore;
        return stream_container(ostr, m.begin(), m.end(), stream_map_value());
    }
}

namespace explore
{
#   if !BOOST_WORKAROUND(BOOST_MSVC, <= 1300)
    template<typename Elem, typename Tr, typename T, std::size_t size>
    std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, T (&a)[size])
    {
        return stream_container(ostr, &a[0], &a[size]);
    }

    template<typename Elem, typename Tr, std::size_t size>
    std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, const Elem* s)
    {
        return stream_container(ostr, &s[0], &s[strlen(s)]);
    }
#   endif

    // manipulator
    template<typename Elem>
    detail::manipfunc<Elem> separator(const Elem* sep)
    {
        return detail::manipfunc<Elem>(&detail::separatorFn, sep);
    }

    // manipulator
    template<typename Elem>
    detail::manipfunc<Elem> start(const Elem* Start)
    {
        return detail::manipfunc<Elem>(&detail::startFn, Start);
    }

    // manipulator
    template<typename Elem>
    detail::manipfunc<Elem> end(const Elem* end)
    {
        return detail::manipfunc<Elem>(&detail::endFn, end);
    }

    // manipulator
    template<typename Elem>
    detail::manipfunc<Elem> assoc_separator(const Elem* sep)
    {
        return detail::manipfunc<Elem>(&detail::assoc_separatorFn, sep);
    }

    // manipulator
    template<typename Elem>
    detail::manipfunc<Elem> assoc_start(const Elem* start)
    {
        return detail::manipfunc<Elem>(&detail::assoc_startFn, start);
    }

    // manipulator
    template<typename Elem>
    detail::manipfunc<Elem> assoc_end(const Elem* end)
    {
        return detail::manipfunc<Elem>(&detail::assoc_endFn, end);
    }

    // manipulator
    template<typename Elem, typename Tr>
    std::basic_ostream<Elem, Tr>& format_normal(std::basic_ostream<Elem, Tr>& ostr)
    {
        return ostr << start("[") << separator(", ") << end("]")
                    << assoc_start("") << assoc_separator(":") << assoc_end("");;
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
}

namespace boost
{
    // stream boost::array<T>
    template<typename Elem, typename Tr, typename T, std::size_t N>
    std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, const boost::array<T, N>& a)
    {
        return explore::stream_container(ostr, a.begin(), a.end());
    }
}

#endif
