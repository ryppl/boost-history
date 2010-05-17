//  (C) Copyright Brian Bartman 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//  See http://www.boost.org for updates, documentation, and revision history. 

#ifndef BOOST_BIT_MASK_FREE_FUNCTION_SUPPORT_HPP
#define BOOST_BIT_MASK_FREE_FUNCTION_SUPPORT_HPP



namespace boost {

// Contains overloads for all of the bit wise operators which apply to bit_masks
// and applications there of.

// Overloads for bitwise and
template <typename T, unsigned int Value>
inline T operator&(T t, integral_mask<T,Value> ) {
    return t & integral_mask<T,Value>::value;
}

template <typename T, unsigned int Value>
inline T operator&(integral_mask<T,Value>, T t) {
    return integral_mask<T,Value>::value & t;
}

// Overloads for bitwise or
template <typename T, unsigned int Value>
inline T operator|(T t, integral_mask<T,Value>) {
    return t | integral_mask<T,Value>::value;
}

template <typename T, unsigned int Value>
inline T operator|(integral_mask<T,Value>, T t) {
    return integral_mask<T,Value>::value | t;
}

// Overloads for bitwise xor
template <typename T, unsigned int Value>
inline T operator^(T t, integral_mask<T,Value>) {
    return t ^ integral_mask<T,Value>::value;
}

template <typename T, unsigned int Value>
inline T operator^(integral_mask<T,Value>, T t) {
    return integral_mask<T,Value>::value ^ t;
}

} // end of boost namespace.

#endif
