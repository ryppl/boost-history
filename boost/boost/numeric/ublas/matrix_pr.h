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

#ifndef NUMERICS_MATRIX_PR_H
#define NUMERICS_MATRIX_PR_H

#include "config.h"
#include "vector_et.h"
#include "matrix_et.h"

// Iterators based on ideas of Jeremy Siek

namespace numerics {

    // Matrix based row vector class
    template<class M>
    class matrix_row:
		public vector_expression<matrix_row<M> > {
    public:      
        typedef const M const_matrix_type;
        typedef M matrix_type;
        typedef typename M::size_type size_type;
        typedef typename M::difference_type difference_type;
        typedef typename M::value_type value_type;
        typedef typename M::const_reference const_reference;
        typedef typename M::reference reference;
        typedef typename M::const_pointer const_pointer;
        typedef typename M::pointer pointer;
#ifdef NUMERICS_ET_CLOSURE_REFERENCE
        typedef const vector_const_reference<const matrix_row<matrix_type> > const_closure_type;
        typedef vector_reference<matrix_row<matrix_type> > closure_type;
#endif
#ifdef NUMERICS_ET_CLOSURE_VALUE
        typedef const matrix_row<matrix_type> const_closure_type;
        typedef matrix_row<matrix_type> closure_type;
#endif
#ifdef NUMERICS_DEPRECATED
        typedef const vector_range<const matrix_row<const_matrix_type> > const_vector_range_type;
        typedef vector_range<matrix_row<matrix_type> > vector_range_type;
#endif
        typedef typename M::const_iterator2 const_iterator_type;
        typedef typename M::iterator2 iterator_type;
        typedef typename proxy_traits<typename M::storage_category>::storage_category storage_category;

        // Construction and destruction
        NUMERICS_INLINE
        matrix_row (): 
            data_ (nil_), i_ () {}
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
        reference operator () (size_type j) {
            return data_ (i_, j); 
        }

        NUMERICS_INLINE
        value_type operator [] (size_type j) const { 
            return (*this) (j); 
        }
        NUMERICS_INLINE
        reference operator [] (size_type j) { 
            return (*this) (j); 
        }

        // Assignment
        NUMERICS_INLINE
        matrix_row &operator = (const matrix_row &mr) { 
            std::copy (mr.begin (), mr.end (), begin ());
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
            check (this != &mr, external_logic ());
            check (size () == mr.size (), bad_size ());
            std::swap_ranges (begin (), end (), mr.begin ());
        }
#ifndef USE_GCC
        NUMERICS_INLINE
	    friend void swap (matrix_row &mr1, matrix_row &mr2) {
            mr1.swap (mr2);
        }
#endif

#if defined (NUMERICS_USE_CANONICAL_ITERATOR) || defined (NUMERICS_USE_INDEXED_ITERATOR)
        typedef indexed_iterator<matrix_row<matrix_type>, 
								 matrix_type::iterator2::iterator_category> iterator;
        typedef indexed_const_iterator<matrix_row<matrix_type>,
									   matrix_type::const_iterator2::iterator_category> const_iterator;
#else
        class const_iterator;
        class iterator;
#endif

        // Element lookup
        NUMERICS_INLINE
        const_iterator lower_bound (size_type j) const {
            // One has do to this, to get the const member dispatched?!
            const matrix_type &data = data_;
			const_iterator_type it2 (data.lower_bound2 (1, i_, j));
#ifdef NUMERICS_USE_CANONICAL_ITERATOR 
            return const_iterator (*this, it2.index ());
#else
#ifdef NUMERICS_USE_INDEXED_ITERATOR
            return const_iterator (*this, it2.index2 ());
#else
            return const_iterator (*this, it2);
#endif
#endif
        }
        NUMERICS_INLINE
        iterator lower_bound (size_type j) {
			iterator_type it2 (data_.lower_bound2 (1, i_, j));
#ifdef NUMERICS_USE_CANONICAL_ITERATOR 
            return iterator (*this, it2.index ());
#else
#ifdef NUMERICS_USE_INDEXED_ITERATOR
            return iterator (*this, it2.index2 ());
#else
            return iterator (*this, it2);
#endif
#endif
        }
        NUMERICS_INLINE
        const_iterator upper_bound (size_type j) const {
            // One has do to this, to get the const member dispatched?!
            const matrix_type &data = data_;
			const_iterator_type it2 (data.upper_bound2 (1, i_, j));
#ifdef NUMERICS_USE_CANONICAL_ITERATOR 
            return const_iterator (*this, it2.index ());
#else
#ifdef NUMERICS_USE_INDEXED_ITERATOR
            return const_iterator (*this, it2.index2 ());
#else
            return const_iterator (*this, it2);
#endif
#endif
        }
        NUMERICS_INLINE
        iterator upper_bound (size_type j) {
			iterator_type it2 (data_.upper_bound2 (1, i_, j));
#ifdef NUMERICS_USE_CANONICAL_ITERATOR 
            return iterator (*this, it2.index ());
#else
#ifdef NUMERICS_USE_INDEXED_ITERATOR
            return iterator (*this, it2.index2 ());
#else
            return iterator (*this, it2);
#endif
#endif
        }

        // Iterators simply are pointers.

#if ! defined (NUMERICS_USE_CANONICAL_ITERATOR) && ! defined (NUMERICS_USE_INDEXED_ITERATOR)
        class const_iterator:
            public container_const_reference<matrix_row>,
            public random_access_iterator_base<const_iterator, value_type> {
        public:
            typedef typename M::const_iterator2::iterator_category iterator_category;
#ifndef USE_MSVC
            typedef typename M::const_iterator2::difference_type difference_type;
            typedef typename M::const_iterator2::value_type value_type;
            typedef typename M::const_iterator2::value_type reference;
            typedef typename M::const_iterator2::pointer pointer;
#endif

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator ():
                container_const_reference<matrix_row> (), it_ () {}
            NUMERICS_INLINE
            const_iterator (const matrix_row &mr, const const_iterator_type &it):
                container_const_reference<matrix_row> (mr), it_ (it) {}
#ifndef USE_ICC
            NUMERICS_INLINE
            const_iterator (const iterator &it):
                container_const_reference<matrix_row> (it ()), it_ (it.it_) {}
#else
            NUMERICS_INLINE
            const_iterator (const typename matrix_row::iterator &it):
                container_const_reference<matrix_row> (it ()), it_ (it.it_) {}
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
                return it_.index2 ();
            }

            // Assignment 
            NUMERICS_INLINE
            const_iterator &operator = (const const_iterator &it) {
                container_const_reference<matrix_row>::assign (&it ());
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

#if ! defined (NUMERICS_USE_CANONICAL_ITERATOR) && ! defined (NUMERICS_USE_INDEXED_ITERATOR)
        class iterator:
            public container_reference<matrix_row>,
            public random_access_iterator_base<iterator, value_type> {
        public:
            typedef typename M::iterator2::iterator_category iterator_category;
#ifndef USE_MSVC
            typedef typename M::iterator2::difference_type difference_type;
            typedef typename M::iterator2::value_type value_type;
            typedef typename M::iterator2::reference reference;
            typedef typename M::iterator2::pointer pointer;
#endif

            // Construction and destruction
            NUMERICS_INLINE
            iterator ():
                container_reference<matrix_row> (), it_ () {}
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
                return it_.index2 ();
            }

            // Assignment 
            NUMERICS_INLINE
            iterator &operator = (const iterator &it) {
                container_reference<matrix_row>::assign (&it ());
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
        matrix_type &data_;
        size_type i_;
        static matrix_type nil_;
    };

    template<class M>
    matrix_row<M>::matrix_type matrix_row<M>::nil_;

    // Projections
    template<class M>
    NUMERICS_INLINE
    matrix_row<M> row (M &data, std::size_t i) {
        return matrix_row<M> (data, i);
    }

    // Matrix row iterator
    template<class M, class I>
    class matrix_row_iterator:
        public container_reference<M>, 
        public random_access_iterator_base<matrix_row_iterator<M, I>, 
                                           matrix_row<M>,
                                           typename M::difference_type> {
    public:
        typedef M matrix_type;
        typedef I iterator_category;
        typedef typename matrix_type::size_type size_type;
        typedef typename matrix_type::difference_type difference_type;
        typedef matrix_row<matrix_type> value_type;
        typedef matrix_row<matrix_type> reference;
        typedef value_type *pointer;

        // Construction and destruction
        NUMERICS_INLINE
        matrix_row_iterator ():
            container_reference<matrix_type> (), it_ () {}
        NUMERICS_INLINE
        matrix_row_iterator (matrix_type &m, size_type it):
            container_reference<matrix_type> (m), it_ (it) {}

        // Arithmetic
        NUMERICS_INLINE
        matrix_row_iterator &operator ++ () {
            ++ it_;
            return *this;
        }
        NUMERICS_INLINE
        matrix_row_iterator &operator -- () {
            -- it_;
            return *this;
        }
        NUMERICS_INLINE
        matrix_row_iterator &operator += (difference_type n) {
            it_ += n;
            return *this;
        }
        NUMERICS_INLINE
        matrix_row_iterator &operator -= (difference_type n) {
            it_ -= n;
            return *this;
        }
        NUMERICS_INLINE
        difference_type operator - (const matrix_row_iterator &it) const {
            return it_ - it.it_;
        }

        // Dereference
        NUMERICS_INLINE
        reference operator * () const {
            check (index () < (*this) ().size1 (), bad_index ());
            return row ((*this) (), it_);
        }

        // Index
        NUMERICS_INLINE
        size_type index () const {
            return it_;
        }

        // Assignment 
        NUMERICS_INLINE
        matrix_row_iterator &operator = (const matrix_row_iterator &it) {
            assign (&it ());
            it_ = it.it_;
            return *this;
        }

        // Comparison
        NUMERICS_INLINE
        bool operator == (const matrix_row_iterator &it) const {
            check (&(*this) () == &it (), external_logic ());
            return it_ == it.it_;
        }

    private:
        size_type it_;
    };

#ifdef USE_MSVC
    template<class M, class I>
    NUMERICS_INLINE
    matrix_row_iterator<M, I> operator ++ (const matrix_row_iterator<M, I> &it, int) {
        matrix_row_iterator<M, I> tmp (it);
        ++ tmp;
        return tmp;
    }
    template<class M, class I>
    NUMERICS_INLINE
    matrix_row_iterator<M, I> operator -- (const matrix_row_iterator<M, I> &it, int) {
        matrix_row_iterator<M, I> tmp (it);
        -- tmp;
        return tmp;
    }
    template<class M, class I>
    NUMERICS_INLINE
    matrix_row_iterator<M, I> operator + (const matrix_row_iterator<M, I> &it, std::ptrdiff_t n) {
        matrix_row_iterator<M, I> tmp (it);
        return tmp += n;
    }
    template<class M, class I>
    NUMERICS_INLINE
    matrix_row_iterator<M, I> operator - (const matrix_row_iterator<M, I> &it, std::ptrdiff_t n) {
        matrix_row_iterator<M, I> tmp (it);
        return tmp -= n;
    }
#endif

    template<class M, class I>
    class matrix_row_const_iterator:
        public container_const_reference<M>, 
        public random_access_iterator_base<matrix_row_const_iterator<M, I>, 
                                           matrix_row<M>,
                                           typename M::difference_type> {
    public:
        typedef M matrix_type;
        typedef I iterator_category;
        typedef typename matrix_type::size_type size_type;
        typedef typename matrix_type::difference_type difference_type;
        typedef matrix_row<matrix_type> value_type;
        typedef const matrix_row<const matrix_type> const_reference;
        typedef matrix_row<matrix_type> reference;
        typedef value_type *pointer;
        typedef matrix_row_iterator<matrix_type, iterator_category> iterator_type;

        // Construction and destruction
        NUMERICS_INLINE
        matrix_row_const_iterator ():
            container_const_reference<matrix_type> (), it_ () {}
        NUMERICS_INLINE
        matrix_row_const_iterator (const matrix_type &c, size_type it):
            container_const_reference<matrix_type> (c), it_ (it) {}
        NUMERICS_INLINE
        matrix_row_const_iterator (const iterator_type &it):
            container_const_reference<matrix_type> (it ()), it_ (it.index ()) {}

        // Arithmetic
        NUMERICS_INLINE
        matrix_row_const_iterator &operator ++ () {
            ++ it_;
            return *this;
        }
        NUMERICS_INLINE
        matrix_row_const_iterator &operator -- () {
            -- it_;
            return *this;
        }
        NUMERICS_INLINE
        matrix_row_const_iterator &operator += (difference_type n) {
            it_ += n;
            return *this;
        }
        NUMERICS_INLINE
        matrix_row_const_iterator &operator -= (difference_type n) {
            it_ -= n;
            return *this;
        }
        NUMERICS_INLINE
        difference_type operator - (const matrix_row_const_iterator &it) const {
            return it_ - it.it_;
        }

        // Dereference
        NUMERICS_INLINE
        const_reference operator * () const {
//        value_type operator * () const {
            check (index () < (*this) ().size1 (), bad_index ());
            return row ((*this) (), it_);
        }

        // Index
        NUMERICS_INLINE
        size_type index () const {
            return it_;
        }

        // Assignment 
        NUMERICS_INLINE
        matrix_row_const_iterator &operator = (const matrix_row_const_iterator &it) {
            assign (&it ());
            it_ = it.it_;
            return *this;
        }

        // Comparison
        NUMERICS_INLINE
        bool operator == (const matrix_row_const_iterator &it) const {
            check (&(*this) () == &it (), external_logic ());
            return it_ == it.it_;
        }

    private:
        size_type it_;

        friend class matrix_row_iterator<matrix_type, iterator_category>;
    };

#ifdef USE_MSVC
    template<class M, class I>
    NUMERICS_INLINE
    matrix_row_const_iterator<M, I> operator ++ (const matrix_row_const_iterator<M, I> &it, int) {
        matrix_row_const_iterator<M, I> tmp (it);
        ++ tmp;
        return tmp;
    }
    template<class M, class I>
    NUMERICS_INLINE
    matrix_row_const_iterator<M, I> operator -- (const matrix_row_const_iterator<M, I> &it, int) {
        matrix_row_const_iterator<M, I> tmp (it);
        -- tmp;
        return tmp;
    }
    template<class M, class I>
    NUMERICS_INLINE
    matrix_row_const_iterator<M, I> operator + (const matrix_row_const_iterator<M, I> &it, std::ptrdiff_t n) {
        matrix_row_const_iterator<M, I> tmp (it);
        return tmp += n;
    }
    template<class M, class I>
    NUMERICS_INLINE
    matrix_row_const_iterator<M, I> operator - (const matrix_row_const_iterator<M, I> &it, std::ptrdiff_t n) {
        matrix_row_const_iterator<M, I> tmp (it);
        return tmp -= n;
    }
#endif

    // Matrix based column vector class
    template<class M>
    class matrix_column:
		public vector_expression<matrix_column<M> > {
    public:      
        typedef const M const_matrix_type;
        typedef M matrix_type;
        typedef typename M::size_type size_type;
        typedef typename M::difference_type difference_type;
        typedef typename M::value_type value_type;
        typedef typename M::const_reference const_reference;
        typedef typename M::reference reference;
        typedef typename M::const_pointer const_pointer;
        typedef typename M::pointer pointer;
#ifdef NUMERICS_ET_CLOSURE_REFERENCE
        typedef const vector_const_reference<const matrix_column<matrix_type> > const_closure_type;
        typedef vector_reference<matrix_column<matrix_type> > closure_type;
#endif
#ifdef NUMERICS_ET_CLOSURE_VALUE
        typedef const matrix_column<matrix_type> const_closure_type;
        typedef matrix_column<matrix_type> closure_type;
#endif
#ifdef NUMERICS_DEPRECATED
        typedef const vector_range<const matrix_column<const_matrix_type> > const_vector_range_type;
        typedef vector_range<matrix_column<matrix_type> > vector_range_type;
#endif
        typedef typename M::const_iterator1 const_iterator_type;
        typedef typename M::iterator1 iterator_type;
        typedef typename proxy_traits<typename M::storage_category>::storage_category storage_category;

        // Construction and destruction
        NUMERICS_INLINE
        matrix_column (): 
            data_ (nil_), j_ () {}
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
        reference operator () (size_type i) {
            return data_ (i, j_); 
        }

        NUMERICS_INLINE
        value_type operator [] (size_type i) const { 
            return (*this) (i); 
        }
        NUMERICS_INLINE
        reference operator [] (size_type i) { 
            return (*this) (i); 
        }

        // Assignment
        NUMERICS_INLINE
        matrix_column &operator = (const matrix_column &mc) { 
            std::copy (mc.begin (), mc.end (), begin ());
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
            check (this != &mc, external_logic ());
            check (size () == mc.size (), bad_size ());
            std::swap_ranges (begin (), end (), mc.begin ());
        }
#ifndef USE_GCC
        NUMERICS_INLINE
	    friend void swap (matrix_column &mc1, matrix_column &mc2) {
            mc1.swap (mc2);
        }
#endif

#if defined (NUMERICS_USE_CANONICAL_ITERATOR) || defined (NUMERICS_USE_INDEXED_ITERATOR)
        typedef indexed_iterator<matrix_column<matrix_type>, 
								 matrix_type::iterator1::iterator_category> iterator;
        typedef indexed_const_iterator<matrix_column<matrix_type>,
									   matrix_type::const_iterator1::iterator_category> const_iterator;
#else
        class const_iterator;
        class iterator;
#endif

        // Element lookup
        NUMERICS_INLINE
        const_iterator lower_bound (size_type i) const {
            // One has do to this, to get the const member dispatched?!
            const matrix_type &data = data_;
			const_iterator_type it1 (data.lower_bound1 (1, i, j_));
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return const_iterator (*this, it1.index ());
#else
#ifdef NUMERICS_USE_INDEXED_ITERATOR
            return const_iterator (*this, it1.index1 ());
#else
            return const_iterator (*this, it1);
#endif
#endif
        }
        NUMERICS_INLINE
        iterator lower_bound (size_type i) {
			iterator_type it1 (data_.lower_bound1 (1, i, j_));
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return iterator (*this, it1.index ());
#else
#ifdef NUMERICS_USE_INDEXED_ITERATOR
            return iterator (*this, it1.index1 ());
#else
            return iterator (*this, it1);
#endif
#endif
        }
        NUMERICS_INLINE
        const_iterator upper_bound (size_type i) const {
            // One has do to this, to get the const member dispatched?!
            const matrix_type &data = data_;
			const_iterator_type it1 (data.upper_bound1 (1, i, j_));
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return const_iterator (*this, it1.index ());
#else
#ifdef NUMERICS_USE_INDEXED_ITERATOR
            return const_iterator (*this, it1.index1 ());
#else
            return const_iterator (*this, it1);
#endif
#endif
        }
        NUMERICS_INLINE
        iterator upper_bound (size_type i) {
			iterator_type it1 (data_.upper_bound1 (1, i, j_));
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return iterator (*this, it1.index ());
#else
#ifdef NUMERICS_USE_INDEXED_ITERATOR
            return iterator (*this, it1.index1 ());
#else
            return iterator (*this, it1);
#endif
#endif
        }

        // Iterators simply are pointers.

#if ! defined (NUMERICS_USE_CANONICAL_ITERATOR) && ! defined (NUMERICS_USE_INDEXED_ITERATOR)
        class const_iterator:
            public container_const_reference<matrix_column>,
            public random_access_iterator_base<const_iterator, value_type> {
        public:
            typedef typename M::const_iterator1::iterator_category iterator_category;
#ifndef USE_MSVC
            typedef typename M::const_iterator1::difference_type difference_type;
            typedef typename M::const_iterator1::value_type value_type;
            typedef typename M::const_iterator1::value_type reference;
            typedef typename M::const_iterator1::pointer pointer;
#endif

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator ():
                container_const_reference<matrix_column> (), it_ () {}
            NUMERICS_INLINE
            const_iterator (const matrix_column &mc, const const_iterator_type &it):
                container_const_reference<matrix_column> (mc), it_ (it) {}
#ifndef USE_ICC
            NUMERICS_INLINE
            const_iterator (const iterator &it):
                container_const_reference<matrix_column> (it ()), it_ (it.it_) {}
#else
            NUMERICS_INLINE
            const_iterator (const typename matrix_column::iterator &it):
                container_const_reference<matrix_column> (it ()), it_ (it.it_) {}
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
                return it_.index1 ();
            }

            // Assignment 
            NUMERICS_INLINE
            const_iterator &operator = (const const_iterator &it) {
                container_const_reference<matrix_column>::assign (&it ());
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

#if ! defined (NUMERICS_USE_CANONICAL_ITERATOR) && ! defined (NUMERICS_USE_INDEXED_ITERATOR)
        class iterator:
            public container_reference<matrix_column>,
            public random_access_iterator_base<iterator, value_type> {
        public:
            typedef typename M::iterator1::iterator_category iterator_category;
#ifndef USE_MSVC
            typedef typename M::iterator1::difference_type difference_type;
            typedef typename M::iterator1::value_type value_type;
            typedef typename M::iterator1::reference reference;
            typedef typename M::iterator1::pointer pointer;
#endif

            // Construction and destruction
            NUMERICS_INLINE
            iterator ():
                container_reference<matrix_column> (), it_ () {}
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
            reference operator * () const {
                check (index () < (*this) ().size (), bad_index ());
                return *it_;
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                return it_.index1 ();
            }

            // Assignment 
            NUMERICS_INLINE
            iterator &operator = (const iterator &it) {
                container_reference<matrix_column>::assign (&it ());
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
        matrix_type &data_;
        size_type j_;
        static matrix_type nil_;
    };

    template<class M>
    matrix_column<M>::matrix_type matrix_column<M>::nil_;

    // Projections
    template<class M>
    NUMERICS_INLINE
    matrix_column<M> column (M &data, std::size_t j) {
        return matrix_column<M> (data, j);
    }

    // Matrix column iterator
    template<class M, class I>
    class matrix_column_iterator:
        public container_reference<M>, 
        public random_access_iterator_base<matrix_column_iterator<M, I>, 
                                           matrix_column<M>,
                                           typename M::difference_type> {
    public:
        typedef M matrix_type;
        typedef I iterator_category;
        typedef typename matrix_type::size_type size_type;
        typedef typename matrix_type::difference_type difference_type;
        typedef matrix_column<matrix_type> value_type;
        typedef matrix_column<matrix_type> reference;
        typedef value_type *pointer;

        // Construction and destruction
        NUMERICS_INLINE
        matrix_column_iterator ():
            container_reference<matrix_type> (), it_ () {}
        NUMERICS_INLINE
        matrix_column_iterator (matrix_type &m, size_type it):
            container_reference<matrix_type> (m), it_ (it) {}

        // Arithmetic
        NUMERICS_INLINE
        matrix_column_iterator &operator ++ () {
            ++ it_;
            return *this;
        }
        NUMERICS_INLINE
        matrix_column_iterator &operator -- () {
            -- it_;
            return *this;
        }
        NUMERICS_INLINE
        matrix_column_iterator &operator += (difference_type n) {
            it_ += n;
            return *this;
        }
        NUMERICS_INLINE
        matrix_column_iterator &operator -= (difference_type n) {
            it_ -= n;
            return *this;
        }
        NUMERICS_INLINE
        difference_type operator - (const matrix_column_iterator &it) const {
            return it_ - it.it_;
        }

        // Dereference
        NUMERICS_INLINE
        reference operator * () const {
            check (index () < (*this) ().size2 (), bad_index ());
            return column ((*this) (), it_);
        }

        // Index
        NUMERICS_INLINE
        size_type index () const {
            return it_;
        }

        // Assignment 
        NUMERICS_INLINE
        matrix_column_iterator &operator = (const matrix_column_iterator &it) {
            assign (&it ());
            it_ = it.it_;
            return *this;
        }

        // Comparison
        NUMERICS_INLINE
        bool operator == (const matrix_column_iterator &it) const {
            check (&(*this) () == &it (), external_logic ());
            return it_ == it.it_;
        }

    private:
        size_type it_;
    };

#ifdef USE_MSVC
    template<class M, class I>
    NUMERICS_INLINE
    matrix_column_iterator<M, I> operator ++ (const matrix_column_iterator<M, I> &it, int) {
        matrix_column_iterator<M, I> tmp (it);
        ++ tmp;
        return tmp;
    }
    template<class M, class I>
    NUMERICS_INLINE
    matrix_column_iterator<M, I> operator -- (const matrix_column_iterator<M, I> &it, int) {
        matrix_column_iterator<M, I> tmp (it);
        -- tmp;
        return tmp;
    }
    template<class M, class I>
    NUMERICS_INLINE
    matrix_column_iterator<M, I> operator + (const matrix_column_iterator<M, I> &it, std::ptrdiff_t n) {
        matrix_column_iterator<M, I> tmp (it);
        return tmp += n;
    }
    template<class M, class I>
    NUMERICS_INLINE
    matrix_column_iterator<M, I> operator - (const matrix_column_iterator<M, I> &it, std::ptrdiff_t n) {
        matrix_column_iterator<M, I> tmp (it);
        return tmp -= n;
    }
#endif

    template<class M, class I>
    class matrix_column_const_iterator:
        public container_const_reference<M>, 
        public random_access_iterator_base<matrix_column_const_iterator<M, I>, 
                                           matrix_column<M>,
                                           typename M::difference_type> {
    public:
        typedef M matrix_type;
        typedef I iterator_category;
        typedef typename matrix_type::size_type size_type;
        typedef typename matrix_type::difference_type difference_type;
        typedef matrix_column<matrix_type> value_type;
        typedef const matrix_column<const matrix_type> const_reference;
        typedef matrix_column<matrix_type> reference;
        typedef value_type *pointer;
        typedef matrix_column_iterator<matrix_type, iterator_category> iterator_type;

        // Construction and destruction
        NUMERICS_INLINE
        matrix_column_const_iterator ():
            container_const_reference<matrix_type> (), it_ () {}
        NUMERICS_INLINE
        matrix_column_const_iterator (const matrix_type &c, size_type it):
            container_const_reference<matrix_type> (c), it_ (it) {}
        NUMERICS_INLINE 
        matrix_column_const_iterator (const iterator_type &it):
            container_const_reference<matrix_type> (it ()), it_ (it.index ()) {}

        // Arithmetic
        NUMERICS_INLINE
        matrix_column_const_iterator &operator ++ () {
            ++ it_;
            return *this;
        }
        NUMERICS_INLINE
        matrix_column_const_iterator &operator -- () {
            -- it_;
            return *this;
        }
        NUMERICS_INLINE
        matrix_column_const_iterator &operator += (difference_type n) {
            it_ += n;
            return *this;
        }
        NUMERICS_INLINE
        matrix_column_const_iterator &operator -= (difference_type n) {
            it_ -= n;
            return *this;
        }
        NUMERICS_INLINE
        difference_type operator - (const matrix_column_const_iterator &it) const {
            return it_ - it.it_;
        }

        // Dereference
        NUMERICS_INLINE
        const_reference operator * () const {
//        value_type operator * () const {
            check (index () < (*this) ().size2 (), bad_index ());
            return column ((*this) (), it_);
        }

        // Index
        NUMERICS_INLINE
        size_type index () const {
            return it_;
        }

        // Assignment 
        NUMERICS_INLINE
        matrix_column_const_iterator &operator = (const matrix_column_const_iterator &it) {
            assign (&it ());
            it_ = it.it_;
            return *this;
        }

        // Comparison
        NUMERICS_INLINE
        bool operator == (const matrix_column_const_iterator &it) const {
            check (&(*this) () == &it (), external_logic ());
            return it_ == it.it_;
        }

    private:
        size_type it_;

        friend class matrix_column_iterator<matrix_type, iterator_category>;
    };

#ifdef USE_MSVC
    template<class M, class I>
    NUMERICS_INLINE
    matrix_column_const_iterator<M, I> operator ++ (const matrix_column_const_iterator<M, I> &it, int) {
        matrix_column_const_iterator<M, I> tmp (it);
        ++ tmp;
        return tmp;
    }
    template<class M, class I>
    NUMERICS_INLINE
    matrix_column_const_iterator<M, I> operator -- (const matrix_column_const_iterator<M, I> &it, int) {
        matrix_column_const_iterator<M, I> tmp (it);
        -- tmp;
        return tmp;
    }
    template<class M, class I>
    NUMERICS_INLINE
    matrix_column_const_iterator<M, I> operator + (const matrix_column_const_iterator<M, I> &it, std::ptrdiff_t n) {
        matrix_column_const_iterator<M, I> tmp (it);
        return tmp += n;
    }
    template<class M, class I>
    NUMERICS_INLINE
    matrix_column_const_iterator<M, I> operator - (const matrix_column_const_iterator<M, I> &it, std::ptrdiff_t n) {
        matrix_column_const_iterator<M, I> tmp (it);
        return tmp -= n;
    }
#endif

    // Matrix based vector range class
    template<class M>
    class matrix_vector_range:
		public vector_expression<matrix_vector_range<M> > {
    public:      
        typedef const M const_matrix_type;
        typedef M matrix_type;
        typedef typename M::size_type size_type;
        typedef typename M::difference_type difference_type;
        typedef typename M::value_type value_type;
        typedef typename M::const_reference const_reference;
        typedef typename M::reference reference;
        typedef typename M::const_pointer const_pointer;
        typedef typename M::pointer pointer;
#ifdef NUMERICS_ET_CLOSURE_REFERENCE
        typedef const vector_const_reference<const matrix_vector_range<matrix_type> > const_closure_type;
        typedef vector_reference<matrix_vector_range<matrix_type> > closure_type;
#endif
#ifdef NUMERICS_ET_CLOSURE_VALUE
        typedef const matrix_vector_range<matrix_type> const_closure_type;
        typedef matrix_vector_range<matrix_type> closure_type;
#endif
        typedef range::const_iterator const_iterator1_type;
        typedef range::const_iterator iterator1_type;
        typedef range::const_iterator const_iterator2_type;
        typedef range::const_iterator iterator2_type;
        typedef typename proxy_traits<typename M::storage_category>::storage_category storage_category;

        // Construction and destruction
        NUMERICS_INLINE
        matrix_vector_range (): 
            data_ (nil_), r1_ (), r2_ () {}
        NUMERICS_INLINE
        matrix_vector_range (matrix_type &data, const range &r1, const range &r2): 
            data_ (data), r1_ (r1), r2_ (r2) {}
#ifdef NUMERICS_DEPRECATED
        NUMERICS_INLINE
        matrix_vector_range (matrix_type &data, size_type start1, size_type stop1, size_type start2, size_type stop2): 
            data_ (data), r1_ (start1, stop1), r2_ (start2, stop2) {}
#endif

        NUMERICS_INLINE
        size_type size () const { 
            return common (r1_.size (), r2_.size ()); 
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i) const {
            // One has do to this, to get the const member dispatched?!
            const matrix_type &data = data_;
            return data (r1_ (i), r2_ (i)); 
        }
        NUMERICS_INLINE
        reference operator () (size_type i) {
            return data_ (r1_ (i), r2_ (i)); 
        }

        NUMERICS_INLINE
        value_type operator [] (size_type i) const { 
            return (*this) (i); 
        }
        NUMERICS_INLINE
        reference operator [] (size_type i) { 
            return (*this) (i); 
        }

        // Assignment
        NUMERICS_INLINE
        matrix_vector_range &operator = (const matrix_vector_range &mvr) { 
            std::copy (mvr.begin (), mvr.end (), begin ());
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
            check (this != &mvr, external_logic ());
            check (size () == mvr.size (), bad_size ());
            std::swap_ranges (begin (), end (), mvr.begin ());
        }
#ifndef USE_GCC
        NUMERICS_INLINE
	    friend void swap (matrix_vector_range &mvr1, matrix_vector_range &mvr2) {
            mvr1.swap (mvr2);
        }
#endif

        class const_iterator;
        class iterator;

        // Element lookup
        NUMERICS_INLINE
        const_iterator lower_bound (size_type i) const {
            return const_iterator (data_, r1_.begin () + i, r2_.begin () + i);
        }
        NUMERICS_INLINE
        iterator lower_bound (size_type i) {
            return iterator (data_, r1_.begin () + i, r2_.begin () + i);
        }
        NUMERICS_INLINE
        const_iterator upper_bound (size_type i) const {
            return const_iterator (data_, r1_.begin () + i, r2_.begin () + i);
        }
        NUMERICS_INLINE
        iterator upper_bound (size_type i) {
            return iterator (data_, r1_.begin () + i, r2_.begin () + i);
        }

        // Iterators simply are indices.

        class const_iterator:
            public container_const_reference<matrix_type>,
            public random_access_iterator_base<const_iterator, value_type> {
        public:
            typedef typename restrict_traits<typename M::const_iterator1::iterator_category,
                                             typename M::const_iterator2::iterator_category>::iterator_category iterator_category; 
#ifndef USE_MSVC
            typedef typename matrix_type::difference_type difference_type;
            typedef typename matrix_type::value_type value_type;
            typedef typename matrix_type::value_type reference;
            typedef typename matrix_type::const_pointer pointer;
#endif

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator ():
                container_const_reference<matrix_type> (), it1_ (), it2_ () {}
            NUMERICS_INLINE
            const_iterator (const matrix_type &m, const const_iterator1_type &it1, const const_iterator2_type &it2):
                container_const_reference<matrix_type> (m), it1_ (it1), it2_ (it2) {}
#ifndef USE_ICC
            NUMERICS_INLINE
            const_iterator (const iterator &it):
                container_const_reference<matrix_type> (it ()), it1_ (it.it1_), it2_ (it.it2_) {}
#else
            NUMERICS_INLINE
            const_iterator (const typename matrix_vector_range::iterator &it):
                container_const_reference<matrix_type> (it ()), it1_ (it.it1_), it2_ (it.it2_) {}
#endif

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

            // Assignment 
            NUMERICS_INLINE
            const_iterator &operator = (const const_iterator &it) {
                container_const_reference<matrix_type>::assign (&it ());
                it1_ = it.it1_;
                it2_ = it.it2_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it1_ == it.it1_ && it2_ == it.it2_;
            }

        private:
            const_iterator1_type it1_;
            const_iterator2_type it2_;
        };

        NUMERICS_INLINE
        const_iterator begin () const {
            return lower_bound (0); 
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return upper_bound (size ()); 
        }

        class iterator:
            public container_reference<matrix_type>,
            public random_access_iterator_base<iterator, value_type> {
        public:
            typedef typename restrict_traits<typename M::iterator1::iterator_category,
                                             typename M::iterator2::iterator_category>::iterator_category iterator_category; 
#ifndef USE_MSVC
            typedef typename matrix_type::difference_type difference_type;
            typedef typename matrix_type::value_type value_type;
            typedef typename matrix_type::reference reference;
            typedef typename matrix_type::pointer pointer;
#endif

            // Construction and destruction
            NUMERICS_INLINE
            iterator ():
                container_reference<matrix_type> (), it1_ (), it2_ () {}
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
            reference operator * () const {
                return (*this) () (*it1_, *it2_); 
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                return common (it1_.index (), it2_.index ());
            }

            // Assignment 
            NUMERICS_INLINE
            iterator &operator = (const iterator &it) {
                container_reference<matrix_type>::assign (&it ());
                it1_ = it.it1_;
                it2_ = it.it2_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const iterator &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it1_ == it.it1_ && it2_ == it.it2_;
            }

        private:
            iterator1_type it1_;
            iterator2_type it2_;

            friend class const_iterator;
        };

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
        matrix_type &data_;
        range r1_;
        range r2_;
        static matrix_type nil_;
    };

    template<class M>
    matrix_vector_range<M>::matrix_type matrix_vector_range<M>::nil_;

    // Matrix based vector slice class
    template<class M>
    class matrix_vector_slice:
		public vector_expression<matrix_vector_slice<M> > {
    public:      
        typedef const M const_matrix_type;
        typedef M matrix_type;
        typedef typename M::size_type size_type;
        typedef typename M::difference_type difference_type;
        typedef typename M::value_type value_type;
        typedef typename M::const_reference const_reference;
        typedef typename M::reference reference;
        typedef typename M::const_pointer const_pointer;
        typedef typename M::pointer pointer;
#ifdef NUMERICS_ET_CLOSURE_REFERENCE
        typedef const vector_const_reference<const matrix_vector_slice<matrix_type> > const_closure_type;
        typedef vector_reference<matrix_vector_slice<matrix_type> > closure_type;
#endif
#ifdef NUMERICS_ET_CLOSURE_VALUE
        typedef const matrix_vector_slice<matrix_type> const_closure_type;
        typedef matrix_vector_slice<matrix_type> closure_type;
#endif
        typedef slice::const_iterator const_iterator1_type;
        typedef slice::const_iterator iterator1_type;
        typedef slice::const_iterator const_iterator2_type;
        typedef slice::const_iterator iterator2_type;
        typedef typename proxy_traits<typename M::storage_category>::storage_category storage_category;

        // Construction and destruction
        NUMERICS_INLINE
        matrix_vector_slice (): 
            data_ (nil_), s1_ (), s2_ () {}
        NUMERICS_INLINE
        matrix_vector_slice (matrix_type &data, const slice &s1, const slice &s2): 
            data_ (data), s1_ (s1), s2_ (s2) {}
#ifdef NUMERICS_DEPRECATED
        NUMERICS_INLINE
        matrix_vector_slice (matrix_type &data, size_type start1, size_type stride1, size_type size1, size_type start2, size_type stride2, size_type size2): 
            data_ (data), s1_ (start1, stride1, size1), s2_ (start2, stride2, size2) {}
#endif

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
        reference operator () (size_type i) {
            return data_ (s1_ (i), s2_ (i)); 
        }

        NUMERICS_INLINE
        value_type operator [] (size_type i) const { 
            return (*this) (i); 
        }
        NUMERICS_INLINE
        reference operator [] (size_type i) { 
            return (*this) (i); 
        }

        // Assignment
        NUMERICS_INLINE
        matrix_vector_slice &operator = (const matrix_vector_slice &mvs) { 
            std::copy (mvs.begin (), mvs.end (), begin ());
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
            check (this != &mvs, external_logic ());
            check (size () == mvs.size (), bad_size ());
            std::swap_ranges (begin (), end (), mvs.begin ());
        }
#ifndef USE_GCC
        NUMERICS_INLINE
	    friend void swap (matrix_vector_slice &mvs1, matrix_vector_slice &mvs2) {
            mvs1.swap (mvs2);
        }
#endif

        class const_iterator;
        class iterator;

        // Element lookup
        NUMERICS_INLINE
        const_iterator lower_bound (size_type i) const {
            return const_iterator (data_, s1_.begin () + i, s2_.begin () + i);
        }
        NUMERICS_INLINE
        iterator lower_bound (size_type i) {
            return iterator (data_, s1_.begin () + i, s2_.begin () + i);
        }
        NUMERICS_INLINE
        const_iterator upper_bound (size_type i) const {
            return const_iterator (data_, s1_.begin () + i, s2_.begin () + i);
        }
        NUMERICS_INLINE
        iterator upper_bound (size_type i) {
            return iterator (data_, s1_.begin () + i, s2_.begin () + i);
        }

        // Iterators simply are indices.

        class const_iterator:
            public container_const_reference<matrix_type>,
            public random_access_iterator_base<const_iterator, value_type> {
        public:
            typedef typename restrict_traits<typename M::const_iterator1::iterator_category,
                                             typename M::const_iterator2::iterator_category>::iterator_category iterator_category; 
#ifndef USE_MSVC
            typedef typename matrix_type::difference_type difference_type;
            typedef typename matrix_type::value_type value_type;
            typedef typename matrix_type::value_type reference;
            typedef typename matrix_type::const_pointer pointer;
#endif

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator ():
                container_const_reference<matrix_type> (), it1_ (), it2_ () {}
            NUMERICS_INLINE
            const_iterator (const matrix_type &m, const const_iterator1_type &it1, const const_iterator2_type &it2):
                container_const_reference<matrix_type> (m), it1_ (it1), it2_ (it2) {}
#ifndef USE_ICC
            NUMERICS_INLINE
            const_iterator (const iterator &it):
                container_const_reference<matrix_type> (it ()), it1_ (it.it1_), it2_ (it.it2_) {}
#else
            NUMERICS_INLINE
            const_iterator (const typename matrix_vector_slice::iterator &it):
                container_const_reference<matrix_type> (it ()), it1_ (it.it1_), it2_ (it.it2_) {}
#endif

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

            // Assignment 
            NUMERICS_INLINE
            const_iterator &operator = (const const_iterator &it) {
                container_const_reference<matrix_type>::assign (&it ());
                it1_ = it.it1_;
                it2_ = it.it2_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it1_ == it.it1_ && it2_ == it.it2_;
            }

        private:
            const_iterator1_type it1_;
            const_iterator2_type it2_;
        };

        NUMERICS_INLINE
        const_iterator begin () const {
            return lower_bound (0); 
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return upper_bound (size ()); 
        }

        class iterator:
            public container_reference<matrix_type>,
            public random_access_iterator_base<iterator, value_type> {
        public:
            typedef typename restrict_traits<typename M::iterator1::iterator_category,
                                             typename M::iterator2::iterator_category>::iterator_category iterator_category; 
#ifndef USE_MSVC
            typedef typename matrix_type::difference_type difference_type;
            typedef typename matrix_type::value_type value_type;
            typedef typename matrix_type::reference reference;
            typedef typename matrix_type::pointer pointer;
#endif

            // Construction and destruction
            NUMERICS_INLINE
            iterator ():
                container_reference<matrix_type> (), it1_ (), it2_ () {}
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
            reference operator * () const {
                return (*this) () (*it1_, *it2_); 
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                return common (it1_.index (), it2_.index ());
            }

            // Assignment 
            NUMERICS_INLINE
            iterator &operator = (const iterator &it) {
                container_reference<matrix_type>::assign (&it ());
                it1_ = it.it1_;
                it2_ = it.it2_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const iterator &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it1_ == it.it1_ && it2_ == it.it2_;
            }

        private:
            iterator1_type it1_;
            iterator2_type it2_;

            friend class const_iterator;
        };

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
        matrix_type &data_;
        slice s1_;
        slice s2_;
        static matrix_type nil_;
    };

    template<class M>
    matrix_vector_slice<M>::matrix_type matrix_vector_slice<M>::nil_;

    // Matrix based range class
    template<class M>
    class matrix_range:
		public matrix_expression<matrix_range<M> > {
    public:      
        typedef const M const_matrix_type;
        typedef M matrix_type;
        typedef typename M::size_type size_type;
        typedef typename M::difference_type difference_type;
        typedef typename M::value_type value_type;
        typedef typename M::const_reference const_reference;
        typedef typename M::reference reference;
        typedef typename M::const_pointer const_pointer;
        typedef typename M::pointer pointer;
#ifdef NUMERICS_ET_CLOSURE_REFERENCE
        typedef const matrix_const_reference<const matrix_range<matrix_type> > const_closure_type;
        typedef matrix_reference<matrix_range<matrix_type> > closure_type;
#endif
#ifdef NUMERICS_ET_CLOSURE_VALUE
        typedef const matrix_range<matrix_type> const_closure_type;
        typedef matrix_range<matrix_type> closure_type;
#endif
#ifdef NUMERICS_DEPRECATED
        typedef const matrix_row<const matrix_range<const_matrix_type> > const_matrix_row_type;
        typedef matrix_row<matrix_range<matrix_type> > matrix_row_type;
        typedef const matrix_column<const matrix_range<const_matrix_type> > const_matrix_column_type;
        typedef matrix_column<matrix_range<matrix_type> > matrix_column_type;
#endif
        typedef typename M::const_iterator1 const_iterator1_type;
        typedef typename M::iterator1 iterator1_type;
        typedef typename M::const_iterator2 const_iterator2_type;
        typedef typename M::iterator2 iterator2_type;
        typedef typename proxy_traits<typename M::storage_category>::storage_category storage_category;
        typedef typename M::orientation_category orientation_category;

        // Construction and destruction
        NUMERICS_INLINE
        matrix_range (): 
            data_ (nil_), r1_ (), r2_ () {}
        NUMERICS_INLINE
        matrix_range (matrix_type &data, const range &r1, const range &r2): 
            data_ (data), r1_ (r1), r2_ (r2) {}
#ifdef NUMERICS_DEPRECATED
        NUMERICS_INLINE
        matrix_range (matrix_type &data, size_type start1, size_type stop1, size_type start2, size_type stop2): 
            data_ (data), r1_ (start1, stop1), r2_ (start2, stop2) {}
#endif

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
        reference operator () (size_type i, size_type j) {
            return data_ (r1_ (i), r2_ (j)); 
        }

#ifdef NUMERICS_DEPRECATED
        NUMERICS_INLINE
        const_matrix_row_type operator [] (size_type i) const {
            return const_matrix_row_type (*this, i);
        }
        NUMERICS_INLINE
        matrix_row_type operator [] (size_type i) {
            return matrix_row_type (*this, i);
        }
#endif
#ifdef NUMERICS_HAS_PARTIAL_TEMPLATE_SPECIALIZATION
        NUMERICS_INLINE
        matrix_range<matrix_type> project (const range &r1, const range &r2) const {
            return matrix_range<matrix_type>  (data_, r1_.composite (r1), r2_.composite (r2));
        }
        NUMERICS_INLINE
        matrix_range<matrix_type>  project (const range &r1, const range &r2) {
            return matrix_range<matrix_type>  (data_, r1_.composite (r1), r2_.composite (r2));
        }
#endif

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
            check (this != &mr, external_logic ());
            check (size1 () == mr.size1 (), bad_size ());
            check (size2 () == mr.size2 (), bad_size ());
            matrix_swap<scalar_swap<value_type, value_type> > () (*this, mr); 
        }
#ifndef USE_GCC
        NUMERICS_INLINE
	    friend void swap (matrix_range &mr1, matrix_range &mr2) {
            mr1.swap (mr2);
        }
#endif

#ifdef NUMERICS_USE_CANONICAL_ITERATOR
        typedef matrix_row_iterator<matrix_range<matrix_type>,
								    matrix_type::iterator1::iterator_category> iterator1;
        typedef matrix_column_iterator<matrix_range<matrix_type>,
							           matrix_type::iterator2::iterator_category> iterator2;
        typedef matrix_row_const_iterator<matrix_range<matrix_type>,
										  matrix_type::const_iterator1::iterator_category> const_iterator1;
        typedef matrix_column_const_iterator<matrix_range<matrix_type>, 
										     matrix_type::const_iterator2::iterator_category> const_iterator2;
#ifdef USE_MSVC
        typedef reverse_iterator<const_iterator1, typename matrix_row<matrix_range<matrix_type> >, typename matrix_row<matrix_range<const_matrix_type> > > const_reverse_iterator1;
        typedef reverse_iterator<iterator1, typename matrix_row<matrix_range<matrix_type> >, typename matrix_row<matrix_range<matrix_type> > > reverse_iterator1;
        typedef reverse_iterator<const_iterator2, typename matrix_column<matrix_range<matrix_type> >, typename matrix_column<matrix_range<const_matrix_type> > > const_reverse_iterator2;
        typedef reverse_iterator<iterator2, typename matrix_column<matrix_range<matrix_type> >, typename matrix_column<matrix_range<matrix_type> > > reverse_iterator2;
#else
        typedef reverse_iterator<const_iterator1> const_reverse_iterator1;
        typedef reverse_iterator<iterator1> reverse_iterator1;
        typedef reverse_iterator<const_iterator2> const_reverse_iterator2;
        typedef reverse_iterator<iterator2> reverse_iterator2;
#endif
#else
#ifdef NUMERICS_USE_INDEXED_ITERATOR
        typedef indexed_iterator1<matrix_range<matrix_type>,
								  matrix_type::iterator1::iterator_category> iterator1;
        typedef indexed_iterator2<matrix_range<matrix_type>,
							      matrix_type::iterator2::iterator_category> iterator2;
        typedef indexed_const_iterator1<matrix_range<matrix_type>,
										matrix_type::const_iterator1::iterator_category> const_iterator1;
        typedef indexed_const_iterator2<matrix_range<matrix_type>, 
										matrix_type::const_iterator2::iterator_category> const_iterator2;
#else
        class const_iterator1;
        class iterator1;
        class const_iterator2;
        class iterator2;
#endif
#ifdef USE_MSVC
        typedef reverse_iterator1<const_iterator1, value_type, value_type> const_reverse_iterator1;
        typedef reverse_iterator1<iterator1, value_type, reference> reverse_iterator1;
        typedef reverse_iterator2<const_iterator2, value_type, value_type> const_reverse_iterator2;
        typedef reverse_iterator2<iterator2, value_type, reference> reverse_iterator2;
#else
        typedef reverse_iterator1<const_iterator1> const_reverse_iterator1;
        typedef reverse_iterator1<iterator1> reverse_iterator1;
        typedef reverse_iterator2<const_iterator2> const_reverse_iterator2;
        typedef reverse_iterator2<iterator2> reverse_iterator2;
#endif
#endif

        // Element lookup
        NUMERICS_INLINE
        const_iterator1 lower_bound1 (int rank, size_type i, size_type j) const {
            // One has do to this, to get the const member dispatched?!
            const matrix_type &data = data_;
            const_iterator1_type it1 (data.lower_bound1 (rank, start1 () + i, start2 () + j));
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return const_iterator1 (*this, it1.index ());
#else
#ifdef NUMERICS_USE_INDEXED_ITERATOR
            return const_iterator1 (*this, it1.index1 (), it1.index2 ());
#else
            return const_iterator1 (*this, it1);
#endif
#endif
        }
        NUMERICS_INLINE
        iterator1 lower_bound1 (int rank, size_type i, size_type j) {
            iterator1_type it1 (data_.lower_bound1 (rank, start1 () + i, start2 () + j));
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return iterator1 (*this, it1.index ());
#else
#ifdef NUMERICS_USE_INDEXED_ITERATOR
            return iterator1 (*this, it1.index1 (), it1.index2 ());
#else
            return iterator1 (*this, it1);
#endif
#endif
        }
        NUMERICS_INLINE
        const_iterator1 upper_bound1 (int rank, size_type i, size_type j) const {
            // One has do to this, to get the const member dispatched?!
            const matrix_type &data = data_;
            const_iterator1_type it1 (data.upper_bound1 (rank, start1 () + i, start2 () + j));
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return const_iterator1 (*this, it1.index ());
#else
#ifdef NUMERICS_USE_INDEXED_ITERATOR
            return const_iterator1 (*this, it1.index1 (), it1.index2 ());
#else
            return const_iterator1 (*this, it1);
#endif
#endif
        }
        NUMERICS_INLINE
        iterator1 upper_bound1 (int rank, size_type i, size_type j) {
            iterator1_type it1 (data_.upper_bound1 (rank, start1 () + i, start2 () + j));
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return iterator1 (*this, it1.index ());
#else
#ifdef NUMERICS_USE_INDEXED_ITERATOR
            return iterator1 (*this, it1.index1 (), it1.index2 ());
#else
            return iterator1 (*this, it1);
#endif
#endif
        }
        NUMERICS_INLINE
        const_iterator2 lower_bound2 (int rank, size_type i, size_type j) const {
            // One has do to this, to get the const member dispatched?!
            const matrix_type &data = data_;
            const_iterator2_type it2 (data.lower_bound2 (rank, start1 () + i, start2 () + j));
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return const_iterator2 (*this, it2.index ());
#else
#ifdef NUMERICS_USE_INDEXED_ITERATOR
            return const_iterator2 (*this, it2.index1 (), it2.index2 ());
#else
            return const_iterator2 (*this, it2);
#endif
#endif
        }
        NUMERICS_INLINE
        iterator2 lower_bound2 (int rank, size_type i, size_type j) {
            iterator2_type it2 (data_.lower_bound2 (rank, start1 () + i, start2 () + j));
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return iterator2 (*this, it2.index ());
#else
#ifdef NUMERICS_USE_INDEXED_ITERATOR
            return iterator2 (*this, it2.index1 (), it2.index2 ());
#else
            return iterator2 (*this, it2);
#endif
#endif
        }
        NUMERICS_INLINE
        const_iterator2 upper_bound2 (int rank, size_type i, size_type j) const {
            // One has do to this, to get the const member dispatched?!
            const matrix_type &data = data_;
            const_iterator2_type it2 (data.upper_bound2 (rank, start1 () + i, start2 () + j));
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return const_iterator2 (*this, it2.index ());
#else
#ifdef NUMERICS_USE_INDEXED_ITERATOR
            return const_iterator2 (*this, it2.index1 (), it2.index2 ());
#else
            return const_iterator2 (*this, it2);
#endif
#endif
        }
        NUMERICS_INLINE
        iterator2 upper_bound2 (int rank, size_type i, size_type j) {
            iterator2_type it2 (data_.upper_bound2 (rank, start1 () + i, start2 () + j));
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return iterator2 (*this, it2.index ());
#else
#ifdef NUMERICS_USE_INDEXED_ITERATOR
            return iterator2 (*this, it2.index1 (), it2.index2 ());
#else
            return iterator2 (*this, it2);
#endif
#endif
        }

        // Iterators simply are pointers.

#if ! defined (NUMERICS_USE_CANONICAL_ITERATOR) && ! defined (NUMERICS_USE_INDEXED_ITERATOR)
        class const_iterator1:
            public container_const_reference<matrix_range>,
            public random_access_iterator_base<const_iterator1, value_type> {
        public:
            typedef typename M::const_iterator1::iterator_category iterator_category;
#ifndef USE_MSVC
            typedef typename M::const_iterator1::difference_type difference_type;
            typedef typename M::const_iterator1::value_type value_type;
            typedef typename M::const_iterator1::value_type reference;
            typedef typename M::const_iterator1::pointer pointer;
#endif
            typedef const_iterator2 dual_iterator_type;
            typedef const_reverse_iterator2 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator1 ():
                container_const_reference<matrix_range> (), it_ () {}
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
                return mr.lower_bound2 (1, index1 (), 0);
            }
            NUMERICS_INLINE
            const_iterator2 end () const {
                const matrix_range &mr = (*this) ();
                return mr.upper_bound2 (1, index1 (), mr.size2 ());
            }
            NUMERICS_INLINE
            const_reverse_iterator2 rbegin () const {
                return const_reverse_iterator2 (end ());
            }
            NUMERICS_INLINE
            const_reverse_iterator2 rend () const {
                return const_reverse_iterator2 (begin ());
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

            // Assignment 
            NUMERICS_INLINE
            const_iterator1 &operator = (const const_iterator1 &it) {
                container_const_reference<matrix_range>::assign (&it ());
                it_ = it.it_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator1 &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it_ == it.it_;
            }

        private:
            const_iterator1_type it_;
        };
#endif

        NUMERICS_INLINE
        const_iterator1 begin1 () const {
            return lower_bound1 (0, 0, 0); 
        }
        NUMERICS_INLINE
        const_iterator1 end1 () const {
            return upper_bound1 (0, size1 (), 0); 
        }

#if ! defined (NUMERICS_USE_CANONICAL_ITERATOR) && ! defined (NUMERICS_USE_INDEXED_ITERATOR)
        class iterator1:
            public container_reference<matrix_range>,
            public random_access_iterator_base<iterator1, value_type> {
        public:
            typedef typename M::iterator1::iterator_category iterator_category;
#ifndef USE_MSVC
            typedef typename M::iterator1::difference_type difference_type;
            typedef typename M::iterator1::value_type value_type;
            typedef typename M::iterator1::reference reference;
            typedef typename M::iterator1::pointer pointer;
#endif
            typedef iterator2 dual_iterator_type;
            typedef reverse_iterator2 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            iterator1 ():
                container_reference<matrix_range> (), it_ () {}
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
            reference operator * () const {
                return *it_; 
            }

            NUMERICS_INLINE
            iterator2 begin () const {
                matrix_range &mr = (*this) ();
                return mr.lower_bound2 (1, index1 (), 0);
            }
            NUMERICS_INLINE
            iterator2 end () const {
                matrix_range &mr = (*this) ();
                return mr.upper_bound2 (1, index1 (), mr.size2 ());
            }
            NUMERICS_INLINE
            reverse_iterator2 rbegin () const {
                return reverse_iterator2 (end ());
            }
            NUMERICS_INLINE
            reverse_iterator2 rend () const {
                return reverse_iterator2 (begin ());
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

            // Assignment 
            NUMERICS_INLINE
            iterator1 &operator = (const iterator1 &it) {
                container_reference<matrix_range>::assign (&it ());
                it_ = it.it_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const iterator1 &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it_ == it.it_;
            }

        private:
            iterator1_type it_;

            friend class const_iterator1;
        };
#endif

        NUMERICS_INLINE
        iterator1 begin1 () {
            return lower_bound1 (0, 0, 0); 
        }
        NUMERICS_INLINE
        iterator1 end1 () {
            return upper_bound1 (0, size1 (), 0); 
        }

#if ! defined (NUMERICS_USE_CANONICAL_ITERATOR) && ! defined (NUMERICS_USE_INDEXED_ITERATOR)
        class const_iterator2:
            public container_const_reference<matrix_range>,
            public random_access_iterator_base<const_iterator2, value_type> {
        public:
            typedef typename M::const_iterator2::iterator_category iterator_category;
#ifndef USE_MSVC
            typedef typename M::const_iterator2::difference_type difference_type;
            typedef typename M::const_iterator2::value_type value_type;
            typedef typename M::const_iterator2::value_type reference;
            typedef typename M::const_iterator2::pointer pointer;
#endif
            typedef const_iterator1 dual_iterator_type;
            typedef const_reverse_iterator1 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator2 ():
                container_const_reference<matrix_range> (), it_ () {}
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
                return mr.lower_bound1 (1, 0, index2 ());
            }
            NUMERICS_INLINE
            const_iterator1 end () const {
                const matrix_range &mr = (*this) ();
                return mr.upper_bound1 (1, mr.size1 (), index2 ());
            }
            NUMERICS_INLINE
            const_reverse_iterator1 rbegin () const {
                return const_reverse_iterator1 (end ());
            }
            NUMERICS_INLINE
            const_reverse_iterator1 rend () const {
                return const_reverse_iterator1 (begin ());
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

            // Assignment 
            NUMERICS_INLINE
            const_iterator2 &operator = (const const_iterator2 &it) {
                container_const_reference<matrix_range>::assign (&it ());
                it_ = it.it_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator2 &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it_ == it.it_;
            }

        private:
            const_iterator2_type it_;
        };
#endif

        NUMERICS_INLINE
        const_iterator2 begin2 () const {
            return lower_bound2 (0, 0, 0); 
        }
        NUMERICS_INLINE
        const_iterator2 end2 () const {
            return upper_bound2 (0, 0, size2 ()); 
        }

#if ! defined (NUMERICS_USE_CANONICAL_ITERATOR) && ! defined (NUMERICS_USE_INDEXED_ITERATOR)
        class iterator2:
            public container_reference<matrix_range>,
            public random_access_iterator_base<iterator2, value_type> {
        public:
            typedef typename M::iterator2::iterator_category iterator_category;
#ifndef USE_MSVC
            typedef typename M::iterator2::difference_type difference_type;
            typedef typename M::iterator2::value_type value_type;
            typedef typename M::iterator2::reference reference;
            typedef typename M::iterator2::pointer pointer;
#endif
            typedef iterator1 dual_iterator_type;
            typedef reverse_iterator1 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            iterator2 ():
                container_reference<matrix_range> (), it_ () {}
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
            reference operator * () const {
                return *it_;
            }

            NUMERICS_INLINE
            iterator1 begin () const {
                matrix_range &mr = (*this) ();
                return mr.lower_bound1 (1, 0, index2 ());
            }
            NUMERICS_INLINE
            iterator1 end () const {
                matrix_range &mr = (*this) ();
                return mr.upper_bound1 (1, mr.size1 (), index2 ());
            }
            NUMERICS_INLINE
            reverse_iterator1 rbegin () const {
                return reverse_iterator1 (end ());
            }
            NUMERICS_INLINE
            reverse_iterator1 rend () const {
                return reverse_iterator1 (begin ());
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

            // Assignment 
            NUMERICS_INLINE
            iterator2 &operator = (const iterator2 &it) {
                container_reference<matrix_range>::assign (&it ());
                it_ = it.it_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const iterator2 &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it_ == it.it_;
            }

        private:
            iterator2_type it_;

            friend class const_iterator2;
        };
#endif

        NUMERICS_INLINE
        iterator2 begin2 () {
            return lower_bound2 (0, 0, 0); 
        }
        NUMERICS_INLINE
        iterator2 end2 () {
            return upper_bound2 (0, 0, size2 ()); 
        }

        // Reverse iterators

        NUMERICS_INLINE
        const_reverse_iterator1 rbegin1 () const {
            return const_reverse_iterator1 (end1 ());
        }
        NUMERICS_INLINE
        const_reverse_iterator1 rend1 () const {
            return const_reverse_iterator1 (begin1 ());
        }

        NUMERICS_INLINE
        reverse_iterator1 rbegin1 () {
            return reverse_iterator1 (end1 ());
        }
        NUMERICS_INLINE
        reverse_iterator1 rend1 () {
            return reverse_iterator1 (begin1 ());
        }

        NUMERICS_INLINE
        const_reverse_iterator2 rbegin2 () const {
            return const_reverse_iterator2 (end2 ());
        }
        NUMERICS_INLINE
        const_reverse_iterator2 rend2 () const {
            return const_reverse_iterator2 (begin2 ());
        }

        NUMERICS_INLINE
        reverse_iterator2 rbegin2 () {
            return reverse_iterator2 (end2 ());
        }
        NUMERICS_INLINE
        reverse_iterator2 rend2 () {
            return reverse_iterator2 (begin2 ());
        }

    private:
        matrix_type &data_;
        range r1_;
        range r2_;
        static matrix_type nil_;
    };

    template<class M>
    matrix_range<M>::matrix_type matrix_range<M>::nil_;

    // Projections
#ifdef NUMERICS_DEPRECATED
    template<class M>
    NUMERICS_INLINE
    matrix_range<M> project (M &data, std::size_t start1, std::size_t stop1, std::size_t start2, std::size_t stop2) {
        return matrix_range<M> (data, start1, stop1, start2, stop2);
    }
#endif
    template<class M>
    NUMERICS_INLINE
    matrix_range<M> project (M &data, const range &r1, const range &r2) {
        return matrix_range<M> (data, r1, r2);
    }
#ifdef NUMERICS_HAS_PARTIAL_TEMPLATE_SPECIALIZATION
    template<class M>
    NUMERICS_INLINE
    matrix_range<M> project (const matrix_range<M> &data, const range &r1, const range &r2) {
		NUMERICS_TRACE ("project (matrix_range<M>, range, range)");
        return data.project (r1, r2);
    }
#endif

    // Matrix based slice class
    template<class M>
    class matrix_slice:
		public matrix_expression<matrix_slice<M> > {
    public:      
        typedef const M const_matrix_type;
        typedef M matrix_type;
        typedef typename M::size_type size_type;
        typedef typename M::difference_type difference_type;
        typedef typename M::value_type value_type;
        typedef typename M::const_reference const_reference;
        typedef typename M::reference reference;
        typedef typename M::const_pointer const_pointer;
        typedef typename M::pointer pointer;
#ifdef NUMERICS_ET_CLOSURE_REFERENCE
        typedef const matrix_const_reference<const matrix_slice<matrix_type> > const_closure_type;
        typedef matrix_reference<matrix_slice<matrix_type> > closure_type;
#endif
#ifdef NUMERICS_ET_CLOSURE_VALUE
        typedef const matrix_slice<matrix_type> const_closure_type;
        typedef matrix_slice<matrix_type> closure_type;
#endif
#ifdef NUMERICS_DEPRECATED
        typedef const matrix_row<const matrix_slice<const_matrix_type> > const_matrix_row_type;
        typedef matrix_row<matrix_slice<matrix_type> > matrix_row_type;
        typedef const matrix_column<const matrix_slice<const_matrix_type> > const_matrix_column_type;
        typedef matrix_column<matrix_slice<matrix_type> > matrix_column_type;
#endif
        typedef slice::const_iterator const_iterator1_type;
        typedef slice::const_iterator iterator1_type;
        typedef slice::const_iterator const_iterator2_type;
        typedef slice::const_iterator iterator2_type;
        typedef typename proxy_traits<typename M::storage_category>::storage_category storage_category;
        typedef typename M::orientation_category orientation_category;

        // Construction and destruction
        NUMERICS_INLINE
        matrix_slice (): 
            data_ (nil_), s1_ (), s2_ () {}
        NUMERICS_INLINE
        matrix_slice (matrix_type &data, const slice &s1, const slice &s2): 
            data_ (data), s1_ (s1), s2_ (s2) {}
#ifdef NUMERICS_DEPRECATED
        NUMERICS_INLINE
        matrix_slice (matrix_type &data, size_type start1, size_type stride1, size_type size1, size_type start2, size_type stride2, size_type size2): 
            data_ (data), s1_ (start1, stride1, size1), s2_ (start2, stride2, size2) {}
#endif

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
        reference operator () (size_type i, size_type j) {
            return data_ (s1_ (i), s2_ (j)); 
        }

#ifdef NUMERICS_DEPRECATED
        NUMERICS_INLINE
        const_matrix_row_type operator [] (size_type i) const {
            return const_matrix_row_type (*this, i);
        }
        NUMERICS_INLINE
        matrix_row_type operator [] (size_type i) {
            return matrix_row_type (*this, i);
        }
#endif
#ifdef NUMERICS_HAS_PARTIAL_TEMPLATE_SPECIALIZATION
        NUMERICS_INLINE
        matrix_slice<matrix_type> project (const range &r1, const range &r2) const {
            return matrix_slice<matrix_type>  (data_, s1_.composite (r1), s2_.composite (r2));
        }
        NUMERICS_INLINE
        matrix_slice<matrix_type> project (const range &r1, const range &r2) {
            return matrix_slice<matrix_type>  (data_, s1_.composite (r1), s2_.composite (r2));
        }
        NUMERICS_INLINE
        matrix_slice<matrix_type> project (const slice &s1, const slice &s2) const {
            return matrix_slice<matrix_type>  (data_, s1_.composite (s1), s2_.composite (s2));
        }
        NUMERICS_INLINE
        matrix_slice<matrix_type> project (const slice &s1, const slice &s2) {
            return matrix_slice<matrix_type>  (data_, s1_.composite (s1), s2_.composite (s2));
        }
#endif

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
            check (this != &ms, external_logic ());
            check (size1 () == ms.size1 (), bad_size ());
            check (size2 () == ms.size2 (), bad_size ());
            matrix_swap<scalar_swap<value_type, value_type> > () (*this, ms); 
        }
#ifndef USE_GCC
        NUMERICS_INLINE
	    friend void swap (matrix_slice &ms1, matrix_slice &ms2) {
            ms1.swap (ms2);
        }
#endif

#ifdef NUMERICS_USE_CANONICAL_ITERATOR
        typedef matrix_row_iterator<matrix_slice<matrix_type>,
								    matrix_type::iterator1::iterator_category> iterator1;
        typedef matrix_column_iterator<matrix_slice<matrix_type>,
							           matrix_type::iterator2::iterator_category> iterator2;
        typedef matrix_row_const_iterator<matrix_slice<matrix_type>,
										  matrix_type::const_iterator1::iterator_category> const_iterator1;
        typedef matrix_column_const_iterator<matrix_slice<matrix_type>, 
										     matrix_type::const_iterator2::iterator_category> const_iterator2;
#ifdef USE_MSVC
        typedef reverse_iterator<const_iterator1, typename matrix_row<matrix_slice<matrix_type> >, typename matrix_row<matrix_slice<const_matrix_type> > > const_reverse_iterator1;
        typedef reverse_iterator<iterator1, typename matrix_row<matrix_slice<matrix_type> >, typename matrix_row<matrix_slice<matrix_type> > > reverse_iterator1;
        typedef reverse_iterator<const_iterator2, typename matrix_column<matrix_slice<matrix_type> >, typename matrix_column<matrix_slice<const_matrix_type> > > const_reverse_iterator2;
        typedef reverse_iterator<iterator2, typename matrix_column<matrix_slice<matrix_type> >, typename matrix_column<matrix_slice<matrix_type> > > reverse_iterator2;
#else
        typedef reverse_iterator<const_iterator1> const_reverse_iterator1;
        typedef reverse_iterator<iterator1> reverse_iterator1;
        typedef reverse_iterator<const_iterator2> const_reverse_iterator2;
        typedef reverse_iterator<iterator2> reverse_iterator2;
#endif
#else
#ifdef NUMERICS_USE_INDEXED_ITERATOR
        typedef indexed_iterator1<matrix_slice<matrix_type>,
								  matrix_type::iterator1::iterator_category> iterator1;
        typedef indexed_iterator2<matrix_slice<matrix_type>,
								  matrix_type::iterator2::iterator_category> iterator2;
        typedef indexed_const_iterator1<matrix_slice<matrix_type>,
									    matrix_type::const_iterator1::iterator_category> const_iterator1;
        typedef indexed_const_iterator2<matrix_slice<matrix_type>,
										matrix_type::const_iterator2::iterator_category> const_iterator2;
#else
        class const_iterator1;
        class iterator1;
        class const_iterator2;
        class iterator2;
#endif
#ifdef USE_MSVC
        typedef reverse_iterator1<const_iterator1, value_type, value_type> const_reverse_iterator1;
        typedef reverse_iterator1<iterator1, value_type, reference> reverse_iterator1;
        typedef reverse_iterator2<const_iterator2, value_type, value_type> const_reverse_iterator2;
        typedef reverse_iterator2<iterator2, value_type, reference> reverse_iterator2;
#else
        typedef reverse_iterator1<const_iterator1> const_reverse_iterator1;
        typedef reverse_iterator1<iterator1> reverse_iterator1;
        typedef reverse_iterator2<const_iterator2> const_reverse_iterator2;
        typedef reverse_iterator2<iterator2> reverse_iterator2;
#endif
#endif

        // Element lookup
        NUMERICS_INLINE
        const_iterator1 lower_bound1 (int rank, size_type i, size_type j) const {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return const_iterator1 (*this, i);
#else
#ifdef NUMERICS_USE_INDEXED_ITERATOR
            return const_iterator1 (*this, i, j);
#else
            return const_iterator1 (data_, s1_.begin () + i, s2_.begin () + j);
#endif
#endif
        }
        NUMERICS_INLINE
        iterator1 lower_bound1 (int rank, size_type i, size_type j) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return iterator1 (*this, i);
#else
#ifdef NUMERICS_USE_INDEXED_ITERATOR
            return iterator1 (*this, i, j);
#else
            return iterator1 (data_, s1_.begin () + i, s2_.begin () + j);
#endif
#endif
        }
        NUMERICS_INLINE
        const_iterator1 upper_bound1 (int rank, size_type i, size_type j) const {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return const_iterator1 (*this, i);
#else
#ifdef NUMERICS_USE_INDEXED_ITERATOR
            return const_iterator1 (*this, i, j);
#else
            return const_iterator1 (data_, s1_.begin () + i, s2_.begin () + j);
#endif
#endif
        }
        NUMERICS_INLINE
        iterator1 upper_bound1 (int rank, size_type i, size_type j) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return iterator1 (*this, i);
#else
#ifdef NUMERICS_USE_INDEXED_ITERATOR
            return iterator1 (*this, i, j);
#else
            return iterator1 (data_, s1_.begin () + i, s2_.begin () + j);
#endif
#endif
        }
        NUMERICS_INLINE
        const_iterator2 lower_bound2 (int rank, size_type i, size_type j) const {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return const_iterator2 (*this, j);
#else
#ifdef NUMERICS_USE_INDEXED_ITERATOR
            return const_iterator2 (*this, i, j);
#else
            return const_iterator2 (data_, s1_.begin () + i, s2_.begin () + j);
#endif
#endif
        }
        NUMERICS_INLINE
        iterator2 lower_bound2 (int rank, size_type i, size_type j) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return iterator2 (*this, j);
#else
#ifdef NUMERICS_USE_INDEXED_ITERATOR
            return iterator2 (*this, i, j);
#else
            return iterator2 (data_, s1_.begin () + i, s2_.begin () + j);
#endif
#endif
        }
        NUMERICS_INLINE
        const_iterator2 upper_bound2 (int rank, size_type i, size_type j) const {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return const_iterator2 (*this, j);
#else
#ifdef NUMERICS_USE_INDEXED_ITERATOR
            return const_iterator2 (*this, i, j);
#else
            return const_iterator2 (data_, s1_.begin () + i, s2_.begin () + j);
#endif
#endif
        }
        NUMERICS_INLINE
        iterator2 upper_bound2 (int rank, size_type i, size_type j) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return iterator2 (*this, j);
#else
#ifdef NUMERICS_USE_INDEXED_ITERATOR
            return iterator2 (*this, i, j);
#else
            return iterator2 (data_, s1_.begin () + i, s2_.begin () + j);
#endif
#endif
        }

        // Iterators simply are indices.

#if ! defined (NUMERICS_USE_CANONICAL_ITERATOR) && ! defined (NUMERICS_USE_INDEXED_ITERATOR)
        class const_iterator1:
            public container_const_reference<matrix_type>,
            public random_access_iterator_base<const_iterator1, value_type> {
        public:
            typedef typename M::const_iterator1::iterator_category iterator_category;
#ifndef USE_MSVC
            typedef typename M::const_iterator1::difference_type difference_type;
            typedef typename M::const_iterator1::value_type value_type;
            typedef typename M::const_iterator1::value_type reference;
            typedef typename M::const_iterator1::pointer pointer;
#endif
            typedef const_iterator2 dual_iterator_type;
            typedef const_reverse_iterator2 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator1 ():
                container_const_reference<matrix_type> (), it1_ (), it2_ () {}
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
            NUMERICS_INLINE
            const_reverse_iterator2 rbegin () const {
                return const_reverse_iterator2 (end ());
            }
            NUMERICS_INLINE
            const_reverse_iterator2 rend () const {
                return const_reverse_iterator2 (begin ());
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

            // Assignment 
            NUMERICS_INLINE
            const_iterator1 &operator = (const const_iterator1 &it) {
                container_const_reference<matrix_type>::assign (&it ());
                it1_ = it.it1_;
                it2_ = it.it2_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator1 &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it1_ == it.it1_ && it2_ == it.it2_;
            }

        private:
            const_iterator1_type it1_;
            const_iterator2_type it2_;
        };
#endif

        NUMERICS_INLINE
        const_iterator1 begin1 () const {
            return lower_bound1 (0, 0, 0);
        }
        NUMERICS_INLINE
        const_iterator1 end1 () const {
            return upper_bound1 (0, size1 (), 0);
        }

#if ! defined (NUMERICS_USE_CANONICAL_ITERATOR) && ! defined (NUMERICS_USE_INDEXED_ITERATOR)
        class iterator1:
            public container_reference<matrix_type>,
            public random_access_iterator_base<iterator1, value_type> {
        public:
            typedef typename M::iterator1::iterator_category iterator_category;
#ifndef USE_MSVC
            typedef typename M::iterator1::difference_type difference_type;
            typedef typename M::iterator1::value_type value_type;
            typedef typename M::iterator1::reference reference;
            typedef typename M::iterator1::pointer pointer;
#endif
            typedef iterator2 dual_iterator_type;
            typedef reverse_iterator2 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            iterator1 ():
                container_reference<matrix_type> (), it1_ (), it2_ () {}
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
            reference operator * () const {
                return (*this) () (*it1_, *it2_); 
            }

            NUMERICS_INLINE
            iterator2 begin () const {
                return iterator2 ((*this) (), it1_, it2_ ().begin ());
            }
            NUMERICS_INLINE
            iterator2 end () const {
                return iterator2 ((*this) (), it1_, it2_ ().end ()); 
            }
            NUMERICS_INLINE
            reverse_iterator2 rbegin () const {
                return reverse_iterator2 (end ());
            }
            NUMERICS_INLINE
            reverse_iterator2 rend () const {
                return reverse_iterator2 (begin ());
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

            // Assignment 
            NUMERICS_INLINE
            iterator1 &operator = (const iterator1 &it) {
                container_reference<matrix_type>::assign (&it ());
                it1_ = it.it1_;
                it2_ = it.it2_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const iterator1 &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it1_ == it.it1_ && it2_ == it.it2_;
            }

        private:
            iterator1_type it1_;
            iterator2_type it2_;

            friend class const_iterator1;
        };
#endif

        NUMERICS_INLINE
        iterator1 begin1 () {
            return lower_bound1 (0, 0, 0);
        }
        NUMERICS_INLINE
        iterator1 end1 () {
            return upper_bound1 (0, size1 (), 0);
        }

#if ! defined (NUMERICS_USE_CANONICAL_ITERATOR) && ! defined (NUMERICS_USE_INDEXED_ITERATOR)
        class const_iterator2:
            public container_const_reference<matrix_type>,
            public random_access_iterator_base<const_iterator2, value_type> {
        public:
            typedef typename M::const_iterator2::iterator_category iterator_category;
#ifndef USE_MSVC
            typedef typename M::const_iterator2::difference_type difference_type;
            typedef typename M::const_iterator2::value_type value_type;
            typedef typename M::const_iterator2::value_type reference;
            typedef typename M::const_iterator2::pointer pointer;
#endif
            typedef const_iterator1 dual_iterator_type;
            typedef const_reverse_iterator1 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator2 ():
                container_const_reference<matrix_type> (), it1_ (), it2_ () {}
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
            NUMERICS_INLINE
            const_reverse_iterator1 rbegin () const {
                return const_reverse_iterator1 (end ());
            }
            NUMERICS_INLINE
            const_reverse_iterator1 rend () const {
                return const_reverse_iterator1 (begin ());
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

            // Assignment 
            NUMERICS_INLINE
            const_iterator2 &operator = (const const_iterator2 &it) {
                container_const_reference<matrix_type>::assign (&it ());
                it1_ = it.it1_;
                it2_ = it.it2_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator2 &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it1_ == it.it1_ && it2_ == it.it2_;
            }

        private:
            const_iterator1_type it1_;
            const_iterator2_type it2_;
        };
#endif

        NUMERICS_INLINE
        const_iterator2 begin2 () const {
            return lower_bound2 (0, 0, 0);
        }
        NUMERICS_INLINE
        const_iterator2 end2 () const {
            return upper_bound2 (0, 0, size2 ());
        }

#if ! defined (NUMERICS_USE_CANONICAL_ITERATOR) && ! defined (NUMERICS_USE_INDEXED_ITERATOR)
        class iterator2:
            public container_reference<matrix_type>,
            public random_access_iterator_base<iterator2, value_type> {
        public:
            typedef typename M::iterator2::iterator_category iterator_category;
#ifndef USE_MSVC
            typedef typename M::iterator2::difference_type difference_type;
            typedef typename M::iterator2::value_type value_type;
            typedef typename M::iterator2::reference reference;
            typedef typename M::iterator2::pointer pointer;
#endif
            typedef iterator1 dual_iterator_type;
            typedef reverse_iterator1 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            iterator2 ():
                container_reference<matrix_type> (), it1_ (), it2_ () {}
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
            reference operator * () const {
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
            NUMERICS_INLINE
            reverse_iterator1 rbegin () const {
                return reverse_iterator1 (end ());
            }
            NUMERICS_INLINE
            reverse_iterator1 rend () const {
                return reverse_iterator1 (begin ());
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

            // Assignment 
            NUMERICS_INLINE
            iterator2 &operator = (const iterator2 &it) {
                container_reference<matrix_type>::assign (&it ());
                it1_ = it.it1_;
                it2_ = it.it2_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const iterator2 &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it1_ == it.it1_ && it2_ == it.it2_;
            }

        private:
            iterator1_type it1_;
            iterator2_type it2_;

            friend class const_iterator2;
        };
#endif

        NUMERICS_INLINE
        iterator2 begin2 () {
            return lower_bound2 (0, 0, 0);
        }
        NUMERICS_INLINE
        iterator2 end2 () {
            return upper_bound2 (0, 0, size2 ());
        }

        // Reverse iterators

        NUMERICS_INLINE
        const_reverse_iterator1 rbegin1 () const {
            return const_reverse_iterator1 (end1 ());
        }
        NUMERICS_INLINE
        const_reverse_iterator1 rend1 () const {
            return const_reverse_iterator1 (begin1 ());
        }

        NUMERICS_INLINE
        reverse_iterator1 rbegin1 () {
            return reverse_iterator1 (end1 ());
        }
        NUMERICS_INLINE
        reverse_iterator1 rend1 () {
            return reverse_iterator1 (begin1 ());
        }

        NUMERICS_INLINE
        const_reverse_iterator2 rbegin2 () const {
            return const_reverse_iterator2 (end2 ());
        }
        NUMERICS_INLINE
        const_reverse_iterator2 rend2 () const {
            return const_reverse_iterator2 (begin2 ());
        }

        NUMERICS_INLINE
        reverse_iterator2 rbegin2 () {
            return reverse_iterator2 (end2 ());
        }
        NUMERICS_INLINE
        reverse_iterator2 rend2 () {
            return reverse_iterator2 (begin2 ());
        }

    private:
        matrix_type &data_;
        slice s1_;
        slice s2_;
        static matrix_type nil_;
    };

    template<class M>
    matrix_slice<M>::matrix_type matrix_slice<M>::nil_;

    // Projections
#ifdef NUMERICS_HAS_PARTIAL_TEMPLATE_SPECIALIZATION
    template<class M>
    NUMERICS_INLINE
    matrix_slice<M> project (const matrix_slice<M> &data, const range &r1, const range &r2) {
		NUMERICS_TRACE ("project (matrix_slice<M>, range, range)");
        return data.project (r1, r2);
    }
#endif
    template<class M>
    NUMERICS_INLINE
    matrix_slice<M> project (M &data, const slice &s1, const slice &s2) {
        return matrix_slice<M> (data, s1, s2);
    }
#ifdef NUMERICS_HAS_PARTIAL_TEMPLATE_SPECIALIZATION
    template<class M>
    NUMERICS_INLINE
    matrix_slice<M> project (const matrix_slice<M> &data, const slice &s1, const slice &s2) {
		NUMERICS_TRACE ("project (matrix_slice<M>, slice, slice)");
        return data.project (s1, s2);
    }
#endif

}

#endif 





