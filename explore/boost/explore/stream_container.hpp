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
#include "is_assoc_iter.hpp"

#include <ostream>
#include <vector>
#include <boost/functional/detail/container_fwd.hpp>
#include <boost/array.hpp>
#include <boost/range/iterator_range.hpp>

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

        template<typename Elem>
        struct depth_guard;
    }


    // A simple collection of additional stream state
    template<typename Elem>
    struct container_stream_state
    {
        typedef std::basic_string<Elem> str_typ;
        typedef std::vector<str_typ, std::allocator<str_typ> > cont_typ;

        container_stream_state()
            : m_depth(0), m_itemw(0)
        {
            init<Elem>();
        }

        // Concern: this is only specialized for char and wchar_t streams.
        template<typename El>
        void init()
        {
            init(m_separator, detail::init_separator<El>());
            init(m_start, detail::init_start<El>());
            init(m_end, detail::init_end<El>());
            init(m_assoc_separator, detail::init_assoc_separator<El>());
            init(m_assoc_start, detail::init_assoc_start<El>());
            init(m_assoc_end, detail::init_assoc_end<El>());
        }

        // read
        const str_typ& separator(std::size_t index = 0) const { return at(m_separator, index); }
        const str_typ& start(std::size_t index = 0) const { return at(m_start, index); }
        const str_typ& end(std::size_t index = 0) const { return at(m_end, index); }
        const str_typ& assoc_separator(std::size_t index = 0) const { return at(m_assoc_separator, index); }
        const str_typ& assoc_start(std::size_t index = 0) const { return at(m_assoc_start, index); }
        const str_typ& assoc_end(std::size_t index = 0) const { return at(m_assoc_end, index); }
        std::streamsize itemw() const { return m_itemw; }

        // write
        void set_separator(const str_typ& str, std::size_t index = 0) { at(m_separator, index) = str; }
        void set_start(const str_typ& str, std::size_t index = 0) { at(m_start, index) = str; }
        void set_end(const str_typ& str, std::size_t index = 0) { at(m_end, index) = str; }
        void set_assoc_separator(const str_typ& str, std::size_t index = 0) { at(m_assoc_separator, index) = str; }
        void set_assoc_start(const str_typ& str, std::size_t index = 0) { at(m_assoc_start, index) = str; }
        void set_assoc_end(const str_typ& str, std::size_t index = 0) { at(m_assoc_end, index) = str; }
        void set_itemw(std::streamsize itemw, std::size_t index = 0) { m_itemw = itemw; }

        std::size_t depth() const
        {
            // we start at 0, increment before use, so we must decrement upon query.
            return m_depth - 1;
        }

    private:
        friend struct detail::depth_guard<Elem>;

        cont_typ m_separator;
        cont_typ m_start;
        cont_typ m_end;
        cont_typ m_assoc_separator;
        cont_typ m_assoc_start;
        cont_typ m_assoc_end;
        std::size_t m_depth;

        std::streamsize m_itemw;

        void init(cont_typ& c, str_typ val)
        {
            c.resize(1);
            c[0] = val;
        }

        // read
        const str_typ& at(const cont_typ& c, std::size_t index) const
        {
            // return the highest item if it does not exist at the given index
            return c[std::min(index, c.size() - 1)];
        }

        // write
        str_typ& at(cont_typ& c, std::size_t index)
        {
            if( c.size() <= index )
            {
                c.resize(index+1);
            }

            return c[index];
        }
    };

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

        // function ptr object for setitemw
        //template<typename Elem>
        void setitemwFn(std::ios_base& ostr, std::streamsize sz, std::size_t depth)
        {
            explore::get_stream_state<container_stream_state<char> >(ostr)->set_itemw(sz, depth);
        }
    }

    struct stream_normal_value
    {
        template<typename Elem, typename Tr, typename T>
        void operator()(std::basic_ostream<Elem, Tr>& ostr, const T& val, container_stream_state<Elem>*)
        {
            ostr << val;
        }
    };

    // stream value from associative container
    struct stream_map_value
    {
        template<typename Elem, typename Tr, typename T>
        void operator()(std::basic_ostream<Elem, Tr>& ostr, const T& val, container_stream_state<Elem>* state)
        {
            ostr << state->assoc_start() << val.first << state->assoc_separator() << val.second << state->assoc_end();
        }
    };

    template<typename Elem, typename Tr, typename FwdIter, typename F>
    std::basic_ostream<Elem, Tr>& stream_container(std::basic_ostream<Elem, Tr>& ostr, FwdIter first, FwdIter last, F f)
    {
        // grab the extra data embedded in the stream object.
        container_stream_state<Elem>* state = explore::get_stream_state<container_stream_state<Elem> >(ostr);
        detail::depth_guard<Elem> guard(state);
        std::size_t depth = state->depth();

        // starting delimiter
        ostr << state->start(depth);

        while( first != last )
        {
            // value
            f(ostr, *first, state);
            if( ++first != last )
            {
                // separation delimiter
                ostr << state->separator(depth);
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

    // used to work around some problems with overriding for operator<< for boost::iterator_range
    // there already exists such an operator that does not do what we want.
    template<typename T>
    struct iterator_range_wrapper
    {
        iterator_range_wrapper(const boost::iterator_range<T>& ir) : t(ir) {}
        boost::iterator_range<T> t;
        typedef typename boost::mpl::if_<typename is_assoc_iter<T>::type, stream_map_value, stream_normal_value>::type stream_type;
    };

    template<typename T>
    iterator_range_wrapper<T> as_container(const boost::iterator_range<T>& ir)
    {
        return iterator_range_wrapper<T>(ir);
    }

    template< typename IteratorT >
    inline iterator_range_wrapper<IteratorT>
    make_iterator_range(IteratorT Begin, IteratorT End)
    {
        return iterator_range_wrapper<IteratorT>(boost::make_iterator_range(Begin, End));
    }

    template< class ForwardRange >
    iterator_range_wrapper<BOOST_DEDUCED_TYPENAME boost::range_const_iterator<ForwardRange>::type>
        make_iterator_range(const ForwardRange& r)
    {
        return as_container(boost::make_iterator_range(r));
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

    // stream boost::iterator_range
    template<typename Elem, typename Tr, typename T>
    std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr,
        const iterator_range_wrapper<T>& r)
    {
        return stream_container(ostr, r.t.begin(), r.t.end(), typename iterator_range_wrapper<T>::stream_type());
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

    detail::manipfunc<std::streamsize> setitemw(std::streamsize sz, std::size_t depth = 0)
    {
        return detail::manipfunc<std::streamsize>(detail::setitemwFn, sz, depth);
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
