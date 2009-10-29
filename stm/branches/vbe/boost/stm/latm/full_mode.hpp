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

#ifndef BOOST_STM_LATM_FULL_MIXIN__HPP
#define BOOST_STM_LATM_FULL_MIXIN__HPP

//-----------------------------------------------------------------------------
#include <boost/stm/detail/config.hpp>
#include <boost/stm/latm/datatypes.hpp>
#include <boost/stm/latm/base_mode.hpp>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
namespace boost { namespace stm { namespace latm {

//-----------------------------------------------------------------------------
template <typename Base=static_mode>
class full_mode : public base_mode<Base> {
public:

    ~full_mode() {}
//-----------------------------------------------------------------------------
    latm_type const protection() { return eFullLatmProtection; }
    std::string const protection_str() {
        return "full_protect";
    }

    bool doing_full_lock_protection() {
        return true;
    }

    bool doing_tm_lock_protection() {
        return false;
    }

    bool doing_tx_lock_protection() {
        return false;
    }

//-----------------------------------------------------------------------------
    bool can_go_inflight()
    {
        // allow transactions
        // to start only if no locks are obtained or the only lock that
        // is obtained is on this_thread::get_id()
        for (latm::mutex_thread_id_map::iterator j = this->latmLockedLocksOfThreadMap_.begin();
        j != this->latmLockedLocksOfThreadMap_.end(); ++j)
        {
            if (this_thread::get_id() != j->second)
            {
                return false;
            }
        }

        return true;
    }

//-----------------------------------------------------------------------------
    void tm_lock_conflict(latm::mutex_type &lock)
    {
        tm_lock_conflict(&lock);
    }
    void tm_lock_conflict(latm::mutex_type* inLock)
    {
        return ;
    }

//-----------------------------------------------------------------------------
        //int lock(mutex_type& lock);
        //int try_lock(mutex_type& lock);
        //int unlock(mutex_type& lock);

    };

}}}

#endif // BOOST_STM_LATM_FULL_MIXIN__HPP


