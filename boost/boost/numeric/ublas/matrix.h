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

#ifndef NUMERICS_MATRIX_H
#define NUMERICS_MATRIX_H

#include <valarray>
#include <vector>

#include "config.h"
#include "storage.h"
#include "vector_et.h"
#include "matrix_et.h"

// Iterators based on ideas of Jeremy Siek

namespace numerics {

    template<class F1, class F2>
    struct row_major {
        typedef F1 functor1_type;
        typedef F2 functor2_type;

        static 
        NUMERICS_INLINE
        size_type element (size_type i, size_type size1, size_type j, size_type size2) {
            i = functor1_type::element (i, size1);
            check<bad_index>::precondition (i < size1);
            j = functor2_type::element (j, size2);
            check<bad_index>::precondition (j < size2);
            return i * size2 + j;
        }
        static 
        NUMERICS_INLINE
        size_type begin1 (size_type size1, size_type size2) {
            return functor1_type::begin (size1) * size2 + functor2_type::begin (size2);
        }
        static 
        NUMERICS_INLINE
        size_type end1 (size_type size1, size_type size2) {
            return functor1_type::end (size1) * size2 + functor2_type::begin (size2);
        }
        static 
        NUMERICS_INLINE
        size_type one1 (size_type size1, size_type size2) {
            return functor1_type::one () * size2;
        }
        static 
        NUMERICS_INLINE
        size_type begin2 (size_type size1, size_type size2) {
            return functor1_type::begin (size1) * size2 + functor2_type::begin (size2);
        }
        static 
        NUMERICS_INLINE
        size_type end2 (size_type size1, size_type size2) {
            return functor1_type::begin (size1) * size2 + functor2_type::end (size2);
        }
        static 
        NUMERICS_INLINE
        size_type one2 (size_type size1, size_type size2) {
            return functor2_type::one ();
        }
    };

    template<class F1, class F2>
    struct column_major {
        typedef F1 functor1_type;
        typedef F2 functor2_type;

        static 
        NUMERICS_INLINE
        size_type element (size_type i, size_type size1, size_type j, size_type size2) {
            i = functor1_type::element (i, size1);
            check<bad_index>::precondition (i < size1);
            j = functor2_type::element (j, size2);
            check<bad_index>::precondition (j < size2);
            return i + j * size1;
        }
        static 
        NUMERICS_INLINE
        size_type begin1 (size_type size1, size_type size2) {
            return functor1_type::begin (size1) + functor2_type::begin (size2) * size1;
        }
        static 
        NUMERICS_INLINE
        size_type end1 (size_type size1, size_type size2) {
            return functor1_type::end (size1) + functor2_type::begin (size2) * size1;
        }
        static 
        NUMERICS_INLINE
        size_type one1 (size_type size1, size_type size2) {
            return functor1_type::one ();
        }
        static 
        NUMERICS_INLINE
        size_type begin2 (size_type size1, size_type size2) {
            return functor1_type::begin (size1) + functor2_type::begin (size2) * size1;
        }
        static 
        NUMERICS_INLINE
        size_type end2 (size_type size1, size_type size2) {
            return functor1_type::begin (size1) + functor2_type::end (size2) * size1;
        }
        static 
        NUMERICS_INLINE
        size_type one2 (size_type size1, size_type size2) {
            return functor2_type::one () * size1;
        }
    };

    // Array based matrix class 
    template<class T, class A, class F>
    class matrix: 
        public matrix_expression<T, matrix<T, A, F> > {
    public:      
        typedef unsigned size_type;
        typedef T value_type;
        typedef T &reference_type;
        typedef T *pointer_type;
        typedef const A const_array_type;
        typedef A array_type;
        typedef F functor_type;
        typedef typename F::functor1_type functor1_type;
        typedef typename F::functor2_type functor2_type;
// #ifndef USE_MSVC
        typedef matrix_vector_slice<matrix<T, A, F> > matrix_vector_slice_type;
// #endif

        NUMERICS_EXPLICIT
        NUMERICS_INLINE
        matrix (): 
            size1_ (0), size2_ (0), data_ (0) {}
        NUMERICS_EXPLICIT
        NUMERICS_INLINE
        matrix (size_type size1, size_type size2): 
            size1_ (size1), size2_ (size2), data_ (size1 * size2) {}
        NUMERICS_EXPLICIT
        NUMERICS_INLINE
        matrix (const matrix &m): 
            size1_ (m.size1_), size2_ (m.size2_), data_ (m.size1_ * m.size2_) { 
            safe_copy (data_, m.data_);
        }
        template<class AT, class AE>
        NUMERICS_EXPLICIT
        NUMERICS_INLINE
        matrix (const matrix_expression<AT, AE> &ae): 
            size1_ (ae.size1 ()), size2_ (ae.size2 ()), data_ (ae.size1 () * ae.size2 ()) { 
            assign (ae); 
        }
        NUMERICS_INLINE
        virtual ~matrix () {}

        NUMERICS_INLINE
        void resize (size_type size1, size_type size2) {
            size1_ = size1;
            size2_ = size2;
            data_.resize (size1 * size2);
        }

        NUMERICS_INLINE
        const_array_type &data () const {
            return data_;
        }
        NUMERICS_INLINE
        array_type &data () {
            return data_;
        }
        NUMERICS_INLINE
        size_type size1 () const { 
            return size1_;
        }
        NUMERICS_INLINE
        size_type size2 () const { 
            return size2_;
        }

        NUMERICS_INLINE
        value_type operator () (size_type i, size_type j) const {
            return data_ [functor_type::element (i, size1_, j, size2_)]; 
        }
        NUMERICS_INLINE
        reference_type operator () (size_type i, size_type j) {
            return data_ [functor_type::element (i, size1_, j, size2_)]; 
        }

        NUMERICS_INLINE
// #ifdef USE_MSVC
//          matrix_vector_slice<matrix> operator [] (size_type i) {
// #else
        matrix_vector_slice_type operator [] (size_type i) {
// #endif
            return matrix_vector_slice<matrix> (*this, slice (i, 0, size2_), slice (0, 1, size2_));
        }
        NUMERICS_INLINE
// #ifdef USE_MSVC
//         matrix_vector_slice<matrix> row (size_type i) {
// #else
        matrix_vector_slice_type row (size_type i) {
// #endif
            return matrix_vector_slice<matrix> (*this, slice (i, 0, size2_), slice (0, 1, size2_));
        }
        NUMERICS_INLINE
// #ifdef USE_MSVC
//         matrix_vector_slice<matrix> column (size_type j) {
// #else
        matrix_vector_slice_type column (size_type j) {
// #endif
            return matrix_vector_slice<matrix> (*this, slice (0, 1, size1_), slice (j, 0, size1_));
        }

        NUMERICS_INLINE
        matrix &operator = (const matrix &m) { 
            safe_copy (data_, m.data_);
            return *this;
        }
        template<class AT, class AE>
        NUMERICS_INLINE
        matrix &operator ^= (const matrix_expression<AT, AE> &ae) { 
            assign (ae); 
            return *this;
        }
        template<class AT, class AE>
        NUMERICS_INLINE
        matrix &operator = (const matrix_expression<AT, AE> &ae) { 
            matrix e (ae);
            fast_copy (data_, e.data_);
            return *this;
        }
        template<class AT, class AE>
        NUMERICS_INLINE
        matrix& operator += (const matrix_expression<AT, AE> &ae) {
            matrix e (*this + ae);
            fast_copy (data_, e.data_);
            return *this;
        }
        template<class AT, class AE>
        NUMERICS_INLINE
        matrix& operator -= (const matrix_expression<AT, AE> &ae) {
            matrix e (*this - ae);
            fast_copy (data_, e.data_);
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        matrix& operator *= (const AT &at) {
#ifndef NUMERICS_USE_ITERATOR
            for (size_type i = 0; i < size1_; ++ i)
                for (size_type j = 0; j < size2_; ++ j)
                    (*this ) (i, j) *= at;
#else // NUMERICS_USE_ITERATOR
            iterator1 it1 (begin1 ());
            for (size_type i = 0; i < size1_; ++ i) {
                iterator2 it2 (it1.begin ());
                for (size_type j = 0; j < size2_; ++ j)
                    *it2 *= at, ++ it2;
                ++ it1;
            }
#endif // NUMERICS_USE_ITERATOR
            return *this;
        }

#ifdef NUMERICS_USE_ITERATOR
        class const_iterator2;

        class const_iterator1 {
        public:
            NUMERICS_INLINE
            const_iterator1 (const matrix &m, pointer_type it):
                m_ (m), it_ (it) {}

            NUMERICS_INLINE
            const_iterator1 &operator ++ () {
                it_ += functor_type::one1 (m_.size1 (), m_.size2 ());
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator -- () {
                it_ -= functor_type::one1 (m_.size1 (), m_.size2 ());
                return *this;
            }
            NUMERICS_INLINE
            value_type operator * () const {
                check<bad_index>::precondition (m_.data ().data () <= it_);
                check<bad_index>::precondition (it_ < m_.data ().data () + m_.size1 () * m_.size2 ());
                return *it_;
            }

            NUMERICS_INLINE
            const_iterator2 begin () const {
                return const_iterator2 (m_, it_); 
            }
            NUMERICS_INLINE
            const_iterator2 end () const {
                return const_iterator2 (m_, it_ + matrix::functor2_type::end (m_.size2 ())); 
            }

            NUMERICS_INLINE
            friend bool operator == (const const_iterator1 &it1, const const_iterator1 &it2) {
                return &it1.m_ == &it2.m_ && it1.it_ == it2.it_;
            }
            NUMERICS_INLINE
            friend bool operator != (const const_iterator1 &it1, const const_iterator1 &it2) {
                return ! (it1 == it2);
            }

        protected:
            const matrix &m_;
            pointer_type it_;
        };

        NUMERICS_INLINE
        const_iterator1 begin1 () const {
            return const_iterator1 (*this, data_.data () + functor_type::begin1 (size1 (), size2 ()));
        }
        NUMERICS_INLINE
        const_iterator1 end1 () const {
            return const_iterator1 (*this, data_.data () + functor_type::end1 (size1 (), size2 ()));
        }

        class iterator2;

        class iterator1:
            public const_iterator1 {
        public:
            NUMERICS_INLINE
            iterator1 (const matrix &m, pointer_type it):
                const_iterator1 (m, it) {}

            NUMERICS_INLINE
            reference_type operator * () {
                check<bad_index>::precondition (m_.data ().data () <= it_);
                check<bad_index>::precondition (it_ < m_.data ().data () + m_.size1 () * m_.size2 ());
                return *it_;
            }

            NUMERICS_INLINE
            iterator2 begin () {
                return iterator2 (m_, it_); 
            }
            NUMERICS_INLINE
            iterator2 end () {
                return iterator2 (m_, it_ + matrix::functor2_type::end (m_.size2 ())); 
            }
        };

        NUMERICS_INLINE
        iterator1 begin1 () {
            return iterator1 (*this, data_.data () + functor_type::begin1 (size1 (), size2 ()));
        }
        NUMERICS_INLINE
        iterator1 end1 () {
            return iterator1 (*this, data_.data () + functor_type::end1 (size1 (), size2 ()));
        }

        class const_iterator2 {
        public:
            NUMERICS_INLINE
            const_iterator2 (const matrix &m, pointer_type it):
                m_ (m), it_ (it) {}

            NUMERICS_INLINE
            const_iterator2 &operator ++ () {
                it_ += functor_type::one2 (m_.size1 (), m_.size2 ());
                return *this;
            }
            NUMERICS_INLINE
            const_iterator2 &operator -- () {
                it_ -= functor_type::one2 (m_.size1 (), m_.size2 ());
                return *this;
            }
            NUMERICS_INLINE
            value_type operator * () const {
                check<bad_index>::precondition (m_.data ().data () <= it_);
                check<bad_index>::precondition (it_ < m_.data ().data () + m_.size1 () * m_.size2 ());
                return *it_;
            }

            NUMERICS_INLINE
            const_iterator1 begin () const {
                return const_iterator1 (m_, it_); 
            }
            NUMERICS_INLINE
            const_iterator1 end () const {
                return const_iterator1 (m_, it_ + matrix::functor1_type::end (m_.size1 ())); 
            }

            NUMERICS_INLINE
            friend bool operator == (const const_iterator2 &it1, const const_iterator2 &it2) {
                return &it1.m_ == &it2.m_ && it1.it_ == it2.it_;
            }
            NUMERICS_INLINE
            friend bool operator != (const const_iterator2 &it1, const const_iterator2 &it2) {
                return ! (it1 == it2);
            }

        protected:
            const matrix &m_;
            pointer_type it_;
        };

        NUMERICS_INLINE
        const_iterator2 begin2 () const {
            return const_iterator2 (*this, data_.data () + functor_type::begin2 (size1 (), size2 ()));
        }
        NUMERICS_INLINE
        const_iterator2 end2 () const {
            return const_iterator2 (*this, data_.data () + functor_type::end2 (size1 (), size2 ()));
        }

        class iterator2: 
            public const_iterator2 {
        public:
            NUMERICS_INLINE
            iterator2 (const matrix &m, pointer_type it):
                const_iterator2 (m, it) {}

            NUMERICS_INLINE
            reference_type operator * () {
                check<bad_index>::precondition (m_.data ().data () <= it_);
                check<bad_index>::precondition (it_ < m_.data ().data () + m_.size1 () * m_.size2 ());
                return *it_;
            }

            NUMERICS_INLINE
            iterator1 begin () {
                return iterator1 (m_, it_); 
            }
            NUMERICS_INLINE
            iterator1 end () {
                return iterator1 (m_, it_ + matrix::functor1_type::end (m_.size1 ())); 
            }
        };

        NUMERICS_INLINE
        iterator2 begin2 () {
            return iterator2 (*this, data_.data () + functor_type::begin2 (size1 (), size2 ()));
        }
        NUMERICS_INLINE
        iterator2 end2 () {
            return iterator2 (*this, data_.data () + functor_type::end2 (size1 (), size2 ()));
        }
#endif

    private:
        size_type size1_;
        size_type size2_;
        array_type data_;

        template<class AT, class AE>
        NUMERICS_INLINE
        void assign (const matrix_expression<AT, AE> &ae) {
            size_type size1 = common (size1_, ae.size1 ());
            size_type size2 = common (size2_, ae.size2 ());
#ifndef NUMERICS_USE_ITERATOR
            for (size_type i = 0; i < size1; ++ i)
                for (size_type j = 0; j < size2; ++ j)
                    (*this ) (i, j) = ae (i, j);
#else // NUMERICS_USE_ITERATOR
            iterator1 it1 (begin1 ());
            AE::const_iterator1 it1ae (ae.expression ().begin1 ());
            for (size_type i = 0; i < size1; ++ i) {
                iterator2 it2 (it1.begin ());
                AE::const_iterator2 it2ae (it1ae.begin ());
                for (size_type j = 0; j < size2; ++ j)
                    *it2 = *it2ae, ++ it2, ++ it2ae;
                ++ it1, ++ it1ae;
            }
#endif // NUMERICS_USE_ITERATOR
        }
    };

    template<class T, class A, class F>
    NUMERICS_INLINE
    matrix<T, A, F> &safe_copy (matrix<T, A, F> &m1, matrix<T, A, F> &m2) {
        check<bad_size>::precondition (m1.size1 () == m2.size1 ());
        check<bad_size>::precondition (m1.size2 () == m2.size2 ());
        safe_copy (m1.data (), m2.data ());
        return m1;
    }
    template<class T, class A, class F>
    NUMERICS_INLINE
    matrix<T, A, F> &fast_copy (matrix<T, A, F> &m1, matrix<T, A, F> &m2) {
        check<bad_size>::precondition (m1.size1 () == m2.size1 ());
        check<bad_size>::precondition (m1.size2 () == m2.size2 ());
        fast_copy (m1.data (), m2.data ());
        return m1;
    }
    template<class T, class A, class F>
    NUMERICS_INLINE
    void safe_swap (matrix<T, A, F> &m1, matrix<T, A, F> &m2) {
        check<bad_size>::precondition (m1.size1 () == m2.size1 ());
        check<bad_size>::precondition (m1.size2 () == m2.size2 ());
        safe_swap (m1.data (), m2.data ());
    }
    template<class T, class A, class F>
    NUMERICS_INLINE
    void fast_swap (matrix<T, A, F> &m1, matrix<T, A, F> &m2) {
        check<bad_size>::precondition (m1.size1 () == m2.size1 ());
        check<bad_size>::precondition (m1.size2 () == m2.size2 ());
        safe_swap (m1.data (), m2.data ());
    }

    // Matrix based vector slice class
    template<class M>
    class matrix_vector_slice:
		public vector_expression<typename M::value_type, matrix_vector_slice<M> > {
    public:      
        typedef M matrix_type;
        typedef typename M::size_type size_type;
        typedef typename M::value_type value_type;
        typedef typename M::reference_type reference_type;
#ifdef NUMERICS_USE_ITERATOR
        typedef slice::const_iterator const_iterator_type;
#endif // NUMERICS_USE_ITERATOR

        NUMERICS_INLINE
        matrix_vector_slice (matrix_type &data, const slice &s1, const slice &s2): 
            data_ (data), s1_ (s1), s2_ (s2) {}
        NUMERICS_INLINE
        virtual ~matrix_vector_slice () {}

        NUMERICS_INLINE
        size_type size () const { 
            return common (s1_.size (), s2_.size ()); 
        }

        NUMERICS_INLINE
        value_type operator () (size_type i) const {
            return data_ (s1_ (i), s2_ (i)); 
        }
        NUMERICS_INLINE
        reference_type operator () (size_type i) {
            return data_ (s1_ (i), s2_ (i)); 
        }

        NUMERICS_INLINE
        value_type operator [] (size_type i) const { 
            return data_ (s1_ (i). s2_ (i)); 
        }
        NUMERICS_INLINE
        reference_type operator [] (size_type i) { 
            return data_ (s1_ (i), s2_ (i)); 
        }

        NUMERICS_INLINE
        matrix_vector_slice &operator ^= (const matrix_vector_slice &mvs) { 
            assign (mvs);
            return *this;
        }
        NUMERICS_INLINE
        matrix_vector_slice &operator = (const matrix_vector_slice &mvs) { 
            vector<value_type> mv (mvs);
            assign (mv);
            return *this;
        }
        template<class AT, class AE>
        NUMERICS_INLINE
        matrix_vector_slice &operator ^= (const vector_expression<AT, AE> &ae) {
            assign (ae);
            return *this;
        }
        template<class AT, class AE>
        NUMERICS_INLINE
        matrix_vector_slice &operator = (const vector_expression<AT, AE> &ae) {        
            vector<value_type> e (ae);
            assign (e);
            return *this;
        }
        template<class AT, class AE>
        NUMERICS_INLINE
        matrix_vector_slice &operator += (const vector_expression<AT, AE> &ae) {
            vector<value_type> e (*this + ae);
            assign (e);
            return *this;
        }
        template<class AT, class AE>
        NUMERICS_INLINE
        matrix_vector_slice &operator -= (const vector_expression<AT, AE> &ae) {
            vector<value_type> e (*this - ae);
            assign (e);
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        matrix_vector_slice &operator *= (const AT &at) {
            size_type size = this->size ();
#ifndef NUMERICS_USE_ITERATOR
            for (size_type i = 0; i < size; ++ i)
                (*this) (i) *= at; 
#else // NUMERICS_USE_ITERATOR
            iterator it (begin ());
            for (size_type i = 0; i < size; ++ i) 
                *it *= at, ++ it;
#endif // NUMERICS_USE_ITERATOR
            return *this;
        }

#ifdef NUMERICS_USE_ITERATOR
        class const_iterator {
        public:
            NUMERICS_INLINE
            const_iterator (matrix_type &m, const const_iterator_type &it1, const const_iterator_type &it2):
                m_ (m), it1_ (it1), it2_ (it2) {}

            NUMERICS_INLINE
            const_iterator &operator ++ () {
                ++ it1_, ++ it2_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator &operator -- () {
                -- it1_, -- it2_;
                return *this;
            }
            NUMERICS_INLINE
            value_type operator * () const {
                return m_ (*it1_, *it2_); 
            }

            NUMERICS_INLINE
            friend bool operator == (const const_iterator &it1, const const_iterator &it2) {
                return &it1.m_ == &it2.m_ && it1.it1_ == it2.it1_ && it1.it2_ == it2.it2_;
            }
            NUMERICS_INLINE
            friend bool operator != (const const_iterator &it1, const const_iterator &it2) {
                return ! (it1 == it2);
            }

        protected:
            matrix_type &m_;
            const_iterator_type it1_;
            const_iterator_type it2_;
        };

        NUMERICS_INLINE
        const_iterator begin () const {
            return const_iterator (data_, s1_.begin (), s2_.begin ()); 
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return const_iterator (data_, s1_.end (), s2_.end ()); 
        }

        class iterator:
            public const_iterator {
        public:
            NUMERICS_INLINE
            iterator (matrix_type &m, const const_iterator_type &it1, const const_iterator_type &it2):
                const_iterator (m, it1, it2) {}

            NUMERICS_INLINE
            reference_type operator * () {
                return m_ (*it1_, *it2_); 
            }
        };

        NUMERICS_INLINE
        iterator begin () {
            return iterator (data_, s1_.begin (), s2_.begin ()); 
        }
        NUMERICS_INLINE
        iterator end () {
            return iterator (data_, s1_.end (), s2_.end ()); 
        }
#endif // NUMERICS_USE_ITERATOR

    private:
        matrix_type &data_;
        slice s1_;
        slice s2_;

        template<class AT, class AE>
        NUMERICS_INLINE
        void assign (const vector_expression<AT, AE> &ae) {
            size_type size = common (this->size (), ae.size ());
#ifndef NUMERICS_USE_ITERATOR
            for (size_type i = 0; i < size; ++ i)
                (*this) (i) = ae (i); 
#else // NUMERICS_USE_ITERATOR
            iterator it (begin ());
            AE::const_iterator itae (ae.expression ().begin ());
            for (size_type i = 0; i < size; ++ i) 
                *it = *itae, ++ it, ++ itae;
#endif // NUMERICS_USE_ITERATOR
        }
    };

    // Matrix based slice class
    template<class M>
    class matrix_slice:
		public matrix_expression<typename M::value_type, matrix_slice<M> > {
    public:      
        typedef M matrix_type;
        typedef typename M::size_type size_type;
        typedef typename M::value_type value_type;
        typedef typename M::reference_type reference_type;
#ifdef NUMERICS_USE_ITERATOR
        typedef slice::const_iterator const_iterator_type;
#endif // NUMERICS_USE_ITERATOR

        NUMERICS_INLINE
        matrix_slice (matrix_type &data, const slice &s1, const slice &s2): 
            data_ (data), s1_ (s1), s2_ (s2) {}
        NUMERICS_INLINE
        virtual ~matrix_slice () {}

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
            return data_ (s1_ (i), s2_ (j)); 
        }
        NUMERICS_INLINE
        reference_type operator () (size_type i, size_type j) {
            return data_ (s1_ (i), s2_ (j)); 
        }

        NUMERICS_INLINE
        matrix_vector_slice<matrix_type> operator [] (size_type i) {
            return matrix_vector_slice<matrix_type> (data_, slice (i, 0, size2 ()), slice (0, 1, size2 ()));
        }
        NUMERICS_INLINE
        matrix_vector_slice<matrix_type> row (size_type i) {
            return matrix_vector_slice<matrix_type> (data_, slice (i, 0, size2 ()), slice (0, 1, size2 ()));
        }
        NUMERICS_INLINE
        matrix_vector_slice<matrix_type> column (size_type j) {
            return matrix_vector_slice<matrix_type> (data_, slice (0, 1, size1 ()), slice (j, 0, size2 ()));
        }

        NUMERICS_INLINE
        matrix_slice &operator ^= (const matrix_slice &ms) { 
            assign (ms); 
            return *this;
        }
        NUMERICS_INLINE
        matrix_slice &operator = (const matrix_slice &ms) { 
            matrix<value_type> m (ms);
            assign (m); 
            return *this;
        }
        template<class AT, class AE>
        NUMERICS_INLINE
        matrix_slice &operator ^= (const matrix_expression<AT, AE> &ae) { 
            assign (ae); 
            return *this;
        }
        template<class AT, class AE>
        NUMERICS_INLINE
        matrix_slice &operator = (const matrix_expression<AT, AE> &ae) { 
            matrix<value_type> e (ae);
            assign (e); 
            return *this;
        }
        template<class AT, class AE>
        NUMERICS_INLINE
        matrix_slice& operator += (const matrix_expression<AT, AE> &ae) {
            matrix<value_type> e (*this + ae);
            assign (e); 
            return *this;
        }
        template<class AT, class AE>
        NUMERICS_INLINE
        matrix_slice& operator -= (const matrix_expression<AT, AE> &ae) {
            matrix<value_type> e (*this - ae);
            assign (e); 
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        matrix_slice& operator *= (const AT &at) {
            size_type size1 = s1_.size ();
            size_type size2 = s2_.size ();
#ifndef NUMERICS_USE_ITERATOR
            for (size_type i = 0; i < size1; ++ i)
                for (size_type j = 0; j < size2; ++ j)
                    (*this) (i, j) *= at;
#else // NUMERICS_USE_ITERATOR
            iterator1 it1 (begin1 ());
            for (size_type i = 0; i < size1; ++ i) {
                iterator2 it2 (it1.begin ());
                for (size_type j = 0; j < size2; ++ j)
                    *it2 *= at, ++ it2;
                ++ it1;
            }
#endif // NUMERICS_USE_ITERATOR
            return *this;
        }

#ifdef NUMERICS_USE_ITERATOR
        class const_iterator2;

        class const_iterator1 {
        public:
            NUMERICS_INLINE
            const_iterator1 (matrix_type &m, const const_iterator_type &it1, const const_iterator_type &it2):
                m_ (m), it1_ (it1), it2_ (it2) {}

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
                return m_ (*it1_, *it2_); 
            }

            NUMERICS_INLINE
            const_iterator2 begin () const {
                return const_iterator2 (m_, it1_, it2_); 
            }
            NUMERICS_INLINE
            const_iterator2 end () const {
                return const_iterator2 (m_, it1_, it2_ + m_.size2 ()); 
            }

            NUMERICS_INLINE
            friend bool operator == (const const_iterator1 &it1, const const_iterator1 &it2) {
                return &it1.m_ == &it2.m_ && it1.it1_ == it2.it1_ && it1.it2_ == it2.it2_;
            }
            NUMERICS_INLINE
            friend bool operator != (const const_iterator1 &it1, const const_iterator1 &it2) {
                return ! (it1 == it2);
            }

        protected:
            matrix_type &m_;
            const_iterator_type it1_;
            const_iterator_type it2_;
        };

        NUMERICS_INLINE
        const_iterator1 begin1 () const {
            return const_iterator1 (data_, s1_.begin (), s2_.begin ()); 
        }
        NUMERICS_INLINE
        const_iterator1 end1 () const {
            return const_iterator1 (data_, s1_.end (), s2_.begin ()); 
        }

        class iterator2;

        class iterator1:
            public const_iterator1 {
        public:
            NUMERICS_INLINE
            iterator1 (matrix_type &m, const const_iterator_type &it1, const const_iterator_type &it2):
                const_iterator1 (m, it1, it2) {}

            NUMERICS_INLINE
            reference_type operator * () {
                return m_ (*it1_, *it2_); 
            }

            NUMERICS_INLINE
            iterator2 begin () {
                return iterator2 (m_, it1_, it2_); 
            }
            NUMERICS_INLINE
            iterator2 end () {
                return iterator2 (m_, it1_, it2_ + m_.size2 ()); 
            }
        };

        NUMERICS_INLINE
        iterator1 begin1 () {
            return iterator1 (data_, s1_.begin (), s2_.begin ()); 
        }
        NUMERICS_INLINE
        iterator1 end1 () {
            return iterator1 (data_, s1_.end (), s2_.begin ()); 
        }

        class const_iterator2 {
        public:
            NUMERICS_INLINE
            const_iterator2 (matrix_type &m, const const_iterator_type &it1, const const_iterator_type &it2):
                m_ (m), it1_ (it1), it2_ (it2) {}

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
                return m_ (*it1_, *it2_); 
            }

            NUMERICS_INLINE
            const_iterator1 begin () const {
                return const_iterator1 (m_, it1_, it2_); 
            }
            NUMERICS_INLINE
            const_iterator1 end () const {
                return const_iterator1 (m_, it1_ + m_.size1 (), it2_); 
            }

            NUMERICS_INLINE
            friend bool operator == (const const_iterator2 &it1, const const_iterator2 &it2) {
                return &it1.m_ == &it2.m_ && it1.it1_ == it2.it1_ && it1.it2_ == it2.it2_;
            }
            NUMERICS_INLINE
            friend bool operator != (const const_iterator2 &it1, const const_iterator2 &it2) {
                return ! (it1 == it2);
            }

        protected:
            matrix_type &m_;
            const_iterator_type it1_;
            const_iterator_type it2_;
        };

        NUMERICS_INLINE
        const_iterator2 begin2 () const {
            return const_iterator2 (data_, s1_.begin (), s2_.begin ()); 
        }
        NUMERICS_INLINE
        const_iterator2 end2 () const {
            return const_iterator2 (data_, s1_.begin (), s2_.end ()); 
        }

        class iterator2:
            public const_iterator2 {
        public:
            NUMERICS_INLINE
            iterator2 (matrix_type &m, const const_iterator_type &it1, const const_iterator_type &it2):
                const_iterator2 (m, it1, it2) {}

            NUMERICS_INLINE
            reference_type operator * () {
                return m_ (*it1_, *it2_); 
            }

            NUMERICS_INLINE
            iterator1 begin () {
                return iterator1 (m_, it1_, it2_); 
            }
            NUMERICS_INLINE
            iterator1 end () {
                return iterator1 (m_, it1_ + m_.size1 (), it2_); 
            }
        };

        NUMERICS_INLINE
        iterator2 begin2 () {
            return iterator1 (data_, s1_.begin (), s2_.begin ()); 
        }
        NUMERICS_INLINE
        iterator2 end2 () {
            return iterator1 (data_, s1_.begin (), s2_.end ()); 
        }
#endif // NUMERICS_USE_ITERATOR

    private:
        matrix_type &data_;
        slice s1_;
        slice s2_;

        template<class AT, class AE>
        NUMERICS_INLINE
        void assign (const matrix_expression<AT, AE> &ae) {
            size_type size1 = common (this->size1 (), ae.size1 ());
            size_type size2 = common (this->size2 (), ae.size2 ());
#ifdef NUMERICS_USE_ITERATOR
            for (size_type i = 0; i < size1; ++ i)
                for (size_type j = 0; j < size2; ++ j)
                    (*this ) (i, j) = ae (i, j);
#else // NUMERICS_USE_ITERATOR
            iterator1 it1 (begin1 ());
            AE::const_iterator1 it1ae (ae.expression ().begin1 ());
            for (size_type i = 0; i < size1; ++ i) {
                iterator2 it2 (it1.begin ());
                AE::const_iterator2 it2ae (it1ae.begin ());
                for (size_type j = 0; j < size2; ++ j)
                    *it2 = *it2ae, ++ it2, ++ it2ae;
                ++ it1, ++ it1ae;
            }
#endif // NUMERICS_USE_ITERATOR
        }
    };

    // Array based band matrix class 
    template<class T, class A, class F>
    class band_matrix: 
        public matrix_expression<T, band_matrix<T, A, F> > {
    public:      
        typedef unsigned size_type;
        typedef T value_type;
        typedef T &reference_type;
        typedef T *pointer_type;
        typedef const A const_array_type;
        typedef A array_type;
        typedef F functor_type;
        typedef typename F::functor1_type functor1_type;
        typedef typename F::functor2_type functor2_type;
// #ifndef USE_MSVC
        typedef matrix_vector_slice<band_matrix<T, A, F> > matrix_vector_slice_type;
// #endif

        NUMERICS_EXPLICIT
        NUMERICS_INLINE
        band_matrix (): 
            size1_ (0), size2_ (0),
            lower_ (0), upper_ (0), data_ (0) {}
        NUMERICS_EXPLICIT
        NUMERICS_INLINE
        band_matrix (size_type size1, size_type size2, size_type lower, size_type upper): 
            size1_ (size1), size2_ (size2),
            lower_ (lower), upper_ (upper),
            data_ (std::min (size1, size2) * (lower + 1 + upper)) {}
        NUMERICS_EXPLICIT
        NUMERICS_INLINE
        band_matrix (const band_matrix &m): 
            size1_ (m.size1 ()), size2_ (m.size2 ()),       
            lower_ (m.lower ()), upper_ (m.upper ()),
            data_ (std::min (m.size1 (), m.size2 ()) * (m.lower () + 1 + m.upper ())) {
            safe_copy (data_, m.data_);
        }
        template<class AT, class AE>
        NUMERICS_EXPLICIT
        NUMERICS_INLINE
        band_matrix (const matrix_expression<AT, AE> &ae, size_type lower, size_type upper): 
            size1_ (ae.size1 ()), size2_ (ae.size2 ()),
            lower_ (lower), upper_ (upper),
            data_ (std::min (ae.size1 (), ae.size2 ()) * (lower + 1 + upper)) {
            assign (ae); 
        }
        NUMERICS_INLINE
        virtual ~band_matrix () {}

        NUMERICS_INLINE
        void resize (size_type size1, size_type size2, size_type lower, size_type upper) {
            size1_ = size1;
            size2_ = size2;
            lower_ = lower;
            upper_ = upper;
            data_.resize (std::min (size1, size2) * (lower + 1 + upper));
        }

        NUMERICS_INLINE
        array_type &data () {
            return data_;
        }
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
        value_type operator () (size_type i, size_type j) const {
            check<bad_index>::precondition (i < size1 ());
            check<bad_index>::precondition (j < size2 ());
            size_type k = std::min (i, j);
            size_type l = lower_ + j - i;
            if (k < std::min (size1 (), size2 ()) &&
                l < lower_ + 1 + upper_)
                return data_ [functor_type::element (k, std::min (size1 (), size2 ()),
                                                     l, lower_ + 1 + upper_)]; 
            return 0;
        }
        NUMERICS_INLINE
        reference_type operator () (size_type i, size_type j) {
            check<bad_index>::precondition (i < size1 ());
            check<bad_index>::precondition (j < size2 ());
            size_type k = std::min (i, j);
            size_type l = lower_ + j - i;
            return data_ [functor_type::element (k, std::min (size1 (), size2 ()), 
                                                 l, lower_ + 1 + upper_)]; 
        }

        NUMERICS_INLINE
// #ifdef USE_MSVC
//         matrix_vector_slice<band_matrix> operator [] (size_type i) {
// #else
        matrix_vector_slice_type operator [] (size_type i) {
// #endif
            return matrix_vector_slice<band_matrix> (*this, slice (i, 0, size2 ()), slice (0, 1, size2 ()));
        }
        NUMERICS_INLINE
// #ifdef USE_MSVC
//         matrix_vector_slice<band_matrix> row (size_type i) {
// #else
        matrix_vector_slice_type row (size_type i) {
// #endif
            return matrix_vector_slice<band_matrix> (*this, slice (i, 0, size2 ()), slice (0, 1, size2 ()));
        }
        NUMERICS_INLINE
// #ifdef USE_MSVC
//         matrix_vector_slice<band_matrix> column (size_type j) {
// #else
        matrix_vector_slice_type column (size_type j) {
// #endif
            return matrix_vector_slice<band_matrix> (*this, slice (0, 1, size1 ()), slice (j, 0, size2 ()));
        }

        NUMERICS_INLINE
        band_matrix &operator = (const band_matrix &m) { 
            safe_copy (data_, m.data_);
            return *this;
        }
        template<class AT, class AE>
        NUMERICS_INLINE
        band_matrix &operator ^= (const matrix_expression<AT, AE> &ae) { 
            assign (ae); 
            return *this;
        }
        template<class AT, class AE>
        NUMERICS_INLINE
        band_matrix &operator = (const matrix_expression<AT, AE> &ae) { 
            band_matrix e (ae, lower_, upper_);
            fast_copy (data_, e.data_);
            return *this;
        }
        template<class AT, class AE>
        NUMERICS_INLINE
        band_matrix& operator += (const matrix_expression<AT, AE> &ae) {
            band_matrix e (*this + ae, lower_, upper_);
            fast_copy (data_, e.data_);
            return *this;
        }
        template<class AT, class AE>
        NUMERICS_INLINE
        band_matrix& operator -= (const matrix_expression<AT, AE> &ae) {
            band_matrix e (*this - ae, lower_, upper_);
            fast_copy (data_, e.data_);
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        band_matrix& operator *= (const AT &at) {
            size_type size1 = this->size1 ();
            size_type size2 = this->size2 ();
            for (size_type i = 0; i < size1; ++ i) {
                for (size_type j = 0; j < size2; ++ j) {
                    size_type k = std::min (i, j);
                    size_type l = lower_ + j - i;
                    if (k < std::min (size1 (), size2 ()) &&
                        l < lower_ + 1 + upper_)
//                        (*this ) (i, j) *= at;
                          data_ [functor_type::element (k, std::min (size1 (), size2 ()),
                                                        l, lower_ + 1 + upper_)] *= at;
                }
            }
            return *this;
        }

    private:
        size_type size1_;
        size_type size2_;
        size_type lower_;
        size_type upper_;
        array_type data_;

        template<class AT, class AE>
        NUMERICS_INLINE
        void assign (const matrix_expression<AT, AE> &ae) {
            size_type size1 = common (this->size1 (), ae.size1 ());
            size_type size2 = common (this->size2 (), ae.size2 ());
            for (size_type i = 0; i < size1; ++ i) {
                for (size_type j = 0; j < size2; ++ j) {
                    size_type k = std::min (i, j);
                    size_type l = lower_ + j - i;
                    if (k < std::min (size1 (), size2 ()) &&
                        l < lower_ + 1 + upper_)
//                        (*this) (i, j) = ae (i, j);
                        data_ [functor_type::element (k, std::min (size1 (), size2 ()),
                                                      l, lower_ + 1 + upper_)] = ae (i, j);
                    else 
                        check<non_band>::precondition (ae (i, j) != AT (0));                            
                }
            }
        }
    };

}

#endif // NUMERICS_MATRIX_H 