//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_PROCESS_SYNCHROHRONIZATION_FAMILY__HPP
#define BOOST_SYNCHRO_PROCESS_SYNCHROHRONIZATION_FAMILY__HPP

#include <boost/synchro/process/mutex.hpp>
#include <boost/synchro/process/recursive_mutex.hpp>
#include <boost/synchro/process/upgradable_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>

namespace boost {
namespace synchro {


//!Describes interprocess_mutex family to use with Sync framework
//!based on boost::interprocess synchronization objects.
struct process_synchronization_family
{
   typedef boost::synchro::interprocess_mutex              mutex_type;
   typedef boost::synchro::interprocess_recursive_mutex    recursive_mutex_type;
   typedef boost::synchro::interprocess_mutex                 timed_mutex_type;
   typedef boost::synchro::interprocess_recursive_mutex    recursive_timed_mutex_type;
   typedef boost::synchro::interprocess_upgradable_mutex    shared_mutex_type;
   typedef boost::interprocess::interprocess_condition             condition_type;
   typedef boost::interprocess::interprocess_condition             condition_any_type;
};

}
}

#endif
