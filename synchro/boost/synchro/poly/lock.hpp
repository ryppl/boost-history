//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_POLY_LOCK__HPP
#define BOOST_SYNCHRO_POLY_LOCK__HPP
#include <boost/chrono/chrono.hpp>
#include <boost/thread/thread_time.hpp>

namespace boost { namespace synchro { namespace poly {

//////////////////////////////////////////////////////////////////////////////

struct exclusive_lock {
    virtual ~exclusive_lock()=0;
    virtual void lock()=0;
    virtual void unlock()=0;
    virtual bool try_lock()=0;
};

//////////////////////////////////////////////////////////////////////////////

struct timed_lock : exclusive_lock {
    virtual ~timed_lock()=0;
    virtual bool try_lock_until(boost::system_time  const&  abs_time)=0;
    template<typename DurationType>
    bool try_lock_for(DurationType const& rel_time)
    {
        return try_lock_until(get_system_time()+rel_time);
    }
    virtual bool lock_until(boost::system_time  const&  abs_time)=0;
    template<typename DurationType>
    bool lock_for(DurationType const& rel_time)
    {
        return lock_until(get_system_time()+rel_time);
    }
    virtual bool try_lock_until(chrono::system_clock::time_point const & abs_time)=0;
    virtual bool try_lock_for(chrono::nanoseconds const & relative_time)=0;
    virtual void lock_until(chrono::system_clock::time_point const & abs_time)=0;
    virtual void lock_for(chrono::nanoseconds const & relative_time)=0;
};


//////////////////////////////////////////////////////////////////////////////

struct sharable_lock : timed_lock {
    virtual ~sharable_lock();
    virtual void lock_shared()=0;
    virtual bool try_lock_shared()=0;
    virtual bool try_lock_shared_until(boost::system_time  const&  abs_time)=0;
    template<typename DurationType>
    bool try_lock_shared_for(DurationType const& rel_time)
    {
        return try_lock_shared_until(get_system_time()+rel_time);
    }
    virtual void lock_shared_until(boost::system_time  const&  abs_time)=0;
    template<typename DurationType>
    void lock_shared_for(DurationType const& rel_time)
    {
        lock_shared_until(get_system_time()+rel_time);
    }
    virtual void unlock_shared()=0;
    virtual bool try_lock_shared_until(chrono::system_clock::time_point const & abs_time)=0;
    virtual bool try_lock_shared_for(chrono::nanoseconds const & relative_time)=0;
    virtual void lock_shared_until(chrono::system_clock::time_point const & abs_time)=0;
    virtual void lock_shared_for(chrono::nanoseconds const & relative_time)=0;
};

//////////////////////////////////////////////////////////////////////////////

struct upgradable_lock : sharable_lock {
    virtual ~upgradable_lock();
    virtual void lock_upgrade()=0;
    
    virtual bool try_lock_upgrade_until(system_time const&t)=0;
    template<typename TimeDuration>   
    bool try_lock_upgrade_for(TimeDuration const&t)   
    {
        return try_lock_upgrade_until(get_system_time()+t);
    }
    
    virtual void lock_upgrade_until(system_time const&t)=0;
    template<typename TimeDuration>   
    void lock_upgrade_for(TimeDuration const&t)
    {
        lock_upgrade_until(get_system_time()+t);
    }
    virtual bool try_lock_upgrade_until(chrono::system_clock::time_point const & abs_time)=0;
    virtual bool try_lock_shared_for(chrono::nanoseconds const & relative_time)=0;
    virtual void lock_upgrade_until(chrono::system_clock::time_point const & abs_time)=0;
    virtual void lock_shared_for(chrono::nanoseconds const & relative_time)=0;
    
    virtual void unlock_upgrade()=0;
    virtual void unlock_upgrade_and_lock()=0;
    virtual void unlock_and_lock_upgrade()=0;
    virtual void unlock_and_lock_shared()=0;
    virtual void unlock_upgrade_and_lock_shared()=0;
};

}
}
}

#endif
