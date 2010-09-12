//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2007 Anthony Williams
// (C) Copyright Vicente J. Botet Escriba 2010. 
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying fileLICENSE_1_0.txt 
// or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/sync for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNC_LOCK_GUARD_HPP
#define BOOST_SYNC_LOCK_GUARD_HPP

#include <boost/sync/locks/lock_options.hpp>

namespace boost {

    template<typename Mutex>
    class lock_guard
    {
    public:
        typedef Mutex lockable_type;
        explicit lock_guard(lockable_type& m_):
            m(m_)
        {
            m.lock();
        }
        lock_guard(lockable_type& m_,adopt_lock_t):
            m(m_)
        {}
        ~lock_guard()
        {
            m.unlock();
        }
    private:
        lockable_type& m;

        explicit lock_guard(lock_guard&);
        lock_guard& operator=(lock_guard&);
    };
} // namespace boost{

#endif // BOOST_SYNC_LOCK_GUARD_HPP
