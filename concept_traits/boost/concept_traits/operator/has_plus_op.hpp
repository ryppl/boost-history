
//  (C) Copyright Terje Slettebø and Tobias Schwinger 2004
//  Use, modification and distribution are subject to the Boost Software 
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#ifndef BOOST_CT_OPERATOR_HAS_PLUS_OP_HPP
#define BOOST_CT_OPERATOR_HAS_PLUS_OP_HPP

#include "boost/mpl/bool.hpp"
#include "boost/mpl/eval_if.hpp"

#include "boost/concept_traits/operator/detail/binop.hpp"
#include "boost/concept_traits/operator/detail/plus_minus_binop_prefilter.hpp"

// Should be the last file to include
#include "boost/type_traits/detail/bool_trait_def.hpp"

namespace boost { 

BOOST_CT_OPERATOR_DETAIL_DEFINE_BINOP_TRAIT(test_plus_op, +)

namespace concept_traits
{

template <typename T, typename U>
struct has_plus_op_impl
{
  typedef typename facility::filtered_bool_trait< 
    detail::test_plus_op<T,U>
  , detail::plus_minus_binop_prefilter<T,U> >::type type;

  BOOST_STATIC_CONSTANT(bool, value = (type::value));
};

} // namespace concept_traits


BOOST_TT_AUX_BOOL_TRAIT_DEF2(has_plus_op,T,U,(
  ::boost::concept_traits::has_plus_op_impl<T,U>::value 
))


#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
BOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC2_1(typename T,has_plus_op,T,T,(
  ::boost::concept_traits::has_plus_op_impl<T,T>::value 
))
#endif

} // namespace ::boost

#include "boost/type_traits/detail/bool_trait_undef.hpp"

#endif // BOOST_CT_OPERATOR_HAS_PLUS_OP_HPP
