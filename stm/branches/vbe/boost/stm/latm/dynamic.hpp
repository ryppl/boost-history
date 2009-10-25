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

#ifndef BOOST_STM_LATM_DYNAMIC__HPP
#define BOOST_STM_LATM_DYNAMIC__HPP

#include <boost/stm/detail/config.hpp>
#include <boost/stm/latm/datatypes.hpp>

namespace boost { namespace stm { namespace latm {
    #if 0
    struct poly {
        static full_mode<base_mode> full_;
        static tm_mode<base_mode> tm_;
        static tx_mode<base_mode> tx_;
        static base_mode* more_;      
    };
    void do_full_lock_protection() {
        poly::mode_ = &full_;
    }
    void do_tm_lock_protection() {
        poly::mode_ = &tm_;
    }
    void do_tx_lock_protection() {
        poly::mode_ = &tx_;
    }
    inline base_mode& instance() {
        return poly::mode_;
    }
    
    #endif
    class dynamic {
        latm_type eLatmType_;
    public:
        dynamic() : eLatmType_(eFullLatmProtection) {}
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
        
#if BOOST_STM_LATM_GENERIC    
       template <typename Lockable>
       void lock(Lockable& lock);
       template <typename Lockable>
       bool try_lock(Lockable& lock);
       template <typename Lockable>
       void unlock(Lockable& lock);
#else    
       int lock(mutex_type& lock);
       int try_lock(mutex_type& lock);
       int unlock(mutex_type& lock);
#endif    
        
    };
    inline dynamic& instance() {
        static dynamic ctx;
        return ctx;
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

#endif // BOOST_STM_LATM_DYNAMIC__HPP


