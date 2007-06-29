/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_CONFIG_INTERLOCKED_HAS_HPP
#define BOOST_ACT_CONFIG_INTERLOCKED_HAS_HPP

#include <boost/act/interlocked/detail/cas_support.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/logical/bitor.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/control/if.hpp>

#define BOOST_ACT_CONFIG_DETAIL_IS_DEFAULT_SEMANTICSacquire()   0
#define BOOST_ACT_CONFIG_DETAIL_IS_DEFAULT_SEMANTICSrelease()   0
#define BOOST_ACT_CONFIG_DETAIL_IS_DEFAULT_SEMANTICSacq_rel()   0
#define BOOST_ACT_CONFIG_DETAIL_IS_DEFAULT_SEMANTICSunordered() 0
#define BOOST_ACT_CONFIG_DETAIL_IS_DEFAULT_SEMANTICSdefault()   1
#define BOOST_ACT_CONFIG_DETAIL_IS_DEFAULT_SEMANTICSdefault_()  1
#define BOOST_ACT_CONFIG_DETAIL_IS_DEFAULT_SEMANTICS_()         1

#define BOOST_ACT_CONFIG_DETAIL_DEFAULT_OF_add_assign()      acq_rel
#define BOOST_ACT_CONFIG_DETAIL_DEFAULT_OF_assign()          acq_rel
#define BOOST_ACT_CONFIG_DETAIL_DEFAULT_OF_assign_if_was()   acq_rel
#define BOOST_ACT_CONFIG_DETAIL_DEFAULT_OF_bitand_assign()   acq_rel
#define BOOST_ACT_CONFIG_DETAIL_DEFAULT_OF_bitor_assign()    acq_rel
#define BOOST_ACT_CONFIG_DETAIL_DEFAULT_OF_compiler_fence()  acq_rel /*Change*/
#define BOOST_ACT_CONFIG_DETAIL_DEFAULT_OF_compl_assign()    acq_rel
#define BOOST_ACT_CONFIG_DETAIL_DEFAULT_OF_decrement()       acq_rel
#define BOOST_ACT_CONFIG_DETAIL_DEFAULT_OF_divide_assign()   acq_rel
#define BOOST_ACT_CONFIG_DETAIL_DEFAULT_OF_fence()           acq_rel /*Change*/
#define BOOST_ACT_CONFIG_DETAIL_DEFAULT_OF_increment()       acq_rel
#define BOOST_ACT_CONFIG_DETAIL_DEFAULT_OF_lshift_assign()   acq_rel
#define BOOST_ACT_CONFIG_DETAIL_DEFAULT_OF_mod_assign()      acq_rel
#define BOOST_ACT_CONFIG_DETAIL_DEFAULT_OF_modify()          acq_rel
#define BOOST_ACT_CONFIG_DETAIL_DEFAULT_OF_multiply_assign() acq_rel
#define BOOST_ACT_CONFIG_DETAIL_DEFAULT_OF_negate_assign()   acq_rel
#define BOOST_ACT_CONFIG_DETAIL_DEFAULT_OF_not_assign()      acq_rel
#define BOOST_ACT_CONFIG_DETAIL_DEFAULT_OF_load()            acquire
#define BOOST_ACT_CONFIG_DETAIL_DEFAULT_OF_rshift_assign()   acq_rel
#define BOOST_ACT_CONFIG_DETAIL_DEFAULT_OF_store()           release
#define BOOST_ACT_CONFIG_DETAIL_DEFAULT_OF_subtract_assign() acq_rel
#define BOOST_ACT_CONFIG_DETAIL_DEFAULT_OF_xor_assign()      acq_rel

#define BOOST_ACT_CONFIG_DETAIL_DEFAULT_OF( name )                             \
BOOST_PP_CAT( BOOST_ACT_CONFIG_DETAIL_DEFAULT_OF_, name )()

#define BOOST_ACT_CONFIG_DETAIL_IS_DEFAULT_SEMANTICS( semantics )              \
BOOST_PP_CAT( BOOST_ACT_CONFIG_DETAIL_IS_DEFAULT_SEMANTICS, semantics )()

#define BOOST_ACT_CONFIG_DETAIL_HAS_ASSIGN_IF_WAS_acq_rel()                    \
BOOST_ACT_INTERLOCKED_DETAIL_HAS_CUSTOM_IMPL( assign_if_was, acq_rel )

#define BOOST_ACT_CONFIG_DETAIL_HAS_ASSIGN_IF_WAS_acquire()                    \
BOOST_PP_BITOR                                                                 \
( BOOST_ACT_INTERLOCKED_DETAIL_HAS_CUSTOM_IMPL( assign_if_was, acquire )       \
, BOOST_ACT_CONFIG_DETAIL_HAS_ASSIGN_IF_WAS_acq_rel()                          \
)

#define BOOST_ACT_CONFIG_DETAIL_HAS_ASSIGN_IF_WAS_release()                    \
BOOST_PP_BITOR                                                                 \
( BOOST_ACT_INTERLOCKED_DETAIL_HAS_CUSTOM_IMPL( assign_if_was, release )       \
, BOOST_ACT_CONFIG_DETAIL_HAS_ASSIGN_IF_WAS_acq_rel()                          \
)

#define BOOST_ACT_CONFIG_DETAIL_HAS_ASSIGN_IF_WAS_unordered()                  \
BOOST_PP_BITOR                                                                 \
( BOOST_ACT_INTERLOCKED_DETAIL_HAS_CUSTOM_IMPL( assign_if_was, unordered )     \
, BOOST_PP_BITOR                                                               \
  ( BOOST_ACT_CONFIG_DETAIL_HAS_ASSIGN_IF_WAS_acquire()                        \
  , BOOST_ACT_CONFIG_DETAIL_HAS_ASSIGN_IF_WAS_release()                        \
  )                                                                            \
)

#define BOOST_ACT_CONFIG_DETAIL_HAS_ASSIGN_IF_WAS( semantics )                 \
BOOST_PP_CAT( BOOST_ACT_CONFIG_DETAIL_HAS_ASSIGN_IF_WAS_, semantics )()

#define BOOST_ACT_CONFIG_DETAIL_HAS_ASSIGN_IF_WAS_DUMMY( name, semantics )     \
BOOST_ACT_CONFIG_DETAIL_HAS_ASSIGN_IF_WAS( semantics )

#define BOOST_ACT_CONFIG_DETAIL_HAS_AIW_DERIVABLE_acq_rel( name )              \
BOOST_PP_BITOR                                                                 \
( BOOST_ACT_INTERLOCKED_DETAIL_HAS_CUSTOM_IMPL( name, acq_rel )                \
, BOOST_PP_BITOR                                                               \
  ( BOOST_ACT_CONFIG_DETAIL_HAS_ASSIGN_IF_WAS_acquire()                        \
  , BOOST_ACT_CONFIG_DETAIL_HAS_ASSIGN_IF_WAS_release()                        \
  )                                                                            \
)

#define BOOST_ACT_CONFIG_DETAIL_HAS_AIW_DERIVABLE_acquire( name )              \
BOOST_PP_BITOR                                                                 \
( BOOST_ACT_INTERLOCKED_DETAIL_HAS_CUSTOM_IMPL( name, acquire )                \
, BOOST_PP_BITOR                                                               \
  ( BOOST_ACT_CONFIG_DETAIL_HAS_ASSIGN_IF_WAS_acquire()                        \
  , BOOST_ACT_CONFIG_DETAIL_HAS_AIW_DERIVABLE_acq_rel( name )                  \
  )                                                                            \
)

#define BOOST_ACT_CONFIG_DETAIL_HAS_AIW_DERIVABLE_release( name )              \
BOOST_PP_BITOR                                                                 \
( BOOST_ACT_INTERLOCKED_DETAIL_HAS_CUSTOM_IMPL( name, release )                \
, BOOST_PP_BITOR                                                               \
  ( BOOST_ACT_CONFIG_DETAIL_HAS_ASSIGN_IF_WAS_release()                        \
  , BOOST_ACT_CONFIG_DETAIL_HAS_AIW_DERIVABLE_acq_rel( name )                  \
  )                                                                            \
)

#define BOOST_ACT_CONFIG_DETAIL_HAS_AIW_DERIVABLE_unordered( name )            \
BOOST_PP_BITOR                                                                 \
( BOOST_ACT_INTERLOCKED_DETAIL_HAS_CUSTOM_IMPL( name, unordered )              \
, BOOST_PP_BITOR                                                               \
  ( BOOST_ACT_CONFIG_DETAIL_HAS_ASSIGN_IF_WAS_unordered()                      \
  , BOOST_PP_BITOR                                                             \
    ( BOOST_ACT_CONFIG_DETAIL_HAS_AIW_DERIVABLE_acquire( name )                \
    , BOOST_ACT_CONFIG_DETAIL_HAS_AIW_DERIVABLE_release( name )                \
    )                                                                          \
  )                                                                            \
)

#define BOOST_ACT_CONFIG_DETAIL_HAS_AIW_DERIVABLE( name, semantics )           \
BOOST_PP_CAT( BOOST_ACT_CONFIG_DETAIL_HAS_AIW_DERIVABLE_, semantics )( name )  \

#define BOOST_ACT_CONFIG_DETAIL_HAS_FENCE_full()                               \
BOOST_ACT_INTERLOCKED_DETAIL_HAS_CUSTOM_IMPL( fence, full )

#define BOOST_ACT_CONFIG_DETAIL_HAS_FENCE_read()                               \
BOOST_PP_BITOR                                                                 \
( BOOST_ACT_CONFIG_DETAIL_HAS_FENCE_full()                                     \
, BOOST_ACT_INTERLOCKED_DETAIL_HAS_CUSTOM_IMPL( fence, read )                  \
)

#define BOOST_ACT_CONFIG_DETAIL_HAS_FENCE_write()                              \
BOOST_PP_BITOR                                                                 \
( BOOST_ACT_CONFIG_DETAIL_HAS_FENCE_full()                                     \
, BOOST_ACT_INTERLOCKED_DETAIL_HAS_CUSTOM_IMPL( fence, write )                 \
)

#define BOOST_ACT_CONFIG_DETAIL_HAS_FENCE( name, semantics )                   \
BOOST_PP_CAT( BOOST_ACT_CONFIG_DETAIL_HAS_FENCE_, semantics )()

#define BOOST_ACT_CONFIG_DETAIL_HAS_NONCOMPILER_FENCE( name, semantics )       \
BOOST_PP_IF                                                                    \
( BOOST_ACT_INTERLOCKED_DETAIL_IS_NAME( name, fence )                          \
, BOOST_ACT_CONFIG_DETAIL_HAS_FENCE                                            \
, BOOST_ACT_CONFIG_DETAIL_HAS_AIW_DERIVABLE                                    \
)                                                                              \
( name, semantics )

#define BOOST_ACT_CONFIG_DETAIL_HAS_COMPILER_FENCE_full()                      \
BOOST_PP_BITOR                                                                 \
( BOOST_ACT_CONFIG_DETAIL_HAS_FENCE_full()                                     \
, BOOST_ACT_INTERLOCKED_DETAIL_HAS_CUSTOM_IMPL( compiler_fence, full )         \
)

#define BOOST_ACT_CONFIG_DETAIL_HAS_COMPILER_FENCE_read()                      \
BOOST_PP_BITOR                                                                 \
( BOOST_PP_BITOR                                                               \
  ( BOOST_ACT_CONFIG_DETAIL_HAS_COMPILER_FENCE_full()                          \
  , BOOST_ACT_CONFIG_DETAIL_HAS_FENCE_read()                                   \
  )                                                                            \
, BOOST_ACT_INTERLOCKED_DETAIL_HAS_CUSTOM_IMPL( compiler_fence, read )         \
)
#define BOOST_ACT_CONFIG_DETAIL_HAS_COMPILER_FENCE_write()                     \
BOOST_PP_BITOR                                                                 \
( BOOST_PP_BITOR                                                               \
  ( BOOST_ACT_CONFIG_DETAIL_HAS_COMPILER_FENCE_full                            \
  , BOOST_ACT_CONFIG_DETAIL_HAS_FENCE_write()                                  \
  )                                                                            \
, BOOST_ACT_INTERLOCKED_DETAIL_HAS_CUSTOM_IMPL( compiler_fence, write )        \
)

#define BOOST_ACT_CONFIG_DETAIL_HAS_COMPILER_FENCE( name, semantics )          \
BOOST_PP_CAT( BOOST_ACT_CONFIG_DETAIL_HAS_COMPILER_FENCE_, semantics )()

#define BOOST_ACT_CONFIG_DETAIL_HAS_NONLOAD_STORE( name, semantics )           \
BOOST_PP_IF                                                                    \
( BOOST_ACT_INTERLOCKED_DETAIL_IS_NAME( name, compiler_fence )                 \
, BOOST_ACT_CONFIG_DETAIL_HAS_COMPILER_FENCE                                   \
, BOOST_ACT_CONFIG_DETAIL_HAS_NONCOMPILER_FENCE                                \
)                                                                              \
( name, semantics )

#define BOOST_ACT_CONFIG_DETAIL_HAS_STORE_release()                            \
BOOST_PP_BITOR                                                                 \
( BOOST_ACT_INTERLOCKED_DETAIL_HAS_VOLATILE_STORE_RELEASE                      \
, BOOST_ACT_INTERLOCKED_DETAIL_HAS_CUSTOM_IMPL( store, release )               \
)

#define BOOST_ACT_CONFIG_DETAIL_HAS_STORE_unordered()                          \
BOOST_PP_BITOR                                                                 \
( BOOST_ACT_INTERLOCKED_DETAIL_HAS_VOLATILE_STORE_UNORDERED                    \
, BOOST_PP_BITOR                                                               \
  ( BOOST_ACT_CONFIG_DETAIL_HAS_STORE_release()                                \
  , BOOST_ACT_INTERLOCKED_DETAIL_HAS_CUSTOM_IMPL( store, unordered )           \
  )                                                                            \
)

#define BOOST_ACT_CONFIG_DETAIL_HAS_STORE( name, semantics )                   \
BOOST_PP_CAT( BOOST_ACT_CONFIG_DETAIL_HAS_STORE_, semantics )()

#define BOOST_ACT_CONFIG_DETAIL_HAS_NONLOAD( name, semantics )                 \
BOOST_PP_IF                                                                    \
( BOOST_ACT_INTERLOCKED_DETAIL_IS_NAME( name, store )                          \
, BOOST_ACT_CONFIG_DETAIL_HAS_STORE                                            \
, BOOST_ACT_CONFIG_DETAIL_HAS_NONLOAD_STORE                                    \
)                                                                              \
( name, semantics )

#define BOOST_ACT_CONFIG_DETAIL_HAS_LOAD_acquire()                             \
BOOST_PP_BITOR                                                                 \
( BOOST_ACT_INTERLOCKED_DETAIL_HAS_VOLATILE_LOAD_ACQUIRE                       \
, BOOST_ACT_INTERLOCKED_DETAIL_HAS_CUSTOM_IMPL( load, acquire )                \
)

#define BOOST_ACT_CONFIG_DETAIL_HAS_LOAD_unordered()                           \
BOOST_PP_BITOR                                                                 \
( BOOST_ACT_INTERLOCKED_DETAIL_HAS_VOLATILE_LOAD_UNORDERED                     \
, BOOST_PP_BITOR                                                               \
  ( BOOST_ACT_CONFIG_DETAIL_HAS_LOAD_acquire()                                 \
  , BOOST_ACT_INTERLOCKED_DETAIL_HAS_CUSTOM_IMPL( load, unordered )            \
  )                                                                            \
)

#define BOOST_ACT_CONFIG_DETAIL_HAS_LOAD( name, semantics )                    \
BOOST_PP_CAT( BOOST_ACT_CONFIG_DETAIL_HAS_LOAD_, semantics )()

#define BOOST_ACT_CONFIG_INTERLOCKED_HAS_IMPL( name, semantics )               \
BOOST_PP_IF                                                                    \
( BOOST_ACT_INTERLOCKED_DETAIL_IS_NAME( name, load )                           \
, BOOST_ACT_CONFIG_DETAIL_HAS_LOAD                                             \
, BOOST_ACT_CONFIG_DETAIL_HAS_NONLOAD                                          \
)                                                                              \
( name, semantics )

#define BOOST_ACT_CONFIG_INTERLOCKED_HAS_INTERNAL( name, semantics )           \
BOOST_PP_IF                                                                    \
( BOOST_ACT_INTERLOCKED_DETAIL_IS_NAME( name, load )                           \
, BOOST_ACT_CONFIG_DETAIL_HAS_LOAD                                             \
, BOOST_ACT_CONFIG_DETAIL_HAS_NONLOAD                                          \
)                                                                              \
( name, semantics )

#define BOOST_ACT_CONFIG_INTERLOCKED_HAS_DEFAULT_INTERNAL( name, semantics )   \
BOOST_ACT_CONFIG_INTERLOCKED_HAS_INTERNAL                                      \
( name, BOOST_ACT_CONFIG_DETAIL_DEFAULT_OF( name ) )

#define BOOST_ACT_CONFIG_INTERLOCKED_HAS( name, semantics )                    \
BOOST_PP_IF                                                                    \
( BOOST_ACT_CONFIG_DETAIL_IS_DEFAULT_SEMANTICS( semantics )                    \
, BOOST_ACT_CONFIG_INTERLOCKED_HAS_DEFAULT_INTERNAL                            \
, BOOST_ACT_CONFIG_INTERLOCKED_HAS_INTERNAL                                    \
)                                                                              \
( name, semantics )

#endif
