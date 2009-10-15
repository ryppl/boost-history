//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Justin E. Gottchlich 2009.
// (C) Copyright Vicente J. Botet Escriba 2009.
// Distributed under the Boost
// Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/stm for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_STM_SYNCHO__HPP
#define BOOST_STM_SYNCHO__HPP

//-----------------------------------------------------------------------------
#include <stdarg.h>
#include <pthread.h>
//-----------------------------------------------------------------------------
#include <list>
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#include <boost/stm/detail/config.hpp>
//-----------------------------------------------------------------------------
//#include <boost/stm/exceptions.hpp>
//-----------------------------------------------------------------------------
//#include <boost/stm/detail/memory_pool.hpp>
//-----------------------------------------------------------------------------

#ifndef BOOST_STM_USE_BOOST_MUTEX
   typedef pthread_mutex_t Mutex;
#else
   typedef boost::mutex Mutex;
#endif

//-----------------------------------------------------------------------------
namespace boost { namespace stm {

//-----------------------------------------------------------------------------
// forward declarations
//-----------------------------------------------------------------------------

#ifndef BOOST_STM_USE_BOOST_MUTEX
typedef pthread_mutex_t PLOCK;
#else
typedef boost::mutex PLOCK;
#endif

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#ifndef BOOST_STM_USE_BOOST_MUTEX

#if 0
   template <typename T>
   inline int lock(T *lock) { throw "unsupported lock type"; }

   template <typename T>
   inline int trylock(T *lock) { throw "unsupported lock type"; }

   template <typename T>
   inline int unlock(T *lock) { throw "unsupported lock type"; }

   template <>
   inline int lock(Mutex &lock) { return pthread_mutex_lock(&lock); }

   template <>
   inline int lock(Mutex *lock) { return pthread_mutex_lock(lock); }

   template <>
   inline int trylock(Mutex &lock) { return pthread_mutex_trylock(&lock); }

   template <>
   inline int trylock(Mutex *lock) { return pthread_mutex_trylock(lock); }

   template <>
   inline int unlock(Mutex &lock) { return pthread_mutex_unlock(&lock); }

   template <>
   inline int unlock(Mutex *lock) { return pthread_unlock(lock); }
#else
   inline int lock(PLOCK &lock) { return pthread_mutex_lock(&lock); }
   inline int lock(PLOCK *lock) { return pthread_mutex_lock(lock); }

   inline int trylock(PLOCK &lock) { return pthread_mutex_trylock(&lock); }
   inline int trylock(PLOCK *lock) { return pthread_mutex_trylock(lock); }

   inline int unlock(PLOCK &lock) { return pthread_mutex_unlock(&lock); }
   inline int unlock(PLOCK *lock) { return pthread_mutex_unlock(lock); }
#endif
#else
   inline void lock(PLOCK &lock) { lock.lock(); }
   inline void lock(PLOCK *lock) { lock->lock(); }

   inline bool trylock(PLOCK &lock) { return lock.try_lock(); }
   inline bool trylock(PLOCK *lock) { return lock->try_lock(); }

   inline void unlock(PLOCK &lock) { lock.unlock(); }
   inline void unlock(PLOCK *lock) { lock->unlock(); }
#endif


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
    template<typename Lockable>
    class lock_guard2
    {
    private:
        Lockable& m;
        //bool owns_;

        explicit lock_guard2(lock_guard2&);
        lock_guard2& operator=(lock_guard2&);
    public:
        inline explicit lock_guard2(Lockable& m_):
            m(m_)
        {
            lock();
        }
        inline ~lock_guard2()
        {
            //unlock();
        }
        //inline bool owns_lock() { return owns_;}
        inline void lock() {
            //if (owns_)
                stm::lock(m);
            //owns_=true;
        }
        inline void unlock() {
            //if (owns_)
                stm::unlock(m);
            //owns_=false;
        }
        //inline void release() {
        //    owns_=false;
        //}
    };

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

class light_auto_lock
{
public:

   light_auto_lock(Mutex &mutex) : lock_(NULL)
   {
      do_auto_lock(&mutex);
   }

   light_auto_lock(Mutex *mutex) : lock_(NULL)
   {
      do_auto_lock(mutex);
   }

   ~light_auto_lock() { do_auto_unlock(); }

   bool has_lock() { return hasLock_; }

   void release_lock() { do_auto_unlock(); }

private:

   void do_auto_lock(Mutex *mutex)
   {
      lock_ = mutex;
      pthread_mutex_lock(mutex);
      hasLock_ = true;
   }

   void do_auto_unlock()
   {
      if (hasLock_)
      {
         hasLock_ = false;
         pthread_mutex_unlock(lock_);
      }
   }

   bool hasLock_;
   Mutex *lock_;
};


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
template <typename T>
class var_auto_lock
{
public:

   //--------------------------------------------------------------------------
   typedef T lock_type;
   typedef std::list<lock_type*> lock_list;

   //--------------------------------------------------------------------------
   var_auto_lock(lock_type *l, ...)
   {
      va_list ap;
      va_start(ap, l);

      while (l)
      {
         lockList_.push_back(l);
         l = va_arg(ap, lock_type*);
      }

      va_end(ap);

      std::list<PLOCK*>::iterator i = lockList_.begin();

      for (; i != lockList_.end(); ++i)
      {
         lock(*i);
      }
   }

   //--------------------------------------------------------------------------
   ~var_auto_lock()
   {
      for (std::list<PLOCK*>::iterator i = lockList_.begin(); i != lockList_.end(); ++i)
      {
         unlock(*i);
      }
   }

private:
   std::list<lock_type*> lockList_;

};


} // namespace core
}
#endif // BOOST_STM_SYNCHO__HPP


