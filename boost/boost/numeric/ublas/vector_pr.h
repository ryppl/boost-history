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

#ifndef NUMERICS_VECTOR_PR_H
#define NUMERICS_VECTOR_PR_H

#include "config.h"
#include "vector_et.h"

// Iterators based on ideas of Jeremy Siek

namespace numerics {

    // Vector based range class
    template<class V>
    class vector_range:
        public vector_expression<vector_range<V> > {
    public:      
        typedef V vector_type;
        typedef typename V::size_type size_type;
        typedef typename V::difference_type difference_type;
        typedef typename V::value_type value_type;
        typedef typename V::const_reference const_reference;
        typedef typename V::reference reference;
        typedef typename V::const_pointer const_pointer;
        typedef typename V::pointer pointer;
#ifdef NUMERICS_ET_CLOSURE_REFERENCE
        typedef const vector_const_reference<const vector_range<vector_type> > const_closure_type;
        typedef vector_reference<vector_range<vector_type> > closure_type;
#endif
#ifdef NUMERICS_ET_CLOSURE_VALUE
        typedef const vector_range<vector_type> const_closure_type;
        typedef vector_range<vector_type> closure_type;
#endif
        typedef typename V::const_iterator const_iterator_type;
        typedef typename V::iterator iterator_type;
        typedef typename proxy_traits<typename V::storage_category>::storage_category storage_category;

        // Construction and destruction
        NUMERICS_INLINE
        vector_range (): 
            data_ (nil_), r_ () {}
        NUMERICS_INLINE
        vector_range (vector_type &data, const range &r): 
            data_ (data), r_ (r) {}
#ifdef NUMERICS_DEPRECATED
        NUMERICS_INLINE
        vector_range (vector_type &data, size_type start, size_type stop): 
            data_ (data), r_ (start, stop) {}
#endif

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
        reference operator () (size_type i) {
            return data_ (r_ (i)); 
        }

        NUMERICS_INLINE
        value_type operator [] (size_type i) const { 
            return (*this) (i); 
        }
        NUMERICS_INLINE
        reference operator [] (size_type i) { 
            return (*this) (i); 
        }

#ifdef NUMERICS_HAS_PARTIAL_TEMPLATE_SPECIALIZATION
        NUMERICS_INLINE
        vector_range<vector_type> project (const range &r) const {
            return vector_range<vector_type> (data_, r_.composite (r));
        }
        NUMERICS_INLINE
        vector_range<vector_type> project (const range &r) {
            return vector_range<vector_type> (data_, r_.composite (r));
        }
#endif

        // Assignment
        NUMERICS_INLINE
        vector_range &operator = (const vector_range &vr) { 
            std::copy (vr.begin (), vr.end (), begin ());
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
            check (this != &vr, external_logic ());
            check (size () == vr.size (), bad_size ());
            std::swap_ranges (begin (), end (), vr.begin ());
        }
#ifndef USE_GCC
        NUMERICS_INLINE
	    friend void swap (vector_range &vr1, vector_range &vr2) {
            vr1.swap (vr2);
        }
#endif

#ifdef NUMERICS_USE_INDEXED_ITERATOR
        typedef indexed_iterator<vector_range<vector_type>, 
								 vector_type::iterator::iterator_category> iterator;
        typedef indexed_const_iterator<vector_range<vector_type>, 
									   vector_type::const_iterator::iterator_category> const_iterator;
#else
        class const_iterator;
        class iterator;
#endif

        // Element lookup
        NUMERICS_INLINE
        const_iterator lower_bound (size_type i) const {
            // One has do to this, to get the const member dispatched?!
            const vector_type &data = data_;
            const_iterator_type it (data.lower_bound (start () + i));
#ifdef NUMERICS_USE_INDEXED_ITERATOR
            return const_iterator (*this, it.index ());
#else
            return const_iterator (*this, it);
#endif
        }
        NUMERICS_INLINE
        iterator lower_bound (size_type i) {
            iterator_type it (data_.lower_bound (start () + i));
#ifdef NUMERICS_USE_INDEXED_ITERATOR
            return iterator (*this, it.index ());
#else
            return iterator (*this, it);
#endif
        }
        NUMERICS_INLINE
        const_iterator upper_bound (size_type i) const {
            // One has do to this, to get the const member dispatched?!
            const vector_type &data = data_;
            const_iterator_type it (data.upper_bound (start () + i));
#ifdef NUMERICS_USE_INDEXED_ITERATOR
            return const_iterator (*this, it.index ());
#else
            return const_iterator (*this, it);
#endif
        }
        NUMERICS_INLINE
        iterator upper_bound (size_type i) {
            iterator_type it (data_.upper_bound (start () + i));
#ifdef NUMERICS_USE_INDEXED_ITERATOR
            return iterator (*this, it.index ());
#else
            return iterator (*this, it);
#endif
        }

        // Iterators simply are pointers.

#ifndef NUMERICS_USE_INDEXED_ITERATOR
        class const_iterator:
            public container_const_reference<vector_range>,
            public random_access_iterator_base<const_iterator, value_type> {
        public:
            typedef typename V::const_iterator::iterator_category iterator_category;
#ifndef USE_MSVC
            typedef typename V::const_iterator::difference_type difference_type;
            typedef typename V::const_iterator::value_type value_type;
            typedef typename V::const_iterator::value_type reference;
            typedef typename V::const_iterator::pointer pointer;
#endif

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator ():
                container_const_reference<vector_range> (), it_ () {}
            NUMERICS_INLINE
            const_iterator (const vector_range &vr, const const_iterator_type &it):
                container_const_reference<vector_range> (vr), it_ (it) {}
#ifndef USE_ICC
            NUMERICS_INLINE
            const_iterator (const iterator &it):
                container_const_reference<vector_range> (it ()), it_ (it.it_) {}
#else
            NUMERICS_INLINE
            const_iterator (const typename vector_range::iterator &it):
                container_const_reference<vector_range> (it ()), it_ (it.it_) {}
#endif

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
                return *it_;
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                return it_.index () - (*this) ().start ();
            }

            // Assignment 
            NUMERICS_INLINE
            const_iterator &operator = (const const_iterator &it) {
                container_const_reference<vector_range>::assign (&it ());
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
#endif

        NUMERICS_INLINE
        const_iterator begin () const {
            return lower_bound (0);
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return upper_bound (size ());
        }

#ifndef NUMERICS_USE_INDEXED_ITERATOR
        class iterator:
            public container_reference<vector_range>,
            public random_access_iterator_base<iterator, value_type> {
        public:
            typedef typename V::iterator::iterator_category iterator_category;
#ifndef USE_MSVC
            typedef typename V::iterator::difference_type difference_type;
            typedef typename V::iterator::value_type value_type;
            typedef typename V::iterator::reference reference;
            typedef typename V::iterator::pointer pointer;
#endif

            // Construction and destruction
            NUMERICS_INLINE
            iterator ():
                container_reference<vector_range> (), it_ () {}
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
            reference operator * () const {
                check (index () < (*this) ().size (), bad_index ());
                return *it_;
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                return it_.index () - (*this) ().start ();
            }

            // Assignment 
            NUMERICS_INLINE
            iterator &operator = (const iterator &it) {
                container_reference<vector_range>::assign (&it ());
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
            return lower_bound (0);
        }
        NUMERICS_INLINE
        iterator end () {
            return upper_bound (size ());
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
        vector_type &data_;
        range r_;
        static vector_type nil_;
    };

    template<class V>
    vector_range<V>::vector_type vector_range<V>::nil_;

    // Projections
#ifdef NUMERICS_DEPRECATED
    template<class V>
    NUMERICS_INLINE
    vector_range<V> project (V &data, std::size_t start, std::size_t stop) {
        return vector_range<V> (data, start, stop);
    }
#endif
    template<class V>
    NUMERICS_INLINE
    vector_range<V> project (V &data, const range &r) {
        return vector_range<V> (data, r);
    }
#ifdef NUMERICS_HAS_PARTIAL_TEMPLATE_SPECIALIZATION
    template<class V>
    NUMERICS_INLINE
    vector_range<V> project (const vector_range<V> &data, const range &r) {
		NUMERICS_TRACE ("project (vector_range<V>, range)");
        return data.project (r);
    }
#endif

    // Vector based slice class
    template<class V>
    class vector_slice:
		public vector_expression<vector_slice<V> > {
    public:
        typedef V vector_type;
        typedef typename V::size_type size_type;
        typedef typename V::difference_type difference_type;
        typedef typename V::value_type value_type;
        typedef typename V::const_reference const_reference;
        typedef typename V::reference reference;
        typedef typename V::const_pointer const_pointer;
        typedef typename V::pointer pointer;
#ifdef NUMERICS_ET_CLOSURE_REFERENCE
        typedef const vector_const_reference<const vector_slice<vector_type> > const_closure_type;
        typedef vector_reference<vector_slice<vector_type> > closure_type;
#endif
#ifdef NUMERICS_ET_CLOSURE_VALUE
        typedef const vector_slice<vector_type> const_closure_type;
        typedef vector_slice<vector_type> closure_type;
#endif
        typedef slice::const_iterator const_iterator_type;
        typedef slice::const_iterator iterator_type;
        typedef typename proxy_traits<typename V::storage_category>::storage_category storage_category;

        // Construction and destruction
        NUMERICS_INLINE
        vector_slice (): 
            data_ (nil_), s_ () {}
        NUMERICS_INLINE
        vector_slice (vector_type &data, const slice &s): 
            data_ (data), s_ (s) {}
#ifdef NUMERICS_DEPRECATED
        NUMERICS_INLINE
        vector_slice (vector_type &data, size_type start, size_type stride, size_type size): 
            data_ (data), s_ (start, stride, size) {}
#endif

        NUMERICS_INLINE
        size_type start () const { 
            return s_.start (); 
        }
        NUMERICS_INLINE
        size_type stride () const { 
            return s_.stride (); 
        }
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
        reference operator () (size_type i) {
            return data_ (s_ (i)); 
        }

        NUMERICS_INLINE
        value_type operator [] (size_type i) const { 
            return (*this) (i); 
        }
        NUMERICS_INLINE
        reference operator [] (size_type i) { 
            return (*this) (i); 
        }

#ifdef NUMERICS_HAS_PARTIAL_TEMPLATE_SPECIALIZATION
        NUMERICS_INLINE
        vector_slice<vector_type> project (const range &r) const {
            return vector_slice<vector_type>  (data_, s_.composite (r));
        }
        NUMERICS_INLINE
        vector_slice<vector_type> project (const range &r) {
            return vector_slice<vector_type>  (data_, s_.composite (r));
        }
        NUMERICS_INLINE
        vector_slice<vector_type> project (const slice &s) const {
            return vector_slice<vector_type>  (data_, s_.composite (s));
        }
		NUMERICS_INLINE
        vector_slice<vector_type> project (const slice &s) {
            return vector_slice<vector_type> (data_, s_.composite (s));
        }
#endif

        // Assignment
        NUMERICS_INLINE
        vector_slice &operator = (const vector_slice &vs) { 
            std::copy (vs.begin (), vs.end (), begin ());
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
            check (this != &vs, external_logic ());
            check (size () == vs.size (), bad_size ());
            std::swap_ranges (begin (), end (), vs.begin ());
        }
#ifndef USE_GCC
        NUMERICS_INLINE
	    friend void swap (vector_slice &vs1, vector_slice &vs2) {
            vs1.swap (vs2);
        }
#endif

#ifdef NUMERICS_USE_INDEXED_ITERATOR
        typedef indexed_iterator<vector_slice<vector_type>, 
							     vector_type::iterator::iterator_category> iterator;
        typedef indexed_const_iterator<vector_slice<vector_type>,
									   vector_type::const_iterator::iterator_category> const_iterator;
#else
        class const_iterator;
        class iterator;
#endif

        // Element lookup
        NUMERICS_INLINE
        const_iterator lower_bound (size_type i) const {
#ifdef NUMERICS_USE_INDEXED_ITERATOR
            return const_iterator (*this, i);
#else
            return const_iterator (data_, s_.begin () + i);
#endif
        }
        NUMERICS_INLINE
        iterator lower_bound (size_type i) {
#ifdef NUMERICS_USE_INDEXED_ITERATOR
            return iterator (*this, i);
#else
            return iterator (data_, s_.begin () + i);
#endif
        }
        NUMERICS_INLINE
        const_iterator upper_bound (size_type i) const {
#ifdef NUMERICS_USE_INDEXED_ITERATOR
            return const_iterator (*this, i);
#else
            return const_iterator (data_, s_.begin () + i);
#endif
        }
        NUMERICS_INLINE
        iterator upper_bound (size_type i) {
#ifdef NUMERICS_USE_INDEXED_ITERATOR
            return iterator (*this, i);
#else
            return iterator (data_, s_.begin () + i);
#endif
        }

        // Iterators simply are indices.

#ifndef NUMERICS_USE_INDEXED_ITERATOR
        class const_iterator:
            public container_const_reference<vector_type>,
            public random_access_iterator_base<const_iterator, value_type> {
        public:
            typedef typename V::const_iterator::iterator_category iterator_category;
#ifndef USE_MSVC
            typedef typename V::const_iterator::difference_type difference_type;
            typedef typename V::const_iterator::value_type value_type;
            typedef typename V::const_iterator::value_type reference;
            typedef typename V::const_iterator::pointer pointer;
#endif

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator ():
                container_const_reference<vector_type> (), it_ () {}
            NUMERICS_INLINE
            const_iterator (const vector_type &v, const const_iterator_type &it):
                container_const_reference<vector_type> (v), it_ (it) {}
#ifndef USE_ICC
            NUMERICS_INLINE
            const_iterator (const iterator &it):
                container_const_reference<vector_type> (it ()), it_ (it.it_) {}
#else
            NUMERICS_INLINE
            const_iterator (const typename vector_slice::iterator &it):
                container_const_reference<vector_type> (it ()), it_ (it.it_) {}
#endif

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
                return (*this) () (*it_);
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                return it_.index ();
            }

            // Assignment 
            NUMERICS_INLINE
            const_iterator &operator = (const const_iterator &it) {
                container_const_reference<vector_type>::assign (&it ());
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
#endif

        NUMERICS_INLINE
        const_iterator begin () const {
            return lower_bound (0);
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return upper_bound (size ());
        }

#ifndef NUMERICS_USE_INDEXED_ITERATOR
        class iterator:
            public container_reference<vector_type>,
            public random_access_iterator_base<iterator, value_type> {
        public:
            typedef typename V::iterator::iterator_category iterator_category;
#ifndef USE_MSVC
            typedef typename V::iterator::difference_type difference_type;
            typedef typename V::iterator::value_type value_type;
            typedef typename V::iterator::reference reference;
            typedef typename V::iterator::pointer pointer;
#endif

            // Construction and destruction
            NUMERICS_INLINE
            iterator ():
                container_reference<vector_type> (), it_ () {}
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
            reference operator * () const {
                check (index () < (*this) ().size (), bad_index ());
                return (*this) () (*it_);
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                return it_.index ();
            }

            // Assignment 
            NUMERICS_INLINE
            iterator &operator = (const iterator &it) {
                container_reference<vector_type>::assign (&it ());
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
            return lower_bound (0);
        }
        NUMERICS_INLINE
        iterator end () {
            return upper_bound (size ());
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
        vector_type &data_;
        slice s_;
        static vector_type nil_;
    };

    template<class V>
    vector_slice<V>::vector_type vector_slice<V>::nil_;

    // Projections
#ifdef NUMERICS_HAS_PARTIAL_TEMPLATE_SPECIALIZATION
    template<class V>
    NUMERICS_INLINE
    vector_slice<V> project (const vector_slice<V> &data, const range &r) {
		NUMERICS_TRACE ("project (vector_slice<V>, range)");
        return data.project (r);
    }
#endif
    template<class V>
    NUMERICS_INLINE
    vector_slice<V> project (V &data, const slice &s) {
        return vector_slice<V> (data, s);
    }
#ifdef NUMERICS_HAS_PARTIAL_TEMPLATE_SPECIALIZATION
    template<class V>
    NUMERICS_INLINE
    vector_slice<V> project (const vector_slice<V> &data, const slice &s) {
		NUMERICS_TRACE ("project (vector_slice<V>, slice)");
        return data.project (s);
    }
#endif

}

#endif 



