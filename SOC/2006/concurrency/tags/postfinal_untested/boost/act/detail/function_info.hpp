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

#ifndef BOOST_ACT_DETAIL_FUNCTION_HPP
#define BOOST_ACT_DETAIL_FUNCTION_HPP

/*
  info format: ( object id, data )
*/

#define BOOST_ACT_DETAIL_MACRO_CALLER( macro, params ) macro params

#define BOOST_ACT_OBJ_ID( obj ) BOOST_PP_TUPLE_ELEM( 2, 0, obj )
#define BOOST_ACT_OBJ_DATA( obj ) BOOST_PP_TUPLE_ELEM( 2, 1, obj )

#define BOOST_ACT_OBJ_FUN_NAME( obj, fun_id )                                  \
BOOST_PP_CAT( BOOST_PP_CAT( BOOST_ACT_OBJ_ID( obj ), _ ), fun_id )

#define BOOST_ACT_VFUN( obj, fun_id, params )                                  \
BOOST_ACT_DETAIL_MACRO_CALLER                                                  \
( BOOST_ACT_OBJ_FUN_NAME( obj, fun_id )                                        \
, BOOST_PP_SEQ_TO_TUPLE                                                        \
    ( BOOST_ACT_SEQ_PUSH_FRONT( params, BOOST_ACT_OBJ_DATA( obj ) ) )          \
)

#define BOOST_ACT_NULLARY_VFUN( obj, fun_id )                                  \
BOOST_ACT_DETAIL_MACRO_CALLER                                                  \
( BOOST_ACT_OBJ_FUN_NAME( obj, fun_id )                                        \
, BOOST_ACT_OBJ_DATA( obj )                                                    \
)

// virtual functions
/*
DECL_HEADER( data )
DECL_HEADER_T( data )
IMPL_HEADER( data )
IMPL_HEADER_T( data )
CHANGE_NAME( data, name )
CHANGE_RETURN_TYPE( data, return_type )
TRANSFORM_PARAMS( data, trans_macro )
PUSH_FRONT_PARAM( data, param )
PUSH_BACK_PARAM( data, param )
POP_FRONT_PARAM( data )
POP_BACK_PARAM( data )
*/

#define BOOST_ACT_DECL_HEADER( obj )                                           \
BOOST_ACT_DETAIL_NULLARY_VFUN( obj, DECL_HEADER )

#define BOOST_ACT_DECL_HEADER_T( obj, dummy_type )                             \
BOOST_ACT_DETAIL_VFUN( obj, DECL_HEADER_T, (dummy_type) )

#define BOOST_ACT_IMPL_HEADER( obj )                                           \
BOOST_ACT_DETAIL_NULLARY_VFUN( obj, IMPL_HEADER )

#define BOOST_ACT_IMPL_HEADER_T( obj, dummy_type )                             \
BOOST_ACT_DETAIL_VFUN( obj, IMPL_HEADER_T, (dummy_type) )

#define BOOST_ACT_CHANGE_NAME( obj, name )                                     \
BOOST_ACT_DETAIL_VFUN( obj, CHANGE_NAME, (name) )

#define BOOST_ACT_CHANGE_RETURN_TYPE( obj, return_type )                       \
BOOST_ACT_DETAIL_VFUN( obj, CHANGE_RETURN_TYPE, (return_type) )

#define BOOST_ACT_TRANSFORM_PARAMS( obj, trans_macro )                         \
BOOST_ACT_DETAIL_VFUN( obj, TRANSFORM_PARAMS, (trans_macro) )

#define BOOST_ACT_DETAIL_NOTHING()
#define BOOST_ACT_DETAIL_CONST()    const
#define BOOST_ACT_DETAIL_VOLATILE() volatile
#define BOOST_ACT_DETAIL_CV()       volatile const
#define BOOST_ACT_DETAIL_STATIC()   static
#define BOOST_ACT_DETAIL_EXTERN()   extern
#define BOOST_ACT_DETAIL_INLINE()   inline

// Object declarations
#define BOOST_ACT_SPECIFIED_FUN( specifier, return_type, name, params )        \
( BOOST_ACT_FUN, (specifier)(return_type)(name)(params) )

#define BOOST_ACT_FUN( return_type, name, params )                             \
BOOST_ACT_SPECIFIED_FUN                                                        \
  ( BOOST_ACT_DETAIL_NOTHING, return_type, name, params )

#define BOOST_ACT_QUALIFIED_MEM_FUN( return_type,type,name,params,qualifier )  \
( BOOST_ACT_MEM_FUN                                                            \
, (BOOST_ACT_DETAIL_NOTHING)(return_type)(type)(name)(params)(qualifier)       \
)

#define BOOST_ACT_MEM_FUN( return_type, type, name, params )                   \
BOOST_ACT_QUALIFIED_MEM_FUN                                                    \
  ( return_type, type, name, params, BOOST_ACT_DETAIL_NOTHING )

#define BOOST_ACT_CONST_MEM_FUN( return_type, type, name, params )             \
BOOST_ACT_QUALIFIED_MEM_FUN                                                    \
  ( return_type, type, name, params, BOOST_ACT_DETAIL_CONST )

#define BOOST_ACT_STATIC_MEM_FUN( return_type, type, name, params )            \
( BOOST_ACT_MEM_FUN                                                            \
, (BOOST_ACT_DETAIL_STATIC)(return_type)(type)(name)(type)(params)             \
  (BOOST_ACT_DETAIL_NOTHING)                                                   \
)

#define BOOST_ACT_SPECIFIED_FUN_T( t_params,specifier,return_type,name,params )\
( BOOST_ACT_DETAIL_FUN_T, (t_params)(specifier)(return_type)(name)(params) )

#define BOOST_ACT_FUN_T( t_params, return_type, name, params )                 \
BOOST_ACT_SPECIFIED_FUN_T                                                      \
  ( t_params, BOOST_ACT_DETAIL_NOTHING, return_type, name, params )

#define BOOST_ACT_QUALIFIED_MEM_FUN_T(t_params,r_type,type,name,params,qual)   \
( BOOST_DETAIL_ACT_MEM_FUN_T                                                   \
, (t_params)(BOOST_ACT_DETAIL_NOTHING)(r_type)(type)(name)(params)(qual)       \
)

#define BOOST_ACT_MEM_FUN_T( t_params, return_type, type, name, params )       \
BOOST_ACT_QUALIFIED_MEM_FUN_T                                                  \
  ( t_params, return_type, type, name, params, BOOST_ACT_DETAIL_NOTHING )

#define BOOST_ACT_CONST_MEM_FUN_T( t_params, return_type, type, name, params ) \
BOOST_ACT_QUALIFIED_MEM_FUN_T                                                  \
  ( t_params, return_type, type, name, params, BOOST_ACT_DETAIL_CONST )

#define BOOST_ACT_STATIC_MEM_FUN_T( t_params, r_type, type, name, params )     \
( BOOST_ACT_DETAIL_MEM_FUN_T                                                   \
, (t_params)(BOOST_ACT_DETAIL_STATIC)(r_type)(type)(name)(params)              \
  (BOOST_ACT_DETAIL_NOTHING)                                                   \
)

#define BOOST_ACT_QUALIFIED_CONVERSION_OP( type, target_type, qualifier )      \
( BOOST_ACT_DETAIL_CONVERSION_OP                                               \
, (type)(target_type)(qualifier)                                               \
)

#define BOOST_ACT_CONVERSION_OP( type, target_type )                           \
BOOST_ACT_QUALIFIED_CONVERSION_OP( type, target_type, BOOST_ACT_DETAIL_NOTHING )

#define BOOST_ACT_CONST_CONVERSION_OP( type, target_type )                     \
BOOST_ACT_QUALIFIED_CONVERSION_OP( type, target_type, BOOST_ACT_DETAIL_CONST )

// Virtual preprocessor metafunction definitions
#define BOOST_ACT_DETAIL_FUN_HEADER( data )                                    \
BOOST_ACT_DETAIL_FUN_SPECIFIER( data )                                         \
::boost::act::detail::type_from_function_params                                \
  < void BOOST_ACT_DETAIL_FUN_RETURN_TYPE( data ) >::type                      \
BOOST_ACT_DETAIL_FUN_NAME( data )                                              \
  ( BOOST_ACT_DETAIL_ENUM_PLIST                                                \
      ( BOOST_ACT_SIMPLE_PLIST( BOOST_ACT_DETAIL_FUN_PARAMS( data ) ) ) )

// ToDo: Use dummy type for params too
#define BOOST_ACT_DETAIL_FUN_HEADER_T( data, dummy_type )                      \
BOOST_ACT_DETAIL_FUN_SPECIFIER( data )                                         \
typename ::boost::act::detail::type_from_function_params                       \
  < void BOOST_ACT_DETAIL_FUN_RETURN_TYPE( data ), dummy_type >::type          \
BOOST_ACT_DETAIL_FUN_NAME( data )                                              \
  ( BOOST_ACT_DETAIL_ENUM_PLIST                                                \
      ( BOOST_ACT_SIMPLE_PLIST( BOOST_ACT_DETAIL_FUN_PARAMS( data ) ) ) )

#define BOOST_ACT_DETAIL_NESTED_FUN_HEADER( data )                             \
BOOST_ACT_DETAIL_FUN_SPECIFIER( data ) friend                                  \
::boost::act::detail::type_from_function_params                                \
  < void BOOST_ACT_DETAIL_FUN_RETURN_TYPE( data ) >::type                      \
BOOST_ACT_DETAIL_FUN_NAME( data )                                              \
  ( BOOST_ACT_DETAIL_ENUM_PLIST                                                \
      ( BOOST_ACT_SIMPLE_PLIST( BOOST_ACT_DETAIL_FUN_PARAMS( data ) ) ) )

#define BOOST_ACT_DETAIL_MEM_FUN_HEADER( data )                                \
::boost::act::detail::type_from_function_params                                \
  < void BOOST_ACT_DETAIL_MEM_FUN_RETURN_TYPE( data ) >::type                  \
BOOST_ACT_DETAIL_MEM_FUN_TYPE( data )::BOOST_ACT_DETAIL_MEM_FUN_NAME( data )   \
  ( BOOST_ACT_DETAIL_ENUM_PLIST                                                \
      ( BOOST_ACT_SIMPLE_PLIST( BOOST_ACT_DETAIL_MEM_FUN_PARAMS( data ) ) ) )

#define BOOST_ACT_DETAIL_NESTED_MEM_FUN_HEADER( data )                         \
BOOST_ACT_DETAIL_MEM_FUN_SPECIFIER( data )                                     \
::boost::act::detail::type_from_function_params                                \
  < void BOOST_ACT_DETAIL_MEM_FUN_RETURN_TYPE( data ) >::type                  \
BOOST_ACT_DETAIL_MEM_FUN_NAME( data )                                          \
  ( BOOST_ACT_DETAIL_ENUM_PLIST                                                \
      ( BOOST_ACT_SIMPLE_PLIST( BOOST_ACT_DETAIL_MEM_FUN_PARAMS( data ) ) ) )  \
BOOST_ACT_DETAIL_MEM_FUN_QUALIFIER( data )

#define BOOST_ACT_DETAIL_NESTED_CONVERSION_OP_HEADER( data )                   \
operator ::boost::act::detail::type_from_function_params                       \
           < void BOOST_ACT_DETAIL_CONVERSION_OP_RETURN_TYPE( data ) >::type   \
BOOST_ACT_DETAIL_CONVERSION_OP_QUALIFIER( data )

#define BOOST_ACT_DETAIL_CONVERSION_OP_HEADER( data )                          \
BOOST_ACT_DETAIL_CONVERSION_OP_TYPE( data )                                    \
::operator ::boost::act::detail::type_from_function_params                     \
             < void BOOST_ACT_DETAIL_CONVERSION_OP_RETURN_TYPE( data ) >::type \
BOOST_ACT_DETAIL_CONVERSION_OP_QUALIFIER( data )

// Name changers
#define BOOST_ACT_DETAIL_FUN_CHANGE_NAME( data, name )                         \
BOOST_ACT_SPECIFIED_FUN( BOOST_ACT_DETAIL_FUN_SPECIFIER( data )                \
                       , BOOST_ACT_DETAIL_FUN_RETURN_TYPE( data )              \
                       , name                                                  \
                       , BOOST_ACT_DETAIL_FUN_PARAMS( data )                   \
                       )

#define BOOST_ACT_DETAIL_MEM_FUN_CHANGE_NAME( data, name )                     \
BOOST_ACT_MEM_FUN( BOOST_ACT_DETAIL_MEM_FUN_SPECIFIER( data )                  \
                 , BOOST_ACT_DETAIL_MEM_FUN_RETURN_TYPE( data )                \
                 , BOOST_ACT_DETAIL_MEM_FUN_TYPE( data )                       \
                 , name                                                        \
                 , BOOST_ACT_DETAIL_MEM_FUN_PARAMS( data )                     \
                 , BOOST_ACT_DETAIL_MEM_FUN_QUALIFIER( data )                  \
                 )

#define BOOST_ACT_DETAIL_FUN_T_CHANGE_NAME( data, name )                       \
BOOST_ACT_SPECIFIED_FUN_T( BOOST_ACT_DETAIL_FUN_T_T_PARAMS( data )             \
                         , BOOST_ACT_DETAIL_FUN_T_SPECIFIER( data )            \
                         , BOOST_ACT_DETAIL_FUN_T_RETURN_TYPE( data )          \
                         , name                                                \
                         , BOOST_ACT_DETAIL_FUN_T_PARAMS( data )               \
                         )

#define BOOST_ACT_DETAIL_MEM_FUN_T_CHANGE_NAME( data, name )                   \
BOOST_ACT_MEM_FUN_T( BOOST_ACT_DETAIL_MEM_FUN_T_T_PARAMS( data )               \
                   , BOOST_ACT_DETAIL_MEM_FUN_T_SPECIFIER( data )              \
                   , BOOST_ACT_DETAIL_MEM_FUN_T_RETURN_TYPE( data )            \
                   , BOOST_ACT_DETAIL_MEM_FUN_T_TYPE( data )                   \
                   , name                                                      \
                   , BOOST_ACT_DETAIL_MEM_FUN_T_PARAMS( data )                 \
                   , BOOST_ACT_DETAIL_MEM_FUN_T_QUALIFIER( data )              \
                   )

// Return Type changers
#define BOOST_ACT_DETAIL_FUN_CHANGE_RETURN_TYPE( data, return_type )           \
BOOST_ACT_SPECIFIED_FUN( BOOST_ACT_DETAIL_FUN_SPECIFIER( data )                \
                       , return_type                                           \
                       , BOOST_ACT_DETAIL_FUN_PARAMS_NAME( data )              \
                       , BOOST_ACT_DETAIL_FUN_PARAMS( data )                   \
                       )

#define BOOST_ACT_DETAIL_MEM_FUN_CHANGE_RETURN_TYPE( data, return_type )       \
BOOST_ACT_MEM_FUN( BOOST_ACT_DETAIL_MEM_FUN_SPECIFIER( data )                  \
                 , return_type                                                 \
                 , BOOST_ACT_DETAIL_MEM_FUN_TYPE( data )                       \
                 , BOOST_ACT_DETAIL_MEM_FUN_NAME( data )                       \
                 , BOOST_ACT_DETAIL_MEM_FUN_PARAMS( data )                     \
                 , BOOST_ACT_DETAIL_MEM_FUN_QUALIFIER( data )                  \
                 )

#define BOOST_ACT_DETAIL_FUN_T_CHANGE_RETURN_TYPE( data, return_type )         \
BOOST_ACT_SPECIFIED_FUN_T( BOOST_ACT_DETAIL_FUN_T_T_PARAMS( data )             \
                         , BOOST_ACT_DETAIL_FUN_T_SPECIFIER( data )            \
                         , return_type                                         \
                         , BOOST_ACT_DETAIL_FUN_T_TYPE( data )                 \
                         , BOOST_ACT_DETAIL_FUN_T_NAME( data )                 \
                         , BOOST_ACT_DETAIL_FUN_T_PARAMS( data )               \
                         )

#define BOOST_ACT_DETAIL_MEM_FUN_T_CHANGE_RETURN_TYPE( data, return_type )     \
BOOST_ACT_MEM_FUN_T( BOOST_ACT_DETAIL_MEM_FUN_T_T_PARAMS( data )               \
                   , BOOST_ACT_DETAIL_MEM_FUN_T_SPECIFIER( data )              \
                   , return_type                                               \
                   , BOOST_ACT_DETAIL_MEM_FUN_T_TYPE( data )                   \
                   , BOOST_ACT_DETAIL_MEM_FUN_T_NAME( data )                   \
                   , BOOST_ACT_DETAIL_MEM_FUN_T_PARAMS( data )                 \
                   , BOOST_ACT_DETAIL_MEM_FUN_T_QUALIFIER( data )              \
                   )

#define BOOST_ACT_DETAIL_CONVERSION_OP_CHANGE_RETURN_TYPE( data, return_type ) \
BOOST_ACT_QUALIFIED_CONVERSION_OP                                              \
( BOOST_ACT_DETAIL_CONVERSION_OP_TYPE( data )                                  \
, return_type                                                                  \
, BOOST_ACT_DETAIL_CONVERSION_OP_QUALIFIER( data )                             \
)

// Non-polymorphic accessors
#define BOOST_ACT_DETAIL_FUN_SPECIFIER( data )   BOOST_PP_SEQ_ELEM( 0, data )
#define BOOST_ACT_DETAIL_FUN_RETURN_TYPE( data ) BOOST_PP_SEQ_ELEM( 1, data )
#define BOOST_ACT_DETAIL_FUN_NAME( data )        BOOST_PP_SEQ_ELEM( 2, data )
#define BOOST_ACT_DETAIL_FUN_PARAMS( data )      BOOST_PP_SEQ_ELEM( 3, data )

#define BOOST_ACT_DETAIL_MEM_FUN_SPECIFIER( data )  BOOST_PP_SEQ_ELEM( 0, data )
#define BOOST_ACT_DETAIL_MEM_FUN_RETURN_TYPE( data )BOOST_PP_SEQ_ELEM( 1, data )
#define BOOST_ACT_DETAIL_MEM_FUN_TYPE( data )       BOOST_PP_SEQ_ELEM( 2, data )
#define BOOST_ACT_DETAIL_MEM_FUN_NAME( data )       BOOST_PP_SEQ_ELEM( 3, data )
#define BOOST_ACT_DETAIL_MEM_FUN_PARAMS( data )     BOOST_PP_SEQ_ELEM( 4, data )
#define BOOST_ACT_DETAIL_MEM_FUN_QUALIFIER( data )  BOOST_PP_SEQ_ELEM( 5, data )

#define BOOST_ACT_DETAIL_FUN_T_T_PARAMS( data )    BOOST_PP_SEQ_ELEM( 0, data )
#define BOOST_ACT_DETAIL_FUN_T_SPECIFIER( data )   BOOST_PP_SEQ_ELEM( 1, data )
#define BOOST_ACT_DETAIL_FUN_T_RETURN_TYPE( data ) BOOST_PP_SEQ_ELEM( 2, data )
#define BOOST_ACT_DETAIL_FUN_T_NAME( data )        BOOST_PP_SEQ_ELEM( 3, data )
#define BOOST_ACT_DETAIL_FUN_T_PARAMS( data )      BOOST_PP_SEQ_ELEM( 4, data )

#define BOOST_ACT_DETAIL_MEM_FUN_T_T_PARAMS( data ) BOOST_PP_SEQ_ELEM( 0, data )
#define BOOST_ACT_DETAIL_MEM_FUN_T_SPECIFIER( data )BOOST_PP_SEQ_ELEM( 1, data )
#define BOOST_ACT_DETAIL_MEM_FUN_T_TYPE(data)       BOOST_PP_SEQ_ELEM( 2, data )
#define BOOST_ACT_DETAIL_MEM_FUN_T_RETURN_TYPE(data)BOOST_PP_SEQ_ELEM( 3, data )
#define BOOST_ACT_DETAIL_MEM_FUN_T_NAME( data )     BOOST_PP_SEQ_ELEM( 4, data )
#define BOOST_ACT_DETAIL_MEM_FUN_T_PARAMS( data )   BOOST_PP_SEQ_ELEM( 5, data )
#define BOOST_ACT_DETAIL_MEM_FUN_T_QUALIFIER( data )BOOST_PP_SEQ_ELEM( 6, data )

#define BOOST_ACT_DETAIL_CONVERSION_OP_TYPE( data ) BOOST_PP_SEQ_ELEM( 0, data )

#define BOOST_ACT_DETAIL_CONVERSION_OP_RETURN_TYPE( data )                     \
BOOST_PP_SEQ_ELEM( 1, data )

#define BOOST_ACT_DETAIL_CONVERSION_OP_QUALIFIER( data )                       \
BOOST_PP_SEQ_ELEM( 2, data )

// Misc methods
#define BOOST_ACT_DETAIL_FRIEND_DECL_HEADER( data )
#define BOOST_ACT_DETAIL_FRIEND_IMPL_HEADER( data )

// Member function methods (ToDo: Define these)
#define BOOST_ACT_DETAIL_MEM_FUN_NESTED_DECL_HEADER( data )
#define BOOST_ACT_DETAIL_MEM_FUN_NESTED_IMPL_HEADER( data )



/*
template< BOOST_ACT_DETAIL_ENUM_PLIST( BOOST_ACT_SIMPLE_PLIST( t_params ) ) >  \
typename ::boost::act::detail::type_from_function_params                       \
           < void return_type, SurgeActDetailAlgoModel >::type                 \
BOOST_PP_CAT( name, _type )< SurgeActDetailAlgoModel >                         \
::operator ()( BOOST_ACT_DETAIL_ENUM_PLIST                                     \
               ( BOOST_ACT_SIMPLE_PLIST( params ) )                            \
             ) const                                                           \
*/
#endif
