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
            check<bad_index>::precondition (i <= size);
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
            check<bad_index>::precondition (i <= size);
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

#ifndef NUMERICS_USE_ITERATOR
    // Indexing version for the computation of 
    // vector assignment_operation scalar
    template<class F>
    struct vector_assign_scalar {
        typedef F functor_type;
        typedef typename F::assign_category assign_category;

        template<class V, class T>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (V &v, const T &t) {
            typedef typename V::size_type size_type;
            size_type size (v.size ());            
            for (size_type i = 0; i < size; ++ i)
                functor_type () (v (i), t); 
        }
    };
#else 
    template<class S, class A> 
    struct vector_assign_scalar_traits {
        typedef S dispatch_category;
    };

    // Iterating version for the computation of 
    // vector assignment_operation scalar
    template<class F>
    struct vector_assign_scalar {
        typedef F functor_type;
        typedef typename F::assign_category assign_category;

        // Dense case
        template<class V, class T>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (V &v, const T &t, dense_tag) {
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
#endif

#ifndef NUMERICS_USE_ITERATOR
    // Indexing version for the computation of 
    // vector assignment_operation vector_expression
    template<class F>
    struct vector_assign {
        typedef F functor_type;
        typedef typename F::assign_category assign_category;

        template<class V, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (V &v, const vector_expression<E> &e) {
            typedef typename V::size_type size_type;
            size_type size (common (v.size (), e ().size ()));
            for (size_type i = 0; i < size; ++ i)
                functor_type () (v (i), e () (i));
        }
    };
#else 
    template<class LS, class A, class RI> 
    struct vector_assign_traits {
        typedef LS dispatch_category;
    };

    template<> 
    struct vector_assign_traits<dense_tag, assign_tag, std::bidirectional_iterator_tag> {
        typedef sparse_tag dispatch_category;
    };
    template<> 
    struct vector_assign_traits<dense_tag, computed_assign_tag, std::bidirectional_iterator_tag> {
        typedef sparse_tag dispatch_category;
    };

    template<> 
    struct vector_assign_traits<sparse_tag, computed_assign_tag, std::random_access_iterator_tag> {
        typedef sparse_proxy_tag dispatch_category;
    };
    template<> 
    struct vector_assign_traits<sparse_tag, computed_assign_tag, std::bidirectional_iterator_tag> {
        typedef sparse_proxy_tag dispatch_category;
    };

    // Iterating version for the computation of 
    // vector assignment_operation vector_expression
    template<class F>
    struct vector_assign {
        typedef F functor_type;
        typedef typename F::assign_category assign_category;

        // Dense case
        template<class V, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (V &v, const vector_expression<E> &e, dense_tag) {
            typedef typename V::difference_type difference_type;
            typename V::iterator it (v.begin ());
            typename E::const_iterator ite (e ().begin ());
            difference_type size (common (v.end () - it, e ().end () - ite));
            while (-- size >= 0) 
                functor_type () (*it, *ite), ++ it, ++ ite;
        }
        // Sparse case
        template<class V, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (V &v, const vector_expression<E> &e, sparse_tag) {
            check<bad_size>::precondition (v.size () == e ().size ());
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
            check<bad_size>::precondition (v.size () == e ().size ());
            typedef typename V::value_type value_type;
            typename V::iterator it (v.begin ());
            typename V::iterator it_end (v.end ());
            typename E::const_iterator ite (e ().begin ());
            typename E::const_iterator ite_end (e ().end ());
            while (ite != ite_end) {
                check<bad_index>::invariant (it == it_end || it.index () >= ite.index ());
                if (it != it_end && it.index () == ite.index ()) 
                    functor_type () (*it, *ite), ++ it;
                else
                    check<bad_index>::invariant (*ite == value_type (0));
                ++ ite;
            }
            check<bad_size>::postcondition (it == it_end);
            check<bad_size>::postcondition (ite == ite_end);
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
#endif

    template<class E> 
    class vector_const_reference;
    template<class E> 
    class vector_reference;

    // Array based vector class 
    template<class T, class A, class F>
    class vector: 
        public vector_expression<vector<T, A, F> > {
    public:      
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef const T &const_reference_type;
        typedef T &reference_type;
        typedef A array_type;
        typedef F functor_type;
        typedef vector<T, A, F> self_type;
        typedef vector_const_reference<self_type> const_closure_type;
        typedef vector_reference<self_type> closure_type;
        typedef vector_range<self_type> vector_range_type;
        typedef typename A::const_iterator const_iterator_type;
        typedef typename A::iterator iterator_type;
        typedef struct dense_tag storage_category;

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
        NUMERICS_EXPLICIT NUMERICS_INLINE
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
        value_type operator () (size_type i) const {
            return data_ [functor_type::element (i, size_)]; 
        }
        NUMERICS_INLINE
        reference_type operator () (size_type i) {
            return data_ [functor_type::element (i, size_)]; 
        }

        NUMERICS_INLINE
        value_type operator [] (size_type i) const { 
            return data_ [functor_type::element (i, size_)]; 
        }
        NUMERICS_INLINE
        reference_type operator [] (size_type i) { 
            return data_ [functor_type::element (i, size_)]; 
        }

        NUMERICS_INLINE
        vector_range_type project (size_type start, size_type stop) {
            return vector_range_type (*this, start, stop);
        }
        NUMERICS_INLINE
        vector_range_type project (const range &r) {
            return vector_range_type (r);
        }

        // Assignment
        NUMERICS_INLINE
        vector &operator = (const vector &v) { 
            check<bad_size>::precondition (size_ == v.size_);
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
            check<external_logic>::precondition (this != &v);
            check<bad_size>::precondition (size_ == v.size_);
            std::swap (size_, v.size_);
            data_.swap (v.data_);
        }
#ifndef USE_GCC
        NUMERICS_INLINE
	    friend void swap (vector &v1, vector &v2) {
            v1.swap (v2);
        }
#endif

        // Element insertion
        NUMERICS_INLINE
        void clear () {
            data_.clear ();
        }
        NUMERICS_INLINE
        void insert (size_type i, const T &t) {
            data_.insert (data_.begin () + functor_type::element (i, size_), t);
        }

        class const_iterator;
        class iterator;

        // Element lookup
        NUMERICS_INLINE
        const_iterator find (size_type i) const {
            return const_iterator (*this, data_.begin () + functor_type::element (i, size_));
        }
        NUMERICS_INLINE
        iterator find (size_type i) {
            return iterator (*this, data_.begin () + functor_type::element (i, size_));
        }

        // Iterators simply are pointers.

        class const_iterator:
            public container_const_reference<vector>,
            public random_access_iterator_base<const_iterator, value_type> {
        public:
            typedef std::random_access_iterator_tag iterator_category;

            // Construction and destruction
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
            value_type operator * () const {
                check<bad_index>::precondition (index () < (*this) ().size ());
                return *it_;
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                const vector &v = (*this) ();
                return functor_type::index (it_ - v.begin ().it_, v.size ());
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator &it) const {
                check<external_logic>::precondition (&(*this) () == &it ());
                return it_ == it.it_;
            }

        private:
            const_iterator_type it_;

            friend class iterator;
        };

        NUMERICS_INLINE
        const_iterator begin () const {
            return find (0);
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return find (size_);
        }

        class iterator:
            public container_reference<vector>,
            public random_access_iterator_base<iterator, value_type> {
        public:
            typedef std::random_access_iterator_tag iterator_category;

            // Construction and destruction
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
            reference_type operator * () {
                check<bad_index>::precondition (index () < (*this) ().size ());
                return *it_;
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                const vector &v = (*this) ();
                return functor_type::index (it_ - v.begin ().it_, v.size ());
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
            return find (size_);
        }

        // Reverse iterator

#ifndef USE_GCC

#ifdef USE_MSVC
        typedef std::reverse_iterator<const_iterator, value_type, const_reference_type> const_reverse_iterator;
#else
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
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
        typedef std::reverse_iterator<iterator, value_type, reference_type> reverse_iterator;
#else
        typedef std::reverse_iterator<iterator> reverse_iterator;
#endif

        NUMERICS_INLINE
        reverse_iterator rbegin () {
            return reverse_iterator (end ());
        }
        NUMERICS_INLINE
        reverse_iterator rend () {
            return reverse_iterator (begin ());
        }

#endif

    private:
        size_type size_;
        array_type data_;
    };

    // Vector based range class
    template<class V>
    class vector_range:
		public vector_expression<vector_range<V> > {
    public:      
        typedef V vector_type;
        typedef typename V::size_type size_type;
        typedef typename V::difference_type difference_type;
        typedef typename V::value_type value_type;
        typedef typename V::const_reference_type const_reference_type;
        typedef typename V::reference_type reference_type;
        typedef vector_range<V> const_closure_type;
        typedef vector_range<V> closure_type;
        typedef typename V::const_iterator const_iterator_type;
        typedef typename V::iterator iterator_type;
        typedef typename proxy_traits<typename V::storage_category>::storage_category storage_category;

        // Construction and destruction
        NUMERICS_INLINE
        vector_range (vector_type &data, const range &r): 
            data_ (data), r_ (r) {}
        NUMERICS_INLINE
        vector_range (vector_type &data, size_type start, size_type stop): 
            data_ (data), r_ (start, stop) {}

        NUMERICS_INLINE
        size_type start () const { 
            return r_.start (); 
        }
        NUMERICS_INLINE
        size_type size () const { 
            return r_.size (); 
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i) const {
            // One has do to this, to get the const member dispatched?!
            const vector_type &data = data_;
            return data (r_ (i));
        }
        NUMERICS_INLINE
        reference_type operator () (size_type i) {
            return data_ (r_ (i)); 
        }

        NUMERICS_INLINE
        value_type operator [] (size_type i) const { 
            // One has do to this, to get the const member dispatched?!
            const vector_type &data = data_;
            return data (r_ (i)); 
        }
        NUMERICS_INLINE
        reference_type operator [] (size_type i) { 
            return data_ (r_ (i)); 
        }

        NUMERICS_INLINE
        vector_range project (size_type start, size_type stop) {
            return vector_range (data_, r_.composite (start, stop));
        }
        NUMERICS_INLINE
        vector_range project (const range &r) {
            return vector_range (data_, r_.composite (r));
        }

        // Assignment
        NUMERICS_INLINE
        vector_range &operator = (const vector_range &vr) { 
#ifndef USE_GCC
            std::copy (vr.begin (), vr.end (), begin ());
#else
            detail::copy (vr.begin (), vr.end (), begin ());
#endif
            return *this;
        }
        NUMERICS_INLINE
        vector_range &assign_temporary (vector_range &vr) { 
            return *this = vr;
        }
        template<class AE>
        NUMERICS_INLINE
        vector_range &operator = (const vector_expression<AE> &ae) {        
            vector_assign<scalar_assign<value_type, value_type> > () (*this, vector<value_type> (ae));
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        vector_range &assign (const vector_expression<AE> &ae) {
            vector_assign<scalar_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        vector_range &operator += (const vector_expression<AE> &ae) {
            vector_assign<scalar_assign<value_type, value_type> > () (*this, vector<value_type> (*this + ae));
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        vector_range &plus_assign (const vector_expression<AE> &ae) {
            vector_assign<scalar_plus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        vector_range &operator -= (const vector_expression<AE> &ae) {
            vector_assign<scalar_assign<value_type, value_type> > () (*this, vector<value_type> (*this - ae));
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        vector_range &minus_assign (const vector_expression<AE> &ae) {
            vector_assign<scalar_minus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        vector_range &operator *= (const AT &at) {
            vector_assign_scalar<scalar_multiplies_assign<value_type, AT> > () (*this, at);
            return *this;
        }

        // Swapping
        NUMERICS_INLINE
	    void swap (vector_range &vr) {
            check<external_logic>::precondition (this != &vr);
            check<bad_size>::precondition (size () == vr.size ());
#ifndef USE_GCC
            std::swap_ranges (begin (), end (), vr.begin ());
#else
            detail::swap_ranges (begin (), end (), vr.begin ());
#endif
        }
#ifndef USE_GCC
        NUMERICS_INLINE
	    friend void swap (vector_range &vr1, vector_range &vr2) {
            vr1.swap (vr2);
        }
#endif

        class const_iterator;
        class iterator;

        // Element lookup
        NUMERICS_INLINE
        const_iterator find (size_type i) const {
            return const_iterator (*this, data_.find (start () + i));
        }
        NUMERICS_INLINE
        iterator find (size_type i) {
            return iterator (*this, data_.find (start () + i));
        }

        // Iterators simply are pointers.

        class const_iterator:
            public container_const_reference<vector_range>,
            public random_access_iterator_base<const_iterator, value_type> {
        public:
            typedef typename V::const_iterator::iterator_category iterator_category;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator (const vector_range &vr, const const_iterator_type &it):
                container_const_reference<vector_range> (vr), it_ (it) {}
            NUMERICS_INLINE
            const_iterator (const iterator &it):
                container_const_reference<vector_range> (it ()), it_ (it.it_) {}

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
                return it_.index () - (*this) ().start ();
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
            public container_reference<vector_range>,
            public random_access_iterator_base<iterator, value_type> {
        public:
            typedef typename V::iterator::iterator_category iterator_category;

            // Construction and destruction
            NUMERICS_INLINE
            iterator (vector_range &vr, const iterator_type &it):
                container_reference<vector_range> (vr), it_ (it) {}

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
                return it_.index () - (*this) ().start ();
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
        vector_type &data_;
        range r_;
    };

    // Vector based slice class
    template<class V>
    class vector_slice:
		public vector_expression<vector_slice<V> > {
    public:      
        typedef V vector_type;
        typedef typename V::size_type size_type;
        typedef typename V::difference_type difference_type;
        typedef typename V::value_type value_type;
        typedef typename V::reference_type reference_type;
        typedef vector_slice<V> const_closure_type;
        typedef vector_slice<V> closure_type;
        typedef slice::const_iterator const_iterator_type;
        typedef slice::const_iterator iterator_type;
        typedef typename proxy_traits<typename V::storage_category>::storage_category storage_category;

        // Construction and destruction
        NUMERICS_INLINE
        vector_slice (vector_type &data, const slice &s): 
            data_ (data), s_ (s) {}
        NUMERICS_INLINE
        vector_slice (vector_type &data, size_type start, size_type stride, size_type size): 
            data_ (data), s_ (start, stride, size) {}

        NUMERICS_INLINE
        size_type size () const { 
            return s_.size (); 
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i) const {
            // One has do to this, to get the const member dispatched?!
            const vector_type &data = data_;
            return data (s_ (i)); 
        }
        NUMERICS_INLINE
        reference_type operator () (size_type i) {
            return data_ (s_ (i)); 
        }

        NUMERICS_INLINE
        value_type operator [] (size_type i) const { 
            // One has do to this, to get the const member dispatched?!
            const vector_type &data = data_;
            return data (s_ (i)); 
        }
        NUMERICS_INLINE
        reference_type operator [] (size_type i) { 
            return data_ (s_ (i)); 
        }

        NUMERICS_INLINE
        vector_slice project (const range &r) {
            return vector_slice (data_, s_.composite (r));
        }
        NUMERICS_INLINE
        vector_slice project (const slice &s) {
            return vector_slice (data_, s_.composite (s));
        }

        // Assignment
        NUMERICS_INLINE
        vector_slice &operator = (const vector_slice &vs) { 
#ifndef USE_GCC
            std::copy (vs.begin (), vs.end (), begin ());
#else
            detail::copy (vs.begin (), vs.end (), begin ());
#endif
            return *this;
        }
        NUMERICS_INLINE
        vector_slice &assign_temporary (vector_slice &vs) { 
            return *this = vs;
        }
        template<class AE>
        NUMERICS_INLINE
        vector_slice &operator = (const vector_expression<AE> &ae) {        
            vector_assign<scalar_assign<value_type, value_type> > () (*this, vector<value_type> (ae));
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        vector_slice &assign (const vector_expression<AE> &ae) {
            vector_assign<scalar_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        vector_slice &operator += (const vector_expression<AE> &ae) {
            vector_assign<scalar_assign<value_type, value_type> > () (*this, vector<value_type> (*this + ae));
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        vector_slice &plus_assign (const vector_expression<AE> &ae) {
            vector_assign<scalar_plus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        vector_slice &operator -= (const vector_expression<AE> &ae) {
            vector_assign<scalar_assign<value_type, value_type> > () (*this, vector<value_type> (*this - ae));
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        vector_slice &minus_assign (const vector_expression<AE> &ae) {
            vector_assign<scalar_minus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        vector_slice &operator *= (const AT &at) {
            vector_assign_scalar<scalar_multiplies_assign<value_type, AT> > () (*this, at);
            return *this;
        }

        // Swapping
        NUMERICS_INLINE
	    void swap (vector_slice &vs) {
            check<external_logic>::precondition (this != &vs);
            check<bad_size>::precondition (size () == vs.size ());
#ifndef USE_GCC
            std::swap_ranges (begin (), end (), vs.begin ());
#else
            detail::swap_ranges (begin (), end (), vs.begin ());
#endif
        }
#ifndef USE_GCC
        NUMERICS_INLINE
	    friend void swap (vector_slice &vs1, vector_slice &vs2) {
            vs1.swap (vs2);
        }
#endif

        // Iterators simply are indexes.

        class iterator;

        class const_iterator:
            public container_const_reference<vector_type>,
            public random_access_iterator_base<const_iterator, value_type> {
        public:
            typedef std::random_access_iterator_tag iterator_category;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator (const vector_type &v, const const_iterator_type &it):
                container_const_reference<vector_type> (v), it_ (it) {}
            NUMERICS_INLINE
            const_iterator (const iterator &it):
                container_const_reference<vector_type> (it ()), it_ (it.it_) {}

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
                return (*this) () (*it_);
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                return it_.index ();
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
            return const_iterator (data_, s_.begin ());
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return const_iterator (data_, s_.end ());
        }

        class iterator:
            public container_reference<vector_type>,
            public random_access_iterator_base<iterator, value_type> {
        public:
            typedef std::random_access_iterator_tag iterator_category;

            // Construction and destruction
            NUMERICS_INLINE
            iterator (vector_type &v, const iterator_type &it):
                container_reference<vector_type> (v), it_ (it) {}

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
                return (*this) () (*it_);
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                return it_.index ();
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
            return iterator (data_, s_.begin ());
        }
        NUMERICS_INLINE
        iterator end () {
            return iterator (data_, s_.end ());
        }

    private:
        vector_type &data_;
        slice s_;
    };

}

#endif 


