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

#ifndef BOOST_STM_LATM_ABSTRACT_MODE__HPP
#define BOOST_STM_LATM_ABSTRACT_MODE__HPP

#include <boost/stm/detail/config.hpp>
#include <boost/stm/latm/datatypes.hpp>

namespace boost { namespace stm { namespace latm {

    class abstract_mode {
    public:
        virtual latm_type const protection() =0;
        virtual std::string const protection_str()=0;

        virtual bool doing_full_lock_protection()=0;
        virtual bool doing_tm_lock_protection()=0;
        virtual bool doing_tx_lock_protection()=0;

        virtual bool can_go_inflight()=0;
    
        virtual void tm_lock_conflict(latm::mutex_type &lock)=0;
        virtual void tm_lock_conflict(latm::mutex_type* inLock)=0;
    
        //virtual void lock(mutex_type& lock);
        //virtual bool try_lock(mutex_type& lock);
        //virtual void unlock(mutex_type& lock);
        virtual ~abstract_mode() {}
    };
   
}}}

#endif // BOOST_STM_LATM_DYNAMIC__HPP


