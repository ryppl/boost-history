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

    template<class T, class E>
    struct matrix_expression {
        typedef unsigned size_type;
        typedef T value_type;
        typedef T &reference_type;
        typedef E expression_type;

        NUMERICS_INLINE
        virtual ~matrix_expression () {}

        NUMERICS_INLINE
        void resize (size_type size1, size_type size2) {
            expression_type *e = static_cast<expression_type *> (this);
            return e->resize (size1, size2); 
        }

        NUMERICS_INLINE
        size_type size1 () const { 
            const expression_type *e = static_cast<const expression_type *> (this);
            return e->size1 (); 
        }
        NUMERICS_INLINE
        size_type size2 () const { 
            const expression_type *e = static_cast<const expression_type *> (this);
            return e->size2 (); 
        }

        NUMERICS_INLINE
        value_type operator () (size_type i, size_type j) const { 
            const expression_type *e = static_cast<const expression_type *> (this);
            return (*e) (i, j); 
        }
        NUMERICS_INLINE
        reference_type operator () (size_type i, size_type j) { 
            expression_type *e = static_cast<expression_type *> (this);
            return (*e) (i, j); 
        }

#ifdef NUMERICS_USE_ITERATOR
        const expression_type &expression () const {
            const expression_type *e = static_cast<const expression_type *> (this);
            return *e; 
        }
        expression_type &expression () {
            expression_type *e = static_cast<expression_type *> (this);
            return *e; 
        }
#endif // NUMERICS_USE_ITERATOR
    };

    template<class E>
    class matrix_const_reference:
        public matrix_expression<typename E::value_type, matrix_const_reference<E> > {
    public:
        typedef E expression_type;
        typedef typename E::size_type size_type;
        typedef typename E::value_type value_type;
#ifdef NUMERICS_USE_ITERATOR
        typedef typename E::expression_type::const_iterator1 const_iterator1_type;
        typedef typename E::expression_type::const_iterator2 const_iterator2_type;
#endif

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

        NUMERICS_INLINE
        value_type operator () (size_type i, size_type j) const { 
            return e_ (i, j); 
        }

#ifdef NUMERICS_USE_ITERATOR
        class const_iterator2;

        class const_iterator1 {
        public:
            NUMERICS_INLINE
            const_iterator1 (const const_iterator1_type &it):
                it_ (it) {}

            NUMERICS_INLINE
            const_iterator1 &operator ++ () {
                ++ it_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator -- () {
                -- it;
                return *this;
            }
            NUMERICS_INLINE
            value_type operator * () const {
                return *it_;
            }

            NUMERICS_INLINE
            const_iterator2 begin () const {
                return const_iterator2 (it_.begin ()); 
            }
            NUMERICS_INLINE
            const_iterator2 end () const {
                return const_iterator2 (it_.end ()); 
            }

            NUMERICS_INLINE
            friend bool operator == (const const_iterator1 &it1, const const_iterator1 &it2) {
                return it1.it_ == it2.it_;
            }
            NUMERICS_INLINE
            friend bool operator != (const const_iterator1 &it1, const const_iterator1 &it2) {
                return ! (it1 == it2);
            }

        protected:
            const_iterator1_type it_;
        };

        NUMERICS_INLINE
        const_iterator1 begin1 () const {
            return const_iterator1 (e_.expression ().begin1 ());
        }
        NUMERICS_INLINE
        const_iterator1 end1 () const {
            return const_iterator1 (e_.expression ().end1 ());
        }

        class const_iterator2 {
        public:
            NUMERICS_INLINE
            const_iterator2 (const const_iterator2_type &it):
                it_ (it) {}

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
            value_type operator * () const {
               return *it_;
            }

            NUMERICS_INLINE
            const_iterator1 begin () const {
                return const_iterator1 (it_.begin ()); 
            }
            NUMERICS_INLINE
            const_iterator1 end () const {
                return const_iterator1 (it.end ()); 
            }

            NUMERICS_INLINE
            friend bool operator == (const const_iterator2 &it1, const const_iterator2 &it2) {
                return it1.it_ == it2.it_;
            }
            NUMERICS_INLINE
            friend bool operator != (const const_iterator2 &it1, const const_iterator2 &it2) {
                return ! (it1 == it2);
            }

        protected:
            const_iterator2_type it_;
        };

        NUMERICS_INLINE
        const_iterator2 begin2 () const {
            return const_iterator2 (e_.expression ().begin2 ());
        }
        NUMERICS_INLINE
        const_iterator2 end2 () const {
            return const_iterator2 (e_.expression ().end2 ());
        }
#endif // NUMERICS_USE_ITERATOR

    private:
        const expression_type &e_;
    };

    template<class E>
    class matrix_reference:
        public matrix_expression<typename E::value_type, matrix_reference<E> > {
    public:
        typedef E expression_type;
        typedef typename E::size_type size_type;
        typedef typename E::value_type value_type;
        typedef typename E::reference_type reference_type;
#ifdef NUMERICS_USE_ITERATOR
        typedef typename E::expression_type::const_iterator1 const_iterator1_type;
        typedef typename E::expression_type::iterator1 iterator1_type;
        typedef typename E::expression_type::const_iterator2 const_iterator2_type;
        typedef typename E::expression_type::iterator2 iterator2_type;
#endif

        NUMERICS_INLINE
        matrix_reference (expression_type &e):
              e_ (e) {}

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

        NUMERICS_INLINE
        value_type operator () (size_type i, size_type j) const { 
            return e_ (i, j); 
        }
        NUMERICS_INLINE
        reference_type operator () (size_type i, size_type j) { 
            return e_ (i, j); 
        }

#ifdef NUMERICS_USE_ITERATOR
        class const_iterator2;

        class const_iterator1 {
        public:
            NUMERICS_INLINE
            const_iterator1 (const const_iterator1_type &it):
                it_ (it) {}

            NUMERICS_INLINE
            const_iterator1 &operator ++ () {
                ++ it_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator -- () {
                -- it;
                return *this;
            }
            NUMERICS_INLINE
            value_type operator * () const {
                return *it_;
            }

            NUMERICS_INLINE
            const_iterator2 begin () const {
                return const_iterator2 (it_.begin ()); 
            }
            NUMERICS_INLINE
            const_iterator2 end () const {
                return const_iterator2 (it_.end ()); 
            }

            NUMERICS_INLINE
            friend bool operator == (const const_iterator1 &it1, const const_iterator1 &it2) {
                return it1.it_ == it2.it_;
            }
            NUMERICS_INLINE
            friend bool operator != (const const_iterator1 &it1, const const_iterator1 &it2) {
                return ! (it1 == it2);
            }

        protected:
            const_iterator1_type it_;
        };

        NUMERICS_INLINE
        const_iterator1 begin1 () const {
            return const_iterator1 (e_.expression ().begin1 ());
        }
        NUMERICS_INLINE
        const_iterator1 end1 () const {
            return const_iterator1 (e_.expression ().end1 ());
        }

        class iterator2;

        class iterator1 {
        public:
            NUMERICS_INLINE
            iterator1 (const iterator1_type &it):
                const_iterator1 (it) {}

            NUMERICS_INLINE
            reference_type operator * () {
                return *it_;
            }

            NUMERICS_INLINE
            iterator2 begin () {
                return iterator2 (it_.begin ()); 
            }
            NUMERICS_INLINE
            iterator2 end () {
                return iterator2 (it_.end ()); 
            }
        };

        NUMERICS_INLINE
        iterator1 begin1 () {
            return iterator1 (e_.expression ().begin1 ());
        }
        NUMERICS_INLINE
        iterator1 end1 () {
            return iterator1 (e_.expression ().end1 ());
        }

        class const_iterator2 {
        public:
            NUMERICS_INLINE
            const_iterator2 (const const_iterator2_type &it):
                it_ (it) {}

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
            value_type operator * () const {
               return *it_;
            }

            NUMERICS_INLINE
            const_iterator1 begin () const {
                return const_iterator1 (it_.begin ()); 
            }
            NUMERICS_INLINE
            const_iterator1 end () const {
                return const_iterator1 (it.end ()); 
            }

            NUMERICS_INLINE
            friend bool operator == (const const_iterator2 &it1, const const_iterator2 &it2) {
                return it1.it_ == it2.it_;
            }
            NUMERICS_INLINE
            friend bool operator != (const const_iterator2 &it1, const const_iterator2 &it2) {
                return ! (it1 == it2);
            }

        protected:
            const_iterator2_type it_;
        };

        NUMERICS_INLINE
        const_iterator2 begin2 () const {
            return const_iterator2 (e_.expression ().begin2 ());
        }
        NUMERICS_INLINE
        const_iterator2 end2 () const {
            return const_iterator2 (e_.expression ().end2 ());
        }

        class iterator2 {
        public:
            NUMERICS_INLINE
            iterator2 (const iterator2_type &it):
                const_iterator2 (it) {}

            NUMERICS_INLINE
            reference_type operator * () {
               return *it_;
            }

            NUMERICS_INLINE
            iterator1 begin () {
                return iterator1 (it_.begin ()); 
            }
            NUMERICS_INLINE
            iterator1 end () {
                return iterator1 (it.end ()); 
            }
        };

        NUMERICS_INLINE
        iterator2 begin2 () {
            return iterator2 (e_.expression ().begin2 ());
        }
        NUMERICS_INLINE
        iterator2 end2 () {
            return iterator2 (e_.expression ().end2 ());
        }
#endif // NUMERICS_USE_ITERATOR

    private:
        expression_type &e_;
    };

    template<class E1, class E2, class F>
    class vector_matrix_binary:
        public matrix_expression<typename F::value_type, vector_matrix_binary<E1, E2, F> > {
    public:
        typedef E1 expression1_type;
        typedef E2 expression2_type;
        typedef F functor_type;
        typedef typename F::value_type value_type;
#ifdef NUMERICS_USE_ITERATOR
        typedef typename E1::const_iterator const_iterator1_type;
        typedef typename E2::const_iterator const_iterator2_type;
#endif // NUMERICS_USE_ITERATOR

        NUMERICS_INLINE
        vector_matrix_binary (const expression1_type &e1, const expression2_type &e2): 
            e1_ (e1), e2_ (e2) {}

        NUMERICS_INLINE
        size_type size1 () const { 
            return e1_.size (); 
        }
        NUMERICS_INLINE
        size_type size2 () const { 
            return e2_.size (); 
        }

        NUMERICS_INLINE
        value_type operator () (size_type i, size_type j) const { 
            return functor_type () (e1_, e2_, i, j); 
        }

#ifdef NUMERICS_USE_ITERATOR
        class const_iterator2;

        class const_iterator1 {
        public:
            NUMERICS_INLINE
            const_iterator1 (const vector_matrix_binary &vmb, const const_iterator1_type &it1, const const_iterator2_type &it2):
                vmb_ (vmb), it1_ (it1), it2_ (it2) {}

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
            value_type operator * () const {
                return functor_type () (it1_, it2_); 
            }

            NUMERICS_INLINE
            const_iterator2 begin () const {
                return const_iterator2 (vmb_, it1_, it2_); 
            }
            NUMERICS_INLINE
            const_iterator2 end () const {
                return const_iterator2 (vmb_, it1_, it2_ + vmb_.size2 ()); 
            }

            NUMERICS_INLINE
            friend bool operator == (const const_iterator1 &it1, const const_iterator1 &it2) {
                return it1.it1_ == it2.it1_ && it1.it2_ == it2.it2_;
            }
            NUMERICS_INLINE
            friend bool operator != (const const_iterator1 &it1, const const_iterator1 &it2) {
                return ! (it1 == it2);
            }

        protected:
            const vector_matrix_binary &vmb_;
            const_iterator1_type it1_;
            const_iterator2_type it2_;
        };

        NUMERICS_INLINE
        const_iterator1 begin1 () const {
            return const_iterator1 (*this, e1_.expression ().begin (), e2_.expression ().begin ());
        }
        NUMERICS_INLINE
        const_iterator1 end1 () const {
            return const_iterator1 (*this, e1_.expression ().end (), e2_.expression ().begin ());
        }

        class const_iterator2 {
        public:
            NUMERICS_INLINE
            const_iterator2 (const vector_matrix_binary &vmb, const const_iterator1_type &it1, const const_iterator2_type &it2):
                vmb_ (vmb), it1_ (it1), it2_ (it2) {}

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
            value_type operator * () const {
                return functor_type () (it1_, it2_); 
            }

            NUMERICS_INLINE
            const_iterator1 begin () const {
                return const_iterator1 (vmb_, it1_, it2_); 
            }
            NUMERICS_INLINE
            const_iterator1 end () const {
                return const_iterator1 (vmb_, it1_ + vbm_.size1 (), it2_); 
            }

            NUMERICS_INLINE
            friend bool operator == (const const_iterator2 &it1, const const_iterator2 &it2) {
                return it1.it1_ == it2.it1_ && it1.it2_ == it2.it2_;
            }
            NUMERICS_INLINE
            friend bool operator != (const const_iterator2 &it1, const const_iterator2 &it2) {
                return ! (it1 == it2);
            }

        protected:
            const vector_matrix_binary &vmb_;
            const_iterator1_type it1_;
            const_iterator2_type it2_;
        };

        NUMERICS_INLINE
        const_iterator2 begin2 () const {
            return const_iterator2 (*this, e1_.expression ().begin (), e2_.expression ().begin ());
        }
        NUMERICS_INLINE
        const_iterator2 end2 () const {
            return const_iterator2 (*this, e1_.expression ().begin (), e2_.expression ().end ());
        }
#endif // NUMERICS_USE_ITERATOR

    private:
        expression1_type e1_;
        expression2_type e2_;
    };

#ifdef NUMERICS_USE_ET

    template<class T1, class E1, class T2, class E2>
    NUMERICS_INLINE
    vector_matrix_binary<vector_const_reference<vector_expression<T1, E1> >, 
                         vector_const_reference<vector_expression<T2, E2> >, 
                         vector_outer_prod<T1, T2> > 
    outer_prod (const vector_expression<T1, E1> &e1, 
                const vector_expression<T2, E2> &e2) {
        return vector_matrix_binary<vector_const_reference<vector_expression<T1, E1> >, 
                                    vector_const_reference<vector_expression<T2, E2> >, 
                                    vector_outer_prod<T1, T2> > (e1, e2);
    }

#else // NUMERICS_USE_ET

    template<class T1, class A1, class F1, class T2, class A2, class F2>
    NUMERICS_INLINE
    matrix<promote_traits<T1, T2>::promote_type>
    outer_prod (const vector<T1, A1, F1> &e1, 
                const vector<T2, A2, F2> &e2) {
        matrix<promote_traits<T1, T2>::promote_type> r (e1.size (), e2.size ());
        vector_outer_prod<T1, T2> f;
        for (size_type i = 0; i < r.size1 (); ++ i) 
            for (size_type j = 0; j < r.size2 (); ++ j) 
                r (i, j) = f (e1, e2, i, j);
        return r;
    }

#endif // NUMERICS_USE_ET

    template<class E, class F>
    class matrix_unary1:
        public matrix_expression<typename F::value_type, matrix_unary1<E, F> > {
    public:
        typedef E expression_type;
        typedef F functor_type;
        typedef typename E::size_type size_type;
        typedef typename F::value_type value_type;
#ifdef NUMERICS_USE_ITERATOR
        typedef typename E::const_iterator1 const_iterator1_type;
        typedef typename E::const_iterator2 const_iterator2_type;
#endif // NUMERICS_USE_ITERATOR

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

        NUMERICS_INLINE
        value_type operator () (size_type i, size_type j) const { 
            return functor_type () (e_ (i, j)); 
        }

#ifdef NUMERICS_USE_ITERATOR
        class const_iterator2;

        class const_iterator1 {
        public:
            NUMERICS_INLINE
            const_iterator1 (const const_iterator1_type &it):
                it_ (it) {}

            NUMERICS_INLINE
            const_iterator1 &operator ++ () {
                ++ it_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator -- () {
                -- it;
                return *this;
            }
            NUMERICS_INLINE
            value_type operator * () const {
                return functor_type () (*it_);
            }

            NUMERICS_INLINE
            const_iterator2 begin () const {
                return const_iterator2 (it_.begin ()); 
            }
            NUMERICS_INLINE
            const_iterator2 end () const {
                return const_iterator2 (it_.end ()); 
            }

            NUMERICS_INLINE
            friend bool operator == (const const_iterator1 &it1, const const_iterator1 &it2) {
                return it1.it_ == it2.it_;
            }
            NUMERICS_INLINE
            friend bool operator != (const const_iterator1 &it1, const const_iterator1 &it2) {
                return ! (it1 == it2);
            }

        protected:
            const_iterator1_type it_;
        };

        NUMERICS_INLINE
        const_iterator1 begin1 () const {
            return const_iterator1 (e_.expression ().begin1 ());
        }
        NUMERICS_INLINE
        const_iterator1 end1 () const {
            return const_iterator1 (e_.expression ().end1 ());
        }

        class const_iterator2 {
        public:
            NUMERICS_INLINE
            const_iterator2 (const const_iterator2_type &it):
                it_ (it) {}

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
            value_type operator * () const {
                return functor_type () (*it_);
            }

            NUMERICS_INLINE
            const_iterator1 begin () const {
                return const_iterator1 (it_.begin ()); 
            }
            NUMERICS_INLINE
            const_iterator1 end () const {
                return const_iterator1 (it.end ()); 
            }

            NUMERICS_INLINE
            friend bool operator == (const const_iterator2 &it1, const const_iterator2 &it2) {
                return it1.it_ == it2.it_;
            }
            NUMERICS_INLINE
            friend bool operator != (const const_iterator2 &it1, const const_iterator2 &it2) {
                return ! (it1 == it2);
            }

        protected:
            const_iterator2_type it_;
        };

        NUMERICS_INLINE
        const_iterator2 begin2 () const {
            return const_iterator2 (e_.expression ().begin2 ());
        }
        NUMERICS_INLINE
        const_iterator2 end2 () const {
            return const_iterator2 (e_.expression ().end2 ());
        }
#endif // NUMERICS_USE_ITERATOR

    private:
        expression_type e_;
    };

#ifdef NUMERICS_USE_ET

    template<class T, class E> 
    NUMERICS_INLINE
    matrix_unary1<matrix_const_reference<matrix_expression<T, E> >, 
                  scalar_negate<T> > 
    operator - (const matrix_expression<T, E> &e) {
        return matrix_unary1<matrix_const_reference<matrix_expression<T, E> >, 
                             scalar_negate<T> > (e);
    }

    template<class T, class E> 
    NUMERICS_INLINE
    matrix_unary1<matrix_const_reference<matrix_expression<T, E> >, 
                  scalar_conj<T> > 
    conj (const matrix_expression<T, E> &e) {
        return matrix_unary1<matrix_const_reference<matrix_expression<T, E> >, 
                             scalar_conj<T> > (e);
    }

#else // NUMERICS_USE_ET

    template<class T, class A, class F> 
    NUMERICS_INLINE
    matrix<T>
    operator - (const matrix<T, A, F> &e) {
        matrix<T> r (e.size1 (), e.size2 ());
        scalar_negate<T> f;
#ifndef NUMERICS_USE_ITERATOR
        for (size_type i = 0; i < r.size1 (); ++ i) 
            for (size_type j = 0; j < r.size2 (); ++ j) 
                r (i, j) = f (e (i, j));
#else // NUMERICS_USE_ITERATOR
        matrix<T>::iterator1 it1r (r.begin1 ());
        matrix<T, A, F>::const_iterator1 it1e (e.begin1 ());
        for (size_type i = 0; i < r.size1 (); ++ i, ++ it1r, ++ it1e) {
            matrix<T>::iterator2 it2r (it1r.begin ());
            matrix<T, A, F>::const_iterator2 it2e (it1e.begin ());
            for (size_type j = 0; j < r.size2 (); ++ j, ++ it2r, ++ it2e) 
                *it2r  = f (*it2e);
        }
#endif // NUMERICS_USE_ITERATOR
        return r;
    }

    template<class T, class A, class F> 
    NUMERICS_INLINE
    matrix<T>
    conj (const matrix<T, A, F> &e) {
        matrix<T> r (e.size1 (), e.size2 ());
        scalar_conj<T> f;
#ifndef NUMERICS_USE_ITERATOR
        for (size_type i = 0; i < r.size1 (); ++ i) 
            for (size_type j = 0; j < r.size2 (); ++ j) 
                r (i, j) = f (e (i, j));
#else // NUMERICS_USE_ITERATOR
        matrix<T>::iterator1 it1r (r.begin1 ());
        matrix<T, A, F>::const_iterator1 it1e (e.begin1 ());
        for (size_type i = 0; i < r.size1 (); ++ i, ++ it1r, ++ it1e) {
            matrix<T>::iterator2 it2r (it1r.begin ());
            matrix<T, A, F>::const_iterator2 it2e (it1e.begin ());
            for (size_type j = 0; j < r.size2 (); ++ j, ++ it2r, ++ it2e) 
                *it2r  = f (*it2e);
        }
#endif // NUMERICS_USE_ITERATOR
        return r;
    }

#endif // NUMERICS_USE_ET

    template<class E, class F>
    class matrix_unary2:
        public matrix_expression<typename F::value_type, matrix_unary2<E, F> > {
    public:
        typedef E expression_type;
        typedef F functor_type;
        typedef typename E::size_type size_type;
        typedef typename F::value_type value_type;
#ifdef NUMERICS_USE_ITERATOR
        typedef typename E::const_iterator1 const_iterator2_type;
        typedef typename E::const_iterator2 const_iterator1_type;
#endif // NUMERICS_USE_ITERATOR

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

        NUMERICS_INLINE
        value_type operator () (size_type i, size_type j) const { 
            return functor_type () (e_, i, j); 
        }

#ifdef NUMERICS_USE_ITERATOR
        class const_iterator2;

        class const_iterator1 {
        public:
            NUMERICS_INLINE
            const_iterator1 (const const_iterator1_type &it):
                it_ (it) {}

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
            value_type operator * () const {
                return functor_type () (it_); 
            }

            NUMERICS_INLINE
            const_iterator2 begin () const {
                return const_iterator2 (it_.begin ()); 
            }
            NUMERICS_INLINE
            const_iterator2 end () const {
                return const_iterator2 (it_.end ()); 
            }

            NUMERICS_INLINE
            friend bool operator == (const const_iterator1 &it1, const const_iterator1 &it2) {
                return it1.it_ == it2.it_;
            }
            NUMERICS_INLINE
            friend bool operator != (const const_iterator1 &it1, const const_iterator1 &it2) {
                return ! (it1 == it2);
            }

        protected:
            const_iterator1_type it_;
        };

        NUMERICS_INLINE
        const_iterator1 begin1 () const {
            return const_iterator1 (e_.expression ().begin2 ());
        }
        NUMERICS_INLINE
        const_iterator1 end1 () const {
            return const_iterator1 (e_.expression ().end2 ());
        }

        class const_iterator2 {
        public:
            NUMERICS_INLINE
            const_iterator2 (const const_iterator2_type &it):
                it_ (it) {}

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
            value_type operator * () const {
                return functor_type () (it_); 
            }

            NUMERICS_INLINE
            const_iterator1 begin () const {
                return const_iterator1 (it_.begin ()); 
            }
            NUMERICS_INLINE
            const_iterator1 end () const {
                return const_iterator1 (it_.end ()); 
            }

            NUMERICS_INLINE
            friend bool operator == (const const_iterator2 &it1, const const_iterator2 &it2) {
                return it1.it_ == it2.it_;
            }
            NUMERICS_INLINE
            friend bool operator != (const const_iterator2 &it1, const const_iterator2 &it2) {
                return ! (it1 == it2);
            }

        protected:
            const_iterator2_type it_;
        };

        NUMERICS_INLINE
        const_iterator2 begin2 () const {
            return const_iterator2 (e_.expression ().begin1 ());
        }
        NUMERICS_INLINE
        const_iterator2 end2 () const {
            return const_iterator2 (e_.expression ().end1 ());
        }
#endif // NUMERICS_USE_ITERATOR

    private:
        expression_type e_;
    };

#ifdef NUMERICS_USE_ET

    template<class T, class E> 
    NUMERICS_INLINE
    matrix_unary2<matrix_const_reference<matrix_expression<T, E> >, 
                  matrix_trans<T> > 
    trans (const matrix_expression<T, E> &e) {
        return matrix_unary2<matrix_const_reference<matrix_expression<T, E> >, 
                             matrix_trans<T> > (e);
    }
    template<class T, class E> 
    NUMERICS_INLINE
    matrix_unary2<matrix_const_reference<matrix_expression<T, E> >, 
                  matrix_herm<T> > 
    herm (const matrix_expression<T, E> &e) {
        return matrix_unary2<matrix_const_reference<matrix_expression<T, E> >, 
                             matrix_herm<T> > (e);
    }

#else // NUMERICS_USE_ET

    template<class T, class A, class F> 
    NUMERICS_INLINE
    matrix<T>
    trans (const matrix<T, A, F> &e) {
        matrix<T> r (e.size2 (), e.size1 ());
        matrix_trans<T> f;
#ifndef NUMERICS_USE_ITERATOR
        for (size_type i = 0; i < r.size1 (); ++ i) 
            for (size_type j = 0; j < r.size2 (); ++ j) 
                r (i, j) = f (e, i, j);
#else // NUMERICS_USE_ITERATOR
        matrix<T>::iterator1 it1r (r.begin1 ());
        for (size_type i = 0; i < r.size1 (); ++ i, ++ it1r) {
            matrix<T>::iterator2 it2r (it1r.begin ());
            for (size_type j = 0; j < r.size2 (); ++ j, ++ it2r) 
                *it2r  = f (e, i, j);
        }
#endif // NUMERICS_USE_ITERATOR
        return r;
    }
    template<class T, class A, class F> 
    NUMERICS_INLINE
    matrix<T>
    herm (const matrix<T, A, F> &e) {
        matrix<T> r (e.size2 (), e.size1 ());
        matrix_herm<T> f;
#ifndef NUMERICS_USE_ITERATOR
        for (size_type i = 0; i < r.size1 (); ++ i) 
            for (size_type j = 0; j < r.size2 (); ++ j) 
                r (i, j) = f (e, i, j);
#else // NUMERICS_USE_ITERATOR
        matrix<T>::iterator1 it1r (r.begin1 ());
        for (size_type i = 0; i < r.size1 (); ++ i, ++ it1r) {
            matrix<T>::iterator2 it2r (it1r.begin ());
            for (size_type j = 0; j < r.size2 (); ++ j, ++ it2r) 
                *it2r  = f (e, i, j);
        }
#endif // NUMERICS_USE_ITERATOR
        return r;
    }

#endif // NUMERICS_USE_ET

    template<class E1, class E2, class F>
    class matrix_binary:
        public matrix_expression<typename F::value_type, matrix_binary<E1, E2, F> > {
    public:
        typedef E1 expression1_type;
        typedef E2 expression2_type;
        typedef F functor_type;
        typedef promote_traits<typename E1::size_type, typename E2::size_type>::promote_type size_type;
        typedef typename F::value_type value_type;
#ifdef NUMERICS_USE_ITERATOR
        typedef typename E1::const_iterator1 const_iterator11_type;
        typedef typename E1::const_iterator2 const_iterator12_type;
        typedef typename E2::const_iterator1 const_iterator21_type;
        typedef typename E2::const_iterator2 const_iterator22_type;
#endif // NUMERICS_USE_ITERATOR

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

        NUMERICS_INLINE
        value_type operator () (size_type i, size_type j) const { 
            return functor_type () (e1_ (i, j), e2_ (i, j)); 
        }

#ifdef NUMERICS_USE_ITERATOR
        class const_iterator2;

        class const_iterator1 {
        public:
            NUMERICS_INLINE
            const_iterator1 (const const_iterator11_type &it1, const const_iterator21_type &it2):
                it1_ (it1), it2_ (it2) {}

            NUMERICS_INLINE
            const_iterator1 &operator ++ () {
                ++ it1_, ++ it2_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator -- () {
                -- it1_, -- it2_;
                return *this;
            }
            NUMERICS_INLINE
            value_type operator * () const {
                return functor_type () (*it1_, *it2_);
            }

            NUMERICS_INLINE
            const_iterator2 begin () const {
                return const_iterator2 (it1_.begin (), it2_.begin ()); 
            }
            NUMERICS_INLINE
            const_iterator2 end () const {
                return const_iterator2 (it1_.end (), it2_.end ()); 
            }

            NUMERICS_INLINE
            friend bool operator == (const const_iterator1 &it1, const const_iterator1 &it2) {
                return it1.it1_ == it2.it1_ && it1.it2_ == it2.it2_;
            }
            NUMERICS_INLINE
            friend bool operator != (const const_iterator1 &it1, const const_iterator1 &it2) {
                return ! (it1 == it2);
            }

        protected:
            const_iterator11_type it1_;
            const_iterator21_type it2_;
        };

        NUMERICS_INLINE
        const_iterator1 begin1 () const {
            return const_iterator1 (e1_.expression ().begin1 (), e2_.expression ().begin1 ());
        }
        NUMERICS_INLINE
        const_iterator1 end1 () const {
            return const_iterator1 (e1_.expression ().end1 (), e2_.expression ().end1 ());
        }

        class const_iterator2 {
        public:
            NUMERICS_INLINE
            const_iterator2 (const const_iterator12_type &it1, const const_iterator22_type &it2):
                it1_ (it1), it2_ (it2) {}

            NUMERICS_INLINE
            const_iterator2 &operator ++ () {
                ++ it1_, ++ it2_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator2 &operator -- () {
                -- it1_, -- it2_;
                return *this;
            }
            NUMERICS_INLINE
            value_type operator * () const {
                return functor_type () (*it1_, *it2_);
            }

            NUMERICS_INLINE
            const_iterator1 begin () const {
                return const_iterator1 (it1_.begin (), it2_.begin ()); 
            }
            NUMERICS_INLINE
            const_iterator1 end () const {
                return const_iterator2 (it1_.end (), it2_.end ()); 
            }

            NUMERICS_INLINE
            friend bool operator == (const const_iterator2 &it1, const const_iterator2 &it2) {
                return it1.it_ == it2.it_;
            }
            NUMERICS_INLINE
            friend bool operator != (const const_iterator2 &it1, const const_iterator2 &it2) {
                return ! (it1 == it2);
            }

        protected:
            const_iterator12_type it1_;
            const_iterator22_type it2_;
        };

        NUMERICS_INLINE
        const_iterator2 begin2 () const {
            return const_iterator2 (e1_.expression ().begin2 (), e2_.expression ().begin2 ());
        }
        NUMERICS_INLINE
        const_iterator2 end2 () const {
            return const_iterator2 (e1_.expression ().end2 (), e2_.expression ().end2 ());
        }
#endif // NUMERICS_USE_ITERATOR

    private:
        expression1_type e1_;
        expression2_type e2_;
    };

#ifdef NUMERICS_USE_ET

    template<class T1, class E1, class T2, class E2>
    NUMERICS_INLINE
    matrix_binary<matrix_const_reference<matrix_expression<T1, E1> >, 
                  matrix_const_reference<matrix_expression<T2, E2> >, 
                  scalar_plus<T1, T2> > 
    operator + (const matrix_expression<T1, E1> &e1, 
                const matrix_expression<T2, E2> &e2) {
        return matrix_binary<matrix_const_reference<matrix_expression<T1, E1> >, 
                             matrix_const_reference<matrix_expression<T2, E2> >, 
                             scalar_plus<T1, T2> > (e1, e2);
    }

    template<class T1, class E1, class T2, class E2>
    NUMERICS_INLINE
    matrix_binary<matrix_const_reference<matrix_expression<T1, E1> >, 
                  matrix_const_reference<matrix_expression<T2, E2> >, 
                  scalar_minus<T1, T2> > 
    operator - (const matrix_expression<T1, E1> &e1, 
                const matrix_expression<T2, E2> &e2) {
        return matrix_binary<matrix_const_reference<matrix_expression<T1, E1> >, 
                             matrix_const_reference<matrix_expression<T2, E2> >, 
                             scalar_minus<T1, T2> > (e1, e2);
    }

#else // NUMERICS_USE_ET

    template<class T1, class A1, class F1, class T2, class A2, class F2> 
    NUMERICS_INLINE
    matrix<promote_traits<T1, T2>::promote_type>
    operator + (const matrix<T1, A1, F1> &e1, 
                const matrix<T2, A2, F2> &e2) {
        matrix<promote_traits<T1, T2>::promote_type> r (common (e1.size1 (), e2.size1 ()),
                                                        common (e1.size2 (), e2.size2 ()));
        scalar_plus<T1, T2> f;
#ifndef NUMERICS_USE_ITERATOR
        for (size_type i = 0; i < r.size1 (); ++ i) 
            for (size_type j = 0; j < r.size2 (); ++ j) 
                r (i, j) = f (e1 (i, j), e2 (i, j));
#else // NUMERICS_USE_ITERATOR
        matrix<promote_traits<T1, T2>::promote_type>::iterator1 it1r (r.begin1 ());
        matrix<T1, A1, F1>::const_iterator1 it1e1 (e1.begin1 ());
        matrix<T2, A2, F2>::const_iterator1 it1e2 (e2.begin1 ());
        for (size_type i = 0; i < r.size1 (); ++ i, ++ it1r, ++ it1e1, ++ it1e2) {
            matrix<promote_traits<T1, T2>::promote_type>::iterator2 it2r (it1r.begin ());
            matrix<T1, A1, F1>::const_iterator2 it2e1 (it1e1.begin ());
            matrix<T2, A2, F2>::const_iterator2 it2e2 (it1e2.begin ());
            for (size_type j = 0; j < r.size2 (); ++ j, ++ it2r, ++ it2e1, ++ it2e2) 
                *it2r  = f (*it2e1, *it2e2);
        }
#endif // NUMERICS_USE_ITERATOR
        return r;
    }

    template<class T1, class A1, class F1, class T2, class A2, class F2> 
    NUMERICS_INLINE
    matrix<promote_traits<T1, T2>::promote_type>
    operator - (const matrix<T1, A1, F1> &e1, 
                const matrix<T2, A2, F2> &e2) {
        matrix<promote_traits<T1, T2>::promote_type> r (common (e1.size1 (), e2.size1 ()),
                                                        common (e1.size2 (), e2.size2 ()));
        scalar_minus<T1, T2> f;
#ifndef NUMERICS_USE_ITERATOR
        for (size_type i = 0; i < r.size1 (); ++ i) 
            for (size_type j = 0; j < r.size2 (); ++ j) 
                r (i, j) = f (e1 (i, j), e2 (i, j));
#else // NUMERICS_USE_ITERATOR
        matrix<promote_traits<T1, T2>::promote_type>::iterator1 it1r (r.begin1 ());
        matrix<T1, A1, F1>::const_iterator1 it1e1 (e1.begin1 ());
        matrix<T2, A2, F2>::const_iterator1 it1e2 (e2.begin1 ());
        for (; it1r != r.end1 (); ++ it1r, ++ it1e1, ++ it1e2) {
            matrix<promote_traits<T1, T2>::promote_type>::iterator2 it2r (it1r.begin ());
            matrix<T1, A1, F1>::const_iterator2 it2e1 (it1e1.begin ());
            matrix<T2, A2, F2>::const_iterator2 it2e2 (it1e2.begin ());
            for (; it2r != it1r.end (); ++ it2r, ++ it2e1, ++ it2e2) 
                *it2r  = f (*it2e1, *it2e2);
        }
#endif // NUMERICS_USE_ITERATOR
        return r;
    }

#endif // NUMERICS_USE_ET

    template<class E1, class E2, class F>
    class matrix_binary_scalar:
        public matrix_expression<typename F::value_type, matrix_binary_scalar<E1, E2, F> > {
    public:
        typedef E1 expression1_type;
        typedef E2 expression2_type;
        typedef F functor_type;
        typedef typename E2::size_type size_type;
        typedef typename F::value_type value_type;
#ifdef NUMERICS_USE_ITERATOR
        typedef typename E1::value_type const_iterator1_type;
        typedef typename E2::const_iterator1 const_iterator21_type;
        typedef typename E2::const_iterator2 const_iterator22_type;
#endif // NUMERICS_USE_ITERATOR

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

        NUMERICS_INLINE
        value_type operator () (size_type i, size_type j) const { 
            return functor_type () (e1_, e2_ (i, j)); 
        }

#ifdef NUMERICS_USE_ITERATOR
        class const_iterator2;

        class const_iterator1 {
        public:
            NUMERICS_INLINE
            const_iterator1 (const const_iterator1_type &it1, const const_iterator21_type &it2):
                it1_ (it1), it2_ (it2) {}

            NUMERICS_INLINE
            const_iterator1 &operator ++ () {
                ++ it2_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator -- () {
                -- it2_;
                return *this;
            }
            NUMERICS_INLINE
            value_type operator * () const {
                return functor_type () (it1_, *it2_); 
            }

            NUMERICS_INLINE
            const_iterator2 begin () const {
                return const_iterator2 (it1_, it2_.begin ()); 
            }
            NUMERICS_INLINE
            const_iterator2 end () const {
                return const_iterator2 (it1, it2_.end ()); 
            }

            NUMERICS_INLINE
            friend bool operator == (const const_iterator1 &it1, const const_iterator1 &it2) {
                return it1.it1_ == it2.it1_ && it1.it2_ == it2.it2_;
            }
            NUMERICS_INLINE
            friend bool operator != (const const_iterator1 &it1, const const_iterator1 &it2) {
                return ! (it1 == it2);
            }

        protected:
            const_iterator1_type it1_;
            const_iterator21_type it2_;
        };

        NUMERICS_INLINE
        const_iterator1 begin1 () const {
            return const_iterator1 (e1_, e2_.expression ().begin1 ());
        }
        NUMERICS_INLINE
        const_iterator1 end1 () const {
            return const_iterator1 (e1_, e2_.expression ().end1 ());
        }

        class const_iterator2 {
        public:
            NUMERICS_INLINE
            const_iterator2 (const const_iterator1_type &it1, const const_iterator22_type &it2):
                it1_ (it1), it2_ (it2) {}

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
            value_type operator * () const {
                return functor_type () (it1_, *it2_); 
            }

            NUMERICS_INLINE
            const_iterator1 begin () const {
                return const_iterator2 (it1_, it2_.begin ()); 
            }
            NUMERICS_INLINE
            const_iterator1 end () const {
                return const_iterator2 (it1_, it2_.end ()); 
            }

            NUMERICS_INLINE
            friend bool operator == (const const_iterator2 &it1, const const_iterator2 &it2) {
                return it1.it1_ == it2.it1_ && it1.it2_ == it2.it2_;
            }
            NUMERICS_INLINE
            friend bool operator != (const const_iterator2 &it1, const const_iterator2 &it2) {
                return ! (it1 == it2);
            }

        protected:
            const_iterator1_type it1_;
            const_iterator22_type it2_;
        };

        NUMERICS_INLINE
        const_iterator2 begin2 () const {
            return const_iterator2 (e1_, e2_.expression ().begin2 ());
        }
        NUMERICS_INLINE
        const_iterator2 end2 () const {
            return const_iterator2 (e1_, e2_.expression ().end2 ());
        }
#endif // NUMERICS_USE_ITERATOR

    private:
        expression1_type e1_;
        expression2_type e2_;
    };

#ifdef NUMERICS_USE_ET

    template<class T1, class T2, class E2>
    NUMERICS_INLINE
    matrix_binary_scalar<scalar_const_reference<T1>, 
                         matrix_const_reference<matrix_expression<T2, E2> >, 
                         scalar_multiplies<T1, T2> > 
    operator * (const T1 &e1, 
                const matrix_expression<T2, E2> &e2) {
        return matrix_binary_scalar<scalar_const_reference<T1>, 
                                    matrix_const_reference<matrix_expression<T2, E2> >, 
                                    scalar_multiplies<T1, T2> > (e1, e2);
    }

#else // NUMERICS_USE_ET

    template<class T1, class T2, class A2, class F2> 
    NUMERICS_INLINE
    matrix<promote_traits<T1, T2>::promote_type>
    operator * (const T1 &e1, 
                const matrix<T2, A2, F2> &e2) {
        matrix<promote_traits<T1, T2>::promote_type> r (e2.size1 (), e2.size2 ());
        scalar_multiplies<T1, T2> f;
#ifndef NUMERICS_USE_ITERATOR
        for (size_type i = 0; i < r.size1 (); ++ i) 
            for (size_type j = 0; j < r.size2 (); ++ j) 
                r (i, j) = f (e1, e2 (i, j));
#else // NUMERICS_USE_ITERATOR
        matrix<promote_traits<T1, T2>::promote_type>::iterator1 it1r (r.begin1 ());
        matrix<T2, A2, F2>::const_iterator1 it1e2 (e2.begin1 ());
        for (size_type i = 0; i < r.size1 (); ++ i, ++ it1r, ++ it1e2) {
            matrix<promote_traits<T1, T2>::promote_type>::iterator2 it2r (it1r.begin ());
            matrix<T2, A2, F2>::const_iterator2 it2e2 (it1e2.begin ());
            for (size_type j = 0; j < r.size2 (); ++ j, ++ it2r, ++ it2e2) 
                *it2r  = f (e1, *it2e2);
        }
#endif // NUMERICS_USE_ITERATOR
        return r;
    }

#endif // NUMERICS_USE_ET

    template<class E>
    class matrix_vector_unary:
        public vector_expression<typename E::value_type, matrix_vector_unary<E> > {
    public:
        typedef E expression_type;
        typedef typename E::size_type size_type;
        typedef typename E::value_type value_type;
#ifdef NUMERICS_USE_ITERATOR
        typedef typename slice::const_iterator const_iterator_type;
#endif // NUMERICS_USE_ITERATOR

        NUMERICS_INLINE
        matrix_vector_unary (const expression_type &e, const slice &s1, const slice &s2): 
            e_ (e), s1_ (s1), s2_ (s2) {}

        NUMERICS_INLINE
        size_type size () const { 
            return common (s1_.size (), s2_.size ());
        }

        NUMERICS_INLINE
        value_type operator () (size_type i) const { 
            return e_ (s1_ (i), s2_ (i)); 
        }

#ifdef NUMERICS_USE_ITERATOR
        class const_iterator {
        public:
            NUMERICS_INLINE
            const_iterator (const expression_type &e, const const_iterator_type &it1, const const_iterator_type &it2): 
                e_ (e), it1_ (it1), it2_ (it2) {}

            NUMERICS_INLINE
            const_iterator &operator ++ () {
                ++ it1_, ++ it2_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator &operator -- () {
                -- it1_, -- it2;
                return *this;
            }
            NUMERICS_INLINE
            value_type operator * () const {
                return e_ (*it1_, *it2_); 
            }

            NUMERICS_INLINE
            friend bool operator == (const const_iterator &it1, const const_iterator &it2) {
                return it1.it1_ == it2.it1_ && it1.it2_ == it2.it2_; 
            }
            NUMERICS_INLINE
            friend bool operator != (const const_iterator &it1, const const_iterator &it2) {
                return ! (it1 == it2);
            }

        protected:
            const expression_type &e_;
            const_iterator_type it1_;
            const_iterator_type it2_;
        };

        NUMERICS_INLINE
        const_iterator begin () const {
            return const_iterator (e_, s1_.begin (), s2_.begin ()); 
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return const_iterator (e_, s1_.end (), s2_.end ()); 
        }
#endif // NUMERICS_USE_ITERATOR

    private:
        expression_type e_;
        slice s1_;
        slice s2_;
    };

#ifdef NUMERICS_USE_ET

    template<class T, class E>
    NUMERICS_INLINE
    matrix_vector_unary<matrix_const_reference<matrix_expression<T, E> > > 
    row (const matrix_expression<T, E> &e, 
         size_type i) {
        return matrix_vector_unary<matrix_const_reference<matrix_expression<T, E> > > 
            (e, slice (i, 0, e.size2 ()), slice (0, 1, e.size2 ()));
    }

    template<class T, class E>
    NUMERICS_INLINE
    matrix_vector_unary<matrix_const_reference<matrix_expression<T, E> > > 
    column (const matrix_expression<T, E> &e, 
            size_type j) {
        return matrix_vector_unary<matrix_const_reference<matrix_expression<T, E> > > 
            (e, slice (0, 1, e.size1 ()), slice (j, 0, e.size1 ()));
    }

#else // NUMERICS_USE_ET

    template<class T, class A, class F>
    NUMERICS_INLINE
    vector<T>
    row (const matrix<T, A, F> &e, 
         size_type i) {
        vector<T> r (e.size1 ());
#ifndef NUMERICS_USE_ITERATOR
        for (size_type j = 0; j < r.size (); ++ j) 
            r (j) = e (i, j);
#else // NUMERICS_USE_ITERATOR
        vector<T>::iterator itr (r.begin ());
        for (size_type j = 0; j < r.size (); ++ j, ++ itr) 
            *itr = e (i, j);
#endif // NUMERICS_USE_ITERATOR
        return r;
    }

    template<class T, class E>
    NUMERICS_INLINE
    vector<T>
    column (const matrix_expression<T, E> &e, 
            size_type j) {
        vector<T> r (e.size2 ());
#ifndef NUMERICS_USE_ITERATOR
        for (size_type i = 0; i < r.size (); ++ i) 
            r (i) = e (i, j);
#else // NUMERICS_USE_ITERATOR
        vector<T>::iterator itr (r.begin ());
        for (size_type i = 0; i < r.size (); ++ i, ++ itr) 
            *itr = e (i, j);
#endif // NUMERICS_USE_ITERATOR
        return r;
    }

#endif // NUMERICS_USE_ET

    template<class E1, class E2, class F>
    class matrix_vector_binary:
        public vector_expression<typename F::value_type, matrix_vector_binary<E1, E2, F> > {
    public:
        typedef E1 expression1_type;
        typedef E2 expression2_type;
        typedef F functor_type;
        typedef promote_traits<typename E1::size_type, typename E2::size_type>::promote_type size_type;
        typedef typename F::value_type value_type;
#ifdef NUMERICS_USE_ITERATOR
        typedef typename E1::const_iterator1 const_iterator1_type;
        typedef typename E2::const_iterator const_iterator2_type;
#endif

        NUMERICS_INLINE
        matrix_vector_binary (const expression1_type &e1, const expression2_type &e2): 
            e1_ (e1), e2_ (e2) {}

        NUMERICS_INLINE
        size_type size () const { 
            return e1_.size1 (); 
        }

        NUMERICS_INLINE
        value_type operator () (size_type i) const { 
            return functor_type () (e1_, e2_, i); 
        }

#ifdef NUMERICS_USE_ITERATOR
        class const_iterator {
        public:
            NUMERICS_INLINE
            const_iterator (size_type size, const const_iterator1_type &it1, const const_iterator2_type &it2):
                size_ (size), it1_ (it1), it2_ (it2) {}

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
            value_type operator * () const {
                return functor_type () (size_, it1_.begin (), const_iterator2_type (it2_)); 
            }

            NUMERICS_INLINE
            friend bool operator == (const const_iterator &it1, const const_iterator &it2) {
                return it1.size_ == it2.size_ && it1.it1_ == it2.it1_ && it1.it2_ == it2.it2_; 
            }
            NUMERICS_INLINE
            friend bool operator != (const const_iterator &it1, const const_iterator &it2) {
                return ! (it1 == it2);
            }

        protected:
            size_type size_;
            const_iterator1_type it1_;
            const_iterator2_type it2_;
        };

        NUMERICS_INLINE
        const_iterator begin () const {
            return const_iterator (common (e1_.size2 (), e2_.size ()), e1_.begin1 (), e2_.begin ()); 
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return const_iterator (common (e1_.size2 (), e2_.size ()), e1_.end1 (), e2_.begin ()); 
        }
#endif // NUMERICS_USE_ITERATOR

    private:
        expression1_type e1_;
        expression2_type e2_;
    };

#ifdef NUMERICS_USE_ET

    template<class T1, class E1, class T2, class E2>
    NUMERICS_INLINE
    matrix_vector_binary<matrix_const_reference<matrix_expression<T1, E1> >, 
                         vector_const_reference<vector_expression<T2, E2> >, 
                         matrix_vector_prod<T1, T2> > 
    prod (const matrix_expression<T1, E1> &e1, 
          const vector_expression<T2, E2> &e2) {
        return matrix_vector_binary<matrix_const_reference<matrix_expression<T1, E1> >, 
                                    vector_const_reference<vector_expression<T2, E2> >, 
                                    matrix_vector_prod<T1, T2> > (e1, e2);
    }

#else // NUMERICS_USE_ET

    template<class T1, class A1, class F1, class T2, class A2, class F2>
    NUMERICS_INLINE
    vector<promote_traits<T1, T2>::promote_type>
    prod (const matrix<T1, A1, F1> &e1, 
          const vector<T2, A2, F2> &e2) {
        vector<promote_traits<T1, T2>::promote_type> r (e1.size1 ());
        matrix_vector_prod<T1, T2> f;
#ifndef NUMERICS_USE_ITERATOR
        for (size_type i = 0; i < r.size (); ++ i) 
            r (i) = f (e1, e2, i);
#else // NUMERICS_USE_ITERATOR
        vector<promote_traits<T1, T2>::promote_type>::iterator itr (r.begin ());
        for (size_type i = 0; i < r.size (); ++ i, ++ itr) 
            *itr = f (e1, e2, i);
#endif // NUMERICS_USE_ITERATOR
        return r;
    }

#endif // NUMERICS_USE_ET

    template<class E1, class E2, class E3, class F>
    class matrix_vector_ternary:
        public vector_expression<typename F::value_type, matrix_vector_ternary<E1, E2, E3, F> > {
    public:
        typedef E1 expression1_type;
        typedef E2 expression2_type;
        typedef E3 expression3_type;
        typedef F functor_type;
        typedef typename E2::size_type size_type;
        typedef typename F::value_type value_type;

        NUMERICS_INLINE
        matrix_vector_ternary (const expression1_type &e1, const expression2_type &e2, const expression3_type &e3, int hint): 
            e1_ (e1), e2_ (e2), e3_ (e3), hint_ (hint), f_ () {}

        NUMERICS_INLINE
        size_type size () const { 
            return e2_.size (); 
        }

        NUMERICS_INLINE
        value_type operator () (size_type i) const { 
            return f_ (e1_, e2_, e3_, hint_, i); 
        }

#ifdef NUMERICS_USE_ITERATOR
        class const_iterator {
        public:
            NUMERICS_INLINE
            const_iterator (const matrix_vector_ternary &mvt, size_type it): 
                mvt_ (mvt), it_ (it) {}

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
            value_type operator * () const {
                return mvt_ (it_); 
            }

            NUMERICS_INLINE
            friend bool operator == (const const_iterator &it1, const const_iterator &it2) {
                return &it1.mvt_ == &it2.mvt_ && it1.it_ == it2.it_; 
            }
            NUMERICS_INLINE
            friend bool operator != (const const_iterator &it1, const const_iterator &it2) {
                return ! (it1 == it2);
            }

        protected:
            const matrix_vector_ternary &mvt_;
            size_type it_;
        };

        NUMERICS_INLINE
        const_iterator begin () const {
            return const_iterator (*this, 0); 
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return const_iterator (*this, e2_.size ()); 
        }
#endif // NUMERICS_USE_ITERATOR

    private:
        expression1_type e1_;
        mutable expression2_type e2_;
        expression3_type e3_;
        int hint_;
        functor_type f_;
    };

#ifdef NUMERICS_USE_ET

    template<class T1, class E1, class T2, class A2, class F2, class T3, class E3>
    NUMERICS_INLINE
    matrix_vector_ternary<matrix_const_reference<matrix_expression<T1, E1> >, 
                          vector_reference<vector<T2, A2, F2> >, 
                          vector_const_reference<vector_expression<T3, E3> >, 
                          matrix_vector_solve<T1, T2, T3> > 
    solve (const matrix_expression<T1, E1> &e1, 
           vector<T2, A2, F2> &e2,
           const vector_expression<T3, E3> &e3,
           int hint) {
        return matrix_vector_ternary<matrix_const_reference<matrix_expression<T1, E1> >, 
                                     vector_reference<vector<T2, A2, F2> >, 
                                     vector_const_reference<vector_expression<T3, E3> >, 
                                     matrix_vector_solve<T1, T2, T3> > (e1, e2, e3, hint);
    }

#else // NUMERICS_USE_ET

    template<class T1, class A1, class F1, class T2, class A2, class F2, class T3, class A3, class F3>
    NUMERICS_INLINE
    vector<T2, A2, F2>
    solve (const matrix<T1, A1, F1> &e1, 
           vector<T2, A2, F2> &e2,
           const vector<T3, A3, F3> &e3,
           int hint) {
        matrix_vector_solve<T1, T2, T3> f;
        f (e1, e2, e3, hint, 0);
        return e2;
    }

#endif // NUMERICS_USE_ET

    template<class E1, class E2, class F>
    class matrix_matrix_binary:
        public matrix_expression<typename F::value_type, matrix_matrix_binary<E1, E2, F> > {
    public:
        typedef E1 expression1_type;
        typedef E2 expression2_type;
        typedef F functor_type;
        typedef promote_traits<typename E1::size_type, typename E2::size_type>::promote_type size_type;
        typedef typename F::value_type value_type;
#ifdef NUMERICS_USE_ITERATOR
        typedef typename E1::const_iterator1 const_iterator11_type;
        typedef typename E1::const_iterator2 const_iterator12_type;
        typedef typename E2::const_iterator1 const_iterator21_type;
        typedef typename E2::const_iterator2 const_iterator22_type;
#endif // NUMERICS_USE_ITERATOR

        NUMERICS_INLINE
        matrix_matrix_binary (const expression1_type &e1, const expression2_type &e2): 
            e1_ (e1), e2_ (e2) {}

        NUMERICS_INLINE
        size_type size1 () const { 
            return e1_.size1 ();
        }
        NUMERICS_INLINE
        size_type size2 () const { 
            return e2_.size2 ();
        }

        NUMERICS_INLINE
        value_type operator () (size_type i, size_type j) const { 
            return functor_type () (e1_, e2_, i, j); 
        }

#ifdef NUMERICS_USE_ITERATOR
        class const_iterator2;

        class const_iterator1 {
        public:
            NUMERICS_INLINE
            const_iterator1 (size_type size, const const_iterator11_type &it1, const const_iterator21_type &it2):
                size_ (size), it1_ (it1), it2_ (it2) {}

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
            value_type operator * () const {
                return functor_type () (size_, it1_.begin (), const_iterator21_type (it2_)); 
            }

            NUMERICS_INLINE
            const_iterator2 begin () const {
                return const_iterator2 (size_, it1_.begin (), it2_.begin ()); 
            }
            NUMERICS_INLINE
            const_iterator2 end () const {
                return const_iterator2 (size_, it1_.end (), it2_.begin ()); 
            }

            NUMERICS_INLINE
            friend bool operator == (const const_iterator1 &it1, const const_iterator1 &it2) {
                return it1.size_ == it2.size_ && it1.it1_ == it2.it1_ && it1.it2_ == it2.it2_;
            }
            NUMERICS_INLINE
            friend bool operator != (const const_iterator1 &it1, const const_iterator1 &it2) {
                return ! (it1 == it2);
            }

        protected:
            size_type size_;
            const_iterator11_type it1_;
            const_iterator21_type it2_;
        };

        NUMERICS_INLINE
        const_iterator1 begin1 () const {
            return const_iterator1 (common (e1_.size2 (), e2_.size1 ()), e1_.expression ().begin1 (), e2_.expression ().begin1 ());
        }
        NUMERICS_INLINE
        const_iterator1 end1 () const {
            return const_iterator1 (common (e1_.size2 (), e2_.size1 ()), e1_.expression ().end1 (), e2_.expression ().begin1 ());
        }

        class const_iterator2 {
        public:
            NUMERICS_INLINE
            const_iterator2 (size_type size, const const_iterator12_type &it1, const const_iterator22_type &it2):
                size_ (size), it1_ (it1), it2_ (it2) {}

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
            value_type operator * () const {
                return functor_type () (size_, const_iterator12_type (it1_), it2_.begin ()); 
            }

            NUMERICS_INLINE
            const_iterator1 begin () const {
                return const_iterator1 (size_, it1_.begin (), it2_.begin ()); 
            }
            NUMERICS_INLINE
            const_iterator1 end () const {
                return const_iterator1 (size_, it1_.begin (), it2_.end ()); 
            }

            NUMERICS_INLINE
            friend bool operator == (const const_iterator2 &it1, const const_iterator2 &it2) {
                return it1.size_ == it2.size_ && it1.it1_ == it2.it1_ && it1.it2_ == it2.it2_;
            }
            NUMERICS_INLINE
            friend bool operator != (const const_iterator2 &it1, const const_iterator2 &it2) {
                return ! (it1 == it2);
            }

        protected:
            size_type size_;
            const_iterator12_type it1_;
            const_iterator22_type it2_;
        };

        NUMERICS_INLINE
        const_iterator2 begin2 () const {
            return const_iterator2 (common (e1_.size2 (), e2_.size1 ()), e1_.expression ().begin2 (), e2_.expression ().begin2 ());
        }
        NUMERICS_INLINE
        const_iterator2 end2 () const {
            return const_iterator2 (common (e1_.size2 (), e2_.size1 ()), e1_.expression ().begin2 (), e2_.expression ().end2 ());
        }
#endif // NUMERICS_USE_ITERATOR

    private:
        expression1_type e1_;
        expression2_type e2_;
    };

#ifdef NUMERICS_USE_ET

    template<class T1, class E1, class T2, class E2>
    NUMERICS_INLINE
    matrix_matrix_binary<matrix_const_reference<matrix_expression<T1, E1> >, 
                         matrix_const_reference<matrix_expression<T2, E2> >, 
                         matrix_matrix_prod<T1, T2> > 
    prod (const matrix_expression<T1, E1> &e1, 
          const matrix_expression<T2, E2> &e2) {
        return matrix_matrix_binary<matrix_const_reference<matrix_expression<T1, E1> >, 
                                    matrix_const_reference<matrix_expression<T2, E2> >, 
                                    matrix_matrix_prod<T1, T2> > (e1, e2);
    }

#else // NUMERICS_USE_ET

    template<class T1, class A1, class F1, class T2, class A2, class F2>
    NUMERICS_INLINE
    matrix<promote_traits<T1, T2>::promote_type>
    prod (const matrix<T1, A1, F1> &e1, 
          const matrix<T2, A2, F2> &e2) {
        matrix<promote_traits<T1, T2>::promote_type> r (e1.size1 (), e2.size2 ());
        matrix_matrix_prod<T1, T2> f;
        for (size_type i = 0; i < r.size1 (); ++ i) 
            for (size_type j = 0; j < r.size2 (); ++ j) 
                r (i, j) = f (e1, e2, i, j);
        return r;
    }

#endif // NUMERICS_USE_ET

    template<class E1, class E2, class E3, class F>
    class matrix_matrix_ternary:
        public matrix_expression<typename F::value_type, matrix_matrix_ternary<E1, E2, E3, F> > {
    public:
        typedef E1 expression1_type;
        typedef E2 expression2_type;
        typedef E3 expression3_type;
        typedef F functor_type;
        typedef typename E2::size_type size_type;
        typedef typename F::value_type value_type;

        NUMERICS_INLINE
        matrix_matrix_ternary (const expression1_type &e1, const expression2_type &e2, const expression3_type &e3, int hint): 
            e1_ (e1), e2_ (e2), e3_ (e3), hint_ (hint), f_ () {}

        NUMERICS_INLINE
        size_type size1 () const { 
            return e2_.size1 ();
        }
        NUMERICS_INLINE
        size_type size2 () const { 
            return e2_.size2 ();
        }

        NUMERICS_INLINE
        value_type operator () (size_type i, size_type j) const { 
            return f_ (e1_, e2_, e3_, hint_, i, j); 
        }

#ifdef NUMERICS_USE_ITERATOR
        class const_iterator2;

        class const_iterator1 {
        public:
            NUMERICS_INLINE
            const_iterator1 (const matrix_matrix_ternary &mmt, size_type it1, size_type it2):
                mmt_ (mmt), it1_ (it1), it2_ (it2) {}

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
            value_type operator * () const {
                return mmt_ (it1_, it2_); 
            }

            NUMERICS_INLINE
            const_iterator2 begin () const {
                return const_iterator2 (mmt_, it1_, 0); 
            }
            NUMERICS_INLINE
            const_iterator2 end () const {
                return const_iterator2 (mmt_, it1_, mmt_.size2 ()); 
            }

            NUMERICS_INLINE
            friend bool operator == (const const_iterator1 &it1, const const_iterator1 &it2) {
                return &it1.mmt_ == &it2.mmt_ && it1.it1_ == it2.it1_ && it1.it2_ == it2.it2_;
            }
            NUMERICS_INLINE
            friend bool operator != (const const_iterator1 &it1, const const_iterator1 &it2) {
                return ! (it1 == it2);
            }

        protected:
            const matrix_matrix_ternary &mmt_;
            size_type it1_;
            size_type it2_;
        };

        NUMERICS_INLINE
        const_iterator1 begin1 () const {
            return const_iterator1 (*this, 0, 0);
        }
        NUMERICS_INLINE
        const_iterator1 end1 () const {
            return const_iterator1 (*this, size1_, 0);
        }

        class const_iterator2 {
        public:
            NUMERICS_INLINE
            const_iterator2 (const matrix_matrix_ternary &mmt, size_type it1, size_type it2):
                mmt_ (mmt), it1_ (it1), it2_ (it2) {}

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
            value_type operator * () const {
                return mmt_ (it1_, it2_); 
            }

            NUMERICS_INLINE
            const_iterator1 begin () const {
                return const_iterator2 (mmt_, 0, it2_); 
            }
            NUMERICS_INLINE
            const_iterator1 end () const {
                return const_iterator2 (mmt_, mmt_.size1 (), it2_); 
            }

            NUMERICS_INLINE
            friend bool operator == (const const_iterator2 &it1, const const_iterator2 &it2) {
                return &it1.mmt_ == &it2.mmt_ && it1.it1_ == it2.it1_ && it1.it2_ == it2.it2_;
            }
            NUMERICS_INLINE
            friend bool operator != (const const_iterator2 &it1, const const_iterator2 &it2) {
                return ! (it1 == it2);
            }

        protected:
            const matrix_matrix_ternary &mmt_;
            size_type it1_;
            size_type it2_;
        };

        NUMERICS_INLINE
        const_iterator2 begin2 () const {
            return const_iterator2 (*this, 0, 0);
        }
        NUMERICS_INLINE
        const_iterator2 end2 () const {
            return const_iterator2 (*this, 0, size2_);
        }
#endif // NUMERICS_USE_ITERATOR

    private:
        expression1_type e1_;
        mutable expression2_type e2_;
        expression3_type e3_;
        int hint_;
        functor_type f_;
    };

#ifdef NUMERICS_USE_ET

    template<class T1, class E1, class T2, class A2, class F2, class T3, class E3>
    NUMERICS_INLINE
    matrix_matrix_ternary<matrix_const_reference<matrix_expression<T1, E1> >, 
                          matrix_reference<matrix<T2, A2, F2> >, 
                          matrix_const_reference<matrix_expression<T3, E3> >, 
                          matrix_matrix_solve<T1, T2, T3> > 
    solve (const matrix_expression<T1, E1> &e1, 
           matrix<T2, A2, F2> &e2,
           const matrix_expression<T3, E3> &e3,
           int hint) {
        return matrix_matrix_ternary<matrix_const_reference<matrix_expression<T1, E1> >, 
                                     matrix_reference<matrix<T2, A2, F2> >, 
                                     matrix_const_reference<matrix_expression<T3, E3> >, 
                                     matrix_matrix_solve<T1, T2, T3> > (e1, e2, e3, hint);
    }

#else // NUMERICS_USE_ET

    template<class T1, class A1, class F1, class T2, class A2, class F2, class T3, class A3, class F3>
    NUMERICS_INLINE
    matrix<T2, A2, F2>
    solve (const matrix<T1, A1, F1> &e1, 
           matrix<T2, A2, F2> &e2,
           const matrix<T3, A3, F3> &e3,
           int hint) {
        matrix_matrix_solve<T1, T2, T3> f;
        f (e1, e2, e3, hint, 0, 0);
        return e2;
    }

#endif // NUMERICS_USE_ET

    template<class E>
    class matrix_expr_slice:
        public matrix_expression<typename E::value_type, matrix_expr_slice<E> > {
    public:
        typedef E expression_type;
        typedef typename E::size_type size_type;
        typedef typename E::value_type value_type;

        NUMERICS_INLINE
        matrix_expr_slice (const expression_type &e, const slice &s1, const slice &s2): 
            e_ (e), s1_ (s1), s2_ (s2) {}

        NUMERICS_INLINE
        size_type size1 () const { 
            return s1_.size ();
        }
        NUMERICS_INLINE
        size_type size2 () const { 
            return s2_.size ();
        }

        NUMERICS_INLINE
        value_type operator () (size_type i, size_type j) const { 
            return e_ (s_ (i), s_ (j)); 
        }

    private:
        expression_type e_;
        slice s1_;
        slice s2_;
    };

}

#endif // NUMERICS_MATRIX_ET_H