///////////////////////////////////////////////////////////////////////////////
// Operator traits
//
// (C) Copyright Terje Slettebø and Tobias Schwinger 2004.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).
///////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_CT_OPERATOR_HAS_FUNCTION_CALL_OP_HPP
#define BOOST_CT_OPERATOR_HAS_FUNCTION_CALL_OP_HPP

#include <boost/type_traits/is_convertible.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/void.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/eval.hpp>
#include <boost/concept_traits/operator/detail/is_class_or_union.hpp>
#include <boost/concept_traits/operator/detail/has_function_call_op_impl.hpp>

namespace boost {

///////////////////////////////////////////////////////////////////////////////
// has_function_call_op
//
// The function call operator has to be a member function, so it can only be
// detected for classes and unions by specialising this trait. It gives default
// true for these.
///////////////////////////////////////////////////////////////////////////////

template<
  class T,class R,
  class P1 = mpl::void_,class P2 = mpl::void_,class P3 = mpl::void_,
  class P4 = mpl::void_,class P5 = mpl::void_,class P6 = mpl::void_,
  class P7 = mpl::void_,class P8 = mpl::void_,class P9 = mpl::void_,
  class P10 = mpl::void_
>
struct has_function_call_op :
  mpl::or_<
    detail::is_class_or_union_<typename remove_reference<T>::type>,
    mpl::and_<
      detail::is_function_and_arity_is<typename remove_reference<T>::type,10>,
      mpl::eval<
        is_convertible<
          mpl::identity<R>,
          detail::function_result_type<typename remove_reference<T>::type>
        >
      >,
      detail::is_arg_convertible_to_parameter<T,P1,1>,
      detail::is_arg_convertible_to_parameter<T,P2,2>,
      mpl::and_<
        detail::is_arg_convertible_to_parameter<T,P3,3>,
        detail::is_arg_convertible_to_parameter<T,P4,4>,
        detail::is_arg_convertible_to_parameter<T,P5,5>,
        detail::is_arg_convertible_to_parameter<T,P6,6>,
        mpl::and_<
          detail::is_arg_convertible_to_parameter<T,P7,7>,
          detail::is_arg_convertible_to_parameter<T,P8,8>,
          detail::is_arg_convertible_to_parameter<T,P9,9>,
          detail::is_arg_convertible_to_parameter<T,P10,10>
        >
      >
    >
  > {};

template<
  class T,class R
>
struct has_function_call_op<
  T,R,
  mpl::void_,mpl::void_,mpl::void_,
  mpl::void_,mpl::void_,mpl::void_,
  mpl::void_,mpl::void_,mpl::void_,
  mpl::void_
> :
  mpl::or_<
    detail::is_class_or_union_<typename remove_reference<T>::type>,
    mpl::and_<
      detail::is_function_and_arity_is<typename remove_reference<T>::type,0>,
      mpl::eval<
        is_convertible<
          mpl::identity<R>,
          detail::function_result_type<typename remove_reference<T>::type>
        >
      >
    >
  > {};

template<
  class T,class R,
  class P1
>
struct has_function_call_op<
  T,R,
  P1,mpl::void_,mpl::void_,
  mpl::void_,mpl::void_,mpl::void_,
  mpl::void_,mpl::void_,mpl::void_,
  mpl::void_
> :
  mpl::or_<
    detail::is_class_or_union_<typename remove_reference<T>::type>,
    mpl::and_<
      detail::is_function_and_arity_is<typename remove_reference<T>::type,1>,
      mpl::eval<
        is_convertible<
          mpl::identity<R>,
          detail::function_result_type<typename remove_reference<T>::type>
        >
      >,
      detail::is_arg_convertible_to_parameter<T,P1,1>
    >
  > {};

template<
  class T,class R,
  class P1,class P2
>
struct has_function_call_op<
  T,R,
  P1,P2,mpl::void_,
  mpl::void_,mpl::void_,mpl::void_,
  mpl::void_,mpl::void_,mpl::void_,
  mpl::void_
> :
  mpl::or_<
    detail::is_class_or_union_<typename remove_reference<T>::type>,
    mpl::and_<
      detail::is_function_and_arity_is<typename remove_reference<T>::type,2>,
      mpl::eval<
        is_convertible<
          mpl::identity<R>,
          detail::function_result_type<typename remove_reference<T>::type>
        >
      >,
      detail::is_arg_convertible_to_parameter<T,P1,1>,
      detail::is_arg_convertible_to_parameter<T,P2,2>
    >
  > {};

template<
  class T,class R,
  class P1,class P2,class P3
>
struct has_function_call_op<
  T,R,
  P1,P2,P3,
  mpl::void_,mpl::void_,mpl::void_,
  mpl::void_,mpl::void_,mpl::void_,
  mpl::void_
> :
  mpl::or_<
    detail::is_class_or_union_<typename remove_reference<T>::type>,
    mpl::and_<
      detail::is_function_and_arity_is<typename remove_reference<T>::type,3>,
      mpl::eval<
        is_convertible<
          mpl::identity<R>,
          detail::function_result_type<typename remove_reference<T>::type>
        >
      >,
      detail::is_arg_convertible_to_parameter<T,P1,1>,
      detail::is_arg_convertible_to_parameter<T,P2,2>,
      detail::is_arg_convertible_to_parameter<T,P3,3>
    >
  > {};

template<
  class T,class R,
  class P1,class P2,class P3,
  class P4
>
struct has_function_call_op<
  T,R,
  P1,P2,P3,
  P4,mpl::void_,mpl::void_,
  mpl::void_,mpl::void_,mpl::void_,
  mpl::void_
> :
  mpl::or_<
    detail::is_class_or_union_<T>,
    mpl::and_<
      detail::is_function_and_arity_is<T,4>,
      mpl::eval<
        is_convertible<
          mpl::identity<R>,
          detail::function_result_type<typename remove_reference<T>::type>
        >
      >,
      detail::is_arg_convertible_to_parameter<T,P1,1>,
      detail::is_arg_convertible_to_parameter<T,P2,2>,
      mpl::and_<
        detail::is_arg_convertible_to_parameter<T,P3,3>,
        detail::is_arg_convertible_to_parameter<T,P4,4>
      >
    >
  > {};

template<
  class T,class R,
  class P1,class P2,class P3,
  class P4,class P5
>
struct has_function_call_op<
  T,R,
  P1,P2,P3,
  P4,P5,mpl::void_,
  mpl::void_,mpl::void_,mpl::void_,
  mpl::void_
> :
  mpl::or_<
    detail::is_class_or_union_<typename remove_reference<T>::type>,
    mpl::and_<
      detail::is_function_and_arity_is<typename remove_reference<T>::type,5>,
      mpl::eval<
        is_convertible<
          mpl::identity<R>,
          detail::function_result_type<typename remove_reference<T>::type>
        >
      >,
      detail::is_arg_convertible_to_parameter<T,P1,1>,
      detail::is_arg_convertible_to_parameter<T,P2,2>,
      mpl::and_<
        detail::is_arg_convertible_to_parameter<T,P3,3>,
        detail::is_arg_convertible_to_parameter<T,P4,4>,
        detail::is_arg_convertible_to_parameter<T,P5,5>
      >
    >
  > {};

template<
  class T,class R,
  class P1,class P2,class P3,
  class P4,class P5,class P6
>
struct has_function_call_op<
  T,R,
  P1,P2,P3,
  P4,P5,P6,
  mpl::void_,mpl::void_,mpl::void_,
  mpl::void_
> :
  mpl::or_<
    detail::is_class_or_union_<typename remove_reference<T>::type>,
    mpl::and_<
      detail::is_function_and_arity_is<typename remove_reference<T>::type,6>,
      mpl::eval<
        is_convertible<
          mpl::identity<R>,
          detail::function_result_type<typename remove_reference<T>::type>
        >
      >,
      detail::is_arg_convertible_to_parameter<T,P1,1>,
      detail::is_arg_convertible_to_parameter<T,P2,2>,
      mpl::and_<
        detail::is_arg_convertible_to_parameter<T,P3,3>,
        detail::is_arg_convertible_to_parameter<T,P4,4>,
        detail::is_arg_convertible_to_parameter<T,P5,5>,
        detail::is_arg_convertible_to_parameter<T,P6,6>
      >
    >
  > {};

template<
  class T,class R,
  class P1,class P2,class P3,
  class P4,class P5,class P6,
  class P7
>
struct has_function_call_op<
  T,R,
  P1,P2,P3,
  P4,P5,P6,
  P7,mpl::void_,mpl::void_,
  mpl::void_
> :
  mpl::or_<
    detail::is_class_or_union_<typename remove_reference<T>::type>,
    mpl::and_<
      detail::is_function_and_arity_is<typename remove_reference<T>::type,7>,
      mpl::eval<
        is_convertible<
          mpl::identity<R>,
          detail::function_result_type<typename remove_reference<T>::type>
        >
      >,
      detail::is_arg_convertible_to_parameter<T,P1,1>,
      detail::is_arg_convertible_to_parameter<T,P2,2>,
      mpl::and_<
        detail::is_arg_convertible_to_parameter<T,P3,3>,
        detail::is_arg_convertible_to_parameter<T,P4,4>,
        detail::is_arg_convertible_to_parameter<T,P5,5>,
        detail::is_arg_convertible_to_parameter<T,P6,6>,
        detail::is_arg_convertible_to_parameter<T,P7,7>
      >
    >
  > {};

template<
  class T,class R,
  class P1,class P2,class P3,
  class P4,class P5,class P6,
  class P7,class P8
>
struct has_function_call_op<
  T,R,
  P1,P2,P3,
  P4,P5,P6,
  P7,P8,mpl::void_,
  mpl::void_
> :
  mpl::or_<
    detail::is_class_or_union_<typename remove_reference<T>::type>,
    mpl::and_<
      detail::is_function_and_arity_is<typename remove_reference<T>::type,8>,
      mpl::eval<
        is_convertible<
          mpl::identity<R>,
          detail::function_result_type<typename remove_reference<T>::type>
        >
      >,
      detail::is_arg_convertible_to_parameter<T,P1,1>,
      detail::is_arg_convertible_to_parameter<T,P2,2>,
      mpl::and_<
        detail::is_arg_convertible_to_parameter<T,P3,3>,
        detail::is_arg_convertible_to_parameter<T,P4,4>,
        detail::is_arg_convertible_to_parameter<T,P5,5>,
        detail::is_arg_convertible_to_parameter<T,P6,6>,
        mpl::and_<
          detail::is_arg_convertible_to_parameter<T,P7,7>,
          detail::is_arg_convertible_to_parameter<T,P8,8>
        >
      >
    >
  > {};

template<
  class T,class R,
  class P1,class P2,class P3,
  class P4,class P5,class P6,
  class P7,class P8,class P9
>
struct has_function_call_op<
  T,R,
  P1,P2,P3,
  P4,P5,P6,
  P7,P8,P9,
  mpl::void_
> :
  mpl::or_<
    detail::is_class_or_union_<typename remove_reference<T>::type>,
    mpl::and_<
      detail::is_function_and_arity_is<typename remove_reference<T>::type,9>,
      mpl::eval<
        is_convertible<
          mpl::identity<R>,
          detail::function_result_type<typename remove_reference<T>::type>
        >
      >,
      detail::is_arg_convertible_to_parameter<T,P1,1>,
      detail::is_arg_convertible_to_parameter<T,P2,2>,
      mpl::and_<
        detail::is_arg_convertible_to_parameter<T,P3,3>,
        detail::is_arg_convertible_to_parameter<T,P4,4>,
        detail::is_arg_convertible_to_parameter<T,P5,5>,
        detail::is_arg_convertible_to_parameter<T,P6,6>,
        mpl::and_<
          detail::is_arg_convertible_to_parameter<T,P7,7>,
          detail::is_arg_convertible_to_parameter<T,P8,8>,
          detail::is_arg_convertible_to_parameter<T,P9,9>
        >
      >
    >
  > {};

} // namespace boost

#endif // BOOST_CT_OPERATOR_HAS_FUNCTION_CALL_OP_HPP
