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

#ifndef NUMERICS_TRAITS_H
#define NUMERICS_TRAITS_H

#include <complex>

#include "config.h"

// Promote traits borrowed from Todd Veldhuizen

namespace numerics {

// We should use BOOST_STATIC_CONSTANT one day.
#ifndef USE_MSVC
#define NUMERICS_STATIC_CONSTANT(type, assignment) static const type assignment
#else
// Static constant workaround from John Maddock
#define NUMERICS_STATIC_CONSTANT(type, assignment) enum { assignment }
#endif

    template<class T>
    struct type_traits {
        typedef T value_type;
        typedef T real_type;
        typedef T precision_type;

        NUMERICS_STATIC_CONSTANT (std::size_t, plus_complexity = 0);
        NUMERICS_STATIC_CONSTANT (std::size_t, multiplies_complexity = 0);

        static
        NUMERICS_INLINE
        real_type real (const value_type &t);
        static
        NUMERICS_INLINE
        real_type imag (const value_type &t);
        static
        NUMERICS_INLINE
        value_type conj (const value_type &t);

        static
        NUMERICS_INLINE
        real_type abs (const value_type &t);
        static
        NUMERICS_INLINE
        value_type sqrt (const value_type &t);
    };

    template<>
    struct type_traits<float> {
        typedef float value_type;
        typedef float real_type;
        typedef double precision_type;

        NUMERICS_STATIC_CONSTANT (std::size_t, plus_complexity = 1);
        NUMERICS_STATIC_CONSTANT (std::size_t, multiplies_complexity = 1);

        static
        NUMERICS_INLINE
        real_type real (const value_type &t) {
                return t;
        }
        static
        NUMERICS_INLINE
        real_type imag (const value_type &t) {
                return 0;
        }
        static
        NUMERICS_INLINE
        value_type conj (const value_type &t) {
                return t;
        }

        static
        NUMERICS_INLINE
        real_type abs (const value_type &t) {
#ifdef USE_MSVC
            return ::fabsf (t);
#else
            return std::fabs (t);
#endif
        }
        static
        NUMERICS_INLINE
        value_type sqrt (const value_type &t) {
#ifdef USE_MSVC
            return ::sqrt (t);
#else
            return std::sqrt (t);
#endif
        }
    };
    template<>
    struct type_traits<double> {
        typedef double value_type;
        typedef double real_type;
        typedef double precision_type;

        NUMERICS_STATIC_CONSTANT (std::size_t, plus_complexity = 1);
        NUMERICS_STATIC_CONSTANT (std::size_t, multiplies_complexity = 1);

        static
        NUMERICS_INLINE
        real_type real (const value_type &t) {
                return t;
        }
        static
        NUMERICS_INLINE
        real_type imag (const value_type &t) {
                return 0;
        }
        static
        NUMERICS_INLINE
        value_type conj (const value_type &t) {
                return t;
        }

        static
        NUMERICS_INLINE
        real_type abs (const value_type &t) {
#ifdef USE_MSVC
            return ::fabs (t);
#else
            return std::fabs (t);
#endif
        }
        static
        NUMERICS_INLINE
        value_type sqrt (const value_type &t) {
#ifdef USE_MSVC
            return ::sqrt (t);
#else
            return std::sqrt (t);
#endif
        }
    };

    template<>
    struct type_traits<std::complex<float> > {
        typedef std::complex<float> value_type;
        typedef float real_type;
        typedef std::complex<double> precision_type;

        NUMERICS_STATIC_CONSTANT (std::size_t, plus_complexity = 2);
        NUMERICS_STATIC_CONSTANT (std::size_t, multiplies_complexity = 6);

        static
        NUMERICS_INLINE
        real_type real (const value_type &t) {
                // return t.real ();
                return std::real (t);
        }
        static
        NUMERICS_INLINE
        real_type imag (const value_type &t) {
                // return t.imag ();
                return std::imag (t);
        }
        static
        NUMERICS_INLINE
        value_type conj (const value_type &t) {
                // return t.conj ();
                return std::conj (t);
        }

        static
        NUMERICS_INLINE
        real_type abs (const value_type &t) {
                return std::abs (t);
        }
        static
        NUMERICS_INLINE
        value_type sqrt (const value_type &t) {
                return std::sqrt (t);
        }
    };
    template<>
    struct type_traits<std::complex<double> > {
        typedef std::complex<double> value_type;
        typedef double real_type;
        typedef std::complex<double> precision_type;

        NUMERICS_STATIC_CONSTANT (std::size_t, plus_complexity = 2);
        NUMERICS_STATIC_CONSTANT (std::size_t, multiplies_complexity = 6);

        static
        NUMERICS_INLINE
        real_type real (const value_type &t) {
                // return t.real ();
                return std::real (t);
        }
        static
        NUMERICS_INLINE
        real_type imag (const value_type &t) {
                // return t.imag ();
                return std::imag (t);
        }
        static
        NUMERICS_INLINE
        value_type conj (const value_type &t) {
                // return t.conj ();
                return std::conj (t);
        }

        static
        NUMERICS_INLINE
        real_type abs (const value_type &t) {
                return std::abs (t);
        }
        static
        NUMERICS_INLINE
        value_type sqrt (const value_type &t) {
                return std::sqrt (t);
        }
    };

    template<class T1, class T2>
    struct promote_traits {
        typedef T1 promote_type;
    };

    template<>
    struct promote_traits<float, double> {
        typedef double promote_type;
    };
    template<>
    struct promote_traits<double, float> {
        typedef double promote_type;
    };

    template<>
    struct promote_traits<std::complex<float>, std::complex<double> > {
        typedef std::complex<double> promote_type;
    };
    template<>
    struct promote_traits<std::complex<double>, std::complex<float> > {
        typedef std::complex<double> promote_type;
    };

    struct unknown_storage_tag {};
    struct sparse_proxy_tag {};
    struct sparse_tag: public sparse_proxy_tag {};
    struct packed_tag: public sparse_tag {};
    struct dense_tag: public packed_tag {};

    template<class S>
    struct proxy_traits {
        typedef S storage_category;
    };

    template<>
    struct proxy_traits<sparse_tag> {
        typedef sparse_proxy_tag storage_category;
    };

    struct sparse_bidirectional_iterator_tag : public std::bidirectional_iterator_tag {};
    struct packed_random_access_iterator_tag : public std::random_access_iterator_tag {};
    struct dense_random_access_iterator_tag : public packed_random_access_iterator_tag {};

    template<class I1, class I2>
    struct restrict_traits {
        typedef I1 iterator_category;
    };

    // FIXME: eliminate this.
    template<>
    struct restrict_traits<std::random_access_iterator_tag, std::bidirectional_iterator_tag> {
        typedef std::bidirectional_iterator_tag iterator_category;
    };

    template<>
    struct restrict_traits<packed_random_access_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_bidirectional_iterator_tag iterator_category;
    };
    template<>
    struct restrict_traits<dense_random_access_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_bidirectional_iterator_tag iterator_category;
    };
    template<>
    struct restrict_traits<dense_random_access_iterator_tag, packed_random_access_iterator_tag> {
        typedef packed_random_access_iterator_tag iterator_category;
    };

}

#endif 


