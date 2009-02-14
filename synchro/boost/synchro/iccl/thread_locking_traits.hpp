/* Copyright 2008-2009 Vicente J. Botet Escriba
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/luid for library home page.
 */

#ifndef BOOST_ICCL_THREAD_LOCKING_TRAITS_HPP_
#define BOOST_ICCL_THREAD_LOCKING_TRAITS_HPP_

#include <boost/thread/mutex.hpp> 
#include <boost/thread/locks.hpp> 
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp> 

namespace boost {
namespace iccl {    

namespace detail {    
struct null_tag{
    template <typename T> null_tag(T&) {} 
};

}
struct null_locking_traits {
    typedef detail::null_tag    mutex_type;
    typedef detail::null_tag    scoped_lock;
    static const int try_to_lock() {return 0;}
};


struct thread_locking_traits {
    typedef boost::mutex                mutex_type;
    typedef boost::mutex::scoped_lock    scoped_lock;
    static const boost::try_to_lock_t try_to_lock() {return boost::try_to_lock;}
};

struct process_locking_traits {
    typedef boost::interprocess::interprocess_mutex            mutex_type;
    typedef boost::interprocess::scoped_lock<mutex_type>    scoped_lock;
    static boost::interprocess::detail::try_to_lock_type    try_to_lock() 
    {
        return boost::interprocess::try_to_lock;
    }
};



}
}
#endif // BOOST_LUID_THREAD_LOCKING_TRAITS_HPP_
