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

#ifndef NUMERICS_STORAGE_H
#define NUMERICS_STORAGE_H

#include <algorithm>
#include <valarray>
#include <vector>

#define NUMERICS_SIMPLE_ARRAY_ADAPTOR
#ifndef NUMERICS_SIMPLE_ARRAY_ADAPTOR
#include <boost/shared_array.hpp>
#endif

#include <boost/numeric/ublas/config.h>
#include <boost/numeric/ublas/exception.h>
#include <boost/numeric/ublas/iterator.h>

namespace boost { namespace numerics {

#ifndef NUMERICS_USE_FAST_COMMON
//    template<class T>
//    NUMERICS_INLINE
//    const T &common_impl (const T &size1, const T &size2) {
//        check (size1 == size2, bad_size ());
//        return std::min (size1, size2);
//    }
// #define common(size1, size2) common_impl((size1), (size2))
    template<class T>
    NUMERICS_INLINE
    const T &common_impl_ex (const T &size1, const T &size2, const char *file, int line) {
        check_ex (size1 == size2, file, line, bad_size ());
        return std::min (size1, size2);
    }
#define common(size1, size2) common_impl_ex ((size1), (size2), __FILE__, __LINE__)
#else
//    template<class T>
//    NUMERICS_INLINE
//    const T &common_impl (const T &size1, const T &size2) {
//        return size1;
//    }
// #define common(size1, size2) common_impl ((size1), (size2))
// FIXME: this one is too agressive for MSVC?!
#define common(size1, size2) (size1)
#endif 

    // Unbounded array 
    template<class T>
    class unbounded_array {
    public:      
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef const T &const_reference;
        typedef T &reference;
        typedef const T *const_pointer;
        typedef T *pointer;

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
                pointer data = new value_type [size];
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
        const_reference operator [] (size_type i) const {
            check (i < size_, bad_index ());
            return data_ [i];
        }
        NUMERICS_INLINE
        reference operator [] (size_type i) {
            check (i < size_, bad_index ());
            return data_ [i];
        }

        // Assignment
        NUMERICS_INLINE
        unbounded_array &operator = (const unbounded_array &a) {
            // Too unusual semantic.
            // Thanks to Michael Stevens for spotting this.
            // check (this != &a, external_logic ());
            if (this != &a) {
                check (size_ == a.size_, bad_size ());
                std::copy (a.data_, a.data_ + a.size_, data_);
            }
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
            // Too unusual semantic.
            // check (this != &a, external_logic ());
            if (this != &a) {
                check (size_ == a.size_, bad_size ());
                std::swap (size_, a.size_);
                std::swap (data_, a.data_);
            }
        }
#ifdef NUMERICS_FRIEND_FUNCTION
        NUMERICS_INLINE
        friend void swap (unbounded_array &a1, unbounded_array &a2) {
            a1.swap (a2);
        }
#endif

        // Element insertion and deletion
        NUMERICS_INLINE
        pointer insert (pointer it, const value_type &t) {
            check (begin () <= it && it < end (), bad_index ());
            check (*it == value_type (), external_logic ());
            *it = t;
            return it;
        }
        NUMERICS_INLINE
        void insert (pointer it, pointer it1, pointer it2) {
            while (it1 != it2) {
                check (begin () <= it && it < end (), bad_index ());
                check (*it == value_type (), external_logic ());
                *it = *it1;
                ++ it, ++ it1;
            }
        }
        NUMERICS_INLINE
        void erase (pointer it) {
            check (begin () <= it && it < end (), bad_index ());
            *it = value_type ();
        }
        NUMERICS_INLINE
        void erase (pointer it1, pointer it2) {
            while (it1 != it2) {
                check (begin () <= it1 && it1 < end (), bad_index ());
                *it1 = value_type ();
                ++ it1;
            }
        }
        NUMERICS_INLINE
        void clear () {
            erase (begin (), end ());
        }

        // Iterators simply are pointers.

        typedef const_pointer const_iterator;

        NUMERICS_INLINE
        const_iterator begin () const {
            return data_;
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return data_ + size_;
        }

        typedef pointer iterator;

        NUMERICS_INLINE
        iterator begin () {
            return data_;
        }
        NUMERICS_INLINE
        iterator end () {
            return data_ + size_;
        }

        // Reverse iterators

#ifdef BOOST_MSVC_STD_ITERATOR
        typedef std::reverse_iterator<const_iterator, value_type, const_reference> const_reverse_iterator;
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

#ifdef BOOST_MSVC_STD_ITERATOR
        typedef std::reverse_iterator<iterator, value_type, reference> reverse_iterator;
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

    private:
        size_type size_;
        pointer data_;
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
        typedef const T &const_reference;
        typedef T &reference;
        typedef const T *const_pointer;
        typedef T *pointer;

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
        const_reference operator [] (size_type i) const {
            check (i < size_, bad_index ());
            return data_ [i];
        }
        NUMERICS_INLINE
        reference operator [] (size_type i) {
            check (i < size_, bad_index ());
            return data_ [i];
        }

        // Assignment
        NUMERICS_INLINE
        bounded_array &operator = (const bounded_array &a) {
            // Too unusual semantic.
            // Thanks to Michael Stevens for spotting this.
            // check (this != &a, external_logic ());
            if (this != &a) {
                check (size_ == a.size_, bad_size ());
                std::copy (a.data_, a.data_ + a.size_, data_);
            }
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
            // Too unusual semantic.
            // check (this != &a, external_logic ());
            if (this != &a) {
                check (size_ == a.size_, bad_size ());
                std::swap_ranges (data_, data_ + size_, a.data_);
            }
        }
#ifdef NUMERICS_FRIEND_FUNCTION
        NUMERICS_INLINE
        friend void swap (bounded_array &a1, bounded_array &a2) {
            a1.swap (a2);
        }
#endif

        // Element insertion and deletion
        NUMERICS_INLINE
        pointer insert (pointer it, const value_type &t) {
            check (begin () <= it && it < end (), bad_index ());
            check (*it == value_type (), external_logic ());
            *it = t;
            return it;
        }
        NUMERICS_INLINE
        void insert (pointer it, pointer it1, pointer it2) {
            while (it1 != it2) {
                check (begin () <= it && it < end (), bad_index ());
                check (*it == value_type (), external_logic ());
                *it = *it1;
                ++ it, ++ it1;
            }
        }
        NUMERICS_INLINE
        void erase (pointer it) {
            check (begin () <= it && it < end (), bad_index ());
            *it = value_type ();
        }
        NUMERICS_INLINE
        void erase (pointer it1, pointer it2) {
            while (it1 != it2) {
                check (begin () <= it1 && it1 < end (), bad_index ());
                *it1 = value_type ();
                ++ it1;
            }
        }
        NUMERICS_INLINE
        void clear () {
            erase (begin (), end ());
        }

        // Iterators simply are pointers.

        typedef const_pointer const_iterator;

        NUMERICS_INLINE
        const_iterator begin () const {
            return data_;
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return data_ + size_;
        }

        typedef pointer iterator;

        NUMERICS_INLINE
        iterator begin () {
            return data_;
        }
        NUMERICS_INLINE
        iterator end () {
            return data_ + size_;
        }

        // Reverse iterators

#ifdef BOOST_MSVC_STD_ITERATOR
        typedef std::reverse_iterator<const_iterator, value_type, const_reference> const_reverse_iterator;
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

#ifdef BOOST_MSVC_STD_ITERATOR
        typedef std::reverse_iterator<iterator, value_type, reference> reverse_iterator;
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

    private:
        size_type size_;
        value_type data_ [N];
    };

    template<class T, std::size_t N>
    NUMERICS_INLINE
    bounded_array<T, N> &assign_temporary (bounded_array<T, N> &a1, bounded_array<T, N> &a2) {
        return a1.assign_temporary (a2);
    }

#ifdef NUMERICS_SIMPLE_ARRAY_ADAPTOR

    // Array adaptor
    template<class T>
    class array_adaptor {
    public:
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef const T &const_reference;
        typedef T &reference;
        typedef const T *const_pointer;
        typedef T *pointer;

        // Construction and destruction
        NUMERICS_INLINE
        array_adaptor ():
            size_ (0), own_ (true), data_ (new value_type [0]) {}
        NUMERICS_EXPLICIT NUMERICS_INLINE
        array_adaptor (size_type size):
            size_ (size), own_ (true), data_ (new value_type [size]) {
            if (! data_)
                throw std::bad_alloc ();
        }
        NUMERICS_INLINE
        array_adaptor (size_type size, pointer data):
            size_ (size), own_ (false), data_ (data) {}
#ifdef NUMERICS_DEEP_COPY
        NUMERICS_INLINE
        array_adaptor (const array_adaptor &a):
            size_ (a.size_), own_ (true), data_ (new value_type [a.size_]) {
            if (! data_)
                throw std::bad_alloc ();
            *this = a;
        }
#else
        NUMERICS_INLINE
        array_adaptor (const array_adaptor &a):
            size_ (a.size_), own_ (a.own_), data_ (a.data_) {
            if (own_)
                throw std::bad_alloc ();
        }
#endif
        NUMERICS_INLINE
        ~array_adaptor () {
            if (own_) {
                if (! data_)
                    throw std::bad_alloc ();
                delete [] data_;
            }
        }

        // Resizing
        NUMERICS_INLINE
        void resize (size_type size) {
            if (own_) {
                if (! data_)
                    throw std::bad_alloc ();
                delete [] data_;
            }
            size_ = size;
            own_ = true;
            data_ = new value_type [size];
            if (! data_)
                throw std::bad_alloc ();
        }
        NUMERICS_INLINE
        void resize (size_type size, pointer data) {
            if (own_) {
                if (! data_)
                    throw std::bad_alloc ();
                delete [] data_;
            }
            size_ = size;
            own_ = false;
            data_ = data;
        }

        NUMERICS_INLINE
        size_type size () const {
            return size_;
        }

        // Element access
        NUMERICS_INLINE
        const_reference operator [] (size_type i) const {
            check (i < size_, bad_index ());
            return data_ [i];
        }
        NUMERICS_INLINE
        reference operator [] (size_type i) {
            check (i < size_, bad_index ());
            return data_ [i];
        }

        // Assignment
        NUMERICS_INLINE
        array_adaptor &operator = (const array_adaptor &a) {
            // Too unusual semantic.
            // Thanks to Michael Stevens for spotting this.
            // check (this != &a, external_logic ());
            if (this != &a) {
                check (size_ == a.size_, bad_size ());
                std::copy (a.data_, a.data_ + a.size_, data_);
            }
            return *this;
        }
        NUMERICS_INLINE
        array_adaptor &assign_temporary (array_adaptor &a) {
            if (own_ && a.own_)
                swap (a);
            else
                *this = a;
            return *this;
        }

        // Swapping
        NUMERICS_INLINE
        void swap (array_adaptor &a) {
            // Too unusual semantic.
            // check (this != &a, external_logic ());
            if (this != &a) {
                check (size_ == a.size_, bad_size ());
                std::swap (size_, a.size_);
                std::swap (own_, a.own_);
                std::swap (data_, a.data_);
            }
        }
#ifdef NUMERICS_FRIEND_FUNCTION
        NUMERICS_INLINE
        friend void swap (array_adaptor &a1, array_adaptor &a2) {
            a1.swap (a2);
        }
#endif

        // Element insertion and deletion
        NUMERICS_INLINE
        pointer insert (pointer it, const value_type &t) {
            check (begin () <= it && it < end (), bad_index ());
            check (*it == value_type (), external_logic ());
            *it = t;
            return it;
        }
        NUMERICS_INLINE
        void insert (pointer it, pointer it1, pointer it2) {
            while (it1 != it2) {
                check (begin () <= it && it < end (), bad_index ());
                check (*it == value_type (), external_logic ());
                *it = *it1;
                ++ it, ++ it1;
            }
        }
        NUMERICS_INLINE
        void erase (pointer it) {
            check (begin () <= it && it < end (), bad_index ());
            *it = value_type ();
        }
        NUMERICS_INLINE
        void erase (pointer it1, pointer it2) {
            while (it1 != it2) {
                check (begin () <= it1 && it1 < end (), bad_index ());
                *it1 = value_type ();
                ++ it1;
            }
        }
        NUMERICS_INLINE
        void clear () {
            erase (begin (), end ());
        }

        // Iterators simply are pointers.

        typedef const_pointer const_iterator;

        NUMERICS_INLINE
        const_iterator begin () const {
            return data_;
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return data_ + size_;
        }

        typedef pointer iterator;

        NUMERICS_INLINE
        iterator begin () {
            return data_;
        }
        NUMERICS_INLINE
        iterator end () {
            return data_ + size_;
        }

        // Reverse iterators

#ifdef BOOST_MSVC_STD_ITERATOR
        typedef std::reverse_iterator<const_iterator, value_type, const_reference> const_reverse_iterator;
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

#ifdef BOOST_MSVC_STD_ITERATOR
        typedef std::reverse_iterator<iterator, value_type, reference> reverse_iterator;
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

    private:
        size_type size_;
        bool own_;
        pointer data_;
    };

#else

    template<class T>
    struct leaker {
        typedef void result_type;
        typedef T *argument_type;

        NUMERICS_INLINE
        result_type operator () (argument_type x) {}
    };

    // Array adaptor
    template<class T>
    class array_adaptor {
    public:
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef const T &const_reference;
        typedef T &reference;
        typedef const T *const_pointer;
        typedef T *pointer;

        // Construction and destruction
        NUMERICS_INLINE
        array_adaptor ():
            size_ (0), own_ (true), data_ (new value_type [0]) {}
        NUMERICS_EXPLICIT NUMERICS_INLINE
        array_adaptor (size_type size):
            size_ (size), own_ (true), data_ (new value_type [size]) {
            if (! data_.get ())
                throw std::bad_alloc ();
        }
        NUMERICS_INLINE
        array_adaptor (size_type size, pointer data):
            size_ (size), own_ (false), data_ (data, leaker<value_type> ()) {}
#ifdef NUMERICS_DEEP_COPY
        NUMERICS_INLINE
        array_adaptor (const array_adaptor &a):
            size_ (a.size_), own_ (true), data_ (new value_type [a.size_]) {
            if (! data_.get ())
                throw std::bad_alloc ();
            *this = a;
        }
#else
        NUMERICS_INLINE
        array_adaptor (const array_adaptor &a):
            size_ (a.size_), own_ (a.own_), data_ (a.data_) {}
#endif
        NUMERICS_INLINE
        ~array_adaptor () {
            if (! data_.get ())
                throw std::bad_alloc ();
        }

        // Resizing
        NUMERICS_INLINE
        void resize (size_type size) {
            if (! data_.get ())
                throw std::bad_alloc ();
            size_ = size;
            data_ = shared_array<value_type> (new value_type [size]);
            if (! data_.get ())
                throw std::bad_alloc ();
        }
        NUMERICS_INLINE
        void resize (size_type size, pointer data) {
            if (! data_.get ())
                throw std::bad_alloc ();
            size_ = size;
            data_ = data;
        }

        NUMERICS_INLINE
        size_type size () const {
            return size_;
        }

        // Element access
        NUMERICS_INLINE
        const_reference operator [] (size_type i) const {
            check (i < size_, bad_index ());
            return data_ [i];
        }
        NUMERICS_INLINE
        reference operator [] (size_type i) {
            check (i < size_, bad_index ());
            return data_ [i];
        }

        // Assignment
        NUMERICS_INLINE
        array_adaptor &operator = (const array_adaptor &a) {
            // Too unusual semantic.
            // Thanks to Michael Stevens for spotting this.
            // check (this != &a, external_logic ());
            if (this != &a) {
                check (size_ == a.size_, bad_size ());
                std::copy (a.data_.get (), a.data_.get () + a.size_, data_.get ());
            }
            return *this;
        }
        NUMERICS_INLINE
        array_adaptor &assign_temporary (array_adaptor &a) {
            if (own_ && a.own_)
                swap (a);
            else
                *this = a;
            return *this;
        }

        // Swapping
        NUMERICS_INLINE
        void swap (array_adaptor &a) {
            // Too unusual semantic.
            // check (this != &a, external_logic ());
            if (this != &a) {
                check (size_ == a.size_, bad_size ());
                std::swap (size_, a.size_);
                std::swap (own_, a.own_);
                std::swap (data_, a.data_);
            }
        }
#ifdef NUMERICS_FRIEND_FUNCTION
        NUMERICS_INLINE
        friend void swap (array_adaptor &a1, array_adaptor &a2) {
            a1.swap (a2);
        }
#endif

        // Element insertion and deletion
        NUMERICS_INLINE
        pointer insert (pointer it, const value_type &t) {
            check (begin () <= it && it < end (), bad_index ());
            check (*it == value_type (), external_logic ());
            *it = t;
            return it;
        }
        NUMERICS_INLINE
        void insert (pointer it, pointer it1, pointer it2) {
            while (it1 != it2) {
                check (begin () <= it && it < end (), bad_index ());
                check (*it == value_type (), external_logic ());
                *it = *it1;
                ++ it, ++ it1;
            }
        }
        NUMERICS_INLINE
        void erase (pointer it) {
            check (begin () <= it && it < end (), bad_index ());
            *it = value_type ();
        }
        NUMERICS_INLINE
        void erase (pointer it1, pointer it2) {
            while (it1 != it2) {
                check (begin () <= it1 && it1 < end (), bad_index ());
                *it1 = value_type ();
                ++ it1;
            }
        }
        NUMERICS_INLINE
        void clear () {
            erase (begin (), end ());
        }

        // Iterators simply are pointers.

        typedef const_pointer const_iterator;

        NUMERICS_INLINE
        const_iterator begin () const {
            return data_.get ();
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return data_.get () + size_;
        }

        typedef pointer iterator;

        NUMERICS_INLINE
        iterator begin () {
            return data_.get ();
        }
        NUMERICS_INLINE
        iterator end () {
            return data_.get () + size_;
        }

        // Reverse iterators

#ifdef BOOST_MSVC_STD_ITERATOR
        typedef std::reverse_iterator<const_iterator, value_type, const_reference> const_reverse_iterator;
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

#ifdef BOOST_MSVC_STD_ITERATOR
        typedef std::reverse_iterator<iterator, value_type, reference> reverse_iterator;
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

    private:
        size_type size_;
        bool own_;
        shared_array<value_type> data_;
    };

#endif

    template<class T>
    NUMERICS_INLINE
    std::valarray<T> &assign_temporary (std::valarray<T> &a1, std::valarray<T> &a2) {
        // Too unusual semantic.
        // check (&a1 != &a2, external_logic ());
        if (&a1 != &a2) {
            check (a1.size () == a2.size (), bad_size ());
            a1 = a2;
        }
        return a1;
    }

    template<class T>
    NUMERICS_INLINE
    std::vector<T> &assign_temporary (std::vector<T> &a1, std::vector<T> &a2) {
        // Too unusual semantic.
        // check (&a1 != &a2, external_logic ());
        if (&a1 != &a2) {
            check (a1.size () == a2.size (), bad_size ());
            a1.swap (a2);
        }
        return a1;
    }

    // Range class
    class range {
    public:
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef difference_type value_type;
        typedef const difference_type &const_reference;
        typedef difference_type reference;
        typedef const difference_type *const_pointer;
        typedef difference_type *pointer;
        typedef size_type const_iterator_type;

        // Construction and destruction
        NUMERICS_INLINE
        range (): 
            start_ (), size_ () {}
        NUMERICS_INLINE
        range (size_type start, size_type stop):
            start_ (start), size_ (stop - start) {
            check (start <= stop, bad_size ());
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
            check (i < size_, bad_index ());
            return start_ + i; 
        }

        // Composition
        NUMERICS_INLINE
        range composite (const range &r) const {
            check (r.start_ + r.size_ <= size_, bad_size ());
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

#ifdef NUMERICS_USE_INDEXED_ITERATOR
        typedef indexed_const_iterator<range, std::random_access_iterator_tag> const_iterator;
#else
        class const_iterator:
            public container_const_reference<range>,
            public random_access_iterator_base<std::random_access_iterator_tag,
                                               const_iterator, value_type> {
        public:

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator (): 
                container_const_reference<range> (), it_ () {}
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
                check ((*this) ().start () <= it_, bad_index ());
                check (it_ < (*this) ().start () + (*this) ().size (), bad_index ());
                return it_; 
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                return it_ - (*this) ().start ();
            }

            // Assignment 
            NUMERICS_INLINE
            const_iterator &operator = (const const_iterator &it) {
                // Comeau recommends...
                this->assign (&it ());
                it_ = it.it_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it_ == it.it_;
            }
            NUMERICS_INLINE
            bool operator < (const const_iterator &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it_ < it.it_;
            }

        private:
            const_iterator_type it_;
        };
#endif

        NUMERICS_INLINE
        const_iterator begin () const {
            return const_iterator (*this, start_);
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return const_iterator (*this, start_ + size_);
        }

        // Reverse iterator

#ifdef BOOST_MSVC_STD_ITERATOR
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
        typedef const difference_type &const_reference;
        typedef difference_type reference;
        typedef const difference_type *const_pointer;
        typedef difference_type *pointer;
        typedef difference_type const_iterator_type;

        // Construction and destruction
        NUMERICS_INLINE
        slice ():
            start_ (), stride_ (), size_ () {}
        NUMERICS_INLINE
        slice (size_type start, difference_type stride, size_type size):
            start_ (start), stride_ (stride), size_ (size) {}

        NUMERICS_INLINE
        size_type start () const {
            return start_;
        }
        NUMERICS_INLINE
        difference_type stride () const {
            return stride_;
        }
        NUMERICS_INLINE
        size_type size () const {
            return size_;
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i) const {
            check (i < size_, bad_index ());
            return start_ + i * stride_;
        }

        // Composition
        NUMERICS_INLINE
        slice composite (const range &r) const {
            check (r.start () + r.size () <= size_, bad_size ());
            return slice (start_ + stride_ * r.start (), stride_, r.size ());
        }
        NUMERICS_INLINE
        slice composite (const slice &s) const {
            check (s.start_ + s.stride_ * s.size_ <= size_, bad_size ());
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

#ifdef NUMERICS_USE_INDEXED_ITERATOR
        typedef indexed_const_iterator<slice, std::random_access_iterator_tag> const_iterator;
#else
        class const_iterator:
            public container_const_reference<slice>,
            public random_access_iterator_base<std::random_access_iterator_tag,
                                               const_iterator, value_type> {
        public:

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator (): 
                container_const_reference<slice> (), it_ () {}
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
                check ((*this) ().stride () != 0, divide_by_zero ());
                return (it_ - it.it_) / (*this) ().stride ();
            }

            // Dereference
            NUMERICS_INLINE
            value_type operator * () const {
                check (index () < (*this) ().size (), bad_index ());
                return it_; 
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                check ((*this) ().stride () != 0, divide_by_zero ());
                return (it_ - difference_type ((*this) ().start ())) / (*this) ().stride ();
            }

            // Assignment 
            NUMERICS_INLINE
            const_iterator &operator = (const const_iterator &it) {
                // Comeau recommends...
                this->assign (&it ());
                it_ = it.it_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it_ == it.it_;
            }
            NUMERICS_INLINE
            bool operator < (const const_iterator &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it_ < it.it_;
            }

        private:
            const_iterator_type it_;
        };
#endif

        NUMERICS_INLINE
        const_iterator begin () const {
            return const_iterator (*this, start_);
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return const_iterator (*this, start_ + stride_ * size_);
        }

        // Reverse iterator

#ifdef BOOST_MSVC_STD_ITERATOR
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

    private:
        size_type start_;
        difference_type stride_;
        size_type size_;
    };

    // Indirect array class
    template<class A = unbounded_array<std::size_t> >
    class indirect_array {
    public:
        typedef A array_type;
        typedef const A const_array_type;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef difference_type value_type;
        typedef const difference_type &const_reference;
        typedef difference_type reference;
        typedef const difference_type *const_pointer;
        typedef difference_type *pointer;
        typedef difference_type const_iterator_type;

        // Construction and destruction
        NUMERICS_INLINE
        indirect_array ():
            size_ (), data_ () {}
        NUMERICS_INLINE
        indirect_array (size_type size, const array_type &data):
            size_ (size), data_ (data) {}
        NUMERICS_INLINE
        indirect_array (pointer start, pointer stop):
            size_ (stop - start), data_ (stop - start) {
            std::copy (start, stop, data_.begin ());
        }

        NUMERICS_INLINE
        size_type size () const {
            return size_;
        }
        NUMERICS_INLINE
        const_array_type data () const {
            return data_;
        }
        NUMERICS_INLINE
        array_type data () {
            return data_;
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i) const {
            check (i < size_, bad_index ());
            return data_ [i];
        }

        // Composition
        NUMERICS_INLINE
        indirect_array composite (const range &r) const {
            check (r.start () + r.size () <= size_, bad_size ());
            array_type data (r.size ());
            for (size_type i = 0; i < r.size (); ++ i)
                data [i] = data_ [r.start () + i];
            return indirect_array (r.size (), data);
        }
        NUMERICS_INLINE
        indirect_array composite (const slice &s) const {
            check (s.start () + s.stride () * s.size () <= size (), bad_size ());
            array_type data (s.size ());
            for (size_type i = 0; i < s.size (); ++ i)
                data [i] = data_ [s.start () + s.stride () * i];
            return indirect_array (s.size (), data);
        }
        NUMERICS_INLINE
        indirect_array composite (const indirect_array &ia) const {
            array_type data (ia.size_);
            for (size_type i = 0; i < ia.size_; ++ i) {
                check (ia.data_ [i] <= size_, bad_size ());
                data [i] = data_ [ia.data_ [i]];
            }
            return indirect_array (ia.size_, data);
        }

        // Comparison
        NUMERICS_INLINE
        bool operator == (const indirect_array &ia) const {
            if (size_ != ia.size_)
                return false;
            for (size_type i = 0; i < common (size_, ia.size_); ++ i)
                if (data_ [i] != ia.data_ [i])
                    return false;
            return true;
        }
        NUMERICS_INLINE
        bool operator != (const indirect_array &ia) const {
            return ! (*this == ia);
        }

        // Iterator simply is a index.

#ifdef NUMERICS_USE_INDEXED_ITERATOR
        typedef indexed_const_iterator<indirect_array, std::random_access_iterator_tag> const_iterator;
#else
        class const_iterator:
            public container_const_reference<indirect_array>,
            public random_access_iterator_base<std::random_access_iterator_tag,
                                               const_iterator, value_type> {
        public:
#ifndef BOOST_MSVC_STD_ITERATOR
            typedef typename indirect_array::difference_type difference_type;
            typedef typename indirect_array::value_type value_type;
            typedef typename indirect_array::const_reference reference;
            typedef typename indirect_array::const_pointer pointer;
#endif

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator ():
                container_const_reference<indirect_array> (), it_ () {}
            NUMERICS_INLINE
            const_iterator (const indirect_array &ia, const const_iterator_type &it):
                container_const_reference<indirect_array> (ia), it_ (it) {}

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
                // Comeau recommends...
                this->assign (&it ());
                it_ = it.it_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it_ == it.it_;
            }
            NUMERICS_INLINE
            bool operator < (const const_iterator &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it_ < it.it_;
            }

        private:
            const_iterator_type it_;
        };
#endif

        NUMERICS_INLINE
        const_iterator begin () const {
            return const_iterator (*this, 0);
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return const_iterator (*this, size_);
        }

        // Reverse iterator

#ifdef BOOST_MSVC_STD_ITERATOR
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

    private:
        size_type size_;
        array_type data_;
    };

}}

#endif






























