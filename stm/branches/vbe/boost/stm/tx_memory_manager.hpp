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

#ifndef BOOST_STM_TX_MM__HPP
#define BOOST_STM_TX_MM__HPP

#if defined(BOOST_STM_USE_SPECIFIC_TRANSACTION_MEMORY_MANAGER)

#include <boost/stm/detail/config.hpp>

#if defined(BOOST_STM_CACHE_USE_MEMORY_MANAGER) && defined (USE_STM_MEMORY_MANAGER)
#include <boost/stm/tx_memory_managers/shared_mm.hpp>
#elif defined(BOOST_STM_CACHE_USE_MALLOC)
#include <boost/stm/tx_memory_managers/malloc_mm.hpp>
#elif defined(BOOST_STM_CACHE_USE_TSS_MONOTONIC_MEMORY_MANAGER)
#include <boost/stm/tx_memory_managers/tss_mm.hpp>
#else
#error "BOOST_STM_CACHE_USE_MEMORY_MANAGER, BOOST_STM_CACHE_USE_MALLOC or BOOST_STM_CACHE_USE_TSS_MONOTONIC_MEMORY_MANAGER must be defined"
#endif

//-----------------------------------------------------------------------------
namespace boost { namespace stm {

    #if defined(BOOST_STM_CACHE_USE_MEMORY_MANAGER) && defined (USE_STM_MEMORY_MANAGER)
    typedef shared_mm tx_memory_manager;       
    #elif defined(BOOST_STM_CACHE_USE_MALLOC)
    typedef malloc_mm tx_memory_manager;    
    #elif defined(BOOST_STM_CACHE_USE_TSS_MONOTONIC_MEMORY_MANAGER)
    typedef tss_mm tx_memory_manager;    
    #else
    #error "BOOST_STM_CACHE_USE_MEMORY_MANAGER, BOOST_STM_CACHE_USE_MALLOC or BOOST_STM_CACHE_USE_TSS_MONOTONIC_MEMORY_MANAGER must be defined"
    #endif

    template <class T> T* cache_allocate(transaction& t) {
        return tx_memory_manager::allocate<T>(t);
    }

    template <class T>
    void cache_deallocate(T* ptr) {
        tx_memory_manager::deallocate(ptr);
    }

} // stm  namespace
} // boost namespace

#endif

//-----------------------------------------------------------------------------
#endif // BOOST_STM_TX_MM__HPP


