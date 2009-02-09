//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_PROCESS_UPGRADABLE_MUTEX__HPP
#define BOOST_SYNCHRO_PROCESS_UPGRADABLE_MUTEX__HPP

#include <boost/interprocess/sync/interprocess_upgradable_mutex.hpp>
#include <boost/synchro/lockable_traits.hpp>
#include <boost/synchro/process/lockable_scope_traits.hpp>

namespace boost { namespace synchro {

class interprocess_upgradable_mutex
: public lock_traits_base<
	multi_process_tag,
	upgradable_lock_tag,
	non_recursive_tag,
	has_timed_interface_tag,
    kernel_lifetime_tag,
    anonymous_tag,
	interprocess::interprocess_upgradable_mutex
>
{

    //Non-copyable
    interprocess_upgradable_mutex(const interprocess_upgradable_mutex &);
    interprocess_upgradable_mutex &operator=(const interprocess_upgradable_mutex &);

public:

	typedef boost::interprocess::interprocess_condition  condition_type;
	typedef boost::interprocess::interprocess_condition  condition_any_type;

   void lock_shared()
   {lock_sharable();}

   bool try_lock_shared()
   {return try_lock_sharable();}

   bool timed_lock_shared(const boost::posix_time::ptime &abs_time)
   {return timed_lock_sharable(abs_time);}

   void unlock_shared()
   {unlock_sharable();}

   void lock_upgrade()
   {lock_upgradable();}

   bool try_lock_upgrade()
   {return try_lock_upgradable();}

   bool timed_lock_upgrade(const boost::posix_time::ptime &abs_time)
   {return timed_lock_upgradable(abs_time);}

   void unlock_upgrade()
   {unlock_upgradable();}

   void unlock_and_lock_upgrade()
   {unlock_and_lock_upgrade();}

   void unlock_and_lock_shared()
   {unlock_and_lock_sharable();}

   void unlock_upgrade_and_lock_shared()
   {unlock_upgradable_and_lock_sharable();}

   void unlock_upgrade_and_lock()
   {unlock_upgrade_and_lock();}

   bool try_unlock_upgrade_and_lock()
   {return try_unlock_upgradable_and_lock();}

   bool timed_unlock_upgrade_and_lock(const boost::posix_time::ptime &abs_time)
   {return timed_unlock_upgradable_and_lock(abs_time);}

   bool try_unlock_share_and_lock()
   {return try_unlock_sharable_and_lock();}

   bool try_unlock_share_and_lock_upgrade()
   {return try_unlock_sharable_and_lock_upgradable();}

};


//template<>
//struct timed_interface_tag<boost::synchro::interprocess_upgradable_mutex> {
//	typedef has_timed_interface_tag type;
//};
//template<>
//struct reentrancy_tag<boost::synchro::interprocess_upgradable_mutex> {
//	typedef non_recursive_tag type;
//};
//template<>
//struct category_tag<boost::synchro::interprocess_upgradable_mutex> {
//	typedef upgradable_lock_tag type;
//};
//template<>
//struct scope_tag<boost::synchro::interprocess_upgradable_mutex> {
//	typedef multi_process_tag type;
//};

#if 0
template<>
struct syntactic_lock_traits<boost::synchro::interprocess_upgradable_mutex>
	: syntactic_process_lock_traits<boost::synchro::interprocess_upgradable_mutex> {};
#endif
}
}

#endif
