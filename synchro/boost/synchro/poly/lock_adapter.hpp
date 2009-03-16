//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_POLY_LOCK_ADAPTER__HPP
#define BOOST_SYNCHRO_POLY_LOCK_ADAPTER__HPP

#include <boost/synchro/poly/lock.hpp>
#include <boost/synchro/detail/deleted_functions.hpp>

namespace boost { namespace synchro { namespace poly {

//////////////////////////////////////////////////////////////////////////////

template <typename Lockable>
class exclusive_lock_adapter
    : public exclusive_lock
{
public:
    typedef Lockable lockable_type;
    typedef typename scope_tag<Lockable>::type scope;
    typedef typename category_tag<Lockable>::type category;
    typedef typename reentrancy_tag<Lockable>::type reentrancy;
    typedef typename timed_interface_tag<Lockable>::type timed_interface;
    typedef typename lifetime_tag<Lockable>::type lifetime;
    typedef typename naming_tag<Lockable>::type naming;

    BOOST_COPY_CONSTRUCTOR_DELETE(exclusive_lock_adapter) /*< disable copy construction >*/
    BOOST_COPY_ASSIGNEMENT_DELETE(exclusive_lock_adapter) /*< disable copy asignement >*/
    exclusive_lock_adapter(): lock_() {}
    ~exclusive_lock_adapter() {}

    void lock()
    {lock_.lock();}
    void unlock()
    {lock_.unlock();}
    bool try_lock()
    { return lock_.try_lock();}
protected:
    Lockable lock_;
};

template <typename TimeLockable>
class timed_lock_adapter
        : public exclusive_lock_adapter<TimeLockable>
        , public timed_lock
{
public:
    ~timed_lock_adapter()    {}
    bool try_lock_until(boost::system_time  const&  abs_time)
    {return the_lock().try_lock_until(abs_time);}
    template<typename DurationType>
    bool try_lock_for(DurationType const& rel_time)
    {return try_lock_for(rel_time);}
    bool lock_until(boost::system_time  const&  abs_time)
    {return the_lock().lock_until(abs_time);}
    template<typename DurationType>
    bool lock_for(DurationType const& rel_time)
    {return lock_for(rel_time);}
    
    template<typename Clock, typename Duration>
    bool try_lock_until(chrono::time_point<Clock, Duration> const & abs_time)
    {return the_lock().try_lock_until(abs_time);}
    template<typename Rep, typename Period>
    bool try_lock_for(chrono::duration<Rep, Period> const & rel_time)
    {return the_lock().try_lock_for(rel_time);}
    
    template<typename Clock, typename Duration>
    void lock_until(chrono::time_point<Clock, Duration> const & abs_time)
    {the_lock().lock_until(abs_time);}
    template<typename Rep, typename Period>
    void lock_for(chrono::duration<Rep, Period> const & rel_time)
    {the_lock().lock_for(rel_time);}
    
    
private:
    TimeLockable& the_lock() {return *static_cast<TimeLockable*>(&this->lock_);}
};


//////////////////////////////////////////////////////////////////////////////

template <typename SharableLock>
class sharable_lock_adapter
        : public timed_lock_adapter<SharableLock>
        , public sharable_lock
{
public:
    ~sharable_lock_adapter()
    {}
    void lock_shared()
    {the_lock().lock_shared();}
    bool try_lock_shared()
    {return the_lock().try_lock_shared();}
    void unlock_shared()
    {the_lock().unlock_shared();}

    bool try_lock_shared_until(system_time const& t)
    {return the_lock().try_lock_shared_until(t);}
    template<typename TimeDuration>   
    bool try_lock_shared_for(TimeDuration const& t)
    {return the_lock().try_lock_shared_for(t);}
    
    template<typename TimeDuration>   
    void lock_shared_for(TimeDuration const& t)
    {the_lock().lock_shared_for(t);}
    void lock_shared_until(system_time const& t)
    {the_lock().lock_shared_until(t);}

    template<typename Clock, typename Duration>
    bool try_lock_shared_until(chrono::time_point<Clock, Duration> const & abs_time)
    {return the_lock().try_lock_shared_until(abs_time);}
    template<typename Rep, typename Period>
    bool try_lock_shared_for(chrono::duration<Rep, Period> const & rel_time)
    {return the_lock().try_lock_shared_for(rel_time);}
    
    
    template<typename Clock, typename Duration>
    void lock_shared_until(chrono::time_point<Clock, Duration> const & abs_time)
    {the_lock().lock_shared_until(abs_time);}
    template<typename Rep, typename Period>
    void lock_shared_for(chrono::duration<Rep, Period> const & rel_time)
    {the_lock().lock_shared_for(rel_time);}
    
private:
    SharableLock& the_lock() {return *static_cast<SharableLock*>(&this->lock_);}
};

//////////////////////////////////////////////////////////////////////////////

template <typename UpgradableLock>
class upgradable_lock_adapter
    : public sharable_lock_adapter<UpgradableLock>
    , public upgradable_lock
{
public:
    ~upgradable_lock_adapter() {}
    void lock_upgrade()
    {the_lock().lock_upgrade();}
    void unlock_upgrade()
    {the_lock().unlock_upgrade();}

    void unlock_upgrade_and_lock()
    {the_lock().unlock_upgrade_and_lock();}
    void unlock_and_lock_upgrade()
    {the_lock().unlock_and_lock_upgrade();}
    void unlock_and_lock_shared()
    {the_lock().unlock_and_lock_shared();}
    void unlock_upgrade_and_lock_shared()
    {the_lock().unlock_upgrade_and_lock_shared();}

    bool try_lock_upgrade()   
    {return the_lock().try_lock_upgrade();}
    bool try_lock_upgrade_until(system_time const&t)   
    {return the_lock().try_lock_upgrade_until(t);}
    template<typename TimeDuration>   
    bool try_lock_upgrade_for(TimeDuration const&t)   
    {return the_lock().try_lock_upgrade_for(t);}
    void lock_upgrade_until(system_time const&t)
    {the_lock().lock_upgrade_until(t);}
    template<typename TimeDuration>   
    void lock_upgrade_for(TimeDuration const&t)
    {the_lock().lock_upgrade_for(t);}

    template<typename Clock, typename Duration>
    bool try_lock_upgrade_until(chrono::time_point<Clock, Duration> const & abs_time)
    {return the_lock().try_lock_upgrade_until(abs_time);}
    template<typename Rep, typename Period>
    bool try_lock_upgrade_for(chrono::duration<Rep, Period> const & rel_time)
    {return the_lock().try_lock_upgrade_for(rel_time);}
    
    template<typename Clock, typename Duration>
    void lock_upgrade_until(chrono::time_point<Clock, Duration> const & abs_time)
    {the_lock().lock_upgrade_until(abs_time);}
    template<typename Rep, typename Period>
    void lock_upgrade_for(chrono::duration<Rep, Period> const & rel_time)
    {the_lock().try_lock_upgrade_for(rel_time);}
    
private:
    UpgradableLock& the_lock() {return *static_cast<UpgradableLock*>(&this->lock_);}
};

}
}
}

#endif
