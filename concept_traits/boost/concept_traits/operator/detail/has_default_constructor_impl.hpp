///////////////////////////////////////////////////////////////////////////////
// Operator traits
//
// (C) Copyright Terje Slettebø and Tobias Schwinger 2004.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).
///////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_CT_OPERATOR_DETAIL_HAS_DEFAULT_CONSTRUCTOR_IMPL_HPP
#define BOOST_CT_OPERATOR_DETAIL_HAS_DEFAULT_CONSTRUCTOR_IMPL_HPP

#include <boost/concept_check.hpp>

namespace boost {
namespace detail {

///////////////////////////////////////////////////////////////////////////////
// has_default_constructor_impl
///////////////////////////////////////////////////////////////////////////////

template<class T>
struct has_default_constructor_impl : mpl::true_
{
  BOOST_CLASS_REQUIRE(T,boost,DefaultConstructibleConcept);
};

} // namespace detail
} // namespace boost

#endif // BOOST_CT_OPERATOR_DETAIL_HAS_DEFAULT_CONSTRUCTOR_IMPL_HPP
