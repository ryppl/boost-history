/* Copyright 2008 Vicente J. Botet Escriba
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/luid for library home page.
 */

#ifndef BOOST_DSL_THREAD_MODEL_HPP_
#define BOOST_DSL_THREAD_MODEL_HPP_

#include <boost/detail/is_xxx.hpp>
#include <boost/parameter/aux_/void.hpp>
#include "boost/iccl/thread_locking_traits.hpp"
//#include "boost/iccl/process_locking_traits.hpp"

/* DSL:
 * 
 * ThreadModel: mono_threaded | multi_threaded[LockingStrategy]
 * ScopeModel: thread | inter_threads[LockingStrategy] | inter_process[LockingStrategy]
 * LockingStrategy: externally_locked[MutexType] | internally_locked[MutexType, ScopedLockType]
 * 
 */ 

namespace boost {
namespace dsl {
//  LockingStrategy 
template <typename LockingTraits=parameter::void_> 
struct externally_locked {
	typedef LockingTraits locking_traits;
};
BOOST_DETAIL_IS_XXX_DEF(externally_locked, externally_locked, 1)

template <typename LockingTraits=parameter::void_> 
struct internally_locked {
	typedef LockingTraits locking_traits;
};
BOOST_DETAIL_IS_XXX_DEF(internally_locked, internally_locked, 1)

template <typename LockingTraits=parameter::void_> 
struct no_locking {
	typedef LockingTraits locking_traits;
};
BOOST_DETAIL_IS_XXX_DEF(no_locking, no_locking, 1)


template <typename LockingStrategy=no_locking<iccl::null_locking_traits> > 
struct mono_threaded {
	typedef LockingStrategy locking_strategy;
};
BOOST_DETAIL_IS_XXX_DEF(mono_threaded, mono_threaded, 1)

template <typename LockingStrategy=internally_locked<iccl::thread_locking_traits> > 
struct multi_threaded {
	typedef LockingStrategy locking_strategy;
};
BOOST_DETAIL_IS_XXX_DEF(multi_threaded, multi_threaded, 1)

template <typename LockingStrategy=internally_locked<iccl::process_locking_traits> > 
struct multi_process {
	typedef LockingStrategy locking_strategy;
};
BOOST_DETAIL_IS_XXX_DEF(multi_process, multi_process, 1)


} // dsl
} // boost
#endif /*THREAD_MODEL_HPP_*/
