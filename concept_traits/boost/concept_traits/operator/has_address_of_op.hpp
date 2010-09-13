///////////////////////////////////////////////////////////////////////////////
// Operator traits
//
// (C) Copyright Terje Slettebø and Tobias Schwinger 2004.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).
///////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_CT_OPERATOR_HAS_ADDRESS_OF_OP_HPP
#define BOOST_CT_OPERATOR_HAS_ADDRESS_OF_OP_HPP

#include <boost/type_traits/is_enum.hpp>
#include <boost/type_traits/is_void.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/concept_traits/operator/detail/unop.hpp>

namespace boost {

///////////////////////////////////////////////////////////////////////////////
// has_address_of_op
///////////////////////////////////////////////////////////////////////////////

BOOST_CT_OPERATOR_DETAIL_DEFINE_UNOP_TRAIT(has_address_of_op_impl, &) // Possibly auto-generated, will give always true or error

template<class T>
struct has_address_of_op :
  mpl::and_<
    mpl::not_<
      mpl::or_<
        is_enum<typename remove_reference<T>::type>,
        is_void<T>
      >  
    >,
    detail::has_address_of_op_impl<T>
  > {};

} // namespace boost

#endif // BOOST_CT_OPERATOR_HAS_ADDRESS_OF_OP_HPP
