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

#ifndef NUMERICS_VECTOR_H
#define NUMERICS_VECTOR_H

#include "config.h"
#include "storage.h"
#include "vector_et.h"

namespace numerics {

    struct forward {
        typedef unsigned size_type;

        static 
        NUMERICS_INLINE
        size_type element (size_type i, size_type size) {
            check<bad_index>::precondition (i < size);
            return i;
        }
        static 
        NUMERICS_INLINE
        size_type begin (size_type size) {
            return 0;
        }
        static 
        NUMERICS_INLINE
        size_type end (size_type size) {
            return size;
        }
        static 
        NUMERICS_INLINE
        size_type one () {
            return 1;
        }
    };
    struct backward {
        typedef unsigned size_type;

        static 
        NUMERICS_INLINE
        size_type element (size_type i, size_type size) {
            check<bad_index>::precondition (i < size);
            return size - 1 - i;
        }
        static 
        NUMERICS_INLINE
        size_type begin (size_type size) {
            return size - 1;
        }
        static 
        NUMERICS_INLINE
        size_type end (size_type size) {
            return size_type (-1);
        }
        static 
        NUMERICS_INLINE
        size_type one () {
            return size_type (-1);
        }
    };

    // Array based vector class 
    template<class T, class A, class F>
    class vector: 
        public vector_expression<T, vector<T, A, F> > {
    public:      
        typedef unsigned size_type;
        typedef T value_type;
        typedef T &reference_type;
        typedef T *pointer_type;
        typedef const A const_array_type;
        typedef A array_type;
        typedef F functor_type;

        NUMERICS_EXPLICIT
        NUMERICS_INLINE
        vector (): 
            size_ (0), data_ (0) {}
        NUMERICS_EXPLICIT
        NUMERICS_INLINE
        vector (size_type size): 
            size_ (size), data_ (size) {}
        NUMERICS_EXPLICIT
        NUMERICS_INLINE
        vector (const vector &v): 
            size_ (v.size_), data_ (v.size_) { 
            safe_copy (data_, v.data_);
        }
        template<class AT, class AE>
        NUMERICS_EXPLICIT
        NUMERICS_INLINE
        vector (const vector_expression<AT, AE> &ae): 
            size_ (ae.size ()), data_ (ae.size ()) { 
            assign (ae);
        }
        NUMERICS_INLINE
        virtual ~vector () {}

        NUMERICS_INLINE
        void resize (size_type size) {
            size_ = size;
            data_.resize (size);
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
        size_type size () const { 
            return size_; 
        }

        NUMERICS_INLINE
        value_type operator () (size_type i) const {
            return data_ [functor_type::element (i, size_)]; 
        }
        NUMERICS_INLINE
        reference_type operator () (size_type i) {
            return data_ [functor_type::element (i, size_)]; 
        }

        NUMERICS_INLINE
        value_type operator [] (size_type i) const { 
            return data_ [functor_type::element (i, size_)]; 
        }
        NUMERICS_INLINE
        reference_type operator [] (size_type i) { 
            return data_ [functor_type::element (i, size_)]; 
        }

        NUMERICS_INLINE
        vector &operator = (const vector &v) { 
            safe_copy (data_, v.data_);
            return *this;
        }
        template<class AT, class AE>
        NUMERICS_INLINE
        vector &operator ^= (const vector_expression<AT, AE> &ae) {
            assign (ae);
            return *this;
        }
        template<class AT, class AE>
        NUMERICS_INLINE
        vector &operator = (const vector_expression<AT, AE> &ae) {        
            vector e (ae);
            fast_copy (data_, e.data_);
            return *this;
        }
        template<class AT, class AE>
        NUMERICS_INLINE
        vector &operator += (const vector_expression<AT, AE> &ae) {
            vector e (*this + ae);
            fast_copy (data_, e.data_);
            return *this;
        }
        template<class AT, class AE>
        NUMERICS_INLINE
        vector &operator -= (const vector_expression<AT, AE> &ae) {
            vector e (*this - ae);
            fast_copy (data_, e.data_);
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        vector &operator *= (const AT &at) {
#ifndef NUMERICS_USE_ITERATOR
            for (size_type i = 0; i < size_; ++ i)
                (*this) (i) *= at; 
#else // NUMERICS_USE_ITERATOR
            iterator it (begin ());
            for (size_type i = 0; i < size_; ++ i) 
                *it *= at, ++ it;
#endif // NUMERICS_USE_ITERATOR
            return *this;
        }

#ifdef NUMERICS_USE_ITERATOR
        class const_iterator {
        public:
            NUMERICS_INLINE
            const_iterator (const vector &v, pointer_type it):
                v_ (v), it_ (it) {}

            NUMERICS_INLINE
            const_iterator &operator ++ () {
                it_ += functor_type::one ();
                return *this;
            }
            NUMERICS_INLINE
            const_iterator &operator -- () {
                it_ -= functor_type::one ();
                return *this;
            }
            NUMERICS_INLINE
            value_type operator * () const {
                check<bad_index>::precondition (v_.data ().data () <= it_);
                check<bad_index>::precondition (it_ < v_.data ().data () + v_.size ());
                return *it_;
            }

            NUMERICS_INLINE
            friend bool operator == (const const_iterator &it1, const const_iterator &it2) {
                return &it1.v_ == &it2.v_ && it1.it_ == it2.it_;
            }
            NUMERICS_INLINE
            friend bool operator != (const const_iterator &it1, const const_iterator &it2) {
                return ! (it1 == it2);
            }

        protected:
            const vector &v_;
            pointer_type it_;
        };

        NUMERICS_INLINE
        const_iterator begin () const {
            return const_iterator (*this, data_.data () + functor_type::begin (size_));
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return const_iterator (*this), data_.data () + functor_type::end (size_);
        }

        class iterator: 
            public const_iterator {
        public:
            NUMERICS_INLINE
            iterator (const vector &v, pointer_type it):
                const_iterator (v, it) {}

            NUMERICS_INLINE
            reference_type operator * () {
                check<bad_index>::precondition (v_.data ().data () <= it_);
                check<bad_index>::precondition (it_ < v_.data ().data () + v_.size ());
                return *it_;
            }
        };

        NUMERICS_INLINE
        iterator begin () {
            return iterator (*this, data_.data () + functor_type::begin (size_));
        }
        NUMERICS_INLINE
        iterator end () {
            return iterator (*this, data_.data () + functor_type::end (size_));
        }
#endif // NUMERICS_USE_ITERATOR

    private:
        size_type size_;
        array_type data_;

        template<class AT, class AE>
        NUMERICS_INLINE
        void assign (const vector_expression<AT, AE> &ae) {
            size_type size = common (size_, ae.size ());
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

    template<class T, class A, class F>
    NUMERICS_INLINE
    vector<T, A, F> &safe_copy (vector<T, A, F> &v1, vector<T, A, F> &v2) {
        check<bad_size>::precondition (v1.size () == v2.size ());
        safe_copy (v1.data (), v2.data ());
        return v1;
    }
    template<class T, class A, class F>
    NUMERICS_INLINE
    vector<T, A, F> &fast_copy (vector<T, A, F> &v1, vector<T, A, F> &v2) {
        check<bad_size>::precondition (v1.size () == v2.size ());
        fast_copy (v1.data (), v2.data ());
        return v1;
    }
    template<class T, class A, class F>
    NUMERICS_INLINE
    void safe_swap (vector<T, A, F> &v1, vector<T, A, F> &v2) {
        check<bad_size>::precondition (v1.size () == v2.size ());
        safe_swap (v1.data (), v2.data ());
    }
    template<class T, class A, class F>
    NUMERICS_INLINE
    void fast_swap (vector<T, A, F> &v1, vector<T, A, F> &v2) {
        check<bad_size>::precondition (v1.size () == v2.size ());
        fast_swap (v1.data (), v2.data ());
    }

    // Vector based slice class
    template<class V>
    class vector_slice:
		public vector_expression<typename V::value_type, vector_slice<V> > {
    public:      
        typedef V vector_type;
        typedef typename V::size_type size_type;
        typedef typename V::value_type value_type;
        typedef typename V::reference_type reference_type;
#ifdef NUMERICS_USE_ITERATOR
        typedef slice::const_iterator const_iterator_type;
#endif // NUMERICS_USE_ITERATOR

        NUMERICS_INLINE
        vector_slice (vector_type &data, const slice &s): 
            data_ (data), s_ (s) {}
        virtual ~vector_slice () {}

        NUMERICS_INLINE
        size_type size () const { 
            return s_.size (); 
        }

        NUMERICS_INLINE
        value_type operator () (size_type i) const {
            return data_ (s_ (i)); 
        }
        NUMERICS_INLINE
        reference_type operator () (size_type i) {
            return data_ (s_ (i)); 
        }

        NUMERICS_INLINE
        value_type operator [] (size_type i) const { 
            return data_ (s_ (i)); 
        }
        NUMERICS_INLINE
        reference_type operator [] (size_type i) { 
            return data_ (s_ (i)); 
        }

        NUMERICS_INLINE
        vector_slice &operator ^= (const vector_slice &vs) { 
            assign (vs);
            return *this;
        }
        NUMERICS_INLINE
        vector_slice &operator = (const vector_slice &vs) { 
            vector<value_type> v (vs);
            assign (v);
            return *this;
        }
        template<class AT, class AE>
        NUMERICS_INLINE
        vector_slice &operator ^= (const vector_expression<AT, AE> &ae) {
            assign (ae);
            return *this;
        }
        template<class AT, class AE>
        NUMERICS_INLINE
        vector_slice &operator = (const vector_expression<AT, AE> &ae) {        
            vector<value_type> e (ae);
            assign (e);
            return *this;
        }
        template<class AT, class AE>
        NUMERICS_INLINE
        vector_slice &operator += (const vector_expression<AT, AE> &ae) {
            vector<value_type> e (*this + ae);
            assign (e);
            return *this;
        }
        template<class AT, class AE>
        NUMERICS_INLINE
        vector_slice &operator -= (const vector_expression<AT, AE> &ae) {
            vector<value_type> e (*this - ae);
            assign (e);
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        vector_slice &operator *= (const AT &at) {
            size_type size = s_.size ();
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
            const_iterator (vector_type &v, const const_iterator_type &it):
                v_ (v), it_ (it) {}

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
                return v_ (*it_);
            }

            NUMERICS_INLINE
            friend bool operator == (const const_iterator &it1, const const_iterator &it2) {
                return &it1.v_ == &it2.v_ && it1.it_ == it2.it_;
            }
            NUMERICS_INLINE
            friend bool operator != (const const_iterator &it1, const const_iterator &it2) {
                return ! (it1 == it2);
            }

        protected:
            vector_type &v_;
            const_iterator_type it_;
        };

        NUMERICS_INLINE
        const_iterator begin () const {
            return const_iterator (data_, s_.begin ());
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return const_iterator (data_, s_.end ());
        }

        class iterator: 
            public const_iterator {
        public:
            NUMERICS_INLINE
            iterator (vector_type &v, const slice::const_iterator &it):
                const_iterator (v, it) {}

            NUMERICS_INLINE
            reference_type operator * () {
                return v_ (*it_);
            }
        };

        NUMERICS_INLINE
        iterator begin () {
            return iterator (data_, s_.begin ());
        }
        NUMERICS_INLINE
        iterator end () {
            return iterator (data_, s_.end ());
        }
#endif // NUMERICS_USE_ITERATOR

    private:
        vector_type &data_;
        slice s_;

        template<class AT, class AE>
        NUMERICS_INLINE
        void assign (const vector_expression<AT, AE> &ae) {
            size_type size = common (s_.size (), ae.size ());
#ifndef NUMERICS_USE_ITERATOR
            for (size_type i = 0; i < size; ++ i)
                (*this) (i) = ae (i); 
#else // NUMERICS_USE_ITERATOR
            iterator it (begin ());
            AE::const_iterator itae (ae.expression ().begin ());
            for (size_type i = 0; i < size; ++ i) 
                *it = *itae, ++ it, ++ itae;
#endif // NUMERICS_USE_ITERATOR
            return *this;
        }
    };

}

#endif // NUMERICS_VECTOR_H 