//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_NULL_CONDITION_HPP
#define BOOST_SYNCHRO_NULL_CONDITION_HPP


#include <boost/synchro/lockable_traits.hpp>
#include "boost/thread/thread_time.hpp"

//!\file
//!Describes null_mutex classes

namespace boost {

namespace posix_time
{  class ptime;   }

namespace synchro {
class null_condition;
//!Implements a mutex that simulates a mutex without doing any operation and
//!simulates a successful operation.
class null_mutex
: public lock_traits_base<
    mono_threaded_tag,
    upgradable_lock_tag,
    recursive_tag,
    has_timed_interface_tag, void
>
{
   /// @cond
   null_mutex(const null_mutex&);
   null_mutex &operator= (const null_mutex&);
   /// @endcond
public:
    typedef null_condition condition_type;
    typedef null_condition condition_any_type;

   //!Constructor.
   //!Empty.
   null_mutex(){}

   //!Destructor.
   //!Empty.
   ~null_mutex(){}

   //!Simulates a mutex lock() operation. Empty function.
   void lock(){}

   //!Simulates a mutex try_lock() operation.
   //!Equivalent to "return true;"
   bool try_lock()
   {  return true;   }

   //!Simulates a mutex timed_lock() operation.
   //!Equivalent to "return true;"
   bool timed_lock(const boost::posix_time::ptime &)
   {  return true;   }

//   bool timed_lock(system_time const & abs_time)
//   {return true;}
   template<typename TimeDuration>
   bool timed_lock(TimeDuration const & relative_time)
   {return true;}


   //!Simulates a mutex unlock() operation.
   //!Empty function.
   void unlock(){}

   //!Simulates a mutex lock_share() operation.
   //!Empty function.
   void lock_shared(){}

   //!Simulates a mutex try_lock_share() operation.
   //!Equivalent to "return true;"
   bool try_lock_shared()
   {  return true;   }

   //!Simulates a mutex timed_lock_share() operation.
   //!Equivalent to "return true;"
   bool timed_lock_shared(const boost::posix_time::ptime &)
   {  return true;   }

   //!Simulates a mutex unlock_share() operation.
   //!Empty function.
   void unlock_shared(){}

   //!Simulates a mutex lock_upgrade() operation.
   //!Empty function.
   void lock_upgrade(){}

   //!Simulates a mutex try_lock_upgrade() operation.
   //!Equivalent to "return true;"
   bool try_lock_upgrade()
   {  return true;   }

   //!Simulates a mutex timed_lock_upgrade() operation.
   //!Equivalent to "return true;"
   bool timed_lock_upgrade(boost::posix_time::ptime const &)
   {  return true;   }

   //!Simulates a mutex unlock_upgrade() operation.
   //!Empty function.
   void unlock_upgrade(){}

   //!Simulates unlock_and_lock_upgrade().
   //!Empty function.
   void unlock_and_lock_upgrade(){}

   //!Simulates unlock_and_lock_share().
   //!Empty function.
   void unlock_and_lock_shared(){}

   //!Simulates unlock_upgrade_and_lock_share().
   //!Empty function.
   void unlock_upgrade_and_lock_shared(){}

   //Promotions

   //!Simulates unlock_upgrade_and_lock().
   //!Empty function.
   void unlock_upgrade_and_lock(){}

   //!Simulates try_unlock_upgrade_and_lock().
   //!Equivalent to "return true;"
   bool try_unlock_upgrade_and_lock()
   {  return true;   }

   //!Simulates timed_unlock_upgrade_and_lock().
   //!Equivalent to "return true;"
   bool timed_unlock_upgrade_and_lock(const boost::posix_time::ptime &)
   {  return true;   }

   //!Simulates try_unlock_share_and_lock().
   //!Equivalent to "return true;"
   bool try_unlock_share_and_lock()
   {  return true;   }

   //!Simulates try_unlock_share_and_lock_upgrade().
   //!Equivalent to "return true;"
   bool try_unlock_share_and_lock_upgrade()
   {  return true;   }
};


}
}

#endif   //BOOST_SYNCHRO_NULL_MUTEX_HPP
