//
//  Copyright (c) 2000-2001
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
//	GeNeSys mbH & Co. KG in producing this work.
//

#ifndef NUMERICS_CONFIG_H
#define NUMERICS_CONFIG_H

#include <cstddef>

#define NUMERICS_DEPRECATED

#ifdef USE_MSVC

#pragma warning (disable: 4355)
#pragma warning (disable: 4503)
#pragma warning (disable: 4786)

#define NUMERICS_TYPENAME

#define NUMERICS_EXPLICIT explicit

#ifdef _DEBUG

#define NUMERICS_USE_INSTANT
#define NUMERICS_INLINE

// Bounds check
#define NUMERICS_BOUNDS_CHECK

// Use expression templates.
#define NUMERICS_USE_ET
#define NUMERICS_ET_VALUE
// #define NUMERICS_ET_REFERENCE
// #define NUMERICS_ET_CLOSURE_VALUE
#define NUMERICS_ET_CLOSURE_REFERENCE

// Use iterators.
#define NUMERICS_USE_ITERATOR
#define NUMERICS_ITERATOR_THRESHOLD 32

// Use invariant hoisting.
#define NUMERICS_USE_INVARIANT_HOISTING

#else

// #pragma inline_recursion (off)
#pragma inline_recursion (on)
// #pragma inline_depth ()
#pragma inline_depth (255)
// #pragma auto_inline (off)
#pragma auto_inline (on)
// #define NUMERICS_INLINE inline
#define NUMERICS_INLINE __forceinline

// Do not check sizes!
#define NUMERICS_USE_FAST_COMMON

// Use expression templates.
#define NUMERICS_USE_ET
#define NUMERICS_ET_VALUE
// #define NUMERICS_ET_REFERENCE
// #define NUMERICS_ET_CLOSURE_VALUE
#define NUMERICS_ET_CLOSURE_REFERENCE

// Use iterators.
#define NUMERICS_USE_ITERATOR
// #define NUMERICS_ITERATOR_THRESHOLD 0
#define NUMERICS_ITERATOR_THRESHOLD 32

// Use indexed iterators.
// #define NUMERICS_USE_INDEXED_ITERATOR
// #define NUMERICS_INDEXED_ITERATOR_PROXIES

// Use invariant hoisting.
#define NUMERICS_USE_INVARIANT_HOISTING

#endif

namespace std {

    typedef unsigned size_t;
    typedef signed ptrdiff_t;

    template<class T>
    NUMERICS_INLINE
    const T &max (const T &t1, const T &t2) {
        return t1 > t2 ? t1 : t2;
    }
    template<class T>
    NUMERICS_INLINE
    const T &min (const T &t1, const T &t2) {
        return t1 < t2 ? t1 : t2;
    }

}

#endif 



#ifdef USE_GCC

#define NUMERICS_TYPENAME typename

#define NUMERICS_EXPLICIT explicit

#define NUMERICS_INLINE inline

// Do not check sizes!
#define NUMERICS_USE_FAST_COMMON

// Use expression templates.
#define NUMERICS_USE_ET
#define NUMERICS_ET_VALUE
// #define NUMERICS_ET_REFERENCE
// #define NUMERICS_ET_CLOSURE_VALUE
#define NUMERICS_ET_CLOSURE_REFERENCE

// Use iterators.
#define NUMERICS_USE_ITERATOR
#define NUMERICS_ITERATOR_THRESHOLD 32

// Use indexed iterators.
// #define NUMERICS_USE_INDEXED_ITERATOR
// #define NUMERICS_INDEXED_ITERATOR_PROXIES

// Use invariant hoisting.
#define NUMERICS_USE_INVARIANT_HOISTING

#endif 



#ifdef USE_BCC

#define NUMERICS_TYPENAME typename

#define NUMERICS_EXPLICIT explicit

#define NUMERICS_INLINE inline

// Do not check sizes!
#define NUMERICS_USE_FAST_COMMON

// Use expression templates.
#define NUMERICS_USE_ET
// #define NUMERICS_ET_VALUE
#define NUMERICS_ET_REFERENCE
// #define NUMERICS_ET_CLOSURE_VALUE
#define NUMERICS_ET_CLOSURE_REFERENCE

// Use iterators.
#define NUMERICS_USE_ITERATOR
#define NUMERICS_ITERATOR_THRESHOLD 32

// Use indexed iterators.
// #define NUMERICS_USE_INDEXED_ITERATOR
// #define NUMERICS_INDEXED_ITERATOR_PROXIES

// Use invariant hoisting.
#define NUMERICS_USE_INVARIANT_HOISTING

#endif



#ifdef USE_ICC

#define NUMERICS_TYPENAME typename

#define NUMERICS_EXPLICIT explicit

// #pragma inline_recursion (off)
#pragma inline_recursion (on)
// #pragma inline_depth ()
#pragma inline_depth (255)
// #pragma auto_inline (off)
#pragma auto_inline (on)
#define NUMERICS_INLINE inline

// Do not check sizes!
#define NUMERICS_USE_FAST_COMMON

// Use expression templates.
#define NUMERICS_USE_ET
#define NUMERICS_ET_VALUE
// #define NUMERICS_ET_REFERENCE
// #define NUMERICS_ET_CLOSURE_VALUE
#define NUMERICS_ET_CLOSURE_REFERENCE

// Use iterators.
#define NUMERICS_USE_ITERATOR
#define NUMERICS_ITERATOR_THRESHOLD 32

// Use indexed iterators.
// #define NUMERICS_USE_INDEXED_ITERATOR
// #define NUMERICS_INDEXED_ITERATOR_PROXIES

// Use invariant hoisting.
#define NUMERICS_USE_INVARIANT_HOISTING

#endif



// Forward declarations
namespace numerics {

    template<class T>
    class unbounded_array;

    template<class I, class T>
    class compressed_array;

    struct vector_tag {};

    template<class E>
    struct vector_expression;

#ifdef NUMERICS_DEPRECATED
    template<class E>
    class vector_expression_range;
#endif

    struct matrix_tag {};

    template<class E>
    struct matrix_expression;

#ifdef NUMERICS_DEPRECATED
    template<class E>
    class matrix_expression_row;
    template<class E>
    class matrix_expression_column;
    template<class E>
    class matrix_expression_range;
#endif

    template<class E>
    class vector_range;

    template<class E>
    class matrix_row;
    template<class E>
    class matrix_column;
    template<class E>
    class matrix_range;

    struct forward;
    struct backward;

    template<class F>
    struct vector_assign_scalar;
    template<class F>
    struct vector_assign;

    template<class T, class F = forward, class A = unbounded_array<T> >
    class vector;

    template<class T, class F = forward, class A = compressed_array<std::size_t, T> >
    class sparse_vector;

    struct unknown_orientation_tag {};

    struct row_major_tag {};
    template<class F1 = forward, class F2 = forward>
    struct row_major;

    struct column_major_tag {};
    template<class F1 = forward, class F2 = forward>
    struct column_major;

    template<class F>
    struct matrix_assign_scalar;
    template<class F>
    struct matrix_assign;
    template<class F>
    struct matrix_swap;

    template<class T, class F = row_major<>, class A = unbounded_array<T> >
    class matrix;

    template<class T, class F = row_major<>, class A = unbounded_array<unbounded_array<T> > >
    class vector_of_vector;

    template<class T, class F = row_major<>, class A = unbounded_array<T> >
    class banded_matrix;

    struct lower_tag {};
    struct lower;

    struct upper_tag {};
    struct upper;

    struct unit_lower_tag: public lower_tag {};
    struct unit_lower;

    struct unit_upper_tag: public upper_tag {};
    struct unit_upper;

    template<class T, class F1 = lower, class F2 = row_major<>, class A = unbounded_array<T> >
    class triangular_matrix;

    template<class M, class F = lower>
    class triangular_adaptor;

    template<class T, class F1 = lower, class F2 = row_major<>, class A = unbounded_array<T> >
    class symmetric_matrix;

    template<class M, class F = lower>
    class symmetric_adaptor;

    template<class T, class F1 = lower, class F2 = row_major<>, class A = unbounded_array<T> >
    class hermitean_matrix;

    template<class M, class F = lower>
    class hermitean_adaptor;

    template<class T, class F = row_major<>, class A = compressed_array<std::size_t, T> >
    class sparse_matrix;

    template<class T, class F = row_major<>, class A = compressed_array<std::size_t, compressed_array<std::size_t, T> > >
    class sparse_vector_of_sparse_vector;

}

#endif 









