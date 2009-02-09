//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_PROCESS_SYNTACTIC_LOCK_TRAITS__HPP
#define BOOST_SYNCHRO_PROCESS_SYNTACTIC_LOCK_TRAITS__HPP

#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/sharable_lock.hpp>
#include <boost/interprocess/sync/upgradable_lock.hpp>
#include <boost/interprocess/sync/lock_options.hpp>
#include <boost/interprocess/detail/move.hpp>
#include <boost/synchro/lockable_scope_traits.hpp>
#include "boost/synchro/lockable_traits.hpp"

namespace boost { namespace synchro {

template<>
struct scope_traits<multi_process_tag>
{

    typedef boost::interprocess::lock_exception lock_error;
    template <typename T>
    struct moved_object : boost::interprocess::detail::moved_object<T> {
        moved_object(T& t_): boost::interprocess::detail::moved_object<T>(t_) {}
    };

    typedef boost::interprocess::defer_lock_type defer_lock_t;
    typedef boost::interprocess::accept_ownership_type adopt_lock_t;
    typedef boost::interprocess::try_to_lock_type try_to_lock_t;


    static const defer_lock_t& defer_lock()	{return boost::interprocess::defer_lock;}
	static const adopt_lock_t& adopt_lock() {return boost::interprocess::accept_ownership;}
	static const try_to_lock_t& try_to_lock()	{return boost::interprocess::try_to_lock;}

};
template<typename Lockable>
struct lockable_scope_traits<multi_process_tag, Lockable> : scope_traits<multi_process_tag>
{
	typedef Lockable                                             lockable_type;
	typedef boost::interprocess::scoped_lock<lockable_type>     scoped_lock;
	typedef boost::interprocess::scoped_lock<lockable_type>     unique_lock;
	typedef boost::interprocess::sharable_lock<lockable_type>   shared_lock;
	typedef boost::interprocess::upgradable_lock<lockable_type> upgrade_lock;

};
//template<typename Lockable>
//lockable_scope_traits<multi_process_tag, Lockable>::defer_lock_t
////boost::interprocess::detail::defer_lock_type
//lockable_scope_traits<multi_process_tag, Lockable>::defer_lock ={};
////(lockable_scope_traits<multi_process_tag, Lockable>::defer_lock_type());
//
//template<typename Lockable>
//const lockable_scope_traits<multi_process_tag, Lockable>::adopt_lock_t
//lockable_scope_traits<multi_process_tag, Lockable>::adopt_lock
//={};
//
//template<typename Lockable>
//const lockable_scope_traits<multi_process_tag, Lockable>::try_to_lock_t
//lockable_scope_traits<multi_process_tag, Lockable>::try_to_lock
//={};


}
}

#endif
