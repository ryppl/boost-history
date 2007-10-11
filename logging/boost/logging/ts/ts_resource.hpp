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

#include <boost/logging/ts/ts.hpp>

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

    /** 
        Locks a resource, and uses TLS. This holds the value, and each thread caches it.
        Once at a given period (like, every 5 seconds), when used, the latest object is copied.

        @sa locker
    */
    template<class type> struct tls_resource_with_cache {
        // FIXME - implement using TLS!!!

        typedef tls_resource_with_cache<type> self_type;

        tls_resource_with_cache(const type& val , int cache_millis  ) : m_val(val) {}


    public:
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
        type m_val;
    };


}}}

#endif

