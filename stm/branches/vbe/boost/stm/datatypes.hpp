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

#ifndef BOOST_STM_DATATYPES__HPP
#define BOOST_STM_DATATYPES__HPP

//-----------------------------------------------------------------------------
#include <pthread.h>
//-----------------------------------------------------------------------------
#include <cstddef>
//-----------------------------------------------------------------------------
#include <boost/stm/detail/config.hpp>
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
namespace boost { namespace stm {

    //-----------------------------------------------------------------------------
    // The possible states a transaction can be in:
    //
    // e_no_state    - initial state of transaction.
    // e_aborted     - aborted transaction.
    // e_committed   - transaction has committed.
    // e_hand_off    - transaction memory has been handed off to another
    //                 transaction. This is the vital state for in-flight
    //                 transactions which are composed.
    // e_in_flight   - transaction currently in process.
    //-----------------------------------------------------------------------------
    enum transaction_state
    {
       e_no_state = -1, // no state is -1
       e_aborted,      // ensure aborted = 0
       e_committed,    // committed is positive
       e_hand_off,      // so is handoff in case bool conversion
       e_in_flight
    };

    //-----------------------------------------------------------------------------
    enum transaction_type
    {
        kMinIrrevocableType = 0,
        eNormalTx = kMinIrrevocableType,
        eIrrevocableTx,
        eIrrevocableAndIsolatedTx,
        kMaxIrrevocableType
    };
    typedef transaction_type TxType;

    //-----------------------------------------------------------------------------
    enum latm_type
    {
        kMinLatmType = 0,
        eFullLatmProtection = kMinLatmType,
        eTmConflictingLockLatmProtection,
        eTxConflictingLockLatmProtection,
        kMaxLatmType
    };
    typedef latm_type LatmType;

//-----------------------------------------------------------------------------

    typedef std::size_t new_memory_t;
    
//-----------------------------------------------------------------------------

    typedef std::size_t priority_t;

//-----------------------------------------------------------------------------

    typedef std::size_t clock_t;
    typedef std::size_t milliseconds_t;

//-----------------------------------------------------------------------------

    typedef std::size_t version_t;

//-----------------------------------------------------------------------------
    
#ifndef BOOST_STM_USE_BOOST_THREAD_ID
    typedef std::size_t thread_id_t;
    namespace detail {
    thread_id_t const kInvalidThread = 0xffffffff;
    }
    inline thread_id_t invalid_thread_id() {
        return detail::kInvalidThread;
    }
    inline bool is_invalid_thread_id(thread_id_t t) {
        return detail::kInvalidThread==t;
    }   
    namespace this_thread {
        inline thread_id_t get_id() {
#ifdef WINOS
#pragma warning (disable:4786)
            return (thread_id_t)pthread_self().p;
#else // !WINOS
            return (thread_id_t)pthread_self();
#endif // WINOS
        }
    }
#else // BOOST_STM_USE_BOOST_THREAD_ID
    typedef boost::thread::id thread_id_t;
    thread_id_t const kInvalidThread;
    inline thread_id_t invalid_thread_id() {
        return thread_id_t();
    }
    
    inline bool is_invalid_thread_id(thread_id_t t) {
        return thread_id_t()==t;
    }   
    namespace this_thread {
        inline thread_id_t get_id() {
            return boost::this_thread::get_id();
        }
    }
#endif // BOOST_STM_USE_BOOST_THREAD_ID
    
    
#ifndef BOOST_STM_USE_BOOST_SLEEP
#ifdef WINOS
#define SLEEP(x) Sleep(x)
#else
#include <unistd.h>
#define SLEEP(x) usleep(x*1000)
#endif
#else
#define SLEEP(x) boost::this_thread::sleep(boost::posix_time::milliseconds(x))
#endif
    
}}

#ifndef BOOST_STM_USE_BOOST_MUTEX
    typedef pthread_mutex_t Mutex;
    //typedef pthread_mutex_t PLOCK;
#else
    typedef boost::mutex Mutex;
#endif


#endif // BOOST_STM_DATATYPES__HPP


