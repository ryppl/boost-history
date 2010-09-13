///////////////////////////////////////////////////////////////////////////////
// Operator traits
//
// (C) Copyright Terje Slettebø and Tobias Schwinger 2004.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).
///////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_CT_OPERATOR_HAS_ASSIGNMENT_OP_HPP
#define BOOST_CT_OPERATOR_HAS_ASSIGNMENT_OP_HPP

#include <boost/type_traits/remove_reference.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/concept_traits/operator/detail/is_object_and_not_array_or_member_function_pointer_and_same_type.hpp>
#include <boost/concept_traits/operator/detail/is_arithmetic_and_arithmetic_or_enum.hpp>
#include <boost/concept_traits/operator/detail/is_compatible_pointers.hpp>
#include <boost/concept_traits/operator/detail/is_not_const.hpp>
#include <boost/concept_traits/operator/detail/is_class_or_union.hpp>
#include <boost/concept_traits/operator/detail/has_assignment_op_impl.hpp>

namespace boost {

///////////////////////////////////////////////////////////////////////////////
// has_assignment_op
//
// If T is a class or union with no accessible assignment operator, this trait
// will give an error, rather than false.
///////////////////////////////////////////////////////////////////////////////

template<class T,class U = T>
struct has_assignment_op :
  mpl::and_<
    detail::is_not_const<typename remove_reference<T>::type>,
    mpl::or_<
      detail::is_object_and_not_array_or_member_function_pointer_and_same_type<T,U>,
      detail::is_arithmetic_and_arithmetic_or_enum<T,U>,
      detail::is_compatible_pointers<T,U>,
      detail::is_class_or_union_<typename remove_reference<T>::type>
    >,
    detail::has_assignment_op_impl<T,U>
  > {};

} // namespace boost

#endif // BOOST_CT_OPERATOR_HAS_ASSIGNMENT_OP_HPP
