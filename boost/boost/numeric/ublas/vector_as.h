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

#ifndef NUMERICS_VECTOR_AS_H
#define NUMERICS_VECTOR_AS_H

#include <boost/numeric/ublas/config.h>
#include <boost/numeric/ublas/vector_et.h>

// Iterators based on ideas of Jeremy Siek

namespace boost { namespace numerics {

#ifdef NUMERICS_SPECIALIZE_ASSIGN
    // Iterating case
    template<class F, class V, class T>
    // This function seems to be big. So we do not let the compiler inline it.
    // NUMERICS_INLINE
    void iterating_vector_assign_scalar (const F &f, V &v, const T &t) {
        typedef F functor_type;
        typedef typename V::difference_type difference_type;
        difference_type size (v.size ());
        typename V::iterator it (v.begin ());
        check (v.end () - it == size, bad_size ());
#ifndef NUMERICS_USE_DUFF_DEVICE
        while (-- size >= 0)
            functor_type () (*it, t), ++ it;
#else
        DD (size, 4, r, (functor_type () (*it, t), ++ it));
#endif
    }
    // Indexing case
    template<class F, class V, class T>
    // This function seems to be big. So we do not let the compiler inline it.
    // NUMERICS_INLINE
    void indexing_vector_assign_scalar (const F &f, V &v, const T &t) {
        typedef F functor_type;
        typedef typename V::difference_type difference_type;
        difference_type size (v.size ());
#ifndef NUMERICS_USE_DUFF_DEVICE
        for (difference_type i = 0; i < size; ++ i)
            functor_type () (v (i), t);
#else
        difference_type i (0);
        DD (size, 4, r, (functor_type () (v (i), t), ++ i));
#endif
    }

    // Dense (proxy) case
    template<class F, class V, class T>
    // This function seems to be big. So we do not let the compiler inline it.
    // NUMERICS_INLINE
    void evaluate_vector_assign_scalar (const F &f, V &v, const T &t, dense_proxy_tag) {
        typedef F functor_type;
        typedef typename V::difference_type difference_type;
        difference_type size (v.size ());
        if (size >= NUMERICS_ITERATOR_THRESHOLD)
            iterating_vector_assign_scalar (functor_type (), v, t);
        else
            indexing_vector_assign_scalar (functor_type (), v, t);
    }
    // Packed (proxy) case
    template<class F, class V, class T>
    // This function seems to be big. So we do not let the compiler inline it.
    // NUMERICS_INLINE
    void evaluate_vector_assign_scalar (const F &f, V &v, const T &t, packed_proxy_tag) {
        typedef F functor_type;
        typedef typename V::difference_type difference_type;
        typename V::iterator it (v.begin ());
        difference_type size (v.end () - it);
        while (-- size >= 0)
            functor_type () (*it, t), ++ it;
    }
    // Sparse (proxy) case
    template<class F, class V, class T>
    // This function seems to be big. So we do not let the compiler inline it.
    // NUMERICS_INLINE
    void evaluate_vector_assign_scalar (const F &f, V &v, const T &t, sparse_proxy_tag) {
        typedef F functor_type;
        typename V::iterator it (v.begin ());
        typename V::iterator it_end (v.end ());
        while (it != it_end)
            functor_type () (*it, t), ++ it;
    }
#endif

    // vector assignment_operation scalar
    template<class F>
    struct vector_assign_scalar {
        typedef F functor_type;
        typedef typename F::assign_category assign_category;

#ifndef NUMERICS_SPECIALIZE_ASSIGN
        // Iterating case
        template<class V, class T>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void iterating_assign (V &v, const T &t) {
            typedef typename V::difference_type difference_type;
            difference_type size (v.size ());
            typename V::iterator it (v.begin ());
            check (v.end () - it == size, bad_size ());
#ifndef NUMERICS_USE_DUFF_DEVICE
            while (-- size >= 0)
                functor_type () (*it, t), ++ it;
#else
            DD (size, 4, r, (functor_type () (*it, t), ++ it));
#endif
        }
        // Indexing case
        template<class V, class T>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void indexing_assign (V &v, const T &t) {
            typedef typename V::difference_type difference_type;
            difference_type size (v.size ());
#ifndef NUMERICS_USE_DUFF_DEVICE
            for (difference_type i = 0; i < size; ++ i)
                functor_type () (v (i), t);
#else
            difference_type i (0);
            DD (size, 4, r, (functor_type () (v (i), t), ++ i));
#endif
        }

        // Dense (proxy) case
        template<class V, class T>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (V &v, const T &t, dense_proxy_tag) {
            typedef typename V::difference_type difference_type;
            difference_type size (v.size ());
            if (size >= NUMERICS_ITERATOR_THRESHOLD)
                iterating_assign (v, t);
            else
                indexing_assign (v, t);
        }
        // Packed (proxy) case
        template<class V, class T>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (V &v, const T &t, packed_proxy_tag) {
            typedef typename V::difference_type difference_type;
            typename V::iterator it (v.begin ());
            difference_type size (v.end () - it);
            while (-- size >= 0)
                functor_type () (*it, t), ++ it;
        }
        // Sparse (proxy) case
        template<class V, class T>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (V &v, const T &t, sparse_proxy_tag) {
            typename V::iterator it (v.begin ());
            typename V::iterator it_end (v.end ());
            while (it != it_end)
                functor_type () (*it, t), ++ it;
        }
#endif

        // Dispatcher
        template<class V, class T>
        NUMERICS_INLINE
        void operator () (V &v, const T &t) {
            typedef typename V::storage_category storage_category;
#ifndef NUMERICS_SPECIALIZE_ASSIGN
            operator () (v, t, storage_category ());
#else
            evaluate_vector_assign_scalar (functor_type (), v, t, storage_category ());
#endif
        }
    };

    template<class LS, class A, class RI>
    struct vector_assign_traits {
        typedef LS storage_category;
    };

    template<>
    struct vector_assign_traits<dense_tag, assign_tag, packed_random_access_iterator_tag> {
        typedef packed_tag storage_category;
    };
    template<>
    struct vector_assign_traits<dense_tag, computed_assign_tag, packed_random_access_iterator_tag> {
        typedef packed_tag storage_category;
    };
    template<>
    struct vector_assign_traits<dense_tag, assign_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_tag storage_category;
    };
    template<>
    struct vector_assign_traits<dense_tag, computed_assign_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_tag storage_category;
    };

    template<>
    struct vector_assign_traits<dense_proxy_tag, assign_tag, packed_random_access_iterator_tag> {
        typedef packed_proxy_tag storage_category;
    };
    template<>
    struct vector_assign_traits<dense_proxy_tag, computed_assign_tag, packed_random_access_iterator_tag> {
        typedef packed_proxy_tag storage_category;
    };
    template<>
    struct vector_assign_traits<dense_proxy_tag, assign_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_tag storage_category;
    };
    template<>
    struct vector_assign_traits<dense_proxy_tag, computed_assign_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_tag storage_category;
    };

    template<>
    struct vector_assign_traits<packed_tag, assign_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_tag storage_category;
    };
    template<>
    struct vector_assign_traits<packed_tag, computed_assign_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_tag storage_category;
    };

    template<>
    struct vector_assign_traits<packed_proxy_tag, assign_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_tag storage_category;
    };
    template<>
    struct vector_assign_traits<packed_proxy_tag, computed_assign_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_tag storage_category;
    };

    template<>
    struct vector_assign_traits<sparse_tag, computed_assign_tag, dense_random_access_iterator_tag> {
        typedef sparse_proxy_tag storage_category;
    };
    template<>
    struct vector_assign_traits<sparse_tag, computed_assign_tag, packed_random_access_iterator_tag> {
        typedef sparse_proxy_tag storage_category;
    };
    template<>
    struct vector_assign_traits<sparse_tag, computed_assign_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_tag storage_category;
    };

#ifdef NUMERICS_SPECIALIZE_ASSIGN
    // Iterating case
    template<class F, class V, class E>
    // This function seems to be big. So we do not let the compiler inline it.
    // NUMERICS_INLINE
    void iterating_vector_assign (const F &f, V &v, const vector_expression<E> &e) {
        typedef F functor_type;
        typedef typename V::difference_type difference_type;
        difference_type size (common (v.size (), e ().size ()));
        typename V::iterator it (v.begin ());
        check (v.end () - it == size, bad_size ());
        typename E::const_iterator ite (e ().begin ());
        check (e ().end () - ite == size, bad_size ());
#ifndef NUMERICS_USE_DUFF_DEVICE
        while (-- size >= 0)
            functor_type () (*it, *ite), ++ it, ++ ite;
#else
        DD (size, 2, r, (functor_type () (*it, *ite), ++ it, ++ ite));
#endif
    }
    // Indexing scase
    template<class F, class V, class E>
    // This function seems to be big. So we do not let the compiler inline it.
    // NUMERICS_INLINE
    void indexing_vector_assign (const F &f, V &v, const vector_expression<E> &e) {
        typedef F functor_type;
        typedef typename V::difference_type difference_type;
        difference_type size (common (v.size (), e ().size ()));
#ifndef NUMERICS_USE_DUFF_DEVICE
        for (difference_type i = 0; i < size; ++ i)
            functor_type () (v (i), e () (i));
#else
        difference_type i (0);
        DD (size, 2, r, (functor_type () (v (i), e () (i)), ++ i));
#endif
    }

    // Dense (proxy) case
    template<class F, class V, class E>
    // This function seems to be big. So we do not let the compiler inline it.
    // NUMERICS_INLINE
    void evaluate_vector_assign (const F &f, V &v, const vector_expression<E> &e, dense_proxy_tag) {
        typedef F functor_type;
        typedef typename V::size_type size_type;
        typedef typename V::difference_type difference_type;
        difference_type size (common (v.size (), e ().size ()));
        if (size >= NUMERICS_ITERATOR_THRESHOLD)
            iterating_vector_assign (functor_type (), v, e);
        else
            indexing_vector_assign (functor_type (), v, e);
    }
    // Packed (proxy) case
    template<class F, class V, class E>
    // This function seems to be big. So we do not let the compiler inline it.
    // NUMERICS_INLINE
    void evaluate_vector_assign (const F &f, V &v, const vector_expression<E> &e, packed_proxy_tag) {
        check (v.size () == e ().size (), bad_size ());
        typedef F functor_type;
        typedef typename V::size_type size_type;
        typedef typename V::value_type value_type;
        typename V::iterator it (v.begin ());
        typename V::iterator it_end (v.end ());
        typename E::const_iterator ite (e ().begin ());
        typename E::const_iterator ite_end (e ().end ());
        if (ite != ite_end && ite.index () < it.index ())
            ite += std::min (it.index () - ite.index (), size_type (ite_end - ite));
        while (it != it_end && ite != ite_end && it.index () < ite.index ()) {
            functor_type () (*it, value_type ());
            ++ it;
        }
        while (it != it_end && ite != ite_end) {
            functor_type () (*it, *ite);
            ++ it, ++ ite;
        }
        while (it != it_end) {
            functor_type () (*it, value_type ());
            ++ it;
        }
#ifdef NUMERICS_BOUNDS_CHECK_EX
        {
            // Need the const member dispatched.
            const V &cv = v;
            typename E::const_iterator ite (e ().begin ());
            typename E::const_iterator ite_end (e ().end ());
            while (ite != ite_end) {
                // FIXME: we need a better floating point comparison...
                check (*ite == cv (ite.index ()), bad_index ());
                ++ ite;
            }
        }
#endif
    }
    // Sparse case
    template<class F, class V, class E>
    // This function seems to be big. So we do not let the compiler inline it.
    // NUMERICS_INLINE
    void evaluate_vector_assign (const F &f, V &v, const vector_expression<E> &e, sparse_tag) {
        check (v.size () == e ().size (), bad_size ());
        v.clear ();
        typename E::const_iterator ite (e ().begin ());
        typename E::const_iterator ite_end (e ().end ());
        while (ite != ite_end)
            v.insert (ite.index (), *ite), ++ ite;
    }
    // Sparse proxy case
    template<class F, class V, class E>
    // This function seems to be big. So we do not let the compiler inline it.
    // NUMERICS_INLINE
    void evaluate_vector_assign (const F &f, V &v, const vector_expression<E> &e, sparse_proxy_tag) {
        check (v.size () == e ().size (), bad_size ());
        typedef F functor_type;
        typedef typename V::value_type value_type;
        typename V::iterator it (v.begin ());
        typename V::iterator it_end (v.end ());
        typename E::const_iterator ite (e ().begin ());
        typename E::const_iterator ite_end (e ().end ());
        while (it != it_end && ite != ite_end) {
            int compare = it.index () - ite.index ();
            if (compare == 0) {
                functor_type () (*it, *ite);
                ++ it, ++ ite;
            } else if (compare < 0) {
                functor_type () (*it, value_type ());
                ++ it;
            } else if (compare > 0) {
#ifdef NUMERICS_BOUNDS_CHECK_EX
                // Need the const member dispatched.
                const V &cv = v;
                // FIXME: we need a better floating point comparison...
                check (*ite == cv (ite.index ()), bad_index ());
#endif
                ++ ite;
            }
        }
        while (it != it_end) {
            functor_type () (*it, value_type ());
            ++ it;
        }
#ifdef NUMERICS_BOUNDS_CHECK_EX
        while (ite != ite_end) {
            // Need the const member dispatched.
            const V &cv = v;
            // FIXME: we need a better floating point comparison...
            check (*ite == cv (ite.index ()), bad_index ());
            ++ ite;
        }
#endif
    }
#endif

    // vector assignment_operation vector_expression
    template<class F>
    struct vector_assign {
        typedef F functor_type;
        typedef typename F::assign_category assign_category;

#ifndef NUMERICS_SPECIALIZE_ASSIGN
        // Iterating case
        template<class V, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void iterating_assign (V &v, const vector_expression<E> &e) {
            typedef typename V::difference_type difference_type;
            difference_type size (common (v.size (), e ().size ()));
            typename V::iterator it (v.begin ());
            check (v.end () - it == size, bad_size ());
            typename E::const_iterator ite (e ().begin ());
            check (e ().end () - ite == size, bad_size ());
#ifndef NUMERICS_USE_DUFF_DEVICE
            while (-- size >= 0)
                functor_type () (*it, *ite), ++ it, ++ ite;
#else
            DD (size, 2, r, (functor_type () (*it, *ite), ++ it, ++ ite));
#endif
        }
        // Indexing scase
        template<class V, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void indexing_assign (V &v, const vector_expression<E> &e) {
            typedef typename V::difference_type difference_type;
            difference_type size (common (v.size (), e ().size ()));
#ifndef NUMERICS_USE_DUFF_DEVICE
            for (difference_type i = 0; i < size; ++ i)
                functor_type () (v (i), e () (i));
#else
            difference_type i (0);
            DD (size, 2, r, (functor_type () (v (i), e () (i)), ++ i));
#endif
        }

        // Dense (proxy) case
        template<class V, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (V &v, const vector_expression<E> &e, dense_proxy_tag) {
            typedef typename V::size_type size_type;
            typedef typename V::difference_type difference_type;
            difference_type size (common (v.size (), e ().size ()));
            if (size >= NUMERICS_ITERATOR_THRESHOLD)
                iterating_assign (v, e);
            else
                indexing_assign (v, e);
        }
        // Packed (proxy) case
        template<class V, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (V &v, const vector_expression<E> &e, packed_proxy_tag) {
            check (v.size () == e ().size (), bad_size ());
            typedef typename V::size_type size_type;
            typedef typename V::value_type value_type;
            typename V::iterator it (v.begin ());
            typename V::iterator it_end (v.end ());
            typename E::const_iterator ite (e ().begin ());
            typename E::const_iterator ite_end (e ().end ());
            if (ite != ite_end && ite.index () < it.index ())
                ite += std::min (it.index () - ite.index (), size_type (ite_end - ite));
            while (it != it_end && ite != ite_end && it.index () < ite.index ()) {
                functor_type () (*it, value_type ());
                ++ it;
            }
            while (it != it_end && ite != ite_end) {
                functor_type () (*it, *ite);
                ++ it, ++ ite;
            }
            while (it != it_end) {
                functor_type () (*it, value_type ());
                ++ it;
            }
#ifdef NUMERICS_BOUNDS_CHECK_EX
            {
                // Need the const member dispatched.
                const V &cv = v;
                typename E::const_iterator ite (e ().begin ());
                typename E::const_iterator ite_end (e ().end ());
                while (ite != ite_end) {
                    // FIXME: we need a better floating point comparison...
                    check (*ite == cv (ite.index ()), bad_index ());
                    ++ ite;
                }
            }
#endif
        }
        // Sparse case
        template<class V, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (V &v, const vector_expression<E> &e, sparse_tag) {
            check (v.size () == e ().size (), bad_size ());
            v.clear ();
            typename E::const_iterator ite (e ().begin ());
            typename E::const_iterator ite_end (e ().end ());
            while (ite != ite_end)
                v.insert (ite.index (), *ite), ++ ite;
        }
        // Sparse proxy case
        template<class V, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (V &v, const vector_expression<E> &e, sparse_proxy_tag) {
            check (v.size () == e ().size (), bad_size ());
            typedef typename V::value_type value_type;
            typename V::iterator it (v.begin ());
            typename V::iterator it_end (v.end ());
            typename E::const_iterator ite (e ().begin ());
            typename E::const_iterator ite_end (e ().end ());
            while (it != it_end && ite != ite_end) {
                int compare = it.index () - ite.index ();
                if (compare == 0) {
                    functor_type () (*it, *ite);
                    ++ it, ++ ite;
                } else if (compare < 0) {
                    functor_type () (*it, value_type ());
                    ++ it;
                } else if (compare > 0) {
#ifdef NUMERICS_BOUNDS_CHECK_EX
                    // Need the const member dispatched.
                    const V &cv = v;
                    // FIXME: we need a better floating point comparison...
                    check (*ite == cv (ite.index ()), bad_index ());
#endif
                    ++ ite;
                }
            }
            while (it != it_end) {
                functor_type () (*it, value_type ());
                ++ it;
            }
#ifdef NUMERICS_BOUNDS_CHECK_EX
            while (ite != ite_end) {
                // Need the const member dispatched.
                const V &cv = v;
                // FIXME: we need a better floating point comparison...
                check (*ite == cv (ite.index ()), bad_index ());
                ++ ite;
            }
#endif
        }
#endif

        // Dispatcher
        template<class V, class E>
        NUMERICS_INLINE
        void operator () (V &v, const vector_expression<E> &e) {
            typedef typename vector_assign_traits<NUMERICS_TYPENAME V::storage_category,
                                                  assign_category,
                                                  NUMERICS_TYPENAME E::const_iterator::iterator_category>::storage_category storage_category;
#ifndef NUMERICS_SPECIALIZE_ASSIGN
            operator () (v, e, storage_category ());
#else
            evaluate_vector_assign (functor_type (), v, e, storage_category ());
#endif
        }
    };

}}

#endif



















