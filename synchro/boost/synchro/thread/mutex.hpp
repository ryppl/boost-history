//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_THREAD_MUTEX__HPP
#define BOOST_SYNCHRO_THREAD_MUTEX__HPP


#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/synchro/lockable_traits.hpp>
#include <boost/thread/thread_time.hpp>
#include <boost/synchro/thread/lockable_scope_traits.hpp>
#include <boost/synchro/timeout_exception.hpp>
#include <boost/synchro/detail/deleted_functions.hpp>


namespace boost { namespace synchro {

class thread_mutex
: public lock_traits_base<
    multi_threaded_tag,
    exclusive_lock_tag,
    non_recursive_tag,
    hasnt_timed_interface_tag,
    process_lifetime_tag,
    anonymous_tag,
    mutex
>
{
public:
    // public types
    typedef boost::condition_variable  best_condition_type;
    typedef boost::condition_variable_any  best_condition_any_type;

    //Non-copyable
    BOOST_COPY_CONSTRUCTOR_DELETE(thread_mutex) /*< disable copy construction >*/
    BOOST_COPY_ASSIGNEMENT_DELETE(thread_mutex) /*< disable copy asignement >*/

    thread_mutex() {}
};

template <>
struct unique_lock_type<thread_mutex> {
    typedef boost::unique_lock<boost::mutex> type;
};

template <>
struct shared_lock_type<thread_mutex> {
    typedef boost::shared_lock<boost::mutex> type;
};

template <>
struct upgrade_lock_type<thread_mutex> {
    typedef boost::upgrade_lock<boost::mutex> type;
};

template <>
struct upgrade_to_unique_locker_type<thread_mutex> {
    typedef boost::upgrade_to_unique_lock<boost::mutex> type;
};


template <>
struct lock_error_type<boost::mutex> {
    typedef boost::lock_error type;
};

#if 0
typedef boost::mutex thread_mutex;

template<>
struct timed_interface_tag<boost::mutex> {
    typedef hasnt_timed_interface_tag type;
};
template<>
struct reentrancy_tag<boost::mutex> {
    typedef non_recursive_tag type;
};
template<>
struct category_tag<boost::mutex> {
    typedef exclusive_lock_tag type;
};

template<>
struct scope_tag<boost::mutex> {
    typedef multi_threaded_tag type;
};

template<>
struct lifetime_tag<boost::mutex> {
    typedef process_lifetime_tag type;
};

template<>
struct naming_tag<boost::mutex> {
    typedef anonymous_tag type;
};

template <>
struct best_condition<boost::mutex> {
    typedef boost::condition_variable type;
};
template <>
struct best_condition_any<boost::mutex> {
    typedef boost::condition_variable_any type;
};
#endif

////////////////////////////////////////////////////

class thread_timed_mutex
: public lock_traits_base<
    multi_threaded_tag,
    exclusive_lock_tag,
    non_recursive_tag,
    has_timed_interface_tag,
    process_lifetime_tag,
    anonymous_tag,
    timed_mutex
> {
public:
    typedef boost::condition_variable_any  best_condition_type;
    typedef boost::condition_variable_any  best_condition_any_type;

    //Non-copyable
    BOOST_COPY_CONSTRUCTOR_DELETE(thread_timed_mutex) /*< disable copy construction >*/
    BOOST_COPY_ASSIGNEMENT_DELETE(thread_timed_mutex) /*< disable copy asignement >*/
    thread_timed_mutex () {}
        
    bool try_lock_until(system_time const & abs_time) {
        return timed_lock(abs_time);
    }
    template<typename TimeDuration>
    bool try_lock_for(TimeDuration const & relative_time) {
        return timed_lock(relative_time);
    }

    void lock_until(system_time const & abs_time) {
        if(!timed_lock(abs_time)) throw timeout_exception();
    }
    template<typename TimeDuration>
    void lock_for(TimeDuration const & relative_time) {
        if(!timed_lock(relative_time)) throw timeout_exception();
    }
};    

#if 0
typedef boost::timed_mutex thread_timed_mutex;

template<>
struct timed_interface_tag<boost::timed_mutex> {
    typedef has_timed_interface_tag type;
};
template<>
struct reentrancy_tag<boost::timed_mutex> {
    typedef non_recursive_tag type;
};
template<>
struct category_tag<boost::timed_mutex> {
    typedef exclusive_lock_tag type;
};
template<>
struct scope_tag<boost::timed_mutex> {
    typedef multi_threaded_tag type;
};

template<>
struct lifetime_tag<boost::timed_mutex> {
    typedef process_lifetime_tag type;
};

template<>
struct naming_tag<boost::timed_mutex> {
    typedef anonymous_tag type;
};

template <>
struct best_condition<boost::timed_mutex> {
    typedef boost::condition_variable_any type;
};
template <>
struct best_condition_any<boost::timed_mutex> {
    typedef boost::condition_variable_any type;
};

#endif
}
}

#endif
