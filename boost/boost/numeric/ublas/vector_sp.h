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
#include "storage.h"
#include "storage_sp.h"
#include "vector_et.h"
#include "vector.h"

// Iterators based on ideas of Jeremy Siek

namespace numerics {

    // Array based sparse vector class 
    template<class T, class A, class F>
    class sparse_vector: 
        public vector_expression<sparse_vector<T, A, F> > {
    public:      
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef const T &const_reference_type;
        typedef T &reference_type;
        typedef A array_type;
        typedef F functor_type;
        typedef sparse_vector<T, A, F> self_type;
        typedef vector_const_reference<self_type> const_closure_type;
        typedef vector_reference<self_type> closure_type;
        typedef vector_range<self_type> vector_range_type;
        typedef typename A::const_iterator const_iterator_type;
        typedef typename A::iterator iterator_type;
        typedef sparse_tag storage_category;

        // Construction and destruction
        NUMERICS_INLINE
        sparse_vector (): 
            size_ (0), non_zeroes_ (0), data_ () {}
        NUMERICS_INLINE
        sparse_vector (size_type size, size_type non_zeroes): 
            size_ (size), non_zeroes_ (non_zeroes), data_ () {}
        NUMERICS_INLINE
        sparse_vector (const sparse_vector &v): 
            size_ (v.size_), non_zeroes_ (v.non_zeroes_), data_ (v.data_) {}
        template<class AE>
        NUMERICS_INLINE
        sparse_vector (size_type non_zeroes, const vector_expression<AE> &ae): 
            size_ (ae ().size ()), non_zeroes_ (non_zeroes), data_ () { 
            vector_assign<scalar_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
        }

        // Resizing
        NUMERICS_INLINE
        void resize (size_type size, size_type non_zeroes) {
            size_ = size;
            non_zeroes_ = non_zeroes_;
            data_.resize (non_zeroes);
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
                return value_type (0);
            return (*it).second;
        }
        NUMERICS_INLINE
        reference_type operator () (size_type i) {
            return data_ [functor_type::element (i, size_)]; 
        }

        NUMERICS_INLINE
        value_type operator [] (size_type i) const { 
            const_iterator_type it (data_.find (functor_type::element (i, size_)));
            if (it == data_.end ())
                throw external_logic ();
            return (*it).second;
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
        sparse_vector &operator = (const sparse_vector &v) { 
            check<bad_size>::precondition (size_ == v.size_);
            check<bad_size>::precondition (non_zeroes_ == v.non_zeroes_);
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
            return assign_temporary (self_type (non_zeroes_, ae));
#else
            return assign (self_type (non_zeroes_, ae));
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
            return assign_temporary (self_type (non_zeroes_, *this + ae));
#else
            return assign (self_type (non_zeroes_, *this + ae));
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
            return assign_temporary (self_type (non_zeroes_, *this - ae));
#else
            return assign (self_type (non_zeroes_, *this - ae));
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
            check<external_logic>::precondition (this != &v);
            check<bad_size>::precondition (size_ == v.size_);
            check<bad_size>::precondition (non_zeroes_ == v.non_zeroes_);
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

        // Element insertion
        NUMERICS_INLINE
        void clear () {
            data_.clear ();
        }
        NUMERICS_INLINE
        void insert (size_type i, const T &t) {
            data_.insert (data_.end (), std::pair<size_type, T> (functor_type::element (i, size_), t));
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
            typedef std::bidirectional_iterator_tag iterator_category;

            // Construction and destruction
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
                check<bad_index>::precondition (index () < (*this) ().size ());
                return (*it_).second;
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                return (*it_).first;
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
            return find (size_);
        }

        class iterator:
            public container_reference<sparse_vector>,
            public bidirectional_iterator_base<iterator, value_type> {
        public:
            typedef std::bidirectional_iterator_tag iterator_category;

            // Construction and destruction
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
            reference_type operator * () {
                check<bad_index>::precondition (index () < (*this) ().size ());
                return (*it_).second;
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                return (*it_).first;
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

    private:
        size_type size_;
        size_type non_zeroes_;
        array_type data_;
    };

}

#endif 


