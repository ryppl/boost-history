/*==============================================================================
    Copyright (c) 2010 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_AUTO_FUNCTION_DETAIL_PARAM_TEMPLATES_HPP
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_TEMPLATES_HPP

#include <boost/auto_function/support.hpp>

#ifndef BOOST_NO_BOOST_AUTO_FUNCTION

#include <boost/auto_function/limits.hpp>
#include <boost/config.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/empty_base.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/type_traits/is_void.hpp>
#include <boost/type_traits/remove_reference.hpp>

#if 0

#if !(    BOOST_NO_BOOST_AUTO_FUNCTION_PARAM( auto if )                        \
       && BOOST_NO_BOOST_AUTO_FUNCTION_PARAM( auto not )                       \
     )
#include <boost/mpl/apply.hpp>
#endif

#else

#include <boost/mpl/apply.hpp>

#endif

#ifndef BOOST_NO_RVALUE_REFERENCES
#include <boost/type_traits/add_rvalue_reference.hpp>
#else
#include <boost/type_traits/add_reference.hpp>
#endif

// Headers needed if we have to simulate variadic templates
#ifdef BOOST_NO_VARIADIC_TEMPLATES

#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>

#endif // End check for variadic templates

// ToDo: Use is_convertible to bool checks in the SFINAE conditions for the
//       metafunction versions of the if, not, auto if, and auto not arguments

// ToDo: Implement auto family of argument types

namespace boost { namespace auto_function { namespace detail {

// Used for triggering SFINAE
template< class DummyType, class OtherDummyType = void >
struct always_void { typedef void type; };

// Used for triggering SFINAE
template< bool DummyBoolValue >
struct always_void_by_bool { typedef void type; };

// Used for triggering SFINAE
template< class AlwaysType, class DummyType = void >
struct always { typedef AlwaysType type; };

//Used to specify a condition that is required for an auto function
template< bool Value >
struct if_ {};

template<>
struct if_< true > { typedef void type; };

////////////////////////////////////////////////////////////////////////////////
// Here's where some magic happens... the BOOST_AUTO_FUNCTION macro allows    //
// "if" to take either a value or a type, and automatically handles both      //
// without the user having to tell the macro which one it is passing. The     //
// trick I use internally relies on the fact that bool( arg ) is a function   //
// type when "arg" is a type, and a compile-time constant bool when "arg" is a//
// compile-time constant convertible to bool. So, if I make two function      //
// template overloads, one which takes a compile-time bool as a template      //
// argument, and another that takes a type, I can make the expression         //
// "decltype( make_if< bool( arg ) >() )" "do the right thing" in both        //
// cases. Further, since "arg" is now parenthesized, expressions that have a >//
// such as "sizeof( left ) > sizeof( right )" won't incorrectly end the       //
// template argument list.                                                    //
////////////////////////////////////////////////////////////////////////////////
template< bool Value >
typename if_< Value >::type
your_if_condition_isnt_dependent_on_a_function_template_argument();

template< class FunctionType, class Enabler = void >
struct result_of_if_check;

template< class BoolMetavalue >
struct result_of_if_check
       < bool ( BoolMetavalue )
       , typename always_void_by_bool< BoolMetavalue::value >::type
       >
  : if_< BoolMetavalue::value > {};

template< class ValueAsFunctionParam >
typename result_of_if_check< ValueAsFunctionParam >::type
your_if_condition_isnt_dependent_on_a_function_template_argument();

//template< bool Value >
//typename if_< Value >::type perform_auto_if_check();

template< class ReturnType, class FunctionType, class Enabler = void >
struct result_of_perform_auto_if_check;

template< class ReturnType, class BoolMetafunction >
struct result_of_perform_auto_if_check
       < ReturnType, bool ( BoolMetafunction )
       , typename always_void_by_bool
         < ::boost::mpl::apply< BoolMetafunction, ReturnType >::type::value >
         ::type
       >
  : if_< ::boost::mpl::apply< BoolMetafunction, ReturnType >::type::value > {};

template< class ReturnType, class ValueAsFunctionParam >
typename result_of_perform_auto_if_check
< ReturnType, ValueAsFunctionParam >::type
perform_auto_if_check();

template< class ReturnType, class FunctionType, class Enabler = void >
struct result_of_perform_auto_not_check;

template< class ReturnType, class BoolMetafunction >
struct result_of_perform_auto_not_check
       < ReturnType, bool ( BoolMetafunction )
       , typename always_void_by_bool
         < ::boost::mpl::apply< BoolMetafunction, ReturnType >::type::value >
         ::type
       >
  : if_< !::boost::mpl::apply< BoolMetafunction, ReturnType >::type::value > {};

template< class ReturnType, class ValueAsFunctionParam >
typename result_of_perform_auto_not_check
< ReturnType, ValueAsFunctionParam >::type
perform_auto_not_check();

template< bool Value >
typename if_< !Value >::type
your_not_condition_isnt_dependent_on_a_function_template_argument();

template< class FunctionType, class Enabler = void >
struct result_of_perform_not_check;

template< class BoolMetavalue >
struct result_of_perform_not_check
       < bool ( BoolMetavalue )
       , typename always_void_by_bool< BoolMetavalue::value >::type
       >
  : if_< !BoolMetavalue::value > {};

template< class ValueAsFunctionParam >
typename result_of_perform_not_check< ValueAsFunctionParam >::type
your_not_condition_isnt_dependent_on_a_function_template_argument();

#ifndef BOOST_NO_VARIADIC_TEMPLATES

// Used with decltype in return types to get SFINAE
template< class... T >
struct types;

#else

// Used with decltype in return types to get SFINAE
template< BOOST_PP_ENUM_BINARY_PARAMS
          ( BOOST_PP_INC( BOOST_AUTO_FUNCTION_MAX_REQUIRES_EXPRESSIONS )
          , T, = void BOOST_PP_INTERCEPT
          )
        >
struct types;

#endif // End no variadic template check

template< class T >
struct perform_try_check;

template< class ReturnType >
ReturnType impl_explicit_type();

template< class ReturnTypeMetafunction >
typename ReturnTypeMetafunction::type impl_virtual_explicit_type();

template< class PossibleReferenceType >
typename remove_reference< PossibleReferenceType >::type
impl_return_value_type();

template< class DummyType >
typename always_void< DummyType >::type impl_return_void_type();

template< class ReturnType, class SFINAEType >
typename always< ReturnType, SFINAEType >::type impl_explicit();

template< class ReturnType, class SFINAEType >
typename always< ReturnType, SFINAEType >::type impl_do();

#ifndef BOOST_NO_RVALUE_REFERENCES

template< typename T >
struct return_expression_type : add_rvalue_reference< T > {};

#else

// ToDo: Possibly change this
template< typename T >
struct return_expression_type : add_reference< T > {};

#endif

template< class RawDoReturnType, class ExplicitType >
struct is_raw_do_convertible_to_explicit_nonvoid
  : is_convertible< typename return_expression_type< RawDoReturnType >::type
                  , ExplicitType
                  > {};

template< class RawDoReturnType, class ExplicitType >
struct is_raw_do_convertible_to_explicit
  : mpl::if_< is_void< ExplicitType >
            , mpl::true_
            , is_raw_do_convertible_to_explicit_nonvoid< RawDoReturnType
                                                       , ExplicitType
                                                       >
            >::type {};

// A metafunction to determine if a 1-line return is compatible with an
// explicitly-specified explicit_
template< class ExplicitType, class RawDoReturnType >
struct is_valid_explicit_for_raw_do
  : mpl::and_
    < mpl::equal_to< is_void< ExplicitType >, is_void< RawDoReturnType > >
    , is_raw_do_convertible_to_explicit< RawDoReturnType, ExplicitType >
    > {};

template< class ExplicitType, class RawDoReturnType, class SFINAEType >
struct impl_explicit_and_do_return_type
  : mpl::if_
    < is_valid_explicit_for_raw_do< ExplicitType, RawDoReturnType >
    , mpl::identity< ExplicitType >
    , mpl::empty_base
    >::type {};

template< class ExplicitType, class RawDoReturnType, class SFINAEType >
typename impl_explicit_and_do_return_type
< ExplicitType, RawDoReturnType, SFINAEType >::type
impl_explicit_and_do();

} } }

#endif // End check for BOOST_AUTO_FUNCTION support

#endif // BOOST_AUTO_FUNCTION_DETAIL_PARAM_TEMPLATES_HPP
