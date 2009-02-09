//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_CONDITION_LOCKER__HPP
#define BOOST_SYNCHRO_CONDITION_LOCKER__HPP

#include <boost/synchro/lockable_concepts.hpp>
#include <boost/thread/condition.hpp>
#include "boost/synchro/thread/mutex.hpp"

namespace boost { namespace synchro {



template <
    class Condition
>
class condition_backdoor;

template <
    class Condition
>
class condition_safe_boosted {
public:
	typedef Condition condition;
	typedef condition_backdoor<Condition> backdoor;
    friend class condition_backdoor<Condition>;
private:
    template <typename Locker>
    void notify_one(Locker& lock) {
    	++num_notifies_requested;
    	if (num_blocked > 0) {
    		cond_.notify_one();
        	++num_notifies_done;
    	}
    }
    template <typename Locker>
    void notify_all(Locker& lock) {
    	++num_notifies_requested;
    	if (num_blocked > 0) {
    		cond_.notify_all();
        	++num_notifies_done;
    	}
    }
    template <typename Locker, typename Predicate>
    void wait_when(Locker& lock, Predicate pred) {
        while(!pred()) wait(lock);
    }
    template <typename Locker>
    void wait(Locker& lock) {
    	++num_waits;
    	++num_blocked;
    	cond_.wait(lock);
    	--num_blocked;
    }

    template <typename Locker>
    bool wait_until(Locker& lock, boost::system_time  const&  abs_time) {
    	++num_waits;
    	++num_blocked;
    	bool b = cond_.timed_wait(lock);
    	--num_blocked;
    	return b;
    }

    template<typename Locker, typename duration_type>
    bool wait_for(Locker& lock,duration_type const& rel_time) {
    	++num_waits;
    	++num_blocked;
    	bool b =cond_.timed_wait(lock);
    	--num_blocked;
    	return b;
    }

    template<typename Locker, typename predicate_type>
    bool wait_when_until(Locker& lock, predicate_type pred, boost::system_time const& abs_time) {
        while (!pred())
        {
            if(!wait_until(lock, abs_time))
                return pred();
        }
        return true;
    }
    template<typename Locker, typename duration_type, typename predicate_type>
    bool wait_when_for(Locker& lock, predicate_type pred, duration_type const& rel_time) {
        while (!pred())
        {
            if(!wait_for(lock, rel_time))
                return pred();
        }
        return true;
    }
private:
    Condition cond_;
    unsigned num_blocked;
    unsigned num_waits;
    unsigned num_notifies_requested;
    unsigned num_notifies_done;
};

template <
    class Condition
>
class condition_safe {
public:
	typedef Condition condition;
	typedef condition_backdoor<Condition> backdoor;
    void notify_one() { cond_.notify_one(); }
    void notify_all() { cond_.notify_all(); }
private:
    friend class condition_backdoor<Condition>;
    template <typename Locker, typename Predicate>
    void wait_when(Locker& lock, Predicate pred) {
    	cond_.wait(lock, pred);
    }
    template <typename Locker>
    void wait(Locker& lock) {
    	cond_.wait(lock);
    }

    template <typename Locker>
    bool wait_until(Locker& lock, boost::system_time  const&  abs_time) {
    	return cond_.timed_wait(lock);;
    }

    template<typename Locker, typename duration_type>
    bool wait_for(Locker& lock,duration_type const& rel_time) {
    	return cond_.timed_wait(lock);
    }

//    template<typename Locker, typename predicate_type>
//    bool wait_when(Locker& lock, predicate_type pred) {
//    	return cond_.wait(lock, pred);
//    }
    template<typename Locker, typename predicate_type>
    bool wait_when_until(Locker& lock, predicate_type pred, boost::system_time const& abs_time) {
    	return cond_.timed_wait(lock, pred, abs_time);
    }
    template<typename Locker, typename duration_type, typename predicate_type>
    bool wait_when_for(Locker& lock, predicate_type pred, duration_type const& rel_time) {
    	return cond_.timed_wait(lock, pred, rel_time);
    }
private:
    Condition cond_;
};

template <
    class Condition
>
struct condition_backdoor {
	condition_backdoor(condition_safe<Condition>&cnd): that_(cnd) {}
    template <typename Locker, typename Predicate>
    void wait_when(Locker& lock, Predicate pred){
    	that_.wait_when(lock, pred);
    }
    template <typename Locker>
    void wait(Locker& lock) {
    	that_.wait(lock);
    }
    template <typename Locker>
    bool wait_until(Locker& lock, boost::system_time  const&  abs_time) {
    	return that_.wait_until(lock, abs_time);
    }

    template<typename Locker, typename duration_type>
    bool wait_for(Locker& lock, duration_type const& rel_time) {
    	return that_.wait_for(lock, rel_time);
    }

    template<typename Locker, typename predicate_type>
    bool wait_when_until(Locker& lock, predicate_type pred, boost::system_time const& abs_time) {
    	return that_.timed_wait(lock, pred, abs_time);
    }
    template <typename Locker>
    void notify_one(Locker& lock) {
    	that_.notify_one(lock);
    }
    template <typename Locker>
    void notify_all(Locker& lock) {
    	that_.notify_all(lock);
    }
private:
	condition_safe<Condition>& that_;
};

#if 0
//[condition_unique_locker
template <
	typename Lockable,
    class Condition=condition_safe<typename best_condition<Lockable>::type >
>
class condition_unique_locker
	: protected unique_lock<Lockable>
{
    BOOST_CONCEPT_ASSERT((LockableConcept<Lockable>));
    typedef unique_lock<Lockable> super_type;
public:
    typedef Lockable lockable_type;
    typedef Condition condition;

    explicit condition_unique_locker(lockable_type& obj); /*< locks on construction >*/
    condition_unique_locker(lockable_type& obj, condition &cond); /*< relock on condition >*/
    template <typename Predicate>
    condition_unique_locker(lockable_type& obj, condition &cond, Predicate pred); /*< relock condition when predicate satisfaied>*/
    ~condition_unique_locker() /*< unlocks on destruction >*/

    typedef bool (condition_unique_locker::*bool_type)() const; /*< safe bool idiom >*/
    operator bool_type() const; /*< always owned >*/
    bool operator!() const { return false; } /*< always owned >*/
    bool owns_lock() const { return true; } /*< always owned >*/
    bool is_locking(lockable_type* l) const /*< strict lockers specific function >*/

    void relock_on(condition & cond);
    void relock_until(condition & cond, boost::system_time const& abs_time);
    template<typename duration_type>
    void relock_on_for(condition & cond, duration_type const& rel_time);

    template<typename Predicate>
    void relock_when(condition &cond, Predicate pred);
    template<typename Predicate>
    void relock_when_until(condition &cond, Predicate pred,
    		boost::system_time const& abs_time);
    template<typename Predicate, typename duration_type>
    void relock_when_for(condition &cond, Predicate pred,
    		duration_type const& rel_time);

    void notify_one(condition_boosted &cond);
    void notify_all(condition_boosted &cond);

    /*< no possibility to unlock without blocking on wait... >*/

};
//]
#endif

template <
	typename Lockable,
    class Condition=condition_safe<typename best_condition<Lockable>::type >,
	class ConditionBoosted=condition_safe_boosted<typename best_condition<Lockable>::type >
>
class condition_unique_locker
	: protected unique_lock<Lockable>
{
    BOOST_CONCEPT_ASSERT((LockableConcept<Lockable>));
    typedef unique_lock<Lockable> super_type;
public:
    typedef Lockable lockable_type;
    typedef Condition condition;
    typedef ConditionBoosted condition_boosted;

    explicit condition_unique_locker(lockable_type& obj)
        : super_type(obj) { } /*< locks on construction >*/

    condition_unique_locker(lockable_type& obj, condition &cond)
        : super_type(obj) {
    		typename condition::backdoor(cond).wait(*static_cast<unique_lock<Lockable>*>(this)); /*< relock on condition >*/
        }
    condition_unique_locker(lockable_type& obj, condition_boosted &cond)
        : super_type(obj) {
    		typename condition::backdoor(cond).wait(*static_cast<unique_lock<Lockable>*>(this)); /*< relock on condition >*/
        }

    template <typename Predicate>
    condition_unique_locker(lockable_type& obj, condition &cond, Predicate pred)
        : super_type(obj) {
    		typename condition::backdoor(cond).wait_when(*static_cast<unique_lock<Lockable>*>(this), pred); /*< relock condition when predicate satisfaied>*/
        }
    template <typename Predicate>
    condition_unique_locker(lockable_type& obj, condition_boosted &cond, Predicate pred)
        : super_type(obj) {
    		typename condition::backdoor(cond).wait_when(*static_cast<unique_lock<Lockable>*>(this), pred); /*< relock condition when predicate satisfaied>*/
        }

    ~condition_unique_locker() { } /*< unlocks on destruction >*/

    typedef bool (condition_unique_locker::*bool_type)() const; /*< safe bool idiom >*/
    operator bool_type() const { return &condition_unique_locker::owns_lock;  }
    bool operator!() const { return false; } /*< always owned >*/
    bool owns_lock() const { return true; }
    bool is_locking(lockable_type* l) const { return l==mutex(); } /*< strict lockers specific function >*/

    void relock_on(condition & cond) {
    	typename condition::backdoor(cond).wait(*this);
    }
    void relock_on(condition_boosted & cond) {
    	typename condition::backdoor(cond).wait(*this);
    }

    void relock_until(condition & cond, boost::system_time const& abs_time) {
       	typename condition::backdoor(cond).wait_until(*this, abs_time);
    }
    void relock_until(condition_boosted & cond, boost::system_time const& abs_time) {
       	typename condition::backdoor(cond).wait_until(*this, abs_time);
    }

    template<typename duration_type>
    void relock_on_for(condition & cond, duration_type const& rel_time) {
    	typename condition::backdoor(cond).wait_for(*this, rel_time);
    }
    template<typename duration_type>
    void relock_on_for(condition_boosted & cond, duration_type const& rel_time) {
    	typename condition::backdoor(cond).wait_for(*this, rel_time);
    }

    template<typename Predicate>
    void relock_when(condition &cond, Predicate pred){
    	typename condition::backdoor(cond).wait_when(*this, pred);
    }
    template<typename Predicate>
    void relock_when(condition_boosted &cond, Predicate pred){
    	typename condition::backdoor(cond).wait_when(*this, pred);
    }

    template<typename Predicate>
    void relock_when_until(condition &cond, Predicate pred,
    		boost::system_time const& abs_time){
    	typename condition::backdoor(cond).wait_when_until(*this, pred, abs_time);
    }
    template<typename Predicate>
    void relock_when_until(condition_boosted &cond, Predicate pred,
    		boost::system_time const& abs_time){
    	typename condition::backdoor(cond).wait_when_until(*this, pred, abs_time);
    }

    template<typename Predicate, typename duration_type>
    void relock_when_for(condition &cond, Predicate pred,
    		duration_type const& rel_time){
    	typename condition::backdoor(cond).wait_when_for(*this, pred, rel_time);
    }
    template<typename Predicate, typename duration_type>
    void relock_when_for(condition_boosted &cond, Predicate pred,
    		duration_type const& rel_time){
    	typename condition::backdoor(cond).wait_when_for(*this, pred, rel_time);
    }

    void notify_one(condition_boosted &cond){
    	typename condition::backdoor(cond).notify_one(*this);
    }
    void notify_all(condition_boosted &cond){
    	typename condition::backdoor(cond).notify_all(*this);
    }

    /*< no possibility to unlock without blocking on wait... >*/

private:
    friend class boost::condition_variable;
    Lockable* mutex() { return this->super_type::mutex(); }
    friend class boost::condition_variable_any;
    void lock() {this->super_type::lock();}
    void unlock() {this->super_type::unlock();}
    bool try_lock() { return this->super_type::try_lock();}

};

//[condition_shared_locker
template <
	typename Lockable,
    class Condition=condition_safe<typename best_condition_any<Lockable>::type >,
	class ConditionBoosted=condition_safe_boosted<typename best_condition_any<Lockable>::type >
>
class condition_shared_locker
	: protected shared_lock<Lockable> {
    BOOST_CONCEPT_ASSERT((LockableConcept<Lockable>));
    typedef shared_lock<Lockable> super_type;
public:
    typedef Lockable lockable_type;
    typedef Condition condition;
    typedef ConditionBoosted condition_boosted;

    explicit condition_shared_locker(lockable_type& obj)
        : super_type(obj) { } /*< locks on construction >*/
    condition_shared_locker(lockable_type& obj, condition &cond)
        : super_type(obj) {
    		typename condition::backdoor(cond).wait(*this); /*< relock on condition >*/
        }
    condition_shared_locker(lockable_type& obj, condition_boosted &cond)
        : super_type(obj) {
    		typename condition::backdoor(cond).wait(*this); /*< relock on condition >*/
        }
    template <typename Predicate>
    condition_shared_locker(lockable_type& obj, condition &cond, Predicate pred)
        : super_type(obj) {
    		typename condition::backdoor(cond).wait(*this, pred); /*< relock condition when predicate satisfaied>*/
        }
    template <typename Predicate>
    condition_shared_locker(lockable_type& obj, condition_boosted &cond, Predicate pred)
        : super_type(obj) {
    		typename condition::backdoor(cond).wait(*this, pred); /*< relock condition when predicate satisfaied>*/
        }
    ~condition_shared_locker() { } /*< unlocks on destruction >*/

    typedef bool (condition_shared_locker::*bool_type)() const; /*< safe bool idiom >*/
    operator bool_type() const { return &condition_shared_locker::owns_lock;  }
    bool operator!() const { return false; } /*< always owned >*/
    bool owns_lock() const { return true; }
    bool is_locking(lockable_type* l) const { return l==mutex(); } /*< strict lockers specific function >*/

    void relock_on(condition & cond) {
    	typename condition::backdoor(cond).wait(*this);
    }
    void relock_on(condition_boosted & cond) {
    	typename condition::backdoor(cond).wait(*this);
    }

    void relock_until(condition & cond, boost::system_time const& abs_time) {
       	typename condition::backdoor(cond).wait_until(*this, abs_time);
    }
    void relock_until(condition_boosted & cond, boost::system_time const& abs_time) {
       	typename condition::backdoor(cond).wait_until(*this, abs_time);
    }

    template<typename duration_type>
    void relock_on_for(condition & cond, duration_type const& rel_time) {
    	typename condition::backdoor(cond).wait_for(*this, rel_time);
    }
    template<typename duration_type>
    void relock_on_for(condition_boosted & cond, duration_type const& rel_time) {
    	typename condition::backdoor(cond).wait_for(*this, rel_time);
    }

    template<typename Predicate>
    void relock_when(condition &cond, Predicate pred){
    	typename condition::backdoor(cond).wait_when(*this, pred);
    }
    template<typename Predicate>
    void relock_when(condition_boosted &cond, Predicate pred){
    	typename condition::backdoor(cond).wait_when(*this, pred);
    }

    template<typename Predicate>
    void relock_when_until(condition &cond, Predicate pred,
    		boost::system_time const& abs_time){
    	typename condition::backdoor(cond).wait_when_until(*this, pred, abs_time);
    }
    template<typename Predicate>
    void relock_when_until(condition_boosted &cond, Predicate pred,
    		boost::system_time const& abs_time){
    	typename condition::backdoor(cond).wait_when_until(*this, pred, abs_time);
    }

    template<typename Predicate, typename duration_type>
    void relock_when_for(condition &cond, Predicate pred,
    		duration_type const& rel_time){
    	typename condition::backdoor(cond).wait_when_for(*this, pred, rel_time);
    }
    template<typename Predicate, typename duration_type>
    void relock_when_for(condition_boosted &cond, Predicate pred,
    		duration_type const& rel_time){
    	typename condition::backdoor(cond).wait_when_for(*this, pred, rel_time);
    }

    /*< no possibility to unlock without blocking on wait>*/

private:
    friend class boost::condition_variable;
    Lockable* mutex() { return this->super_type::mutex(); }
    friend class boost::condition_variable_any;
    void lock() {this->super_type::lock();}
    void unlock() {this->super_type::unlock();}
    bool try_lock() { return this->super_type::try_lock();}
};
//]



//[condition_lockable
template <
	typename Lockable,
    class Condition=condition_safe<typename best_condition<Lockable>::type >
>
class condition_lockable
	: protected Lockable
{
    BOOST_CONCEPT_ASSERT((LockableConcept<Lockable>));
    typedef Lockable super_type;
public:
    typedef Lockable lockable_type;
    typedef Condition condition;

    condition_lockable()
        : super_type() { }

    ~condition_lockable() { }

    void relock_on(condition & cond) {
    	typename condition::backdoor(cond).wait(*this);
    }

    void relock_until(condition & cond, boost::system_time const& abs_time) {
       	typename condition::backdoor(cond).wait_until(*this, abs_time);
    }

    template<typename duration_type>
    void relock_on_for(condition & cond, duration_type const& rel_time) {
    	typename condition::backdoor(cond).wait_for(*this, rel_time);
    }

    template<typename Predicate>
    void relock_when(condition &cond, Predicate pred){
    	typename condition::backdoor(cond).wait_when(*this, pred);
    }

    template<typename Predicate>
    void relock_when_until(condition &cond, Predicate pred,
    		boost::system_time const& abs_time){
    	typename condition::backdoor(cond).wait_when_until(*this, pred, abs_time);
    }

    template<typename Predicate, typename duration_type>
    void relock_when_for(condition &cond, Predicate pred,
    		duration_type const& rel_time){
    	typename condition::backdoor(cond).wait_when_for(*this, pred, rel_time);
    }

    void notify_one(condition &cond){
    	typename condition::backdoor(cond).notify_one(*this);
    }
    void notify_all(condition &cond){
    	typename condition::backdoor(cond).notify_all(*this);
    }

private:
    friend class boost::condition_variable;
    friend class boost::condition_variable_any;
};
//]

}
}
#endif
