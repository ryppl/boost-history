// Boost.Bimap
//
// Copyright (c) 2006 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file relation/detail/totally_ordered_pair.hpp
/// \brief Macro to help build totally ordered pairs.

#ifndef BOOST_BIMAP_RELATION_DETAIL_TOTALLY_ORDERED_PAIR
#define BOOST_BIMAP_RELATION_DETAIL_TOTALLY_ORDERED_PAIR

/****************************************************************************/

#define BOOST_BIMAP_TOTALLY_ORDERED_PAIR_IMPLEMENTATION(                     \
        A_FIRST, A_SECOND,                                                   \
        B_TYPE,                                                              \
        B_FIRST, B_SECOND                                                    \
    )                                                                        \
                                                                             \
    bool operator==(const B_TYPE & b) const                                  \
    {                                                                        \
        return ( ( A_FIRST  == b.B_FIRST  ) &&                               \
                 ( A_SECOND == b.B_SECOND ) );                               \
    }                                                                        \
                                                                             \
    bool operator!=(const B_TYPE & b) const                                  \
    {                                                                        \
        return ( ( A_FIRST  != b.B_FIRST  ) ||                               \
                 ( A_SECOND != b.B_SECOND ) );                               \
    }                                                                        \
                                                                             \
    bool operator< (const B_TYPE & b) const                                  \
    {                                                                        \
        return ( ( A_FIRST  <  b.B_FIRST  ) &&                               \
                 ( A_SECOND <  b.B_SECOND ) );                               \
    }                                                                        \
                                                                             \
    bool operator<=(const B_TYPE & b) const                                  \
    {                                                                        \
        return ( *this < b || *this == b );                                  \
    }                                                                        \
                                                                             \
    bool operator> (const B_TYPE & b) const                                  \
    {                                                                        \
        return ! (*this <= b);                                               \
    }                                                                        \
                                                                             \
    bool operator>=(const B_TYPE & b) const                                  \
    {                                                                        \
        return ( *this > b || *this == b );                                  \
    }

/****************************************************************************/

#endif // BOOST_BIMAP_RELATION_DETAIL_TOTALLY_ORDERED_PAIR

