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
#include "vector.h"
#include "matrix_et.h"
#include "matrix_pr.h"

// Iterators based on ideas of Jeremy Siek

namespace numerics {

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

        static 
        NUMERICS_INLINE
        size_type element1 (size_type i, size_type size1, size_type j, size_type size2) {
            i = functor1_type::element (i, size1);
            check<bad_index>::precondition (i <= size1);
            return i;
        }
        static 
        NUMERICS_INLINE
        size_type element2 (size_type i, size_type size1, size_type j, size_type size2) {
            j = functor2_type::element (j, size2);
            check<bad_index>::precondition (j <= size2);
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

        static 
        NUMERICS_INLINE
        size_type element1 (size_type i, size_type size1, size_type j, size_type size2) {
            j = functor2_type::element (j, size2);
            check<bad_index>::precondition (j <= size2);
            return j;
        }
        static 
        NUMERICS_INLINE
        size_type element2 (size_type i, size_type size1, size_type j, size_type size2) {
            i = functor1_type::element (i, size1);
            check<bad_index>::precondition (i <= size1);
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
            // FIXME: switch to indexing version, if possible.
            typedef typename M::difference_type difference_type;
            typename M::iterator1 it1 (m.begin1 ());
            difference_type size1 (m.end1 () - it1);
            while (-- size1 >= 0) {
                typename M::iterator2 it2 (it1.begin ());
                difference_type size2 (it1.end () - it2); 
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
        // Sparse (proxy) column major case
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
            // FIXME: switch to indexing version, if possible.
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
                            check<bad_index>::invariant (*it2e == value_type ());
                        ++ it2e;
                    }
                    check<bad_size>::postcondition (it2 == it2_end);
                    check<bad_size>::postcondition (it2e == it2e_end);
                    ++ it1;
                } else {
                    typename E::const_iterator2 it2e (it1e.begin ());
                    typename E::const_iterator2 it2e_end (it1e.end ());
                    while (it2e != it2e_end) {
                        check<bad_index>::invariant (*it2e == value_type ());
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
                            check<bad_index>::invariant (*it1e == value_type ());
                        ++ it1e;
                    }
                    check<bad_size>::postcondition (it1 == it1_end);
                    check<bad_size>::postcondition (it1e == it1e_end);
                    ++ it2;
                } else {
                    typename E::const_iterator1 it1e (it2e.begin ());
                    typename E::const_iterator1 it1e_end (it2e.end ());
                    while (it1e != it1e_end) {
                        check<bad_index>::invariant (*it1e == value_type ());
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
    template<class T, class F, class A>
    class matrix: 
        public matrix_expression<matrix<T, F, A> > {
    public:      
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef const T &const_reference_type;
        typedef T &reference_type;
        typedef F functor_type;
        typedef A array_type;
        typedef const matrix<T, F, A> const_self_type;
        typedef matrix<T, F, A> self_type;
        typedef const matrix_const_reference<const_self_type> const_closure_type;
        typedef matrix_reference<self_type> closure_type;
        typedef const matrix_row<const_self_type> const_matrix_row_type;
        typedef matrix_row<self_type> matrix_row_type;
        typedef const matrix_column<const_self_type> const_matrix_column_type;
        typedef matrix_column<self_type> matrix_column_type;
        typedef const matrix_range<const_self_type> const_matrix_range_type;
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
        const_reference_type operator () (size_type i, size_type j) const {
            return data_ [functor_type::element (i, size1_, j, size2_)]; 
        }
        NUMERICS_INLINE
        reference_type operator () (size_type i, size_type j) {
            return data_ [functor_type::element (i, size1_, j, size2_)]; 
        }

        NUMERICS_INLINE
        const_matrix_row_type operator [] (size_type i) const {
            return const_matrix_row_type (*this, i);
        }
        NUMERICS_INLINE
        matrix_row_type operator [] (size_type i) {
            return matrix_row_type (*this, i);
        }
        NUMERICS_INLINE
        const_matrix_row_type row (size_type i) const {
            return const_matrix_row_type (*this, i);
        }
        NUMERICS_INLINE
        matrix_row_type row (size_type i) {
            return matrix_row_type (*this, i);
        }
        NUMERICS_INLINE
        const_matrix_column_type column (size_type j) const {
            return const_matrix_column_type (*this, j);
        }
        NUMERICS_INLINE
        matrix_column_type column (size_type j) {
            return matrix_column_type (*this, j);
        }

        NUMERICS_INLINE
        const_matrix_range_type project (size_type start1, size_type stop1, size_type start2, size_type stop2) const {
            return const_matrix_range_type (*this, start1, stop1, start2, stop2);
        }
        NUMERICS_INLINE
        const_matrix_range_type project (const range &r1, const range &r2) const {
            return const_matrix_range_type (*this, r1, r2);
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
            data_.fill (0);
        }
        NUMERICS_INLINE
        void insert (size_type i, size_type j, const_reference_type t) {
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
#ifdef USE_GCC
            typedef typename matrix::difference_type difference_type;
            typedef typename matrix::value_type value_type;
#endif

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator1 ():
                container_const_reference<matrix> (), it_ () {}
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
            const_reference_type operator * () const {
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
#ifdef USE_GCC
            typedef typename matrix::difference_type difference_type;
            typedef typename matrix::value_type value_type;
#endif

            // Construction and destruction
            NUMERICS_INLINE
            iterator1 ():
                container_reference<matrix> (), it_ () {}
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
#ifdef USE_GCC
            typedef typename matrix::difference_type difference_type;
            typedef typename matrix::value_type value_type;
#endif

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator2 ():
                container_const_reference<matrix> (), it_ () {}
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
            const_reference_type operator * () const {
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
#ifdef USE_GCC
            typedef typename matrix::difference_type difference_type;
            typedef typename matrix::value_type value_type;
#endif

            // Construction and destruction
            NUMERICS_INLINE
            iterator2 ():
                container_reference<matrix> (), it_ () {}
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

    // Array based matrix class 
    template<class T, class F, class A>
    class vector_of_vector: 
        public matrix_expression<vector_of_vector<T, F, A> > {
    public:      
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef const T &const_reference_type;
        typedef T &reference_type;
        typedef F functor_type;
        typedef A array_type;
        typedef const vector_of_vector<T, F, A> const_self_type;
        typedef vector_of_vector<T, F, A> self_type;
        typedef const matrix_const_reference<const_self_type> const_closure_type;
        typedef matrix_reference<self_type> closure_type;
        typedef const matrix_row<const_self_type> const_matrix_row_type;
        typedef matrix_row<self_type> matrix_row_type;
        typedef const matrix_column<const_self_type> const_matrix_column_type;
        typedef matrix_column<self_type> matrix_column_type;
        typedef const matrix_range<const_self_type> const_matrix_range_type;
        typedef matrix_range<self_type> matrix_range_type;
        typedef typename A::const_iterator vector_const_iterator_type;
        typedef typename A::iterator vector_iterator_type;
        typedef typename A::value_type::const_iterator const_iterator_type;
        typedef typename A::value_type::iterator iterator_type;
        typedef struct dense_tag storage_category;
        typedef struct major_tag orientation_category;

        // Construction and destruction
        NUMERICS_INLINE
        vector_of_vector (): 
            size1_ (0), size2_ (0), data_ (1) {}
        NUMERICS_INLINE
        vector_of_vector (size_type size1, size_type size2): 
            size1_ (size1), size2_ (size2), 
            data_ (functor_type::size1 (size1, size2) + 1) {
            for (size_type k = 0; k < functor_type::size1 (size1, size2); ++ k) 
                data_ [k].resize (functor_type::size2 (size1, size2));
        }
        NUMERICS_INLINE
        vector_of_vector (const vector_of_vector &m): 
            size1_ (m.size1_), size2_ (m.size2_), data_ (m.data_) {}
        template<class AE>
        NUMERICS_EXPLICIT NUMERICS_INLINE
        vector_of_vector (const matrix_expression<AE> &ae): 
            size1_ (ae ().size1 ()), size2_ (ae ().size2 ()),
            data_ (functor_type::size1 (ae ().size1 (), ae ().size2 ()) + 1) { 
            for (size_type k = 0; k < functor_type::size1 (ae ().size1 (), ae ().size2 ()); ++ k) 
                data_ [k].resize (functor_type::size2 (ae ().size1 (), ae ().size2 ()));
            matrix_assign<scalar_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
        }

        // Resizing
        NUMERICS_INLINE
        void resize (size_type size1, size_type size2) {
            size1_ = size1;
            size2_ = size2;
            data_.resize (functor_type::size1 (size1, size2) + 1);
            for (size_type k = 0; k < functor_type::size1 (size1, size2); ++ k) 
                data_ [k].resize (functor_type::size2 (size1, size2));
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
        const_reference_type operator () (size_type i, size_type j) const {
            return data_ [functor_type::element1 (i, size1_, j, size2_)] [functor_type::element2 (i, size1_, j, size2_)]; 
        }
        NUMERICS_INLINE
        reference_type operator () (size_type i, size_type j) {
            return data_ [functor_type::element1 (i, size1_, j, size2_)] [functor_type::element2 (i, size1_, j, size2_)]; 
        }

        NUMERICS_INLINE
        const_matrix_row_type operator [] (size_type i) const {
            return const_matrix_row_type (*this, i);
        }
        NUMERICS_INLINE
        matrix_row_type operator [] (size_type i) {
            return matrix_row_type (*this, i);
        }
        NUMERICS_INLINE
        const_matrix_row_type row (size_type i) const {
            return const_matrix_row_type (*this, i);
        }
        NUMERICS_INLINE
        matrix_row_type row (size_type i) {
            return matrix_row_type (*this, i);
        }
        NUMERICS_INLINE
        const_matrix_column_type column (size_type j) const {
            return const_matrix_column_type (*this, j);
        }
        NUMERICS_INLINE
        matrix_column_type column (size_type j) {
            return matrix_column_type (*this, j);
        }

        NUMERICS_INLINE
        const_matrix_range_type project (size_type start1, size_type stop1, size_type start2, size_type stop2) const {
            return const_matrix_range_type (*this, start1, stop1, start2, stop2);
        }
        NUMERICS_INLINE
        const_matrix_range_type project (const range &r1, const range &r2) const {
            return const_matrix_range_type (*this, r1, r2);
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
        vector_of_vector &operator = (const vector_of_vector &m) { 
            check<bad_size>::precondition (size1_ == m.size1_);
            check<bad_size>::precondition (size2_ == m.size2_);
            data_ = m.data_;
            return *this;
        }
        NUMERICS_INLINE
        vector_of_vector &assign_temporary (vector_of_vector &m) { 
            swap (m);
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        vector_of_vector &operator = (const matrix_expression<AE> &ae) { 
#ifndef USE_GCC
            return assign_temporary (self_type (ae));
#else
            return assign (self_type (ae));
#endif
        }
        template<class AE>
        NUMERICS_INLINE
        vector_of_vector &assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        vector_of_vector& operator += (const matrix_expression<AE> &ae) {
#ifndef USE_GCC
            return assign_temporary (self_type (*this + ae));
#else
            return assign (self_type (*this + ae));
#endif
        }
        template<class AE>
        NUMERICS_INLINE
        vector_of_vector &plus_assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_plus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        vector_of_vector& operator -= (const matrix_expression<AE> &ae) {
#ifndef USE_GCC
            return assign_temporary (self_type (*this - ae));
#else
            return assign (self_type (*this - ae));
#endif
        }
        template<class AE>
        NUMERICS_INLINE
        vector_of_vector &minus_assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_minus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        vector_of_vector& operator *= (const AT &at) {
            matrix_assign_scalar<scalar_multiplies_assign<value_type, AT> > () (*this, at);
            return *this;
        }

        // Swapping
        NUMERICS_INLINE
	    void swap (vector_of_vector &m) {
            check<external_logic>::precondition (this != &m);
            check<bad_size>::precondition (size1_ == m.size1_);
            check<bad_size>::precondition (size2_ == m.size2_);
            std::swap (size1_, m.size1_);
            std::swap (size2_, m.size2_);
            data_.swap (m.data_);
        }
#ifndef USE_GCC
        NUMERICS_INLINE
	    friend void swap (vector_of_vector &m1, vector_of_vector &m2) {
            m1.swap (m2);
        }
#endif

        // Element insertion
        NUMERICS_INLINE
        void clear () {
            for (size_type k = 0; k < functor_type::size1 (size1_, size2_); ++ k) 
                data_ [k].fill (0);
        }
        NUMERICS_INLINE
        void insert (size_type i, size_type j, const_reference_type t) {
            data_ [functor_type::element1 (i, size1_, j, size2_)] [functor_type::element2 (i, size1_, j, size2_)] = t; 
        }

        class const_iterator1;
        class iterator1;
        class const_iterator2;
        class iterator2;

        // Element lookup
        NUMERICS_INLINE
        const_iterator1 find1 (size_type i, size_type j) const {
            return const_iterator1 (*this, i, j, data_ [functor_type::element1 (i, size1_, j, size2_)].begin ()  + functor_type::element2 (i, size1_, j, size2_));
        }
        NUMERICS_INLINE
        iterator1 find1 (size_type i, size_type j) {
            return iterator1 (*this, i, j, data_ [functor_type::element1 (i, size1_, j, size2_)].begin ()  + functor_type::element2 (i, size1_, j, size2_));
        }
        NUMERICS_INLINE
        const_iterator2 find2 (size_type i, size_type j) const {
            return const_iterator2 (*this, i, j, data_ [functor_type::element1 (i, size1_, j, size2_)].begin ()  + functor_type::element2 (i, size1_, j, size2_));
        }
        NUMERICS_INLINE
        iterator2 find2 (size_type i, size_type j) {
            return iterator2 (*this, i, j, data_ [functor_type::element1 (i, size1_, j, size2_)].begin () + functor_type::element2 (i, size1_, j, size2_));
        }

        // Iterators simply are pointers.

        class const_iterator1:
            public container_const_reference<vector_of_vector>,
            public random_access_iterator_base<const_iterator1, value_type> {
        public:
            typedef std::random_access_iterator_tag iterator_category;
#ifdef USE_GCC
            typedef typename vector_of_vector::difference_type difference_type;
            typedef typename vector_of_vector::value_type value_type;
#endif
            typedef typename functor_type::functor1_type functor1_type;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator1 ():
                container_const_reference<vector_of_vector> (), i_ (), j_ (), it_ () {}
            NUMERICS_INLINE
            const_iterator1 (const vector_of_vector &m, size_type i, size_type j, const const_iterator_type &it):
                container_const_reference<vector_of_vector> (m), i_ (i), j_ (j), it_ (it) {}
            NUMERICS_INLINE
            const_iterator1 (const iterator1 &it):
                container_const_reference<vector_of_vector> (it ()), i_ (it.i_), j_ (it.j_), it_ (it.it_) {}

            // Arithmetic
            NUMERICS_INLINE
            const_iterator1 &operator ++ () {
                ++ i_;
                const vector_of_vector &m = (*this) ();
                difference_type o (functor_type::one1 (m.size1 (), m.size2 ()));
                switch (o) {
                case 1:
                case -1:
                    functor1_type::increment (it_);
                    break;
                default:
                    it_ = m.find1 (i_, j_).it_;
                }
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator -- () {
                -- i_;
                const vector_of_vector &m = (*this) ();
                difference_type o (functor_type::one1 (m.size1 (), m.size2 ()));
                switch (o) {
                case 1:
                case -1:
                    functor1_type::decrement (it_);
                    break;
                default:
                    it_ = m.find1 (i_, j_).it_;
                }
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator += (difference_type n) {
                i_ += n;
                const vector_of_vector &m = (*this) ();
                it_ = m.find1 (i_, j_).it_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator -= (difference_type n) {
                i_ -= n;
                const vector_of_vector &m = (*this) ();
                it_ = m.find1 (i_, j_).it_;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const const_iterator1 &it) const {
                check<bad_index>::precondition (index2 () == it.index2 ());
                return index1 () - it.index1 ();
            }

            // Dereference
            NUMERICS_INLINE
            const_reference_type operator * () const {
                check<bad_index>::precondition (index1 () < (*this) ().size1 ());
                check<bad_index>::precondition (index2 () < (*this) ().size2 ());
                return *it_;
            }

            NUMERICS_INLINE
            const_iterator2 begin () const {
                const vector_of_vector &m = (*this) ();
                return m.find2 (index1 (), 0); 
            }
            NUMERICS_INLINE
            const_iterator2 end () const {
                const vector_of_vector &m = (*this) ();
                return m.find2 (index1 (), m.size2 ()); 
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                return i_;
            }
            NUMERICS_INLINE
            size_type index2 () const {
                return j_;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator1 &it) const {
                check<external_logic>::precondition (&(*this) () == &it ());
                return it_ == it.it_;
            }

        private:
            size_type i_;
            size_type j_;
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
            public container_reference<vector_of_vector>,
            public random_access_iterator_base<iterator1, value_type> {
        public:
            typedef std::random_access_iterator_tag iterator_category;
#ifdef USE_GCC
            typedef typename vector_of_vector::difference_type difference_type;
            typedef typename vector_of_vector::value_type value_type;
#endif
            typedef typename functor_type::functor1_type functor1_type;

            // Construction and destruction
            NUMERICS_INLINE
            iterator1 ():
                container_reference<vector_of_vector> (), i_ (), j_ (), it_ () {}
            NUMERICS_INLINE
            iterator1 (vector_of_vector &m, size_type i, size_type j, const iterator_type &it):
                container_reference<vector_of_vector> (m), i_ (i), j_ (j), it_ (it) {}

            // Arithmetic
            NUMERICS_INLINE
            iterator1 &operator ++ () {
                ++ i_;
                vector_of_vector &m = (*this) ();
                difference_type o (functor_type::one1 (m.size1 (), m.size2 ()));
                switch (o) {
                case 1:
                case -1:
                    functor1_type::increment (it_);
                    break;
                default:
                    it_ = m.find1 (i_, j_).it_;
                }
                return *this;
            }
            NUMERICS_INLINE
            iterator1 &operator -- () {
                -- i_;
                vector_of_vector &m = (*this) ();
                difference_type o (functor_type::one1 (m.size1 (), m.size2 ()));
                switch (o) {
                case 1:
                case -1:
                    functor1_type::decrement (it_);
                    break;
                default:
                    it_ = m.find1 (i_, j_).it_;
                }
                return *this;
            }
            NUMERICS_INLINE
            iterator1 &operator += (difference_type n) {
                i_ += n;
                vector_of_vector &m = (*this) ();
                it_ = m.find1 (i_, j_).it_;
                return *this;
            }
            NUMERICS_INLINE
            iterator1 &operator -= (difference_type n) {
                i_ -= n;
                vector_of_vector &m = (*this) ();
                it_ = m.find1 (i_, j_).it_;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const iterator1 &it) const {
                check<bad_index>::precondition (index2 () == it.index2 ());
                return index1 () - it.index1 ();
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
                vector_of_vector &m = (*this) ();
                return m.find2 (index1 (), 0);
            }
            NUMERICS_INLINE
            iterator2 end () {
                vector_of_vector &m = (*this) ();
                return m.find2 (index1 (), m.size2 ());
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                return i_;
            }
            NUMERICS_INLINE
            size_type index2 () const {
                return j_;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const iterator1 &it) const {
                check<external_logic>::precondition (&(*this) () == &it ());
                return it_ == it.it_;
            }

        private:
            size_type i_;
            size_type j_;
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
            public container_const_reference<vector_of_vector>,
            public random_access_iterator_base<const_iterator2, value_type> {
        public:
            typedef std::random_access_iterator_tag iterator_category;
#ifdef USE_GCC
            typedef typename vector_of_vector::difference_type difference_type;
            typedef typename vector_of_vector::value_type value_type;
#endif
            typedef typename functor_type::functor2_type functor2_type;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator2 ():
                container_const_reference<vector_of_vector> (), i_ (), j_ (), it_ () {}
            NUMERICS_INLINE
            const_iterator2 (const vector_of_vector &m, size_type i, size_type j, const const_iterator_type &it):
                container_const_reference<vector_of_vector> (m), i_ (i), j_ (j), it_ (it) {}
            NUMERICS_INLINE
            const_iterator2 (const iterator2 &it):
                container_const_reference<vector_of_vector> (it ()), i_ (it.i_), j_ (it.j_), it_ (it.it_) {}

            // Arithmetic
            NUMERICS_INLINE
            const_iterator2 &operator ++ () {
                ++ j_;
                const vector_of_vector &m = (*this) ();
                difference_type o (functor_type::one2 (m.size1 (), m.size2 ()));
                switch (o) {
                case 1:
                case -1:
                    functor2_type::increment (it_);
                    break;
                default:
                    it_ = m.find2 (i_, j_).it_;
                }
                return *this;
            }
            NUMERICS_INLINE
            const_iterator2 &operator -- () {
                -- j_;
                const vector_of_vector &m = (*this) ();
                difference_type o (functor_type::one2 (m.size1 (), m.size2 ()));
                switch (o) {
                case 1:
                case -1:
                    functor2_type::decrement (it_);
                    break;
                default:
                    it_ = m.find2 (i_, j_).it_;
                }
                return *this;
            }
            NUMERICS_INLINE
            const_iterator2 &operator += (difference_type n) {
                j_ += n;
                const vector_of_vector &m = (*this) ();
                it_ = m.find2 (i_, j_).it_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator2 &operator -= (difference_type n) {
                j_ -= n;
                const vector_of_vector &m = (*this) ();
                it_ = m.find2 (i_, j_).it_;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const const_iterator2 &it) const {
                check<bad_index>::precondition (index1 () == it.index1 ());
                return index2 () - it.index2 ();
            }

            // Dereference
            NUMERICS_INLINE
            const_reference_type operator * () const {
                check<bad_index>::precondition (index1 () < (*this) ().size1 ());
                check<bad_index>::precondition (index2 () < (*this) ().size2 ());
                return *it_;
            }

            NUMERICS_INLINE
            const_iterator1 begin () const {
                const vector_of_vector &m = (*this) ();
                return m.find1 (0, index2 ()); 
            }
            NUMERICS_INLINE
            const_iterator1 end () const {
                const vector_of_vector &m = (*this) ();
                return m.find1 (m.size1 (), index2 ()); 
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                return i_;
            }
            NUMERICS_INLINE
            size_type  index2 () const {
                return j_;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator2 &it) const {
                check<external_logic>::precondition (&(*this) () == &it ());
                return it_ == it.it_;
            }

        private:
            size_type i_;
            size_type j_;
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
            public container_reference<vector_of_vector>,
            public random_access_iterator_base<iterator2, value_type> {
        public:
            typedef std::random_access_iterator_tag iterator_category;
#ifdef USE_GCC
            typedef typename vector_of_vector::difference_type difference_type;
            typedef typename vector_of_vector::value_type value_type;
#endif
            typedef typename functor_type::functor2_type functor2_type;

            // Construction and destruction
            NUMERICS_INLINE
            iterator2 ():
                container_reference<vector_of_vector> (), i_ (), j_ (), it_ () {}
            NUMERICS_INLINE
            iterator2 (vector_of_vector &m, size_type i, size_type j, const iterator_type &it):
                container_reference<vector_of_vector> (m), i_ (i), j_ (j), it_ (it) {}

            // Arithmetic
            NUMERICS_INLINE
            iterator2 &operator ++ () {
                ++ j_;
                vector_of_vector &m = (*this) ();
                difference_type o (functor_type::one2 (m.size1 (), m.size2 ()));
                switch (o) {
                case 1:
                case -1:
                    functor2_type::increment (it_);
                    break;
                default:
                    it_ = m.find2 (i_, j_).it_;
                }
                return *this;
            }
            NUMERICS_INLINE
            iterator2 &operator -- () {
                -- j_;
                vector_of_vector &m = (*this) ();
                difference_type o (functor_type::one2 (m.size1 (), m.size2 ()));
                switch (o) {
                case 1:
                case -1:
                    functor2_type::decrement (it_);
                    break;
                default:
                    it_ = m.find2 (i_, j_).it_;
                }
                return *this;
            }
            NUMERICS_INLINE
            iterator2 &operator += (difference_type n) {
                j_ += n;
                vector_of_vector &m = (*this) ();
                it_ = m.find2 (i_, j_).it_;
                return *this;
            }
            NUMERICS_INLINE
            iterator2 &operator -= (difference_type n) {
                j_ -= n;
                vector_of_vector &m = (*this) ();
                it_ = m.find2 (i_, j_).it_;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const iterator2 &it) const {
                check<bad_index>::precondition (index1 () == it.index1 ());
                return index2 () - it.index2 ();
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
                vector_of_vector &m = (*this) ();
                return m.find1 (0, index2 ());
            }
            NUMERICS_INLINE
            iterator1 end () {
                vector_of_vector &m = (*this) ();
                return m.find1 (m.size1 (), index2 ()); 
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                return i_;
            }
            NUMERICS_INLINE
            size_type index2 () const {
                return j_;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const iterator2 &it) const {
                check<external_logic>::precondition (&(*this) () == &it ());
                return it_ == it.it_;
            }

        private:
            size_type i_;
            size_type j_;
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

    // Identity matrix class
    template<class T>
    class identity_matrix: 
        public matrix_expression<identity_matrix<T> > {
    public:      
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef const T &const_reference_type;
        typedef T &reference_type;
        typedef const identity_matrix<T> const_self_type;
        typedef identity_matrix<T> self_type;
        typedef const matrix_const_reference<const_self_type> const_closure_type;
        typedef const matrix_row<const_self_type> const_matrix_row_type;
        typedef const matrix_column<const_self_type> const_matrix_column_type;
        typedef const matrix_range<const_self_type> const_matrix_range_type;
        typedef size_type const_iterator_type;
        typedef struct dense_tag storage_category;
        typedef struct major_tag orientation_category;

        // Construction and destruction
        NUMERICS_INLINE
        identity_matrix (): 
            size1_ (0), size2_ (0) {}
        NUMERICS_INLINE
        identity_matrix (size_type size): 
            size1_ (size), size2_ (size) {}
        NUMERICS_INLINE
        identity_matrix (size_type size1, size_type size2): 
            size1_ (size1), size2_ (size2) {}
        NUMERICS_INLINE
        identity_matrix (const identity_matrix &m): 
            size1_ (m.size1_), size2_ (m.size2_) {}

        // Resizing
        NUMERICS_INLINE
        void resize (size_type size1, size_type size2) {
            size1_ = size1;
            size2_ = size2;
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
            return i == j; 
        }

        NUMERICS_INLINE
        const_matrix_row_type operator [] (size_type i) const {
            return const_matrix_row_type (*this, i);
        }
        NUMERICS_INLINE
        const_matrix_row_type row (size_type i) const {
            return const_matrix_row_type (*this, i);
        }
        NUMERICS_INLINE
        const_matrix_column_type column (size_type j) const {
            return const_matrix_column_type (*this, j);
        }

        NUMERICS_INLINE
        const_matrix_range_type project (size_type start1, size_type stop1, size_type start2, size_type stop2) const {
            return const_matrix_range_type (*this, start1, stop1, start2, stop2);
        }
        NUMERICS_INLINE
        const_matrix_range_type project (const range &r1, const range &r2) const {
            return const_matrix_range_type (*this, r1, r2);
        }

        // Assignment
        NUMERICS_INLINE
        identity_matrix &operator = (const identity_matrix &m) { 
            check<bad_size>::precondition (size1_ == m.size1_);
            check<bad_size>::precondition (size2_ == m.size2_);
            return *this;
        }
        NUMERICS_INLINE
        identity_matrix &assign_temporary (identity_matrix &m) { 
            swap (m);
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        identity_matrix &operator = (const matrix_expression<AE> &ae) { 
#ifndef USE_GCC
            return assign_temporary (self_type (ae));
#else
            return assign (self_type (ae));
#endif
        }
        template<class AE>
        NUMERICS_INLINE
        identity_matrix &assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        identity_matrix& operator += (const matrix_expression<AE> &ae) {
#ifndef USE_GCC
            return assign_temporary (self_type (*this + ae));
#else
            return assign (self_type (*this + ae));
#endif
        }
        template<class AE>
        NUMERICS_INLINE
        identity_matrix &plus_assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_plus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        identity_matrix& operator -= (const matrix_expression<AE> &ae) {
#ifndef USE_GCC
            return assign_temporary (self_type (*this - ae));
#else
            return assign (self_type (*this - ae));
#endif
        }
        template<class AE>
        NUMERICS_INLINE
        identity_matrix &minus_assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_minus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        identity_matrix& operator *= (const AT &at) {
            matrix_assign_scalar<scalar_multiplies_assign<value_type, AT> > () (*this, at);
            return *this;
        }

        // Swapping
        NUMERICS_INLINE
	    void swap (identity_matrix &m) {
            check<external_logic>::precondition (this != &m);
            check<bad_size>::precondition (size1_ == m.size1_);
            check<bad_size>::precondition (size2_ == m.size2_);
            std::swap (size1_, m.size1_);
            std::swap (size2_, m.size2_);
        }
#ifndef USE_GCC
        NUMERICS_INLINE
	    friend void swap (identity_matrix &m1, identity_matrix &m2) {
            m1.swap (m2);
        }
#endif

        class const_iterator1;
        class const_iterator2;

        // Element lookup
        NUMERICS_INLINE
        const_iterator1 find1 (size_type i, size_type j) const {
            return const_iterator1 (*this, i, j);
        }
        NUMERICS_INLINE
        const_iterator2 find2 (size_type i, size_type j) const {
            return const_iterator2 (*this, i, j);
        }

        // Iterators simply are pointers.

        class const_iterator1:
            public container_const_reference<identity_matrix>,
            public random_access_iterator_base<const_iterator1, value_type> {
        public:
            typedef std::random_access_iterator_tag iterator_category;
#ifdef USE_GCC
            typedef typename identity_matrix::difference_type difference_type;
            typedef typename identity_matrix::value_type value_type;
#endif

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator1 ():
                container_const_reference<identity_matrix> (), it1_ (), it2_ () {}
            NUMERICS_INLINE
            const_iterator1 (const identity_matrix &m, const const_iterator_type &it1, const const_iterator_type &it2):
                container_const_reference<identity_matrix> (m), it1_ (it1), it2_ (it2) {}

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
                check<bad_index>::precondition (index1 () < (*this) ().size1 ());
                check<bad_index>::precondition (index2 () < (*this) ().size2 ());
                return it1_ == it2_;
            }

            NUMERICS_INLINE
            const_iterator2 begin () const {
                const identity_matrix &m = (*this) ();
                return m.find2 (index1 (), 0); 
            }
            NUMERICS_INLINE
            const_iterator2 end () const {
                const identity_matrix &m = (*this) ();
                return m.find2 (index1 (), m.size2 ()); 
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                return it1_;
            }
            NUMERICS_INLINE
            size_type index2 () const {
                return it2_;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator1 &it) const {
                check<external_logic>::precondition (&(*this) () == &it ());
                return it1_ == it.it1_ && it2_ == it.it2_ ;
            }

        private:
            const_iterator_type it1_;
            const_iterator_type it2_;
        };

        NUMERICS_INLINE
        const_iterator1 begin1 () const {
            return find1 (0, 0);
        }
        NUMERICS_INLINE
        const_iterator1 end1 () const {
            return find1 (size1_, 0);
        }

        class const_iterator2:
            public container_const_reference<identity_matrix>,
            public random_access_iterator_base<const_iterator2, value_type> {
        public:
            typedef std::random_access_iterator_tag iterator_category;
#ifdef USE_GCC
            typedef typename identity_matrix::difference_type difference_type;
            typedef typename identity_matrix::value_type value_type;
#endif

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator2 ():
                container_const_reference<identity_matrix> (), it1_ (), it2_ () {}
            NUMERICS_INLINE
            const_iterator2 (const identity_matrix &m, const const_iterator_type &it1, const const_iterator_type &it2):
                container_const_reference<identity_matrix> (m), it1_ (it1), it2_ (it2) {}

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
                check<bad_index>::precondition (index1 () < (*this) ().size1 ());
                check<bad_index>::precondition (index2 () < (*this) ().size2 ());
                return it1_ == it2_;
            }

            NUMERICS_INLINE
            const_iterator1 begin () const {
                const identity_matrix &m = (*this) ();
                return m.find1 (0, index2 ()); 
            }
            NUMERICS_INLINE
            const_iterator1 end () const {
                const identity_matrix &m = (*this) ();
                return m.find1 (m.size1 (), index2 ()); 
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                return it1_;
            }
            NUMERICS_INLINE
            size_type  index2 () const {
                return it2_;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator2 &it) const {
                check<external_logic>::precondition (&(*this) () == &it ());
                return it1_ == it.it1_ && it2_ == it.it2_;
            }

        private:
            const_iterator_type it1_;
            const_iterator_type it2_;
        };

        NUMERICS_INLINE
        const_iterator2 begin2 () const {
            return find2 (0, 0);
        }
        NUMERICS_INLINE
        const_iterator2 end2 () const {
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

#endif

    private:
        size_type size1_;
        size_type size2_;
    };

    // Array based matrix class 
    template<class T, std::size_t N, std::size_t M>
    class c_matrix: 
        public matrix_expression<c_matrix<T, N, M> > {
    public:      
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef const T &const_reference_type;
        typedef T &reference_type;
        typedef const c_matrix<T, N, M> const_self_type;
        typedef c_matrix<T, N, M> self_type;
        typedef const matrix_const_reference<const_self_type> const_closure_type;
        typedef matrix_reference<self_type> closure_type;
        typedef const matrix_row<const_self_type> const_matrix_row_type;
        typedef matrix_row<self_type> matrix_row_type;
        typedef const matrix_column<const_self_type> const_matrix_column_type;
        typedef matrix_column<self_type> matrix_column_type;
        typedef const matrix_range<const_self_type> const_matrix_range_type;
        typedef matrix_range<self_type> matrix_range_type;
        typedef const T *const_iterator_type;
        typedef T *iterator_type;
        typedef struct dense_tag storage_category;
        typedef struct major_tag orientation_category;

        // Construction and destruction
        NUMERICS_INLINE
        c_matrix (): 
            size1_ (N), size2_ (M) /* , data_ () */ {
        }
        NUMERICS_INLINE
        c_matrix (size_type size1, size_type size2): 
            size1_ (size1), size2_ (size2) /* , data_ () */ {
            if (size1_ > N || size2_ > M) 
                throw std::bad_alloc ();
        }
        NUMERICS_INLINE
        c_matrix (const c_matrix &m): 
            size1_ (m.size1_), size2_ (m.size2_) /* , data_ () */ {
            if (size1_ > N || size2_ > M) 
                throw std::bad_alloc ();
            *this = m;
        }
        template<class AE>
        NUMERICS_EXPLICIT NUMERICS_INLINE
        c_matrix (const matrix_expression<AE> &ae): 
            size1_ (ae ().size1 ()), size2_ (ae ().size2 ()) /* , data_ () */ { 
            if (size1_ > N || size2_ > M) 
                throw std::bad_alloc ();
            matrix_assign<scalar_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
        }

        // Resizing
        NUMERICS_INLINE
        void resize (size_type size1, size_type size2) {
            if (size1 > N || size2 > M) 
                throw std::bad_alloc ();
            // The content of the array is intentionally not copied.
            size1_ = size1;
            size2_ = size2;
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
        const_reference_type operator () (size_type i, size_type j) const {
            check<bad_index>::precondition (i < size1_);
            check<bad_index>::precondition (j < size2_);
            return data_ [i] [j]; 
        }
        NUMERICS_INLINE
        reference_type operator () (size_type i, size_type j) {
            check<bad_index>::precondition (i < size1_);
            check<bad_index>::precondition (j < size2_);
            return data_ [i] [j]; 
        }

        NUMERICS_INLINE
        const_matrix_row_type operator [] (size_type i) const {
            return const_matrix_row_type (*this, i);
        }
        NUMERICS_INLINE
        matrix_row_type operator [] (size_type i) {
            return matrix_row_type (*this, i);
        }
        NUMERICS_INLINE
        const_matrix_row_type row (size_type i) const {
            return const_matrix_row_type (*this, i);
        }
        NUMERICS_INLINE
        matrix_row_type row (size_type i) {
            return matrix_row_type (*this, i);
        }
        NUMERICS_INLINE
        const_matrix_column_type column (size_type j) const {
            return const_matrix_column_type (*this, j);
        }
        NUMERICS_INLINE
        matrix_column_type column (size_type j) {
            return matrix_column_type (*this, j);
        }

        NUMERICS_INLINE
        const_matrix_range_type project (size_type start1, size_type stop1, size_type start2, size_type stop2) const {
            return const_matrix_range_type (*this, start1, stop1, start2, stop2);
        }
        NUMERICS_INLINE
        const_matrix_range_type project (const range &r1, const range &r2) const {
            return const_matrix_range_type (*this, r1, r2);
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
        c_matrix &operator = (const c_matrix &m) { 
            check<bad_size>::precondition (size1_ == m.size1_);
            check<bad_size>::precondition (size2_ == m.size2_);
            data_ = m.data_;
            return *this;
        }
        NUMERICS_INLINE
        c_matrix &assign_temporary (c_matrix &m) { 
            swap (m);
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        c_matrix &operator = (const matrix_expression<AE> &ae) { 
#ifndef USE_GCC
            return assign_temporary (self_type (ae));
#else
            return assign (self_type (ae));
#endif
        }
        template<class AE>
        NUMERICS_INLINE
        c_matrix &assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        c_matrix& operator += (const matrix_expression<AE> &ae) {
#ifndef USE_GCC
            return assign_temporary (self_type (*this + ae));
#else
            return assign (self_type (*this + ae));
#endif
        }
        template<class AE>
        NUMERICS_INLINE
        c_matrix &plus_assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_plus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        c_matrix& operator -= (const matrix_expression<AE> &ae) {
#ifndef USE_GCC
            return assign_temporary (self_type (*this - ae));
#else
            return assign (self_type (*this - ae));
#endif
        }
        template<class AE>
        NUMERICS_INLINE
        c_matrix &minus_assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_minus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        c_matrix& operator *= (const AT &at) {
            matrix_assign_scalar<scalar_multiplies_assign<value_type, AT> > () (*this, at);
            return *this;
        }

        // Swapping
        NUMERICS_INLINE
	    void swap (c_matrix &m) {
            check<external_logic>::precondition (this != &m);
            check<bad_size>::precondition (size1_ == m.size1_);
            check<bad_size>::precondition (size2_ == m.size2_);
            std::swap (size1_, m.size1_);
            std::swap (size2_, m.size2_);
            for (size_type i = 0; i < size1_; ++ i)
                std::swap_ranges (data_ [i], data_ [i] + size2_, m.data_ [i]);
        }
#ifndef USE_GCC
        NUMERICS_INLINE
	    friend void swap (c_matrix &m1, c_matrix &m2) {
            m1.swap (m2);
        }
#endif

        // Element insertion
        NUMERICS_INLINE
        void clear () {
            for (size_type i = 0; i < size1_; ++ i)
                std::fill (data_ [i], data_ [i] + size2_, 0);
        }
        NUMERICS_INLINE
        void insert (size_type i, size_type j, const_reference_type t) {
            check<bad_index>::precondition (i < size1_);
            check<bad_index>::precondition (j < size2_);
            check<bad_index>::precondition (data_ [i] [j] == value_type ());
            data_ [i] [j] = t;
        }

        class const_iterator1;
        class iterator1;
        class const_iterator2;
        class iterator2;

        // Element lookup
        NUMERICS_INLINE
        const_iterator1 find1 (size_type i, size_type j) const {
            return const_iterator1 (*this, &data_ [i] [j]);
        }
        NUMERICS_INLINE
        iterator1 find1 (size_type i, size_type j) {
            return iterator1 (*this, &data_ [i] [j]);
        }
        NUMERICS_INLINE
        const_iterator2 find2 (size_type i, size_type j) const {
            return const_iterator2 (*this, &data_ [i] [j]);
        }
        NUMERICS_INLINE
        iterator2 find2 (size_type i, size_type j) {
            return iterator2 (*this, &data_ [i] [j]);
        }

        // Iterators simply are pointers.

        class const_iterator1:
            public container_const_reference<c_matrix>,
            public random_access_iterator_base<const_iterator1, value_type> {
        public:
            typedef std::random_access_iterator_tag iterator_category;
#ifdef USE_GCC
            typedef typename c_matrix::difference_type difference_type;
            typedef typename c_matrix::value_type value_type;
#endif

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator1 ():
                container_const_reference<c_matrix> (), it_ () {}
            NUMERICS_INLINE
            const_iterator1 (const c_matrix &m, const const_iterator_type &it):
                container_const_reference<c_matrix> (m), it_ (it) {}
            NUMERICS_INLINE
            const_iterator1 (const iterator1 &it):
                container_const_reference<c_matrix> (it ()), it_ (it.it_) {}

            // Arithmetic
            NUMERICS_INLINE
            const_iterator1 &operator ++ () {
                it_ += M;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator -- () {
                it_ -= M;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator += (difference_type n) {
                it_ += n * M;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator -= (difference_type n) {
                it_ -= n * M;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const const_iterator1 &it) const {
                return (it_ - it.it_) / M;
            }

            // Dereference
            NUMERICS_INLINE
            const_reference_type operator * () const {
                check<bad_index>::precondition (index1 () < (*this) ().size1 ());
                check<bad_index>::precondition (index2 () < (*this) ().size2 ());
                return *it_;
            }

            NUMERICS_INLINE
            const_iterator2 begin () const {
                const c_matrix &m = (*this) ();
                return m.find2 (index1 (), 0); 
            }
            NUMERICS_INLINE
            const_iterator2 end () const {
                const c_matrix &m = (*this) ();
                return m.find2 (index1 (), m.size2 ()); 
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                const c_matrix &m = (*this) ();
                return (it_ - m.begin1 ().it_) / M;
            }
            NUMERICS_INLINE
            size_type index2 () const {
                const c_matrix &m = (*this) ();
                return (it_ - m.begin1 ().it_) % M;
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
            public container_reference<c_matrix>,
            public random_access_iterator_base<iterator1, value_type> {
        public:
            typedef std::random_access_iterator_tag iterator_category;
#ifdef USE_GCC
            typedef typename c_matrix::difference_type difference_type;
            typedef typename c_matrix::value_type value_type;
#endif

            // Construction and destruction
            NUMERICS_INLINE
            iterator1 ():
                container_reference<c_matrix> (), it_ () {}
            NUMERICS_INLINE
            iterator1 (c_matrix &m, const iterator_type &it):
                container_reference<c_matrix> (m), it_ (it) {}

            // Arithmetic
            NUMERICS_INLINE
            iterator1 &operator ++ () {
                it_ += M;
                return *this;
            }
            NUMERICS_INLINE
            iterator1 &operator -- () {
                it_ -= M;
                return *this;
            }
            NUMERICS_INLINE
            iterator1 &operator += (difference_type n) {
                it_ += n * M;
                return *this;
            }
            NUMERICS_INLINE
            iterator1 &operator -= (difference_type n) {
                it_ -= n * M;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const iterator1 &it) const {
                return (it_ - it.it_) / M;
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
                c_matrix &m = (*this) ();
                return m.find2 (index1 (), 0);
            }
            NUMERICS_INLINE
            iterator2 end () {
                c_matrix &m = (*this) ();
                return m.find2 (index1 (), m.size2 ());
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                const c_matrix &m = (*this) ();
                return (it_ - m.begin1 ().it_) / M;
            }
            NUMERICS_INLINE
            size_type index2 () const {
                const c_matrix &m = (*this) ();
                return (it_ - m.begin1 ().it_) % M;
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
            public container_const_reference<c_matrix>,
            public random_access_iterator_base<const_iterator2, value_type> {
        public:
            typedef std::random_access_iterator_tag iterator_category;
#ifdef USE_GCC
            typedef typename c_matrix::difference_type difference_type;
            typedef typename c_matrix::value_type value_type;
#endif

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator2 ():
                container_const_reference<c_matrix> (), it_ () {}
            NUMERICS_INLINE
            const_iterator2 (const c_matrix &m, const const_iterator_type &it):
                container_const_reference<c_matrix> (m), it_ (it) {}
            NUMERICS_INLINE
            const_iterator2 (const iterator2 &it):
                container_const_reference<c_matrix> (it ()), it_ (it.it_) {}

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
            const_reference_type operator * () const {
                check<bad_index>::precondition (index1 () < (*this) ().size1 ());
                check<bad_index>::precondition (index2 () < (*this) ().size2 ());
                return *it_;
            }

            NUMERICS_INLINE
            const_iterator1 begin () const {
                const c_matrix &m = (*this) ();
                return m.find1 (0, index2 ()); 
            }
            NUMERICS_INLINE
            const_iterator1 end () const {
                const c_matrix &m = (*this) ();
                return m.find1 (m.size1 (), index2 ()); 
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                const c_matrix &m = (*this) ();
                return (it_ - m.begin2 ().it_) / M;
            }
            NUMERICS_INLINE
            size_type  index2 () const {
                const c_matrix &m = (*this) ();
                return (it_ - m.begin2 ().it_) % M;
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
            public container_reference<c_matrix>,
            public random_access_iterator_base<iterator2, value_type> {
        public:
            typedef std::random_access_iterator_tag iterator_category;
#ifdef USE_GCC
            typedef typename c_matrix::difference_type difference_type;
            typedef typename c_matrix::value_type value_type;
#endif

            // Construction and destruction
            NUMERICS_INLINE
            iterator2 ():
                container_reference<c_matrix> (), it_ () {}
            NUMERICS_INLINE
            iterator2 (c_matrix &m, const iterator_type &it):
                container_reference<c_matrix> (m), it_ (it) {}

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
                check<bad_index>::precondition (index1 () < (*this) ().size1 ());
                check<bad_index>::precondition (index2 () < (*this) ().size2 ());
                return *it_;
            }

            NUMERICS_INLINE
            iterator1 begin () {
                c_matrix &m = (*this) ();
                return m.find1 (0, index2 ());
            }
            NUMERICS_INLINE
            iterator1 end () {
                c_matrix &m = (*this) ();
                return m.find1 (m.size1 (), index2 ()); 
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                const c_matrix &m = (*this) ();
                return (it_ - m.begin2 ().it_) / M;
            }
            NUMERICS_INLINE
            size_type index2 () const {
                const c_matrix &m = (*this) ();
                return (it_ - m.begin2 ().it_) % M;
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
        value_type data_ [N] [M];
    };

    struct matrix_tag {};
    struct lower_triangular_tag: public matrix_tag {};
    struct upper_triangular_tag: public matrix_tag {};
    struct unit_lower_triangular_tag: public matrix_tag {};
    struct unit_upper_triangular_tag: public matrix_tag {};

    // FIXME: don't we need iterator versions?
    template<class E1, class E2>
    struct matrix_vector_solve_traits {
        typedef typename promote_traits<typename E1::value_type, typename E2::value_type>::promote_type promote_type;
        typedef vector<promote_type> result_type;
    };

    // Operations: 
    //  n * (n - 1) / 2 + n = n * (n + 1) / 2 multiplies,
    //  n * (n - 1) / 2 plus

    template<class E1, class E2>
    NUMERICS_INLINE
    typename matrix_vector_solve_traits<E1, E2>::result_type
    solve (const matrix_expression<E1> &e1, 
           const vector_expression<E2> &e2,
           lower_triangular_tag) {
        typedef NUMERICS_TYPENAME matrix_vector_solve_traits<E1, E2>::result_type result_type;
        typedef NUMERICS_TYPENAME result_type::size_type size_type;
        typedef NUMERICS_TYPENAME result_type::difference_type difference_type;
        typedef NUMERICS_TYPENAME result_type::value_type value_type;

        check<bad_size>::precondition (e1 ().size1 () == e1 ().size2 ());
        check<bad_size>::precondition (e1 ().size2 () == e2 ().size ());
        result_type r (e2);
        size_type size = r.size ();
        for (size_type n = 0; n < size; ++ n) {
            check<singular>::precondition (e1 () (n, n) != value_type ());
            r (n) /= e1 () (n, n);
            for (size_type m = n + 1; m < size; ++ m) 
                r (m) -= e1 () (m, n) * r (n);
        }
        return r;
    }

    template<class E1, class E2>
    NUMERICS_INLINE
    typename matrix_vector_solve_traits<E1, E2>::result_type
    solve (const matrix_expression<E1> &e1, 
           const vector_expression<E2> &e2,
           upper_triangular_tag) {
        typedef NUMERICS_TYPENAME matrix_vector_solve_traits<E1, E2>::result_type result_type;
        typedef NUMERICS_TYPENAME result_type::size_type size_type;
        typedef NUMERICS_TYPENAME result_type::difference_type difference_type;
        typedef NUMERICS_TYPENAME result_type::value_type value_type;

        check<bad_size>::precondition (e1 ().size1 () == e1 ().size2 ());
        check<bad_size>::precondition (e1 ().size2 () == e2 ().size ());
        result_type r (e2);
        size_type size = r.size ();
        for (difference_type n = size - 1; n >= 0; -- n) {
            check<singular>::precondition (e1 () (n, n) != value_type ());
            r (n) /= e1 () (n, n);
            for (difference_type m = n - 1; (signed) m >= 0; -- m) 
                r (m) -= e1 () (m, n) * r (n);
        }
        return r;
    }

    template<class E1, class E2>
    NUMERICS_INLINE
    typename matrix_vector_solve_traits<E1, E2>::result_type
    solve (const matrix_expression<E1> &e1, 
           const vector_expression<E2> &e2,
           unit_lower_triangular_tag) {
        typedef NUMERICS_TYPENAME matrix_vector_solve_traits<E1, E2>::result_type result_type;
        typedef NUMERICS_TYPENAME result_type::size_type size_type;
        typedef NUMERICS_TYPENAME result_type::difference_type difference_type;
        typedef NUMERICS_TYPENAME result_type::value_type value_type;

        check<bad_size>::precondition (e1 ().size1 () == e1 ().size2 ());
        check<bad_size>::precondition (e1 ().size2 () == e2 ().size ());
        result_type r (e2);
        size_type size = r.size ();
        for (size_type n = 0; n < size; ++ n) {
            for (size_type m = n + 1; m < size; ++ m) 
                r (m) -= e1 () (m, n) * r (n);
        }
        return r;
    }

    template<class E1, class E2>
    NUMERICS_INLINE
    typename matrix_vector_solve_traits<E1, E2>::result_type
    solve (const matrix_expression<E1> &e1, 
           const vector_expression<E2> &e2,
           unit_upper_triangular_tag) {
        typedef NUMERICS_TYPENAME matrix_vector_solve_traits<E1, E2>::result_type result_type;
        typedef NUMERICS_TYPENAME result_type::size_type size_type;
        typedef NUMERICS_TYPENAME result_type::difference_type difference_type;
        typedef NUMERICS_TYPENAME result_type::value_type value_type;

        check<bad_size>::precondition (e1 ().size1 () == e1 ().size2 ());
        check<bad_size>::precondition (e1 ().size2 () == e2 ().size ());
        result_type r (e2);
        size_type size = r.size ();
        for (difference_type n = size - 1; n >= 0; -- n) {
            for (difference_type m = n - 1; (signed) m >= 0; -- m) 
                r (m) -= e1 () (m, n) * r (n);
        }
        return r;
    }

    template<class E1, class E2>
    NUMERICS_INLINE
    typename matrix_vector_solve_traits<E1, E2>::result_type
    solve (const vector_expression<E1> &e1, 
           const matrix_expression<E2> &e2,
           lower_triangular_tag) {
        typedef NUMERICS_TYPENAME matrix_vector_solve_traits<E1, E2>::result_type result_type;
        typedef NUMERICS_TYPENAME result_type::size_type size_type;
        typedef NUMERICS_TYPENAME result_type::difference_type difference_type;
        typedef NUMERICS_TYPENAME result_type::value_type value_type;

        check<bad_size>::precondition (e1 ().size () == e2 ().size1 ());
        check<bad_size>::precondition (e2 ().size1 () == e2 ().size2 ());
        result_type r (e1);
        size_type size = r.size ();
        for (size_type n = 0; n < size; ++ n) {
            check<singular>::precondition (e2 () (n, n) != value_type ());
            r (n) /= e2 () (n, n);
            for (size_type m = n + 1; m < size; ++ m) 
                r (m) -= r (n) * e2 () (n, m);
        }
        return r;
    }

    template<class E1, class E2>
    NUMERICS_INLINE
    typename matrix_vector_solve_traits<E1, E2>::result_type
    solve (const vector_expression<E1> &e1, 
           const matrix_expression<E2> &e2,
           upper_triangular_tag) {
        typedef NUMERICS_TYPENAME matrix_vector_solve_traits<E1, E2>::result_type result_type;
        typedef NUMERICS_TYPENAME result_type::size_type size_type;
        typedef NUMERICS_TYPENAME result_type::difference_type difference_type;
        typedef NUMERICS_TYPENAME result_type::value_type value_type;

        check<bad_size>::precondition (e1 ().size () == e2 ().size1 ());
        check<bad_size>::precondition (e2 ().size1 () == e2 ().size2 ());
        result_type r (e1);
        size_type size = r.size ();
        for (difference_type n = size - 1; n >= 0; -- n) {
            check<singular>::precondition (e2 () (n, n) != value_type ());
            r (n) /= e2 () (n, n);
            for (difference_type m = n - 1; (signed) m >= 0; -- m) 
                r (m) -= r (n) * e2 () (n, m);
        }
        return r;
    }

    template<class E1, class E2>
    NUMERICS_INLINE
    typename matrix_vector_solve_traits<E1, E2>::result_type
    solve (const vector_expression<E1> &e1, 
           const matrix_expression<E2> &e2,
           unit_lower_triangular_tag) {
        typedef NUMERICS_TYPENAME matrix_vector_solve_traits<E1, E2>::result_type result_type;
        typedef NUMERICS_TYPENAME result_type::size_type size_type;
        typedef NUMERICS_TYPENAME result_type::difference_type difference_type;
        typedef NUMERICS_TYPENAME result_type::value_type value_type;

        check<bad_size>::precondition (e1 ().size1 () == e1 ().size2 ());
        check<bad_size>::precondition (e1 ().size2 () == e2 ().size ());
        result_type r (e2);
        size_type size = r.size ();
        for (size_type n = 0; n < size; ++ n) {
            for (size_type m = n + 1; m < size; ++ m) 
                r (m) -= r (n) * e2 () (n, m);
        }
        return r;
    }

    template<class E1, class E2>
    NUMERICS_INLINE
    typename matrix_vector_solve_traits<E1, E2>::result_type
    solve (const vector_expression<E1> &e1, 
           const matrix_expression<E2> &e2,
           unit_upper_triangular_tag) {
        typedef NUMERICS_TYPENAME matrix_vector_solve_traits<E1, E2>::result_type result_type;
        typedef NUMERICS_TYPENAME result_type::size_type size_type;
        typedef NUMERICS_TYPENAME result_type::difference_type difference_type;
        typedef NUMERICS_TYPENAME result_type::value_type value_type;

        check<bad_size>::precondition (e1 ().size () == e2 ().size1 ());
        check<bad_size>::precondition (e2 ().size1 () == e2 ().size2 ());
        result_type r (e1);
        size_type size = r.size ();
        for (difference_type n = size - 1; n >= 0; -- n) {
            for (difference_type m = n - 1; (signed) m >= 0; -- m) 
                r (m) -= r (n) * e2 () (n, m);
        }
        return r;
    }

    // FIXME: don't we need iterator versions?
    template<class E1, class E2>
    struct matrix_matrix_solve_traits {
        typedef typename promote_traits<typename E1::value_type, typename E2::value_type>::promote_type promote_type;
        typedef matrix<promote_type> result_type;
    };

    // Operations: 
    //  k * n * (n - 1) / 2 + k * n = k * n * (n + 1) / 2 multiplies,
    //  k * n * (n - 1) / 2 plus

    template<class E1, class E2>
    NUMERICS_INLINE
    typename matrix_matrix_solve_traits<E1, E2>::result_type
    solve (const matrix_expression<E1> &e1, 
           const matrix_expression<E2> &e2,
           lower_triangular_tag) {
        typedef NUMERICS_TYPENAME matrix_matrix_solve_traits<E1, E2>::result_type result_type;
        typedef NUMERICS_TYPENAME result_type::size_type size_type;
        typedef NUMERICS_TYPENAME result_type::difference_type difference_type;
        typedef NUMERICS_TYPENAME result_type::value_type value_type;

        check<bad_size>::precondition (e1 ().size1 () == e1 ().size2 ());
        check<bad_size>::precondition (e1 ().size2 () == e2 ().size1 ());
        result_type r (e2);
        size_type size1 = r.size1 ();
        size_type size2 = r.size2 ();
        for (size_type n = 0; n < size1; ++ n) {
            check<singular>::precondition (e1 () (n, n) != value_type ());
            for (size_type l = 0; l < size2; ++ l) {
                r (n, l) /= e1 () (n, n);
                for (size_type m = n + 1; m < size1; ++ m) 
                    r (m, l) -= e1 () (m, n) * r (n, l);
            }
        }
        return r;
    }

    template<class E1, class E2>
    NUMERICS_INLINE
    typename matrix_matrix_solve_traits<E1, E2>::result_type
    solve (const matrix_expression<E1> &e1, 
           const matrix_expression<E2> &e2,
           upper_triangular_tag) {
        typedef NUMERICS_TYPENAME matrix_matrix_solve_traits<E1, E2>::result_type result_type;
        typedef NUMERICS_TYPENAME result_type::size_type size_type;
        typedef NUMERICS_TYPENAME result_type::difference_type difference_type;
        typedef NUMERICS_TYPENAME result_type::value_type value_type;

        check<bad_size>::precondition (e1 ().size1 () == e1 ().size2 ());
        check<bad_size>::precondition (e1 ().size2 () == e2 ().size1 ());
        result_type r (e2);
        size_type size1 = r.size1 ();
        size_type size2 = r.size2 ();
        for (difference_type n = size1 - 1; n >= 0; -- n) {
            check<singular>::precondition (e1 () (n, n) != value_type ());
            for (difference_type l = size2 - 1; l >= 0; -- l) {
                r (n, l) /= e1 () (n, n);
                for (difference_type m = n - 1; m >= 0; -- m) 
                    r (m, l) -= e1 () (m, n) * r (n, l);
            }
        }
        return r;
    }

    template<class E1, class E2>
    NUMERICS_INLINE
    typename matrix_matrix_solve_traits<E1, E2>::result_type
    solve (const matrix_expression<E1> &e1, 
           const matrix_expression<E2> &e2,
           unit_lower_triangular_tag) {
        typedef NUMERICS_TYPENAME matrix_matrix_solve_traits<E1, E2>::result_type result_type;
        typedef NUMERICS_TYPENAME result_type::size_type size_type;
        typedef NUMERICS_TYPENAME result_type::difference_type difference_type;
        typedef NUMERICS_TYPENAME result_type::value_type value_type;

        check<bad_size>::precondition (e1 ().size1 () == e1 ().size2 ());
        check<bad_size>::precondition (e1 ().size2 () == e2 ().size1 ());
        result_type r (e2);
        size_type size1 = r.size1 ();
        size_type size2 = r.size2 ();
        for (size_type n = 0; n < size1; ++ n) {
            for (size_type l = 0; l < size2; ++ l) {
                for (size_type m = n + 1; m < size1; ++ m) 
                    r (m, l) -= e1 () (m, n) * r (n, l);
            }
        }
        return r;
    }

    template<class E1, class E2>
    NUMERICS_INLINE
    typename matrix_matrix_solve_traits<E1, E2>::result_type
    solve (const matrix_expression<E1> &e1, 
           const matrix_expression<E2> &e2,
           unit_upper_triangular_tag) {
        typedef NUMERICS_TYPENAME matrix_matrix_solve_traits<E1, E2>::result_type result_type;
        typedef NUMERICS_TYPENAME result_type::size_type size_type;
        typedef NUMERICS_TYPENAME result_type::difference_type difference_type;
        typedef NUMERICS_TYPENAME result_type::value_type value_type;

        check<bad_size>::precondition (e1 ().size1 () == e1 ().size2 ());
        check<bad_size>::precondition (e1 ().size2 () == e2 ().size1 ());
        result_type r (e2);
        size_type size1 = r.size1 ();
        size_type size2 = r.size2 ();
        for (difference_type n = size1 - 1; n >= 0; -- n) {
            for (difference_type l = size2 - 1; l >= 0; -- l) {
                for (difference_type m = n - 1; m >= 0; -- m) 
                    r (m, l) -= e1 () (m, n) * r (n, l);
            }
        }
        return r;
    }

}

#endif 


