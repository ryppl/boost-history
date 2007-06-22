/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_DETAIL_IMPLEMENTATION_INFO_OPERATION_SUPPORT_HPP
#define BOOST_ACT_INTERLOCKED_DETAIL_IMPLEMENTATION_INFO_OPERATION_SUPPORT_HPP

// Note: BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT has the form
//
// ( name, valid_bit_count_for_cas, cas_return, retrieve_info
// , custom_implementations
// )
//
// Where valid_bit_count_for_cas is a Boost.Preprocessor sequence of object
//   sizes in bits which may be used with the underlying cas operation
//
// Where cas_return is either:
//   old_value, meaning the old value is returned from assign_if_was_impl
//                    -or-
//   success_value, meaning it returns true if the operation was successful
//
// Where retrieve_info is either:
//   volatile_retrieve, meaning interlocked::retrieve is automatically
//     implemented to access the passed object with added volatile qualification
//                    -or-
//   custom_retrieve, meaning interlocked::retrieve does not have a default
//     implementation and must be provided by the implementor
//
// Where custom_implementations is a preprocessor sequence of tuples of the form
//   ( function_name, semantice_sequence )
//     Where function_name is the name of a function with a custom implmentation
//
//     Where semantic_sequence is a sequence whose elements are each one of:
//       acquire, meanining there is an implementation for acquire semantics
//                    -or-
//       release, meanining there is an one for release semantics
//                    -or-
//       full_fence, meanining there is one for full barrier semantics
//

#include <boost/config.hpp>

#ifdef BOOST_WINDOWS

  // ToDo: Only use volatile_retrieve if VC 8.0 or higher

  #if WINVER < 0x0600 // Windows prior to vista

    // Note: Same name as vista windows on purpose
    #define BOOST_ACT_INTERLOCKED_DETAIL_OPERATION_SUPPORT                     \
              ( windows,(32),old_value,volatile_retrieve                       \
              , ( ( assign,        ( full_fence ) ) )                        \
                ( ( assign_if_was, ( full_fence ) ) )                        \
              )

  #else // Windows Vista and higher

    // Note: Same name as pre-vista windows on purpose
    #define BOOST_ACT_INTERLOCKED_DETAIL_OPERATION_SUPPORT                     \
              ( windows,(32)(64),old_value,volatile_retrieve                   \
              , ( ( assign,        ( full_fence )( acquire ) ) )             \
                ( ( assign_if_was, ( full_fence )( acquire )( release ) ) )  \
              )

  #endif // End WINVER checks

#elif defined( __GNUC__ )

  #if defined( __i386__ )

    // Note: Same name as x64 on purpose
    #define BOOST_ACT_INTERLOCKED_DETAIL_OPERATION_SUPPORT                     \
              ( gcc_x86,(32),old_value,volatile_retrieve                       \
              , ( ( assign,        ( full_fence ) ) )                        \
                ( ( assign_if_was, ( full_fence ) ) )                        \
              )

  #elif defined( __x86_64__ )

    #error GCC not supported for 64-bit.

  #endif

#endif // End OS checks

#endif
