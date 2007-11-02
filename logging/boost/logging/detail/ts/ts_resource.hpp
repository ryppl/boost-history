// ts_resource.hpp

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


#ifndef JT28092007_ts_resource_HPP_DEFINED
#define JT28092007_ts_resource_HPP_DEFINED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/logging/detail/ts/ts.hpp>
#include <boost/logging/detail/tss/tss.hpp>
#include <time.h>

namespace boost { namespace logging { 
    
/** 
    @brief Contains implementations of locker objects. Such a locker can lock data for read or write.

    Assume you have
    @code
    struct data {
        std::string name;
        std::string desc;
    };

    some_locker<data> lk;
    @endcode


    In order to access the locked data for write, you do:

    @code
    some_locker<data>::write obj(lk);
    obj->name = "coolio";
    obj->desc = "it works!";
    @endcode

    In order to access the locked data for read, you do:
    @code
    some_locker<data>::read obj(lk);
    std::cout << obj->name << "/" << obj->desc;
    @endcode

*/
namespace locker {

    /** 
        the data to be locked. It's locked using default thread-safety

        @sa locker
    */
    template<class type, class mutex = boost::logging::threading::mutex > struct ts_resource {
        typedef ts_resource<type,mutex> self_type;

        ts_resource(const type& val = type() ) : m_val(val) {}

        struct read;
        struct write;
        friend struct read;
        friend struct write;

        struct write {
            self_type & self ;
            write(self_type & self) : self(self) {
                self.m_cs.Lock();
            }
            ~write() {
                self.m_cs.Unlock();
            }

            type & use() { return self.m_val ; }
            type* operator->() { return &use(); }
        };

        struct read {
            const self_type & self ;
            read(const self_type & self) : self(self) {
                self.m_cs.Lock();
            }
            ~read() {
                self.m_cs.Unlock();
            }

            const type & use() { return self.m_val ; }
            const type* operator->() { return &use(); }
        };


    private:
        mutable mutex m_cs;
        type m_val;
    };




#ifndef BOOST_LOG_NO_TSS

    /** 
        Locks a resource, and uses TLS. This holds the value, and each thread caches it.
        Once at a given period (like, every 5 seconds), when used, the latest object is copied.

        @sa locker
        @sa default_cache_millis how many secs to cache the data. By default, 5
    */
    template<class type, int default_cache_secs = 5> struct tss_resource_with_cache {
        // FIXME - implement using TLS!!!
//        I NEED TO CREATE TESTS FOR tss_value / tss_resource_with_cache

        typedef tss_resource_with_cache<type, default_cache_secs> self_type;

    private:
        struct value_and_time {
            value_and_time(const type & val = type() ) : val(val) {
                time_ = time(0);
            }
            type val;
            time_t time_;
        };

    public:
        tss_resource_with_cache(const type& val , int cache_secs = default_cache_secs ) : m_val(val), m_cache_secs(cache_secs) {}

        struct read;
        struct write;
        friend struct read;
        friend struct write;

        struct write {
            self_type & self ;
            write(self_type & self) : self(self) {
            }
            ~write() {
            }

            type & use() { return self.m_val ; }
            type* operator->() { return &use(); }
        };

        struct read {
            const self_type & self ;
            read(const self_type & self) : self(self) {
            }
            ~read() {
            }

            const type & use() { return self.m_val ; }
            const type* operator->() { return &use(); }
        };



    private:
        int m_cache_secs;
        tss_value<value_and_time> m_cache;
        type m_val;
    };

#endif

}}}

#endif

