// lowest_bit.hpp
//
// Position of the lowest bit 'on'
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
// (C) Gennaro Prota 2003

#ifndef BOOST_LOWEST_BIT_HPP_GP_20030301
#define BOOST_LOWEST_BIT_HPP_GP_20030301

#include <cassert>
#include "boost/integer_log2.hpp"


namespace boost {

    template <typename T>
    int lowest_bit(T x) {

        assert( x >= 1); // PRE

        // clear all bits on except the rightmost one,
        // then calculate the logarithm base 2
        //
        return boost::integer_log2<T>( x - ( x & (x-1) ) );

    }


}



#endif // include guard
