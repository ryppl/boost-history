
//  (C) Copyright Terje Slettebø and Tobias Schwinger 2004
//  Use, modification and distribution are subject to the Boost Software 
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#ifndef BOOST_CT_DETAIL_PLUS_MINUS_BINOP_PREFILTER_HPP_INCLUDED
#define BOOST_CT_DETAIL_PLUS_MINUS_BINOP_PREFILTER_HPP_INCLUDED

#include "boost/concept_traits/facility/binary_type_filter_def.hpp"

namespace boost { namespace detail {

#define BOOST_CT_TYPE_FILTER plus_minus_binop_prefilter

BOOST_CT_TF_RULESET_BEGIN
  BOOST_CT_TF_RULE_TARGET(0) (any_type          , any_type          );

// V-- change target of these rules to  1 to detect builtin operators
  BOOST_CT_TF_RULE_PASS      (arithmetic_or_enum, arithmetic_or_enum);
  BOOST_CT_TF_RULE_PASS      (obj_ptr_or_array  , integral_or_enum  );
  BOOST_CT_TF_RULE_PASS      (integral_or_enum  , obj_ptr_or_array  );
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  BOOST_CT_TF_RULE_PASS      (aggregate         , non_void          );
  BOOST_CT_TF_RULE_PASS      (non_void          , aggregate         );
  BOOST_CT_TF_RULE_PASS      (aggregate         , aggregate         );
BOOST_CT_TF_RULESET_END


} } // namespace ::boost::detail

#include "boost/concept_traits/facility/type_filter_undef.hpp"

#endif // BOOST_CT_DETAIL_PLUS_MINUS_BINOP_PREFILTER_HPP_INCLUDED
