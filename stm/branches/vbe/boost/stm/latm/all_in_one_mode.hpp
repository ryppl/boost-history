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

#ifndef BOOST_STM_LATM_ALL_IN_ONE_MODE__HPP
#define BOOST_STM_LATM_ALL_IN_ONE_MODE__HPP

#include <boost/stm/detail/config.hpp>
#include <boost/stm/latm/datatypes.hpp>
#include <boost/stm/latm/base_mode.hpp>

//-----------------------------------------------------------------------------
namespace boost { namespace stm { namespace latm {

//-----------------------------------------------------------------------------
class all_in_one_mode : public base_mode<> {
    latm_type eLatmType_;
public:
//-----------------------------------------------------------------------------
    all_in_one_mode() : eLatmType_(eFullLatmProtection) {}
    latm_type const protection() { return eLatmType_; }
    std::string const protection_str() {
        switch (eLatmType_)
        {
        case eFullLatmProtection:
            return "full_protect";
        case eTmConflictingLockLatmProtection:
            return "tm_protect";
        case eTxConflictingLockLatmProtection:
            return "tx_protect";
        default:
            throw "invalid LATM type";
        }
    }
    void do_full_lock_protection_i() {
        eLatmType_ = eFullLatmProtection;
    }
    void do_tm_lock_protection_i() {
        eLatmType_ = eTmConflictingLockLatmProtection;
    }
    void do_tx_lock_protection_i() {
        eLatmType_ = eTxConflictingLockLatmProtection;
    }

    bool doing_full_lock_protection() {
        return eFullLatmProtection == eLatmType_;
    }
    bool doing_tm_lock_protection() {
        return eTmConflictingLockLatmProtection == eLatmType_;
    }
    bool doing_tx_lock_protection() {
        return eTxConflictingLockLatmProtection == eLatmType_;
    }

//-----------------------------------------------------------------------------
    bool can_go_inflight()
    {
       // if we're doing full lock protection, allow transactions
       // to start only if no locks are obtained or the only lock that
       // is obtained is on this_thread::get_id()
        if (doing_full_lock_protection())
        {
            for (latm::mutex_thread_id_map::iterator j = latmLockedLocksOfThreadMap_.begin();
                    j != latmLockedLocksOfThreadMap_.end(); ++j)
            {
                if (this_thread::get_id() != j->second)
                {
                    return false;
                }
            }
        }

        // if we're doing tm lock protection, allow transactions
        // to start only if
        else if (doing_tm_lock_protection())
        {
            for (latm::mutex_set::iterator i = tmConflictingLocks_.begin(); i != tmConflictingLocks_.end(); ++i)
            {
                // if one of your conflicting locks is currently locked ...
                if (latmLockedLocks_.end() != latmLockedLocks_.find(*i))
                {
                    // if it is locked by our thread, it is ok ... otherwise it is not
                    latm::mutex_thread_id_map::iterator j = latmLockedLocksOfThreadMap_.find(*i);

                    if (j != latmLockedLocksOfThreadMap_.end() &&
                        this_thread::get_id() != j->second)
                    {
                        return false;
                    }
                }
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
        if (!this->doing_tm_lock_protection()) return;

        synchro::lock_guard<Mutex> lock_l(latmMutex_);

       //-------------------------------------------------------------------------
       // insert can throw an exception
       //-------------------------------------------------------------------------
        tmConflictingLocks_.insert(inLock);
    }

};

typedef all_in_one_mode latm_mode;

struct mode {
    static latm_mode instance_;
};

inline all_in_one_mode& instance() {
    return mode::instance_;
}
inline void do_full_lock_protection() {
    instance().do_full_lock_protection_i();
}
inline void do_tm_lock_protection() {
    instance().do_tm_lock_protection_i();
}
inline void do_tx_lock_protection() {
    instance().do_tx_lock_protection_i();
}

}}}

#endif // BOOST_STM_LATM_ALL_IN_ONE_MODE__HPP


