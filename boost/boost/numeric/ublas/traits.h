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

#include <cmath>
#include <complex>

#include "config.h"

// Promote traits borrowed from Todd Veldhuizen

namespace boost { namespace numerics {

    template<class T>
    struct type_traits {
        typedef T value_type;
        typedef T real_type;
        typedef T precision_type;

        BOOST_STATIC_CONSTANT (std::size_t, plus_complexity = 0);
        BOOST_STATIC_CONSTANT (std::size_t, multiplies_complexity = 0);

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

        BOOST_STATIC_CONSTANT (std::size_t, plus_complexity = 1);
        BOOST_STATIC_CONSTANT (std::size_t, multiplies_complexity = 1);

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
#ifdef BOOST_NO_STDC_NAMESPACE
            return ::fabsf (t);
#else
#ifdef NUMERICS_FLOAT_FUNCTION
            return std::fabsf (t);
#else
            return std::fabs (t);
#endif
#endif
        }
        static
        NUMERICS_INLINE
        value_type sqrt (const value_type &t) {
#ifdef BOOST_NO_STDC_NAMESPACE
            return ::sqrtf (t);
#else
#ifdef NUMERICS_FLOAT_FUNCTION
            return std::sqrtf (t);
#else
            return std::sqrt (t);
#endif
#endif
        }
    };
    template<>
    struct type_traits<double> {
        typedef double value_type;
        typedef double real_type;
        typedef double precision_type;

        BOOST_STATIC_CONSTANT (std::size_t, plus_complexity = 1);
        BOOST_STATIC_CONSTANT (std::size_t, multiplies_complexity = 1);

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
#ifdef BOOST_NO_STDC_NAMESPACE
            return ::fabs (t);
#else
            return std::fabs (t);
#endif
        }
        static
        NUMERICS_INLINE
        value_type sqrt (const value_type &t) {
#ifdef BOOST_NO_STDC_NAMESPACE
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

        BOOST_STATIC_CONSTANT (std::size_t, plus_complexity = 2);
        BOOST_STATIC_CONSTANT (std::size_t, multiplies_complexity = 6);

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

        BOOST_STATIC_CONSTANT (std::size_t, plus_complexity = 2);
        BOOST_STATIC_CONSTANT (std::size_t, multiplies_complexity = 6);

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
    struct promote_traits<float, std::complex<float> > {
        typedef std::complex<float> promote_type;
    };
    template<>
    struct promote_traits<std::complex<float>, float> {
        typedef std::complex<float> promote_type;
    };
    template<>
    struct promote_traits<float, std::complex<double> > {
        typedef std::complex<double> promote_type;
    };
    template<>
    struct promote_traits<std::complex<double>, float> {
        typedef std::complex<double> promote_type;
    };

    template<>
    struct promote_traits<double, std::complex<float> > {
        // Here we'd better go the conservative way.
        // typedef std::complex<float> promote_type;
        typedef std::complex<double> promote_type;
    };
    template<>
    struct promote_traits<std::complex<float>, double> {
        // Here we'd better go the conservative way.
        // typedef std::complex<float> promote_type;
        typedef std::complex<double> promote_type;
    };
    template<>
    struct promote_traits<double, std::complex<double> > {
        typedef std::complex<double> promote_type;
    };
    template<>
    struct promote_traits<std::complex<double>, double> {
        typedef std::complex<double> promote_type;
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
    struct packed_proxy_tag: public sparse_proxy_tag {};
    struct packed_tag: public packed_proxy_tag {};
    struct dense_proxy_tag: public packed_proxy_tag {};
    struct dense_tag: public dense_proxy_tag {};

    template<class S>
    struct proxy_traits {
        typedef S storage_category;
    };

    template<>
    struct proxy_traits<sparse_tag> {
        typedef sparse_proxy_tag storage_category;
    };
    template<>
    struct proxy_traits<packed_tag> {
        typedef packed_proxy_tag storage_category;
    };
    template<>
    struct proxy_traits<dense_tag> {
        typedef dense_proxy_tag storage_category;
    };

    struct sparse_bidirectional_iterator_tag : public std::bidirectional_iterator_tag {};
    struct packed_random_access_iterator_tag : public std::random_access_iterator_tag {};
    struct dense_random_access_iterator_tag : public packed_random_access_iterator_tag {};

    // Thanks to Kresimir Fresl for convincing Comeau with iterator_base_traits ;-)
    template<class IC>
    struct iterator_base_traits {};

    template<>
    struct iterator_base_traits<std::forward_iterator_tag> {
        template<class I, class T>
        struct iterator_base {
            typedef forward_iterator_base<std::forward_iterator_tag, I, T> type;
        };
    };

    template<>
    struct iterator_base_traits<std::bidirectional_iterator_tag> {
        template<class I, class T>
        struct iterator_base {
            typedef bidirectional_iterator_base<std::bidirectional_iterator_tag, I, T> type;
        };
    };
    template<>
    struct iterator_base_traits<sparse_bidirectional_iterator_tag> {
        template<class I, class T>
        struct iterator_base {
            typedef bidirectional_iterator_base<sparse_bidirectional_iterator_tag, I, T> type;
        };
    };

    template<>
    struct iterator_base_traits<std::random_access_iterator_tag> {
        template<class I, class T>
        struct iterator_base {
            typedef random_access_iterator_base<std::bidirectional_iterator_tag, I, T> type;
        };
    };
    template<>
    struct iterator_base_traits<packed_random_access_iterator_tag> {
        template<class I, class T>
        struct iterator_base {
            typedef random_access_iterator_base<packed_random_access_iterator_tag, I, T> type;
        };
    };
    template<>
    struct iterator_base_traits<dense_random_access_iterator_tag> {
        template<class I, class T>
        struct iterator_base {
            typedef random_access_iterator_base<dense_random_access_iterator_tag, I, T> type;
        };
    };

    template<class I1, class I2>
    struct iterator_restrict_traits {
        typedef I1 iterator_category;
    };

    // FIXME: eliminate this.
    // template<>
    // struct iterator_restrict_traits<std::random_access_iterator_tag, std::bidirectional_iterator_tag> {
    //     typedef std::bidirectional_iterator_tag iterator_category;
    // };

    template<>
    struct iterator_restrict_traits<packed_random_access_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_bidirectional_iterator_tag iterator_category;
    };
    template<>
    struct iterator_restrict_traits<sparse_bidirectional_iterator_tag, packed_random_access_iterator_tag> {
        typedef sparse_bidirectional_iterator_tag iterator_category;
    };

    template<>
    struct iterator_restrict_traits<dense_random_access_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_bidirectional_iterator_tag iterator_category;
    };
    template<>
    struct iterator_restrict_traits<sparse_bidirectional_iterator_tag, dense_random_access_iterator_tag> {
        typedef sparse_bidirectional_iterator_tag iterator_category;
    };

    template<>
    struct iterator_restrict_traits<dense_random_access_iterator_tag, packed_random_access_iterator_tag> {
        typedef packed_random_access_iterator_tag iterator_category;
    };
    template<>
    struct iterator_restrict_traits<packed_random_access_iterator_tag, dense_random_access_iterator_tag> {
        typedef packed_random_access_iterator_tag iterator_category;
    };

}}

#endif


