//
// container_stream_state.hpp - stream state for containers
//
// Copyright (C) 2007-2009, Jeffrey Faust
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_EXPLORE_CONTAINER_STREAM_STATE_H
#define BOOST_EXPLORE_CONTAINER_STREAM_STATE_H

#include <cassert>
#include <string>
#include <vector>
#include <boost/explore/stream_state.hpp>

// generate string init functions for both char and wchar_t types
#define BOOST_EXPLORE_INIT_STRING(name, str)                             \
template<typename charType> std::basic_string<charType> init_##name();   \
template<> std::basic_string<char> init_##name<char>() { return (str); } \
template<> std::basic_string<wchar_t> init_##name<wchar_t>() { return L##str; }

namespace boost { namespace explore
{
    namespace detail
    {
        BOOST_EXPLORE_INIT_STRING(separator, ", ")
        BOOST_EXPLORE_INIT_STRING(start, "[")
        BOOST_EXPLORE_INIT_STRING(end, "]")
        BOOST_EXPLORE_INIT_STRING(assoc_item_separator, ":")
        BOOST_EXPLORE_INIT_STRING(assoc_item_start, "")
        BOOST_EXPLORE_INIT_STRING(assoc_item_end, "")

        struct increment_depth;

        // read
        template<typename T>
        const T& value_at(const std::vector<T>& c, size_t level)
        {
            // return the highest item if it does not exist at the given index
            return c[std::min(level, c.size() - 1)];
        }

        // write
        template<typename T>
        T& value_at(std::vector<T>& c, size_t level)
        {
            if( c.size() <= level )
            {
                c.resize(level+1);
            }

            return c[level];
        }
    }

    // state not dependent on character type
    struct container_common_stream_state
    {
        container_common_stream_state(const std::ios_base* stream)
            : m_level(0), m_depth(0), m_cols(1), m_itemwidth(1), m_quotestrings(false)
        {
        }

        size_t set_level(size_t l)
        {
            size_t prev = m_level;
            m_level = l;
            return prev;
        }

        void level_up() { ++m_level; }
        void level_down() { --m_level; }
        size_t level() const { return m_level; }

        std::size_t depth() const { return m_depth; }

        std::size_t cols() const { return at(m_cols); }
        void set_cols(std::size_t cols) { at(m_cols) = cols; }

        std::size_t itemwidth() const { return at(m_itemwidth); }
        void set_itemwidth(std::size_t iw) { at(m_itemwidth) = iw; }

        bool quote_strings() const { return m_quotestrings; }
        void set_quote_strings(bool qs) { m_quotestrings = qs; }

        void swap(container_common_stream_state& other)
        {
            std::swap(m_level, other.m_level);
            std::swap(m_depth, other.m_depth);
            std::swap(m_cols, other.m_cols);
            std::swap(m_itemwidth, other.m_itemwidth);
            std::swap(m_quotestrings, other.m_quotestrings);
        }

        bool operator==(const container_common_stream_state& other) const
        {
            return other.m_level == m_level &&
                   other.m_depth == m_depth &&
                   other.m_cols == m_cols &&
                   other.m_itemwidth == m_itemwidth &&
                   other.m_quotestrings == m_quotestrings;
        }

   private:
        friend struct detail::increment_depth;

        typedef std::vector<std::size_t, std::allocator<std::size_t> > size_cont_typ;

        // read
        template<typename T>
        const T& at(const std::vector<T>& c) const
        {
            // return the highest item if it does not exist at the given index
            return detail::value_at(c, level());
        }

        // write
        template<typename T>
        T& at(std::vector<T>& c)
        {
            assert(depth() <= level());
            return detail::value_at(c, level());
        }

        std::size_t m_level;
        std::size_t m_depth;

        size_cont_typ m_cols;
        size_cont_typ m_itemwidth;

        bool m_quotestrings;
    };

    // A simple collection of additional stream state
    template<typename Elem>
    struct container_stream_state
    {
        typedef std::basic_string<Elem> str_typ;
        typedef std::vector<str_typ, std::allocator<str_typ> > str_cont_typ;

        explicit container_stream_state(std::ios_base* stream)
            : m_stream(stream)
        {
            assert(m_stream);
            init<Elem>();
        }

        // Concern: this is only specialized for char and wchar_t streams.
        template<typename El>
        void init()
        {
            // called not only to initialize, but to reset the state.
            init(m_separator, detail::init_separator<El>());
            init(m_start, detail::init_start<El>());
            init(m_end, detail::init_end<El>());
            init(m_assoc_item_separator, detail::init_assoc_item_separator<El>());
            init(m_assoc_item_start, detail::init_assoc_item_start<El>());
            init(m_assoc_item_end, detail::init_assoc_item_end<El>());
        }

        // read
        const str_typ& separator() const { return at(m_separator); }
        const str_typ& start(std::size_t index = 0) const { return at(m_start); }
        const str_typ& end() const { return at(m_end); }
        const str_typ& assoc_item_separator() const { return at(m_assoc_item_separator); }
        const str_typ& assoc_item_start() const { return at(m_assoc_item_start); }
        const str_typ& assoc_item_end() const { return at(m_assoc_item_end); }

        // write
        void set_separator(const str_typ& str) { at(m_separator) = str; }
        void set_start(const str_typ& str) { at(m_start) = str; }
        void set_end(const str_typ& str) { at(m_end) = str; }
        void set_assoc_item_separator(const str_typ& str) { at(m_assoc_item_separator) = str; }
        void set_assoc_item_start(const str_typ& str) { at(m_assoc_item_start) = str; }
        void set_assoc_item_end(const str_typ& str) { at(m_assoc_item_end) = str; }

        void swap(container_stream_state<Elem>& other)
        {
            assert(m_stream == other.m_stream);
            m_separator.swap(other.m_separator);
            m_start.swap(other.m_start);
            m_end.swap(other.m_end);
            m_assoc_item_separator.swap(other.m_assoc_item_separator);
            m_assoc_item_start.swap(other.m_assoc_item_start);
            m_assoc_item_end.swap(other.m_assoc_item_end);
        }

        bool operator==(const container_stream_state<Elem>& other) const
        {
            assert(m_stream == other.m_stream);
            return m_separator == other.m_separator &&
                   m_start == other.m_start &&
                   m_end == other.m_end &&
                   m_assoc_item_separator == other.m_assoc_item_separator &&
                   m_assoc_item_start == other.m_assoc_item_start &&
                   m_assoc_item_end == other.m_assoc_item_end;
        }

    private:
        container_common_stream_state* common()
        {
            return get_stream_state<container_common_stream_state>(*m_stream);
        }

        const container_common_stream_state* common() const
        {
            return get_stream_state<container_common_stream_state>(*m_stream);
        }

        size_t level() const
        {
            return common()->level();
        }

        str_cont_typ m_separator;
        str_cont_typ m_start;
        str_cont_typ m_end;
        str_cont_typ m_assoc_item_separator;
        str_cont_typ m_assoc_item_start;
        str_cont_typ m_assoc_item_end;

        std::ios_base* m_stream;

        template<typename T>
        void init(str_cont_typ& c, const T& val)
        {
            c.resize(1);
            c[0] = val;
        }

        // read
        template<typename T>
        const T& at(const std::vector<T>& c) const
        {
            // return the highest item if it does not exist at the given index
            return detail::value_at(c, level());
        }

        // write
        template<typename T>
        T& at(std::vector<T>& c)
        {
            assert(common()->depth() <= level());
            return detail::value_at(c, level());
        }
    };
}}

#endif
