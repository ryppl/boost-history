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

#define NUMERICS_TYPENAME

#define NUMERICS_EXPLICIT explicit

#ifdef _DEBUG

#define NUMERICS_USE_INSTANT
#define NUMERICS_INLINE

// Bounds check
#define NUMERICS_BOUNDS_CHECK

#else 

// #pragma inline_recursion (off)
#pragma inline_recursion (on)
// #pragma inline_depth ()
#pragma inline_depth (255)
// #pragma auto_inline (off)
#pragma auto_inline (on)
#define NUMERICS_INLINE inline
// #define NUMERICS_INLINE __forceinline

// Do not check sizes!
#define NUMERICS_FAST_COMMON

// Use expression templates.
#define NUMERICS_USE_ET

// Use iterators.
#define NUMERICS_USE_ITERATOR

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
#define NUMERICS_FAST_COMMON

// Use expression templates.
#define NUMERICS_USE_ET

// Use iterators.
#define NUMERICS_USE_ITERATOR

namespace std {

    typedef unsigned size_t;
    typedef signed ptrdiff_t;

}

namespace numerics {

    namespace detail {

        template<class I1, class I2>
        NUMERICS_INLINE
        void copy (I1 it1, const I1 &it1_end, I2 it2) {
            while (it1 != it1_end)
                *it2 = *it1, ++ it2, ++ it1;
        }
        template<class I1, class I2>
        NUMERICS_INLINE
        void swap_ranges (I1 it1, const I1 &it1_end, I2 it2) {
            while (it1 != it1_end)
                std::swap (*it1, *it2), ++ it2, ++ it1;
        }

    }

}

#endif 



#ifdef USE_BCC

#define NUMERICS_TYPENAME typename

#define NUMERICS_EXPLICIT explicit

#define NUMERICS_INLINE inline

// Do not check sizes!
#define NUMERICS_FAST_COMMON

// Use expression templates.
#define NUMERICS_USE_ET

// Use iterators.
#define NUMERICS_USE_ITERATOR

#endif 



// Forward declarations
namespace numerics {

    struct sparse_proxy_tag {};
    struct sparse_tag: public sparse_proxy_tag {};
    struct dense_tag: public sparse_tag {};

    template<class S>
    struct proxy_traits {
        typedef S storage_category;
    };

    template<>
    struct proxy_traits<sparse_tag> {
        typedef sparse_proxy_tag storage_category;
    };

    template<class T>
    class unbounded_array;

    template<class I, class T>
    class compressed_array;

    template<class E>
    struct vector_expression;

    template<class E>
    struct matrix_expression;

    struct forward;
    template<class V>
    class vector_range;
    template<class V>
    class vector_slice;
    template<class T, class A = unbounded_array<T>, class F = forward>
    class vector;

    template<class T, class A = compressed_array<std::size_t, T>, class F = forward>
    class sparse_vector;

    template<class F1 = forward, class F2 = forward>
    struct row_major;
    template<class F1 = forward, class F2 = forward>
    struct column_major;
    template<class M>
    class matrix_row;
    template<class M>
    class matrix_column;
    template<class M>
    class matrix_vector_range;
    template<class M>
    class matrix_vector_slice;
    template<class T, class A = unbounded_array<T>, class F = row_major<> >
    class matrix;
    template<class M>
    class matrix_range;
    template<class M>
    class matrix_slice;
    template<class T, class A = unbounded_array<T>, class F = row_major<> >
    class band_matrix;

    template<class T, class A = compressed_array<std::size_t, T>, class F = row_major<> >
    class sparse_matrix;
}

#endif 


