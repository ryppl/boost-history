// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GRAPH_NUMERIC_VALUES_HPP
#define BOOST_GRAPH_NUMERIC_VALUES_HPP

#include <limits>

namespace boost
{

    // This generic type reports various numeric values for
    // some type. This must be specialized for non-numeric
    // types such that the return values have the equivalent
    // semantics of zero and infinity. This classd essentially
    // builds abstractions for zero and infinity out of types
    // that don't necessarily support it.
    //
    // Specializations are provided for float and double types
    // since they actually have an infinity value.

    template <typename T>
    struct numeric_values
    {
        typedef T value_type;

        static inline T zero()
        { return T(); }

        static inline T infinity()
        { return std::numeric_limits<T>::max(); }
    };

    template <>
    struct numeric_values<float>
    {
        typedef float value_type;

        static inline float zero()
        { return float(0.0); }

        static inline float infinity()
        { return std::numeric_limits<float>::infinity(); }
    };

    template <>
    struct numeric_values<double>
    {
        typedef double value_type;

        static inline double zero()
        { return double(0.0); }

        static inline double infinity()
        { return std::numeric_limits<double>::infinity(); }
    };

    template <>
    struct numeric_values<long double>
    {
        typedef long double value_type;

        static inline long double zero()
        { return value_type(0.0); }

        static inline long double infinity()
        { return std::numeric_limits<long double>::infinity(); }
    };
}

#endif
