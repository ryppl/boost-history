//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_CONDITION_LOCKABLE__HPP
#define BOOST_SYNCHRO_CONDITION_LOCKABLE__HPP

#include <boost/synchro/lockable_concepts.hpp>
#include <boost/thread/condition.hpp>
#include <boost/interprocess/interprocess_condition.hpp>
#include <boost/synchro/thread/mutex.hpp>
#include <boost/synchro/condition_backdoor.hpp>
#include <boost/synchro/condition_safe.hpp>
#include <boost/synchro/timeout_exception.hpp>

namespace boost { namespace synchro {

//[condition_lockable
template <
    typename Lockable=thread_mutex,
    class Condition=condition_safe<typename best_condition<Lockable>::type >
>
class condition_lockable;

template <typename Lockable, class Condition >
class condition_lockable : public Lockable {
    BOOST_CONCEPT_ASSERT((LockableConcept<Lockable>));
public:
    typedef Lockable lockable_type;
    typedef Condition condition;

    condition_lockable() {}
    ~condition_lockable() {}
  
    template<typename Predicate>
    void lock_when(condition &cond, Predicate pred){
        typename condition::backdoor(cond).wait_when(*this, pred);
    }
    template<typename Predicate>
    void lock_when_until(condition &cond, Predicate pred,
            boost::system_time const& abs_time){
        typename condition::backdoor(cond).wait_when_until(*this, pred, abs_time);
    }
    template<typename Predicate, typename duration_type>
    void lock_when_for(condition &cond, Predicate pred,
            duration_type const& rel_time){
        typename condition::backdoor(cond).wait_when_for(*this, pred, rel_time);
    }
        
    void relock_on(condition & cond) {
        typename condition::backdoor(cond).wait(*this);
    }
    void relock_on_until(condition & cond, boost::system_time const& abs_time) {
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
    //friend typename best_condition<Lockable>::type;
    //friend typename best_condition_any<Lockable>::type;
    friend class boost::condition_variable;
    friend class boost::condition_variable_any;
    friend class boost::interprocess::condition;
};
//]

typedef condition_lockable<> thread_condition_mutex;
typedef condition_lockable<interprocess_mutex> interprocess_condition_mutex;

}
}
#endif
