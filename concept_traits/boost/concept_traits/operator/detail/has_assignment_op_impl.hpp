///////////////////////////////////////////////////////////////////////////////
// Operator traits
//
// (C) Copyright Terje Slettebø and Tobias Schwinger 2004.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).
///////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_CT_OPERATOR_DETAIL_HAS_ASSIGNMENT_OP_IMPL_HPP
#define BOOST_CT_OPERATOR_DETAIL_HAS_ASSIGNMENT_OP_IMPL_HPP

#include <boost/concept_check.hpp>

namespace boost {
namespace detail {

// Assignable requires more than an accessible assignment operator, so it can't be used here

template<class T>
struct AssignmentOperatorConcept
{
  void constraints()
  {
    T u=t;
  }

  static T t;
};

///////////////////////////////////////////////////////////////////////////////
// has_assignment_op_impl
///////////////////////////////////////////////////////////////////////////////

template<class T,class U>
struct has_assignment_op_impl : mpl::true_
{
//  BOOST_CLASS_REQUIRE(T,::boost::detail,AssignmentOperatorConcept); *** Problem with MSVC 7.1
};

} // namespace detail
} // namespace boost

#endif // BOOST_CT_OPERATOR_DETAIL_HAS_ASSIGNMENT_OP_IMPL_HPP
