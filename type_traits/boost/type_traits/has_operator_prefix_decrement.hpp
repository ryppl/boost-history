//  (C) Copyright 2009-2011 Frédéric Bron (frederic.bron@m4x.org)
//
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef BOOST_TT_HAS_OPERATOR_PREFIX_DECREMENT_HPP_INCLUDED
#define BOOST_TT_HAS_OPERATOR_PREFIX_DECREMENT_HPP_INCLUDED

#define BOOST_TT_TRAIT_NAME has_operator_prefix_decrement
#define BOOST_TT_TRAIT_OP --
#define BOOST_TT_DEFAULT_RET void
#define BOOST_TT_FORBIDDEN_IF\
   /* bool or void* or const fundamental */\
   ::boost::type_traits::ice_or<\
      ::boost::is_same< bool, rhs_nocv >::value,\
      ::boost::type_traits::ice_and<\
         ::boost::is_pointer< rhs_noref >::value,\
         ::boost::is_void< rhs_noptr >::value\
      >::value,\
      ::boost::type_traits::ice_and<\
         ::boost::is_fundamental< rhs_nocv >::value,\
         ::boost::is_const< rhs_noref >::value\
     >::value\
   >::value


#include <boost/type_traits/detail/has_prefix_operator.hpp>

#undef BOOST_TT_TRAIT_NAME
#undef BOOST_TT_TRAIT_OP
#undef BOOST_TT_DEFAULT_RET
#undef BOOST_TT_FORBIDDEN_IF

#endif
