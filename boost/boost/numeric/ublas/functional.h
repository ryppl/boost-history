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

#ifndef NUMERICS_FUNCTIONAL_H
#define NUMERICS_FUNCTIONAL_H

#include <functional>

#include "config.h"
#include "exception.h"
#include "traits.h"
#include "math.h"

#ifdef USE_GCC

namespace numerics {

    namespace detail {

        NUMERICS_INLINE
        float negate (const float &t) {
            return - t;
        }
        NUMERICS_INLINE
        double negate (const double &t) {
            return - t;
        }

        template<class T>
        NUMERICS_INLINE
        std::complex<T> negate (const std::complex<T> &t) {
//            return - t;
            return std::complex<T> (- t.real (), - t.imag ());
        }

        NUMERICS_INLINE
        float plus (const float &t1, const float &t2) {
            return t1 + t2;
        }
        NUMERICS_INLINE
        double plus (const double &t1, const double &t2) {
            return t1 + t2;
        }

        template<class T>
        NUMERICS_INLINE
        std::complex<T> plus (const std::complex<T> &t1, const std::complex<T> &t2) {
//            return t1 + t2;
            return std::complex<T> (t1.real () + t2.real (), t1.imag () + t2.imag ());
        }

        NUMERICS_INLINE
        float minus (const float &t1, const float &t2) {
            return t1 - t2;
        }
        NUMERICS_INLINE
        double minus (const double &t1, const double &t2) {
            return t1 - t2;
        }

        template<class T>
        NUMERICS_INLINE
        std::complex<T> minus (const std::complex<T> &t1, const std::complex<T> &t2) {
//            return t1 - t2;
            return std::complex<T> (t1.real () - t2.real (), t1.imag () - t2.imag ());
        }

        NUMERICS_INLINE
        float multiplies (const float &t1, const float &t2) {
            return t1 * t2;
        }
        NUMERICS_INLINE
        double multiplies (const double &t1, const double &t2) {
            return t1 * t2;
        }

        template<class T>
        NUMERICS_INLINE
        std::complex<T> multiplies (const std::complex<T> &t1, const std::complex<T> &t2) {
//            return t1 * t2;
            return std::complex<T> (t1.real () * t2.real () - t1.imag () * t2.imag (), 
                                    t1.real () * t2.imag () + t1.imag () * t2.real ());
        }

        NUMERICS_INLINE
        float divides (const float &t1, const float &t2) {
            return t1 / t2;
        }
        NUMERICS_INLINE
        double divides (const double &t1, const double &t2) {
            return t1 / t2;
        }

        template<class T>
        NUMERICS_INLINE
        std::complex<T> divides (const std::complex<T> &t1, const std::complex<T> &t2) {
//            return t1 / t2;
            T r1 (multiplies (t2, conj (t2)).real ());
            std::complex<T> r2 (multiplies (t1, conj (t2)));
            return std::complex<T> (r1 * r2.real (), r1 * r2.imag ());
        }

    }

}

#endif

namespace numerics {

    // Scalar functors

    // Unary
    template<class T>
    struct scalar_unary_functor {
        typedef T value_type;
    };

    template<class T>
    struct scalar_identity: 
        public scalar_unary_functor<T> {
        typedef typename scalar_unary_functor<T>::value_type value_type;

        NUMERICS_INLINE
        value_type operator () (const value_type &t) const { 
            return t; 
        }
    };
    template<class T>
    struct scalar_negate: 
        public scalar_unary_functor<T> {
        typedef typename scalar_unary_functor<T>::value_type value_type;

        NUMERICS_INLINE
        value_type operator () (const value_type &t) const { 
#ifndef USE_GCC
            return - t; 
#else
            return detail::negate (t);
#endif
        }
    };
    template<class T>
    struct scalar_conj: 
        public scalar_unary_functor<T> {
        typedef typename scalar_unary_functor<T>::value_type value_type;

        NUMERICS_INLINE
        value_type operator () (const value_type &t) const { 
            return detail::conj (t); 
        }
    };

    // Binary
    template<class T1, class T2>
    struct scalar_binary_functor {
        typedef T1 value1_type;
        typedef T2 value2_type;
        typedef typename promote_traits<T1, T2>::promote_type value_type;
    };

    template<class T1, class T2>
    struct scalar_plus: 
        public scalar_binary_functor<T1, T2> { 
        typedef typename scalar_binary_functor<T1, T2>::value1_type value1_type;
        typedef typename scalar_binary_functor<T1, T2>::value2_type value2_type;
        typedef typename scalar_binary_functor<T1, T2>::value_type value_type;

        NUMERICS_INLINE
        value_type operator () (const value1_type &t1, const value2_type &t2) const { 
#ifndef USE_GCC
            return t1 + t2; 
#else
            return detail::plus (t1, t2);
#endif
        }
    };
    template<class T1, class T2>
    struct scalar_minus: 
        public scalar_binary_functor<T1, T2> { 
        typedef typename scalar_binary_functor<T1, T2>::value1_type value1_type;
        typedef typename scalar_binary_functor<T1, T2>::value2_type value2_type;
        typedef typename scalar_binary_functor<T1, T2>::value_type value_type;

        NUMERICS_INLINE
        value_type operator () (const value1_type &t1, const value2_type &t2) const { 
#ifndef USE_GCC
            return t1 - t2; 
#else
            return detail::minus (t1, t2);
#endif
        }
    };
    template<class T1, class T2>
    struct scalar_multiplies: 
        public scalar_binary_functor<T1, T2> { 
        typedef typename scalar_binary_functor<T1, T2>::value1_type value1_type;
        typedef typename scalar_binary_functor<T1, T2>::value2_type value2_type;
        typedef typename scalar_binary_functor<T1, T2>::value_type value_type;

        NUMERICS_INLINE
        value_type operator () (const value1_type &t1, const value2_type &t2) const { 
#ifndef USE_GCC
            return t1 * t2; 
#else
            return detail::multiplies (t1, t2);
#endif
        }
    };
    template<class T1, class T2>
    struct scalar_divides: 
        public scalar_binary_functor<T1, T2> { 
        typedef typename scalar_binary_functor<T1, T2>::value1_type value1_type;
        typedef typename scalar_binary_functor<T1, T2>::value2_type value2_type;
        typedef typename scalar_binary_functor<T1, T2>::value_type value_type;

        NUMERICS_INLINE
        value_type operator () (const value1_type &t1, const value2_type &t2) const { 
#ifndef USE_GCC
            return t1 / t2; 
#else
            return detail::divides (t1, t2);
#endif
        }
    };

    struct assign_tag {};
    struct computed_assign_tag {};

    template<class T1, class T2>
    struct scalar_assign: 
        public scalar_binary_functor<T1, T2> {
        typedef typename scalar_binary_functor<T1, T2>::value1_type value1_type;
        typedef typename scalar_binary_functor<T1, T2>::value2_type value2_type;
        typedef assign_tag assign_category;

        NUMERICS_INLINE
        void operator () (value1_type &t1, const value2_type &t2) const { 
            t1 = t2;
        }
    };
    template<class T1, class T2>
    struct scalar_plus_assign: 
        public scalar_binary_functor<T1, T2> {
        typedef typename scalar_binary_functor<T1, T2>::value1_type value1_type;
        typedef typename scalar_binary_functor<T1, T2>::value2_type value2_type;
        typedef computed_assign_tag assign_category;

        NUMERICS_INLINE
        void operator () (value1_type &t1, const value2_type &t2) const { 
            t1 += t2;
        }
    };
    template<class T1, class T2>
    struct scalar_minus_assign: 
        public scalar_binary_functor<T1, T2> {
        typedef typename scalar_binary_functor<T1, T2>::value1_type value1_type;
        typedef typename scalar_binary_functor<T1, T2>::value2_type value2_type;
        typedef computed_assign_tag assign_category;

        NUMERICS_INLINE
        void operator () (value1_type &t1, const value2_type &t2) const { 
            t1 -= t2;
        }
    };
    template<class T1, class T2>
    struct scalar_multiplies_assign: 
        public scalar_binary_functor<T1, T2> {
        typedef typename scalar_binary_functor<T1, T2>::value1_type value1_type;
        typedef typename scalar_binary_functor<T1, T2>::value2_type value2_type;
        typedef computed_assign_tag assign_category;

        NUMERICS_INLINE
        void operator () (value1_type &t1, const value2_type &t2) const { 
            t1 *= t2;
        }
    };
    template<class T1, class T2>
    struct scalar_divides_assign: 
        public scalar_binary_functor<T1, T2> {
        typedef typename scalar_binary_functor<T1, T2>::value1_type value1_type;
        typedef typename scalar_binary_functor<T1, T2>::value2_type value2_type;
        typedef computed_assign_tag assign_category;

        NUMERICS_INLINE
        void operator () (value1_type &t1, const value2_type &t2) const { 
            t1 /= t2;
        }
    };

    template<class T1, class T2>
    struct scalar_swap: 
        public scalar_binary_functor<T1, T2> {
        typedef typename scalar_binary_functor<T1, T2>::value1_type value1_type;
        typedef typename scalar_binary_functor<T1, T2>::value2_type value2_type;

        NUMERICS_INLINE
        void operator () (value1_type &t1, value2_type &t2) const { 
            std::swap (t1, t2);
        }
    };

    // Vector functors

    // Unary returning scalar
    template<class T>
    struct vector_scalar_unary_functor {
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
    };

    template<class T>
    struct vector_sum: 
        public vector_scalar_unary_functor<T> {
        typedef typename vector_scalar_unary_functor<T>::size_type size_type;
        typedef typename vector_scalar_unary_functor<T>::difference_type difference_type;
        typedef typename vector_scalar_unary_functor<T>::value_type value_type;

        template<class E>
        NUMERICS_INLINE
        value_type operator () (const vector_expression<E> &e) const { 
            value_type t (0);
            size_type size (e ().size ());
            for (size_type i = 0; i < size; ++ i)
                t += e () (i);
            return t; 
        }
        template<class I>
        NUMERICS_INLINE
        value_type operator () (I it, const I &it_end, std::random_access_iterator_tag) const { 
            value_type t (0);
            difference_type size (it_end - it);
//            for (difference_type i = 0; i < size; ++ i)
            while (-- size >= 0)
                t += *it, ++ it;
            return t; 
        }
        template<class I>
        NUMERICS_INLINE
        value_type operator () (I it, const I &it_end, std::bidirectional_iterator_tag) const { 
            value_type t (0);
            while (it != it_end) 
                t += *it, ++ it;
            return t; 
        }
    };

    // Unary returning scalar norm
    template<class T>
    struct vector_scalar_norm_unary_functor {
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef type_traits<T>::norm_type value_type;
    };

    template<class T>
    struct vector_norm_1: 
        public vector_scalar_norm_unary_functor<T> {
        typedef typename vector_scalar_norm_unary_functor<T>::size_type size_type;
        typedef typename vector_scalar_norm_unary_functor<T>::difference_type difference_type;
        typedef typename vector_scalar_norm_unary_functor<T>::value_type value_type;

        template<class E>
        NUMERICS_INLINE
        value_type operator () (const vector_expression<E> &e) const { 
            value_type t (0);
            size_type size (e ().size ());
            for (size_type i = 0; i < size; ++ i) {
                value_type u (detail::abs (e () (i)));
                t += u;
            }
            return t; 
        }
        template<class I>
        NUMERICS_INLINE
        value_type operator () (I it, const I &it_end, std::random_access_iterator_tag) const { 
            value_type t (0);
            difference_type size (it_end - it);
//            for (difference_type i = 0; i < size; ++ i) {
            while (-- size >= 0) {
                value_type u (detail::abs (*it));
                t += u;
                ++ it;
            }
            return t; 
        }
        template<class I>
        NUMERICS_INLINE
        value_type operator () (I it, const I &it_end, std::bidirectional_iterator_tag) const { 
            value_type t (0);
            while (it != it_end) {
                value_type u (detail::abs (*it));
                t += u;
                ++ it;
            }
            return t; 
        }
    };
    template<class T>
    struct vector_norm_2: 
        public vector_scalar_norm_unary_functor<T> {
        typedef typename vector_scalar_norm_unary_functor<T>::size_type size_type;
        typedef typename vector_scalar_norm_unary_functor<T>::difference_type difference_type;
        typedef typename vector_scalar_norm_unary_functor<T>::value_type value_type;

        template<class E>
        NUMERICS_INLINE
        value_type operator () (const vector_expression<E> &e) const { 
            value_type t (0);
            size_type size (e ().size ());
            for (size_type i = 0; i < size; ++ i) {
                value_type u (detail::abs (e () (i)));
                t +=  u * u;
            }
            return detail::sqrt (t); 
        }
        template<class I>
        NUMERICS_INLINE
        value_type operator () (I it, const I &it_end, std::random_access_iterator_tag) const { 
            value_type t (0);
            difference_type size (it_end - it);
//            for (difference_type i = 0; i < size; ++ i) {
            while (-- size >= 0) {
                value_type u (detail::abs (*it));
                t +=  u * u;
                ++ it;
            }
            return detail::sqrt (t); 
        }
        template<class I>
        NUMERICS_INLINE
        value_type operator () (I it, const I &it_end, std::bidirectional_iterator_tag) const { 
            value_type t (0);
            while (it != it_end) {
                value_type u (detail::abs (*it));
                t +=  u * u;
                ++ it;
            }
            return detail::sqrt (t); 
        }
    };
    template<class T>
    struct vector_norm_inf: 
        public vector_scalar_norm_unary_functor<T> {
        typedef typename vector_scalar_norm_unary_functor<T>::size_type size_type;
        typedef typename vector_scalar_norm_unary_functor<T>::difference_type difference_type;
        typedef typename vector_scalar_norm_unary_functor<T>::value_type value_type;

        template<class E>
        NUMERICS_INLINE
        value_type operator () (const vector_expression<E> &e) const { 
            value_type t (0);
            size_type size (e ().size ());
            for (size_type i = 0; i < size; ++ i) {
                value_type u (detail::abs (e () (i)));
                if (u > t) 
                    t = u;
            }
            return t; 
        }
        template<class I>
        NUMERICS_INLINE
        value_type operator () (I it, const I &it_end, std::random_access_iterator_tag) const { 
            value_type t (0);
            difference_type size (it_end - it);
//            for (difference_type i = 0; i < size; ++ i) {
            while (-- size >= 0) {
                value_type u (detail::abs (*it));
                if (u > t) 
                    t = u;
                ++ it;
            }
            return t; 
        }
        template<class I>
        NUMERICS_INLINE
        value_type operator () (I it, const I &it_end, std::bidirectional_iterator_tag) const { 
            value_type t (0);
            while (it != it_end) {
                value_type u (detail::abs (*it));
                if (u > t) 
                    t = u;
                ++ it;
            }
            return t; 
        }
    };

    template<class T>
    struct vector_index_norm_inf: 
        public vector_scalar_norm_unary_functor<T> {
        typedef typename vector_scalar_norm_unary_functor<T>::size_type size_type;
        typedef typename vector_scalar_norm_unary_functor<T>::difference_type difference_type;
        typedef typename vector_scalar_norm_unary_functor<T>::value_type value_type;

        template<class E>
        NUMERICS_INLINE
        size_type operator () (const vector_expression<E> &e) const { 
            value_type t (0);
            size_type i_norm_inf (0);
            size_type size (e ().size ());
            for (size_type i = 0; i < size; ++ i) {
                value_type u (detail::abs (e () (i)));
                if (u > t) {
                    i_norm_inf = i;
                    t = u;
                }
            }
            return i_norm_inf; 
        }
        template<class I>
        NUMERICS_INLINE
        size_type operator () (I it, const I &it_end, std::random_access_iterator_tag) const { 
            value_type t (0);
            size_type i_norm_inf (0);
            difference_type size (it_end - it);
//            for (difference_type i = 0; i < size; ++ i) {
            while (-- size >= 0) {
                value_type u (detail::abs (*it));
                if (u > t) {
//                    i_norm_inf = i;
                    i_norm_inf = it.index ();
                    t = u;
                }
                ++ it;
            }
            return i_norm_inf; 
        }
        template<class I>
        NUMERICS_INLINE
        size_type operator () (I it, const I &it_end, std::bidirectional_iterator_tag) const { 
            value_type t (0);
            size_type i_norm_inf (0);
            while (it != it_end) {
                value_type u (detail::abs (*it));
                if (u > t) {
                    i_norm_inf = it.index ();
                    t = u;
                }
                ++ it;
            }
            return i_norm_inf; 
        }
    };

    // Binary returning scalar
    template<class T1, class T2, class TR>
    struct vector_scalar_binary_functor {
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef TR value_type;
    };

    template<class T1, class T2, class TR>
    struct vector_inner_prod: 
        public vector_scalar_binary_functor<T1, T2, TR> {
        typedef typename vector_scalar_binary_functor<T1, T2, TR>::size_type size_type ;
        typedef typename vector_scalar_binary_functor<T1, T2, TR>::difference_type difference_type;
        typedef typename vector_scalar_binary_functor<T1, T2, TR>::value_type value_type;

        template<class E1, class E2>
        NUMERICS_INLINE
        value_type operator () (const vector_expression<E1> &e1, 
                                const vector_expression<E2> &e2) const { 
            value_type t (0);
            size_type size (common (e1 ().size (), e2 ().size ()));
            for (size_type i = 0; i < size; ++ i)
                t += e1 () (i) * e2 () (i);
            return t; 
        }
        template<class I1, class I2>
        NUMERICS_INLINE
        value_type operator () (I1 it1, const I1 &it1_end, I2 it2, const I2 &it2_end, std::random_access_iterator_tag) const { 
            value_type t (0);
            difference_type size (common (it1_end - it1, it2_end - it2));
//          for (difference_type i = 0; i < size; ++ i)
            while (-- size >= 0)
                t += *it1 * *it2, ++ it1, ++ it2;
            return t; 
        }
        template<class I1, class I2>
        NUMERICS_INLINE
        value_type operator () (I1 it1, const I1 &it1_end, I2 it2, const I2 &it2_end, std::bidirectional_iterator_tag) const { 
            value_type t (0);
            while (it1 != it1_end && it2 != it2_end) {
                difference_type compare (it1.index () - it2.index ());
                if (compare < 0) 
                    ++ it1;
                else if (compare == 0) 
                    t += *it1 * *it2, ++ it1, ++ it2;
                else if (compare > 0)
                    ++ it2;
            }
            return t; 
        }
    };

    // Matrix functors

    // Binary returning vector
    template<class T1, class T2, class TR>
    struct matrix_vector_binary_functor {
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef TR value_type;
    };

    template<class T1, class T2, class TR>
    struct matrix_vector_prod1: 
        public matrix_vector_binary_functor<T1, T2, TR> {
        typedef typename matrix_vector_binary_functor<T1, T2, TR>::size_type size_type;
        typedef typename matrix_vector_binary_functor<T1, T2, TR>::difference_type difference_type;
        typedef typename matrix_vector_binary_functor<T1, T2, TR>::value_type value_type;

        template<class E1, class E2>
        NUMERICS_INLINE
        value_type operator () (const matrix_expression<E1> &e1, 
                                const vector_expression<E2> &e2, 
                                size_type i) const { 
            size_type size = common (e1 ().size2 (), e2 ().size ());
            value_type t (0);
            for (size_type j = 0; j < size; ++ j)
                t += e1 () (i, j) * e2 () (j);
            return t; 
        }
        template<class I1, class I2>
        NUMERICS_INLINE
        value_type operator () (I1 it1, const I1 &it1_end, I2 it2, const I2 &it2_end, std::random_access_iterator_tag) const { 
            value_type t (0);
            difference_type size (common (it1_end - it1, it2_end - it2));
//          for (difference_type j = 0; j < size; ++ j)
            while (-- size >= 0)
                t += *it1 * *it2, ++ it1, ++ it2;
            return t; 
        }
        template<class I1, class I2>
        NUMERICS_INLINE
        value_type operator () (I1 it1, const I1 &it1_end, I2 it2, const I2 &it2_end, std::bidirectional_iterator_tag) const { 
            value_type t (0);
            while (it1 != it1_end && it2 != it2_end) {
                difference_type compare = it1.index2 () - it2.index ();
                if (compare < 0) 
                    ++ it1;
                else if (compare == 0) 
                    t += *it1 * *it2, ++ it1, ++ it2;
                else if (compare > 0)
                    ++ it2;
            }
            return t; 
        }
    };

    template<class T1, class T2, class TR>
    struct matrix_vector_prod2: 
        public matrix_vector_binary_functor<T1, T2, TR> {
        typedef typename matrix_vector_binary_functor<T1, T2, TR>::size_type size_type;
        typedef typename matrix_vector_binary_functor<T1, T2, TR>::difference_type difference_type;
        typedef typename matrix_vector_binary_functor<T1, T2, TR>::value_type value_type;

        template<class E1, class E2>
        NUMERICS_INLINE
        value_type operator () (const vector_expression<E1> &e1, 
                                const matrix_expression<E2> &e2, 
                                size_type i) const { 
            size_type size = common (e1 ().size (), e2 ().size1 ());
            value_type t (0);
            for (size_type j = 0; j < size; ++ j)
                t += e1 () (i) * e2 () (i, j);
            return t; 
        }
        template<class I1, class I2>
        NUMERICS_INLINE
        value_type operator () (I1 it1, const I1 &it1_end, I2 it2, const I2 &it2_end, std::random_access_iterator_tag) const { 
            value_type t (0);
            difference_type size (common (it1_end - it1, it2_end - it2));
//          for (difference_type j = 0; j < size; ++ j)
            while (-- size >= 0)
                t += *it1 * *it2, ++ it1, ++ it2;
            return t; 
        }
        template<class I1, class I2>
        NUMERICS_INLINE
        value_type operator () (I1 it1, const I1 &it1_end, I2 it2, const I2 &it2_end, std::bidirectional_iterator_tag) const { 
            value_type t (0);
            while (it1 != it1_end && it2 != it2_end) {
                difference_type compare = it1.index2 () - it2.index ();
                if (compare < 0) 
                    ++ it1;
                else if (compare == 0) 
                    t += *it1 * *it2, ++ it1, ++ it2;
                else if (compare > 0)
                    ++ it2;
            }
            return t; 
        }
    };

    // Binary returning matrix
    template<class T1, class T2, class TR>
    struct matrix_matrix_binary_functor {
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef TR value_type;
    };

    template<class T1, class T2, class TR>
    struct matrix_matrix_prod: 
        public matrix_matrix_binary_functor<T1, T2, TR> {
        typedef typename matrix_matrix_binary_functor<T1, T2, TR>::size_type size_type;
        typedef typename matrix_matrix_binary_functor<T1, T2, TR>::difference_type difference_type;
        typedef typename matrix_matrix_binary_functor<T1, T2, TR>::value_type value_type;

        template<class E1, class E2>
        NUMERICS_INLINE
        value_type operator () (const matrix_expression<E1> &e1, 
                                const matrix_expression<E2> &e2, 
                                size_type i, size_type j) const { 
            size_type size = common (e1 ().size2 (), e2 ().size1 ());
            value_type t (0);
            for (size_type k = 0; k < size; ++ k) 
                t += e1 () (i, k) * e2 () (k, j);
            return t; 
        }
        template<class I1, class I2>
        NUMERICS_INLINE
        value_type operator () (I1 it1, const I1 &it1_end, I2 it2, const I2 &it2_end, std::random_access_iterator_tag) const { 
            value_type t (0);
            difference_type size = common (it1_end - it1, it2_end - it2);
//          for (difference_type k = 0; k < size; ++ k) 
            while (-- size >= 0) 
                t += *it1 * *it2, ++ it1, ++ it2;
            return t; 
        }
        template<class I1, class I2>
        NUMERICS_INLINE
        value_type operator () (I1 it1, const I1 &it1_end, I2 it2, const I2 &it2_end, std::bidirectional_iterator_tag) const { 
            value_type t (0);
            while (it1 != it1_end && it2 != it2_end) {
                difference_type compare = it1.index2 () - it2.index1 ();
                if (compare < 0) 
                    ++ it1;
                else if (compare == 0) 
                    t += *it1 * *it2, ++ it1, ++ it2;
                else if (compare > 0)
                    ++ it2;
            }
            return t; 
        }
    };

}

#endif 


