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

#ifdef USE_MSVC

#pragma warning (disable: 4355)
#pragma warning (disable: 4503)
#pragma warning (disable: 4786)

namespace std {

    template<class T>
    const T &max (const T &t1, const T &t2) {
        return t1 > t2 ? t1 : t2;
    }
    template<class T>
    const T &min (const T &t1, const T &t2) {
        return t1 < t2 ? t1 : t2;
    }

}

#define NUMERICS_EXPLICIT explicit

#ifdef _DEBUG

#define NUMERICS_USE_INSTANT
#define NUMERICS_INLINE

// Bounds check
#define NUMERICS_BOUNDS_CHECK

#else // _DEBUG

// #pragma inline_recursion (off)
 #pragma inline_recursion (on)
// #pragma inline_depth ()
#pragma inline_depth (255)
// #define NUMERICS_INLINE inline
#define NUMERICS_INLINE __forceinline

// Do not check sizes!
#define NUMERICS_FAST_COMMON

// Use expression templates.
#define NUMERICS_USE_ET

// Use iterators.
#define NUMERICS_USE_ITERATOR

#endif // _DEBUG

#endif // USE_MSVC

#ifdef USE_GCC

#define NUMERICS_EXPLICIT explicit

#define NUMERICS_INLINE inline

// Do not check sizes!
#define NUMERICS_FAST_COMMON

// Use iterators.
#define NUMERICS_USE_ITERATOR

#endif // USE_GCC

#ifdef USE_BCC

#define NUMERICS_EXPLICIT explicit

#define NUMERICS_INLINE inline

// Do not check sizes!
#define NUMERICS_FAST_COMMON

// Use expression templates.
#define NUMERICS_USE_ET

// Use iterators.
// #define NUMERICS_USE_ITERATOR

#endif // USE_BCC

// Forward declarations
namespace numerics {

    typedef unsigned size_type;

    template<class T, class E>
    struct vector_expression;

    template<class T, class E>
    struct matrix_expression;

    template<class T>
    class unbounded_array;

    struct forward;
    template<class T, class A = unbounded_array<T>, class F = forward>
    class vector;
    template<class V>
    class vector_slice;

    template<class F1 = forward, class F2 = forward>
    struct row_major;
    template<class F1 = forward, class F2 = forward>
    struct column_major;
    template<class M>
    class matrix_vector_slice;
    template<class T, class A = unbounded_array<T>, class F = row_major<> >
    class matrix;
    template<class M>
    class matrix_slice;
    template<class T, class A = unbounded_array<T>, class F = row_major<> >
    class band_matrix;

}

#endif // NUMERICS_CONFIG_H