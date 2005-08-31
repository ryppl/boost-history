// Copyright (C) 2005 Peder Holt
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_METAMATH_DOUBLE_NEGATE_HPP
#define BOOST_METAMATH_DOUBLE_NEGATE_HPP

#include <boost/mpl/negate.hpp>
#include <boost/metamath/double/double.hpp>

namespace boost { namespace mpl {
   template<>
   struct negate_impl< metamath::double_c_tag>
   {
       template<typename N> struct apply
       {
           BOOST_STATIC_CONSTANT(bool,sign=!N::sign);
           typedef boost::metamath::double_<typename N::mantissa,N::exponent,sign> type;
       };
   };
}}

#endif //BOOST_METAMATH_DOUBLE_NEGATE_HPP
