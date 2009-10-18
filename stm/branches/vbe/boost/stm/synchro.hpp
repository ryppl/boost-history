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

typedef pthread_mutex_t PLOCK;

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
        void swap(unique_lock& other)
        {
            std::swap(m,other.m);
            std::swap(is_locked,other.is_locked);
        }
        
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

    template<typename Mutex>
    void swap(unique_lock<Mutex>& lhs,unique_lock<Mutex>& rhs)
    {
        lhs.swap(rhs);
    }   
}
#endif
namespace stm {


} // namespace core
}
#endif // BOOST_STM_SYNCHO__HPP


