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

namespace boost { namespace synchro {

    
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

    //Non-copyable
    thread_recursive_mutex(const thread_recursive_mutex &);
    thread_recursive_mutex &operator=(const thread_recursive_mutex &);

public:

    typedef boost::condition_variable_any  best_condition_type;
    typedef boost::condition_variable_any  best_condition_any_type;
    thread_recursive_mutex() {}
};    
#if 0    
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

    //Non-copyable
    thread_recursive_timed_mutex(const thread_recursive_timed_mutex &);
    thread_recursive_timed_mutex &operator=(const thread_recursive_timed_mutex &);

public:
    thread_recursive_timed_mutex() {}
        
    typedef boost::condition_variable_any  best_condition_type;
    typedef boost::condition_variable_any  best_condition_any_type;

    bool try_lock_until(system_time const & abs_time)
    {return this->timed_lock(abs_time);}
    template<typename TimeDuration>
    bool try_lock_for(TimeDuration const & relative_time)
    {return timed_lock(relative_time);}

    void lock_until(system_time const & abs_time)
    {if(!timed_lock(abs_time)) throw timeout_exception();}
    template<typename TimeDuration>
    void lock_for(TimeDuration const & relative_time)
    {if(!timed_lock(relative_time)) throw timeout_exception();}
    
};    

#if 0
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
}
}

#endif
