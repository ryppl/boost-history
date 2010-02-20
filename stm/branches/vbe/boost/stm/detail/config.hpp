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

#ifndef BOOST_STM_DETAIL_CONFIG_H
#define BOOST_STM_DETAIL_CONFIG_H

#ifdef WIN32
#define WINOS
#elif WIN64
#define WINOS
#else
#define UNIX
#endif


///////////////////////////////////////////////////////////////////////////////
// LOGGING
//#define LOGGING_COMMITS_AND_ABORTS 1
//#define LOGGING_BLOCKS 1


///////////////////////////////////////////////////////////////////////////////
//#define DELAY_INVALIDATION_DOOMED_TXS_UNTIL_COMMIT
//#define PERFORMING_VALIDATION 1
#define PERFORMING_COMPOSITION 1
#define PERFORMING_LATM 1
#define USING_TRANSACTION_SPECIFIC_LATM 1
//#define ALWAYS_ALLOW_ABORT 1
#define USING_SHARED_FORCED_TO_ABORT 1


///////////////////////////////////////////////////////////////////////////////
// The cache can be structured in different ways. Define ony one of
//      USE_SINGLE_THREAD_CONTEXT_MAP: use of a common tx_context as TSS
//      BOOST_STM_HAVE_SINGLE_TSS_CONTEXT_MAP: use of common tss_context as TSS
//      OTHER: each TSS data has its specific TSS

#define USE_SINGLE_THREAD_CONTEXT_MAP 1
//#define BOOST_STM_HAVE_SINGLE_TSS_CONTEXT_MAP 1


///////////////////////////////////////////////////////////////////////////////
// When USE_SINGLE_THREAD_CONTEXT_MAP or BOOST_STM_HAVE_SINGLE_TSS_CONTEXT_MAP are defined you can want to store references to the TSS or not
//      BOOST_STM_TX_CONTAINS_REFERENCES_TO_TSS_FIELDS if you want to add these references

#define BOOST_STM_TX_CONTAINS_REFERENCES_TO_TSS_FIELDS 1

///////////////////////////////////////////////////////////////////////////////
// Define only one of
//      BOOST_STM_CM_STATIC_CONF when you want a staticacly configured CM
//      BOOST_STM_CM_DYNAMIC_CONF when you want a polymorphic CM configured at run time

//#define BOOST_STM_CM_STATIC_CONF 1
//#define BOOST_STM_CM_DYNAMIC_CONF 1

// Define only one of
//      BOOST_STM_CM_STATIC_CONF_except_and_back_off_on_abort_notice_cm when configuration manager is except_and_back_off_on_abort_notice_cm

#define BOOST_STM_CM_STATIC_CONF <boost/stm/contention_managers/except_and_back_off_on_abort_notice_cm.hpp>
//#define BOOST_STM_CM_STATIC_CONF_except_and_back_off_on_abort_notice_cm 1

#define BOOST_STM_CM_STATIC_INITIAL_SLEEP_TIME 0
#define BOOST_STM_CM_STATIC_SLEEP_INCREASE 0
#define BOOST_STM_CM_STATIC_MAX_INCREASES 0

///////////////////////////////////////////////////////////////////////////////
// OPTIMIZATIONS: MEMORY MANAGERS
// Define only one of
//// BOOST_STM_USE_DEFAULT_MEMORY_MANAGER: uses the global memory manager
//// BOOST_STM_USE_GLOBAL_MEMORY_MANAGER: uses the global memory manager
//// BOOST_STM_USE_CLASS_MEMORY_MANAGER: uses the class specific memory manager

//#define BOOST_STM_USE_DEFAULT_MEMORY_MANAGER 1
//#define BOOST_STM_USE_GLOBAL_MEMORY_MANAGER 1
//#define BOOST_STM_USE_CLASS_MEMORY_MANAGER 1

#if (defined(BOOST_STM_USE_DEFAULT_MEMORY_MANAGER) && defined(BOOST_STM_USE_GLOBAL_MEMORY_MANAGER)) || \
    (defined(BOOST_STM_USE_GLOBAL_MEMORY_MANAGER) && defined(BOOST_STM_USE_CLASS_MEMORY_MANAGER)) || \
    (defined(BOOST_STM_USE_CLASS_MEMORY_MANAGER) && defined(BOOST_STM_USE_DEFAULT_MEMORY_MANAGER))
#error only one of BOOST_STM_USE_DEFAULT_MEMORY_MANAGER, BOOST_STM_USE_GLOBAL_MEMORY_MANAGER or BOOST_STM_USE_CLASS_MEMORY_MANAGER can be defined
#endif

// Define BOOST_STM_USE_MEMORY_MANAGER if you use BOOST_STM_USE_GLOBAL_MEMORY_MANAGER or BOOST_STM_USE_CLASS_MEMORY_MANAGER
#if defined(BOOST_STM_USE_GLOBAL_MEMORY_MANAGER) || defined(BOOST_STM_USE_CLASS_MEMORY_MANAGER)
#define BOOST_STM_USE_MEMORY_MANAGER 1
#define USE_STM_MEMORY_MANAGER 1
#endif

///////////////////////////////////////////////////////////////////////////////
// BOOST_STM_USE_SPECIFIC_TRANSACTION_MEMORY_MANAGER
//// The cache using BOOST_STM_USE_SPECIFIC_TRANSACTION_MEMORY_MANAGER can use the following memory managers
//// BOOST_STM_CACHE_USE_MALLOC: uses malloc/free
//// BOOST_STM_CACHE_USE_GLOBAL_MEMORY_MANAGER: uses the global memory manager
//// BOOST_STM_CACHE_USE_CLASS_MEMORY_MANAGER: uses the class specific memory manager
//// BOOST_STM_CACHE_USE_TSS_MONOTONIC_MEMORY_MANAGER: uses the monotonic storage memory manager

#define BOOST_STM_CACHE_USE_MALLOC  1
#if defined(BOOST_STM_USE_GLOBAL_MEMORY_MANAGER)
//#define BOOST_STM_CACHE_USE_MEMORY_MANAGER 1
#endif
//#define BOOST_STM_CACHE_USE_CLASS_MEMORY_MANAGER 1
#if defined(USE_SINGLE_THREAD_CONTEXT_MAP)
//#define BOOST_STM_CACHE_USE_TSS_MONOTONIC_MEMORY_MANAGER  1
#endif

#if defined(BOOST_STM_CACHE_USE_MALLOC) || defined(BOOST_STM_CACHE_USE_MEMORY_MANAGER) || defined(BOOST_STM_CACHE_USE_TSS_MONOTONIC_MEMORY_MANAGER)
#define BOOST_STM_USE_SPECIFIC_TRANSACTION_MEMORY_MANAGER 1
#endif

///////////////////////////////////////////////////////////////////////////////
// OPTIMIZATIONS: BLOOM FILTERS
///////////////////////////////////////////////////////////////////////////////
// define USE_BLOOM_FILTER if you want STM uses Bloom filters for read set
#define USE_BLOOM_FILTER 1
///////////////////////////////////////////////////////////////////////////////
// define PERFORMING_WRITE_BLOOM if you want STM uses Bloom filters for write set
#define PERFORMING_WRITE_BLOOM 1

#ifdef USE_BLOOM_FILTER
#define DISABLE_READ_SETS 1
#endif

///////////////////////////////////////////////////////////////////////////////
// OPTIMIZATIONS: MAPS AND SETS
///////////////////////////////////////////////////////////////////////////////
// define MAP_WRITE_CONTAINER if you want STM uses flat map for write map
#define MAP_WRITE_CONTAINER 1
///////////////////////////////////////////////////////////////////////////////
// define MAP_NEW_CONTAINER if you want STM uses flat map for new set
//#define MAP_NEW_CONTAINER 1
///////////////////////////////////////////////////////////////////////////////
// define MAP_THREAD_MUTEX_CONTAINER if you want STM uses flat map for thread mutex set
//#define MAP_THREAD_MUTEX_CONTAINER 1
///////////////////////////////////////////////////////////////////////////////
// define MAP_THREAD_BOOL_CONTAINER if you want STM uses flat map for thread bool set
#define MAP_THREAD_BOOL_CONTAINER 1

///////////////////////////////////////////////////////////////////////////////
// Define BOOST_STM_USE_BOOST when you want STM uses BOOST internally

#define BOOST_STM_USE_BOOST 1

#ifdef BOOST_STM_USE_BOOST
//#define BOOST_STM_BLOOM_FILTER_USE_DYNAMIC_BITSET 1
//#define BOOST_STM_USE_BOOST_SLEEP 1
//#define BOOST_STM_USE_BOOST_THREAD_ID 1
//#define BOOST_STM_USE_BOOST_MUTEX 1
//#define BOOST_STM_T_USE_BOOST_MUTEX 1
#endif

///////////////////////////////////////////////////////////////////////////////
// COMPILERS DEPENDENT
///////////////////////////////////////////////////////////////////////////////
#define BOOST_STM_NO_PARTIAL_SPECIALIZATION 1
///////////////////////////////////////////////////////////////////////////////
#define BUILD_MOVE_SEMANTICS 0

///////////////////////////////////////////////////////////////////////////////
// Define BOOST_STM_ALLOWS_EMBEDEEDS when you want embeed TO
#define BOOST_STM_ALLOWS_EMBEDEEDS 1

///////////////////////////////////////////////////////////////////////////////
// Define BOOST_STM_ALLOWS_DELETERS when you want STM manage with others deleters than delete
#define BOOST_STM_ALLOWS_DELETERS 1

///////////////////////////////////////////////////////////////////////////////
// BOOST_STM_LATM_STATIC_MODE
// BOOST_STM_LATM_STATIC_FULL_MODE
// BOOST_STM_LATM_STATIC_TM_MODE
// BOOST_STM_LATM_STATIC_TX_MODE
// BOOST_STM_LATM_DYNAMIC_MODE

//#define BOOST_STM_LATM_STATIC_FULL_MODE 1
//#define BOOST_STM_LATM_STATIC_TM_MODE 1
//#define BOOST_STM_LATM_STATIC_TX_MODE 1
//#define BOOST_STM_LATM_DYNAMIC_MODE 1

#if defined(BOOST_STM_LATM_STATIC_FULL_MODE) || defined(BOOST_STM_LATM_STATIC_TM_MODE) || defined(BOOST_STM_LATM_STATIC_TX_MODE)
#define BOOST_STM_LATM_STATIC_MODE 1
#endif

///////////////////////////////////////////////////////////////////////////////
// Define BOOST_STM_LATM_GENERIC when you want Boost.STM to manage with arbitrary mutex

#define BOOST_STM_LATM_GENERIC 1

///////////////////////////////////////////////////////////////////////////////
#endif // BOOST_STM_DETAIL_CONFIG_H









