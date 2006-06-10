//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_DETAIL_ARITHMETIC_TYPES_HPP
#define BOOST_ACT_DETAIL_ARITHMETIC_TYPES_HPP

// ToDo: Include long long types if supported
#define boost_DETAIL_INTEGRAL_TYPES()                                          \
  (bool)(char)(signed char)(unsigned char)(wchar_t)(short)(unsigned short)     \
  (int)(unsigned)(long)(unsigned long)

#define boost_DETAIL_FLOATING_POINT_TYPES() (long double)(double)(float)

#define boost_DETAIL_ARITHMETIC_TYPES()                                        \
  boost_DETAIL_INTEGRAL_TYPES()                                                \
  boost_DETAIL_FLOATING_POINT_TYPES() 

#define boost_DETAIL_ADD_QUALIFIER( s, qualifier, type ) type qualifier 

#define boost_DETAIL_VOLATILE_INTEGRAL_TYPES()                                 \
  BOOST_PP_SEQ_TRANSFORM( boost_DETAIL_ADD_QUALIFIER                           \
                        , volatile                                             \
                        , boost_DETAIL_INTEGRAL_TYPES()                        \
                        ) 

#define boost_DETAIL_VOLATILE_ARITHMETIC_TYPES()                               \
  boost_DETAIL_VOLATILE_INTEGRAL_TYPES()                                       \
  BOOST_PP_SEQ_TRANSFORM( boost_DETAIL_ADD_QUALIFIER                           \
                        , volatile                                             \
                        , boost_DETAIL_FLOATING_POINT_TYPES()                  \
                        ) 

#define boost_DETAIL_VQ_INTEGRAL_TYPES()                                       \
  boost_DETAIL_INTEGRAL_TYPES()                                                \
  boost_DETAIL_VOLATILE_INTEGRAL_TYPES() 

#define boost_DETAIL_VQ_ARITHMETIC_TYPES()                                     \
  boost_DETAIL_ARITHMETIC_TYPES()                                              \
  boost_DETAIL_VOLATILE_ARITHMETIC_TYPES() 

#define boost_DETAIL_ARITHMETIC_OPERATORS() ( 4, ( (plus)(+), (minus)(-)       \
                                                 , (times)(*), (divide)(/)     \
                                                 )                             \
                                            ) 

#define boost_DETAIL_COMPARISON_OPERATORS() (<)(>)(<=)(>=)(==)(!=)

#define boost_DETAIL_INTEGRAL_OPERATORS() (%)(&)(^)(|)(<<)(>>)

#define boost_DETAIL_ARITHMETIC_ASSIGNMENT_OPERATORS() (=)(+=)(-=)(*=)(/=)

#define boost_DETAIL_INTEGRAL_ASSIGNMENT_OPERATORS() (%=)(&=)(^=)(|=)(<<=)(>>=)

namespace boost
{
namespace act
{
namespace detail
{

template< typename Type >
struct arithmetic_rank;

template< typename DummyType >
struct empty_base
{
};

#define BOOST_ACTIVE_DETAIL_SET_ARITHMETIC_RANK( r, index_a_seq )              \
template<>                                                                     \
struct arithmetic_rank< BOOST_PP_SEQ_ELEM                                      \
                        (                                                      \
                          BOOST_PP_TUPLE_ELEM( 2, 0, index_a_seq )             \
                        , BOOST_PP_TUPLE_ELEM( 2, 1, index_a_seq )             \
                        )                                                      \
                      >                                                        \
  : ::boost::mpl::size_t< BOOST_PP_TUPLE_ELEM( 2, 0, index_a_seq ) >           \
{                                                                              \
}; 

#define BOOST_ACTIVE_DETAIL_LESS_THAN_SEQ_SIZE( r, index_a_seq )               \
BOOST_PP_LESS( BOOST_PP_TUPLE_ELEM( 2, 0, index_a_seq )                        \
             , BOOST_PP_SEQ_SIZE(BOOST_PP_TUPLE_ELEM( 2, 1, index_a_seq ))     \
             ) 

#define BOOST_ACTIVE_DETAIL_INC_INDEX( r, index_a_seq )                        \
( BOOST_PP_INC( BOOST_PP_TUPLE_ELEM( 2, 0, index_a_seq ) )                     \
, BOOST_PP_TUPLE_ELEM( 2, 1, index_a_seq )                                     \
) 

BOOST_PP_FOR( ( 0, boost_DETAIL_VQ_ARITHMETIC_TYPES() )
            , BOOST_ACTIVE_DETAIL_LESS_THAN_SEQ_SIZE
            , BOOST_ACTIVE_DETAIL_INC_INDEX
            , BOOST_ACTIVE_DETAIL_SET_ARITHMETIC_RANK
            )

template< typename Type0
        , typename Type1
        >
struct ordered_arithmetic_types
{
  typedef typename ::boost::mpl::if_< ::boost::mpl::greater
                                      <
                                        arithmetic_rank< Type0 >
                                      , arithmetic_rank< Type1 >
                                      >
                                    , Type0
                                    , Type1
                                    >
                                    ::type first_type;

  typedef typename ::boost::mpl::if_< ::boost::is_same< first_type, Type0 >
                                    , Type1
                                    , Type0
                                    >
                                    ::type second_type;
};

#define BOOST_ACTIVE_DETAIL_BINARY_FUNCTION_OBJECT( name, op )                 \
template< typename RightType, typename ParamFunc >                             \
class BOOST_PP_CAT( active_binary_function_object_, name )                     \
{                                                                              \
public:                                                                        \
  BOOST_PP_CAT( active_binary_function_object_, name )                         \
  ( RightType const& right_init )                                              \
    : right_m( right_init )                                                    \
  {                                                                            \
  }                                                                            \
                                                                               \
  template< typename LeftType >                                                \
  /* ToDo: Make return type */                                                 \
  operator ()( LeftType const& left ) const                                    \
  {                                                                            \
    return left op ::boost::synchronous_value( right_m );                      \
  }                                                                            \
private:                                                                       \
  typename ::boost::mpl::apply< ParamFunc, RightType >::type right_m;          \
};                                                                             \
                                                                               \
template< typename ParamFunc, typename RightType >                             \
BOOST_PP_CAT(active_binary_function_object_,name)< RightType, ParamFunc >      \
BOOST_PP_CAT( make_active_binary_function_object_, name )                      \
( RightType const& right )                                                     \
{                                                                              \
  return BOOST_PP_CAT( active_binary_function_object_, name )                  \
         < RightType, ParamFunc >( right );                                    \
} 

// ToDo: Use static function in template for noncompliant compilers
#define BOOST_ACTIVE_DETAIL_MAKE_BINARY_FUNCTION_OBJECT(name, right, pfunc)    \
BOOST_PP_CAT( make_active_binary_function_object_, name )                      \
< pfunc >( right ) 

#define BOOST_ACTIVE_DETAIL_UNORDERED_BIN_FUNCS(name,op,l_t,r_t,ret,impl)      \
friend ret operator op                                                         \
           ( typename ::boost::act::add_active<l_t,impl>::type const& left     \
           , r_t const& right                                                  \
           )                                                                   \
  {                                                                            \
    return left.call                                                           \
    ( BOOST_ACTIVE_DETAIL_MAKE_BINARY_FUNCTION_OBJECT( name                    \
                                                     , right                   \
                                                     , ParamFunc               \
                                                     )                         \
    );                                                                         \
  }                                                                            \
                                                                               \
  friend ret operator op                                                       \
             ( l_t const& left                                                 \
             , typename ::boost::act::add_active<r_t,impl>::type const& right  \
             )                                                                 \
  {                                                                            \
    return right op left;                                                      \
  }                                                                            \
                                                                               \
  friend ret operator op                                                       \
             ( typename ::boost::act::add_active<l_t,impl>::type const& left   \
             , typename ::boost::act::add_active<r_t,impl>::type const& right  \
             )                                                                 \
  {                                                                            \
    return left.call                                                           \
    ( BOOST_ACTIVE_DETAIL_MAKE_BINARY_FUNCTION_OBJECT( name                    \
                                                     , right                   \
                                                     , ParamFunc               \
                                                     )                         \
    );                                                                         \
  }                                                                            \

#define BOOST_ACTIVE_DETAIL_UNORDERED_BIN_TEMP( name, op, l_t, r_t, ret )      \
template< typename LeftChildType                                               \
        , typename RightChildType                                              \
        >                                                                      \
friend ReturnType operator op                                                  \
       ( abstract_active< LeftChildType, l_t > const& left                     \
       , abstract_active< RightChildType, r_t > const& right                   \
       )                                                                       \
{                                                                              \
  return left.call                                                             \
  ( BOOST_ACTIVE_DETAIL_MAKE_BINARY_FUNCTION_OBJECT( name                      \
                                                   , right                     \
                                                   , ParamFunc                 \
                                                   )                           \
  );                                                                           \
} 

// ToDo: Make ParamFunc readily available
// ToDo: Recheck ActiveImpl/ParamFunc stuff
#define BOOST_ACTIVE_DETAIL_MAKE_UNORDERED_BINARY_OPERATION_BASE( name, op )   \
template< typename Type0                                                       \
        , typename Type1                                                       \
        , typename ReturnType                                                  \
        , typename ParamFunc                                                   \
        >                                                                      \
class BOOST_PP_CAT                                                             \
      (                                                                        \
        BOOST_PP_CAT( unordered_active_binary_operation_, name )               \
      , _template_base                                                         \
      )                                                                        \
{                                                                              \
  BOOST_ACTIVE_DETAIL_UNORDERED_BIN_TEMP(name,op,Type0,Type1,ReturnType)       \
  BOOST_ACTIVE_DETAIL_UNORDERED_BIN_TEMP(name,op,Type1,Type0,ReturnType)       \
};                                                                             \
                                                                               \
template< typename Type                                                        \
        , typename ReturnType                                                  \
        , typename ParamFunc                                                   \
        >                                                                      \
class BOOST_PP_CAT                                                             \
      (                                                                        \
        BOOST_PP_CAT( unordered_active_binary_operation_, name )               \
      , _template_base                                                         \
      )                                                                        \
      < Type, Type, ReturnType, ParamFunc >                                    \
{                                                                              \
  BOOST_ACTIVE_DETAIL_UNORDERED_BIN_TEMP(name,op,Type,Type,ReturnType)         \
};                                                                             \
                                                                               \
template< typename Type0                                                       \
        , typename Type1                                                       \
        , typename ReturnType                                                  \
        , typename ActiveImpl                                                  \
        >                                                                      \
class BOOST_PP_CAT                                                             \
      (                                                                        \
        BOOST_PP_CAT( unordered_active_binary_operation_, name )               \
      , _base                                                                  \
      )                                                                        \
  : BOOST_PP_CAT                                                               \
    (                                                                          \
      BOOST_PP_CAT( unordered_active_binary_operation_, name )                 \
    , _template_base                                                           \
    )                                                                          \
    < Type0, Type1, ReturnType, ActiveImpl >                                   \
{                                                                              \
  BOOST_ACTIVE_DETAIL_UNORDERED_BIN_FUNCS( name, op, Type0, Type1              \
                                         , ReturnType, ActiveImpl              \
                                         )                                     \
                                                                               \
  BOOST_ACTIVE_DETAIL_UNORDERED_BIN_FUNCS( name, op, Type1, Type0              \
                                         , ReturnType, ActiveImpl              \
                                         )                                     \
};                                                                             \
                                                                               \
template< typename Type                                                        \
        , typename ReturnType                                                  \
        , typename ActiveImpl                                                  \
        >                                                                      \
class BOOST_PP_CAT                                                             \
      (                                                                        \
        BOOST_PP_CAT( unordered_active_binary_operation_, name )               \
      , _base                                                                  \
      )                                                                        \
      <                                                                        \
        Type, Type                                                             \
      , ReturnType                                                             \
      , ActiveImpl                                                             \
      >                                                                        \
  : BOOST_PP_CAT                                                               \
    (                                                                          \
      BOOST_PP_CAT( unordered_active_binary_operation_, name )                 \
    , _template_base                                                           \
    )                                                                          \
    < Type, Type, ReturnType, ActiveImpl >                                     \
{                                                                              \
  BOOST_ACTIVE_DETAIL_UNORDERED_BIN_FUNCS( name, op, Type, Type                \
                                         , ReturnType, ActiveImpl              \
                                         )                                     \
};                                                                             \
                                                                               \
template< typename Type0                                                       \
        , typename Type1                                                       \
        , typename ReturnType                                                  \
        , typename ActiveImpl                                                  \
        >                                                                      \
class BOOST_PP_CAT( unordered_active_binary_operation_, name )                 \
  : BOOST_PP_CAT                                                               \
    (                                                                          \
      BOOST_PP_CAT( unordered_active_binary_operation_, name )                 \
    , _base                                                                    \
    )                                                                          \
    < typename ordered_arithmetic_types< Type0, Type1 >::first_type            \
    , typename ordered_arithmetic_types< Type0, Type1 >::second_type           \
    , ReturnType                                                               \
    , ActiveImpl                                                               \
    >                                                                          \
{                                                                              \
}; 

#define BOOST_ACTIVE_DETAIL_MAKE_OP_TEMPLATE_PREDICATE( r, ops )               \
BOOST_PP_NOT_EQUAL( BOOST_PP_ARRAY_SIZE( ops ), 0 ) 

#define BOOST_ACTIVE_DETAIL_MAKE_OP_TEMPLATE_OP( r, ops )                      \
BOOST_PP_ARRAY_POP_FRONT( ops ) 

#define BOOST_ACTIVE_DETAIL_MAKE_OP_TEMPLATE_BODY( r, ops )                    \
BOOST_ACTIVE_DETAIL_MAKE_UNORDERED_BINARY_OPERATION_BASE                       \
(                                                                              \
  BOOST_PP_SEQ_ELEM( 0, BOOST_PP_ARRAY_ELEM( 0, ops ) )                        \
, BOOST_PP_SEQ_ELEM( 1, BOOST_PP_ARRAY_ELEM( 0, ops ) )                        \
) 

BOOST_PP_FOR( boost_DETAIL_ARITHMETIC_OPERATORS()
            , BOOST_ACTIVE_DETAIL_MAKE_OP_TEMPLATE_PREDICATE
            , BOOST_ACTIVE_DETAIL_MAKE_OP_TEMPLATE_OP
            , BOOST_ACTIVE_DETAIL_MAKE_OP_TEMPLATE_BODY
            )

#define BOOST_ACTIVE_DETAIL_INHERIT_POLYOP(main_type,other_type,name,impl)     \
BOOST_PP_CAT( unordered_active_binary_operation_, name )                       \
< main_type, other_type                                                        \
, typename ::boost::usual_arithmetic_conversions< main_type, other_type >::type\
, impl                                                                         \
>, 

#define BOOST_ACTIVE_DETAIL_INHERIT_OP_MACRO(r,main_a_name_a_impl,element)     \
BOOST_ACTIVE_DETAIL_INHERIT_POLYOP                                             \
(                                                                              \
  BOOST_PP_TUPLE_ELEM( 3, 0, main_a_name_a_impl )                              \
, element                                                                      \
, BOOST_PP_TUPLE_ELEM( 3, 1, main_a_name_a_impl)                               \
, BOOST_PP_TUPLE_ELEM( 3, 2, main_a_name_a_impl)                               \
) 

#define BOOST_ACTIVE_DETAIL_OP( r, name )                                      \
template< typename ArithmeticType                                              \
        , typename ActiveImpl                                                  \
        >                                                                      \
class BOOST_PP_CAT( active_arithmetic_operation_base_, name )                  \
  : BOOST_PP_SEQ_FOR_EACH_R( r                                                 \
                           , BOOST_ACTIVE_DETAIL_INHERIT_OP_MACRO              \
                           , ( ArithmeticType, name, ActiveImpl )              \
                           , boost_DETAIL_VQ_ARITHMETIC_TYPES()                \
                           )                                                   \
    empty_base< BOOST_PP_CAT( active_arithmetic_operation_base_, name )        \
                < ArithmeticType, ActiveImpl >                                 \
              >                                                                \
{                                                                              \
}; 

#define BOOST_ACTIVE_DETAIL_CREATE_OPS( r, element )                           \
BOOST_ACTIVE_DETAIL_OP( r                                                      \
                      , BOOST_PP_SEQ_ELEM( 0                                   \
                                         , BOOST_PP_ARRAY_ELEM(0,element)      \
                                         )                                     \
                      ) 

#define BOOST_ACTIVE_DETAIL_OP_CREATOR_PREDICATE( r, state )                   \
BOOST_PP_NOT_EQUAL( BOOST_PP_ARRAY_SIZE( state ), 0 ) 

#define BOOST_ACTIVE_DETAIL_OP_CREATOR_OP( r, state )                          \
BOOST_PP_ARRAY_POP_FRONT( state ) 

BOOST_PP_FOR( boost_DETAIL_ARITHMETIC_OPERATORS()
            , BOOST_ACTIVE_DETAIL_OP_CREATOR_PREDICATE
            , BOOST_ACTIVE_DETAIL_OP_CREATOR_OP
            , BOOST_ACTIVE_DETAIL_CREATE_OPS
            )

#define BOOST_ACTIVE_DETAIL_OP_BASE( name, arithmetic_type, impl )             \
BOOST_PP_CAT( active_arithmetic_operation_base_, name )                        \
< arithmetic_type, impl >, 

#define BOOST_ACTIVE_DETAIL_OP_BASE_MACRO( arithmetic_type_a_impl, op )        \
BOOST_ACTIVE_DETAIL_OP_BASE                                                    \
(                                                                              \
  BOOST_PP_SEQ_ELEM( 0, op )                                                   \
, BOOST_PP_TUPLE_ELEM( 2, 0, arithmetic_type_a_impl )                          \
, BOOST_PP_TUPLE_ELEM( 2, 1, arithmetic_type_a_impl )                          \
) 

#define BOOST_ACTIVE_DETAIL_OP_INSTANTIATOR_PREDICATE( r, state )              \
BOOST_PP_NOT_EQUAL( BOOST_PP_ARRAY_SIZE( BOOST_PP_TUPLE_ELEM(2,0,state) )      \
                  , 0                                                          \
                  ) 

#define BOOST_ACTIVE_DETAIL_OP_INSTANTIATOR_OP( r, state )                     \
( BOOST_PP_ARRAY_POP_FRONT( BOOST_PP_TUPLE_ELEM( 2, 0, state ) )               \
, BOOST_PP_TUPLE_ELEM( 2, 1, state )                                           \
)

#define BOOST_ACTIVE_DETAIL_OP_INSTANTIATOR_BODY( r, state )                   \
BOOST_ACTIVE_DETAIL_OP_BASE_MACRO                                              \
(                                                                              \
  BOOST_PP_TUPLE_ELEM( 2, 1, state )                                           \
, BOOST_PP_ARRAY_ELEM( 0, BOOST_PP_TUPLE_ELEM( 2, 0, state ) )                 \
) 

template< typename ArithmeticType
        , typename ActiveImpl
        >
class active_interface_arithmetic_operations
  : BOOST_PP_FOR( ( boost_DETAIL_ARITHMETIC_OPERATORS()
                  , ( ArithmeticType, ActiveImpl )
                  )
                , BOOST_ACTIVE_DETAIL_OP_INSTANTIATOR_PREDICATE
                , BOOST_ACTIVE_DETAIL_OP_INSTANTIATOR_OP
                , BOOST_ACTIVE_DETAIL_OP_INSTANTIATOR_BODY
                )
    empty_base< active_interface_arithmetic_operations
                  < ArithmeticType, ActiveImpl >
              >
{
};

}
}
}

#endif
