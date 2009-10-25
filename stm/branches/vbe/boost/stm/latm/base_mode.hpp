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

namespace boost { namespace stm { namespace latm {

    class base_mode {
    public:
        virtual latm_type const protection() =0;
        virtual std::string const protection_str();

        virtual bool doing_full_lock_protection();
        virtual bool doing_tm_lock_protection();
        virtual bool doing_tx_lock_protection();
        
        virtual void lock(mutex_type& lock);
        virtual bool try_lock(mutex_type& lock);
        virtual void unlock(mutex_type& lock);
    };
    
}}}

#endif // BOOST_STM_LATM_DYNAMIC__HPP


