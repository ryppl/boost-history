//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_THREAD_SYNCHROHRONIZATION_FAMILY__HPP
#define BOOST_SYNCHRO_THREAD_SYNCHROHRONIZATION_FAMILY__HPP

#include <boost/synchro/thread/mutex.hpp>
#include <boost/synchro/thread/recursive_mutex.hpp>
#include <boost/synchro/thread/shared_mutex.hpp>
#include <boost/thread/condition_variable.hpp>

namespace boost {
namespace synchro {

//!\file
//!Describes thread synchronization family to use with Sync framework
//!based on boost synchronization objects.
//[thread_synchronization_family
struct thread_synchronization_family
{
   typedef thread_mutex                     mutex_type;
   typedef thread_recursive_mutex           recursive_mutex_type;
   typedef thread_timed_mutex               timed_mutex_type;
   typedef thread_recursive_timed_mutex     recursive_timed_mutex_type;
   typedef thread_shared_mutex              shared_mutex_type;
   typedef boost::condition_variable        condition_type;
   typedef boost::condition_variable_any    condition_type_any;
};
//]


}
}

#endif
