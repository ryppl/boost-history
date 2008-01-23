// logger_base.hpp

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


#ifndef JT28092007_logger_base_HPP_DEFINED
#define JT28092007_logger_base_HPP_DEFINED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/logging/detail/fwd.hpp>
#include <boost/logging/detail/after_being_destroyed.hpp>

#ifndef JT28092007_logger_HPP_DEFINED
#error donot include this directly. include <boost/logging/logging.hpp> instead
#endif

namespace boost { namespace logging {
    // forward declare
    template<class gather_msg , class write_msg > struct logger ;
    template<class gather_msg, class write_msg> struct forward_to_logger ;

    namespace detail {
        template<class type> type& as_non_const(const type & t) { return const_cast<type&>(t); }

        template<class gather_msg> struct find_gather_if_default {
            typedef typename use_default<gather_msg, 
                    gather::ostream_like::return_str< std::basic_string<char_type>, std::basic_ostringstream<char_type> > > ::type gather_type;
            typedef typename gather_type::msg_type msg_type;
        };

        /** 
            @brief default implementation of keeping cache 
            
            (note : you can override the cache() functions, to implement your own cache keeping strategy)
        */
        template<class cache_type> struct default_cache_keeper {
            default_cache_keeper() : m_is_cache_turned_off(false) {}
            ~default_cache_keeper() {
                // after we're destroyed, always consider the cache turned off
                // (just in case someone is using the logger after it's been destroyed
                m_is_cache_turned_off = true;
            }

            /** 
                I've implemented this as a fast "is turned off" question.

                that is, I want to avoid calling cache().is_cache_turned_off(), since calling cache() involves a virtual call
            */
            bool is_cache_turned_off() const {
                if ( m_is_cache_turned_off)
                    return true;

                if ( cache().is_cache_turned_off() ) {
                    m_is_cache_turned_off = true;
                    return true;
                }
                else
                    return false;
            }

            /** note: this call does not need to be very efficient, since the cache is used seldom, 
                      only at the beginning of the app, when the logging hasn't yet been initialized
                      thus, we can afford to make it virtual, and the code will become easier
            */
            virtual cache_type & cache()                    { return m_cache; }
            virtual const cache_type & cache() const        { return m_cache; }
        private:
            cache_type m_cache;
            mutable bool m_is_cache_turned_off;
        };

    }


    namespace detail {
        template<class gather_msg , class write_msg > struct common_base_holder {
            typedef typename detail::find_gather_if_default<gather_msg>::gather_type gather_type;
            typedef logger<gather_msg, default_> common_base_type;

            /** 
                ... returns a base object - one that can be used to log messages, without having to know the full type of the log.
                    Thus, it can also be passed between a library and the application that uses it, and vice-versa.
            */
            const common_base_type* common_base() const    { return &m_base; }
            common_base_type* common_base()                { return &m_base; }

        protected:
            // a base object - one that can be used to log messages, without having to know the full type of the log.
            forward_to_logger<gather_msg, write_msg> m_base;
        };

        // specialize - when write_msg is default, our common base is ourselves
        template<class gather_msg> struct common_base_holder<gather_msg, default_> {
            typedef typename detail::find_gather_if_default<gather_msg>::gather_type gather_type;
            typedef logger<gather_msg, default_> subclass_type;

            const subclass_type* common_base() const    { return static_cast<const subclass_type*>(this); }
            subclass_type* common_base()                { return static_cast<subclass_type*>(this); }
        };
    }


    template<class gather_msg , class write_msg, class dummy = override > struct logger_base 
            : detail::default_cache_keeper<  detail::cache_before_init<typename detail::find_gather_if_default<gather_msg>::msg_type > >,
              detail::common_base_holder<gather_msg, write_msg>,
              after_being_destroyed<dummy> {
        typedef detail::cache_before_init<typename detail::find_gather_if_default<gather_msg>::msg_type > cache_type;
        typedef detail::default_cache_keeper< cache_type > cache_base;
        using cache_base::cache;

        typedef logger<gather_msg, write_msg> subclass_type;
        typedef after_being_destroyed<dummy> after_being_destroyed_base;
        typedef detail::common_base_holder<gather_msg, write_msg> common_base_type;
        typedef typename after_being_destroyed_base::after_destroyed_func after_destroyed_func ;

        virtual void set_after_destroyed(after_destroyed_func f) {
            if ( f == after_being_destroyed_base::m_after_being_destroyed )
                // avoid infinite calls (since this can be forwarded back and forth - to the 
                // forward logger and back)
                return;
            after_being_destroyed_base::m_after_being_destroyed = f;
            // we have a forwarder - forward it to that as well
            common_base_type::common_base()->set_after_destroyed(f);
        }

    protected:
        logger_base() {}
        logger_base(const logger_base&) {}

    private:
        subclass_type & self()                      { return static_cast<subclass_type &>(*this); }
        const subclass_type & self() const          { return static_cast<const subclass_type &>(*this); }


    public:

        /** @brief Marks this logger as initialized

        You might log messages before the logger is initialized. In this case, they are cached, and written to the logger, 
        when you mark it as "initialized"
        */
        void mark_as_initialized() {
            self().turn_cache_off();
        }
    };

}}

#endif

