//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_CONDITION_SAFE__HPP
#define BOOST_SYNCHRO_CONDITION_SAFE__HPP

#include <boost/synchro/lockable_concepts.hpp>
#include <boost/thread/condition.hpp>
#include <boost/synchro/thread/mutex.hpp>

namespace boost { namespace synchro {

template <
    class Condition
>
class condition_backdoor;

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
//        return cond_.wait(lock, pred);
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


}
}
#endif
