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

#ifndef NUMERICS_STORAGE_SP_H
#define NUMERICS_STORAGE_SP_H

#include <algorithm>
#include <map>

#include "config.h"
#include "exception.h"
#include "iterator.h"
#include "storage.h"

namespace numerics {

    template<class P>
    struct less {
        bool operator () (const P &p1, const P &p2) {
            return p1.first < p2.first;
        }
    };

    // Compressed array 
    template<class I, class T>
    class compressed_array {
    public:      
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef I index_type;
        typedef T data_value_type;
        typedef const T &data_const_reference_type;
        typedef T &data_reference_type;
        typedef std::pair<I, T> value_type;
        typedef const std::pair<I, T> &const_reference_type;
        typedef std::pair<I, T> &reference_type;
        typedef const std::pair<I, T> *const_pointer_type;
        typedef std::pair<I, T> *pointer_type;

        // Construction and destruction
        NUMERICS_INLINE
        compressed_array (): 
            capacity_ (0), data_ (new value_type [0]), size_ (0) { 
            if (! data_)
                throw std::bad_alloc ();
        }
        NUMERICS_EXPLICIT NUMERICS_INLINE
        compressed_array (size_type size): 
            capacity_ (size), data_ (new value_type [size]), size_ (size) { 
            if (! data_)
                throw std::bad_alloc ();
        }
        NUMERICS_INLINE
        compressed_array (const compressed_array &a): 
            capacity_ (a.size_), data_ (new value_type [a.size_]), size_ (a.size_) { 
            if (! data_)
                throw std::bad_alloc ();
            *this = a;
        }
        NUMERICS_INLINE
        ~compressed_array () { 
            if (! data_)
                throw std::bad_alloc ();
            delete [] data_; 
        }

        // Resizing
        NUMERICS_INLINE
        void resize (size_type size) {
            if (size > capacity_) {
                pointer_type data = new value_type [size << 1];
                if (! data)
                    throw std::bad_alloc ();
                if (! data_)
                    throw std::bad_alloc ();
                std::copy (data_, data_ + size_, data);
                delete [] data_;
                capacity_ = size << 1;
                data_ = data;
            }
            size_ = size;
        }

        NUMERICS_INLINE
        size_type size () const { 
            return size_; 
        }

        // Element access
        NUMERICS_INLINE
        data_reference_type operator [] (index_type i) {
            pointer_type it = find (i);
            if (it == end ()) 
                it = insert (end (), value_type (i, data_value_type ()));
            return it->second;
        }

        // Assignment
        NUMERICS_INLINE
        compressed_array &operator = (const compressed_array &a) { 
            check (this != &a, external_logic ());
            resize (a.size_);
            std::copy (a.data_, a.data_ + a.size_, data_);
            return *this;
        }
        NUMERICS_INLINE
        compressed_array &assign_temporary (compressed_array &a) { 
            swap (a);
            return *this;
        }

        // Swapping
        NUMERICS_INLINE
	    void swap (compressed_array &a) {
            check (this != &a, external_logic ());
            std::swap (capacity_, a.capacity_);
            std::swap (data_, a.data_);
            std::swap (size_, a.size_);
        }
#ifndef USE_GCC
        NUMERICS_INLINE
	    friend void swap (compressed_array &a1, compressed_array &a2) {
            a1.swap (a2);
        }
#endif

        // Element insertion and deletion
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        pointer_type insert (pointer_type it, const value_type &p) {
            if (size () == 0 || (it = end () - 1)->first < p.first) {
                resize (size () + 1);
                *(it = end () - 1) = p;
                return it;
            } 
#ifdef NUMERICS_APPEND_ONLY
            throw external_logic ();
#else
            it = std::upper_bound (begin (), end (), p, less<value_type> ());
            check (it != end (), internal_logic ());
            check (it->first != p.first, external_logic ());
            difference_type n = it - begin ();
            resize (size () + 1);
            it = begin () + n;
            std::copy_backward (it, end () - 1, end ());
            *it = p;
            return it;
#endif
        }
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void insert (pointer_type it, pointer_type it1, pointer_type it2) {
#ifdef NUMERICS_BOUNDS_CHECK
            while (it1 != it2) {
                insert (it, *it1);
                ++ it1;
            }
#else
            difference_type n = it - begin ();
            resize (size () + it2 - it1);
            it = begin () + n;
            std::copy (it1, it2, it);
            std::sort (begin (), end (), less<value_type> ()); 
#endif
        }
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void erase (pointer_type it) {
            // FIXME: delete physically?
            check (begin () <= it && it < end (), bad_index ());
            *it = value_type ();
        }
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void erase (pointer_type it1, pointer_type it2) {
            // FIXME: delete physically?
            while (it1 != it2) {
                check (begin () <= it1 && it1 < end (), bad_index ());
                *it1 = value_type ();
                ++ it1;
            }
        }
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void clear () {
            resize (0);
        }

        // Element lookup
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        const_pointer_type find (index_type i) const {
            std::pair<const_pointer_type, const_pointer_type> pit;
//            pit = std::equal_range (begin (), end (), value_type (i, 0), less<value_type> ());
            pit = std::equal_range (begin (), end (), value_type (i, data_value_type ()), less<value_type> ());
            if (pit.first->first == i)
                return pit.first;
            else if (pit.second->first == i)
                return pit.second;
            else
                return end ();
        }
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        pointer_type find (index_type i) {
            std::pair<pointer_type, pointer_type> pit;
//            pit = std::equal_range (begin (), end (), value_type (i, 0), less<value_type> ());
            pit = std::equal_range (begin (), end (), value_type (i, data_value_type ()), less<value_type> ());
            if (pit.first->first == i)
                return pit.first;
            else if (pit.second->first == i)
                return pit.second;
            else
                return end ();
        }
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        const_pointer_type lower_bound (index_type i) const {
//            return std::lower_bound (begin (), end (), value_type (i, 0), less<value_type> ());
            return std::lower_bound (begin (), end (), value_type (i, data_value_type ()), less<value_type> ());
        }
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        pointer_type lower_bound (index_type i) {
//            return std::lower_bound (begin (), end (), value_type (i, 0), less<value_type> ());
            return std::lower_bound (begin (), end (), value_type (i, data_value_type ()), less<value_type> ());
        }
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        const_pointer_type upper_bound (index_type i) const {
//            return std::upper_bound (begin (), end (), value_type (i, 0), less<value_type> ());
            return std::upper_bound (begin (), end (), value_type (i, data_value_type ()), less<value_type> ());
        }
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        pointer_type upper_bound (index_type i) {
//            return std::upper_bound (begin (), end (), value_type (i, 0), less<value_type> ());
            return std::upper_bound (begin (), end (), value_type (i, data_value_type ()), less<value_type> ());
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

    private:
        size_type capacity_;
        pointer_type data_;
        size_type size_;
    };

    template<class I, class T>
    NUMERICS_INLINE
    compressed_array<I, T> &assign_temporary (compressed_array<I, T> &a1, compressed_array<I, T> &a2) { 
        return a1.assign_temporary (a2);
    }

    template<class I, class T, class F>
    NUMERICS_INLINE
    std::map<I, T, F> &assign_temporary (std::map<I, T, F> &a1, std::map<I, T, F> &a2) { 
        check (&a1 != &a2, external_logic ());
        a1.swap (a2);
        return  a1;
    }
    // This specialization is missing in Dinkumware's STL?!
    template<class I, class T, class F>
    NUMERICS_INLINE
    void swap (std::map<I, T, F> &a1, std::map<I, T, F> &a2) { 
        check (&a1 != &a2, external_logic ());
        a1.swap (a2);
    }

}

#endif 


