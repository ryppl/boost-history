///////////////////////////////////////////////////////////////////////////////
// Operator traits
//
// (C) Copyright Terje Slettebø and Tobias Schwinger 2004.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).
///////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_CT_OPERATOR_HAS_DEFAULT_CONSTRUCTOR_HPP
#define BOOST_CT_OPERATOR_HAS_DEFAULT_CONSTRUCTOR_HPP

#include <boost/type_traits/is_object.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/concept_traits/operator/detail/is_not_const.hpp>
#include <boost/concept_traits/operator/detail/is_class_or_union.hpp>
#include <boost/concept_traits/operator/detail/has_default_constructor_impl.hpp>

namespace boost {

///////////////////////////////////////////////////////////////////////////////
// has_default_constructor
//
// If T is a class or union with no accessible default constructor, this trait
// will give an error, rather than false.
///////////////////////////////////////////////////////////////////////////////

template<class T>
struct has_default_constructor :
  mpl::and_<
    mpl::or_<
      mpl::and_<
        detail::is_not_const<T>,
        is_object<T>
      >,
      detail::is_class_or_union_<T>
    >,
    detail::has_default_constructor_impl<T>
  > {};

} // namespace boost

#endif // BOOST_CT_OPERATOR_HAS_DEFAULT_CONSTRUCTOR_HPP
