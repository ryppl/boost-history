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
template <typename T, typename Mask>
inline T operator&(T t, Mask) {
    return t & Mask::value;
}

template <typename Mask, typename T>
inline T operator&(Mask m, T t) {
    return Mask::value & t;
}

// Overloads for bitwise or
template <typename T, typename Mask>
inline T operator|(T t, Mask) {
    return t | Mask::value;
}

template <typename Mask, typename T>
inline T operator|(Mask, T t) {
    return Mask::value | t;
}

// Overloads for bitwise xor
template <typename T, typename Mask>
inline T operator^(T t, Mask) {
    return t ^ Mask::value;
}

template <typename T, typename Mask>
inline T operator^(Mask, T t) {
    return Mask::value ^ t;
}

} // end of boost namespace.

#endif
