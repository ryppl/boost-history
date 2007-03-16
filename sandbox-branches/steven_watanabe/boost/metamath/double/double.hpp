// Copyright (C) 2005 Peder Holt
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_METAMATH_DOUBLE_DOUBLE_HPP
#define BOOST_METAMATH_DOUBLE_DOUBLE_HPP

#include <boost/config.hpp>
#include <boost/cstdint.hpp>
#include <boost/metamath/double/detail/meta_double_to_double.hpp>
#include <boost/metamath/double/detail/mantissa.hpp>

namespace boost { namespace metamath {

    struct double_c_tag {
        BOOST_STATIC_CONSTANT(int,value=0);
    };

    template<typename Mantissa,int16_t Exponent,bool Sign>
    struct double_ {
        typedef double value_type;
        typedef double_<Mantissa,Exponent,Sign> type;
        typedef double_c_tag tag;
        BOOST_STATIC_CONSTANT(bool,sign=Sign);
        BOOST_STATIC_CONSTANT(int16_t,exponent=Exponent);
        typedef Mantissa mantissa;

        operator value_type() const {
            return detail::meta_double_to_double(type());
        }
    };

}}//namespace boost::metamath

#endif
