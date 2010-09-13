///////////////////////////////////////////////////////////////////////////////
// Operator traits
//
// (C) Copyright Terje Slettebø and Tobias Schwinger 2004.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).
///////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_CT_OPERATOR_DETAIL_HAS_FUNCTION_CALL_OP_IMPL_HPP
#define BOOST_CT_OPERATOR_DETAIL_HAS_FUNCTION_CALL_OP_IMPL_HPP

#include <boost/type_traits/is_function.hpp>
#include <boost/type_traits/function_traits.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/eval.hpp>

namespace boost {
namespace detail {

///////////////////////////////////////////////////////////////////////////////
// function_arity
///////////////////////////////////////////////////////////////////////////////

template<class T>
struct function_arity :
  mpl::int_<function_traits<T>::arity> {};

///////////////////////////////////////////////////////////////////////////////
// is_function_and_arity_is
///////////////////////////////////////////////////////////////////////////////

template<class T,int Arity>
struct is_function_and_arity_is :
  mpl::and_<
    is_function<T>,
    mpl::equal_to<
      function_arity<T>,
      mpl::int_<Arity>
    >
  > {};

///////////////////////////////////////////////////////////////////////////////
// function_arg_type
///////////////////////////////////////////////////////////////////////////////

template<class T,int Arg>
struct function_arg_type;

template<class T>
struct function_arg_type<T,1> :
  mpl::identity<typename function_traits<T>::arg1_type> {};

template<class T>
struct function_arg_type<T,2> :
  mpl::identity<typename function_traits<T>::arg2_type> {};

template<class T>
struct function_arg_type<T,3> :
  mpl::identity<typename function_traits<T>::arg3_type> {};

template<class T>
struct function_arg_type<T,4> :
  mpl::identity<typename function_traits<T>::arg4_type> {};

template<class T>
struct function_arg_type<T,5> :
  mpl::identity<typename function_traits<T>::arg5_type> {};

template<class T>
struct function_arg_type<T,6> :
  mpl::identity<typename function_traits<T>::arg6_type> {};

template<class T>
struct function_arg_type<T,7> :
  mpl::identity<typename function_traits<T>::arg7_type> {};

template<class T>
struct function_arg_type<T,8> :
  mpl::identity<typename function_traits<T>::arg8_type> {};

template<class T>
struct function_arg_type<T,9> :
  mpl::identity<typename function_traits<T>::arg9_type> {};

template<class T>
struct function_arg_type<T,10> :
  mpl::identity<typename function_traits<T>::arg10_type> {};

///////////////////////////////////////////////////////////////////////////////
// function_result_type
///////////////////////////////////////////////////////////////////////////////

template<class T>
struct function_result_type :
  mpl::identity<typename function_traits<T>::result_type> {};

///////////////////////////////////////////////////////////////////////////////
// is_arg_convertible_to_parameter
///////////////////////////////////////////////////////////////////////////////

template<class F,class P,int Arg>
struct is_arg_convertible_to_parameter :
  mpl::eval<
    is_convertible<
      mpl::identity<P>,
      detail::function_arg_type<typename remove_reference<F>::type,Arg>
    >
  > {};

} // namespace detail
} // namespace boost

#endif // BOOST_CT_OPERATOR_DETAIL_HAS_FUNCTION_CALL_OP_IMPL_HPP
