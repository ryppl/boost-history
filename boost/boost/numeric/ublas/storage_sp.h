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
#include <set>

#include "config.h"
#include "exception.h"
#include "iterator.h"
#include "storage.h"

namespace numerics {

    template<class P>
    struct less {
		NUMERICS_INLINE
        bool operator () (const P &p1, const P &p2) {
            return p1.first < p2.first;
        }
    };

    // Map array 
    template<class I, class T>
    class map_array {
    public:      
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef I index_type;
        typedef T data_value_type;
        typedef const T &data_const_reference;
        typedef T &data_reference;
        typedef std::pair<I, T> value_type;
        typedef const std::pair<I, T> &const_reference;
        typedef std::pair<I, T> &reference;
        typedef const std::pair<I, T> *const_pointer;
        typedef std::pair<I, T> *pointer;

        // Construction and destruction
        NUMERICS_INLINE
        map_array (): 
            capacity_ (0), data_ (new value_type [0]), size_ (0) { 
            if (! data_)
                throw std::bad_alloc ();
        }
        NUMERICS_EXPLICIT NUMERICS_INLINE
        map_array (size_type size): 
            capacity_ (size), data_ (new value_type [size]), size_ (0) { 
            if (! data_)
                throw std::bad_alloc ();
        }
        NUMERICS_INLINE
        map_array (const map_array &a): 
            capacity_ (a.size_), data_ (new value_type [a.size_]), size_ (a.size_) { 
            if (! data_)
                throw std::bad_alloc ();
            *this = a;
        }
        NUMERICS_INLINE
        ~map_array () { 
            if (! data_)
                throw std::bad_alloc ();
            delete [] data_; 
        }

        // Resizing
        NUMERICS_INLINE
        void resize (size_type size) {
            if (size > capacity_) {
                pointer data = new value_type [size << 1];
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
        data_reference operator [] (index_type i) {
            pointer it = find (i);
            if (it == end ()) 
                it = insert (end (), value_type (i, data_value_type ()));
            return it->second;
        }

        // Assignment
        NUMERICS_INLINE
        map_array &operator = (const map_array &a) { 
            check (this != &a, external_logic ());
            resize (a.size_);
            std::copy (a.data_, a.data_ + a.size_, data_);
            return *this;
        }
        NUMERICS_INLINE
        map_array &assign_temporary (map_array &a) { 
            swap (a);
            return *this;
        }

        // Swapping
        NUMERICS_INLINE
	    void swap (map_array &a) {
            check (this != &a, external_logic ());
            std::swap (capacity_, a.capacity_);
            std::swap (data_, a.data_);
            std::swap (size_, a.size_);
        }
#ifndef USE_GCC
        NUMERICS_INLINE
	    friend void swap (map_array &a1, map_array &a2) {
            a1.swap (a2);
        }
#endif

        // Element insertion and deletion
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        pointer insert (pointer it, const value_type &p) {
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
        void insert (pointer it, pointer it1, pointer it2) {
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
        void erase (pointer it) {
            // FIXME: delete physically?
            check (begin () <= it && it < end (), bad_index ());
            *it = value_type ();
        }
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void erase (pointer it1, pointer it2) {
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
        const_pointer find (index_type i) const {
#ifdef NUMERICS_DEPRECATED
            std::pair<const_pointer, const_pointer> pit;
            pit = std::equal_range (begin (), end (), value_type (i, data_value_type ()), less<value_type> ());
            if (pit.first->first == i)
                return pit.first;
            else if (pit.second->first == i)
                return pit.second;
            else
                return end ();
#else
            const_pointer it (std::lower_bound (begin (), end (), value_type (i, data_value_type ()), less<value_type> ()));
			if (it == end () || it->first != i)
				it = end ();
			return it;
#endif
        }
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        pointer find (index_type i) {
#ifdef NUMERICS_DEPRECATED
            std::pair<pointer, pointer> pit;
            pit = std::equal_range (begin (), end (), value_type (i, data_value_type ()), less<value_type> ());
            if (pit.first->first == i)
                return pit.first;
            else if (pit.second->first == i)
                return pit.second;
            else
                return end ();
#else
            pointer it (std::lower_bound (begin (), end (), value_type (i, data_value_type ()), less<value_type> ()));
			if (it == end () || it->first != i)
				it = end ();
			return it;
#endif
        }
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        const_pointer lower_bound (index_type i) const {
            return std::lower_bound (begin (), end (), value_type (i, data_value_type ()), less<value_type> ());
        }
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        pointer lower_bound (index_type i) {
            return std::lower_bound (begin (), end (), value_type (i, data_value_type ()), less<value_type> ());
        }
#ifdef NUMERICS_DEPRECATED
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        const_pointer upper_bound (index_type i) const {
            return std::upper_bound (begin (), end (), value_type (i, data_value_type ()), less<value_type> ());
        }
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        pointer upper_bound (index_type i) {
            return std::upper_bound (begin (), end (), value_type (i, data_value_type ()), less<value_type> ());
        }
#endif

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

#ifdef USE_MSVC
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

#ifdef USE_MSVC
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
        size_type capacity_;
        pointer data_;
        size_type size_;
    };

    template<class I, class T>
    NUMERICS_INLINE
    map_array<I, T> &assign_temporary (map_array<I, T> &a1, map_array<I, T> &a2) { 
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

    // Set array 
    template<class I>
    class set_array {
    public:      
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef I index_type;
        typedef I value_type;
        typedef const I &const_reference;
        typedef I &reference;
        typedef const I *const_pointer;
        typedef I *pointer;

        // Construction and destruction
        NUMERICS_INLINE
        set_array (): 
            capacity_ (0), data_ (new value_type [0]), size_ (0) { 
            if (! data_)
                throw std::bad_alloc ();
        }
        NUMERICS_EXPLICIT NUMERICS_INLINE
        set_array (size_type size): 
            capacity_ (size), data_ (new value_type [size]), size_ (0) { 
            if (! data_)
                throw std::bad_alloc ();
        }
        NUMERICS_INLINE
        set_array (const set_array &a): 
            capacity_ (a.size_), data_ (new value_type [a.size_]), size_ (a.size_) { 
            if (! data_)
                throw std::bad_alloc ();
            *this = a;
        }
        NUMERICS_INLINE
        ~set_array () { 
            if (! data_)
                throw std::bad_alloc ();
            delete [] data_; 
        }

        // Resizing
        NUMERICS_INLINE
        void resize (size_type size) {
            if (size > capacity_) {
                pointer data = new value_type [size << 1];
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
        const_reference operator [] (index_type i) {
            pointer it = find (i);
            if (it == end ()) 
                it = insert (end (), i);
            return *it;
        }

        // Assignment
        NUMERICS_INLINE
        set_array &operator = (const set_array &a) { 
            check (this != &a, external_logic ());
            resize (a.size_);
            std::copy (a.data_, a.data_ + a.size_, data_);
            return *this;
        }
        NUMERICS_INLINE
        set_array &assign_temporary (set_array &a) { 
            swap (a);
            return *this;
        }

        // Swapping
        NUMERICS_INLINE
	    void swap (set_array &a) {
            check (this != &a, external_logic ());
            std::swap (capacity_, a.capacity_);
            std::swap (data_, a.data_);
            std::swap (size_, a.size_);
        }
#ifndef USE_GCC
        NUMERICS_INLINE
	    friend void swap (set_array &a1, set_array &a2) {
            a1.swap (a2);
        }
#endif

        // Element insertion and deletion
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        pointer insert (pointer it, const value_type &p) {
            if (size () == 0 || (*(it = end () - 1)) < p) {
                resize (size () + 1);
                *(it = end () - 1) = p;
                return it;
            } 
#ifdef NUMERICS_APPEND_ONLY
            throw external_logic ();
#else
            it = std::upper_bound (begin (), end (), p, std::less<value_type> ());
            check (it != end (), internal_logic ());
            check (*it != p, external_logic ());
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
        void insert (pointer it, pointer it1, pointer it2) {
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
            std::sort (begin (), end (), std::less<value_type> ()); 
#endif
        }
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void erase (pointer it) {
            check (begin () <= it && it < end (), bad_index ());
            std::copy (it + 1, end (), it);
            resize (size () - 1);
        }
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void erase (pointer it1, pointer it2) {
            check (begin () <= it1 && it1 < it2 && it2 <= end (), bad_index ());
            std::copy (it2, end (), it1);
            resize (size () - (it2 - it1));
        }
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void clear () {
            resize (0);
        }

        // Element lookup
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        const_pointer find (index_type i) const {
#ifdef NUMERICS_DEPRECATED
            std::pair<const_pointer, const_pointer> pit;
            pit = std::equal_range (begin (), end (), i, std::less<value_type> ());
            if (*pit.first == i)
                return pit.first;
            else if (*pit.second == i)
                return pit.second;
            else
                return end ();
#else
            const_pointer it (std::lower_bound (begin (), end (), i, std::less<value_type> ()));
			if (it == end () || *it != i)
				it = end ();
			return it;
#endif
        }
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        pointer find (index_type i) {
#ifdef NUMERICS_DEPRECATED
            std::pair<pointer, pointer> pit;
            pit = std::equal_range (begin (), end (), i, std::less<value_type> ());
            if (*pit.first == i)
                return pit.first;
            else if (*pit.second == i)
                return pit.second;
            else
                return end ();
#else
            pointer it (std::lower_bound (begin (), end (), i, std::less<value_type> ()));
			if (it == end () || *it != i)
				it = end ();
			return it;
#endif
        }
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        const_pointer lower_bound (index_type i) const {
            return std::lower_bound (begin (), end (), i, std::less<value_type> ());
        }
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        pointer lower_bound (index_type i) {
            return std::lower_bound (begin (), end (), i, std::less<value_type> ());
        }
#ifdef NUMERICS_DEPRECATED
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        const_pointer upper_bound (index_type i) const {
            return std::upper_bound (begin (), end (), i, std::less<value_type> ());
        }
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        pointer upper_bound (index_type i) {
            return std::upper_bound (begin (), end (), i, std::less<value_type> ());
        }
#endif

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

#ifdef USE_MSVC
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

#ifdef USE_MSVC
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
        size_type capacity_;
        pointer data_;
        size_type size_;
    };

    template<class I>
    NUMERICS_INLINE
    set_array<I> &assign_temporary (set_array<I> &a1, set_array<I> &a2) { 
        return a1.assign_temporary (a2);
    }

    template<class I, class F>
    NUMERICS_INLINE
    std::set<I, F> &assign_temporary (std::set<I, F> &a1, std::set<I, F> &a2) { 
        check (&a1 != &a2, external_logic ());
        a1.swap (a2);
        return  a1;
    }
    // This specialization is missing in Dinkumware's STL?!
    template<class I, class F>
    NUMERICS_INLINE
    void swap (std::set<I, F> &a1, std::set<I, F> &a2) { 
        check (&a1 != &a2, external_logic ());
        a1.swap (a2);
    }

}

#endif 


