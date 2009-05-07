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
#include <boost/synchro/detail/deleted_functions.hpp>
#include <boost/synchro/detail/defaulted_functions.hpp>
#include <boost/conversion/boost/chrono_time_point_to_posix_time_ptime.hpp>
#include <boost/conversion/boost/chrono_duration_to_posix_time_duration.hpp>

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

    BOOST_DEFAULT_CONSTRUCTOR_DEFAULT(condition_safe) /*< reestablish default construction >*/
    BOOST_COPY_CONSTRUCTOR_DELETE(condition_safe) /*< disable copy construction >*/
    BOOST_COPY_ASSIGNEMENT_DELETE(condition_safe) /*< disable copy asignement >*/

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

    template <typename Locker, typename Clock, typename Duration>
    void wait_until(Locker& lock, chrono::time_point<Clock, Duration> const& abs_time) {
        if (!cond_.timed_wait(lock,boost::convert_to<posix_time::ptime>(abs_time))) throw timeout_exception();
    }

    template<typename Locker, typename Rep, typename Period>
    void wait_for(Locker& lock,chrono::duration<Rep, Period> const& rel_time) {
        if (!cond_.timed_wait(lock, boost::convert_to<posix_time::time_duration>(rel_time))) throw timeout_exception();
    }

    template<typename Locker, typename predicate_type, typename Clock, typename Duration>
    void wait_when_until(Locker& lock, predicate_type pred, chrono::time_point<Clock, Duration> const& abs_time) {
        if (!cond_.timed_wait(lock, pred, boost::convert_to<posix_time::ptime>(abs_time))) throw timeout_exception();
    }
    template<typename Locker, typename predicate_type, typename Rep, typename Period>
    void wait_when_for(Locker& lock, predicate_type pred, chrono::duration<Rep, Period> const& rel_time) {
        if (!cond_.timed_wait(lock, pred, boost::convert_to<posix_time::time_duration>(rel_time))) throw timeout_exception();
    }
private:
    Condition cond_;
};


}
}
#endif
