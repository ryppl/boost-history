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

#ifndef NUMERICS_MATH_H
#define NUMERICS_MATH_H

#include "config.h"
#include "exception.h"
#include "traits.h"

namespace numerics {

    // GNU does not honour std, so we have to use namespace detail?
    namespace detail {

#ifndef NUMERICS_USE_INSTANT 
        NUMERICS_INLINE
        float conj (const float &t) {
            return t;
        }
        NUMERICS_INLINE
        double conj (const double &t) {
            return t;
        }
#else // NUMERICS_USE_INSTANT 
        NUMERICS_INLINE
        float conj (const float &t);
        NUMERICS_INLINE
        double conj (const double &t);
#endif // NUMERICS_USE_INSTANT 

        template<class T>
        NUMERICS_INLINE
        std::complex<T> conj (const std::complex<T> &t) {
            return std::conj (t);
        }

#ifndef NUMERICS_USE_INSTANT 
        NUMERICS_INLINE
        type_traits<float>::norm_type abs (const float &t) {
#ifdef USE_MSVC
            return ::fabsf (t);
#else // USE_MSVC
            return std::fabs (t);
#endif // USE_MSVC
        }
        NUMERICS_INLINE
        type_traits<double>::norm_type abs (const double &t) {
#ifdef USE_MSVC
            return ::fabs (t);
#else // USE_MSVC
            return std::fabs (t);
#endif // USE_MSVC
        }
#else // NUMERICS_USE_INSTANT
        NUMERICS_INLINE
        type_traits<float>::norm_type abs (const float &t);
        NUMERICS_INLINE
        type_traits<double>::norm_type abs (const double &t);
#endif // NUMERICS_USE_INSTANT

        template<class T>
        NUMERICS_INLINE
        type_traits<std::complex<T> >::norm_type abs (const std::complex<T> &t) {
            return std::abs (t);
        }

#ifndef NUMERICS_USE_INSTANT 
        NUMERICS_INLINE
        type_traits<float>::norm_type norm_1 (const float &t) {
            return abs (t);
        }
        NUMERICS_INLINE
        type_traits<double>::norm_type norm_1 (const double &t) {
            return abs (t);
        }
#else // NUMERICS_USE_INSTANT
        NUMERICS_INLINE
        type_traits<float>::norm_type norm_1 (const float &t);
        NUMERICS_INLINE
        type_traits<double>::norm_type norm_1 (const double &t);
#endif // NUMERICS_USE_INSTANT

        template<class T>
        NUMERICS_INLINE
        type_traits<std::complex<T> >::norm_type norm_1 (const std::complex<T> &t) {
            // Oops, should have known that!
            return abs (t.real ()) + abs (t.imag ());
        }

#ifndef NUMERICS_USE_INSTANT 
        NUMERICS_INLINE
        type_traits<float>::norm_type norm_2 (const float &t) {
            return abs (t);
        }
        NUMERICS_INLINE
        type_traits<double>::norm_type norm_2 (const double &t) {
            return abs (t);
        }
#else // NUMERICS_USE_INSTANT 
        NUMERICS_INLINE
        type_traits<float>::norm_type norm_2 (const float &t);
        NUMERICS_INLINE
        type_traits<double>::norm_type norm_2 (const double &t);
#endif // NUMERICS_USE_INSTANT 

        template<class T>
        NUMERICS_INLINE
        type_traits<std::complex<T> >::norm_type norm_2 (const std::complex<T> &t) {
            return abs (t);
        }

#ifndef NUMERICS_USE_INSTANT 
        NUMERICS_INLINE
        type_traits<float>::norm_type norm_inf (const float &t) {
            return abs (t);
        }
        NUMERICS_INLINE
        type_traits<double>::norm_type norm_inf (const double &t) {
            return abs (t);
        }
#else // NUMERICS_USE_INSTANT 
        NUMERICS_INLINE
        type_traits<float>::norm_type norm_inf (const float &t);
        NUMERICS_INLINE
        type_traits<double>::norm_type norm_inf (const double &t);
#endif // NUMERICS_USE_INSTANT 

        template<class T>
        NUMERICS_INLINE
        type_traits<std::complex<T> >::norm_type norm_inf (const std::complex<T> &t) {
            // Oops, should have known that!
            return std::max (abs (t.real ()), abs (t.imag ()));
        }

    }

}

#endif // NUMERICS_MATH_H