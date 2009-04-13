//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_PROCESS_RECURSIVE_MUTEX__HPP
#define BOOST_SYNCHRO_PROCESS_RECURSIVE_MUTEX__HPP

#include <boost/interprocess/sync/interprocess_recursive_mutex.hpp>
#include <boost/synchro/lockable_traits.hpp>
#include <boost/synchro/process/lockable_scope_traits.hpp>
#include <boost/synchro/timeout_exception.hpp>
#include <boost/synchro/detail/deleted_functions.hpp>
#include <boost/conversion/chrono_time_point_to_posix_time_ptime.hpp>
#include <boost/conversion/chrono_duration_to_posix_time_duration.hpp>

namespace boost { namespace synchro {

#if 0
class interprocess_recursive_mutex
: public lock_traits_base<
    multi_process_tag,
    exclusive_lock_tag,
    recursive_tag,
    has_timed_interface_tag,
    kernel_lifetime_tag,
    anonymous_tag,
    boost::interprocess::interprocess_recursive_mutex
>
{
public:
    typedef boost::interprocess::interprocess_condition  best_condition_type;
    typedef boost::interprocess::interprocess_condition  best_condition_any_type;

    //Non-copyable
    BOOST_COPY_CONSTRUCTOR_DELETE(interprocess_recursive_mutex) /*< disable copy construction >*/
    BOOST_COPY_ASSIGNEMENT_DELETE(interprocess_recursive_mutex) /*< disable copy asignement >*/
    interprocess_recursive_mutex(){}
        
    bool try_lock_until(system_time const & abs_time)
    {return this->timed_lock(abs_time);}
    template<typename TimeDuration>
    bool try_lock_for(TimeDuration const & relative_time)
    {return timed_lock(boost::get_system_time()+relative_time);}

    void lock_until(system_time const & abs_time)
    {if(!timed_lock(abs_time)) throw timeout_exception();}
    template<typename TimeDuration>
    void lock_for(TimeDuration const & relative_time)
    {if(!timed_lock(boost::get_system_time()+relative_time)) throw timeout_exception();}
    
    //bool try_lock_shared_until(system_time const& abs_time)
    //{return timed_lock_shared(abs_time);}

};    
#endif

typedef boost::interprocess::interprocess_recursive_mutex interprocess_recursive_mutex;

template<>
struct timed_interface_tag<boost::interprocess::interprocess_recursive_mutex> {
    typedef has_timed_interface_tag type;
};
template<>
struct reentrancy_tag<boost::interprocess::interprocess_recursive_mutex> {
    typedef recursive_tag type;
};
template<>
struct category_tag<boost::interprocess::interprocess_recursive_mutex> {
    typedef exclusive_lock_tag type;
};
template<>
struct scope_tag<boost::interprocess::interprocess_recursive_mutex> {
    typedef multi_process_tag type;
};

template<>
struct lifetime_tag<boost::interprocess::interprocess_recursive_mutex> {
    typedef kernel_lifetime_tag type;
};

template<>
struct naming_tag<boost::interprocess::interprocess_recursive_mutex> {
    typedef anonymous_tag type;
};


template <>
struct best_condition<boost::interprocess::interprocess_recursive_mutex> {
    typedef boost::interprocess::interprocess_condition type;
};

template <>
struct best_condition_any<boost::interprocess::interprocess_recursive_mutex> {
    typedef boost::interprocess::interprocess_condition type;
};

namespace lockable {
    namespace partial_specialization_workaround {
        template <class Clock, class Duration >
        struct lock_until<boost::interprocess::interprocess_recursive_mutex,Clock, Duration>   {
            static void 
            apply( boost::interprocess::interprocess_recursive_mutex& lockable, const chrono::time_point<Clock, Duration>& abs_time ) {
                if(!lockable.timed_lock(boost::convert_to<posix_time::ptime>(abs_time))) throw timeout_exception();
            }
        };
        template <class Rep, class Period >
        struct lock_for<boost::interprocess::interprocess_recursive_mutex,Rep, Period> {
            static void 
            apply( boost::interprocess::interprocess_recursive_mutex& lockable, const chrono::duration<Rep, Period>& rel_time ) {
                if(!lockable.timed_lock(get_system_time()+boost::convert_to<posix_time::time_duration>(rel_time))) throw timeout_exception();
            }
        };
        template <class Clock, class Duration >
        struct try_lock_until<boost::interprocess::interprocess_recursive_mutex,Clock, Duration> {
            static typename result_of::template try_lock_until<boost::interprocess::interprocess_recursive_mutex,Clock, Duration>::type 
            apply( boost::interprocess::interprocess_recursive_mutex& lockable, const chrono::time_point<Clock, Duration>& abs_time ) {
                return lockable.timed_lock(boost::convert_to<posix_time::ptime>(abs_time));
            }
        };
        template <class Rep, class Period >
        struct try_lock_for<boost::interprocess::interprocess_recursive_mutex,Rep, Period> {
            static typename result_of::template try_lock_for<boost::interprocess::interprocess_recursive_mutex,Rep, Period>::type 
            apply( boost::interprocess::interprocess_recursive_mutex& lockable, const chrono::duration<Rep, Period>& rel_time ) {
                return lockable.timed_lock(get_system_time()+boost::convert_to<posix_time::time_duration>(rel_time));
            }
        };
    }
}


}
}

#endif
