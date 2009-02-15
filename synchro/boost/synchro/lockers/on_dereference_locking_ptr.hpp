//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_ON_DEREFERENCE_LOCKING_PTR__HPP
#define BOOST_SYNCHRO_ON_DEREFERENCE_LOCKING_PTR__HPP

#include <boost/synchro/detail/deleted_functions.hpp>

namespace boost {  namespace synchro {

template<typename T, typename Lockable=T>
class on_dereference_locking_ptr;
template<typename T, typename Lockable>
class on_dereference_locking_ptr
{
public:
  class pointer
  {
  public:
    explicit pointer(T* target, Lockable* mutex)
    : target_(target), mutex_(mutex), locked_(false)
    {
    }
    ~pointer()
    {
      if(locked_)
        mutex_->unlock();
    }
    T *operator->()
    {
      mutex_->lock();
      locked_ = true;
      return target_;
    }
  private:
    T* target_;
    Lockable* mutex_;
    bool locked_;
  };

  explicit on_dereference_locking_ptr(T &target, Lockable& mutex)
  : target_(&target), mutex_(&mutex)
  {
  }
  pointer operator->() const
  {
    return pointer(target_, mutex_);
  }
    BOOST_DEFAULT_CONSTRUCTOR_DELETE(on_dereference_locking_ptr) /*< disable default construction >*/
    BOOST_COPY_CONSTRUCTOR_DELETE(on_dereference_locking_ptr) /*< disable copy construction >*/
    BOOST_COPY_ASSIGNEMENT_DELETE(on_dereference_locking_ptr) /*< disable copy asignement >*/
private:
  T* target_;
  Lockable* mutex_;
};


template<typename Lockable>
class on_dereference_locking_ptr<Lockable,Lockable>
{
public:
  class pointer
  {
  public:
    explicit pointer(Lockable *target)
    : target_(target), locked_(false)
    {
    }
    ~pointer()
    {
      if(locked_)
        target_->unlock();
    }
    Lockable *operator->()
    {
      target_->lock();
      locked_ = true;
      return target_;
    }
  private:
      Lockable *target_;
      bool locked_;
  };

  explicit on_dereference_locking_ptr(Lockable &target)
  : target(&target)
  {
  }
  pointer operator->() const
  {
    return pointer(target);
  }
    BOOST_DEFAULT_CONSTRUCTOR_DELETE(on_dereference_locking_ptr) /*< disable default construction >*/
    BOOST_COPY_CONSTRUCTOR_DELETE(on_dereference_locking_ptr) /*< disable copy construction >*/
    BOOST_COPY_ASSIGNEMENT_DELETE(on_dereference_locking_ptr) /*< disable copy asignement >*/
private:
    Lockable *target;
};


#if 0
template<typename T, typename RW_MUTEX>
class on_dereference_read_locking_ptr
{
public:
  class pointer
  {
  public:
    explicit pointer(const T* target, RW_MUTEX* mutex)
    : target_(target), mutex_(mutex), locked(false)
    {
    }
    ~pointer()
    {
      if(locked)
        mutex->unlock();
    }
    const T *operator->() const
    {
      mutex->read_lock();
      locked = true;
      return target;
    }
  private:
    const T* target_;
    mutable T* mutex_;
    bool locked;
  };
  explicit on_dereference_read_locking_ptr(const T &target, RW_MUTEX& mutex)
  : target_(&target), mutex_(&mutex)
  {
  }
  pointer operator->() const
  {
    return pointer(target_, mutex_);
  }
private:
  const T* target_;
  mutable T* mutex_;
};

template<typename T, typename RW_MUTEX>
class on_dereference_write_locking_ptr
{
public:
  class pointer
  {
  public:
    explicit pointer(T* target, RW_MUTEX* mutex)
    : target_(target), mutex_(mutex), locked(false)
    {
    }
    ~pointer()
    {
      if(locked)
        mutex->unlock();
    }
    T *operator->()
    {
      mutex->lock();
      locked = true;
      return target;
    }
  private:
    T* target_;
    mutable T* mutex_;
    bool locked;
  };
  explicit on_dereference_write_locking_ptr(T &target, RW_MUTEX& mutex)
  : target_(&target), mutex_(&mutex)
  {
  }
  pointer operator->()
  {
    return pointer(target_, mutex_);
  }
private:
  T* target_;
  mutable T* mutex_;
};
#endif

}
}

#endif


