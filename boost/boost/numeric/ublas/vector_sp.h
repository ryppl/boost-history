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

#ifndef NUMERICS_VECTOR_SP_H
#define NUMERICS_VECTOR_SP_H

#include "config.h"
#include "storage_sp.h"
#include "vector.h"

// Iterators based on ideas of Jeremy Siek

namespace numerics {

    // Array based sparse vector class 
    template<class T, class F, class A>
    class sparse_vector: 
        public vector_expression<sparse_vector<T, F, A> > {
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
        typedef const sparse_vector<T, F, A> const_self_type;
        typedef sparse_vector<T, F, A> self_type;
        typedef const vector_const_reference<const_self_type> const_closure_type;
        typedef vector_reference<self_type> closure_type;
#ifdef NUMERICS_DEPRECATED
        typedef const vector_range<const_self_type> const_vector_range_type;
        typedef vector_range<self_type> vector_range_type;
#endif
        typedef typename A::const_iterator const_iterator_type;
        typedef typename A::iterator iterator_type;
        typedef sparse_tag storage_category;

        // Construction and destruction
        NUMERICS_INLINE
        sparse_vector (): 
            size_ (0), non_zeroes_ (0), data_ () {}
        NUMERICS_INLINE
        sparse_vector (size_type size, size_type non_zeroes = 0): 
            size_ (size), non_zeroes_ (non_zeroes), data_ () {}
        NUMERICS_INLINE
        sparse_vector (const sparse_vector &v): 
            size_ (v.size_), non_zeroes_ (v.non_zeroes_), data_ (v.data_) {}
        template<class AE>
        NUMERICS_INLINE
        sparse_vector (const vector_expression<AE> &ae, size_type non_zeroes = 0): 
            size_ (ae ().size ()), non_zeroes_ (non_zeroes), data_ () { 
            vector_assign<scalar_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
        }

        // Resizing
        NUMERICS_INLINE
        void resize (size_type size, size_type non_zeroes = 0) {
            size_ = size;
            non_zeroes_ = non_zeroes_;
        }

        NUMERICS_INLINE
        size_type size () const { 
            return size_; 
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i) const {
            const_iterator_type it (data_.find (functor_type::element (i, size_)));
            if (it == data_.end () || (*it).first != functor_type::element (i, size_))
                return value_type ();
            return (*it).second;
        }
        NUMERICS_INLINE
        reference operator () (size_type i) {
            return data_ [functor_type::element (i, size_)]; 
        }

        NUMERICS_INLINE
        value_type operator [] (size_type i) const { 
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
        sparse_vector &operator = (const sparse_vector &v) { 
            check (size_ == v.size_, bad_size ());
            check (non_zeroes_ == v.non_zeroes_, bad_size ());
            data_ = v.data_;
            return *this;
        }
        NUMERICS_INLINE
        sparse_vector &assign_temporary (sparse_vector &v) { 
            swap (v);
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        sparse_vector &operator = (const vector_expression<AE> &ae) {
#ifndef USE_GCC
            return assign_temporary (self_type (ae, non_zeroes_));
#else
            return assign (self_type (ae, non_zeroes_));
#endif
        }
        template<class AE>
        NUMERICS_INLINE
        sparse_vector &assign (const vector_expression<AE> &ae) {
            vector_assign<scalar_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        sparse_vector &operator += (const vector_expression<AE> &ae) {
#ifndef USE_GCC
            return assign_temporary (self_type (*this + ae, non_zeroes_));
#else
            return assign (self_type (*this + ae, non_zeroes_));
#endif
        }
        template<class AE>
        NUMERICS_INLINE
        sparse_vector &plus_assign (const vector_expression<AE> &ae) {
            vector_assign<scalar_plus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        sparse_vector &operator -= (const vector_expression<AE> &ae) {
#ifndef USE_GCC
            return assign_temporary (self_type (*this - ae, non_zeroes_));
#else
            return assign (self_type (*this - ae, non_zeroes_));
#endif
        }
        template<class AE>
        NUMERICS_INLINE
        sparse_vector &minus_assign (const vector_expression<AE> &ae) {
            vector_assign<scalar_minus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        sparse_vector &operator *= (const AT &at) {
            vector_assign_scalar<scalar_multiplies_assign<value_type, AT> > () (*this, at);
            return *this;
        }

        // Swapping
        NUMERICS_INLINE
	    void swap (sparse_vector &v) {
            check (this != &v, external_logic ());
            check (size_ == v.size_, bad_size ());
            check (non_zeroes_ == v.non_zeroes_, bad_size ());
            std::swap (size_, v.size_);
            std::swap (non_zeroes_, v.non_zeroes_);
            data_.swap (v.data_);
        }
#ifndef USE_GCC
        NUMERICS_INLINE
	    friend void swap (sparse_vector &v1, sparse_vector &v2) {
            v1.swap (v2);
        }
#endif

        // Element insertion and erasure
        NUMERICS_INLINE
        void insert (size_type i, const_reference t) {
#ifndef NUMERICS_USE_ET
            if (t == value_type ()) 
                return;
#endif
            check (data_.find (functor_type::element (i, size_)) == data_.end (), bad_index ());
            data_.insert (data_.end (), std::pair<size_type, value_type> (functor_type::element (i, size_), t));
        }
        NUMERICS_INLINE
        void erase (size_type i) {
            iterator_type it = data_.find (functor_type::element (i, size_));
            if (it == data_.end ())
                return;
            data_.erase (it);
        }
        NUMERICS_INLINE
        void clear () {
            data_.clear ();
        }

        class const_iterator;
        class iterator;

        // Element lookup
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        const_iterator find (size_type i) const {
            switch (functor_type::one ()) {
            case 1:
                return const_iterator (*this, data_.lower_bound (functor_type::element (i, size_)));
            case -1:
                return const_iterator (*this, data_.upper_bound (functor_type::element (i, size_)));
            default:
                throw internal_logic ();
            }
        }
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        iterator find (size_type i) {
            switch (functor_type::one ()) {
            case 1:
                return iterator (*this, data_.lower_bound (functor_type::element (i, size_)));
            case -1:
                return iterator (*this, data_.upper_bound (functor_type::element (i, size_)));
            default:
                throw internal_logic ();
            }
        }

        // Iterators simply are pointers.

        class const_iterator:
            public container_const_reference<sparse_vector>,
            public bidirectional_iterator_base<const_iterator, value_type> {
        public:
            typedef sparse_bidirectional_iterator_tag iterator_category;
#ifndef USE_MSVC
            typedef typename sparse_vector::difference_type difference_type;
            typedef typename sparse_vector::value_type value_type;
            typedef typename sparse_vector::value_type reference;
            typedef typename sparse_vector::const_pointer pointer;
#endif

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator ():
                container_const_reference<sparse_vector> (), it_ () {}
            NUMERICS_INLINE
            const_iterator (const sparse_vector &v, const const_iterator_type &it):
                container_const_reference<sparse_vector> (v), it_ (it) {}
            NUMERICS_INLINE
            const_iterator (const iterator &it):
                container_const_reference<sparse_vector> (it ()), it_ (it.it_) {}

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

            // Dereference
            NUMERICS_INLINE
            value_type operator * () const {
                check (index () < (*this) ().size (), bad_index ());
                return (*it_).second;
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                return (*it_).first;
            }

            // Assignment
            NUMERICS_INLINE
            const_iterator &operator = (const const_iterator &it) {
                container_const_reference<sparse_vector>::assign (&it ());
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

        NUMERICS_INLINE
        const_iterator begin () const {
            return find (0);
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return find (size_);
        }

        class iterator:
            public container_reference<sparse_vector>,
            public bidirectional_iterator_base<iterator, value_type> {
        public:
            typedef sparse_bidirectional_iterator_tag iterator_category;
#ifndef USE_MSVC
            typedef typename sparse_vector::difference_type difference_type;
            typedef typename sparse_vector::value_type value_type;
            typedef typename sparse_vector::reference reference;
            typedef typename sparse_vector::pointer pointer;
#endif

            // Construction and destruction
            NUMERICS_INLINE
            iterator ():
                container_reference<sparse_vector> (), it_ () {}
            NUMERICS_INLINE
            iterator (sparse_vector &v, const iterator_type &it):
                container_reference<sparse_vector> (v), it_ (it) {}

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

            // Dereference
            NUMERICS_INLINE
            reference operator * () const {
                check (index () < (*this) ().size (), bad_index ());
                return (*it_).second;
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                return (*it_).first;
            }

            // Assignment
            NUMERICS_INLINE
            iterator &operator = (const iterator &it) {
                container_reference<sparse_vector>::assign (&it ());
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
        size_type non_zeroes_;
        array_type data_;
    };

}

#endif 


