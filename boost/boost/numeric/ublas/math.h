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

#include <cmath>

#include "config.h"
#include "exception.h"
#include "traits.h"

namespace numerics {

    // GNU does not honour std, so we have to use namespace detail?
    namespace detail {

#ifndef NUMERICS_USE_INSTANT 
        NUMERICS_INLINE
        float real (const float &t) {
            return t;
        }
        NUMERICS_INLINE
        double real (const double &t) {
            return t;
        }
#else 
        NUMERICS_INLINE
        float real (const float &t);
        NUMERICS_INLINE
        double real (const double &t);
#endif 

        template<class T>
        NUMERICS_INLINE
        T real (const std::complex<T> &t) {
            return std::real (t);
        }

#ifndef NUMERICS_USE_INSTANT 
        NUMERICS_INLINE
        float imag  (const float &t) {
            return 0;
        }
        NUMERICS_INLINE
        double imag (const double &t) {
            return 0;
        }
#else 
        NUMERICS_INLINE
        float imag (const float &t);
        NUMERICS_INLINE
        double imag (const double &t);
#endif 

        template<class T>
        NUMERICS_INLINE
        T imag (const std::complex<T> &t) {
            return std::imag (t);
        }

#ifndef NUMERICS_USE_INSTANT 
        NUMERICS_INLINE
        float conj (const float &t) {
            return t;
        }
        NUMERICS_INLINE
        double conj (const double &t) {
            return t;
        }
#else 
        NUMERICS_INLINE
        float conj (const float &t);
        NUMERICS_INLINE
        double conj (const double &t);
#endif 

        template<class T>
        NUMERICS_INLINE
        std::complex<T> conj (const std::complex<T> &t) {
            return std::conj (t);
        }

#ifndef NUMERICS_USE_INSTANT 
        NUMERICS_INLINE
        float sqrt (const float &t) {
#ifdef USE_MSVC
            return ::sqrt (t);
#else 
            return std::sqrt (t);
#endif 
        }
        NUMERICS_INLINE
        double sqrt (const double &t) {
#ifdef USE_MSVC
            return ::sqrt (t);
#else 
            return std::sqrt (t);
#endif 
        }
#else 
        NUMERICS_INLINE
        float sqrt (const float &t);
        NUMERICS_INLINE
        double sqrt (const double &t);
#endif 

        template<class T>
        NUMERICS_INLINE
        std::complex<T> sqrt (const std::complex<T> &t) {
            return std::sqrt (t);
        }

#ifndef NUMERICS_USE_INSTANT 
        NUMERICS_INLINE
        type_traits<float>::norm_type abs (const float &t) {
#ifdef USE_MSVC
            return ::fabsf (t);
#else 
            return std::fabs (t);
#endif 
        }
        NUMERICS_INLINE
        type_traits<double>::norm_type abs (const double &t) {
#ifdef USE_MSVC
            return ::fabs (t);
#else 
            return std::fabs (t);
#endif 
        }
#else 
        NUMERICS_INLINE
        type_traits<float>::norm_type abs (const float &t);
        NUMERICS_INLINE
        type_traits<double>::norm_type abs (const double &t);
#endif 

#ifndef USE_GCC
        template<class T>
        NUMERICS_INLINE
        typename type_traits<std::complex<T> >::norm_type abs (const std::complex<T> &t) {
            return std::abs (t);
        }
#else
        template<class T>
        NUMERICS_INLINE
        typename type_traits<T>::norm_type abs (const T &t) {
            return std::abs (t);
        }
#endif

#ifndef NUMERICS_USE_INSTANT 
        NUMERICS_INLINE
        type_traits<float>::norm_type norm_1 (const float &t) {
            return abs (t);
        }
        NUMERICS_INLINE
        type_traits<double>::norm_type norm_1 (const double &t) {
            return abs (t);
        }
#else 
        NUMERICS_INLINE
        type_traits<float>::norm_type norm_1 (const float &t);
        NUMERICS_INLINE
        type_traits<double>::norm_type norm_1 (const double &t);
#endif 

#ifndef USE_GCC
        template<class T>
        NUMERICS_INLINE
        typename type_traits<std::complex<T> >::norm_type norm_1 (const std::complex<T> &t) {
            // Oops, should have known that!
            return abs (t.real ()) + abs (t.imag ());
        }
#else
        template<class T>
        NUMERICS_INLINE
        typename type_traits<T>::norm_type norm_1 (const T &t) {
            // Oops, should have known that!
            return abs (t.real ()) + abs (t.imag ());
        }
#endif

#ifndef NUMERICS_USE_INSTANT 
        NUMERICS_INLINE
        type_traits<float>::norm_type norm_2 (const float &t) {
            return abs (t);
        }
        NUMERICS_INLINE
        type_traits<double>::norm_type norm_2 (const double &t) {
            return abs (t);
        }
#else 
        NUMERICS_INLINE
        type_traits<float>::norm_type norm_2 (const float &t);
        NUMERICS_INLINE
        type_traits<double>::norm_type norm_2 (const double &t);
#endif 

#ifndef USE_GCC
        template<class T>
        NUMERICS_INLINE
        typename type_traits<std::complex<T> >::norm_type norm_2 (const std::complex<T> &t) {
            return abs (t);
        }
#else
        template<class T>
        NUMERICS_INLINE
        typename type_traits<T>::norm_type norm_2 (const T &t) {
            return abs (t);
        }
#endif

#ifndef NUMERICS_USE_INSTANT 
        NUMERICS_INLINE
        type_traits<float>::norm_type norm_inf (const float &t) {
            return abs (t);
        }
        NUMERICS_INLINE
        type_traits<double>::norm_type norm_inf (const double &t) {
            return abs (t);
        }
#else 
        NUMERICS_INLINE
        type_traits<float>::norm_type norm_inf (const float &t);
        NUMERICS_INLINE
        type_traits<double>::norm_type norm_inf (const double &t);
#endif 

#ifndef USE_GCC
        template<class T>
        NUMERICS_INLINE
        typename type_traits<std::complex<T> >::norm_type norm_inf (const std::complex<T> &t) {
            // Oops, should have known that!
            return std::max (abs (t.real ()), abs (t.imag ()));
        }
#else
        template<class T>
        NUMERICS_INLINE
        typename type_traits<T>::norm_type norm_inf (const T &t) {
            // Oops, should have known that!
            return std::max (abs (t.real ()), abs (t.imag ()));
        }
#endif

    }

}

#endif 


