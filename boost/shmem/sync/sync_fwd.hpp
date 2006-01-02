//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga 2005. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/shmem for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SHMEM_SYNC_FWD_HPP
#define BOOST_SHMEM_SYNC_FWD_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

namespace boost { namespace shmem {

template <typename Mutex>
class scoped_lock;

template <typename TryMutex>
class scoped_try_lock;

template <typename TimedMutex>
class scoped_timed_lock;

class named_semaphore;

class null_mutex;

class shared_condition;

class shared_mutex;

class shared_try_mutex;

class shared_timed_mutex;

class shared_recursive_mutex;

class shared_recursive_try_mutex;

class shared_recursive_timed_mutex;

class shared_read_write_mutex;

class shared_try_read_write_mutex;

class shared_timed_read_write_mutex;

class shared_barrier;

struct xtime;

}  }  //namespace boost { namespace shmem {

#endif //#ifndef BOOST_SHMEM_SYNC_FWD_HPP
