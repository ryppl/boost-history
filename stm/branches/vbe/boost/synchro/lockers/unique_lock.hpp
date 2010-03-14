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

#ifndef BOOST_SYNCHO_LOCKER_UNIQUE_LOCK__HPP
#define BOOST_SYNCHO_LOCKER_UNIQUE_LOCK__HPP

//-----------------------------------------------------------------------------
#include <boost/synchro/detail/config.hpp>
//-----------------------------------------------------------------------------
#include <boost/synchro/tags.hpp>
#include <boost/synchro/exceptions.hpp>
#include <boost/synchro/lockable.hpp>
//-----------------------------------------------------------------------------
#include <boost/synchro/call_context.hpp>


namespace boost {
namespace synchro {

    template<typename Mutex>
    class unique_lock
    {
    private:
        Mutex* m;
        bool is_locked;
        BOOST_STM_CALL_CONTEXT_2_DCL();
    
        unique_lock(unique_lock&);
        unique_lock& operator=(unique_lock&);
    public:
        unique_lock( BOOST_STM_CALL_CONTEXT_2_PARAMS1() ):
            m(0),is_locked(false)
            BOOST_STM_CALL_CONTEXT_2_INIT()
        {}

        explicit unique_lock(Mutex& m_ BOOST_STM_CALL_CONTEXT_2_PARAMS()):
            m(&m_),is_locked(false)
            BOOST_STM_CALL_CONTEXT_2_INIT()
        {
            lock();
        }

        unique_lock(Mutex& m_,bool b  BOOST_STM_CALL_CONTEXT_2_PARAMS()):
            m(&m_),is_locked(false)
            BOOST_STM_CALL_CONTEXT_2_INIT()
        {
            if (b) lock();
        }
        unique_lock(Mutex& m_,force_lock_t  BOOST_STM_CALL_CONTEXT_2_PARAMS()):
            m(&m_),is_locked(false)
            BOOST_STM_CALL_CONTEXT_2_INIT()
        {
            lock();
        }
        unique_lock(Mutex& m_,adopt_lock_t  BOOST_STM_CALL_CONTEXT_2_PARAMS()):
            m(&m_),is_locked(true)
            BOOST_STM_CALL_CONTEXT_2_INIT()
        {}
        unique_lock(Mutex& m_,defer_lock_t  BOOST_STM_CALL_CONTEXT_2_PARAMS()):
            m(&m_),is_locked(false)
            BOOST_STM_CALL_CONTEXT_2_INIT()
        {}
        unique_lock(Mutex& m_,try_to_lock_t  BOOST_STM_CALL_CONTEXT_2_PARAMS()):
            m(&m_),is_locked(false)
            BOOST_STM_CALL_CONTEXT_2_INIT()            
        {
            try_lock();
        }
        template<typename TimeDuration>
        unique_lock(Mutex& m_,TimeDuration const& target_time  BOOST_STM_CALL_CONTEXT_2_PARAMS()):
            m(&m_),is_locked(false)
            BOOST_STM_CALL_CONTEXT_2_INIT()
        {
            lock_for(target_time);
        }
        unique_lock(Mutex& m_,system_time const& target_time  BOOST_STM_CALL_CONTEXT_2_PARAMS()):
            m(&m_),is_locked(false)
            BOOST_STM_CALL_CONTEXT_2_INIT()
        {
            lock_until(target_time);
        }
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
#if 0
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

        void swap(detail::thread_move_t<unique_lock<Mutex> > other)
        {
            std::swap(m,other->m);
            std::swap(is_locked,other->is_locked);
        }
#endif
        void swap(unique_lock& other)
        {
            std::swap(m,other.m);
            std::swap(is_locked,other.is_locked);
#ifdef BOOST_STM_USES_CALL_CONTEXT
            std::swap(cctx_,other.cctx_);
            std::swap(name_,other.name_);
#endif            
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
            synchro::lock(*m  BOOST_STM_CALL_CONTEXT_2_ACT_CTX());
            is_locked=true;
        }
        bool try_lock()
        {
            if(owns_lock())
            {
                throw lock_error();
            }
            is_locked=synchro::try_lock(*m  BOOST_STM_CALL_CONTEXT_2_ACT_CTX());
            return is_locked;
        }
        template<typename TimeDuration>
        bool lock_for(TimeDuration const& relative_time)
        {
            is_locked=synchro::lock_for(*m, relative_time  BOOST_STM_CALL_CONTEXT_2_ACT_CTX());
            return is_locked;
        }

        bool lock_until(system_time const& absolute_time)
        {
            is_locked=synchro::lock_until(*m, absolute_time  BOOST_STM_CALL_CONTEXT_2_ACT_CTX());
            return is_locked;
        }
        void unlock()
        {
            if(!owns_lock())
            {
                throw lock_error();
            }
            synchro::unlock(*m  BOOST_STM_CALL_CONTEXT_2_ACT_CTX());
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

#ifdef BOOST_HAS_RVALUE_REFS
    template<typename Mutex>
    inline unique_lock<Mutex>&& move(unique_lock<Mutex>&& ul)
    {
        return ul;
    }
#endif

}}
#endif // BOOST_SYNCHO_LOCKER_UNIQUE_LOCK__HPP


