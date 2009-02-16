//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_NULL_CONDITION__HPP
#define BOOST_SYNCHRO_NULL_CONDITION__HPP

//!\file
//!Describes null_mutex class
#include <boost/synchro/null_mutex.hpp>
#include <boost/thread/thread_time.hpp>
#include <boost/synchro/detail/deleted_functions.hpp>

namespace boost {


namespace synchro {

class null_condition
{
public:
   //Non-copyable
    BOOST_COPY_CONSTRUCTOR_DELETE(null_condition) /*< disable copy construction >*/
    BOOST_COPY_ASSIGNEMENT_DELETE(null_condition) /*< disable copy asignement >*/

   //!Constructs a null_condition. On error throws interprocess_exception.
   null_condition(){}

   //!Destroys *this
   //!liberating system resources.
   ~null_condition(){}

   //!If there is a thread waiting on *this, change that
   //!thread's state to ready. Otherwise there is no effect.
   void notify_one(){}

   //!Change the state of all threads waiting on *this to ready.
   //!If there are no waiting threads, notify_all() has no effect.
   void notify_all(){}

   //!Releases the lock on the interprocess_mutex object associated with lock, blocks
   //!the current thread of execution until readied by a call to
   //!this->notify_one() or this->notify_all(), and then reacquires the lock.
   template <typename L>
   void wait(L& lock) {}

   //!The same as:
   //!while (!pred()) wait(lock)
   template <typename L, typename Pr>
   void wait(L& lock, Pr pred) {}

   //!Releases the lock on the interprocess_mutex object associated with lock, blocks
   //!the current thread of execution until readied by a call to
   //!this->notify_one() or this->notify_all(), or until time abs_time is reached,
   //!and then reacquires the lock.
   //!Returns: false if time abs_time is reached, otherwise true.

   template <typename L>
   bool timed_wait(L& lock, const boost::system_time &abs_time)
   { return true; }

   //!The same as:   while (!pred()) {
   //!                  if (!timed_wait(lock, abs_time)) return pred();
   //!               } return true;
   template <typename L, typename Pr>
   bool timed_wait(L& lock, const boost::system_time &abs_time, Pr pred)
   { return true; }

};
template <>
struct best_condition<null_mutex> {
    typedef null_condition type;
};

}
}

#endif // __BOOST_SYNCHRO_NULL_CONDITION__HPP
