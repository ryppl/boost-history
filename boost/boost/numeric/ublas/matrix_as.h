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

#ifndef NUMERICS_MATRIX_AS_H
#define NUMERICS_MATRIX_AS_H

#include <boost/numeric/ublas/config.h>
#include <boost/numeric/ublas/matrix_et.h>

// Iterators based on ideas of Jeremy Siek

namespace boost { namespace numerics {

#ifdef NUMERICS_SPECIALIZE_ASSIGN
    // Iterating row major case
    template<class F, class M, class T>
    // This function seems to be big. So we do not let the compiler inline it.
    // NUMERICS_INLINE
    void iterating_matrix_assign_scalar (const F &f, M &m, const T &t, row_major_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
        typedef F functor_type;
        typedef typename M::difference_type difference_type;
        difference_type size1 (m.size1 ());
        typename M::iterator1 it1 (m.begin1 ());
#ifdef NUMERICS_BOUNDS_CHECK_EX
        // This fails for size2 == 0
        check (m.end1 () - it1 == size1, bad_size ());
#endif
        while (-- size1 >= 0) {
            difference_type size2 (m.size2 ());
            typename matrix_row<M>::iterator it2 ((*it1).begin ());
            check ((*it1).end () - it2 == size2, bad_size ());
#ifndef NUMERICS_USE_DUFF_DEVICE
            while (-- size2 >= 0)
                functor_type () (*it2, t), ++ it2;
#else
            DD (size2, 4, r, (functor_type () (*it2, t), ++ it2));
#endif
            ++ it1;
        }
#else
        typedef F functor_type;
        typedef typename M::difference_type difference_type;
        difference_type size1 (m.size1 ());
        typename M::iterator1 it1 (m.begin1 ());
#ifdef NUMERICS_BOUNDS_CHECK_EX
        // This fails for size2 == 0
        check (m.end1 () - it1 == size1, bad_size ());
#endif
        while (-- size1 >= 0) {
            difference_type size2 (m.size2 ());
            typename M::iterator2 it2 (it1.begin ());
            check (it1.end () - it2 == size2, bad_size ());
#ifndef NUMERICS_USE_DUFF_DEVICE
            while (-- size2 >= 0)
                functor_type () (*it2, t), ++ it2;
#else
            DD (size2, 4, r, (functor_type () (*it2, t), ++ it2));
#endif
            ++ it1;
        }
#endif
    }
    // Iterating column major case
    template<class F, class M, class T>
    // This function seems to be big. So we do not let the compiler inline it.
    // NUMERICS_INLINE
    void iterating_matrix_assign_scalar (const F &f, M &m, const T &t, column_major_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
        typedef F functor_type;
        typedef typename M::difference_type difference_type;
        difference_type size2 (m.size2 ());
        typename M::iterator2 it2 (m.begin2 ());
#ifdef NUMERICS_BOUNDS_CHECK_EX
        // This fails for size1 == 0
        check (m.end2 () - it2 == size2, bad_size ());
#endif
        while (-- size2 >= 0) {
            difference_type size1 (m.size1 ());
            typename matrix_column<M>::iterator it1 ((*it2).begin ());
            check ((*it2).end () - it1 == size1, bad_size ());
#ifndef NUMERICS_USE_DUFF_DEVICE
            while (-- size1 >= 0)
                functor_type () (*it1, t), ++ it1;
#else
            DD (size1, 4, r, (functor_type () (*it1, t), ++ it1));
#endif
            ++ it2;
        }
#else
        typedef F functor_type;
        typedef typename M::difference_type difference_type;
        difference_type size2 (m.size2 ());
        typename M::iterator2 it2 (m.begin2 ());
#ifdef NUMERICS_BOUNDS_CHECK_EX
        // This fails for size1 == 0
        check (m.end2 () - it2 == size2, bad_size ());
#endif
        while (-- size2 >= 0) {
            difference_type size1 (m.size1 ());
            typename M::iterator1 it1 (it2.begin ());
            check (it2.end () - it1 == size1, bad_size ());
#ifndef NUMERICS_USE_DUFF_DEVICE
            while (-- size1 >= 0)
                functor_type () (*it1, t), ++ it1;
#else
            DD (size1, 4, r, (functor_type () (*it1, t), ++ it1));
#endif
            ++ it2;
        }
#endif
    }
    // Indexing row major case
    template<class F, class M, class T>
    // This function seems to be big. So we do not let the compiler inline it.
    // NUMERICS_INLINE
    void indexing_matrix_assign_scalar (const F &f, M &m, const T &t, row_major_tag) {
        typedef F functor_type;
        typedef typename M::difference_type difference_type;
        difference_type size1 (m.size1 ());
        difference_type size2 (m.size2 ());
        for (difference_type i = 0; i < size1; ++ i) {
#ifndef NUMERICS_USE_DUFF_DEVICE
            for (difference_type j = 0; j < size2; ++ j)
                functor_type () (m (i, j), t);
#else
            difference_type j (0);
            DD (size2, 4, r, (functor_type () (m (i, j), t), ++ j));
#endif
        }
    }
    // Indexing column major case
    template<class F, class M, class T>
    // This function seems to be big. So we do not let the compiler inline it.
    // NUMERICS_INLINE
    void indexing_matrix_assign_scalar (const F &f, M &m, const T &t, column_major_tag) {
        typedef F functor_type;
        typedef typename M::difference_type difference_type;
        difference_type size2 (m.size2 ());
        difference_type size1 (m.size1 ());
        for (difference_type j = 0; j < size2; ++ j)
#ifndef NUMERICS_USE_DUFF_DEVICE
            for (difference_type i = 0; i < size1; ++ i) {
                functor_type () (m (i, j), t);
#else
            difference_type i (0);
            DD (size1, 4, r, (functor_type () (m (i, j), t), ++ i));
#endif
        }
    }

    // Dense (proxy) case
    template<class F, class M, class T, class C>
    // This function seems to be big. So we do not let the compiler inline it.
    // NUMERICS_INLINE
    void evaluate_matrix_assign_scalar (const F &f, M &m, const T &t, dense_proxy_tag, C) {
        typedef F functor_type;
        typedef typename M::size_type size_type;
        typedef typename M::difference_type difference_type;
        difference_type size1 (m.size1 ());
        difference_type size2 (m.size2 ());
        if (size1 >= NUMERICS_ITERATOR_THRESHOLD &&
            size2 >= NUMERICS_ITERATOR_THRESHOLD)
            iterating_matrix_assign_scalar (functor_type (), m, t, C ());
        else
            indexing_matrix_assign_scalar (functor_type (), m, t, C ());
    }
    // Packed (proxy) row major case
    template<class F, class M, class T>
    // This function seems to be big. So we do not let the compiler inline it.
    // NUMERICS_INLINE
    void evaluate_matrix_assign_scalar (const F &f, M &m, const T &t, packed_proxy_tag, row_major_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
        typedef F functor_type;
        typedef typename M::difference_type difference_type;
        typename M::iterator1 it1 (m.begin1 ());
        difference_type size1 (m.end1 () - it1);
        while (-- size1 >= 0) {
            typename matrix_row<M>::iterator it2 ((*it1).begin ());
            difference_type size2 ((*it1).end () - it2);
            while (-- size2 >= 0)
                functor_type () (*it2, t), ++ it2;
            ++ it1;
        }
#else
        typedef F functor_type;
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
#endif
    }
    // Packed (proxy) column major case
    template<class F, class M, class T>
    // This function seems to be big. So we do not let the compiler inline it.
    // NUMERICS_INLINE
    void evaluate_matrix_assign_scalar (const F &f, M &m, const T &t, packed_proxy_tag, column_major_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
        typedef F functor_type;
        typedef typename M::difference_type difference_type;
        typename M::iterator2 it2 (m.begin2 ());
        difference_type size2 (m.end2 () - it2);
        while (-- size2 >= 0) {
            typename matrix_column<M>::iterator it1 ((*it2).begin ());
            difference_type size1 ((*it2).end () - it1);
            while (-- size1 >= 0)
                functor_type () (*it1, t), ++ it1;
            ++ it2;
        }
#else
        typedef F functor_type;
        typedef typename M::difference_type difference_type;
        typename M::iterator2 it2 (m.begin2 ());
        difference_type size2 (m.end2 () - it2);
        while (-- size2 >= 0) {
            typename M::iterator1 it1 (it2.begin ());
            difference_type size1 (it2.end () - it1);
            while (-- size1 >= 0)
                functor_type () (*it1, t), ++ it1;
            ++ it2;
        }
#endif
    }
    // Sparse (proxy) row major case
    template<class F, class M, class T>
    // This function seems to be big. So we do not let the compiler inline it.
    // NUMERICS_INLINE
    void evaluate_matrix_assign_scalar (const F &f, M &m, const T &t, sparse_proxy_tag, row_major_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
        typedef F functor_type;
        typename M::iterator1 it1 (m.begin1 ());
        typename M::iterator1 it1_end (m.end1 ());
        while (it1 != it1_end) {
            typename matrix_row<M>::iterator it2 ((*it1).begin ());
            typename matrix_row<M>::iterator it2_end ((*it1).end ());
            while (it2 != it2_end)
                functor_type () (*it2, t), ++ it2;
            ++ it1;
        }
#else
        typedef F functor_type;
        typename M::iterator1 it1 (m.begin1 ());
        typename M::iterator1 it1_end (m.end1 ());
        while (it1 != it1_end) {
            typename M::iterator2 it2 (it1.begin ());
            typename M::iterator2 it2_end (it1.end ());
            while (it2 != it2_end)
                functor_type () (*it2, t), ++ it2;
            ++ it1;
        }
#endif
    }
    // Sparse (proxy) column major case
    template<class F, class M, class T>
    // This function seems to be big. So we do not let the compiler inline it.
    // NUMERICS_INLINE
    void evaluate_matrix_assign_scalar (const F &f, M &m, const T &t, sparse_proxy_tag, column_major_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
        typedef F functor_type;
        typename M::iterator2 it2 (m.begin2 ());
        typename M::iterator2 it2_end (m.end2 ());
        while (it2 != it2_end) {
            typename matrix_column<M>::iterator1 it1 ((*it2).begin ());
            typename matrix_column<M>::iterator1 it1_end ((*it2).end ());
            while (it1 != it1_end)
                functor_type () (*it1, t), ++ it1;
            ++ it2;
        }
#else
        typedef F functor_type;
        typename M::iterator2 it2 (m.begin2 ());
        typename M::iterator2 it2_end (m.end2 ());
        while (it2 != it2_end) {
            typename M::iterator1 it1 (it2.begin ());
            typename M::iterator1 it1_end (it2.end ());
            while (it1 != it1_end)
                functor_type () (*it1, t), ++ it1;
            ++ it2;
        }
#endif
    }
#endif

    // matrix assignment_operation scalar
    template<class F>
    struct matrix_assign_scalar {
        typedef F functor_type;
        typedef typename F::assign_category assign_category;

#ifndef NUMERICS_SPECIALIZE_ASSIGN
        // Iterating row major case
        template<class M, class T>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void iterating_assign (M &m, const T &t, row_major_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            typedef typename M::difference_type difference_type;
            difference_type size1 (m.size1 ());
            typename M::iterator1 it1 (m.begin1 ());
#ifdef NUMERICS_BOUNDS_CHECK_EX
            // This fails for size2 == 0
            check (m.end1 () - it1 == size1, bad_size ());
#endif
            while (-- size1 >= 0) {
                difference_type size2 (m.size2 ());
                typename matrix_row<M>::iterator it2 ((*it1).begin ());
                check ((*it1).end () - it2 == size2, bad_size ());
#ifndef NUMERICS_USE_DUFF_DEVICE
                while (-- size2 >= 0)
                    functor_type () (*it2, t), ++ it2;
#else
                DD (size2, 4, r, (functor_type () (*it2, t), ++ it2));
#endif
                ++ it1;
            }
#else
            typedef typename M::difference_type difference_type;
            difference_type size1 (m.size1 ());
            typename M::iterator1 it1 (m.begin1 ());
#ifdef NUMERICS_BOUNDS_CHECK_EX
            // This fails for size2 == 0
            check (m.end1 () - it1 == size1, bad_size ());
#endif
            while (-- size1 >= 0) {
                difference_type size2 (m.size2 ());
                typename M::iterator2 it2 (it1.begin ());
                check (it1.end () - it2 == size2, bad_size ());
#ifndef NUMERICS_USE_DUFF_DEVICE
                while (-- size2 >= 0)
                    functor_type () (*it2, t), ++ it2;
#else
                DD (size2, 4, r, (functor_type () (*it2, t), ++ it2));
#endif
                ++ it1;
            }
#endif
        }
        // Iterating column major case
        template<class M, class T>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void iterating_assign (M &m, const T &t, column_major_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            typedef typename M::difference_type difference_type;
            difference_type size2 (m.size2 ());
            typename M::iterator2 it2 (m.begin2 ());
#ifdef NUMERICS_BOUNDS_CHECK_EX
            // This fails for size1 == 0
            check (m.end2 () - it2 == size2, bad_size ());
#endif
            while (-- size2 >= 0) {
                difference_type size1 (m.size1 ());
                typename matrix_column<M>::iterator it1 ((*it2).begin ());
                check ((*it2).end () - it1 == size1, bad_size ());
#ifndef NUMERICS_USE_DUFF_DEVICE
                while (-- size1 >= 0)
                    functor_type () (*it1, t), ++ it1;
#else
                DD (size1, 4, r, (functor_type () (*it1, t), ++ it1));
#endif
                ++ it2;
            }
#else
            typedef typename M::difference_type difference_type;
            difference_type size2 (m.size2 ());
            typename M::iterator2 it2 (m.begin2 ());
#ifdef NUMERICS_BOUNDS_CHECK_EX
            // This fails for size1 == 0
            check (m.end2 () - it2 == size2, bad_size ());
#endif
            while (-- size2 >= 0) {
                difference_type size1 (m.size1 ());
                typename M::iterator1 it1 (it2.begin ());
                check (it2.end () - it1 == size1, bad_size ());
#ifndef NUMERICS_USE_DUFF_DEVICE
                while (-- size1 >= 0)
                    functor_type () (*it1, t), ++ it1;
#else
                DD (size1, 4, r, (functor_type () (*it1, t), ++ it1));
#endif
                ++ it2;
            }
#endif
        }
        // Indexing row major case
        template<class M, class T>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void indexing_assign (M &m, const T &t, row_major_tag) {
            typedef typename M::difference_type difference_type;
            difference_type size1 (m.size1 ());
            difference_type size2 (m.size2 ());
            for (difference_type i = 0; i < size1; ++ i) {
#ifndef NUMERICS_USE_DUFF_DEVICE
                for (difference_type j = 0; j < size2; ++ j)
                    functor_type () (m (i, j), t);
#else
                difference_type j (0);
                DD (size2, 4, r, (functor_type () (m (i, j), t), ++ j));
#endif
            }
        }
        // Indexing column major case
        template<class M, class T>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void indexing_assign (M &m, const T &t, column_major_tag) {
            typedef typename M::difference_type difference_type;
            difference_type size2 (m.size2 ());
            difference_type size1 (m.size1 ());
            for (difference_type j = 0; j < size2; ++ j)
#ifndef NUMERICS_USE_DUFF_DEVICE
                for (difference_type i = 0; i < size1; ++ i) {
                    functor_type () (m (i, j), t);
#else
                difference_type i (0);
                DD (size1, 4, r, (functor_type () (m (i, j), t), ++ i));
#endif
            }
        }

        // Dense (proxy) case
        template<class M, class T, class C>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (M &m, const T &t, dense_proxy_tag, C) {
            typedef typename M::size_type size_type;
            typedef typename M::difference_type difference_type;
            difference_type size1 (m.size1 ());
            difference_type size2 (m.size2 ());
            if (size1 >= NUMERICS_ITERATOR_THRESHOLD &&
                size2 >= NUMERICS_ITERATOR_THRESHOLD)
                iterating_assign (m, t, C ());
            else
                indexing_assign (m, t, C ());
        }
        // Packed (proxy) row major case
        template<class M, class T>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (M &m, const T &t, packed_proxy_tag, row_major_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            typedef typename M::difference_type difference_type;
            typename M::iterator1 it1 (m.begin1 ());
            difference_type size1 (m.end1 () - it1);
            while (-- size1 >= 0) {
                typename matrix_row<M>::iterator it2 ((*it1).begin ());
                difference_type size2 ((*it1).end () - it2);
                while (-- size2 >= 0)
                    functor_type () (*it2, t), ++ it2;
                ++ it1;
            }
#else
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
#endif
        }
        // Packed (proxy) column major case
        template<class M, class T>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (M &m, const T &t, packed_proxy_tag, column_major_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            typedef typename M::difference_type difference_type;
            typename M::iterator2 it2 (m.begin2 ());
            difference_type size2 (m.end2 () - it2);
            while (-- size2 >= 0) {
                typename matrix_column<M>::iterator it1 ((*it2).begin ());
                difference_type size1 ((*it2).end () - it1);
                while (-- size1 >= 0)
                    functor_type () (*it1, t), ++ it1;
                ++ it2;
            }
#else
            typedef typename M::difference_type difference_type;
            typename M::iterator2 it2 (m.begin2 ());
            difference_type size2 (m.end2 () - it2);
            while (-- size2 >= 0) {
                typename M::iterator1 it1 (it2.begin ());
                difference_type size1 (it2.end () - it1);
                while (-- size1 >= 0)
                    functor_type () (*it1, t), ++ it1;
                ++ it2;
            }
#endif
        }
        // Sparse (proxy) row major case
        template<class M, class T>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (M &m, const T &t, sparse_proxy_tag, row_major_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            typename M::iterator1 it1 (m.begin1 ());
            typename M::iterator1 it1_end (m.end1 ());
            while (it1 != it1_end) {
                typename matrix_row<M>::iterator it2 ((*it1).begin ());
                typename matrix_row<M>::iterator it2_end ((*it1).end ());
                while (it2 != it2_end)
                    functor_type () (*it2, t), ++ it2;
                ++ it1;
            }
#else
            typename M::iterator1 it1 (m.begin1 ());
            typename M::iterator1 it1_end (m.end1 ());
            while (it1 != it1_end) {
                typename M::iterator2 it2 (it1.begin ());
                typename M::iterator2 it2_end (it1.end ());
                while (it2 != it2_end)
                    functor_type () (*it2, t), ++ it2;
                ++ it1;
            }
#endif
        }
        // Sparse (proxy) column major case
        template<class M, class T>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (M &m, const T &t, sparse_proxy_tag, column_major_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            typename M::iterator2 it2 (m.begin2 ());
            typename M::iterator2 it2_end (m.end2 ());
            while (it2 != it2_end) {
                typename matrix_column<M>::iterator1 it1 ((*it2).begin ());
                typename matrix_column<M>::iterator1 it1_end ((*it2).end ());
                while (it1 != it1_end)
                    functor_type () (*it1, t), ++ it1;
                ++ it2;
            }
#else
            typename M::iterator2 it2 (m.begin2 ());
            typename M::iterator2 it2_end (m.end2 ());
            while (it2 != it2_end) {
                typename M::iterator1 it1 (it2.begin ());
                typename M::iterator1 it1_end (it2.end ());
                while (it1 != it1_end)
                    functor_type () (*it1, t), ++ it1;
                ++ it2;
            }
#endif
        }
#endif

        // Dispatcher
        template<class M, class T>
        NUMERICS_INLINE
        void operator () (M &m, const T &t) {
            typedef typename M::storage_category storage_category;
            typedef typename M::orientation_category orientation_category;
#ifndef NUMERICS_SPECIALIZE_ASSIGN
            operator () (m, t, storage_category (), orientation_category ());
#else
            evaluate_matrix_assign_scalar (functor_type (), m, t, storage_category (), orientation_category ());
#endif
        }
    };

    template<class LS, class A, class RI1, class RI2>
    struct matrix_assign_traits {
        typedef LS storage_category;
    };

    template<>
    struct matrix_assign_traits<dense_tag, assign_tag, packed_random_access_iterator_tag, packed_random_access_iterator_tag> {
        typedef packed_tag storage_category;
    };
    template<>
    struct matrix_assign_traits<dense_tag, computed_assign_tag, packed_random_access_iterator_tag, packed_random_access_iterator_tag> {
        typedef packed_tag storage_category;
    };
    template<>
    struct matrix_assign_traits<dense_tag, assign_tag, sparse_bidirectional_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_tag storage_category;
    };
    template<>
    struct matrix_assign_traits<dense_tag, computed_assign_tag, sparse_bidirectional_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_tag storage_category;
    };

    template<>
    struct matrix_assign_traits<dense_proxy_tag, assign_tag, packed_random_access_iterator_tag, packed_random_access_iterator_tag> {
        typedef packed_proxy_tag storage_category;
    };
    template<>
    struct matrix_assign_traits<dense_proxy_tag, computed_assign_tag, packed_random_access_iterator_tag, packed_random_access_iterator_tag> {
        typedef packed_proxy_tag storage_category;
    };
    template<>
    struct matrix_assign_traits<dense_proxy_tag, assign_tag, sparse_bidirectional_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_tag storage_category;
    };
    template<>
    struct matrix_assign_traits<dense_proxy_tag, computed_assign_tag, sparse_bidirectional_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_tag storage_category;
    };

    template<>
    struct matrix_assign_traits<packed_tag, assign_tag, sparse_bidirectional_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_tag storage_category;
    };
    template<>
    struct matrix_assign_traits<packed_tag, computed_assign_tag, sparse_bidirectional_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_tag storage_category;
    };

    template<>
    struct matrix_assign_traits<packed_proxy_tag, assign_tag, sparse_bidirectional_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_tag storage_category;
    };
    template<>
    struct matrix_assign_traits<packed_proxy_tag, computed_assign_tag, sparse_bidirectional_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_tag storage_category;
    };

#ifdef NUMERICS_SPECIALIZE_ASSIGN
    // Iterating row major case
    template<class F, class M, class E>
    // This function seems to be big. So we do not let the compiler inline it.
    // NUMERICS_INLINE
    void iterating_matrix_assign (const F &f, M &m, const matrix_expression<E> &e, row_major_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
        typedef F functor_type;
        typedef typename M::difference_type difference_type;
        difference_type size1 (common (m.size1 (), e ().size1 ()));
        typename M::iterator1 it1 (m.begin1 ());
#ifdef NUMERICS_BOUNDS_CHECK_EX
        // This fails for size2 == 0
        check (m.end1 () - it1 == size1, bad_size ());
#endif
        typename E::const_iterator1 it1e (e ().begin1 ());
#ifdef NUMERICS_BOUNDS_CHECK_EX
        // This fails for size2 == 0
        check (e ().end1 () - it1e == size1, bad_size ());
#endif
        while (-- size1 >= 0) {
            difference_type size2 (common (m.size2 (), e ().size2 ()));
            typename matrix_row<M>::iterator it2 ((*it1).begin ());
            check ((*it1).end () - it2 == size2, bad_size ());
            typename matrix_row<const E>::const_iterator it2e ((*it1e).begin ());
            check ((*it1e).end () - it2e == size2, bad_size ());
#ifndef NUMERICS_USE_DUFF_DEVICE
            while (-- size2 >= 0)
                functor_type () (*it2, *it2e), ++ it2, ++ it2e;
#else
            DD (size2, 2, r, (functor_type () (*it2, *it2e), ++ it2, ++ it2e));
#endif
            ++ it1, ++ it1e;
        }
#else
        typedef F functor_type;
        typedef typename M::difference_type difference_type;
        difference_type size1 (common (m.size1 (), e ().size1 ()));
        typename M::iterator1 it1 (m.begin1 ());
#ifdef NUMERICS_BOUNDS_CHECK_EX
        // This fails for size2 == 0
        check (m.end1 () - it1 == size1, bad_size ());
#endif
        typename E::const_iterator1 it1e (e ().begin1 ());
#ifdef NUMERICS_BOUNDS_CHECK_EX
        // This fails for size2 == 0
        check (e ().end1 () - it1e == size1, bad_size ());
#endif
        while (-- size1 >= 0) {
            difference_type size2 (common (m.size2 (), e ().size2 ()));
            typename M::iterator2 it2 (it1.begin ());
            check (it1.end () - it2 == size2, bad_size ());
            typename E::const_iterator2 it2e (it1e.begin ());
            check (it1e.end () - it2e == size2, bad_size ());
#ifndef NUMERICS_USE_DUFF_DEVICE
            while (-- size2 >= 0)
                functor_type () (*it2, *it2e), ++ it2, ++ it2e;
#else
            DD (size2, 2, r, (functor_type () (*it2, *it2e), ++ it2, ++ it2e));
#endif
            ++ it1, ++ it1e;
        }
#endif
    }
    // Iterating column major case
    template<class F, class M, class E>
    // This function seems to be big. So we do not let the compiler inline it.
    // NUMERICS_INLINE
    void iterating_matrix_assign (const F &f, M &m, const matrix_expression<E> &e, column_major_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
        typedef F functor_type;
        typedef typename M::difference_type difference_type;
        difference_type size2 (common (m.size2 (), e ().size2 ()));
        typename M::iterator2 it2 (m.begin2 ());
#ifdef NUMERICS_BOUNDS_CHECK_EX
        // This fails for size1 == 0
        check (m.end2 () - it2 == size2, bad_size ());
#endif
        typename E::const_iterator2 it2e (e ().begin2 ());
#ifdef NUMERICS_BOUNDS_CHECK_EX
        // This fails for size1 == 0
        check (e ().end2 () - it2e == size2, bad_size ());
#endif
        while (-- size2 >= 0) {
            difference_type size1 (common (m.size1 (), e ().size1 ()));
            typename matrix_column<M>::iterator it1 ((*it2).begin ());
            check ((*it2).end () - it1 == size1, bad_size ());
            typename matrix_column<const E>::const_iterator it1e ((*it2e).begin ());
            check ((*it2e).end () - it1e == size1, bad_size ());
#ifndef NUMERICS_USE_DUFF_DEVICE
            while (-- size1 >= 0)
                functor_type () (*it1, *it1e), ++ it1, ++ it1e;
#else
            DD (size1, 2, r, (functor_type () (*it1, *it1e), ++ it1, ++ it1e));
#endif
            ++ it2, ++ it2e;
        }
#else
        typedef F functor_type;
        typedef typename M::difference_type difference_type;
        difference_type size2 (common (m.size2 (), e ().size2 ()));
        typename M::iterator2 it2 (m.begin2 ());
#ifdef NUMERICS_BOUNDS_CHECK_EX
        // This fails for size1 == 0
        check (m.end2 () - it2 == size2, bad_size ());
#endif
        typename E::const_iterator2 it2e (e ().begin2 ());
#ifdef NUMERICS_BOUNDS_CHECK_EX
        // This fails for size1 == 0
        check (e ().end2 () - it2e == size2, bad_size ());
#endif
        while (-- size2 >= 0) {
            difference_type size1 (common (m.size1 (), e ().size1 ()));
            typename M::iterator1 it1 (it2.begin ());
            check (it2.end () - it1 == size1, bad_size ());
            typename E::const_iterator1 it1e (it2e.begin ());
            check (it2e.end () - it1e == size1, bad_size ());
#ifndef NUMERICS_USE_DUFF_DEVICE
            while (-- size1 >= 0)
                functor_type () (*it1, *it1e), ++ it1, ++ it1e;
#else
            DD (size1, 2, r, (functor_type () (*it1, *it1e), ++ it1, ++ it1e));
#endif
            ++ it2, ++ it2e;
        }
#endif
    }
    // Indexing row major case
    template<class F, class M, class E>
    // This function seems to be big. So we do not let the compiler inline it.
    // NUMERICS_INLINE
    void indexing_matrix_assign (const F &f, M &m, const matrix_expression<E> &e, row_major_tag) {
        typedef F functor_type;
        typedef typename M::difference_type difference_type;
        difference_type size1 (common (m.size1 (), e ().size1 ()));
        difference_type size2 (common (m.size2 (), e ().size2 ()));
        for (difference_type i = 0; i < size1; ++ i) {
#ifndef NUMERICS_USE_DUFF_DEVICE
            for (difference_type j = 0; j < size2; ++ j)
                functor_type () (m (i, j), e () (i, j));
#else
            difference_type j (0);
            DD (size2, 2, r, (functor_type () (m (i, j), e () (i, j)), ++ j));
#endif
        }
    }
    // Indexing column major case
    template<class F, class M, class E>
    // This function seems to be big. So we do not let the compiler inline it.
    // NUMERICS_INLINE
    void indexing_matrix_assign (const F &f, M &m, const matrix_expression<E> &e, column_major_tag) {
        typedef F functor_type;
        typedef typename M::difference_type difference_type;
        difference_type size2 (common (m.size2 (), e ().size2 ()));
        difference_type size1 (common (m.size1 (), e ().size1 ()));
        for (difference_type j = 0; j < size2; ++ j)
#ifndef NUMERICS_USE_DUFF_DEVICE
            for (difference_type i = 0; i < size1; ++ i) {
                functor_type () (m (i, j), e () (i, j));
#else
            difference_type i (0);
            DD (size1, 2, r, (functor_type () (m (i, j), e () (i, j)), ++ i));
#endif
        }
    }

    // Dense (proxy) case
    template<class F, class M, class E, class C>
    // This function seems to be big. So we do not let the compiler inline it.
    // NUMERICS_INLINE
    void evaluate_matrix_assign (const F &f, M &m, const matrix_expression<E> &e, dense_proxy_tag, C) {
        typedef F functor_type;
        typedef typename M::size_type size_type;
        typedef typename M::difference_type difference_type;
        difference_type size1 (common (m.size1 (), e ().size1 ()));
        difference_type size2 (common (m.size2 (), e ().size2 ()));
        if (size1 >= NUMERICS_ITERATOR_THRESHOLD &&
            size2 >= NUMERICS_ITERATOR_THRESHOLD)
            iterating_matrix_assign (functor_type (), m, e, C ());
        else
            indexing_matrix_assign (functor_type (), m, e, C ());
    }
    // Packed (proxy) row major case
    template<class F, class M, class E>
    // This function seems to be big. So we do not let the compiler inline it.
    // NUMERICS_INLINE
    void evaluate_matrix_assign (const F &f, M &m, const matrix_expression<E> &e, packed_proxy_tag, row_major_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
        check (m.size1 () == e ().size1 (), bad_size ());
        check (m.size2 () == e ().size2 (), bad_size ());
        typedef F functor_type;
        typedef typename M::size_type size_type;
        typedef typename M::value_type value_type;
        typename M::iterator1 it1 (m.begin1 ());
        typename M::iterator1 it1_end (m.end1 ());
        typename E::const_iterator1 it1e (e ().begin1 ());
        typename E::const_iterator1 it1e_end (e ().end1 ());
        if (it1e != it1e_end && it1e.index1 () < it1.index1 ())
            it1e += std::min (it1.index1 () - it1e.index1 (), size_type (it1e_end - it1e));
        while (it1 != it1_end && it1e != it1e_end && it1.index () < it1e.index ()) {
            typename matrix_row<M>::iterator it2 ((*it1).begin ());
            typename matrix_row<M>::iterator it2_end ((*it1).end ());
            while (it2 != it2_end) {
                functor_type () (*it2, value_type ());
                ++ it2;
            }
            ++ it1;
        }
        while (it1 != it1_end && it1e != it1e_end) {
            typename matrix_row<M>::iterator it2 ((*it1).begin ());
            typename matrix_row<M>::iterator it2_end ((*it1).end ());
            typename matrix_row<const E>::const_iterator it2e ((*it1e).begin ());
            typename matrix_row<const E>::const_iterator it2e_end ((*it1e).end ());
            if (it2e != it2e_end && it2e.index2 () < it2.index2 ())
                it2e += std::min (it2.index2 () - it2e.index2 () <= size_type (it2e_end - it2e));
            while (it2 != it2_end && it2e != it2e_end && it2.index () < it2e.index ()) {
                functor_type () (*it2, value_type ());
                ++ it2;
            }
            while (it2 != it2_end && it2e != it2e_end) {
                functor_type () (*it2, *it2e);
                ++ it2, ++ it2e;
            }
            while (it2 != it2_end) {
                functor_type () (*it2, value_type ());
                ++ it2;
            }
            ++ it1, ++ it1e;
        }
        while (it1 != it1_end) {
            typename matrix_row<M>::iterator it2 ((*it1).begin ());
            typename matrix_row<M>::iterator it2_end ((*it1).end ());
            while (it2 != it2_end) {
                functor_type () (*it2, value_type ());
                ++ it2;
            }
            ++ it1;
        }
#ifdef NUMERICS_BOUNDS_CHECK_EX
        {
            // Need the const member dispatched.
            const M &cm = m;
            typename E::const_iterator1 it1e (e ().begin1 ());
            typename E::const_iterator1 it1e_end (e ().end1 ());
            while (it1e != it1e_end) {
                typename matrix_row<const E>::const_iterator it2e ((*it1e).begin ());
                typename matrix_row<const E>::const_iterator it2e_end ((*it1e).end ());
                while (it2e != it2e_end) {
                    // FIXME: we need a better floating point comparison...
                    check (*it2e == cm (it1e.index (), it2e.index ()), bad_index ());
                    ++ it2e;
                }
                ++ it1e;
            }
        }
#endif
#else
        check (m.size1 () == e ().size1 (), bad_size ());
        check (m.size2 () == e ().size2 (), bad_size ());
        typedef F functor_type;
        typedef typename M::size_type size_type;
        typedef typename M::value_type value_type;
        typename M::iterator1 it1 (m.begin1 ());
        typename M::iterator1 it1_end (m.end1 ());
        typename E::const_iterator1 it1e (e ().begin1 ());
        typename E::const_iterator1 it1e_end (e ().end1 ());
        if (it1e != it1e_end && it1e.index1 () < it1.index1 ())
            it1e += std::min (it1.index1 () - it1e.index1 (), size_type (it1e_end - it1e));
        while (it1 != it1_end && it1e != it1e_end && it1.index1 () < it1e.index1 ()) {
            typename M::iterator2 it2 (it1.begin ());
            typename M::iterator2 it2_end (it1.end ());
            while (it2 != it2_end) {
                functor_type () (*it2, value_type ());
                ++ it2;
            }
            ++ it1;
        }
        while (it1 != it1_end && it1e != it1e_end) {
            typename M::iterator2 it2 (it1.begin ());
            typename M::iterator2 it2_end (it1.end ());
            typename E::const_iterator2 it2e (it1e.begin ());
            typename E::const_iterator2 it2e_end (it1e.end ());
            if (it2e != it2e_end && it2e.index2 () < it2.index2 ())
                it2e += std::min (it2.index2 () - it2e.index2 (), size_type (it2e_end - it2e));
            while (it2 != it2_end && it2e != it2e_end && it2.index2 () < it2e.index2 ()) {
                functor_type () (*it2, value_type ());
                ++ it2;
            }
            while (it2 != it2_end && it2e != it2e_end) {
                functor_type () (*it2, *it2e);
                ++ it2, ++ it2e;
            }
            while (it2 != it2_end) {
                functor_type () (*it2, value_type ());
                ++ it2;
            }
            ++ it1, ++ it1e;
        }
        while (it1 != it1_end) {
            typename M::iterator2 it2 (it1.begin ());
            typename M::iterator2 it2_end (it1.end ());
            while (it2 != it2_end) {
                functor_type () (*it2, value_type ());
                ++ it2;
            }
            ++ it1;
        }
#ifdef NUMERICS_BOUNDS_CHECK_EX
        {
            // Need the const member dispatched.
            const M &cm = m;
            typename E::const_iterator1 it1e (e ().begin1 ());
            typename E::const_iterator1 it1e_end (e ().end1 ());
            while (it1e != it1e_end) {
                typename E::const_iterator2 it2e (it1e.begin ());
                typename E::const_iterator2 it2e_end (it1e.end ());
                while (it2e != it2e_end) {
                    // FIXME: we need a better floating point comparison...
                    check (*it2e == cm (it2e.index1 (), it2e.index2 ()), bad_index ());
                    ++ it2e;
                }
                ++ it1e;
            }
        }
#endif
#endif
    }
    // Packed (proxy) column major case
    template<class F, class M, class E>
    // This function seems to be big. So we do not let the compiler inline it.
    // NUMERICS_INLINE
    void evaluate_matrix_assign (const F &f, M &m, const matrix_expression<E> &e, packed_proxy_tag, column_major_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
        check (m.size3 () == e ().size2 (), bad_size ());
        check (m.size1 () == e ().size1 (), bad_size ());
        typedef F functor_type;
        typedef typename M::size_type size_type;
        typedef typename M::value_type value_type;
        typename M::iterator2 it2 (m.begin2 ());
        typename M::iterator2 it2_end (m.end2 ());
        typename E::const_iterator2 it2e (e ().begin2 ());
        typename E::const_iterator2 it2e_end (e ().end2 ());
        if (it2e != it2e_end && it2e.index2 () < it2.index2 ())
            it2e += std::min (it2.index2 () - it2e.index2 (), size_type (it2e_end - it2e));
        while (it2 != it2_end && it2e != it2e_end && it2.index () < it2e.index ()) {
            typename matrix_column<M>::iterator it1 ((*it2).begin ());
            typename matrix_column<M>::iterator it1_end ((*it2).end ());
            while (it1 != it1_end) {
                functor_type () (*it1, value_type ());
                ++ it1;
            }
            ++ it2;
        }
        while (it2 != it2_end && it2e != it2e_end) {
            typename matrix_column<M>::iterator it1 ((*it2).begin ());
            typename matrix_column<M>::iterator it1_end ((*it2).end ());
            typename matrix_column<const E>::const_iterator it1e ((*it2e).begin ());
            typename matrix_column<const E>::const_iterator it1e_end ((*it2e).end ());
            if (it1e != it1e_end && it1e.index1 () < it1.index1 ())
                it1e += std::min (it1.index1 () - it1e.index1 () <= size_type (it1e_end - it1e));
            while (it1 != it1_end && it1e != it1e_end && it1.index () < it1e.index ()) {
                functor_type () (*it1, value_type ());
                ++ it1;
            }
            while (it1 != it1_end && it1e != it1e_end) {
                functor_type () (*it1, *it1e);
                ++ it1, ++ it1e;
            }
            while (it1 != it1_end) {
                functor_type () (*it1, value_type ());
                ++ it1;
            }
            ++ it2, ++ it2e;
        }
        while (it2 != it2_end) {
            typename matrix_column<M>::iterator it1 ((*it2).begin ());
            typename matrix_column<M>::iterator it1_end ((*it2).end ());
            while (it1 != it1_end) {
                functor_type () (*it1, value_type ());
                ++ it1;
            }
            ++ it2;
        }
#ifdef NUMERICS_BOUNDS_CHECK_EX
        {
            // Need the const member dispatched.
            const M &cm = m;
            typename E::const_iterator2 it2e (e ().begin2 ());
            typename E::const_iterator2 it2e_end (e ().end2 ());
            while (it2e != it2e_end) {
                typename matrix_column<const E>::const_iterator it1e ((*it2e).begin ());
                typename matrix_column<const E>::const_iterator it1e_end ((*it2e).end ());
                while (it1e != it1e_end) {
                    // FIXME: we need a better floating point comparison...
                    check (*it1e == cm (it2e.index (), it1e.index ()), bad_index ());
                    ++ it1e;
                }
                ++ it2e;
            }
        }
#endif
#else
        check (m.size2 () == e ().size2 (), bad_size ());
        check (m.size1 () == e ().size1 (), bad_size ());
        typedef F functor_type;
        typedef typename M::size_type size_type;
        typedef typename M::value_type value_type;
        typename M::iterator2 it2 (m.begin2 ());
        typename M::iterator2 it2_end (m.end2 ());
        typename E::const_iterator2 it2e (e ().begin2 ());
        typename E::const_iterator2 it2e_end (e ().end2 ());
        if (it2e != it2e_end && it2e.index2 () < it2.index2 ())
            it2e += std::min (it2.index2 () - it2e.index2 (), size_type (it2e_end - it2e));
        while (it2 != it2_end && it2e != it2e_end && it2.index2 () < it2e.index2 ()) {
            typename M::iterator1 it1 (it2.begin ());
            typename M::iterator1 it1_end (it2.end ());
            while (it1 != it1_end) {
                functor_type () (*it1, value_type ());
                ++ it1;
            }
            ++ it2;
        }
        while (it2 != it2_end && it2e != it2e_end) {
            typename M::iterator1 it1 (it2.begin ());
            typename M::iterator1 it1_end (it2.end ());
            typename E::const_iterator1 it1e (it2e.begin ());
            typename E::const_iterator1 it1e_end (it2e.end ());
            if (it1e != it1e_end && it1e.index1 () < it1.index1 ())
                it1e += std::min (it1.index1 () - it1e.index1 (), size_type (it1e_end - it1e));
            while (it1 != it1_end && it1e != it1e_end && it1.index1 () < it1e.index1 ()) {
                functor_type () (*it1, value_type ());
                ++ it1;
            }
            while (it1 != it1_end && it1e != it1e_end) {
                functor_type () (*it1, *it1e);
                ++ it1, ++ it1e;
            }
            while (it1 != it1_end) {
                functor_type () (*it1, value_type ());
                ++ it1;
            }
            ++ it2, ++ it2e;
        }
        while (it2 != it2_end) {
            typename M::iterator1 it1 (it2.begin ());
            typename M::iterator1 it1_end (it2.end ());
            while (it1 != it1_end) {
                functor_type () (*it1, value_type ());
                ++ it1;
            }
            ++ it2;
        }
#ifdef NUMERICS_BOUNDS_CHECK_EX
        {
            // Need the const member dispatched.
            const M &cm = m;
            typename E::const_iterator2 it2e (e ().begin2 ());
            typename E::const_iterator2 it2e_end (e ().end2 ());
            while (it2e != it2e_end) {
                typename E::const_iterator1 it1e (it2e.begin ());
                typename E::const_iterator1 it1e_end (it2e.end ());
                while (it1e != it1e_end) {
                    // FIXME: we need a better floating point comparison...
                    check (*it1e == cm (it1e.index2 (), it1e.index1 ()), bad_index ());
                    ++ it1e;
                }
                ++ it2e;
            }
        }
#endif
#endif
    }
    // Sparse row major case
    template<class F, class M, class E>
    // This function seems to be big. So we do not let the compiler inline it.
    // NUMERICS_INLINE
    void evaluate_matrix_assign (const F &f, M &m, const matrix_expression<E> &e, sparse_tag, row_major_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
        check (m.size1 () == e ().size1 (), bad_size ());
        check (m.size2 () == e ().size2 (), bad_size ());
        typedef F functor_type;
        m.clear ();
        typename E::const_iterator1 it1e (e ().begin1 ());
        typename E::const_iterator1 it1e_end (e ().end1 ());
        while (it1e != it1e_end) {
            typename matrix_row<const E>::const_iterator it2e ((*it1e).begin ());
            typename matrix_row<const E>::const_iterator it2e_end ((*it1e).end ());
            while (it2e != it2e_end)
                m.insert (it1e.index (), it2e.index (), *it2e), ++ it2e;
            ++ it1e;
        }
#else
        check (m.size1 () == e ().size1 (), bad_size ());
        check (m.size2 () == e ().size2 (), bad_size ());
        typedef F functor_type;
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
#endif
    }
    // Sparse column major case
    template<class F, class M, class E>
    // This function seems to be big. So we do not let the compiler inline it.
    // NUMERICS_INLINE
    void evaluate_matrix_assign (const F &f, M &m, const matrix_expression<E> &e, sparse_tag, column_major_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
        check (m.size1 () == e ().size1 (), bad_size ());
        check (m.size2 () == e ().size2 (), bad_size ());
        typedef F functor_type;
        m.clear ();
        typename E::const_iterator2 it2e (e ().begin2 ());
        typename E::const_iterator2 it2e_end (e ().end2 ());
        while (it2e != it2e_end) {
            typename matrix_column<const E>::const_iterator it1e ((*it2e).begin ());
            typename matrix_column<const E>::const_iterator it1e_end ((*it2e).end ());
            while (it1e != it1e_end)
                m.insert (it1e.index (), it2e.index (), *it1e), ++ it1e;
            ++ it2e;
        }
#else
        check (m.size1 () == e ().size1 (), bad_size ());
        check (m.size2 () == e ().size2 (), bad_size ());
        typedef F functor_type;
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
#endif
    }
    // Sparse proxy row major case
    template<class F, class M, class E>
    // This function seems to be big. So we do not let the compiler inline it.
    // NUMERICS_INLINE
    void evaluate_matrix_assign (const F &f, M &m, const matrix_expression<E> &e, sparse_proxy_tag, row_major_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
        check (m.size1 () == e ().size1 (), bad_size ());
        check (m.size2 () == e ().size2 (), bad_size ());
        typedef F functor_type;
        typedef typename M::value_type value_type;
        typename M::iterator1 it1 (m.begin1 ());
        typename M::iterator1 it1_end (m.end1 ());
        typename E::const_iterator1 it1e (e ().begin1 ());
        typename E::const_iterator1 it1e_end (e ().end1 ());
        while (it1 != it1_end && it1e != it1e_end) {
            int compare = it1.index () - it1e.index ();
            if (compare == 0) {
                typename matrix_row<M>::iterator it2 ((*it1).begin ());
                typename matrix_row<M>::iterator it2_end ((*it1).end ());
                typename matrix_row<const E>::const_iterator it2e ((*it1e).begin ());
                typename matrix_row<const E>::const_iterator it2e_end ((*it1e).end ());
                while (it2 != it2_end && it2e != it2e_end) {
                    int compare = it2.index () - it2e.index ();
                    if (compare == 0) {
                        functor_type () (*it2, *it2e);
                        ++ it2, ++ it2e;
                    } else if (compare < 0) {
                        functor_type () (*it2, value_type ());
                        ++ it2;
                    } else if (compare > 0) {
#ifdef NUMERICS_BOUNDS_CHECK_EX
                        // Need the const member dispatched.
                        const M &cm = m;
                        // FIXME: we need a better floating point comparison...
                        check (*it2e == cm (it1e.index (), it2e.index ()), bad_index ());
#endif
                        ++ it2e;
                    }
                }
                while (it2 != it2_end) {
                    functor_type () (*it2, value_type ());
                    ++ it2;
                }
#ifdef NUMERICS_BOUNDS_CHECK_EX
                while (it2e != it2e_end) {
                    // Need the const member dispatched.
                    const M &cm = m;
                    // FIXME: we need a better floating point comparison...
                    check (*it2e == cm (it1e.index (), it2e.index ()), bad_index ());
                    ++ it2e;
                }
#endif
                ++ it1, ++ it1e;
            } else if (compare < 0) {
                typename matrix_row<M>::iterator it2 ((*it1).begin ());
                typename matrix_row<M>::iterator it2_end ((*it1).end ());
                while (it2 != it2_end) {
                    functor_type () (*it2, value_type ());
                    ++ it2;
                }
                ++ it1;
            } else if (compare > 0) {
#ifdef NUMERICS_BOUNDS_CHECK_EX
                typename matrix_row<const E>::const_iterator it2e ((*it1e).begin ());
                typename matrix_row<const E>::const_iterator it2e_end ((*it1e).end ());
                while (it2e != it2e_end) {
                    // Need the const member dispatched.
                    const M &cm = m;
                    // FIXME: we need a better floating point comparison...
                    check (*it2e == cm (it1e.index (), it2e.index ()), bad_index ());
                    ++ it2e;
                }
#endif
                ++ it1e;
            }
        }
        while (it1 != it1_end) {
            typename matrix_row<M>::iterator it2 ((*it1).begin ());
            typename matrix_row<M>::iterator it2_end ((*it1).end ());
            while (it2 != it2_end) {
                functor_type () (*it2, value_type ());
                ++ it2;
            }
            ++ it1;
        }
#ifdef NUMERICS_BOUNDS_CHECK_EX
        while (it1e != it1e_end) {
            typename matrix_row<const E>::const_iterator it2e ((*it1e).begin ());
            typename matrix_row<const E>::const_iterator it2e_end ((*it1e).end ());
            while (it2e != it2e_end) {
                // Need the const member dispatched.
                const M &cm = m;
                // FIXME: we need a better floating point comparison...
                check (*it2e == cm (it1e.index (), it2e.index ()), bad_index ());
                ++ it2e;
            }
            ++ it1e;
        }
#endif
#else
        check (m.size1 () == e ().size1 (), bad_size ());
        check (m.size2 () == e ().size2 (), bad_size ());
        typedef F functor_type;
        typedef typename M::value_type value_type;
        typename M::iterator1 it1 (m.begin1 ());
        typename M::iterator1 it1_end (m.end1 ());
        typename E::const_iterator1 it1e (e ().begin1 ());
        typename E::const_iterator1 it1e_end (e ().end1 ());
        while (it1 != it1_end && it1e != it1e_end) {
            int compare = it1.index1 () - it1e.index1 ();
            if (compare == 0) {
                typename M::iterator2 it2 (it1.begin ());
                typename M::iterator2 it2_end (it1.end ());
                typename E::const_iterator2 it2e (it1e.begin ());
                typename E::const_iterator2 it2e_end (it1e.end ());
                while (it2 != it2_end && it2e != it2e_end) {
                    int compare = it2.index2 () - it2e.index2 ();
                    if (compare == 0) {
                        functor_type () (*it2, *it2e);
                        ++ it2, ++ it2e;
                    } else if (compare < 0) {
                        functor_type () (*it2, value_type ());
                        ++ it2;
                    } else if (compare > 0) {
#ifdef NUMERICS_BOUNDS_CHECK_EX
                        // Need the const member dispatched.
                        const M &cm = m;
                        // FIXME: we need a better floating point comparison...
                        check (*it2e == cm (it2e.index1 (), it2e.index2 ()), bad_index ());
#endif
                        ++ it2e;
                    }
                }
                while (it2 != it2_end) {
                    functor_type () (*it2, value_type ());
                    ++ it2;
                }
#ifdef NUMERICS_BOUNDS_CHECK_EX
                while (it2e != it2e_end) {
                    // Need the const member dispatched.
                    const M &cm = m;
                    // FIXME: we need a better floating point comparison...
                    check (*it2e == cm (it2e.index1 (), it2e.index2 ()), bad_index ());
                    ++ it2e;
                }
#endif
                ++ it1, ++ it1e;
            } else if (compare < 0) {
                typename M::iterator2 it2 (it1.begin ());
                typename M::iterator2 it2_end (it1.end ());
                while (it2 != it2_end) {
                    functor_type () (*it2, value_type ());
                    ++ it2;
                }
                ++ it1;
            } else if (compare > 0) {
#ifdef NUMERICS_BOUNDS_CHECK_EX
                typename E::const_iterator2 it2e (it1e.begin ());
                typename E::const_iterator2 it2e_end (it1e.end ());
                while (it2e != it2e_end) {
                    // Need the const member dispatched.
                    const M &cm = m;
                    // FIXME: we need a better floating point comparison...
                    check (*it2e == cm (it2e.index1 (), it2e.index2 ()), bad_index ());
                    ++ it2e;
                }
#endif
                ++ it1e;
            }
        }
        while (it1 != it1_end) {
            typename M::iterator2 it2 (it1.begin ());
            typename M::iterator2 it2_end (it1.end ());
            while (it2 != it2_end) {
                functor_type () (*it2, value_type ());
                ++ it2;
            }
            ++ it1;
        }
#ifdef NUMERICS_BOUNDS_CHECK_EX
        while (it1e != it1e_end) {
            typename E::const_iterator2 it2e (it1e.begin ());
            typename E::const_iterator2 it2e_end (it1e.end ());
            while (it2e != it2e_end) {
                // Need the const member dispatched.
                const M &cm = m;
                // FIXME: we need a better floating point comparison...
                check (*it2e == cm (it2e.index1 (), it2e.index2 ()), bad_index ());
                ++ it2e;
            }
            ++ it1e;
        }
#endif
#endif
    }
    // Sparse proxy column major case
    template<class F, class M, class E>
    // This function seems to be big. So we do not let the compiler inline it.
    // NUMERICS_INLINE
    void evaluate_matrix_assign (const F &f, M &m, const matrix_expression<E> &e, sparse_proxy_tag, column_major_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
        check (m.size1 () == e ().size1 (), bad_size ());
        check (m.size2 () == e ().size2 (), bad_size ());
        typedef F functor_type;
        typedef typename M::value_type value_type;
        typename M::iterator2 it2 (m.begin2 ());
        typename M::iterator2 it2_end (m.end2 ());
        typename E::const_iterator2 it2e (e ().begin2 ());
        typename E::const_iterator2 it2e_end (e ().end2 ());
        while (it2 != it2_end && it2e != it2e_end) {
            int compare = it2.index () - it2e.index ();
            if (compare == 0) {
                typename matrix_column<M>::iterator it1 ((*it2).begin ());
                typename matrix_column<M>::iterator it1_end ((*it2).end ());
                typename matrix_column<const E>::const_iterator it1e ((*it2e).begin ());
                typename matrix_column<const E>::const_iterator it1e_end ((*it2e).end ());
                while (it1 != it1_end && it1e != it1e_end) {
                    int compare = it1.index () - it1e.index ();
                    if (compare == 0) {
                        functor_type () (*it1, *it1e);
                        ++ it1, ++ it1e;
                    } else if (compare < 0) {
                        functor_type () (*it1, value_type ());
                        ++ it1;
                    } else if (compare > 0) {
#ifdef NUMERICS_BOUNDS_CHECK_EX
                        // Need the const member dispatched.
                        const M &cm = m;
                        // FIXME: we need a better floating point comparison...
                        check (*it1e == cm (it1e.index (), it2e.index ()), bad_index ());
#endif
                        ++ it1e;
                    }
                }
                while (it1 != it1_end) {
                    functor_type () (*it1, value_type ());
                    ++ it1;
                }
#ifdef NUMERICS_BOUNDS_CHECK_EX
                while (it1e != it1e_end) {
                    // Need the const member dispatched.
                    const M &cm = m;
                    // FIXME: we need a better floating point comparison...
                    check (*it1e == cm (it1e.index (), it2e.index ()), bad_index ());
                    ++ it1e;
                }
#endif
                ++ it2, ++ it2e;
            } else if (compare < 0) {
                typename matrix_column<M>::iterator it1 ((*it2).begin ());
                typename matrix_column<M>::iterator it1_end ((*it2).end ());
                while (it1 != it1_end) {
                    functor_type () (*it1, value_type ());
                    ++ it1;
                }
                ++ it2;
            } else if (compare > 0) {
#ifdef NUMERICS_BOUNDS_CHECK_EX
                typename matrix_column<const E>::const_iterator it1e ((*it2e).begin ());
                typename matrix_column<const E>::const_iterator it1e_end ((*it2e).end ());
                while (it1e != it1e_end) {
                    // Need the const member dispatched.
                    const M &cm = m;
                    // FIXME: we need a better floating point comparison...
                    check (*it1e == cm (it1e.index (), it2e.index ()), bad_index ());
                    ++ it1e;
                }
#endif
                ++ it2e;
            }
        }
        while (it2 != it2_end) {
            typename matrix_column<M>::iterator it1 ((*it2).begin ());
            typename matrix_column<M>::iterator it1_end ((*it2).end ());
            while (it1 != it1_end) {
                functor_type () (*it1, value_type ());
                ++ it1;
            }
            ++ it2;
        }
#ifdef NUMERICS_BOUNDS_CHECK_EX
        while (it2e != it2e_end) {
            typename matrix_column<const E>::const_iterator it1e ((*it2e).begin ());
            typename matrix_column<const E>::const_iterator it1e_end ((*it2e).end ());
            while (it1e != it1e_end) {
                // Need the const member dispatched.
                const M &cm = m;
                // FIXME: we need a better floating point comparison...
                check (*it1e == cm (it1e.index (), it2e.index ()), bad_index ());
                ++ it1e;
            }
            ++ it2e;
        }
#endif
#else
        check (m.size1 () == e ().size1 (), bad_size ());
        check (m.size2 () == e ().size2 (), bad_size ());
        typedef F functor_type;
        typedef typename M::value_type value_type;
        typename M::iterator2 it2 (m.begin2 ());
        typename M::iterator2 it2_end (m.end2 ());
        typename E::const_iterator2 it2e (e ().begin2 ());
        typename E::const_iterator2 it2e_end (e ().end2 ());
        while (it2 != it2_end && it2e != it2e_end) {
            int compare = it2.index2 () - it2e.index2 ();
            if (compare == 0) {
                typename M::iterator1 it1 (it2.begin ());
                typename M::iterator1 it1_end (it2.end ());
                typename E::const_iterator1 it1e (it2e.begin ());
                typename E::const_iterator1 it1e_end (it2e.end ());
                while (it1 != it1_end && it1e != it1e_end) {
                    int compare = it1.index1 () - it1e.index1 ();
                    if (compare == 0) {
                        functor_type () (*it1, *it1e);
                        ++ it1, ++ it1e;
                    } else if (compare < 0) {
                        functor_type () (*it1, value_type ());
                        ++ it1;
                    } else if (compare > 0) {
#ifdef NUMERICS_BOUNDS_CHECK_EX
                        // Need the const member dispatched.
                        const M &cm = m;
                        // FIXME: we need a better floating point comparison...
                        check (*it1e == cm (it1e.index1 (), it1e.index2 ()), bad_index ());
#endif
                        ++ it1e;
                    }
                }
                while (it1 != it1_end) {
                    functor_type () (*it1, value_type ());
                    ++ it1;
                }
#ifdef NUMERICS_BOUNDS_CHECK_EX
                while (it1e != it1e_end) {
                    // Need the const member dispatched.
                    const M &cm = m;
                    // FIXME: we need a better floating point comparison...
                    check (*it1e == cm (it1e.index1 (), it1e.index2 ()), bad_index ());
                    ++ it1e;
                }
#endif
                ++ it2, ++ it2e;
            } else if (compare < 0) {
                typename M::iterator1 it1 (it2.begin ());
                typename M::iterator1 it1_end (it2.end ());
                while (it1 != it1_end) {
                    functor_type () (*it1, value_type ());
                    ++ it1;
                }
                ++ it2;
            } else if (compare > 0) {
#ifdef NUMERICS_BOUNDS_CHECK_EX
                typename E::const_iterator1 it1e (it2e.begin ());
                typename E::const_iterator1 it1e_end (it2e.end ());
                while (it1e != it1e_end) {
                    // Need the const member dispatched.
                    const M &cm = m;
                    // FIXME: we need a better floating point comparison...
                    check (*it1e == cm (it1e.index1 (), it1e.index2 ()), bad_index ());
                    ++ it1e;
                }
#endif
                ++ it2e;
            }
        }
        while (it2 != it2_end) {
            typename M::iterator1 it1 (it2.begin ());
            typename M::iterator1 it1_end (it2.end ());
            while (it1 != it1_end) {
                functor_type () (*it1, value_type ());
                ++ it1;
            }
            ++ it2;
        }
#ifdef NUMERICS_BOUNDS_CHECK_EX
        while (it2e != it2e_end) {
            typename E::const_iterator1 it1e (it2e.begin ());
            typename E::const_iterator1 it1e_end (it2e.end ());
            while (it1e != it1e_end) {
                // Need the const member dispatched.
                const M &cm = m;
                // FIXME: we need a better floating point comparison...
                check (*it1e == cm (it1e.index1 (), it1e.index2 ()), bad_index ());
                ++ it1e;
            }
            ++ it2e;
        }
#endif
#endif
    }
#endif

    // matrix assignment_operation matrix_expression
    template<class F>
    struct matrix_assign {
        typedef F functor_type;
        typedef typename F::assign_category assign_category;

#ifndef NUMERICS_SPECIALIZE_ASSIGN
        // Iterating row major case
        template<class M, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void iterating_assign (M &m, const matrix_expression<E> &e, row_major_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            typedef typename M::difference_type difference_type;
            difference_type size1 (common (m.size1 (), e ().size1 ()));
            typename M::iterator1 it1 (m.begin1 ());
#ifdef NUMERICS_BOUNDS_CHECK_EX
            // This fails for size2 == 0
            check (m.end1 () - it1 == size1, bad_size ());
#endif
            typename E::const_iterator1 it1e (e ().begin1 ());
#ifdef NUMERICS_BOUNDS_CHECK_EX
            // This fails for size2 == 0
            check (e ().end1 () - it1e == size1, bad_size ());
#endif
            while (-- size1 >= 0) {
                difference_type size2 (common (m.size2 (), e ().size2 ()));
                typename matrix_row<M>::iterator it2 ((*it1).begin ());
                check ((*it1).end () - it2 == size2, bad_size ());
                typename matrix_row<const E>::const_iterator it2e ((*it1e).begin ());
                check ((*it1e).end () - it2e == size2, bad_size ());
#ifndef NUMERICS_USE_DUFF_DEVICE
                while (-- size2 >= 0)
                    functor_type () (*it2, *it2e), ++ it2, ++ it2e;
#else
                DD (size2, 2, r, (functor_type () (*it2, *it2e), ++ it2, ++ it2e));
#endif
                ++ it1, ++ it1e;
            }
#else
            typedef typename M::difference_type difference_type;
            difference_type size1 (common (m.size1 (), e ().size1 ()));
            typename M::iterator1 it1 (m.begin1 ());
#ifdef NUMERICS_BOUNDS_CHECK_EX
            // This fails for size2 == 0
            check (m.end1 () - it1 == size1, bad_size ());
#endif
            typename E::const_iterator1 it1e (e ().begin1 ());
#ifdef NUMERICS_BOUNDS_CHECK_EX
            // This fails for size2 == 0
            check (e ().end1 () - it1e == size1, bad_size ());
#endif
            while (-- size1 >= 0) {
                difference_type size2 (common (m.size2 (), e ().size2 ()));
                typename M::iterator2 it2 (it1.begin ());
                check (it1.end () - it2 == size2, bad_size ());
                typename E::const_iterator2 it2e (it1e.begin ());
                check (it1e.end () - it2e == size2, bad_size ());
#ifndef NUMERICS_USE_DUFF_DEVICE
                while (-- size2 >= 0)
                    functor_type () (*it2, *it2e), ++ it2, ++ it2e;
#else
                DD (size2, 2, r, (functor_type () (*it2, *it2e), ++ it2, ++ it2e));
#endif
                ++ it1, ++ it1e;
            }
#endif
        }
        // Iterating column major case
        template<class M, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void iterating_assign (M &m, const matrix_expression<E> &e, column_major_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            typedef typename M::difference_type difference_type;
            difference_type size2 (common (m.size2 (), e ().size2 ()));
            typename M::iterator2 it2 (m.begin2 ());
#ifdef NUMERICS_BOUNDS_CHECK_EX
            // This fails for size1 == 0
            check (m.end2 () - it2 == size2, bad_size ());
#endif
            typename E::const_iterator2 it2e (e ().begin2 ());
#ifdef NUMERICS_BOUNDS_CHECK_EX
            // This fails for size1 == 0
            check (e ().end2 () - it2e == size2, bad_size ());
#endif
            while (-- size2 >= 0) {
                difference_type size1 (common (m.size1 (), e ().size1 ()));
                typename matrix_column<M>::iterator it1 ((*it2).begin ());
                check ((*it2).end () - it1 == size1, bad_size ());
                typename matrix_column<const E>::const_iterator it1e ((*it2e).begin ());
                check ((*it2e).end () - it1e == size1, bad_size ());
#ifndef NUMERICS_USE_DUFF_DEVICE
                while (-- size1 >= 0)
                    functor_type () (*it1, *it1e), ++ it1, ++ it1e;
#else
                DD (size1, 2, r, (functor_type () (*it1, *it1e), ++ it1, ++ it1e));
#endif
                ++ it2, ++ it2e;
            }
#else
            typedef typename M::difference_type difference_type;
            difference_type size2 (common (m.size2 (), e ().size2 ()));
            typename M::iterator2 it2 (m.begin2 ());
#ifdef NUMERICS_BOUNDS_CHECK_EX
            // This fails for size1 == 0
            check (m.end2 () - it2 == size2, bad_size ());
#endif
            typename E::const_iterator2 it2e (e ().begin2 ());
#ifdef NUMERICS_BOUNDS_CHECK_EX
            // This fails for size1 == 0
            check (e ().end2 () - it2e == size2, bad_size ());
#endif
            while (-- size2 >= 0) {
                difference_type size1 (common (m.size1 (), e ().size1 ()));
                typename M::iterator1 it1 (it2.begin ());
                check (it2.end () - it1 == size1, bad_size ());
                typename E::const_iterator1 it1e (it2e.begin ());
                check (it2e.end () - it1e == size1, bad_size ());
#ifndef NUMERICS_USE_DUFF_DEVICE
                while (-- size1 >= 0)
                    functor_type () (*it1, *it1e), ++ it1, ++ it1e;
#else
                DD (size1, 2, r, (functor_type () (*it1, *it1e), ++ it1, ++ it1e));
#endif
                ++ it2, ++ it2e;
            }
#endif
        }
        // Indexing row major case
        template<class M, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void indexing_assign (M &m, const matrix_expression<E> &e, row_major_tag) {
            typedef typename M::difference_type difference_type;
            difference_type size1 (common (m.size1 (), e ().size1 ()));
            difference_type size2 (common (m.size2 (), e ().size2 ()));
            for (difference_type i = 0; i < size1; ++ i) {
#ifndef NUMERICS_USE_DUFF_DEVICE
                for (difference_type j = 0; j < size2; ++ j)
                    functor_type () (m (i, j), e () (i, j));
#else
                difference_type j (0);
                DD (size2, 2, r, (functor_type () (m (i, j), e () (i, j)), ++ j));
#endif
            }
        }
        // Indexing column major case
        template<class M, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void indexing_assign (M &m, const matrix_expression<E> &e, column_major_tag) {
            typedef typename M::difference_type difference_type;
            difference_type size2 (common (m.size2 (), e ().size2 ()));
            difference_type size1 (common (m.size1 (), e ().size1 ()));
            for (difference_type j = 0; j < size2; ++ j)
#ifndef NUMERICS_USE_DUFF_DEVICE
                for (difference_type i = 0; i < size1; ++ i) {
                    functor_type () (m (i, j), e () (i, j));
#else
                difference_type i (0);
                DD (size1, 2, r, (functor_type () (m (i, j), e () (i, j)), ++ i));
#endif
            }
        }

        // Dense (proxy) case
        template<class M, class E, class C>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (M &m, const matrix_expression<E> &e, dense_proxy_tag, C) {
            typedef typename M::size_type size_type;
            typedef typename M::difference_type difference_type;
            difference_type size1 (common (m.size1 (), e ().size1 ()));
            difference_type size2 (common (m.size2 (), e ().size2 ()));
            if (size1 >= NUMERICS_ITERATOR_THRESHOLD &&
                size2 >= NUMERICS_ITERATOR_THRESHOLD)
                iterating_assign (m, e, C ());
            else
                indexing_assign (m, e, C ());
        }
        // Packed (proxy) row major case
        template<class M, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (M &m, const matrix_expression<E> &e, packed_proxy_tag, row_major_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            check (m.size1 () == e ().size1 (), bad_size ());
            check (m.size2 () == e ().size2 (), bad_size ());
            typedef typename M::size_type size_type;
            typedef typename M::value_type value_type;
            typename M::iterator1 it1 (m.begin1 ());
            typename M::iterator1 it1_end (m.end1 ());
            typename E::const_iterator1 it1e (e ().begin1 ());
            typename E::const_iterator1 it1e_end (e ().end1 ());
            if (it1e != it1e_end && it1e.index1 () < it1.index1 ())
                it1e += std::min (it1.index1 () - it1e.index1 (), size_type (it1e_end - it1e));
            while (it1 != it1_end && it1e != it1e_end && it1.index () < it1e.index ()) {
                typename matrix_row<M>::iterator it2 ((*it1).begin ());
                typename matrix_row<M>::iterator it2_end ((*it1).end ());
                while (it2 != it2_end) {
                    functor_type () (*it2, value_type ());
                    ++ it2;
                }
                ++ it1;
            }
            while (it1 != it1_end && it1e != it1e_end) {
                typename matrix_row<M>::iterator it2 ((*it1).begin ());
                typename matrix_row<M>::iterator it2_end ((*it1).end ());
                typename matrix_row<const E>::const_iterator it2e ((*it1e).begin ());
                typename matrix_row<const E>::const_iterator it2e_end ((*it1e).end ());
                if (it2e != it2e_end && it2e.index2 () < it2.index2 ()) 
                    it2e += std::min (it2.index2 () - it2e.index2 () <= size_type (it2e_end - it2e));
                while (it2 != it2_end && it2e != it2e_end && it2.index () < it2e.index ()) {
                    functor_type () (*it2, value_type ());
                    ++ it2;
                }
                while (it2 != it2_end && it2e != it2e_end) {
                    functor_type () (*it2, *it2e);
                    ++ it2, ++ it2e;
                }
                while (it2 != it2_end) {
                    functor_type () (*it2, value_type ());
                    ++ it2;
                }
                ++ it1, ++ it1e;
            }
            while (it1 != it1_end) {
                typename matrix_row<M>::iterator it2 ((*it1).begin ());
                typename matrix_row<M>::iterator it2_end ((*it1).end ());
                while (it2 != it2_end) {
                    functor_type () (*it2, value_type ());
                    ++ it2;
                }
                ++ it1;
            }
#ifdef NUMERICS_BOUNDS_CHECK_EX
            {
                // Need the const member dispatched.
                const M &cm = m;
                typename E::const_iterator1 it1e (e ().begin1 ());
                typename E::const_iterator1 it1e_end (e ().end1 ());
                while (it1e != it1e_end) {
                    typename matrix_row<const E>::const_iterator it2e ((*it1e).begin ());
                    typename matrix_row<const E>::const_iterator it2e_end ((*it1e).end ());
                    while (it2e != it2e_end) {
                        // FIXME: we need a better floating point comparison...
                        check (*it2e == cm (it1e.index (), it2e.index ()), bad_index ());
                        ++ it2e;
                    }
                    ++ it1e;
                }
            }
#endif
#else
            check (m.size1 () == e ().size1 (), bad_size ());
            check (m.size2 () == e ().size2 (), bad_size ());
            typedef typename M::size_type size_type;
            typedef typename M::value_type value_type;
            typename M::iterator1 it1 (m.begin1 ());
            typename M::iterator1 it1_end (m.end1 ());
            typename E::const_iterator1 it1e (e ().begin1 ());
            typename E::const_iterator1 it1e_end (e ().end1 ());
            if (it1e != it1e_end && it1e.index1 () < it1.index1 ())
                it1e += std::min (it1.index1 () - it1e.index1 (), size_type (it1e_end - it1e));
            while (it1 != it1_end && it1e != it1e_end && it1.index1 () < it1e.index1 ()) {
                typename M::iterator2 it2 (it1.begin ());
                typename M::iterator2 it2_end (it1.end ());
                while (it2 != it2_end) {
                    functor_type () (*it2, value_type ());
                    ++ it2;
                }
                ++ it1;
            }
            while (it1 != it1_end && it1e != it1e_end) {
                typename M::iterator2 it2 (it1.begin ());
                typename M::iterator2 it2_end (it1.end ());
                typename E::const_iterator2 it2e (it1e.begin ());
                typename E::const_iterator2 it2e_end (it1e.end ());
                if (it2e != it2e_end && it2e.index2 () < it2.index2 ()) 
                    it2e += std::min (it2.index2 () - it2e.index2 (), size_type (it2e_end - it2e));
                while (it2 != it2_end && it2e != it2e_end && it2.index2 () < it2e.index2 ()) {
                    functor_type () (*it2, value_type ());
                    ++ it2;
                }
                while (it2 != it2_end && it2e != it2e_end) {
                    functor_type () (*it2, *it2e);
                    ++ it2, ++ it2e;
                }
                while (it2 != it2_end) {
                    functor_type () (*it2, value_type ());
                    ++ it2;
                }
                ++ it1, ++ it1e;
            }
            while (it1 != it1_end) {
                typename M::iterator2 it2 (it1.begin ());
                typename M::iterator2 it2_end (it1.end ());
                while (it2 != it2_end) {
                    functor_type () (*it2, value_type ());
                    ++ it2;
                }
                ++ it1;
            }
#ifdef NUMERICS_BOUNDS_CHECK_EX
            {
                // Need the const member dispatched.
                const M &cm = m;
                typename E::const_iterator1 it1e (e ().begin1 ());
                typename E::const_iterator1 it1e_end (e ().end1 ());
                while (it1e != it1e_end) {
                    typename E::const_iterator2 it2e (it1e.begin ());
                    typename E::const_iterator2 it2e_end (it1e.end ());
                    while (it2e != it2e_end) {
                        // FIXME: we need a better floating point comparison...
                        check (*it2e == cm (it2e.index1 (), it2e.index2 ()), bad_index ());
                        ++ it2e;
                    }
                    ++ it1e;
                }
            }
#endif
#endif
        }
        // Packed (proxy) column major case
        template<class M, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (M &m, const matrix_expression<E> &e, packed_proxy_tag, column_major_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            check (m.size3 () == e ().size2 (), bad_size ());
            check (m.size1 () == e ().size1 (), bad_size ());
            typedef typename M::size_type size_type;
            typedef typename M::value_type value_type;
            typename M::iterator2 it2 (m.begin2 ());
            typename M::iterator2 it2_end (m.end2 ());
            typename E::const_iterator2 it2e (e ().begin2 ());
            typename E::const_iterator2 it2e_end (e ().end2 ());
            if (it2e != it2e_end && it2e.index2 () < it2.index2 ())
                it2e += std::min (it2.index2 () - it2e.index2 (), size_type (it2e_end - it2e));
            while (it2 != it2_end && it2e != it2e_end && it2.index () < it2e.index ()) {
                typename matrix_column<M>::iterator it1 ((*it2).begin ());
                typename matrix_column<M>::iterator it1_end ((*it2).end ());
                while (it1 != it1_end) {
                    functor_type () (*it1, value_type ());
                    ++ it1;
                }
                ++ it2;
            }
            while (it2 != it2_end && it2e != it2e_end) {
                typename matrix_column<M>::iterator it1 ((*it2).begin ());
                typename matrix_column<M>::iterator it1_end ((*it2).end ());
                typename matrix_column<const E>::const_iterator it1e ((*it2e).begin ());
                typename matrix_column<const E>::const_iterator it1e_end ((*it2e).end ());
                if (it1e != it1e_end && it1e.index1 () < it1.index1 ())
                    it1e += std::min (it1.index1 () - it1e.index1 () <= size_type (it1e_end - it1e));
                while (it1 != it1_end && it1e != it1e_end && it1.index () < it1e.index ()) {
                    functor_type () (*it1, value_type ());
                    ++ it1;
                }
                while (it1 != it1_end && it1e != it1e_end) {
                    functor_type () (*it1, *it1e);
                    ++ it1, ++ it1e;
                }
                while (it1 != it1_end) {
                    functor_type () (*it1, value_type ());
                    ++ it1;
                }
                ++ it2, ++ it2e;
            }
            while (it2 != it2_end) {
                typename matrix_column<M>::iterator it1 ((*it2).begin ());
                typename matrix_column<M>::iterator it1_end ((*it2).end ());
                while (it1 != it1_end) {
                    functor_type () (*it1, value_type ());
                    ++ it1;
                }
                ++ it2;
            }
#ifdef NUMERICS_BOUNDS_CHECK_EX
            {
                // Need the const member dispatched.
                const M &cm = m;
                typename E::const_iterator2 it2e (e ().begin2 ());
                typename E::const_iterator2 it2e_end (e ().end2 ());
                while (it2e != it2e_end) {
                    typename matrix_column<const E>::const_iterator it1e ((*it2e).begin ());
                    typename matrix_column<const E>::const_iterator it1e_end ((*it2e).end ());
                    while (it1e != it1e_end) {
                        // FIXME: we need a better floating point comparison...
                        check (*it1e == cm (it2e.index (), it1e.index ()), bad_index ());
                        ++ it1e;
                    }
                    ++ it2e;
                }
            }
#endif
#else
            check (m.size2 () == e ().size2 (), bad_size ());
            check (m.size1 () == e ().size1 (), bad_size ());
            typedef typename M::size_type size_type;
            typedef typename M::value_type value_type;
            typename M::iterator2 it2 (m.begin2 ());
            typename M::iterator2 it2_end (m.end2 ());
            typename E::const_iterator2 it2e (e ().begin2 ());
            typename E::const_iterator2 it2e_end (e ().end2 ());
            if (it2e != it2e_end && it2e.index2 () < it2.index2 ())
                it2e += std::min (it2.index2 () - it2e.index2 (), size_type (it2e_end - it2e));
            while (it2 != it2_end && it2e != it2e_end && it2.index2 () < it2e.index2 ()) {
                typename M::iterator1 it1 (it2.begin ());
                typename M::iterator1 it1_end (it2.end ());
                while (it1 != it1_end) {
                    functor_type () (*it1, value_type ());
                    ++ it1;
                }
                ++ it2;
            }
            while (it2 != it2_end && it2e != it2e_end) {
                typename M::iterator1 it1 (it2.begin ());
                typename M::iterator1 it1_end (it2.end ());
                typename E::const_iterator1 it1e (it2e.begin ());
                typename E::const_iterator1 it1e_end (it2e.end ());
                if (it1e != it1e_end && it1e.index1 () < it1.index1 ())
                    it1e += std::min (it1.index1 () - it1e.index1 (), size_type (it1e_end - it1e));
                while (it1 != it1_end && it1e != it1e_end && it1.index1 () < it1e.index1 ()) {
                    functor_type () (*it1, value_type ());
                    ++ it1;
                }
                while (it1 != it1_end && it1e != it1e_end) {
                    functor_type () (*it1, *it1e);
                    ++ it1, ++ it1e;
                }
                while (it1 != it1_end) {
                    functor_type () (*it1, value_type ());
                    ++ it1;
                }
                ++ it2, ++ it2e;
            }
            while (it2 != it2_end) {
                typename M::iterator1 it1 (it2.begin ());
                typename M::iterator1 it1_end (it2.end ());
                while (it1 != it1_end) {
                    functor_type () (*it1, value_type ());
                    ++ it1;
                }
                ++ it2;
            }
#ifdef NUMERICS_BOUNDS_CHECK_EX
            {
                // Need the const member dispatched.
                const M &cm = m;
                typename E::const_iterator2 it2e (e ().begin2 ());
                typename E::const_iterator2 it2e_end (e ().end2 ());
                while (it2e != it2e_end) {
                    typename E::const_iterator1 it1e (it2e.begin ());
                    typename E::const_iterator1 it1e_end (it2e.end ());
                    while (it1e != it1e_end) {
                        // FIXME: we need a better floating point comparison...
                        check (*it1e == cm (it1e.index2 (), it1e.index1 ()), bad_index ());
                        ++ it1e;
                    }
                    ++ it2e;
                }
            }
#endif
#endif
        }
        // Sparse row major case
        template<class M, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (M &m, const matrix_expression<E> &e, sparse_tag, row_major_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            check (m.size1 () == e ().size1 (), bad_size ());
            check (m.size2 () == e ().size2 (), bad_size ());
            m.clear ();
            typename E::const_iterator1 it1e (e ().begin1 ());
            typename E::const_iterator1 it1e_end (e ().end1 ());
            while (it1e != it1e_end) {
                typename matrix_row<const E>::const_iterator it2e ((*it1e).begin ());
                typename matrix_row<const E>::const_iterator it2e_end ((*it1e).end ());
                while (it2e != it2e_end)
                    m.insert (it1e.index (), it2e.index (), *it2e), ++ it2e;
                ++ it1e;
            }
#else
            check (m.size1 () == e ().size1 (), bad_size ());
            check (m.size2 () == e ().size2 (), bad_size ());
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
#endif
        }
        // Sparse column major case
        template<class M, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (M &m, const matrix_expression<E> &e, sparse_tag, column_major_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            check (m.size1 () == e ().size1 (), bad_size ());
            check (m.size2 () == e ().size2 (), bad_size ());
            m.clear ();
            typename E::const_iterator2 it2e (e ().begin2 ());
            typename E::const_iterator2 it2e_end (e ().end2 ());
            while (it2e != it2e_end) {
                typename matrix_column<const E>::const_iterator it1e ((*it2e).begin ());
                typename matrix_column<const E>::const_iterator it1e_end ((*it2e).end ());
                while (it1e != it1e_end)
                    m.insert (it1e.index (), it2e.index (), *it1e), ++ it1e;
                ++ it2e;
            }
#else
            check (m.size1 () == e ().size1 (), bad_size ());
            check (m.size2 () == e ().size2 (), bad_size ());
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
#endif
        }
        // Sparse proxy row major case
        template<class M, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (M &m, const matrix_expression<E> &e, sparse_proxy_tag, row_major_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            check (m.size1 () == e ().size1 (), bad_size ());
            check (m.size2 () == e ().size2 (), bad_size ());
            typedef typename M::value_type value_type;
            typename M::iterator1 it1 (m.begin1 ());
            typename M::iterator1 it1_end (m.end1 ());
            typename E::const_iterator1 it1e (e ().begin1 ());
            typename E::const_iterator1 it1e_end (e ().end1 ());
            while (it1 != it1_end && it1e != it1e_end) {
                int compare = it1.index () - it1e.index ();
                if (compare == 0) {
                    typename matrix_row<M>::iterator it2 ((*it1).begin ());
                    typename matrix_row<M>::iterator it2_end ((*it1).end ());
                    typename matrix_row<const E>::const_iterator it2e ((*it1e).begin ());
                    typename matrix_row<const E>::const_iterator it2e_end ((*it1e).end ());
                    while (it2 != it2_end && it2e != it2e_end) {
                        int compare = it2.index () - it2e.index ();
                        if (compare == 0) {
                            functor_type () (*it2, *it2e);
                            ++ it2, ++ it2e;
                        } else if (compare < 0) {
                            functor_type () (*it2, value_type ());
                            ++ it2;
                        } else if (compare > 0) {
#ifdef NUMERICS_BOUNDS_CHECK_EX
                            // Need the const member dispatched.
                            const M &cm = m;
                            // FIXME: we need a better floating point comparison...
                            check (*it2e == cm (it1e.index (), it2e.index ()), bad_index ());
#endif
                            ++ it2e;
                        }
                    }
                    while (it2 != it2_end) {
                        functor_type () (*it2, value_type ());
                        ++ it2;
                    }
#ifdef NUMERICS_BOUNDS_CHECK_EX
                    while (it2e != it2e_end) {
                        // Need the const member dispatched.
                        const M &cm = m;
                        // FIXME: we need a better floating point comparison...
                        check (*it2e == cm (it1e.index (), it2e.index ()), bad_index ());
                        ++ it2e;
                    }
#endif
                    ++ it1, ++ it1e;
                } else if (compare < 0) {
                    typename matrix_row<M>::iterator it2 ((*it1).begin ());
                    typename matrix_row<M>::iterator it2_end ((*it1).end ());
                    while (it2 != it2_end) {
                        functor_type () (*it2, value_type ());
                        ++ it2;
                    }
                    ++ it1;
                } else if (compare > 0) {
#ifdef NUMERICS_BOUNDS_CHECK_EX
                    typename matrix_row<const E>::const_iterator it2e ((*it1e).begin ());
                    typename matrix_row<const E>::const_iterator it2e_end ((*it1e).end ());
                    while (it2e != it2e_end) {
                        // Need the const member dispatched.
                        const M &cm = m;
                        // FIXME: we need a better floating point comparison...
                        check (*it2e == cm (it1e.index (), it2e.index ()), bad_index ());
                        ++ it2e;
                    }
#endif
                    ++ it1e;
                }
            }
            while (it1 != it1_end) {
                typename matrix_row<M>::iterator it2 ((*it1).begin ());
                typename matrix_row<M>::iterator it2_end ((*it1).end ());
                while (it2 != it2_end) {
                    functor_type () (*it2, value_type ());
                    ++ it2;
                }
                ++ it1;
            }
#ifdef NUMERICS_BOUNDS_CHECK_EX
            while (it1e != it1e_end) {
                typename matrix_row<const E>::const_iterator it2e ((*it1e).begin ());
                typename matrix_row<const E>::const_iterator it2e_end ((*it1e).end ());
                while (it2e != it2e_end) {
                    // Need the const member dispatched.
                    const M &cm = m;
                    // FIXME: we need a better floating point comparison...
                    check (*it2e == cm (it1e.index (), it2e.index ()), bad_index ());
                    ++ it2e;
                }
                ++ it1e;
            }
#endif
#else
            check (m.size1 () == e ().size1 (), bad_size ());
            check (m.size2 () == e ().size2 (), bad_size ());
            typedef typename M::value_type value_type;
            typename M::iterator1 it1 (m.begin1 ());
            typename M::iterator1 it1_end (m.end1 ());
            typename E::const_iterator1 it1e (e ().begin1 ());
            typename E::const_iterator1 it1e_end (e ().end1 ());
            while (it1 != it1_end && it1e != it1e_end) {
                int compare = it1.index1 () - it1e.index1 ();
                if (compare == 0) {
                    typename M::iterator2 it2 (it1.begin ());
                    typename M::iterator2 it2_end (it1.end ());
                    typename E::const_iterator2 it2e (it1e.begin ());
                    typename E::const_iterator2 it2e_end (it1e.end ());
                    while (it2 != it2_end && it2e != it2e_end) {
                        int compare = it2.index2 () - it2e.index2 ();
                        if (compare == 0) {
                            functor_type () (*it2, *it2e);
                            ++ it2, ++ it2e;
                        } else if (compare < 0) {
                            functor_type () (*it2, value_type ());
                            ++ it2;
                        } else if (compare > 0) {
#ifdef NUMERICS_BOUNDS_CHECK_EX
                            // Need the const member dispatched.
                            const M &cm = m;
                            // FIXME: we need a better floating point comparison...
                            check (*it2e == cm (it2e.index1 (), it2e.index2 ()), bad_index ());
#endif
                            ++ it2e;
                        }
                    }
                    while (it2 != it2_end) {
                        functor_type () (*it2, value_type ());
                        ++ it2;
                    }
#ifdef NUMERICS_BOUNDS_CHECK_EX
                    while (it2e != it2e_end) {
                        // Need the const member dispatched.
                        const M &cm = m;
                        // FIXME: we need a better floating point comparison...
                        check (*it2e == cm (it2e.index1 (), it2e.index2 ()), bad_index ());
                        ++ it2e;
                    }
#endif
                    ++ it1, ++ it1e;
                } else if (compare < 0) {
                    typename M::iterator2 it2 (it1.begin ());
                    typename M::iterator2 it2_end (it1.end ());
                    while (it2 != it2_end) {
                        functor_type () (*it2, value_type ());
                        ++ it2;
                    }
                    ++ it1;
                } else if (compare > 0) {
#ifdef NUMERICS_BOUNDS_CHECK_EX
                    typename E::const_iterator2 it2e (it1e.begin ());
                    typename E::const_iterator2 it2e_end (it1e.end ());
                    while (it2e != it2e_end) {
                        // Need the const member dispatched.
                        const M &cm = m;
                        // FIXME: we need a better floating point comparison...
                        check (*it2e == cm (it2e.index1 (), it2e.index2 ()), bad_index ());
                        ++ it2e;
                    }
#endif
                    ++ it1e;
                }
            }
            while (it1 != it1_end) {
                typename M::iterator2 it2 (it1.begin ());
                typename M::iterator2 it2_end (it1.end ());
                while (it2 != it2_end) {
                    functor_type () (*it2, value_type ());
                    ++ it2;
                }
                ++ it1;
            }
#ifdef NUMERICS_BOUNDS_CHECK_EX
            while (it1e != it1e_end) {
                typename E::const_iterator2 it2e (it1e.begin ());
                typename E::const_iterator2 it2e_end (it1e.end ());
                while (it2e != it2e_end) {
                    // Need the const member dispatched.
                    const M &cm = m;
                    // FIXME: we need a better floating point comparison...
                    check (*it2e == cm (it2e.index1 (), it2e.index2 ()), bad_index ());
                    ++ it2e;
                }
                ++ it1e;
            }
#endif
#endif
        }
        // Sparse proxy column major case
        template<class M, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (M &m, const matrix_expression<E> &e, sparse_proxy_tag, column_major_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            check (m.size1 () == e ().size1 (), bad_size ());
            check (m.size2 () == e ().size2 (), bad_size ());
            typedef typename M::value_type value_type;
            typename M::iterator2 it2 (m.begin2 ());
            typename M::iterator2 it2_end (m.end2 ());
            typename E::const_iterator2 it2e (e ().begin2 ());
            typename E::const_iterator2 it2e_end (e ().end2 ());
            while (it2 != it2_end && it2e != it2e_end) {
                int compare = it2.index () - it2e.index ();
                if (compare == 0) {
                    typename matrix_column<M>::iterator it1 ((*it2).begin ());
                    typename matrix_column<M>::iterator it1_end ((*it2).end ());
                    typename matrix_column<const E>::const_iterator it1e ((*it2e).begin ());
                    typename matrix_column<const E>::const_iterator it1e_end ((*it2e).end ());
                    while (it1 != it1_end && it1e != it1e_end) {
                        int compare = it1.index () - it1e.index ();
                        if (compare == 0) {
                            functor_type () (*it1, *it1e);
                            ++ it1, ++ it1e;
                        } else if (compare < 0) {
                            functor_type () (*it1, value_type ());
                            ++ it1;
                        } else if (compare > 0) {
#ifdef NUMERICS_BOUNDS_CHECK_EX
                            // Need the const member dispatched.
                            const M &cm = m;
                            // FIXME: we need a better floating point comparison...
                            check (*it1e == cm (it1e.index (), it2e.index ()), bad_index ());
#endif
                            ++ it1e;
                        }
                    }
                    while (it1 != it1_end) {
                        functor_type () (*it1, value_type ());
                        ++ it1;
                    }
#ifdef NUMERICS_BOUNDS_CHECK_EX
                    while (it1e != it1e_end) {
                        // Need the const member dispatched.
                        const M &cm = m;
                        // FIXME: we need a better floating point comparison...
                        check (*it1e == cm (it1e.index (), it2e.index ()), bad_index ());
                        ++ it1e;
                    }
#endif
                    ++ it2, ++ it2e;
                } else if (compare < 0) {
                    typename matrix_column<M>::iterator it1 ((*it2).begin ());
                    typename matrix_column<M>::iterator it1_end ((*it2).end ());
                    while (it1 != it1_end) {
                        functor_type () (*it1, value_type ());
                        ++ it1;
                    }
                    ++ it2;
                } else if (compare > 0) {
#ifdef NUMERICS_BOUNDS_CHECK_EX
                    typename matrix_column<const E>::const_iterator it1e ((*it2e).begin ());
                    typename matrix_column<const E>::const_iterator it1e_end ((*it2e).end ());
                    while (it1e != it1e_end) {
                        // Need the const member dispatched.
                        const M &cm = m;
                        // FIXME: we need a better floating point comparison...
                        check (*it1e == cm (it1e.index (), it2e.index ()), bad_index ());
                        ++ it1e;
                    }
#endif
                    ++ it2e;
                }
            }
            while (it2 != it2_end) {
                typename matrix_column<M>::iterator it1 ((*it2).begin ());
                typename matrix_column<M>::iterator it1_end ((*it2).end ());
                while (it1 != it1_end) {
                    functor_type () (*it1, value_type ());
                    ++ it1;
                }
                ++ it2;
            }
#ifdef NUMERICS_BOUNDS_CHECK_EX
            while (it2e != it2e_end) {
                typename matrix_column<const E>::const_iterator it1e ((*it2e).begin ());
                typename matrix_column<const E>::const_iterator it1e_end ((*it2e).end ());
                while (it1e != it1e_end) {
                    // Need the const member dispatched.
                    const M &cm = m;
                    // FIXME: we need a better floating point comparison...
                    check (*it1e == cm (it1e.index (), it2e.index ()), bad_index ());
                    ++ it1e;
                }
                ++ it2e;
            }
#endif
#else
            check (m.size1 () == e ().size1 (), bad_size ());
            check (m.size2 () == e ().size2 (), bad_size ());
            typedef typename M::value_type value_type;
            typename M::iterator2 it2 (m.begin2 ());
            typename M::iterator2 it2_end (m.end2 ());
            typename E::const_iterator2 it2e (e ().begin2 ());
            typename E::const_iterator2 it2e_end (e ().end2 ());
            while (it2 != it2_end && it2e != it2e_end) {
                int compare = it2.index2 () - it2e.index2 ();
                if (compare == 0) {
                    typename M::iterator1 it1 (it2.begin ());
                    typename M::iterator1 it1_end (it2.end ());
                    typename E::const_iterator1 it1e (it2e.begin ());
                    typename E::const_iterator1 it1e_end (it2e.end ());
                    while (it1 != it1_end && it1e != it1e_end) {
                        int compare = it1.index1 () - it1e.index1 ();
                        if (compare == 0) {
                            functor_type () (*it1, *it1e);
                            ++ it1, ++ it1e;
                        } else if (compare < 0) {
                            functor_type () (*it1, value_type ());
                            ++ it1;
                        } else if (compare > 0) {
#ifdef NUMERICS_BOUNDS_CHECK_EX
                            // Need the const member dispatched.
                            const M &cm = m;
                            // FIXME: we need a better floating point comparison...
                            check (*it1e == cm (it1e.index1 (), it1e.index2 ()), bad_index ());
#endif
                            ++ it1e;
                        }
                    }
                    while (it1 != it1_end) {
                        functor_type () (*it1, value_type ());
                        ++ it1;
                    }
#ifdef NUMERICS_BOUNDS_CHECK_EX
                    while (it1e != it1e_end) {
                        // Need the const member dispatched.
                        const M &cm = m;
                        // FIXME: we need a better floating point comparison...
                        check (*it1e == cm (it1e.index1 (), it1e.index2 ()), bad_index ());
                        ++ it1e;
                    }
#endif
                    ++ it2, ++ it2e;
                } else if (compare < 0) {
                    typename M::iterator1 it1 (it2.begin ());
                    typename M::iterator1 it1_end (it2.end ());
                    while (it1 != it1_end) {
                        functor_type () (*it1, value_type ());
                        ++ it1;
                    }
                    ++ it2;
                } else if (compare > 0) {
#ifdef NUMERICS_BOUNDS_CHECK_EX
                    typename E::const_iterator1 it1e (it2e.begin ());
                    typename E::const_iterator1 it1e_end (it2e.end ());
                    while (it1e != it1e_end) {
                        // Need the const member dispatched.
                        const M &cm = m;
                        // FIXME: we need a better floating point comparison...
                        check (*it1e == cm (it1e.index1 (), it1e.index2 ()), bad_index ());
                        ++ it1e;
                    }
#endif
                    ++ it2e;
                }
            }
            while (it2 != it2_end) {
                typename M::iterator1 it1 (it2.begin ());
                typename M::iterator1 it1_end (it2.end ());
                while (it1 != it1_end) {
                    functor_type () (*it1, value_type ());
                    ++ it1;
                }
                ++ it2;
            }
#ifdef NUMERICS_BOUNDS_CHECK_EX
            while (it2e != it2e_end) {
                typename E::const_iterator1 it1e (it2e.begin ());
                typename E::const_iterator1 it1e_end (it2e.end ());
                while (it1e != it1e_end) {
                    // Need the const member dispatched.
                    const M &cm = m;
                    // FIXME: we need a better floating point comparison...
                    check (*it1e == cm (it1e.index1 (), it1e.index2 ()), bad_index ());
                    ++ it1e;
                }
                ++ it2e;
            }
#endif
#endif
        }
#endif

        // Dispatcher
        template<class M, class E>
        NUMERICS_INLINE
        void operator () (M &m, const matrix_expression<E> &e) {
            typedef typename matrix_assign_traits<NUMERICS_TYPENAME M::storage_category,
                                                  assign_category,
                                                  NUMERICS_TYPENAME E::const_iterator1::iterator_category,
                                                  NUMERICS_TYPENAME E::const_iterator2::iterator_category>::storage_category storage_category;
            // FIXME: can we improve the dispatch here?
            // typedef typename E::orientation_category orientation_category;
            typedef typename M::orientation_category orientation_category;
#ifndef NUMERICS_SPECIALIZE_ASSIGN
            operator () (m, e, storage_category (), orientation_category ());
#else
            evaluate_matrix_assign (functor_type (), m, e, storage_category (), orientation_category ());
#endif
        }
    };

    template<class LS, class RI1, class RI2>
    struct matrix_swap_traits {
        typedef LS storage_category;
    };

    template<>
    struct matrix_swap_traits<dense_proxy_tag, sparse_bidirectional_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_tag storage_category;
    };

    template<>
    struct matrix_swap_traits<packed_proxy_tag, sparse_bidirectional_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_tag storage_category;
    };

#ifdef NUMERICS_SPECIALIZE_ASSIGN
    // Dense (proxy) row major case
    template<class F, class M, class E>
    // This function seems to be big. So we do not let the compiler inline it.
    // NUMERICS_INLINE
    void evaluate_matrix_swap (const F &f, M &m, matrix_expression<E> &e, dense_proxy_tag, row_major_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
        typedef F functor_type;
        typedef typename M::size_type size_type;
        typedef typename M::difference_type difference_type;
        typename M::iterator1 it1 (m.begin1 ());
        typename E::iterator1 it1e (e ().begin1 ());
        difference_type size1 (common (m.size1 (), size_type (e ().end1 () - it1e)));
        while (-- size1 >= 0) {
            typename matrix_row<M>::iterator it2 ((*it1).begin ());
            typename matrix_row<E>::iterator it2e ((*it1e).begin ());
            difference_type size2 (common (m.size2 (), size_type ((*it1e).end () - it2e)));
            while (-- size2 >= 0)
                functor_type () (*it2, *it2e), ++ it2, ++ it2e;
            ++ it1, ++ it1e;
        }
#else
        typedef F functor_type;
        typedef typename M::size_type size_type;
        typedef typename M::difference_type difference_type;
        typename M::iterator1 it1 (m.begin1 ());
        typename E::iterator1 it1e (e ().begin1 ());
        difference_type size1 (common (m.size1 (), size_type (e ().end1 () - it1e)));
        while (-- size1 >= 0) {
            typename M::iterator2 it2 (it1.begin ());
            typename E::iterator2 it2e (it1e.begin ());
            difference_type size2 (common (m.size2 (), size_type (it1e.end () - it2e)));
            while (-- size2 >= 0)
                functor_type () (*it2, *it2e), ++ it2, ++ it2e;
            ++ it1, ++ it1e;
        }
#endif
    }
    // Dense (proxy) column major case
    template<class F, class M, class E>
    // This function seems to be big. So we do not let the compiler inline it.
    // NUMERICS_INLINE
    void evaluate_matrix_swap (const F &f, M &m, matrix_expression<E> &e, dense_proxy_tag, column_major_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
        typedef F functor_type;
        typedef typename M::size_type size_type;
        typedef typename M::difference_type difference_type;
        typename M::iterator2 it2 (m.begin2 ());
        typename E::iterator2 it2e (e ().begin2 ());
        difference_type size2 (common (m.size2 (), size_type (e ().end2 () - it2e)));
        while (-- size2 >= 0) {
            typename matrix_column<M>::iterator it1 ((*it2).begin ());
            typename matrix_column<E>::iterator it1e ((*it2e).begin ());
            difference_type size1 (common (m.size1 (), size_type ((*it2e).end () - it1e)));
            while (-- size1 >= 0)
                functor_type () (*it1, *it1e), ++ it1, ++ it1e;
            ++ it2, ++ it2e;
        }
#else
        typedef F functor_type;
        typedef typename M::size_type size_type;
        typedef typename M::difference_type difference_type;
        typename M::iterator2 it2 (m.begin2 ());
        typename E::iterator2 it2e (e ().begin2 ());
        difference_type size2 (common (m.size2 (), size_type (e ().end2 () - it2e)));
        while (-- size2 >= 0) {
            typename M::iterator1 it1 (it2.begin ());
            typename E::iterator1 it1e (it2e.begin ());
            difference_type size1 (common (m.size1 (), size_type (it2e.end () - it1e)));
            while (-- size1 >= 0)
                functor_type () (*it1, *it1e), ++ it1, ++ it1e;
            ++ it2, ++ it2e;
        }
#endif
    }
    // Packed (proxy) row major case
    template<class F, class M, class E>
    // This function seems to be big. So we do not let the compiler inline it.
    // NUMERICS_INLINE
    void evaluate_matrix_swap (const F &f, M &m, matrix_expression<E> &e, packed_proxy_tag, row_major_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
        typedef F functor_type;
        typedef typename M::size_type size_type;
        typedef typename M::difference_type difference_type;
        typename M::iterator1 it1 (m.begin1 ());
        typename E::iterator1 it1e (e ().begin1 ());
        difference_type size1 (common (m.end1 () - it1, e ().end1 () - it1e));
        while (-- size1 >= 0) {
            typename matrix_row<M>::iterator it2 ((*it1).begin ());
            typename matrix_row<E>::iterator it2e ((*it1e).begin ());
            difference_type size2 (common ((*it1).end () - it2, (*it1e).end () - it2e));
            while (-- size2 >= 0)
                functor_type () (*it2, *it2e), ++ it2, ++ it2e;
            ++ it1, ++ it1e;
        }
#else
        typedef F functor_type;
        typedef typename M::size_type size_type;
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
#endif
    }
    // Packed (proxy) column major case
    template<class F, class M, class E>
    // This function seems to be big. So we do not let the compiler inline it.
    // NUMERICS_INLINE
    void evaluate_matrix_swap (const F &f, M &m, matrix_expression<E> &e, packed_proxy_tag, column_major_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
        typedef F functor_type;
        typedef typename M::size_type size_type;
        typedef typename M::difference_type difference_type;
        typename M::iterator2 it2 (m.begin2 ());
        typename E::iterator2 it2e (e ().begin2 ());
        difference_type size2 (common (m.end2 () - it2, e ().end2 () - it2e));
        while (-- size2 >= 0) {
            typename matrix_column<M>::iterator it1 ((*it2).begin ());
            typename matrix_column<E>::iterator it1e ((*it2e).begin ());
            difference_type size1 (common ((*it2).end () - it1, (*it2e).end () - it1e));
            while (-- size1 >= 0)
                functor_type () (*it1, *it1e), ++ it1, ++ it1e;
            ++ it2, ++ it2e;
        }
#else
        typedef F functor_type;
        typedef typename M::size_type size_type;
        typedef typename M::difference_type difference_type;
        typename M::iterator2 it2 (m.begin2 ());
        typename E::iterator2 it2e (e ().begin2 ());
        difference_type size2 (common (m.end2 () - it2, e ().end2 () - it2e));
        while (-- size2 >= 0) {
            typename M::iterator1 it1 (it2.begin ());
            typename E::iterator1 it1e (it2e.begin ());
            difference_type size1 (common (it2.end () - it1, it2e.end () - it1e));
            while (-- size1 >= 0)
                functor_type () (*it1, *it1e), ++ it1, ++ it1e;
            ++ it2, ++ it2e;
        }
#endif
    }
    // Sparse (proxy) row major case
    template<class F, class M, class E>
    // This function seems to be big. So we do not let the compiler inline it.
    // NUMERICS_INLINE
    void evaluate_matrix_swap (const F &f, M &m, matrix_expression<E> &e, sparse_proxy_tag, row_major_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
        check (m.size1 () == e ().size1 (), bad_size ());
        check (m.size2 () == e ().size2 (), bad_size ());
        typedef F functor_type;
        typedef typename M::value_type value_type;
        typename M::iterator1 it1 (m.begin1 ());
        typename M::iterator1 it1_end (m.end1 ());
        typename E::iterator1 it1e (e ().begin1 ());
        typename E::iterator1 it1e_end (e ().end1 ());
        while (it1 != it1_end && it1e != it1e_end) {
            int compare = it1.index () - it1e.index ();
            if (compare == 0) {
                typename matrix_row<M>::iterator it2 ((*it1).begin ());
                typename matrix_row<M>::iterator it2_end ((*it1).end ());
                typename matrix_row<const E>::iterator it2e ((*it1e).begin ());
                typename matrix_row<const E>::iterator it2e_end ((*it1e).end ());
                while (it2 != it2_end && it2e != it2e_end) {
                    int compare = it2.index () - it2e.index ();
                    if (compare == 0) {
                        functor_type () (*it2, *it2e);
                        ++ it2, ++ it2e;
                    } else if (compare < 0) {
                        throw external_logic ();
                    } else if (compare > 0) {
#ifdef NUMERICS_BOUNDS_CHECK_EX
                        // Need the const member dispatched.
                        const M &cm = m;
                        // FIXME: we need a better floating point comparison...
                        check (*it2e == cm (it1e.index (), it2e.index ()), bad_index ());
#endif
                        ++ it2e;
                    }
                }
                if (it2 != it2_end) {
                    throw external_logic ();
                }
#ifdef NUMERICS_BOUNDS_CHECK_EX
                while (it2e != it2e_end) {
                    // Need the const member dispatched.
                    const M &cm = m;
                    // FIXME: we need a better floating point comparison...
                    check (*it2e == cm (it1e.index (), it2e.index ()), bad_index ());
                    ++ it2e;
                }
#endif
                ++ it1, ++ it1e;
            } else if (compare < 0) {
                typename matrix_row<M>::iterator it2 ((*it1).begin ());
                typename matrix_row<M>::iterator it2_end ((*it1).end ());
                if (it2 != it2_end) {
                    throw external_logic ();
                }
                ++ it1;
            } else if (compare > 0) {
#ifdef NUMERICS_BOUNDS_CHECK_EX
                typename matrix_row<const E>::const_iterator it2e ((*it1e).begin ());
                typename matrix_row<const E>::const_iterator it2e_end ((*it1e).end ());
                while (it2e != it2e_end) {
                    // Need the const member dispatched.
                    const M &cm = m;
                    // FIXME: we need a better floating point comparison...
                    check (*it2e == cm (it1e.index (), it2e.index ()), bad_index ());
                    ++ it2e;
                }
#endif
                ++ it1e;
            }
        }
        while (it1 != it1_end) {
            typename matrix_row<M>::iterator it2 ((*it1).begin ());
            typename matrix_row<M>::iterator it2_end ((*it1).end ());
            if (it2 != it2_end) {
                throw external_logic ();
            }
            ++ it1;
        }
#ifdef NUMERICS_BOUNDS_CHECK_EX
        while (it1e != it1e_end) {
            typename matrix_row<const E>::const_iterator it2e ((*it1e).begin ());
            typename matrix_row<const E>::const_iterator it2e_end ((*it1e).end ());
            while (it2e != it2e_end) {
                // Need the const member dispatched.
                const M &cm = m;
                // FIXME: we need a better floating point comparison...
                check (*it2e == cm (it1e.index (), it2e.index ()), bad_index ());
                ++ it2e;
            }
            ++ it1e;
        }
#endif
#else
        check (m.size1 () == e ().size1 (), bad_size ());
        check (m.size2 () == e ().size2 (), bad_size ());
        typedef F functor_type;
        typedef typename M::value_type value_type;
        typename M::iterator1 it1 (m.begin1 ());
        typename M::iterator1 it1_end (m.end1 ());
        typename E::iterator1 it1e (e ().begin1 ());
        typename E::iterator1 it1e_end (e ().end1 ());
        while (it1 != it1_end && it1e != it1e_end) {
            int compare = it1.index1 () - it1e.index1 ();
            if (compare == 0) {
                typename M::iterator2 it2 (it1.begin ());
                typename M::iterator2 it2_end (it1.end ());
                typename E::iterator2 it2e (it1e.begin ());
                typename E::iterator2 it2e_end (it1e.end ());
                while (it2 != it2_end && it2e != it2e_end) {
                    int compare = it2.index2 () - it2e.index2 ();
                    if (compare == 0) {
                        functor_type () (*it2, *it2e);
                        ++ it2, ++ it2e;
                    } else if (compare < 0) {
                        throw external_logic ();
                    } else if (compare > 0) {
#ifdef NUMERICS_BOUNDS_CHECK_EX
                        // Need the const member dispatched.
                        const M &cm = m;
                        // FIXME: we need a better floating point comparison...
                        check (*it2e == cm (it2e.index1 (), it2e.index2 ()), bad_index ());
#endif
                        ++ it2e;
                    }
                }
                if (it2 != it2_end) {
                    throw external_logic ();
                }
#ifdef NUMERICS_BOUNDS_CHECK_EX
                while (it2e != it2e_end) {
                    // Need the const member dispatched.
                    const M &cm = m;
                    // FIXME: we need a better floating point comparison...
                    check (*it2e == cm (it2e.index1 (), it2e.index2 ()), bad_index ());
                    ++ it2e;
                }
#endif
                ++ it1, ++ it1e;
            } else if (compare < 0) {
                typename M::iterator2 it2 (it1.begin ());
                typename M::iterator2 it2_end (it1.end ());
                if (it2 != it2_end) {
                    throw external_logic ();
                }
                ++ it1;
            } else if (compare > 0) {
#ifdef NUMERICS_BOUNDS_CHECK_EX
                typename E::const_iterator2 it2e (it1e.begin ());
                typename E::const_iterator2 it2e_end (it1e.end ());
                while (it2e != it2e_end) {
                    // Need the const member dispatched.
                    const M &cm = m;
                    // FIXME: we need a better floating point comparison...
                    check (*it2e == cm (it2e.index1 (), it2e.index2 ()), bad_index ());
                    ++ it2e;
                }
#endif
                ++ it1e;
            }
        }
        while (it1 != it1_end) {
            typename M::iterator2 it2 (it1.begin ());
            typename M::iterator2 it2_end (it1.end ());
            if (it2 != it2_end) {
                throw external_logic ();
            }
            ++ it1;
        }
#ifdef NUMERICS_BOUNDS_CHECK_EX
        while (it1e != it1e_end) {
            typename E::const_iterator2 it2e (it1e.begin ());
            typename E::const_iterator2 it2e_end (it1e.end ());
            while (it2e != it2e_end) {
                // Need the const member dispatched.
                const M &cm = m;
                // FIXME: we need a better floating point comparison...
                check (*it2e == cm (it2e.index1 (), it2e.index2 ()), bad_index ());
                ++ it2e;
            }
            ++ it1e;
        }
#endif
#endif
    }
    // Sparse (proxy) column major case
    template<class F, class M, class E>
    // This function seems to be big. So we do not let the compiler inline it.
    // NUMERICS_INLINE
    void evaluate_matrix_swap (const F &f, M &m, matrix_expression<E> &e, sparse_proxy_tag, column_major_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
        check (m.size1 () == e ().size1 (), bad_size ());
        check (m.size2 () == e ().size2 (), bad_size ());
        typedef F functor_type;
        typedef typename M::value_type value_type;
        typename M::iterator2 it2 (m.begin2 ());
        typename M::iterator2 it2_end (m.end2 ());
        typename E::iterator2 it2e (e ().begin2 ());
        typename E::iterator2 it2e_end (e ().end2 ());
        while (it2 != it2_end && it2e != it2e_end) {
            int compare = it2.index () - it2e.index ();
            if (compare == 0) {
                typename matrix_column<M>::iterator it1 ((*it2).begin ());
                typename matrix_column<M>::iterator it1_end ((*it2).end ());
                typename matrix_column<const E>::iterator it1e ((*it2e).begin ());
                typename matrix_column<const E>::iterator it1e_end ((*it2e).end ());
                while (it1 != it1_end && it1e != it1e_end) {
                    int compare = it1.index () - it1e.index ();
                    if (compare == 0) {
                        functor_type () (*it1, *it1e);
                        ++ it1, ++ it1e;
                    } else if (compare < 0) {
                        throw external_logic ();
                    } else if (compare > 0) {
#ifdef NUMERICS_BOUNDS_CHECK_EX
                        // Need the const member dispatched.
                        const M &cm = m;
                        // FIXME: we need a better floating point comparison...
                        check (*it1e == cm (it1e.index (), it2e.index ()), bad_index ());
#endif
                        ++ it1e;
                    }
                }
                if (it1 != it1_end) {
                    throw external_logic ();
                }
#ifdef NUMERICS_BOUNDS_CHECK_EX
                while (it1e != it1e_end) {
                    // Need the const member dispatched.
                    const M &cm = m;
                    // FIXME: we need a better floating point comparison...
                    check (*it1e == cm (it1e.index (), it2e.index ()), bad_index ());
                    ++ it1e;
                }
#endif
                ++ it2, ++ it2e;
            } else if (compare < 0) {
                typename matrix_column<M>::iterator it1 ((*it2).begin ());
                typename matrix_column<M>::iterator it1_end ((*it2).end ());
                if (it1 != it1_end) {
                    throw external_logic ();
                }
                ++ it2;
            } else if (compare > 0) {
#ifdef NUMERICS_BOUNDS_CHECK_EX
                typename matrix_column<const E>::const_iterator it1e ((*it2e).begin ());
                typename matrix_column<const E>::const_iterator it1e_end ((*it2e).end ());
                while (it1e != it1e_end) {
                    // Need the const member dispatched.
                    const M &cm = m;
                    // FIXME: we need a better floating point comparison...
                    check (*it1e == cm (it1e.index (), it2e.index ()), bad_index ());
                    ++ it1e;
                }
#endif
                ++ it2e;
            }
        }
        while (it2 != it2_end) {
            typename matrix_column<M>::iterator it1 ((*it2).begin ());
            typename matrix_column<M>::iterator it1_end ((*it2).end ());
            if (it1 != it1_end) {
                throw external_logic ();
            }
            ++ it2;
        }
#ifdef NUMERICS_BOUNDS_CHECK_EX
        while (it2e != it2e_end) {
            typename matrix_column<const E>::const_iterator it1e ((*it2e).begin ());
            typename matrix_column<const E>::const_iterator it1e_end ((*it2e).end ());
            while (it1e != it1e_end) {
                // Need the const member dispatched.
                const M &cm = m;
                // FIXME: we need a better floating point comparison...
                check (*it1e == cm (it1e.index (), it2e.index ()), bad_index ());
                ++ it1e;
            }
            ++ it2e;
        }
#endif
#else
        check (m.size1 () == e ().size1 (), bad_size ());
        check (m.size2 () == e ().size2 (), bad_size ());
        typedef F functor_type;
        typedef typename M::value_type value_type;
        typename M::iterator2 it2 (m.begin2 ());
        typename M::iterator2 it2_end (m.end2 ());
        typename E::iterator2 it2e (e ().begin2 ());
        typename E::iterator2 it2e_end (e ().end2 ());
        while (it2 != it2_end && it2e != it2e_end) {
            int compare = it2.index2 () - it2e.index2 ();
            if (compare == 0) {
                typename M::iterator1 it1 (it2.begin ());
                typename M::iterator1 it1_end (it2.end ());
                typename E::iterator1 it1e (it2e.begin ());
                typename E::iterator1 it1e_end (it2e.end ());
                while (it1 != it1_end && it1e != it1e_end) {
                    int compare = it1.index1 () - it1e.index1 ();
                    if (compare == 0) {
                        functor_type () (*it1, *it1e);
                        ++ it1, ++ it1e;
                    } else if (compare < 0) {
                        throw external_logic ();
                    } else if (compare > 0) {
#ifdef NUMERICS_BOUNDS_CHECK_EX
                        // Need the const member dispatched.
                        const M &cm = m;
                        // FIXME: we need a better floating point comparison...
                        check (*it1e == cm (it1e.index1 (), it1e.index2 ()), bad_index ());
#endif
                        ++ it1e;
                    }
                }
                if (it1 != it1_end) {
                    throw external_logic ();
                }
#ifdef NUMERICS_BOUNDS_CHECK_EX
                while (it1e != it1e_end) {
                    // Need the const member dispatched.
                    const M &cm = m;
                    // FIXME: we need a better floating point comparison...
                    check (*it1e == cm (it1e.index1 (), it1e.index2 ()), bad_index ());
                    ++ it1e;
                }
#endif
                ++ it2, ++ it2e;
            } else if (compare < 0) {
                typename M::iterator1 it1 (it2.begin ());
                typename M::iterator1 it1_end (it2.end ());
                if (it1 != it1_end) {
                    throw external_logic ();
                }
                ++ it2;
            } else if (compare > 0) {
#ifdef NUMERICS_BOUNDS_CHECK_EX
                typename E::const_iterator1 it1e (it2e.begin ());
                typename E::const_iterator1 it1e_end (it2e.end ());
                while (it1e != it1e_end) {
                    // Need the const member dispatched.
                    const M &cm = m;
                    // FIXME: we need a better floating point comparison...
                    check (*it1e == cm (it1e.index1 (), it1e.index2 ()), bad_index ());
                    ++ it1e;
                }
#endif
                ++ it2e;
            }
        }
        while (it2 != it2_end) {
            typename M::iterator1 it1 (it2.begin ());
            typename M::iterator1 it1_end (it2.end ());
            if (it1 != it1_end) {
                throw external_logic ();
            }
            ++ it2;
        }
#ifdef NUMERICS_BOUNDS_CHECK_EX
        while (it2e != it2e_end) {
            typename E::const_iterator1 it1e (it2e.begin ());
            typename E::const_iterator1 it1e_end (it2e.end ());
            while (it1e != it1e_end) {
                // Need the const member dispatched.
                const M &cm = m;
                // FIXME: we need a better floating point comparison...
                check (*it1e == cm (it1e.index1 (), it1e.index2 ()), bad_index ());
                ++ it1e;
            }
            ++ it2e;
        }
#endif
#endif
    }
#endif

    // matrix swap_operation matrix_expression
    template<class F>
    struct matrix_swap {
        typedef F functor_type;

#ifndef NUMERICS_SPECIALIZE_ASSIGN
        // Dense (proxy) row major case
        template<class M, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (M &m, matrix_expression<E> &e, dense_proxy_tag, row_major_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            typedef typename M::size_type size_type;
            typedef typename M::difference_type difference_type;
            typename M::iterator1 it1 (m.begin1 ());
            typename E::iterator1 it1e (e ().begin1 ());
            difference_type size1 (common (m.size1 (), size_type (e ().end1 () - it1e)));
            while (-- size1 >= 0) {
                typename matrix_row<M>::iterator it2 ((*it1).begin ());
                typename matrix_row<E>::iterator it2e ((*it1e).begin ());
                difference_type size2 (common (m.size2 (), size_type ((*it1e).end () - it2e)));
                while (-- size2 >= 0)
                    functor_type () (*it2, *it2e), ++ it2, ++ it2e;
                ++ it1, ++ it1e;
            }
#else
            typedef typename M::size_type size_type;
            typedef typename M::difference_type difference_type;
            typename M::iterator1 it1 (m.begin1 ());
            typename E::iterator1 it1e (e ().begin1 ());
            difference_type size1 (common (m.size1 (), size_type (e ().end1 () - it1e)));
            while (-- size1 >= 0) {
                typename M::iterator2 it2 (it1.begin ());
                typename E::iterator2 it2e (it1e.begin ());
                difference_type size2 (common (m.size2 (), size_type (it1e.end () - it2e)));
                while (-- size2 >= 0)
                    functor_type () (*it2, *it2e), ++ it2, ++ it2e;
                ++ it1, ++ it1e;
            }
#endif
        }
        // Dense (proxy) column major case
        template<class M, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (M &m, matrix_expression<E> &e, dense_proxy_tag, column_major_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            typedef typename M::size_type size_type;
            typedef typename M::difference_type difference_type;
            typename M::iterator2 it2 (m.begin2 ());
            typename E::iterator2 it2e (e ().begin2 ());
            difference_type size2 (common (m.size2 (), size_type (e ().end2 () - it2e)));
            while (-- size2 >= 0) {
                typename matrix_column<M>::iterator it1 ((*it2).begin ());
                typename matrix_column<E>::iterator it1e ((*it2e).begin ());
                difference_type size1 (common (m.size1 (), size_type ((*it2e).end () - it1e)));
                while (-- size1 >= 0)
                    functor_type () (*it1, *it1e), ++ it1, ++ it1e;
                ++ it2, ++ it2e;
            }
#else
            typedef typename M::size_type size_type;
            typedef typename M::difference_type difference_type;
            typename M::iterator2 it2 (m.begin2 ());
            typename E::iterator2 it2e (e ().begin2 ());
            difference_type size2 (common (m.size2 (), size_type (e ().end2 () - it2e)));
            while (-- size2 >= 0) {
                typename M::iterator1 it1 (it2.begin ());
                typename E::iterator1 it1e (it2e.begin ());
                difference_type size1 (common (m.size1 (), size_type (it2e.end () - it1e)));
                while (-- size1 >= 0)
                    functor_type () (*it1, *it1e), ++ it1, ++ it1e;
                ++ it2, ++ it2e;
            }
#endif
        }
        // Packed (proxy) row major case
        template<class M, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (M &m, matrix_expression<E> &e, packed_proxy_tag, row_major_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            typedef typename M::size_type size_type;
            typedef typename M::difference_type difference_type;
            typename M::iterator1 it1 (m.begin1 ());
            typename E::iterator1 it1e (e ().begin1 ());
            difference_type size1 (common (m.end1 () - it1, e ().end1 () - it1e));
            while (-- size1 >= 0) {
                typename matrix_row<M>::iterator it2 ((*it1).begin ());
                typename matrix_row<E>::iterator it2e ((*it1e).begin ());
                difference_type size2 (common ((*it1).end () - it2, (*it1e).end () - it2e));
                while (-- size2 >= 0)
                    functor_type () (*it2, *it2e), ++ it2, ++ it2e;
                ++ it1, ++ it1e;
            }
#else
            typedef typename M::size_type size_type;
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
#endif
        }
        // Packed (proxy) column major case
        template<class M, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (M &m, matrix_expression<E> &e, packed_proxy_tag, column_major_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            typedef typename M::size_type size_type;
            typedef typename M::difference_type difference_type;
            typename M::iterator2 it2 (m.begin2 ());
            typename E::iterator2 it2e (e ().begin2 ());
            difference_type size2 (common (m.end2 () - it2, e ().end2 () - it2e));
            while (-- size2 >= 0) {
                typename matrix_column<M>::iterator it1 ((*it2).begin ());
                typename matrix_column<E>::iterator it1e ((*it2e).begin ());
                difference_type size1 (common ((*it2).end () - it1, (*it2e).end () - it1e));
                while (-- size1 >= 0)
                    functor_type () (*it1, *it1e), ++ it1, ++ it1e;
                ++ it2, ++ it2e;
            }
#else
            typedef typename M::size_type size_type;
            typedef typename M::difference_type difference_type;
            typename M::iterator2 it2 (m.begin2 ());
            typename E::iterator2 it2e (e ().begin2 ());
            difference_type size2 (common (m.end2 () - it2, e ().end2 () - it2e));
            while (-- size2 >= 0) {
                typename M::iterator1 it1 (it2.begin ());
                typename E::iterator1 it1e (it2e.begin ());
                difference_type size1 (common (it2.end () - it1, it2e.end () - it1e));
                while (-- size1 >= 0)
                    functor_type () (*it1, *it1e), ++ it1, ++ it1e;
                ++ it2, ++ it2e;
            }
#endif
        }
        // Sparse (proxy) row major case
        template<class M, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (M &m, matrix_expression<E> &e, sparse_proxy_tag, row_major_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            check (m.size1 () == e ().size1 (), bad_size ());
            check (m.size2 () == e ().size2 (), bad_size ());
            typedef typename M::value_type value_type;
            typename M::iterator1 it1 (m.begin1 ());
            typename M::iterator1 it1_end (m.end1 ());
            typename E::iterator1 it1e (e ().begin1 ());
            typename E::iterator1 it1e_end (e ().end1 ());
            while (it1 != it1_end && it1e != it1e_end) {
                int compare = it1.index () - it1e.index ();
                if (compare == 0) {
                    typename matrix_row<M>::iterator it2 ((*it1).begin ());
                    typename matrix_row<M>::iterator it2_end ((*it1).end ());
                    typename matrix_row<const E>::iterator it2e ((*it1e).begin ());
                    typename matrix_row<const E>::iterator it2e_end ((*it1e).end ());
                    while (it2 != it2_end && it2e != it2e_end) {
                        int compare = it2.index () - it2e.index ();
                        if (compare == 0) {
                            functor_type () (*it2, *it2e);
                            ++ it2, ++ it2e;
                        } else if (compare < 0) {
                            throw external_logic ();
                        } else if (compare > 0) {
#ifdef NUMERICS_BOUNDS_CHECK_EX
                            // Need the const member dispatched.
                            const M &cm = m;
                            // FIXME: we need a better floating point comparison...
                            check (*it2e == cm (it1e.index (), it2e.index ()), bad_index ());
#endif
                            ++ it2e;
                        }
                    }
                    if (it2 != it2_end) {
                        throw external_logic ();
                    }
#ifdef NUMERICS_BOUNDS_CHECK_EX
                    while (it2e != it2e_end) {
                        // Need the const member dispatched.
                        const M &cm = m;
                        // FIXME: we need a better floating point comparison...
                        check (*it2e == cm (it1e.index (), it2e.index ()), bad_index ());
                        ++ it2e;
                    }
#endif
                    ++ it1, ++ it1e;
                } else if (compare < 0) {
                    typename matrix_row<M>::iterator it2 ((*it1).begin ());
                    typename matrix_row<M>::iterator it2_end ((*it1).end ());
                    if (it2 != it2_end) {
                        throw external_logic ();
                    }
                    ++ it1;
                } else if (compare > 0) {
#ifdef NUMERICS_BOUNDS_CHECK_EX
                    typename matrix_row<const E>::const_iterator it2e ((*it1e).begin ());
                    typename matrix_row<const E>::const_iterator it2e_end ((*it1e).end ());
                    while (it2e != it2e_end) {
                        // Need the const member dispatched.
                        const M &cm = m;
                        // FIXME: we need a better floating point comparison...
                        check (*it2e == cm (it1e.index (), it2e.index ()), bad_index ());
                        ++ it2e;
                    }
#endif
                    ++ it1e;
                }
            }
            while (it1 != it1_end) {
                typename matrix_row<M>::iterator it2 ((*it1).begin ());
                typename matrix_row<M>::iterator it2_end ((*it1).end ());
                if (it2 != it2_end) {
                    throw external_logic ();
                }
                ++ it1;
            }
#ifdef NUMERICS_BOUNDS_CHECK_EX
            while (it1e != it1e_end) {
                typename matrix_row<const E>::const_iterator it2e ((*it1e).begin ());
                typename matrix_row<const E>::const_iterator it2e_end ((*it1e).end ());
                while (it2e != it2e_end) {
                    // Need the const member dispatched.
                    const M &cm = m;
                    // FIXME: we need a better floating point comparison...
                    check (*it2e == cm (it1e.index (), it2e.index ()), bad_index ());
                    ++ it2e;
                }
                ++ it1e;
            }
#endif
#else
            check (m.size1 () == e ().size1 (), bad_size ());
            check (m.size2 () == e ().size2 (), bad_size ());
            typedef typename M::value_type value_type;
            typename M::iterator1 it1 (m.begin1 ());
            typename M::iterator1 it1_end (m.end1 ());
            typename E::iterator1 it1e (e ().begin1 ());
            typename E::iterator1 it1e_end (e ().end1 ());
            while (it1 != it1_end && it1e != it1e_end) {
                int compare = it1.index1 () - it1e.index1 ();
                if (compare == 0) {
                    typename M::iterator2 it2 (it1.begin ());
                    typename M::iterator2 it2_end (it1.end ());
                    typename E::iterator2 it2e (it1e.begin ());
                    typename E::iterator2 it2e_end (it1e.end ());
                    while (it2 != it2_end && it2e != it2e_end) {
                        int compare = it2.index2 () - it2e.index2 ();
                        if (compare == 0) {
                            functor_type () (*it2, *it2e);
                            ++ it2, ++ it2e;
                        } else if (compare < 0) {
                            throw external_logic ();
                        } else if (compare > 0) {
#ifdef NUMERICS_BOUNDS_CHECK_EX
                            // Need the const member dispatched.
                            const M &cm = m;
                            // FIXME: we need a better floating point comparison...
                            check (*it2e == cm (it2e.index1 (), it2e.index2 ()), bad_index ());
#endif
                            ++ it2e;
                        }
                    }
                    if (it2 != it2_end) {
                        throw external_logic ();
                    }
#ifdef NUMERICS_BOUNDS_CHECK_EX
                    while (it2e != it2e_end) {
                        // Need the const member dispatched.
                        const M &cm = m;
                        // FIXME: we need a better floating point comparison...
                        check (*it2e == cm (it2e.index1 (), it2e.index2 ()), bad_index ());
                        ++ it2e;
                    }
#endif
                    ++ it1, ++ it1e;
                } else if (compare < 0) {
                    typename M::iterator2 it2 (it1.begin ());
                    typename M::iterator2 it2_end (it1.end ());
                    if (it2 != it2_end) {
                        throw external_logic ();
                    }
                    ++ it1;
                } else if (compare > 0) {
#ifdef NUMERICS_BOUNDS_CHECK_EX
                    typename E::const_iterator2 it2e (it1e.begin ());
                    typename E::const_iterator2 it2e_end (it1e.end ());
                    while (it2e != it2e_end) {
                        // Need the const member dispatched.
                        const M &cm = m;
                        // FIXME: we need a better floating point comparison...
                        check (*it2e == cm (it2e.index1 (), it2e.index2 ()), bad_index ());
                        ++ it2e;
                    }
#endif
                    ++ it1e;
                }
            }
            while (it1 != it1_end) {
                typename M::iterator2 it2 (it1.begin ());
                typename M::iterator2 it2_end (it1.end ());
                if (it2 != it2_end) {
                    throw external_logic ();
                }
                ++ it1;
            }
#ifdef NUMERICS_BOUNDS_CHECK_EX
            while (it1e != it1e_end) {
                typename E::const_iterator2 it2e (it1e.begin ());
                typename E::const_iterator2 it2e_end (it1e.end ());
                while (it2e != it2e_end) {
                    // Need the const member dispatched.
                    const M &cm = m;
                    // FIXME: we need a better floating point comparison...
                    check (*it2e == cm (it2e.index1 (), it2e.index2 ()), bad_index ());
                    ++ it2e;
                }
                ++ it1e;
            }
#endif
#endif
        }
        // Sparse (proxy) column major case
        template<class M, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (M &m, matrix_expression<E> &e, sparse_proxy_tag, column_major_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            check (m.size1 () == e ().size1 (), bad_size ());
            check (m.size2 () == e ().size2 (), bad_size ());
            typedef typename M::value_type value_type;
            typename M::iterator2 it2 (m.begin2 ());
            typename M::iterator2 it2_end (m.end2 ());
            typename E::iterator2 it2e (e ().begin2 ());
            typename E::iterator2 it2e_end (e ().end2 ());
            while (it2 != it2_end && it2e != it2e_end) {
                int compare = it2.index () - it2e.index ();
                if (compare == 0) {
                    typename matrix_column<M>::iterator it1 ((*it2).begin ());
                    typename matrix_column<M>::iterator it1_end ((*it2).end ());
                    typename matrix_column<const E>::iterator it1e ((*it2e).begin ());
                    typename matrix_column<const E>::iterator it1e_end ((*it2e).end ());
                    while (it1 != it1_end && it1e != it1e_end) {
                        int compare = it1.index () - it1e.index ();
                        if (compare == 0) {
                            functor_type () (*it1, *it1e);
                            ++ it1, ++ it1e;
                        } else if (compare < 0) {
                            throw external_logic ();
                        } else if (compare > 0) {
#ifdef NUMERICS_BOUNDS_CHECK_EX
                            // Need the const member dispatched.
                            const M &cm = m;
                            // FIXME: we need a better floating point comparison...
                            check (*it1e == cm (it1e.index (), it2e.index ()), bad_index ());
#endif
                            ++ it1e;
                        }
                    }
                    if (it1 != it1_end) {
                        throw external_logic ();
                    }
#ifdef NUMERICS_BOUNDS_CHECK_EX
                    while (it1e != it1e_end) {
                        // Need the const member dispatched.
                        const M &cm = m;
                        // FIXME: we need a better floating point comparison...
                        check (*it1e == cm (it1e.index (), it2e.index ()), bad_index ());
                        ++ it1e;
                    }
#endif
                    ++ it2, ++ it2e;
                } else if (compare < 0) {
                    typename matrix_column<M>::iterator it1 ((*it2).begin ());
                    typename matrix_column<M>::iterator it1_end ((*it2).end ());
                    if (it1 != it1_end) {
                        throw external_logic ();
                    }
                    ++ it2;
                } else if (compare > 0) {
#ifdef NUMERICS_BOUNDS_CHECK_EX
                    typename matrix_column<const E>::const_iterator it1e ((*it2e).begin ());
                    typename matrix_column<const E>::const_iterator it1e_end ((*it2e).end ());
                    while (it1e != it1e_end) {
                        // Need the const member dispatched.
                        const M &cm = m;
                        // FIXME: we need a better floating point comparison...
                        check (*it1e == cm (it1e.index (), it2e.index ()), bad_index ());
                        ++ it1e;
                    }
#endif
                    ++ it2e;
                }
            }
            while (it2 != it2_end) {
                typename matrix_column<M>::iterator it1 ((*it2).begin ());
                typename matrix_column<M>::iterator it1_end ((*it2).end ());
                if (it1 != it1_end) {
                    throw external_logic ();
                }
                ++ it2;
            }
#ifdef NUMERICS_BOUNDS_CHECK_EX
            while (it2e != it2e_end) {
                typename matrix_column<const E>::const_iterator it1e ((*it2e).begin ());
                typename matrix_column<const E>::const_iterator it1e_end ((*it2e).end ());
                while (it1e != it1e_end) {
                    // Need the const member dispatched.
                    const M &cm = m;
                    // FIXME: we need a better floating point comparison...
                    check (*it1e == cm (it1e.index (), it2e.index ()), bad_index ());
                    ++ it1e;
                }
                ++ it2e;
            }
#endif
#else
            check (m.size1 () == e ().size1 (), bad_size ());
            check (m.size2 () == e ().size2 (), bad_size ());
            typedef typename M::value_type value_type;
            typename M::iterator2 it2 (m.begin2 ());
            typename M::iterator2 it2_end (m.end2 ());
            typename E::iterator2 it2e (e ().begin2 ());
            typename E::iterator2 it2e_end (e ().end2 ());
            while (it2 != it2_end && it2e != it2e_end) {
                int compare = it2.index2 () - it2e.index2 ();
                if (compare == 0) {
                    typename M::iterator1 it1 (it2.begin ());
                    typename M::iterator1 it1_end (it2.end ());
                    typename E::iterator1 it1e (it2e.begin ());
                    typename E::iterator1 it1e_end (it2e.end ());
                    while (it1 != it1_end && it1e != it1e_end) {
                        int compare = it1.index1 () - it1e.index1 ();
                        if (compare == 0) {
                            functor_type () (*it1, *it1e);
                            ++ it1, ++ it1e;
                        } else if (compare < 0) {
                            throw external_logic ();
                        } else if (compare > 0) {
#ifdef NUMERICS_BOUNDS_CHECK_EX
                            // Need the const member dispatched.
                            const M &cm = m;
                            // FIXME: we need a better floating point comparison...
                            check (*it1e == cm (it1e.index1 (), it1e.index2 ()), bad_index ());
#endif
                            ++ it1e;
                        }
                    }
                    if (it1 != it1_end) {
                        throw external_logic ();
                    }
#ifdef NUMERICS_BOUNDS_CHECK_EX
                    while (it1e != it1e_end) {
                        // Need the const member dispatched.
                        const M &cm = m;
                        // FIXME: we need a better floating point comparison...
                        check (*it1e == cm (it1e.index1 (), it1e.index2 ()), bad_index ());
                        ++ it1e;
                    }
#endif
                    ++ it2, ++ it2e;
                } else if (compare < 0) {
                    typename M::iterator1 it1 (it2.begin ());
                    typename M::iterator1 it1_end (it2.end ());
                    if (it1 != it1_end) {
                        throw external_logic ();
                    }
                    ++ it2;
                } else if (compare > 0) {
#ifdef NUMERICS_BOUNDS_CHECK_EX
                    typename E::const_iterator1 it1e (it2e.begin ());
                    typename E::const_iterator1 it1e_end (it2e.end ());
                    while (it1e != it1e_end) {
                        // Need the const member dispatched.
                        const M &cm = m;
                        // FIXME: we need a better floating point comparison...
                        check (*it1e == cm (it1e.index1 (), it1e.index2 ()), bad_index ());
                        ++ it1e;
                    }
#endif
                    ++ it2e;
                }
            }
            while (it2 != it2_end) {
                typename M::iterator1 it1 (it2.begin ());
                typename M::iterator1 it1_end (it2.end ());
                if (it1 != it1_end) {
                    throw external_logic ();
                }
                ++ it2;
            }
#ifdef NUMERICS_BOUNDS_CHECK_EX
            while (it2e != it2e_end) {
                typename E::const_iterator1 it1e (it2e.begin ());
                typename E::const_iterator1 it1e_end (it2e.end ());
                while (it1e != it1e_end) {
                    // Need the const member dispatched.
                    const M &cm = m;
                    // FIXME: we need a better floating point comparison...
                    check (*it1e == cm (it1e.index1 (), it1e.index2 ()), bad_index ());
                    ++ it1e;
                }
                ++ it2e;
            }
#endif
#endif
        }
#endif

        // Dispatcher
        template<class M, class E>
        NUMERICS_INLINE
        void operator () (M &m, matrix_expression<E> &e) {
            typedef typename matrix_swap_traits<NUMERICS_TYPENAME M::storage_category,
                                                NUMERICS_TYPENAME E::const_iterator1::iterator_category,
                                                NUMERICS_TYPENAME E::const_iterator2::iterator_category>::storage_category storage_category;
            // FIXME: can we improve the dispatch here?
            // typedef typename E::orientation_category orientation_category;
            typedef typename M::orientation_category orientation_category;
#ifndef NUMERICS_SPECIALIZE_ASSIGN
            operator () (m, e, storage_category (), orientation_category ());
#else
            evaluate_matrix_swap (functor_type (), m, e, storage_category (), orientation_category ());
#endif
        }
    };

}}

#endif




















