//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Justin E. Gottchlich 2009. 
// (C) Copyright Vicente J. Botet Escriba 2009. 
// Distributed under the Boost
// Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or 
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/stm for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_STM_LATM_DATATYPES__HPP
#define BOOST_STM_LATM_DATATYPES__HPP

#include <boost/stm/detail/config.hpp>
#include <boost/stm/datatypes.hpp>
#include <set>
#include <map>

namespace boost { namespace stm { namespace latm {
    struct static_mode {};
#if BOOST_STM_LATM_GENERIC    
   //typedef synchro::poly::mutex mutex_type;
#else    
   typedef Mutex mutex_type;
#endif    

    typedef std::set<thread_id_t> thread_id_set;

    typedef std::set<mutex_type*> mutex_set;
    //typedef std::map<thread_id_t, mutex_set* > thread_mutex_set_map;
    typedef std::map<thread_id_t, mutex_set* > thread_id_mutex_set_map;
    
    //typedef std::map<latm::mutex_type*, thread_id_set > MutexThreadSetMap;   
    typedef std::map<mutex_type*, thread_id_set > mutex_thread_id_set_map;
    
    //typedef std::map<latm::mutex_type*, thread_id_t> MutexThreadMap;
    typedef std::map<mutex_type*, thread_id_t> mutex_thread_id_map;
    
}}}

#endif // BOOST_STM_LATM_DATATYPES__HPP


