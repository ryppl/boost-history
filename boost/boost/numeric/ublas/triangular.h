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

#ifndef NUMERICS_TRIANGULAR_H
#define NUMERICS_TRIANGULAR_H

#include "config.h"
#include "storage.h"
#include "vector_et.h"
#include "matrix_et.h"

// Iterators based on ideas of Jeremy Siek

namespace boost { namespace numerics {

    struct lower {
        typedef std::size_t size_type;
        typedef lower_tag packed_category;

        static
        NUMERICS_INLINE
        size_type packed_size (size_type size1, size_type size2) {
            size_type size = std::max (size1, size2);
            return ((size + 1) * size) / 2;
        }

        static 
        NUMERICS_INLINE
        bool zero (size_type i, size_type j) {
            return j > i;
        }
        static 
        NUMERICS_INLINE
        bool one (size_type i, size_type j) {
            return false;
        }
        static 
        NUMERICS_INLINE
        bool other (size_type i, size_type j) {
            return j <= i;
        }
        template<class F>
        static 
        NUMERICS_INLINE
        size_type element (F, size_type i, size_type size1, size_type j, size_type size2) {
            return F::lower_element (i, size1, j, size2);
        }

        static 
        NUMERICS_INLINE
        size_type restrict1 (size_type i, size_type j) {
            return std::max (i, j);
        }
        static 
        NUMERICS_INLINE
        size_type restrict2 (size_type i, size_type j) {
            return std::min (i + 1, j);
        }
        static 
        NUMERICS_INLINE
        size_type mutable_restrict1 (size_type i, size_type j) {
            return std::max (i, j);
        }
        static 
        NUMERICS_INLINE
        size_type mutable_restrict2 (size_type i, size_type j) {
            return std::min (i + 1, j);
        }
    };
    struct upper {
        typedef std::size_t size_type;
        typedef upper_tag packed_category;

        static
        NUMERICS_INLINE
        size_type packed_size (size_type size1, size_type size2) {
            size_type size = std::max (size1, size2);
            return ((size + 1) * size) / 2;
        }

        static 
        NUMERICS_INLINE
        bool zero (size_type i, size_type j) {
            return j < i;
        }
        static 
        NUMERICS_INLINE
        bool one (size_type i, size_type j) {
            return false;
        }
        static 
        NUMERICS_INLINE
        bool other (size_type i, size_type j) {
            return j >= i;
        }
        template<class F>
        static 
        NUMERICS_INLINE
        size_type element (F, size_type i, size_type size1, size_type j, size_type size2) {
            return F::upper_element (i, size1, j, size2);
        }

        static 
        NUMERICS_INLINE
        size_type restrict1 (size_type i, size_type j) {
            return std::min (i, j + 1);
        }
        static 
        NUMERICS_INLINE
        size_type restrict2 (size_type i, size_type j) {
            return std::max (i, j);
        }
        static 
        NUMERICS_INLINE
        size_type mutable_restrict1 (size_type i, size_type j) {
            return std::min (i, j + 1);
        }
        static 
        NUMERICS_INLINE
        size_type mutable_restrict2 (size_type i, size_type j) {
            return std::max (i, j);
        }
    };
    struct unit_lower {
        typedef std::size_t size_type;
        typedef unit_lower_tag packed_category;

        static
        NUMERICS_INLINE
        size_type packed_size (size_type size1, size_type size2) {
            size_type size = std::max (size1, size2);
            return ((size + 1) * size) / 2;
        }

        static 
        NUMERICS_INLINE
        bool zero (size_type i, size_type j) {
            return j > i;
        }
        static 
        NUMERICS_INLINE
        bool one (size_type i, size_type j) {
            return j == i;
        }
        static 
        NUMERICS_INLINE
        bool other (size_type i, size_type j) {
            return j < i;
        }
        template<class F>
        static 
        NUMERICS_INLINE
        size_type element (F, size_type i, size_type size1, size_type j, size_type size2) {
            return F::lower_element (i, size1, j, size2);
        }

        static 
        NUMERICS_INLINE
        size_type restrict1 (size_type i, size_type j) {
            return std::max (i, j);
        }
        static 
        NUMERICS_INLINE
        size_type restrict2 (size_type i, size_type j) {
            return std::min (i + 1, j);
        }
        static 
        NUMERICS_INLINE
        size_type mutable_restrict1 (size_type i, size_type j) {
            return std::max (i, j);
        }
        static 
        NUMERICS_INLINE
        size_type mutable_restrict2 (size_type i, size_type j) {
            return std::min (i, j);
        }
    };
    struct unit_upper {
        typedef std::size_t size_type;
        typedef unit_upper_tag packed_category;

        static
        NUMERICS_INLINE
        size_type packed_size (size_type size1, size_type size2) {
            size_type size = std::max (size1, size2);
            return ((size + 1) * size) / 2;
        }

        static 
        NUMERICS_INLINE
        bool zero (size_type i, size_type j) {
            return j < i;
        }
        static 
        NUMERICS_INLINE
        bool one (size_type i, size_type j) {
            return j == i;
        }
        static 
        NUMERICS_INLINE
        bool other (size_type i, size_type j) {
            return j > i;
        }
        template<class F>
        static 
        NUMERICS_INLINE
        size_type element (F, size_type i, size_type size1, size_type j, size_type size2) {
            return F::upper_element (i, size1, j, size2);
        }

        static 
        NUMERICS_INLINE
        size_type restrict1 (size_type i, size_type j) {
            return std::min (i, j + 1);
        }
        static 
        NUMERICS_INLINE
        size_type restrict2 (size_type i, size_type j) {
            return std::max (i, j);
        }
        static 
        NUMERICS_INLINE
        size_type mutable_restrict1 (size_type i, size_type j) {
            return std::min (i, j);
        }
        static 
        NUMERICS_INLINE
        size_type mutable_restrict2 (size_type i, size_type j) {
            return std::max (i, j);
        }
    };

    // Array based triangular matrix class 
    template<class T, class F1, class F2, class A>
    class triangular_matrix: 
        public matrix_expression<triangular_matrix<T, F1, F2, A> > {
    public:      
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef const T &const_reference;
        typedef T &reference;
        typedef const T *const_pointer;
        typedef T *pointer;
        typedef F1 functor1_type;
        typedef F2 functor2_type;
        typedef A array_type;
        typedef const A const_array_type;
        typedef const triangular_matrix<T, F1, F2, A> const_self_type;
        typedef triangular_matrix<T, F1, F2, A> self_type;
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
        typedef packed_proxy_tag storage_category;
        typedef typename F1::packed_category packed_category;
        typedef typename F2::orientation_category orientation_category;

        // Construction and destruction
        NUMERICS_INLINE
        triangular_matrix (): 
            size1_ (0), size2_ (0),
            data_ (0) {}
        NUMERICS_INLINE
        triangular_matrix (size_type size1, size_type size2): 
            size1_ (size1), size2_ (size2),
            data_ (functor1_type::packed_size (size1, size2)) {}
        NUMERICS_INLINE
        triangular_matrix (size_type size1, size_type size2, const array_type &data): 
            size1_ (size1), size2_ (size2),
            data_ (data) {}
        NUMERICS_INLINE
        triangular_matrix (const triangular_matrix &m): 
            size1_ (m.size1_), size2_ (m.size2_),
            data_ (m.data_) {}
        template<class AE>
        NUMERICS_INLINE
        triangular_matrix (const matrix_expression<AE> &ae): 
            size1_ (ae ().size1 ()), size2_ (ae ().size2 ()),
            data_ (functor1_type::packed_size (ae ().size1 (), ae ().size2 ())) {
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
        const_array_type &data () const {
            return data_;
        }
        NUMERICS_INLINE
        array_type &data () {
            return data_;
        }

        // Resizing
        NUMERICS_INLINE
        void resize (size_type size1, size_type size2) {
            size1_ = size1;
            size2_ = size2;
            data ().resize (functor1_type::packed_size (size1, size2));
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i, size_type j) const {
            check (i < size1_, bad_index ());
            check (j < size2_, bad_index ());
            if (functor1_type::other (i, j))
                return data () [functor1_type::element (functor2_type (), i, size1_, j, size2_)];
            else if (functor1_type::one (i, j))
                return value_type (1);
            else
                return value_type (0);
        }
        NUMERICS_INLINE
        reference operator () (size_type i, size_type j) {
            check (i < size1_, bad_index ());
            check (j < size2_, bad_index ());
            if (functor1_type::other (i, j))
                return data () [functor1_type::element (functor2_type (), i, size1_, j, size2_)];
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
#endif

        // Assignment
        NUMERICS_INLINE
        triangular_matrix &operator = (const triangular_matrix &m) { 
            check (size1_ == m.size1_, bad_size ());
            check (size2_ == m.size2_, bad_size ());
            size1_ = m.size1_;
            size2_ = m.size2_;
            data () = m.data ();
            return *this;
        }
        NUMERICS_INLINE
        triangular_matrix &assign_temporary (triangular_matrix &m) { 
            swap (m);
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        triangular_matrix &operator = (const matrix_expression<AE> &ae) { 
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
        triangular_matrix &reset (const matrix_expression<AE> &ae) {
            self_type temporary (ae);
            resize (temporary.size1 (), temporary.size2 ());
            return assign_temporary (temporary);
        }
        template<class AE>
        NUMERICS_INLINE
        triangular_matrix &assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        triangular_matrix& operator += (const matrix_expression<AE> &ae) {
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
        triangular_matrix &plus_assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_plus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        triangular_matrix& operator -= (const matrix_expression<AE> &ae) {
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
        triangular_matrix &minus_assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_minus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        triangular_matrix& operator *= (const AT &at) {
            matrix_assign_scalar<scalar_multiplies_assign<value_type, AT> > () (*this, at);
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        triangular_matrix& operator /= (const AT &at) {
            matrix_assign_scalar<scalar_divides_assign<value_type, AT> > () (*this, at);
            return *this;
        }

        // Swapping
        NUMERICS_INLINE
        void swap (triangular_matrix &m) {
            check (this != &m, external_logic ());
            check (size1_ == m.size1_, bad_size ());
            check (size2_ == m.size2_, bad_size ());
            std::swap (size1_, m.size1_);
            std::swap (size2_, m.size2_);
            data ().swap (m.data ());
        }
#ifdef NUMERICS_FRIEND_FUNCTION
        NUMERICS_INLINE
        friend void swap (triangular_matrix &m1, triangular_matrix &m2) {
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
            if (functor1_type::other (i, j)) {
                check (data () [functor1_type::element (functor2_type (), i, size1_, j, size2_)] == value_type (), bad_index ());
                // data ().insert (data ().begin () + functor1_type::element (functor2_type (), i, size1_, j, size2_), t);
                data () [functor1_type::element (functor2_type (), i, size1_, j, size2_)] = t;
            } else
                throw external_logic ();
        }
        NUMERICS_INLINE
        void erase (size_type i, size_type j) {
            check (i < size1_, bad_index ());
            check (j < size2_, bad_index ());
            if (functor1_type::other (i, j)) 
                // data ().erase (data ().begin () + functor1_type::element (functor2_type (), i, size1_, j, size2_));
                data () [functor1_type::element (functor2_type (), i, size1_, j, size2_)] = value_type ();
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
        const_iterator1 find_first1 (int rank, size_type i, size_type j) const {
            if (rank == 1) 
                i = functor1_type::restrict1 (i, j);
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return const_iterator1 (*this, i);
#else
            return const_iterator1 (*this, i, j);
#endif
        }
        NUMERICS_INLINE
        iterator1 find_first1 (int rank, size_type i, size_type j) {
            if (rank == 1) 
                i = functor1_type::mutable_restrict1 (i, j);
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return iterator1 (*this, i);
#else
            return iterator1 (*this, i, j);
#endif
        }
        NUMERICS_INLINE
        const_iterator1 find_last1 (int rank, size_type i, size_type j) const {
            if (rank == 1) 
                i = functor1_type::restrict1 (i, j);
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return const_iterator1 (*this, i);
#else
            return const_iterator1 (*this, i, j);
#endif
        }
        NUMERICS_INLINE
        iterator1 find_last1 (int rank, size_type i, size_type j) {
            if (rank == 1) 
                i = functor1_type::mutable_restrict1 (i, j);
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return iterator1 (*this, i);
#else
            return iterator1 (*this, i, j);
#endif
        }
        NUMERICS_INLINE
        const_iterator2 find_first2 (int rank, size_type i, size_type j) const {
            if (rank == 1) 
                j = functor1_type::restrict2 (i, j);
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return const_iterator2 (*this, j);
#else
            return const_iterator2 (*this, i, j);
#endif
        }
        NUMERICS_INLINE
        iterator2 find_first2 (int rank, size_type i, size_type j) {
            if (rank == 1) 
                j = functor1_type::mutable_restrict2 (i, j);
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return iterator2 (*this, j);
#else
            return iterator2 (*this, i, j);
#endif
        }
        NUMERICS_INLINE
        const_iterator2 find_last2 (int rank, size_type i, size_type j) const {
            if (rank == 1) 
                j = functor1_type::restrict2 (i, j);
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return const_iterator2 (*this, j);
#else
            return const_iterator2 (*this, i, j);
#endif
        }
        NUMERICS_INLINE
        iterator2 find_last2 (int rank, size_type i, size_type j) {
            if (rank == 1) 
                j = functor1_type::mutable_restrict2 (i, j);
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return iterator2 (*this, j);
#else
            return iterator2 (*this, i, j);
#endif
        }

        // Iterators simply are indices.

#if ! defined (NUMERICS_USE_CANONICAL_ITERATOR) && ! defined (NUMERICS_USE_INDEXED_ITERATOR)
        class const_iterator1:
            public container_const_reference<triangular_matrix>,
            public random_access_iterator_base<packed_random_access_iterator_tag,
                                               const_iterator1, value_type> {
        public:
            typedef packed_random_access_iterator_tag iterator_category;
#ifndef BOOST_MSVC_STD_ITERATOR
            typedef typename triangular_matrix::difference_type difference_type;
            typedef typename triangular_matrix::value_type value_type;
            typedef typename triangular_matrix::value_type reference;
            typedef typename triangular_matrix::const_pointer pointer;
#endif
            typedef const_iterator2 dual_iterator_type;
            typedef const_reverse_iterator2 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator1 ():
                container_const_reference<triangular_matrix> (), it1_ (), it2_ () {}
            NUMERICS_INLINE
            const_iterator1 (const triangular_matrix &m, size_type it1, size_type it2):
                container_const_reference<triangular_matrix> (m), it1_ (it1), it2_ (it2) {}
            NUMERICS_INLINE
            const_iterator1 (const iterator1 &it):
                container_const_reference<triangular_matrix> (it ()), it1_ (it.it1_), it2_ (it.it2_) {}

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
                container_const_reference<triangular_matrix>::assign (&it ());
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
            public container_reference<triangular_matrix>,
            public random_access_iterator_base<packed_random_access_iterator_tag,
                                               iterator1, value_type> {
        public:
            typedef packed_random_access_iterator_tag iterator_category;
#ifndef BOOST_MSVC_STD_ITERATOR
            typedef typename triangular_matrix::difference_type difference_type;
            typedef typename triangular_matrix::value_type value_type;
            typedef typename triangular_matrix::reference reference;
            typedef typename triangular_matrix::pointer pointer;
#endif
            typedef iterator2 dual_iterator_type;
            typedef reverse_iterator2 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            iterator1 ():
                container_reference<triangular_matrix> (), it1_ (), it2_ () {}
            NUMERICS_INLINE
            iterator1 (triangular_matrix &m, size_type it1, size_type it2):
                container_reference<triangular_matrix> (m), it1_ (it1), it2_ (it2) {}

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
                container_reference<triangular_matrix>::assign (&it ());
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
            public container_const_reference<triangular_matrix>,
            public random_access_iterator_base<packed_random_access_iterator_tag,
                                               const_iterator2, value_type> {
        public:
            typedef packed_random_access_iterator_tag iterator_category;
#ifndef BOOST_MSVC_STD_ITERATOR
            typedef typename triangular_matrix::difference_type difference_type;
            typedef typename triangular_matrix::value_type value_type;
            typedef typename triangular_matrix::value_type reference;
            typedef typename triangular_matrix::const_pointer pointer;
#endif
            typedef const_iterator1 dual_iterator_type;
            typedef const_reverse_iterator1 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator2 ():
                container_const_reference<triangular_matrix> (), it1_ (), it2_ () {}
            NUMERICS_INLINE
            const_iterator2 (const triangular_matrix &m, size_type it1, size_type it2):
                container_const_reference<triangular_matrix> (m), it1_ (it1), it2_ (it2) {}
            NUMERICS_INLINE
            const_iterator2 (const iterator2 &it):
                container_const_reference<triangular_matrix> (it ()), it1_ (it.it1_), it2_ (it.it2_) {}

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
                container_const_reference<triangular_matrix>::assign (&it ());
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
            public container_reference<triangular_matrix>,
            public random_access_iterator_base<packed_random_access_iterator_tag,
                                               iterator2, value_type> {
        public:
            typedef packed_random_access_iterator_tag iterator_category;
#ifndef BOOST_MSVC_STD_ITERATOR
            typedef typename triangular_matrix::difference_type difference_type;
            typedef typename triangular_matrix::value_type value_type;
            typedef typename triangular_matrix::reference reference;
            typedef typename triangular_matrix::pointer pointer;
#endif
            typedef iterator1 dual_iterator_type;
            typedef reverse_iterator1 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            iterator2 ():
                container_reference<triangular_matrix> (), it1_ (), it2_ () {}
            NUMERICS_INLINE
            iterator2 (triangular_matrix &m, size_type it1, size_type it2):
                container_reference<triangular_matrix> (m), it1_ (it1), it2_ (it2) {}

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
                container_reference<triangular_matrix>::assign (&it ());
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
        array_type data_;
    };

    // Triangular matrix adaptor class
    template<class M, class F>
    class triangular_adaptor:
        public matrix_expression<triangular_adaptor<M, F> > {
    public:
        typedef const M const_matrix_type;
        typedef M matrix_type;
        typedef F functor_type;
        typedef typename M::size_type size_type;
        typedef typename M::difference_type difference_type;
        typedef typename M::value_type value_type;
        typedef typename M::const_reference const_reference;
        typedef typename M::reference reference;
        typedef typename M::const_pointer const_pointer;
        typedef typename M::pointer pointer;
        typedef const triangular_adaptor<M, F> const_self_type;
        typedef triangular_adaptor<M, F> self_type;
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
        typedef typename proxy_traits<typename M::storage_category>::storage_category storage_category;
        typedef typename F::packed_category packed_category;
        typedef typename M::orientation_category orientation_category;

        // Construction and destruction
        NUMERICS_INLINE
        triangular_adaptor (): 
            data_ (nil_) {}
        NUMERICS_INLINE
        triangular_adaptor (matrix_type &data): 
            data_ (data) {}
        NUMERICS_INLINE
        triangular_adaptor (const triangular_adaptor &m): 
            data_ (m.data_) {}

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
        const_matrix_type &data () const {
            return data_;
        }
        NUMERICS_INLINE
        matrix_type &data () {
            return data_;
        }

        // Resetting
        NUMERICS_INLINE
        void reset (matrix_type &data) {
            data () = data;
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i, size_type j) const {
            check (i < size1 (), bad_index ());
            check (j < size2 (), bad_index ());
            if (functor_type::other (i, j))
                return data () (i, j);
            else if (functor_type::one (i, j))
                return value_type (1);
            else
                return value_type (0);
        }
        NUMERICS_INLINE
        reference operator () (size_type i, size_type j) {
            check (i < size1 (), bad_index ());
            check (j < size2 (), bad_index ());
            if (functor_type::other (i, j))
                return data () (i, j);
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
#endif

        // Assignment
        NUMERICS_INLINE
        triangular_adaptor &operator = (const triangular_adaptor &m) {
            matrix_assign<scalar_assign<value_type, value_type> > () (*this, m);
            return *this;
        }
        NUMERICS_INLINE
        triangular_adaptor &assign_temporary (triangular_adaptor &m) {
            *this = m;
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        triangular_adaptor &operator = (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_assign<value_type, value_type> > () (*this, matrix<value_type> (ae)); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        triangular_adaptor &assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        triangular_adaptor& operator += (const matrix_expression<AE> &ae) {
            matrix_assign<scalar_assign<value_type, value_type> > () (*this, matrix<value_type> (*this + ae)); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        triangular_adaptor &plus_assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_plus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        triangular_adaptor& operator -= (const matrix_expression<AE> &ae) {
            matrix_assign<scalar_assign<value_type, value_type> > () (*this, matrix<value_type> (*this - ae)); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        triangular_adaptor &minus_assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_minus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        triangular_adaptor& operator *= (const AT &at) {
            matrix_assign_scalar<scalar_multiplies_assign<value_type, AT> > () (*this, at);
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        triangular_adaptor& operator /= (const AT &at) {
            matrix_assign_scalar<scalar_divides_assign<value_type, AT> > () (*this, at);
            return *this;
        }

        // Swapping
        NUMERICS_INLINE
        void swap (triangular_adaptor &m) {
            check (this != &m, external_logic ());
            matrix_swap<scalar_swap<value_type, value_type> > () (*this, m); 
        }
#ifdef NUMERICS_FRIEND_FUNCTION
        NUMERICS_INLINE
        friend void swap (triangular_adaptor &m1, triangular_adaptor &m2) {
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
        const_iterator1 find_first1 (int rank, size_type i, size_type j) const {
            if (rank == 1) 
                i = functor_type::restrict1 (i, j);
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return const_iterator1 (*this, i);
#else
            return const_iterator1 (*this, i, j);
#endif
        }
        NUMERICS_INLINE
        iterator1 find_first1 (int rank, size_type i, size_type j) {
            if (rank == 1) 
                i = functor_type::mutable_restrict1 (i, j);
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return iterator1 (*this, i);
#else
            return iterator1 (*this, i, j);
#endif
        }
        NUMERICS_INLINE
        const_iterator1 find_last1 (int rank, size_type i, size_type j) const {
            if (rank == 1) 
                i = functor_type::restrict1 (i, j);
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return const_iterator1 (*this, i);
#else
            return const_iterator1 (*this, i, j);
#endif
        }
        NUMERICS_INLINE
        iterator1 find_last1 (int rank, size_type i, size_type j) {
            if (rank == 1) 
                i = functor_type::mutable_restrict1 (i, j);
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return iterator1 (*this, i);
#else
            return iterator1 (*this, i, j);
#endif
        }
        NUMERICS_INLINE
        const_iterator2 find_first2 (int rank, size_type i, size_type j) const {
            if (rank == 1) 
                j = functor_type::restrict2 (i, j);
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return const_iterator2 (*this, j);
#else
            return const_iterator2 (*this, i, j);
#endif
        }
        NUMERICS_INLINE
        iterator2 find_first2 (int rank, size_type i, size_type j) {
            if (rank == 1) 
                j = functor_type::mutable_restrict2 (i, j);
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return iterator2 (*this, j);
#else
            return iterator2 (*this, i, j);
#endif
        }
        NUMERICS_INLINE
        const_iterator2 find_last2 (int rank, size_type i, size_type j) const {
            if (rank == 1) 
                j = functor_type::restrict2 (i, j);
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return const_iterator2 (*this, j);
#else
            return const_iterator2 (*this, i, j);
#endif
        }
        NUMERICS_INLINE
        iterator2 find_last2 (int rank, size_type i, size_type j) {
            if (rank == 1) 
                j = functor_type::mutable_restrict2 (i, j);
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return iterator2 (*this, j);
#else
            return iterator2 (*this, i, j);
#endif
        }

        // Iterators simply are indices.

#if ! defined (NUMERICS_USE_CANONICAL_ITERATOR) && ! defined (NUMERICS_USE_INDEXED_ITERATOR)
        class const_iterator1:
            public container_const_reference<triangular_adaptor>,
            public random_access_iterator_base<packed_random_access_iterator_tag,
                                               const_iterator1, value_type> {
        public:
            typedef packed_random_access_iterator_tag iterator_category;
#ifndef BOOST_MSVC_STD_ITERATOR
            typedef typename triangular_adaptor::difference_type difference_type;
            typedef typename triangular_adaptor::value_type value_type;
            typedef typename triangular_adaptor::value_type reference;
            typedef typename triangular_adaptor::const_pointer pointer;
#endif
            typedef const_iterator2 dual_iterator_type;
            typedef const_reverse_iterator2 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator1 ():
                container_const_reference<triangular_adaptor> (), it1_ (), it2_ () {}
            NUMERICS_INLINE
            const_iterator1 (const triangular_adaptor &m, size_type it1, size_type it2):
                container_const_reference<triangular_adaptor> (m), it1_ (it1), it2_ (it2) {}
            NUMERICS_INLINE
            const_iterator1 (const iterator1 &it):
                container_const_reference<triangular_adaptor> (it ()), it1_ (it.it1_), it2_ (it.it2_) {}

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
                container_const_reference<triangular_adaptor>::assign (&it ());
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
            public container_reference<triangular_adaptor>,
            public random_access_iterator_base<packed_random_access_iterator_tag,
                                               iterator1, value_type> {
        public:
            typedef packed_random_access_iterator_tag iterator_category;
#ifndef BOOST_MSVC_STD_ITERATOR
            typedef typename triangular_adaptor::difference_type difference_type;
            typedef typename triangular_adaptor::value_type value_type;
            typedef typename triangular_adaptor::reference reference;
            typedef typename triangular_adaptor::pointer pointer;
#endif
            typedef iterator2 dual_iterator_type;
            typedef reverse_iterator2 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            iterator1 ():
                container_reference<triangular_adaptor> (), it1_ (), it2_ () {}
            NUMERICS_INLINE
            iterator1 (triangular_adaptor &m, size_type it1, size_type it2):
                container_reference<triangular_adaptor> (m), it1_ (it1), it2_ (it2) {}

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
                container_reference<triangular_adaptor>::assign (&it ());
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
            public container_const_reference<triangular_adaptor>,
            public random_access_iterator_base<packed_random_access_iterator_tag,
                                               const_iterator2, value_type> {
        public:
            typedef packed_random_access_iterator_tag iterator_category;
#ifndef BOOST_MSVC_STD_ITERATOR
            typedef typename triangular_adaptor::difference_type difference_type;
            typedef typename triangular_adaptor::value_type value_type;
            typedef typename triangular_adaptor::value_type reference;
            typedef typename triangular_adaptor::const_pointer pointer;
#endif
            typedef const_iterator1 dual_iterator_type;
            typedef const_reverse_iterator1 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator2 ():
                container_const_reference<triangular_adaptor> (), it1_ (), it2_ () {}
            NUMERICS_INLINE
            const_iterator2 (const triangular_adaptor &m, size_type it1, size_type it2):
                container_const_reference<triangular_adaptor> (m), it1_ (it1), it2_ (it2) {}
            NUMERICS_INLINE
            const_iterator2 (const iterator2 &it):
                container_const_reference<triangular_adaptor> (it ()), it1_ (it.it1_), it2_ (it.it2_) {}

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
                container_const_reference<triangular_adaptor>::assign (&it ());
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
            public container_reference<triangular_adaptor>,
            public random_access_iterator_base<packed_random_access_iterator_tag,
                                               iterator2, value_type> {
        public:
            typedef packed_random_access_iterator_tag iterator_category;
#ifndef BOOST_MSVC_STD_ITERATOR
            typedef typename triangular_adaptor::difference_type difference_type;
            typedef typename triangular_adaptor::value_type value_type;
            typedef typename triangular_adaptor::reference reference;
            typedef typename triangular_adaptor::pointer pointer;
#endif
            typedef iterator1 dual_iterator_type;
            typedef reverse_iterator1 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            iterator2 ():
                container_reference<triangular_adaptor> (), it1_ (), it2_ () {}
            NUMERICS_INLINE
            iterator2 (triangular_adaptor &m, size_type it1, size_type it2):
                container_reference<triangular_adaptor> (m), it1_ (it1), it2_ (it2) {}

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
                container_reference<triangular_adaptor>::assign (&it ());
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
        static matrix_type nil_;
    };

    template<class M, class F>
    typename triangular_adaptor<M, F>::matrix_type triangular_adaptor<M, F>::nil_;

    // FIXME: don't we need iterator versions?
    template<class E1, class E2>
    struct matrix_vector_solve_traits {
        typedef typename promote_traits<typename E1::value_type, typename E2::value_type>::promote_type promote_type;
        typedef vector<promote_type> result_type;
    };

    // Operations: 
    //  n * (n - 1) / 2 + n = n * (n + 1) / 2 multiplies,
    //  n * (n - 1) / 2 plus

    template<class E1, class E2>
    NUMERICS_INLINE
    void inplace_solve (const matrix_expression<E1> &e1, 
                        E2 &e2,
                        lower_tag,
                        vector_tag) {
        typedef NUMERICS_TYPENAME E2::size_type size_type;
        typedef NUMERICS_TYPENAME E2::difference_type difference_type;
        typedef NUMERICS_TYPENAME E2::value_type value_type;

        check (e1 ().size1 () == e1 ().size2 (), bad_size ());
        check (e1 ().size2 () == e2.size (), bad_size ());
        size_type size = e2.size ();
        for (size_type n = 0; n < size; ++ n) {
            check (e1 () (n, n) != value_type (), singular ());
            value_type t = e2 (n) /= e1 () (n, n);
            for (size_type m = n + 1; m < size; ++ m) 
                e2 (m) -= e1 () (m, n) * t;
        }
    }
    template<class E1, class E2>
    NUMERICS_INLINE
    void inplace_solve (const matrix_expression<E1> &e1, 
                        E2 &e2,
                        upper_tag,
                        vector_tag) {
        typedef NUMERICS_TYPENAME E2::size_type size_type;
        typedef NUMERICS_TYPENAME E2::difference_type difference_type;
        typedef NUMERICS_TYPENAME E2::value_type value_type;

        check (e1 ().size1 () == e1 ().size2 (), bad_size ());
        check (e1 ().size2 () == e2.size (), bad_size ());
        size_type size = e2.size ();
        for (difference_type n = size - 1; n >= 0; -- n) {
            check (e1 () (n, n) != value_type (), singular ());
            value_type t = e2 (n) /= e1 () (n, n);
            for (difference_type m = n - 1; m >= 0; -- m) 
                e2 (m) -= e1 () (m, n) * t;
        }
    }
    template<class E1, class E2>
    NUMERICS_INLINE
    void inplace_solve (const matrix_expression<E1> &e1, 
                        E2 &e2,
                        unit_lower_tag,
                        vector_tag) {
        typedef NUMERICS_TYPENAME E2::size_type size_type;
        typedef NUMERICS_TYPENAME E2::difference_type difference_type;
        typedef NUMERICS_TYPENAME E2::value_type value_type;

        check (e1 ().size1 () == e1 ().size2 (), bad_size ());
        check (e1 ().size2 () == e2.size (), bad_size ());
        size_type size = e2.size ();
        for (size_type n = 0; n < size; ++ n) {
            value_type t = e2 (n);
            for (size_type m = n + 1; m < size; ++ m) 
                e2 (m) -= e1 () (m, n) * t;
        }
    }
    template<class E1, class E2>
    NUMERICS_INLINE
    void inplace_solve (const matrix_expression<E1> &e1, 
                        E2 &e2,
                        unit_upper_tag,
                        vector_tag) {
        typedef NUMERICS_TYPENAME E2::size_type size_type;
        typedef NUMERICS_TYPENAME E2::difference_type difference_type;
        typedef NUMERICS_TYPENAME E2::value_type value_type;

        check (e1 ().size1 () == e1 ().size2 (), bad_size ());
        check (e1 ().size2 () == e2.size (), bad_size ());
        size_type size = e2.size ();
        for (difference_type n = size - 1; n >= 0; -- n) {
            value_type t = e2 (n);
            for (difference_type m = n - 1; m >= 0; -- m) 
                e2 (m) -= e1 () (m, n) * t;
        }
    }

    template<class E1, class E2, class C>
    NUMERICS_INLINE
    typename matrix_vector_solve_traits<E1, E2>::result_type
    solve (const matrix_expression<E1> &e1, 
           const vector_expression<E2> &e2,
           C) {
        typename matrix_vector_solve_traits<E1, E2>::result_type r (e2);
        inplace_solve (e1, r, C (), vector_tag ());
        return r;
    }

    template<class E1, class E2>
    NUMERICS_INLINE
    void inplace_solve (E1 &e1,
                        const matrix_expression<E2> &e2, 
                        vector_tag, 
                        lower_tag) {
        typedef NUMERICS_TYPENAME E1::size_type size_type;
        typedef NUMERICS_TYPENAME E1::difference_type difference_type;
        typedef NUMERICS_TYPENAME E1::value_type value_type;

        check (e1 ().size () == e2.size1 (), bad_size ());
        check (e2.size1 () == e2.size2 (), bad_size ());
        size_type size = e1.size ();
        for (size_type n = 0; n < size; ++ n) {
            check (e2 (n, n) != value_type (), singular ());
            value_type t = e1 (n) /= e2 (n, n);
            for (size_type m = n + 1; m < size; ++ m) 
                e1 (m) -= t * e2 (n, m);
        }
    }
    template<class E1, class E2>
    NUMERICS_INLINE
    void inplace_solve (E1 &e1,
                        const matrix_expression<E2> &e2, 
                        vector_tag, 
                        upper_tag) {
        typedef NUMERICS_TYPENAME E1::size_type size_type;
        typedef NUMERICS_TYPENAME E1::difference_type difference_type;
        typedef NUMERICS_TYPENAME E1::value_type value_type;

        check (e1 ().size () == e2.size1 (), bad_size ());
        check (e2.size1 () == e2.size2 (), bad_size ());
        size_type size = e1.size ();
        for (difference_type n = size - 1; n >= 0; -- n) {
            check (e2 (n, n) != value_type (), singular ());
            value_type t = e1 (n) /= e2 (n, n);
            for (difference_type m = n - 1; m >= 0; -- m) 
                e1 (m) -= t * e2 (n, m);
        }
    }
    template<class E1, class E2>
    NUMERICS_INLINE
    void inplace_solve (E1 &e1,
                        const matrix_expression<E2> &e2, 
                        vector_tag, 
                        unit_lower_tag) {
        typedef NUMERICS_TYPENAME E2::size_type size_type;
        typedef NUMERICS_TYPENAME E2::difference_type difference_type;
        typedef NUMERICS_TYPENAME E2::value_type value_type;

        check (e1 ().size () == e2.size1 (), bad_size ());
        check (e2.size1 () == e2.size2 (), bad_size ());
        size_type size = e1.size ();
        for (size_type n = 0; n < size; ++ n) {
            value_type t = e1 (n);
            for (size_type m = n + 1; m < size; ++ m) 
                e1 (m) -= t * e2 (n, m);
        }
    }
    template<class E1, class E2>
    NUMERICS_INLINE
    void inplace_solve (E1 &e1,
                        const matrix_expression<E2> &e2, 
                        vector_tag, 
                        unit_upper_tag) {
        typedef NUMERICS_TYPENAME E2::size_type size_type;
        typedef NUMERICS_TYPENAME E2::difference_type difference_type;
        typedef NUMERICS_TYPENAME E2::value_type value_type;

        check (e1 ().size1 () == e1 ().size2 (), bad_size ());
        check (e1 ().size2 () == e2.size (), bad_size ());
        size_type size = e1.size ();
        for (difference_type n = size - 1; n >= 0; -- n) {
            value_type t = e1 (n);
            for (difference_type m = n - 1; m >= 0; -- m) 
                e1 (m) -= t * e2 (n, m);
        }
    }

    template<class E1, class E2, class C>
    NUMERICS_INLINE
    typename matrix_vector_solve_traits<E1, E2>::result_type
    solve (const vector_expression<E1> &e1, 
           const matrix_expression<E2> &e2,
           C) {
        typename matrix_vector_solve_traits<E1, E2>::result_type r (e1);
        inplace_solve (r, e2, vector_tag (), C ());
        return r;
    }

    // FIXME: don't we need iterator versions?
    template<class E1, class E2>
    struct matrix_matrix_solve_traits {
        typedef typename promote_traits<typename E1::value_type, typename E2::value_type>::promote_type promote_type;
        typedef matrix<promote_type> result_type;
    };

    // Operations: 
    //  k * n * (n - 1) / 2 + k * n = k * n * (n + 1) / 2 multiplies,
    //  k * n * (n - 1) / 2 plus

    template<class E1, class E2>
    NUMERICS_INLINE
    void inplace_solve (const matrix_expression<E1> &e1, 
                        E2 &e2,
                        lower_tag,
                        matrix_tag) {
        typedef NUMERICS_TYPENAME E2::size_type size_type;
        typedef NUMERICS_TYPENAME E2::difference_type difference_type;
        typedef NUMERICS_TYPENAME E2::value_type value_type;

        check (e1 ().size1 () == e1 ().size2 (), bad_size ());
        check (e1 ().size2 () == e2.size1 (), bad_size ());
        size_type size1 = e2.size1 ();
        size_type size2 = e2.size2 ();
        for (size_type n = 0; n < size1; ++ n) {
            check (e1 () (n, n) != value_type (), singular ());
            for (size_type l = 0; l < size2; ++ l) {
                value_type t = e2 (n, l) /= e1 () (n, n);
                for (size_type m = n + 1; m < size1; ++ m) 
                    e2 (m, l) -= e1 () (m, n) * t;
            }
        }
    }
    template<class E1, class E2>
    NUMERICS_INLINE
    void inplace_solve (const matrix_expression<E1> &e1, 
                        E2 &e2,
                        upper_tag,
                        matrix_tag) {
        typedef NUMERICS_TYPENAME E2::size_type size_type;
        typedef NUMERICS_TYPENAME E2::difference_type difference_type;
        typedef NUMERICS_TYPENAME E2::value_type value_type;

        check (e1 ().size1 () == e1 ().size2 (), bad_size ());
        check (e1 ().size2 () == e2.size1 (), bad_size ());
        size_type size1 = e2.size1 ();
        size_type size2 = e2.size2 ();
        for (difference_type n = size1 - 1; n >= 0; -- n) {
            check (e1 () (n, n) != value_type (), singular ());
            for (difference_type l = size2 - 1; l >= 0; -- l) {
                value_type t = e2 (n, l) /= e1 () (n, n);
                for (difference_type m = n - 1; m >= 0; -- m) 
                    e2 (m, l) -= e1 () (m, n) * t;
            }
        }
    }
    template<class E1, class E2>
    NUMERICS_INLINE
    void inplace_solve (const matrix_expression<E1> &e1, 
                        E2 &e2,
                        unit_lower_tag,
                        matrix_tag) {
        typedef NUMERICS_TYPENAME E2::size_type size_type;
        typedef NUMERICS_TYPENAME E2::difference_type difference_type;
        typedef NUMERICS_TYPENAME E2::value_type value_type;

        check (e1 ().size1 () == e1 ().size2 (), bad_size ());
        check (e1 ().size2 () == e2.size1 (), bad_size ());
        size_type size1 = e2.size1 ();
        size_type size2 = e2.size2 ();
        for (size_type n = 0; n < size1; ++ n) {
            for (size_type l = 0; l < size2; ++ l) {
                value_type t = e2 (n, l);
                for (size_type m = n + 1; m < size1; ++ m) 
                    e2 (m, l) -= e1 () (m, n) * t;
            }
        }
    }
    template<class E1, class E2>
    NUMERICS_INLINE
    void inplace_solve (const matrix_expression<E1> &e1, 
                        E2 &e2,
                        unit_upper_tag,
                        matrix_tag) {
        typedef NUMERICS_TYPENAME E2::size_type size_type;
        typedef NUMERICS_TYPENAME E2::difference_type difference_type;
        typedef NUMERICS_TYPENAME E2::value_type value_type;

        check (e1 ().size1 () == e1 ().size2 (), bad_size ());
        check (e1 ().size2 () == e2.size1 (), bad_size ());
        size_type size1 = e2.size1 ();
        size_type size2 = e2.size2 ();
        for (difference_type n = size1 - 1; n >= 0; -- n) {
            for (difference_type l = size2 - 1; l >= 0; -- l) {
                value_type t = e2 (n, l);
                for (difference_type m = n - 1; m >= 0; -- m) 
                    e2 (m, l) -= e1 () (m, n) * t;
            }
        }
    }

    template<class E1, class E2, class C>
    NUMERICS_INLINE
    typename matrix_matrix_solve_traits<E1, E2>::result_type
    solve (const matrix_expression<E1> &e1, 
           const matrix_expression<E2> &e2,
           C) {
        typename matrix_matrix_solve_traits<E1, E2>::result_type r (e2);
        inplace_solve (e1, r, C (), matrix_tag ());
        return r;
    }

}}

#endif





