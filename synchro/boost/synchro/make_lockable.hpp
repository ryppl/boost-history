//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_MAKE_LOCKABLE__HPP
#define BOOST_SYNCHRO_MAKE_LOCKABLE__HPP

#include <boost/synchro/lockable_traits.hpp>
#include "boost/noncopyable.hpp"
#include "boost/thread/thread_time.hpp"

namespace boost { namespace synchro {

//[make_exclusive_lockable
template <typename Lockable>
class make_exclusive_lockable : private boost::noncopyable
{
public:
    typedef Lockable lockable_type;
    typedef typename scope_tag<Lockable>::type scope;
    typedef typename category_tag<Lockable>::type category;
    typedef typename reentrancy_tag<Lockable>::type reentrancy;
    typedef typename timed_interface_tag<Lockable>::type timed_interface;
    typedef typename lifetime_tag<Lockable>::type lifetime;
    typedef typename naming_tag<Lockable>::type naming;


    void lock() {lock_.lock();}
    void unlock() {lock_.unlock();}
    bool try_lock() { return lock_.try_lock();}
    lockable_type* mutex() const { return &lock_; }

protected:
    mutable Lockable lock_;
};
//]

//[make_timed_lockable
template <typename TimedLock>
class make_timed_lockable
    : public make_exclusive_lockable<TimedLock>
{
public:
    typedef TimedLock lockable_base_type;

    bool timed_lock(system_time const & abs_time)
    {return the_lock().timed_lock(abs_time);}
    template<typename TimeDuration>
    bool timed_lock(TimeDuration const & relative_time)
    {return the_lock().timed_lock(relative_time);}
protected:
    TimedLock& the_lock() {return *static_cast<TimedLock*>(&this->lock_);}
};
//]

//[make_share_lockable
template <typename SharableLock>
class make_share_lockable
    : public make_timed_lockable<SharableLock>
{
public:
    typedef SharableLock lockable_base_type;

    void lock_shared()
    {the_lock().lock_shared();}
    bool try_lock_shared()
    {return the_lock().try_lock_shared();}
    void unlock_shared()
    {the_lock().unlock_shared();}
    bool timed_lock_shared(system_time const& t)
    {return the_lock().timed_lock_shared(t);}

protected:
    SharableLock& the_lock() {return *static_cast<SharableLock*>(&this->lock_);}
};
//]

//[make_upgrade_lockable
template <typename UpgradableLock>
class make_upgrade_lockable
    : public make_share_lockable<UpgradableLock>
{
public:
    typedef UpgradableLock lockable_base_type;

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
    bool timed_lock_upgrade(system_time const&t)
    {return the_lock().timed_lock_upgrade(t);}

protected:
    UpgradableLock& the_lock() {return *static_cast<UpgradableLock*>(&this->lock_);}
};
//]


//[make_lockable
template <
      typename Lockable
      , typename category=typename category_tag<Lockable>::type
      , typename timed_interface=typename timed_interface_tag<Lockable>::type
> struct make_lockable;

template <typename Lockable>
struct make_lockable<Lockable, exclusive_lock_tag, hasnt_timed_interface_tag>
    : protected make_exclusive_lockable<Lockable>
{};

template <typename Lockable>
struct make_lockable<Lockable, exclusive_lock_tag, has_timed_interface_tag>
    : protected make_timed_lockable<Lockable>
{};

template <typename Lockable, typename timed_interface>
struct make_lockable<Lockable, sharable_lock_tag, timed_interface>
    : protected make_share_lockable<Lockable>
{};

template <typename Lockable, typename timed_interface>
struct make_lockable<Lockable, upgradable_lock_tag, timed_interface>
    : protected make_upgrade_lockable<Lockable>
{};

//]

}
}
#endif
