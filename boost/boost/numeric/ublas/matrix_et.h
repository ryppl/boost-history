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

#ifndef NUMERICS_MATRIX_ET_H
#define NUMERICS_MATRIX_ET_H

#include "config.h"
#include "exception.h"
#include "functional.h"

// Expression templates based on ideas of Todd Veldhuizen and Geoffrey Furnish 
// Iterators based on ideas of Jeremy Siek

namespace numerics {

    // Base class for the Barton Nackman trick
    template<class E>
    struct matrix_expression {
        typedef E expression_type;
        typedef matrix_tag type_category;

        // This class could define an common interface for all 
        // statically derived expression type classes.
        // Due to a compiler deficiency - one can not reference class typedefs of E 
        // on MSVC 6.0 (error C2027) - we only implement the casts.

        NUMERICS_INLINE
        const expression_type &operator () () const {
            return *static_cast<const expression_type *> (this);
        }
        NUMERICS_INLINE
        expression_type &operator () () {
            return *static_cast<expression_type *> (this);
        }
    };

    template<class E>
    class matrix_const_reference:
        public matrix_expression<matrix_const_reference<E> > {
    public:
        typedef E expression_type;
        typedef typename E::size_type size_type;
        typedef typename E::difference_type difference_type;
        typedef typename E::value_type value_type;
        typedef typename E::const_reference const_reference;
        typedef const_reference reference;
        typedef typename E::const_pointer const_pointer;
        typedef const_pointer pointer;
        typedef typename E::orientation_category orientation_category;
        typedef typename E::const_iterator1 const_iterator1_type;
        typedef typename E::const_iterator2 const_iterator2_type;
#ifdef NUMERICS_DEPRECATED
#ifdef NUMERICS_DEPRECATED
        typedef const matrix_expression_row<const E> const_matrix_row_type;
        typedef const matrix_expression_column<const E> const_matrix_column_type;
        typedef const matrix_expression_range<const E> const_matrix_range_type;
#else
        typedef const matrix_row<const E> const_matrix_row_type;
        typedef const matrix_column<const E> const_matrix_column_type;
        typedef const matrix_range<const E> const_matrix_range_type;
#endif
#endif
        typedef unknown_storage_tag storage_category;

        // Construction and destruction
        NUMERICS_INLINE
        matrix_const_reference ():
            e_ (nil_) {}
        NUMERICS_INLINE
        matrix_const_reference (const expression_type &e):
            e_ (e) {}

        NUMERICS_INLINE
        size_type size1 () const { 
            return e_.size1 (); 
        }
        NUMERICS_INLINE
        size_type size2 () const { 
            return e_.size2 (); 
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i, size_type j) const { 
            return e_ (i, j); 
        }

#ifdef NUMERICS_DEPRECATED
        NUMERICS_INLINE
        const_matrix_row_type operator [] (size_type i) const {
            return e_ [i]; 
        }
        NUMERICS_INLINE
        const_matrix_row_type row (size_type i) const {
            return const_matrix_row_type (e_, i);
        }
        NUMERICS_INLINE
        const_matrix_column_type column (size_type j) const {
            return const_matrix_column_type (e_, j);
        }

        NUMERICS_INLINE
        const_matrix_range_type project (size_type start1, size_type stop1, size_type start2, size_type stop2) const {
            return const_matrix_range_type (e_, start1, stop1, start2, stop2);
        }
        NUMERICS_INLINE
        const_matrix_range_type project (const range &r1, const range &r2) const {
            return const_matrix_range_type (e_, r1, r2);
        }
#endif

        typedef const_iterator1_type const_iterator1;
        typedef const_iterator1 iterator1;
        typedef const_iterator2_type const_iterator2;
        typedef const_iterator2 iterator2;

        // Element lookup
        NUMERICS_INLINE
        const_iterator1 find1 (int rank, size_type i, size_type j) const {
            return const_iterator1 (e_.find1 (rank, i, j));
        }
        NUMERICS_INLINE
        const_iterator2 find2 (int rank, size_type i, size_type j) const {
            return const_iterator2 (e_.find2 (rank, i, j));
        }

        // Iterators are the iterators of the referenced expression.

        NUMERICS_INLINE
        const_iterator1 begin1 () const {
            return e_.begin1 ();
        }
        NUMERICS_INLINE
        const_iterator1 end1 () const {
            return e_.end1 ();
        }

        NUMERICS_INLINE
        const_iterator2 begin2 () const {
            return e_.begin2 ();
        }
        NUMERICS_INLINE
        const_iterator2 end2 () const {
            return e_.end2 ();
        }

        // Reverse iterators

#ifdef USE_MSVC
        typedef reverse_iterator1<const_iterator1, value_type, value_type> const_reverse_iterator1;
#else
        typedef reverse_iterator1<const_iterator1> const_reverse_iterator1;
#endif

        NUMERICS_INLINE
        const_reverse_iterator1 rbegin1 () const {
            return const_reverse_iterator1 (end1 ());
        }
        NUMERICS_INLINE
        const_reverse_iterator1 rend1 () const {
            return const_reverse_iterator1 (begin1 ());
        }

#ifdef USE_MSVC
        typedef reverse_iterator2<const_iterator2, value_type, value_type> const_reverse_iterator2;
#else
        typedef reverse_iterator2<const_iterator2> const_reverse_iterator2;
#endif

        NUMERICS_INLINE
        const_reverse_iterator2 rbegin2 () const {
            return const_reverse_iterator2 (end2 ());
        }
        NUMERICS_INLINE
        const_reverse_iterator2 rend2 () const {
            return const_reverse_iterator2 (begin2 ());
        }

    private:
        const expression_type &e_;
        static expression_type nil_;
    };

    template<class E>
    matrix_const_reference<E>::expression_type matrix_const_reference<E>::nil_;

    template<class E>
    class matrix_reference:
        public matrix_expression<matrix_reference<E> > {
    public:
        typedef E expression_type;
        typedef typename E::size_type size_type;
        typedef typename E::difference_type difference_type;
        typedef typename E::value_type value_type;
        typedef typename E::const_reference const_reference;
        typedef typename E::reference reference;
        typedef typename E::const_pointer const_pointer;
        typedef typename E::pointer pointer;
        typedef typename E::orientation_category orientation_category;
        typedef typename E::const_iterator1 const_iterator1_type;
        typedef typename E::iterator1 iterator1_type;
        typedef typename E::const_iterator2 const_iterator2_type;
        typedef typename E::iterator2 iterator2_type;
#ifdef NUMERICS_DEPRECATED
#ifdef NUMERICS_DEPRECATED
        typedef const matrix_expression_row<const E> const_matrix_row_type;
        typedef matrix_expression_row<E> matrix_row_type;
        typedef const matrix_expression_column<const E> const_matrix_column_type;
        typedef matrix_expression_column<E> matrix_column_type;
        typedef const matrix_expression_range<const E> const_matrix_range_type;
        typedef matrix_expression_range<E> matrix_range_type;
#else
        typedef const matrix_row<const E> const_matrix_row_type;
        typedef matrix_row<E> matrix_row_type;
        typedef const matrix_column<const E> const_matrix_column_type;
        typedef matrix_column<E> matrix_column_type;
        typedef const matrix_range<const E> const_matrix_range_type;
        typedef matrix_range<E> matrix_range_type;
#endif
#endif
        typedef unknown_storage_tag storage_category;

        // Construction and destruction
        NUMERICS_INLINE
        matrix_reference ():
              e_ (nil_) {}
        NUMERICS_INLINE
        matrix_reference (expression_type &e):
              e_ (e) {}

        // Resizing
        NUMERICS_INLINE
        void resize (size_type size1, size_type size2) {
            e_.resize (size1, size2);
        }

        NUMERICS_INLINE
        size_type size1 () const { 
            return e_.size1 ();
        }
        NUMERICS_INLINE
        size_type size2 () const { 
            return e_.size2 ();
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i, size_type j) const { 
            return e_ (i, j); 
        }
        NUMERICS_INLINE
        reference operator () (size_type i, size_type j) { 
            return e_ (i, j); 
        }

#ifdef NUMERICS_DEPRECATED
        NUMERICS_INLINE
        const_matrix_row_type operator [] (size_type i) const {
            return e [i];
        }
        NUMERICS_INLINE
        matrix_row_type operator [] (size_type i) {
            return e [i];
        }
        NUMERICS_INLINE
        const_matrix_row_type row (size_type i) const {
            return const_matrix_row_type (e_, i);
        }
        NUMERICS_INLINE
        matrix_row_type row (size_type i) {
            return matrix_row_type (e_, i);
        }
        NUMERICS_INLINE
        const_matrix_column_type column (size_type j) const {
            return const_matrix_column_type (e_, j);
        }
        NUMERICS_INLINE
        matrix_column_type column (size_type j) {
            return matrix_column_type (e_, j);
        }

        NUMERICS_INLINE
        const_matrix_range_type project (size_type start1, size_type stop1, size_type start2, size_type stop2) const {
            return const_matrix_range_type (e_, start1, stop1, start2, stop2);
        }
        NUMERICS_INLINE
        const_matrix_range_type project (const range &r1, const range &r2) const {
            return const_matrix_range_type (e_, r1, r2);
        }
        NUMERICS_INLINE
        matrix_range_type project (size_type start1, size_type stop1, size_type start2, size_type stop2) {
            return matrix_range_type (e_, start1, stop1, start2, stop2);
        }
        NUMERICS_INLINE
        matrix_range_type project (const range &r1, const range &r2) {
            return matrix_range_type (e_, r1, r2);
        }
#endif

        typedef const_iterator1_type const_iterator1;
        typedef iterator1_type iterator1;
        typedef const_iterator2_type const_iterator2;
        typedef iterator2_type iterator2;

        // Element lookup
        NUMERICS_INLINE
        const_iterator1 find1 (int rank, size_type i, size_type j) const {
            return const_iterator1 (e_.find1 (rank, i, j));
        }
        NUMERICS_INLINE
        iterator1 find1 (int rank, size_type i, size_type j) {
            return iterator1 (e_.find1 (rank, i, j));
        }
        NUMERICS_INLINE
        const_iterator2 find2 (int rank, size_type i, size_type j) const {
            return const_iterator2 (e_.find2 (rank, i, j));
        }
        NUMERICS_INLINE
        iterator2 find2 (int rank, size_type i, size_type j) {
            return iterator2 (e_.find2 (rank, i, j));
        }

        // Iterators are the iterators of the referenced expression.

        NUMERICS_INLINE
        const_iterator1 begin1 () const {
            return e_.begin1 ();
        }
        NUMERICS_INLINE
        const_iterator1 end1 () const {
            return e_.end1 ();
        }

        NUMERICS_INLINE
        iterator1 begin1 () {
            return e_.begin1 ();
        }
        NUMERICS_INLINE
        iterator1 end1 () {
            return e_.end1 ();
        }

        NUMERICS_INLINE
        const_iterator2 begin2 () const {
            return e_.begin2 ();
        }
        NUMERICS_INLINE
        const_iterator2 end2 () const {
            return e_.end2 ();
        }

        NUMERICS_INLINE
        iterator2 begin2 () {
            return e_.begin2 ();
        }
        NUMERICS_INLINE
        iterator2 end2 () {
            return e_.end2 ();
        }

        // Reverse iterators

#ifdef USE_MSVC
        typedef reverse_iterator1<const_iterator1, value_type, value_type> const_reverse_iterator1;
#else
        typedef reverse_iterator1<const_iterator1> const_reverse_iterator1;
#endif

        NUMERICS_INLINE
        const_reverse_iterator1 rbegin1 () const {
            return const_reverse_iterator1 (end1 ());
        }
        NUMERICS_INLINE
        const_reverse_iterator1 rend1 () const {
            return const_reverse_iterator1 (begin1 ());
        }

#ifdef USE_MSVC
        typedef reverse_iterator1<iterator1, value_type, reference> reverse_iterator1;
#else
        typedef reverse_iterator1<iterator1> reverse_iterator1;
#endif

        NUMERICS_INLINE
        reverse_iterator1 rbegin1 () {
            return reverse_iterator1 (end1 ());
        }
        NUMERICS_INLINE
        reverse_iterator1 rend1 () {
            return reverse_iterator1 (begin1 ());
        }

#ifdef USE_MSVC
        typedef reverse_iterator2<const_iterator2, value_type, value_type> const_reverse_iterator2;
#else
        typedef reverse_iterator2<const_iterator2> const_reverse_iterator2;
#endif

        NUMERICS_INLINE
        const_reverse_iterator2 rbegin2 () const {
            return const_reverse_iterator2 (end2 ());
        }
        NUMERICS_INLINE
        const_reverse_iterator2 rend2 () const {
            return const_reverse_iterator2 (begin2 ());
        }

#ifdef USE_MSVC
        typedef reverse_iterator2<iterator2, value_type, reference> reverse_iterator2;
#else
        typedef reverse_iterator2<iterator2> reverse_iterator2;
#endif

        NUMERICS_INLINE
        reverse_iterator2 rbegin2 () {
            return reverse_iterator2 (end2 ());
        }
        NUMERICS_INLINE
        reverse_iterator2 rend2 () {
            return reverse_iterator2 (begin2 ());
        }

    private:
        expression_type &e_;
        static expression_type nil_;
    };

    template<class E>
    matrix_reference<E>::expression_type matrix_reference<E>::nil_;

    template<class E1, class E2, class F>
    class vector_matrix_binary:
        public matrix_expression<vector_matrix_binary<E1, E2, F> > {
    public:
        typedef E1 expression1_type;
        typedef E2 expression2_type;
        typedef F functor_type;
        typedef typename promote_traits<typename E1::size_type, typename E2::size_type>::promote_type size_type;
        typedef typename promote_traits<typename E1::difference_type, typename E2::difference_type>::promote_type difference_type;
        typedef typename F::result_type value_type;
        typedef const value_type &const_reference;
        typedef const_reference reference;
        typedef const value_type *const_pointer;
        typedef const_pointer pointer;
        typedef const vector_matrix_binary<E1, E2, F> const_closure_type;
        typedef unknown_orientation_tag orientation_category;
        typedef typename E1::const_iterator const_iterator1_type;
        typedef typename E2::const_iterator const_iterator2_type;
#ifdef NUMERICS_DEPRECATED
#ifdef NUMERICS_DEPRECATED
        typedef const matrix_expression_row<const_closure_type> const_matrix_row_type;
        typedef const matrix_expression_column<const_closure_type> const_matrix_column_type;
        typedef const matrix_expression_range<const_closure_type> const_matrix_range_type;
#else
        typedef const matrix_row<const_closure_type> const_matrix_row_type;
        typedef const matrix_column<const_closure_type> const_matrix_column_type;
        typedef const matrix_range<const_closure_type> const_matrix_range_type;
#endif
#endif
        typedef unknown_storage_tag storage_category;

        // Construction and destruction 
        NUMERICS_INLINE
        vector_matrix_binary (): 
            e1_ (), e2_ () {}
        NUMERICS_INLINE
        vector_matrix_binary (const expression1_type &e1, const expression2_type &e2): 
            e1_ (e1), e2_ (e2) {}

        NUMERICS_INLINE
        const expression1_type &expression1 () const {
            return e1_;
        }
        NUMERICS_INLINE
        const expression2_type &expression2 () const {
            return e2_;
        }

        NUMERICS_INLINE
        size_type size1 () const { 
            return e1_.size (); 
        }
        NUMERICS_INLINE
        size_type size2 () const { 
            return e2_.size (); 
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i, size_type j) const { 
            return functor_type () (e1_ (i), e2_ (j)); 
        }

#ifdef NUMERICS_DEPRECATED
        NUMERICS_INLINE
        const_matrix_row_type operator [] (size_type i) const {
            return const_matrix_row_type (*this, i); 
        }
        NUMERICS_INLINE
        const_matrix_row_type row (size_type i) const {
            return const_matrix_row_type (*this, i);
        }
        NUMERICS_INLINE
        const_matrix_column_type column (size_type j) const {
            return const_matrix_column_type (*this, j);
        }

        NUMERICS_INLINE
        const_matrix_range_type project (size_type start1, size_type stop1, size_type start2, size_type stop2) const {
            return const_matrix_range_type (*this, start1, stop1, start2, stop2);
        }
        NUMERICS_INLINE
        const_matrix_range_type project (const range &r1, const range &r2) const {
            return const_matrix_range_type (*this, r1, r2);
        }
#endif

        class const_iterator1;
        typedef const_iterator1 iterator1;
        class const_iterator2;
        typedef const_iterator2 iterator2;

        // Element lookup
        NUMERICS_INLINE
        const_iterator1 find1 (int rank, size_type i, size_type j) const {
#ifdef NUMERICS_USE_INVARIANT_HOISTING
            const_iterator2_type it2 (e2_.find (j));
            return const_iterator1 (*this, e1_.find (i), it2, *it2);
#else
            return const_iterator1 (*this, e1_.find (i), e2_.find (j));
#endif
        }
        NUMERICS_INLINE
        const_iterator2 find2 (int rank, size_type i, size_type j) const {
#ifdef NUMERICS_USE_INVARIANT_HOISTING
            const_iterator1_type it1 (e1_.find (i));
            return const_iterator2 (*this, it1, e2_.find (j), *it1);
#else
            return const_iterator2 (*this, e1_.find (i), e2_.find (j));
#endif
        }

        // Iterators enhance the iterators of the referenced expressions
        // with the unary functor.

#ifdef USE_MSVC
        typedef reverse_iterator1<const_iterator1, value_type, value_type> const_reverse_iterator1;
        typedef reverse_iterator2<const_iterator2, value_type, value_type> const_reverse_iterator2;
#else
        typedef reverse_iterator1<const_iterator1> const_reverse_iterator1;
        typedef reverse_iterator2<const_iterator2> const_reverse_iterator2;
#endif

        class const_iterator1:
            public container_const_reference<vector_matrix_binary>,
            public random_access_iterator_base<const_iterator1, value_type> {
        public:
            typedef typename restrict_traits<typename E1::const_iterator::iterator_category, 
                                             typename E2::const_iterator::iterator_category>::iterator_category iterator_category;
#ifndef USE_MSVC
            typedef typename vector_matrix_binary::difference_type difference_type;
            typedef typename vector_matrix_binary::value_type value_type;
            typedef typename vector_matrix_binary::value_type reference;
            typedef typename vector_matrix_binary::const_pointer pointer;
#endif
            typedef const_iterator2 dual_iterator_type;
            typedef const_reverse_iterator2 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
#ifdef NUMERICS_USE_INVARIANT_HOISTING
            const_iterator1 ():
                container_const_reference<vector_matrix_binary> (), it1_ (), it2_ (), t2_ () {}
            const_iterator1 (const vector_matrix_binary &vmb, const const_iterator1_type &it1, const const_iterator2_type &it2, value_type t2):
                container_const_reference<vector_matrix_binary> (vmb), it1_ (it1), it2_ (it2), t2_ (t2) {}
#else
            const_iterator1 ():
                container_const_reference<vector_matrix_binary> (), it1_ (), it2_ () {}
            const_iterator1 (const vector_matrix_binary &vmb, const const_iterator1_type &it1, const const_iterator2_type &it2):
                container_const_reference<vector_matrix_binary> (vmb), it1_ (it1), it2_ (it2) {}
#endif

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
#ifdef NUMERICS_USE_INVARIANT_HOISTING
                return functor_type () (*it1_, t2_); 
#else
                return functor_type () (*it1_, *it2_); 
#endif
            }

            NUMERICS_INLINE
            const_iterator2 begin () const {
#ifdef NUMERICS_USE_INVARIANT_HOISTING
                return const_iterator2 ((*this) (), it1_, (*this) ().expression2 ().begin (), *it1_); 
#else
                return const_iterator2 ((*this) (), it1_, (*this) ().expression2 ().begin ()); 
#endif
            }
            NUMERICS_INLINE
            const_iterator2 end () const {
#ifdef NUMERICS_USE_INVARIANT_HOISTING
                return const_iterator2 ((*this) (), it1_, (*this) ().expression2 ().end (), value_type ()); 
#else
                return const_iterator2 ((*this) (), it1_, (*this) ().expression2 ().end ()); 
#endif
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
            size_type  index2 () const {
                return it2_.index ();
            }

            // Assignment 
            NUMERICS_INLINE
            const_iterator1 &operator = (const const_iterator1 &it) {
                container_const_reference<vector_matrix_binary>::assign (&it ());
                it1_ = it.it1_;
                it2_ = it.it2_;
#ifdef NUMERICS_USE_INVARIANT_HOISTING
                t2_ = it.t2_;
#endif
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator1 &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it1_ == it.it1_ && it2_ == it.it2_;
            }

        private:
#ifdef NUMERICS_USE_INVARIANT_HOISTING
            const_iterator1_type it1_;
            // Mutable due to assignment 
            /* const */ const_iterator2_type it2_; 
            value_type t2_;
#else
            const_iterator1_type it1_;
            const_iterator2_type it2_;
#endif
        };

        NUMERICS_INLINE
        const_iterator1 begin1 () const {
#ifdef NUMERICS_USE_INVARIANT_HOISTING
            return const_iterator1 (*this, e1_.begin (), e2_.begin (), 
                                    e2_.size () ? *e2_.begin () : value_type ());
#else
            return const_iterator1 (*this, e1_.begin (), e2_.begin ());
#endif
        }
        NUMERICS_INLINE
        const_iterator1 end1 () const {
#ifdef NUMERICS_USE_INVARIANT_HOISTING
            return const_iterator1 (*this, e1_.end (), e2_.begin (), value_type ());
#else
            return const_iterator1 (*this, e1_.end (), e2_.begin ());
#endif
        }

        class const_iterator2:
            public container_const_reference<vector_matrix_binary>,
            public random_access_iterator_base<const_iterator2, value_type> {
        public:
            typedef typename restrict_traits<typename E1::const_iterator::iterator_category, 
                                             typename E2::const_iterator::iterator_category>::iterator_category iterator_category;
#ifndef USE_MSVC
            typedef typename vector_matrix_binary::difference_type difference_type;
            typedef typename vector_matrix_binary::value_type value_type;
            typedef typename vector_matrix_binary::value_type reference;
            typedef typename vector_matrix_binary::const_pointer pointer;
#endif
            typedef const_iterator1 dual_iterator_type;
            typedef const_reverse_iterator1 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
#ifdef NUMERICS_USE_INVARIANT_HOISTING
            const_iterator2 ():
                container_const_reference<vector_matrix_binary> (), it1_ (), it2_ (), t1_ () {}
            const_iterator2 (const vector_matrix_binary &vmb, const const_iterator1_type &it1, const const_iterator2_type &it2, value_type t1):
                container_const_reference<vector_matrix_binary> (vmb), it1_ (it1), it2_ (it2), t1_ (t1) {}
#else
            const_iterator2 ():
                container_const_reference<vector_matrix_binary> (), it1_ (), it2_ () {}
            const_iterator2 (const vector_matrix_binary &vmb, const const_iterator1_type &it1, const const_iterator2_type &it2):
                container_const_reference<vector_matrix_binary> (vmb), it1_ (it1), it2_ (it2) {}
#endif

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
#ifdef NUMERICS_USE_INVARIANT_HOISTING
                return functor_type () (t1_, *it2_); 
#else
                return functor_type () (*it1_, *it2_); 
#endif
            }

            NUMERICS_INLINE
            const_iterator1 begin () const {
#ifdef NUMERICS_USE_INVARIANT_HOISTING
                return const_iterator1 ((*this) (), (*this) ().expression1 ().begin (), it2_, *it2_); 
#else
                return const_iterator1 ((*this) (), (*this) ().expression1 ().begin (), it2_); 
#endif
            }
            NUMERICS_INLINE
            const_iterator1 end () const {
#ifdef NUMERICS_USE_INVARIANT_HOISTING
                return const_iterator1 ((*this) (), (*this) ().expression1 ().end (), it2_, value_type ()); 
#else
                return const_iterator1 ((*this) (), (*this) ().expression1 ().end (), it2_); 
#endif
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
            size_type  index2 () const {
                return it2_.index ();
            }

            // Assignment 
            NUMERICS_INLINE
            const_iterator2 &operator = (const const_iterator2 &it) {
                container_const_reference<vector_matrix_binary>::assign (&it ());
                it1_ = it.it1_;
                it2_ = it.it2_;
#ifdef NUMERICS_USE_INVARIANT_HOISTING
                t1_ = it.t1_;
#endif
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator2 &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it1_ == it.it1_ && it2_ == it.it2_;
            }

        private:
#ifdef NUMERICS_USE_INVARIANT_HOISTING
            // Mutable due to assignment 
            /* const */ const_iterator1_type it1_;
            const_iterator2_type it2_;
            value_type t1_;
#else
            const_iterator1_type it1_;
            const_iterator2_type it2_;
#endif
        };

        NUMERICS_INLINE
        const_iterator2 begin2 () const {
#ifdef NUMERICS_USE_INVARIANT_HOISTING
            return const_iterator2 (*this, e1_.begin (), e2_.begin (), 
                                    e1_.size () ? *e1_.begin () : value_type ());
#else
            return const_iterator2 (*this, e1_.begin (), e2_.begin ());
#endif
        }
        NUMERICS_INLINE
        const_iterator2 end2 () const {
#ifdef NUMERICS_USE_INVARIANT_HOISTING
            return const_iterator2 (*this, e1_.begin (), e2_.end (), value_type ());
#else
            return const_iterator2 (*this, e1_.begin (), e2_.end ());
#endif
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
        const_reverse_iterator2 rbegin2 () const {
            return const_reverse_iterator2 (end2 ());
        }
        NUMERICS_INLINE
        const_reverse_iterator2 rend2 () const {
            return const_reverse_iterator2 (begin2 ());
        }

    private:
#ifdef NUMERICS_ET_VALUE
        expression1_type e1_;
        expression2_type e2_;
#endif
#ifdef NUMERICS_ET_REFERENCE
        const expression1_type &e1_;
        const expression2_type &e2_;
#endif
    };

    template<class E1, class E2, class F>
    struct vector_matrix_binary_traits {
        typedef vector_matrix_binary<typename E1::const_closure_type, 
                                     typename E2::const_closure_type, F> expression_type;
#ifdef NUMERICS_USE_ET
        typedef expression_type result_type; 
#else
        typedef matrix<typename F::result_type> result_type;
#endif
    };

    template<class E1, class E2>
    NUMERICS_INLINE
    typename vector_matrix_binary_traits<E1, E2, scalar_multiplies<typename E1::value_type, typename E2::value_type> >::result_type
    outer_prod (const vector_expression<E1> &e1, 
                const vector_expression<E2> &e2) {
        typedef NUMERICS_TYPENAME vector_matrix_binary_traits<E1, E2, scalar_multiplies<NUMERICS_TYPENAME E1::value_type, NUMERICS_TYPENAME E2::value_type> >::expression_type expression_type;
        return expression_type (e1 (), e2 ());
    }

    template<class E, class F>
    class matrix_unary1:
        public matrix_expression<matrix_unary1<E, F> > {
    public:
        typedef E expression_type;
        typedef F functor_type;
        typedef typename E::size_type size_type;
        typedef typename E::difference_type difference_type;
        typedef typename F::result_type value_type;
        typedef const value_type &const_reference;
        typedef const_reference reference;
        typedef const value_type *const_pointer;
        typedef const_pointer pointer;
        typedef const matrix_unary1<E, F> const_closure_type;
        typedef typename E::orientation_category orientation_category;
        typedef typename E::const_iterator1 const_iterator1_type;
        typedef typename E::const_iterator2 const_iterator2_type;
#ifdef NUMERICS_DEPRECATED
#ifdef NUMERICS_DEPRECATED
        typedef const matrix_expression_row<const_closure_type> const_matrix_row_type;
        typedef const matrix_expression_column<const_closure_type> const_matrix_column_type;
        typedef const matrix_expression_range<const_closure_type> const_matrix_range_type;
#else
        typedef const matrix_row<const_closure_type> const_matrix_row_type;
        typedef const matrix_column<const_closure_type> const_matrix_column_type;
        typedef const matrix_range<const_closure_type> const_matrix_range_type;
#endif
#endif
        typedef unknown_storage_tag storage_category;

        // Construction and destruction
        NUMERICS_INLINE
        matrix_unary1 (): 
            e_ () {}
        NUMERICS_INLINE
        matrix_unary1 (const expression_type &e): 
            e_ (e) {}

        NUMERICS_INLINE
        size_type size1 () const { 
            return e_.size1 ();
        }
        NUMERICS_INLINE
        size_type size2 () const { 
            return e_.size2 ();
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i, size_type j) const { 
            return functor_type () (e_ (i, j)); 
        }

#ifdef NUMERICS_DEPRECATED
        NUMERICS_INLINE
        const_matrix_row_type operator [] (size_type i) const {
            return const_matrix_row_type (*this, i); 
        }
        NUMERICS_INLINE
        const_matrix_row_type row (size_type i) const {
            return const_matrix_row_type (*this, i);
        }
        NUMERICS_INLINE
        const_matrix_column_type column (size_type j) const {
            return const_matrix_column_type (*this, j);
        }

        NUMERICS_INLINE
        const_matrix_range_type project (size_type start1, size_type stop1, size_type start2, size_type stop2) const {
            return const_matrix_range_type (*this, start1, stop1, start2, stop2);
        }
        NUMERICS_INLINE
        const_matrix_range_type project (const range &r1, const range &r2) const {
            return const_matrix_range_type (*this, r1, r2);
        }
#endif

        class const_iterator1;
        typedef const_iterator1 iterator1;
        class const_iterator2;
        typedef const_iterator2 iterator2;

        // Element lookup
        NUMERICS_INLINE
        const_iterator1 find1 (int rank, size_type i, size_type j) const {
            return const_iterator1 (*this, e_.find1 (rank, i, j));
        }
        NUMERICS_INLINE
        const_iterator2 find2 (int rank, size_type i, size_type j) const {
            return const_iterator2 (*this, e_.find2 (rank, i, j));
        }

        // Iterators enhance the iterators of the referenced expression
        // with the unary functor.

#ifdef USE_MSVC
        typedef reverse_iterator1<const_iterator1, value_type, value_type> const_reverse_iterator1;
        typedef reverse_iterator2<const_iterator2, value_type, value_type> const_reverse_iterator2;
#else
        typedef reverse_iterator1<const_iterator1> const_reverse_iterator1;
        typedef reverse_iterator2<const_iterator2> const_reverse_iterator2;
#endif

        class const_iterator1:
            public container_const_reference<matrix_unary1>,
            public random_access_iterator_base<const_iterator1, value_type> {
        public:
            typedef typename E::const_iterator1::iterator_category iterator_category;
#ifndef USE_MSVC
            typedef typename E::const_iterator1::difference_type difference_type;
            typedef typename E::const_iterator1::value_type value_type;
            typedef typename E::const_iterator1::value_type reference;
            typedef typename E::const_iterator1::pointer pointer;
#endif
            typedef const_iterator2 dual_iterator_type;
            typedef const_reverse_iterator2 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator1 ():
                container_const_reference<matrix_unary1> (), it_ () {}
            NUMERICS_INLINE
            const_iterator1 (const matrix_unary1 &mu, const const_iterator1_type &it):
                container_const_reference<matrix_unary1> (mu), it_ (it) {}

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
                return functor_type () (*it_);
            }

            NUMERICS_INLINE
            const_iterator2 begin () const {
                return const_iterator2 ((*this) (), it_.begin ()); 
            }
            NUMERICS_INLINE
            const_iterator2 end () const {
                return const_iterator2 ((*this) (), it_.end ()); 
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
                return it_.index1 ();
            }
            NUMERICS_INLINE
            size_type index2 () const {
                return it_.index2 ();
            }

            // Assignment 
            NUMERICS_INLINE
            const_iterator1 &operator = (const const_iterator1 &it) {
                container_const_reference<matrix_unary1>::assign (&it ());
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

        NUMERICS_INLINE
        const_iterator1 begin1 () const {
            return const_iterator1 (*this, e_.begin1 ());
        }
        NUMERICS_INLINE
        const_iterator1 end1 () const {
            return const_iterator1 (*this, e_.end1 ());
        }

        class const_iterator2:
            public container_const_reference<matrix_unary1>,
            public random_access_iterator_base<const_iterator2, value_type> {
        public:
            typedef typename E::const_iterator2::iterator_category iterator_category;
#ifndef USE_MSVC
            typedef typename E::const_iterator2::difference_type difference_type;
            typedef typename E::const_iterator2::value_type value_type;
            typedef typename E::const_iterator2::value_type reference;
            typedef typename E::const_iterator2::pointer pointer;
#endif
            typedef const_iterator1 dual_iterator_type;
            typedef const_reverse_iterator1 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator2 ():
                container_const_reference<matrix_unary1> (), it_ () {}
            NUMERICS_INLINE
            const_iterator2 (const matrix_unary1 &mu, const const_iterator2_type &it):
                container_const_reference<matrix_unary1> (mu), it_ (it) {}

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
                return functor_type () (*it_);
            }

            NUMERICS_INLINE
            const_iterator1 begin () const {
                return const_iterator1 ((*this) (), it_.begin ()); 
            }
            NUMERICS_INLINE
            const_iterator1 end () const {
                return const_iterator1 ((*this) (), it_.end ()); 
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
                return it_.index1 ();
            }
            NUMERICS_INLINE
            size_type index2 () const {
                return it_.index2 ();
            }

            // Assignment 
            NUMERICS_INLINE
            const_iterator2 &operator = (const const_iterator2 &it) {
                container_const_reference<matrix_unary1>::assign (&it ());
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

        NUMERICS_INLINE
        const_iterator2 begin2 () const {
            return const_iterator2 (*this, e_.begin2 ());
        }
        NUMERICS_INLINE
        const_iterator2 end2 () const {
            return const_iterator2 (*this, e_.end2 ());
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
        const_reverse_iterator2 rbegin2 () const {
            return const_reverse_iterator2 (end2 ());
        }
        NUMERICS_INLINE
        const_reverse_iterator2 rend2 () const {
            return const_reverse_iterator2 (begin2 ());
        }

    private:
#ifdef NUMERICS_ET_VALUE
        expression_type e_;
#endif
#ifdef NUMERICS_ET_REFERENCE
        const expression_type &e_;
#endif
    };

    template<class E, class F>
    struct matrix_unary1_traits {
        typedef matrix_unary1<typename E::const_closure_type, F> expression_type;
#ifdef NUMERICS_USE_ET
        typedef expression_type result_type; 
#else
        typedef matrix<typename E::value_type> result_type;
#endif
    };

    // (- m) [i] [j] = - m [i] [j]
    template<class E> 
    NUMERICS_INLINE
    typename matrix_unary1_traits<E, scalar_negate<typename E::value_type> >::result_type
    operator - (const matrix_expression<E> &e) {
        typedef NUMERICS_TYPENAME matrix_unary1_traits<E, scalar_negate<NUMERICS_TYPENAME E::value_type> >::expression_type expression_type;
        return expression_type (e ());
    }

    // (conj m) [i] [j] = conj (m [i] [j])
    template<class E> 
    NUMERICS_INLINE
    typename matrix_unary1_traits<E, scalar_conj<typename E::value_type> >::result_type
    conj (const matrix_expression<E> &e) {
        typedef NUMERICS_TYPENAME matrix_unary1_traits<E, scalar_conj<NUMERICS_TYPENAME E::value_type> >::expression_type expression_type;
        return expression_type (e ());
    }

    template<class E, class F>
    class matrix_unary2:
        public matrix_expression<matrix_unary2<E, F> > {
    public:
        typedef E expression_type;
        typedef F functor_type;
        typedef typename E::size_type size_type;
        typedef typename E::difference_type difference_type;
        typedef typename F::result_type value_type;
        typedef const value_type &const_reference;
        typedef const_reference reference;
        typedef const value_type *const_pointer;
        typedef const_pointer pointer;
        typedef const matrix_unary2<E, F> const_closure_type;
        typedef typename E::orientation_category orientation_category;
        typedef typename E::const_iterator1 const_iterator2_type;
        typedef typename E::const_iterator2 const_iterator1_type;
#ifdef NUMERICS_DEPRECATED
#ifdef NUMERICS_DEPRECATED
        typedef const matrix_expression_row<const_closure_type> const_matrix_row_type;
        typedef const matrix_expression_column<const_closure_type> const_matrix_column_type;
        typedef const matrix_expression_range<const_closure_type> const_matrix_range_type;
#else
        typedef const matrix_row<const_closure_type> const_matrix_row_type;
        typedef const matrix_column<const_closure_type> const_matrix_column_type;
        typedef const matrix_range<const_closure_type> const_matrix_range_type;
#endif
#endif
        typedef unknown_storage_tag storage_category;

        // Construction and destruction
        NUMERICS_INLINE
        matrix_unary2 (): 
            e_ () {}
        NUMERICS_INLINE
        matrix_unary2 (const expression_type &e): 
            e_ (e) {}

        NUMERICS_INLINE
        size_type size1 () const { 
            return e_.size2 ();
        }
        NUMERICS_INLINE
        size_type size2 () const { 
            return e_.size1 (); 
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i, size_type j) const { 
            return functor_type () (e_ (j, i)); 
        }

#ifdef NUMERICS_DEPRECATED
        NUMERICS_INLINE
        const_matrix_row_type operator [] (size_type i) const {
            return const_matrix_row_type (*this, i); 
        }
        NUMERICS_INLINE
        const_matrix_row_type row (size_type i) const {
            return const_matrix_row_type (*this, i);
        }
        NUMERICS_INLINE
        const_matrix_column_type column (size_type j) const {
            return const_matrix_column_type (*this, j);
        }

        NUMERICS_INLINE
        const_matrix_range_type project (size_type start1, size_type stop1, size_type start2, size_type stop2) const {
            return const_matrix_range_type (*this, start1, stop1, start2, stop2);
        }
        NUMERICS_INLINE
        const_matrix_range_type project (const range &r1, const range &r2) const {
            return const_matrix_range_type (*this, r1, r2);
        }
#endif

        class const_iterator1;
        typedef const_iterator1 iterator1;
        class const_iterator2;
        typedef const_iterator2 iterator2;

        // Element lookup
        NUMERICS_INLINE
        const_iterator1 find1 (int rank, size_type i, size_type j) const {
            return const_iterator1 (*this, e_.find2 (rank, j, i));
        }
        NUMERICS_INLINE
        const_iterator2 find2 (int rank, size_type i, size_type j) const {
            return const_iterator2 (*this, e_.find1 (rank, j, i));
        }

        // Iterators enhance the iterators of the referenced expression
        // with the unary functor.

#ifdef USE_MSVC
        typedef reverse_iterator1<const_iterator1, value_type, value_type> const_reverse_iterator1;
        typedef reverse_iterator2<const_iterator2, value_type, value_type> const_reverse_iterator2;
#else
        typedef reverse_iterator1<const_iterator1> const_reverse_iterator1;
        typedef reverse_iterator2<const_iterator2> const_reverse_iterator2;
#endif

        class const_iterator1:
            public container_const_reference<matrix_unary2>,
            public random_access_iterator_base<const_iterator1, value_type> {
        public:
            typedef typename E::const_iterator1::iterator_category iterator_category;
#ifndef USE_MSVC
            typedef typename E::const_iterator1::difference_type difference_type;
            typedef typename E::const_iterator1::value_type value_type;
            typedef typename E::const_iterator1::value_type reference;
            typedef typename E::const_iterator1::pointer pointer;
#endif
            typedef const_iterator2 dual_iterator_type;
            typedef const_reverse_iterator2 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator1 ():
                container_const_reference<matrix_unary2> (), it_ () {}
            NUMERICS_INLINE
            const_iterator1 (const matrix_unary2 &mu, const const_iterator1_type &it):
                container_const_reference<matrix_unary2> (mu), it_ (it) {}

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
                return functor_type () (*it_);
            }

            NUMERICS_INLINE
            const_iterator2 begin () const {
                return const_iterator2 ((*this) (), it_.begin ()); 
            }
            NUMERICS_INLINE
            const_iterator2 end () const {
                return const_iterator2 ((*this) (), it_.end ()); 
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
                return it_.index2 ();
            }
            NUMERICS_INLINE
            size_type index2 () const {
                return it_.index1 ();
            }

            // Assignment 
            NUMERICS_INLINE
            const_iterator1 &operator = (const const_iterator1 &it) {
                container_const_reference<matrix_unary2>::assign (&it ());
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

        NUMERICS_INLINE
        const_iterator1 begin1 () const {
            return const_iterator1 (*this, e_.begin2 ());
        }
        NUMERICS_INLINE
        const_iterator1 end1 () const {
            return const_iterator1 (*this, e_.end2 ());
        }

        class const_iterator2:
            public container_const_reference<matrix_unary2>,
            public random_access_iterator_base<const_iterator2, value_type> {
        public:
            typedef typename E::const_iterator2::iterator_category iterator_category;
#ifndef USE_MSVC
            typedef typename E::const_iterator2::difference_type difference_type;
            typedef typename E::const_iterator2::value_type value_type;
            typedef typename E::const_iterator2::value_type reference;
            typedef typename E::const_iterator2::pointer pointer;
#endif
            typedef const_iterator1 dual_iterator_type;
            typedef const_reverse_iterator1 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator2 ():
                container_const_reference<matrix_unary2> (), it_ () {}
            NUMERICS_INLINE
            const_iterator2 (const matrix_unary2 &mu, const const_iterator2_type &it):
                container_const_reference<matrix_unary2> (mu), it_ (it) {}

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
                return functor_type () (*it_);
            }

            NUMERICS_INLINE
            const_iterator1 begin () const {
                return const_iterator1 ((*this) (), it_.begin ()); 
            }
            NUMERICS_INLINE
            const_iterator1 end () const {
                return const_iterator1 ((*this) (), it_.end ()); 
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
                return it_.index2 ();
            }
            NUMERICS_INLINE
            size_type index2 () const {
                return it_.index1 ();
            }

            // Assignment 
            NUMERICS_INLINE
            const_iterator2 &operator = (const const_iterator2 &it) {
                container_const_reference<matrix_unary2>::assign (&it ());
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

        NUMERICS_INLINE
        const_iterator2 begin2 () const {
            return const_iterator2 (*this, e_.begin1 ());
        }
        NUMERICS_INLINE
        const_iterator2 end2 () const {
            return const_iterator2 (*this, e_.end1 ());
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
        const_reverse_iterator2 rbegin2 () const {
            return const_reverse_iterator2 (end2 ());
        }
        NUMERICS_INLINE
        const_reverse_iterator2 rend2 () const {
            return const_reverse_iterator2 (begin2 ());
        }

    private:
#ifdef NUMERICS_ET_VALUE
        expression_type e_;
#endif
#ifdef NUMERICS_ET_REFERENCE
        const expression_type &e_;
#endif
    };

    template<class E, class F>
    struct matrix_unary2_traits {
        typedef matrix_unary2<typename E::const_closure_type, F> expression_type;
#ifdef NUMERICS_USE_ET
        typedef expression_type result_type; 
#else
        typedef matrix<typename E::value_type> result_type;
#endif
    };

    // (trans m) [i] [j] = m [j] [i]
    template<class E> 
    NUMERICS_INLINE
    typename matrix_unary2_traits<E, scalar_identity<typename E::value_type> >::result_type
    trans (const matrix_expression<E> &e) {
        typedef NUMERICS_TYPENAME matrix_unary2_traits<E, scalar_identity<NUMERICS_TYPENAME E::value_type> >::expression_type expression_type;
        return expression_type (e ());
    }

    // (herm m) [i] [j] = conj (m [j] [i])
    template<class E> 
    NUMERICS_INLINE
    typename matrix_unary2_traits<E, scalar_conj<typename E::value_type> >::result_type
    herm (const matrix_expression<E> &e) {
        typedef NUMERICS_TYPENAME matrix_unary2_traits<E, scalar_conj<NUMERICS_TYPENAME E::value_type> >::expression_type expression_type;
        return expression_type (e ());
    }

    template<class E1, class E2, class F>
    class matrix_binary:
        public matrix_expression<matrix_binary<E1, E2, F> > {
    public:
        typedef E1 expression1_type;
        typedef E2 expression2_type;
        typedef F functor_type;
        typedef typename promote_traits<typename E1::size_type, typename E2::size_type>::promote_type size_type;
        typedef typename promote_traits<typename E1::difference_type, typename E2::difference_type>::promote_type difference_type;
        typedef typename F::result_type value_type;
        typedef const value_type &const_reference;
        typedef const_reference reference;
        typedef const value_type *const_pointer;
        typedef const_pointer pointer;
        typedef const matrix_binary<E1, E2, F> const_closure_type;
        typedef unknown_orientation_tag orientation_category;
        typedef typename E1::const_iterator1 const_iterator11_type;
        typedef typename E1::const_iterator2 const_iterator12_type;
        typedef typename E2::const_iterator1 const_iterator21_type;
        typedef typename E2::const_iterator2 const_iterator22_type;
#ifdef NUMERICS_DEPRECATED
#ifdef NUMERICS_DEPRECATED
        typedef const matrix_expression_row<const_closure_type> const_matrix_row_type;
        typedef const matrix_expression_column<const_closure_type> const_matrix_column_type;
        typedef const matrix_expression_range<const_closure_type> const_matrix_range_type;
#else
        typedef const matrix_row<const_closure_type> const_matrix_row_type;
        typedef const matrix_column<const_closure_type> const_matrix_column_type;
        typedef const matrix_range<const_closure_type> const_matrix_range_type;
#endif
#endif
        typedef unknown_storage_tag storage_category;

        // Construction and destruction
        NUMERICS_INLINE
        matrix_binary (): 
            e1_ (), e2_ () {}
        NUMERICS_INLINE
        matrix_binary (const E1 &e1, const E2 &e2): 
            e1_ (e1), e2_ (e2) {}

        NUMERICS_INLINE
        size_type size1 () const { 
            return common (e1_.size1 (), e2_.size1 ());
        }
        NUMERICS_INLINE
        size_type size2 () const { 
            return common (e1_.size2 (), e2_.size2 ()); 
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i, size_type j) const { 
            return functor_type () (e1_ (i, j), e2_ (i, j)); 
        }

#ifdef NUMERICS_DEPRECATED
        NUMERICS_INLINE
        const_matrix_row_type operator [] (size_type i) const {
            return const_matrix_row_type (*this, i); 
        }
        NUMERICS_INLINE
        const_matrix_row_type row (size_type i) const {
            return const_matrix_row_type (*this, i);
        }
        NUMERICS_INLINE
        const_matrix_column_type column (size_type j) const {
            return const_matrix_column_type (*this, j);
        }

        NUMERICS_INLINE
        const_matrix_range_type project (size_type start1, size_type stop1, size_type start2, size_type stop2) const {
            return const_matrix_range_type (*this, start1, stop1, start2, stop2);
        }
        NUMERICS_INLINE
        const_matrix_range_type project (const range &r1, const range &r2) const {
            return const_matrix_range_type (*this, r1, r2);
        }
#endif

        class const_iterator1;
        typedef const_iterator1 iterator1;
        class const_iterator2;
        typedef const_iterator2 iterator2;

        // Element lookup
        NUMERICS_INLINE
        const_iterator1 find1 (int rank, size_type i, size_type j) const {
            return const_iterator1 (*this, i, e1_.find1 (rank, i, j), e2_.find1 (rank, i, j));
        }
        NUMERICS_INLINE
        const_iterator2 find2 (int rank, size_type i, size_type j) const {
            return const_iterator2 (*this, j, e1_.find2 (rank, i, j), e2_.find2 (rank, i, j));
        }

        // Iterators enhance the iterators of the referenced expression
        // with the binary functor.

#ifdef USE_MSVC
        typedef reverse_iterator1<const_iterator1, value_type, value_type> const_reverse_iterator1;
        typedef reverse_iterator2<const_iterator2, value_type, value_type> const_reverse_iterator2;
#else
        typedef reverse_iterator1<const_iterator1> const_reverse_iterator1;
        typedef reverse_iterator2<const_iterator2> const_reverse_iterator2;
#endif

        class const_iterator1:
            public container_const_reference<matrix_binary>,
            public random_access_iterator_base<const_iterator1, value_type> {
        public:
            typedef typename restrict_traits<typename E1::const_iterator1::iterator_category, 
                                             typename E2::const_iterator1::iterator_category>::iterator_category iterator_category;
#ifndef USE_MSVC
            typedef typename matrix_binary::difference_type difference_type;
            typedef typename matrix_binary::value_type value_type;
            typedef typename matrix_binary::value_type reference;
            typedef typename matrix_binary::const_pointer pointer;
#endif
            typedef const_iterator2 dual_iterator_type;
            typedef const_reverse_iterator2 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator1 ():
                container_const_reference<matrix_binary> (), i_ (), it1_ (), it2_ () {}
            NUMERICS_INLINE
            const_iterator1 (const matrix_binary &mb, size_type i, const const_iterator11_type &it1, const const_iterator21_type &it2):
                container_const_reference<matrix_binary> (mb), i_ (i), it1_ (it1), it2_ (it2) {}

            // Random access specializations
            NUMERICS_INLINE
            void increment (std::random_access_iterator_tag) {
                ++ i_, ++ it1_, ++ it2_;
            }
            NUMERICS_INLINE
            void decrement (std::random_access_iterator_tag) {
                -- i_, -- it1_, -- it2_;
            }
            NUMERICS_INLINE
            value_type dereference (std::random_access_iterator_tag) const {
                return functor_type () (*it1_, *it2_);
            }

            // Bidirectional specializations
            NUMERICS_INLINE
            void increment (std::bidirectional_iterator_tag) {
                if (it1_.index1 () <= i_) 
                    ++ it1_;
                if (it2_.index1 () <= i_) 
                    ++ it2_;
                i_ = std::min (it1_.index1 (), it2_.index1 ());
            }
            NUMERICS_INLINE
            void decrement (std::bidirectional_iterator_tag) {
                if (i_ <= it1_.index1 ()) 
                    -- it1_;
                if (i_ <= it2_.index1 ()) 
                    -- it2_;
                i_ = std::max (it1_.index1 (), it2_.index1 ());
            }
            NUMERICS_INLINE
            value_type dereference (std::bidirectional_iterator_tag) const {
#ifndef USE_MSVC
                value_type t1 = i_ - it1_.index1 () ? value_type () : *it1_;
                value_type t2 = i_ - it2_.index1 () ? value_type () : *it2_;
#else
                value_type t1, t2;
                if (i_ - it1_.index1 ()) t1 = value_type (); else t1 = *it1_;
                if (i_ - it2_.index1 ()) t2 = value_type (); else t2 = *it2_;
#endif
                return functor_type () (t1, t2);
            }

            // Arithmetic
            NUMERICS_INLINE
            const_iterator1 &operator ++ () {
                increment (iterator_category ());
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator -- () {
                decrement (iterator_category ());
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator += (difference_type n) {
                i_ += n, it1_ += n, it2_ += n;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator -= (difference_type n) {
                i_ -= n, it1_ -= n, it2_ -= n;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const const_iterator1 &it) const {
                return common (it1_ - it.it1_, it2_ - it.it2_);
            }

            // Dereference
            NUMERICS_INLINE
            value_type operator * () const {
                return dereference (iterator_category ());
            }

            NUMERICS_INLINE
            const_iterator2 begin () const {
//                return const_iterator2 ((*this) (), 0, it1_.begin (), it2_.begin ()); 
                const_iterator12_type it1_begin (it1_.begin ());
                const_iterator22_type it2_begin (it2_.begin ());
                return const_iterator2 ((*this) (), std::min (it1_begin.index2 (), it2_begin.index2 ()), 
                                        it1_begin, it2_begin);
            }
            NUMERICS_INLINE
            const_iterator2 end () const {
//                return const_iterator2 ((*this) (), (*this) ().size2 (), it1_.end (), it2_.end ()); 
                const_iterator12_type it1_end (it1_.end ());
                const_iterator22_type it2_end (it2_.end ());
                return const_iterator2 ((*this) (), std::max (it1_end.index2 (), it2_end .index2 ()), 
                                        it1_end, it2_end);
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
                return i_;
            }
            NUMERICS_INLINE
            size_type index2 () const {
                return common (it1_.index2 (), it2_.index2 ());
            }

            // Assignment 
            NUMERICS_INLINE
            const_iterator1 &operator = (const const_iterator1 &it) {
                container_const_reference<matrix_binary>::assign (&it ());
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
            size_type i_;
            const_iterator11_type it1_;
            const_iterator21_type it2_;
        };

        NUMERICS_INLINE
        const_iterator1 begin1 () const {
//            return const_iterator1 (*this, 0, e1_.begin1 (), e2_.begin1 ());
            const_iterator11_type e1_begin1 (e1_.begin1 ());
            const_iterator21_type e2_begin1 (e2_.begin1 ());
            return const_iterator1 (*this, std::min (e1_begin1.index1 (), e2_begin1.index1 ()), 
                                    e1_begin1, e2_begin1);
        }
        NUMERICS_INLINE
        const_iterator1 end1 () const {
//            return const_iterator1 (*this, size1 (), e1_.end1 (), e2_.end1 ());
            const_iterator11_type e1_end1 (e1_.end1 ());
            const_iterator21_type e2_end1 (e2_.end1 ());
            return const_iterator1 (*this, std::max (e1_end1.index1 (), e2_end1.index1 ()), 
                                    e1_end1, e2_end1);
        }

        class const_iterator2:
            public container_const_reference<matrix_binary>,
            public random_access_iterator_base<const_iterator2, value_type> {
        public:
            typedef typename restrict_traits<typename E1::const_iterator2::iterator_category, 
                                             typename E2::const_iterator2::iterator_category>::iterator_category iterator_category;
#ifndef USE_MSVC
            typedef typename matrix_binary::difference_type difference_type;
            typedef typename matrix_binary::value_type value_type;
            typedef typename matrix_binary::value_type reference;
            typedef typename matrix_binary::const_pointer pointer;
#endif
            typedef const_iterator1 dual_iterator_type;
            typedef const_reverse_iterator1 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator2 ():
                container_const_reference<matrix_binary> (), j_ (), it1_ (), it2_ () {}
            NUMERICS_INLINE
            const_iterator2 (const matrix_binary &mb, size_type j, const const_iterator12_type &it1, const const_iterator22_type &it2):
                container_const_reference<matrix_binary> (mb), j_ (j), it1_ (it1), it2_ (it2) {}

            // Random access specializations
            NUMERICS_INLINE
            void increment (std::random_access_iterator_tag) {
                ++ j_, ++ it1_, ++ it2_;
            }
            NUMERICS_INLINE
            void decrement (std::random_access_iterator_tag) {
                -- j_, -- it1_, -- it2_;
            }
            NUMERICS_INLINE
            value_type dereference (std::random_access_iterator_tag) const {
                return functor_type () (*it1_, *it2_);
            }

            // Bidirectional specializations
            NUMERICS_INLINE
            void increment (std::bidirectional_iterator_tag) {
                if (it1_.index2 () <= j_) 
                    ++ it1_;
                if (it2_.index2 () <= j_) 
                    ++ it2_;
                j_ = std::min (it1_.index2 (), it2_.index2 ());
            }
            NUMERICS_INLINE
            void decrement (std::bidirectional_iterator_tag) {
                if (j_ <= it1_.index2 ()) 
                    -- it1_;
                if (j_ <= it2_.index2 ()) 
                    -- it2_;
                j_ = std::max (it1_.index2 (), it2_.index2 ());
            }
            NUMERICS_INLINE
            value_type dereference (std::bidirectional_iterator_tag) const {
#ifndef USE_MSVC
                value_type t1 = j_ - it1_.index2 () ? value_type () : *it1_;
                value_type t2 = j_ - it2_.index2 () ? value_type () : *it2_;
#else
                value_type t1, t2;
                if (j_ - it1_.index2 ()) t1 = value_type (); else t1 = *it1_;
                if (j_ - it2_.index2 ()) t2 = value_type (); else t2 = *it2_;
#endif
                return functor_type () (t1, t2);
            }

            // Arithmetic
            NUMERICS_INLINE
            const_iterator2 &operator ++ () {
                increment (iterator_category ());
                return *this;
            }
            NUMERICS_INLINE
            const_iterator2 &operator -- () {
                decrement (iterator_category ());
                return *this;
            }
            NUMERICS_INLINE
            const_iterator2 &operator += (difference_type n) {
                j_ += n, it1_ += n, it2_ += n;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator2 &operator -= (difference_type n) {
                j_ -= n, it1_ -= n, it2_ -= n;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const const_iterator2 &it) const {
                return common (it1_ - it.it1_, it2_ - it.it2_);
            }

            // Dereference
            NUMERICS_INLINE
            value_type operator * () const {
                return dereference (iterator_category ());
            }

            NUMERICS_INLINE
            const_iterator1 begin () const {
//                return const_iterator1 ((*this) (), 0, it1_.begin (), it2_.begin ()); 
                const_iterator11_type it1_begin (it1_.begin ());
                const_iterator21_type it2_begin (it2_.begin ());
                return const_iterator1 ((*this) (), std::min (it1_begin.index1 (), it2_begin.index1 ()), 
                                        it1_begin, it2_begin);
            }
            NUMERICS_INLINE
            const_iterator1 end () const {
//                return const_iterator2 ((*this) (), (*this) ().size1 (), it1_.end (), it2_.end ()); 
                const_iterator11_type it1_end (it1_.end ());
                const_iterator21_type it2_end (it2_.end ());
                return const_iterator1 ((*this) (), std::max (it1_end.index1 (), it2_end .index1 ()), 
                                        it1_end, it2_end);
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
                return common (it1_.index1 (), it2_.index1 ());
            }
            NUMERICS_INLINE
            size_type index2 () const {
                return j_;
            }

            // Assignment 
            NUMERICS_INLINE
            const_iterator2 &operator = (const const_iterator2 &it) {
                container_const_reference<matrix_binary>::assign (&it ());
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
            size_type j_;
            const_iterator12_type it1_;
            const_iterator22_type it2_;
        };

        NUMERICS_INLINE
        const_iterator2 begin2 () const {
//            return const_iterator2 (*this, 0, e1_.begin2 (), e2_.begin2 ());
            const_iterator12_type e1_begin2 (e1_.begin2 ());
            const_iterator22_type e2_begin2 (e2_.begin2 ());
            return const_iterator2 (*this, std::min (e1_begin2.index2 (), e2_begin2.index2 ()), 
                                    e1_begin2, e2_begin2);
        }
        NUMERICS_INLINE
        const_iterator2 end2 () const {
//            return const_iterator2 (*this, size2 (), e1_.end2 (), e2_.end2 ());
            const_iterator12_type e1_end2 (e1_.end2 ());
            const_iterator22_type e2_end2 (e2_.end2 ());
            return const_iterator2 (*this, std::max (e1_end2.index2 (), e2_end2.index2 ()), 
                                    e1_end2, e2_end2);
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
        const_reverse_iterator2 rbegin2 () const {
            return const_reverse_iterator2 (end2 ());
        }
        NUMERICS_INLINE
        const_reverse_iterator2 rend2 () const {
            return const_reverse_iterator2 (begin2 ());
        }

    private:
#ifdef NUMERICS_ET_VALUE
        expression1_type e1_;
        expression2_type e2_;
#endif
#ifdef NUMERICS_ET_REFERENCE
        const expression1_type &e1_;
        const expression2_type &e2_;
#endif
    };

    template<class E1, class E2, class F>
    struct matrix_binary_traits {
        typedef matrix_binary<typename E1::const_closure_type, 
                              typename E2::const_closure_type, F> expression_type;
#ifdef NUMERICS_USE_ET
        typedef expression_type result_type; 
#else
        typedef matrix<typename promote_traits<typename E1::value_type, 
                                               typename E2::value_type>::promote_type> result_type;
#endif
    };

    // (m1 + m2) [i] [j] = m1 [i] [j] + m2 [i] [j]
    template<class E1, class E2>
    NUMERICS_INLINE
    typename matrix_binary_traits<E1, E2, scalar_plus<typename E1::value_type, 
                                                      typename E2::value_type> >::result_type
    operator + (const matrix_expression<E1> &e1, 
                const matrix_expression<E2> &e2) {
        typedef NUMERICS_TYPENAME matrix_binary_traits<E1, E2, scalar_plus<NUMERICS_TYPENAME E1::value_type, 
                                                                           NUMERICS_TYPENAME E2::value_type> >::expression_type expression_type;
        return expression_type (e1 (), e2 ());
    }

    // (m1 - m2) [i] [j] = m1 [i] [j] - m2 [i] [j]
    template<class E1, class E2>
    NUMERICS_INLINE
    typename matrix_binary_traits<E1, E2, scalar_minus<typename E1::value_type, 
                                                       typename E2::value_type> >::result_type
    operator - (const matrix_expression<E1> &e1, 
                const matrix_expression<E2> &e2) {
        typedef NUMERICS_TYPENAME matrix_binary_traits<E1, E2, scalar_minus<NUMERICS_TYPENAME E1::value_type, 
                                                                            NUMERICS_TYPENAME E2::value_type> >::expression_type expression_type;
        return expression_type (e1 (), e2 ());
    }

    template<class E1, class E2, class F>
    class matrix_binary_scalar:
        public matrix_expression<matrix_binary_scalar<E1, E2, F> > {
    public:
        typedef E1 expression1_type;
        typedef E2 expression2_type;
        typedef F functor_type;
        typedef typename E2::size_type size_type;
        typedef typename E2::difference_type difference_type;
        typedef typename F::result_type value_type;
        typedef const value_type &const_reference;
        typedef const_reference reference;
        typedef const value_type *const_pointer;
        typedef const_pointer pointer;
        typedef const matrix_binary_scalar<E1, E2, F> const_closure_type;
        typedef typename E2::orientation_category orientation_category;
        typedef typename E1::value_type const_iterator1_type;
        typedef typename E2::const_iterator1 const_iterator21_type;
        typedef typename E2::const_iterator2 const_iterator22_type;
#ifdef NUMERICS_DEPRECATED
#ifdef NUMERICS_DEPRECATED
        typedef const matrix_expression_row<const_closure_type> const_matrix_row_type;
        typedef const matrix_expression_column<const_closure_type> const_matrix_column_type;
        typedef const matrix_expression_range<const_closure_type> const_matrix_range_type;
#else
        typedef const matrix_row<const_closure_type> const_matrix_row_type;
        typedef const matrix_column<const_closure_type> const_matrix_column_type;
        typedef const matrix_range<const_closure_type> const_matrix_range_type;
#endif
#endif
        typedef unknown_storage_tag storage_category;

        // Construction and destruction
        NUMERICS_INLINE
        matrix_binary_scalar (): 
            e1_ (), e2_ () {}
        NUMERICS_INLINE
        matrix_binary_scalar (const expression1_type &e1, const expression2_type &e2): 
            e1_ (e1), e2_ (e2) {}

        NUMERICS_INLINE
        size_type size1 () const { 
            return e2_.size1 ();
        }
        NUMERICS_INLINE
        size_type size2 () const { 
            return e2_.size2 ();
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i, size_type j) const { 
            return functor_type () (e1_, e2_ (i, j)); 
        }

#ifdef NUMERICS_DEPRECATED
        NUMERICS_INLINE
        const_matrix_row_type operator [] (size_type i) const {
            return const_matrix_row_type (*this, i); 
        }
        NUMERICS_INLINE
        const_matrix_row_type row (size_type i) const {
            return const_matrix_row_type (*this, i);
        }
        NUMERICS_INLINE
        const_matrix_column_type column (size_type j) const {
            return const_matrix_column_type (*this, j);
        }

        NUMERICS_INLINE
        const_matrix_range_type project (size_type start1, size_type stop1, size_type start2, size_type stop2) const {
            return const_matrix_range_type (*this, start1, stop1, start2, stop2);
        }
        NUMERICS_INLINE
        const_matrix_range_type project (const range &r1, const range &r2) const {
            return const_matrix_range_type (*this, r1, r2);
        }
#endif

        class const_iterator1;
        typedef const_iterator1 iterator1;
        class const_iterator2;
        typedef const_iterator2 iterator2;

        // Element lookup
        NUMERICS_INLINE
        const_iterator1 find1 (int rank, size_type i, size_type j) const {
            return const_iterator1 (*this, e1_, e2_.find1 (rank, i, j));
        }
        NUMERICS_INLINE
        const_iterator2 find2 (int rank, size_type i, size_type j) const {
            return const_iterator2 (*this, e1_, e2_.find2 (rank, i, j));
        }

        // Iterators enhance the iterators of the referenced expression
        // with the binary functor.

#ifdef USE_MSVC
        typedef reverse_iterator1<const_iterator1, value_type, value_type> const_reverse_iterator1;
        typedef reverse_iterator2<const_iterator2, value_type, value_type> const_reverse_iterator2;
#else
        typedef reverse_iterator1<const_iterator1> const_reverse_iterator1;
        typedef reverse_iterator2<const_iterator2> const_reverse_iterator2;
#endif

        class const_iterator1:
            public container_const_reference<matrix_binary_scalar>,
            public random_access_iterator_base<const_iterator1, value_type> {
        public:
            typedef typename E2::const_iterator1::iterator_category iterator_category;
#ifndef USE_MSVC
            typedef typename E2::const_iterator1::difference_type difference_type;
            typedef typename E2::const_iterator1::value_type value_type;
            typedef typename E2::const_iterator1::value_type reference;
            typedef typename E2::const_iterator1::pointer pointer;
#endif
            typedef const_iterator2 dual_iterator_type;
            typedef const_reverse_iterator2 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator1 ():
                container_const_reference<matrix_binary_scalar> (), it1_ (), it2_ () {}
            NUMERICS_INLINE
            const_iterator1 (const matrix_binary_scalar &mbs, const const_iterator1_type &it1, const const_iterator21_type &it2):
                container_const_reference<matrix_binary_scalar> (mbs), it1_ (it1), it2_ (it2) {}

            // Arithmetic
            NUMERICS_INLINE
            const_iterator1 &operator ++ () {
                ++ it2_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator -- () {
                -- it2_ ;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator += (difference_type n) {
                it2_ += n;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator -= (difference_type n) {
                it2_ -= n;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const const_iterator1 &it) const {
                return it2_ - it.it2_;
            }

            // Dereference
            NUMERICS_INLINE
            value_type operator * () const {
                return functor_type () (it1_, *it2_); 
            }

            NUMERICS_INLINE
            const_iterator2 begin () const {
                return const_iterator2 ((*this) (), it1_, it2_.begin ()); 
            }
            NUMERICS_INLINE
            const_iterator2 end () const {
                return const_iterator2 ((*this) (), it1_, it2_.end ()); 
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
                return it2_.index1 ();
            }
            NUMERICS_INLINE
            size_type index2 () const {
                return it2_.index2 ();
            }

            // Assignment 
            NUMERICS_INLINE
            const_iterator1 &operator = (const const_iterator1 &it) {
                container_const_reference<matrix_binary_scalar>::assign (&it ());
                it1_ = it.it1_;
                it2_ = it.it2_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator1 &it) const {
                check (&(*this) () == &it (), external_logic ());
                // FIXME: we shouldn't compare floats
                // check (it1_ == it.it1_, external_logic ());
                return it2_ == it.it2_;
            }

        private:
            const_iterator1_type it1_;
            const_iterator21_type it2_;
        };

        NUMERICS_INLINE
        const_iterator1 begin1 () const {
            return const_iterator1 (*this, e1_, e2_.begin1 ());
        }
        NUMERICS_INLINE
        const_iterator1 end1 () const {
            return const_iterator1 (*this, e1_, e2_.end1 ());
        }

        class const_iterator2:
            public container_const_reference<matrix_binary_scalar>,
            public random_access_iterator_base<const_iterator2, value_type> {
        public:
            typedef typename E2::const_iterator2::iterator_category iterator_category;
#ifndef USE_MSVC
            typedef typename E2::const_iterator2::difference_type difference_type;
            typedef typename E2::const_iterator2::value_type value_type;
            typedef typename E2::const_iterator2::value_type reference;
            typedef typename E2::const_iterator2::pointer pointer;
#endif
            typedef const_iterator1 dual_iterator_type;
            typedef const_reverse_iterator1 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator2 ():
                container_const_reference<matrix_binary_scalar> (), it1_ (), it2_ () {}
            NUMERICS_INLINE
            const_iterator2 (const matrix_binary_scalar &mbs, const const_iterator1_type &it1, const const_iterator22_type &it2):
                container_const_reference<matrix_binary_scalar> (mbs), it1_ (it1), it2_ (it2) {}

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
                return functor_type () (it1_, *it2_); 
            }

            NUMERICS_INLINE
            const_iterator1 begin () const {
                return const_iterator1 ((*this) (), it1_, it2_.begin ()); 
            }
            NUMERICS_INLINE
            const_iterator1 end () const {
                return const_iterator1 ((*this) (), it1_, it2_.end ()); 
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
                return it2_.index1 ();
            }
            NUMERICS_INLINE
            size_type index2 () const {
                return it2_.index2 ();
            }

            // Assignment 
            NUMERICS_INLINE
            const_iterator2 &operator = (const const_iterator2 &it) {
                container_const_reference<matrix_binary_scalar>::assign (&it ());
                it1_ = it.it1_;
                it2_ = it.it2_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator2 &it) const {
                check (&(*this) () == &it (), external_logic ());
                // FIXME: we shouldn't compare floats
                // check (it1_ == it.it1_, external_logic ());
                return it2_ == it.it2_;
            }

        private:
            const_iterator1_type it1_;
            const_iterator22_type it2_;
        };

        NUMERICS_INLINE
        const_iterator2 begin2 () const {
            return const_iterator2 (*this, e1_, e2_.begin2 ());
        }
        NUMERICS_INLINE
        const_iterator2 end2 () const {
            return const_iterator2 (*this, e1_, e2_.end2 ());
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
        const_reverse_iterator2 rbegin2 () const {
            return const_reverse_iterator2 (end2 ());
        }
        NUMERICS_INLINE
        const_reverse_iterator2 rend2 () const {
            return const_reverse_iterator2 (begin2 ());
        }

    private:
#ifdef NUMERICS_ET_VALUE
        expression1_type e1_;
        expression2_type e2_;
#endif
#ifdef NUMERICS_ET_REFERENCE
        const expression1_type &e1_;
        const expression2_type &e2_;
#endif
    };

    template<class T1, class E2, class F>
    struct matrix_binary_scalar_traits {
        typedef matrix_binary_scalar<scalar_const_reference<T1>, 
                                     typename E2::const_closure_type, F> expression_type;
#ifdef NUMERICS_USE_ET
        typedef expression_type result_type; 
#else
        typedef matrix<typename promote_traits<T1, typename E2::value_type>::promote_type> result_type;
#endif
    };

    // (t * m) [i] [j] = t * m [i] [j]
    template<class T1, class E2>
    NUMERICS_INLINE
    typename matrix_binary_scalar_traits<T1, E2, scalar_multiplies<T1, typename E2::value_type> >::result_type
    operator * (const T1 &e1, 
                const matrix_expression<E2> &e2) {
        typedef NUMERICS_TYPENAME matrix_binary_scalar_traits<T1, E2, scalar_multiplies<T1, NUMERICS_TYPENAME E2::value_type> >::expression_type expression_type;
        return expression_type (e1, e2 ());
    }

#ifdef NUMERICS_DEPRECATED

    template<class E>
    class matrix_expression_row:
        public vector_expression<matrix_expression_row<E> > {
    public:
        typedef E expression_type;
        typedef typename E::size_type size_type;
        typedef typename E::difference_type difference_type;
        typedef typename E::value_type value_type;
        typedef typename E::const_reference const_reference;
        typedef const_reference reference;
        typedef typename E::const_pointer const_pointer;
        typedef const_pointer pointer;
        typedef const matrix_expression_row<E> const_closure_type;
        typedef typename E::size_type const_iterator_type;
#ifdef NUMERICS_DEPRECATED
        typedef const vector_expression_range<const_closure_type> const_vector_range_type;
#endif
        typedef unknown_storage_tag storage_category;

        // Construction and destruction
        NUMERICS_INLINE
        matrix_expression_row (): 
            e_ (), i_ () {}
        NUMERICS_INLINE
        matrix_expression_row (const expression_type &e, size_type i): 
            e_ (e), i_ (i) {}

        NUMERICS_INLINE
        size_type size () const { 
            return e_.size2 ();
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type j) const { 
            return e_ (i_, j); 
        }

#ifdef NUMERICS_DEPRECATED
        NUMERICS_INLINE
        value_type operator [] (size_type j) const { 
            return e_ (i_, j); 
        }

        NUMERICS_INLINE
        const_vector_range_type project (size_type start, size_type stop) const {
            return const_vector_range_type (*this, start, stop);
        }
        NUMERICS_INLINE
        const_vector_range_type project (const range &r) const {
            return const_vector_range_type (*this, r);
        }
#endif

        class const_iterator;
        typedef const_iterator iterator;

        // Element lookup
        NUMERICS_INLINE
        const_iterator find (size_type j) const {
            return const_iterator (*this, j);
        }

        // Iterator simply is an index.
        // FIXME: is it possible to rearrange this using pointers?

        class const_iterator:
            public container_const_reference<matrix_expression_row>,
            public random_access_iterator_base<const_iterator, value_type> {
        public:
            typedef typename E::const_iterator2::iterator_category iterator_category;
#ifndef USE_MSVC
            typedef typename E::const_iterator2::difference_type difference_type;
            typedef typename E::const_iterator2::value_type value_type;
            typedef typename E::const_iterator2::value_type reference;
            typedef typename E::const_iterator2::pointer pointer;
#endif

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator (): 
                container_const_reference<matrix_expression_row> (), it_ () {}
            NUMERICS_INLINE
            const_iterator (const matrix_expression_row &mvu, const const_iterator_type &it): 
                container_const_reference<matrix_expression_row> (mvu), it_ (it) {}

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
                return (*this) () (it_); 
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                return it_;
            }

            // Assignment 
            NUMERICS_INLINE
            const_iterator &operator = (const const_iterator &it) {
                container_const_reference<matrix_expression_row>::assign (&it ());
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
            return const_iterator (*this, 0); 
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return const_iterator (*this, e_.size2 ()); 
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
#ifdef NUMERICS_ET_VALUE
        expression_type e_;
#endif
#ifdef NUMERICS_ET_REFERENCE
        const expression_type &e_;
#endif
        size_type i_;
    };

    template<class E>
    class matrix_expression_column:
        public vector_expression<matrix_expression_column<E> > {
    public:
        typedef E expression_type;
        typedef typename E::size_type size_type;
        typedef typename E::difference_type difference_type;
        typedef typename E::value_type value_type;
        typedef typename E::const_reference const_reference;
        typedef const_reference reference;
        typedef typename E::const_pointer const_pointer;
        typedef const_pointer pointer;
        typedef const matrix_expression_column<E> const_closure_type;
        typedef typename E::size_type const_iterator_type;
#ifdef NUMERICS_DEPRECATED
        typedef const vector_expression_range<const_closure_type> const_vector_range_type;
#endif
        typedef unknown_storage_tag storage_category;

        // Construction and destruction
        NUMERICS_INLINE
        matrix_expression_column (): 
            e_ (), j_ () {}
        NUMERICS_INLINE
        matrix_expression_column (const expression_type &e, size_type j): 
            e_ (e), j_ (j) {}

        NUMERICS_INLINE
        size_type size () const { 
            return e_.size1 ();
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i) const { 
            return e_ (i, j_); 
        }

#ifdef NUMERICS_DEPRECATED
        NUMERICS_INLINE
        value_type operator [] (size_type i) const { 
            return e_ (i, j_); 
        }

        NUMERICS_INLINE
        const_vector_range_type project (size_type start, size_type stop) const {
            return const_vector_range_type (*this, start, stop);
        }
        NUMERICS_INLINE
        const_vector_range_type project (const range &r) const {
            return const_vector_range_type (*this, r);
        }
#endif

        class const_iterator;
        typedef const_iterator iterator;

        // Element lookup
        NUMERICS_INLINE
        const_iterator find (size_type i) const {
            return const_iterator (*this, i);
        }

        // Iterator simply is an index.
        // FIXME: is it possible to rearrange this using pointers?

        class const_iterator:
            public container_const_reference<matrix_expression_column>,
            public random_access_iterator_base<const_iterator, value_type> {
        public:
            typedef typename E::const_iterator1::iterator_category iterator_category;
#ifndef USE_MSVC
            typedef typename E::const_iterator1::difference_type difference_type;
            typedef typename E::const_iterator1::value_type value_type;
            typedef typename E::const_iterator1::value_type reference;
            typedef typename E::const_iterator1::pointer pointer;
#endif

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator (): 
                container_const_reference<matrix_expression_column> (), it_ () {}
            NUMERICS_INLINE
            const_iterator (const matrix_expression_column &mvu, const const_iterator_type &it): 
                container_const_reference<matrix_expression_column> (mvu), it_ (it) {}

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
                return (*this) () (it_); 
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                return it_;
            }

            // Assignment 
            NUMERICS_INLINE
            const_iterator &operator = (const const_iterator &it) {
                container_const_reference<matrix_expression_column>::assign (&it ());
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
            return const_iterator (*this, 0); 
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return const_iterator (*this, e_.size1 ()); 
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
#ifdef NUMERICS_ET_VALUE
        expression_type e_;
#endif
#ifdef NUMERICS_ET_REFERENCE
        const expression_type &e_;
#endif
        size_type j_;
    };

    template<class E>
    struct matrix_expression_row_traits {
        typedef matrix_expression_row<typename E::const_closure_type> expression_type;
#ifdef NUMERICS_USE_ET
        typedef expression_type result_type; 
#else
        typedef vector<typename E::value_type> result_type;
#endif
    };

    template<class E>
    NUMERICS_INLINE
    typename matrix_expression_row_traits<E>::result_type
    row (const matrix_expression<E> &e, 
         std::size_t i) {
        typedef NUMERICS_TYPENAME matrix_expression_row_traits<E>::expression_type expression_type;
        return expression_type (e (), i);
    }

    template<class E>
    struct matrix_expression_column_traits {
        typedef matrix_expression_column<typename E::const_closure_type> expression_type;
#ifdef NUMERICS_USE_ET
        typedef expression_type result_type; 
#else
        typedef vector<typename E::value_type> result_type;
#endif
    };

    template<class E>
    NUMERICS_INLINE
    typename matrix_expression_column_traits<E>::result_type
    column (const matrix_expression<E> &e, 
            std::size_t j) {
        typedef NUMERICS_TYPENAME matrix_expression_column_traits<E>::expression_type expression_type;
        return expression_type (e (), j);
    }

#endif

    template<class E1, class E2, class F>
    class matrix_vector_binary1:
        public vector_expression<matrix_vector_binary1<E1, E2, F> > {
    public:
        typedef E1 expression1_type;
        typedef E2 expression2_type;
        typedef F functor_type;
        typedef typename promote_traits<typename E1::size_type, typename E2::size_type>::promote_type size_type;
        typedef typename promote_traits<typename E1::difference_type, typename E2::difference_type>::promote_type difference_type;
        typedef typename F::result_type value_type;
        typedef const value_type &const_reference;
        typedef const_reference reference;
        typedef const value_type *const_pointer;
        typedef const_pointer pointer;
        typedef const matrix_vector_binary1<E1, E2, F> const_closure_type;
        typedef typename E1::const_iterator1 const_iterator1_type;
        typedef typename E2::const_iterator const_iterator2_type;
#ifdef NUMERICS_DEPRECATED
#ifdef NUMERICS_DEPRECATED
        typedef const vector_expression_range<const_closure_type> const_vector_range_type;
#else
        typedef const vector_range<const_closure_type> const_vector_range_type;
#endif
#endif
        typedef unknown_storage_tag storage_category;

        // Construction and destruction
        NUMERICS_INLINE
        matrix_vector_binary1 (): 
            e1_ (), e2_ () {}
        NUMERICS_INLINE
        matrix_vector_binary1 (const expression1_type &e1, const expression2_type &e2): 
            e1_ (e1), e2_ (e2) {}

        NUMERICS_INLINE
        const expression1_type &expression1 () const {
            return e1_;
        }
        NUMERICS_INLINE
        const expression2_type &expression2 () const {
            return e2_;
        }

        NUMERICS_INLINE
        size_type size () const { 
            return e1_.size1 (); 
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i) const {
            return functor_type () (e1_, e2_, i); 
        }

#ifdef NUMERICS_DEPRECATED
        NUMERICS_INLINE
        value_type operator [] (size_type i) const { 
            return functor_type () (e1_, e2_, i); 
        }

        NUMERICS_INLINE
        const_vector_range_type project (size_type start, size_type stop) const {
            return const_vector_range_type (*this, start, stop);
        }
        NUMERICS_INLINE
        const_vector_range_type project (const range &r) const {
            return const_vector_range_type (*this, r);
        }
#endif

        class const_iterator;
        typedef const_iterator iterator;

        // Element lookup
        NUMERICS_INLINE
        const_iterator find (size_type i) const {
            return const_iterator (e2_, e1_.find1 (0, i, 0));
        }

        // Iterator simply is a pointer.

        class const_iterator:
            public container_const_reference<matrix_vector_binary1>,
            public random_access_iterator_base<const_iterator, value_type> {
        public:
            typedef typename restrict_traits<typename E1::const_iterator1::iterator_category, 
                                             typename E2::const_iterator::iterator_category>::iterator_category iterator_category;
#ifndef USE_MSVC
            typedef typename matrix_vector_binary1::difference_type difference_type;
            typedef typename matrix_vector_binary1::value_type value_type;
            typedef typename matrix_vector_binary1::value_type reference;
            typedef typename matrix_vector_binary1::const_pointer pointer;
#endif

            // Construction and destruction
            NUMERICS_INLINE
#ifdef NUMERICS_USE_INVARIANT_HOISTING
            const_iterator ():
                container_const_reference<matrix_vector_binary1> (), it1_ (), e2_begin_ (), e2_end_ () {}
            const_iterator (const matrix_vector_binary1 &mvb, const const_iterator1_type &it1):
                container_const_reference<matrix_vector_binary1> (mvb), it1_ (it1), e2_begin_ (mvb.expression2 ().begin ()), e2_end_ (mvb.expression2 ().end ()) {}
#else
            const_iterator ():
                container_const_reference<matrix_vector_binary1> (), it1_ () {}
            const_iterator (const matrix_vector_binary1 &mvb, const const_iterator1_type &it1):
                container_const_reference<matrix_vector_binary1> (mvb), it1_ (it1) {}
#endif

            // Dense random access specialization
            NUMERICS_INLINE
            value_type dereference (dense_random_access_iterator_tag) const {
                const matrix_vector_binary1 &mvb = (*this) ();
                difference_type size = common (mvb.expression1 ().size2 (), mvb.expression2 ().size ());
                if (size >= NUMERICS_ITERATOR_THRESHOLD)
#ifdef NUMERICS_USE_INVARIANT_HOISTING
                    return functor_type () (size, it1_.begin (), e2_begin_);
#else
                    return functor_type () (size, it1_.begin (), mvb.expression2 ().begin ());
#endif
                else
                    return mvb (index ());
            }

            // Packed bidirectional specialization
            NUMERICS_INLINE
            value_type dereference (packed_bidirectional_iterator_tag) const {
#ifdef NUMERICS_USE_INVARIANT_HOISTING
                return functor_type () (it1_.begin (), it1_.end (), e2_begin_, e2_end_);
#else
                const matrix_vector_binary1 &mvb = (*this) ();
                return functor_type () (it1_.begin (), it1_.end (), mvb.expression2 ().begin (), mvb.expression2 ().end ());
#endif
            }

            // Sparse bidirectional specialization
            NUMERICS_INLINE
            value_type dereference (sparse_bidirectional_iterator_tag) const {
#ifdef NUMERICS_USE_INVARIANT_HOISTING
                return functor_type () (it1_.begin (), it1_.end (), e2_begin_, e2_end_, sparse_bidirectional_iterator_tag ());
#else
                const matrix_vector_binary1 &mvb = (*this) ();
                return functor_type () (it1_.begin (), it1_.end (), mvb.expression2 ().begin (), mvb.expression2 ().end (), sparse_bidirectional_iterator_tag ());
#endif
            }

            // Arithmetic
            NUMERICS_INLINE
            const_iterator &operator ++ () {
                ++ it1_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator &operator -- () {
                -- it1_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator &operator += (difference_type n) {
                it1_ += n;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator &operator -= (difference_type n) {
                it1_ -= n;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const const_iterator &it) const {
                return it1_ - it.it1_;
            }

            // Dereference
            NUMERICS_INLINE
            value_type operator * () const {
                return dereference (iterator_category ());
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                return it1_.index1 ();
            }

            // Assignment 
            NUMERICS_INLINE
            const_iterator &operator = (const const_iterator &it) {
                container_const_reference<matrix_vector_binary1>::assign (&it ());
                it1_ = it.it1_;
#ifdef NUMERICS_USE_INVARIANT_HOISTING
                e2_begin_ = it.e2_begin_;
                e2_end_ = it.e2_end_;
#endif
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it1_ == it.it1_; 
            }

        private:
            const_iterator1_type it1_;
#ifdef NUMERICS_USE_INVARIANT_HOISTING
            // Mutable due to assignment 
            /* const */ const_iterator2_type e2_begin_;
            /* const */ const_iterator2_type e2_end_;
#endif
        };

        NUMERICS_INLINE
        const_iterator begin () const {
            return const_iterator (*this, e1_.begin1 ()); 
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return const_iterator (*this, e1_.end1 ()); 
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
#ifdef NUMERICS_ET_VALUE
        expression1_type e1_;
        expression2_type e2_;
#endif
#ifdef NUMERICS_ET_REFERENCE
        const expression1_type &e1_;
        const expression2_type &e2_;
#endif
    };

    template<class T1, class E1, class T2, class E2>
    struct matrix_vector_binary1_traits {
        typedef row_major_tag dispatch_category;
        typedef typename promote_traits<T1, T2>::promote_type promote_type;
        typedef matrix_vector_binary1<typename E1::const_closure_type, 
                                      typename E2::const_closure_type, 
                                      matrix_vector_prod1<T1, T2, promote_type> > expression_type;
#ifdef NUMERICS_USE_ET
        typedef expression_type result_type; 
#else
        typedef vector<promote_type> result_type;
#endif
    };

    template<class E1, class E2>
    NUMERICS_INLINE
    typename matrix_vector_binary1_traits<typename E1::value_type, E1, 
                                          typename E2::value_type, E2>::result_type
    prod (const matrix_expression<E1> &e1, 
          const vector_expression<E2> &e2,
          row_major_tag) {
        typedef NUMERICS_TYPENAME matrix_vector_binary1_traits<NUMERICS_TYPENAME E1::value_type, E1, 
                                                               NUMERICS_TYPENAME E2::value_type, E2>::expression_type expression_type;
        return expression_type (e1 (), e2 ());
    }

    // Dispatcher
    template<class E1, class E2>
    NUMERICS_INLINE
    typename matrix_vector_binary1_traits<typename E1::value_type, E1, 
                                          typename E2::value_type, E2>::result_type
    prod (const matrix_expression<E1> &e1, 
          const vector_expression<E2> &e2) {
        typedef NUMERICS_TYPENAME matrix_vector_binary1_traits<NUMERICS_TYPENAME E1::value_type, E1, 
                                                               NUMERICS_TYPENAME E2::value_type, E2>::dispatch_category dispatch_category;
        return prod (e1, e2, dispatch_category ());
    }

    template<class E1, class E2>
    NUMERICS_INLINE
    typename matrix_vector_binary1_traits<typename type_traits<typename E1::value_type>::precision_type, E1, 
                                          typename type_traits<typename E2::value_type>::precision_type, E2>::result_type
    prec_prod (const matrix_expression<E1> &e1, 
               const vector_expression<E2> &e2,
               row_major_tag) {
        typedef NUMERICS_TYPENAME matrix_vector_binary1_traits<NUMERICS_TYPENAME type_traits<NUMERICS_TYPENAME E1::value_type>::precision_type, E1, 
                                                               NUMERICS_TYPENAME type_traits<NUMERICS_TYPENAME E2::value_type>::precision_type, E2>::expression_type expression_type;
        return expression_type (e1 (), e2 ());
    }

    // Dispatcher
    template<class E1, class E2>
    NUMERICS_INLINE
    typename matrix_vector_binary1_traits<typename type_traits<typename E1::value_type>::precision_type, E1, 
                                          typename type_traits<typename E2::value_type>::precision_type, E2>::result_type
    prec_prod (const matrix_expression<E1> &e1, 
               const vector_expression<E2> &e2) {
        typedef NUMERICS_TYPENAME matrix_vector_binary1_traits<NUMERICS_TYPENAME type_traits<NUMERICS_TYPENAME E1::value_type>::precision_type, E1, 
                                                               NUMERICS_TYPENAME type_traits<NUMERICS_TYPENAME E2::value_type>::precision_type, E2>::dispatch_category dispatch_category;
        return prec_prod (e1, e2, dispatch_category ());
    }

    template<class E1, class E2, class F>
    class matrix_vector_binary2:
        public vector_expression<matrix_vector_binary2<E1, E2, F> > {
    public:
        typedef E1 expression1_type;
        typedef E2 expression2_type;
        typedef F functor_type;
        typedef typename promote_traits<typename E1::size_type, typename E2::size_type>::promote_type size_type;
        typedef typename promote_traits<typename E1::difference_type, typename E2::difference_type>::promote_type difference_type;
        typedef typename F::result_type value_type;
        typedef const value_type &const_reference;
        typedef const_reference reference;
        typedef const value_type *const_pointer;
        typedef const_pointer pointer;
        typedef const matrix_vector_binary2<E1, E2, F> const_closure_type;
        typedef typename E1::const_iterator const_iterator1_type;
        typedef typename E2::const_iterator2 const_iterator2_type;
#ifdef NUMERICS_DEPRECATED
#ifdef NUMERICS_DEPRECATED
        typedef const vector_expression_range<const_closure_type> const_vector_range_type;
#else
        typedef const vector_range<const_closure_type> const_vector_range_type;
#endif
#endif
        typedef unknown_storage_tag storage_category;

        // Construction and destruction
        NUMERICS_INLINE
        matrix_vector_binary2 (): 
            e1_ (), e2_ () {}
        NUMERICS_INLINE
        matrix_vector_binary2 (const expression1_type &e1, const expression2_type &e2): 
            e1_ (e1), e2_ (e2) {}

        NUMERICS_INLINE
        const expression1_type &expression1 () const {
            return e1_;
        }
        NUMERICS_INLINE
        const expression2_type &expression2 () const {
            return e2_;
        }

        NUMERICS_INLINE
        size_type size () const { 
            return e2_.size2 (); 
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type j) const { 
            return functor_type () (e1_, e2_, j); 
        }

#ifdef NUMERICS_DEPRECATED
        NUMERICS_INLINE
        value_type operator [] (size_type j) const { 
            return functor_type () (e1_, e2_, j); 
        }

        NUMERICS_INLINE
        const_vector_range_type project (size_type start, size_type stop) const {
            return const_vector_range_type (*this, start, stop);
        }
        NUMERICS_INLINE
        const_vector_range_type project (const range &r) const {
            return const_vector_range_type (*this, r);
        }
#endif

        class const_iterator;
        typedef const_iterator iterator;

        // Element lookup
        NUMERICS_INLINE
        const_iterator find (size_type j) const {
            return const_iterator (e1_, e2_.find2 (0, 0, j));
        }

        // Iterator simply is a pointer.

        class const_iterator:
            public container_const_reference<matrix_vector_binary2>,
            public random_access_iterator_base<const_iterator, value_type> {
        public:
            typedef typename restrict_traits<typename E1::const_iterator::iterator_category, 
                                             typename E2::const_iterator2::iterator_category>::iterator_category iterator_category;
#ifndef USE_MSVC
            typedef typename matrix_vector_binary2::difference_type difference_type;
            typedef typename matrix_vector_binary2::value_type value_type;
            typedef typename matrix_vector_binary2::value_type reference;
            typedef typename matrix_vector_binary2::const_pointer pointer;
#endif

            // Construction and destruction
            NUMERICS_INLINE
#ifdef NUMERICS_USE_INVARIANT_HOISTING
            const_iterator ():
                container_const_reference<matrix_vector_binary2> (), it2_ (), e1_begin_ (), e1_end_ () {}
            const_iterator (const matrix_vector_binary2 &mvb, const const_iterator2_type &it2):
                container_const_reference<matrix_vector_binary2> (mvb), it2_ (it2), e1_begin_ (mvb.expression1 ().begin ()), e1_end_ (mvb.expression1 ().end ()) {}
#else
            const_iterator ():
                container_const_reference<matrix_vector_binary2> (), it2_ () {}
            const_iterator (const matrix_vector_binary2 &mvb, const const_iterator2_type &it2):
                container_const_reference<matrix_vector_binary2> (mvb), it2_ (it2) {}
#endif

            // Dense random access specialization
            NUMERICS_INLINE
            value_type dereference (dense_random_access_iterator_tag) const {
                const matrix_vector_binary2 &mvb = (*this) ();
                difference_type size = common (mvb.expression2 ().size1 (), mvb.expression1 ().size ());
                if (size >= NUMERICS_ITERATOR_THRESHOLD)
#ifdef NUMERICS_USE_INVARIANT_HOISTING
                    return functor_type () (size, it2_.begin (), e1_begin_);
#else
                    return functor_type () (size, it2_.begin (), mvb.expression1 ().begin ());
#endif
                else
                    return mvb (index ());
            }

            // Packed bidirectional specialization
            NUMERICS_INLINE
            value_type dereference (packed_bidirectional_iterator_tag) const {
#ifdef NUMERICS_USE_INVARIANT_HOISTING
                return functor_type () (it2_.begin (), it2_.end (), e1_begin_, e1_end_);
#else
                const matrix_vector_binary2 &mvb = (*this) ();
                return functor_type () (it2_.begin (), it2_.end (), mvb.expression1 ().begin (), mvb.expression1 ().end ());
#endif
            }

            // Sparse bidirectional specialization
            NUMERICS_INLINE
            value_type dereference (sparse_bidirectional_iterator_tag) const {
#ifdef NUMERICS_USE_INVARIANT_HOISTING
                return functor_type () (it2_.begin (), it2_.end (), e1_begin_, e1_end_, sparse_bidirectional_iterator_tag ());
#else
                const matrix_vector_binary2 &mvb = (*this) ();
                return functor_type () (it2_.begin (), it2_.end (), mvb.expression1 ().begin (), mvb.expression1 ().end (), sparse_bidirectional_iterator_tag ());
#endif
            }

            // Arithmetic
            NUMERICS_INLINE
            const_iterator &operator ++ () {
                ++ it2_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator &operator -- () {
                -- it2_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator &operator += (difference_type n) {
                it2_ += n;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator &operator -= (difference_type n) {
                it2_ -= n;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const const_iterator &it) const {
                return it2_ - it.it2_;
            }

            // Dereference
            NUMERICS_INLINE
            value_type operator * () const {
                return dereference (iterator_category ());
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                return it2_.index2 ();
            }

            // Assignment 
            NUMERICS_INLINE
            const_iterator &operator = (const const_iterator &it) {
                container_const_reference<matrix_vector_binary2>::assign (&it ());
                it2_ = it.it2_;
#ifdef NUMERICS_USE_INVARIANT_HOISTING
                e1_begin_ = it.e1_begin_;
                e1_end_ = it.e1_end_;
#endif
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it2_ == it.it2_; 
            }

        private:
            const_iterator2_type it2_;
#ifdef NUMERICS_USE_INVARIANT_HOISTING
            // Mutable due to assignment 
            /* const */ const_iterator1_type e1_begin_;
            /* const */ const_iterator1_type e1_end_;
#endif
        };

        NUMERICS_INLINE
        const_iterator begin () const {
            return const_iterator (*this, e2_.begin2 ()); 
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return const_iterator (*this, e2_.end2 ()); 
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
#ifdef NUMERICS_ET_VALUE
        expression1_type e1_;
        expression2_type e2_;
#endif
#ifdef NUMERICS_ET_REFERENCE
        const expression1_type &e1_;
        const expression2_type &e2_;
#endif
    };

    template<class T1, class E1, class T2, class E2>
    struct matrix_vector_binary2_traits {
        typedef column_major_tag dispatch_category;
        typedef typename promote_traits<T1, T2>::promote_type promote_type;
        typedef matrix_vector_binary2<typename E1::const_closure_type, 
                                      typename E2::const_closure_type, 
                                      matrix_vector_prod2<T1, T2, promote_type> > expression_type;
#ifdef NUMERICS_USE_ET
        typedef expression_type result_type; 
#else
        typedef vector<promote_type> result_type;
#endif
    };

    template<class E1, class E2>
    NUMERICS_INLINE
    typename matrix_vector_binary2_traits<typename E1::value_type, E1, 
                                          typename E2::value_type, E2>::result_type
    prod (const vector_expression<E1> &e1, 
          const matrix_expression<E2> &e2,
          column_major_tag) {
        typedef NUMERICS_TYPENAME matrix_vector_binary2_traits<NUMERICS_TYPENAME E1::value_type, E1, 
                                                               NUMERICS_TYPENAME E2::value_type, E2>::expression_type expression_type;
        return expression_type (e1 (), e2 ());
    }

    // Dispatcher
    template<class E1, class E2>
    NUMERICS_INLINE
    typename matrix_vector_binary2_traits<typename E1::value_type, E1, 
                                          typename E2::value_type, E2>::result_type
    prod (const vector_expression<E1> &e1, 
          const matrix_expression<E2> &e2) {
        typedef NUMERICS_TYPENAME matrix_vector_binary2_traits<NUMERICS_TYPENAME E1::value_type, E1, 
                                                               NUMERICS_TYPENAME E2::value_type, E2>::dispatch_category dispatch_category;
        return prod (e1, e2, dispatch_category ());
    }

    template<class E1, class E2>
    NUMERICS_INLINE
    typename matrix_vector_binary2_traits<typename type_traits<typename E1::value_type>::precision_type, E1, 
                                          typename type_traits<typename E2::value_type>::precision_type, E2>::result_type
    prec_prod (const vector_expression<E1> &e1, 
               const matrix_expression<E2> &e2,
               column_major_tag) {
        typedef NUMERICS_TYPENAME matrix_vector_binary2_traits<NUMERICS_TYPENAME type_traits<NUMERICS_TYPENAME E1::value_type>::precision_type, E1, 
                                                               NUMERICS_TYPENAME type_traits<NUMERICS_TYPENAME E2::value_type>::precision_type, E2>::expression_type expression_type;
        return expression_type (e1 (), e2 ());
    }

    // Dispatcher
    template<class E1, class E2>
    NUMERICS_INLINE
    typename matrix_vector_binary2_traits<typename type_traits<typename E1::value_type>::precision_type, E1, 
                                          typename type_traits<typename E2::value_type>::precision_type, E2>::result_type
    prec_prod (const vector_expression<E1> &e1, 
               const matrix_expression<E2> &e2) {
        typedef NUMERICS_TYPENAME matrix_vector_binary2_traits<NUMERICS_TYPENAME type_traits<NUMERICS_TYPENAME E1::value_type>::precision_type, E1, 
                                                               NUMERICS_TYPENAME type_traits<NUMERICS_TYPENAME E2::value_type>::precision_type, E2>::dispatch_category dispatch_category;
        return prod (e1, e2, dispatch_category ());
    }

    template<class E1, class E2, class F>
    class matrix_matrix_binary:
        public matrix_expression<matrix_matrix_binary<E1, E2, F> > {
    public:
        typedef E1 expression1_type;
        typedef E2 expression2_type;
        typedef F functor_type;
        typedef typename promote_traits<typename E1::size_type, typename E2::size_type>::promote_type size_type;
        typedef typename promote_traits<typename E1::difference_type, typename E2::difference_type>::promote_type difference_type;
        typedef typename F::result_type value_type;
        typedef const value_type &const_reference;
        typedef const_reference reference;
        typedef const value_type *const_pointer;
        typedef const_pointer pointer;
        typedef const matrix_matrix_binary<E1, E2, F> const_closure_type;
        typedef unknown_orientation_tag orientation_category;
        typedef typename E1::const_iterator1 const_iterator11_type;
        typedef typename E1::const_iterator2 const_iterator12_type;
        typedef typename E2::const_iterator1 const_iterator21_type;
        typedef typename E2::const_iterator2 const_iterator22_type;
#ifdef NUMERICS_DEPRECATED
#ifdef NUMERICS_DEPRECATED
        typedef const matrix_expression_row<const_closure_type> const_matrix_row_type;
        typedef const matrix_expression_column<const_closure_type> const_matrix_column_type;
        typedef const matrix_expression_range<const_closure_type> const_matrix_range_type;
#else
        typedef const matrix_row<const_closure_type> const_matrix_row_type;
        typedef const matrix_column<const_closure_type> const_matrix_column_type;
        typedef const matrix_range<const_closure_type> const_matrix_range_type;
#endif
#endif
        typedef unknown_storage_tag storage_category;

        // Construction and destruction
        NUMERICS_INLINE
        matrix_matrix_binary (): 
            e1_ (), e2_ () {}
        NUMERICS_INLINE
        matrix_matrix_binary (const expression1_type &e1, const expression2_type &e2): 
            e1_ (e1), e2_ (e2) {}

        NUMERICS_INLINE
        const expression1_type &expression1 () const {
            return e1_;
        }
        NUMERICS_INLINE
        const expression2_type &expression2 () const {
            return e2_;
        }

        NUMERICS_INLINE
        size_type size1 () const { 
            return e1_.size1 ();
        }
        NUMERICS_INLINE
        size_type size2 () const { 
            return e2_.size2 ();
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i, size_type j) const { 
            return functor_type () (e1_, e2_, i, j); 
        }

#ifdef NUMERICS_DEPRECATED
        NUMERICS_INLINE
        const_matrix_row_type operator [] (size_type i) const {
            return const_matrix_row_type (*this, i); 
        }
        NUMERICS_INLINE
        const_matrix_row_type row (size_type i) const {
            return const_matrix_row_type (*this, i);
        }
        NUMERICS_INLINE
        const_matrix_column_type column (size_type j) const {
            return const_matrix_column_type (*this, j);
        }

        NUMERICS_INLINE
        const_matrix_range_type project (size_type start1, size_type stop1, size_type start2, size_type stop2) const {
            return const_matrix_range_type (*this, start1, stop1, start2, stop2);
        }
        NUMERICS_INLINE
        const_matrix_range_type project (const range &r1, const range &r2) const {
            return const_matrix_range_type (*this, r1, r2);
        }
#endif

        class const_iterator1;
        typedef const_iterator1 iterator1;
        class const_iterator2;
        typedef const_iterator2 iterator2;

        // Element lookup
        NUMERICS_INLINE
        const_iterator1 find1 (int rank, size_type i, size_type j) const {
            return const_iterator1 (*this, e1_.find1 (rank, i, j), e2_.find2 (rank, i, j));
        }
        NUMERICS_INLINE
        const_iterator2 find2 (int rank, size_type i, size_type j) const {
            return const_iterator2 (*this, e1_.find1 (rank, i, j), e2_.find2 (rank, i, j));
        }

        // Iterators simply are pointers.

#ifdef USE_MSVC
        typedef reverse_iterator1<const_iterator1, value_type, value_type> const_reverse_iterator1;
        typedef reverse_iterator2<const_iterator2, value_type, value_type> const_reverse_iterator2;
#else
        typedef reverse_iterator1<const_iterator1> const_reverse_iterator1;
        typedef reverse_iterator2<const_iterator2> const_reverse_iterator2;
#endif

        class const_iterator1:
            public container_const_reference<matrix_matrix_binary>,
            public random_access_iterator_base<const_iterator1, value_type> {
        public:
            typedef typename restrict_traits<typename E1::const_iterator1::iterator_category, 
                                             typename E2::const_iterator2::iterator_category>::iterator_category iterator_category;
#ifndef USE_MSVC
            typedef typename matrix_matrix_binary::difference_type difference_type;
            typedef typename matrix_matrix_binary::value_type value_type;
            typedef typename matrix_matrix_binary::value_type reference;
            typedef typename matrix_matrix_binary::const_pointer pointer;
#endif
            typedef const_iterator2 dual_iterator_type;
            typedef const_reverse_iterator2 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
#ifdef NUMERICS_USE_INVARIANT_HOISTING
            const_iterator1 ():
                container_const_reference<matrix_matrix_binary> (), it1_ (), it2_ (), it2_begin_ (), it2_end_ () {}
            const_iterator1 (const matrix_matrix_binary &mmb, const const_iterator11_type &it1, const const_iterator22_type &it2):
                container_const_reference<matrix_matrix_binary> (mmb), it1_ (it1), it2_ (it2), it2_begin_ (it2.begin ()), it2_end_ (it2.end ()) {}
#else
            const_iterator1 ():
                container_const_reference<matrix_matrix_binary> (mmb), it1_ (), it2_ () {}
            const_iterator1 (const matrix_matrix_binary &mmb, const const_iterator11_type &it1, const const_iterator22_type &it2):
                container_const_reference<matrix_matrix_binary> (mmb), it1_ (it1), it2_ (it2) {}
#endif

            // Random access specialization
            NUMERICS_INLINE
            value_type dereference (std::random_access_iterator_tag) const {
                const matrix_matrix_binary &mmb = (*this) ();
                difference_type size = common (mmb.expression1 ().size2 (), mmb.expression2 ().size1 ());
                if (size >= NUMERICS_ITERATOR_THRESHOLD)
#ifdef NUMERICS_USE_INVARIANT_HOISTING
                    return functor_type () (size, it1_.begin (), it2_begin_); 
#else
                    return functor_type () (size, it1_.begin (), it2_.begin ()); 
#endif
                else 
                    return mvb (index1 (), index2 ()); 
            }

            // Bidirectional specialization
            NUMERICS_INLINE
            value_type dereference (std::bidirectional_iterator_tag) const {
#ifdef NUMERICS_USE_INVARIANT_HOISTING
                return functor_type () (it1_.begin (), it1_.end (), it2_begin_, it2_end_, 0); 
#else
                return functor_type () (it1_.begin (), it1_.end (), it2_.begin (), it2_.end (), 0); 
#endif
            }

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
                return dereference (iterator_category ());
            }

            NUMERICS_INLINE
            const_iterator2 begin () const {
                return const_iterator2 ((*this) (), it1_, (*this) ().expression2 ().begin2 ()); 
            }
            NUMERICS_INLINE
            const_iterator2 end () const {
                return const_iterator2 ((*this) (), it1_, (*this) ().expression2 ().end2 ()); 
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
                return it1_.index1 ();
            }
            NUMERICS_INLINE
            size_type index2 () const {
                return it2_.index2 ();
            }

            // Assignment 
            NUMERICS_INLINE
            const_iterator1 &operator = (const const_iterator1 &it) {
                container_const_reference<matrix_matrix_binary>::assign (&it ());
                it1_ = it.it1_;
                it2_ = it.it2_;
#ifdef NUMERICS_USE_INVARIANT_HOISTING
                it2_begin_ = it.it2_begin_;
                it2_end_ = it.it2_end_;
#endif
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator1 &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it1_ == it.it1_ && it2_ == it.it2_;
            }

        private:
            const_iterator11_type it1_;
            // Mutable due to assignment 
            /* const */ const_iterator22_type it2_;
#ifdef NUMERICS_USE_INVARIANT_HOISTING
            /* const */ const_iterator21_type it2_begin_;
            /* const */ const_iterator21_type it2_end_;
#endif
        };

        NUMERICS_INLINE
        const_iterator1 begin1 () const {
            return const_iterator1 (*this, e1_.begin1 (), e2_.begin2 ());
        }
        NUMERICS_INLINE
        const_iterator1 end1 () const {
            return const_iterator1 (*this, e1_.end1 (), e2_.begin2 ());
        }

        class const_iterator2:
            public container_const_reference<matrix_matrix_binary>,
            public random_access_iterator_base<const_iterator2, value_type> {
        public:
            typedef typename restrict_traits<typename E1::const_iterator1::iterator_category, 
                                             typename E2::const_iterator2::iterator_category>::iterator_category iterator_category;
#ifndef USE_MSVC
            typedef typename matrix_matrix_binary::difference_type difference_type;
            typedef typename matrix_matrix_binary::value_type value_type;
            typedef typename matrix_matrix_binary::value_type reference;
            typedef typename matrix_matrix_binary::const_pointer pointer;
#endif
            typedef const_iterator1 dual_iterator_type;
            typedef const_reverse_iterator1 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
#ifdef NUMERICS_USE_INVARIANT_HOISTING
            const_iterator2 ():
                container_const_reference<matrix_matrix_binary> (), it1_ (), it2_ (), it1_begin_ (), it1_end_ () {}
            const_iterator2 (const matrix_matrix_binary &mmb, const const_iterator11_type &it1, const const_iterator22_type &it2):
                container_const_reference<matrix_matrix_binary> (mmb), it1_ (it1), it2_ (it2), it1_begin_ (it1.begin ()), it1_end_ (it1.end ()) {}
#else
            const_iterator2 ():
                container_const_reference<matrix_matrix_binary> (), it1_ (), it2_ () {}
            const_iterator2 (const matrix_matrix_binary &mmb, const const_iterator11_type &it1, const const_iterator22_type &it2):
                container_const_reference<matrix_matrix_binary> (mmb), it1_ (it1), it2_ (it2) {}
#endif

            // Random access specialization
            NUMERICS_INLINE
            value_type dereference (dense_random_access_iterator_tag) const {
                const matrix_matrix_binary &mmb = (*this) ();
                difference_type size = common (mmb.expression1 ().size2 (), mmb.expression2 ().size1 ());
                if (size >= NUMERICS_ITERATOR_THRESHOLD)
#ifdef NUMERICS_USE_INVARIANT_HOISTING
                    return functor_type () (size, it1_begin_, it2_.begin ()); 
#else
                    return functor_type () (size, it1_.begin (), it2_.begin ()); 
#endif
                else 
                    return mmb (index1 (), index2 ()); 
            }

            // Packed bidirectional specialization
            NUMERICS_INLINE
            value_type dereference (packed_bidirectional_iterator_tag) const {
#ifdef NUMERICS_USE_INVARIANT_HOISTING
                return functor_type () (it1_begin_, it1_end_, it2_.begin (), it2_.end (), packed_bidirectional_iterator_tag ()); 
#else
                return functor_type () (it1_.begin (), it1_.end (), it2_.begin (), it2_.end (), packed_bidirectional_iterator_tag ()); 
#endif
            }

            // Sparse bidirectional specialization
            NUMERICS_INLINE
            value_type dereference (sparse_bidirectional_iterator_tag) const {
#ifdef NUMERICS_USE_INVARIANT_HOISTING
                return functor_type () (it1_begin_, it1_end_, it2_.begin (), it2_.end (), sparse_bidirectional_iterator_tag ()); 
#else
                return functor_type () (it1_.begin (), it1_.end (), it2_.begin (), it2_.end (), sparse_bidirectional_iterator_tag ()); 
#endif
            }

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
                return dereference (iterator_category ());
            }

            NUMERICS_INLINE
            const_iterator1 begin () const {
                return const_iterator1 ((*this) (), (*this) ().expression1 ().begin1 (), it2_); 
            }
            NUMERICS_INLINE
            const_iterator1 end () const {
                return const_iterator1 ((*this) (), (*this) ().expression1 ().end1 (), it2_); 
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
                return it1_.index1 ();
            }
            NUMERICS_INLINE
            size_type index2 () const {
                return it2_.index2 ();
            }

            // Assignment 
            NUMERICS_INLINE
            const_iterator2 &operator = (const const_iterator2 &it) {
                container_const_reference<matrix_matrix_binary>::assign (&it ());
                it1_ = it.it1_;
                it2_ = it.it2_;
#ifdef NUMERICS_USE_INVARIANT_HOISTING
                it1_begin_ = it.it1_begin_;
                it1_end_ = it.it1_end_;
#endif
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator2 &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it1_ == it.it1_ && it2_ == it.it2_;
            }

        private:
            // Mutable due to assignment 
            /* const */ const_iterator11_type it1_;
            const_iterator22_type it2_;
#ifdef NUMERICS_USE_INVARIANT_HOISTING
            /* const */ const_iterator12_type it1_begin_;
            /* const */ const_iterator12_type it1_end_;
#endif
        };

        NUMERICS_INLINE
        const_iterator2 begin2 () const {
            return const_iterator2 (*this, e1_.begin1 (), e2_.begin2 ());
        }
        NUMERICS_INLINE
        const_iterator2 end2 () const {
            return const_iterator2 (*this, e1_.begin1 (), e2_.end2 ());
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
        const_reverse_iterator2 rbegin2 () const {
            return const_reverse_iterator2 (end2 ());
        }
        NUMERICS_INLINE
        const_reverse_iterator2 rend2 () const {
            return const_reverse_iterator2 (begin2 ());
        }

    private:
#ifdef NUMERICS_ET_VALUE
        expression1_type e1_;
        expression2_type e2_;
#endif
#ifdef NUMERICS_ET_REFERENCE
        const expression1_type &e1_;
        const expression2_type &e2_;
#endif
    };

    template<class T1, class E1, class T2, class E2>
    struct matrix_matrix_binary_traits {
        typedef unknown_orientation_tag dispatch_category;
        typedef typename promote_traits<T1, T2>::promote_type promote_type;
        typedef matrix_matrix_binary<typename E1::const_closure_type, 
                                     typename E2::const_closure_type, 
                                     matrix_matrix_prod<T1, T2, promote_type> > expression_type;
#ifdef NUMERICS_USE_ET
        typedef expression_type result_type; 
#else
        typedef matrix<promote_type> result_type;
#endif
    };

    template<class E1, class E2>
    NUMERICS_INLINE
    typename matrix_matrix_binary_traits<typename E1::value_type, E1, 
                                         typename E2::value_type, E2>::result_type
    prod (const matrix_expression<E1> &e1, 
          const matrix_expression<E2> &e2,
          unknown_orientation_tag) {
        typedef NUMERICS_TYPENAME matrix_matrix_binary_traits<NUMERICS_TYPENAME E1::value_type, E1, 
                                                              NUMERICS_TYPENAME E2::value_type, E2>::expression_type expression_type;
        return expression_type (e1 (), e2 ());
    }

    // Dispatcher
    template<class E1, class E2>
    NUMERICS_INLINE
    typename matrix_matrix_binary_traits<typename E1::value_type, E1, 
                                         typename E2::value_type, E2>::result_type
    prod (const matrix_expression<E1> &e1, 
          const matrix_expression<E2> &e2) {
        typedef NUMERICS_TYPENAME matrix_matrix_binary_traits<NUMERICS_TYPENAME E1::value_type, E1, 
                                                              NUMERICS_TYPENAME E2::value_type, E2>::dispatch_category dispatch_category;
        return prod (e1, e2, dispatch_category ());
    }

    template<class E1, class E2>
    NUMERICS_INLINE
    typename matrix_matrix_binary_traits<typename type_traits<typename E1::value_type>::precision_type, E1, 
                                         typename type_traits<typename E2::value_type>::precision_type, E2>::result_type
    prec_prod (const matrix_expression<E1> &e1, 
               const matrix_expression<E2> &e2,
               unknown_orientation_tag) {
        typedef NUMERICS_TYPENAME matrix_matrix_binary_traits<NUMERICS_TYPENAME type_traits<NUMERICS_TYPENAME E1::value_type>::precision_type, E1, 
                                                              NUMERICS_TYPENAME type_traits<NUMERICS_TYPENAME E2::value_type>::precision_type, E2>::expression_type expression_type;
        return expression_type (e1 (), e2 ());
    }

    // Dispatcher
    template<class E1, class E2>
    NUMERICS_INLINE
    typename matrix_matrix_binary_traits<typename type_traits<typename E1::value_type>::precision_type, E1, 
                                         typename type_traits<typename E2::value_type>::precision_type, E2>::result_type
    prec_prod (const matrix_expression<E1> &e1, 
               const matrix_expression<E2> &e2) {
        typedef NUMERICS_TYPENAME matrix_matrix_binary_traits<NUMERICS_TYPENAME type_traits<NUMERICS_TYPENAME E1::value_type>::precision_type, E1, 
                                                              NUMERICS_TYPENAME type_traits<NUMERICS_TYPENAME E2::value_type>::precision_type, E2>::dispatch_category dispatch_category;
        return prec_prod (e1, e2, dispatch_category ());
    }

#ifdef NUMERICS_DEPRECATED

    template<class E>
    class matrix_expression_range:
        public matrix_expression<matrix_expression_range<E> > {
    public:
        typedef E expression_type;
        typedef typename E::size_type size_type;
        typedef typename E::difference_type difference_type;
        typedef typename E::value_type value_type;
        typedef typename E::const_reference const_reference;
        typedef const_reference reference;
        typedef typename E::const_pointer const_pointer;
        typedef const_pointer pointer;
        typedef const matrix_expression_range<E> const_closure_type;
        typedef typename E::orientation_category orientation_category;
        typedef range::const_iterator const_iterator_type;
        typedef const matrix_expression_row<const_closure_type> const_matrix_row_type;
        typedef const matrix_expression_column<const_closure_type> const_matrix_column_type;
        typedef unknown_storage_tag storage_category;

        // Construction and destruction
        NUMERICS_INLINE
        matrix_expression_range (): 
            e_ (), r1_ (), r2_ () {}
        NUMERICS_INLINE
        matrix_expression_range (const expression_type &e, const range &r1, const range &r2): 
            e_ (e), r1_ (r1), r2_ (r2) {}
        NUMERICS_INLINE
        matrix_expression_range (const expression_type &e, size_type start1, size_type stop1, size_type start2, size_type stop2): 
            e_ (e), r1_ (start1, stop1), r2_ (start2, stop2) {}

        NUMERICS_INLINE
        size_type size1 () const { 
            return r1_.size ();
        }
        NUMERICS_INLINE
        size_type size2 () const { 
            return r2_.size ();
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i, size_type j) const { 
            return e_ (r1_ (i), r2_ (j)); 
        }

#ifdef NUMERICS_DEPRECATED
        NUMERICS_INLINE
        const_matrix_row_type operator [] (size_type i) const {
            return const_matrix_row_type (*this, i); 
        }
        NUMERICS_INLINE
        const_matrix_row_type row (size_type i) const {
            return const_matrix_row_type (*this, i);
        }
        NUMERICS_INLINE
        const_matrix_column_type column (size_type j) const {
            return const_matrix_column_type (*this, j);
        }

        NUMERICS_INLINE
        matrix_expression_range project (size_type start1, size_type stop1, size_type start2, size_type stop2) const {
            return matrix_expression_range (e_, r1_.composite (start1, stop1), r2_.composite (start2, stop2));
        }
        NUMERICS_INLINE
        matrix_expression_range project (const range &r1, const range &r2) const {
            return matrix_expression_range (e_, r1_.composite (r1), r2_.composite (r2));
        }
#endif

        class const_iterator1;
        typedef const_iterator1 iterator1;
        class const_iterator2;
        typedef const_iterator2 iterator2;

        // Element lookup
        NUMERICS_INLINE
        const_iterator1 find1 (int rank, size_type i, size_type j) const {
            return const_iterator1 (e_, const_iterator_type (r1_, i), const_iterator_type (r2_, j));
        }
        NUMERICS_INLINE
        const_iterator2 find2 (int rank, size_type i, size_type j) const {
            return const_iterator2 (e_, const_iterator_type (r1_, i), const_iterator_type (r2_, j));
        }

        // Iterators simply are indices.
        // FIXME: is it possible to rearrange this using pointers?

#ifdef USE_MSVC
        typedef reverse_iterator1<const_iterator1, value_type, value_type> const_reverse_iterator1;
        typedef reverse_iterator2<const_iterator2, value_type, value_type> const_reverse_iterator2;
#else
        typedef reverse_iterator1<const_iterator1> const_reverse_iterator1;
        typedef reverse_iterator2<const_iterator2> const_reverse_iterator2;
#endif

        class const_iterator1:
            public container_const_reference<expression_type>,
            public random_access_iterator_base<const_iterator1, value_type> {
        public:
            typedef typename E::const_iterator1::iterator_category iterator_category;
#ifndef USE_MSVC
            typedef typename E::const_iterator1::difference_type difference_type;
            typedef typename E::const_iterator1::value_type value_type;
            typedef typename E::const_iterator1::value_type reference;
            typedef typename E::const_iterator1::pointer pointer;
#endif
            typedef const_iterator2 dual_iterator_type;
            typedef const_reverse_iterator2 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator1 ():
                container_const_reference<expression_type> (), it1_ (), it2_ () {}
            NUMERICS_INLINE
            const_iterator1 (const expression_type &e, const const_iterator_type &it1, const const_iterator_type &it2):
                container_const_reference<expression_type> (e), it1_ (it1), it2_ (it2) {}

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
                container_const_reference<expression_type>::assign (&it ());
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
            const_iterator_type it1_;
            const_iterator_type it2_;
        };

        NUMERICS_INLINE
        const_iterator1 begin1 () const {
            return const_iterator1 (e_, r1_.begin (), r2_.begin ()); 
        }
        NUMERICS_INLINE
        const_iterator1 end1 () const {
            return const_iterator1 (e_, r1_.end (), r2_.begin ()); 
        }

        class const_iterator2:
            public container_const_reference<expression_type>,
            public random_access_iterator_base<const_iterator2, value_type> {
        public:
            typedef typename E::const_iterator2::iterator_category iterator_category;
#ifndef USE_MSVC
            typedef typename E::const_iterator2::difference_type difference_type;
            typedef typename E::const_iterator2::value_type value_type;
            typedef typename E::const_iterator2::value_type reference;
            typedef typename E::const_iterator2::pointer pointer;
#endif
            typedef const_iterator1 dual_iterator_type;
            typedef const_reverse_iterator1 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator2 ():
                container_const_reference<expression_type> (), it1_ (), it2_ () {}
            NUMERICS_INLINE
            const_iterator2 (const expression_type &e, const const_iterator_type &it1, const const_iterator_type &it2):
                container_const_reference<expression_type> (e), it1_ (it1), it2_ (it2) {}

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
                container_const_reference<expression_type>::assign (&it ());
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
            const_iterator_type it1_;
            const_iterator_type it2_;
        };

        NUMERICS_INLINE
        const_iterator2 begin2 () const {
            return const_iterator2 (e_, r1_.begin (), r2_.begin ()); 
        }
        NUMERICS_INLINE
        const_iterator2 end2 () const {
            return const_iterator2 (e_, r1_.begin (), r2_.end ()); 
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
        const_reverse_iterator2 rbegin2 () const {
            return const_reverse_iterator2 (end2 ());
        }
        NUMERICS_INLINE
        const_reverse_iterator2 rend2 () const {
            return const_reverse_iterator2 (begin2 ());
        }

    private:
#ifdef NUMERICS_ET_VALUE
        expression_type e_;
        range r1_;
        range r2_;
#endif
#ifdef NUMERICS_ET_REFERENCE
        const expression_type &e_;
        const range &r1_;
        const range &r2_;
#endif
    };

    template<class E>
    struct matrix_expression_range_traits {
        typedef matrix_expression_range<typename E::const_closure_type> expression_type;
#ifdef NUMERICS_USE_ET
        typedef expression_type result_type; 
#else
        typedef matrix<typename E::value_type> result_type;
#endif
    };

    template<class E>
    NUMERICS_INLINE
    typename matrix_expression_range_traits<E>::result_type
    project (const matrix_expression<E> &e, 
             const range &r1,
             const range &r2) {
        typedef NUMERICS_TYPENAME matrix_expression_range_traits<E>::expression_type expression_type;
        return expression_type(e (), r1, r2);
    }
    template<class E>
    NUMERICS_INLINE
    typename matrix_expression_range_traits<E>::result_type
    project (const matrix_expression<E> &e, 
             std::size_t start1,
             std::size_t stop1,
             std::size_t start2,
             std::size_t stop2) {
        typedef NUMERICS_TYPENAME matrix_expression_range_traits<E>::expression_type expression_type;
        return expression_type(e (), start1, stop1, start2, stop2);
    }

    template<class E>
    class matrix_expression_slice:
        public matrix_expression<matrix_expression_slice<E> > {
    public:
        typedef E expression_type;
        typedef typename E::size_type size_type;
        typedef typename E::difference_type difference_type;
        typedef typename E::value_type value_type;
        typedef typename E::const_reference const_reference;
        typedef const_reference reference;
        typedef typename E::const_pointer const_pointer;
        typedef const_pointer pointer;
        typedef const matrix_expression_slice<E> const_closure_type;
        typedef typename E::orientation_category orientation_category;
        typedef slice::const_iterator const_iterator_type;
        typedef const matrix_expression_row<const_closure_type> const_matrix_row_type;
        typedef const matrix_expression_column<const_closure_type> const_matrix_column_type;
        typedef unknown_storage_tag storage_category;

        // Construction and destruction
        NUMERICS_INLINE
        matrix_expression_slice (): 
            e_ (), s1_ (), s2_ () {}
        NUMERICS_INLINE
        matrix_expression_slice (const expression_type &e, const slice &s1, const slice &s2): 
            e_ (e), s1_ (s1), s2_ (s2) {}
        NUMERICS_INLINE
        matrix_expression_slice (const expression_type &e, size_type start1, size_type stride1, size_type size1, size_type start2, size_type stride2, size_type size2): 
            e_ (e), s1_ (start1, stride1, size1), s2_ (start2, stride2, size2) {}

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
            return e_ (s1_ (i), s2_ (j)); 
        }

#ifdef NUMERICS_DEPRECATED
        NUMERICS_INLINE
        const_matrix_row_type operator [] (size_type i) const {
            return const_matrix_row_type (*this, i); 
        }
        NUMERICS_INLINE
        const_matrix_row_type row (size_type i) const {
            return const_matrix_row_type (*this, i);
        }
        NUMERICS_INLINE
        const_matrix_column_type column (size_type j) const {
            return const_matrix_column_type (*this, j);
        }

        NUMERICS_INLINE
        matrix_expression_slice project (const range &r1, const range &r2) const {
            return matrix_expression_slice (e_, s1_.composite (r1), s2_.composite (r2));
        }
        NUMERICS_INLINE
        matrix_expression_slice project (const slice &s1, const slice &s2) const {
            return matrix_expression_slice (e_, s1_.composite (s1), s2_.composite (s2));
        }
#endif

        class const_iterator1;
        typedef const_iterator1 iterator1;
        class const_iterator2;
        typedef const_iterator2 iterator2;

        // Element lookup
        NUMERICS_INLINE
        const_iterator1 find1 (int rank, size_type i, size_type j) const {
            return const_iterator1 (e_, const_iterator_type (s1_, i), const_iterator_type (s2_, j));
        }
        NUMERICS_INLINE
        const_iterator2 find2 (int rank, size_type i, size_type j) const {
            return const_iterator2 (e_, const_iterator_type (s1_, i), const_iterator_type (s2_, j));
        }

        // Iterators simply are indices.

#ifdef USE_MSVC
        typedef reverse_iterator1<const_iterator1, value_type, value_type> const_reverse_iterator1;
        typedef reverse_iterator2<const_iterator2, value_type, value_type> const_reverse_iterator2;
#else
        typedef reverse_iterator1<const_iterator1> const_reverse_iterator1;
        typedef reverse_iterator2<const_iterator2> const_reverse_iterator2;
#endif

        class const_iterator1:
            public container_const_reference<expression_type>,
            public random_access_iterator_base<const_iterator1, value_type> {
        public:
            typedef typename E::const_iterator1::iterator_category iterator_category;
#ifndef USE_MSVC
            typedef typename E::const_iterator1::difference_type difference_type;
            typedef typename E::const_iterator1::value_type value_type;
            typedef typename E::const_iterator1::value_type reference;
            typedef typename E::const_iterator1::pointer pointer;
#endif
            typedef const_iterator2 dual_iterator_type;
            typedef const_reverse_iterator2 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator1 ():
                container_const_reference<expression_type> (), it1_ (), it2_ () {}
            NUMERICS_INLINE
            const_iterator1 (const expression_type &e, const const_iterator_type &it1, const const_iterator_type &it2):
                container_const_reference<expression_type> (e), it1_ (it1), it2_ (it2) {}

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
                container_const_reference<expression_type>::assign (&it ());
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
            const_iterator_type it1_;
            const_iterator_type it2_;
        };

        NUMERICS_INLINE
        const_iterator1 begin1 () const {
            return const_iterator1 (e_, s1_.begin (), s2_.begin ()); 
        }
        NUMERICS_INLINE
        const_iterator1 end1 () const {
            return const_iterator1 (e_, s1_.end (), s2_.begin ()); 
        }

        class const_iterator2:
            public container_const_reference<expression_type>,
            public random_access_iterator_base<const_iterator2, value_type> {
        public:
            typedef typename E::const_iterator2::iterator_category iterator_category;
#ifndef USE_MSVC
            typedef typename E::const_iterator2::difference_type difference_type;
            typedef typename E::const_iterator2::value_type value_type;
            typedef typename E::const_iterator2::value_type reference;
            typedef typename E::const_iterator2::pointer pointer;
#endif
            typedef const_iterator1 dual_iterator_type;
            typedef const_reverse_iterator1 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator2 ():
                container_const_reference<expression_type> (), it1_ (), it2_ () {}
            NUMERICS_INLINE
            const_iterator2 (const expression_type &e, const const_iterator_type &it1, const const_iterator_type &it2):
                container_const_reference<expression_type> (e), it1_ (it1), it2_ (it2) {}

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
                container_const_reference<expression_type>::assign (&it ());
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
            const_iterator_type it1_;
            const_iterator_type it2_;
        };

        NUMERICS_INLINE
        const_iterator2 begin2 () const {
            return const_iterator2 (e_, s1_.begin (), s2_.begin ()); 
        }
        NUMERICS_INLINE
        const_iterator2 end2 () const {
            return const_iterator2 (e_, s1_.begin (), s2_.end ()); 
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
        const_reverse_iterator2 rbegin2 () const {
            return const_reverse_iterator2 (end2 ());
        }
        NUMERICS_INLINE
        const_reverse_iterator2 rend2 () const {
            return const_reverse_iterator2 (begin2 ());
        }

    private:
#ifdef NUMERICS_ET_VALUE
        expression_type e_;
        slice s1_;
        slice s2_;
#endif
#ifdef NUMERICS_ET_REFERENCE
        const expression_type &e_;
        const slice &s1_;
        const slice &s2_;
#endif
    };

    template<class E>
    struct matrix_expression_slice_traits {
        typedef matrix_expression_slice<typename E::const_closure_type> expression_type;
#ifdef NUMERICS_USE_ET
        typedef expression_type result_type; 
#else
        typedef matrix<typename E::value_type> result_type;
#endif
    };

    template<class E>
    NUMERICS_INLINE
    typename matrix_expression_slice_traits<E>::result_type
    project (const matrix_expression<E> &e, 
             const slice &s1,
             const slice &s2) {
        typedef NUMERICS_TYPENAME matrix_expression_slice_traits<E>::expression_type expression_type;
        return expression_type(e (), s1, s2);
    }
    template<class E>
    NUMERICS_INLINE
    typename matrix_expression_slice_traits<E>::result_type
    project (const matrix_expression<E> &e, 
             std::size_t start1,
             std::size_t stride1,
             std::size_t size1,
             std::size_t start2,
             std::size_t stride2,
             std::size_t size2) {
        typedef NUMERICS_TYPENAME matrix_expression_slice_traits<E>::expression_type expression_type;
        return expression_type(e (), start1, stride1, size1, start2, stride2, size2);
    }

#endif

    template<class E, class F>
    class matrix_scalar_unary:
        public scalar_expression<typename F::result_type> {
    public:
        typedef E expression_type;
        typedef F functor_type;
        typedef typename F::result_type value_type;

        // Construction and destruction
        NUMERICS_INLINE
        matrix_scalar_unary (): 
            e_ () {}
        NUMERICS_INLINE
        matrix_scalar_unary (const expression_type &e): 
            e_ (e) {}

        NUMERICS_INLINE
        operator value_type () const { 
            return functor_type () (e_); 
        }

    private:
#ifdef NUMERICS_ET_VALUE
        expression_type e_;
#endif
#ifdef NUMERICS_ET_REFERENCE
        const expression_type &e_;
#endif
    };
    
    template<class E, class F>
    struct matrix_scalar_unary_traits {
        typedef matrix_scalar_unary<typename E::const_closure_type, F> expression_type;
#ifdef NUMERICS_USE_ET
         typedef expression_type result_type; 
#else
         typedef typename F::result_type result_type;
#endif
    };

    template<class E>
    NUMERICS_INLINE
    typename matrix_scalar_unary_traits<E, matrix_norm_1<typename E::value_type> >::result_type
    norm_1 (const matrix_expression<E> &e) {
        typedef NUMERICS_TYPENAME matrix_scalar_unary_traits<E, matrix_norm_1<NUMERICS_TYPENAME E::value_type> >::expression_type expression_type;
        return expression_type (e ());
    }

    template<class E>
    NUMERICS_INLINE
    typename matrix_scalar_unary_traits<E, matrix_norm_2<typename E::value_type> >::result_type
    norm_2 (const matrix_expression<E> &e) {
        typedef NUMERICS_TYPENAME matrix_scalar_unary_traits<E, matrix_norm_2<NUMERICS_TYPENAME E::value_type> >::expression_type expression_type;
        return expression_type (e ());
    }

    template<class E>
    NUMERICS_INLINE
    typename matrix_scalar_unary_traits<E, matrix_norm_inf<typename E::value_type> >::result_type
    norm_inf (const matrix_expression<E> &e) {
        typedef NUMERICS_TYPENAME matrix_scalar_unary_traits<E, matrix_norm_inf<NUMERICS_TYPENAME E::value_type> >::expression_type expression_type;
        return expression_type (e ());
    }

}

#endif 



