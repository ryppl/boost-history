/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_DATA_HPP
#define BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_DATA_HPP

// Note: BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT has the form
//
// ( pp_seq_of_valid_bit_amounts_for_cas, cas_return, arithmetic_return )
//
// Where cas_return is either:
//   old_value, meaning the old value is returned from assign_if_was_impl
//                    -or-
//   success_value, meaning it returns true if the operation was successful
//
// Where arithmetic_return is either:
//   old_value, meaning the old value is returned from arithmetic impls
//                    -or-
//   new_value, meaning the new value is returned from arithmetic impls

#include <boost/config.hpp>

#ifdef BOOST_WINDOWS

  #include <windows.h>

  #if WINVER < 0x0600 // Windows prior to vista

    #define BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_DATA                      \
              ( (32),old_value,new_value )

  #else // Windows Vista and higher

    #define BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_DATA                      \
              ( (32)(64),old_value,new_value )

  #endif // End WINVER checks

#endif // End OS checks

#endif
