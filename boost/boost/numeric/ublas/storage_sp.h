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

#ifndef NUMERICS_STORAGE_SP_H
#define NUMERICS_STORAGE_SP_H

#include <algorithm>
#include <map>
#include <set>

#include <boost/numeric/ublas/config.h>
#include <boost/numeric/ublas/exception.h>
#include <boost/numeric/ublas/iterator.h>
#include <boost/numeric/ublas/storage.h>

#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
// This fixes a [1] = a [0] = 1, but won't work on broken compilers.
// Thanks to Marc Duflot for spotting this.
#define NUMERICS_STRICT_SPARSE_ELEMENT_ASSIGN
#include <complex>
#endif

namespace boost { namespace numerics {

    template<class P>
    struct less {
        NUMERICS_INLINE
        bool operator () (const P &p1, const P &p2) {
            return p1.first < p2.first;
        }
    };

#ifdef NUMERICS_STRICT_SPARSE_ELEMENT_ASSIGN
    template<class D>
    struct inner_map_traits {
        typedef typename D::index_type index_type;
        typedef typename D::data_const_reference data_const_reference;
        typedef typename D::data_reference data_reference;
    };
    template<>
    struct inner_map_traits<float> {
        typedef std::size_t index_type;
        typedef void data_const_reference;
        typedef void data_reference;
    };
    template<>
    struct inner_map_traits<double> {
        typedef std::size_t index_type;
        typedef void data_const_reference;
        typedef void data_reference;
    };
#ifdef NUMERICS_USE_LONG_DOUBLE
    template<>
    struct inner_map_traits<long double> {
        typedef std::size_t index_type;
        typedef void data_const_reference;
        typedef void data_reference;
    };
#endif
    template<>
    struct inner_map_traits<std::complex<float> > {
        typedef std::size_t index_type;
        typedef void data_const_reference;
        typedef void data_reference;
    };
    template<>
    struct inner_map_traits<std::complex<double> > {
        typedef std::size_t index_type;
        typedef void data_const_reference;
        typedef void data_reference;
    };
#ifdef NUMERICS_USE_LONG_DOUBLE
    template<>
    struct inner_map_traits<std::complex<long double> > {
        typedef std::size_t index_type;
        typedef void data_const_reference;
        typedef void data_reference;
    };
#endif
#endif

    // Map array
    template<class I, class T>
    class map_array {
    public:
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef I index_type;
        typedef T data_value_type;
        typedef const T &data_const_reference;
#ifndef NUMERICS_STRICT_SPARSE_ELEMENT_ASSIGN
        typedef T &data_reference;
#else
        class proxy;
        typedef proxy data_reference;
#endif
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

#ifdef NUMERICS_STRICT_SPARSE_ELEMENT_ASSIGN
        class proxy:
            public container_reference<map_array> {
        public:
#ifdef BOOST_MSVC
            typedef I index_type;
            typedef T data_value_type;
            typedef const T &data_const_reference;
            typedef T &data_reference;
            typedef std::pair<I, T> *pointer;
#else
            typedef typename map_array::index_type index_type;
            typedef typename map_array::data_value_type data_value_type;
            typedef const typename map_array::data_value_type &data_const_reference;
            typedef typename map_array::data_value_type &data_reference;
            typedef std::pair<index_type, data_value_type> *pointer;
#endif

            // Construction and destruction
            NUMERICS_INLINE
            proxy (map_array &a, pointer it):
                container_reference<map_array> (a), it_ (it), i_ (it->first), d_ (it->second) {}
            NUMERICS_INLINE
            proxy (map_array &a, index_type i):
                container_reference<map_array> (a), it_ (), i_ (i), d_ () {
                pointer it = (*this) ().find (i_);
                if (it == (*this) ().end ())
                    it = (*this) ().insert ((*this) ().end (), value_type (i_, d_));
                d_ = it->second;
            }
            NUMERICS_INLINE
            ~proxy () {
                if (! it_)
                    it_ = (*this) ().find (i_);
                it_->second = d_;
            }

            // Element access
            // FIXME: GCC 3.1 warn's, if enabled
            // NUMERICS_INLINE
            // const inner_map_traits<data_value_type>::data_const_reference
            // operator [] (typename inner_map_traits<data_value_type>::index_type i) const {
            //     return d_ [i];
            // }
            NUMERICS_INLINE
            typename inner_map_traits<data_value_type>::data_reference
            operator [] (typename inner_map_traits<data_value_type>::index_type i) {
                return d_ [i];
            }

            // Assignment
            template<class D>
            NUMERICS_INLINE
            proxy &operator = (const D &d) {
                d_ = d;
                return *this;
            }
            NUMERICS_INLINE
            proxy &operator = (const proxy &p) {
                d_ = p.d_;
                return *this;
            }
            template<class D>
            NUMERICS_INLINE
            proxy &operator += (const D &d) {
                d_ += d;
                return *this;
            }
            NUMERICS_INLINE
            proxy &operator += (const proxy &p) {
                d_ += p.d_;
                return *this;
            }
            template<class D>
            NUMERICS_INLINE
            proxy &operator -= (const D &d) {
                d_ -= d;
                return *this;
            }
            NUMERICS_INLINE
            proxy &operator -= (const proxy &p) {
                d_ -= p.d_;
                return *this;
            }
            template<class D>
            NUMERICS_INLINE
            proxy &operator *= (const D &d) {
                d_ *= d;
                return *this;
            }
            NUMERICS_INLINE
            proxy &operator *= (const proxy &p) {
                d_ *= p.d_;
                return *this;
            }
            template<class D>
            NUMERICS_INLINE
            proxy &operator /= (const D &d) {
                d_ /= d;
                return *this;
            }
            NUMERICS_INLINE
            proxy &operator /= (const proxy &p) {
                d_ /= p.d_;
                return *this;
            }

            // Conversion
            // FIXME: GCC 3.1 warn's, if enabled
            //  NUMERICS_INLINE
            //  operator const data_const_reference () const {
            //      return d_;
            //  }
            NUMERICS_INLINE
            operator data_reference () {
                return d_;
            }

        private:
            pointer it_;
            index_type i_;
            data_value_type d_;
        };
#endif

        // Element access
        NUMERICS_INLINE
        data_reference operator [] (index_type i) {
#ifndef NUMERICS_STRICT_SPARSE_ELEMENT_ASSIGN
            pointer it = find (i);
            if (it == end ())
                it = insert (end (), value_type (i, data_value_type ()));
            return it->second;
#else
            // This fixes a [1] = a [0] = 1.
            // Thanks to Marc Duflot for spotting this.
            return data_reference (*this, i);
#endif
        }

        // Assignment
        NUMERICS_INLINE
        map_array &operator = (const map_array &a) {
            // Too unusual semantic.
            // Thanks to Michael Stevens for spotting this.
            // check (this != &a, external_logic ());
            if (this != &a) {
                resize (a.size_);
                std::copy (a.data_, a.data_ + a.size_, data_);
            }
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
            // Too unusual semantic.
            // check (this != &a, external_logic ());
            if (this != &a) {
                std::swap (capacity_, a.capacity_);
                std::swap (data_, a.data_);
                std::swap (size_, a.size_);
            }
        }
#ifdef NUMERICS_FRIEND_FUNCTION
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
        // Too unusual semantic.
        // check (&a1 != &a2, external_logic ());
        if (&a1 != &a2) 
            a1.swap (a2);
        return  a1;
    }
    // This specialization is missing in Dinkumware's STL?!
    template<class I, class T, class F>
    NUMERICS_INLINE
    void swap (std::map<I, T, F> &a1, std::map<I, T, F> &a2) {
        // Too unusual semantic.
        // check (&a1 != &a2, external_logic ());
        if (&a1 != &a2)
            a1.swap (a2);
    }

#ifdef NUMERICS_STRICT_SPARSE_ELEMENT_ASSIGN
    template<class A>
    struct map_traits {
        typedef void proxy;
    };
    template<class I, class T>
    struct map_traits<std::map<I, T> > {
        typedef typename std::map<I, T>::mapped_type &reference;
        static reference make_reference (std::map<I, T> &a, typename std::map<I, T>::iterator it) {
            return (*it).second;
        }
    };
    template<class I, class T>
    struct map_traits<map_array<I, T> > {
        typedef typename map_array<I, T>::data_reference reference;
        static reference make_reference (map_array<I, T> &a, typename map_array<I, T>::iterator it) {
            return reference (a, it);
        }
    };
#endif

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
            // Too unusual semantic.
            // Thanks to Michael Stevens for spotting this.
            // check (this != &a, external_logic ());
            if (this != &a) {
                resize (a.size_);
                std::copy (a.data_, a.data_ + a.size_, data_);
            }
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
            // Too unusual semantic.
            // check (this != &a, external_logic ());
            if (this != &a) {
                std::swap (capacity_, a.capacity_);
                std::swap (data_, a.data_);
                std::swap (size_, a.size_);
            }
        }
#ifdef NUMERICS_FRIEND_FUNCTION
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
        // Too unusual semantic.
        // check (&a1 != &a2, external_logic ());
        if (&a1 != &a2)
            a1.swap (a2);
        return  a1;
    }
    // This specialization is missing in Dinkumware's STL?!
    template<class I, class F>
    NUMERICS_INLINE
    void swap (std::set<I, F> &a1, std::set<I, F> &a2) {
        // Too unusual semantic.
        // check (&a1 != &a2, external_logic ());
        if (&a1 != &a2)
            a1.swap (a2);
    }

}}

#endif


