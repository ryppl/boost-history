//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_NULL_CONDITION_HPP
#define BOOST_SYNCHRO_NULL_CONDITION_HPP


#include <boost/synchro/lockable_traits.hpp>
#include <boost/thread/thread_time.hpp>
#include <boost/synchro/detail/deleted_functions.hpp>
#include <boost/chrono/chrono.hpp>

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
public:
    typedef null_condition best_condition_type;
    typedef null_condition best_condition_any_type;

    BOOST_COPY_CONSTRUCTOR_DELETE(null_mutex) /*< disable copy construction >*/
    BOOST_COPY_ASSIGNEMENT_DELETE(null_mutex) /*< disable copy asignement >*/

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
   bool try_lock_until(const boost::posix_time::ptime &)
   {  return true;   }

   //!Simulates a mutex try_lock_for() operation.
   //!Equivalent to "return true;"
   template<typename TimeDuration>
   bool try_lock_for(TimeDuration const & relative_time)
   {return true;}

   template<typename Clock, typename Duration>
   bool try_lock_until(chrono::time_point<Clock, Duration> const & abs_time)
   {return true;}

   template<typename Rep, typename Period>
   bool try_lock_for(chrono::duration<Rep, Period> const & rel_time)
   {return true;}

   //!Simulates a mutex timed_lock() operation.
   //!Equivalent to "return true;"
   bool lock_until(const boost::posix_time::ptime &)
   {  return true;   }

   //!Simulates a mutex lock_for() operation.
   //!Equivalent to "return true;"
   template<typename TimeDuration>
   bool lock_for(TimeDuration const & relative_time)
   {return true;}

   template<typename Clock, typename Duration>
   void lock_until(chrono::time_point<Clock, Duration> const & abs_time)
   {}

   template<typename Rep, typename Period>
   void lock_for(chrono::duration<Rep, Period> const & rel_time)
   {}

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

   //!Simulates a mutex try_lock_shared_until() operation.
   //!Equivalent to "return true;"
   bool try_lock_shared_until(const boost::posix_time::ptime &)
   {  return true;   }

   //!Simulates a mutex try_lock_shared_for() operation.
   //!Equivalent to "return true;"
   template<typename TimeDuration>
   bool try_lock_shared_for(const TimeDuration &)
   {  return true;   }

   template<typename Clock, typename Duration>
   bool try_lock_shared_until(chrono::time_point<Clock, Duration> const & relative_time)
   {return true;}

   template<typename Rep, typename Period>
   bool try_lock_shared_for(chrono::duration<Rep, Period> const & relative_time)
   {return true;}
   
   //!Simulates a mutex lock_shared_until() operation.
   //!Equivalent to "return true;"
   bool lock_shared_until(const boost::posix_time::ptime &)
   {  return true;   }

   //!Simulates a mutex lock_shared_for() operation.
   //!Equivalent to "return true;"
   template<typename TimeDuration>
   bool lock_shared_for(const TimeDuration &)
   {  return true;   }

   template<typename Clock, typename Duration>
   void lock_shared_until(chrono::time_point<Clock, Duration> const & relative_time)
   {}
   template<typename Rep, typename Period>
   void lock_shared_for(chrono::duration<Rep, Period> const & relative_time)
   {}
   
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

   //!Simulates a mutex try_lock_upgrade_until() operation.
   //!Equivalent to "return true;"
   bool try_lock_upgrade_until(boost::posix_time::ptime const &)
   {  return true;   }

   template<typename Clock, typename Duration>
   bool try_lock_upgrade_until(chrono::time_point<Clock, Duration> const & relative_time)
   {return true;}

   template<typename Rep, typename Period>
   bool try_lock_upgrade_for(chrono::duration<Rep, Period> const & relative_time)
   {return true;}
   
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

   //!Simulates unlock_upgrade_and_lock_until().
   //!Equivalent to "return true;"
   bool unlock_upgrade_and_lock_until(const boost::posix_time::ptime &)
   {  return true;   }

   template<typename Clock, typename Duration>
   bool unlock_upgrade_and_lock_until(chrono::time_point<Clock, Duration> const & relative_time)
   {return true;}

   template<typename Rep, typename Period>
   bool unlock_upgrade_and_lock_for(chrono::duration<Rep, Period> const & relative_time)
   {return true;}
   
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
