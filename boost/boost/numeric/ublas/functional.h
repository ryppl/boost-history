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

namespace numerics {

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
        // Dense random access iterator
        template<class I>
        NUMERICS_INLINE
        result_type operator () (difference_type size, I it) const { 
            result_type t (0);
            while (-- size >= 0)
                t += *it, ++ it;
            return t; 
        }
        // Packed/sparse bidirectional iterator
        template<class I>
        NUMERICS_INLINE
        result_type operator () (I it, const I &it_end) const { 
            result_type t (0);
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
        typedef T value_type;
        typedef typename type_traits<T>::norm_type norm_type;
        typedef norm_type result_type;
    };

    template<class T>
    struct vector_norm_1: 
        public vector_scalar_norm_unary_functor<T> {
        typedef typename vector_scalar_norm_unary_functor<T>::size_type size_type;
        typedef typename vector_scalar_norm_unary_functor<T>::difference_type difference_type;
        typedef typename vector_scalar_norm_unary_functor<T>::value_type value_type;
        typedef typename vector_scalar_norm_unary_functor<T>::norm_type norm_type;
        typedef typename vector_scalar_norm_unary_functor<T>::result_type result_type;

        template<class E>
        NUMERICS_INLINE
        result_type operator () (const vector_expression<E> &e) const { 
            result_type t (0);
            size_type size (e ().size ());
            for (size_type i = 0; i < size; ++ i) {
                result_type u (detail::abs (e () (i)));
                t += u;
            }
            return t; 
        }
        // Dense random access iterator
        template<class I>
        NUMERICS_INLINE
        result_type operator () (difference_type size, I it) const { 
            result_type t (0);
            while (-- size >= 0) {
                result_type u (detail::abs (*it));
                t += u;
                ++ it;
            }
            return t; 
        }
        // Packed/sparse bidirectional iterator
        template<class I>
        NUMERICS_INLINE
        result_type operator () (I it, const I &it_end) const { 
            result_type t (0);
            while (it != it_end) {
                result_type u (detail::abs (*it));
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
        typedef typename vector_scalar_norm_unary_functor<T>::norm_type norm_type;
        typedef typename vector_scalar_norm_unary_functor<T>::result_type result_type;

        template<class E>
        NUMERICS_INLINE
        result_type operator () (const vector_expression<E> &e) const { 
            norm_type t (0);
            size_type size (e ().size ());
            for (size_type i = 0; i < size; ++ i) {
                norm_type u (detail::abs (e () (i)));
                t +=  u * u;
            }
            return detail::sqrt (t); 
        }
        // Dense random access iterator
        template<class I>
        NUMERICS_INLINE
        result_type operator () (difference_type size, I it) const { 
            norm_type t (0);
            while (-- size >= 0) {
                norm_type u (detail::abs (*it));
                t +=  u * u;
                ++ it;
            }
            return detail::sqrt (t); 
        }
        // Packed/sparse bidirectional iterator
        template<class I>
        NUMERICS_INLINE
        result_type operator () (I it, const I &it_end) const { 
            norm_type t (0);
            while (it != it_end) {
                norm_type u (detail::abs (*it));
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
        typedef typename vector_scalar_norm_unary_functor<T>::norm_type norm_type;
        typedef typename vector_scalar_norm_unary_functor<T>::result_type result_type;

        template<class E>
        NUMERICS_INLINE
        result_type operator () (const vector_expression<E> &e) const { 
            norm_type t (0);
            size_type size (e ().size ());
            for (size_type i = 0; i < size; ++ i) {
                norm_type u (detail::abs (e () (i)));
                if (u > t) 
                    t = u;
            }
            return t; 
        }
        // Dense random access iterator
        template<class I>
        NUMERICS_INLINE
        result_type operator () (difference_type size, I it) const { 
            norm_type t (0);
            while (-- size >= 0) {
                norm_type u (detail::abs (*it));
                if (u > t) 
                    t = u;
                ++ it;
            }
            return t; 
        }
        // Packed/sparse bidirectional iterator
        template<class I>
        NUMERICS_INLINE
        result_type operator () (I it, const I &it_end) const { 
            norm_type t (0);
            while (it != it_end) {
                norm_type u (detail::abs (*it));
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
        typedef typename type_traits<T>::norm_type norm_type;
        typedef size_type result_type;
    };

    template<class T>
    struct vector_index_norm_inf: 
        public vector_scalar_index_unary_functor<T> {
        typedef typename vector_scalar_index_unary_functor<T>::size_type size_type;
        typedef typename vector_scalar_index_unary_functor<T>::difference_type difference_type;
        typedef typename vector_scalar_index_unary_functor<T>::value_type value_type;
        typedef typename vector_scalar_index_unary_functor<T>::norm_type norm_type;
        typedef typename vector_scalar_index_unary_functor<T>::result_type result_type;

        template<class E>
        NUMERICS_INLINE
        result_type operator () (const vector_expression<E> &e) const { 
            result_type i_norm_inf (0);
            norm_type t (0);
            size_type size (e ().size ());
            for (size_type i = 0; i < size; ++ i) {
                norm_type u (detail::abs (e () (i)));
                if (u > t) {
                    i_norm_inf = i;
                    t = u;
                }
            }
            return i_norm_inf; 
        }
        // Dense random access iterator
        template<class I>
        NUMERICS_INLINE
        result_type operator () (difference_type size, I it) const { 
            result_type i_norm_inf (0);
            norm_type t (0);
            while (-- size >= 0) {
                norm_type u (detail::abs (*it));
                if (u > t) {
                    i_norm_inf = it.index ();
                    t = u;
                }
                ++ it;
            }
            return i_norm_inf; 
        }
        // Packed/sparse bidirectional iterator
        template<class I>
        NUMERICS_INLINE
        result_type operator () (I it, const I &it_end) const { 
            result_type i_norm_inf (it.index ());
            norm_type t (0);
            while (it != it_end) {
                norm_type u (detail::abs (*it));
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
            for (size_type i = 0; i < size; ++ i)
                t = t + e1 () (i) * e2 () (i);
            return t; 
        }
        // Dense random access iterator
        template<class I1, class I2>
        NUMERICS_INLINE
        result_type operator () (difference_type size, I1 it1, I2 it2) const { 
            result_type t (0);
            while (-- size >= 0)
                t = t + *it1 * *it2, ++ it1, ++ it2;
            return t; 
        }
        // Packed bidirectional iterator
        template<class I1, class I2>
        NUMERICS_INLINE
        result_type operator () (I1 it1, const I1 &it1_end, I2 it2, const I2 &it2_end) const { 
            result_type t (0);
            while (it1 != it1_end && it1.index () < it2.index ())
                ++ it1;
            while (it2 != it2_end && it2.index () < it1.index ())
                ++ it2;
            while (it1 != it1_end && it2 != it2_end) 
                t += *it1 * *it2, ++ it1, ++ it2;
            return t; 
        }
        // Sparse bidirectional iterator
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
            for (size_type j = 0; j < size; ++ j)
                t += e1 () (i, j) * e2 () (j);
            return t; 
        }
        // Dense random access iterator
        template<class I1, class I2>
        NUMERICS_INLINE
        result_type operator () (difference_type size, I1 it1, I2 it2) const { 
            result_type t (0);
            while (-- size >= 0)
                t += *it1 * *it2, ++ it1, ++ it2;
            return t; 
        }
        // Packed bidirectional iterator
        template<class I1, class I2>
        NUMERICS_INLINE
        result_type operator () (I1 it1, const I1 &it1_end, I2 it2, const I2 &it2_end) const { 
            result_type t (0);
            while (it1 != it1_end && it1.index2 () < it2.index ())
                ++ it1;
            while (it2 != it2_end && it2.index () < it1.index2 ())
                ++ it2;
            while (it1 != it1_end && it2 != it2_end) 
                t += *it1 * *it2, ++ it1, ++ it2;
            return t; 
        }
        // Sparse bidirectional iterator
        template<class I1, class I2>
        NUMERICS_INLINE
        result_type operator () (I1 it1, const I1 &it1_end, I2 it2, const I2 &it2_end, sparse_bidirectional_iterator_tag) const { 
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
            for (size_type j = 0; j < size; ++ j)
                t += e1 () (i) * e2 () (i, j);
            return t; 
        }
        // Dense random access iterator
        template<class I1, class I2>
        NUMERICS_INLINE
        result_type operator () (difference_type size, I1 it1, I2 it2) const { 
            result_type t (0);
            while (-- size >= 0)
                t += *it1 * *it2, ++ it1, ++ it2;
            return t; 
        }
        // Packed bidirectional iterator
        template<class I1, class I2>
        NUMERICS_INLINE
        result_type operator () (I1 it1, const I1 &it1_end, I2 it2, const I2 &it2_end) const { 
            result_type t (0);
            while (it1 != it1_end && it1.index1 () < it2.index ())
                ++ it1;
            while (it2 != it2_end && it2.index () < it1.index1 ())
                ++ it2;
            while (it1 != it1_end && it2 != it2_end) 
                t += *it1 * *it2, ++ it1, ++ it2;
            return t; 
        }
        // Sparse bidirectional iterator
        template<class I1, class I2>
        NUMERICS_INLINE
        result_type operator () (I1 it1, const I1 &it1_end, I2 it2, const I2 &it2_end, sparse_bidirectional_iterator_tag) const { 
            result_type t (0);
            while (it1 != it1_end && it2 != it2_end) {
                difference_type compare = it1.index1 () - it2.index ();
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
            for (size_type k = 0; k < size; ++ k) 
                t += e1 () (i, k) * e2 () (k, j);
            return t; 
        }
        // Dense random access iterator
        template<class I1, class I2>
        NUMERICS_INLINE
        result_type operator () (difference_type size, I1 it1, I2 it2) const { 
            result_type t (0);
            while (-- size >= 0)
                t += *it1 * *it2, ++ it1, ++ it2;
            return t; 
        }
        // Packed bidirectional iterator
        template<class I1, class I2>
        NUMERICS_INLINE
        result_type operator () (I1 it1, const I1 &it1_end, I2 it2, const I2 &it2_end, packed_bidirectional_iterator_tag) const { 
            result_type t (0);
            while (it1 != it1_end && it1.index2 () < it2.index1 ())
                ++ it1;
            while (it2 != it2_end && it2.index1 () < it1.index2 ())
                ++ it2;
            while (it1 != it1_end && it2 != it2_end) 
                t += *it1 * *it2, ++ it1, ++ it2;
            return t; 
        }
        // Sparse bidirectional iterator
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
    struct matrix_scalar_norm_unary_functor {
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
		typedef T value_type;
        typedef typename type_traits<T>::norm_type norm_type;
		typedef norm_type result_type;
    };

    template<class T>
    struct matrix_norm_1: 
        public matrix_scalar_norm_unary_functor<T> {
        typedef typename matrix_scalar_norm_unary_functor<T>::size_type size_type;
        typedef typename matrix_scalar_norm_unary_functor<T>::difference_type difference_type;
        typedef typename matrix_scalar_norm_unary_functor<T>::value_type value_type;
        typedef typename matrix_scalar_norm_unary_functor<T>::norm_type norm_type;
        typedef typename matrix_scalar_norm_unary_functor<T>::result_type result_type;

        template<class E>
        NUMERICS_INLINE
        result_type operator () (const matrix_expression<E> &e) const { 
            norm_type t (0);
            size_type size2 (e ().size2 ());
            for (size_type j = 0; j < size2; ++ j) {
                norm_type u (0);
                size_type size1 (e ().size1 ());
                for (size_type i = 0; i < size1; ++ i) {
                    norm_type v (detail::abs (e () (i, j)));
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
        public matrix_scalar_norm_unary_functor<T> {
        typedef typename matrix_scalar_norm_unary_functor<T>::size_type size_type;
        typedef typename matrix_scalar_norm_unary_functor<T>::difference_type difference_type;
        typedef typename matrix_scalar_norm_unary_functor<T>::value_type value_type;
        typedef typename matrix_scalar_norm_unary_functor<T>::norm_type norm_type;
        typedef typename matrix_scalar_norm_unary_functor<T>::result_type result_type;

        template<class E>
        NUMERICS_INLINE
        result_type operator () (const matrix_expression<E> &e) const { 
            norm_type t (0);
            size_type size1 (e ().size1 ());
            for (size_type i = 0; i < size1; ++ i) {
                size_type size2 (e ().size2 ());
                for (size_type j = 0; j < size2; ++ j) {
                    norm_type u (detail::abs (e () (i, j)));
                    t +=  u * u;
                }
            }
            return detail::sqrt (t); 
        }
    };
    template<class T>
    struct matrix_norm_inf: 
        public matrix_scalar_norm_unary_functor<T> {
        typedef typename matrix_scalar_norm_unary_functor<T>::size_type size_type;
        typedef typename matrix_scalar_norm_unary_functor<T>::difference_type difference_type;
        typedef typename matrix_scalar_norm_unary_functor<T>::value_type value_type;
        typedef typename matrix_scalar_norm_unary_functor<T>::norm_type norm_type;
        typedef typename matrix_scalar_norm_unary_functor<T>::result_type result_type;

        template<class E>
        NUMERICS_INLINE
        result_type operator () (const matrix_expression<E> &e) const { 
            norm_type t (0);
            size_type size1 (e ().size1 ());
            for (size_type i = 0; i < size1; ++ i) {
                norm_type u (0);
                size_type size2 (e ().size2 ());
                for (size_type j = 0; j < size2; ++ j) {
                    norm_type v (detail::abs (e () (i, j)));
                    u += v;
                }
                if (u > t) 
                    t = u;  
            }
            return t; 
        }
    };

}

#endif 






