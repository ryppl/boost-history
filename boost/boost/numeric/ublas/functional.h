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

#ifndef NUMERICS_FUNCTIONAL_H
#define NUMERICS_FUNCTIONAL_H

#include <functional>

#include <boost/numeric/ublas/config.h>
#include <boost/numeric/ublas/exception.h>
#include <boost/numeric/ublas/traits.h>
#include <boost/numeric/ublas/math.h>
#include <boost/numeric/ublas/duff.h>

namespace boost { namespace numerics {

    // Scalar functors

    // Unary
    template<class T>
    struct scalar_unary_functor {
        typedef T argument_type;
        typedef T result_type;
    };

    template<class T>
    struct scalar_identity: 
        public scalar_unary_functor<T> {
        typedef typename scalar_unary_functor<T>::argument_type argument_type;
        typedef typename scalar_unary_functor<T>::result_type result_type;

        NUMERICS_INLINE
        result_type operator () (const argument_type &t) const { 
            return t; 
        }
    };
    template<class T>
    struct scalar_negate: 
        public scalar_unary_functor<T> {
        typedef typename scalar_unary_functor<T>::argument_type argument_type;
        typedef typename scalar_unary_functor<T>::result_type result_type;

        NUMERICS_INLINE
        result_type operator () (const argument_type &t) const { 
            return - t; 
        }
    };
    template<class T>
    struct scalar_conj: 
        public scalar_unary_functor<T> {
        typedef typename scalar_unary_functor<T>::argument_type argument_type;
        typedef typename scalar_unary_functor<T>::result_type result_type;

        NUMERICS_INLINE
        result_type operator () (const argument_type &t) const { 
            return detail::conj (t); 
        }
    };

    // Unary returning real
    template<class T>
    struct scalar_real_unary_functor {
        typedef T argument_type;
        typedef typename type_traits<T>::real_type result_type;
    };

    template<class T>
    struct scalar_real:
        public scalar_real_unary_functor<T> {
        typedef typename scalar_real_unary_functor<T>::argument_type argument_type;
        typedef typename scalar_real_unary_functor<T>::result_type result_type;

        NUMERICS_INLINE
        result_type operator () (const argument_type &t) const {
            return detail::real (t);
        }
    };
    template<class T>
    struct scalar_imag:
        public scalar_real_unary_functor<T> {
        typedef typename scalar_real_unary_functor<T>::argument_type argument_type;
        typedef typename scalar_real_unary_functor<T>::result_type result_type;

        NUMERICS_INLINE
        result_type operator () (const argument_type &t) const {
            return detail::imag (t);
        }
    };

    // Binary
    template<class T1, class T2>
    struct scalar_binary_functor {
        typedef T1 argument1_type;
        typedef T2 argument2_type;
        typedef typename promote_traits<T1, T2>::promote_type result_type;
    };

    template<class T1, class T2>
    struct scalar_plus:
        public scalar_binary_functor<T1, T2> {
        typedef typename scalar_binary_functor<T1, T2>::argument1_type argument1_type;
        typedef typename scalar_binary_functor<T1, T2>::argument2_type argument2_type;
        typedef typename scalar_binary_functor<T1, T2>::result_type result_type;

        NUMERICS_INLINE
        result_type operator () (const argument1_type &t1, const argument2_type &t2) const {
            return t1 + t2;
        }
    };
    template<class T1, class T2>
    struct scalar_minus: 
        public scalar_binary_functor<T1, T2> { 
        typedef typename scalar_binary_functor<T1, T2>::argument1_type argument1_type;
        typedef typename scalar_binary_functor<T1, T2>::argument2_type argument2_type;
        typedef typename scalar_binary_functor<T1, T2>::result_type result_type;

        NUMERICS_INLINE
        result_type operator () (const argument1_type &t1, const argument2_type &t2) const {
            return t1 - t2; 
        }
    };
    template<class T1, class T2>
    struct scalar_multiplies: 
        public scalar_binary_functor<T1, T2> { 
        typedef typename scalar_binary_functor<T1, T2>::argument1_type argument1_type;
        typedef typename scalar_binary_functor<T1, T2>::argument2_type argument2_type;
        typedef typename scalar_binary_functor<T1, T2>::result_type result_type;

        NUMERICS_INLINE
        result_type operator () (const argument1_type &t1, const argument2_type &t2) const { 
            return t1 * t2; 
        }
    };
    template<class T1, class T2>
    struct scalar_divides:
        public scalar_binary_functor<T1, T2> {
        typedef typename scalar_binary_functor<T1, T2>::argument1_type argument1_type;
        typedef typename scalar_binary_functor<T1, T2>::argument2_type argument2_type;
        typedef typename scalar_binary_functor<T1, T2>::result_type result_type;

        NUMERICS_INLINE
        result_type operator () (const argument1_type &t1, const argument2_type &t2) const {
            return t1 / t2;
        }
    };

    struct assign_tag {};
    struct computed_assign_tag {};

    template<class T1, class T2>
    struct scalar_assign:
        public scalar_binary_functor<T1, T2> {
        typedef typename scalar_binary_functor<T1, T2>::argument1_type argument1_type;
        typedef typename scalar_binary_functor<T1, T2>::argument2_type argument2_type;
        typedef assign_tag assign_category;

        NUMERICS_INLINE
        void operator () (argument1_type &t1, const argument2_type &t2) const {
            t1 = t2;
        }
    };
    template<class T1, class T2>
    struct scalar_plus_assign:
        public scalar_binary_functor<T1, T2> {
        typedef typename scalar_binary_functor<T1, T2>::argument1_type argument1_type;
        typedef typename scalar_binary_functor<T1, T2>::argument2_type argument2_type;
        typedef computed_assign_tag assign_category;

        NUMERICS_INLINE
        void operator () (argument1_type &t1, const argument2_type &t2) const {
            t1 += t2;
        }
    };
    template<class T1, class T2>
    struct scalar_minus_assign:
        public scalar_binary_functor<T1, T2> {
        typedef typename scalar_binary_functor<T1, T2>::argument1_type argument1_type;
        typedef typename scalar_binary_functor<T1, T2>::argument2_type argument2_type;
        typedef computed_assign_tag assign_category;

        NUMERICS_INLINE
        void operator () (argument1_type &t1, const argument2_type &t2) const {
            t1 -= t2;
        }
    };
    template<class T1, class T2>
    struct scalar_multiplies_assign:
        public scalar_binary_functor<T1, T2> {
        typedef typename scalar_binary_functor<T1, T2>::argument1_type argument1_type;
        typedef typename scalar_binary_functor<T1, T2>::argument2_type argument2_type;
        typedef computed_assign_tag assign_category;

        NUMERICS_INLINE
        void operator () (argument1_type &t1, const argument2_type &t2) const {
            t1 *= t2;
        }
    };
    template<class T1, class T2>
    struct scalar_divides_assign:
        public scalar_binary_functor<T1, T2> {
        typedef typename scalar_binary_functor<T1, T2>::argument1_type argument1_type;
        typedef typename scalar_binary_functor<T1, T2>::argument2_type argument2_type;
        typedef computed_assign_tag assign_category;

        NUMERICS_INLINE
        void operator () (argument1_type &t1, const argument2_type &t2) const {
            t1 /= t2;
        }
    };

    template<class T1, class T2>
    struct scalar_swap:
        public scalar_binary_functor<T1, T2> {
        typedef typename scalar_binary_functor<T1, T2>::argument1_type argument1_type;
        typedef typename scalar_binary_functor<T1, T2>::argument2_type argument2_type;

        NUMERICS_INLINE
        void operator () (argument1_type &t1, argument2_type &t2) const {
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
        typedef T result_type;
    };

    template<class T>
    struct vector_sum: 
        public vector_scalar_unary_functor<T> {
        typedef typename vector_scalar_unary_functor<T>::size_type size_type;
        typedef typename vector_scalar_unary_functor<T>::difference_type difference_type;
        typedef typename vector_scalar_unary_functor<T>::value_type value_type;
        typedef typename vector_scalar_unary_functor<T>::result_type result_type;

        template<class E>
        NUMERICS_INLINE
        result_type operator () (const vector_expression<E> &e) const { 
            result_type t (0);
            size_type size (e ().size ());
            for (size_type i = 0; i < size; ++ i)
                t += e () (i);
            return t; 
        }
        // Dense case
        template<class I>
        NUMERICS_INLINE
        result_type operator () (difference_type size, I it) const { 
            result_type t (0);
            while (-- size >= 0)
                t += *it, ++ it;
            return t; 
        }
        // Sparse case
        template<class I>
        NUMERICS_INLINE
        result_type operator () (I it, const I &it_end) const { 
            result_type t (0);
            while (it != it_end) 
                t += *it, ++ it;
            return t; 
        }
    };

    // Unary returning real scalar 
    template<class T>
    struct vector_scalar_real_unary_functor {
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef typename type_traits<T>::real_type real_type;
        typedef real_type result_type;
    };

    template<class T>
    struct vector_norm_1: 
        public vector_scalar_real_unary_functor<T> {
        typedef typename vector_scalar_real_unary_functor<T>::size_type size_type;
        typedef typename vector_scalar_real_unary_functor<T>::difference_type difference_type;
        typedef typename vector_scalar_real_unary_functor<T>::value_type value_type;
        typedef typename vector_scalar_real_unary_functor<T>::real_type real_type;
        typedef typename vector_scalar_real_unary_functor<T>::result_type result_type;

        template<class E>
        NUMERICS_INLINE
        result_type operator () (const vector_expression<E> &e) const { 
            real_type t (0);
            size_type size (e ().size ());
            for (size_type i = 0; i < size; ++ i) {
                real_type u (detail::norm_1 (e () (i)));
                t += u;
            }
            return t; 
        }
        // Dense case
        template<class I>
        NUMERICS_INLINE
        result_type operator () (difference_type size, I it) const { 
            real_type t (0);
            while (-- size >= 0) {
                real_type u (detail::norm_1 (*it));
                t += u;
                ++ it;
            }
            return t; 
        }
        // Sparse case
        template<class I>
        NUMERICS_INLINE
        result_type operator () (I it, const I &it_end) const { 
            real_type t (0);
            while (it != it_end) {
                real_type u (detail::norm_1 (*it));
                t += u;
                ++ it;
            }
            return t;
        }
    };
    template<class T>
    struct vector_norm_2:
        public vector_scalar_real_unary_functor<T> {
        typedef typename vector_scalar_real_unary_functor<T>::size_type size_type;
        typedef typename vector_scalar_real_unary_functor<T>::difference_type difference_type;
        typedef typename vector_scalar_real_unary_functor<T>::value_type value_type;
        typedef typename vector_scalar_real_unary_functor<T>::real_type real_type;
        typedef typename vector_scalar_real_unary_functor<T>::result_type result_type;

        template<class E>
        NUMERICS_INLINE
        result_type operator () (const vector_expression<E> &e) const {
#ifndef NUMERICS_SCALED_NORM
            real_type t (0);
            size_type size (e ().size ());
            for (size_type i = 0; i < size; ++ i) {
                real_type u (detail::norm_2 (e () (i)));
                t +=  u * u;
            }
            return detail::sqrt (t);
#else
            real_type scale (0);
            real_type sum_squares (1);
            size_type size (e ().size ());
            for (size_type i = 0; i < size; ++ i) {
                real_type u (detail::norm_2 (e () (i)));
                if (scale < u) {
                    real_type v (scale / u);
                    sum_squares = sum_squares * v * v + real_type (1);
                    scale = u;
                } else {
                    real_type v (u / scale);
                    sum_squares += v * v;
                }
            }
            return scale * detail::sqrt (sum_squares);
#endif
        }
        // Dense case
        template<class I>
        NUMERICS_INLINE
        result_type operator () (difference_type size, I it) const {
#ifndef NUMERICS_SCALED_NORM
            real_type t (0);
            while (-- size >= 0) {
                real_type u (detail::norm_2 (*it));
                t +=  u * u;
                ++ it;
            }
            return detail::sqrt (t);
#else
            real_type scale (0);
            real_type sum_squares (1);
            while (-- size >= 0) {
                real_type u (detail::norm_2 (*it));
                if (scale < u) {
                    real_type v (scale / u);
                    sum_squares = sum_squares * v * v + real_type (1);
                    scale = u;
                } else {
                    real_type v (u / scale);
                    sum_squares += v * v;
                }
                ++ it;
            }
            return scale * detail::sqrt (sum_squares);
#endif
        }
        // Sparse case
        template<class I>
        NUMERICS_INLINE
        result_type operator () (I it, const I &it_end) const {
#ifndef NUMERICS_SCALED_NORM
            real_type t (0);
            while (it != it_end) {
                real_type u (detail::norm_2 (*it));
                t +=  u * u;
                ++ it;
            }
            return detail::sqrt (t);
#else
            real_type scale (0);
            real_type sum_squares (1);
            while (it != it_end) {
                real_type u (detail::norm_2 (*it));
                if (scale < u) {
                    real_type v (scale / u);
                    sum_squares = sum_squares * v * v + real_type (1);
                    scale = u;
                } else {
                    real_type v (u / scale);
                    sum_squares += v * v;
                }
                ++ it;
            }
            return scale * detail::sqrt (sum_squares);
#endif
        }
    };
    template<class T>
    struct vector_norm_inf:
        public vector_scalar_real_unary_functor<T> {
        typedef typename vector_scalar_real_unary_functor<T>::size_type size_type;
        typedef typename vector_scalar_real_unary_functor<T>::difference_type difference_type;
        typedef typename vector_scalar_real_unary_functor<T>::value_type value_type;
        typedef typename vector_scalar_real_unary_functor<T>::real_type real_type;
        typedef typename vector_scalar_real_unary_functor<T>::result_type result_type;

        template<class E>
        NUMERICS_INLINE
        result_type operator () (const vector_expression<E> &e) const { 
            real_type t (0);
            size_type size (e ().size ());
            for (size_type i = 0; i < size; ++ i) {
                real_type u (detail::norm_inf (e () (i)));
                if (u > t) 
                    t = u;
            }
            return t; 
        }
        // Dense case
        template<class I>
        NUMERICS_INLINE
        result_type operator () (difference_type size, I it) const { 
            real_type t (0);
            while (-- size >= 0) {
                real_type u (detail::norm_inf (*it));
                if (u > t) 
                    t = u;
                ++ it;
            }
            return t; 
        }
        // Sparse case
        template<class I>
        NUMERICS_INLINE
        result_type operator () (I it, const I &it_end) const { 
            real_type t (0);
            while (it != it_end) {
                real_type u (detail::norm_inf (*it));
                if (u > t) 
                    t = u;
                ++ it;
            }
            return t; 
        }
    };

    // Unary returning index
    template<class T>
    struct vector_scalar_index_unary_functor {
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef typename type_traits<T>::real_type real_type;
        typedef difference_type result_type;
    };

    template<class T>
    struct vector_index_norm_inf:
        public vector_scalar_index_unary_functor<T> {
        typedef typename vector_scalar_index_unary_functor<T>::size_type size_type;
        typedef typename vector_scalar_index_unary_functor<T>::difference_type difference_type;
        typedef typename vector_scalar_index_unary_functor<T>::value_type value_type;
        typedef typename vector_scalar_index_unary_functor<T>::real_type real_type;
        typedef typename vector_scalar_index_unary_functor<T>::result_type result_type;

        template<class E>
        NUMERICS_INLINE
        result_type operator () (const vector_expression<E> &e) const {
            result_type i_norm_inf (-1);
            real_type t (-1);
            size_type size (e ().size ());
            for (size_type i = 0; i < size; ++ i) {
                real_type u (detail::norm_inf (e () (i)));
                if (u > t) {
                    i_norm_inf = i;
                    t = u;
                }
            }
            return i_norm_inf;
        }
        // Dense case
        template<class I>
        NUMERICS_INLINE
        result_type operator () (difference_type size, I it) const {
            result_type i_norm_inf (-1);
            real_type t (-1);
            while (-- size >= 0) {
                real_type u (detail::norm_inf (*it));
                if (u > t) {
                    i_norm_inf = it.index ();
                    t = u;
                }
                ++ it;
            }
            return i_norm_inf;
        }
        // Sparse case
        template<class I>
        NUMERICS_INLINE
        result_type operator () (I it, const I &it_end) const {
            result_type i_norm_inf (-1);
            real_type t (-1);
            while (it != it_end) {
                real_type u (detail::norm_inf (*it));
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
        typedef TR result_type;
    };

    template<class T1, class T2, class TR>
    struct vector_inner_prod: 
        public vector_scalar_binary_functor<T1, T2, TR> {
        typedef typename vector_scalar_binary_functor<T1, T2, TR>::size_type size_type ;
        typedef typename vector_scalar_binary_functor<T1, T2, TR>::difference_type difference_type;
        typedef typename vector_scalar_binary_functor<T1, T2, TR>::value_type value_type;
        typedef typename vector_scalar_binary_functor<T1, T2, TR>::result_type result_type;

        template<class E1, class E2>
        NUMERICS_INLINE
        result_type operator () (const vector_expression<E1> &e1, 
                                 const vector_expression<E2> &e2) const { 
            result_type t (0);
            size_type size (common (e1 ().size (), e2 ().size ()));
#ifndef NUMERICS_USE_DUFF_DEVICE
            for (size_type i = 0; i < size; ++ i)
                t += e1 () (i) * e2 () (i);
#else
            size_type i (0);
            DD (size, 4, r, (t += e1 () (i) * e2 () (i), ++ i));
#endif
            return t; 
        }
        // Dense case
        template<class I1, class I2>
        NUMERICS_INLINE
        result_type operator () (difference_type size, I1 it1, I2 it2) const { 
            result_type t (0);
#ifndef NUMERICS_USE_DUFF_DEVICE
            while (-- size >= 0)
                t += *it1 * *it2, ++ it1, ++ it2;
#else
            DD (size, 4, r, (t += *it1 * *it2, ++ it1, ++ it2));
#endif
            return t; 
        }
        // Packed case
        template<class I1, class I2>
        NUMERICS_INLINE
        result_type operator () (I1 it1, const I1 &it1_end, I2 it2, const I2 &it2_end) const {
            result_type t (0);
            if (it1 != it1_end && it1.index () < it2.index ())
                it1 += std::min (it2.index () - it1.index (), size_type (it1_end - it1));
            if (it2 != it2_end && it2.index () < it1.index ())
                it2 += std::min (it1.index () - it2.index (), size_type (it2_end - it2));
            difference_type size (std::min (size_type (it1_end - it1), size_type (it2_end - it2)));
            while (-- size >= 0)
                t += *it1 * *it2, ++ it1, ++ it2;
            return t;
        }
        // Sparse case
        template<class I1, class I2>
        NUMERICS_INLINE
        result_type operator () (I1 it1, const I1 &it1_end, I2 it2, const I2 &it2_end, sparse_bidirectional_iterator_tag) const { 
            result_type t (0);
            while (it1 != it1_end && it2 != it2_end) {
                difference_type compare = it1.index () - it2.index ();
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
        typedef TR result_type;
    };

    template<class T1, class T2, class TR>
    struct matrix_vector_prod1:
        public matrix_vector_binary_functor<T1, T2, TR> {
        typedef typename matrix_vector_binary_functor<T1, T2, TR>::size_type size_type;
        typedef typename matrix_vector_binary_functor<T1, T2, TR>::difference_type difference_type;
        typedef typename matrix_vector_binary_functor<T1, T2, TR>::value_type value_type;
        typedef typename matrix_vector_binary_functor<T1, T2, TR>::result_type result_type;

        template<class E1, class E2>
        NUMERICS_INLINE
        result_type operator () (const matrix_expression<E1> &e1,
                                 const vector_expression<E2> &e2,
                                 size_type i) const {
            size_type size = common (e1 ().size2 (), e2 ().size ());
            result_type t (0);
#ifndef NUMERICS_USE_DUFF_DEVICE
            for (size_type j = 0; j < size; ++ j)
                t += e1 () (i, j) * e2 () (j);
#else
            size_type j (0);
            DD (size, 4, r, (t += e1 () (i, j) * e2 () (j), ++ j));
#endif
            return t;
        }
        // Dense case
        template<class I1, class I2>
        NUMERICS_INLINE
        result_type operator () (difference_type size, I1 it1, I2 it2) const {
            result_type t (0);
#ifndef NUMERICS_USE_DUFF_DEVICE
            while (-- size >= 0)
                t += *it1 * *it2, ++ it1, ++ it2;
#else
            DD (size, 4, r, (t += *it1 * *it2, ++ it1, ++ it2));
#endif
            return t;
        }
        // Packed case
        template<class I1, class I2>
        NUMERICS_INLINE
        result_type operator () (I1 it1, const I1 &it1_end, I2 it2, const I2 &it2_end) const {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            result_type t (0);
            if (it1 != it1_end && it1.index () < it2.index ())
                it1 += std::min (it2.index () - it1.index (), size_type (it1_end - it1));
            if (it2 != it2_end && it2.index () < it1.index ())
                it2 += std::min (it1.index () - it2.index (), size_type (it2_end - it2));
            difference_type size (std::min (size_type (it1_end - it1), size_type (it2_end - it2)));
            while (-- size >= 0)
                t += *it1 * *it2, ++ it1, ++ it2;
            return t;
#else
            result_type t (0);
            if (it1 != it1_end && it1.index2 () < it2.index ())
                it1 += std::min (it2.index () - it1.index2 (), size_type (it1_end - it1));
            if (it2 != it2_end && it2.index () < it1.index2 ())
                it2 += std::min (it1.index2 () - it2.index (), size_type (it2_end - it2));
            difference_type size (std::min (size_type (it1_end - it1), size_type (it2_end - it2)));
            while (-- size >= 0)
                t += *it1 * *it2, ++ it1, ++ it2;
            return t;
#endif
        }
        // Sparse case
        template<class I1, class I2>
        NUMERICS_INLINE
        result_type operator () (I1 it1, const I1 &it1_end, I2 it2, const I2 &it2_end, sparse_bidirectional_iterator_tag) const {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            result_type t (0);
            while (it1 != it1_end && it2 != it2_end) {
                difference_type compare = it1.index () - it2.index ();
                if (compare < 0)
                    ++ it1;
                else if (compare == 0)
                    t += *it1 * *it2, ++ it1, ++ it2;
                else if (compare > 0)
                    ++ it2;
            }
            return t;
#else
            result_type t (0);
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
#endif
        }
    };

    template<class T1, class T2, class TR>
    struct matrix_vector_prod2: 
        public matrix_vector_binary_functor<T1, T2, TR> {
        typedef typename matrix_vector_binary_functor<T1, T2, TR>::size_type size_type;
        typedef typename matrix_vector_binary_functor<T1, T2, TR>::difference_type difference_type;
        typedef typename matrix_vector_binary_functor<T1, T2, TR>::value_type value_type;
        typedef typename matrix_vector_binary_functor<T1, T2, TR>::result_type result_type;

        template<class E1, class E2>
        NUMERICS_INLINE
        result_type operator () (const vector_expression<E1> &e1, 
                                 const matrix_expression<E2> &e2, 
                                 size_type i) const { 
            size_type size = common (e1 ().size (), e2 ().size1 ());
            result_type t (0);
#ifndef NUMERICS_USE_DUFF_DEVICE
            for (size_type j = 0; j < size; ++ j)
                t += e1 () (j) * e2 () (j, i);
#else
            size_type j (0);
            DD (size, 4, r, (t += e1 () (j) * e2 () (j, i), ++ j));
#endif
            return t; 
        }
        // Dense case
        template<class I1, class I2>
        NUMERICS_INLINE
        result_type operator () (difference_type size, I1 it1, I2 it2) const { 
            result_type t (0);
#ifndef NUMERICS_USE_DUFF_DEVICE
            while (-- size >= 0)
                t += *it1 * *it2, ++ it1, ++ it2;
#else
            DD (size, 4, r, (t += *it1 * *it2, ++ it1, ++ it2));
#endif
            return t; 
        }
        // Packed case
        template<class I1, class I2>
        NUMERICS_INLINE
        result_type operator () (I1 it1, const I1 &it1_end, I2 it2, const I2 &it2_end) const {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            result_type t (0);
            if (it1 != it1_end && it1.index () < it2.index ())
                it1 += std::min (it2.index () - it1.index (), size_type (it1_end - it1));
            if (it2 != it2_end && it2.index () < it1.index ())
                it2 += std::min (it1.index () - it2.index (), size_type (it2_end - it2));
            difference_type size (std::min (size_type (it1_end - it1), size_type (it2_end - it2)));
            while (-- size >= 0)
                t += *it1 * *it2, ++ it1, ++ it2;
            return t;
#else
            result_type t (0);
            if (it1 != it1_end && it1.index () < it2.index1 ())
                it1 += std::min (it2.index1 () - it1.index (), size_type (it1_end - it1));
            if (it2 != it2_end && it2.index1 () < it1.index ())
                it2 += std::min (it1.index () - it2.index1 (), size_type (it2_end - it2));
            difference_type size (std::min (size_type (it1_end - it1), size_type (it2_end - it2)));
            while (-- size >= 0)
                t += *it1 * *it2, ++ it1, ++ it2;
            return t; 
#endif
        }
        // Sparse case
        template<class I1, class I2>
        NUMERICS_INLINE
        result_type operator () (I1 it1, const I1 &it1_end, I2 it2, const I2 &it2_end, sparse_bidirectional_iterator_tag) const { 
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            result_type t (0);
            while (it1 != it1_end && it2 != it2_end) {
                difference_type compare = it1.index () - it2.index ();
                if (compare < 0) 
                    ++ it1;
                else if (compare == 0) 
                    t += *it1 * *it2, ++ it1, ++ it2;
                else if (compare > 0)
                    ++ it2;
            }
            return t; 
#else
            result_type t (0);
            while (it1 != it1_end && it2 != it2_end) {
                difference_type compare = it1.index () - it2.index1 ();
                if (compare < 0) 
                    ++ it1;
                else if (compare == 0) 
                    t += *it1 * *it2, ++ it1, ++ it2;
                else if (compare > 0)
                    ++ it2;
            }
            return t; 
#endif
        }
    };

    // Binary returning matrix
    template<class T1, class T2, class TR>
    struct matrix_matrix_binary_functor {
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef TR value_type;
        typedef TR result_type;
    };

    template<class T1, class T2, class TR>
    struct matrix_matrix_prod: 
        public matrix_matrix_binary_functor<T1, T2, TR> {
        typedef typename matrix_matrix_binary_functor<T1, T2, TR>::size_type size_type;
        typedef typename matrix_matrix_binary_functor<T1, T2, TR>::difference_type difference_type;
        typedef typename matrix_matrix_binary_functor<T1, T2, TR>::value_type value_type;
        typedef typename matrix_matrix_binary_functor<T1, T2, TR>::result_type result_type;

        template<class E1, class E2>
        NUMERICS_INLINE
        result_type operator () (const matrix_expression<E1> &e1, 
                                 const matrix_expression<E2> &e2, 
                                 size_type i, size_type j) const { 
            size_type size = common (e1 ().size2 (), e2 ().size1 ());
            result_type t (0);
#ifndef NUMERICS_USE_DUFF_DEVICE
            for (size_type k = 0; k < size; ++ k) 
                t += e1 () (i, k) * e2 () (k, j);
#else
            size_type k (0);
            DD (size, 4, r, (t += e1 () (i, k) * e2 () (k, j), ++ k));
#endif
            return t; 
        }
        // Dense case
        template<class I1, class I2>
        NUMERICS_INLINE
        result_type operator () (difference_type size, I1 it1, I2 it2) const { 
            result_type t (0);
#ifndef NUMERICS_USE_DUFF_DEVICE
            while (-- size >= 0)
                t += *it1 * *it2, ++ it1, ++ it2;
#else
            DD (size, 4, r, (t += *it1 * *it2, ++ it1, ++ it2));
#endif
            return t; 
        }
        // Packed case
        template<class I1, class I2>
        NUMERICS_INLINE
        result_type operator () (I1 it1, const I1 &it1_end, I2 it2, const I2 &it2_end, packed_random_access_iterator_tag) const {
            result_type t (0);
            if (it1 != it1_end && it1.index2 () < it2.index1 ())
                it1 += std::min (it2.index1 () - it1.index2 (), size_type (it1_end - it1));
            if (it2 != it2_end && it2.index1 () < it1.index2 ())
                it2 += std::min (it1.index2 () - it2.index1 (), size_type (it2_end - it2));
            difference_type size (std::min (size_type (it1_end - it1), size_type (it2_end - it2)));
            while (-- size >= 0)
                t += *it1 * *it2, ++ it1, ++ it2;
            return t; 
        }
        // Sparse case
        template<class I1, class I2>
        NUMERICS_INLINE
        result_type operator () (I1 it1, const I1 &it1_end, I2 it2, const I2 &it2_end, sparse_bidirectional_iterator_tag) const { 
            result_type t (0);
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

    // Unary returning scalar norm
    template<class T>
    struct matrix_scalar_real_unary_functor {
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef typename type_traits<T>::real_type real_type;
        typedef real_type result_type;
    };

    template<class T>
    struct matrix_norm_1: 
        public matrix_scalar_real_unary_functor<T> {
        typedef typename matrix_scalar_real_unary_functor<T>::size_type size_type;
        typedef typename matrix_scalar_real_unary_functor<T>::difference_type difference_type;
        typedef typename matrix_scalar_real_unary_functor<T>::value_type value_type;
        typedef typename matrix_scalar_real_unary_functor<T>::real_type real_type;
        typedef typename matrix_scalar_real_unary_functor<T>::result_type result_type;

        template<class E>
        NUMERICS_INLINE
        result_type operator () (const matrix_expression<E> &e) const { 
            real_type t (0);
            size_type size2 (e ().size2 ());
            for (size_type j = 0; j < size2; ++ j) {
                real_type u (0);
                size_type size1 (e ().size1 ());
                for (size_type i = 0; i < size1; ++ i) {
                    real_type v (detail::norm_1 (e () (i, j)));
                    u += v;
                }
                if (u > t) 
                    t = u;  
            }
            return t; 
        }
    };
    template<class T>
    struct matrix_norm_2: 
        public matrix_scalar_real_unary_functor<T> {
        typedef typename matrix_scalar_real_unary_functor<T>::size_type size_type;
        typedef typename matrix_scalar_real_unary_functor<T>::difference_type difference_type;
        typedef typename matrix_scalar_real_unary_functor<T>::value_type value_type;
        typedef typename matrix_scalar_real_unary_functor<T>::real_type real_type;
        typedef typename matrix_scalar_real_unary_functor<T>::result_type result_type;

        template<class E>
        NUMERICS_INLINE
        result_type operator () (const matrix_expression<E> &e) const { 
            real_type t (0);
            size_type size1 (e ().size1 ());
            for (size_type i = 0; i < size1; ++ i) {
                size_type size2 (e ().size2 ());
                for (size_type j = 0; j < size2; ++ j) {
                    real_type u (detail::norm_2 (e () (i, j)));
                    t +=  u * u;
                }
            }
            return detail::sqrt (t); 
        }
    };
    template<class T>
    struct matrix_norm_inf: 
        public matrix_scalar_real_unary_functor<T> {
        typedef typename matrix_scalar_real_unary_functor<T>::size_type size_type;
        typedef typename matrix_scalar_real_unary_functor<T>::difference_type difference_type;
        typedef typename matrix_scalar_real_unary_functor<T>::value_type value_type;
        typedef typename matrix_scalar_real_unary_functor<T>::real_type real_type;
        typedef typename matrix_scalar_real_unary_functor<T>::result_type result_type;

        template<class E>
        NUMERICS_INLINE
        result_type operator () (const matrix_expression<E> &e) const { 
            real_type t (0);
            size_type size1 (e ().size1 ());
            for (size_type i = 0; i < size1; ++ i) {
                real_type u (0);
                size_type size2 (e ().size2 ());
                for (size_type j = 0; j < size2; ++ j) {
                    real_type v (detail::norm_inf (e () (i, j)));
                    u += v;
                }
                if (u > t) 
                    t = u;  
            }
            return t; 
        }
    };

    // This functor computes the address translation
    // matrix [i] [j] -> storage [i * size2 + j]
    struct row_major {
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef row_major_tag orientation_category;

        // Indexing
        static
        NUMERICS_INLINE
        size_type element (size_type i, size_type size1, size_type j, size_type size2) {
            check (i <= size1, bad_index ());
            check (j <= size2, bad_index ());
            return i * size2 + j;
        }
        static
        NUMERICS_INLINE
        difference_type distance1 (difference_type k, size_type size1, size_type size2) {
            return size2 != 0 ? k / size2 : 0;
        }
        static
        NUMERICS_INLINE
        difference_type distance2 (difference_type k, size_type size1, size_type size2) {
            return k;
        }
        static
        NUMERICS_INLINE
        size_type index1 (difference_type k, size_type size1, size_type size2) {
            return size2 != 0 ? k / size2 : 0;
        }
        static
        NUMERICS_INLINE
        size_type index2 (difference_type k, size_type size1, size_type size2) {
            return size2 != 0 ? k % size2 : 0;
        }
        static
        NUMERICS_INLINE
        bool fast1 () {
            return false;
        }
        static
        NUMERICS_INLINE
        size_type one1 (size_type size1, size_type size2) {
            return size2;
        }
        static
        NUMERICS_INLINE
        bool fast2 () {
            return true;
        }
        static
        NUMERICS_INLINE
        size_type one2 (size_type size1, size_type size2) {
            return 1;
        }

        static
        NUMERICS_INLINE
        size_type lower_element (size_type i, size_type size1, size_type j, size_type size2) {
            check (i <= size1, bad_index ());
            check (j <= size2, bad_index ());
            check (i >= j, bad_index ());
            // sigma_i (i + 1) = (i + 1) * i / 2
            // i = 0 1 2 3, sigma = 0 1 3 6
            return ((i + 1) * i) / 2 + j;
        }
        static
        NUMERICS_INLINE
        size_type upper_element (size_type i, size_type size1, size_type j, size_type size2) {
            check (i <= size1, bad_index ());
            check (j <= size2, bad_index ());
            check (i <= j, bad_index ());
            // sigma_i (size - i) = size * i - i * (i - 1) / 2
            // i = 0 1 2 3, sigma = 0 4 7 9
            return (i * (2 * std::max (size1, size2) - i + 1)) / 2 + j - i;
        }

        static
        NUMERICS_INLINE
        size_type element1 (size_type i, size_type size1, size_type j, size_type size2) {
            check (i <= size1, bad_index ());
            return i;
        }
        static
        NUMERICS_INLINE
        size_type element2 (size_type i, size_type size1, size_type j, size_type size2) {
            check (j <= size2, bad_index ());
            return j;
        }
        static
        NUMERICS_INLINE
        size_type index1 (size_type index1, size_type index2) {
            return index1;
        }
        static
        NUMERICS_INLINE
        size_type index2 (size_type index1, size_type index2) {
            return index2;
        }
        static
        NUMERICS_INLINE
        size_type size1 (size_type size1, size_type size2) {
            return size1;
        }
        static
        NUMERICS_INLINE
        size_type size2 (size_type size1, size_type size2) {
            return size2;
        }

        // Iterating
        template<class I>
        static
        NUMERICS_INLINE
        void increment1 (I &it, size_type size1, size_type size2) {
            it += size2;
        }
        template<class I>
        static
        NUMERICS_INLINE
        void decrement1 (I &it, size_type size1, size_type size2) {
            it -= size2;
        }
        template<class I>
        static
        NUMERICS_INLINE
        void increment2 (I &it, size_type size1, size_type size2) {
            ++ it;
        }
        template<class I>
        static
        NUMERICS_INLINE
        void decrement2 (I &it, size_type size1, size_type size2) {
            -- it;
        }
    };

    // This functor computes the address translation
    // matrix [i] [j] -> storage [i + j * size1]
    struct column_major {
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef column_major_tag orientation_category;

        // Indexing
        static
        NUMERICS_INLINE
        size_type element (size_type i, size_type size1, size_type j, size_type size2) {
            check (i <= size1, bad_index ());
            check (j <= size2, bad_index ());
            return i + j * size1;
        }
        static
        NUMERICS_INLINE
        difference_type distance1 (difference_type k, size_type size1, size_type size2) {
            return k;
        }
        static
        NUMERICS_INLINE
        difference_type distance2 (difference_type k, size_type size1, size_type size2) {
            return size1 != 0 ? k / size1 : 0;
        }
        static
        NUMERICS_INLINE
        size_type index1 (difference_type k, size_type size1, size_type size2) {
            return size1 != 0 ? k % size1 : 0;
        }
        static
        NUMERICS_INLINE
        size_type index2 (difference_type k, size_type size1, size_type size2) {
            return size1 != 0 ? k / size1 : 0;
        }
        static
        NUMERICS_INLINE
        bool fast1 () {
            return true;
        }
        static
        NUMERICS_INLINE
        size_type one1 (size_type size1, size_type size2) {
            return 1;
        }
        static
        NUMERICS_INLINE
        bool fast2 () {
            return false;
        }
        static
        NUMERICS_INLINE
        size_type one2 (size_type size1, size_type size2) {
            return size1;
        }

        static
        NUMERICS_INLINE
        size_type lower_element (size_type i, size_type size1, size_type j, size_type size2) {
            check (i <= size1, bad_index ());
            check (j <= size2, bad_index ());
            check (i >= j, bad_index ());
            // sigma_j (size - j) = size * j - j * (j - 1) / 2
            // j = 0 1 2 3, sigma = 0 4 7 9
            return i - j + (j * (2 * std::max (size1, size2) - j + 1)) / 2;
        }
        static
        NUMERICS_INLINE
        size_type upper_element (size_type i, size_type size1, size_type j, size_type size2) {
            check (i <= size1, bad_index ());
            check (j <= size2, bad_index ());
            check (i <= j, bad_index ());
            // sigma_j (j + 1) = (j + 1) * j / 2
            // j = 0 1 2 3, sigma = 0 1 3 6
            return i + ((j + 1) * j) / 2;
        }

        static
        NUMERICS_INLINE
        size_type element1 (size_type i, size_type size1, size_type j, size_type size2) {
            check (j <= size2, bad_index ());
            return j;
        }
        static
        NUMERICS_INLINE
        size_type element2 (size_type i, size_type size1, size_type j, size_type size2) {
            check (i <= size1, bad_index ());
            return i;
        }
        static
        NUMERICS_INLINE
        size_type index1 (size_type index1, size_type index2) {
            return index2;
        }
        static
        NUMERICS_INLINE
        size_type index2 (size_type index1, size_type index2) {
            return index1;
        }
        static
        NUMERICS_INLINE
        size_type size1 (size_type size1, size_type size2) {
            return size2;
        }
        static
        NUMERICS_INLINE
        size_type size2 (size_type size1, size_type size2) {
            return size1;
        }

        // Iterating
        template<class I>
        static
        NUMERICS_INLINE
        void increment1 (I &it, size_type size1, size_type size2) {
            ++ it;
        }
        template<class I>
        static
        NUMERICS_INLINE
        void decrement1 (I &it, size_type size1, size_type size2) {
            -- it;
        }
        template<class I>
        static
        NUMERICS_INLINE
        void increment2 (I &it, size_type size1, size_type size2) {
            it += size1;
        }
        template<class I>
        static
        NUMERICS_INLINE
        void decrement2 (I &it, size_type size1, size_type size2) {
            it -= size1;
        }
    };

    struct lower {
        typedef std::size_t size_type;
        typedef lower_tag packed_category;

        static
        NUMERICS_INLINE
        size_type packed_size (size_type size1, size_type size2) {
            size_type size = std::max (size1, size2);
            return ((size + 1) * size) / 2;
        }

        static
        NUMERICS_INLINE
        bool zero (size_type i, size_type j) {
            return j > i;
        }
        static
        NUMERICS_INLINE
        bool one (size_type i, size_type j) {
            return false;
        }
        static
        NUMERICS_INLINE
        bool other (size_type i, size_type j) {
            return j <= i;
        }
        template<class F>
        static
        NUMERICS_INLINE
        size_type element (F, size_type i, size_type size1, size_type j, size_type size2) {
            return F::lower_element (i, size1, j, size2);
        }

        static
        NUMERICS_INLINE
        size_type restrict1 (size_type i, size_type j) {
            return std::max (i, j);
        }
        static
        NUMERICS_INLINE
        size_type restrict2 (size_type i, size_type j) {
            return std::min (i + 1, j);
        }
        static
        NUMERICS_INLINE
        size_type mutable_restrict1 (size_type i, size_type j) {
            return std::max (i, j);
        }
        static
        NUMERICS_INLINE
        size_type mutable_restrict2 (size_type i, size_type j) {
            return std::min (i + 1, j);
        }
    };
    struct upper {
        typedef std::size_t size_type;
        typedef upper_tag packed_category;

        static
        NUMERICS_INLINE
        size_type packed_size (size_type size1, size_type size2) {
            size_type size = std::max (size1, size2);
            return ((size + 1) * size) / 2;
        }

        static
        NUMERICS_INLINE
        bool zero (size_type i, size_type j) {
            return j < i;
        }
        static
        NUMERICS_INLINE
        bool one (size_type i, size_type j) {
            return false;
        }
        static
        NUMERICS_INLINE
        bool other (size_type i, size_type j) {
            return j >= i;
        }
        template<class F>
        static
        NUMERICS_INLINE
        size_type element (F, size_type i, size_type size1, size_type j, size_type size2) {
            return F::upper_element (i, size1, j, size2);
        }

        static
        NUMERICS_INLINE
        size_type restrict1 (size_type i, size_type j) {
            return std::min (i, j + 1);
        }
        static
        NUMERICS_INLINE
        size_type restrict2 (size_type i, size_type j) {
            return std::max (i, j);
        }
        static
        NUMERICS_INLINE
        size_type mutable_restrict1 (size_type i, size_type j) {
            return std::min (i, j + 1);
        }
        static
        NUMERICS_INLINE
        size_type mutable_restrict2 (size_type i, size_type j) {
            return std::max (i, j);
        }
    };
    struct unit_lower {
        typedef std::size_t size_type;
        typedef unit_lower_tag packed_category;

        static
        NUMERICS_INLINE
        size_type packed_size (size_type size1, size_type size2) {
            size_type size = std::max (size1, size2);
            return ((size + 1) * size) / 2;
        }

        static
        NUMERICS_INLINE
        bool zero (size_type i, size_type j) {
            return j > i;
        }
        static
        NUMERICS_INLINE
        bool one (size_type i, size_type j) {
            return j == i;
        }
        static
        NUMERICS_INLINE
        bool other (size_type i, size_type j) {
            return j < i;
        }
        template<class F>
        static
        NUMERICS_INLINE
        size_type element (F, size_type i, size_type size1, size_type j, size_type size2) {
            return F::lower_element (i, size1, j, size2);
        }

        static
        NUMERICS_INLINE
        size_type restrict1 (size_type i, size_type j) {
            return std::max (i, j);
        }
        static
        NUMERICS_INLINE
        size_type restrict2 (size_type i, size_type j) {
            return std::min (i + 1, j);
        }
        static
        NUMERICS_INLINE
        size_type mutable_restrict1 (size_type i, size_type j) {
            return std::max (i, j);
        }
        static
        NUMERICS_INLINE
        size_type mutable_restrict2 (size_type i, size_type j) {
            return std::min (i, j);
        }
    };
    struct unit_upper {
        typedef std::size_t size_type;
        typedef unit_upper_tag packed_category;

        static
        NUMERICS_INLINE
        size_type packed_size (size_type size1, size_type size2) {
            size_type size = std::max (size1, size2);
            return ((size + 1) * size) / 2;
        }

        static
        NUMERICS_INLINE
        bool zero (size_type i, size_type j) {
            return j < i;
        }
        static
        NUMERICS_INLINE
        bool one (size_type i, size_type j) {
            return j == i;
        }
        static
        NUMERICS_INLINE
        bool other (size_type i, size_type j) {
            return j > i;
        }
        template<class F>
        static
        NUMERICS_INLINE
        size_type element (F, size_type i, size_type size1, size_type j, size_type size2) {
            return F::upper_element (i, size1, j, size2);
        }

        static
        NUMERICS_INLINE
        size_type restrict1 (size_type i, size_type j) {
            return std::min (i, j + 1);
        }
        static
        NUMERICS_INLINE
        size_type restrict2 (size_type i, size_type j) {
            return std::max (i, j);
        }
        static
        NUMERICS_INLINE
        size_type mutable_restrict1 (size_type i, size_type j) {
            return std::min (i, j);
        }
        static
        NUMERICS_INLINE
        size_type mutable_restrict2 (size_type i, size_type j) {
            return std::max (i, j);
        }
    };

}}

#endif



















