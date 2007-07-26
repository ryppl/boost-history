/*=============================================================================
    Copyright (c) 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_CSTDATOMIC_HPP
#define BOOST_ACT_INTERLOCKED_CSTDATOMIC_HPP

#include <boost/act/interlocked/store.hpp>
#include <boost/act/interlocked/load.hpp>
#include <boost/act/interlocked/add_assign.hpp>
#include <boost/act/interlocked/subtract_assign.hpp>
#include <boost/act/interlocked/and_assign.hpp>
#include <boost/act/interlocked/or_assign.hpp>
#include <boost/act/interlocked/xor_assign.hpp>
#include <boost/act/interlocked/integer/types.hpp>
#include <boost/act/interlocked/flag.hpp>
#include <boost/noncopyable.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <cassert>
#include <cstddef>

namespace boost { namespace act { namespace interlocked { namespace std_atomic {

enum memory_order
{ memory_order_relaxed
, memory_order_acquire
, memory_order_release
, memory_order_acq_rel
, memory_order_seq_cst
};

#define BOOST_ACT_DETAIL_RETURN_CALL_IMPL( macro, name, order, args, data )    \
switch( order )                                                                \
{                                                                              \
case memory_order_relaxed:                                                     \
  macro( interlocked::name< unordered > args, data )                           \
  break;                                                                       \
case memory_order_acquire:                                                     \
  macro( interlocked::name< acquire > args, data )                             \
  break;                                                                       \
case memory_order_release:                                                     \
  macro( interlocked::name< release > args, data )                             \
  break;                                                                       \
case memory_order_acq_rel:                                                     \
  macro( interlocked::name< acq_rel > args, data )                             \
  break;                                                                       \
case memory_order_seq_cst:                                                     \
  macro( interlocked::name< sequential > args, data )                          \
  break;                                                                       \
}

#define BOOST_ACT_DETAIL_CALL_MAC( expression, dummy ) result;

#define BOOST_ACT_DETAIL_RETURN_CALL_MAC( expression, dummy ) return result;

#define BOOST_ACT_DETAIL_CAS_CALL_MAC( expression, type_and_old_val )          \
BOOST_PP_TUPLE_ELEM( 2, 0 ) expected_val = BOOST_PP_TUPLE_ELEM( 2, 1 );        \
BOOST_PP_TUPLE_ELEM( 2, 1 ) = expression;                                      \
return expected_val == BOOST_PP_TUPLE_ELEM( 2, 1 );

#define BOOST_ACT_DETAIL_CALL( name, order, args )                             \
BOOST_ACT_DETAIL_RETURN_CALL_IMPL( BOOST_ACT_DETAIL_CALL_MAC                   \
                                 , name, order, args                           \
                                 )

#define BOOST_ACT_DETAIL_RETURN_CALL( name, order, args )                      \
BOOST_ACT_DETAIL_RETURN_CALL_IMPL( BOOST_ACT_DETAIL_RETURN_CALL_MAC            \
                                 , name, order, args                           \
                                 )

#define BOOST_ACT_DETAIL_RETURN_CAS_CALL( name, order, args, type, expected )  \
BOOST_ACT_DETAIL_RETURN_CALL_IMPL( BOOST_ACT_DETAIL_CAS_CALL_MAC               \
                                 , name, order, args, ( type, expected )       \
                                 )

// ToDo: Initialize in constructor to clear (figure out how to guarantee order)
struct atomic_flag
  : noncopyable
{
public:
  bool test_and_set( memory_order order = memory_order_seq_cst ) volatile
  {
    BOOST_ACT_DETAIL_RETURN_CALL( set_if_was_clear, order, ( flag_m ) )
  }

  void clear( memory_order order = memory_order_seq_cst ) volatile
  {
    BOOST_ACT_DETAIL_CALL( clear, order, ( flag_m ) )
  }
private:
  interlocked::int_fast8_t flag_m;
}

inline bool atomic_flag_test_and_set( volatile atomic_flag* target )
{
  assert( target != 0 );
  return target->test_and_set( memory_order_seq_cst );
}

inline bool atomic_flag_test_and_set_explicit( volatile atomic_flag* target
                                             , memory_order order
                                             )
{
  assert( target != 0 );
  return target->test_and_set( order );
}

inline void atomic_flag_clear( volatile atomic_flag* target )
{
  assert( target != 0 );
  target->clear( memory_order_seq_cst );
}

inline void atomic_flag_clear_explicit( volatile atomic_flag* target
                                      , memory_order order
                                      )
{
  assert( target != 0 );
  target->clear( order );
}

struct atomic_bool
  : noncopyable
{
public:
  bool lock_free()
  {
    return true; // ToDo: Change when emulation is supported.
  }

  void store( bool val, memory_order order = memory_order_seq_cst ) volatile
  {
    BOOST_ACT_DETAIL_CALL( store, order, ( bool_m, val ) )
  }

  bool load( memory_order order = memory_order_seq_cst ) volatile
  {
    BOOST_ACT_DETAIL_RETURN_CALL( load, order, ( bool_m ) )
  }

  bool swap( bool val, memory_order = memory_order_seq_cst ) volatile
  {
    BOOST_ACT_DETAIL_RETURN_CALL( assign, order, ( bool_m, val ) )
  }

  bool compare_swap( bool& expected, bool new_val
                   , memory_order order = memory_order_seq_cst
                   ) volatile
  {
    BOOST_ACT_DETAIL_RETURN_CAS_CALL( assign_if_was, order
                                    , ( bool_m, new_val, expected )
                                    , bool, expected
                                    )
  }
/*
  void fence( memory_order ) volatile
  {
    // ToDo: Define
  }
*/
  //constexpr atomic_bool( bool __v__ );

  bool operator =( bool val ) volatile
  {
    store( val );
    return val;
  }

  operator bool() volatile
  {
    return load();
  }
private:
  interlocked::bool_fast_t bool_m;
};

inline bool atomic_lock_free( volatile atomic_bool* target )
{
  assert( target != 0 );
  return target->lock_free();
}

inline void atomic_store( volatile atomic_bool* target, bool val )
{
  assert( target != 0 );
  target->store( val );
}

inline void atomic_store_explicit( volatile atomic_bool* target, bool val
                                 , memory_order order
                                 )
{
  assert( target != 0 );
  target->store( val, order );
}

inline bool atomic_load( volatile atomic_bool* target )
{
  assert( target != 0 );
  return target->load();
}

inline bool atomic_load_explicit( volatile atomic_bool* target, memory_order order )
{
  assert( target != 0 );
  return target->load( order );
}

inline bool atomic_swap( volatile atomic_bool* target, bool val )
{
  assert( target != 0 );
  return target->swap( val );
}

inline bool atomic_swap_explicit( volatile atomic_bool* target, bool val
                                , memory_order order
                                )
{
  assert( target != 0 );
  return target->swap( val, order );
}

inline bool atomic_compare_swap( volatile atomic_bool* target
                               , bool* expected, bool val
                               )
{
  assert( target != 0 );
  return target->compare_swap( *expected, val );
}

inline bool atomic_compare_swap_explicit( volatile atomic_bool* target
                                        , bool*, bool val, memory_order order
                                        )
{
  assert( target != 0 );
  return target->compare_swap( *expected, val, order );
}

inline void atomic_fence( volatile atomic_bool* target, memory_order order )
{
  assert( target != 0 );
  // ToDo: Define
}

struct atomic_address
  : noncopyable
{
  bool lock_free()
  {
    return true; // ToDo: Change when emulation is supported.
  }
  void store( void* val, memory_order order = memory_order_seq_cst ) volatile
  {
    BOOST_ACT_DETAIL_CALL( store, order, ( address_m, val ) )
  }

  void* load( memory_order order = memory_order_seq_cst ) volatile
  {
    BOOST_ACT_DETAIL_RETURN_CALL( load, order, ( address_m ) )
  }

  void* swap( void* val, memory_order order = memory_order_seq_cst ) volatile
  {
    BOOST_ACT_DETAIL_RETURN_CALL( assign, order, ( address_m, val ) )
  }

  void* compare_swap( void*& expected, void*, new_val
                    , memory_order order = memory_order_seq_cst
                    ) volatile
  {
    BOOST_ACT_DETAIL_RETURN_CAS_CALL( assign_if_was, order
                                    , ( address_m, new_val, expected )
                                    , void*, expected
                                    )
  }
/*
  void fence( memory_order ) volatile
  {
    // ToDo: Define
  }
*/
  void* fetch_add( std::ptrdiff_t offset
                 , memory_order order = memory_order_seq_cst
                 ) volatile
  {
    BOOST_ACT_DETAIL_RETURN_CALL( add_assign, order, ( address_m, offset ) )
  }

  void* fetch_sub( std::ptrdiff_t offset
                 , memory_order order = memory_order_seq_cst
                 ) volatile
  {
    BOOST_ACT_DETAIL_RETURN_CALL( subtract_assign, order, ( address_m,offset ) )
  }

  //constexpr atomic_address( void* );

  void* operator =( void* address ) volatile
  {
    store( address );
    return address;
  }

  operator void*() volatile
  {
    return load();
  }

  void* operator +=( std::ptrdiff_t offset ) volatile
  {
    BOOST_ACT_DETAIL_RETURN_CALL( add_assign, order, ( address_m,offset ) )
  }

  void* operator -=( std::ptrdiff_t offset ) volatile
  {
    BOOST_ACT_DETAIL_RETURN_CALL( subtract_assign, order, ( address_m,offset ) )
  }
private:
  void* address_m;
};

inline bool atomic_lock_free( volatile atomic_address* target )
{
  assert( target != 0 );
  return target->lock_free();
}

inline void atomic_store( volatile atomic_address* target, void* val )
{
  assert( target != 0 );
  target->store( val );
}

inline void atomic_store_explicit( volatile atomic_address* target, void* val
                                 , memory_order order
                                 )
{
  assert( target != 0 );
  target->store( val, order );
}

inline void* atomic_load( volatile atomic_address* target )
{
  assert( target != 0 );
  return target->load();
}

inline void* atomic_load_explicit( volatile atomic_address* target
                                 , memory_order order
                                 )
{
  assert( target != 0 );
  return target->load( order );
}

inline void* atomic_swap( volatile atomic_address* target, void* val )
{
  assert( target != 0 );
  return target->swap( val );
}

inline void* atomic_swap_explicit( volatile atomic_address* target, void* val
                                 , memory_order order
                                 )
{
  assert( target != 0 );
  return target->swap( val, order );
}

inline void* atomic_compare_swap( volatile atomic_address* target
                                , void** expected, void* new_val
                                )
{
  assert( target != 0 );
  return target->compare_swap( *expected, val );
}

inline void* atomic_compare_swap_explicit( volatile atomic_address* target
                                         , void** expected, void* new_val
                                         , memory_order order
                                         )
{
  assert( target != 0 );
  return target->compare_swap( *expected, val, order );
}
/*
inline void atomic_fence( volatile atomic_address* target, memory_order order )
{
  assert( target != 0 );
  // ToDo: Define
}
*/
inline void* atomic_fetch_add( volatile atomic_address* target
                             , std::ptrdiff_t offset
                             )
{
  assert( target != 0 );
  return target->fetch_add( offset );
}

inline void* atomic_fetch_add_explicit( volatile atomic_address* target
                                      , std::ptrdiff_t offset
                                      , memory_order order
                                      )
{
  assert( target != 0 );
  return target->fetch_add( offset, order );
}

inline void* atomic_fetch_sub( volatile atomic_address* target
                             , std::ptrdiff_t offset
                             )
{
  assert( target != 0 );
  return target->fetch_sub( offset );
}

inline void* atomic_fetch_sub_explicit( volatile atomic_address* target
                                      , std::ptrdiff_t offset
                                      , memory_order order
                                      )
{
  assert( target != 0 );
  return target->fetch_sub( offset, order );
}

template< typename T >
struct atomic
  : noncopyable
{
  bool lock_free() volatile
  {
    return true; // ToDo: Change when emulation is supported.
  }

  void store( T val, memory_order order = memory_order_seq_cst ) volatile
  {
    BOOST_ACT_DETAIL_CALL( store, order, ( val_m, val ) )
  }

  T load( memory_order order = memory_order_seq_cst ) volatile
  {
    BOOST_ACT_DETAIL_RETURN_CALL( load, order, ( val_m ) )
  }

  T swap( T val, memory_order order = memory_order_seq_cst ) volatile
  {
    BOOST_ACT_DETAIL_RETURN_CALL( assign, order, ( val_m, val ) )
  }

  bool compare_swap( T& expected, T new_val
                   , memory_order order = memory_order_seq_cst
                   ) volatile
  {
    BOOST_ACT_DETAIL_RETURN_CAS_CALL( assign_if_was, order
                                    , ( val_m, new_val, expected )
                                    , T, expected
                                    )
  }
/*
  void fence( memory_order ) volatile
  {
    //ToDo: Define
  }
*/
  //CPP0X(constexpr) atomic( T __v__ ) : __f__( __v__ ) { }

  T operator =( T val ) volatile
  {
    store( val );
    return val;
  }

  operator T() volatile
  {
    return load();
  }
private:
  T val_m;
};

//And for each of the integral (character and integer) types listed above, 

/*
#define BOOST_ACT_ATOMIC_INTEGRAL( type )                                      \
struct BOOST_PP_CAT( atomic_, type )                                           \
  : noncopyable                                                                \
{                                                                              \
  bool lock_free();                                                            \
  void store( integral, memory_order = memory_order_seq_cst ) volatile;        \
  integral load( memory_order = memory_order_seq_cst ) volatile;               \
  integral swap( integral,
                 memory_order = memory_order_seq_cst ) volatile;
  bool compare_swap( integral&, integral,
                     memory_order = memory_order_seq_cst ) volatile;
  void fence( memory_order ) volatile;
  integral fetch_add( integral,
                      memory_order = memory_order_seq_cst ) volatile;
  integral fetch_sub( integral,
                      memory_order = memory_order_seq_cst ) volatile;
  integral fetch_and( integral,
                      memory_order = memory_order_seq_cst ) volatile;
  integral fetch_or( integral,
                      memory_order = memory_order_seq_cst ) volatile;
  integral fetch_xor( integral,
                      memory_order = memory_order_seq_cst ) volatile;

  atomic_integral() = default;
  constexpr atomic_integral( integral );
  integral operator =( integral ) volatile;
  operator integral() volatile;
  integral operator +=( integral ) volatile;
  integral operator -=( integral ) volatile;
  integral operator &=( integral ) volatile;
  integral operator |=( integral ) volatile;
  integral operator ^=( integral ) volatile;
private:
  interlocked::type val_m;
};

inline bool atomic_lock_free( volatile atomic_integral* );
inline void atomic_store( volatile atomic_integral*, integral );
inline void atomic_store_explicit( volatile atomic_integral*, integral, memory_order );
inline integral atomic_load( volatile atomic_integral* );
inline integral atomic_load_explicit( volatile atomic_integral*, memory_order );
inline integral atomic_swap( volatile atomic_integral*, integral );
inline integral atomic_swap_explicit( volatile atomic_integral*, integral,
                               memory_order );
inline bool atomic_compare_swap( volatile atomic_integral*, integral*, integral );
inline bool atomic_compare_swap_explicit( volatile atomic_integral*, integral*,
                                   integral, memory_order );
inline void atomic_fence( volatile atomic_integral*, memory_order ) volatile;
inline integral atomic_fetch_add( volatile atomic_integral*, integral );
inline integral atomic_fetch_add_explicit( volatile atomic_integral*, integral,
                                    memory_order );
inline integral atomic_fetch_sub( volatile atomic_integral*, integral );
inline integral atomic_fetch_sub_explicit( volatile atomic_integral*, integral,
                                    memory_order );
inline integral atomic_fetch_and( volatile atomic_integral*, integral );
inline integral atomic_fetch_and_explicit( volatile atomic_integral*, integral,
                                    memory_order );
inline integral atomic_fetch_or( volatile atomic_integral*, integral );
inline integral atomic_fetch_or_explicit( volatile atomic_integral*, integral,
                                    memory_order );
inline integral atomic_fetch_xor( volatile atomic_integral*, integral );
inline integral atomic_fetch_xor_explicit( volatile atomic_integral*, integral,
                                    memory_order );
*/

#undef BOOST_ACT_DETAIL_RETURN_CAS_CALL
#undef BOOST_ACT_DETAIL_RETURN_CALL
#undef BOOST_ACT_DETAIL_CALL
#undef BOOST_ACT_DETAIL_CAS_CALL_MAC
#undef BOOST_ACT_DETAIL_RETURN_CALL_MAC
#undef BOOST_ACT_DETAIL_CALL_MAC
#undef BOOST_ACT_DETAIL_RETURN_CALL_IMPL

} } } }

#endif
