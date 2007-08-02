//
// container_stream_state.hpp - stream state for containers
//
// Copyright (C) 2007, Jeffrey Faust
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_EXPLORE_CONTAINER_STREAM_STATE_H
#define BOOST_EXPLORE_CONTAINER_STREAM_STATE_H

#include <string>
#include <vector>

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
            : m_depth(0), m_rows(0), m_itemwidth(0)
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
        std::size_t rows() const { return m_rows; }
        std::size_t itemwidth() const { return m_itemwidth; }

        // write
        void set_separator(const str_typ& str, std::size_t index = 0) { at(m_separator, index) = str; }
        void set_start(const str_typ& str, std::size_t index = 0) { at(m_start, index) = str; }
        void set_end(const str_typ& str, std::size_t index = 0) { at(m_end, index) = str; }
        void set_assoc_separator(const str_typ& str, std::size_t index = 0) { at(m_assoc_separator, index) = str; }
        void set_assoc_start(const str_typ& str, std::size_t index = 0) { at(m_assoc_start, index) = str; }
        void set_assoc_end(const str_typ& str, std::size_t index = 0) { at(m_assoc_end, index) = str; }
        void set_rows(std::size_t rows, std::size_t index = 0) { m_rows = rows; }
        void set_itemwidth(std::size_t iw, std::size_t index = 0) { m_itemwidth = iw; }

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

        std::size_t m_rows;
        std::size_t m_itemwidth;

        template<typename T>
        void init(cont_typ& c, const T& val)
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
}

#endif
