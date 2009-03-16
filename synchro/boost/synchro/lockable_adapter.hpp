//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_LOCKABLE_ADAPTER__HPP
#define BOOST_SYNCHRO_LOCKABLE_ADAPTER__HPP

#include <boost/synchro/lockable_traits.hpp>
#include <boost/synchro/timeout_exception.hpp>
#include <boost/synchro/detail/deleted_functions.hpp>
#include <boost/thread/thread_time.hpp>
#include <boost/chrono/chrono.hpp>

namespace boost { namespace synchro {

//[exclusive_lockable_adapter
template <typename Lockable>
class exclusive_lockable_adapter
{
public:
    typedef Lockable lockable_type;
    typedef typename scope_tag<Lockable>::type scope;
    typedef typename category_tag<Lockable>::type category;
    typedef typename reentrancy_tag<Lockable>::type reentrancy;
    typedef typename timed_interface_tag<Lockable>::type timed_interface;
    typedef typename lifetime_tag<Lockable>::type lifetime;
    typedef typename naming_tag<Lockable>::type naming;

    BOOST_COPY_CONSTRUCTOR_DELETE(exclusive_lockable_adapter) /*< disable copy construction >*/
    BOOST_COPY_ASSIGNEMENT_DELETE(exclusive_lockable_adapter) /*< disable copy asignement >*/   
    exclusive_lockable_adapter() {}
    void lock() {lock_.lock();}
    void unlock() {lock_.unlock();}
    bool try_lock() { return lock_.try_lock();}

protected:
    lockable_type* mutex() const { return &lock_; }
    mutable Lockable lock_;
};
//]

//[timed_lockable_adapter
template <typename TimedLock>
class timed_lockable_adapter
    : public exclusive_lockable_adapter<TimedLock>
{
public:
    typedef TimedLock lockable_base_type;

    timed_lockable_adapter() {}

    bool try_lock_until(system_time const & abs_time)
    {return the_lock().try_lock_until(abs_time);}
    template<typename TimeDuration>
    bool try_lock_for(TimeDuration const & relative_time)
    {return the_lock().try_lock_for(relative_time);}

    template<typename Clock, typename Duration>
    bool try_lock_until(chrono::time_point<Clock, Duration> const & abs_time)
    {return the_lock().try_lock_until(abs_time);}
    template<typename Rep, typename Period>
    bool try_lock_for(chrono::duration<Rep, Period> const & rel_time)
    {return the_lock().try_lock_for(rel_time);}
    
    void lock_until(system_time const & abs_time)
    {the_lock().lock_until(abs_time);}
    template<typename TimeDuration>
    void lock_for(TimeDuration const & relative_time)
    {the_lock().lock_for(relative_time);}

    template<typename Clock, typename Duration>
    void lock_until(chrono::time_point<Clock, Duration> const & abs_time)
    {the_lock().lock_until(abs_time);}

    template<typename Rep, typename Period>
    void lock_for(chrono::duration<Rep, Period> const & rel_time)
    {the_lock().lock_for(rel_time);}
    
protected:
    TimedLock& the_lock() {return *static_cast<TimedLock*>(&this->lock_);}
};
//]

//[shared_lockable_adapter
template <typename SharableLock>
class shared_lockable_adapter
    : public timed_lockable_adapter<SharableLock>
{
public:
    typedef SharableLock lockable_base_type;

    shared_lockable_adapter() {}

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

    template<typename Clock, typename Duration>
    bool try_lock_shared_until(chrono::time_point<Clock, Duration> const & abs_time)
    {return the_lock().try_lock_shared_until(abs_time);}
    template<typename Rep, typename Period>
    bool try_lock_shared_for(chrono::duration<Rep, Period> const & rel_time)
    {return the_lock().try_lock_shared_for(rel_time);}
    
    template<typename TimeDuration>   
    void lock_shared_for(TimeDuration const& t)
    {the_lock().lock_shared_for(t);}
    void lock_shared_until(system_time const& t)
    {the_lock().lock_shared_until(t);}
    
    template<typename Clock, typename Duration>
    void lock_shared_until(chrono::time_point<Clock, Duration> const & abs_time)
    {the_lock().lock_shared_until(abs_time);}
    template<typename Rep, typename Period>
    void lock_shared_for(chrono::duration<Rep, Period> const & rel_time)
    {the_lock().lock_shared_for(rel_time);}

protected:
    SharableLock& the_lock() {return *static_cast<SharableLock*>(&this->lock_);}
};

//]

//[upgrade_lockable_adapter
template <typename UpgradableLock>
class upgrade_lockable_adapter
    : public shared_lockable_adapter<UpgradableLock>
{
public:
    typedef UpgradableLock lockable_base_type;

    upgrade_lockable_adapter() {}

    void lock_upgrade()
    {the_lock().lock_upgrade();}

    bool try_lock_upgrade()
    {  return the_lock().try_lock_upgrade(); }

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
    bool try_lock_upgrade_until(system_time const&t)   
    {return the_lock().try_lock_upgrade_until(t);}
    template<typename TimeDuration>   
    bool try_lock_upgrade_for(TimeDuration const&t)   
    {return the_lock().try_lock_upgrade_for(t);}
    
    template<typename Clock, typename Duration>
    bool try_lock_upgrade_until(chrono::time_point<Clock, Duration> const & abs_time)
    {return the_lock().try_lock_upgrade_until(abs_time);}

    template<typename Rep, typename Period>
    bool try_lock_upgrade_for(chrono::duration<Rep, Period> const & rel_time)
    {return the_lock().try_lock_upgrade_for(rel_time);}
    
    void lock_upgrade_until(system_time const&t)
    {the_lock().lock_upgrade_until(t);}
    template<typename TimeDuration>   
    void lock_upgrade_for(TimeDuration const&t)
    {the_lock().lock_upgrade_for(t);}

    template<typename Clock, typename Duration>
    void lock_upgrade_until(chrono::time_point<Clock, Duration> const & abs_time)
    {the_lock().lock_upgrade_until(abs_time);}

    template<typename Rep, typename Period>
    void lock_upgrade_for(chrono::duration<Rep, Period> const & rel_time)
    {the_lock().try_lock_upgrade_for(rel_time);}
protected:
    UpgradableLock& the_lock() {return *static_cast<UpgradableLock*>(&this->lock_);}
};
//]


//[lockable_adapter
template <
        typename Lockable
      , typename category=typename category_tag<Lockable>::type
      , typename timed_interface=typename timed_interface_tag<Lockable>::type
> struct lockable_adapter;

template <typename Lockable>
struct lockable_adapter<Lockable, exclusive_lock_tag, hasnt_timed_interface_tag>
    : protected exclusive_lockable_adapter<Lockable>
{};

template <typename Lockable>
struct lockable_adapter<Lockable, exclusive_lock_tag, has_timed_interface_tag>
    : protected timed_lockable_adapter<Lockable>
{};

template <typename Lockable, typename timed_interface>
struct lockable_adapter<Lockable, sharable_lock_tag, timed_interface>
    : protected shared_lockable_adapter<Lockable>
{};

template <typename Lockable, typename timed_interface>
struct lockable_adapter<Lockable, upgradable_lock_tag, timed_interface>
    : protected upgrade_lockable_adapter<Lockable>
{};

//]

}
}
#endif
