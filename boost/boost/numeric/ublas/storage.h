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

#ifndef NUMERICS_STORAGE_H
#define NUMERICS_STORAGE_H

#include <valarray>
#include <vector>

#include "config.h"
#include "exception.h"

namespace numerics {

#ifndef NUMERICS_USE_INSTANT
    NUMERICS_INLINE
    unsigned common (size_type size1, size_type size2) {
#ifndef NUMERICS_FAST_COMMON
        check<bad_size>::precondition (size1 == size2);
        return std::min (size1, size2);
#else // NUMERICS_FAST_COMMON
        return size1;
#endif // NUMERICS_FAST_COMMON
    }
#else // NUMERICS_USE_INSTANT
    NUMERICS_INLINE
    unsigned common (size_type size1, size_type size2);
#endif // NUMERICS_USE_INSTANT

    // Unbounded array 
    template<class T>
    class unbounded_array {
    public:      
        typedef unsigned size_type;
        typedef T value_type;
        typedef T &reference_type;
        typedef T *pointer_type;
        typedef T *&pointer_reference_type;

        NUMERICS_EXPLICIT
        NUMERICS_INLINE
        unbounded_array (): 
            size_ (0), data_ (new value_type [size]) { 
            if (! data_)
                throw std::bad_alloc ();
        }
        NUMERICS_EXPLICIT
        NUMERICS_INLINE
        unbounded_array (size_type size): 
            size_ (size), data_ (new value_type [size]) { 
            if (! data_)
                throw std::bad_alloc ();
        }
        NUMERICS_EXPLICIT
        NUMERICS_INLINE
        unbounded_array (const unbounded_array &a): 
            size_ (a.size_), data_ (new value_type [a.size_]) { 
            if (! data_)
                throw std::bad_alloc ();
            safe_copy (*this, a);
        }
        NUMERICS_INLINE
        ~unbounded_array () { 
            if (! data_)
                throw std::bad_alloc ();
            delete [] data_; 
        }

        NUMERICS_INLINE
        void resize (size_type size) {
            if (size != size_) {
                pointer_type data = new value_type [size];
                if (! data)
                    throw std::bad_alloc ();
                if (! data_)
                    throw std::bad_alloc ();
                delete data_;
                size_ = size;
                data_ = data;
            }
        }

        NUMERICS_INLINE
        pointer_type data () const {
            return data_;
        }
        NUMERICS_INLINE
        pointer_reference_type data () {
            return data_;
        }
        NUMERICS_INLINE
        size_type size () const { 
            return size_; 
        }

        NUMERICS_INLINE
        value_type operator [] (size_type i) const {
            check<bad_index>::precondition (i < size_);
            return data_ [i];
        }
        NUMERICS_INLINE
        reference_type operator [] (size_type i) {
            check<bad_index>::precondition (i < size_);
            return data_ [i];
        }

#ifdef NUMERICS_USE_ITERATOR
        class const_iterator {
        public:
            NUMERICS_INLINE
            const_iterator (pointer_type p):
                p_ (p) {}

            NUMERICS_INLINE
            const_iterator operator ++ () {
                return ++ p_;
            }
            NUMERICS_INLINE
            const_iterator operator -- () {
                return -- p_;
            }
            NUMERICS_INLINE
            value_type operator * () const {
                return *p_;
            }

            NUMERICS_INLINE
            friend bool operator == (const const_iterator &it1, const const_iterator &it2) {
                return it1.p_ == it2.p_;
            }
            NUMERICS_INLINE
            friend bool operator != (const const_iterator &it1, const const_iterator &it2) {
                return it1.p_ != it2.p_;
            }

        protected:
            pointer_type p_;
        };

        NUMERICS_INLINE
        const_iterator begin () const {
            return data_;
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return data_ + size_;
        }

        class iterator: 
            public const_iterator {
        public:
            NUMERICS_INLINE
            iterator (pointer_type p):
                const_iterator (p) {}

            NUMERICS_INLINE
            reference_type operator * () {
                return *p_;
            }
        };

        NUMERICS_INLINE
        iterator begin () {
            return data_;
        }
        NUMERICS_INLINE
        iterator end () {
            return data_ + size_;
        }
#endif // NUMERICS_USE_ITERATOR

    private:
        size_type size_;
        pointer_type data_;
    };

    template<class T>
    NUMERICS_INLINE
    unbounded_array<T> &safe_copy (unbounded_array<T> &a1, const unbounded_array<T> &a2) {
        if (&a1 == &a2) 
            throw external_logic ();
        size_type size (common (a1.size (), a2.size ()));
#ifndef NUMERICS_USE_ITERATOR
        T *data1 = a1.data ();
        T *data2 = a2.data ();
        for (size_type i = 0; i < size; ++ i) 
            data1 [i] = data2 [i];
#else // NUMERICS_USE_ITERATOR
        unbounded_array<T>::iterator it1 (a1.begin ());
        unbounded_array<T>::const_iterator it2 (a2.begin ());
        for (size_type i = 0; i < size; ++ i) 
            *it1 = *it2, ++ it1, ++ it2;
#endif // NUMERICS_USE_ITERATOR
        return a1;
    }
    template<class T>
    NUMERICS_INLINE
    unbounded_array<T> &fast_copy (unbounded_array<T> &a1, unbounded_array<T> &a2) {
        if (&a1 == &a2) 
            throw external_logic ();
        check<bad_size>::precondition (a1.size () == a2.size ());
        T *&data1 = a1.data ();
        T *&data2 = a2.data ();
        std::swap (data1, data2);
        return a1;
    }
    template<class T>
    NUMERICS_INLINE
    void safe_swap (unbounded_array<T> &a1, unbounded_array<T> &a2) {
        if (&a1 == &a2) 
            throw external_logic ();
        size_type size (common (a1.size (), a2.size ()));
#ifndef NUMERICS_USE_ITERATOR
        T *data1 = a1.data ();
        T *data2 = a2.data ();
        for (size_type i = 0; i < size; ++ i) 
            std::swap (data1 [i], data2 [i]);
#else // NUMERICS_USE_ITERATOR
        unbounded_array<T>::iterator it1 (a1.begin ());
        unbounded_array<T>::iterator it2 (a2.begin ());
        for (size_type i = 0; i < size; ++ i) {
            T t = *it1; 
            *it1 = *it2; 
            *it2 = t; 
            ++ it1, ++ it2;
        }
#endif // NUMERICS_USE_ITERATOR
    }       
    template<class T>
    NUMERICS_INLINE
    void fast_swap (unbounded_array<T> &a1, unbounded_array<T> &a2) {
        if (&a1 == &a2) 
            throw external_logic ();
        check<bad_size>::precondition (a1.size () == a2.size ());
        T *&data1 = a1.data ();
        T *&data2 = a2.data ();
        std::swap (data1, data2);
    }

    // Bounded array 
    template<class T, size_type N>
    class bounded_array {
    public:      
        typedef unsigned size_type;
        typedef T value_type;
        typedef T &reference_type;
        typedef T *pointer_type;

        NUMERICS_EXPLICIT
        NUMERICS_INLINE
        bounded_array (): 
            size_ (N), data_ () {}
        NUMERICS_EXPLICIT
        NUMERICS_INLINE
        bounded_array (size_type size): 
            size_ (size), data_ () {
            if (size_ > N) 
                throw std::bad_alloc ();
        }
        NUMERICS_EXPLICIT
        NUMERICS_INLINE
        bounded_array (const bounded_array &a): 
            size_ (a.size_), data_ () { 
            if (size_ > N) 
                throw std::bad_alloc ();
            safe_copy (*this, a);
        }
        ~bounded_array () {}

        NUMERICS_INLINE
        void resize (size_type size) {
            size_ = size;
            if (size_ > N) 
                throw std::bad_alloc ();
        }

        NUMERICS_INLINE
        pointer_type data () const {
            return const_cast<pointer_type> (data_);
        }
        NUMERICS_INLINE
        size_type size () const { 
            return size_; 
        }

        NUMERICS_INLINE
        value_type operator [] (size_type i) const {
            check<bad_index>::precondition (i < size_);
            return data_ [i];
        }
        NUMERICS_INLINE
        reference_type operator [] (size_type i) {
            check<bad_index>::precondition (i < size_);
            return data_ [i];
        }

#ifdef NUMERICS_USE_ITERATOR
        class const_iterator {
        public:
            NUMERICS_INLINE
            const_iterator (pointer_type p):
                p_ (p) {}

            NUMERICS_INLINE
            const_iterator operator ++ () {
                return ++ p_;
            }
            NUMERICS_INLINE
            const_iterator operator -- () {
                return -- p_;
            }
            NUMERICS_INLINE
            value_type operator * () const {
                return *p_;
            }

            NUMERICS_INLINE
            friend bool operator == (const const_iterator &it1, const const_iterator &it2) {
                return it1.p_ == it2.p_;
            }
            NUMERICS_INLINE
            friend bool operator != (const const_iterator &it1, const const_iterator &it2) {
                return it1.p_ != it2.p_;
            }

        protected:
            pointer_type p_;
        };

        NUMERICS_INLINE
        const_iterator begin () const {
            return const_cast<pointer_type> (data_);
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return const_cast<pointer_type> (data_) + size_;
        }

        class iterator:
            public const_iterator {
        public:
            NUMERICS_INLINE
            iterator (pointer_type p):
                const_iterator (p) {}

            NUMERICS_INLINE
            reference_type operator * () {
                return *p_;
            }
        };

        NUMERICS_INLINE
        iterator begin () {
            return data_;
        }
        NUMERICS_INLINE
        iterator end () {
            return data_ + size_;
        }
#endif // NUMERICS_USE_ITERATOR

    private:
        size_type size_;
        value_type data_ [N];
    };

    template<class T, size_type N>
    NUMERICS_INLINE
    bounded_array<T, N> &safe_copy (bounded_array<T, N> &a1, const bounded_array<T, N> &a2) {
        if (&a1 == &a2) 
            throw external_logic ();
        size_type size (common (a1.size (), a2.size ()));
#ifndef NUMERICS_USE_ITERATOR
        T *data1 = a1.data ();
        T *data2 = a2.data ();
        for (size_type i = 0; i < size; ++ i) 
            data1 [i] = data2 [i];
#else // NUMERICS_USE_ITERATOR
        bounded_array<T, N>::iterator it1 (a1.begin ());
        bounded_array<T, N>::const_iterator it2 (a2.begin ());
        for (size_type i = 0; i < size; ++ i) 
            *it1 = *it2, ++ it1, ++ it2;
#endif // NUMERICS_USE_ITERATOR
        return a1;
    }
    template<class T, size_type N>
    NUMERICS_INLINE
    bounded_array<T, N>  &fast_copy (bounded_array<T, N>  &a1, bounded_array<T, N>  &a2) {
        return safe_copy (a1, a2);
    }
    template<class T, size_type N>
    NUMERICS_INLINE
    void safe_swap (bounded_array<T, N>  &a1, bounded_array<T, N>  &a2) {
        if (&a1 == &a2) 
            throw external_logic ();
        size_type size (common (a1.size (), a2.size ()));
#ifndef NUMERICS_USE_ITERATOR
        T *data1 = a1.data ();
        T *data2 = a2.data ();
        for (size_type i = 0; i < size; ++ i) 
            std::swap (data1 [i], data2 [i]);
#else // NUMERICS_USE_ITERATOR
        bounded_array<T, N>::iterator it1 (a1.begin ());
        bounded_array<T, N>::iterator it2 (a2.begin ());
        for (size_type i = 0; i < size; ++ i) {
            T t = *it1;
            *it1 = *it2;
            *it2 = t;
            ++ it1, ++ it2;
        }
#endif // NUMERICS_USE_ITERATOR
    }
    template<class T, size_type N>
    NUMERICS_INLINE
    void fast_swap (bounded_array<T, N>  &a1, bounded_array<T, N>  &a2) {
        safe_swap (a1, a2);
    }

    template<class T>
    NUMERICS_INLINE
    std::valarray<T> &safe_copy (std::valarray<T> &a1, const std::valarray<T> &a2) {
        if (&a1 == &a2) 
            throw external_logic ();
        return a1 = a2;
    }
    template<class T>
    NUMERICS_INLINE
    std::valarray<T> &fast_copy (std::valarray<T> &a1, std::valarray<T> &a2) {
        if (&a1 == &a2) 
            throw external_logic ();
        return a1 = a2;
    }
    template<class T>
    NUMERICS_INLINE
    void safe_swap (std::valarray<T> &a1, std::valarray<T> &a2) {
        if (&a1 == &a2) 
            throw external_logic ();
        std::swap (a1, a2);
    }
    template<class T>
    NUMERICS_INLINE
    void fast_swap (std::valarray<T> &a1, std::valarray<T> &a2) {
        if (&a1 == &a2) 
            throw external_logic ();
        std::swap (a1, a2);
    }

    template<class T>
    NUMERICS_INLINE
    std::vector<T> &safe_copy (std::vector<T> &a1, const std::vector<T> &a2) {
        if (&a1 == &a2) 
            throw external_logic ();
        return a1 = a2;
    }
    template<class T>
    NUMERICS_INLINE
    std::vector<T> &fast_copy (std::vector<T> &a1, std::vector<T> &a2) {
        if (&a1 == &a2) 
            throw external_logic ();
        return a1 = a2;
    }
    template<class T>
    NUMERICS_INLINE
    void safe_swap (std::vector<T> &a1, std::vector<T> &a2) {
        if (&a1 == &a2) 
            throw external_logic ();
        std::swap (a1, a2);
    }
    template<class T>
    NUMERICS_INLINE
    void fast_swap (std::vector<T> &a1, std::vector<T> &a2) {
        if (&a1 == &a2) 
            throw external_logic ();
        std::swap (a1, a2);
    }

    // Slice class
    class slice {
    public:
        typedef unsigned size_type;

        NUMERICS_INLINE
        slice (size_type start, size_type stride, size_type size): 
            start_ (start), stride_ (stride), size_ (size) {}
        NUMERICS_INLINE
        virtual ~slice () {}

        NUMERICS_INLINE
        size_type start () const { 
            return start_; 
        }
        NUMERICS_INLINE
        size_type stride () const { 
            return stride_; 
        }
        NUMERICS_INLINE
        size_type size () const { 
            return size_; 
        }

        NUMERICS_INLINE
        size_type operator () (size_type i) const { 
            check<bad_index>::precondition (i < size_);
            return start_ + i * stride_; 
        }

        NUMERICS_INLINE
        friend bool operator == (const slice &s1, const slice &s2) {
            return s1.start_ == s2.start_ && s1.stride_ == s2.stride_ && s1.size_ == s2.size_; 
        }
        NUMERICS_INLINE
        friend bool operator != (const slice &s1, const slice &s2) {
            return s1.start_ != s2.start_ || s1.stride_ != s2.stride_ || s1.size_ != s2.size_; 
        }

#ifdef NUMERICS_USE_ITERATOR
        class const_iterator {
        public:
            NUMERICS_INLINE
            const_iterator (const slice &s, size_type i): 
                s_ (s), i_ (i) {}

            NUMERICS_INLINE
            const_iterator &operator ++ () {
                i_ += s_.stride ();
                return *this;
            }
            NUMERICS_INLINE
            const_iterator &operator -- () {
                i_ -= s_.stride ();
                return *this;
            }
            NUMERICS_INLINE
            size_type operator * () const {
                // Stride may be 0!
                check<bad_index>::precondition (i_ <= s_.start () + (s_.size () - 1) * s_.stride ());
                return i_; 
            }

            NUMERICS_INLINE
            friend bool operator == (const const_iterator &it1, const const_iterator &it2) {
                return it1.s_ == it2.s_ && it1.i_ == it2.i_; 
            }
            NUMERICS_INLINE
            friend bool operator != (const const_iterator &it1, const const_iterator &it2) {
                return it1.s_ != it2.s_ || it1.i_ != it2.i_; 
            }

        protected:
            const slice &s_;
            size_type i_;
        };

        NUMERICS_INLINE
        const_iterator begin () const {
            return const_iterator (*this, start_); 
        }
        NUMERICS_INLINE
        const_iterator end () const {
            // Stride may be 0!
            return const_iterator (*this, start_ + (size_ - 1) * stride_ + 1); 
        }
#endif // NUMERICS_USE_ITERATOR

    private:
        size_type start_;
        size_type stride_;
        size_type size_;
    };

}

#endif // NUMERICS_STORAGE_H