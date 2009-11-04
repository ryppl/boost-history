//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_POLY_ADAPTIVE_LOCK__HPP
#define BOOST_SYNCHRO_POLY_ADAPTIVE_LOCK__HPP

#include <boost/synchro/poly/lock.hpp>

namespace boost { namespace synchro { namespace poly {

class adaptive_exclusive_lock
    : public virtual exclusive_lock
{
public:
    virtual ~adaptive_exclusive_lock() {}
    virtual void lock()
    {lock_->lock();}
    virtual void unlock()
    {lock_->unlock();}
    virtual bool try_lock()
    { return lock_->try_lock();}
protected:
    adaptive_exclusive_lock(exclusive_lock* lock) : lock_(lock)
    {
        // assert lock is not null
    }
    exclusive_lock* lock_;
};
#if 0

class adaptive_sharable_lock
    : public adaptive_exclusive_lock
    , public virtual sharable_lock
{
public:
    virtual ~adaptive_sharable_lock() {}
    virtual void lock_shared()
    {the_lock()->lock_shared();}
    virtual bool try_lock_shared()
    { return the_lock()->try_lock_shared();}
    virtual void unlock_shared()
    {the_lock()->unlock_shared();}
protected:
    adaptive_sharable_lock(sharable_lock* lock) : adaptive_exclusive_lock(lock)
    {}

private:
    sharable_lock* the_lock() {return static_cast<sharable_lock*>(this->lock_);}
};

class adaptive_upgradable_lock
    : public adaptive_sharable_lock
    , public virtual upgradable_lock
{
public:
    virtual ~adaptive_upgradable_lock() {}
    virtual void lock_upgrade()
    {the_lock()->lock_upgrade();}
    virtual void unlock_upgrade()
    {the_lock()->unlock_upgrade();}

    virtual void unlock_upgrade_and_lock()
    {the_lock()->unlock_upgrade_and_lock();}
    virtual void unlock_and_lock_upgrade()
    {the_lock()->unlock_and_lock_upgrade();}
    virtual void unlock_and_lock_shared()
    {the_lock()->unlock_and_lock_shared();}
    virtual void unlock_upgrade_and_lock_shared()
    {the_lock()->unlock_upgrade_and_lock_shared();}
protected:
    adaptive_upgradable_lock(upgradable_lock* lock)
    : adaptive_sharable_lock(lock) {}
private:
    upgradable_lock* the_lock() {return static_cast<upgradable_lock*>(this->lock_);}
};
#endif
}
}
}
#endif
