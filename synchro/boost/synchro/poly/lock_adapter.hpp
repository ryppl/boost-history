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
namespace boost { namespace synchro { namespace poly {

//////////////////////////////////////////////////////////////////////////////

template <typename Lockable>
class exclusive_lock_adapter
    : public virtual exclusive_lock
{
    exclusive_lock_adapter(): lock_() {}
    virtual ~exclusive_lock_adapter() {}

    virtual void lock()
    {lock_.lock();}
    virtual void unlock()
    {lock_.unlock();}
    virtual bool try_lock()
    { return lock_.try_lock();}
protected:
    Lockable lock_;
};

//////////////////////////////////////////////////////////////////////////////

template <typename SharableLock>
class sharable_lock_adapter
        : public exclusive_lock_adapter<SharableLock>
        , public virtual sharable_lock
{
public:
    virtual ~sharable_lock_adapter()
    {}
    virtual void lock_shared()
    {the_lock().lock_shared();}
    virtual bool try_lock_shared()
    {return the_lock().try_lock_shared();}
    virtual void unlock_shared()
    {the_lock().unlock_shared();}
private:
    SharableLock& the_lock() {return *static_cast<SharableLock*>(&this->lock_);}
};

//////////////////////////////////////////////////////////////////////////////

template <typename UpgradableLock>
class upgradable_lock_adapter
    : public sharable_lock_adapter<UpgradableLock>
    , public virtual upgradable_lock
{
public:
    virtual ~upgradable_lock_adapter() {}
    virtual void lock_upgrade()
    {the_lock().lock_upgrade();}

    virtual void unlock_upgrade()
    {the_lock().unlock_upgrade();}

    virtual void unlock_upgrade_and_lock()
    {the_lock().unlock_upgrade_and_lock();}
    virtual void unlock_and_lock_upgrade()
    {the_lock().unlock_and_lock_upgrade();}
    virtual void unlock_and_lock_shared()
    {the_lock().unlock_and_lock_shared();}
    virtual void unlock_upgrade_and_lock_shared()
    {the_lock().unlock_upgrade_and_lock_shared();}
private:
    UpgradableLock& the_lock() {return *static_cast<UpgradableLock*>(&this->lock_);}
};

}
}
}

#endif
