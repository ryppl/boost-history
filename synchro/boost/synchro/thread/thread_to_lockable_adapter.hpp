//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_THREAD_LOCKABLE_ADAPTER__HPP
#define BOOST_SYNCHRO_THREAD_LOCKABLE_ADAPTER__HPP

#include <boost/synchro/lockable_traits.hpp>
#include <boost/synchro/timeout_exception.hpp>
#include <boost/synchro/detail/deleted_functions.hpp>
#include <boost/thread/detail/move.hpp>
#include <boost/thread/thread_time.hpp>
#include <boost/chrono/chrono.hpp>
#include <boost/synchro/lockable/functions.hpp>
#include <boost/synchro/is_lockable.hpp>
#include <boost/conversion/boost/chrono_time_point_to_posix_time_ptime.hpp>
#include <boost/conversion/boost/chrono_duration_to_posix_time_duration.hpp>

namespace boost { namespace synchro {

//[exclusive_lockable_adapter
template <typename Lockable>
class thread_to_exclusive_lockable_adapter
{
public:
    typedef Lockable lockable_type;
    typedef typename scope_tag<Lockable>::type scope;
    typedef typename category_tag<Lockable>::type category;
    typedef typename reentrancy_tag<Lockable>::type reentrancy;
    typedef typename timed_interface_tag<Lockable>::type timed_interface;
    typedef typename lifetime_tag<Lockable>::type lifetime;
    typedef typename naming_tag<Lockable>::type naming;


//    thread_to_exclusive_lockable_adapter():lock_(&l) {}
    thread_to_exclusive_lockable_adapter(Lockable&l):lock_(&l) {}
    thread_to_exclusive_lockable_adapter(detail::thread_move_t< thread_to_exclusive_lockable_adapter> &other) 
        {lock_=other.lock_;other.lock_=0;}
    void lock() {lockable::lock(the_lock());}
    void unlock() {lockable::unlock(the_lock());}
    bool try_lock() { return lockable::try_lock(the_lock());}

    lockable_type* mutex() const { return &lock_; }
protected:
    mutable Lockable* lock_;
    lockable_type& the_lock() {return *this->lock_;}
};
//]

//[thread_to_timed_lockable_adapter
template <typename TimedLock>
class thread_to_timed_lockable_adapter
    : public thread_to_exclusive_lockable_adapter<TimedLock>
{
    typedef TimedLock Lockable;
public:
    //typedef TimedLock lockable_base_type;

    typedef Lockable lockable_type;
    typedef typename scope_tag<Lockable>::type scope;
    typedef typename category_tag<Lockable>::type category;
    typedef typename reentrancy_tag<Lockable>::type reentrancy;
    typedef typename timed_interface_tag<Lockable>::type timed_interface;
    typedef typename lifetime_tag<Lockable>::type lifetime;
    typedef typename naming_tag<Lockable>::type naming;

    thread_to_timed_lockable_adapter(TimedLock&l):thread_to_exclusive_lockable_adapter<TimedLock>(l) {}

    bool timed_lock(system_time const & abs_time)
    {return lockable::try_lock_until(the_lock(), convert_to<chrono::system_clock::time_point>(abs_time));}
    template<typename TimeDuration>
    bool timed_lock(TimeDuration const & relative_time)
    {return lockable::try_lock_for(the_lock(), convert_to<chrono::system_clock::duration>(relative_time));}

    template<typename Clock, typename Duration>
    bool try_lock_until(chrono::time_point<Clock, Duration> const & abs_time)
    {return lockable::try_lock_until(the_lock(), abs_time);}
    template<typename Rep, typename Period>
    bool try_lock_for(chrono::duration<Rep, Period> const & rel_time)
    {return lockable::try_lock_for(the_lock(), rel_time);}
    
    void lock_until(system_time const & abs_time)
    {lockable::lock_until(the_lock(), abs_time);}
    template<typename TimeDuration>
    void lock_for(TimeDuration const & relative_time)
    {lockable::lock_for(the_lock(), relative_time);}

    template<typename Clock, typename Duration>
    void lock_until(chrono::time_point<Clock, Duration> const & abs_time)
    {lockable::lock_until(the_lock(), abs_time);}

    template<typename Rep, typename Period>
    void lock_for(chrono::duration<Rep, Period> const & rel_time)
    {lockable::lock_for(the_lock(), rel_time);}
    
protected:
//    TimedLock& the_lock() {return *static_cast<TimedLock*>(&this->lock_);}
    TimedLock& the_lock() {return *this->lock_;}
};
//]

//[thread_to_shared_lockable_adapter
template <typename SharableLock>
class thread_to_shared_lockable_adapter
    : public thread_to_timed_lockable_adapter<SharableLock>
{
    typedef SharableLock Lockable;
public:
    //typedef SharableLock lockable_base_type;
    typedef Lockable lockable_type;
    typedef typename scope_tag<Lockable>::type scope;
    typedef typename category_tag<Lockable>::type category;
    typedef typename reentrancy_tag<Lockable>::type reentrancy;
    typedef typename timed_interface_tag<Lockable>::type timed_interface;
    typedef typename lifetime_tag<Lockable>::type lifetime;
    typedef typename naming_tag<Lockable>::type naming;

    thread_to_shared_lockable_adapter(SharableLock&l):thread_to_timed_lockable_adapter<SharableLock>(l) {}

    void lock_shared()
    {lockable::lock_shared(the_lock());}
    bool try_lock_shared()
    {return lockable::try_lock_shared(the_lock());}
    void unlock_shared()
    {lockable::unlock_shared(the_lock());}

    bool try_lock_shared_until(system_time const& t)
    {return lockable::try_lock_shared_until(the_lock(), t);}
    template<typename TimeDuration>   
    bool try_lock_shared_for(TimeDuration const& t)
    {return lockable::try_lock_shared_for(the_lock(), t);}

    bool timed_lock_shared(system_time const& t)
    {return lockable::try_lock_shared_until(the_lock(), t);}
    template<typename TimeDuration>   
    bool timed_lock_shared(TimeDuration const& t)
    {return lockable::try_lock_shared_for(the_lock(), t);}

    template<typename Clock, typename Duration>
    bool try_lock_shared_until(chrono::time_point<Clock, Duration> const & abs_time)
    {return lockable::try_lock_shared_until(the_lock(), abs_time);}
    template<typename Rep, typename Period>
    bool try_lock_shared_for(chrono::duration<Rep, Period> const & rel_time)
    {return lockable::try_lock_shared_for(the_lock(), rel_time);}
    
    template<typename TimeDuration>   
    void lock_shared_for(TimeDuration const& t)
    {lockable::lock_shared_for(the_lock(), t);}
    void lock_shared_until(system_time const& t)
    {lockable::lock_shared_until(the_lock(), t);}
    
    template<typename Clock, typename Duration>
    void lock_shared_until(chrono::time_point<Clock, Duration> const & abs_time)
    {lockable::lock_shared_until(the_lock(), abs_time);}
    template<typename Rep, typename Period>
    void lock_shared_for(chrono::duration<Rep, Period> const & rel_time)
    {lockable::lock_shared_for(the_lock(), rel_time);}

protected:
    SharableLock& the_lock() {return *this->lock_;}
};

//]

//[thread_to_upgrade_lockable_adapter
template <typename UpgradableLock>
class thread_to_upgrade_lockable_adapter
    : public thread_to_shared_lockable_adapter<UpgradableLock>
{
    typedef UpgradableLock Lockable;
public:
    //typedef UpgradableLock lockable_base_type;
    typedef Lockable lockable_type;
    typedef typename scope_tag<Lockable>::type scope;
    typedef typename category_tag<Lockable>::type category;
    typedef typename reentrancy_tag<Lockable>::type reentrancy;
    typedef typename timed_interface_tag<Lockable>::type timed_interface;
    typedef typename lifetime_tag<Lockable>::type lifetime;
    typedef typename naming_tag<Lockable>::type naming;

    thread_to_upgrade_lockable_adapter(UpgradableLock&l):thread_to_shared_lockable_adapter<UpgradableLock>(l) {}

    void lock_upgrade()
    {lockable::lock_upgrade(the_lock());}

    bool try_lock_upgrade()
    {  return lockable::try_lock_upgrade(the_lock()); }

    void unlock_upgrade()
    {lockable::unlock_upgrade(the_lock());}

    void unlock_upgrade_and_lock()
    {lockable::unlock_upgrade_and_lock(the_lock());}
    void unlock_and_lock_upgrade()
    {lockable::unlock_and_lock_upgrade(the_lock());}
    void unlock_and_lock_shared()
    {lockable::unlock_and_lock_shared(the_lock());}
    void unlock_upgrade_and_lock_shared()
    {lockable::unlock_upgrade_and_lock_shared(the_lock());}
    bool try_lock_upgrade_until(system_time const&t)   
    {return lockable::try_lock_upgrade_until(the_lock(), t);}
    template<typename TimeDuration>   
    bool try_lock_upgrade_for(TimeDuration const&t)   
    {return lockable::try_lock_upgrade_for(the_lock(), t);}
    
    template<typename Clock, typename Duration>
    bool try_lock_upgrade_until(chrono::time_point<Clock, Duration> const & abs_time)
    {return lockable::try_lock_upgrade_until(the_lock(), abs_time);}

    template<typename Rep, typename Period>
    bool try_lock_upgrade_for(chrono::duration<Rep, Period> const & rel_time)
    {return lockable::try_lock_upgrade_for(the_lock(), rel_time);}
    
    void lock_upgrade_until(system_time const&t)
    {lockable::lock_upgrade_until(the_lock(), t);}
    template<typename TimeDuration>   
    void lock_upgrade_for(TimeDuration const&t)
    {lockable::lock_upgrade_for(the_lock(), t);}

    template<typename Clock, typename Duration>
    void lock_upgrade_until(chrono::time_point<Clock, Duration> const & abs_time)
    {lockable::lock_upgrade_until(the_lock(), abs_time);}

    template<typename Rep, typename Period>
    void lock_upgrade_for(chrono::duration<Rep, Period> const & rel_time)
    {lockable::try_lock_upgrade_for(the_lock(), rel_time);}
protected:
    UpgradableLock& the_lock() {return *this->lock_;}
};
//]


//[thread_to_lockable_adapter
template <
        typename Lockable
      , typename category=typename category_tag<Lockable>::type
      , typename timed_interface=typename timed_interface_tag<Lockable>::type
> struct thread_to_lockable_adapter;

template <typename Lockable>
struct thread_to_lockable_adapter<Lockable, exclusive_lock_tag, hasnt_timed_interface_tag>
    : public thread_to_exclusive_lockable_adapter<Lockable>
{
    typedef Lockable lockable_type;
    typedef typename scope_tag<Lockable>::type scope;
    typedef typename category_tag<Lockable>::type category;
    typedef typename reentrancy_tag<Lockable>::type reentrancy;
    typedef typename timed_interface_tag<Lockable>::type timed_interface;
    typedef typename lifetime_tag<Lockable>::type lifetime;
    typedef typename naming_tag<Lockable>::type naming;
    
    thread_to_lockable_adapter(Lockable&l):thread_to_exclusive_lockable_adapter<Lockable>(l) {}
};

template <typename Lockable>
struct thread_to_lockable_adapter<Lockable, exclusive_lock_tag, has_timed_interface_tag>
    : public thread_to_timed_lockable_adapter<Lockable>
{
    thread_to_lockable_adapter(Lockable&l):thread_to_timed_lockable_adapter<Lockable>(l) {}
};

template <typename Lockable, typename timed_interface>
struct thread_to_lockable_adapter<Lockable, sharable_lock_tag, timed_interface>
    : public thread_to_shared_lockable_adapter<Lockable>
{};

template <typename Lockable, typename timed_interface>
struct thread_to_lockable_adapter<Lockable, upgradable_lock_tag, timed_interface>
    : public thread_to_upgrade_lockable_adapter<Lockable>
{
    thread_to_lockable_adapter(Lockable&l):thread_to_upgrade_lockable_adapter<Lockable>(l) {}
};

template <
        typename Lockable
      , typename category
      , typename timed_interface>
    struct is_lockable<thread_to_lockable_adapter<Lockable, category, timed_interface> >
    {
        BOOST_STATIC_CONSTANT(bool, value = true);
    };

//]

}
}
#endif
