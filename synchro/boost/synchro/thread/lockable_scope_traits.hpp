//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_THREAD_LOCKABLE_SCOPE_TRAITS__HPP
#define BOOST_SYNCHRO_THREAD_LOCKABLE_SCOPE_TRAITS__HPP

#include <boost/thread/locks.hpp>
#include <boost/thread/detail/move.hpp>
#include <boost/synchro/lockable_scope_traits.hpp>
#include <boost/synchro/lockable_traits.hpp>

namespace boost { namespace synchro {

template<>
struct scope_traits<multi_threaded_tag>
{
    typedef boost::lock_error lock_error;
    template <typename T>
    struct moved_object : boost::detail::thread_move_t<T> {
        moved_object(T& t_): boost::detail::thread_move_t<T>(t_) {}
    };
    typedef boost::defer_lock_t defer_lock_t;
    typedef boost::adopt_lock_t adopt_lock_t;
    typedef boost::try_to_lock_t try_to_lock_t;

    static const defer_lock_t& defer_lock() {return boost::defer_lock;}
    static const adopt_lock_t& adopt_lock() {return boost::adopt_lock;}
    static const try_to_lock_t& try_to_lock() {return boost::try_to_lock;}
};

template<typename Lockable>
struct lockable_scope_traits<multi_threaded_tag, Lockable> : scope_traits<multi_threaded_tag>
{
    typedef Lockable                            lockable_type;
    typedef boost::unique_lock<lockable_type>   scoped_lock;
    typedef boost::unique_lock<lockable_type>   unique_lock;
    typedef boost::shared_lock<lockable_type>   shared_lock;
    typedef boost::upgrade_lock<lockable_type>  upgrade_lock;


};

}
}

#endif
