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

#include "config.h"
#include "exception.h"
#include "traits.h"
#include "math.h"

namespace numerics {

    // Scalar functors

    // Unary
    template<class T>
    struct scalar_unary_functor {
        typedef T value_type;
    };

    template<class T>
    struct scalar_negate: 
        public scalar_unary_functor<T> {
        typedef typename scalar_unary_functor<T>::value_type value_type;

        NUMERICS_INLINE
        value_type operator () (const T &t) const { 
            return - t; 
        }
    };
    template<class T>
    struct scalar_conj: 
        public scalar_unary_functor<T> {
        typedef typename scalar_unary_functor<T>::value_type value_type;

        NUMERICS_INLINE
        value_type operator () (const T &t) const { 
            return detail::conj (t); 
        }
    };

    // Binary
    template<class T1, class T2>
    struct scalar_binary_functor {
        typedef promote_traits<T1, T2>::promote_type value_type;
    };

    template<class T1, class T2>
    struct scalar_plus: 
        public scalar_binary_functor<T1, T2> { 
        typedef typename scalar_binary_functor<T1, T2>::value_type value_type;

        NUMERICS_INLINE
        value_type operator () (const T1 &t1, const T2 &t2) const { 
            return t1 + t2; 
        }
    };
    template<class T1, class T2>
    struct scalar_minus: 
        public scalar_binary_functor<T1, T2> { 
        typedef typename scalar_binary_functor<T1, T2>::value_type value_type;

        NUMERICS_INLINE
        value_type operator () (const T1 &t1, const T2 &t2) const { 
            return t1 - t2; 
        }
    };
    template<class T1, class T2>
    struct scalar_multiplies: 
        public scalar_binary_functor<T1, T2> { 
        typedef typename scalar_binary_functor<T1, T2>::value_type value_type;

        NUMERICS_INLINE
        value_type operator () (const T1 &t1, const T2 &t2) const { 
            return t1 * t2; 
        }
    };
    template<class T1, class T2>
    struct scalar_divides: 
        public scalar_binary_functor<T1, T2> { 
        typedef typename scalar_binary_functor<T1, T2>::value_type value_type;

        NUMERICS_INLINE
        value_type operator () (const T1 &t1, const T2 &t2) const { 
            return t1 / t2; 
        }
    };

    // Vector functors

    // Unary returning scalar
    template<class T>
    struct vector_scalar_unary_functor {
        typedef T value_type;
    };

    template<class T>
    struct vector_sum: 
        public vector_scalar_unary_functor<T> {
        typedef typename vector_scalar_unary_functor<T>::value_type value_type;

        template<class E>
        NUMERICS_INLINE
        value_type operator () (const vector_expression<T, E> &e) const { 
            size_type size (e.size ());
            value_type t (0);
            for (size_type i = 0; i < size; ++ i)
                t += e (i);
            return t; 
        }
#ifdef NUMERICS_USE_ITERATOR
        template<class I>
        NUMERICS_INLINE
#ifdef USE_MSVC
        value_type operator () (size_type size, I &it) const { 
#else
        value_type operator () (size_type size, I it) const { 
#endif
            value_type t (0);
            for (size_type i = 0; i < size; ++ i)
                t += *it, ++ it;
            return t; 
        }
#endif // NUMERICS_USE_ITERATOR
    };

    // Unary returning scalar norm
    template<class T>
    struct vector_scalar_norm_unary_functor {
        typedef type_traits<T>::norm_type value_type;
    };

    template<class T>
    struct vector_norm_1: 
        public vector_scalar_norm_unary_functor<T> {
        typedef typename vector_scalar_norm_unary_functor<T>::value_type value_type;

        template<class E>
        NUMERICS_INLINE
        value_type operator () (const vector_expression<T, E> &e) const { 
            size_type size (e.size ());
            value_type t (0);
            for (size_type i = 0; i < size; ++ i) {
                value_type u (detail::abs (e (i)));
                t += u;
            }
            return t; 
        }
#ifdef NUMERICS_USE_ITERATOR
        template<class I>
        NUMERICS_INLINE
#ifdef USE_MSVC
        value_type operator () (size_type size, I &it) const { 
#else
        value_type operator () (size_type size, I it) const { 
#endif
            value_type t (0);
            for (size_type i = 0; i < size; ++ i) {
                value_type u (detail::abs (*it));
                t += u;
                ++ it;
            }
            return t; 
        }
#endif // NUMERICS_USE_ITERATOR
    };
    template<class T>
    struct vector_norm_2: 
        public vector_scalar_norm_unary_functor<T> {
        typedef typename vector_scalar_norm_unary_functor<T>::value_type value_type;

        template<class E>
        NUMERICS_INLINE
        value_type operator () (const vector_expression<T, E> &e) const { 
            size_type size (e.size ());
            value_type t (0);
            for (size_type i = 0; i < size; ++ i) {
                value_type u (detail::abs (e (i)));
                t +=  u * u;
            }
#ifdef USE_MSVC
            return ::sqrt (t); 
#else // USE_MSVC
            return std::sqrt (t); 
#endif // USE_MSVC
        }
#ifdef NUMERICS_USE_ITERATOR
        template<class I>
        NUMERICS_INLINE
#ifdef USE_MSVC
        value_type operator () (size_type size, I &it) const { 
#else
        value_type operator () (size_type size, I it) const { 
#endif
            value_type t (0);
            for (size_type i = 0; i < size; ++ i) {
                value_type u (detail::abs (*it));
                t +=  u * u;
                ++ it;
            }
#ifdef USE_MSVC
            return ::sqrt (t); 
#else // USE_MSVC
            return std::sqrt (t); 
#endif // USE_MSVC
        }
#endif // NUMERICS_USE_ITERATOR
    };
    template<class T>
    struct vector_norm_inf: 
        public vector_scalar_norm_unary_functor<T> {
        typedef typename vector_scalar_norm_unary_functor<T>::value_type value_type;

        template<class E>
        NUMERICS_INLINE
        value_type operator () (const vector_expression<T, E> &e) const { 
            size_type size (e.size ());
            value_type t (0);
            for (size_type i = 0; i < size; ++ i) {
                value_type u (detail::abs (e (i)));
                t = std::max (t, u);
            }
            return t; 
        }
#ifdef NUMERICS_USE_ITERATOR
        template<class I>
        NUMERICS_INLINE
#ifdef USE_MSVC
        value_type operator () (size_type size, I &it) const { 
#else
        value_type operator () (size_type size, I it) const { 
#endif
            value_type t (0);
            for (size_type i = 0; i < size; ++ i) {
                value_type u (detail::abs (*it));
                t = std::max (t, u);
                ++ it;
            }
            return t; 
        }
#endif // NUMERICS_USE_ITERATOR
    };

    // Binary returning scalar
    template<class T1, class T2>
    struct vector_scalar_binary_functor {
        typedef promote_traits<T1, T2>::promote_type value_type;
    };
    template<class T1, class T2>
    struct vector_scalar_prec_binary_functor {
        typedef type_traits<promote_traits<T1, T2>::promote_type>::precision_type value_type;
    };

    template<class T1, class T2>
    struct vector_inner_prod: 
        public vector_scalar_binary_functor<T1, T2> {
        typedef typename vector_scalar_binary_functor<T1, T2>::value_type value_type;

        template<class E1, class E2>
        NUMERICS_INLINE
        value_type operator () (const vector_expression<T1, E1> &e1, 
                                const vector_expression<T2, E2> &e2) const { 
            size_type size (common (e1.size (), e2.size ()));
            value_type t (0);
            for (size_type i = 0; i < size; ++ i)
                t += e1 (i) * e2 (i);
            return t; 
        }
#ifdef NUMERICS_USE_ITERATOR
        template<class I1, class I2>
        NUMERICS_INLINE
#ifdef USE_MSVC
        value_type operator () (size_type size, I1 &it1, I2 &it2) const { 
#else
        value_type operator () (size_type size, I1 it1, I2 it2) const { 
#endif
            value_type t (0);
            for (size_type i = 0; i < size; ++ i) 
                t += *it1 * *it2, ++ it1, ++ it2;
            return t; 
        }
#endif // NUMERICS_USE_ITERATOR
    };
    template<class T1, class T2>
    struct vector_prec_inner_prod: 
        public vector_scalar_prec_binary_functor<T1, T2> {
        typedef typename vector_scalar_binary_functor<T1, T2>::value_type value_type;

        template<class E1, class E2>
        NUMERICS_INLINE
        value_type operator () (const vector_expression<T1, E1> &e1, 
                                const vector_expression<T2, E2> &e2) const { 
            size_type size (common (e1.size (), e2.size ()));
            value_type t (0);
            for (size_type i = 0; i < size; ++ i)
                t += e1 (i) * e2 (i);
            return t; 
        }
#ifdef NUMERICS_USE_ITERATOR
        template<class I1, class I2>
        NUMERICS_INLINE
#ifdef USE_MSVC
        value_type operator () (size_type size, I1 &it1, I2 &it2) const { 
#else
        value_type operator () (size_type size, I1 it1, I2 it2) const { 
#endif
            value_type t (0);
            for (size_type i = 0; i < size; ++ i) 
                t += *it1 * *it2, ++ it1, ++ it2;
            return t; 
        }
#endif // NUMERICS_USE_ITERATOR
    };

    // Binary returning matrix
    template<class T1, class T2>
    struct vector_matrix_binary_functor {
        typedef promote_traits<T1, T2>::promote_type value_type;
    };

    template<class T1, class T2>
    struct vector_outer_prod: 
        public vector_matrix_binary_functor<T1, T2> {
        typedef typename vector_matrix_binary_functor<T1, T2>::value_type value_type;

        template<class E1, class E2>
        NUMERICS_INLINE
        value_type operator () (const vector_expression<T1, E1> &e1, 
                                const vector_expression<T2, E2> &e2,
                                size_type i, size_type j) const { 
            return e1 (i) * e2 (j);
        }
#ifdef NUMERICS_USE_ITERATOR
        template<class I1, class I2>
        NUMERICS_INLINE
        value_type operator () (const I1 &it1, const I2 &it2) const { 
            return *it1 * *it2;
        }
#endif // NUMERICS_USE_ITERATOR
    };

    // Matrix functors

    // Unary returning matrix
    template<class T>
    struct matrix_matrix_unary_functor {
        typedef T value_type;
    };

    template<class T>
    struct matrix_trans: 
        public matrix_matrix_unary_functor<T> {
        typedef typename matrix_matrix_unary_functor<T>::value_type value_type;

        template<class E>
        NUMERICS_INLINE
        value_type operator () (const matrix_expression<T, E> &e, 
                                size_type i, size_type j) const { 
            return e (j, i);
        }
#ifdef NUMERICS_USE_ITERATOR
        template<class I>
        NUMERICS_INLINE
        value_type operator () (const I &it) const { 
            return *it;
        }
#endif // NUMERICS_USE_ITERATOR
    };
    template<class T>
    struct matrix_herm: 
        public matrix_matrix_unary_functor<T> {
        typedef typename matrix_matrix_unary_functor<T>::value_type value_type;

        template<class E>
        NUMERICS_INLINE
        value_type operator () (const matrix_expression<T, E> &e, 
                                size_type i, size_type j) const { 
            return detail::conj (e (j, i));
        }
#ifdef NUMERICS_USE_ITERATOR
        template<class I>
        NUMERICS_INLINE
        value_type operator () (const I &it) const { 
            return detail::conj (*it);
        }
#endif // NUMERICS_USE_ITERATOR
    };

    // Binary returning vector
    template<class T1, class T2>
    struct matrix_vector_binary_functor {
        typedef promote_traits<T1, T2>::promote_type value_type;
    };

    template<class T1, class T2>
    struct matrix_vector_prod: 
        public matrix_vector_binary_functor<T1, T2> {
        typedef typename matrix_vector_binary_functor<T1, T2>::value_type value_type;

        template<class E1, class E2>
        NUMERICS_INLINE
        value_type operator () (const matrix_expression<T1, E1> &e1, 
                                const vector_expression<T2, E2> &e2, 
                                size_type i) const { 
            size_type size = common (e1.size2 (), e2.size ());
            value_type t (0);
            for (size_type j = 0; j < size; ++ j)
                t += e1 (i, j) * e2 (j);
            return t; 
        }
#ifdef NUMERICS_USE_ITERATOR
        template<class I1, class I2>
        NUMERICS_INLINE
#ifdef USE_MSVC
        value_type operator () (size_type size, I1 &it1, I2 &it2) const { 
#else
        value_type operator () (size_type size, I1 it1, I2 it2) const { 
#endif
            value_type t (0);
            for (size_type j = 0; j < size; ++ j)
                t += *it1 * *it2, ++ it1, ++ it2;
            return t; 
        }
#endif // NUMERICS_USE_ITERATOR
    };

    // Ternary returning vector
    template<class T1, class T2, class T3>
    struct matrix_vector_ternary_functor {
        typedef promote_traits<T1, T3>::promote_type value_type;
    };

    enum triangular { lower_triangular, upper_triangular };

    template<class T1, class T2, class T3>
    class matrix_vector_solve:
        public matrix_vector_ternary_functor<T1, T2, T3> {
    private:
        mutable bool first_;

    public:
        typedef typename matrix_vector_ternary_functor<T1, T2, T3>::value_type value_type;

        NUMERICS_INLINE
        matrix_vector_solve (): 
            first_ (true) {}

        template<class E1, class E2, class E3>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        value_type operator () (const matrix_expression<T1, E1> &e1, 
                                vector_expression<T2, E2> &e2, 
                                const vector_expression<T3, E3> &e3, 
                                int hint, 
                                size_type i) const { 
            if (first_) {
                check<bad_size>::precondition (e1.size1 () == e1.size2 ());
                check<bad_size>::precondition (e1.size2 () == e2.size ());
                check<bad_size>::precondition (e1.size1 () == e3.size ());
                if (hint == lower_triangular) {
                    size_type size = e2.size ();
                    for (size_type m = 0; m < size; ++ m) 
						e2 (m) = e3 (m);
                    for (size_type n = 0; n < size; ++ n) {
#ifdef NUMERICS_BOUNDS_CHECK
                        if (check<non_triangular>::enabled ())
                            for (size_type m = 0; m < n; ++ m)
                                check<non_triangular>::precondition (e1 (m, n) == T1 (0));
#endif // NUMERICS_BOUNDS_CHECK
                        if (e1 (n, n) == T1 (0))
                            throw singular ();
                        e2 (n) /= e1 (n, n);
                        for (size_type m = n + 1; m < size; ++ m) 
                            e2 (m) -= e1 (m, n) * e2 (n);
                    }
                } else if (hint == upper_triangular) {
                    size_type size = e2.size ();
                    for (size_type m = size - 1; (signed) m >= 0; -- m) 
						e2 (m) = e3 (m);
                    for (size_type n = size - 1; (signed) n >= 0; -- n) {
#ifdef NUMERICS_BOUNDS_CHECK
                        if (check<non_triangular>::enabled ())
                            for (size_type m = size - 1; m > n; -- m) 
                                check<non_triangular>::precondition (e1 (m, n) == T1 (0));
#endif // NUMERICS_BOUNDS_CHECK
                        if (e1 (n, n) == T1 (0))
                            throw singular ();
                        e2 (n) /= e1 (n, n);
                        for (size_type m = n - 1; (signed) m >= 0; -- m) 
                            e2 (m) -= e1 (m, n) * e2 (n);
                    }
                } else {
                    throw non_triangular ();
                }
                first_ = false;
            }
            return e2 (i); 
        }
    };

    // Binary returning matrix
    template<class T1, class T2>
    struct matrix_matrix_binary_functor {
        typedef promote_traits<T1, T2>::promote_type value_type;
    };

    template<class T1, class T2>
    struct matrix_matrix_prod: 
        public matrix_matrix_binary_functor<T1, T2> {
        typedef typename matrix_matrix_binary_functor<T1, T2>::value_type value_type;

        template<class E1, class E2>
        NUMERICS_INLINE
        value_type operator () (const matrix_expression<T1, E1> &e1, 
                                const matrix_expression<T2, E2> &e2, 
                                size_type i, size_type j) const { 
            size_type size = common (e1.size2 (), e2.size1 ());
            value_type t (0);
            for (size_type k = 0; k < size; ++ k)
                t += e1 (i, k) * e2 (k, j);
            return t; 
        }
#ifdef NUMERICS_USE_ITERATOR
        template<class I1, class I2>
        NUMERICS_INLINE
#ifdef USE_MSVC
        value_type operator () (size_type size, I1 &it1, I2 &it2) const { 
#else
        value_type operator () (size_type size, I1 it1, I2 it2) const { 
#endif
            value_type t (0);
            for (size_type k = 0; k < size; ++ k) 
                t += *it1 * *it2, ++ it1, ++ it2;
            return t; 
        }
#endif // NUMERICS_USE_ITERATOR
    };

    // Ternary returning matrix
    template<class T1, class T2, class T3>
    struct matrix_matrix_ternary_functor {
        typedef promote_traits<T1, T3>::promote_type value_type;
    };

    template<class T1, class T2, class T3>
    class matrix_matrix_solve:
        public matrix_matrix_ternary_functor<T1, T2, T3> {
    private:
        mutable bool first_;

    public:
        typedef typename matrix_matrix_ternary_functor<T1, T2, T3>::value_type value_type;

        NUMERICS_INLINE
        matrix_matrix_solve (): 
            first_ (true) {}

        template<class E1, class E2, class E3>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        value_type operator () (const matrix_expression<T1, E1> &e1, 
                                matrix_expression<T2, E2> &e2, 
                                const matrix_expression<T3, E3> &e3, 
                                int hint, 
                                size_type i, 
                                size_type j) const { 
            if (first_) {
                check<bad_size>::precondition (e1.size1 () == e1.size2 ());
                check<bad_size>::precondition (e1.size2 () == e2.size1 ());
                check<bad_size>::precondition (e1.size1 () == e3.size1 ());
                check<bad_size>::precondition (e2.size2 () == e3.size2 ());
                if (hint == lower_triangular) {
                    size_type size = common (e2.size1 (), e2.size2 ());
                    for (size_type l = 0; l < size; ++ l) 
						for (size_type m = 0; m < size; ++ m) 
							e2 (l, m) = e3 (l, m);
                    for (size_type n = 0; n < size; ++ n) {
#ifdef NUMERICS_BOUNDS_CHECK
                        if (check<non_triangular>::enabled ())
                            for (size_type m = 0; m < n; ++ m)
                                check<non_triangular>::precondition (e1 (m, n) == T1 (0));
#endif // NUMERICS_BOUNDS_CHECK
                        if (e1 (n, n) == T1 (0))
                            throw singular ();
                        for (size_type l = 0; l < size; ++ l) {
                            e2 (n, l) /= e1 (n, n);
                            for (size_type m = n + 1; m < size; ++ m) 
                                e2 (m, l) -= e1 (m, n) * e2 (n, l);
                        }
                    }
                } else if (hint == upper_triangular) {
                    size_type size = common (e2.size1 (), e2.size2 ());
                    for (size_type l = size - 1; (signed) l >= 0; -- l) 
						for (size_type m = size - 1; signed (m) >= 0; -- m) 
							e2 (l, m) = e3 (l, m);
                    for (size_type n = size - 1; (signed) n >= 0; -- n) {
#ifdef NUMERICS_BOUNDS_CHECK
                        if (check<non_triangular>::enabled ())
                            for (size_type m = size - 1; m > n; -- m) 
                                check<non_triangular>::precondition (e1 (m, n) == T1 (0));
#endif // NUMERICS_BOUNDS_CHECK
                        if (e1 (n, n) == T1 (0))
                            throw singular ();
                        for (size_type l = size - 1; (signed) l >= 0; -- l) {
                            e2 (n, l) /= e1 (n, n);
                            for (size_type m = n - 1; (signed) m >= 0; -- m) 
                                e2 (m, l) -= e1 (m, n) * e2 (n, l);
                        }
                    }
                } else {
                    throw non_triangular ();
                }
                first_ = false;
            }
            return e2 (i, j); 
        }
    };

}

#endif // NUMERICS_FUNCTIONAL_H