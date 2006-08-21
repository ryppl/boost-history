//  Copyright (c) 2006, Matthew Calabrese
//
//  This code may be used under either of the following two licences:
//
//    Permission is hereby granted, free of charge, to any person
//    obtaining a copy of this software and associated documentation
//    files (the "Software"), to deal in the Software without
//    restriction, including without limitation the rights to use,
//    copy, modify, merge, publish, distribute, sublicense, and/or
//    sell copies of the Software, and to permit persons to whom the
//    Software is furnished to do so, subject to the following
//    conditions:
//
//    The above copyright notice and this permission notice shall be
//    included in all copies or substantial portions of the Software.
//
//    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//    OTHER DEALINGS IN THE SOFTWARE. OF SUCH DAMAGE.
//
//  Or:
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_ACT_ACTIVE_UTILITY_HPP
#define BOOST_ACT_ACTIVE_UTILITY_HPP

#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/mpl/deque.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/repetition/enum_trailing_binary_params.hpp>
#include <boost/preprocessor/repetition/enum.hpp>

#include "../detail/active_call_enablers.hpp"
#include "../detail/type_from_function_params.hpp"
#include "../detail/param_list_container.hpp"
#include "../action.hpp"
#include "../action/as_target.hpp"


namespace boost
{
namespace act
{
namespace detail
{

template< typename FunctionType, typename DummyType >
struct dummy_get_result_type
{
  typedef typename function_traits< FunctionType >::result_type type;
};

template< ::std::size_t OpID, ::std::size_t OverloadID >
struct operator_overload_tag_type
{
};

struct operation_identifier
{
  enum {
         // Note: new, delete, arrow, and function call operators excluded

         // Prefix unary ops
         positive = 1, negative, address_of, dereference, compliment, not_
       , pre_increment, pre_decrement

         // Postfix unary ops
       , post_increment, post_decrement

         // Binary ops
       , plus, minus, times, divide, mod
       , plus_assign, minus_assign, times_assign, divide_assign, mod_assign
       , xor_, bit_and, bit_or
       , left_shift, right_shift
       , xor_assign, bit_and_assign, bit_or_assign
       , left_shift_assign, right_shift_assign
       , and_, or_
       , comma, arrow_star
       , assign, index
       , less, greater, less_equal, greater_equal, equal_to, not_equal_to
       };

  // Prefix unary ops
  char (&operator +() const)[positive];
  char (&operator -() const)[negative];
  char (&operator &() const)[address_of];
  char (&operator *() const)[dereference];
  char (&operator ~() const)[compliment];
  char (&operator !() const)[not_];
  char (&operator ++() const)[pre_increment];
  char (&operator --() const)[pre_decrement];


  // Postfix unary ops
  char (&operator ++(int) const)[post_increment];
  char (&operator --(int) const)[post_decrement];

  // Binary ops

  char (&operator +(bool) const)[plus];
  char (&operator -(bool) const)[minus];
  char (&operator *(bool) const)[times];
  char (&operator /(bool) const)[divide];
  char (&operator %(bool) const)[mod];
  char (&operator +=(bool) const)[plus_assign];
  char (&operator -=(bool) const)[minus_assign];
  char (&operator *=(bool) const)[times_assign];
  char (&operator /=(bool) const)[divide_assign];
  char (&operator %=(bool) const)[mod_assign];
  char (&operator ^(bool) const)[xor_];
  char (&operator &(bool) const)[bit_and];
  char (&operator |(bool) const)[bit_or];
  char (&operator <<(bool) const)[left_shift];
  char (&operator >>(bool) const)[right_shift];
  char (&operator ^=(bool) const)[xor_assign];
  char (&operator &=(bool) const)[bit_and_assign];
  char (&operator |=(bool) const)[bit_or_assign];
  char (&operator <<=(bool) const)[left_shift_assign];
  char (&operator >>=(bool) const)[right_shift_assign];
  char (&operator &&(bool) const)[and_];
  char (&operator ||(bool) const)[or_];
  char (&operator ,(bool) const)[comma];
  char (&operator ->*(bool) const)[arrow_star];
  char (&operator =(bool) const)[assign];
  char (&operator [](bool) const)[index];
  char (&operator <(bool) const)[less];
  char (&operator >(bool) const)[greater];
  char (&operator <=(bool) const)[less_equal];
  char (&operator >=(bool) const)[greater_equal];
  char (&operator ==(bool) const)[equal_to];
  char (&operator !=(bool) const)[not_equal_to];

  static operation_identifier instance;
};

#define BOOST_ACT_DETAIL_UNARY_OP_ID( op )                                     \
sizeof( op ::boost::act::detail::operation_identifier::instance )

#define BOOST_ACT_DETAIL_POST_UNARY_OP_ID( op )                                \
sizeof( ::boost::act::detail::operation_identifier::instance op )

#define BOOST_ACT_DETAIL_BINARY_OP_ID( op )                                    \
sizeof( ::boost::act::detail::operation_identifier::instance op true )

#define BOOST_ACT_DETAIL_BINARY_OP_TAG_TYPE( op, overload_id )                 \
::boost::act::detail::operator_overload_tag_type                               \
  < BOOST_ACT_DETAIL_BINARY_OP_ID( op ), overload_id >

#define BOOST_ACT_DETAIL_BINARY_OP_TAG( op, overload_id )                      \
BOOST_ACT_DETAIL_BINARY_OP_TAG_TYPE( op, overload_id )()

}
}
}

#define BOOST_ACT_DETAIL_MEM_FUNC_RETURN_TYPE( ret )                           \
typename ::boost::mpl::apply                                                   \
<                                                                              \
  typename ActiveDetails::return_meta                                          \
, typename ::boost::act::detail::type_from_function_params                     \
    < void ret, ActiveDetails >::type                                          \
, typename ActiveDetails::act_model                                            \
>                                                                              \
::type

// ToDo: Act Model
#define BOOST_ACT_DETAIL_MEM_FUNC_HEADER( ret, num_params, params )            \
template< BOOST_PP_ENUM_PARAMS( num_params, typename Param ) >                 \
typename detail::active_call_enabler                                           \
<                                                                              \
  typename ActiveDetails::act_model                                            \
, typename ::boost::act::detail::type_from_function_params                     \
    < void ret, ActiveDetails >::type                                          \
  ( BOOST_ACT_DETAIL_ENUM_PLIST( ( 2, ( params, 1 ) ) ) )                      \
, ::boost::mpl::deque                                                          \
    < BOOST_PP_ENUM_BINARY_PARAMS( num_params                                  \
                                 , Param                                       \
                                 , const& BOOST_PP_INTERCEPT                   \
                                 )                                             \
    >                                                                          \
>                                                                              \
::type

// ToDo: Somehow get act model 
#define BOOST_ACT_DETAIL_EMPTY_MEM_FUNC_HEADER( ret, dummy1, dummy2 )          \
BOOST_ACT_DETAIL_MEM_FUNC_RETURN_TYPE( ret )

#define BOOST_ACT_ACTIVE_MEM_FUN_PARAM_MACRO( z, param_index, params )         \
BOOST_PP_CAT( Param, param_index ) const&                                      \
BOOST_ACT_DETAIL_PLIST_NAME( param_index, params )

#define BOOST_ACT_ACTIVE_MEM_FUN_PARAM_IMPL_MACRO( z, param_index, params )    \
BOOST_PP_CAT( Param, param_index ) &                                           \
BOOST_ACT_DETAIL_PLIST_NAME( param_index, params )

#define BOOST_ACT_DETAIL_PARAM_NAME( z, param_index, params )                  \
BOOST_ACT_DETAIL_PLIST_NAME( param_index, params )

#define BOOST_ACT_DETAIL_FUN_PARAM( z, param_index, params )                   \
BOOST_PP_CAT( Param, param_index ) &                                           \
BOOST_ACT_DETAIL_PLIST_NAME( param_index, params )

// ToDo: Handle asynchronous and synchronous differently
//       when working with the default act model
//       (disallow non-polymorphic thread safety)

// ToDo: Use metafunction
//       Make version for functions returning void
//       Account for reference or const reference differently
//       Assert internally if invalid
//       Remove enabler on nullary member function
//       Account for z
//       Make a version of function traits which takes a dummy template param
//       Fix function_type
#define BOOST_ACT_ACTIVE_MEM_FUN_OVERLOAD(id,ret,name,num,params,qualifier)    \
struct BOOST_PP_CAT(BOOST_PP_CAT(BOOST_PP_CAT(detail_,name),id),struct)        \
{                                                                              \
  typedef typename ::boost::act::detail::type_from_function_params             \
            < void ret, ActiveDetails >::type result_type;                     \
                                                                               \
  template< typename TargetType                                                \
            BOOST_PP_ENUM_TRAILING_PARAMS( num, typename Param )               \
          >                                                                    \
  typename ::boost::act::detail::type_from_function_params                     \
    < void ret, TargetType >::type                                             \
  operator ()( TargetType& inactive_this                                       \
               BOOST_PP_ENUM_TRAILING_BINARY_PARAMS( num, Param, & arg )       \
             ) const                                                           \
  {                                                                            \
    return BOOST_PP_CAT(BOOST_PP_CAT(BOOST_PP_CAT(detail_,name),id),_impl)     \
             ( inactive_this BOOST_PP_ENUM_TRAILING_PARAMS( num, arg ) );      \
  }                                                                            \
};                                                                             \
                                                                               \
BOOST_PP_IF( num                                                               \
           , BOOST_ACT_DETAIL_MEM_FUNC_HEADER                                  \
           , BOOST_ACT_DETAIL_EMPTY_MEM_FUNC_HEADER                            \
           )                                                                   \
           ( ret, num, params )                                                \
name( BOOST_PP_ENUM(num,BOOST_ACT_ACTIVE_MEM_FUN_PARAM_MACRO,params))qualifier \
{                                                                              \
  return BOOST_ACT_DETAIL_MEM_FUNC_RETURN_TYPE( ret )                          \
         (                                                                     \
           ::boost::act::as_target                                             \
             ( static_cast< typename ActiveDetails::child_type qualifier& >    \
                 ( *this )                                                     \
             )                                                                 \
         , BOOST_PP_CAT(BOOST_PP_CAT(BOOST_PP_CAT(detail_,name),id),struct)()  \
           BOOST_PP_ENUM_TRAILING( num, BOOST_ACT_DETAIL_PARAM_NAME, params )  \
         );                                                                    \
}                                                                              \
                                                                               \
template< typename TargetType                                                  \
          BOOST_PP_ENUM_TRAILING_PARAMS( num, typename Param )                 \
        >                                                                      \
static                                                                         \
typename ::boost::act::detail::type_from_function_params                       \
  < void ret, TargetType >::type                                               \
BOOST_PP_CAT(BOOST_PP_CAT(BOOST_PP_CAT(detail_,name),id),_impl)                \
( TargetType* const inactive_this                                              \
  BOOST_PP_ENUM_TRAILING( num, BOOST_ACT_DETAIL_FUN_PARAM, params )            \
)

#define BOOST_ACT_BIN_OP_FRIEND_OVERLOADS_DECL(op,ret,num,params,target)       \
struct BOOST_PP_CAT(BOOST_PP_CAT(detail_,name),struct)                         \
{                                                                              \
  typedef typename ::boost::act::detail::type_from_function_params             \
            < void ret, ActiveDetails >::type result_type;                     \
                                                                               \
  template< typename TargetType                                                \
            BOOST_PP_ENUM_TRAILING_PARAMS( num, typename Param )               \
          >                                                                    \
  typename ::boost::act::detail::type_from_function_params                     \
    < void ret, TargetType >::type                                             \
  operator ()( BOOST_PP_ENUM_BINARY_PARAMS( num, Param, & arg ) ) const        \
  {                                                                            \
    return BOOST_PP_CAT(BOOST_PP_CAT(detail_,name),_impl)                      \
             ( BOOST_PP_ENUM_PARAMS( num, arg ) );                             \
  }                                                                            \
};                                                                             \
                                                                               \
friend                                                                         \
BOOST_PP_IF( num                                                               \
           , BOOST_ACT_DETAIL_MEM_FUNC_HEADER                                  \
           , BOOST_ACT_DETAIL_EMPTY_MEM_FUNC_HEADER                            \
           )                                                                   \
           ( ret, num, params )                                                \
operator op( BOOST_PP_ENUM( num, BOOST_ACT_ACTIVE_MEM_FUN_PARAM_MACRO, params))\
{                                                                              \
  return BOOST_ACT_DETAIL_MEM_FUNC_RETURN_TYPE( ret )                          \
         (                                                                     \
           ::boost::act::as_target                                             \
             ( static_cast< typename ActiveDetails::child_type qualifier& >    \
                 ( *this )                                                     \
             )                                                                 \
         , BOOST_PP_CAT(BOOST_PP_CAT(BOOST_PP_CAT(detail_,name),id),struct)()  \
           BOOST_PP_ENUM_TRAILING( num, BOOST_ACT_DETAIL_PARAM_NAME, params )  \
         );                                                                    \
} 

#define BOOST_ACT_BIN_OP_FRIEND_OVERLOAD_DECL_IMPL(ret,op,num,params,target)   \
/* ToDo: Change to result metafun */                                           \
typedef typename ::boost::act::detail::type_from_function_params               \
          < void ret, ActiveDetails >::type result_type;                       \
                                                                               \
template< typename TargetType                                                  \
          BOOST_PP_ENUM_TRAILING_PARAMS( num, typename Param )                 \
        >                                                                      \
typename ::boost::act::detail::type_from_function_params                       \
  < void ret, TargetType >::type                                               \
operator ()( BOOST_PP_ENUM_BINARY_PARAMS( num, Param, & arg ) ) const          \
{                                                                              \
  return BOOST_PP_CAT(BOOST_PP_CAT(detail_,name),_impl)                        \
           ( BOOST_PP_ENUM_PARAMS( num, arg ) );                               \
}                                                                              \
};                                                                             \
                                                                               \
friend                                                                         \
BOOST_PP_IF( num                                                               \
           , BOOST_ACT_DETAIL_MEM_FUNC_HEADER                                  \
           , BOOST_ACT_DETAIL_EMPTY_MEM_FUNC_HEADER                            \
           )                                                                   \
           ( ret, num, params )                                                \
operator op( BOOST_PP_ENUM( num, BOOST_ACT_ACTIVE_MEM_FUN_PARAM_MACRO, params))\
{                                                                              \
  return BOOST_ACT_DETAIL_MEM_FUNC_RETURN_TYPE( ret )                          \
         (                                                                     \
           ::boost::act::as_target                                             \
             ( static_cast< typename ActiveDetails::child_type qualifier& >    \
                 ( *this )                                                     \
             )                                                                 \
         , BOOST_PP_CAT(BOOST_PP_CAT(BOOST_PP_CAT(detail_,name),id),struct)()  \
           BOOST_PP_ENUM_TRAILING( num, BOOST_ACT_DETAIL_PARAM_NAME, params )  \
         );                                                                    \
} 


#define BOOST_ACT_DETAIL_NO_QUALIFIER 

#define BOOST_ACT_ACTIVE_MEM_FUN( ret, name, params )                          \
BOOST_ACT_ACTIVE_MEM_FUN_OVERLOAD( detail_act_overload                         \
                          , ret                                                \
                          , name                                               \
                          , BOOST_ACT_DETAIL_PLIST_SIZE( params )              \
                          , params                                             \
                          , BOOST_ACT_DETAIL_NO_QUALIFIER                      \
                          )

#define BOOST_ACT_ACTIVE_CONST_MEM_FUN( ret, name, params )                    \
BOOST_ACT_ACTIVE_MEM_FUN_OVERLOAD( detail_act_overload                         \
                          , ret                                                \
                          , name                                               \
                          , BOOST_ACT_DETAIL_PLIST_SIZE( params )              \
                          , params                                             \
                          , const                                              \
                          )

//#define BOOST_ACT_CHAIN_MEM_FUN(child,num,ret,name,params,model)               \


#endif
