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

#ifndef NUMERICS_VECTOR_H
#define NUMERICS_VECTOR_H

#include "config.h"
#include "storage.h"
#include "vector_et.h"
#include "vector_pr.h"

// Iterators based on ideas of Jeremy Siek

namespace numerics {

    // This functor computes the address translation 
    // vector [i] -> storage [i]
    struct forward {
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;

        // Indexing
        static NUMERICS_INLINE
        size_type element (size_type i, size_type size) {
            check (i <= size, bad_index ());
            return i;
        }
        static NUMERICS_INLINE
        difference_type distance (difference_type i, size_type size) {
            return i;
        }
        static NUMERICS_INLINE
        size_type index (difference_type i, size_type size) {
            return i;
        }
        static NUMERICS_INLINE
        difference_type one () {
            return 1;
        }

        // Iterating
        template<class I>
        static NUMERICS_INLINE
        void increment (I &it) {
            ++ it;
        }
        template<class I>
        static NUMERICS_INLINE
        void decrement (I &it) {
            -- it;
        }
    };

    // This functor computes the address translation 
    // vector [i] -> storage [size - 1 - i]
    struct backward {
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;

        // Indexing
        static NUMERICS_INLINE
        size_type element (size_type i, size_type size) {
            check (i <= size, bad_index ());
            return size - 1 - i;
        }
        static NUMERICS_INLINE
        difference_type distance (difference_type i, size_type size) {
            return size - 1 - i;
        }
        static NUMERICS_INLINE
        size_type index (difference_type i, size_type size) {
            return size - 1 - i;
        }
        static NUMERICS_INLINE
        difference_type one () {
            return -1;
        }

        // Iterating
        template<class I>
        static NUMERICS_INLINE
        void increment (I &it) {
            -- it;
        }
        template<class I>
        static NUMERICS_INLINE
        void decrement (I &it) {
            ++ it;
        }
    };

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
            while (-- size >= 0)
                functor_type () (*it, t), ++ it;
        }
        // Indexing case
        template<class V, class T>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void indexing_assign (V &v, const T &t) {
            typedef typename V::difference_type difference_type;
            difference_type size (v.size ());
            for (difference_type i = 0; i < size; ++ i)
                functor_type () (v (i), t); 
        }

        // Dense case
        template<class V, class T>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (V &v, const T &t, dense_tag) {
            typedef typename V::difference_type difference_type;
            difference_type size (v.size ());
            if (size >= NUMERICS_ITERATOR_THRESHOLD) 
                iterating_assign (v, t);
            else 
                indexing_assign (v, t);
        }
        // Packed case
        template<class V, class T>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (V &v, const T &t, packed_tag) {
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
    struct vector_assign_traits<dense_tag, assign_tag, packed_bidirectional_iterator_tag> {
        typedef packed_tag dispatch_category;
    };
    template<> 
    struct vector_assign_traits<dense_tag, computed_assign_tag, packed_bidirectional_iterator_tag> {
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
    struct vector_assign_traits<packed_tag, assign_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_tag dispatch_category;
    };
    template<> 
    struct vector_assign_traits<packed_tag, computed_assign_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_tag dispatch_category;
    };

    template<> 
    struct vector_assign_traits<sparse_tag, computed_assign_tag, dense_random_access_iterator_tag> {
        typedef sparse_proxy_tag dispatch_category;
    };
    template<> 
    struct vector_assign_traits<sparse_tag, computed_assign_tag, packed_bidirectional_iterator_tag> {
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
            typename E::const_iterator ite (e ().begin ());
            while (-- size >= 0) 
                functor_type () (*it, *ite), ++ it, ++ ite;
        }
        // Indexing scase
        template<class V, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void indexing_assign (V &v, const vector_expression<E> &e) {
            typedef typename V::difference_type difference_type;
            difference_type size (common (v.size (), e ().size ()));
            for (difference_type i = 0; i < size; ++ i)
                functor_type () (v (i), e () (i)); 
        }

        // Dense case
        template<class V, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (V &v, const vector_expression<E> &e, dense_tag) {
            typedef typename V::size_type size_type;
            typedef typename V::difference_type difference_type;
            difference_type size (common (v.size (), e ().size ()));
            if (size >= NUMERICS_ITERATOR_THRESHOLD) 
                iterating_assign (v, e);
            else
                indexing_assign (v, e);
        }
        // Packed case
        template<class V, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (V &v, const vector_expression<E> &e, packed_tag) {
            check (v.size () == e ().size (), bad_size ());
            typedef typename V::value_type value_type;
            typename V::iterator it (v.begin ());
            typename V::iterator it_end (v.end ());
            typename E::const_iterator ite (e ().begin ());
            typename E::const_iterator ite_end (e ().end ());
            while (ite != ite_end && ite.index () < it.index ()) 
                ++ ite;
            while (it != it_end && ite != ite_end) {
                functor_type () (*it, *ite);
                ++ it, ++ ite;
            }
            while (it != it_end) {
                functor_type () (*it, value_type ());
                ++ it;
            }
#ifdef NUMERICS_BOUNDS_CHECK
            {
                // Need the const member dispatched.
                const V &cv = v;
                typename E::const_iterator ite (e ().begin ());
                typename E::const_iterator ite_end (e ().end ());
                while (ite != ite_end) {
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
#ifdef NUMERICS_BOUNDS_CHECK
                    // Need the const member dispatched.
                    const V &cv = v;
                    check (*ite == cv (ite.index ()), bad_index ());
#endif
                    ++ ite;
                }
            }
            while (it != it_end) {
                functor_type () (*it, value_type ());
                ++ it;
            }
#ifdef NUMERICS_BOUNDS_CHECK
            while (ite != ite_end) {
                // Need the const member dispatched.
                const V &cv = v;
                check (*ite == cv (ite.index ()), bad_index ());
                ++ ite;
            }
#endif
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
    template<class T, class F, class A>
    class vector: 
        public vector_expression<vector<T, F, A> > {
    public:      
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef const T &const_reference;
        typedef T &reference;
        typedef const T *const_pointer;
        typedef T *pointer;
        typedef F functor_type;
        typedef A array_type;
        typedef const vector<T, F, A> const_self_type;
        typedef vector<T, F, A> self_type;
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
        vector (const vector &v): 
            size_ (v.size_), data_ (v.data_) {}
        template<class AE>
        NUMERICS_INLINE
        vector (const vector_expression<AE> &ae): 
            size_ (ae ().size ()), data_ (ae ().size ()) { 
            vector_assign<scalar_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
        }

        // Resizing
        NUMERICS_INLINE
        void resize (size_type size) {
            size_ = size;
            data_.resize (size);
        }

        NUMERICS_INLINE
        size_type size () const { 
            return size_; 
        }

        // Element access
        NUMERICS_INLINE
        const_reference operator () (size_type i) const {
            return data_ [functor_type::element (i, size_)]; 
        }
        NUMERICS_INLINE
        reference operator () (size_type i) {
            return data_ [functor_type::element (i, size_)]; 
        }

        NUMERICS_INLINE
        const_reference operator [] (size_type i) const { 
            return (*this) (i); 
        }
        NUMERICS_INLINE
        reference operator [] (size_type i) { 
            return (*this) (i); 
        }

#ifdef NUMERICS_DEPRECATED
        NUMERICS_INLINE
        const_vector_range_type project (size_type start, size_type stop) const {
            return const_vector_range_type (*this, start, stop);
        }
        NUMERICS_INLINE
        const_vector_range_type project (const range &r) const {
            return const_vector_range_type (*this, r);
        }
        NUMERICS_INLINE
        vector_range_type project (size_type start, size_type stop) {
            return vector_range_type (*this, start, stop);
        }
        NUMERICS_INLINE
        vector_range_type project (const range &r) {
            return vector_range_type (*this, r);
        }
#endif

        // Assignment
        NUMERICS_INLINE
        vector &operator = (const vector &v) { 
            check (size_ == v.size_, bad_size ());
            data_ = v.data_;
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
#ifndef USE_GCC
            return assign_temporary (self_type (ae));
#else
            return assign (self_type (ae));
#endif
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
#ifndef USE_GCC
            return assign_temporary (self_type (*this + ae));
#else
            return assign (self_type (*this + ae));
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
#ifndef USE_GCC
            return assign_temporary (self_type (*this - ae));
#else
            return assign (self_type (*this - ae));
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

        // Swapping
        NUMERICS_INLINE
	    void swap (vector &v) {
            check (this != &v, external_logic ());
            check (size_ == v.size_, bad_size ());
            std::swap (size_, v.size_);
            data_.swap (v.data_);
        }
#ifndef USE_GCC
        NUMERICS_INLINE
	    friend void swap (vector &v1, vector &v2) {
            v1.swap (v2);
        }
#endif

        // Element insertion and erasure
        NUMERICS_INLINE
        void insert (size_type i, const_reference t) {
            check (data_ [functor_type::element (i, size_)] == value_type (), bad_index ());
            data_.insert (data_.begin () + functor_type::element (i, size_), t);
        }
        NUMERICS_INLINE
        void erase (size_type i) {
            data_.erase (data_.begin () + functor_type::element (i, size_));
        }
        NUMERICS_INLINE
        void clear () {
            data_.clear ();
        }

#ifdef NUMERICS_USE_INDEXED_ITERATOR
        typedef indexed_iterator<self_type> iterator;
        typedef indexed_const_iterator<self_type> const_iterator;
#else
        class const_iterator;
        class iterator;
#endif

        // Element lookup
        NUMERICS_INLINE
        const_iterator find (size_type i) const {
#ifndef NUMERICS_USE_INDEXED_ITERATOR
            return const_iterator (*this, data_.begin () + functor_type::element (i, size_));
#else
            return const_iterator (*this, functor_type::element (i, size_));
#endif
        }
        NUMERICS_INLINE
        iterator find (size_type i) {
#ifndef NUMERICS_USE_INDEXED_ITERATOR
            return iterator (*this, data_.begin () + functor_type::element (i, size_));
#else
            return iterator (*this, functor_type::element (i, size_));
#endif
        }

        // Iterators simply are pointers.

#ifndef NUMERICS_USE_INDEXED_ITERATOR
        class const_iterator:
            public container_const_reference<vector>,
            public random_access_iterator_base<const_iterator, value_type> {
        public:
            typedef dense_random_access_iterator_tag iterator_category;
#ifndef USE_MSVC
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
            const_iterator (const iterator &it):
                container_const_reference<vector> (it ()), it_ (it.it_) {}

            // Arithmetic
            NUMERICS_INLINE
            const_iterator &operator ++ () {
                functor_type::increment (it_);
                return *this;
            }
            NUMERICS_INLINE
            const_iterator &operator -- () {
                functor_type::decrement (it_);
                return *this;
            }
            NUMERICS_INLINE
            const_iterator &operator += (difference_type n) {
                it_ += n * functor_type::one ();
                return *this;
            }
            NUMERICS_INLINE
            const_iterator &operator -= (difference_type n) {
                it_ -= n * functor_type::one ();
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const const_iterator &it) const {
                return functor_type::distance (it_ - it.it_, (*this) ().size ());
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
                const vector &v = (*this) ();
                return functor_type::index (it_ - v.begin ().it_, v.size ());
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

        private:
            const_iterator_type it_;

            friend class iterator;
        };
#endif

        NUMERICS_INLINE
        const_iterator begin () const {
            return find (0);
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return find (size_);
        }

#ifndef NUMERICS_USE_INDEXED_ITERATOR
        class iterator:
            public container_reference<vector>,
            public random_access_iterator_base<iterator, value_type> {
        public:
            typedef dense_random_access_iterator_tag iterator_category;
#ifndef USE_MSVC
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
                functor_type::increment (it_);
                return *this;
            }
            NUMERICS_INLINE
            iterator &operator -- () {
                functor_type::decrement (it_);
                return *this;
            }
            NUMERICS_INLINE
            iterator &operator += (difference_type n) {
                it_ += n * functor_type::one ();
                return *this;
            }
            NUMERICS_INLINE
            iterator &operator -= (difference_type n) {
                it_ -= n * functor_type::one ();
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const iterator &it) const {
                return functor_type::distance (it_ - it.it_, (*this) ().size ());
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
                vector &v = (*this) ();
                return functor_type::index (it_ - v.begin ().it_, v.size ());
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

        private:
            iterator_type it_;

            friend class const_iterator;
        };
#endif

        NUMERICS_INLINE
        iterator begin () {
            return find (0);
        }
        NUMERICS_INLINE
        iterator end () {
            return find (size_);
        }

        // Reverse iterator

#ifdef USE_MSVC
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

#ifdef USE_MSVC
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
        typedef dense_tag storage_category;

        // Construction and destruction
        NUMERICS_INLINE
        unit_vector (): 
            size_ (0), index_ (0) {}
        NUMERICS_EXPLICIT NUMERICS_INLINE
        unit_vector (size_type size, size_type index): 
            size_ (size), index_ (index) {}
        NUMERICS_INLINE
        unit_vector (const unit_vector &v): 
            size_ (v.size_), index_ (v.index_) {}

        // Resizing
        NUMERICS_INLINE
        void resize (size_type size) {
            size_ = size;
        }

        NUMERICS_INLINE
        size_type size () const { 
            return size_; 
        }
        NUMERICS_INLINE
        size_type index () const { 
            return index_; 
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

#ifdef NUMERICS_DEPRECATED
        NUMERICS_INLINE
        const_vector_range_type project (size_type start, size_type stop) const {
            return const_vector_range_type (*this, start, stop);
        }
        NUMERICS_INLINE
        const_vector_range_type project (const range &r) const {
            return const_vector_range_type (*this, r);
        }
#endif

        // Assignment
        NUMERICS_INLINE
        unit_vector &operator = (const unit_vector &v) { 
            check (size_ == v.size_, bad_size ());
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
            check (this != &v, external_logic ());
            check (size_ == v.size_, bad_size ());
            std::swap (size_, v.size_);
            std::swap (index_, v.index_);
        }
#ifndef USE_GCC
        NUMERICS_INLINE
	    friend void swap (unit_vector &v1, unit_vector &v2) {
            v1.swap (v2);
        }
#endif

#ifdef NUMERICS_USE_INDEXED_ITERATOR
        typedef indexed_const_iterator<self_type> iterator;
        typedef indexed_const_iterator<self_type> const_iterator;
#else
        class const_iterator;
#endif

        // Element lookup
        NUMERICS_INLINE
        const_iterator find (size_type i) const {
            return const_iterator (*this, i);
        }

        // Iterators simply are pointers.

#ifndef NUMERICS_USE_INDEXED_ITERATOR
        class const_iterator:
            public container_const_reference<unit_vector>,
            public random_access_iterator_base<const_iterator, value_type> {
        public:
            typedef dense_random_access_iterator_tag iterator_category;
#ifndef USE_MSVC
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
                return it_ == (*this) ().index ();
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

        private:
            const_iterator_type it_;
        };

        typedef const_iterator iterator;
#endif

        NUMERICS_INLINE
        const_iterator begin () const {
            return find (0);
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return find (size_);
        }

        // Reverse iterator

#ifdef USE_MSVC
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

        // Resizing
        NUMERICS_INLINE
        void resize (size_type size) {
            if (size > N) 
                throw std::bad_alloc ();
            // The content of the array is intentionally not copied.
            size_ = size;
        }

        NUMERICS_INLINE
        size_type size () const { 
            return size_; 
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

#ifdef NUMERICS_DEPRECATED
        NUMERICS_INLINE
        const_vector_range_type project (size_type start, size_type stop) const {
            return const_vector_range_type (*this, start, stop);
        }
        NUMERICS_INLINE
        const_vector_range_type project (const range &r) const {
            return const_vector_range_type (*this, r);
        }
        NUMERICS_INLINE
        vector_range_type project (size_type start, size_type stop) {
            return vector_range_type (*this, start, stop);
        }
        NUMERICS_INLINE
        vector_range_type project (const range &r) {
            return vector_range_type (*this, r);
        }
#endif

        // Assignment
        NUMERICS_INLINE
        c_vector &operator = (const c_vector &v) { 
            check (size_ == v.size_, bad_size ());
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
#ifndef USE_GCC
            return assign_temporary (self_type (ae));
#else
            return assign (self_type (ae));
#endif
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
#ifndef USE_GCC
            return assign_temporary (self_type (*this + ae));
#else
            return assign (self_type (*this + ae));
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
#ifndef USE_GCC
            return assign_temporary (self_type (*this - ae));
#else
            return assign (self_type (*this - ae));
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

        // Swapping
        NUMERICS_INLINE
	    void swap (c_vector &v) {
            check (this != &v, external_logic ());
            check (size_ == v.size_, bad_size ());
            std::swap (size_, v.size_);
            std::swap_ranges (data_, data_ + size_, v.data_);
        }
#ifndef USE_GCC
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
        typedef indexed_iterator<self_type> iterator;
        typedef indexed_const_iterator<self_type> const_iterator;
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

        // Iterators simply are pointers.

#ifndef NUMERICS_USE_INDEXED_ITERATOR
        class const_iterator:
            public container_const_reference<c_vector>,
            public random_access_iterator_base<const_iterator, value_type> {
        public:
            typedef dense_random_access_iterator_tag iterator_category;
#ifndef USE_MSVC
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
            const_iterator (const iterator &it):
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

        private:
            const_iterator_type it_;

            friend class iterator;
        };
#endif

        NUMERICS_INLINE
        const_iterator begin () const {
            return find (0);
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return find (size_);
        }

#ifndef NUMERICS_USE_INDEXED_ITERATOR
        class iterator:
            public container_reference<c_vector>,
            public random_access_iterator_base<iterator, value_type> {
        public:
            typedef dense_random_access_iterator_tag iterator_category;
#ifndef USE_MSVC
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
                const c_vector &v = (*this) ();
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

        private:
            iterator_type it_;

            friend class const_iterator;
        };
#endif

        NUMERICS_INLINE
        iterator begin () {
            return find (0);
        }
        NUMERICS_INLINE
        iterator end () {
            return find (size_);
        }

        // Reverse iterator

#ifdef USE_MSVC
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

#ifdef USE_MSVC
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

}

#endif 





