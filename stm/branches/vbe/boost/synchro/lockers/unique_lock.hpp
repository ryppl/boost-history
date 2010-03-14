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

namespace boost {
namespace synchro {

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

        unique_lock(Mutex& m_,bool b):
            m(&m_),is_locked(false)
        {
            if (b) lock();
        }
        unique_lock(Mutex& m_,force_lock_t):
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
        template<typename TimeDuration>
        unique_lock(Mutex& m_,TimeDuration const& target_time):
            m(&m_),is_locked(false)
        {
            lock_for(target_time);
        }
        unique_lock(Mutex& m_,system_time const& target_time):
            m(&m_),is_locked(false)
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
        template<typename TimeDuration>
        bool lock_for(TimeDuration const& relative_time)
        {
            is_locked=synchro::lock_for(*m, relative_time);
            return is_locked;
        }

        bool lock_until(system_time const& absolute_time)
        {
            is_locked=synchro::lock_until(*m, absolute_time);
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


