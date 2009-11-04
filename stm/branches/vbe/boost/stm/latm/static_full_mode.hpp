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

#ifndef BOOST_STM_LATM_STATIC_TX__HPP
#define BOOST_STM_LATM_STATIC_TX__HPP

#include <boost/stm/detail/config.hpp>
#include <boost/stm/latm/datatypes.hpp>
#include <boost/stm/latm/full_mode.hpp>

namespace boost { namespace stm { namespace latm {
    typedef full_mode<> latm_mode;
    struct mode {
        static latm_mode instance_;      
    };
    inline void do_full_lock_protection() {
    }
    inline void do_tm_lock_protection() {
    }
    inline void do_tx_lock_protection() {
    }
    inline latm_mode& instance() {
        return mode::instance_;
    }
    
}}}

#endif // BOOST_STM_LATM_STATIC_TX__HPP


