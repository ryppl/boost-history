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

#include <algorithm>
#include <valarray>
#include <vector>

#include "config.h"
#include "exception.h"
#include "iterator.h"

namespace numerics {

    template<class T>
    NUMERICS_INLINE
    const T &common (const T &size1, const T &size2) {
#ifndef NUMERICS_FAST_COMMON
        check<bad_size>::precondition (size1 == size2);
        return std::min (size1, size2);
#else 
        return size1;
#endif 
    }

    // Unbounded array 
    template<class T>
    class unbounded_array {
    public:      
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef const T &const_reference_type;
        typedef T &reference_type;
        typedef const T *const_pointer_type;
        typedef T *pointer_type;

        // Construction and destruction
        NUMERICS_INLINE
        unbounded_array (): 
            size_ (0), data_ (new value_type [0]) { 
            if (! data_)
                throw std::bad_alloc ();
        }
        NUMERICS_EXPLICIT NUMERICS_INLINE
        unbounded_array (size_type size): 
            size_ (size), data_ (new value_type [size]) { 
            if (! data_)
                throw std::bad_alloc ();
        }
        NUMERICS_INLINE
        unbounded_array (const unbounded_array &a): 
            size_ (a.size_), data_ (new value_type [a.size_]) { 
            if (! data_)
                throw std::bad_alloc ();
            *this = a;
        }
        NUMERICS_INLINE
        ~unbounded_array () { 
            if (! data_)
                throw std::bad_alloc ();
            delete [] data_; 
        }

        // Resizing
        NUMERICS_INLINE
        void resize (size_type size) {
            if (size != size_) {
                pointer_type data = new value_type [size];
                if (! data)
                    throw std::bad_alloc ();
                if (! data_)
                    throw std::bad_alloc ();
                // The content of the array is intentionally not copied.
                delete [] data_;
                size_ = size;
                data_ = data;
            }
        }

        NUMERICS_INLINE
        size_type size () const { 
            return size_; 
        }

        // Element access
        NUMERICS_INLINE
        const_reference_type operator [] (size_type i) const {
            check<bad_index>::precondition (i < size_);
            return data_ [i];
        }
        NUMERICS_INLINE
        reference_type operator [] (size_type i) {
            check<bad_index>::precondition (i < size_);
            return data_ [i];
        }

        // Assignment
        NUMERICS_INLINE
        unbounded_array &operator = (const unbounded_array &a) { 
            check<external_logic>::precondition (this != &a);
            check<bad_size>::precondition (size_ == a.size_);
            std::copy (a.data_, a.data_ + a.size_, data_);
            return *this;
        }
        NUMERICS_INLINE
        unbounded_array &assign_temporary (unbounded_array &a) { 
            swap (a);
            return *this;
        }

        // Swapping
        NUMERICS_INLINE
	    void swap (unbounded_array &a) {
            check<external_logic>::precondition (this != &a);
            check<bad_size>::precondition (size_ == a.size_);
            std::swap (size_, a.size_);
            std::swap (data_, a.data_);
        }
#ifndef USE_GCC
        NUMERICS_INLINE
	    friend void swap (unbounded_array &a1, unbounded_array &a2) {
            a1.swap (a2);
        }
#endif

        NUMERICS_INLINE
        void fill (const value_type &t) {
            std::fill (begin (), end (), t);
        }
        NUMERICS_INLINE
        pointer_type insert (pointer_type it, const value_type &t) {
            check<bad_index>::precondition (begin () <= it && it < end ());
            check<bad_index>::precondition (*it == value_type ());
            *it = t;
            return it;
        }

        // Iterators simply are pointers.

        typedef const_pointer_type const_iterator;

        NUMERICS_INLINE
        const_iterator begin () const {
            return data_;
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return data_ + size_;
        }

        typedef pointer_type iterator;

        NUMERICS_INLINE
        iterator begin () {
            return data_;
        }
        NUMERICS_INLINE
        iterator end () {
            return data_ + size_;
        }

        // Reverse iterators

#ifndef USE_GCC

#ifdef USE_MSVC
        typedef std::reverse_iterator<const_iterator, value_type, const_reference_type> const_reverse_iterator;
#else
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
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
        typedef std::reverse_iterator<iterator, value_type, reference_type> reverse_iterator;
#else
        typedef std::reverse_iterator<iterator> reverse_iterator;
#endif

        NUMERICS_INLINE
        reverse_iterator rbegin () {
            return reverse_iterator (end ());
        }
        NUMERICS_INLINE
        reverse_iterator rend () {
            return reverse_iterator (begin ());
        }

#endif

    private:
        size_type size_;
        pointer_type data_;
    };

    template<class T>
    NUMERICS_INLINE
    unbounded_array<T> &assign_temporary (unbounded_array<T> &a1, unbounded_array<T> &a2) { 
        return a1.assign_temporary (a2);
    }

    // Bounded array 
    template<class T, std::size_t N>
    class bounded_array {
    public:      
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef const T &const_reference_type;
        typedef T &reference_type;
        typedef const T *const_pointer_type;
        typedef T *pointer_type;

        // Construction and destruction
        NUMERICS_INLINE
        bounded_array (): 
            size_ (N) /* , data_ () */ {}
        NUMERICS_EXPLICIT NUMERICS_INLINE
        bounded_array (size_type size): 
            size_ (size) /* , data_ () */ {
            if (size_ > N) 
                throw std::bad_alloc ();
        }
        NUMERICS_INLINE
        bounded_array (const bounded_array &a): 
            size_ (a.size_) /* , data_ () */ { 
            if (size_ > N) 
                throw std::bad_alloc ();
            *this = a;
        }

        // Resizing
        NUMERICS_INLINE
        void resize (size_type size) {
            if (size > N) 
                throw std::bad_alloc ();
            // The content of the array is intentionally not copied.
            size_ = size;
        }

        NUMERICS_INLINE
        size_type size () const { 
            return size_; 
        }

        // Element access
        NUMERICS_INLINE
        const_reference_type operator [] (size_type i) const {
            check<bad_index>::precondition (i < size_);
            return data_ [i];
        }
        NUMERICS_INLINE
        reference_type operator [] (size_type i) {
            check<bad_index>::precondition (i < size_);
            return data_ [i];
        }

        // Assignment
        NUMERICS_INLINE
        bounded_array &operator = (const bounded_array &a) { 
            check<external_logic>::precondition (this != &a);
            check<bad_size>::precondition (size_ == a.size_);
            std::copy (a.data_, a.data_ + a.size_, data_);
            return *this;
        }
        NUMERICS_INLINE
        bounded_array &assign_temporary (bounded_array &a) { 
            *this = a;
            return *this;
        }

        // Swapping
        NUMERICS_INLINE
	    void swap (bounded_array &a) {
            check<external_logic>::precondition (this != &a);
            check<bad_size>::precondition (size_ == a.size_);
            std::swap_ranges (data_, data_ + size_, a.data_);
        }
#ifndef USE_GCC
        NUMERICS_INLINE
	    friend void swap (bounded_array &a1, bounded_array &a2) {
            a1.swap (a2);
        }
#endif

        // Element insertion
        NUMERICS_INLINE
        void fill (const value_type &t) {
            std::fill (begin (), end (), t);
        }
        NUMERICS_INLINE
        pointer_type insert (pointer_type it, const value_type &t) {
            check<bad_index>::precondition (begin () <= it && it < end ());
            check<bad_index>::precondition (*it == value_type ());
            *it = t;
            return it;
        }

        // Iterators simply are pointers.

        typedef const_pointer_type const_iterator;

        NUMERICS_INLINE
        const_iterator begin () const {
            return data_;
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return data_ + size_;
        }

        typedef pointer_type iterator;

        NUMERICS_INLINE
        iterator begin () {
            return data_;
        }
        NUMERICS_INLINE
        iterator end () {
            return data_ + size_;
        }

        // Reverse iterators

#ifndef USE_GCC

#ifdef USE_MSVC
        typedef std::reverse_iterator<const_iterator, value_type, const_reference_type> const_reverse_iterator;
#else
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
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
        typedef std::reverse_iterator<iterator, value_type, reference_type> reverse_iterator;
#else
        typedef std::reverse_iterator<iterator> reverse_iterator;
#endif

        NUMERICS_INLINE
        reverse_iterator rbegin () {
            return reverse_iterator (end ());
        }
        NUMERICS_INLINE
        reverse_iterator rend () {
            return reverse_iterator (begin ());
        }

#endif

    private:
        size_type size_;
        value_type data_ [N];
    };

    template<class T, std::size_t N>
    NUMERICS_INLINE
    bounded_array<T, N> &assign_temporary (bounded_array<T, N> &a1, bounded_array<T, N> &a2) { 
        return a1.assign_temporary (a2);
    }

    template<class T>
    NUMERICS_INLINE
    std::valarray<T> &assign_temporary (std::valarray<T> &a1, std::valarray<T> &a2) { 
        check<external_logic>::precondition (&a1 != &a2);
        check<bad_size>::precondition (a1.size () == a2.size ());
        return a1 = a2;
    }

    template<class T>
    NUMERICS_INLINE
    std::vector<T> &assign_temporary (std::vector<T> &a1, std::vector<T> &a2) { 
        check<external_logic>::precondition (&a1 != &a2);
        check<bad_size>::precondition (a1.size () == a2.size ());
        a1.swap (a2);
        return a1;
    }

    // Range class
    class range {
    public:
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef difference_type value_type;
        typedef size_type const_iterator_type;

        // Construction and destruction
        NUMERICS_INLINE
        range (size_type start, size_type stop): 
            start_ (start), size_ (stop - start) {
            check<bad_size>::precondition (start <= stop);
        }

        NUMERICS_INLINE
        size_type start () const { 
            return start_; 
        }
        NUMERICS_INLINE
        size_type size () const { 
            return size_; 
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i) const { 
            check<bad_index>::precondition (i < size_);
            return start_ + i; 
        }

        // Composition
        NUMERICS_INLINE
        range composite (const range &r) const {
            check<bad_size>::precondition (start_ + r.size_ <= size_);
            return range (start_ + r.start_, start_ + r.start_ + r.size_);
        }

        // Comparison
        NUMERICS_INLINE
        bool operator == (const range &r) const {
            return start_ == r.start_ && size_ == r.size_; 
        }
        NUMERICS_INLINE
        bool operator != (const range &r) const {
            return ! (*this == r); 
        }

        // Iterator simply is a index.

        class const_iterator:
            public container_const_reference<range>,
            public random_access_iterator_base<const_iterator, value_type> {
        public:

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator (const range &r, const const_iterator_type &it): 
                container_const_reference<range> (r), it_ (it) {}

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
                check<bad_index>::precondition ((*this) ().start () <= it_);
                check<bad_index>::precondition (it_ < (*this) ().start () + (*this) ().size ());
                return it_; 
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                return it_ - (*this) ().start ();
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
            return const_iterator (*this, start_);
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return const_iterator (*this, start_ + size_); 
        }

        // Reverse iterator

#ifndef USE_GCC

#ifdef USE_MSVC
        typedef std::reverse_iterator<const_iterator, value_type, value_type> const_reverse_iterator;
#else
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
#endif

        NUMERICS_INLINE
        const_reverse_iterator rbegin () const {
            return const_reverse_iterator (end ());
        }
        NUMERICS_INLINE
        const_reverse_iterator rend () const {
            return const_reverse_iterator (begin ());
        }

#endif

    private:
        size_type start_;
        size_type size_;
    };

    // Slice class
    class slice {
    public:
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef difference_type value_type;
        typedef size_type const_iterator_type;

        // Construction and destruction
        NUMERICS_INLINE
        slice (size_type start, size_type stride, size_type size): 
            start_ (start), stride_ (stride), size_ (size) {}

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

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i) const { 
            check<bad_index>::precondition (i < size_);
            return start_ + i * stride_; 
        }

        // Composition
        NUMERICS_INLINE
        slice composite (const range &r) const {
            check<bad_size>::precondition (r.start () + r.size () <= size_);
            return slice (start_ + stride_ * r.start (), stride_, r.size ());
        }
        NUMERICS_INLINE
        slice composite (const slice &s) const {
            check<bad_size>::precondition (s.start_ + s.stride_ * s.size_ <= size_);
            return slice (start_ + stride_ * s.start_, stride_ * s.stride_, s.size_);
        }

        // Comparison
        NUMERICS_INLINE
        bool operator == (const slice &s) const {
            return start_ == s.start_ && stride_ == s.stride_ && size_ == s.size_; 
        }
        NUMERICS_INLINE
        bool operator != (const slice &s) const {
            return ! (*this == s); 
        }

        // Iterator simply is a index.

        class const_iterator:
            public container_const_reference<slice>,
            public random_access_iterator_base<const_iterator, value_type> {
        public:

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator (const slice &s, const const_iterator_type &it): 
                container_const_reference<slice> (s), it_ (it) {}

            // Arithmetic
            NUMERICS_INLINE
            const_iterator &operator ++ () {
                it_ += (*this) ().stride ();
                return *this;
            }
            NUMERICS_INLINE
            const_iterator &operator -- () {
                it_ -= (*this) ().stride ();
                return *this;
            }
            NUMERICS_INLINE
            const_iterator &operator += (difference_type n) {
                it_ += n * (*this) ().stride ();
                return *this;
            }
            NUMERICS_INLINE
            const_iterator &operator -= (difference_type n) {
                it_ -= n * (*this) ().stride ();
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const const_iterator &it) const {
                check<divide_by_zero>::precondition ((*this) ().stride () != 0);
                return (it_ - it.it_) / (*this) ().stride ();
            }

            // Dereference
            NUMERICS_INLINE
            value_type operator * () const {
                check<bad_index>::precondition ((*this) ().start () <= it_);
                check<bad_index>::precondition (it_ < (*this) ().start () + (*this) ().stride () * (*this) ().size ());
                return it_; 
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                check<divide_by_zero>::precondition ((*this) ().stride () != 0);
                return (it_ - (*this) ().start ()) / (*this) ().stride ();
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
            return const_iterator (*this, start_); 
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return const_iterator (*this, start_ + stride_ * size_); 
        }

        // Reverse iterator

#ifndef USE_GCC

#ifdef USE_MSVC
        typedef std::reverse_iterator<const_iterator, value_type, value_type> const_reverse_iterator;
#else
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
#endif

        NUMERICS_INLINE
        const_reverse_iterator rbegin () const {
            return const_reverse_iterator (end ());
        }
        NUMERICS_INLINE
        const_reverse_iterator rend () const {
            return const_reverse_iterator (begin ());
        }

#endif

    private:
        size_type start_;
        size_type stride_;
        size_type size_;
    };

}

#endif 


