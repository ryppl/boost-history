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

#ifndef NUMERICS_MATH_H
#define NUMERICS_MATH_H

#include <algorithm>

#include <boost/numeric/ublas/config.h>
#include <boost/numeric/ublas/traits.h>

namespace boost { namespace numerics {

    // GNU does not honour std, so we have to use namespace detail?
    namespace detail {

        template<class T>
        NUMERICS_INLINE
        typename type_traits<T>::real_type real (const T &t) {
            return type_traits<T>::real (t);
        }

        template<class T>
        NUMERICS_INLINE
        typename type_traits<T>::real_type imag (const T &t) {
            return type_traits<T>::imag (t);
        }

        template<class T>
        NUMERICS_INLINE
        T conj (const T &t) {
            return type_traits<T>::conj (t);
        }

        template<class T>
        NUMERICS_INLINE
        typename type_traits<T>::real_type abs (const T &t) {
            return type_traits<T>::abs (t);
        }

        template<class T>
        NUMERICS_INLINE
        T sqrt (const T &t) {
            return type_traits<T>::sqrt (t);
        }

        template<class T>
        NUMERICS_INLINE
        typename type_traits<T>::real_type norm_1 (const T &t) {
            // Oops, should have known that!
            return abs (real (t)) + abs (imag (t));
        }

        template<class T>
        NUMERICS_INLINE
        typename type_traits<T>::real_type norm_2 (const T &t) {
            return abs (t);
        }

        template<class T>
        NUMERICS_INLINE
        typename type_traits<T>::real_type norm_inf (const T &t) {
            // Oops, should have known that!
            return std::max (abs (real (t)), abs (imag (t)));
        }

    }

}}

#endif


