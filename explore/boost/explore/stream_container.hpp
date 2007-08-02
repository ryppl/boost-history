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
#include "stream_value.hpp"
#include "container_stream_state.hpp"

#include <ostream>
#include <boost/functional/detail/container_fwd.hpp>
#include <boost/array.hpp>

namespace explore
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

    template<typename Elem, typename Tr, typename FwdIter, typename F>
    std::basic_ostream<Elem, Tr>& stream_container(std::basic_ostream<Elem, Tr>& ostr, FwdIter first, FwdIter last, F f)
    {
        // grab the extra data embedded in the stream object.
        container_stream_state<Elem>* state = explore::get_stream_state<container_stream_state<Elem> >(ostr);
        detail::depth_guard<Elem> guard(state);
        std::size_t depth = state->depth();

        // starting delimiter
        ostr << state->start(depth);

        std::size_t rows = state->rows();

        std::size_t cur_row = 0;
        while( first != last )
        {
            // value
            f(ostr, *first, state);

            if( ++first != last )
            {
                // separation delimiter
                ostr << state->separator(depth);

                if( rows && ++cur_row == rows )
                {
                    ostr << '\n';
                    cur_row = 0;
                }
            }
        }

        // ending delimiter
        return ostr << state->end(depth);
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
    template<typename Elem, typename Tr, typename T, typename Allocator>
    std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, const std::vector<T, Allocator>& v)
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
        container_stream_state<Elem>* state = get_stream_state<container_stream_state<Elem> >(ostr);
        detail::depth_guard<Elem> guard(state);
        std::size_t depth = state->depth();
        return ostr << state->start(depth) << p.first << state->separator(depth) << p.second << state->end(depth);
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
