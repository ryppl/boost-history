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
//#include <boost/thread/condition.hpp>
//#include <boost/synchro/thread/mutex.hpp>
#include <boost/synchro/condition_backdoor.hpp>
#include <boost/synchro/condition_safe.hpp>
#include <boost/synchro/lockers.hpp>
#include <boost/convert_to/chrono_time_point_to_posix_time_ptime.hpp>
#include <boost/convert_to/chrono_duration_to_posix_time_duration.hpp>

namespace boost { namespace synchro {

#if 0
//[condition_unique_locker
template <
    typename Lockable,
    class Condition=condition_safe<typename best_condition<Lockable>::type >
    , typename ScopeTag=typename scope_tag<Lockable>::type
>
class condition_unique_locker
    : protected unique_locker<Lockable,ScopeTag>
{
    BOOST_CONCEPT_ASSERT((LockableConcept<Lockable>));
    typedef unique_locker<Lockable, ScopeTag> super_type;
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
    template<typename Clock, typename Duration>
    void relock_until(condition & cond, chrono::time_point<Clock, Duration> const& abs_time);
    template<typename duration_type>
    void relock_on_for(condition & cond, duration_type const& rel_time);

    template<typename Predicate>
    void relock_when(condition &cond, Predicate pred);
    template<typename Predicate>
    template<typename Clock, typename Duration>
    void relock_when_until(condition &cond, Predicate pred,
            chrono::time_point<Clock, Duration> const& abs_time);
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
    , typename ScopeTag=typename scope_tag<Lockable>::type
    //, class ConditionBoosted=condition_safe_boosted<typename best_condition<Lockable>::type >
>
class condition_unique_locker
    : protected unique_locker<Lockable, ScopeTag>
{
    BOOST_CONCEPT_ASSERT((LockableConcept<Lockable>));
    typedef unique_locker<Lockable, ScopeTag> super_type;
public:
    typedef Lockable lockable_type;
    typedef Condition condition;
    //typedef ConditionBoosted condition_boosted;

    explicit condition_unique_locker(lockable_type& obj)
        : super_type(obj) { } /*< locks on construction >*/

    template<typename Rep, typename Period>
    condition_unique_locker(chrono::duration<Rep, Period> const& target_time, lockable_type& m_)
        : super_type(target_time, m_)
    {}
    template<typename Clock, typename Duration>
    condition_unique_locker(chrono::time_point<Clock, Duration> const& target_time, lockable_type& m_)
        : super_type(target_time, m_)
    {}

    condition_unique_locker(lockable_type& obj, condition &cond)
        : super_type(obj) {
            typename condition::backdoor(cond).wait(*static_cast<super_type*>(this)); /*< relock on condition >*/
        }
    //condition_unique_locker(lockable_type& obj, condition_boosted &cond)
    //    : super_type(obj) {
    //        typename condition::backdoor(cond).wait(*static_cast<unique_lock<Lockable>*>(this)); /*< relock on condition >*/
    //    }

    template<typename Rep, typename Period>
    condition_unique_locker(chrono::duration<Rep, Period> const& target_time, lockable_type& obj, condition &cond)
        : super_type(obj) {
            typename condition::backdoor(cond).wait_for(target_time, *static_cast<super_type*>(this)); /*< relock on condition >*/
        }
    template<typename Clock, typename Duration>
    condition_unique_locker(chrono::time_point<Clock, Duration> const& target_time, lockable_type& obj, condition &cond)
        : super_type(obj) {
            typename condition::backdoor(cond).wait_until(target_time, *static_cast<super_type*>(this)); /*< relock on condition >*/
        }
        
    template <typename Predicate>
    condition_unique_locker(lockable_type& obj, condition &cond, Predicate pred)
        : super_type(obj) {
            typename condition::backdoor(cond).wait_when(*static_cast<super_type*>(this), pred); /*< relock condition when predicate satisfaied>*/
            //typename condition::backdoor(cond).wait_when(*this, pred); /*< relock condition when predicate satisfaied>*/
        }
    //template <typename Predicate>
    //condition_unique_locker(lockable_type& obj, condition_boosted &cond, Predicate pred)
    //    : super_type(obj) {
    //        typename condition::backdoor(cond).wait_when(*static_cast<unique_lock<Lockable>*>(this), pred); /*< relock condition when predicate satisfaied>*/
    //    }
    template <typename Rep, typename Period, typename Predicate>
    condition_unique_locker(chrono::duration<Rep, Period> const& target_time, lockable_type& obj, condition &cond, Predicate pred)
        : super_type(obj) {
            typename condition::backdoor(cond).wait_when_for(*static_cast<super_type*>(this), pred, target_time); /*< relock condition when predicate satisfaied>*/
            //typename condition::backdoor(cond).wait_when(*this, pred); /*< relock condition when predicate satisfaied>*/
        }
    template <typename Predicate, typename Clock, typename Duration>
    condition_unique_locker(chrono::time_point<Clock, Duration> const& target_time, lockable_type& obj, condition &cond, Predicate pred)
        : super_type(obj) {
            typename condition::backdoor(cond).wait_when_until(*static_cast<super_type*>(this), pred, target_time); /*< relock condition when predicate satisfaied>*/
            //typename condition::backdoor(cond).wait_when(*this, pred); /*< relock condition when predicate satisfaied>*/
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
    //void relock_on(condition_boosted & cond) {
    //    typename condition::backdoor(cond).wait(*this);
    //}

    template<typename Clock, typename Duration>
    void relock_until(condition & cond, chrono::time_point<Clock, Duration> const& abs_time) {
           typename condition::backdoor(cond).wait_until(*this, abs_time);
    }
    //void relock_until(condition_boosted & cond, chrono::time_point<Clock, Duration> const& abs_time) {
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

    template <typename Predicate, typename Clock, typename Duration>
    void relock_when_until(condition &cond, Predicate pred,
            chrono::time_point<Clock, Duration> const& abs_time){
        typename condition::backdoor(cond).wait_when_until(*this, pred, abs_time);
    }
    //template<typename Predicate>
    //void relock_when_until(condition_boosted &cond, Predicate pred,
    //        chrono::time_point<Clock, Duration> const& abs_time){
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
    , typename ScopeTag=typename scope_tag<Lockable>::type
    //, class ConditionBoosted=condition_safe_boosted<typename best_condition_any<Lockable>::type >
>
class condition_shared_locker
    : protected shared_locker<Lockable, ScopeTag> {
//    : protected shared_lock<Lockable> {
    BOOST_CONCEPT_ASSERT((LockableConcept<Lockable>));
    typedef shared_locker<Lockable,ScopeTag> super_type;
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

    template <typename Clock, typename Duration>
    void relock_until(condition & cond, chrono::time_point<Clock, Duration> const& abs_time) {
           typename condition::backdoor(cond).wait_until(*this, abs_time);
    }
    //void relock_until(condition_boosted & cond, chrono::time_point<Clock, Duration> const& abs_time) {
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

    template <typename Predicate, typename Clock, typename Duration>
    void relock_when_until(condition &cond, Predicate pred,
            chrono::time_point<Clock, Duration> const& abs_time){
        typename condition::backdoor(cond).wait_when_until(*this, pred, abs_time);
    }
    //template<typename Predicate>
    //void relock_when_until(condition_boosted &cond, Predicate pred,
    //        chrono::time_point<Clock, Duration> const& abs_time){
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


}
}
#endif
