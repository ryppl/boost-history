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

#ifndef NUMERICS_VECTOR_H
#define NUMERICS_VECTOR_H

#include "config.h"
#include "storage.h"
#include "vector_et.h"
#include "vector_pr.h"

// Iterators based on ideas of Jeremy Siek

namespace boost { namespace numerics {

    template<class S, class A> 
    struct vector_assign_scalar_traits {
        typedef S dispatch_category;
    };

    // vector assignment_operation scalar
    template<class F>
    struct vector_assign_scalar {
        typedef F functor_type;
        typedef typename F::assign_category assign_category;

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

        // Dispatcher
        template<class V, class T>
        NUMERICS_INLINE
        void operator () (V &v, const T &t) {
            typedef typename vector_assign_scalar_traits<NUMERICS_TYPENAME V::storage_category,
                                                         assign_category>::dispatch_category dispatch_category ;
            operator () (v, t, dispatch_category ());
        }
    };

    template<class LS, class A, class RI>
    struct vector_assign_traits {
        typedef LS dispatch_category;
    };

    template<>
    struct vector_assign_traits<dense_tag, assign_tag, packed_random_access_iterator_tag> {
        typedef packed_tag dispatch_category;
    };
    template<>
    struct vector_assign_traits<dense_tag, computed_assign_tag, packed_random_access_iterator_tag> {
        typedef packed_tag dispatch_category;
    };
    template<>
    struct vector_assign_traits<dense_tag, assign_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_tag dispatch_category;
    };
    template<>
    struct vector_assign_traits<dense_tag, computed_assign_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_tag dispatch_category;
    };

    template<>
    struct vector_assign_traits<dense_proxy_tag, assign_tag, packed_random_access_iterator_tag> {
        typedef packed_proxy_tag dispatch_category;
    };
    template<>
    struct vector_assign_traits<dense_proxy_tag, computed_assign_tag, packed_random_access_iterator_tag> {
        typedef packed_proxy_tag dispatch_category;
    };
    template<>
    struct vector_assign_traits<dense_proxy_tag, assign_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_tag dispatch_category;
    };
    template<>
    struct vector_assign_traits<dense_proxy_tag, computed_assign_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_tag dispatch_category;
    };

    template<>
    struct vector_assign_traits<packed_tag, assign_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_tag dispatch_category;
    };
    template<>
    struct vector_assign_traits<packed_tag, computed_assign_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_tag dispatch_category;
    };

    template<>
    struct vector_assign_traits<packed_proxy_tag, assign_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_tag dispatch_category;
    };
    template<>
    struct vector_assign_traits<packed_proxy_tag, computed_assign_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_tag dispatch_category;
    };

    template<>
    struct vector_assign_traits<sparse_tag, computed_assign_tag, dense_random_access_iterator_tag> {
        typedef sparse_proxy_tag dispatch_category;
    };
    template<>
    struct vector_assign_traits<sparse_tag, computed_assign_tag, packed_random_access_iterator_tag> {
        typedef sparse_proxy_tag dispatch_category;
    };
    template<>
    struct vector_assign_traits<sparse_tag, computed_assign_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_tag dispatch_category;
    };

    // vector assignment_operation vector_expression
    template<class F>
    struct vector_assign {
        typedef F functor_type;
        typedef typename F::assign_category assign_category;

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
        // Unknown case
        template<class V, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (V &v, const vector_expression<E> &e, unknown_storage_tag) {
            throw internal_logic ();
        }

        // Dispatcher
        template<class V, class E>
        NUMERICS_INLINE
        void operator () (V &v, const vector_expression<E> &e) {
            typedef typename vector_assign_traits<NUMERICS_TYPENAME V::storage_category, 
                                                  assign_category, 
                                                  NUMERICS_TYPENAME E::const_iterator::iterator_category>::dispatch_category dispatch_category;
            operator () (v, e, dispatch_category ());
        }
    };

    template<class E> 
    class vector_const_reference;
    template<class E> 
    class vector_reference;

    // Array based vector class 
    template<class T, class A>
    class vector: 
        public vector_expression<vector<T, A> > {
    public:      
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef const T &const_reference;
        typedef T &reference;
        typedef const T *const_pointer;
        typedef T *pointer;
        typedef A array_type;
        typedef const A const_array_type;
        typedef const vector<T, A> const_self_type;
        typedef vector<T, A> self_type;
        typedef const vector_const_reference<const_self_type> const_closure_type;
        typedef vector_reference<self_type> closure_type;
#ifdef NUMERICS_DEPRECATED
        typedef const vector_range<const_self_type> const_vector_range_type;
        typedef vector_range<self_type> vector_range_type;
#endif
        typedef typename A::const_iterator const_iterator_type;
        typedef typename A::iterator iterator_type;
        typedef dense_tag storage_category;

        // Construction and destruction
        NUMERICS_INLINE
        vector (): 
            size_ (0), data_ (0) {}
        NUMERICS_EXPLICIT NUMERICS_INLINE
        vector (size_type size): 
            size_ (size), data_ (size) {}
        NUMERICS_INLINE
        vector (size_type size, const array_type &data): 
            size_ (size), data_ (data) {}
        NUMERICS_INLINE
        vector (const vector &v): 
            size_ (v.size_), data_ (v.data_) {}
        template<class AE>
        NUMERICS_INLINE
        vector (const vector_expression<AE> &ae): 
            size_ (ae ().size ()), data_ (ae ().size ()) { 
            vector_assign<scalar_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
        }

        // Accessors
        NUMERICS_INLINE
        size_type size () const { 
            return size_;
        }
        NUMERICS_INLINE
        const_array_type &data () const {
            return data_;
        }
        NUMERICS_INLINE
        array_type &data () {
            return data_;
        }

        // Resizing
        NUMERICS_INLINE
        void resize (size_type size) {
            size_ = size;
            data ().resize (size);
        }

        // Element access
        NUMERICS_INLINE
        const_reference operator () (size_type i) const {
            return data () [i]; 
        }
        NUMERICS_INLINE
        reference operator () (size_type i) {
            return data () [i]; 
        }

        NUMERICS_INLINE
        const_reference operator [] (size_type i) const { 
            return (*this) (i); 
        }
        NUMERICS_INLINE
        reference operator [] (size_type i) { 
            return (*this) (i); 
        }

        // Assignment
        NUMERICS_INLINE
        vector &operator = (const vector &v) { 
            check (size_ == v.size_, bad_size ());
            size_ = v.size_;
            data () = v.data ();
            return *this;
        }
        NUMERICS_INLINE
        vector &assign_temporary (vector &v) {
            swap (v);
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        vector &operator = (const vector_expression<AE> &ae) {
#ifdef NUMERICS_MUTABLE_TEMPORARY
            return assign_temporary (self_type (ae));
#else
            // return assign (self_type (ae));
            self_type temporary (ae);
            return assign_temporary (temporary);
#endif
        }
        template<class AE>
        NUMERICS_INLINE
        vector &reset (const vector_expression<AE> &ae) {
            self_type temporary (ae);
            resize (temporary.size ());
            return assign_temporary (temporary);
        }
        template<class AE>
        NUMERICS_INLINE
        vector &assign (const vector_expression<AE> &ae) {
            vector_assign<scalar_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        vector &operator += (const vector_expression<AE> &ae) {
#ifdef NUMERICS_MUTABLE_TEMPORARY
            return assign_temporary (self_type (*this + ae));
#else
            // return assign (self_type (*this + ae));
            self_type temporary (*this + ae);
            return assign_temporary (temporary);
#endif
        }
        template<class AE>
        NUMERICS_INLINE
        vector &plus_assign (const vector_expression<AE> &ae) {
            vector_assign<scalar_plus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        vector &operator -= (const vector_expression<AE> &ae) {
#ifdef NUMERICS_MUTABLE_TEMPORARY
            return assign_temporary (self_type (*this - ae));
#else
            // return assign (self_type (*this - ae));
            self_type temporary (*this - ae);
            return assign_temporary (temporary);
#endif
        }
        template<class AE>
        NUMERICS_INLINE
        vector &minus_assign (const vector_expression<AE> &ae) {
            vector_assign<scalar_minus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        vector &operator *= (const AT &at) {
            vector_assign_scalar<scalar_multiplies_assign<value_type, AT> > () (*this, at);
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        vector &operator /= (const AT &at) {
            vector_assign_scalar<scalar_divides_assign<value_type, AT> > () (*this, at);
            return *this;
        }

        // Swapping
        NUMERICS_INLINE
        void swap (vector &v) {
            // Too unusual semantic.
            // check (this != &v, external_logic ());
            if (this != &v) {
                check (size_ == v.size_, bad_size ());
                std::swap (size_, v.size_);
                data ().swap (v.data ());
            }
        }
#ifdef NUMERICS_FRIEND_FUNCTION
        NUMERICS_INLINE
        friend void swap (vector &v1, vector &v2) {
            v1.swap (v2);
        }
#endif

        // Element insertion and erasure
        // These functions should work with std::vector.
        // Thanks to Kresimir Fresl for spotting this.
        NUMERICS_INLINE
        void insert (size_type i, const_reference t) {
            check (data () [i] == value_type (), bad_index ());
            // data ().insert (data ().begin () + i, t);
            data () [i] = t;
        }
        NUMERICS_INLINE
        void erase (size_type i) {
            // data ().erase (data ().begin () + i);
            data () [i] = value_type ();
        }
        NUMERICS_INLINE
        void clear () {
            // data ().clear ();
            std::fill (data ().begin (), data ().end (), value_type ());
        }

#ifdef NUMERICS_USE_INDEXED_ITERATOR
        typedef indexed_iterator<self_type, dense_random_access_iterator_tag> iterator;
        typedef indexed_const_iterator<self_type, dense_random_access_iterator_tag> const_iterator;
#else
        class const_iterator;
        class iterator;
#endif

        // Element lookup
        NUMERICS_INLINE
        const_iterator find (size_type i) const {
#ifndef NUMERICS_USE_INDEXED_ITERATOR
            return const_iterator (*this, data ().begin () + i);
#else
            return const_iterator (*this, i);
#endif
        }
        NUMERICS_INLINE
        iterator find (size_type i) {
#ifndef NUMERICS_USE_INDEXED_ITERATOR
            return iterator (*this, data ().begin () + i);
#else
            return iterator (*this, i);
#endif
        }
        NUMERICS_INLINE
        const_iterator find_first (size_type i) const {
            return find (i);
        }
        NUMERICS_INLINE
        iterator find_first (size_type i) {
            return find (i);
        }
        NUMERICS_INLINE
        const_iterator find_last (size_type i) const {
            return find (i);
        }
        NUMERICS_INLINE
        iterator find_last (size_type i) {
            return find (i);
        }

        // Iterators simply are pointers.

#ifndef NUMERICS_USE_INDEXED_ITERATOR
        class const_iterator:
            public container_const_reference<vector>,
            public random_access_iterator_base<dense_random_access_iterator_tag,
                                               const_iterator, value_type> {
        public:
            typedef dense_random_access_iterator_tag iterator_category;
#ifndef BOOST_MSVC_STD_ITERATOR
            typedef typename vector::difference_type difference_type;
            typedef typename vector::value_type value_type;
            typedef typename vector::const_reference reference;
            typedef typename vector::const_pointer pointer;
#endif

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator ():
                container_const_reference<vector> (), it_ () {}
            NUMERICS_INLINE
            const_iterator (const vector &v, const const_iterator_type &it):
                container_const_reference<vector> (v), it_ (it) {}
            NUMERICS_INLINE
#ifndef NUMERICS_QUALIFIED_TYPENAME
            const_iterator (const iterator &it):
#else
            const_iterator (const typename vector::iterator &it):
#endif
                container_const_reference<vector> (it ()), it_ (it.it_) {}

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
            const_reference operator * () const {
                check (index () < (*this) ().size (), bad_index ());
                return *it_;
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                return it_ - (*this) ().begin ().it_;
            }

            // Assignment 
            NUMERICS_INLINE
            const_iterator &operator = (const const_iterator &it) {
                container_const_reference<vector>::assign (&it ());
                it_ = it.it_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it_ == it.it_;
            }
            NUMERICS_INLINE
            bool operator < (const const_iterator &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it_ < it.it_;
            }

        private:
            const_iterator_type it_;

            friend class iterator;
        };
#endif

        NUMERICS_INLINE
        const_iterator begin () const {
            return find_first (0);
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return find_last (size_);
        }

#ifndef NUMERICS_USE_INDEXED_ITERATOR
        class iterator:
            public container_reference<vector>,
            public random_access_iterator_base<dense_random_access_iterator_tag,
                                               iterator, value_type> {
        public:
            typedef dense_random_access_iterator_tag iterator_category;
#ifndef BOOST_MSVC_STD_ITERATOR
            typedef typename vector::difference_type difference_type;
            typedef typename vector::value_type value_type;
            typedef typename vector::reference reference;
            typedef typename vector::pointer pointer;
#endif

            // Construction and destruction
            NUMERICS_INLINE
            iterator ():
                container_reference<vector> (), it_ () {}
            NUMERICS_INLINE
            iterator (vector &v, const iterator_type &it):
                container_reference<vector> (v), it_ (it) {}

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
            reference operator * () const {
                check (index () < (*this) ().size (), bad_index ());
                return *it_;
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                return it_ - (*this) ().begin ().it_;
            }

            // Assignment
            NUMERICS_INLINE
            iterator &operator = (const iterator &it) {
                container_reference<vector>::assign (&it ());
                it_ = it.it_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const iterator &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it_ == it.it_;
            }
            NUMERICS_INLINE
            bool operator < (const iterator &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it_ < it.it_;
            }

        private:
            iterator_type it_;

            friend class const_iterator;
        };
#endif

        NUMERICS_INLINE
        iterator begin () {
            return find_first (0);
        }
        NUMERICS_INLINE
        iterator end () {
            return find_last (size_);
        }

        // Reverse iterator

#ifdef BOOST_MSVC_STD_ITERATOR
        typedef reverse_iterator<const_iterator, value_type, value_type> const_reverse_iterator;
#else
        typedef reverse_iterator<const_iterator> const_reverse_iterator;
#endif

        NUMERICS_INLINE
        const_reverse_iterator rbegin () const {
            return const_reverse_iterator (end ());
        }
        NUMERICS_INLINE
        const_reverse_iterator rend () const {
            return const_reverse_iterator (begin ());
        }

#ifdef BOOST_MSVC_STD_ITERATOR
        typedef reverse_iterator<iterator, value_type, reference> reverse_iterator;
#else
        typedef reverse_iterator<iterator> reverse_iterator;
#endif

        NUMERICS_INLINE
        reverse_iterator rbegin () {
            return reverse_iterator (end ());
        }
        NUMERICS_INLINE
        reverse_iterator rend () {
            return reverse_iterator (begin ());
        }

    private:
        size_type size_;
        array_type data_;
    };

    // Unit vector class
    template<class T>
    class unit_vector:
        public vector_expression<unit_vector<T> > {
    public:      
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef const T &const_reference;
        typedef T &reference;
        typedef const T *const_pointer;
        typedef T *pointer;
        typedef const unit_vector<T> const_self_type;
        typedef unit_vector<T> self_type;
        typedef const vector_const_reference<const_self_type> const_closure_type;
#ifdef NUMERICS_DEPRECATED
        typedef const vector_range<const_self_type> const_vector_range_type;
#endif
        typedef size_type const_iterator_type;
        typedef packed_tag storage_category;

        // Construction and destruction
        NUMERICS_INLINE
        unit_vector (): 
            size_ (0), index_ (0) {}
        NUMERICS_INLINE
        unit_vector (size_type size, size_type index): 
            size_ (size), index_ (index) {}
        NUMERICS_INLINE
        unit_vector (const unit_vector &v): 
            size_ (v.size_), index_ (v.index_) {}

        // Accessors
        NUMERICS_INLINE
        size_type size () const { 
            return size_; 
        }
        NUMERICS_INLINE
        size_type index () const { 
            return index_; 
        }

        // Resizing
        NUMERICS_INLINE
        void resize (size_type size) {
            size_ = size;
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i) const {
            return i == index_ ? 1 : 0; 
        }

        NUMERICS_INLINE
        value_type operator [] (size_type i) const {
            return (*this) (i); 
        }

        // Assignment
        NUMERICS_INLINE
        unit_vector &operator = (const unit_vector &v) { 
            check (size_ == v.size_, bad_size ());
            size_ = v.size_;
            index_ = v.index_;
            return *this;
        }
        NUMERICS_INLINE
        unit_vector &assign_temporary (unit_vector &v) { 
            swap (v);
            return *this;
        }

        // Swapping
        NUMERICS_INLINE
        void swap (unit_vector &v) {
            // Too unusual semantic.
            // check (this != &v, external_logic ());
            if (this != &v) {
                check (size_ == v.size_, bad_size ());
                std::swap (size_, v.size_);
                std::swap (index_, v.index_);
            }
        }
#ifdef NUMERICS_FRIEND_FUNCTION
        NUMERICS_INLINE
        friend void swap (unit_vector &v1, unit_vector &v2) {
            v1.swap (v2);
        }
#endif

#ifdef NUMERICS_USE_INDEXED_ITERATOR
        typedef indexed_const_iterator<self_type, packed_random_access_iterator_tag> iterator;
        typedef indexed_const_iterator<self_type, packed_random_access_iterator_tag> const_iterator;
#else
        class const_iterator;
#endif

        // Element lookup
        NUMERICS_INLINE
        const_iterator find_first (size_type i) const {
            return const_iterator (*this, std::max (i, index_));
        }
        NUMERICS_INLINE
        const_iterator find_last (size_type i) const {
            return const_iterator (*this, std::min (i, index_ + 1));
        }

        // Iterators simply are pointers.

#ifndef NUMERICS_USE_INDEXED_ITERATOR
        class const_iterator:
            public container_const_reference<unit_vector>,
            public random_access_iterator_base<packed_random_access_iterator_tag,
                                               const_iterator, value_type> {
        public:
            typedef packed_random_access_iterator_tag iterator_category;
#ifndef BOOST_MSVC_STD_ITERATOR
            typedef typename unit_vector::difference_type difference_type;
            typedef typename unit_vector::value_type value_type;
            typedef typename unit_vector::value_type reference;
            typedef typename unit_vector::const_pointer pointer;
#endif

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator ():
                container_const_reference<unit_vector> (), it_ () {}
            NUMERICS_INLINE
            const_iterator (const unit_vector &v, const const_iterator_type &it):
                container_const_reference<unit_vector> (v), it_ (it) {}

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
                check (index () < (*this) ().size (), bad_index ());
                return (*this) () (index ());
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                return it_;
            }

            // Assignment 
            NUMERICS_INLINE
            const_iterator &operator = (const const_iterator &it) {
                container_const_reference<unit_vector>::assign (&it ());
                it_ = it.it_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it_ == it.it_;
            }
            NUMERICS_INLINE
            bool operator < (const const_iterator &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it_ < it.it_;
            }

        private:
            const_iterator_type it_;
        };

        typedef const_iterator iterator;
#endif

        NUMERICS_INLINE
        const_iterator begin () const {
            return find_first (0);
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return find_last (size_);
        }

        // Reverse iterator

#ifdef BOOST_MSVC_STD_ITERATOR
        typedef reverse_iterator<const_iterator, value_type, value_type> const_reverse_iterator;
#else
        typedef reverse_iterator<const_iterator> const_reverse_iterator;
#endif

        NUMERICS_INLINE
        const_reverse_iterator rbegin () const {
            return const_reverse_iterator (end ());
        }
        NUMERICS_INLINE
        const_reverse_iterator rend () const {
            return const_reverse_iterator (begin ());
        }

    private:
        size_type size_;
        size_type index_;
    };

    // Zero vector class
    template<class T>
    class zero_vector:
        public vector_expression<zero_vector<T> > {
    public:
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef const T &const_reference;
        typedef T &reference;
        typedef const T *const_pointer;
        typedef T *pointer;
        typedef const zero_vector<T> const_self_type;
        typedef zero_vector<T> self_type;
        typedef const vector_const_reference<const_self_type> const_closure_type;
#ifdef NUMERICS_DEPRECATED
        typedef const vector_range<const_self_type> const_vector_range_type;
#endif
        typedef size_type const_iterator_type;
        typedef sparse_tag storage_category;

        // Construction and destruction
        NUMERICS_INLINE
        zero_vector ():
            size_ (0) {}
        NUMERICS_INLINE
        zero_vector (size_type size, size_type index):
            size_ (size) {}
        NUMERICS_INLINE
        zero_vector (const zero_vector &v):
            size_ (v.size_) {}

        // Accessors
        NUMERICS_INLINE
        size_type size () const {
            return size_;
        }

        // Resizing
        NUMERICS_INLINE
        void resize (size_type size) {
            size_ = size;
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i) const {
            return value_type ();
        }

        NUMERICS_INLINE
        value_type operator [] (size_type i) const {
            return (*this) (i);
        }

        // Assignment
        NUMERICS_INLINE
        zero_vector &operator = (const zero_vector &v) {
            check (size_ == v.size_, bad_size ());
            size_ = v.size_;
            return *this;
        }
        NUMERICS_INLINE
        zero_vector &assign_temporary (zero_vector &v) {
            swap (v);
            return *this;
        }

        // Swapping
        NUMERICS_INLINE
        void swap (zero_vector &v) {
            // Too unusual semantic.
            // check (this != &v, external_logic ());
            if (this != &v) {
                check (size_ == v.size_, bad_size ());
                std::swap (size_, v.size_);
            }
        }
#ifdef NUMERICS_FRIEND_FUNCTION
        NUMERICS_INLINE
        friend void swap (zero_vector &v1, zero_vector &v2) {
            v1.swap (v2);
        }
#endif

        class const_iterator;

        // Element lookup
        NUMERICS_INLINE
        const_iterator find_first (size_type i) const {
            return const_iterator (*this, 0);
        }
        NUMERICS_INLINE
        const_iterator find_last (size_type i) const {
            return const_iterator (*this, 0);
        }

        // Iterators simply are pointers.

        class const_iterator:
            public container_const_reference<zero_vector>,
            public bidirectional_iterator_base<sparse_bidirectional_iterator_tag,
                                               const_iterator, value_type> {
        public:
            typedef sparse_bidirectional_iterator_tag iterator_category;
#ifndef BOOST_MSVC_STD_ITERATOR
            typedef typename zero_vector::difference_type difference_type;
            typedef typename zero_vector::value_type value_type;
            typedef typename zero_vector::value_type reference;
            typedef typename zero_vector::const_pointer pointer;
#endif

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator ():
                container_const_reference<zero_vector> (), it_ () {}
            NUMERICS_INLINE
            const_iterator (const zero_vector &v, const const_iterator_type &it):
                container_const_reference<zero_vector> (v), it_ (it) {}

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

            // Dereference
            NUMERICS_INLINE
            value_type operator * () const {
                check (index () < (*this) ().size (), bad_index ());
                return (*this) () (index ());
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                return it_;
            }

            // Assignment
            NUMERICS_INLINE
            const_iterator &operator = (const const_iterator &it) {
                container_const_reference<zero_vector>::assign (&it ());
                it_ = it.it_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it_ == it.it_;
            }

        private:
            const_iterator_type it_;
        };

        typedef const_iterator iterator;

        NUMERICS_INLINE
        const_iterator begin () const {
            return find_first (0);
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return find_last (size_);
        }

        // Reverse iterator

#ifdef BOOST_MSVC_STD_ITERATOR
        typedef reverse_iterator<const_iterator, value_type, value_type> const_reverse_iterator;
#else
        typedef reverse_iterator<const_iterator> const_reverse_iterator;
#endif

        NUMERICS_INLINE
        const_reverse_iterator rbegin () const {
            return const_reverse_iterator (end ());
        }
        NUMERICS_INLINE
        const_reverse_iterator rend () const {
            return const_reverse_iterator (begin ());
        }

    private:
        size_type size_;
    };

    // Scalar vector class
    template<class T>
    class scalar_vector:
        public vector_expression<scalar_vector<T> > {
    public:
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef const T &const_reference;
        typedef T &reference;
        typedef const T *const_pointer;
        typedef T *pointer;
        typedef const scalar_vector<T> const_self_type;
        typedef scalar_vector<T> self_type;
        typedef const vector_const_reference<const_self_type> const_closure_type;
#ifdef NUMERICS_DEPRECATED
        typedef const vector_range<const_self_type> const_vector_range_type;
#endif
        typedef size_type const_iterator_type;
        typedef dense_tag storage_category;

        // Construction and destruction
        NUMERICS_INLINE
        scalar_vector ():
            size_ (0), value_ () {}
        NUMERICS_INLINE
        scalar_vector (size_type size, const value_type &value):
            size_ (size), value_ (value) {}
        NUMERICS_INLINE
        scalar_vector (const scalar_vector &v):
            size_ (v.size_), value_ (v.value_) {}

        // Accessors
        NUMERICS_INLINE
        size_type size () const { 
            return size_; 
        }

        // Resizing
        NUMERICS_INLINE
        void resize (size_type size) {
            size_ = size;
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i) const {
            return value_; 
        }

        NUMERICS_INLINE
        value_type operator [] (size_type i) const { 
            return value_; 
        }

        // Assignment
        NUMERICS_INLINE
        scalar_vector &operator = (const scalar_vector &v) {
            check (size_ == v.size_, bad_size ());
            size_ = v.size_;
            value_ = v.value_;
            return *this;
        }
        NUMERICS_INLINE
        scalar_vector &assign_temporary (scalar_vector &v) { 
            swap (v);
            return *this;
        }

        // Swapping
        NUMERICS_INLINE
        void swap (scalar_vector &v) {
            // Too unusual semantic.
            // check (this != &v, external_logic ());
            if (this != &v) {
                check (size_ == v.size_, bad_size ());
                std::swap (size_, v.size_);
                std::swap (value_, v.value_);
            }
        }
#ifdef NUMERICS_FRIEND_FUNCTION
        NUMERICS_INLINE
        friend void swap (scalar_vector &v1, scalar_vector &v2) {
            v1.swap (v2);
        }
#endif

#ifdef NUMERICS_USE_INDEXED_ITERATOR
        typedef indexed_const_iterator<self_type, dense_random_access_iterator_tag> iterator;
        typedef indexed_const_iterator<self_type, dense_random_access_iterator_tag> const_iterator;
#else
        class const_iterator;
#endif

        // Element lookup
        NUMERICS_INLINE
        const_iterator find (size_type i) const {
            return const_iterator (*this, i);
        }
        NUMERICS_INLINE
        const_iterator find_first (size_type i) const {
            return find (i);
        }
        NUMERICS_INLINE
        const_iterator find_last (size_type i) const {
            return find (i);
        }

        // Iterators simply are pointers.

#ifndef NUMERICS_USE_INDEXED_ITERATOR
        class const_iterator:
            public container_const_reference<scalar_vector>,
            public random_access_iterator_base<dense_random_access_iterator_tag,
                                               const_iterator, value_type> {
        public:
            typedef dense_random_access_iterator_tag iterator_category;
#ifndef BOOST_MSVC_STD_ITERATOR
            typedef typename scalar_vector::difference_type difference_type;
            typedef typename scalar_vector::value_type value_type;
            typedef typename scalar_vector::value_type reference;
            typedef typename scalar_vector::const_pointer pointer;
#endif

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator ():
                container_const_reference<scalar_vector> (), it_ () {}
            NUMERICS_INLINE
            const_iterator (const scalar_vector &v, const const_iterator_type &it):
                container_const_reference<scalar_vector> (v), it_ (it) {}

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
                check (index () < (*this) ().size (), bad_index ());
                return (*this) () (index ());
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                return it_;
            }

            // Assignment 
            NUMERICS_INLINE
            const_iterator &operator = (const const_iterator &it) {
                container_const_reference<scalar_vector>::assign (&it ());
                it_ = it.it_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it_ == it.it_;
            }
            NUMERICS_INLINE
            bool operator < (const const_iterator &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it_ < it.it_;
            }

        private:
            const_iterator_type it_;
        };

        typedef const_iterator iterator;
#endif

        NUMERICS_INLINE
        const_iterator begin () const {
            return find_first (0);
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return find_last (size_);
        }

        // Reverse iterator

#ifdef BOOST_MSVC_STD_ITERATOR
        typedef reverse_iterator<const_iterator, value_type, value_type> const_reverse_iterator;
#else
        typedef reverse_iterator<const_iterator> const_reverse_iterator;
#endif

        NUMERICS_INLINE
        const_reverse_iterator rbegin () const {
            return const_reverse_iterator (end ());
        }
        NUMERICS_INLINE
        const_reverse_iterator rend () const {
            return const_reverse_iterator (begin ());
        }

    private:
        size_type size_;
        value_type value_;
    };

    // Array based vector class 
    template<class T, std::size_t N>
    class c_vector: 
        public vector_expression<c_vector<T, N> > {
    public:      
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef const T &const_reference;
        typedef T &reference;
        typedef const T *const_pointer;
        typedef T *pointer;
        typedef const c_vector<T, N> const_self_type;
        typedef c_vector<T, N> self_type;
        typedef const vector_const_reference<const_self_type> const_closure_type;
        typedef vector_reference<self_type> closure_type;
#ifdef NUMERICS_DEPRECATED
        typedef const vector_range<const_self_type> const_vector_range_type;
        typedef vector_range<self_type> vector_range_type;
#endif
        typedef const T *const_iterator_type;
        typedef T *iterator_type;
        typedef dense_tag storage_category;

        // Construction and destruction
        NUMERICS_INLINE
        c_vector (): 
            size_ (N) /* , data_ () */ {}
        NUMERICS_EXPLICIT NUMERICS_INLINE
        c_vector (size_type size): 
            size_ (size) /* , data_ () */ {
            if (size_ > N) 
                throw std::bad_alloc ();
        }
        NUMERICS_INLINE
        c_vector (const c_vector &v): 
            size_ (v.size_) /* , data_ () */ {
            if (size_ > N) 
                throw std::bad_alloc ();
            *this = v;
        }
        template<class AE>
        NUMERICS_INLINE
        c_vector (const vector_expression<AE> &ae): 
            size_ (ae ().size ()) /* , data_ () */ { 
            if (size_ > N) 
                throw std::bad_alloc ();
            vector_assign<scalar_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
        }

        // Accessors
        NUMERICS_INLINE
        size_type size () const { 
            return size_; 
        }

        // Resizing
        NUMERICS_INLINE
        void resize (size_type size) {
            if (size > N) 
                throw std::bad_alloc ();
            // The content of the array is intentionally not copied.
            size_ = size;
        }

        // Element access
        NUMERICS_INLINE
        const_reference operator () (size_type i) const {
            check (i < size_,  bad_index ());
            return data_ [i]; 
        }
        NUMERICS_INLINE
        reference operator () (size_type i) {
            check (i < size_, bad_index ());
            return data_ [i]; 
        }

        NUMERICS_INLINE
        const_reference operator [] (size_type i) const { 
            return (*this) (i); 
        }
        NUMERICS_INLINE
        reference operator [] (size_type i) { 
            return (*this) (i); 
        }

        // Assignment
        NUMERICS_INLINE
        c_vector &operator = (const c_vector &v) { 
            check (size_ == v.size_, bad_size ());
            size_ = v.size_;
            std::copy (v.data_, v.data_ + v.size_, data_);
            return *this;
        }
        NUMERICS_INLINE
        c_vector &assign_temporary (c_vector &v) {
            swap (v);
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        c_vector &operator = (const vector_expression<AE> &ae) {
#ifdef NUMERICS_MUTABLE_TEMPORARY
            return assign_temporary (self_type (ae));
#else
            // return assign (self_type (ae));
            self_type temporary (ae);
            return assign_temporary (temporary);
#endif
        }
        template<class AE>
        NUMERICS_INLINE
        c_vector &reset (const vector_expression<AE> &ae) {
            self_type temporary (ae);
            resize (temporary.size ());
            return assign_temporary (temporary);
        }
        template<class AE>
        NUMERICS_INLINE
        c_vector &assign (const vector_expression<AE> &ae) {
            vector_assign<scalar_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        c_vector &operator += (const vector_expression<AE> &ae) {
#ifdef NUMERICS_MUTABLE_TEMPORARY
            return assign_temporary (self_type (*this + ae));
#else
            // return assign (self_type (*this + ae));
            self_type temporary (*this + ae);
            return assign_temporary (temporary);
#endif
        }
        template<class AE>
        NUMERICS_INLINE
        c_vector &plus_assign (const vector_expression<AE> &ae) {
            vector_assign<scalar_plus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        c_vector &operator -= (const vector_expression<AE> &ae) {
#ifdef NUMERICS_MUTABLE_TEMPORARY
            return assign_temporary (self_type (*this - ae));
#else
            // return assign (self_type (*this - ae));
            self_type temporary (*this - ae);
            return assign_temporary (temporary);
#endif
        }
        template<class AE>
        NUMERICS_INLINE
        c_vector &minus_assign (const vector_expression<AE> &ae) {
            vector_assign<scalar_minus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        c_vector &operator *= (const AT &at) {
            vector_assign_scalar<scalar_multiplies_assign<value_type, AT> > () (*this, at);
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        c_vector &operator /= (const AT &at) {
            vector_assign_scalar<scalar_divides_assign<value_type, AT> > () (*this, at);
            return *this;
        }

        // Swapping
        NUMERICS_INLINE
        void swap (c_vector &v) {
            // Too unusual semantic.
            // check (this != &v, external_logic ());
            if (this != &v) {
                check (size_ == v.size_, bad_size ());
                std::swap (size_, v.size_);
                std::swap_ranges (data_, data_ + size_, v.data_);
            }
        }
#ifdef NUMERICS_FRIEND_FUNCTION
        NUMERICS_INLINE
        friend void swap (c_vector &v1, c_vector &v2) {
            v1.swap (v2);
        }
#endif

        // Element insertion and erasure
        NUMERICS_INLINE
        void insert (size_type i, const_reference t) {
            check (i < size_, bad_index ());
            check (data_ [i] == value_type (), bad_index ());
            data_ [i] = t;
        }
        NUMERICS_INLINE
        void erase (size_type i) {
            check (i < size_, bad_index ());
            data_ [i] = value_type ();
        }
        NUMERICS_INLINE
        void clear () {
            std::fill (data_, data_ + size_, value_type ());
        }

#ifdef NUMERICS_USE_INDEXED_ITERATOR
        typedef indexed_iterator<self_type, dense_random_access_iterator_tag> iterator;
        typedef indexed_const_iterator<self_type, dense_random_access_iterator_tag> const_iterator;
#else
        class const_iterator;
        class iterator;
#endif

        // Element lookup
        NUMERICS_INLINE
        const_iterator find (size_type i) const {
#ifndef NUMERICS_USE_INDEXED_ITERATOR
            return const_iterator (*this, &data_ [i]);
#else
            return const_iterator (*this, i);
#endif
        }
        NUMERICS_INLINE
        iterator find (size_type i) {
#ifndef NUMERICS_USE_INDEXED_ITERATOR
            return iterator (*this, &data_ [i]);
#else
            return iterator (*this, i);
#endif
        }
        NUMERICS_INLINE
        const_iterator find_first (size_type i) const {
            return find (i);
        }
        NUMERICS_INLINE
        iterator find_first (size_type i) {
            return find (i);
        }
        NUMERICS_INLINE
        const_iterator find_last (size_type i) const {
            return find (i);
        }
        NUMERICS_INLINE
        iterator find_last (size_type i) {
            return find (i);
        }

        // Iterators simply are pointers.

#ifndef NUMERICS_USE_INDEXED_ITERATOR
        class const_iterator:
            public container_const_reference<c_vector>,
            public random_access_iterator_base<dense_random_access_iterator_tag,
                                               const_iterator, value_type> {
        public:
            typedef dense_random_access_iterator_tag iterator_category;
#ifndef BOOST_MSVC_STD_ITERATOR
            typedef typename c_vector::difference_type difference_type;
            typedef typename c_vector::value_type value_type;
            typedef typename c_vector::const_reference reference;
            typedef typename c_vector::const_pointer pointer;
#endif

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator ():
                container_const_reference<c_vector> (), it_ () {}
            NUMERICS_INLINE
            const_iterator (const c_vector &v, const const_iterator_type &it):
                container_const_reference<c_vector> (v), it_ (it) {}
            NUMERICS_INLINE
#ifndef NUMERICS_QUALIFIED_TYPENAME
            const_iterator (const iterator &it):
#else
            const_iterator (const typename c_vector::iterator &it):
#endif
                container_const_reference<c_vector> (it ()), it_ (it.it_) {}

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
            const_reference operator * () const {
                check (index () < (*this) ().size (), bad_index ());
                return *it_;
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                const c_vector &v = (*this) ();
                return it_ - v.begin ().it_;
            }

            // Assignment 
            NUMERICS_INLINE
            const_iterator &operator = (const const_iterator &it) {
                container_const_reference<c_vector>::assign (&it ());
                it_ = it.it_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it_ == it.it_;
            }
            NUMERICS_INLINE
            bool operator < (const const_iterator &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it_ < it.it_;
            }

        private:
            const_iterator_type it_;

            friend class iterator;
        };
#endif

        NUMERICS_INLINE
        const_iterator begin () const {
            return find_first (0);
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return find_last (size_);
        }

#ifndef NUMERICS_USE_INDEXED_ITERATOR
        class iterator:
            public container_reference<c_vector>,
            public random_access_iterator_base<dense_random_access_iterator_tag,
                                               iterator, value_type> {
        public:
            typedef dense_random_access_iterator_tag iterator_category;
#ifndef BOOST_MSVC_STD_ITERATOR
            typedef typename c_vector::difference_type difference_type;
            typedef typename c_vector::value_type value_type;
            typedef typename c_vector::reference reference;
            typedef typename c_vector::pointer pointer;
#endif

            // Construction and destruction
            NUMERICS_INLINE
            iterator ():
                container_reference<c_vector> (), it_ () {}
            NUMERICS_INLINE
            iterator (c_vector &v, const iterator_type &it):
                container_reference<c_vector> (v), it_ (it) {}

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
            reference operator * () const {
                check (index () < (*this) ().size (), bad_index ());
                return *it_;
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                c_vector &v = (*this) ();
                return it_ - v.begin ().it_;
            }

            // Assignment 
            NUMERICS_INLINE
            iterator &operator = (const iterator &it) {
                container_reference<c_vector>::assign (&it ());
                it_ = it.it_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const iterator &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it_ == it.it_;
            }
            NUMERICS_INLINE
            bool operator < (const iterator &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it_ < it.it_;
            }

        private:
            iterator_type it_;

            friend class const_iterator;
        };
#endif

        NUMERICS_INLINE
        iterator begin () {
            return find_first (0);
        }
        NUMERICS_INLINE
        iterator end () {
            return find_last (size_);
        }

        // Reverse iterator

#ifdef BOOST_MSVC_STD_ITERATOR
        typedef reverse_iterator<const_iterator, value_type, value_type> const_reverse_iterator;
#else
        typedef reverse_iterator<const_iterator> const_reverse_iterator;
#endif

        NUMERICS_INLINE
        const_reverse_iterator rbegin () const {
            return const_reverse_iterator (end ());
        }
        NUMERICS_INLINE
        const_reverse_iterator rend () const {
            return const_reverse_iterator (begin ());
        }

#ifdef BOOST_MSVC_STD_ITERATOR
        typedef reverse_iterator<iterator, value_type, reference> reverse_iterator;
#else
        typedef reverse_iterator<iterator> reverse_iterator;
#endif

        NUMERICS_INLINE
        reverse_iterator rbegin () {
            return reverse_iterator (end ());
        }
        NUMERICS_INLINE
        reverse_iterator rend () {
            return reverse_iterator (begin ());
        }

    private:
        size_type size_;
        value_type data_ [N];
    };

}}

#endif



















