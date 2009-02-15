//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_LOCKERS__HPP
#define BOOST_SYNCHRO_LOCKERS__HPP


#include <boost/thread/locks.hpp>
#include <boost/synchro/lockable_traits.hpp>

namespace boost { namespace synchro {

    struct force_lock_t
    {};
    struct defer_lock_t
    {};
    struct try_to_lock_t
    {};
    struct adopt_lock_t
    {};
    struct throw_lock_t
    {};



    const force_lock_t force_lock={};
    const defer_lock_t defer_lock={};
    const try_to_lock_t try_to_lock={};
    const adopt_lock_t adopt_lock={};
    const throw_lock_t throw_lock={};

    template<typename Mutex, typename ScopeTag=typename scope_tag<Mutex>::type>
    class unique_locker;

    template<typename Mutex, typename ScopeTag=typename scope_tag<Mutex>::type>
    class try_unique_locker;

    template<typename Mutex, typename ScopeTag=typename scope_tag<Mutex>::type>
    class shared_locker;

    template<typename Mutex, typename ScopeTag=typename scope_tag<Mutex>::type>
    class upgrade_locker;

    template<typename Mutex, typename ScopeTag=typename scope_tag<Mutex>::type>
    class upgrade_to_unique_locker;

#ifdef BOOST_HAS_RVALUE_REFS
    template<typename Mutex, typename ScopeTag>
    void swap(unique_locker<Mutex, ScopeTag>&& lhs, unique_locker<Mutex, ScopeTag>&& rhs)
    {
        lhs.swap(rhs);
    }
#else
    template<typename Mutex, typename ScopeTag>
    void swap(unique_locker<Mutex, ScopeTag>& lhs, unique_locker<Mutex, ScopeTag>& rhs)
    {
        lhs.swap(rhs);
    }
#endif

#ifdef BOOST_HAS_RVALUE_REFS
    template<typename Mutex, typename ScopeTag>
    inline unique_locker<Mutex, ScopeTag>&& move(unique_locker<Mutex, ScopeTag>&& ul)
    {
        return ul;
    }
#endif

#ifdef BOOST_HAS_RVALUE_REFS
    template<typename Mutex, typename ScopeTag>
    void swap(try_unique_locker<Mutex, ScopeTag>&& lhs, try_unique_locker<Mutex, ScopeTag>&& rhs)
    {
        lhs.swap(rhs);
    }
#else
    template<typename Mutex, typename ScopeTag>
    void swap(try_unique_locker<Mutex, ScopeTag>& lhs, try_unique_locker<Mutex, ScopeTag>& rhs)
    {
        lhs.swap(rhs);
    }
#endif

#ifdef BOOST_HAS_RVALUE_REFS
    template<typename Mutex, typename ScopeTag>
    inline try_unique_locker<Mutex, ScopeTag>&& move(try_unique_locker<Mutex, ScopeTag>&& ul)
    {
        return ul;
    }
#endif
    
    #ifdef BOOST_HAS_RVALUE_REFS
    template<typename Mutex, typename ScopeTag>
    void swap(shared_locker<Mutex, ScopeTag>&& lhs,shared_locker<Mutex, ScopeTag>&& rhs)
    {
        lhs.swap(rhs);
    }
#else
    template<typename Mutex, typename ScopeTag>
    void swap(shared_locker<Mutex, ScopeTag>& lhs,shared_locker<Mutex, ScopeTag>& rhs)
    {
        lhs.swap(rhs);
    }
#endif


}}

#endif
