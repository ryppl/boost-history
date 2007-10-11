// optimize.hpp

// Boost Logging library
//
// Author: John Torjo, www.torjo.com
//
// Copyright (C) 2007 John Torjo (see www.torjo.com for email)
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org for updates, documentation, and revision history.
// See http://www.torjo.com/log2/ for more details

#ifndef JT28092007_optimize_HPP_DEFINED
#define JT28092007_optimize_HPP_DEFINED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/logging/detail/fwd.hpp>
#include <boost/assert.hpp>

namespace boost { namespace logging { 

/** 
    @brief Gathering the message: contains optimizers for formatting and/or destinations: for example, caching techniques
*/
namespace optimize {

    /** 
        It optimizes the formatting for prepending and/or appending strings to the original message

        It keeps all the modified message in one string. Useful if some formatter needs to access the whole
        string at once.

        reserve_prepend() - the size that is reserved for prepending (similar to string::reserve function)
        reserve_append() - the size that is reserved for appending (similar to string::reserve function)

        Note : as strings are prepended, reserve_prepend() shrinks. Same goes for append.
    */
    template<class string_type_ = boost::logging::hold_string_type > struct cache_string_one_str {
        typedef cache_string_one_str<string_type_> self_type;
        typedef string_type_ string_type;

        /** 
        @param reserve_prepend - how many chars to have space to prepend by default
        @param reserve_append - how many chars to have space to append by default
        @param grow_size - in case we add a string and there's no room for it, with how much should we grow? We'll
                           grow this much in addition to the added string - in the needed direction
         */
        cache_string_one_str(int reserve_prepend, int reserve_append, int grow_size = 10) 
                : m_reserve_prepend(reserve_prepend), m_reserve_append(reserve_append), m_grow_size(grow_size), m_full_msg_computed(false) {}

        /** 
        @param msg - the message that is originally cached
        @param reserve_prepend - how many chars to have space to prepend by default
        @param reserve_append - how many chars to have space to append by default
        @param grow_size - in case we add a string and there's no room for it, with how much should we grow? We'll
                           grow this much in addition to the added string - in the needed direction
         */
        cache_string_one_str(const string_type & msg, int reserve_prepend, int reserve_append, int grow_size = 10) 
                : m_reserve_prepend(reserve_prepend), m_reserve_append(reserve_append), m_grow_size(grow_size), m_full_msg_computed(false) {
            set_string(msg);
        }

        void set_string(const string_type & str) {
            m_str.resize( str.size() + m_reserve_prepend + m_reserve_append);
            std::copy( str.begin(), str.end(), m_str.begin() + m_reserve_prepend);
            m_full_msg_computed = false;
        }

        int reserve_prepend() const { return m_reserve_prepend; }
        int reserve_append() const { return m_reserve_append; }
        int grow_size() const { return m_grow_size; }

        void reserve_prepend(int new_size) {
            resize_string(new_size, m_reserve_append);
        }

        void reserve_append(int new_size) {
            resize_string(m_reserve_prepend, new_size);
        }

        void grow_size(int new_size) {
            m_grow_size = new_size;
        }


        void prepend_string(const string_type & str) {
            if ( m_reserve_prepend < (int)str.size()) {
                int new_reserve_prepend = (int)str.size() + m_grow_size ;
                resize_string( new_reserve_prepend, m_reserve_append);
            }

            BOOST_ASSERT(m_reserve_prepend >= (int)str.size() );

            int start_idx = m_reserve_prepend - (int)str.size();
            m_reserve_prepend -= (int)str.size();

            std::copy(str.begin(), str.end(), m_str.begin() + start_idx);
            m_full_msg_computed = false;
        }

        void append_string(const string_type & str) {
            if ( m_reserve_append < (int)str.size()) {
                int new_reserve_append = (int)str.size() + m_grow_size ;
                resize_string( m_reserve_prepend, new_reserve_append);
            }

            BOOST_ASSERT(m_reserve_append >= (int)str.size() );

            int start_idx = (int)m_str.size() - m_reserve_append;

            std::copy(str.begin(), str.end(), m_str.begin() + start_idx);
            m_reserve_append -= (int)str.size();
            m_full_msg_computed = false;
        }

        /** 
            writes the current cached contents to a stream
        */
        template<class stream_type> void to_stream(stream_type & stream) const {
            stream.write( m_str.begin() + m_reserve_prepend, m_str.size() - m_reserve_prepend - m_reserve_append);
        }

        /** 
            returns the full string
        */
        const string_type & full_string() const {
            if ( !m_full_msg_computed) {
                m_full_msg_computed = true;
                m_full_msg = m_str.substr(m_reserve_prepend, m_str.size() - m_reserve_prepend - m_reserve_append );
            } 
            return m_full_msg;
        }

        operator const string_type&() const { return full_string(); }

    private:
        void resize_string(int reserve_prepend, int reserve_append) {
            BOOST_ASSERT( reserve_prepend >= 0 && reserve_append >= 0);

            if ( is_string_set() ) {
                int to_add = reserve_prepend + reserve_append - m_reserve_prepend - m_reserve_append ;
                int new_size = (int)m_str.size() + to_add;
                if ( new_size < 0)
                    new_size = 0;
                // I'm creating a new string instead of resizing the existing one
                // this is because the new string could be of lower size
                string_type new_str(reserve_prepend, 0);
                int used_size = (int)m_str.size() - m_reserve_prepend - m_reserve_append;
                new_str.insert( new_str.end(), m_str.begin() + m_reserve_prepend, m_str.begin() + m_reserve_prepend + used_size);

                BOOST_ASSERT(new_size == reserve_prepend + used_size + reserve_append);

                new_str.resize( new_size, 0);
                std::swap(new_str, m_str);
            }

            m_reserve_prepend = reserve_prepend;
            m_reserve_append = reserve_append;
        }

        // if true, string was already set
        bool is_string_set() const {
            return (int)m_str.size() > 0;
        }
    private:
        int m_reserve_prepend;
        int m_reserve_append;
        int m_grow_size;
        string_type m_str;

        // caching
        mutable bool m_full_msg_computed;
        mutable string_type m_full_msg;
    };

    /** 
        This gives you 2 extra streams :
        prepend(), and append()
    */
    template<class char_type> struct extra_streams {
        // FIXME
    };

}}}

#endif

