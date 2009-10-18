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

#include <boost/stm/detail/config.hpp>

#ifdef BOOST_STM_USE_BOOST_SYNCHRO
#include <boost/synchro.hpp>
#endif

//-----------------------------------------------------------------------------
#include <stdarg.h>
#include <pthread.h>
//-----------------------------------------------------------------------------
#include <list>
#include <stdexcept>
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//#include <boost/stm/exceptions.hpp>
//-----------------------------------------------------------------------------
//#include <boost/stm/detail/memory_pool.hpp>
//-----------------------------------------------------------------------------

#ifdef BOOST_STM_USE_BOOST
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread_time.hpp>
#endif

#ifndef BOOST_STM_USE_BOOST_MUTEX
   typedef pthread_mutex_t Mutex;
#else
   typedef boost::mutex Mutex;
#endif

//-----------------------------------------------------------------------------

namespace boost { 
#ifndef BOOST_STM_USE_BOOST_SYNCHRO
namespace synchro {

    class lock_error : public std::exception
    {
    public:
        lock_error() {}
        ~lock_error() throw() {}

        virtual const char* what() const throw() {return "synchro::lock_error";}
    };    
    
    struct defer_lock_t
    {};
    struct try_to_lock_t
    {};
    struct adopt_lock_t
    {};
    
    const defer_lock_t defer_lock={};
    const try_to_lock_t try_to_lock={};
    const adopt_lock_t adopt_lock={};
        
    template< typename Lockable >
    inline void lock(Lockable& lockable) {
        lockable.lock();
    }

    template< typename Lockable >
    inline void unlock(Lockable& lockable) {
        lockable.unlock();
    }

    template< typename Lockable >
    inline bool try_lock(Lockable& lockable) {
        return lockable.try_lock();
    }

#ifdef BOOST_PTHREAD_HAS_TIMEDLOCK
    template <typename Lockable, typename TimeDuration >
    bool lock_for(Lockable& lockable, const TimeDuration& rel_time) {
        return lock_until(&lockable, get_system_time()+rel_time);
    }
    
    template< typename Lockable>
    inline bool lock_until(Lockable& lockable, system_time const& target_time) {
        return lockable.timed_lock(target_time);
    }
#endif
    template<>
    inline void lock<pthread_mutex_t>(pthread_mutex_t& lockable) {
        pthread_mutex_lock(&lockable);
    }

    template<>
    inline void unlock<pthread_mutex_t>(pthread_mutex_t& lockable) {
        pthread_mutex_unlock(&lockable);
    }
    
    template<>
    inline bool try_lock<pthread_mutex_t>(pthread_mutex_t& lockable) {
        return pthread_mutex_trylock(&lockable);
    }

#ifdef BOOST_PTHREAD_HAS_TIMEDLOCK
    template<>
    inline bool lock_until<pthread_mutex_t>(
                pthread_mutex_t& lockable, system_time const& abs_time) {
        struct timespec const timeout=boost::detail::get_timespec(abs_time);
        int const res=pthread_mutex_timedlock(&lockable,&timeout);
        //BOOST_ASSERT(!res || res==ETIMEDOUT);
        return !res;
    }
#endif       


    template<typename Mutex>
    class lock_guard
    {
    private:
        Mutex& m;

        explicit lock_guard(lock_guard&);
        lock_guard& operator=(lock_guard&);
    public:
        explicit lock_guard(Mutex& m_)
            : m(m_)
        {
            synchro::lock(m);
        }
        lock_guard(Mutex& m_,adopt_lock_t)
            : m(m_)
        {}
        ~lock_guard()
        {
            synchro::unlock(m);
        }
    };    

    template<typename Mutex>
    class lock_guard_if
    {
    private:
        Mutex& m;
        bool cnd_;

        explicit lock_guard_if(lock_guard_if&);
        lock_guard_if& operator=(lock_guard_if&);
    public:
        explicit lock_guard_if(Mutex& m_, bool cnd)
            : m(m_)
            , cnd_(cnd)
        {
            if (cnd_) synchro::lock(m);
        }
        lock_guard_if(Mutex& m_, bool cnd,adopt_lock_t)
            : m(m_)
            , cnd_(cnd)
        {}
        ~lock_guard_if()
        {
            if (cnd_) synchro::unlock(m);
        }
    };    
    
    template<typename Mutex>
    class unique_lock
    {
    private:
        Mutex* m;
        bool is_locked;
        unique_lock(unique_lock&);
        unique_lock& operator=(unique_lock&);
    public:
        unique_lock():
            m(0),is_locked(false)
        {}
        
        explicit unique_lock(Mutex& m_):
            m(&m_),is_locked(false)
        {
            lock();
        }
        
        unique_lock(Mutex& m_,adopt_lock_t):
            m(&m_),is_locked(true)
        {}
        unique_lock(Mutex& m_,defer_lock_t):
            m(&m_),is_locked(false)
        {}
        unique_lock(Mutex& m_,try_to_lock_t):
            m(&m_),is_locked(false)
        {
            try_lock();
        }
#ifdef BOOST_PTHREAD_HAS_TIMEDLOCK
        template<typename TimeDuration>
        unique_lock(Mutex& m_,TimeDuration const& target_time):
            m(&m_),is_locked(false)
        {
            timed_lock(target_time);
        }
        unique_lock(Mutex& m_,boost::system_time const& target_time):
            m(&m_),is_locked(false)
        {
            timed_lock(target_time);
        }
#endif        
#if 0        
#ifdef BOOST_HAS_RVALUE_REFS
        unique_lock(unique_lock&& other):
            m(other.m),is_locked(other.is_locked)
        {
            other.is_locked=false;
            other.m=0;
        }
        unique_lock<Mutex>&& move()
        {
            return static_cast<unique_lock<Mutex>&&>(*this);
        }


        unique_lock& operator=(unique_lock<Mutex>&& other)
        {
            unique_lock temp(other);
            swap(temp);
            return *this;
        }

        void swap(unique_lock&& other)
        {
            std::swap(m,other.m);
            std::swap(is_locked,other.is_locked);
        }
#else
        unique_lock(detail::thread_move_t<unique_lock<Mutex> > other):
            m(other->m),is_locked(other->is_locked)
        {
            other->is_locked=false;
            other->m=0;
        }

        operator detail::thread_move_t<unique_lock<Mutex> >()
        {
            return move();
        }

        detail::thread_move_t<unique_lock<Mutex> > move()
        {
            return detail::thread_move_t<unique_lock<Mutex> >(*this);
        }

        unique_lock& operator=(detail::thread_move_t<unique_lock<Mutex> > other)
        {
            unique_lock temp(other);
            swap(temp);
            return *this;
        }

        void swap(unique_lock& other)
        {
            std::swap(m,other.m);
            std::swap(is_locked,other.is_locked);
        }
        void swap(detail::thread_move_t<unique_lock<Mutex> > other)
        {
            std::swap(m,other->m);
            std::swap(is_locked,other->is_locked);
        }
#endif
#else
        void swap(unique_lock& other)
        {
            std::swap(m,other.m);
            std::swap(is_locked,other.is_locked);
        }
        
#endif        
        ~unique_lock()
        {
            if(owns_lock())
            {
                synchro::unlock(*m);
            }
        }
        void lock()
        {
            if(owns_lock())
            {
                throw lock_error();
            }
            synchro::lock(*m);
            is_locked=true;
        }
        bool try_lock()
        {
            if(owns_lock())
            {
                throw lock_error();
            }
            is_locked=synchro::try_lock(*m);
            return is_locked;
        }
#ifdef BOOST_PTHREAD_HAS_TIMEDLOCK
        template<typename TimeDuration>
        bool timed_lock(TimeDuration const& relative_time)
        {
            is_locked=synchro::timed_lock(*m, relative_time);
            return is_locked;
        }
        
        bool timed_lock(::boost::system_time const& absolute_time)
        {
            is_locked=synchro::timed_lock(*m, absolute_time);
            return is_locked;
        }
        bool timed_lock(::boost::xtime const& absolute_time)
        {
            is_locked=synchro::timed_lock(*m, absolute_time);
            return is_locked;
        }
#endif        
        void unlock()
        {
            if(!owns_lock())
            {
                throw lock_error();
            }
            synchro::unlock(*m);
            is_locked=false;
        }
            
        typedef void (unique_lock::*bool_type)();
        operator bool_type() const
        {
            return is_locked?&unique_lock::lock:0;
        }
        bool operator!() const
        {
            return !owns_lock();
        }
        bool owns_lock() const
        {
            return is_locked;
        }

        Mutex* mutex() const
        {
            return m;
        }

        Mutex* release()
        {
            Mutex* const res=m;
            m=0;
            is_locked=false;
            return res;
        }
    };

#if 0
#ifdef BOOST_HAS_RVALUE_REFS
    template<typename Mutex>
    void swap(unique_lock<Mutex>&& lhs,unique_lock<Mutex>&& rhs)
    {
        lhs.swap(rhs);
    }
#else
    template<typename Mutex>
    void swap(unique_lock<Mutex>& lhs,unique_lock<Mutex>& rhs)
    {
        lhs.swap(rhs);
    }
#endif
#else
    template<typename Mutex>
    void swap(unique_lock<Mutex>& lhs,unique_lock<Mutex>& rhs)
    {
        lhs.swap(rhs);
    }
#endif    

#if 0
#ifdef BOOST_HAS_RVALUE_REFS
    template<typename Mutex>
    inline unique_lock<Mutex>&& move(unique_lock<Mutex>&& ul)
    {
        return ul;
    }
#endif
#endif

    
}
#endif
namespace stm {

//-----------------------------------------------------------------------------
// forward declarations
//-----------------------------------------------------------------------------

typedef pthread_mutex_t PLOCK;

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

   inline int lock22(PLOCK &lock) { return pthread_mutex_lock(&lock); }
   inline int lock22(PLOCK *lock) { return pthread_mutex_lock(lock); }

   inline int trylock22(PLOCK &lock) { return pthread_mutex_trylock(&lock); }
   inline int trylock22(PLOCK *lock) { return pthread_mutex_trylock(lock); }

   inline int unlock22(PLOCK &lock) { return pthread_mutex_unlock(&lock); }
   inline int unlock22(PLOCK *lock) { return pthread_mutex_unlock(lock); }


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

class light_auto_lock
{
public:

   light_auto_lock(Mutex &mutex) : lock_(0)
   {
      do_auto_lock(&mutex);
   }

   light_auto_lock(Mutex *mutex) : lock_(0)
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
      synchro::lock(*mutex);
      hasLock_ = true;
   }

   void do_auto_unlock()
   {
      if (hasLock_)
      {
         hasLock_ = false;
         synchro::unlock(*lock_);
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
         synchro::lock(**i);
      }
   }

   //--------------------------------------------------------------------------
   ~var_auto_lock()
   {
      for (std::list<PLOCK*>::iterator i = lockList_.begin(); i != lockList_.end(); ++i)
      {
         synchro::unlock(**i);
      }
   }

private:
   std::list<lock_type*> lockList_;

};


} // namespace core
}
#endif // BOOST_STM_SYNCHO__HPP


