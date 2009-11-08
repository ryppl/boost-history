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

#ifndef BOOST_STM_SYNCH_LOCK_GUARD__HPP
#define BOOST_STM_SYNCH_LOCK_GUARD__HPP

//-----------------------------------------------------------------------------
#include <boost/synchro/tags.hpp>
#include <boost/synchro/exceptions.hpp>
#include <boost/synchro/lockable/lock.hpp>
#include <boost/synchro/lockable/unlock.hpp>
//-----------------------------------------------------------------------------

namespace boost {
namespace stm {

#if 0
    template<typename Mutex, Mutex& m>
    class static_lock_guard {
    private:

        explicit static_lock_guard(static_lock_guard&);
        static_lock_guard& operator=(static_lock_guard&);
    public:
        static_lock_guard()
        {
            lock(m);
        }
        explicit static_lock_guard(adopt_lock_t)
        {}
        ~static_lock_guard() {
            unlock(m);
        }
    };
#endif
    template<typename Mutex>
    class lock_guard {
    private:
        //Mutex& m;
        typedef Mutex mutex_type;
        mutex_type& m;

        explicit lock_guard(lock_guard&);
        lock_guard& operator=(lock_guard&);
    public:
        explicit lock_guard(mutex_type& m_)
            : m(m_)
        {
            m.lock();
        }
        lock_guard(mutex_type& m_, bool cnd, adopt_lock_t)
            : m(m_)
        {}
        ~lock_guard() {
            m.unlock();
        }
    };

#if 0
    template<typename Mutex, Mutex& m>
    class static_lock_guard_if {
    private:
        bool cnd_;

        explicit static_lock_guard_if(static_lock_guard_if&);
        static_lock_guard_if& operator=(static_lock_guard_if&);
    public:
        explicit static_lock_guard_if(bool cnd)
            : cnd_(cnd)
        {
            if (cnd_) lock(m);
        }
        static_lock_guard_if(bool cnd,adopt_lock_t)
            : cnd_(cnd)
        {}
        ~static_lock_guard_if() {
            if (cnd_) unlock(m);
        }
    };

    template<typename Mutex>
    class lock_guard_if {
    private:
        Mutex& m;
        bool cnd_;

        explicit lock_guard_if(lock_guard_if&);
        lock_guard_if& operator=(lock_guard_if&);
    public:
        lock_guard_if(Mutex& m_, bool cnd)
            : m(m_)
            , cnd_(cnd)
        {
            if (cnd_) lock(m);
        }
        lock_guard_if(Mutex& m_, bool cnd, adopt_lock_t)
            : m(m_)
            , cnd_(cnd)
        {}
        ~lock_guard_if() {
            if (cnd_) unlock(m);
        }
    };
#endif

}}
#endif // BOOST_STM_SYNCH_LOCK_GUARD__HPP

