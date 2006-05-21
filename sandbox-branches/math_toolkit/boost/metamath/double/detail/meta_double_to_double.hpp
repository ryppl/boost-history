// Copyright (C) 2005 Peder Holt
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_METAMATH_DOUBLE_DETAIL_META_DOUBLE_TO_DOUBLE_HPP
#define BOOST_METAMATH_DOUBLE_DETAIL_META_DOUBLE_TO_DOUBLE_HPP

namespace boost { namespace metamath {
    namespace detail {    
        struct double_encoding {
            unsigned long mantissa2;
            unsigned long mantissa1:20;
            unsigned long exp:11;
            unsigned long sign:1;
        };

        union double_converter {
            double value;
            double_encoding encoding;
        };

        template<typename T>
        double meta_double_to_double(const T& arg) {
            double_converter converter;
            converter.encoding.sign=T::sign;
            converter.encoding.exp=(T::exponent+1023);
            converter.encoding.mantissa1=T::mantissa::part1>>10;
            converter.encoding.mantissa2=(T::mantissa::part1<<22)|(T::mantissa::part2>>9);
            return converter.value;
        }
    } //namespace detail
}} //namespace boost::metamath

#endif
