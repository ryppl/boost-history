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

#ifndef NUMERICS_HERMITEAN_H
#define NUMERICS_HERMITEAN_H

#include "config.h"
#include "storage.h"
#include "vector_et.h"
#include "matrix_et.h"

// Iterators based on ideas of Jeremy Siek

namespace numerics {

    // Array based hermitean matrix class 
    template<class T, class F1, class F2, class A>
    class hermitean_matrix: 
        public matrix_expression<hermitean_matrix<T, F1, F2, A> > {
    public:      
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef const T &const_reference_type;
        typedef T &reference_type;
        typedef const T *const_pointer_type;
        typedef T *pointer_type;
        typedef F1 functor1_type;
        typedef F2 functor2_type;
        typedef A array_type;
        typedef const hermitean_matrix<T, F1, F2, A> const_self_type;
        typedef hermitean_matrix<T, F1, F2, A> self_type;
        typedef const matrix_const_reference<const_self_type> const_closure_type;
        typedef matrix_reference<self_type> closure_type;
#ifdef NUMERICS_DEPRECATED
        typedef const matrix_row<const_self_type> const_matrix_row_type;
        typedef matrix_row<self_type> matrix_row_type;
        typedef const matrix_column<const_self_type> const_matrix_column_type;
        typedef matrix_column<self_type> matrix_column_type;
        typedef const matrix_range<const_self_type> const_matrix_range_type;
        typedef matrix_range<self_type> matrix_range_type;
#endif
        typedef packed_tag storage_category;
        typedef typename F1::packed_category packed_category;
        typedef typename F2::orientation_category orientation_category;

        // Construction and destruction
        NUMERICS_INLINE
        hermitean_matrix (): 
            size1_ (0), size2_ (0), 
            data_ (0) {}
        NUMERICS_INLINE
        hermitean_matrix (size_type size1, size_type size2): 
            size1_ (size1), size2_ (size2), 
            data_ (functor1_type::packed_size (size1, size2)) {}
        NUMERICS_INLINE
        hermitean_matrix (const hermitean_matrix &m): 
            size1_ (m.size1_), size2_ (m.size2_),
            data_ (m.data_) {}
        template<class AE>
        NUMERICS_EXPLICIT NUMERICS_INLINE
        hermitean_matrix (const matrix_expression<AE> &ae): 
            size1_ (ae ().size1 ()), size2_ (ae ().size2 ()),
            data_ (functor1_type::packed_size (ae ().size1 (), ae ().size2 ())) {
            matrix_assign<scalar_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
        }

        // Resizing
        NUMERICS_INLINE
        void resize (size_type size1, size_type size2) {
            size1_ = size1;
            size2_ = size2;
            data_.resize (functor1_type::packed_size (size1, size2));
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
            check (i < size1_, bad_index ());
            check (j < size2_, bad_index ());
            if (functor1_type::other (i, j))
                return data_ [functor1_type::element (functor2_type (), i, size1_, j, size2_)];
            else
                return detail::conj (data_ [functor1_type::element (functor2_type (), j, size2_, i, size1_)]);
        }
        NUMERICS_INLINE
        reference_type operator () (size_type i, size_type j) {
            check (i < size1_, bad_index ());
            check (j < size2_, bad_index ());
            if (functor1_type::other (i, j))
                return data_ [functor1_type::element (functor2_type (), i, size1_, j, size2_)];
            else
                throw external_logic ();
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
#endif

        // Assignment
        NUMERICS_INLINE
        hermitean_matrix &operator = (const hermitean_matrix &m) { 
            check (size1_ == m.size1_, bad_size ());
            check (size2_ == m.size2_, bad_size ());
            data_ = m.data_;
            return *this;
        }
        NUMERICS_INLINE
        hermitean_matrix &assign_temporary (hermitean_matrix &m) { 
            swap (m);
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        hermitean_matrix &operator = (const matrix_expression<AE> &ae) { 
#ifndef USE_GCC
            return assign_temporary (self_type (ae));
#else
            return assign (self_type (ae));
#endif
        }
        template<class AE>
        NUMERICS_INLINE
        hermitean_matrix &assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        hermitean_matrix& operator += (const matrix_expression<AE> &ae) {
#ifndef USE_GCC
            return assign_temporary (self_type (*this + ae));
#else
            return assign (self_type (*this + ae));
#endif
        }
        template<class AE>
        NUMERICS_INLINE
        hermitean_matrix &plus_assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_plus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        hermitean_matrix& operator -= (const matrix_expression<AE> &ae) {
#ifndef USE_GCC
            return assign_temporary (self_type (*this - ae));
#else
            return assign (self_type (*this - ae));
#endif
        }
        template<class AE>
        NUMERICS_INLINE
        hermitean_matrix &minus_assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_minus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        hermitean_matrix& operator *= (const AT &at) {
            matrix_assign_scalar<scalar_multiplies_assign<value_type, AT> > () (*this, at);
            return *this;
        }

        // Swapping
        NUMERICS_INLINE
	    void swap (hermitean_matrix &m) {
            check (this != &m, external_logic ());
            check (size1_ == m.size1_, bad_size ());
            check (size2_ == m.size2_, bad_size ());
            std::swap (size1_, m.size1_);
            std::swap (size2_, m.size2_);
            data_.swap (m.data_);
        }
#ifndef USE_GCC
        NUMERICS_INLINE
	    friend void swap (hermitean_matrix &m1, hermitean_matrix &m2) {
            m1.swap (m2);
        }
#endif

        // Element insertion and erasure
        NUMERICS_INLINE
        void insert (size_type i, size_type j, const_reference_type t) {
            check (i < size1_, bad_index ());
            check (j < size2_, bad_index ());
#ifndef NUMERICS_USE_ET
            if (t == value_type ()) 
                return;
#endif
            if (functor1_type::other (i, j)) {
                check (data_ [functor1_type::element (functor2_type (), i, size1_, j, size2_)] == value_type (), bad_index ());
                data_.insert (data_.begin () + functor1_type::element (functor2_type (), i, size1_, j, size2_), t);
            } else 
                throw external_logic ();
        }
        NUMERICS_INLINE
        void erase (size_type i, size_type j) {
            check (i < size1_, bad_index ());
            check (j < size2_, bad_index ());
            if (functor1_type::other (i, j)) {
                check (data_ [functor1_type::element (functor2_type (), i, size1_, j, size2_)] == value_type (), bad_index ());
                data_.erase (data_.begin () + functor1_type::element (functor2_type (), i, size1_, j, size2_));
            } else {
                check (data_ [functor1_type::element (functor2_type (), j, size2_, i, size1_)] == value_type (), bad_index ());
                data_.erase (data_.begin () + functor1_type::element (functor2_type (), j, size2_, i, size1_));
            }
        }
        NUMERICS_INLINE
        void clear () {
            data_.clear ();
        }

#ifdef NUMERICS_USE_INDEXED_ITERATOR
        typedef indexed_iterator1<self_type> iterator1;
        typedef indexed_iterator2<self_type> iterator2;
        typedef indexed_const_iterator1<self_type> const_iterator1;
        typedef indexed_const_iterator2<self_type> const_iterator2;
#else
        class const_iterator1;
        class iterator1;
        class const_iterator2;
        class iterator2;
#endif
#ifdef USE_MSVC
        typedef reverse_iterator1<const_iterator1, value_type, value_type> const_reverse_iterator1;
        typedef reverse_iterator1<iterator1, value_type, reference_type> reverse_iterator1;
        typedef reverse_iterator2<const_iterator2, value_type, value_type> const_reverse_iterator2;
        typedef reverse_iterator2<iterator2, value_type, reference_type> reverse_iterator2;
#else
        typedef reverse_iterator1<const_iterator1> const_reverse_iterator1;
        typedef reverse_iterator1<iterator1> reverse_iterator1;
        typedef reverse_iterator2<const_iterator2> const_reverse_iterator2;
        typedef reverse_iterator2<iterator2> reverse_iterator2;
#endif

        // Element lookup
        NUMERICS_INLINE
        const_iterator1 find1 (int rank, size_type i, size_type j) const {
            return const_iterator1 (*this, i, j);
        }
        NUMERICS_INLINE
        iterator1 find1 (int rank, size_type i, size_type j) {
            if (rank == 1 && i <= size1_ && j < size2_) 
                i = functor1_type::restrict1 (i, j);
            return iterator1 (*this, i, j);
        }
        NUMERICS_INLINE
        const_iterator2 find2 (int rank, size_type i, size_type j) const {
            return const_iterator2 (*this, i, j);
        }
        NUMERICS_INLINE
        iterator2 find2 (int rank, size_type i, size_type j) {
            if (rank == 1 && i < size1_ && j <= size2_) 
                j = functor1_type::restrict2 (i, j);
            return iterator2 (*this, i, j);
        }

        // Iterators simply are indices.

#ifndef NUMERICS_USE_INDEXED_ITERATOR
        class const_iterator1:
            public container_const_reference<hermitean_matrix>,
            public random_access_iterator_base<const_iterator1, value_type> {
        public:
            typedef packed_bidirectional_iterator_tag iterator_category;
#ifndef USE_MSVC
            typedef typename hermitean_matrix::difference_type difference_type;
            typedef typename hermitean_matrix::value_type value_type;
            typedef typename hermitean_matrix::value_type reference;
            typedef typename hermitean_matrix::const_pointer_type pointer;
#endif
            typedef const_iterator2 dual_iterator_type;
            typedef const_reverse_iterator2 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator1 ():
                container_const_reference<hermitean_matrix> (), it1_ (), it2_ () {}
            NUMERICS_INLINE
            const_iterator1 (const hermitean_matrix &m, size_type it1, size_type it2):
                container_const_reference<hermitean_matrix> (m), it1_ (it1), it2_ (it2) {}
            NUMERICS_INLINE
            const_iterator1 (const iterator1 &it):
                container_const_reference<hermitean_matrix> (it ()), it1_ (it.it1_), it2_ (it.it2_) {}

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
                return (*this) () (it1_, it2_); 
            }

            NUMERICS_INLINE
            const_iterator2 begin () const {
                return (*this) ().find2 (1, it1_, 0);
            }
            NUMERICS_INLINE
            const_iterator2 end () const {
                return (*this) ().find2 (1, it1_, (*this) ().size2 ());
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
                return it1_;
            }
            NUMERICS_INLINE
            size_type index2 () const {
                return it2_;
            }

            // Assignment 
            NUMERICS_INLINE
            const_iterator1 &operator = (const const_iterator1 &it) {
                container_const_reference<hermitean_matrix>::assign (&it ());
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
            size_type it1_;
            size_type it2_;
        };
#endif

        NUMERICS_INLINE
        const_iterator1 begin1 () const {
            return find1 (0, 0, 0);
        }
        NUMERICS_INLINE
        const_iterator1 end1 () const {
            return find1 (0, size1_, 0);
        }

#ifndef NUMERICS_USE_INDEXED_ITERATOR
        class iterator1:
            public container_reference<hermitean_matrix>,
            public random_access_iterator_base<iterator1, value_type> {
        public:
            typedef packed_bidirectional_iterator_tag iterator_category;
#ifndef USE_MSVC
            typedef typename hermitean_matrix::difference_type difference_type;
            typedef typename hermitean_matrix::value_type value_type;
            typedef typename hermitean_matrix::reference_type reference;
            typedef typename hermitean_matrix::pointer_type pointer;
#endif
            typedef iterator2 dual_iterator_type;
            typedef reverse_iterator2 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            iterator1 ():
                container_reference<hermitean_matrix> (), it1_ (), it2_ () {}
            NUMERICS_INLINE
            iterator1 (hermitean_matrix &m, size_type it1, size_type it2):
                container_reference<hermitean_matrix> (m), it1_ (it1), it2_ (it2) {}

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
            reference_type operator * () const {
                return (*this) () (it1_, it2_); 
            }

            NUMERICS_INLINE
            iterator2 begin () const {
                return (*this) ().find2 (1, it1_, 0);
            }
            NUMERICS_INLINE
            iterator2 end () const {
                return (*this) ().find2 (1, it1_, (*this) ().size2 ());
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
                return it1_;
            }
            NUMERICS_INLINE
            size_type index2 () const {
                return it2_;
            }

            // Assignment 
            NUMERICS_INLINE
            iterator1 &operator = (const iterator1 &it) {
                container_reference<hermitean_matrix>::assign (&it ());
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
            size_type it1_;
            size_type it2_;

            friend class const_iterator1;
        };
#endif

        NUMERICS_INLINE
        iterator1 begin1 () {
            return find1 (0, 0, 0);
        }
        NUMERICS_INLINE
        iterator1 end1 () {
            return find1 (0, size1_, 0);
        }

#ifndef NUMERICS_USE_INDEXED_ITERATOR
        class const_iterator2:
            public container_const_reference<hermitean_matrix>,
            public random_access_iterator_base<const_iterator2, value_type> {
        public:
            typedef packed_bidirectional_iterator_tag iterator_category;
#ifndef USE_MSVC
            typedef typename hermitean_matrix::difference_type difference_type;
            typedef typename hermitean_matrix::value_type value_type;
            typedef typename hermitean_matrix::value_type reference;
            typedef typename hermitean_matrix::const_pointer_type pointer;
#endif
            typedef const_iterator1 dual_iterator_type;
            typedef const_reverse_iterator1 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator2 ():
                container_const_reference<hermitean_matrix> (), it1_ (), it2_ () {}
            NUMERICS_INLINE
            const_iterator2 (const hermitean_matrix &m, size_type it1, size_type it2):
                container_const_reference<hermitean_matrix> (m), it1_ (it1), it2_ (it2) {}
            NUMERICS_INLINE
            const_iterator2 (const iterator2 &it):
                container_const_reference<hermitean_matrix> (it ()), it1_ (it.it1_), it2_ (it.it2_) {}

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
                return (*this) () (it1_, it2_); 
            }

            NUMERICS_INLINE
            const_iterator1 begin () const {
                return (*this) ().find1 (1, 0, it2_);
            }
            NUMERICS_INLINE
            const_iterator1 end () const {
                return (*this) ().find1 (1, (*this) ().size1 (), it2_);
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
                return it1_;
            }
            NUMERICS_INLINE
            size_type index2 () const {
                return it2_;
            }

            // Assignment 
            NUMERICS_INLINE
            const_iterator2 &operator = (const const_iterator2 &it) {
                container_const_reference<hermitean_matrix>::assign (&it ());
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
            size_type it1_;
            size_type it2_;
        };
#endif

        NUMERICS_INLINE
        const_iterator2 begin2 () const {
            return find2 (0, 0, 0);
        }
        NUMERICS_INLINE
        const_iterator2 end2 () const {
            return find2 (0, 0, size2_);
        }

#ifndef NUMERICS_USE_INDEXED_ITERATOR
        class iterator2:
            public container_reference<hermitean_matrix>,
            public random_access_iterator_base<iterator2, value_type> {
        public:
            typedef packed_bidirectional_iterator_tag iterator_category;
#ifndef USE_MSVC
            typedef typename hermitean_matrix::difference_type difference_type;
            typedef typename hermitean_matrix::value_type value_type;
            typedef typename hermitean_matrix::reference_type reference;
            typedef typename hermitean_matrix::pointer_type pointer;
#endif
            typedef iterator1 dual_iterator_type;
            typedef reverse_iterator1 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            iterator2 ():
                container_reference<hermitean_matrix> (), it1_ (), it2_ () {}
            NUMERICS_INLINE
            iterator2 (hermitean_matrix &m, size_type it1, size_type it2):
                container_reference<hermitean_matrix> (m), it1_ (it1), it2_ (it2) {}

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
            reference_type operator * () const {
                return (*this) () (it1_, it2_); 
            }

            NUMERICS_INLINE
            iterator1 begin () const {
                return (*this) ().find1 (1, 0, it2_);
            }
            NUMERICS_INLINE
            iterator1 end () const {
                return (*this) ().find1 (1, (*this) ().size1 (), it2_);
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
                return it1_;
            }
            NUMERICS_INLINE
            size_type index2 () const {
                return it2_;
            }

            // Assignment 
            NUMERICS_INLINE
            iterator2 &operator = (const iterator2 &it) {
                container_reference<hermitean_matrix>::assign (&it ());
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
            size_type it1_;
            size_type it2_;

            friend class const_iterator2;
        };
#endif

        NUMERICS_INLINE
        iterator2 begin2 () {
            return find2 (0, 0, 0);
        }
        NUMERICS_INLINE
        iterator2 end2 () {
            return find2 (0, 0, size2_);
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
        size_type size1_;
        size_type size2_;
        array_type data_;
    };

    // Hermitean matrix adaptor class 
    template<class M, class F>
    class hermitean_adaptor: 
        public matrix_expression<hermitean_adaptor<M, F> > {
    public:      
        typedef const M const_matrix_type;
        typedef M matrix_type;
        typedef F functor_type;
        typedef typename M::size_type size_type;
        typedef typename M::difference_type difference_type;
        typedef typename M::value_type value_type;
        typedef typename M::const_reference_type const_reference_type;
        typedef typename M::reference_type reference_type;
        typedef typename M::const_pointer_type const_pointer_type;
        typedef typename M::pointer_type pointer_type;
        typedef const hermitean_adaptor<M, F> const_self_type;
        typedef hermitean_adaptor<M, F> self_type;
        typedef const matrix_const_reference<const_self_type> const_closure_type;
        typedef matrix_reference<self_type> closure_type;
#ifdef NUMERICS_DEPRECATED
        typedef const matrix_row<const_self_type> const_matrix_row_type;
        typedef matrix_row<self_type> matrix_row_type;
        typedef const matrix_column<const_self_type> const_matrix_column_type;
        typedef matrix_column<self_type> matrix_column_type;
        typedef const matrix_range<const_self_type> const_matrix_range_type;
        typedef matrix_range<self_type> matrix_range_type;
#endif
        typedef packed_tag storage_category;
        typedef typename F::packed_category packed_category;
        typedef typename M::orientation_category orientation_category;

        // Construction and destruction
        NUMERICS_INLINE
        hermitean_adaptor (): 
            data_ (nil_) {}
        NUMERICS_INLINE
        hermitean_adaptor (matrix_type &data): 
            data_ (data) {}
        NUMERICS_INLINE
        hermitean_adaptor (const hermitean_adaptor &m): 
            data_ (m.data_) {}

        NUMERICS_INLINE
        size_type size1 () const { 
            return data_.size1 ();
        }
        NUMERICS_INLINE
        size_type size2 () const { 
            return data_.size2 ();
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i, size_type j) const {
            check (i < size1 (), bad_index ());
            check (j < size2 (), bad_index ());
            if (functor_type::other (i, j))
                return data_ (i, j);
            else
                return detail::conj (data_ (j, i));
        }
        NUMERICS_INLINE
        reference_type operator () (size_type i, size_type j) {
            check (i < size1 (), bad_index ());
            check (j < size2 (), bad_index ());
            if (functor_type::other (i, j))
                return data_ (i, j);
            else
                throw external_logic ();
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
#endif

        // Assignment
        NUMERICS_INLINE
        hermitean_adaptor &operator = (const hermitean_adaptor &m) { 
            matrix_assign<scalar_assign<value_type, value_type> > () (*this, m); 
            return *this;
        }
        NUMERICS_INLINE
        hermitean_adaptor &assign_temporary (hermitean_adaptor &m) { 
            *this = m;
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        hermitean_adaptor &operator = (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_assign<value_type, value_type> > () (*this, matrix<value_type> (ae)); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        hermitean_adaptor &assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        hermitean_adaptor& operator += (const matrix_expression<AE> &ae) {
            matrix_assign<scalar_assign<value_type, value_type> > () (*this, matrix<value_type> (*this + ae)); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        hermitean_adaptor &plus_assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_plus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        hermitean_adaptor& operator -= (const matrix_expression<AE> &ae) {
            matrix_assign<scalar_assign<value_type, value_type> > () (*this, matrix<value_type> (*this - ae)); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        hermitean_adaptor &minus_assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_minus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        hermitean_adaptor& operator *= (const AT &at) {
            matrix_assign_scalar<scalar_multiplies_assign<value_type, AT> > () (*this, at);
            return *this;
        }

        // Swapping
        NUMERICS_INLINE
	    void swap (hermitean_adaptor &m) {
            check (this != &m, external_logic ());
            matrix_swap<scalar_swap<value_type, value_type> > () (*this, m); 
        }
#ifndef USE_GCC
        NUMERICS_INLINE
	    friend void swap (hermitean_adaptor &m1, hermitean_adaptor &m2) {
            m1.swap (m2);
        }
#endif

#ifdef NUMERICS_USE_INDEXED_ITERATOR
        typedef indexed_iterator1<self_type> iterator1;
        typedef indexed_iterator2<self_type> iterator2;
        typedef indexed_const_iterator1<self_type> const_iterator1;
        typedef indexed_const_iterator2<self_type> const_iterator2;
#else
        class const_iterator1;
        class iterator1;
        class const_iterator2;
        class iterator2;
#endif
#ifdef USE_MSVC
        typedef reverse_iterator1<const_iterator1, value_type, value_type> const_reverse_iterator1;
        typedef reverse_iterator1<iterator1, value_type, reference_type> reverse_iterator1;
        typedef reverse_iterator2<const_iterator2, value_type, value_type> const_reverse_iterator2;
        typedef reverse_iterator2<iterator2, value_type, reference_type> reverse_iterator2;
#else
        typedef reverse_iterator1<const_iterator1> const_reverse_iterator1;
        typedef reverse_iterator1<iterator1> reverse_iterator1;
        typedef reverse_iterator2<const_iterator2> const_reverse_iterator2;
        typedef reverse_iterator2<iterator2> reverse_iterator2;
#endif

        // Element lookup
        NUMERICS_INLINE
        const_iterator1 find1 (int rank, size_type i, size_type j) const {
            return const_iterator1 (*this, i, j);
        }
        NUMERICS_INLINE
        iterator1 find1 (int rank, size_type i, size_type j) {
            if (rank == 1 && i <= size1 () && j < size2 ()) 
                i = functor_type::restrict1 (i, j);
            return iterator1 (*this, i, j);
        }
        NUMERICS_INLINE
        const_iterator2 find2 (int rank, size_type i, size_type j) const {
            return const_iterator2 (*this, i, j);
        }
        NUMERICS_INLINE
        iterator2 find2 (int rank, size_type i, size_type j) {
            if (rank == 1 && i < size1 () && j <= size2 ()) 
                j = functor_type::restrict2 (i, j);
            return iterator2 (*this, i, j);
        }

        // Iterators simply are indices.

#ifndef NUMERICS_USE_INDEXED_ITERATOR
        class const_iterator1:
            public container_const_reference<hermitean_adaptor>,
            public random_access_iterator_base<const_iterator1, value_type> {
        public:
            typedef packed_bidirectional_iterator_tag iterator_category;
#ifndef USE_MSVC
            typedef typename hermitean_adaptor::difference_type difference_type;
            typedef typename hermitean_adaptor::value_type value_type;
            typedef typename hermitean_adaptor::value_type reference;
            typedef typename hermitean_adaptor::const_pointer_type pointer;
#endif
            typedef const_iterator2 dual_iterator_type;
            typedef const_reverse_iterator2 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator1 ():
                container_const_reference<hermitean_adaptor> (), it1_ (), it2_ () {}
            NUMERICS_INLINE
            const_iterator1 (const hermitean_adaptor &m, size_type it1, size_type it2):
                container_const_reference<hermitean_adaptor> (m), it1_ (it1), it2_ (it2) {}
            NUMERICS_INLINE
            const_iterator1 (const iterator1 &it):
                container_const_reference<hermitean_adaptor> (it ()), it1_ (it.it1_), it2_ (it.it2_) {}

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
                return (*this) () (it1_, it2_); 
            }

            NUMERICS_INLINE
            const_iterator2 begin () const {
                return (*this) ().find2 (1, it1_, 0);
            }
            NUMERICS_INLINE
            const_iterator2 end () const {
                return (*this) ().find2 (1, it1_, (*this) ().size2 ());
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
                return it1_;
            }
            NUMERICS_INLINE
            size_type index2 () const {
                return it2_;
            }

            // Assignment 
            NUMERICS_INLINE
            const_iterator1 &operator = (const const_iterator1 &it) {
                container_const_reference<hermitean_adaptor>::assign (&it ());
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
            size_type it1_;
            size_type it2_;
        };
#endif

        NUMERICS_INLINE
        const_iterator1 begin1 () const {
            return find1 (0, 0, 0);
        }
        NUMERICS_INLINE
        const_iterator1 end1 () const {
            return find1 (0, size1 (), 0);
        }

#ifndef NUMERICS_USE_INDEXED_ITERATOR
        class iterator1:
            public container_reference<hermitean_adaptor>,
            public random_access_iterator_base<iterator1, value_type> {
        public:
            typedef packed_bidirectional_iterator_tag iterator_category;
#ifndef USE_MSVC
            typedef typename hermitean_adaptor::difference_type difference_type;
            typedef typename hermitean_adaptor::value_type value_type;
            typedef typename hermitean_adaptor::reference_type reference;
            typedef typename hermitean_adaptor::pointer_type pointer;
#endif
            typedef iterator2 dual_iterator_type;
            typedef reverse_iterator2 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            iterator1 ():
                container_reference<hermitean_adaptor> (), it1_ (), it2_ () {}
            NUMERICS_INLINE
            iterator1 (hermitean_adaptor &m, size_type it1, size_type it2):
                container_reference<hermitean_adaptor> (m), it1_ (it1), it2_ (it2) {}

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
            reference_type operator * () const {
                return (*this) () (it1_, it2_); 
            }

            NUMERICS_INLINE
            iterator2 begin () const {
                return (*this) ().find2 (1, it1_, 0);
            }
            NUMERICS_INLINE
            iterator2 end () const {
                return (*this) ().find2 (1, it1_, (*this) ().size2 ());
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
                return it1_;
            }
            NUMERICS_INLINE
            size_type index2 () const {
                return it2_;
            }

            // Assignment 
            NUMERICS_INLINE
            iterator1 &operator = (const iterator1 &it) {
                container_reference<hermitean_adaptor>::assign (&it ());
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
            size_type it1_;
            size_type it2_;

            friend class const_iterator1;
        };
#endif

        NUMERICS_INLINE
        iterator1 begin1 () {
            return find1 (0, 0, 0);
        }
        NUMERICS_INLINE
        iterator1 end1 () {
            return find1 (0, size1 (), 0);
        }

#ifndef NUMERICS_USE_INDEXED_ITERATOR
        class const_iterator2:
            public container_const_reference<hermitean_adaptor>,
            public random_access_iterator_base<const_iterator2, value_type> {
        public:
            typedef packed_bidirectional_iterator_tag iterator_category;
#ifndef USE_MSVC
            typedef typename hermitean_adaptor::difference_type difference_type;
            typedef typename hermitean_adaptor::value_type value_type;
            typedef typename hermitean_adaptor::value_type reference;
            typedef typename hermitean_adaptor::const_pointer_type pointer;
#endif
            typedef const_iterator1 dual_iterator_type;
            typedef const_reverse_iterator1 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator2 ():
                container_const_reference<hermitean_adaptor> (), it1_ (), it2_ () {}
            NUMERICS_INLINE
            const_iterator2 (const hermitean_adaptor &m, size_type it1, size_type it2):
                container_const_reference<hermitean_adaptor> (m), it1_ (it1), it2_ (it2) {}
            NUMERICS_INLINE
            const_iterator2 (const iterator2 &it):
                container_const_reference<hermitean_adaptor> (it ()), it1_ (it.it1_), it2_ (it.it2_) {}

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
                return (*this) () (it1_, it2_); 
            }

            NUMERICS_INLINE
            const_iterator1 begin () const {
                return (*this) ().find1 (1, 0, it2_);
            }
            NUMERICS_INLINE
            const_iterator1 end () const {
                return (*this) ().find1 (1, (*this) ().size1 (), it2_);
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
                return it1_;
            }
            NUMERICS_INLINE
            size_type index2 () const {
                return it2_;
            }

            // Assignment 
            NUMERICS_INLINE
            const_iterator2 &operator = (const const_iterator2 &it) {
                container_const_reference<hermitean_adaptor>::assign (&it ());
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
            size_type it1_;
            size_type it2_;
        };
#endif

        NUMERICS_INLINE
        const_iterator2 begin2 () const {
            return find2 (0, 0, 0);
        }
        NUMERICS_INLINE
        const_iterator2 end2 () const {
            return find2 (0, 0, size2 ());
        }

#ifndef NUMERICS_USE_INDEXED_ITERATOR
        class iterator2:
            public container_reference<hermitean_adaptor>,
            public random_access_iterator_base<iterator2, value_type> {
        public:
            typedef packed_bidirectional_iterator_tag iterator_category;
#ifndef USE_MSVC
            typedef typename hermitean_adaptor::difference_type difference_type;
            typedef typename hermitean_adaptor::value_type value_type;
            typedef typename hermitean_adaptor::reference_type reference;
            typedef typename hermitean_adaptor::pointer_type pointer;
#endif
            typedef iterator1 dual_iterator_type;
            typedef reverse_iterator1 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            iterator2 ():
                container_reference<hermitean_adaptor> (), it1_ (), it2_ () {}
            NUMERICS_INLINE
            iterator2 (hermitean_adaptor &m, size_type it1, size_type it2):
                container_reference<hermitean_adaptor> (m), it1_ (it1), it2_ (it2) {}

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
            reference_type operator * () const {
                return (*this) () (it1_, it2_); 
            }

            NUMERICS_INLINE
            iterator1 begin () const {
                return (*this) ().find1 (1, 0, it2_);
            }
            NUMERICS_INLINE
            iterator1 end () const {
                return (*this) ().find1 (1, (*this) ().size1 (), it2_);
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
                return it1_;
            }
            NUMERICS_INLINE
            size_type index2 () const {
                return it2_;
            }

            // Assignment 
            NUMERICS_INLINE
            iterator2 &operator = (const iterator2 &it) {
                container_reference<hermitean_adaptor>::assign (&it ());
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
            size_type it1_;
            size_type it2_;

            friend class const_iterator2;
        };
#endif

        NUMERICS_INLINE
        iterator2 begin2 () {
            return find2 (0, 0, 0);
        }
        NUMERICS_INLINE
        iterator2 end2 () {
            return find2 (0, 0, size2 ());
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
        static matrix_type nil_;
    };

    template<class M, class F>
    hermitean_adaptor<M, F>::matrix_type hermitean_adaptor<M, F>::nil_;

}

#endif 


