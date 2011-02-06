//  (C) Copyright 2009-2011 Frédéric Bron (frederic.bron@m4x.org)
//
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef BOOST_TT_left_shift_HPP_INCLUDED
#define BOOST_TT_left_shift_HPP_INCLUDED

#define BOOST_TT_TRAIT_NAME has_operator_left_shift
#define BOOST_TT_TRAIT_OP <<
#define BOOST_TT_DEFAULT_RET void
#define BOOST_TT_FORBIDDEN_IF\
   ::boost::type_traits::ice_or<\
      /* two fundamental, one non integral */\
      ::boost::type_traits::ice_and<\
         ::boost::is_fundamental< lhs_nocv >::value,\
         ::boost::is_fundamental< rhs_nocv >::value,\
         ::boost::type_traits::ice_or<\
            ::boost::type_traits::ice_not< ::boost::is_integral< lhs_noref >::value >::value,\
            ::boost::type_traits::ice_not< ::boost::is_integral< rhs_noref >::value >::value\
         >::value\
      >::value,\
      /* one fundamental, one pointer */\
      ::boost::type_traits::ice_or<\
         ::boost::type_traits::ice_and<\
            ::boost::is_fundamental< lhs_nocv >::value,\
            ::boost::is_pointer< rhs_noref >::value\
         >::value,\
         ::boost::type_traits::ice_and<\
            ::boost::is_fundamental< rhs_nocv >::value,\
            ::boost::is_pointer< lhs_noref >::value\
         >::value\
      >::value,\
      /* two pointers */\
      ::boost::type_traits::ice_and<\
         ::boost::is_pointer< lhs_noref >::value,\
         ::boost::is_pointer< rhs_noref >::value\
      >::value\
   >::value


#include <boost/type_traits/detail/has_binary_operator.hpp>

#undef BOOST_TT_TRAIT_NAME
#undef BOOST_TT_TRAIT_OP
#undef BOOST_TT_DEFAULT_RET
#undef BOOST_TT_FORBIDDEN_IF

#endif
