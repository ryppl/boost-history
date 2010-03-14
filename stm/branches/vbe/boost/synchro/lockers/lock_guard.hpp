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

#ifndef BOOST_SYNCHO_LOCKER_LOCK_GUARD_HPP
#define BOOST_SYNCHO_LOCKER_LOCK_GUARD_HPP

//-----------------------------------------------------------------------------
#include <boost/synchro/tags.hpp>
#include <boost/synchro/exceptions.hpp>
#include <boost/synchro/lockable/lock.hpp>
#include <boost/synchro/lockable/unlock.hpp>
//-----------------------------------------------------------------------------
#include <boost/synchro/call_context.hpp>

namespace boost {
namespace synchro {

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

    template<typename Mutex>
    class lock_guard {
    private:
        Mutex& m;
        BOOST_STM_CALL_CONTEXT_2_DCL();

        explicit lock_guard(lock_guard&);
        lock_guard& operator=(lock_guard&);
    public:
        explicit lock_guard(Mutex& m_ BOOST_STM_CALL_CONTEXT_2_PARAMS())
            : m(m_)
            BOOST_STM_CALL_CONTEXT_2_INIT()
        {
            lock(m BOOST_STM_CALL_CONTEXT_2_ACT_CTX());
        }
        lock_guard(Mutex& m_, bool cnd, adopt_lock_t BOOST_STM_CALL_CONTEXT_2_PARAMS())
            : m(m_)
            BOOST_STM_CALL_CONTEXT_2_INIT()
        {}
        ~lock_guard() {
            unlock(m BOOST_STM_CALL_CONTEXT_2_ACT_CTX());
        }
    };

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
        BOOST_STM_CALL_CONTEXT_2_DCL();

        explicit lock_guard_if(lock_guard_if&);
        lock_guard_if& operator=(lock_guard_if&);
    public:
        lock_guard_if(Mutex& m_, bool cnd  BOOST_STM_CALL_CONTEXT_2_PARAMS())
            : m(m_)
            , cnd_(cnd)
            BOOST_STM_CALL_CONTEXT_2_INIT()
        {
            if (cnd_) lock(m  BOOST_STM_CALL_CONTEXT_2_ACT_CTX());
        }
        lock_guard_if(Mutex& m_, bool cnd, adopt_lock_t  BOOST_STM_CALL_CONTEXT_2_PARAMS())
            : m(m_)
            , cnd_(cnd)
            BOOST_STM_CALL_CONTEXT_2_INIT()
        {}
        ~lock_guard_if() {
            if (cnd_) unlock(m  BOOST_STM_CALL_CONTEXT_2_ACT_CTX());
        }
    };

}}
#endif // BOOST_SYNCHO_LOCKER_LOCK_GUARD_HPP

