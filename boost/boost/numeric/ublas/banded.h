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

#ifndef NUMERICS_BANDED_H
#define NUMERICS_BANDED_H

#include "config.h"
#include "storage.h"
#include "vector_et.h"
#include "matrix_et.h"

// Iterators based on ideas of Jeremy Siek

namespace boost { namespace numerics {

    // Array based banded matrix class 
    template<class T, class F, class A>
    class banded_matrix: 
        public matrix_expression<banded_matrix<T, F, A> > {
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
        typedef const A const_array_type;
        typedef const banded_matrix<T, F, A> const_self_type;
        typedef banded_matrix<T, F, A> self_type;
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
        typedef typename F::orientation_category orientation_category;

        // Construction and destruction
        NUMERICS_INLINE
        banded_matrix (): 
            size1_ (0), size2_ (0),
            lower_ (0), upper_ (0), data_ (0) {}
        NUMERICS_INLINE
        banded_matrix (size_type size1, size_type size2, size_type lower = 0, size_type upper = 0): 
            size1_ (size1), size2_ (size2),
            lower_ (lower), upper_ (upper),
            data_ (std::max (size1, size2) * (lower + 1 + upper)) {}
        NUMERICS_INLINE
        banded_matrix (size_type size1, size_type size2, size_type lower, size_type upper, const array_type &data): 
            size1_ (size1), size2_ (size2),
            lower_ (lower), upper_ (upper),
            data_ (data) {}
        NUMERICS_INLINE
        banded_matrix (const banded_matrix &m): 
            size1_ (m.size1_), size2_ (m.size2_),       
            lower_ (m.lower_), upper_ (m.upper_),
            data_ (m.data_) {}
        template<class AE>
        NUMERICS_INLINE
        banded_matrix (const matrix_expression<AE> &ae, size_type lower = 0, size_type upper = 0): 
            size1_ (ae ().size1 ()), size2_ (ae ().size2 ()),
            lower_ (lower), upper_ (upper),
            data_ (std::max (ae ().size1 (), ae ().size2 ()) * (lower + 1 + upper)) {
            matrix_assign<scalar_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
        }

        // Accessors
        NUMERICS_INLINE
        size_type size1 () const { 
            return size1_;
        }
        NUMERICS_INLINE
        size_type size2 () const { 
            return size2_;
        }
        NUMERICS_INLINE
        size_type lower () const {
            return lower_;
        }
        NUMERICS_INLINE
        size_type upper () const {
            return upper_;
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
        void resize (size_type size1, size_type size2, size_type lower = 0, size_type upper = 0) {
            size1_ = size1;
            size2_ = size2;
            lower_ = lower;
            upper_ = upper;
            data ().resize (std::max (size1, size2) * (lower + 1 + upper));
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i, size_type j) const {
            check (i < size1_, bad_index ());
            check (j < size2_, bad_index ());
#ifdef NUMERICS_OWN_BANDED
            size_type k = std::max (i, j);
            size_type l = lower_ + j - i;
            if (k < std::max (size1_, size2_) &&
                l < lower_ + 1 + upper_) 
                return data () [functor_type::element (k, std::max (size1_, size2_),
                                                       l, lower_ + 1 + upper_)]; 
#else
            size_type k = j;
            size_type l = upper_ + i - j;
            if (k < size2_ &&
                l < lower_ + 1 + upper_)
                return data () [functor_type::element (k, size2_,
                                                       l, lower_ + 1 + upper_)]; 
#endif
            return value_type ();
        }
        NUMERICS_INLINE
        reference operator () (size_type i, size_type j) {
            check (i < size1_, bad_index ());
            check (j < size2_, bad_index ());
#ifdef NUMERICS_OWN_BANDED
            size_type k = std::max (i, j);
            size_type l = lower_ + j - i;
            if (k < std::max (size1_, size2_) &&
                l < lower_ + 1 + upper_) 
                return data () [functor_type::element (k, std::max (size1_, size2_),
                                                       l, lower_ + 1 + upper_)]; 
#else
            size_type k = j;
            size_type l = upper_ + i - j;
            if (k < size2_ &&
                l < lower_ + 1 + upper_) 
                return data () [functor_type::element (k, size2_,
                                                       l, lower_ + 1 + upper_)]; 
#endif
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
#endif

        // Assignment
        NUMERICS_INLINE
        banded_matrix &operator = (const banded_matrix &m) { 
            check (size1_ == m.size1_, bad_size ());
            check (size2_ == m.size2_, bad_size ());
            check (lower_ == m.lower_, bad_size ());
            check (upper_ == m.upper_, bad_size ());
            size1_ = m.size1_;
            size2_ = m.size2_;
            lower_ = m.lower_;
            upper_ = m.upper_;
            data () = m.data ();
            return *this;
        }
        NUMERICS_INLINE
        banded_matrix &assign_temporary (banded_matrix &m) { 
            swap (m);
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        banded_matrix &operator = (const matrix_expression<AE> &ae) { 
#ifdef NUMERICS_MUTABLE_TEMPORARY
            return assign_temporary (self_type (ae, lower_, upper_));
#else
            // return assign (self_type (ae, lower_, upper_));
            self_type temporary (ae, lower_, upper_);
            return assign_temporary (temporary);
#endif
        }
        template<class AE>
        NUMERICS_INLINE
        banded_matrix &reset (const matrix_expression<AE> &ae) { 
            self_type temporary (ae, lower_, upper_);
            resize (temporary.size1 (), temporary.size2 (), lower_, upper_);
            return assign_temporary (temporary);
        }
        template<class AE>
        NUMERICS_INLINE
        banded_matrix &assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        banded_matrix& operator += (const matrix_expression<AE> &ae) {
#ifdef NUMERICS_MUTABLE_TEMPORARY
            return assign_temporary (self_type (*this + ae, lower_, upper_));
#else
            // return assign (self_type (*this + ae, lower_, upper_));
            self_type temporary (*this + ae, lower_, upper_);
            return assign_temporary (temporary);
#endif
        }
        template<class AE>
        NUMERICS_INLINE
        banded_matrix &plus_assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_plus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        banded_matrix& operator -= (const matrix_expression<AE> &ae) {
#ifdef NUMERICS_MUTABLE_TEMPORARY
            return assign_temporary (self_type (*this - ae, lower_, upper_));
#else
            // return assign (self_type (*this - ae, lower_, upper_));
            self_type temporary (*this - ae, lower_, upper_);
            return assign_temporary (temporary);
#endif
        }
        template<class AE>
        NUMERICS_INLINE
        banded_matrix &minus_assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_minus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        banded_matrix& operator *= (const AT &at) {
            matrix_assign_scalar<scalar_multiplies_assign<value_type, AT> > () (*this, at);
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        banded_matrix& operator /= (const AT &at) {
            matrix_assign_scalar<scalar_divides_assign<value_type, AT> > () (*this, at);
            return *this;
        }

        // Swapping
        NUMERICS_INLINE
        void swap (banded_matrix &m) {
            check (this != &m, external_logic ());
            check (size1_ == m.size1_, bad_size ());
            check (size2_ == m.size2_, bad_size ());
            check (lower_ == m.lower_, bad_size ());
            check (upper_ == m.upper_, bad_size ());
            std::swap (size1_, m.size1_);
            std::swap (size2_, m.size2_);
            std::swap (lower_, m.lower_);
            std::swap (upper_, m.upper_);
            data ().swap (m.data ());
        }
#ifdef NUMERICS_FRIEND_FUNCTION
        NUMERICS_INLINE
        friend void swap (banded_matrix &m1, banded_matrix &m2) {
            m1.swap (m2);
        }
#endif

        // Element insertion and erasure
        // These functions should work with std::vector.
        // Thanks to Kresimir Fresl for spotting this.
        NUMERICS_INLINE
        void insert (size_type i, size_type j, const_reference t) {
            check (i < size1_, bad_index ());
            check (j < size2_, bad_index ());
#ifndef NUMERICS_USE_ET
            if (t == value_type ()) 
                return;
#endif
#ifdef NUMERICS_OWN_BANDED
            size_type k = std::max (i, j);
            size_type l = lower_ + j - i;
            check (data () [functor_type::element (k, std::max (size1_, size2_), 
                                                   l, lower_ + 1 + upper_)] == value_type (), bad_index ());
            // data ().insert (data ().begin () + functor_type::element (k, std::max (size1_, size2_), 
            //                                                           l, lower_ + 1 + upper_), t);
            data () [functor_type::element (k, std::max (size1_, size2_), 
                                            l, lower_ + 1 + upper_)] = t;
#else
            size_type k = j;
            size_type l = upper_ + i - j;
            check (data () [functor_type::element (k, size2_, 
                                                   l, lower_ + 1 + upper_)] == value_type (), bad_index ());
            // data ().insert (data ().begin () + functor_type::element (k, size2_, 
            //                                                           l, lower_ + 1 + upper_), t);
            data () [functor_type::element (k, size2_, 
                                            l, lower_ + 1 + upper_)] = t;
#endif
        }
        NUMERICS_INLINE
        void erase (size_type i, size_type j) {
            check (i < size1_, bad_index ());
            check (j < size2_, bad_index ());
#ifdef NUMERICS_OWN_BANDED
            size_type k = std::max (i, j);
            size_type l = lower_ + j - i;
            // data ().erase (data ().begin () + functor_type::element (k, std::max (size1_, size2_), 
            //                                                         l, lower_ + 1 + upper_));
            data () [functor_type::element (k, std::max (size1_, size2_), 
                                            l, lower_ + 1 + upper_)] = value_type ();
#else
            size_type k = j;
            size_type l = upper_ + i - j;
            // data ().erase (data ().begin () + functor_type::element (k, size2_, 
            //                                                          l, lower_ + 1 + upper_));
            data () [functor_type::element (k, size2_, 
                                            l, lower_ + 1 + upper_)] = value_type ();
#endif
        }
        NUMERICS_INLINE
        void clear () {
            // data ().clear ();
            std::fill (data ().begin (), data ().end (), value_type ());
        }

#ifdef NUMERICS_USE_CANONICAL_ITERATOR
        typedef matrix_row_iterator<self_type, packed_random_access_iterator_tag> iterator1;
        typedef matrix_column_iterator<self_type, packed_random_access_iterator_tag> iterator2;
        typedef matrix_row_const_iterator<self_type, packed_random_access_iterator_tag> const_iterator1;
        typedef matrix_column_const_iterator<self_type, packed_random_access_iterator_tag> const_iterator2;
#ifdef BOOST_MSVC_STD_ITERATOR
        typedef reverse_iterator<const_iterator1, typename matrix_row<self_type>, typename matrix_row<const_self_type> > const_reverse_iterator1;
        typedef reverse_iterator<iterator1, typename matrix_row<self_type>, typename matrix_row<self_type> > reverse_iterator1;
        typedef reverse_iterator<const_iterator2, typename matrix_column<self_type>, typename matrix_column<const_self_type> > const_reverse_iterator2;
        typedef reverse_iterator<iterator2, typename matrix_column<self_type>, typename matrix_column<self_type> > reverse_iterator2;
#else
        typedef reverse_iterator<const_iterator1> const_reverse_iterator1;
        typedef reverse_iterator<iterator1> reverse_iterator1;
        typedef reverse_iterator<const_iterator2> const_reverse_iterator2;
        typedef reverse_iterator<iterator2> reverse_iterator2;
#endif
#else
#ifdef NUMERICS_USE_INDEXED_ITERATOR
        typedef indexed_iterator1<self_type, packed_random_access_iterator_tag> iterator1;
        typedef indexed_iterator2<self_type, packed_random_access_iterator_tag> iterator2;
        typedef indexed_const_iterator1<self_type, packed_random_access_iterator_tag> const_iterator1;
        typedef indexed_const_iterator2<self_type, packed_random_access_iterator_tag> const_iterator2;
#else
        class const_iterator1;
        class iterator1;
        class const_iterator2;
        class iterator2;
#endif
#ifdef BOOST_MSVC_STD_ITERATOR
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
        const_iterator1 find1 (int rank, size_type i, size_type j) const {
            if (rank == 1) {
                size_type lower_i = std::max (difference_type (j - lower_), difference_type (0));
                i = std::max (i, lower_i);
                size_type upper_i = std::min (j + 1 + upper_, size1_);
                i = std::min (i, upper_i);
            }
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return const_iterator1 (*this, i);
#else
            return const_iterator1 (*this, i, j);
#endif
        }
        NUMERICS_INLINE
        iterator1 find1 (int rank, size_type i, size_type j) {
            if (rank == 1) {
                size_type lower_i = std::max (difference_type (j - lower_), difference_type (0));
                i = std::max (i, lower_i);
                size_type upper_i = std::min (j + 1 + upper_, size1_);
                i = std::min (i, upper_i);
            }
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return iterator1 (*this, i);
#else
            return iterator1 (*this, i, j);
#endif
        }
        NUMERICS_INLINE
        const_iterator2 find2 (int rank, size_type i, size_type j) const {
            if (rank == 1) {
                size_type lower_j = std::max (difference_type (i - lower_), difference_type (0));
                j = std::max (j, lower_j);
                size_type upper_j = std::min (i + 1 + upper_, size2_);
                j = std::min (j, upper_j);
            }
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return const_iterator2 (*this, j);
#else
            return const_iterator2 (*this, i, j);
#endif
        }
        NUMERICS_INLINE
        iterator2 find2 (int rank, size_type i, size_type j) {
            if (rank == 1) {
                size_type lower_j = std::max (difference_type (i - lower_), difference_type (0));
                j = std::max (j, lower_j);
                size_type upper_j = std::min (i + 1 + upper_, size2_);
                j = std::min (j, upper_j);
            }
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return iterator2 (*this, j);
#else
            return iterator2 (*this, i, j);
#endif
        }
        NUMERICS_INLINE
        const_iterator1 find_first1 (int rank, size_type i, size_type j) const {
            return find1 (rank, i, j);
        }
        NUMERICS_INLINE
        iterator1 find_first1 (int rank, size_type i, size_type j) {
            return find1 (rank, i, j);
        }
        NUMERICS_INLINE
        const_iterator1 find_last1 (int rank, size_type i, size_type j) const {
            return find1 (rank, i, j);
        }
        NUMERICS_INLINE
        iterator1 find_last1 (int rank, size_type i, size_type j) {
            return find1 (rank, i, j);
        }
        NUMERICS_INLINE
        const_iterator2 find_first2 (int rank, size_type i, size_type j) const {
            return find2 (rank, i, j);
        }
        NUMERICS_INLINE
        iterator2 find_first2 (int rank, size_type i, size_type j) {
            return find2 (rank, i, j);
        }
        NUMERICS_INLINE
        const_iterator2 find_last2 (int rank, size_type i, size_type j) const {
            return find2 (rank, i, j);
        }
        NUMERICS_INLINE
        iterator2 find_last2 (int rank, size_type i, size_type j) {
            return find2 (rank, i, j);
        }

        // Iterators simply are indices.

#if ! defined (NUMERICS_USE_CANONICAL_ITERATOR) && ! defined (NUMERICS_USE_INDEXED_ITERATOR)
        class const_iterator1:
            public container_const_reference<banded_matrix>,
            public random_access_iterator_base<packed_random_access_iterator_tag,
                                               const_iterator1, value_type> {
        public:
            typedef packed_random_access_iterator_tag iterator_category;
#ifndef BOOST_MSVC_STD_ITERATOR
            typedef typename banded_matrix::difference_type difference_type;
            typedef typename banded_matrix::value_type value_type;
            typedef typename banded_matrix::value_type reference;
            typedef typename banded_matrix::const_pointer pointer;
#endif
            typedef const_iterator2 dual_iterator_type;
            typedef const_reverse_iterator2 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator1 ():
                container_const_reference<banded_matrix> (), it1_ (), it2_ () {}
            NUMERICS_INLINE
            const_iterator1 (const banded_matrix &m, size_type it1, size_type it2):
                container_const_reference<banded_matrix> (m), it1_ (it1), it2_ (it2) {}
            NUMERICS_INLINE
            const_iterator1 (const iterator1 &it):
                container_const_reference<banded_matrix> (it ()), it1_ (it.it1_), it2_ (it.it2_) {}

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
                return (*this) ().find_first2 (1, it1_, 0);
            }
            NUMERICS_INLINE
            const_iterator2 end () const {
                return (*this) ().find_last2 (1, it1_, (*this) ().size2 ());
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
                container_const_reference<banded_matrix>::assign (&it ());
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
            return find_first1 (0, 0, 0);
        }
        NUMERICS_INLINE
        const_iterator1 end1 () const {
            return find_last1 (0, size1_, 0);
        }

#if ! defined (NUMERICS_USE_CANONICAL_ITERATOR) && ! defined (NUMERICS_USE_INDEXED_ITERATOR)
        class iterator1:
            public container_reference<banded_matrix>,
            public random_access_iterator_base<packed_random_access_iterator_tag,
                                               iterator1, value_type> {
        public:
            typedef packed_random_access_iterator_tag iterator_category;
#ifndef BOOST_MSVC_STD_ITERATOR
            typedef typename banded_matrix::difference_type difference_type;
            typedef typename banded_matrix::value_type value_type;
            typedef typename banded_matrix::reference reference;
            typedef typename banded_matrix::pointer pointer;
#endif
            typedef iterator2 dual_iterator_type;
            typedef reverse_iterator2 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            iterator1 ():
                container_reference<banded_matrix> (), it1_ (), it2_ () {}
            NUMERICS_INLINE
            iterator1 (banded_matrix &m, size_type it1, size_type it2):
                container_reference<banded_matrix> (m), it1_ (it1), it2_ (it2) {}

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
                return (*this) () (it1_, it2_); 
            }

            NUMERICS_INLINE
            iterator2 begin () const {
                return (*this) ().find_first2 (1, it1_, 0);
            }
            NUMERICS_INLINE
            iterator2 end () const {
                return (*this) ().find_last2 (1, it1_, (*this) ().size2 ());
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
                container_reference<banded_matrix>::assign (&it ());
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
            return find_first1 (0, 0, 0);
        }
        NUMERICS_INLINE
        iterator1 end1 () {
            return find_last1 (0, size1_, 0);
        }

#if ! defined (NUMERICS_USE_CANONICAL_ITERATOR) && ! defined (NUMERICS_USE_INDEXED_ITERATOR)
        class const_iterator2:
            public container_const_reference<banded_matrix>,
            public random_access_iterator_base<packed_random_access_iterator_tag,
                                               const_iterator2, value_type> {
        public:
            typedef packed_random_access_iterator_tag iterator_category;
#ifndef BOOST_MSVC_STD_ITERATOR
            typedef typename banded_matrix::difference_type difference_type;
            typedef typename banded_matrix::value_type value_type;
            typedef typename banded_matrix::value_type reference;
            typedef typename banded_matrix::const_pointer pointer;
#endif
            typedef const_iterator1 dual_iterator_type;
            typedef const_reverse_iterator1 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator2 ():
                container_const_reference<banded_matrix> (), it1_ (), it2_ () {}
            NUMERICS_INLINE
            const_iterator2 (const banded_matrix &m, size_type it1, size_type it2):
                container_const_reference<banded_matrix> (m), it1_ (it1), it2_ (it2) {}
            NUMERICS_INLINE
            const_iterator2 (const iterator2 &it):
                container_const_reference<banded_matrix> (it ()), it1_ (it.it1_), it2_ (it.it2_) {}

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
                return (*this) ().find_first1 (1, 0, it2_);
            }
            NUMERICS_INLINE
            const_iterator1 end () const {
                return (*this) ().find_last1 (1, (*this) ().size1 (), it2_);
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
                container_const_reference<banded_matrix>::assign (&it ());
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
            return find_first2 (0, 0, 0);
        }
        NUMERICS_INLINE
        const_iterator2 end2 () const {
            return find_last2 (0, 0, size2_);
        }

#if ! defined (NUMERICS_USE_CANONICAL_ITERATOR) && ! defined (NUMERICS_USE_INDEXED_ITERATOR)
        class iterator2:
            public container_reference<banded_matrix>,
            public random_access_iterator_base<packed_random_access_iterator_tag,
                                               iterator2, value_type> {
        public:
            typedef packed_random_access_iterator_tag iterator_category;
#ifndef BOOST_MSVC_STD_ITERATOR
            typedef typename banded_matrix::difference_type difference_type;
            typedef typename banded_matrix::value_type value_type;
            typedef typename banded_matrix::reference reference;
            typedef typename banded_matrix::pointer pointer;
#endif
            typedef iterator1 dual_iterator_type;
            typedef reverse_iterator1 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            iterator2 ():
                container_reference<banded_matrix> (), it1_ (), it2_ () {}
            NUMERICS_INLINE
            iterator2 (banded_matrix &m, size_type it1, size_type it2):
                container_reference<banded_matrix> (m), it1_ (it1), it2_ (it2) {}

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
                return (*this) () (it1_, it2_); 
            }

            NUMERICS_INLINE
            iterator1 begin () const {
                return (*this) ().find_first1 (1, 0, it2_);
            }
            NUMERICS_INLINE
            iterator1 end () const {
                return (*this) ().find_last1 (1, (*this) ().size1 (), it2_);
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
                container_reference<banded_matrix>::assign (&it ());
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
            return find_first2 (0, 0, 0);
        }
        NUMERICS_INLINE
        iterator2 end2 () {
            return find_last2 (0, 0, size2_);
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
        size_type lower_;
        size_type upper_;
        array_type data_;
    };

    // Banded matrix adaptor class 
    template<class M>
    class banded_adaptor:
        public matrix_expression<banded_adaptor<M> > {
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
        typedef const banded_adaptor<M> const_self_type;
        typedef banded_adaptor<M> self_type;
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
        typedef typename storage_restrict_traits<typename M::storage_category,
                                                 packed_proxy_tag>::storage_category storage_category;
        typedef typename M::orientation_category orientation_category;

        // Construction and destruction
        NUMERICS_INLINE
        banded_adaptor (): 
            data_ (nil_), lower_ (0), upper_ (0) {}
        NUMERICS_INLINE
        banded_adaptor (matrix_type &data, size_type lower = 0, size_type upper = 0): 
            data_ (data), lower_ (lower), upper_ (upper) {}
        NUMERICS_INLINE
        banded_adaptor (const banded_adaptor &m): 
            data_ (m.data_), lower_ (m.lower_), upper_ (m.upper_) {}

        // Accessors
        NUMERICS_INLINE
        size_type size1 () const { 
            return data_.size1 ();
        }
        NUMERICS_INLINE
        size_type size2 () const { 
            return data_.size2 ();
        }
        NUMERICS_INLINE
        size_type lower () const {
            return lower_;
        }
        NUMERICS_INLINE
        size_type upper () const {
            return upper_;
        }
        NUMERICS_INLINE
        const_matrix_type &data () const {
            return data_;
        }
        NUMERICS_INLINE
        matrix_type &data () {
            return data_;
        }

        // Resetting
        NUMERICS_INLINE
        void reset (matrix_type &data, size_type lower = 0, size_type upper = 0) {
            data () = data;
            lower_ = lower;
            upper_ = upper;
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i, size_type j) const {
            check (i < size1 (), bad_index ());
            check (j < size2 (), bad_index ());
#ifdef NUMERICS_OWN_BANDED
            size_type k = std::max (i, j);
            size_type l = lower_ + j - i;
            if (k < std::max (size1 (), size2 ()) &&
                l < lower_ + 1 + upper_)
                return data () (i, j);
#else
            size_type k = j;
            size_type l = upper_ + i - j;
            if (k < size2 () &&
                l < lower_ + 1 + upper_)
                return data () (i, j);
#endif
            return value_type ();
        }
        NUMERICS_INLINE
        reference operator () (size_type i, size_type j) {
            check (i < size1 (), bad_index ());
            check (j < size2 (), bad_index ());
#ifdef NUMERICS_OWN_BANDED
            size_type k = std::max (i, j);
            size_type l = lower_ + j - i;
            if (k < std::max (size1 (), size2 ()) &&
                l < lower_ + 1 + upper_) 
                return data () (i, j); 
#else
            size_type k = j;
            size_type l = upper_ + i - j;
            if (k < size2 () &&
                l < lower_ + 1 + upper_) 
                return data () (i, j); 
#endif
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
#endif

        // Assignment
        NUMERICS_INLINE
        banded_adaptor &operator = (const banded_adaptor &m) { 
            matrix_assign<scalar_assign<value_type, value_type> > () (*this, m); 
            return *this;
        }
        NUMERICS_INLINE
        banded_adaptor &assign_temporary (banded_adaptor &m) { 
            *this = m;
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        banded_adaptor &operator = (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_assign<value_type, value_type> > () (*this, matrix<value_type> (ae)); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        banded_adaptor &assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        banded_adaptor& operator += (const matrix_expression<AE> &ae) {
            matrix_assign<scalar_assign<value_type, value_type> > () (*this, matrix<value_type> (*this + ae)); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        banded_adaptor &plus_assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_plus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        banded_adaptor& operator -= (const matrix_expression<AE> &ae) {
            matrix_assign<scalar_assign<value_type, value_type> > () (*this, matrix<value_type> (*this - ae)); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        banded_adaptor &minus_assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_minus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        banded_adaptor& operator *= (const AT &at) {
            matrix_assign_scalar<scalar_multiplies_assign<value_type, AT> > () (*this, at);
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        banded_adaptor& operator /= (const AT &at) {
            matrix_assign_scalar<scalar_divides_assign<value_type, AT> > () (*this, at);
            return *this;
        }

        // Swapping
        NUMERICS_INLINE
        void swap (banded_adaptor &m) {
            check (this != &m, external_logic ());
            check (lower_ == m.lower_, bad_size ());
            check (upper_ == m.upper_, bad_size ());
            matrix_swap<scalar_swap<value_type, value_type> > () (*this, m); 
        }
#ifdef NUMERICS_FRIEND_FUNCTION
        NUMERICS_INLINE
        friend void swap (banded_adaptor &m1, banded_adaptor &m2) {
            m1.swap (m2);
        }
#endif

#ifdef NUMERICS_USE_CANONICAL_ITERATOR
        typedef matrix_row_iterator<self_type, packed_random_access_iterator_tag> iterator1;
        typedef matrix_column_iterator<self_type, packed_random_access_iterator_tag> iterator2;
        typedef matrix_row_const_iterator<self_type, packed_random_access_iterator_tag> const_iterator1;
        typedef matrix_column_const_iterator<self_type, packed_random_access_iterator_tag> const_iterator2;
#ifdef BOOST_MSVC_STD_ITERATOR
        typedef reverse_iterator<const_iterator1, typename matrix_row<self_type>, typename matrix_row<const_self_type> > const_reverse_iterator1;
        typedef reverse_iterator<iterator1, typename matrix_row<self_type>, typename matrix_row<self_type> > reverse_iterator1;
        typedef reverse_iterator<const_iterator2, typename matrix_column<self_type>, typename matrix_column<const_self_type> > const_reverse_iterator2;
        typedef reverse_iterator<iterator2, typename matrix_column<self_type>, typename matrix_column<self_type> > reverse_iterator2;
#else
        typedef reverse_iterator<const_iterator1> const_reverse_iterator1;
        typedef reverse_iterator<iterator1> reverse_iterator1;
        typedef reverse_iterator<const_iterator2> const_reverse_iterator2;
        typedef reverse_iterator<iterator2> reverse_iterator2;
#endif
#else
#ifdef NUMERICS_USE_INDEXED_ITERATOR
        typedef indexed_iterator1<self_type, packed_random_access_iterator_tag> iterator1;
        typedef indexed_iterator2<self_type, packed_random_access_iterator_tag> iterator2;
        typedef indexed_const_iterator1<self_type, packed_random_access_iterator_tag> const_iterator1;
        typedef indexed_const_iterator2<self_type, packed_random_access_iterator_tag> const_iterator2;
#else
        class const_iterator1;
        class iterator1;
        class const_iterator2;
        class iterator2;
#endif
#ifdef BOOST_MSVC_STD_ITERATOR
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
        const_iterator1 find1 (int rank, size_type i, size_type j) const {
            if (rank == 1) {
                size_type lower_i = std::max (difference_type (j - lower_), difference_type (0));
                i = std::max (i, lower_i);
                size_type upper_i = std::min (j + 1 + upper_, size1 ());
                i = std::min (i, upper_i);
            }
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return const_iterator1 (*this, i);
#else
            return const_iterator1 (*this, i, j);
#endif
        }
        NUMERICS_INLINE
        iterator1 find1 (int rank, size_type i, size_type j) {
            if (rank == 1) {
                size_type lower_i = std::max (difference_type (j - lower_), difference_type (0));
                i = std::max (i, lower_i);
                size_type upper_i = std::min (j + 1 + upper_, size1 ());
                i = std::min (i, upper_i);
            }
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return iterator1 (*this, i);
#else
            return iterator1 (*this, i, j);
#endif
        }
        NUMERICS_INLINE
        const_iterator2 find2 (int rank, size_type i, size_type j) const {
            if (rank == 1) {
                size_type lower_j = std::max (difference_type (i - lower_), difference_type (0));
                j = std::max (j, lower_j);
                size_type upper_j = std::min (i + 1 + upper_, size2 ());
                j = std::min (j, upper_j);
            }
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return const_iterator2 (*this, j);
#else
            return const_iterator2 (*this, i, j);
#endif
        }
        NUMERICS_INLINE
        iterator2 find2 (int rank, size_type i, size_type j) {
            if (rank == 1) {
                size_type lower_j = std::max (difference_type (i - lower_), difference_type (0));
                j = std::max (j, lower_j);
                size_type upper_j = std::min (i + 1 + upper_, size2 ());
                j = std::min (j, upper_j);
            }
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return iterator2 (*this, j);
#else
            return iterator2 (*this, i, j);
#endif
        }
        NUMERICS_INLINE
        const_iterator1 find_first1 (int rank, size_type i, size_type j) const {
            return find1 (rank, i, j);
        }
        NUMERICS_INLINE
        iterator1 find_first1 (int rank, size_type i, size_type j) {
            return find1 (rank, i, j);
        }
        NUMERICS_INLINE
        const_iterator1 find_last1 (int rank, size_type i, size_type j) const {
            return find1 (rank, i, j);
        }
        NUMERICS_INLINE
        iterator1 find_last1 (int rank, size_type i, size_type j) {
            return find1 (rank, i, j);
        }
        NUMERICS_INLINE
        const_iterator2 find_first2 (int rank, size_type i, size_type j) const {
            return find2 (rank, i, j);
        }
        NUMERICS_INLINE
        iterator2 find_first2 (int rank, size_type i, size_type j) {
            return find2 (rank, i, j);
        }
        NUMERICS_INLINE
        const_iterator2 find_last2 (int rank, size_type i, size_type j) const {
            return find2 (rank, i, j);
        }
        NUMERICS_INLINE
        iterator2 find_last2 (int rank, size_type i, size_type j) {
            return find2 (rank, i, j);
        }

        // Iterators simply are indices.

#if ! defined (NUMERICS_USE_CANONICAL_ITERATOR) && ! defined (NUMERICS_USE_INDEXED_ITERATOR)
        class const_iterator1:
            public container_const_reference<banded_adaptor>,
            public random_access_iterator_base<packed_random_access_iterator_tag,
                                               const_iterator1, value_type> {
        public:
            typedef packed_random_access_iterator_tag iterator_category;
#ifndef BOOST_MSVC_STD_ITERATOR
            typedef typename banded_adaptor::difference_type difference_type;
            typedef typename banded_adaptor::value_type value_type;
            typedef typename banded_adaptor::value_type reference;
            typedef typename banded_adaptor::const_pointer pointer;
#endif
            typedef const_iterator2 dual_iterator_type;
            typedef const_reverse_iterator2 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator1 ():
                container_const_reference<banded_adaptor> (), it1_ (), it2_ () {}
            NUMERICS_INLINE
            const_iterator1 (const banded_adaptor &m, size_type it1, size_type it2):
                container_const_reference<banded_adaptor> (m), it1_ (it1), it2_ (it2) {}
            NUMERICS_INLINE
            const_iterator1 (const iterator1 &it):
                container_const_reference<banded_adaptor> (it ()), it1_ (it.it1_), it2_ (it.it2_) {}

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
                return (*this) ().find_first2 (1, it1_, 0);
            }
            NUMERICS_INLINE
            const_iterator2 end () const {
                return (*this) ().find_last2 (1, it1_, (*this) ().size2 ());
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
                container_const_reference<banded_adaptor>::assign (&it ());
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
            return find_first1 (0, 0, 0);
        }
        NUMERICS_INLINE
        const_iterator1 end1 () const {
            return find_last1 (0, size1 (), 0);
        }

#if ! defined (NUMERICS_USE_CANONICAL_ITERATOR) && ! defined (NUMERICS_USE_INDEXED_ITERATOR)
        class iterator1:
            public container_reference<banded_adaptor>,
            public random_access_iterator_base<packed_random_access_iterator_tag,
                                               iterator1, value_type> {
        public:
            typedef packed_random_access_iterator_tag iterator_category;
#ifndef BOOST_MSVC_STD_ITERATOR
            typedef typename banded_adaptor::difference_type difference_type;
            typedef typename banded_adaptor::value_type value_type;
            typedef typename banded_adaptor::reference reference;
            typedef typename banded_adaptor::pointer pointer;
#endif
            typedef iterator2 dual_iterator_type;
            typedef reverse_iterator2 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            iterator1 ():
                container_reference<banded_adaptor> (), it1_ (), it2_ () {}
            NUMERICS_INLINE
            iterator1 (banded_adaptor &m, size_type it1, size_type it2):
                container_reference<banded_adaptor> (m), it1_ (it1), it2_ (it2) {}

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
                return (*this) () (it1_, it2_); 
            }

            NUMERICS_INLINE
            iterator2 begin () const {
                return (*this) ().find_first2 (1, it1_, 0);
            }
            NUMERICS_INLINE
            iterator2 end () const {
                return (*this) ().find_last2 (1, it1_, (*this) ().size2 ());
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
                container_reference<banded_adaptor>::assign (&it ());
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
            return find_first1 (0, 0, 0);
        }
        NUMERICS_INLINE
        iterator1 end1 () {
            return find_last1 (0, size1 (), 0);
        }

#if ! defined (NUMERICS_USE_CANONICAL_ITERATOR) && ! defined (NUMERICS_USE_INDEXED_ITERATOR)
        class const_iterator2:
            public container_const_reference<banded_adaptor>,
            public random_access_iterator_base<packed_random_access_iterator_tag,
                                               const_iterator2, value_type> {
        public:
            typedef packed_random_access_iterator_tag iterator_category;
#ifndef BOOST_MSVC_STD_ITERATOR
            typedef typename banded_adaptor::difference_type difference_type;
            typedef typename banded_adaptor::value_type value_type;
            typedef typename banded_adaptor::value_type reference;
            typedef typename banded_adaptor::const_pointer pointer;
#endif
            typedef const_iterator1 dual_iterator_type;
            typedef const_reverse_iterator1 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator2 ():
                container_const_reference<banded_adaptor> (), it1_ (), it2_ () {}
            NUMERICS_INLINE
            const_iterator2 (const banded_adaptor &m, size_type it1, size_type it2):
                container_const_reference<banded_adaptor> (m), it1_ (it1), it2_ (it2) {}
            NUMERICS_INLINE
            const_iterator2 (const iterator2 &it):
                container_const_reference<banded_adaptor> (it ()), it1_ (it.it1_), it2_ (it.it2_) {}

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
                return (*this) ().find_first1 (1, 0, it2_);
            }
            NUMERICS_INLINE
            const_iterator1 end () const {
                return (*this) ().find_last1 (1, (*this) ().size1 (), it2_);
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
                container_const_reference<banded_adaptor>::assign (&it ());
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
            return find_first2 (0, 0, 0);
        }
        NUMERICS_INLINE
        const_iterator2 end2 () const {
            return find_last2 (0, 0, size2 ());
        }

#if ! defined (NUMERICS_USE_CANONICAL_ITERATOR) && ! defined (NUMERICS_USE_INDEXED_ITERATOR)
        class iterator2:
            public container_reference<banded_adaptor>,
            public random_access_iterator_base<packed_random_access_iterator_tag,
                                               iterator2, value_type> {
        public:
            typedef packed_random_access_iterator_tag iterator_category;
#ifndef BOOST_MSVC_STD_ITERATOR
            typedef typename banded_adaptor::difference_type difference_type;
            typedef typename banded_adaptor::value_type value_type;
            typedef typename banded_adaptor::reference reference;
            typedef typename banded_adaptor::pointer pointer;
#endif
            typedef iterator1 dual_iterator_type;
            typedef reverse_iterator1 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            iterator2 ():
                container_reference<banded_adaptor> (), it1_ (), it2_ () {}
            NUMERICS_INLINE
            iterator2 (banded_adaptor &m, size_type it1, size_type it2):
                container_reference<banded_adaptor> (m), it1_ (it1), it2_ (it2) {}

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
                return (*this) () (it1_, it2_); 
            }

            NUMERICS_INLINE
            iterator1 begin () const {
                return (*this) ().find_first1 (1, 0, it2_);
            }
            NUMERICS_INLINE
            iterator1 end () const {
                return (*this) ().find_last1 (1, (*this) ().size1 (), it2_);
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
                container_reference<banded_adaptor>::assign (&it ());
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
            return find_first2 (0, 0, 0);
        }
        NUMERICS_INLINE
        iterator2 end2 () {
            return find_last2 (0, 0, size2 ());
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
        size_type lower_;
        size_type upper_;
        static matrix_type nil_;
    };

    template<class M>
    typename banded_adaptor<M>::matrix_type banded_adaptor<M>::nil_;

}}

#endif






