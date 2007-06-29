/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_DETAIL_IMPLEMENTATION_INFO_OPERATION_SUPPORT_HPP
#define BOOST_ACT_INTERLOCKED_DETAIL_IMPLEMENTATION_INFO_OPERATION_SUPPORT_HPP

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
// This file is used to determine which implementations are to be used when
// compiling interlocked operations. For instance, if you are planning on adding
// support for x86-64 with gcc compilers, you would implement branching in this
// file which detects such systems, provides a unique "name", and specifies
// exactly which functionality is supported. For this particular example, stubs
// are currently in-place for x86-64 with gcc detection, so if you would, please
// provide such an implementation for it so that I don't have to :p
// Instructions on how this is to be accomplished are detailed in the comments
// below.
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

// Conditional compilation in Boost.Act is based primarily on the macro
// BOOST_ACT_INTERLOCKED_DETAIL_OPERATION_SUPPORT
//
// Each target system defines this macro in a way which provides details on
// exactly which functionality is supported to the internals of the library and
// which also allows users to query via the preprocessor if a given operation
// is supported and precicely what its semantics are. Therefore, adding support
// for a new system implies preprocessor branching in this file which results in
// an accurate definition of this macro. If no definition of this macro is
// present, then no support for interlocked operations is assumed. A description
// of the format you must use for the macro is detailed below, and examples are
// presented shortly after in comments.
//
// BOOST_ACT_INTERLOCKED_DETAIL_OPERATION_SUPPORT has the form
//
// ( name, valid_bit_counts, load_info, store_info, custom_implementations )
//
// Where valid_bit_counts is a Boost.Preprocessor sequence of object sizes in
//   bits which may be used with interlocked operations
//
// Where load_info is one of:
//   volatile_load_acquire, meaning interlocked::load_acquire is automatically
//     implemented to access the passed object with added volatile qualification
//                    -or-
//   custom_load_acquire, meaning interlocked::load_acquire does not have a
//     default implementation and must be provided by the implementor
//                    -or-
//   volatile_load_unordered, meaning interlocked::load_unordered is
//     automatically implemented to access the passed object with added volatile
//     qualification
//
// Where store_info is either:
//   volatile_store_release, meaning interlocked::store_release is automatically
//     implemented to access the passed object with added volatile qualification
//                    -or-
//   custom_store_release, meaning interlocked::store_release does not have a
//     default implementation and must be provided by the implementor
//                    -or-
//   volatile_store_unordered, meaning interlocked::store_unordered is
//     automatically implemented to access the passed object with added volatile
//     qualification
//
// Where custom_implementations is a preprocessor sequence of tuples of the form
//   ( function_name, semantic_sequence )
//     Where function_name is the name of a function with a custom implmentation
//
//     Where semantic_sequence is a sequence whose elements are each one of:
//       acquire, meanining there is an implementation for acquire semantics
//                    -or-
//       release, meanining there is an one for release semantics
//                    -or-
//       acq_rel, meanining there is one for full barrier semantics
//                    -or-
//       unordered, meanining there is no specific ordering required
//
//
// Because of the heavy branching and use of macros that occur in practice, it
// may be unclear from the actual implementation the exact syntax, so here is a
// quick example.

/*
#define BOOST_ACT_INTERLOCKED_DETAIL_OPERATION_SUPPORT                         \
        ( some_64_bit_system,(32)(64)                                          \
        , volatile_load_acquire                                                \
        , custom_store_release                                                 \
        , (( load,         ( unordered ) ))                                    \
          (( store,        ( release )( unordered ) ))                         \
          (( assign,       ( acq_rel )( acquire )( release )( unordered ) ))\
          (( assign_if_was,( acq_rel ) ))                                   \
          (( add_assign,   ( acq_rel ) ))                                   \
        )
*/

// In this example, some_64_bit_system is specified as the name for this
// implementation. When the internals of the library are defining interlocked
// operations for which a custom implementation has been specified as existing
// by this macro definition, it looks for the implementation in a file in the
// corresponding operation's directory's detail subdirectory. The name of the
// file that it looks for is the name of the operation, including semantics,
// concatenated with the name specified by the macro (with no extension). In
// this example, the internals of the library would look for the implementation
// of load_unordered in
// <boost/act/interlocked/load/detail/load_unordered_some_64_bit_system>
//
// For details on how to provide this implementation, please use one of the
// non-default-affixed files in assign as an example.
//
// The (32)(64) in the example specifies that both 32 and 64-bit types may
// support interlocked operations; volatile_load_acquire means that an
// implementation for load_acquire will be generated automatically which just
// casts a reference to the object to volatile and then accesses the result;
// custom_store_release means that no default implementation will be generated
// for store_release (and therefore it must be implemented manually if it is to
// be supported ); the final parameter specifies for which operations a custom
// implementation is provided. Note that the operations in the last parameter
// may appear in any order along with the semantic information in any order.
//
// Boost.Act.Interlocked also attempts to automatically define functionality
// whenever possible based on other operations and fences. If a acq_rel
// implementation is specified but no other forms are specified, the acquire,
// release, and unordered forms are automatically generated based on the
// acq_rel implementation. If an unordered, acquire, or release form is
// specified, and memory fence instruction support is specified in the the
// fence_support header, the acq_rel, acquire, and/or release forms of any
// function will be automatically generated wherever possible. If no store
// support is specified, yet assign support is specified, then store
// implementations are generated based on assign implementations wherever
// possible. Finally, if assign_if_was implementations are available, yet
// compound assign or increment/decrement operations are not provided, their
// implementations are automatically generated based on corresponding
// assign_if_was calls. If you may implement such operations without
// assign_if_was emulation, you are encouraged to do so as it will likely
// provide better performance. There are often several choices the
// implementation may make when providing default implementations, and so it is
// likely that all of the above default-implementation generation will be able
// to be toggled in the future.

/*
    The following are useful macros to use later on after detection.
*/
#define BOOST_ACT_INTERLOCKED_DETAIL_ITANIUM_RW_ORDERING_SUPPORT               \
        ( acq_rel )( acquire )( release )( unordered )

#define BOOST_ACT_INTERLOCKED_DETAIL_ITANIUM_LOAD_ORDERING_SUPPORT             \
        ( acq_rel )( acquire )( unordered )

#define BOOST_ACT_INTERLOCKED_DETAIL_ITANIUM_STORE_ORDERING_SUPPORT            \
        ( acq_rel )( release )( unordered )

#define BOOST_ACT_INTERLOCKED_DETAIL_X86_RW_ORDERING_SUPPORT                   \
        ( acq_rel )

#define BOOST_ACT_INTERLOCKED_DETAIL_X86_LOAD_ORDERING_SUPPORT                 \
        ( acquire )

#define BOOST_ACT_INTERLOCKED_DETAIL_X86_STORE_ORDERING_SUPPORT                \
        ( release )

#define BOOST_ACT_INTERLOCKED_DETAIL_NO_FENCE_SUPPORT                          \
        ( none )

#define BOOST_ACT_INTERLOCKED_DETAIL_X86_FENCE_SUPPORT                         \
        BOOST_ACT_INTERLOCKED_DETAIL_NO_FENCE_SUPPORT

#define BOOST_ACT_INTERLOCKED_DETAIL_SSE_FENCE_SUPPORT                         \
        ( write )

#define BOOST_ACT_INTERLOCKED_DETAIL_SSE2_FENCE_SUPPORT                        \
        ( full )( read )( write )

#define BOOST_ACT_INTERLOCKED_DETAIL_ITANIUM_FENCE_SUPPORT                     \
        ( full )( read )( write )

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/*                                                                            */
/*                         DETECTION CODE BEGINS HERE                         */
/*                                                                            */
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

#include <boost/config.hpp>

// To anyone adding support to this file, I have already layed out some stub
// branching for systems not yet supported. Please look through the code below
// to see if stubs for your system are already in place without implementation.
// Currently, errors are output during compilation when these stubs are reached,
// however, this is only temporary to bring implementors to the line of code
// where they should be specifying implementation support.

// Note: For windows, x86 is currently assumed, however, in the future, if
// projects such as Wine support non-x86 systems, this may have to change, or
// branches for such libraries must be put in place...
#if defined( WIN32 ) || defined( _WIN32 ) || defined( __WIN32__ )

  #if defined(WIN64)||defined(_WIN64)||defined(__WIN64__)||defined(_M_IA64)
    #define BOOST_ACT_INTERLOCKED_DETAIL_INTERLOCKED_SIZES (32)(64)
  #else
    #define BOOST_ACT_INTERLOCKED_DETAIL_INTERLOCKED_SIZES (32)
  #endif

  #ifdef BOOST_MSVC // Is MSVC

    #if _MSC_VER >= 1400 // Is VC8 or higher

      #define BOOST_ACT_INTERLOCKED_DETAIL_LOAD_STYLE  volatile_load_acquire
      #define BOOST_ACT_INTERLOCKED_DETAIL_STORE_STYLE volatile_store_release

    #else

      #define BOOST_ACT_INTERLOCKED_DETAIL_LOAD_STYLE  volatile_load_unordered
      #define BOOST_ACT_INTERLOCKED_DETAIL_STORE_STYLE volatile_store_unordered

    #endif

    #ifdef _M_IA64

      #define BOOST_ACT_INTERLOCKED_DETAIL_RW_ORDERING_SUPPORT                 \
              BOOST_ACT_INTERLOCKED_DETAIL_ITANIUM_RMW_ORDERING_SUPPORT

      #define BOOST_ACT_INTERLOCKED_DETAIL_LOAD_ORDERING_SUPPORT               \
              BOOST_ACT_INTERLOCKED_DETAIL_ITANIUM_LOAD_ORDERING_SUPPORT

      #define BOOST_ACT_INTERLOCKED_DETAIL_STORE_ORDERING_SUPPORT              \
              BOOST_ACT_INTERLOCKED_DETAIL_ITANIUM_STORE_ORDERING_SUPPORT

    #elif defined( _M_IX86_FP ) // SSE support detection possible

      #define BOOST_ACT_INTERLOCKED_DETAIL_RW_ORDERING_SUPPORT                 \
              BOOST_ACT_INTERLOCKED_DETAIL_X86_RW_ORDERING_SUPPORT

      #define BOOST_ACT_INTERLOCKED_DETAIL_LOAD_ORDERING_SUPPORT               \
              BOOST_ACT_INTERLOCKED_DETAIL_X86_LOAD_ORDERING_SUPPORT

      #define BOOST_ACT_INTERLOCKED_DETAIL_STORE_ORDERING_SUPPORT              \
              BOOST_ACT_INTERLOCKED_DETAIL_X86_STORE_ORDERING_SUPPORT

      #if _M_IX86_FP >= 2 // SSE2 generation is enabled

        #define BOOST_ACT_INTERLOCKED_DETAIL_FENCE_SUPPORT                     \
                BOOST_ACT_INTERLOCKED_DETAIL_SSE2_FENCE_SUPPORT

      #elif _M_IX86_FP >= 1 // Else, if SSE generation is enabled

        #define BOOST_ACT_INTERLOCKED_DETAIL_FENCE_SUPPORT                     \
                BOOST_ACT_INTERLOCKED_DETAIL_SSE_FENCE_SUPPORT

      #else // Else, SSE generation not enabled

        #define BOOST_ACT_INTERLOCKED_DETAIL_FENCE_SUPPORT                     \
                BOOST_ACT_INTERLOCKED_DETAIL_X86_FENCE_SUPPORT

      #endif // End detailed SSE support checks

    #else // Else, SSE generation not enabled

      #define BOOST_ACT_INTERLOCKED_DETAIL_FENCE_SUPPORT                       \
              BOOST_ACT_INTERLOCKED_DETAIL_X86_FENCE_SUPPORT

    #endif // End architecture detection

  #elif defined( __INTEL_COMPILER ) // Else: if intel compiler

    #define BOOST_ACT_INTERLOCKED_DETAIL_LOAD_STYLE  volatile_load_acquire
    #define BOOST_ACT_INTERLOCKED_DETAIL_STORE_STYLE volatile_store_release

    #if defined( _M_AMD64 ) || defined( _M_X64 ) // Intel 64

      // No support yet!

    // Else: if IA64
    #elif defined( __INITIAL_POINTER_SIZE ) && ( __INITIAL_POINTER_SIZE == 64 )

      #define BOOST_ACT_INTERLOCKED_DETAIL_RW_ORDERING_SUPPORT                 \
              BOOST_ACT_INTERLOCKED_DETAIL_ITANIUM_RW_ORDERING_SUPPORT

      #define BOOST_ACT_INTERLOCKED_DETAIL_FENCE_SUPPORT                       \
              BOOST_ACT_INTERLOCKED_DETAIL_ITANIUM_FENCE_SUPPORT

    #elif defined( __QMSPP_ ) // Else: if x86

      #define BOOST_ACT_INTERLOCKED_DETAIL_RW_ORDERING_SUPPORT                 \
              BOOST_ACT_INTERLOCKED_DETAIL_X86_RW_ORDERING_SUPPORT

      #define BOOST_ACT_INTERLOCKED_DETAIL_RW_ORDERING_SUPPORT                 \
              BOOST_ACT_INTERLOCKED_DETAIL_X86_RW_ORDERING_SUPPORT

    #else

      // No support yet!

    #endif

  #else // Else: Default to basic Windows support

    #define BOOST_ACT_INTERLOCKED_DETAIL_LOAD_STYLE  volatile_load_unordered
    #define BOOST_ACT_INTERLOCKED_DETAIL_STORE_STYLE volatile_store_unordered

    #define BOOST_ACT_INTERLOCKED_DETAIL_FENCE_SUPPORT                         \
            BOOST_ACT_INTERLOCKED_DETAIL_NO_FENCE_SUPPORT

    #define BOOST_ACT_INTERLOCKED_DETAIL_RW_ORDERING_SUPPORT                   \
            BOOST_ACT_INTERLOCKED_DETAIL_X86_RW_ORDERING_SUPPORT

  #endif // End Compiler detection

  #define BOOST_ACT_INTERLOCKED_DETAIL_NONVISTA_HAS_OP_SUPPORT()               \
          (     ( WINVER < 0x0600 )                                            \
            &&  defined( BOOST_ACT_INTERLOCKED_DETAIL_LOAD_STYLE )             \
            &&  defined( BOOST_ACT_INTERLOCKED_DETAIL_STORE_STYLE )            \
            &&  defined( BOOST_ACT_INTERLOCKED_DETAIL_RW_ORDERING_SUPPORT )    \
          )

  #define BOOST_ACT_INTERLOCKED_DETAIL_VISTA_HAS_OP_SUPPORT()                  \
          (     ( WINVER >= 0x0600 )                                           \
            &&  defined( BOOST_ACT_INTERLOCKED_DETAIL_LOAD_STYLE )             \
            &&  defined( BOOST_ACT_INTERLOCKED_DETAIL_STORE_STYLE )            \
            &&  defined( BOOST_ACT_INTERLOCKED_DETAIL_RW_ORDERING_SUPPORT )    \
            &&  defined( BOOST_ACT_INTERLOCKED_DETAIL_INTERLOCKED_SIZES )      \
          )

  // ToDo: Possibly change to check for 64-bit instead
  #if BOOST_ACT_INTERLOCKED_DETAIL_NONVISTA_HAS_OP_SUPPORT() // Pre-vista

    // Note: Same name as vista windows on purpose
    // ToDo: Add assembly implementation when possible even though named
    //       versions are not available (including 64-bit support).
    #define BOOST_ACT_INTERLOCKED_DETAIL_OPERATION_SUPPORT                     \
    ( windows, (32), BOOST_ACT_INTERLOCKED_DETAIL_LOAD_STYLE                   \
                   , BOOST_ACT_INTERLOCKED_DETAIL_STORE_STYLE                  \
    , (( assign,        BOOST_ACT_INTERLOCKED_DETAIL_RW_ORDERING_SUPPORT ))    \
      (( assign_if_was, BOOST_ACT_INTERLOCKED_DETAIL_RW_ORDERING_SUPPORT ))    \
    )

  #elif BOOST_ACT_INTERLOCKED_DETAIL_VISTA_HAS_OP_SUPPORT() // Vista and up

    // Note: Same name as pre-vista windows on purpose
    #define BOOST_ACT_INTERLOCKED_DETAIL_OPERATION_SUPPORT                     \
    ( windows, BOOST_ACT_INTERLOCKED_DETAIL_INTERLOCKED_SIZES                  \
    , BOOST_ACT_INTERLOCKED_DETAIL_LOAD_STYLE                                  \
    , BOOST_ACT_INTERLOCKED_DETAIL_STORE_STYLE                                 \
    , (( assign,        BOOST_ACT_INTERLOCKED_DETAIL_RW_ORDERING_SUPPORT ))    \
      (( assign_if_was, BOOST_ACT_INTERLOCKED_DETAIL_RW_ORDERING_SUPPORT ))    \
    )

  #endif // End WINVER checks

#elif defined( __GNUC__ ) // GCC

  #if defined( __x86_64__ ) // x86-64

    // No support yet!

  #elif defined( __i386__ ) // x86

    #define BOOST_ACT_INTERLOCKED_DETAIL_RW_ORDERING_SUPPORT ( acq_rel )

    #ifdef __SSE2__ // SSE2 support

      #define BOOST_ACT_INTERLOCKED_DETAIL_FENCE_SUPPORT                       \
              BOOST_ACT_INTERLOCKED_DETAIL_SSE2_FENCE_SUPPORT

    #elif defined( __SSE__ ) // SSE support

      #define BOOST_ACT_INTERLOCKED_DETAIL_FENCE_SUPPORT                       \
              BOOST_ACT_INTERLOCKED_DETAIL_SSE_FENCE_SUPPORT

    #else // Else: Neither SSE nor SSE2 support

      #define BOOST_ACT_INTERLOCKED_DETAIL_FENCE_SUPPORT                       \
              BOOST_ACT_INTERLOCKED_DETAIL_X86_FENCE_SUPPORT

    #endif // End SSE detection

    #define BOOST_ACT_INTERLOCKED_DETAIL_OPERATION_SUPPORT                     \
    ( gcc_x86, (32), volatile_load_acquire, volatile_store_release             \
    , (( assign,        BOOST_ACT_INTERLOCKED_DETAIL_RW_ORDERING_SUPPORT ))    \
      (( assign_if_was, BOOST_ACT_INTERLOCKED_DETAIL_RW_ORDERING_SUPPORT ))    \
    )

  #elif defined( __powerpc__ ) || defined( __ppc__ )

    // No support yet!

  #elif defined( __sparcv8 ) || defined( __sparcv9 )

    // No support yet!

  #else

    // No support yet!

  #endif

#elif defined( __MWERKS__ ) // Metrowerks

  #if defined( __POWERPC__ ) // powerpc

    // No support yet!

  #else

    // No support yet!

  #endif

#else // Unknown system

    // No support yet!

#endif // End system checks

#endif
