//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_PROCESS_NAMED_RECURSIVE_MUTEX__HPP
#define BOOST_SYNCHRO_PROCESS_NAMED_RECURSIVE_MUTEX__HPP

#include <boost/interprocess/sync/named_recursive_mutex.hpp>
#include <boost/synchro/lockable_traits.hpp>
#include <boost/synchro/process/lockable_scope_traits.hpp>
#include <boost/synchro/timeout_exception.hpp>

namespace boost { namespace synchro {
class named_recursive_mutex
:   public boost::interprocess::named_recursive_mutex,
    public lock_traits_base<
    multi_process_tag,
    exclusive_lock_tag,
    recursive_tag,
    has_timed_interface_tag,
    kernel_lifetime_tag,
    named_tag
>
{

    //Non-copyable
    named_recursive_mutex(const named_recursive_mutex &);
    named_recursive_mutex &operator=(const named_recursive_mutex &);
    named_recursive_mutex();

public:

    typedef boost::interprocess::interprocess_condition  best_condition_type;
    typedef boost::interprocess::interprocess_condition  best_condition_any_type;

	named_recursive_mutex(interprocess::create_only_t create_only, const char *name)
   	    : interprocess::named_recursive_mutex(create_only, name) {};
   	named_recursive_mutex(interprocess::open_or_create_t open_or_create, const char *name)
  	    : interprocess::named_recursive_mutex(open_or_create, name) {};
  	named_recursive_mutex(interprocess::open_only_t open_only, const char *name)
  	    : interprocess::named_recursive_mutex(open_only, name) {};

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
typedef boost::interprocess::named_recursive_mutex named_recursive_mutex;

template<>
struct timed_interface_tag<boost::interprocess::named_recursive_mutex> {
	typedef has_timed_interface_tag type;
};
template<>
struct reentrancy_tag<boost::interprocess::named_recursive_mutex> {
	typedef recursive_tag type;
};
template<>
struct category_tag<boost::interprocess::named_recursive_mutex> {
	typedef exclusive_lock_tag type;
};
template<>
struct scope_tag<boost::interprocess::named_recursive_mutex> {
	typedef multi_process_tag type;
};

template<>
struct lifetime_tag<boost::interprocess::named_recursive_mutex> {
	typedef kernel_lifetime_tag type;
};

template<>
struct naming_tag<boost::interprocess::named_recursive_mutex> {
	typedef anonymous_tag type;
};

template <>
struct best_condition<boost::interprocess::named_recursive_mutex> {
	typedef boost::interprocess::interprocess_condition type;
};

template <>
struct best_condition_any<boost::interprocess::named_recursive_mutex> {
	typedef boost::interprocess::interprocess_condition type;
};

#endif

}
}

#endif
