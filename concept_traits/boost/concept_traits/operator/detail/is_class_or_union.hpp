///////////////////////////////////////////////////////////////////////////////
// Operator traits
//
// (C) Copyright Terje Slettebø and Tobias Schwinger 2004.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).
///////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_CT_OPERATOR_DETAIL_IS_CLASS_OR_UNION_HPP
#define BOOST_CT_OPERATOR_DETAIL_IS_CLASS_OR_UNION_HPP

#include <boost/type_traits/is_class.hpp>
#include <boost/type_traits/is_union.hpp>
#include <boost/mpl/or.hpp>

namespace boost {
namespace detail {

///////////////////////////////////////////////////////////////////////////////
// is_class_or_union_
//
// There is a detail::is_class_or_union in the boost::is_enum implementation,
// also... However, as it's not in the public interface, we can't rely on it,
// so we define our own here, with a slightly different name to avoid collision.
//
// To avoid the duplication, perhaps it should be part of the public interface
// of the type traits?
///////////////////////////////////////////////////////////////////////////////

template<class T>  
struct is_class_or_union_ :
  mpl::or_<
    is_class<T>,
    is_union<T>
  > {};

} // namespace detail
} // namespace boost

#endif // BOOST_CT_OPERATOR_DETAIL_IS_CLASS_OR_UNION_HPP
