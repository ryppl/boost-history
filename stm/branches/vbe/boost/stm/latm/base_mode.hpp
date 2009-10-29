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

#ifndef BOOST_STM_LATM_BASE_MODE__HPP
#define BOOST_STM_LATM_BASE_MODE__HPP

#include <boost/stm/detail/config.hpp>
#include <boost/stm/latm/datatypes.hpp>
#include <boost/stm/datatypes.hpp>
#include <boost/synchro.hpp>

namespace boost { namespace stm { namespace latm {

template <typename Base=static_mode>
class base_mode : public Base {
public:
    latm::mutex_set tmConflictingLocks_;
    latm::mutex_set latmLockedLocks_;
    latm::mutex_thread_id_set_map latmLockedLocksAndThreadIdsMap_;
    latm::mutex_thread_id_map latmLockedLocksOfThreadMap_;

    Mutex latmMutex_;


    latm::mutex_set get_tm_conflicting_locks() { return tmConflictingLocks_; }

    void clear_tm_conflicting_locks()
    {
        synchro::lock_guard<Mutex> lock_l(latmMutex_);
        tmConflictingLocks_.clear();
    }

    void must_be_in_tm_conflicting_lock_set(latm::mutex_type* inLock)
    {
       if (tmConflictingLocks_.find(inLock) == tmConflictingLocks_.end())
       {
          throw "lock not in tx conflict lock set, use add_tm_conflicting_lock";
       }
    }

};

}}}

#endif // BOOST_STM_LATM_DYNAMIC__HPP


