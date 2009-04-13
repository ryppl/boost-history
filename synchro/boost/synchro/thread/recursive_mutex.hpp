//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_THREAD_RECURSIVE_MUTEX__HPP
#define BOOST_SYNCHRO_THREAD_RECURSIVE_MUTEX__HPP

#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/synchro/thread/lockable_scope_traits.hpp>
#include <boost/synchro/lockable_traits.hpp>
#include <boost/synchro/timeout_exception.hpp>
#include <boost/synchro/detail/deleted_functions.hpp>
#include <boost/synchro/lockable/functions.hpp>
#include <boost/conversion/chrono_time_point_to_posix_time_ptime.hpp>
#include <boost/conversion/chrono_duration_to_posix_time_duration.hpp>

namespace boost { namespace synchro {

#if 0    
class thread_recursive_mutex
: public lock_traits_base<
    multi_threaded_tag,
    exclusive_lock_tag,
    recursive_tag,
    hasnt_timed_interface_tag,
    process_lifetime_tag,
    anonymous_tag,
    recursive_mutex
>
{
public:
    typedef boost::condition_variable_any  best_condition_type;
    typedef boost::condition_variable_any  best_condition_any_type;

    //Non-copyable
    BOOST_COPY_CONSTRUCTOR_DELETE(thread_recursive_mutex) /*< disable copy construction >*/
    BOOST_COPY_ASSIGNEMENT_DELETE(thread_recursive_mutex) /*< disable copy asignement >*/
    thread_recursive_mutex() {}
};   
#endif
#if 1    
typedef boost::recursive_mutex thread_recursive_mutex;

template<>
struct timed_interface_tag<boost::recursive_mutex> {
    typedef hasnt_timed_interface_tag type;
};

template<>
struct reentrancy_tag<boost::recursive_mutex> {
    typedef recursive_tag type;
};

template<>
struct category_tag<boost::recursive_mutex> {
    typedef exclusive_lock_tag type;
};

template<>
struct scope_tag<boost::recursive_mutex> {
    typedef multi_threaded_tag type;
};

template<>
struct lifetime_tag<boost::recursive_mutex> {
    typedef process_lifetime_tag type;
};

template<>
struct naming_tag<boost::recursive_mutex> {
    typedef anonymous_tag type;
};

template <>
struct best_condition<boost::recursive_mutex> {
    typedef boost::condition_variable_any type;
};
template <>
struct best_condition_any<boost::recursive_mutex> {
    typedef boost::condition_variable_any type;
};
#endif

#if 0
class thread_recursive_timed_mutex
: public lock_traits_base<
    multi_threaded_tag,
    exclusive_lock_tag,
    recursive_tag,
    has_timed_interface_tag,
    process_lifetime_tag,
    anonymous_tag,
    recursive_timed_mutex
>
{

public:
    typedef boost::condition_variable_any  best_condition_type;
    typedef boost::condition_variable_any  best_condition_any_type;

    //Non-copyable
    BOOST_COPY_CONSTRUCTOR_DELETE(thread_recursive_timed_mutex) /*< disable copy construction >*/
    BOOST_COPY_ASSIGNEMENT_DELETE(thread_recursive_timed_mutex) /*< disable copy asignement >*/
    thread_recursive_timed_mutex() {}

    template<typename Clock, typename Duration>
    bool try_lock_until(chrono::time_point<Clock, Duration> const & abs_time)
    {
        return timed_lock(boost::convert_to<posix_time::ptime>(abs_time));
    }
    template<typename Rep, typename Period>
    bool try_lock_for(chrono::duration<Rep, Period> const & rel_time)
    {
        return timed_lock(boost::convert_to<posix_time::time_duration>(rel_time));
    }

    template<typename Clock, typename Duration>
    void lock_until(chrono::time_point<Clock, Duration> const & abs_time)
    {
        if(!timed_lock(boost::convert_to<posix_time::ptime>(abs_time))) throw timeout_exception();
    }
    template<typename Rep, typename Period>
    void lock_for(chrono::duration<Rep, Period> const & rel_time)
    {
        if(!timed_lock(boost::convert_to<posix_time::time_duration>(rel_time))) throw timeout_exception();
    }
    
};    
#endif
#if 1
typedef boost::recursive_timed_mutex thread_recursive_timed_mutex;

template<>
struct timed_interface_tag<boost::recursive_timed_mutex> {
    typedef has_timed_interface_tag type;
};
template<>
struct reentrancy_tag<boost::recursive_timed_mutex> {
    typedef recursive_tag type;
};
template<>
struct category_tag<boost::recursive_timed_mutex> {
    typedef exclusive_lock_tag type;
};
template<>
struct scope_tag<boost::recursive_timed_mutex> {
    typedef multi_threaded_tag type;
};

template<>
struct lifetime_tag<boost::recursive_timed_mutex> {
    typedef process_lifetime_tag type;
};

template<>
struct naming_tag<boost::recursive_timed_mutex> {
    typedef anonymous_tag type;
};

template <>
struct best_condition<boost::recursive_timed_mutex> {
    typedef boost::condition_variable_any type;
};
template <>
struct best_condition_any<boost::recursive_timed_mutex> {
    typedef boost::condition_variable_any type;
};

#endif

namespace lockable {
    namespace partial_specialization_workaround {
        template <class Clock, class Duration >
        struct lock_until<boost::recursive_timed_mutex,Clock, Duration>   {
            static void 
            apply( boost::recursive_timed_mutex& lockable, const chrono::time_point<Clock, Duration>& abs_time ) {
                if(!lockable.timed_lock(boost::convert_to<posix_time::ptime>(abs_time))) throw timeout_exception();
            }
        };
        template <class Rep, class Period >
        struct lock_for<boost::recursive_timed_mutex,Rep, Period> {
            static void 
            apply( boost::recursive_timed_mutex& lockable, const chrono::duration<Rep, Period>& rel_time ) {
                if(!lockable.timed_lock(boost::convert_to<posix_time::time_duration>(rel_time))) throw timeout_exception();
            }
        };
        template <class Clock, class Duration >
        struct try_lock_until<boost::recursive_timed_mutex,Clock, Duration> {
            static typename result_of::template try_lock_until<boost::recursive_timed_mutex,Clock, Duration>::type 
            apply( boost::recursive_timed_mutex& lockable, const chrono::time_point<Clock, Duration>& abs_time ) {
                return lockable.timed_lock(boost::convert_to<posix_time::ptime>(abs_time));
            }
        };
        template <class Rep, class Period >
        struct try_lock_for<boost::recursive_timed_mutex,Rep, Period> {
            static typename result_of::template try_lock_for<boost::recursive_timed_mutex,Rep, Period>::type 
            apply( boost::recursive_timed_mutex& lockable, const chrono::duration<Rep, Period>& rel_time ) {
                return lockable.timed_lock(boost::convert_to<posix_time::time_duration>(rel_time));
            }
        };
    }
}

}
}

#endif
