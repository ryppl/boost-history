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
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#include <boost/stm/detail/config.hpp>
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
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


unsigned const kInvalidThread = 0xffffffff;



} // namespace core
}
#endif // BOOST_STM_DATATYPES__HPP


