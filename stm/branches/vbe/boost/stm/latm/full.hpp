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

    struct static_mode {};
    template <typename Base=static_mode>
    class full_mode : {
    public:
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
        
        int lock(mutex_type& lock);
        int try_lock(mutex_type& lock);
        int unlock(mutex_type& lock);
        
    };
    
}}}

#endif // BOOST_STM_LATM_DYNAMIC__HPP


