//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
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
#include <boost/synchro/thread/mutex.hpp>
#include <boost/synchro/condition_backdoor.hpp>
#include <boost/synchro/condition_safe.hpp>

namespace boost { namespace synchro {

#if 0
//[condition_unique_locker
template <
    typename Lockable,
    class Condition=condition_safe<typename best_condition<Lockable>::type >
>
class condition_unique_locker
    : protected unique_lock_type<Lockable>::type
{
    BOOST_CONCEPT_ASSERT((LockableConcept<Lockable>));
    typedef typename unique_lock_type<Lockable>::type super_type;
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

    /*< no possibility to unlock without blocking on wait... >*/
};
//]
#endif

template <
    typename Lockable,
    class Condition=condition_safe<typename best_condition<Lockable>::type >
    //, class ConditionBoosted=condition_safe_boosted<typename best_condition<Lockable>::type >
>
class condition_unique_locker
    : protected unique_lock_type<Lockable>::type
{
    BOOST_CONCEPT_ASSERT((LockableConcept<Lockable>));
    typedef typename unique_lock_type<Lockable>::type super_type;
public:
    typedef Lockable lockable_type;
    typedef Condition condition;
    //typedef ConditionBoosted condition_boosted;

    explicit condition_unique_locker(lockable_type& obj)
        : super_type(obj) { } /*< locks on construction >*/

    condition_unique_locker(lockable_type& obj, condition &cond)
        : super_type(obj) {
            typename condition::backdoor(cond).wait(*static_cast<typename unique_lock_type<Lockable>::type*>(this)); /*< relock on condition >*/
        }
    //condition_unique_locker(lockable_type& obj, condition_boosted &cond)
    //    : super_type(obj) {
    //        typename condition::backdoor(cond).wait(*static_cast<unique_lock<Lockable>*>(this)); /*< relock on condition >*/
    //    }

    template <typename Predicate>
    condition_unique_locker(lockable_type& obj, condition &cond, Predicate pred)
        : super_type(obj) {
            typename condition::backdoor(cond).wait_when(*static_cast<typename unique_lock_type<Lockable>::type*>(this), pred); /*< relock condition when predicate satisfaied>*/
        }
    //template <typename Predicate>
    //condition_unique_locker(lockable_type& obj, condition_boosted &cond, Predicate pred)
    //    : super_type(obj) {
    //        typename condition::backdoor(cond).wait_when(*static_cast<unique_lock<Lockable>*>(this), pred); /*< relock condition when predicate satisfaied>*/
    //    }

    ~condition_unique_locker() { } /*< unlocks on destruction >*/

    typedef bool (condition_unique_locker::*bool_type)() const; /*< safe bool idiom >*/
    operator bool_type() const { return &condition_unique_locker::owns_lock;  }
    bool operator!() const { return false; } /*< always owned >*/
    bool owns_lock() const { return true; }
    bool is_locking(lockable_type* l) const { return l==mutex(); } /*< strict lockers specific function >*/

    void relock_on(condition & cond) {
        typename condition::backdoor(cond).wait(*this);
    }
    //void relock_on(condition_boosted & cond) {
    //    typename condition::backdoor(cond).wait(*this);
    //}

    void relock_until(condition & cond, boost::system_time const& abs_time) {
           typename condition::backdoor(cond).wait_until(*this, abs_time);
    }
    //void relock_until(condition_boosted & cond, boost::system_time const& abs_time) {
    //       typename condition::backdoor(cond).wait_until(*this, abs_time);
    //}

    template<typename duration_type>
    void relock_on_for(condition & cond, duration_type const& rel_time) {
        typename condition::backdoor(cond).wait_for(*this, rel_time);
    }
    //template<typename duration_type>
    //void relock_on_for(condition_boosted & cond, duration_type const& rel_time) {
    //    typename condition::backdoor(cond).wait_for(*this, rel_time);
    //}

    template<typename Predicate>
    void relock_when(condition &cond, Predicate pred){
        typename condition::backdoor(cond).wait_when(*this, pred);
    }
    //template<typename Predicate>
    //void relock_when(condition_boosted &cond, Predicate pred){
    //    typename condition::backdoor(cond).wait_when(*this, pred);
    //}

    template<typename Predicate>
    void relock_when_until(condition &cond, Predicate pred,
            boost::system_time const& abs_time){
        typename condition::backdoor(cond).wait_when_until(*this, pred, abs_time);
    }
    //template<typename Predicate>
    //void relock_when_until(condition_boosted &cond, Predicate pred,
    //        boost::system_time const& abs_time){
    //    typename condition::backdoor(cond).wait_when_until(*this, pred, abs_time);
    //}

    template<typename Predicate, typename duration_type>
    void relock_when_for(condition &cond, Predicate pred,
            duration_type const& rel_time){
        typename condition::backdoor(cond).wait_when_for(*this, pred, rel_time);
    }
    //template<typename Predicate, typename duration_type>
    //void relock_when_for(condition_boosted &cond, Predicate pred,
    //        duration_type const& rel_time){
    //    typename condition::backdoor(cond).wait_when_for(*this, pred, rel_time);
    //}

    //void notify_one(condition_boosted &cond){
    //    typename condition::backdoor(cond).notify_one(*this);
    //}
    //void notify_all(condition_boosted &cond){
    //    typename condition::backdoor(cond).notify_all(*this);
    //}

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
    class Condition=condition_safe<typename best_condition_any<Lockable>::type >
    //, class ConditionBoosted=condition_safe_boosted<typename best_condition_any<Lockable>::type >
>
class condition_shared_locker
    : protected shared_lock<Lockable> {
    BOOST_CONCEPT_ASSERT((LockableConcept<Lockable>));
    typedef shared_lock<Lockable> super_type;
public:
    typedef Lockable lockable_type;
    typedef Condition condition;
    //typedef ConditionBoosted condition_boosted;

    explicit condition_shared_locker(lockable_type& obj)
        : super_type(obj) { } /*< locks on construction >*/
    condition_shared_locker(lockable_type& obj, condition &cond)
        : super_type(obj) {
            typename condition::backdoor(cond).wait(*this); /*< relock on condition >*/
        }
    //condition_shared_locker(lockable_type& obj, condition_boosted &cond)
    //    : super_type(obj) {
    //        typename condition::backdoor(cond).wait(*this); /*< relock on condition >*/
    //    }
    template <typename Predicate>
    condition_shared_locker(lockable_type& obj, condition &cond, Predicate pred)
        : super_type(obj) {
            typename condition::backdoor(cond).wait(*this, pred); /*< relock condition when predicate satisfaied>*/
        }
    //template <typename Predicate>
    //condition_shared_locker(lockable_type& obj, condition_boosted &cond, Predicate pred)
    //    : super_type(obj) {
    //        typename condition::backdoor(cond).wait(*this, pred); /*< relock condition when predicate satisfaied>*/
    //    }
    ~condition_shared_locker() { } /*< unlocks on destruction >*/

    typedef bool (condition_shared_locker::*bool_type)() const; /*< safe bool idiom >*/
    operator bool_type() const { return &condition_shared_locker::owns_lock;  }
    bool operator!() const { return false; } /*< always owned >*/
    bool owns_lock() const { return true; }
    bool is_locking(lockable_type* l) const { return l==mutex(); } /*< strict lockers specific function >*/

    void relock_on(condition & cond) {
        typename condition::backdoor(cond).wait(*this);
    }
    //void relock_on(condition_boosted & cond) {
    //    typename condition::backdoor(cond).wait(*this);
    //}

    void relock_until(condition & cond, boost::system_time const& abs_time) {
           typename condition::backdoor(cond).wait_until(*this, abs_time);
    }
    //void relock_until(condition_boosted & cond, boost::system_time const& abs_time) {
    //       typename condition::backdoor(cond).wait_until(*this, abs_time);
    //}

    template<typename duration_type>
    void relock_on_for(condition & cond, duration_type const& rel_time) {
        typename condition::backdoor(cond).wait_for(*this, rel_time);
    }
    //template<typename duration_type>
    //void relock_on_for(condition_boosted & cond, duration_type const& rel_time) {
    //    typename condition::backdoor(cond).wait_for(*this, rel_time);
    //}

    template<typename Predicate>
    void relock_when(condition &cond, Predicate pred){
        typename condition::backdoor(cond).wait_when(*this, pred);
    }
    //template<typename Predicate>
    //void relock_when(condition_boosted &cond, Predicate pred){
    //    typename condition::backdoor(cond).wait_when(*this, pred);
    //}

    template<typename Predicate>
    void relock_when_until(condition &cond, Predicate pred,
            boost::system_time const& abs_time){
        typename condition::backdoor(cond).wait_when_until(*this, pred, abs_time);
    }
    //template<typename Predicate>
    //void relock_when_until(condition_boosted &cond, Predicate pred,
    //        boost::system_time const& abs_time){
    //    typename condition::backdoor(cond).wait_when_until(*this, pred, abs_time);
    //}

    template<typename Predicate, typename duration_type>
    void relock_when_for(condition &cond, Predicate pred,
            duration_type const& rel_time){
        typename condition::backdoor(cond).wait_when_for(*this, pred, rel_time);
    }
    //template<typename Predicate, typename duration_type>
    //void relock_when_for(condition_boosted &cond, Predicate pred,
    //        duration_type const& rel_time){
    //    typename condition::backdoor(cond).wait_when_for(*this, pred, rel_time);
    //}

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
    : public Lockable
{
    BOOST_CONCEPT_ASSERT((LockableConcept<Lockable>));
    typedef Lockable super_type;
public:
    typedef Lockable lockable_type;
    typedef Condition condition;

    condition_lockable()
        : super_type() { }

    ~condition_lockable() { }

    //void lock();
    //void unlock();
    //bool try_lock();
    //bool try_lock_until(system_time const & abs_time)
    //{return the_lock().timed_lock(abs_time);}
    //template<typename TimeDuration>
    //bool try_lock_for(TimeDuration const & relative_time)
    //{return the_lock().timed_lock(relative_time);}
    
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


private:
    friend class boost::condition_variable;
    friend class boost::condition_variable_any;
};
//]

}
}
#endif
