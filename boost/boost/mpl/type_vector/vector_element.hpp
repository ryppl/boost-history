//-----------------------------------------------------------------------------
// boost mpl/type_vector/vector_element.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2000-01
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_TYPE_VECTOR_VECTOR_ELEMENT_HPP
#define BOOST_MPL_TYPE_VECTOR_VECTOR_ELEMENT_HPP

#include "boost/mpl/null.hpp"
#include "boost/mpl/preprocessor/config.hpp"
#include "boost/preprocessor/repeat.hpp"
#include "boost/config.hpp"

namespace boost {
namespace mpl {
namespace detail {

template<typename Vector, long N> struct vector_element;

template<typename Vector, long N>
struct vector_iterator
{
    typedef typename vector_element<Vector, N>::type type;
    typedef vector_iterator<Vector, N + 1> next;
    BOOST_STATIC_CONSTANT(long, index = N);

    template<long D> struct advance
    {
        typedef vector_iterator<Vector, N + D> iterator;
    };
};

// "null" vector_element
template<typename Vector, long N>
struct vector_element
{
    typedef vector_iterator<Vector, N> iterator;
    typedef Vector vector_type;
    typedef mpl::null_t type;
};

// "real" vector_element
#define BOOST_MPL_TYPE_VECTOR_ELEMENT(i, unused1, unused2)                    \
    template<typename Vector>                                                 \
    struct vector_element<Vector, i>                                          \
    {                                                                         \
        typedef vector_iterator<Vector, i> iterator;                          \
        typedef Vector vector_type;                                           \
        typedef typename Vector::value_type_##i type;                         \
    };                                                                        \
/**/

// the result of the macro expansion (N == BOOST_MPL_PARAMETERS_NUMBER):
//    template<typename Vector> struct vector_element<Vector, 0> { ... };
//    template<typename Vector> struct vector_element<Vector, 1> { ... };
//    ...
//    template<typename Vector> struct vector_element<Vector, N> { ... };

BOOST_PREPROCESSOR_REPEAT(
      BOOST_MPL_PARAMETERS_NUMBER
    , BOOST_MPL_TYPE_VECTOR_ELEMENT
    , unused
    , unused
    )

#undef BOOST_MPL_TYPE_VECTOR_ELEMENT

} // namespace detail
} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_TYPE_VECTOR_VECTOR_ELEMENT_HPP
