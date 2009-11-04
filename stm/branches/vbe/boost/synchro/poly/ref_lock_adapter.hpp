//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_POLY_REF_LOCK_ADAPTER__HPP
#define BOOST_SYNCHRO_POLY_REF_LOCK_ADAPTER__HPP

#include <boost/synchro/poly/lock.hpp>
#include <boost/synchro/detail/deleted_functions.hpp>
#include <boost/synchro/lockable.hpp>

namespace boost { namespace synchro { namespace poly {

//////////////////////////////////////////////////////////////////////////////

template <typename Lockable, typename Base=exclusive_lock>
class exclusive_ref_lock_adapter
    : public Base
{
public:
    typedef Lockable lockable_type;
#if 0
    typedef typename scope_tag<Lockable>::type scope;
    typedef typename category_tag<Lockable>::type category;
    typedef typename reentrancy_tag<Lockable>::type reentrancy;
    typedef typename timed_interface_tag<Lockable>::type timed_interface;
    typedef typename lifetime_tag<Lockable>::type lifetime;
    typedef typename naming_tag<Lockable>::type naming;
#endif
    BOOST_COPY_CONSTRUCTOR_DELETE(exclusive_ref_lock_adapter) /*< disable copy construction >*/
    BOOST_COPY_ASSIGNEMENT_DELETE(exclusive_ref_lock_adapter) /*< disable copy asignement >*/
    exclusive_ref_lock_adapter(lockable_type& lock): lock_(lock) {}
    ~exclusive_ref_lock_adapter() {}

    void lock() {synchro::lock(lock_);}
    void unlock() {synchro::unlock(lock_);}
    bool try_lock() { return synchro::try_lock(lock_);}
protected:
    Lockable& lock_;
};

#if 0

template <typename TimeLockable, typename Base=timed_lock>
class timed_ref_lock_adapter
        : public exclusive_ref_lock_adapter<TimeLockable, Base>
{
    typedef TimeLockable lockable_type;
    typedef exclusive_ref_lock_adapter<TimeLockable, Base> base_type;
public:
    timed_ref_lock_adapter(lockable_type& lock) : base_type(lock) {}
    ~timed_ref_lock_adapter()    {}
    bool try_lock_until(chrono::system_clock::time_point  const&  abs_time)
    {return the_lock().try_lock_until(abs_time);}
    bool try_lock_for(chrono::nanoseconds const& rel_time)
    {return try_lock_for(rel_time);}
    void lock_until(chrono::system_clock::time_point const&  abs_time)
    {the_lock().lock_until(abs_time);}
    void lock_for(chrono::nanoseconds const& rel_time)
    {lock_for(rel_time);}
    
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

template <typename SharableLock, typename Base=sharable_lock>
class sharable_ref_lock_adapter
        : public timed_ref_lock_adapter<SharableLock, Base>
{
    typedef SharableLock lockable_type;
    typedef timed_ref_lock_adapter<lockable_type, Base> base_type;
public:
    sharable_ref_lock_adapter(lockable_type& lock) : base_type(lock) {}
    ~sharable_ref_lock_adapter()
    {}
    void lock_shared()
    {the_lock().lock_shared();}
    bool try_lock_shared()
    {return the_lock().try_lock_shared();}
    void unlock_shared()
    {the_lock().unlock_shared();}

    bool try_lock_shared_until(chrono::system_clock::time_point const& t)
    {return the_lock().try_lock_shared_until(t);}
    bool try_lock_shared_for(chrono::nanoseconds const& t)
    {return the_lock().try_lock_shared_for(t);}
    
    void lock_shared_until(chrono::system_clock::time_point const& t)
    {the_lock().lock_shared_until(t);}
    void lock_shared_for(chrono::nanoseconds const& t)
    {the_lock().lock_shared_for(t);}

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

template <typename UpgradableLock, typename Base=upgradable_lock>
class upgradable_ref_lock_adapter
    : public sharable_ref_lock_adapter<UpgradableLock, Base>
{
    typedef UpgradableLock lockable_type;
    typedef sharable_ref_lock_adapter<lockable_type, Base> base_type;
public:
    sharable_ref_lock_adapter(lockable_type& lock) : base_type(lock) {}
    ~upgradable_ref_lock_adapter() {}
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
    bool try_lock_upgrade_until(chrono::system_clock::time_point const&t)   
    {return the_lock().try_lock_upgrade_until(t);}
    bool try_lock_upgrade_for(chrono::nanoseconds const&t)   
    {return the_lock().try_lock_upgrade_for(t);}
    void lock_upgrade_until(chrono::system_clock::time_point const&t)
    {the_lock().lock_upgrade_until(t);}
    void lock_upgrade_for(chrono::nanoseconds const&t)
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

#endif

}
}
}

#endif
