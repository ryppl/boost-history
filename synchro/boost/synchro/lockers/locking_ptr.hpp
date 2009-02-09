//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_LOCKING_PTR__HPP
#define BOOST_SYNCHRO_LOCKING_PTR__HPP

#include "boost/noncopyable.hpp"
#include "boost/thread/mutex.hpp"

namespace boost { namespace synchro {
/**
* templated with the type of the controlled variable, and the mutex type.
* @use For example, if you want to control a Product_Queue, you use a
* locking_ptr <Product_Queue> that you initialize with a variable of type volatile Product_Queue.
* <code>
* volatile Product_Queue& q;
* boost::mutex m;
* locking_ptr <const Product_Queue> q_lptr(q, m);
* // q_lptr allows to access all the Product_Queue function
* <code>

* <code>
* volatile const Product_Queue& q;
* rw_mutex m;
* rw_locking_ptr <const Product_Queue, mutex> q_rlptr(q, m);
* // q_rlptr allows to access all the const Product_Queue function by read locking  the mutex
* <code>

* <code>
* volatile Product_Queue& wq;
* rw_mutex m;
* rw_locking_ptr <Product_Queue, mutex> q_wlptr(wq, m);
* // q_wlptr allows to access all the wq Product_Queue function by write locking the mutex
* <code>

* <code>
* volatile Product_Queue& wq;
* rw_mutex m;
* {
*   rw_locking_ptr <const Product_Queue, mutex> q_rlptr(wq, m);
*   // q_rlptr allows to access all the const Product_Queue function by read locking  the mutex
*   {
*     rw_promoting_ptr <Product_Queue, mutex> q_wlptr(q_rlptr);
*     // q_wlptr allows to access all the wq Product_Queue function by write locking the mutex
*   }
* }
* <code>

* <code>
* volatile Product_Queue& wq;
* rw_mutex m;
* {
*   rw_locking_ptr <Product_Queue, mutex> q_wlptr(wq, m);
*   // q_wlptr allows to access all the wq Product_Queue function by write locking the mutex
*   {
*     rw_demoting_ptr <const Product_Queue, mutex> q_rlptr(q_wlptr);
*     // q_rlptr allows to access all the const Product_Queue function by read locking  the mutex
*   }
* }

*/
//[locking_ptr
template <typename T, typename Lockable=boost::mutex>
class locking_ptr : private boost::noncopyable /*< Is not copyable >*/ {
public:
    typedef T value_type;
    typedef Lockable lockable_type;

    locking_ptr(volatile value_type& obj, lockable_type& mtx) /*< volatile >*/
        : ptr_(const_cast<value_type*>(&obj)) /*< const_cast >*/
        , mtx_(mtx)
    {    mtx_.lock();    } /*< locks on construction >*/
    ~locking_ptr()
    {    mtx_.unlock();    } /*< unlocks on destruction >*/

    /*< smart pointer related operations >*/
    value_type& operator*()
    {    return *ptr_;    }
    value_type* operator->()
    {   return ptr_;   }
private:
    value_type* ptr_;
    lockable_type& mtx_;
};
//]

//[locking_ptr_lockable_value_type
template <typename Lockable>
class locking_ptr_1 : private boost::noncopyable {
public:
    typedef Lockable value_type;
    typedef Lockable mutex_type;

    locking_ptr_1(volatile value_type& obj)
       : ptr_(const_cast<value_type*>(&obj))
    {    ptr_->lock();    }
    ~locking_ptr_1()
    {    ptr_->unlock();    }

    value_type& operator*()
    {    return *ptr_;    }
    value_type* operator->()
    {   return ptr_;   }
private:
    value_type* ptr_; /*< only one pointer needed >*/
};
//]

template <typename T, typename SharableLockable>
class sharable_locking_ptr : private boost::noncopyable {
public:
    typedef T value_type;
    typedef SharableLockable mutex_type;

     // Constructors/destructors
    sharable_locking_ptr(volatile value_type& obj, mutex_type& mtx)
       : ptr_(const_cast<value_type*>(&obj)),
        mtx_(&mtx)
    {    mtx_->lock();    }
    ~sharable_locking_ptr()
    {    mtx_->unlock();    }
    // Pointer behavior
    value_type& operator*()
    {    return *ptr_;    }
    const value_type& operator*() const
    {    return *ptr_;    }
    value_type* operator->()
    {   return ptr_;   }
    const value_type* operator->() const
    {   return ptr_;   }
private:
    value_type* ptr_;
    mutex_type* mtx_;
};

template <typename T, typename SharableLockable>
class sharable_locking_ptr<const T, SharableLockable> : private boost::noncopyable {
public:
  typedef T value_type;
  typedef SharableLockable mutex_type;

   // Constructors/destructors
  sharable_locking_ptr(volatile const value_type& obj, mutex_type& mtx)
       : ptr_(const_cast<value_type*>(&obj)),
        mtx_(&mtx)
   {    mtx_->lock_shared();    }
   ~sharable_locking_ptr()
   {    mtx_->unlock_shared();    }
   // Pointer behavior
   value_type& operator*()
   {    return *ptr_;    }
   const value_type& operator*() const
   {    return *ptr_;    }
   value_type* operator->()
   {   return ptr_;   }
   const value_type* operator->() const
   {   return ptr_;   }
private:
   value_type* ptr_;
   mutex_type* mtx_;
};
#if 0

template <typename T, typename MUTEX>
class rw_locking_ptr<T, MUTEX> : private boost::noncopyable {
public:
  typedef const T value_type;
  typedef MUTEX mutex_type;

   // Constructors/destructors
   locking_ptr(volatile value_type& obj, mutex_type& mtx)
       : ptr_(const_cast<value_type*>(&obj)),
        mtx_(&mtx)
   {    mtx_->write_lock();    }
   ~locking_ptr()
   {    mtx_->unlock();    }
   // Pointer behavior
   value_type& operator*()
   {    return *ptr_;    }
   value_type* operator->()
   {   return ptr_;   }
private:
   value_type* ptr_;
   mutex_type* mtx_;
};

template <typename T, typename MUTEX>
class rw_locking_ptr<const T, MUTEX> : private boost::noncopyable {
public:
  typedef T value_type;
  typedef MUTEX mutex_type;

   // Constructors/destructors
   locking_ptr(volatile const value_type& obj, mutex_type& mtx)
       : ptr_(const_cast<value_type*>(&obj)),
        mtx_(&mtx)
   {    mtx_->read_lock();    }
   ~locking_ptr()
   {    mtx_->unlock();    }
   // Pointer behavior
   const value_type& operator*()
   {    return *ptr_;    }
   const value_type* operator->()
   {   return ptr_;   }
private:
   value_type* ptr_;
   mutex_type* mtx_;
};

template <typename LOCKABLE>
class locking_ptr<LOCKABLE, LOCKABLE> : private boost::noncopyable {
public:
  typedef LOCKABLE value_type;
  typedef LOCKABLE mutex_type;

   // Constructors/destructors
   locking_ptr(volatile value_type& obj)
       : ptr_(const_cast<value_type*>(&obj)),
   {    ptr_->lock();    }
   ~locking_ptr()
   {    ptr_->unlock();    }
   // Pointer behavior
   value_type& operator*()
   {    return *ptr_;    }
   value_type* operator->()
   {   return ptr_;   }
private:
   value_type* ptr_;
};

template <typename T, typename MUTEX>
class read_locking_ptr : private boost::noncopyable {
public:
  typedef T value_type;
  typedef MUTEX mutex_type;

   // Constructors/destructors
   read_locking_ptr(volatile const value_type& obj, mutex_type& mtx)
       : ptr_(const_cast<value_type*>(&obj)),
        mtx_(&mtx)
   {    mtx_->read_lock();    }
   ~locking_ptr()
   {    mtx_->unlock();    }
   // Pointer behavior
   const value_type& operator*() const
   {    return *ptr_;    }
   const value_type* operator->() const
   {   return ptr_;   }
private:
   const value_type* ptr_;
   mutex_type* mtx_;
};

#endif
}
}
#endif

