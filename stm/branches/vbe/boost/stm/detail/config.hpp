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

//#define DELAY_INVALIDATION_DOOMED_TXS_UNTIL_COMMIT
//#define LOGGING_COMMITS_AND_ABORTS 1
//#define PERFORMING_VALIDATION 1
#define PERFORMING_LATM 1
#define PERFORMING_COMPOSITION 1
//#define USE_STM_MEMORY_MANAGER 1
#define BUILD_MOVE_SEMANTICS 0
#define USING_TRANSACTION_SPECIFIC_LATM 1
#define USE_BLOOM_FILTER 1
#define PERFORMING_WRITE_BLOOM 1
//#define ALWAYS_ALLOW_ABORT 1
//#define LOGGING_BLOCKS 1
#define USE_SINGLE_THREAD_CONTEXT_MAP 1
//#define BOOST_STM_HAVE_SINGLE_TSS_CONTEXT_MAP 1
#define BOOST_STM_TX_CONTAINS_REFERENCES_TO_TSS_FIELDS 1

// BOOST_STM_CM_STATIC_CONF: configuration manager is static
#define BOOST_STM_CM_STATIC_CONF 1
// BOOST_STM_CM_STATIC_CONF_ExceptAndBackOffOnAbortNoticeCM: configuration manager is ExceptAndBackOffOnAbortNoticeCM
#define BOOST_STM_CM_STATIC_CONF_ExceptAndBackOffOnAbortNoticeCM 1

// BOOST_STM_USE_MEMCOPY: STM uses memcpy insted of the copy constructor
#define BOOST_STM_USE_MEMCOPY 1

//// The cache using uninitialized_copy can use the following memeory manager
////BOOST_STM_CACHE_USE_MALLOC: uses malloc/free
#define BOOST_STM_CACHE_USE_MALLOC  1
//// BOOST_STM_CACHE_USE_MEMORY_MANAGER: uses the class specific memory manager
//#define BOOST_STM_CACHE_USE_MEMORY_MANAGER 1
//// BOOST_STM_CACHE_USE_TSS_MONOTONIC_MEMORY_MANAGER: uses the monotonic storage memory manager
//#define BOOST_STM_CACHE_USE_TSS_MONOTONIC_MEMORY_MANAGER  1

#ifdef USE_BLOOM_FILTER
#define DISABLE_READ_SETS 1
#endif

#define MAP_WRITE_CONTAINER 1
//#define MAP_NEW_CONTAINER 1
//#define MAP_THREAD_MUTEX_CONTAINER 1
#define MAP_THREAD_BOOL_CONTAINER 1
#define USING_SHARED_FORCED_TO_ABORT 1

//#define BOOST_STM_BLOOM_FILTER_USE_DYNAMIC_BITSET 1
//#define BOOST_STM_USE_BOOST_SLEEP 1
//#define BOOST_STM_USE_BOOST_THREAD_ID 1
//#define BOOST_STM_USE_BOOST_MUTEX 1

#define BOOST_STM_NO_PARTIAL_SPECIALIZATION 1

#endif // BOOST_STM_DETAIL_CONFIG_H









