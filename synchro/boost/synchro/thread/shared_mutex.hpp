//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_THREAD_SHARED_MUTEX__HPP
#define BOOST_SYNCHRO_THREAD_SHARED_MUTEX__HPP

#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/synchro/lockable_traits.hpp>
#include <boost/synchro/thread/lockable_scope_traits.hpp>
#include <boost/synchro/timeout_exception.hpp>

namespace boost { namespace synchro {
    
class thread_shared_mutex
: public lock_traits_base<
    multi_threaded_tag,
    upgradable_lock_tag,
    non_recursive_tag,
    has_timed_interface_tag,
    process_lifetime_tag,
    anonymous_tag,
    shared_mutex
>
{

    //Non-copyable
    thread_shared_mutex(const thread_shared_mutex &);
    thread_shared_mutex &operator=(const thread_shared_mutex &);

public:
    typedef boost::condition_variable_any  best_condition_type;
    typedef boost::condition_variable_any  best_condition_any_type;

    thread_shared_mutex() {}

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
    
    bool try_lock_shared_until(system_time const& abs_time)
    {return timed_lock_shared(abs_time);}

    void lock_shared_until(system_time const& abs_time)
    {if(!timed_lock_shared(abs_time)) throw timeout_exception();}
    
};    

#if 0

typedef boost::shared_mutex thread_shared_mutex;

template<>
struct timed_interface_tag<boost::shared_mutex> {
	typedef has_timed_interface_tag type;
};
template<>
struct reentrancy_tag<boost::shared_mutex> {
	typedef non_recursive_tag type;
};
template<>
struct category_tag<boost::shared_mutex> {
	typedef upgradable_lock_tag type;
};
template<>
struct scope_tag<boost::shared_mutex> {
	typedef multi_threaded_tag type;
};

template<>
struct lifetime_tag<boost::shared_mutex> {
	typedef process_lifetime_tag type;
};

template<>
struct naming_tag<boost::shared_mutex> {
	typedef anonymous_tag type;
};

template <>
struct best_condition<boost::shared_mutex> {
	typedef boost::condition_variable_any type;
};

template <>
struct best_condition_any<boost::shared_mutex> {
	typedef boost::condition_variable_any type;
};
#endif
}
}

#endif
