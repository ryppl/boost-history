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

#ifndef NUMERICS_MATRIX_H
#define NUMERICS_MATRIX_H

#include "config.h"
#include "storage.h"
#include "vector_et.h"
#include "matrix_et.h"

// Iterators based on ideas of Jeremy Siek

namespace numerics {

    struct major_tag {};

    struct row_major_tag /* : public major_tag */ {};

    // This functor computes the address translation 
    // matrix [i] [j] -> storage [F1 [i] * size2 + F2 [j]]
    template<class F1, class F2>
    struct row_major {
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef F1 functor1_type;
        typedef F2 functor2_type;
        typedef row_major_tag orientation_category;

        // Indexing
        static 
        NUMERICS_INLINE
        size_type element (size_type i, size_type size1, size_type j, size_type size2) {
            i = functor1_type::element (i, size1);
            check<bad_index>::precondition (i <= size1);
            j = functor2_type::element (j, size2);
            check<bad_index>::precondition (j <= size2);
            return i * size2 + j;
        }
        static 
        NUMERICS_INLINE
        difference_type distance1 (difference_type k, size_type size1, size_type size2) {
            return functor1_type::index (size2 != 0 ? k / size2 : 0, size1);
        }
        static 
        NUMERICS_INLINE
        difference_type distance2 (difference_type k, size_type size1, size_type size2) {
            return functor2_type::index (k, size2);
        }
        static 
        NUMERICS_INLINE
        size_type index1 (difference_type k, size_type size1, size_type size2) {
            return functor1_type::index (size2 != 0 ? k / size2 : 0, size1);
        }
        static 
        NUMERICS_INLINE
        size_type index2 (difference_type k, size_type size1, size_type size2) {
            return functor2_type::index (size2 != 0 ? k % size2 : 0, size2);
        }
        static 
        NUMERICS_INLINE
        difference_type one1 (size_type size1, size_type size2) {
            return functor1_type::one () * size2;
        }
        static 
        NUMERICS_INLINE
        difference_type one2 (size_type size1, size_type size2) {
            return functor2_type::one ();
        }

        // Iterating
        template<class I>
        static 
        NUMERICS_INLINE
        void increment1 (I &it, size_type size1, size_type size2) {
            it += functor1_type::one () * size2;
        }
        template<class I>
        static 
        NUMERICS_INLINE
        void decrement1 (I &it, size_type size1, size_type size2) {
            it -= functor1_type::one () * size2;
        }
        template<class I>
        static 
        NUMERICS_INLINE
        void increment2 (I &it, size_type size1, size_type size2) {
            functor2_type::increment (it);
        }
        template<class I>
        static 
        NUMERICS_INLINE
        void decrement2 (I &it, size_type size1, size_type size2) {
            functor2_type::decrement (it);
        }
    };

    struct column_major_tag /* : public major_tag */ {};

    // This functor computes the address translation 
    // matrix [i] [j] -> storage [F1 [i] + F2 [j] * size1]
    template<class F1, class F2>
    struct column_major {
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef F1 functor1_type;
        typedef F2 functor2_type;
        typedef column_major_tag orientation_category;

        // Indexing
        static 
        NUMERICS_INLINE
        size_type element (size_type i, size_type size1, size_type j, size_type size2) {
            i = functor1_type::element (i, size1);
            check<bad_index>::precondition (i <= size1);
            j = functor2_type::element (j, size2);
            check<bad_index>::precondition (j <= size2);
            return i + j * size1;
        }
        static 
        NUMERICS_INLINE
        difference_type distance1 (difference_type k, size_type size1, size_type size2) {
            return functor1_type::index (k, size1);
        }
        static 
        NUMERICS_INLINE
        difference_type distance2 (difference_type k, size_type size1, size_type size2) {
            return functor2_type::index (size1 != 0 ? k / size1 : 0, size2);
        }
        static 
        NUMERICS_INLINE
        size_type index1 (difference_type k, size_type size1, size_type size2) {
            return functor1_type::index (size1 != 0 ? k % size1 : 0, size1);
        }
        static 
        NUMERICS_INLINE
        size_type index2 (difference_type k, size_type size1, size_type size2) {
            return functor2_type::index (size1 != 0 ? k / size1 : 0, size2);
        }
        static 
        NUMERICS_INLINE
        difference_type one1 (size_type size1, size_type size2) {
            return functor1_type::one ();
        }
        static 
        NUMERICS_INLINE
        difference_type one2 (size_type size1, size_type size2) {
            return functor2_type::one () * size1;
        }

        // Iterating
        template<class I>
        static 
        NUMERICS_INLINE
        void increment1 (I &it, size_type size1, size_type size2) {
            functor1_type::increment (it);
        }
        template<class I>
        static 
        NUMERICS_INLINE
        void decrement1 (I &it, size_type size1, size_type size2) {
            functor1_type::decrement (it);
        }
        template<class I>
        static 
        NUMERICS_INLINE
        void increment2 (I &it, size_type size1, size_type size2) {
            it += functor2_type::one () * size1;
        }
        template<class I>
        static 
        NUMERICS_INLINE
        void decrement2 (I &it, size_type size1, size_type size2) {
            it -= functor2_type::one () * size1;
        }
    };

    struct sparse_proxy_row_major_tag {};
    struct sparse_proxy_column_major_tag {};
    struct sparse_row_major_tag: public sparse_proxy_row_major_tag {};
    struct sparse_column_major_tag: public sparse_proxy_column_major_tag {};

    template<class S, class O> 
    struct storage_orientation_traits {
        typedef S storage_orientation_category;
    };

    template<> 
    struct storage_orientation_traits<sparse_tag, row_major_tag> {
        typedef sparse_row_major_tag storage_orientation_category;
    };
    template<> 
    struct storage_orientation_traits<sparse_tag, column_major_tag> {
        typedef sparse_column_major_tag storage_orientation_category;
    };
    template<> 
    struct storage_orientation_traits<sparse_proxy_tag, row_major_tag> {
        typedef sparse_proxy_row_major_tag storage_orientation_category;
    };
    template<> 
    struct storage_orientation_traits<sparse_proxy_tag, column_major_tag> {
        typedef sparse_proxy_column_major_tag storage_orientation_category;
    };

#ifndef NUMERICS_USE_ITERATOR
    // Indexing version for the computation of 
    // matrix assignment_operation scalar
    template<class F>
    struct matrix_assign_scalar {
        typedef F functor_type;
        typedef typename F::assign_category assign_category;

        template<class M, class T>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (M &m, const T &t) {
            typedef typename M::size_type size_type;
            size_type size1 (m.size1 ());
            for (size_type i = 0; i < size1; ++ i) {
                size_type size2 (m.size2 ());
                for (size_type j = 0; j < size2; ++ j)
                    functor_type () (m (i, j), t);
            }
        }
    };
#else
    template<class LSO, class A> 
    struct matrix_assign_scalar_traits {
        typedef LSO dispatch_category;
    };

    // Iterating version for the computation of 
    // matrix assignment_operation scalar
    template<class F>
    struct matrix_assign_scalar {
        typedef F functor_type;
        typedef typename F::assign_category assign_category;

        // Dense case
        template<class M, class T>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (M &m, const T &t, dense_tag) {
            typedef typename M::difference_type difference_type;
            typename M::iterator1 it1 (m.begin1 ());
            difference_type size1 (m.end1 () - m.begin1 ());
            while (-- size1 >= 0) {
                typename M::iterator2 it2 (it1.begin ());
                difference_type size2 (it1.end () - it1.begin ()); 
                while (-- size2 >= 0) 
                    functor_type () (*it2, t), ++ it2;
                ++ it1;
            }
        }
        // Sparse (proxy) row major case
        template<class M, class T>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (M &m, const T &t, sparse_proxy_row_major_tag) {
            typename M::iterator1 it1 (m.begin1 ());
            typename M::iterator1 it1_end (m.end1 ());
            while (it1 != it1_end) {
                typename M::iterator2 it2 (it1.begin ());
                typename M::iterator2 it2_end (it1.end ());
                while (it2 != it2_end) 
                    functor_type () (*it2, t), ++ it2;
                ++ it1;
            }
        }
        // Sparse column major case
        template<class M, class T>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (M &m, const T &t, sparse_proxy_column_major_tag) {
            typename M::iterator2 it2 (m.begin2 ());
            typename M::iterator2 it2_end (m.end2 ());
            while (it2 != it2_end) {
                typename M::iterator1 it1 (it2.begin ());
                typename M::iterator1 it1_end (it2.end ());
                while (it1 != it1_end) 
                    functor_type () (*it1, t), ++ it1;
                ++ it2;
            }
        }

        // Dispatcher
        template<class M, class T>
        NUMERICS_INLINE
        void operator () (M &m, const T &t) {
            typedef typename storage_orientation_traits<NUMERICS_TYPENAME M::storage_category, 
                                                        NUMERICS_TYPENAME M::orientation_category>::storage_orientation_category storage_orientation_category;
            typedef typename matrix_assign_scalar_traits<storage_orientation_category,
                                                         assign_category>::dispatch_category dispatch_category;
            operator () (m, t, dispatch_category ());
        }
    };
#endif

#ifndef NUMERICS_USE_ITERATOR
    // Indexing version for the computation of 
    // matrix assignment_operation matrix_expression
    template<class F>
    struct matrix_assign {
        typedef F functor_type;
        typedef typename F::assign_category assign_category;

        template<class M, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (M &m, const matrix_expression<E> &e) {
            typedef typename M::size_type size_type;
            size_type size1 (common (m.size1 (), e ().size1 ()));
            for (size_type i = 0; i < size1; ++ i) {
                size_type size2 (common (m.size2 (), e ().size2 ()));
                for (size_type j = 0; j < size2; ++ j)
                    functor_type () (m (i, j), e () (i, j));
            }
        }
    };

    // Indexing version for the computation of 
    // matrix swap_operation matrix_expression
    template<class F>
    struct matrix_swap {
        typedef F functor_type;

        template<class M, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (M &m, matrix_expression<E> &e) {
            typedef typename M::size_type size_type;
            size_type size1 (common (m.size1 (), e ().size1 ()));
            for (size_type i = 0; i < size1; ++ i) {
                size_type size2 (common (m.size2 (), e ().size2 ()));
                for (size_type j = 0; j < size2; ++ j)
                    functor_type () (m (i, j), e () (i, j));
            }
        }
    };
#else
    template<class LSO, class A, class RO, class RI1, class RI2> 
    struct matrix_assign_traits {
        typedef LSO dispatch_category;
    };

    template<>         
    struct matrix_assign_traits<dense_tag, assign_tag, major_tag, std::bidirectional_iterator_tag, std::bidirectional_iterator_tag> {
        typedef sparse_row_major_tag dispatch_category;
    };
    template<>         
    struct matrix_assign_traits<dense_tag, computed_assign_tag, major_tag, std::bidirectional_iterator_tag, std::bidirectional_iterator_tag> {
        typedef sparse_row_major_tag dispatch_category;
    };
    template<>         
    struct matrix_assign_traits<dense_tag, assign_tag, row_major_tag, std::bidirectional_iterator_tag, std::bidirectional_iterator_tag> {
        typedef sparse_row_major_tag dispatch_category;
    };
    template<>         
    struct matrix_assign_traits<dense_tag, computed_assign_tag, row_major_tag, std::bidirectional_iterator_tag, std::bidirectional_iterator_tag> {
        typedef sparse_row_major_tag dispatch_category;
    };
    template<>         
    struct matrix_assign_traits<dense_tag, assign_tag, column_major_tag, std::bidirectional_iterator_tag, std::bidirectional_iterator_tag> {
        typedef sparse_column_major_tag dispatch_category;
    };
    template<>         
    struct matrix_assign_traits<dense_tag, computed_assign_tag, column_major_tag, std::bidirectional_iterator_tag, std::bidirectional_iterator_tag> {
        typedef sparse_column_major_tag dispatch_category;
    };

    // Iterating version for the computation of 
    // matrix assignment_operation matrix_expression
    template<class F>
    struct matrix_assign {
        typedef F functor_type;
        typedef typename F::assign_category assign_category;

        // Dense case
        template<class M, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (M &m, const matrix_expression<E> &e, dense_tag) {
            typedef typename M::difference_type difference_type;
            typename M::iterator1 it1 (m.begin1 ());
            typename E::const_iterator1 it1e (e ().begin1 ());
            difference_type size1 (common (m.end1 () - it1, e ().end1 () - it1e));
            while (-- size1 >= 0) {
                typename M::iterator2 it2 (it1.begin ());
                typename E::const_iterator2 it2e (it1e.begin ());
                difference_type size2 (common (it1.end () - it2, it1e.end () - it2e));
                while (-- size2 >= 0) 
                    functor_type () (*it2, *it2e), ++ it2, ++ it2e;
                ++ it1, ++ it1e;
            }
        }
        // Sparse row major case
        template<class M, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (M &m, const matrix_expression<E> &e, sparse_row_major_tag) {
            check<bad_size>::precondition (m.size1 () == e ().size1 ());
            check<bad_size>::precondition (m.size2 () == e ().size2 ());
            m.clear ();
            typename E::const_iterator1 it1e (e ().begin1 ());
            typename E::const_iterator1 it1e_end (e ().end1 ());
            while (it1e != it1e_end) {
                typename E::const_iterator2 it2e (it1e.begin ());
                typename E::const_iterator2 it2e_end (it1e.end ());
                while (it2e != it2e_end) 
                    m.insert (it2e.index1 (), it2e.index2 (), *it2e), ++ it2e;
                ++ it1e;
            }
        }
        // Sparse column major case
        template<class M, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (M &m, const matrix_expression<E> &e, sparse_column_major_tag) {
            check<bad_size>::precondition (m.size1 () == e ().size1 ());
            check<bad_size>::precondition (m.size2 () == e ().size2 ());
            m.clear ();
            typename E::const_iterator2 it2e (e ().begin2 ());
            typename E::const_iterator2 it2e_end (e ().end2 ());
            while (it2e != it2e_end) {
                typename E::const_iterator1 it1e (it2e.begin ());
                typename E::const_iterator1 it1e_end (it2e.end ());
                while (it1e != it1e_end) 
                    m.insert (it1e.index1 (), it1e.index2 (), *it1e), ++ it1e;
                ++ it2e;
            }
        }
        // Sparse proxy row major case
        template<class M, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (M &m, const matrix_expression<E> &e, sparse_proxy_row_major_tag) {
            check<bad_size>::precondition (m.size1 () == e ().size1 ());
            check<bad_size>::precondition (m.size2 () == e ().size2 ());
            typedef typename M::value_type value_type;
            typename M::iterator1 it1 (m.begin1 ());
            typename M::iterator1 it1_end (m.end1 ());
            typename E::const_iterator1 it1e (e ().begin1 ());
            typename E::const_iterator1 it1e_end (e ().end1 ());
            while (it1e != it1e_end) {
                check<bad_index>::invariant (it1 == it1_end || it1.index1 () >= it1e.index1 ());
                if (it1 != it1_end && it1.index1 () == it1e.index1 ()) {
                    typename M::iterator2 it2 (it1.begin ());
                    typename M::iterator2 it2_end (it1.end ());
                    typename E::const_iterator2 it2e (it1e.begin ());
                    typename E::const_iterator2 it2e_end (it1e.end ());
                    while (it2e != it2e_end) {
                        check<bad_index>::invariant (it2 == it2_end || it2.index2 () >= it2e.index2 ());
                        if (it2 != it2_end && it2.index2 () == it2e.index2 ()) 
                            functor_type () (*it2, *it2e), ++ it2;
                        else
                            check<bad_index>::invariant (*it2e == value_type (0));
                        ++ it2e;
                    }
                    check<bad_size>::postcondition (it2 == it2_end);
                    check<bad_size>::postcondition (it2e == it2e_end);
                    ++ it1;
                } else {
                    typename E::const_iterator2 it2e (it1e.begin ());
                    typename E::const_iterator2 it2e_end (it1e.end ());
                    while (it2e != it2e_end) {
                        check<bad_index>::invariant (*it2e == value_type (0));
                        ++ it2e;
                    }
                    check<bad_size>::postcondition (it2e == it2e_end);
                }
                ++ it1e;
            }
            check<bad_size>::postcondition (it1 == it1_end);
            check<bad_size>::postcondition (it1e == it1e_end);
        }
        // Sparse proxy column major case
        template<class M, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (M &m, const matrix_expression<E> &e, sparse_proxy_column_major_tag) {
            check<bad_size>::precondition (m.size1 () == e ().size1 ());
            check<bad_size>::precondition (m.size2 () == e ().size2 ());
            typedef typename M::value_type value_type;
            typename M::iterator2 it2 (m.begin2 ());
            typename M::iterator2 it2_end (m.end2 ());
            typename E::const_iterator2 it2e (e ().begin2 ());
            typename E::const_iterator2 it2e_end (e ().end2 ());
            while (it2e != it2e_end) {
                check<bad_index>::invariant (it2 == it2_end || it2.index2 () >= it2e.index2 ());
                if (it2 != it2_end && it2.index2 () == it2e.index2 ()) {
                    typename M::iterator1 it1 (it2.begin ());
                    typename M::iterator1 it1_end (it2.end ());
                    typename E::const_iterator1 it1e (it2e.begin ());
                    typename E::const_iterator1 it1e_end (it2e.end ());
                    while (it1e != it1e_end) {
                        check<bad_index>::invariant (it1 == it1_end || it1.index1 () >= it1e.index1 ());
                        if (it1 != it1_end && it1.index1 () == it1e.index1 ()) 
                            functor_type () (*it1, *it1e), ++ it1;
                        else
                            check<bad_index>::invariant (*it1e == value_type (0));
                        ++ it1e;
                    }
                    check<bad_size>::postcondition (it1 == it1_end);
                    check<bad_size>::postcondition (it1e == it1e_end);
                    ++ it2;
                } else {
                    typename E::const_iterator1 it1e (it2e.begin ());
                    typename E::const_iterator1 it1e_end (it2e.end ());
                    while (it1e != it1e_end) {
                        check<bad_index>::invariant (*it1e == value_type (0));
                        ++ it1e;
                    }
                    check<bad_size>::postcondition (it1e == it1e_end);
                }
                ++ it2e;
            }
            check<bad_size>::postcondition (it2 == it2_end);
            check<bad_size>::postcondition (it2e == it2e_end);
        }

        // Dispatcher
        template<class M, class E>
        NUMERICS_INLINE
        void operator () (M &m, const matrix_expression<E> &e) {
            typedef typename storage_orientation_traits<NUMERICS_TYPENAME M::storage_category, 
                                                        NUMERICS_TYPENAME M::orientation_category>::storage_orientation_category storage_orientation_category;
            typedef typename matrix_assign_traits<storage_orientation_category, 
                                                  assign_category, 
                                                  NUMERICS_TYPENAME E::orientation_category, 
                                                  NUMERICS_TYPENAME E::const_iterator1::iterator_category,
                                                  NUMERICS_TYPENAME E::const_iterator2::iterator_category>::dispatch_category dispatch_category;
            operator () (m, e, dispatch_category ());
        }
    };

    template<class LSO, class RO, class RI1, class RI2> 
    struct matrix_swap_traits {
        typedef LSO dispatch_category;
    };

    template<>         
    struct matrix_swap_traits<dense_tag, major_tag, std::bidirectional_iterator_tag, std::bidirectional_iterator_tag> {
        typedef sparse_row_major_tag dispatch_category;
    };
    template<>         
    struct matrix_swap_traits<dense_tag, row_major_tag, std::bidirectional_iterator_tag, std::bidirectional_iterator_tag> {
        typedef sparse_row_major_tag dispatch_category;
    };
    template<>         
    struct matrix_swap_traits<dense_tag, column_major_tag, std::bidirectional_iterator_tag, std::bidirectional_iterator_tag> {
        typedef sparse_column_major_tag dispatch_category;
    };

    // Iterating version for the computation of 
    // matrix swap_operation matrix_expression
    template<class F>
    struct matrix_swap {
        typedef F functor_type;

        // Dense case
        template<class M, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (M &m, matrix_expression<E> &e, dense_tag) {
            typedef typename M::difference_type difference_type;
            typename M::iterator1 it1 (m.begin1 ());
            typename E::iterator1 it1e (e ().begin1 ());
            difference_type size1 (common (m.end1 () - it1, e ().end1 () - it1e));
            while (-- size1 >= 0) {
                typename M::iterator2 it2 (it1.begin ());
                typename E::iterator2 it2e (it1e.begin ());
                difference_type size2 (common (it1.end () - it2, it1e.end () - it2e));
                while (-- size2 >= 0) 
                    functor_type () (*it2, *it2e), ++ it2, ++ it2e;
                ++ it1, ++ it1e;
            }
        }
        // Sparse (proxy) row major case
        template<class M, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (M &m, matrix_expression<E> &e, sparse_proxy_row_major_tag) {
            check<bad_size>::precondition (m.size1 () == e ().size1 ());
            check<bad_size>::precondition (m.size2 () == e ().size2 ());
            typename M::iterator1 it1 (m.begin1 ());
            typename M::iterator1 it1_end (m.end1 ());
            typename E::iterator1 it1e (e ().begin1 ());
            typename E::iterator1 it1e_end (e ().end1 ());
            while (it1 != it1_end && it1e != it1e_end) {
                check<bad_index>::precondition (it1.index1 () == it1e.index1 ());
                typename M::iterator2 it2 (it1.begin ());
                typename M::iterator2 it2_end (it1.end ());
                typename E::iterator2 it2e (it1e.begin ());
                typename E::iterator2 it2e_end (it1e.end ());
                while (it2 != it2_end && it2e != it2e_end) {
                    check<bad_index>::precondition (it2.index2 () == it2e.index2 ());
                    functor_type () (*it2, *it2e), ++ it2, ++ it2e;
                }
                check<bad_size>::postcondition (it2 == it2_end && it2e == it2e_end);
                ++ it1, ++ it1e;
            }
            check<bad_size>::postcondition (it1 == it1_end && it1e == it1e_end);
        }
        // Sparse (proxy) column major case
        template<class M, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (M &m, matrix_expression<E> &e, sparse_proxy_column_major_tag) {
            check<bad_size>::precondition (m.size1 () == e ().size1 ());
            check<bad_size>::precondition (m.size2 () == e ().size2 ());
            typename M::iterator2 it2 (m.begin2 ());
            typename M::iterator2 it2_end (m.end2 ());
            typename E::iterator2 it2e (e ().begin2 ());
            typename E::iterator2 it2e_end (e ().end2 ());
            while (it2 != it2_end && it2e != it2e_end) {
                check<bad_index>::precondition (it2.index2 () == it2e.index2 ());
                typename M::iterator1 it1 (it2.begin ());
                typename M::iterator1 it1_end (it2.end ());
                typename E::iterator1 it1e (it2e.begin ());
                typename E::iterator1 it1e_end (it2e.end ());
                while (it1 != it1_end && it1e != it1e_end) {
                    check<bad_index>::precondition (it1.index1 () == it1e.index1 ());
                    functor_type () (*it1, *it1e), ++ it1, ++ it1e;
                }
                check<bad_size>::postcondition (it1 == it1_end && it1e == it1e_end);
                ++ it2, ++ it2e;
            }
            check<bad_size>::postcondition (it2 == it2_end && it2e == it2e_end);
        }

        // Dispatcher
        template<class M, class E>
        NUMERICS_INLINE
        void operator () (M &m, matrix_expression<E> &e) {
            typedef typename storage_orientation_traits<NUMERICS_TYPENAME M::storage_category, 
                                                        NUMERICS_TYPENAME M::orientation_category>::storage_orientation_category storage_orientation_category;
            typedef typename matrix_swap_traits<storage_orientation_category, 
                                                NUMERICS_TYPENAME E::orientation_category, 
                                                NUMERICS_TYPENAME E::const_iterator1::iterator_category,
                                                NUMERICS_TYPENAME E::const_iterator2::iterator_category>::dispatch_category dispatch_category;
            operator () (m, e, dispatch_category ());
        }
    };
#endif

    template<class E> 
    class matrix_const_reference;
    template<class E> 
    class matrix_reference;

    // Array based matrix class 
    template<class T, class A, class F>
    class matrix: 
        public matrix_expression<matrix<T, A, F> > {
    public:      
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef const T &const_reference_type;
        typedef T &reference_type;
        typedef A array_type;
        typedef F functor_type;
        typedef matrix<T, A, F> self_type;
        typedef matrix_const_reference<self_type> const_closure_type;
        typedef matrix_reference<self_type> closure_type;
        typedef matrix_row<self_type> matrix_row_type;
        typedef matrix_column<self_type> matrix_column_type;
        typedef matrix_range<self_type> matrix_range_type;
        typedef typename A::const_iterator const_iterator_type;
        typedef typename A::iterator iterator_type;
        typedef struct dense_tag storage_category;
        typedef struct major_tag orientation_category;

        // Construction and destruction
        NUMERICS_INLINE
        matrix (): 
            size1_ (0), size2_ (0), data_ (0) {}
        NUMERICS_INLINE
        matrix (size_type size1, size_type size2): 
            size1_ (size1), size2_ (size2), data_ (size1 * size2) {}
        NUMERICS_INLINE
        matrix (const matrix &m): 
            size1_ (m.size1_), size2_ (m.size2_), data_ (m.data_) {}
        template<class AE>
        NUMERICS_EXPLICIT NUMERICS_INLINE
        matrix (const matrix_expression<AE> &ae): 
            size1_ (ae ().size1 ()), size2_ (ae ().size2 ()), data_ (ae ().size1 () * ae ().size2 ()) { 
            matrix_assign<scalar_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
        }

        // Resizing
        NUMERICS_INLINE
        void resize (size_type size1, size_type size2) {
            size1_ = size1;
            size2_ = size2;
            data_.resize (size1 * size2);
        }

        NUMERICS_INLINE
        size_type size1 () const { 
            return size1_;
        }
        NUMERICS_INLINE
        size_type size2 () const { 
            return size2_;
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i, size_type j) const {
            return data_ [functor_type::element (i, size1_, j, size2_)]; 
        }
        NUMERICS_INLINE
        reference_type operator () (size_type i, size_type j) {
            return data_ [functor_type::element (i, size1_, j, size2_)]; 
        }

        NUMERICS_INLINE
        matrix_row_type operator [] (size_type i) {
            return matrix_row_type (*this, i);
        }
        NUMERICS_INLINE
        matrix_row_type row (size_type i) {
            return matrix_row_type (*this, i);
        }
        NUMERICS_INLINE
        matrix_column_type column (size_type j) {
            return matrix_column_type (*this, j);
        }

        NUMERICS_INLINE
        matrix_range_type project (size_type start1, size_type stop1, size_type start2, size_type stop2) {
            return matrix_range_type (*this, start1, stop1, start2, stop2);
        }
        NUMERICS_INLINE
        matrix_range_type project (const range &r1, const range &r2) {
            return matrix_range_type (*this, r1, r2);
        }

        // Assignment
        NUMERICS_INLINE
        matrix &operator = (const matrix &m) { 
            check<bad_size>::precondition (size1_ == m.size1_);
            check<bad_size>::precondition (size2_ == m.size2_);
            data_ = m.data_;
            return *this;
        }
        NUMERICS_INLINE
        matrix &assign_temporary (matrix &m) { 
            swap (m);
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix &operator = (const matrix_expression<AE> &ae) { 
#ifndef USE_GCC
            return assign_temporary (self_type (ae));
#else
            return assign (self_type (ae));
#endif
        }
        template<class AE>
        NUMERICS_INLINE
        matrix &assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix& operator += (const matrix_expression<AE> &ae) {
#ifndef USE_GCC
            return assign_temporary (self_type (*this + ae));
#else
            return assign (self_type (*this + ae));
#endif
        }
        template<class AE>
        NUMERICS_INLINE
        matrix &plus_assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_plus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix& operator -= (const matrix_expression<AE> &ae) {
#ifndef USE_GCC
            return assign_temporary (self_type (*this - ae));
#else
            return assign (self_type (*this - ae));
#endif
        }
        template<class AE>
        NUMERICS_INLINE
        matrix &minus_assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_minus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        matrix& operator *= (const AT &at) {
            matrix_assign_scalar<scalar_multiplies_assign<value_type, AT> > () (*this, at);
            return *this;
        }

        // Swapping
        NUMERICS_INLINE
	    void swap (matrix &m) {
            check<external_logic>::precondition (this != &m);
            check<bad_size>::precondition (size1_ == m.size1_);
            check<bad_size>::precondition (size2_ == m.size2_);
            std::swap (size1_, m.size1_);
            std::swap (size2_, m.size2_);
            data_.swap (m.data_);
        }
#ifndef USE_GCC
        NUMERICS_INLINE
	    friend void swap (matrix &m1, matrix &m2) {
            m1.swap (m2);
        }
#endif

        // Element insertion
        NUMERICS_INLINE
        void clear () {
            data_.clear ();
        }
        NUMERICS_INLINE
        void insert (size_type i, size_type j, const T &t) {
            data_.insert (data_.begin () + functor_type::element (i, size1_, j, size2_), t);
        }

        class const_iterator1;
        class iterator1;
        class const_iterator2;
        class iterator2;

        // Element lookup
        NUMERICS_INLINE
        const_iterator1 find1 (size_type i, size_type j) const {
            return const_iterator1 (*this, data_.begin () + functor_type::element (i, size1_, j, size2_));
        }
        NUMERICS_INLINE
        iterator1 find1 (size_type i, size_type j) {
            return iterator1 (*this, data_.begin () + functor_type::element (i, size1_, j, size2_));
        }
        NUMERICS_INLINE
        const_iterator2 find2 (size_type i, size_type j) const {
            return const_iterator2 (*this, data_.begin () + functor_type::element (i, size1_, j, size2_));
        }
        NUMERICS_INLINE
        iterator2 find2 (size_type i, size_type j) {
            return iterator2 (*this, data_.begin () + functor_type::element (i, size1_, j, size2_));
        }

        // Iterators simply are pointers.

        class const_iterator1:
            public container_const_reference<matrix>,
            public random_access_iterator_base<const_iterator1, value_type> {
        public:
            typedef std::random_access_iterator_tag iterator_category;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator1 (const matrix &m, const const_iterator_type &it):
                container_const_reference<matrix> (m), it_ (it) {}
            NUMERICS_INLINE
            const_iterator1 (const iterator1 &it):
                container_const_reference<matrix> (it ()), it_ (it.it_) {}

            // Arithmetic
            NUMERICS_INLINE
            const_iterator1 &operator ++ () {
                functor_type::increment1 (it_, (*this) ().size1 (), (*this) ().size2 ());
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator -- () {
                functor_type::decrement1 (it_, (*this) ().size1 (), (*this) ().size2 ());
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator += (difference_type n) {
                it_ += n * functor_type::one1 ((*this) ().size1 (), (*this) ().size2 ());
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator -= (difference_type n) {
                it_ -= n * functor_type::one1 ((*this) ().size1 (), (*this) ().size2 ());
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const const_iterator1 &it) const {
                return functor_type::distance1 (it_ - it.it_, (*this) ().size1 (), (*this) ().size2 ());
            }

            // Dereference
            NUMERICS_INLINE
            value_type operator * () const {
                check<bad_index>::precondition (index1 () < (*this) ().size1 ());
                check<bad_index>::precondition (index2 () < (*this) ().size2 ());
                return *it_;
            }

            NUMERICS_INLINE
            const_iterator2 begin () const {
                const matrix &m = (*this) ();
                return m.find2 (index1 (), 0); 
            }
            NUMERICS_INLINE
            const_iterator2 end () const {
                const matrix &m = (*this) ();
                return m.find2 (index1 (), m.size2 ()); 
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                const matrix &m = (*this) ();
                return functor_type::index1 (it_ - m.begin1 ().it_, m.size1 (), m.size2 ());
            }
            NUMERICS_INLINE
            size_type index2 () const {
                const matrix &m = (*this) ();
                return functor_type::index2 (it_ - m.begin1 ().it_, m.size1 (), m.size2 ());
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator1 &it) const {
                check<external_logic>::precondition (&(*this) () == &it ());
                return it_ == it.it_;
            }

        private:
            const_iterator_type it_;

            friend class iterator1;
        };

        NUMERICS_INLINE
        const_iterator1 begin1 () const {
            return find1 (0, 0);
        }
        NUMERICS_INLINE
        const_iterator1 end1 () const {
            return find1 (size1_, 0);
        }

        class iterator1:
            public container_reference<matrix>,
            public random_access_iterator_base<iterator1, value_type> {
        public:
            typedef std::random_access_iterator_tag iterator_category;

            // Construction and destruction
            NUMERICS_INLINE
            iterator1 (matrix &m, const iterator_type &it):
                container_reference<matrix> (m), it_ (it) {}

            // Arithmetic
            NUMERICS_INLINE
            iterator1 &operator ++ () {
                functor_type::increment1 (it_, (*this) ().size1 (), (*this) ().size2 ());
                return *this;
            }
            NUMERICS_INLINE
            iterator1 &operator -- () {
                functor_type::decrement1 (it_, (*this) ().size1 (), (*this) ().size2 ());
                return *this;
            }
            NUMERICS_INLINE
            iterator1 &operator += (difference_type n) {
                it_ += n * functor_type::one1 ((*this) ().size1 (), (*this) ().size2 ());
                return *this;
            }
            NUMERICS_INLINE
            iterator1 &operator -= (difference_type n) {
                it_ -= n * functor_type::one1 ((*this) ().size1 (), (*this) ().size2 ());
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const iterator1 &it) const {
                return functor_type::distance1 (it_ - it.it_, (*this) ().size1 (), (*this) ().size2 ());
            }

            // Dereference
            NUMERICS_INLINE
            reference_type operator * () {
                check<bad_index>::precondition (index1 () < (*this) ().size1 ());
                check<bad_index>::precondition (index2 () < (*this) ().size2 ());
                return *it_;
            }
            
            NUMERICS_INLINE
            iterator2 begin () {
                matrix &m = (*this) ();
                return m.find2 (index1 (), 0);
            }
            NUMERICS_INLINE
            iterator2 end () {
                matrix &m = (*this) ();
                return m.find2 (index1 (), m.size2 ());
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                const matrix &m = (*this) ();
                return functor_type::index1 (it_ - m.begin1 ().it_, m.size1 (), m.size2 ());
            }
            NUMERICS_INLINE
            size_type index2 () const {
                const matrix &m = (*this) ();
                return functor_type::index2 (it_ - m.begin1 ().it_, m.size1 (), m.size2 ());
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const iterator1 &it) const {
                check<external_logic>::precondition (&(*this) () == &it ());
                return it_ == it.it_;
            }

        private:
            iterator_type it_;

            friend class const_iterator1;
        };

        NUMERICS_INLINE
        iterator1 begin1 () {
            return find1 (0, 0);
        }
        NUMERICS_INLINE
        iterator1 end1 () {
            return find1 (size1_, 0);
        }

        class const_iterator2:
            public container_const_reference<matrix>,
            public random_access_iterator_base<const_iterator2, value_type> {
        public:
            typedef std::random_access_iterator_tag iterator_category;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator2 (const matrix &m, const const_iterator_type &it):
                container_const_reference<matrix> (m), it_ (it) {}
            NUMERICS_INLINE
            const_iterator2 (const iterator2 &it):
                container_const_reference<matrix> (it ()), it_ (it.it_) {}

            // Arithmetic
            NUMERICS_INLINE
            const_iterator2 &operator ++ () {
                functor_type::increment2 (it_, (*this) ().size1 (), (*this) ().size2 ());
                return *this;
            }
            NUMERICS_INLINE
            const_iterator2 &operator -- () {
                functor_type::decrement2 (it_, (*this) ().size1 (), (*this) ().size2 ());
                return *this;
            }
            NUMERICS_INLINE
            const_iterator2 &operator += (difference_type n) {
                it_ += n * functor_type::one2 ((*this) ().size1 (), (*this) ().size2 ());
                return *this;
            }
            NUMERICS_INLINE
            const_iterator2 &operator -= (difference_type n) {
                it_ -= n * functor_type::one2 ((*this) ().size1 (), (*this) ().size2 ());
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const const_iterator2 &it) const {
                return functor_type::distance2 (it_ - it.it_, (*this) ().size1 (), (*this) ().size2 ());
            }

            // Dereference
            NUMERICS_INLINE
            value_type operator * () const {
                check<bad_index>::precondition (index1 () < (*this) ().size1 ());
                check<bad_index>::precondition (index2 () < (*this) ().size2 ());
                return *it_;
            }

            NUMERICS_INLINE
            const_iterator1 begin () const {
                const matrix &m = (*this) ();
                return m.find1 (0, index2 ()); 
            }
            NUMERICS_INLINE
            const_iterator1 end () const {
                const matrix &m = (*this) ();
                return m.find1 (m.size1 (), index2 ()); 
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                const matrix &m = (*this) ();
                return functor_type::index1 (it_ - m.begin2 ().it_, m.size1 (), m.size2 ());
            }
            NUMERICS_INLINE
            size_type  index2 () const {
                const matrix &m = (*this) ();
                return functor_type::index2 (it_ - m.begin2 ().it_, m.size1 (), m.size2 ());
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator2 &it) const {
                check<external_logic>::precondition (&(*this) () == &it ());
                return it_ == it.it_;
            }

        private:
            const_iterator_type it_;

            friend class iterator2;
        };

        NUMERICS_INLINE
        const_iterator2 begin2 () const {
            return find2 (0, 0);
        }
        NUMERICS_INLINE
        const_iterator2 end2 () const {
            return find2 (0, size2_);
        }

        class iterator2:
            public container_reference<matrix>,
            public random_access_iterator_base<iterator2, value_type> {
        public:
            typedef std::random_access_iterator_tag iterator_category;

            // Construction and destruction
            NUMERICS_INLINE
            iterator2 (matrix &m, const iterator_type &it):
                container_reference<matrix> (m), it_ (it) {}

            // Arithmetic
            NUMERICS_INLINE
            iterator2 &operator ++ () {
                functor_type::increment2 (it_, (*this) ().size1 (), (*this) ().size2 ());
                return *this;
            }
            NUMERICS_INLINE
            iterator2 &operator -- () {
                functor_type::decrement2 (it_, (*this) ().size1 (), (*this) ().size2 ());
                return *this;
            }
            NUMERICS_INLINE
            iterator2 &operator += (difference_type n) {
                it_ += n * functor_type::one2 ((*this) ().size1 (), (*this) ().size2 ());
                return *this;
            }
            NUMERICS_INLINE
            iterator2 &operator -= (difference_type n) {
                it_ -= n * functor_type::one2 ((*this) ().size1 (), (*this) ().size2 ());
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const iterator2 &it) const {
                return functor_type::distance2 (it_ - it.it_, (*this) ().size1 (), (*this) ().size2 ());
            }

            // Dereference
            NUMERICS_INLINE
            reference_type operator * () {
                check<bad_index>::precondition (index1 () < (*this) ().size1 ());
                check<bad_index>::precondition (index2 () < (*this) ().size2 ());
                return *it_;
            }

            NUMERICS_INLINE
            iterator1 begin () {
                matrix &m = (*this) ();
                return m.find1 (0, index2 ());
            }
            NUMERICS_INLINE
            iterator1 end () {
                matrix &m = (*this) ();
                return m.find1 (m.size1 (), index2 ()); 
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                const matrix &m = (*this) ();
                return functor_type::index1 (it_ - m.begin2 ().it_, m.size1 (), m.size2 ());
            }
            NUMERICS_INLINE
            size_type index2 () const {
                const matrix &m = (*this) ();
                return functor_type::index2 (it_ - m.begin2 ().it_, m.size1 (), m.size2 ());
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const iterator2 &it) const {
                check<external_logic>::precondition (&(*this) () == &it ());
                return it_ == it.it_;
            }

        private:
            iterator_type it_;

            friend class const_iterator2;
        };

        NUMERICS_INLINE
        iterator2 begin2 () {
            return find2 (0, 0);
        }
        NUMERICS_INLINE
        iterator2 end2 () {
            return find2 (0, size2_);
        }

        // Reverse iterators

#ifndef USE_GCC

#ifndef USE_BCC
        typedef std::reverse_iterator<const_iterator1, value_type, const_reference_type> const_reverse_iterator1;
#else
        typedef std::reverse_iterator<const_iterator1> const_reverse_iterator1;
#endif

        NUMERICS_INLINE
        const_reverse_iterator1 rbegin1 () const {
            return const_reverse_iterator1 (end1 ());
        }
        NUMERICS_INLINE
        const_reverse_iterator1 rend1 () const {
            return const_reverse_iterator1 (begin1 ());
        }

#ifndef USE_BCC
        typedef std::reverse_iterator<iterator1, value_type, reference_type> reverse_iterator1;
#else
        typedef std::reverse_iterator<iterator1> reverse_iterator1;
#endif

        NUMERICS_INLINE
        reverse_iterator1 rbegin1 () {
            return reverse_iterator1 (end1 ());
        }
        NUMERICS_INLINE
        reverse_iterator1 rend1 () {
            return reverse_iterator1 (begin1 ());
        }

#ifndef USE_BCC
        typedef std::reverse_iterator<const_iterator2, value_type, const_reference_type> const_reverse_iterator2;
#else
        typedef std::reverse_iterator<const_iterator2> const_reverse_iterator2;
#endif

        NUMERICS_INLINE
        const_reverse_iterator2 rbegin2 () const {
            return const_reverse_iterator2 (end2 ());
        }
        NUMERICS_INLINE
        const_reverse_iterator2 rend2 () const {
            return const_reverse_iterator2 (begin2 ());
        }

#ifndef USE_BCC
        typedef std::reverse_iterator<iterator2, value_type, reference_type> reverse_iterator2;
#else
        typedef std::reverse_iterator<iterator2> reverse_iterator2;
#endif

        NUMERICS_INLINE
        reverse_iterator2 rbegin2 () {
            return reverse_iterator2 (end2 ());
        }
        NUMERICS_INLINE
        reverse_iterator2 rend2 () {
            return reverse_iterator2 (begin2 ());
        }

#endif

    private:
        size_type size1_;
        size_type size2_;
        array_type data_;
    };

    // Matrix based row vector class
    template<class M>
    class matrix_row:
		public vector_expression<matrix_row<M> > {
    public:      
        typedef M matrix_type;
        typedef typename M::size_type size_type;
        typedef typename M::difference_type difference_type;
        typedef typename M::value_type value_type;
        typedef typename M::const_reference_type const_reference_type;
        typedef typename M::reference_type reference_type;
        typedef vector_const_reference<matrix_row<M> > const_closure_type;
        typedef vector_reference<matrix_row<M> > closure_type;
        typedef vector_range<matrix_row<M> > vector_range_type;
        typedef typename M::const_iterator2 const_iterator_type;
        typedef typename M::iterator2 iterator_type;
        typedef typename proxy_traits<typename M::storage_category>::storage_category storage_category;

        // Construction and destruction
        NUMERICS_INLINE
        matrix_row (matrix_type &data, size_type i): 
            data_ (data), i_ (i) {}

        NUMERICS_INLINE
        size_type size () const { 
            return data_.size2 (); 
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type j) const {
            // One has do to this, to get the const member dispatched?!
            const matrix_type &data = data_;
            return data (i_, j); 
        }
        NUMERICS_INLINE
        reference_type operator () (size_type j) {
            return data_ (i_, j); 
        }

        NUMERICS_INLINE
        value_type operator [] (size_type j) const { 
            // One has do to this, to get the const member dispatched?!
            const matrix_type &data = data_;
            return data (i_, j); 
        }
        NUMERICS_INLINE
        reference_type operator [] (size_type j) { 
            return data_ (i_, j); 
        }

        NUMERICS_INLINE
        vector_range_type project (size_type start, size_type stop) {
            return vector_range_type (*this, start, stop);
        }
        NUMERICS_INLINE
        vector_range_type project (const range &r) {
            return vector_range_type (*this, r);
        }

        // Assignment
        NUMERICS_INLINE
        matrix_row &operator = (const matrix_row &mr) { 
#ifndef USE_GCC
            std::copy (mr.begin (), mr.end (), begin ());
#else
            detail::copy (mr.begin (), mr.end (), begin ());
#endif
            return *this;
        }
        NUMERICS_INLINE
        matrix_row &assign_temporary (matrix_row &mr) { 
            return *this = mr;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_row &operator = (const vector_expression<AE> &ae) {        
            vector_assign<scalar_assign<value_type, value_type> > () (*this, vector<value_type> (ae));
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_row &assign (const vector_expression<AE> &ae) {
            vector_assign<scalar_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_row &operator += (const vector_expression<AE> &ae) {
            vector_assign<scalar_assign<value_type, value_type> > () (*this, vector<value_type> (*this + ae));
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_row &plus_assign (const vector_expression<AE> &ae) {
            vector_assign<scalar_plus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_row &operator -= (const vector_expression<AE> &ae) {
            vector_assign<scalar_assign<value_type, value_type> > () (*this, vector<value_type> (*this - ae));
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_row &minus_assign (const vector_expression<AE> &ae) {
            vector_assign<scalar_minus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        matrix_row &operator *= (const AT &at) {
            vector_assign_scalar<scalar_multiplies_assign<value_type, AT> > () (*this, at);
            return *this;
        }

        // Swapping
        NUMERICS_INLINE
	    void swap (matrix_row &mr) {
            check<external_logic>::precondition (this != &mr);
            check<bad_size>::precondition (size () == mr.size ());
#ifndef USE_GCC
            std::swap_ranges (begin (), end (), mr.begin ());
#else
            detail::swap_ranges (begin (), end (), mr.begin ());
#endif
        }
#ifndef USE_GCC
        NUMERICS_INLINE
	    friend void swap (matrix_row &mr1, matrix_row &mr2) {
            mr1.swap (mr2);
        }
#endif

        class const_iterator;
        class iterator;

        // Element lookup
        NUMERICS_INLINE
        const_iterator find (size_type j) const {
            return const_iterator (*this, data_.find2 (i_, j));
        }
        NUMERICS_INLINE
        iterator find (size_type j) {
            return iterator (*this, data_.find2 (i_, j));
        }

        // Iterators simply are pointers.

        class const_iterator:
            public container_const_reference<matrix_row>,
            public random_access_iterator_base<const_iterator, value_type> {
        public:
            typedef typename M::const_iterator2::iterator_category iterator_category;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator (const matrix_row &mr, const const_iterator_type &it):
                container_const_reference<matrix_row> (mr), it_ (it) {}
            NUMERICS_INLINE
            const_iterator (const iterator &it):
                container_const_reference<matrix_row> (it ()), it_ (it.it_) {}

            // Arithmetic
            NUMERICS_INLINE
            const_iterator &operator ++ () {
                ++ it_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator &operator -- () {
                -- it_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator &operator += (difference_type n) {
                it_ += n;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator &operator -= (difference_type n) {
                it_ -= n;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const const_iterator &it) const {
                return it_ - it.it_;
            }

            // Dereference
            NUMERICS_INLINE
            value_type operator * () const {
                check<bad_index>::precondition (index () < (*this) ().size ());
                return *it_;
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                return it_.index2 ();
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator &it) const {
                check<external_logic>::precondition (&(*this) () == &it ());
                return it_ == it.it_;
            }

        private:
            const_iterator_type it_;
        };

        NUMERICS_INLINE
        const_iterator begin () const {
            return find (0);
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return find (size ());
        }

        class iterator:
            public container_reference<matrix_row>,
            public random_access_iterator_base<iterator, value_type> {
        public:
            typedef typename M::iterator2::iterator_category iterator_category;

            // Construction and destruction
            NUMERICS_INLINE
            iterator (matrix_row &mr, const iterator_type &it):
                container_reference<matrix_row> (mr), it_ (it) {}

            // Arithmetic
            NUMERICS_INLINE
            iterator &operator ++ () {
                ++ it_;
                return *this;
            }
            NUMERICS_INLINE
            iterator &operator -- () {
                -- it;
                return *this;
            }
            NUMERICS_INLINE
            iterator &operator += (difference_type n) {
                it_ += n;
                return *this;
            }
            NUMERICS_INLINE
            iterator &operator -= (difference_type n) {
                it_ -= n;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const iterator &it) const {
                return it_ - it.it_;
            }

            // Dereference
            NUMERICS_INLINE
            reference_type operator * () {
                check<bad_index>::precondition (index () < (*this) ().size ());
                return *it_;
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                return it_.index2 ();
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const iterator &it) const {
                check<external_logic>::precondition (&(*this) () == &it ());
                return it_ == it.it_;
            }

        private:
            iterator_type it_;

            friend class const_iterator;
        };

        NUMERICS_INLINE
        iterator begin () {
            return find (0);
        }
        NUMERICS_INLINE
        iterator end () {
            return find (size ());
        }

    private:
        matrix_type &data_;
        size_type i_;
    };

    // Matrix based column vector class
    template<class M>
    class matrix_column:
		public vector_expression<matrix_column<M> > {
    public:      
        typedef M matrix_type;
        typedef typename M::size_type size_type;
        typedef typename M::difference_type difference_type;
        typedef typename M::value_type value_type;
        typedef typename M::const_reference_type const_reference_type;
        typedef typename M::reference_type reference_type;
        typedef vector_const_reference<matrix_column<M> > const_closure_type;
        typedef vector_reference<matrix_column<M> > closure_type;
        typedef vector_range<matrix_column<M> > vector_range_type;
        typedef typename M::const_iterator1 const_iterator_type;
        typedef typename M::iterator1 iterator_type;
        typedef typename proxy_traits<typename M::storage_category>::storage_category storage_category;

        // Construction and destruction
        NUMERICS_INLINE
        matrix_column (matrix_type &data, size_type j): 
            data_ (data), j_ (j) {}

        NUMERICS_INLINE
        size_type size () const { 
            return data_.size1 (); 
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i) const {
            // One has do to this, to get the const member dispatched?!
            const matrix_type &data = data_;
            return data (i, j_); 
        }
        NUMERICS_INLINE
        reference_type operator () (size_type i) {
            return data_ (i, j_); 
        }

        NUMERICS_INLINE
        value_type operator [] (size_type i) const { 
            // One has do to this, to get the const member dispatched?!
            const matrix_type &data = data_;
            return data (i, j_); 
        }
        NUMERICS_INLINE
        reference_type operator [] (size_type i) { 
            return data_ (i, j_); 
        }

        NUMERICS_INLINE
        vector_range_type project (size_type start, size_type stop) {
            return vector_range_type (*this, start, stop);
        }
        NUMERICS_INLINE
        vector_range_type project (const range &r) {
            return vector_range_type (*this, r);
        }

        // Assignment
        NUMERICS_INLINE
        matrix_column &operator = (const matrix_column &mc) { 
#ifndef USE_GCC
            std::copy (mc.begin (), mc.end (), begin ());
#else
            detail::copy (mc.begin (), mc.end (), begin ());
#endif
            return *this;
        }
        NUMERICS_INLINE
        matrix_column &assign_temporary (matrix_column &mc) { 
            return *this = mc;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_column &operator = (const vector_expression<AE> &ae) {        
            vector_assign<scalar_assign<value_type, value_type> > () (*this, vector<value_type> (ae));
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_column &assign (const vector_expression<AE> &ae) {
            vector_assign<scalar_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_column &operator += (const vector_expression<AE> &ae) {
            vector_assign<scalar_assign<value_type, value_type> > () (*this, vector<value_type> (*this + ae));
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_column &plus_assign (const vector_expression<AE> &ae) {
            vector_assign<scalar_plus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_column &operator -= (const vector_expression<AE> &ae) {
            vector_assign<scalar_assign<value_type, value_type> > () (*this, vector<value_type> (*this - ae));
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_column &minus_assign (const vector_expression<AE> &ae) {
            vector_assign<scalar_minus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        matrix_column &operator *= (const AT &at) {
            vector_assign_scalar<scalar_multiplies_assign<value_type, AT> > () (*this, at);
            return *this;
        }

        // Swapping
        NUMERICS_INLINE
	    void swap (matrix_column &mc) {
            check<external_logic>::precondition (this != &mc);
            check<bad_size>::precondition (size () == mc.size ());
#ifndef USE_GCC
            std::swap_ranges (begin (), end (), mc.begin ());
#else
            detail::swap_ranges (begin (), end (), mc.begin ());
#endif
        }
#ifndef USE_GCC
        NUMERICS_INLINE
	    friend void swap (matrix_column &mc1, matrix_column &mc2) {
            mc1.swap (mc2);
        }
#endif

        class const_iterator;
        class iterator;

        // Element lookup
        NUMERICS_INLINE
        const_iterator find (size_type i) const {
            return const_iterator (*this, data_.find1 (i, j_));
        }
        NUMERICS_INLINE
        iterator find (size_type i) {
            return iterator (*this, data_.find1 (i, j_));
        }

        // Iterators simply are pointers.

        class const_iterator:
            public container_const_reference<matrix_column>,
            public random_access_iterator_base<const_iterator, value_type> {
        public:
            typedef typename M::const_iterator1::iterator_category iterator_category;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator (const matrix_column &mc, const const_iterator_type &it):
                container_const_reference<matrix_column> (mc), it_ (it) {}
            NUMERICS_INLINE
            const_iterator (const iterator &it):
                container_const_reference<matrix_column> (it ()), it_ (it.it_) {}

            // Arithmetic
            NUMERICS_INLINE
            const_iterator &operator ++ () {
                ++ it_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator &operator -- () {
                -- it_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator &operator += (difference_type n) {
                it_ += n;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator &operator -= (difference_type n) {
                it_ -= n;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const const_iterator &it) const {
                return it_ - it.it_;
            }

            // Dereference
            NUMERICS_INLINE
            value_type operator * () const {
                check<bad_index>::precondition (index () < (*this) ().size ());
                return *it_;
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                return it_.index1 ();
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator &it) const {
                check<external_logic>::precondition (&(*this) () == &it ());
                return it_ == it.it_;
            }

        private:
            const_iterator_type it_;
        };

        NUMERICS_INLINE
        const_iterator begin () const {
            return find (0);
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return find (size ());
        }

        class iterator:
            public container_reference<matrix_column>,
            public random_access_iterator_base<iterator, value_type> {
        public:
            typedef typename M::iterator1::iterator_category iterator_category;

            // Construction and destruction
            NUMERICS_INLINE
            iterator (matrix_column &mc, const iterator_type &it):
                container_reference<matrix_column> (mc), it_ (it) {}

            // Arithmetic
            NUMERICS_INLINE
            iterator &operator ++ () {
                ++ it_;
                return *this;
            }
            NUMERICS_INLINE
            iterator &operator -- () {
                -- it_;
                return *this;
            }
            NUMERICS_INLINE
            iterator &operator += (difference_type n) {
                it_ += n;
                return *this;
            }
            NUMERICS_INLINE
            iterator &operator -= (difference_type n) {
                it_ -= n;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const iterator &it) const {
                return it_ - it.it_;
            }

            // Dereference
            NUMERICS_INLINE
            reference_type operator * () {
                check<bad_index>::precondition (index () < (*this) ().size ());
                return *it_;
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                return it_.index1 ();
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const iterator &it) const {
                check<external_logic>::precondition (&(*this) () == &it ());
                return it_ == it.it_;
            }

        private:
            iterator_type it_;

            friend class const_iterator;
        };

        NUMERICS_INLINE
        iterator begin () {
            return find (0);
        }
        NUMERICS_INLINE
        iterator end () {
            return find (size ());
        }

    private:
        matrix_type &data_;
        size_type j_;
    };

    // Matrix based vector range class
    template<class M>
    class matrix_vector_range:
		public vector_expression<matrix_vector_range<M> > {
    public:      
        typedef M matrix_type;
        typedef typename M::size_type size_type;
        typedef typename M::difference_type difference_type;
        typedef typename M::value_type value_type;
        typedef typename M::const_reference_type const_reference_type;
        typedef typename M::reference_type reference_type;
        typedef vector_const_reference<matrix_vector_range<M> > const_closure_type;
        typedef vector_reference<matrix_vector_range<M> > closure_type;
        typedef range::const_iterator const_iterator1_type;
        typedef range::const_iterator iterator1_type;
        typedef range::const_iterator const_iterator2_type;
        typedef range::const_iterator iterator2_type;
        typedef typename proxy_traits<typename M::storage_category>::storage_category storage_category;

        // Construction and destruction
        NUMERICS_INLINE
        matrix_vector_range (matrix_type &data, const range &r1, const range &r2): 
            data_ (data), r1_ (r1), r2_ (r2) {}
        NUMERICS_INLINE
        matrix_vector_range (matrix_type &data, size_type start1, size_type stop1, size_type start2, size_type stop2): 
            data_ (data), r1_ (start1, stop1), r2_ (start2, stop2) {}

        NUMERICS_INLINE
        size_type size () const { 
            return std::max (r1_.size (), r2_.size ()); 
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i) const {
            // One has do to this, to get the const member dispatched?!
            const matrix_type &data = data_;
            return data (r1_ (i), r2_ (i)); 
        }
        NUMERICS_INLINE
        reference_type operator () (size_type i) {
            return data_ (r1_ (i), r2_ (i)); 
        }

        NUMERICS_INLINE
        value_type operator [] (size_type i) const { 
            // One has do to this, to get the const member dispatched?!
            const matrix_type &data = data_;
            return data (r1_ (i), r2_ (i)); 
        }
        NUMERICS_INLINE
        reference_type operator [] (size_type i) { 
            return data_ (r1_ (i), r2_ (i)); 
        }

        NUMERICS_INLINE
        matrix_vector_range project (size_type start, size_type stop) {
            return matrix_vector_range (data_, r1_.composite (start, stop), r2_.composite (start, stop));
        }
        NUMERICS_INLINE
        matrix_vector_range project (const range &r) {
            return matrix_vector_range_type (data_, r1_.composite (r), r2_.composite (r));
        }

        // Assignment
        NUMERICS_INLINE
        matrix_vector_range &operator = (const matrix_vector_range &mvr) { 
#ifndef USE_GCC
            std::copy (mvr.begin (), mvr.end (), begin ());
#else
            detail::copy (mvr.begin (), mvr.end (), begin ());
#endif
            return *this;
        }
        NUMERICS_INLINE
        matrix_vector_range &assign_temporary (matrix_vector_range &mvr) { 
            return *this = mvr;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_vector_range &operator = (const vector_expression<AE> &ae) {        
            vector_assign<scalar_assign<value_type, value_type> > () (*this, vector<value_type> (ae));
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_vector_range &assign (const vector_expression<AE> &ae) {
            vector_assign<scalar_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_vector_range &operator += (const vector_expression<AE> &ae) {
            vector_assign<scalar_assign<value_type, value_type> > () (*this, vector<value_type> (*this + ae));
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_vector_range &plus_assign (const vector_expression<AE> &ae) {
            vector_assign<scalar_plus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_vector_range &operator -= (const vector_expression<AE> &ae) {
            vector_assign<scalar_assign<value_type, value_type> > () (*this, vector<value_type> (*this - ae));
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_vector_range &minus_assign (const vector_expression<AE> &ae) {
            vector_assign<scalar_minus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        matrix_vector_range &operator *= (const AT &at) {
            vector_assign_scalar<scalar_multiplies_assign<value_type, AT> > () (*this, at);
            return *this;
        }

        // Swapping
        NUMERICS_INLINE
	    void swap (matrix_vector_range &mvr) {
            check<external_logic>::precondition (this != &mvr);
            check<bad_size>::precondition (size () == mvr.size ());
#ifndef USE_GCC
            std::swap_ranges (begin (), end (), mvr.begin ());
#else
            detail::swap_ranges (begin (), end (), mvr.begin ());
#endif
        }
#ifndef USE_GCC
        NUMERICS_INLINE
	    friend void swap (matrix_vector_range &mvr1, matrix_vector_range &mvr2) {
            mvr1.swap (mvr2);
        }
#endif

        // Iterators simply are indexes.

        class iterator;

        class const_iterator:
            public container_const_reference<matrix_type>,
            public random_access_iterator_base<const_iterator, value_type> {
        public:
            typedef typename restrict_traits<typename M::const_iterator1::iterator_category,
                                             typename M::const_iterator2::iterator_category>::iterator_category iterator_category; 

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator (const matrix_type &m, const const_iterator1_type &it1, const const_iterator2_type &it2):
                container_const_reference<matrix_type> (m), it1_ (it1), it2_ (it2) {}
            NUMERICS_INLINE
            const_iterator (const iterator &it):
                container_const_reference<matrix_type> (it ()), it1_ (it.it1_), it2_ (it.it2_) {}

            // Arithmetic
            NUMERICS_INLINE
            const_iterator &operator ++ () {
                ++ it1_;
                ++ it2_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator &operator -- () {
                -- it1_;
                -- it2_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator &operator += (difference_type n) {
                it1_ += n;
                it2_ += n;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator &operator -= (difference_type n) {
                it1_ -= n;
                it2_ -= n;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const const_iterator &it) const {
                return common (it1_ - it.it1_, it2_ - it.it2_);
            }

            // Dereference
            NUMERICS_INLINE
            value_type operator * () const {
                return (*this) () (*it1_, *it2_); 
            }

            // Index
            NUMERICS_INLINE
            size_type  index () const {
                return common (it1_.index (), it2_.index ());
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator &it) const {
                check<external_logic>::precondition (&(*this) () == &it ());
                return it1_ == it.it1_ && it2_ == it.it2_;
            }

        private:
            const_iterator1_type it1_;
            const_iterator2_type it2_;
        };

        NUMERICS_INLINE
        const_iterator begin () const {
            return const_iterator (data_, r1_.begin (), r2_.begin ()); 
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return const_iterator (data_, r1_.end (), r2_.end ()); 
        }

        class iterator:
            public container_reference<matrix_type>,
            public random_access_iterator_base<iterator, value_type> {
        public:
            typedef typename restrict_traits<typename M::iterator1::iterator_category,
                                             typename M::iterator2::iterator_category>::iterator_category iterator_category; 

            // Construction and destruction
            NUMERICS_INLINE
            iterator (matrix_type &m, const iterator1_type &it1, const iterator2_type &it2):
                container_reference<matrix_type> (m), it1_ (it1), it2_ (it2) {}

            // Arithmetic
            NUMERICS_INLINE
            iterator &operator ++ () {
                ++ it1_;
                ++ it2_;
                return *this;
            }
            NUMERICS_INLINE
            iterator &operator -- () {
                -- it1_;
                -- it2_;
                return *this;
            }
            NUMERICS_INLINE
            iterator &operator += (difference_type n) {
                it1_ += n;
                it2_ += n;
                return *this;
            }
            NUMERICS_INLINE
            iterator &operator -= (difference_type n) {
                it1_ -= n;
                it2_ -= n;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const iterator &it) const {
                return common (it1_ - it.it1_, it2_ - it.it2_);
            }

            // Dereference
            NUMERICS_INLINE
            reference_type operator * () {
                return (*this) () (*it1_, *it2_); 
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                return common (it1_.index (), it2_.index ());
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const iterator &it) const {
                check<external_logic>::precondition (&(*this) () == &it ());
                return it1_ == it.it1_ && it2_ == it.it2_;
            }

        private:
            iterator1_type it1_;
            iterator2_type it2_;

            friend class const_iterator;
        };

        NUMERICS_INLINE
        iterator begin () {
            return iterator (data_, r1_.begin (), r2_.begin ()); 
        }
        NUMERICS_INLINE
        iterator end () {
            return iterator (data_, r1_.end (), r2_.end ()); 
        }

    private:
        matrix_type &data_;
        range r1_;
        range r2_;
    };

    // Matrix based vector slice class
    template<class M>
    class matrix_vector_slice:
		public vector_expression<matrix_vector_slice<M> > {
    public:      
        typedef M matrix_type;
        typedef typename M::size_type size_type;
        typedef typename M::difference_type difference_type;
        typedef typename M::value_type value_type;
        typedef typename M::const_reference_type const_reference_type;
        typedef typename M::reference_type reference_type;
        typedef vector_const_reference<matrix_vector_slice<M> > const_closure_type;
        typedef vector_reference<matrix_vector_slice<M> > closure_type;
        typedef slice::const_iterator const_iterator1_type;
        typedef slice::const_iterator iterator1_type;
        typedef slice::const_iterator const_iterator2_type;
        typedef slice::const_iterator iterator2_type;
        typedef typename proxy_traits<typename M::storage_category>::storage_category storage_category;

        // Construction and destruction
        NUMERICS_INLINE
        matrix_vector_slice (matrix_type &data, const slice &s1, const slice &s2): 
            data_ (data), s1_ (s1), s2_ (s2) {}
        NUMERICS_INLINE
        matrix_vector_slice (matrix_type &data, size_type start1, size_type stride1, size_type size1, size_type start2, size_type stride2, size_type size2): 
            data_ (data), s1_ (start1, stride1, size1), s2_ (start2, stride2, size2) {}

        NUMERICS_INLINE
        size_type size () const { 
            return common (s1_.size (), s2_.size ()); 
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i) const {
            // One has do to this, to get the const member dispatched?!
            const matrix_type &data = data_;
            return data (s1_ (i), s2_ (i)); 
        }
        NUMERICS_INLINE
        reference_type operator () (size_type i) {
            return data_ (s1_ (i), s2_ (i)); 
        }

        NUMERICS_INLINE
        value_type operator [] (size_type i) const { 
            // One has do to this, to get the const member dispatched?!
            const matrix_type &data = data_;
            return data (s1_ (i). s2_ (i)); 
        }
        NUMERICS_INLINE
        reference_type operator [] (size_type i) { 
            return data_ (s1_ (i), s2_ (i)); 
        }

        NUMERICS_INLINE
        matrix_vector_slice project (const range &r) {
            return matrix_vector_slice (data_, s1_.composite (r), s2_.composite (r));
        }
        NUMERICS_INLINE
        matrix_vector_slice project (const slice &s) {
            return matrix_vector_slice (data_, s1_.composite (s), s2_.composite (s));
        }

        // Assignment
        NUMERICS_INLINE
        matrix_vector_slice &operator = (const matrix_vector_slice &mvs) { 
#ifndef USE_GCC
            std::copy (mvs.begin (), mvs.end (), begin ());
#else
            detail::copy (mvs.begin (), mvs.end (), begin ());
#endif
            return *this;
        }
        NUMERICS_INLINE
        matrix_vector_slice &assign_temporary (matrix_vector_slice &mvs) { 
            return *this = mvs;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_vector_slice &operator = (const vector_expression<AE> &ae) {        
            vector_assign<scalar_assign<value_type, value_type> > () (*this, vector<value_type> (ae));
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_vector_slice &assign (const vector_expression<AE> &ae) {
            vector_assign<scalar_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_vector_slice &operator += (const vector_expression<AE> &ae) {
            vector_assign<scalar_assign<value_type, value_type> > () (*this, vector<value_type> (*this + ae));
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_vector_slice &plus_assign (const vector_expression<AE> &ae) {
            vector_assign<scalar_plus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_vector_slice &operator -= (const vector_expression<AE> &ae) {
            vector_assign<scalar_assign<value_type, value_type> > () (*this, vector<value_type> (*this - ae));
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_vector_slice &minus_assign (const vector_expression<AE> &ae) {
            vector_assign<scalar_minus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        matrix_vector_slice &operator *= (const AT &at) {
            vector_assign_scalar<scalar_multiplies_assign<value_type, AT> > () (*this, at);
            return *this;
        }

        // Swapping
        NUMERICS_INLINE
	    void swap (matrix_vector_slice &mvs) {
            check<external_logic>::precondition (this != &mvs);
            check<bad_size>::precondition (size () == mvs.size ());
#ifndef USE_GCC
            std::swap_ranges (begin (), end (), mvs.begin ());
#else
            detail::swap_ranges (begin (), end (), mvs.begin ());
#endif
        }
#ifndef USE_GCC
        NUMERICS_INLINE
	    friend void swap (matrix_vector_slice &mvs1, matrix_vector_slice &mvs2) {
            mvs1.swap (mvs2);
        }
#endif

        // Iterators simply are indexes.

        class iterator;

        class const_iterator:
            public container_const_reference<matrix_type>,
            public random_access_iterator_base<const_iterator, value_type> {
        public:
            typedef typename restrict_traits<typename M::const_iterator1::iterator_category,
                                             typename M::const_iterator2::iterator_category>::iterator_category iterator_category; 

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator (const matrix_type &m, const const_iterator1_type &it1, const const_iterator2_type &it2):
                container_const_reference<matrix_type> (m), it1_ (it1), it2_ (it2) {}
            NUMERICS_INLINE
            const_iterator (const iterator &it):
                container_const_reference<matrix_type> (it ()), it1_ (it.it1_), it2_ (it.it2_) {}

            // Arithmetic
            NUMERICS_INLINE
            const_iterator &operator ++ () {
                ++ it1_;
                ++ it2_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator &operator -- () {
                -- it1_;
                -- it2_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator &operator += (difference_type n) {
                it1_ += n;
                it2_ += n;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator &operator -= (difference_type n) {
                it1_ -= n;
                it2_ -= n;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const const_iterator &it) const {
                return common (it1_ - it.it1_, it2_ - it.it2_);
            }

            // Dereference
            NUMERICS_INLINE
            value_type operator * () const {
                return (*this) () (*it1_, *it2_); 
            }

            // Index
            NUMERICS_INLINE
            size_type  index () const {
                return common (it1_.index (), it2_.index ());
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator &it) const {
                check<external_logic>::precondition (&(*this) () == &it ());
                return it1_ == it.it1_ && it2_ == it.it2_;
            }

        private:
            const_iterator1_type it1_;
            const_iterator2_type it2_;
        };

        NUMERICS_INLINE
        const_iterator begin () const {
            return const_iterator (data_, s1_.begin (), s2_.begin ()); 
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return const_iterator (data_, s1_.end (), s2_.end ()); 
        }

        class iterator:
            public container_reference<matrix_type>,
            public random_access_iterator_base<iterator, value_type> {
        public:
            typedef typename restrict_traits<typename M::iterator1::iterator_category,
                                             typename M::iterator2::iterator_category>::iterator_category iterator_category; 

            // Construction and destruction
            NUMERICS_INLINE
            iterator (matrix_type &m, const iterator1_type &it1, const iterator2_type &it2):
                container_reference<matrix_type> (m), it1_ (it1), it2_ (it2) {}

            // Arithmetic
            NUMERICS_INLINE
            iterator &operator ++ () {
                ++ it1_;
                ++ it2_;
                return *this;
            }
            NUMERICS_INLINE
            iterator &operator -- () {
                -- it1_;
                -- it2_;
                return *this;
            }
            NUMERICS_INLINE
            iterator &operator += (difference_type n) {
                it1_ += n;
                it2_ += n;
                return *this;
            }
            NUMERICS_INLINE
            iterator &operator -= (difference_type n) {
                it1_ -= n;
                it2_ -= n;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const iterator &it) const {
                return common (it1_ - it.it1_, it2_ - it.it2_);
            }

            // Dereference
            NUMERICS_INLINE
            reference_type operator * () {
                return (*this) () (*it1_, *it2_); 
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                return common (it1_.index (), it2_.index ());
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const iterator &it) const {
                check<external_logic>::precondition (&(*this) () == &it ());
                return it1_ == it.it1_ && it2_ == it.it2_;
            }

        private:
            iterator1_type it1_;
            iterator2_type it2_;

            friend class const_iterator;
        };

        NUMERICS_INLINE
        iterator begin () {
            return iterator (data_, s1_.begin (), s2_.begin ()); 
        }
        NUMERICS_INLINE
        iterator end () {
            return iterator (data_, s1_.end (), s2_.end ()); 
        }

    private:
        matrix_type &data_;
        slice s1_;
        slice s2_;
    };

    // Matrix based range class
    template<class M>
    class matrix_range:
		public matrix_expression<matrix_range<M> > {
    public:      
        typedef M matrix_type;
        typedef typename M::size_type size_type;
        typedef typename M::difference_type difference_type;
        typedef typename M::value_type value_type;
        typedef typename M::const_reference_type const_reference_type;
        typedef typename M::reference_type reference_type;
        typedef matrix_const_reference<matrix_range<M> > const_closure_type;
        typedef matrix_reference<matrix_range<M> > closure_type;
        typedef matrix_row<matrix_range<M> > matrix_row_type;
        typedef matrix_column<matrix_range<M> > matrix_column_type;
        typedef typename M::const_iterator1 const_iterator1_type;
        typedef typename M::iterator1 iterator1_type;
        typedef typename M::const_iterator2 const_iterator2_type;
        typedef typename M::iterator2 iterator2_type;
        typedef typename proxy_traits<typename M::storage_category>::storage_category storage_category;
        typedef typename M::orientation_category orientation_category;

        // Construction and destruction
        NUMERICS_INLINE
        matrix_range (matrix_type &data, const range &r1, const range &r2): 
            data_ (data), r1_ (r1), r2_ (r2) {}
        NUMERICS_INLINE
        matrix_range (matrix_type &data, size_type start1, size_type stop1, size_type start2, size_type stop2): 
            data_ (data), r1_ (start1, stop1), r2_ (start2, stop2) {}

        NUMERICS_INLINE
        size_type start1 () const { 
            return r1_.start (); 
        }
        NUMERICS_INLINE
        size_type size1 () const { 
            return r1_.size ();
        }
        NUMERICS_INLINE
        size_type start2() const { 
            return r2_.start (); 
        }
        NUMERICS_INLINE
        size_type size2 () const { 
            return r2_.size ();
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i, size_type j) const {
            // One has do to this, to get the const member dispatched?!
            const matrix_type &data = data_;
            return data (r1_ (i), r2_ (j)); 
        }
        NUMERICS_INLINE
        reference_type operator () (size_type i, size_type j) {
            return data_ (r1_ (i), r2_ (j)); 
        }

        NUMERICS_INLINE
        matrix_row_type operator [] (size_type i) {
            return matrix_row_type (*this, i);
        }
        NUMERICS_INLINE
        matrix_row_type row (size_type i) {
            return matrix_row_type (*this, i);
        }
        NUMERICS_INLINE
        matrix_column_type column (size_type j) {
            return matrix_column_type (*this, j);
        }

        NUMERICS_INLINE
        matrix_range project (size_type start1, size_type stop1, size_type start2, size_type stop2) {
            return matrix_range (data_, r1_.composite (start1, stop1), r2_.composite (start2, stop2));
        }
        NUMERICS_INLINE
        matrix_range project (const range &r1, const range &r2) {
            return matrix_range (data_, r1_.composite (r1), r2_.composite (r2));
        }

        // Assignment
        NUMERICS_INLINE
        matrix_range &operator = (const matrix_range &mr) { 
            matrix_assign<scalar_assign<value_type, value_type> > () (*this, mr); 
            return *this;
        }
        NUMERICS_INLINE
        matrix_range &assign_temporary (matrix_range &mr) { 
            return *this = mr;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_range &operator = (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_assign<value_type, value_type> > () (*this, matrix<value_type> (ae)); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_range &assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_range& operator += (const matrix_expression<AE> &ae) {
            matrix_assign<scalar_assign<value_type, value_type> > () (*this, matrix<value_type> (*this + ae)); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_range &plus_assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_plus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_range& operator -= (const matrix_expression<AE> &ae) {
            matrix_assign<scalar_assign<value_type, value_type> > () (*this, matrix<value_type> (*this - ae)); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_range &minus_assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_minus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        matrix_range& operator *= (const AT &at) {
            matrix_assign_scalar<scalar_multiplies_assign<value_type, AT> > () (*this, at);
            return *this;
        }

        // Swapping
        NUMERICS_INLINE
	    void swap (matrix_range &mr) {
            check<external_logic>::precondition (this != &mr);
            check<bad_size>::precondition (size1 () == mr.size1 ());
            check<bad_size>::precondition (size2 () == mr.size2 ());
            matrix_swap<scalar_swap<value_type, value_type> > () (*this, mr); 
        }
#ifndef USE_GCC
        NUMERICS_INLINE
	    friend void swap (matrix_range &mr1, matrix_range &mr2) {
            mr1.swap (mr2);
        }
#endif

        class const_iterator1;
        class iterator1;
        class const_iterator2;
        class iterator2;

        // Element lookup
        NUMERICS_INLINE
        const_iterator1 find1 (size_type i, size_type j) const {
            return const_iterator1 (*this, data_.find1 (start1 () + i, start2 () + j));
        }
        NUMERICS_INLINE
        iterator1 find1 (size_type i, size_type j) {
            return iterator1 (*this, data_.find1 (start1 () + i, start2 () + j));
        }
        NUMERICS_INLINE
        const_iterator2 find2 (size_type i, size_type j) const {
            return const_iterator2 (*this, data_.find2 (start1 () + i, start2 () + j));
        }
        NUMERICS_INLINE
        iterator2 find2 (size_type i, size_type j) {
            return iterator2 (*this, data_.find2 (start1 () + i, start2 () + j));
        }

        // Iterators simply are pointers.

        class const_iterator1:
            public container_const_reference<matrix_range>,
            public random_access_iterator_base<const_iterator1, value_type> {
        public:
            typedef typename M::const_iterator1::iterator_category iterator_category;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator1 (const matrix_range &mr, const const_iterator1_type &it):
                container_const_reference<matrix_range> (mr), it_ (it) {}
            NUMERICS_INLINE
            const_iterator1 (const iterator1 &it):
                container_const_reference<matrix_range> (it ()), it_ (it.it_) {}

            // Arithmetic
            NUMERICS_INLINE
            const_iterator1 &operator ++ () {
                ++ it_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator -- () {
                -- it_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator += (difference_type n) {
                it_ += n;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator -= (difference_type n) {
                it_ -= n;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const const_iterator1 &it) const {
                return it_ - it.it_;
            }

            // Dereference
            NUMERICS_INLINE
            value_type operator * () const {
                return *it_; 
            }

            NUMERICS_INLINE
            const_iterator2 begin () const {
                const matrix_range &mr = (*this) ();
                return mr.find2 (index1 (), 0);
            }
            NUMERICS_INLINE
            const_iterator2 end () const {
                const matrix_range &mr = (*this) ();
                return mr.find2 (index1 (), mr.size2 ());
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                return it_.index1 () - (*this) ().start1 ();
            }
            NUMERICS_INLINE
            size_type index2 () const {
                return it_.index2 () - (*this) ().start2 ();
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator1 &it) const {
                check<external_logic>::precondition (&(*this) () == &it ());
                return it_ == it.it_;
            }

        private:
            const_iterator1_type it_;
        };

        NUMERICS_INLINE
        const_iterator1 begin1 () const {
            return find1 (0, 0); 
        }
        NUMERICS_INLINE
        const_iterator1 end1 () const {
            return find1 (size1 (), 0); 
        }

        class iterator1:
            public container_reference<matrix_range>,
            public random_access_iterator_base<iterator1, value_type> {
        public:
            typedef typename M::iterator1::iterator_category iterator_category;

            // Construction and destruction
            NUMERICS_INLINE
            iterator1 (matrix_range &mr, const iterator1_type &it):
                container_reference<matrix_range> (mr), it_ (it) {}

            // Arithmetic
            NUMERICS_INLINE
            iterator1 &operator ++ () {
                ++ it_;
                return *this;
            }
            NUMERICS_INLINE
            iterator1 &operator -- () {
                -- it_;
                return *this;
            }
            NUMERICS_INLINE
            iterator1 &operator += (difference_type n) {
                it_ += n;
                return *this;
            }
            NUMERICS_INLINE
            iterator1 &operator -= (difference_type n) {
                it_ -= n;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const iterator1 &it) const {
                return it_ - it.it_;
            }

            // Dereference
            NUMERICS_INLINE
            reference_type operator * () {
                return *it_; 
            }

            NUMERICS_INLINE
            iterator2 begin () {
                matrix_range &mr = (*this) ();
                return mr.find2 (index1 (), 0);
            }
            NUMERICS_INLINE
            iterator2 end () {
                matrix_range &mr = (*this) ();
                return mr.find2 (index1 (), mr.size2 ());
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                return it_.index1 () - (*this) ().start1 ();
            }
            NUMERICS_INLINE
            size_type index2 () const {
                return it_.index2 () - (*this) ().start2 ();
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const iterator1 &it) const {
                check<external_logic>::precondition (&(*this) () == &it ());
                return it_ == it.it_;
            }

        private:
            iterator1_type it_;

            friend class const_iterator1;
        };

        NUMERICS_INLINE
        iterator1 begin1 () {
            return find1 (0, 0); 
        }
        NUMERICS_INLINE
        iterator1 end1 () {
            return find1 (size1 (), 0); 
        }

        class const_iterator2:
            public container_const_reference<matrix_range>,
            public random_access_iterator_base<const_iterator2, value_type> {
        public:
            typedef typename M::const_iterator2::iterator_category iterator_category;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator2 (const matrix_range &mr, const const_iterator2_type &it):
                container_const_reference<matrix_range> (mr), it_ (it) {}
            NUMERICS_INLINE
            const_iterator2 (const iterator2 &it):
                container_const_reference<matrix_range> (it ()), it_ (it.it_) {}

            // Arithmetic
            NUMERICS_INLINE
            const_iterator2 &operator ++ () {
                ++ it_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator2 &operator -- () {
                -- it_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator2 &operator += (difference_type n) {
                it_ += n;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator2 &operator -= (difference_type n) {
                it_ -= n;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const const_iterator2 &it) const {
                return it_ - it.it_;
            }

            // Dereference
            NUMERICS_INLINE
            value_type operator * () const {
                return *it_; 
            }

            NUMERICS_INLINE
            const_iterator1 begin () const {
                const matrix_range &mr = (*this) ();
                return mr.find1 (0, index2 ());
            }
            NUMERICS_INLINE
            const_iterator1 end () const {
                const matrix_range &mr = (*this) ();
                return mr.find1 (mr.size1 (), index2 ());
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                return it_.index1 () - (*this) ().start1 ();
            }
            NUMERICS_INLINE
            size_type index2 () const {
                return it_.index2 () - (*this) ().start2 ();
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator2 &it) const {
                check<external_logic>::precondition (&(*this) () == &it ());
                return it_ == it.it_;
            }

        private:
            const_iterator2_type it_;
        };

        NUMERICS_INLINE
        const_iterator2 begin2 () const {
            return find2 (0, 0); 
        }
        NUMERICS_INLINE
        const_iterator2 end2 () const {
            return find2 (0, size2 ()); 
        }

        class iterator2:
            public container_reference<matrix_range>,
            public random_access_iterator_base<iterator2, value_type> {
        public:
            typedef typename M::iterator2::iterator_category iterator_category;

            // Construction and destruction
            NUMERICS_INLINE
            iterator2 (matrix_range &mr, const iterator2_type &it):
                container_reference<matrix_range> (mr), it_ (it) {}

            // Arithmetic
            NUMERICS_INLINE
            iterator2 &operator ++ () {
                ++ it_;
                return *this;
            }
            NUMERICS_INLINE
            iterator2 &operator -- () {
                -- it_;
                return *this;
            }
            NUMERICS_INLINE
            iterator2 &operator += (difference_type n) {
                it_ += n;
                return *this;
            }
            NUMERICS_INLINE
            iterator2 &operator -= (difference_type n) {
                it_ -= n;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const iterator2 &it) const {
                return it_ - it.it_;
            }

            // Dereference
            NUMERICS_INLINE
            reference_type operator * () {
                return *it_;
            }

            NUMERICS_INLINE
            iterator1 begin () {
                matrix_range &mr = (*this) ();
                return mr.find1 (0, index2 ());
            }
            NUMERICS_INLINE
            iterator1 end () {
                matrix_range &mr = (*this) ();
                return mr.find1 (mr.size1 (), index2 ());
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                return it_.index1 () - (*this) ().start1 ();
            }
            NUMERICS_INLINE
            size_type index2 () const {
                return it_.index2 () - (*this) ().start2 ();
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const iterator2 &it) const {
                check<external_logic>::precondition (&(*this) () == &it ());
                return it_ == it.it_;
            }

        private:
            iterator2_type it_;

            friend class const_iterator2;
        };

        NUMERICS_INLINE
        iterator2 begin2 () {
            return find2 (0, 0); 
        }
        NUMERICS_INLINE
        iterator2 end2 () {
            return find2 (0, size2 ()); 
        }

    private:
        matrix_type &data_;
        range r1_;
        range r2_;
    };

    // Matrix based slice class
    template<class M>
    class matrix_slice:
		public matrix_expression<matrix_slice<M> > {
    public:      
        typedef M matrix_type;
        typedef typename M::size_type size_type;
        typedef typename M::difference_type difference_type;
        typedef typename M::value_type value_type;
        typedef typename M::const_reference_type const_reference_type;
        typedef typename M::reference_type reference_type;
        typedef matrix_const_reference<matrix_slice<M> > const_closure_type;
        typedef matrix_reference<matrix_slice<M> > closure_type;
        typedef matrix_row<matrix_slice<M> > matrix_row_type;
        typedef matrix_column<matrix_slice<M> > matrix_column_type;
        typedef slice::const_iterator const_iterator1_type;
        typedef slice::const_iterator iterator1_type;
        typedef slice::const_iterator const_iterator2_type;
        typedef slice::const_iterator iterator2_type;
        typedef typename proxy_traits<typename M::storage_category>::storage_category storage_category;
        typedef typename M::orientation_category orientation_category;

        // Construction and destruction
        NUMERICS_INLINE
        matrix_slice (matrix_type &data, const slice &s1, const slice &s2): 
            data_ (data), s1_ (s1), s2_ (s2) {}
        NUMERICS_INLINE
        matrix_slice (matrix_type &data, size_type start1, size_type stride1, size_type size1, size_type start2, size_type stride2, size_type size2): 
            data_ (data), s1_ (start1, stride1, size1), s2_ (start2, stride2, size2) {}

        NUMERICS_INLINE
        size_type size1 () const { 
            return s1_.size ();
        }
        NUMERICS_INLINE
        size_type size2 () const { 
            return s2_.size ();
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i, size_type j) const {
            // One has do to this, to get the const member dispatched?!
            const matrix_type &data = data_;
            return data (s1_ (i), s2_ (j)); 
        }
        NUMERICS_INLINE
        reference_type operator () (size_type i, size_type j) {
            return data_ (s1_ (i), s2_ (j)); 
        }

        NUMERICS_INLINE
        matrix_row_type operator [] (size_type i) {
            return matrix_row_type (*this, i);
        }
        NUMERICS_INLINE
        matrix_row_type row (size_type i) {
            return matrix_row_type (*this, i);
        }
        NUMERICS_INLINE
        matrix_column_type column (size_type j) {
            return matrix_column_type (*this, j);
        }

        NUMERICS_INLINE
        matrix_slice project (const range &r1, const range &r2) {
            return matrix_slice (data_, s1_.composite (r1), s2_.composite (r2));
        }
        NUMERICS_INLINE
        matrix_slice project (const slice &s) {
            return matrix_slice (data_, s1_.composite (s), s2_.composite (s));
        }

        // Assignment
        NUMERICS_INLINE
        matrix_slice &operator = (const matrix_slice &ms) { 
            matrix_assign<scalar_assign<value_type, value_type> > () (*this, ms); 
            return *this;
        }
        NUMERICS_INLINE
        matrix_slice &assign_temporary (matrix_slice &ms) { 
            return *this = ms;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_slice &operator = (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_assign<value_type, value_type> > () (*this, matrix<value_type> (ae)); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_slice &assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_slice& operator += (const matrix_expression<AE> &ae) {
            matrix_assign<scalar_assign<value_type, value_type> > () (*this, matrix<value_type> (*this + ae)); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_slice &plus_assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_plus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_slice& operator -= (const matrix_expression<AE> &ae) {
            matrix_assign<scalar_assign<value_type, value_type> > () (*this, matrix<value_type> (*this - ae)); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_slice &minus_assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_minus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        matrix_slice& operator *= (const AT &at) {
            matrix_assign_scalar<scalar_multiplies_assign<value_type, AT> > () (*this, at);
            return *this;
        }

        // Swapping
        NUMERICS_INLINE
	    void swap (matrix_slice &ms) {
            check<external_logic>::precondition (this != &ms);
            check<bad_size>::precondition (size1 () == ms.size1 ());
            check<bad_size>::precondition (size2 () == ms.size2 ());
            matrix_swap<scalar_swap<value_type, value_type> > () (*this, ms); 
        }
#ifndef USE_GCC
        NUMERICS_INLINE
	    friend void swap (matrix_slice &ms1, matrix_slice &ms2) {
            ms1.swap (ms2);
        }
#endif

        // Iterators simply are indexes.

        class iterator1;

        class const_iterator2;

        class const_iterator1:
            public container_const_reference<matrix_type>,
            public random_access_iterator_base<const_iterator1, value_type> {
        public:
            typedef typename M::const_iterator1::iterator_category iterator_category;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator1 (const matrix_type &m, const const_iterator1_type &it1, const const_iterator2_type &it2):
                container_const_reference<matrix_type> (m), it1_ (it1), it2_ (it2) {}
            NUMERICS_INLINE
            const_iterator1 (const iterator1 &it):
                container_const_reference<matrix_type> (it ()), it1_ (it.it1_), it2_ (it.it2_) {}

            // Arithmetic
            NUMERICS_INLINE
            const_iterator1 &operator ++ () {
                ++ it1_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator -- () {
                -- it1_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator += (difference_type n) {
                it1_ += n;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator -= (difference_type n) {
                it1_ -= n;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const const_iterator1 &it) const {
                return it1_ - it.it1_;
            }

            // Dereference
            NUMERICS_INLINE
            value_type operator * () const {
                return (*this) () (*it1_, *it2_); 
            }

            NUMERICS_INLINE
            const_iterator2 begin () const {
                return const_iterator2 ((*this) (), it1_, it2_ ().begin ());
            }
            NUMERICS_INLINE
            const_iterator2 end () const {
                return const_iterator2 ((*this) (), it1_, it2_ ().end ()); 
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                return it1_.index ();
            }
            NUMERICS_INLINE
            size_type index2 () const {
                return it2_.index ();
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator1 &it) const {
                check<external_logic>::precondition (&(*this) () == &it ());
                return it1_ == it.it1_ && it2_ == it.it2_;
            }

        private:
            const_iterator1_type it1_;
            const_iterator2_type it2_;
        };

        NUMERICS_INLINE
        const_iterator1 begin1 () const {
            return const_iterator1 (data_, s1_.begin (), s2_.begin ()); 
        }
        NUMERICS_INLINE
        const_iterator1 end1 () const {
            return const_iterator1 (data_, s1_.end (), s2_.begin ()); 
        }

        class iterator2;

        class iterator1:
            public container_reference<matrix_type>,
            public random_access_iterator_base<iterator1, value_type> {
        public:
            typedef typename M::iterator1::iterator_category iterator_category;

            // Construction and destruction
            NUMERICS_INLINE
            iterator1 (matrix_type &m, const iterator1_type &it1, const iterator2_type &it2):
                container_reference<matrix_type> (m), it1_ (it1), it2_ (it2) {}

            // Arithmetic
            NUMERICS_INLINE
            iterator1 &operator ++ () {
                ++ it1_;
                return *this;
            }
            NUMERICS_INLINE
            iterator1 &operator -- () {
                -- it1_;
                return *this;
            }
            NUMERICS_INLINE
            iterator1 &operator += (difference_type n) {
                it1_ += n;
                return *this;
            }
            NUMERICS_INLINE
            iterator1 &operator -= (difference_type n) {
                it1_ -= n;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const iterator1 &it) const {
                return it1_ - it.it1_;
            }

            // Dereference
            NUMERICS_INLINE
            reference_type operator * () {
                return (*this) () (*it1_, *it2_); 
            }

            NUMERICS_INLINE
            iterator2 begin () {
                return iterator2 ((*this) (), it1_, it2_ ().begin ());
            }
            NUMERICS_INLINE
            iterator2 end () {
                return iterator2 ((*this) (), it1_, it2_ ().end ()); 
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                return it1_.index ();
            }
            NUMERICS_INLINE
            size_type index2 () const {
                return it2_.index ();
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const iterator1 &it) const {
                check<external_logic>::precondition (&(*this) () == &it ());
                return it1_ == it.it1_ && it2_ == it.it2_;
            }

        private:
            iterator1_type it1_;
            iterator2_type it2_;

            friend class const_iterator1;
        };

        NUMERICS_INLINE
        iterator1 begin1 () {
            return iterator1 (data_, s1_.begin (), s2_.begin ()); 
        }
        NUMERICS_INLINE
        iterator1 end1 () {
            return iterator1 (data_, s1_.end (), s2_.begin ()); 
        }

        class const_iterator2:
            public container_const_reference<matrix_type>,
            public random_access_iterator_base<const_iterator2, value_type> {
        public:
            typedef typename M::const_iterator2::iterator_category iterator_category;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator2 (const matrix_type &m, const const_iterator1_type &it1, const const_iterator2_type &it2):
                container_const_reference<matrix_type> (m), it1_ (it1), it2_ (it2) {}
            NUMERICS_INLINE
            const_iterator2 (const iterator2 &it):
                container_const_reference<matrix_type> (it ()), it1_ (it.it1_), it2_ (it.it2_) {}

            // Arithmetic
            NUMERICS_INLINE
            const_iterator2 &operator ++ () {
                ++ it2_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator2 &operator -- () {
                -- it2_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator2 &operator += (difference_type n) {
                it2_ += n;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator2 &operator -= (difference_type n) {
                it2_ -= n;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const const_iterator2 &it) const {
                return it2_ - it.it2_;
            }

            // Dereference
            NUMERICS_INLINE
            value_type operator * () const {
                return (*this) () (*it1_, *it2_); 
            }

            NUMERICS_INLINE
            const_iterator1 begin () const {
                return const_iterator1 ((*this) (), it1_ ().begin (), it2_); 
            }
            NUMERICS_INLINE
            const_iterator1 end () const {
                return const_iterator1 ((*this) (), it1_ ().end (), it2_); 
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                return it1_.index ();
            }
            NUMERICS_INLINE
            size_type index2 () const {
                return it2_.index ();
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator2 &it) const {
                check<external_logic>::precondition (&(*this) () == &it ());
                return it1_ == it.it1_ && it2_ == it.it2_;
            }

        private:
            const_iterator1_type it1_;
            const_iterator2_type it2_;
        };

        NUMERICS_INLINE
        const_iterator2 begin2 () const {
            return const_iterator2 (data_, s1_.begin (), s2_.begin ()); 
        }
        NUMERICS_INLINE
        const_iterator2 end2 () const {
            return const_iterator2 (data_, s1_.begin (), s2_.end ()); 
        }

        class iterator2:
            public container_reference<matrix_type>,
            public random_access_iterator_base<iterator2, value_type> {
        public:
            typedef typename M::iterator2::iterator_category iterator_category;

            // Construction and destruction
            NUMERICS_INLINE
            iterator2 (matrix_type &m, const iterator1_type &it1, const iterator2_type &it2):
                container_reference<matrix_type> (m), it1_ (it1), it2_ (it2) {}

            // Arithmetic
            NUMERICS_INLINE
            iterator2 &operator ++ () {
                ++ it2_;
                return *this;
            }
            NUMERICS_INLINE
            iterator2 &operator -- () {
                -- it2_;
                return *this;
            }
            NUMERICS_INLINE
            iterator2 &operator += (difference_type n) {
                it2_ += n;
                return *this;
            }
            NUMERICS_INLINE
            iterator2 &operator -= (difference_type n) {
                it2_ -= n;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const iterator2 &it) const {
                return it2_ - it.it2_;
            }

            // Dereference
            NUMERICS_INLINE
            reference_type operator * () {
                return (*this) () (*it1_, *it2_); 
            }

            NUMERICS_INLINE
            iterator1 begin () const {
                return iterator1 ((*this) (), it1_ ().begin (), it2_); 
            }
            NUMERICS_INLINE
            iterator1 end () const {
                return iterator1 ((*this) (), it1_ ().end (), it2_); 
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                return it1_.index ();
            }
            NUMERICS_INLINE
            size_type index2 () const {
                return it2_.index ();
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const iterator2 &it) const {
                check<external_logic>::precondition (&(*this) () == &it ());
                return it1_ == it.it1_ && it2_ == it.it2_;
            }

        private:
            iterator1_type it1_;
            iterator2_type it2_;

            friend class const_iterator2;
        };

        NUMERICS_INLINE
        iterator2 begin2 () {
            return iterator1 (data_, s1_.begin (), s2_.begin ()); 
        }
        NUMERICS_INLINE
        iterator2 end2 () {
            return iterator1 (data_, s1_.begin (), s2_.end ()); 
        }

    private:
        matrix_type &data_;
        slice s1_;
        slice s2_;
    };

}

#endif 


