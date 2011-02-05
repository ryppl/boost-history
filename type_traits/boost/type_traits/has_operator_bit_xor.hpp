//  (C) Copyright 2009-2011 Frédéric Bron (frederic.bron@m4x.org)
//
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef BOOST_TT_bit_xor_HPP_INCLUDED
#define BOOST_TT_bit_xor_HPP_INCLUDED

#define BOOST_TT_TRAIT_NAME has_operator_bit_xor
#define BOOST_TT_TRAIT_OP ^
#define BOOST_TT_DEFAULT_RET void
#define BOOST_TT_FORBIDDEN_IF\
   ::boost::type_traits::ice_or<\
      /* two built-in, one non integer */\
      ::boost::type_traits::ice_and<\
         /* two builtin */\
         ::boost::type_traits::ice_not<\
            ::boost::type_traits::ice_or<\
               ::boost::is_class< typename ::boost::remove_reference<LHS>::type >::value,\
               ::boost::is_union< typename ::boost::remove_reference<LHS>::type >::value,\
               ::boost::is_pointer< typename ::boost::remove_reference<LHS>::type >::value,\
               ::boost::is_class< typename ::boost::remove_reference<RHS>::type >::value,\
               ::boost::is_union< typename ::boost::remove_reference<RHS>::type >::value,\
               ::boost::is_pointer< typename ::boost::remove_reference<RHS>::type >::value\
            >::value\
         >::value,\
         /* one non integer */\
         ::boost::type_traits::ice_or<\
            ::boost::type_traits::ice_not< ::boost::is_integral< typename ::boost::remove_reference<LHS>::type >::value >::value,\
            ::boost::type_traits::ice_not< ::boost::is_integral< typename ::boost::remove_reference<RHS>::type >::value >::value\
         >::value\
      >::value,\
      /* one pointer */\
      ::boost::type_traits::ice_or<\
         ::boost::is_pointer< typename ::boost::remove_reference<LHS>::type >::value,\
         ::boost::is_pointer< typename ::boost::remove_reference<RHS>::type >::value\
      >::value\
   >::value


#include <boost/type_traits/detail/has_binary_operator.hpp>

#undef BOOST_TT_TRAIT_NAME
#undef BOOST_TT_TRAIT_OP
#undef BOOST_TT_DEFAULT_RET
#undef BOOST_TT_FORBIDDEN_IF

#endif
