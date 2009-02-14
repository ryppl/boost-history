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

#include <boost/noncopyable.hpp>

namespace boost {  namespace synchro {

template<typename T, typename Lockable>
class on_dereference_locking_ptr : private boost::noncopyable
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


