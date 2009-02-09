//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_NULL_SYNCHROHRONIZATION_FAMILY__HPP
#define BOOST_SYNCHRO_NULL_SYNCHROHRONIZATION_FAMILY__HPP

#include "boost/synchro/null_mutex.hpp"
#include "boost/synchro/null_condition.hpp"

namespace boost {
namespace synchro {

//!Describes interprocess_mutex family to use with Sync frameworks
//!based on null operation synchronization objects.
struct null_synchronization_family
{
   typedef boost::synchro::null_mutex  mutex_type;
   typedef boost::synchro::null_mutex  recursive_mutex_type;
   typedef boost::synchro::null_mutex    timed_mutex_type;
   typedef boost::synchro::null_mutex    recursive_timed_mutex_type;
   typedef boost::synchro::null_mutex    shared_mutex_type;
   typedef boost::synchro::null_condition   condition_type;
   typedef boost::synchro::null_condition   condition_any_type;
};

}
}

#endif
