//
//  Copyright (c) 2000-2002
//  Joerg Walter, Mathias Koch
//
//  Permission to use, copy, modify, distribute and sell this software
//  and its documentation for any purpose is hereby granted without fee,
//  provided that the above copyright notice appear in all copies and
//  that both that copyright notice and this permission notice appear
//  in supporting documentation.  The authors make no representations
//  about the suitability of this software for any purpose.
//  It is provided "as is" without express or implied warranty.
//
//  The authors gratefully acknowledge the support of
//  GeNeSys mbH & Co. KG in producing this work.
//

#ifndef NUMERICS_CONFIG_H
#define NUMERICS_CONFIG_H

#include <cassert>
#include <cstddef>

#include <boost/config.hpp>

#ifdef BOOST_MSVC
// Disable some MSVC specific warnings.
#pragma warning (disable: 4355)
#pragma warning (disable: 4503)
#pragma warning (disable: 4786)
#endif



#ifdef BOOST_MSVC
// MSVC doesn't always accept the 'typename' keyword.
#define NUMERICS_TYPENAME
#else
#define NUMERICS_TYPENAME typename
#endif
// This could be eliminated.
#define NUMERICS_EXPLICIT explicit



// #ifdef BOOST_MSVC
// With MSVC we can could perform IO via basic_stream
// #define NUMERICS_USE_BASIC_STREAM
// #else
// IO via streams
#define NUMERICS_USE_STREAM
// #endif



// Enable performance options in release mode
#ifdef NDEBUG

#ifdef BOOST_MSVC
// MSVC has special inlining options
#pragma inline_recursion (on)
#pragma inline_depth (255)
#pragma auto_inline (on)
// #define NUMERICS_INLINE __forceinline
#define NUMERICS_INLINE __inline
#else
#define NUMERICS_INLINE inline
#endif

// Do not check sizes!
#define NUMERICS_USE_FAST_COMMON

// Use expression templates.
#define NUMERICS_USE_ET

// Disable performance options in debug mode
#else

#ifdef BOOST_MSVC
// MSVC has special inlining options
// #pragma inline_recursion (off)
// #pragma inline_depth ()
// #pragma auto_inline (off)
#endif
#define NUMERICS_INLINE

#ifdef BOOST_MSVC
// Use expression templates (otherwise we get many ICE's)
#define NUMERICS_USE_ET
#endif

// Bounds check
#define NUMERICS_BOUNDS_CHECK

#endif



// Use invariant hoisting.
#define NUMERICS_USE_INVARIANT_HOISTING

// Use Duff's device
// #define NUMERICS_USE_DUFF_DEVICE

// When to switch from indexing to iterating
// #define NUMERICS_ITERATOR_THRESHOLD 0
#define NUMERICS_ITERATOR_THRESHOLD 32
// #define NUMERICS_ITERATOR_THRESHOLD 128

// ET options
#define NUMERICS_ET_VALUE
// #define NUMERICS_ET_REFERENCE
// #define NUMERICS_ET_CLOSURE_VALUE
#define NUMERICS_ET_CLOSURE_REFERENCE

// Use canonical iterators.
// #define NUMERICS_USE_CANONICAL_ITERATOR

// Use indexed iterators.
// #define NUMERICS_USE_INDEXED_ITERATOR



// Compiler specific problems
#ifdef BOOST_MSVC

// Open problems:
// MSVC lacks some specializations in <cmath>
#define NUMERICS_C_MATH
// MSVC's conditional operator is said to be defect?
#define NUMERICS_CONDITIONAL_DEFECT
// MSVC allows to implement free function as friends.
#define NUMERICS_FRIEND_FUNCTION

// Using MSVC the following is missing:
// namespace std {
//
//     typedef unsigned size_t;
//     typedef signed ptrdiff_t;
//
//     template<class T>
//     NUMERICS_INLINE
//     const T &max (const T &t1, const T &t2) {
//         return t1 > t2 ? t1 : t2;
//     }
//     template<class T>
//     NUMERICS_INLINE
//     const T &min (const T &t1, const T &t2) {
//         return t1 < t2 ? t1 : t2;
//     }
//
// }
// But boost provides it for us.

// This seems to be a problem in boost.config, but won't be fixed.
#ifdef __SGI_STL_PORT
#define BOOST_MSVC_STD_ITERATOR
#endif

#endif



#ifdef __GNUC__

// Open problems:
// GCC 2.95.3 is known not to accept NUMERICS_FRIEND_FUNCTION (this seems to be arguable).
// GCC 3.x.x allows to implement free function as friends.
#if !(__GNUC__ <= 2 && __GNUC_MINOR__ <= 95)
#define NUMERICS_FRIEND_FUNCTION
#endif
// GCC 2.95.3 is known not to accept NUMERICS_MUTABLE_TEMPORARY (this seems to be correct).
// GCC 2.95.3 allows to use iterator_base_traits.
#define NUMERICS_USE_ITERATOR_BASE_TRAITS
// GCC 2.95.3 needs NUMERICS_REVERSE_ITERATOR_OVERLOADS (this seems to be arguable).
#define NUMERICS_REVERSE_ITERATOR_OVERLOADS

#define NUMERICS_USE_LONG_DOUBLE

#endif



#ifdef __BORLANDC__

// Open problems:
// BCC allows to implement free function as friends.
#define NUMERICS_FRIEND_FUNCTION

#endif



// FIXME: What about __ICL?
#ifdef __ICC

// Open problems:
// ICC lacks some specializations in <cmath>
#define NUMERICS_C_MATH
// ICC allows to implement free function as friends.
#define NUMERICS_FRIEND_FUNCTION
// ICC sometimes needs qualified type names.
#define NUMERICS_QUALIFIED_TYPENAME

#endif



// Thanks to Kresimir Fresl for porting to Comeau.
#ifdef __COMO__

// Comeau lacks some specializations in <cmath>
#define NUMERICS_C_MATH
// Comeau 4.2.45 seems to have problems with NUMERICS_FRIEND_FUNCTION (this seems to be arguable).
// Comeau allows to use iterator_base_traits.
#define NUMERICS_USE_ITERATOR_BASE_TRAITS

#endif



// Forward declarations
namespace boost { namespace numerics {

    template<class T>
    class unbounded_array;

    template<class I, class T>
    class map_array;

    struct vector_tag {};

    template<class E>
    struct vector_expression;

    struct matrix_tag {};

    template<class E>
    struct matrix_expression;

    template<class E>
    class vector_range;

    template<class E>
    class matrix_row;
    template<class E, class I>
    class matrix_row_iterator;
    template<class E, class I>
    class matrix_row_const_iterator;
    template<class E>
    class matrix_column;
    template<class E, class I>
    class matrix_column_iterator;
    template<class E, class I>
    class matrix_column_const_iterator;
    template<class E>
    class matrix_range;

    template<class F>
    struct vector_assign_scalar;
    template<class F>
    struct vector_assign;

    template<class T, class A = unbounded_array<T> >
    class vector;

    template<class T>
    class unit_vector;

    template<class T>
    class scalar_vector;

    template<class T, class A = map_array<std::size_t, T> >
    class sparse_vector;

    template<class T, class AI = unbounded_array<std::size_t>, class AT = unbounded_array<T> >
    class compressed_vector;

    struct unknown_orientation_tag {};

    struct row_major_tag {};
    struct row_major;

    struct column_major_tag {};
    struct column_major;

    template<class F>
    struct matrix_assign_scalar;
    template<class F>
    struct matrix_assign;
    template<class F>
    struct matrix_swap;

    template<class T, class F = row_major, class A = unbounded_array<T> >
    class matrix;

    template<class T>
    class identity_matrix;

    template<class T>
    class scalar_matrix;

    template<class T, class F = row_major, class A = unbounded_array<unbounded_array<T> > >
    class vector_of_vector;

    template<class T, class F = row_major, class A = unbounded_array<T> >
    class banded_matrix;

    struct lower_tag {};
    struct lower;

    struct upper_tag {};
    struct upper;

    struct unit_lower_tag: public lower_tag {};
    struct unit_lower;

    struct unit_upper_tag: public upper_tag {};
    struct unit_upper;

    template<class T, class F1 = lower, class F2 = row_major, class A = unbounded_array<T> >
    class triangular_matrix;

    template<class M, class F = lower>
    class triangular_adaptor;

    template<class T, class F1 = lower, class F2 = row_major, class A = unbounded_array<T> >
    class symmetric_matrix;

    template<class M, class F = lower>
    class symmetric_adaptor;

    template<class T, class F1 = lower, class F2 = row_major, class A = unbounded_array<T> >
    class hermitean_matrix;

    template<class M, class F = lower>
    class hermitean_adaptor;

    template<class T, class F = row_major, class A = map_array<std::size_t, T> >
    class sparse_matrix;

    template<class T, class F = row_major, class A = map_array<std::size_t, map_array<std::size_t, T> > >
    class sparse_vector_of_sparse_vector;

    template<class T, class F = row_major, class IA = unbounded_array<std::size_t>, class TA = unbounded_array<T> >
    class compressed_matrix;


    template<class V>
    typename V::size_type num_elements (const V &v) {
        return v.size ();
    }
    template<class M>
    typename M::size_type num_rows (const M &m) {
        return m.size1 ();
    }
    template<class M>
    typename M::size_type num_columns (const M &m) {
        return m.size2 ();
    }
    template<class MV>
    typename MV::size_type num_non_zeros (const MV &mv) {
        return mv.non_zeros ();
    }
}}

// This temporarily is intended to keep existent code compatible.
namespace numerics = boost::numerics;

#endif




























