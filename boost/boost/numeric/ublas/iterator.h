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

#ifndef NUMERICS_ITERATOR_H
#define NUMERICS_ITERATOR_H

#include <iterator>

#include "config.h"

// Using GCC the following is missing:
//
// namespace std {
//
//    template <class C, class T, class D = std::ptrdiff_t, class P = T *, class R = T &>
//    struct iterator {
//        typedef C iterator_category;
//        typedef T value_type;
//        typedef D difference_type;
//        typedef P pointer;
//        typedef R reference;
//    };
//
// }
//
// We therefore include the following header
#include <boost/iterator.hpp>
// and use namespace boost instead of std.

namespace boost { namespace numerics {

#ifdef NUMERICS_NEED_CONVERSION
    template<class C>
    class container_reference;
#endif

    template<class C>
    class container_const_reference {
    public:
        typedef C container_type;
#ifdef NUMERICS_NEED_CONVERSION
        typedef container_reference<container_type> container_reference;
#endif

        NUMERICS_INLINE
        container_const_reference ():
            c_ (0) {}
        NUMERICS_INLINE
        container_const_reference (const container_type &c):
            c_ (&c) {}
#ifdef NUMERICS_NEED_CONVERSION
        NUMERICS_INLINE
        container_const_reference (const container_reference &c):
            c_ (c.c ()) {}
#endif

        NUMERICS_INLINE
        const container_type &operator () () const {
            return *c_;
        }

        NUMERICS_INLINE
        container_const_reference &assign (const container_type *c) {
            c_ = c;
            return *this;
        }

    private:
        const container_type *c_;

        container_const_reference &operator = (const container_const_reference &) {
            return *this;
        }
    };

    template<class C>
    class container_reference {
    public:
        typedef C container_type;

        NUMERICS_INLINE
        container_reference ():
            c_ (0) {}
        NUMERICS_INLINE
        container_reference (container_type &c):
            c_ (&c) {}

        NUMERICS_INLINE
        container_type &operator () () const {
           return *c_;
        }

        NUMERICS_INLINE
        container_reference &assign (container_type *c) {
            c_ = c;
            return *this;
        }

    private:
        container_type *c_;

        container_reference operator = (const container_reference &) {
            return *this;
        }
    };

    template<class IC, class I, class T>
    struct forward_iterator_base:
        public boost::iterator<IC, T> {
        typedef I derived_iterator_type;
        typedef T derived_value_type;
        typedef const T &derived_const_reference;
        typedef T &derived_reference;

        // Arithmetic
        NUMERICS_INLINE
        derived_iterator_type operator ++ (int) {
            derived_iterator_type &d (*static_cast<const derived_iterator_type *> (this));
            derived_iterator_type tmp (d);
            ++ d;
            return tmp;
        }
#ifdef NUMERICS_FRIEND_FUNCTION
        NUMERICS_INLINE
        friend derived_iterator_type operator ++ (derived_iterator_type &d, int) {
            derived_iterator_type tmp (d);
            ++ d;
            return tmp;
        }
#endif

        // Comparison
        NUMERICS_INLINE
        bool operator != (const derived_iterator_type &it) const {
            const derived_iterator_type *d = static_cast<const derived_iterator_type *> (this);
            return ! (*d == it);
        }
    };

#ifndef NUMERICS_FRIEND_FUNCTION
    template<class IC, class I, class T>
    NUMERICS_INLINE
    typename forward_iterator_base<IC, I, T>::derived_iterator_type operator ++ (forward_iterator_base<IC, I, T> &it, int) {
        typedef NUMERICS_TYPENAME forward_iterator_base<IC, I, T>::derived_iterator_type derived_iterator_type;
        derived_iterator_type &d (static_cast<derived_iterator_type &> (it));
        derived_iterator_type tmp (d);
        ++ d;
        return tmp;
    }
#endif

    template<class IC, class I, class T>
    struct bidirectional_iterator_base:
        public boost::iterator<IC, T> {
        typedef I derived_iterator_type;
        typedef T derived_value_type;
        typedef const T &derived_const_reference;
        typedef T &derived_reference;

        // Arithmetic
        NUMERICS_INLINE
        derived_iterator_type operator ++ (int) {
            derived_iterator_type &d (*static_cast<const derived_iterator_type *> (this));
            derived_iterator_type tmp (d);
            ++ d;
            return tmp;
        }
#ifdef NUMERICS_FRIEND_FUNCTION
        NUMERICS_INLINE
        friend derived_iterator_type operator ++ (derived_iterator_type &d, int) {
            derived_iterator_type tmp (d);
            ++ d;
            return tmp;
        }
#endif
        NUMERICS_INLINE
        derived_iterator_type operator -- (int) {
            derived_iterator_type &d (*static_cast<const derived_iterator_type *> (this));
            derived_iterator_type tmp (d);
            -- d;
            return tmp;
        }
#ifdef NUMERICS_FRIEND_FUNCTION
        NUMERICS_INLINE
        friend derived_iterator_type operator -- (derived_iterator_type &d, int) {
            derived_iterator_type tmp (d);
            -- d;
            return tmp;
        }
#endif

        // Comparison
        NUMERICS_INLINE
        bool operator != (const derived_iterator_type &it) const {
            const derived_iterator_type *d = static_cast<const derived_iterator_type *> (this);
            return ! (*d == it);
        }
    };

#ifndef NUMERICS_FRIEND_FUNCTION
    template<class IC, class I, class T>
    NUMERICS_INLINE
    typename bidirectional_iterator_base<IC, I, T>::derived_iterator_type operator ++ (bidirectional_iterator_base<IC, I, T> &it, int) {
        typedef NUMERICS_TYPENAME bidirectional_iterator_base<IC, I, T>::derived_iterator_type derived_iterator_type;
        derived_iterator_type &d (static_cast<derived_iterator_type &> (it));
        derived_iterator_type tmp (d);
        ++ d;
        return tmp;
    }
    template<class IC, class I, class T>
    NUMERICS_INLINE
    typename bidirectional_iterator_base<IC, I, T>::derived_iterator_type operator -- (bidirectional_iterator_base<IC, I, T> &it, int) {
        typedef NUMERICS_TYPENAME bidirectional_iterator_base<IC, I, T>::derived_iterator_type derived_iterator_type;
        derived_iterator_type &d (static_cast<derived_iterator_type &> (it));
        derived_iterator_type tmp (d);
        -- d;
        return tmp;
    }
#endif

    template<class IC, class I, class T, class D = std::ptrdiff_t>
    struct random_access_iterator_base:
        public boost::iterator<IC, T> {
        typedef I derived_iterator_type;
        typedef T derived_value_type;
        typedef const T &derived_const_reference;
        typedef T &derived_reference;
        typedef D derived_difference_type;
#ifdef BOOST_MSVC_STD_ITERATOR
        typedef D difference_type;
#endif

        // Arithmetic
        NUMERICS_INLINE
        derived_iterator_type operator ++ (int) {
            derived_iterator_type &d (*static_cast<derived_iterator_type *> (this));
            derived_iterator_type tmp (d);
            ++ d;
            return tmp;
        }
#ifdef NUMERICS_FRIEND_FUNCTION
        NUMERICS_INLINE
        friend derived_iterator_type operator ++ (derived_iterator_type &d, int) {
            derived_iterator_type tmp (d);
            ++ d;
            return tmp;
        }
#endif
        NUMERICS_INLINE
        derived_iterator_type operator -- (int) {
            derived_iterator_type &d (*static_cast<derived_iterator_type *> (this));
            derived_iterator_type tmp (d);
            -- d;
            return tmp;
        }
#ifdef NUMERICS_FRIEND_FUNCTION
        NUMERICS_INLINE
        friend derived_iterator_type operator -- (derived_iterator_type &d, int) {
            derived_iterator_type tmp (d);
            -- d;
            return tmp;
        }
#endif
        NUMERICS_INLINE
        derived_iterator_type operator + (derived_difference_type n) const {
            derived_iterator_type tmp (*static_cast<const derived_iterator_type *> (this));
            return tmp += n;
        }
#ifdef NUMERICS_FRIEND_FUNCTION
        NUMERICS_INLINE
        friend derived_iterator_type operator + (const derived_iterator_type &d, derived_difference_type n) {
            derived_iterator_type tmp (d);
            return tmp += n;
        }
#endif
        NUMERICS_INLINE
        derived_iterator_type operator - (derived_difference_type n) const {
            derived_iterator_type tmp (*static_cast<const derived_iterator_type *> (this));
            return tmp -= n;
        }
#ifdef NUMERICS_FRIEND_FUNCTION
        NUMERICS_INLINE
        friend derived_iterator_type operator - (const derived_iterator_type &d, derived_difference_type n) {
            derived_iterator_type tmp (d);
            return tmp -= n;
        }
#endif

        // Comparison
        NUMERICS_INLINE
        bool operator != (const derived_iterator_type &it) const {
            const derived_iterator_type *d = static_cast<const derived_iterator_type *> (this);
            return ! (*d == it);
        }
    };

#ifndef NUMERICS_FRIEND_FUNCTION
    template<class IC, class I, class T>
    NUMERICS_INLINE
    typename random_access_iterator_base<IC, I, T>::derived_iterator_type operator ++ (random_access_iterator_base<IC, I, T> &it, int) {
        typedef NUMERICS_TYPENAME random_access_iterator_base<IC, I, T>::derived_iterator_type derived_iterator_type;
        derived_iterator_type &d (static_cast<derived_iterator_type &> (it));
        derived_iterator_type tmp (d);
        ++ d;
        return tmp;
    }
    template<class IC, class I, class T>
    NUMERICS_INLINE
    typename random_access_iterator_base<IC, I, T>::derived_iterator_type operator -- (random_access_iterator_base<IC, I, T> &it, int) {
        typedef NUMERICS_TYPENAME random_access_iterator_base<IC, I, T>::derived_iterator_type derived_iterator_type;
        derived_iterator_type &d (static_cast<derived_iterator_type &> (it));
        derived_iterator_type tmp (d);
        -- d;
        return tmp;
    }
    template<class IC, class I, class T>
    NUMERICS_INLINE
    typename random_access_iterator_base<IC, I, T>::derived_iterator_type operator + (const random_access_iterator_base<IC, I, T> &it, std::ptrdiff_t n) {
        typedef NUMERICS_TYPENAME random_access_iterator_base<IC, I, T>::derived_iterator_type derived_iterator_type;
        derived_iterator_type tmp (static_cast<const derived_iterator_type &> (it));
        return tmp += n;
    }
    template<class IC, class I, class T>
    NUMERICS_INLINE
    typename random_access_iterator_base<IC, I, T>::derived_iterator_type operator - (const random_access_iterator_base<IC, I, T> &it, std::ptrdiff_t n) {
        typedef NUMERICS_TYPENAME random_access_iterator_base<IC, I, T>::derived_iterator_type derived_iterator_type;
        derived_iterator_type tmp (static_cast<const derived_iterator_type &> (it));
        return tmp -= n;
    }
#endif

#ifdef BOOST_MSVC_STD_ITERATOR

    template <class I, class T, class R>
    class reverse_iterator:
        public std::reverse_bidirectional_iterator<I, T, R> {
    public:
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef I iterator_type;
        typedef T value_type;
        typedef R reference;
        typedef typename I::container_type container_type;

        // Construction and destruction
        NUMERICS_INLINE
        reverse_iterator ():
            std::reverse_bidirectional_iterator<iterator_type, value_type, reference> () {}
        NUMERICS_INLINE
        reverse_iterator (const iterator_type &it):
            std::reverse_bidirectional_iterator<iterator_type, value_type, reference> (it) {}

        // Arithmetic
        NUMERICS_INLINE
        reverse_iterator &operator += (difference_type n) {
            // Comeau recommends...
            return *this = this->base () - n;
        }
        NUMERICS_INLINE
        reverse_iterator &operator -= (difference_type n) {
            // Comeau recommends...
            return *this = this->base () + n;
        }

        NUMERICS_INLINE
        const container_type &operator () () const {
            // Comeau recommends...
            return this->base () ();
        }

        NUMERICS_INLINE
        size_type index () const {
            // Comeau recommends...
            iterator_type tmp (this->base ());
            return (-- tmp).index ();
        }
    };

    template<class I, class T, class R>
    NUMERICS_INLINE
    reverse_iterator<I, T, R> operator + (const reverse_iterator<I, T, R> &it, std::ptrdiff_t n) {
        reverse_iterator<I, T, R> tmp (it);
        return tmp += n;
    }
    template<class I, class T, class R>
    NUMERICS_INLINE
    reverse_iterator<I, T, R> operator - (const reverse_iterator<I, T, R> &it, std::ptrdiff_t n) {
        reverse_iterator<I, T, R> tmp (it);
        return tmp -= n;
    }
    template<class I, class T, class R>
    NUMERICS_INLINE
    std::ptrdiff_t operator - (const reverse_iterator<I, T, R> &it1, const reverse_iterator<I, T, R> &it2) {
        return it2.base () - it1.base ();
    }

    template <class I, class T, class R>
    class reverse_iterator1:
        public std::reverse_bidirectional_iterator<I, T, R> {
    public:
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef I iterator_type;
        typedef T value_type;
        typedef R reference;
        typedef typename I::container_type container_type;
#ifndef NUMERICS_USE_CANONICAL_ITERATOR
        typedef typename I::dual_iterator_type dual_iterator_type;
        typedef typename I::dual_reverse_iterator_type dual_reverse_iterator_type;
#endif

        // Construction and destruction
        NUMERICS_INLINE
        reverse_iterator1 ():
            std::reverse_bidirectional_iterator<iterator_type, value_type, reference> () {}
        NUMERICS_INLINE
        reverse_iterator1 (const iterator_type &it):
            std::reverse_bidirectional_iterator<iterator_type, value_type, reference> (it) {}

        // Arithmetic
        NUMERICS_INLINE
        reverse_iterator1 &operator += (difference_type n) {
            // Comeau recommends...
            return *this = this->base () - n;
        }
        NUMERICS_INLINE
        reverse_iterator1 &operator -= (difference_type n) {
            // Comeau recommends...
            return *this = this->base () + n;
        }

        NUMERICS_INLINE
        const container_type &operator () () const {
            // Comeau recommends...
            return this->base () ();
        }

        NUMERICS_INLINE
        size_type index1 () const {
            // Comeau recommends...
            iterator_type tmp (this->base ());
            return (-- tmp).index1 ();
        }
        NUMERICS_INLINE
        size_type index2 () const {
            // Comeau recommends...
            iterator_type tmp (this->base ());
            return (-- tmp).index2 ();
        }

#ifndef NUMERICS_USE_CANONICAL_ITERATOR
        NUMERICS_INLINE
        dual_iterator_type begin () const {
            // Comeau recommends...
            return this->base ().begin ();
        }
        NUMERICS_INLINE
        dual_iterator_type end () const {
            // Comeau recommends...
            return this->base ().end ();
        }
        NUMERICS_INLINE
        dual_reverse_iterator_type rbegin () const {
            return dual_reverse_iterator_type (end ());
        }
        NUMERICS_INLINE
        dual_reverse_iterator_type rend () const {
            return dual_reverse_iterator_type (begin ());
        }
#endif
    };

    template<class I, class T, class R>
    NUMERICS_INLINE
    reverse_iterator1<I, T, R> operator + (const reverse_iterator1<I, T, R> &it, std::ptrdiff_t n) {
        reverse_iterator1<I, T, R> tmp (it);
        return tmp += n;
    }
    template<class I, class T, class R>
    NUMERICS_INLINE
    reverse_iterator1<I, T, R> operator - (const reverse_iterator1<I, T, R> &it, std::ptrdiff_t n) {
        reverse_iterator1<I, T, R> tmp (it);
        return tmp -= n;
    }
    template<class I, class T, class R>
    NUMERICS_INLINE
    std::ptrdiff_t operator - (const reverse_iterator1<I, T, R> &it1, const reverse_iterator1<I, T, R> &it2) {
        return it2.base () - it1.base ();
    }

    template <class I, class T, class R>
    class reverse_iterator2:
        public std::reverse_bidirectional_iterator<I, T, R> {
    public:
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef I iterator_type;
        typedef T value_type;
        typedef R reference;
        typedef typename I::container_type container_type;
#ifndef NUMERICS_USE_CANONICAL_ITERATOR
        typedef typename I::dual_iterator_type dual_iterator_type;
        typedef typename I::dual_reverse_iterator_type dual_reverse_iterator_type;
#endif

        // Construction and destruction
        NUMERICS_INLINE
        reverse_iterator2 ():
            std::reverse_bidirectional_iterator<iterator_type, value_type, reference> () {}
        NUMERICS_INLINE
        reverse_iterator2 (const iterator_type &it):
            std::reverse_bidirectional_iterator<iterator_type, value_type, reference> (it) {}

        // Arithmetic
        NUMERICS_INLINE
        reverse_iterator2 &operator += (difference_type n) {
            // Comeau recommends...
            return *this = this->base () - n;
        }
        NUMERICS_INLINE
        reverse_iterator2 &operator -= (difference_type n) {
            // Comeau recommends...
            return *this = this->base () + n;
        }

        NUMERICS_INLINE
        const container_type &operator () () const {
            // Comeau recommends...
            return this->base () ();
        }

        NUMERICS_INLINE
        size_type index1 () const {
            // Comeau recommends...
            iterator_type tmp (this->base ());
            return (-- tmp).index1 ();
        }
        NUMERICS_INLINE
        size_type index2 () const {
            // Comeau recommends...
            iterator_type tmp (this->base ());
            return (-- tmp).index2 ();
        }

#ifndef NUMERICS_USE_CANONICAL_ITERATOR
        NUMERICS_INLINE
        dual_iterator_type begin () const {
            // Comeau recommends...
            return this->base ().begin ();
        }
        NUMERICS_INLINE
        dual_iterator_type end () const {
            // Comeau recommends...
            return this->base ().end ();
        }
        NUMERICS_INLINE
        dual_reverse_iterator_type rbegin () const {
            return dual_reverse_iterator_type (end ());
        }
        NUMERICS_INLINE
        dual_reverse_iterator_type rend () const {
            return dual_reverse_iterator_type (begin ());
        }
#endif
    };

    template<class I, class T, class R>
    NUMERICS_INLINE
    reverse_iterator2<I, T, R> operator + (const reverse_iterator2<I, T, R> &it, std::ptrdiff_t n) {
        reverse_iterator2<I, T, R> tmp (it);
        return tmp += n;
    }
    template<class I, class T, class R>
    NUMERICS_INLINE
    reverse_iterator2<I, T, R> operator - (const reverse_iterator2<I, T, R> &it, std::ptrdiff_t n) {
        reverse_iterator2<I, T, R> tmp (it);
        return tmp -= n;
    }
    template<class I, class T, class R>
    NUMERICS_INLINE
    std::ptrdiff_t operator - (const reverse_iterator2<I, T, R> &it1, const reverse_iterator2<I, T, R> &it2) {
        return it2.base () - it1.base ();
    }

#else

    template <class I>
    class reverse_iterator:
        public std::reverse_iterator<I> {
    public:
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef I iterator_type;
        typedef typename I::container_type container_type;

        // Construction and destruction
        NUMERICS_INLINE
        reverse_iterator ():
            std::reverse_iterator<iterator_type> () {}
        NUMERICS_INLINE
        reverse_iterator (const iterator_type &it):
            std::reverse_iterator<iterator_type> (it) {}

#ifdef NUMERICS_REVERSE_ITERATOR_OVERLOADS
        // Arithmetic
        NUMERICS_INLINE
        reverse_iterator &operator ++ () {
            // Comeau recommends...
            return *this = -- this->base ();
        }
        NUMERICS_INLINE
        reverse_iterator operator ++ (int) {
            // Comeau recommends...
            reverse_iterator tmp (*this);
            *this = -- this->base ();
            return tmp;
        }
        NUMERICS_INLINE
        reverse_iterator &operator -- () {
            // Comeau recommends...
            return *this = ++ this->base ();
        }
        NUMERICS_INLINE
        reverse_iterator operator -- (int) {
            // Comeau recommends...
            reverse_iterator tmp (*this);
            *this = ++ this->base ();
            return tmp;
        }
        NUMERICS_INLINE
        reverse_iterator &operator += (difference_type n) {
            // Comeau recommends...
            return *this = this->base () - n;
        }
        NUMERICS_INLINE
        reverse_iterator &operator -= (difference_type n) {
            // Comeau recommends...
            return *this = this->base () + n;
        }
#endif

#ifdef NUMERICS_FRIEND_FUNCTION
        NUMERICS_INLINE
        friend reverse_iterator operator + (const reverse_iterator &it, difference_type n) {
            reverse_iterator tmp (it);
            return tmp += n;
        }
        NUMERICS_INLINE
        friend reverse_iterator operator - (const reverse_iterator &it, difference_type n) {
            reverse_iterator tmp (it);
            return tmp -= n;
        }
        NUMERICS_INLINE
        friend difference_type operator - (const reverse_iterator &it1, const reverse_iterator &it2) {
            return it2.base () - it1.base ();
        }
#endif

        NUMERICS_INLINE
        const container_type &operator () () const {
            // Comeau recommends...
            return this->base () ();
        }

        NUMERICS_INLINE
        size_type index () const {
            // Comeau recommends...
            iterator_type tmp (this->base ());
            return (-- tmp).index ();
        }
    };

#ifndef NUMERICS_FRIEND_FUNCTION
    template<class I>
    NUMERICS_INLINE
    reverse_iterator<I> operator + (const reverse_iterator<I> &it, std::ptrdiff_t n) {
        reverse_iterator<I> tmp (it);
        return tmp += n;
    }
    template<class I>
    NUMERICS_INLINE
    reverse_iterator<I> operator - (const reverse_iterator<I> &it, std::ptrdiff_t n) {
        reverse_iterator<I> tmp (it);
        return tmp -= n;
    }
    template<class I>
    NUMERICS_INLINE
    std::ptrdiff_t operator - (const reverse_iterator<I> &it1, const reverse_iterator<I> &it2) {
        return it2.base () - it1.base ();
    }
#endif

    template <class I>
    class reverse_iterator1:
        public std::reverse_iterator<I> {
    public:
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef I iterator_type;
        typedef typename I::container_type container_type;
#ifndef NUMERICS_USE_CANONICAL_ITERATOR
        typedef typename I::dual_iterator_type dual_iterator_type;
        typedef typename I::dual_reverse_iterator_type dual_reverse_iterator_type;
#endif

        // Construction and destruction
        NUMERICS_INLINE
        reverse_iterator1 ():
            std::reverse_iterator<iterator_type> () {}
        NUMERICS_INLINE
        reverse_iterator1 (const iterator_type &it):
            std::reverse_iterator<iterator_type> (it) {}

#ifdef NUMERICS_REVERSE_ITERATOR_OVERLOADS
        // Arithmetic
        NUMERICS_INLINE
        reverse_iterator1 &operator ++ () {
            // Comeau recommends...
            return *this = -- this->base ();
        }
        NUMERICS_INLINE
        reverse_iterator1 operator ++ (int) {
            // Comeau recommends...
            reverse_iterator1 tmp (*this);
            *this = -- this->base ();
            return tmp;
        }
        NUMERICS_INLINE
        reverse_iterator1 &operator -- () {
            // Comeau recommends...
            return *this = ++ this->base ();
        }
        NUMERICS_INLINE
        reverse_iterator1 operator -- (int) {
            // Comeau recommends...
            reverse_iterator1 tmp (*this);
            *this = ++ this->base ();
            return tmp;
        }
        NUMERICS_INLINE
        reverse_iterator1 &operator += (difference_type n) {
            // Comeau recommends...
            return *this = this->base () - n;
        }
        NUMERICS_INLINE
        reverse_iterator1 &operator -= (difference_type n) {
            // Comeau recommends...
            return *this = this->base () + n;
        }
#endif

#ifdef NUMERICS_FRIEND_FUNCTION
        NUMERICS_INLINE
        friend reverse_iterator1 operator + (const reverse_iterator1 &it, difference_type n) {
            reverse_iterator1 tmp (it);
            return tmp += n;
        }
        NUMERICS_INLINE
        friend reverse_iterator1 operator - (const reverse_iterator1 &it, difference_type n) {
            reverse_iterator1 tmp (it);
            return tmp -= n;
        }
        NUMERICS_INLINE
        friend difference_type operator - (const reverse_iterator1 &it1, const reverse_iterator1 &it2) {
            return it2.base () - it1.base ();
        }
#endif

        NUMERICS_INLINE
        const container_type &operator () () const {
            // Comeau recommends...
            return this->base () ();
        }

        NUMERICS_INLINE
        size_type index1 () const {
            // Comeau recommends...
            iterator_type tmp (this->base ());
            return (-- tmp).index1 ();
        }
        NUMERICS_INLINE
        size_type index2 () const {
            // Comeau recommends...
            iterator_type tmp (this->base ());
            return (-- tmp).index2 ();
        }

#ifndef NUMERICS_USE_CANONICAL_ITERATOR
        NUMERICS_INLINE
        dual_iterator_type begin () const {
            // Comeau recommends...
            return this->base ().begin ();
        }
        NUMERICS_INLINE
        dual_iterator_type end () const {
            // Comeau recommends...
            return this->base ().end ();
        }
        NUMERICS_INLINE
        dual_reverse_iterator_type rbegin () const {
            return dual_reverse_iterator_type (end ());
        }
        NUMERICS_INLINE
        dual_reverse_iterator_type rend () const {
            return dual_reverse_iterator_type (begin ());
        }
#endif
    };

#ifndef NUMERICS_FRIEND_FUNCTION
    template<class I>
    NUMERICS_INLINE
    reverse_iterator1<I> operator + (const reverse_iterator1<I> &it, std::ptrdiff_t n) {
        reverse_iterator1<I> tmp (it);
        return tmp += n;
    }
    template<class I>
    NUMERICS_INLINE
    reverse_iterator1<I> operator - (const reverse_iterator1<I> &it, std::ptrdiff_t n) {
        reverse_iterator1<I> tmp (it);
        return tmp -= n;
    }
    template<class I>
    NUMERICS_INLINE
    std::ptrdiff_t operator - (const reverse_iterator1<I> &it1, const reverse_iterator1<I> &it2) {
        return it2.base () - it1.base ();
    }
#endif

    template <class I>
    class reverse_iterator2:
        public std::reverse_iterator<I> {
    public:
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef I iterator_type;
        typedef typename I::container_type container_type;
#ifndef NUMERICS_USE_CANONICAL_ITERATOR
        typedef typename I::dual_iterator_type dual_iterator_type;
        typedef typename I::dual_reverse_iterator_type dual_reverse_iterator_type;
#endif

        // Construction and destruction
        NUMERICS_INLINE
        reverse_iterator2 ():
            std::reverse_iterator<iterator_type> () {}
        NUMERICS_INLINE
        reverse_iterator2 (const iterator_type &it):
            std::reverse_iterator<iterator_type> (it) {}

#ifdef NUMERICS_REVERSE_ITERATOR_OVERLOADS
        // Arithmetic
        NUMERICS_INLINE
        reverse_iterator2 &operator ++ () {
            // Comeau recommends...
            return *this = -- this->base ();
        }
        NUMERICS_INLINE
        reverse_iterator2 operator ++ (int) {
            // Comeau recommends...
            reverse_iterator2 tmp (*this);
            *this = -- this->base ();
            return tmp;
        }
        NUMERICS_INLINE
        reverse_iterator2 &operator -- () {
            // Comeau recommends...
            return *this = ++ this->base ();
        }
        NUMERICS_INLINE
        reverse_iterator2 operator -- (int) {
            // Comeau recommends...
            reverse_iterator2 tmp (*this);
            *this = ++ this->base ();
            return tmp;
        }
        NUMERICS_INLINE
        reverse_iterator2 &operator += (difference_type n) {
            // Comeau recommends...
            return *this = this->base () - n;
        }
        NUMERICS_INLINE
        reverse_iterator2 &operator -= (difference_type n) {
            // Comeau recommends...
            return *this = this->base () + n;
        }
#endif

#ifdef NUMERICS_FRIEND_FUNCTION
        NUMERICS_INLINE
        friend reverse_iterator2 operator + (const reverse_iterator2 &it, difference_type n) {
            reverse_iterator2 tmp (it);
            return tmp += n;
        }
        NUMERICS_INLINE
        friend reverse_iterator2 operator - (const reverse_iterator2 &it, difference_type n) {
            reverse_iterator2 tmp (it);
            return tmp -= n;
        }
        NUMERICS_INLINE
        friend difference_type operator - (const reverse_iterator2 &it1, const reverse_iterator2 &it2) {
            return it2.base () - it1.base ();
        }
#endif

        NUMERICS_INLINE
        const container_type &operator () () const {
            // Comeau recommends...
            return this->base () ();
        }

        NUMERICS_INLINE
        size_type index1 () const {
            // Comeau recommends...
            iterator_type tmp (this->base ());
            return (-- tmp).index1 ();
        }
        NUMERICS_INLINE
        size_type index2 () const {
            // Comeau recommends...
            iterator_type tmp (this->base ());
            return (-- tmp).index2 ();
        }

#ifndef NUMERICS_USE_CANONICAL_ITERATOR
        NUMERICS_INLINE
        dual_iterator_type begin () const {
            // Comeau recommends...
            return this->base ().begin ();
        }
        NUMERICS_INLINE
        dual_iterator_type end () const {
            // Comeau recommends...
            return this->base ().end ();
        }
        NUMERICS_INLINE
        dual_reverse_iterator_type rbegin () const {
            return dual_reverse_iterator_type (end ());
        }
        NUMERICS_INLINE
        dual_reverse_iterator_type rend () const {
            return dual_reverse_iterator_type (begin ());
        }
#endif
    };

#ifndef NUMERICS_FRIEND_FUNCTION
    template<class I>
    NUMERICS_INLINE
    reverse_iterator2<I> operator + (const reverse_iterator2<I> &it, std::ptrdiff_t n) {
        reverse_iterator2<I> tmp (it);
        return tmp += n;
    }
    template<class I>
    NUMERICS_INLINE
    reverse_iterator2<I> operator - (const reverse_iterator2<I> &it, std::ptrdiff_t n) {
        reverse_iterator2<I> tmp (it);
        return tmp -= n;
    }
    template<class I>
    NUMERICS_INLINE
    std::ptrdiff_t operator - (const reverse_iterator2<I> &it1, const reverse_iterator2<I> &it2) {
        return it2.base () - it1.base ();
    }
#endif

#endif

    template<class C, class IC>
    class indexed_iterator:
        public container_reference<C>,
        public random_access_iterator_base<IC,
                                           indexed_iterator<C, IC>,
                                           typename C::value_type,
                                           typename C::difference_type> {
    public:
        typedef C container_type;
        typedef IC iterator_category;
        typedef typename container_type::size_type size_type;
        typedef typename container_type::difference_type difference_type;
        typedef typename container_type::value_type value_type;
        typedef typename container_type::reference reference;
        typedef typename container_type::pointer pointer;

        // Construction and destruction
        NUMERICS_INLINE
        indexed_iterator ():
            container_reference<container_type> (), it_ () {}
        NUMERICS_INLINE
        indexed_iterator (container_type &c, size_type it):
            container_reference<container_type> (c), it_ (it) {}

        // Arithmetic
        NUMERICS_INLINE
        indexed_iterator &operator ++ () {
            ++ it_;
            return *this;
        }
        NUMERICS_INLINE
        indexed_iterator &operator -- () {
            -- it_;
            return *this;
        }
        NUMERICS_INLINE
        indexed_iterator &operator += (difference_type n) {
            it_ += n;
            return *this;
        }
        NUMERICS_INLINE
        indexed_iterator &operator -= (difference_type n) {
            it_ -= n;
            return *this;
        }
        NUMERICS_INLINE
        difference_type operator - (const indexed_iterator &it) const {
            return it_ - it.it_;
        }

        // Dereference
        NUMERICS_INLINE
        reference operator * () const {
            check (index () < (*this) ().size (), bad_index ());
            return (*this) () (it_);
        }
        NUMERICS_INLINE
        reference operator [] (difference_type n) const {
            return *((*this) + n);
        }

        // Index
        NUMERICS_INLINE
        size_type index () const {
            return it_;
        }

        // Assignment 
        NUMERICS_INLINE
        indexed_iterator &operator = (const indexed_iterator &it) {
            assign (&it ());
            it_ = it.it_;
            return *this;
        }

        // Comparison
        NUMERICS_INLINE
        bool operator == (const indexed_iterator &it) const {
#ifndef NUMERICS_USE_CANONICAL_ITERATOR
            check (&(*this) () == &it (), external_logic ());
#endif
            return it_ == it.it_;
        }

    private:
        size_type it_;
    };

#ifdef BOOST_MSVC_STD_ITERATOR
    template<class C, class I>
    NUMERICS_INLINE
    indexed_iterator<C, I> operator ++ (const indexed_iterator<C, I> &it, int) {
        indexed_iterator<C, I> tmp (it);
        ++ tmp;
        return tmp;
    }
    template<class C, class I>
    NUMERICS_INLINE
    indexed_iterator<C, I> operator -- (const indexed_iterator<C, I> &it, int) {
        indexed_iterator<C, I> tmp (it);
        -- tmp;
        return tmp;
    }
    template<class C, class I>
    NUMERICS_INLINE
    indexed_iterator<C, I> operator + (const indexed_iterator<C, I> &it, std::ptrdiff_t n) {
        indexed_iterator<C, I> tmp (it);
        return tmp += n;
    }
    template<class C, class I>
    NUMERICS_INLINE
    indexed_iterator<C, I> operator - (const indexed_iterator<C, I> &it, std::ptrdiff_t n) {
        indexed_iterator<C, I> tmp (it);
        return tmp -= n;
    }
#endif

    template<class C, class IC>
    class indexed_const_iterator:
        public container_const_reference<C>,
        public random_access_iterator_base<IC,
                                           indexed_const_iterator<C, IC>, 
                                           typename C::value_type,
                                           typename C::difference_type> {
    public:
        typedef C container_type;
        typedef IC iterator_category;
        typedef typename container_type::size_type size_type;
        typedef typename container_type::difference_type difference_type;
        typedef typename container_type::value_type value_type;
        typedef typename container_type::value_type reference;
        typedef typename container_type::const_pointer pointer;
        typedef indexed_iterator<container_type, iterator_category> iterator_type;

        // Construction and destruction
        NUMERICS_INLINE
        indexed_const_iterator ():
            container_const_reference<container_type> (), it_ () {}
        NUMERICS_INLINE
        indexed_const_iterator (const container_type &c, size_type it):
            container_const_reference<container_type> (c), it_ (it) {}
        NUMERICS_INLINE 
        indexed_const_iterator (const iterator_type &it):
            container_const_reference<container_type> (it ()), it_ (it.index ()) {}

        // Arithmetic
        NUMERICS_INLINE
        indexed_const_iterator &operator ++ () {
            ++ it_;
            return *this;
        }
        NUMERICS_INLINE
        indexed_const_iterator &operator -- () {
            -- it_;
            return *this;
        }
        NUMERICS_INLINE
        indexed_const_iterator &operator += (difference_type n) {
            it_ += n;
            return *this;
        }
        NUMERICS_INLINE
        indexed_const_iterator &operator -= (difference_type n) {
            it_ -= n;
            return *this;
        }
        NUMERICS_INLINE
        difference_type operator - (const indexed_const_iterator &it) const {
            return it_ - it.it_;
        }

        // Dereference
        NUMERICS_INLINE
//        const_reference operator * () const {
        value_type operator * () const {
            check (index () < (*this) ().size (), bad_index ());
            return (*this) () (it_);
        }
        NUMERICS_INLINE
        value_type operator [] (difference_type n) const {
            return *((*this) + n);
        }

        // Index
        NUMERICS_INLINE
        size_type index () const {
            return it_;
        }

        // Assignment 
        NUMERICS_INLINE
        indexed_const_iterator &operator = (const indexed_const_iterator &it) {
            assign (&it ());
            it_ = it.it_;
            return *this;
        }

        // Comparison
        NUMERICS_INLINE
        bool operator == (const indexed_const_iterator &it) const {
#ifndef NUMERICS_USE_CANONICAL_ITERATOR
            check (&(*this) () == &it (), external_logic ());
#endif
            return it_ == it.it_;
        }

    private:
        size_type it_;

        friend class indexed_iterator<container_type, iterator_category>;
    };

#ifdef BOOST_MSVC_STD_ITERATOR
    template<class C, class I>
    NUMERICS_INLINE
    indexed_const_iterator<C, I> operator ++ (const indexed_const_iterator<C, I> &it, int) {
        indexed_const_iterator<C, I> tmp (it);
        ++ tmp;
        return tmp;
    }
    template<class C, class I>
    NUMERICS_INLINE
    indexed_const_iterator<C, I> operator -- (const indexed_const_iterator<C, I> &it, int) {
        indexed_const_iterator<C, I> tmp (it);
        -- tmp;
        return tmp;
    }
    template<class C, class I>
    NUMERICS_INLINE
    indexed_const_iterator<C, I> operator + (const indexed_const_iterator<C, I> &it, std::ptrdiff_t n) {
        indexed_const_iterator<C, I> tmp (it);
        return tmp += n;
    }
    template<class C, class I>
    NUMERICS_INLINE
    indexed_const_iterator<C, I> operator - (const indexed_const_iterator<C, I> &it, std::ptrdiff_t n) {
        indexed_const_iterator<C, I> tmp (it);
        return tmp -= n;
    }
#endif

    template<class C, class IC>
    class indexed_iterator2;

    template<class C, class IC>
    class indexed_iterator1:
        public container_reference<C>, 
        public random_access_iterator_base<IC,
                                           indexed_iterator1<C, IC>, 
                                           typename C::value_type,
                                           typename C::reference> {
    public:
        typedef C container_type;
        typedef IC iterator_category;
        typedef typename container_type::size_type size_type;
        typedef typename container_type::difference_type difference_type;
        typedef typename container_type::value_type value_type;
        typedef typename container_type::reference reference;
        typedef typename container_type::pointer pointer;
        typedef indexed_iterator2<container_type, iterator_category> dual_iterator_type;
#ifdef BOOST_MSVC_STD_ITERATOR
        typedef reverse_iterator2<dual_iterator_type, value_type, reference> dual_reverse_iterator_type;
#else
        typedef reverse_iterator2<dual_iterator_type> dual_reverse_iterator_type;
#endif

        // Construction and destruction
        NUMERICS_INLINE
        indexed_iterator1 ():
            container_reference<container_type> (), it1_ (), it2_ () {}
        NUMERICS_INLINE 
        indexed_iterator1 (container_type &c, size_type it1, size_type it2):
            container_reference<container_type> (c), it1_ (it1), it2_ (it2) {}

        // Arithmetic
        NUMERICS_INLINE
        indexed_iterator1 &operator ++ () {
            ++ it1_;
            return *this;
        }
        NUMERICS_INLINE
        indexed_iterator1 &operator -- () {
            -- it1_;
            return *this;
        }
        NUMERICS_INLINE
        indexed_iterator1 &operator += (difference_type n) {
            it1_ += n;
            return *this;
        }
        NUMERICS_INLINE
        indexed_iterator1 &operator -= (difference_type n) {
            it1_ -= n;
            return *this;
        }
        NUMERICS_INLINE
        difference_type operator - (const indexed_iterator1 &it) const {
            return it1_ - it.it1_;
        }

        // Dereference
        NUMERICS_INLINE
        reference operator * () const {
            check (index1 () < (*this) ().size1 (), bad_index ());
            check (index2 () < (*this) ().size2 (), bad_index ());
            return (*this) () (it1_, it2_);
        }
        NUMERICS_INLINE
        reference operator [] (difference_type n) const {
            return *((*this) + n);
        }

        // Index
        NUMERICS_INLINE
        size_type index1 () const {
            return it1_;
        }
        NUMERICS_INLINE
        size_type index2 () const {
            return it2_;
        }

        NUMERICS_INLINE
        dual_iterator_type begin () const {
            return (*this) ().find_first2 (1, index1 (), 0); 
        }
        NUMERICS_INLINE
        dual_iterator_type end () const {
            return (*this) ().find_first2 (1, index1 (), (*this) ().size2 ()); 
        }
        NUMERICS_INLINE
        dual_reverse_iterator_type rbegin () const {
            return dual_reverse_iterator_type (end ()); 
        }
        NUMERICS_INLINE
        dual_reverse_iterator_type rend () const {
            return dual_reverse_iterator_type (begin ()); 
        }

        // Assignment 
        NUMERICS_INLINE
        indexed_iterator1 &operator = (const indexed_iterator1 &it) {
            assign (&it ());
            it1_ = it.it1_;
            it2_ = it.it2_;
            return *this;
        }

        // Comparison
        NUMERICS_INLINE
        bool operator == (const indexed_iterator1 &it) const {
            check (&(*this) () == &it (), external_logic ());
            return it1_ == it.it1_ && it2_ == it.it2_;
        }

    private:
        size_type it1_;
        size_type it2_;
    };

#ifdef BOOST_MSVC_STD_ITERATOR
    template<class C, class I>
    NUMERICS_INLINE
    indexed_iterator1<C, I> operator ++ (const indexed_iterator1<C, I> &it, int) {
        indexed_iterator1<C, I> tmp (it);
        ++ tmp;
        return tmp;
    }
    template<class C, class I>
    NUMERICS_INLINE
    indexed_iterator1<C, I> operator -- (const indexed_iterator1<C, I> &it, int) {
        indexed_iterator1<C, I> tmp (it);
        -- tmp;
        return tmp;
    }
    template<class C, class I>
    NUMERICS_INLINE
    indexed_iterator1<C, I> operator + (const indexed_iterator1<C, I> &it, std::ptrdiff_t n) {
        indexed_iterator1<C, I> tmp (it);
        return tmp += n;
    }
    template<class C, class I>
    NUMERICS_INLINE
    indexed_iterator1<C, I> operator - (const indexed_iterator1<C, I> &it, std::ptrdiff_t n) {
        indexed_iterator1<C, I> tmp (it);
        return tmp -= n;
    }
#endif

    template<class C, class IC>
    class indexed_const_iterator2;

    template<class C, class IC>
    class indexed_const_iterator1:
        public container_const_reference<C>, 
        public random_access_iterator_base<IC,
                                           indexed_const_iterator1<C, IC>, 
                                           typename C::value_type,
                                           typename C::const_reference> {
    public:
        typedef C container_type;
        typedef IC iterator_category;
        typedef typename container_type::size_type size_type;
        typedef typename container_type::difference_type difference_type;
        typedef typename container_type::value_type value_type;
        typedef typename container_type::value_type reference;
        typedef typename container_type::const_pointer pointer;
        typedef indexed_iterator1<container_type, iterator_category> iterator_type;
        typedef indexed_const_iterator2<container_type, iterator_category> dual_iterator_type;
#ifdef BOOST_MSVC_STD_ITERATOR
        typedef reverse_iterator2<dual_iterator_type, value_type, value_type> dual_reverse_iterator_type;
#else
        typedef reverse_iterator2<dual_iterator_type> dual_reverse_iterator_type;
#endif

        // Construction and destruction
        NUMERICS_INLINE
        indexed_const_iterator1 ():
            container_const_reference<container_type> (), it1_ (), it2_ () {}
        NUMERICS_INLINE
        indexed_const_iterator1 (const container_type &c, size_type it1, size_type it2):
            container_const_reference<container_type> (c), it1_ (it1), it2_ (it2) {}
        NUMERICS_INLINE 
        indexed_const_iterator1 (const iterator_type &it):
            container_const_reference<container_type> (it ()), it1_ (it.index1 ()), it2_ (it.index2 ()) {}

        // Arithmetic
        NUMERICS_INLINE
        indexed_const_iterator1 &operator ++ () {
            ++ it1_;
            return *this;
        }
        NUMERICS_INLINE
        indexed_const_iterator1 &operator -- () {
            -- it1_;
            return *this;
        }
        NUMERICS_INLINE
        indexed_const_iterator1 &operator += (difference_type n) {
            it1_ += n;
            return *this;
        }
        NUMERICS_INLINE
        indexed_const_iterator1 &operator -= (difference_type n) {
            it1_ -= n;
            return *this;
        }
        NUMERICS_INLINE
        difference_type operator - (const indexed_const_iterator1 &it) const {
            return it1_ - it.it1_;
        }

        // Dereference
        NUMERICS_INLINE
//        const_reference operator * () const {
        value_type operator * () const {
            check (index1 () < (*this) ().size1 (), bad_index ());
            check (index2 () < (*this) ().size2 (), bad_index ());
            return (*this) () (it1_, it2_);
        }
        NUMERICS_INLINE
        value_type operator [] (difference_type n) const {
            return *((*this) + n);
        }

        // Index
        NUMERICS_INLINE
        size_type index1 () const {
            return it1_;
        }
        NUMERICS_INLINE
        size_type index2 () const {
            return it2_;
        }

        NUMERICS_INLINE
        dual_iterator_type begin () const {
            return (*this) ().find_first2 (1, index1 (), 0); 
        }
        NUMERICS_INLINE
        dual_iterator_type end () const {
            return (*this) ().find_first2 (1, index1 (), (*this) ().size2 ()); 
        }
        NUMERICS_INLINE
        dual_reverse_iterator_type rbegin () const {
            return dual_reverse_iterator_type (end ()); 
        }
        NUMERICS_INLINE
        dual_reverse_iterator_type rend () const {
            return dual_reverse_iterator_type (begin ()); 
        }

        // Assignment 
        NUMERICS_INLINE
        indexed_const_iterator1 &operator = (const indexed_const_iterator1 &it) {
            assign (&it ());
            it1_ = it.it1_;
            it2_ = it.it2_;
            return *this;
        }

        // Comparison
        NUMERICS_INLINE
        bool operator == (const indexed_const_iterator1 &it) const {
            check (&(*this) () == &it (), external_logic ());
            return it1_ == it.it1_ && it2_ == it.it2_;
        }

    private:
        size_type it1_;
        size_type it2_;

        friend class indexed_iterator1<container_type, iterator_category>;
    };

#ifdef BOOST_MSVC_STD_ITERATOR
    template<class C, class I>
    NUMERICS_INLINE
    indexed_const_iterator1<C, I> operator ++ (const indexed_const_iterator1<C, I> &it, int) {
        indexed_const_iterator1<C, I> tmp (it);
        ++ tmp;
        return tmp;
    }
    template<class C, class I>
    NUMERICS_INLINE
    indexed_const_iterator1<C, I> operator -- (const indexed_const_iterator1<C, I> &it, int) {
        indexed_const_iterator1<C, I> tmp (it);
        -- tmp;
        return tmp;
    }
    template<class C, class I>
    NUMERICS_INLINE
    indexed_const_iterator1<C, I> operator + (const indexed_const_iterator1<C, I> &it, std::ptrdiff_t n) {
        indexed_const_iterator1<C, I> tmp (it);
        return tmp += n;
    }
    template<class C, class I>
    NUMERICS_INLINE
    indexed_const_iterator1<C, I> operator - (const indexed_const_iterator1<C, I> &it, std::ptrdiff_t n) {
        indexed_const_iterator1<C, I> tmp (it);
        return tmp -= n;
    }
#endif

    template<class C, class IC>
    class indexed_iterator2:
        public container_reference<C>, 
        public random_access_iterator_base<IC,
                                           indexed_iterator2<C, IC>, 
                                           typename C::value_type,
                                           typename C::reference> {
    public:
        typedef C container_type;
        typedef IC iterator_category;
        typedef typename container_type::size_type size_type;
        typedef typename container_type::difference_type difference_type;
        typedef typename container_type::value_type value_type;
        typedef typename container_type::reference reference;
        typedef typename container_type::pointer pointer;
        typedef indexed_iterator1<container_type, iterator_category> dual_iterator_type;
#ifdef BOOST_MSVC_STD_ITERATOR
        typedef reverse_iterator1<dual_iterator_type, value_type, reference> dual_reverse_iterator_type;
#else
        typedef reverse_iterator1<dual_iterator_type> dual_reverse_iterator_type;
#endif

        // Construction and destruction
        NUMERICS_INLINE
        indexed_iterator2 ():
            container_reference<container_type> (), it1_ (), it2_ () {}
        NUMERICS_INLINE
        indexed_iterator2 (container_type &c, size_type it1, size_type it2):
            container_reference<container_type> (c), it1_ (it1), it2_ (it2) {}

        // Arithmetic
        NUMERICS_INLINE
        indexed_iterator2 &operator ++ () {
            ++ it2_;
            return *this;
        }
        NUMERICS_INLINE
        indexed_iterator2 &operator -- () {
            -- it2_;
            return *this;
        }
        NUMERICS_INLINE
        indexed_iterator2 &operator += (difference_type n) {
            it2_ += n;
            return *this;
        }
        NUMERICS_INLINE
        indexed_iterator2 &operator -= (difference_type n) {
            it2_ -= n;
            return *this;
        }
        NUMERICS_INLINE
        difference_type operator - (const indexed_iterator2 &it) const {
            return it2_ - it.it2_;
        }

        // Dereference
        NUMERICS_INLINE
        reference operator * () const {
            check (index1 () < (*this) ().size1 (), bad_index ());
            check (index2 () < (*this) ().size2 (), bad_index ());
            return (*this) () (it1_, it2_);
        }
        NUMERICS_INLINE
        reference operator [] (difference_type n) const {
            return *((*this) + n);
        }

        // Index
        NUMERICS_INLINE
        size_type index1 () const {
            return it1_;
        }
        NUMERICS_INLINE
        size_type index2 () const {
            return it2_;
        }

        NUMERICS_INLINE
        dual_iterator_type begin () const {
            return (*this) ().find_first1 (1, 0, index2 ()); 
        }
        NUMERICS_INLINE
        dual_iterator_type end () const {
            return (*this) ().find_first1 (1, (*this) ().size1 (), index2 ()); 
        }
        NUMERICS_INLINE
        dual_reverse_iterator_type rbegin () const {
            return dual_reverse_iterator_type (end ()); 
        }
        NUMERICS_INLINE
        dual_reverse_iterator_type rend () const {
            return dual_reverse_iterator_type (begin ()); 
        }

        // Assignment 
        NUMERICS_INLINE
        indexed_iterator2 &operator = (const indexed_iterator2 &it) {
            assign (&it ());
            it1_ = it.it1_;
            it2_ = it.it2_;
            return *this;
        }

        // Comparison
        NUMERICS_INLINE
        bool operator == (const indexed_iterator2 &it) const {
            check (&(*this) () == &it (), external_logic ());
            return it1_ == it.it1_ && it2_ == it.it2_;
        }

    private:
        size_type it1_;
        size_type it2_;
    };

#ifdef BOOST_MSVC_STD_ITERATOR
    template<class C, class I>
    NUMERICS_INLINE
    indexed_iterator2<C, I> operator ++ (const indexed_iterator2<C, I> &it, int) {
        indexed_iterator2<C, I> tmp (it);
        ++ tmp;
        return tmp;
    }
    template<class C, class I>
    NUMERICS_INLINE
    indexed_iterator2<C, I> operator -- (const indexed_iterator2<C, I> &it, int) {
        indexed_iterator2<C, I> tmp (it);
        -- tmp;
        return tmp;
    }
    template<class C, class I>
    NUMERICS_INLINE
    indexed_iterator2<C, I> operator + (const indexed_iterator2<C, I> &it, std::ptrdiff_t n) {
        indexed_iterator2<C, I> tmp (it);
        return tmp += n;
    }
    template<class C, class I>
    NUMERICS_INLINE
    indexed_iterator2<C, I> operator - (const indexed_iterator2<C, I> &it, std::ptrdiff_t n) {
        indexed_iterator2<C, I> tmp (it);
        return tmp -= n;
    }
#endif

    template<class C, class IC>
    class indexed_const_iterator2:
        public container_const_reference<C>, 
        public random_access_iterator_base<IC,
                                           indexed_const_iterator2<C, IC>, 
                                           typename C::value_type,
                                           typename C::const_reference> {
    public:
        typedef C container_type;
        typedef IC iterator_category;
        typedef typename container_type::size_type size_type;
        typedef typename container_type::difference_type difference_type;
        typedef typename container_type::value_type value_type;
        typedef typename container_type::value_type reference;
        typedef typename container_type::const_pointer pointer;
        typedef indexed_iterator2<container_type, iterator_category> iterator_type;
        typedef indexed_const_iterator1<container_type, iterator_category> dual_iterator_type;
#ifdef BOOST_MSVC_STD_ITERATOR
        typedef reverse_iterator1<dual_iterator_type, value_type, value_type> dual_reverse_iterator_type;
#else
        typedef reverse_iterator1<dual_iterator_type> dual_reverse_iterator_type;
#endif

        // Construction and destruction
        NUMERICS_INLINE
        indexed_const_iterator2 ():
            container_const_reference<container_type> (), it1_ (), it2_ () {}
        NUMERICS_INLINE
        indexed_const_iterator2 (const container_type &c, size_type it1, size_type it2):
            container_const_reference<container_type> (c), it1_ (it1), it2_ (it2) {}
        NUMERICS_INLINE 
        indexed_const_iterator2 (const iterator_type &it):
            container_const_reference<container_type> (it ()), it1_ (it.index1 ()), it2_ (it.index2 ()) {}

        // Arithmetic
        NUMERICS_INLINE
        indexed_const_iterator2 &operator ++ () {
            ++ it2_;
            return *this;
        }
        NUMERICS_INLINE
        indexed_const_iterator2 &operator -- () {
            -- it2_;
            return *this;
        }
        NUMERICS_INLINE
        indexed_const_iterator2 &operator += (difference_type n) {
            it2_ += n;
            return *this;
        }
        NUMERICS_INLINE
        indexed_const_iterator2 &operator -= (difference_type n) {
            it2_ -= n;
            return *this;
        }
        NUMERICS_INLINE
        difference_type operator - (const indexed_const_iterator2 &it) const {
            return it2_ - it.it2_;
        }

        // Dereference
        NUMERICS_INLINE
//        const_reference operator * () const {
        value_type operator * () const {
            check (index1 () < (*this) ().size1 (), bad_index ());
            check (index2 () < (*this) ().size2 (), bad_index ());
            return (*this) () (it1_, it2_);
        }
        NUMERICS_INLINE
        value_type operator [] (difference_type n) const {
            return *((*this) + n);
        }

        // Index
        NUMERICS_INLINE
        size_type index1 () const {
            return it1_;
        }
        NUMERICS_INLINE
        size_type index2 () const {
            return it2_;
        }

        NUMERICS_INLINE
        dual_iterator_type begin () const {
            return (*this) ().find_first1 (1, 0, index2 ()); 
        }
        NUMERICS_INLINE
        dual_iterator_type end () const {
            return (*this) ().find_first1 (1, (*this) ().size1 (), index2 ()); 
        }
        NUMERICS_INLINE
        dual_reverse_iterator_type rbegin () const {
            return dual_reverse_iterator_type (end ()); 
        }
        NUMERICS_INLINE
        dual_reverse_iterator_type rend () const {
            return dual_reverse_iterator_type (begin ()); 
        }

        // Assignment 
        NUMERICS_INLINE
        indexed_const_iterator2 &operator = (const indexed_const_iterator2 &it) {
            assign (&it ());
            it1_ = it.it1_;
            it2_ = it.it2_;
            return *this;
        }

        // Comparison
        NUMERICS_INLINE
        bool operator == (const indexed_const_iterator2 &it) const {
            check (&(*this) () == &it (), external_logic ());
            return it1_ == it.it1_ && it2_ == it.it2_;
        }

    private:
        size_type it1_;
        size_type it2_;

        friend class indexed_iterator2<container_type, iterator_category>;
    };

#ifdef BOOST_MSVC_STD_ITERATOR
    template<class C, class I>
    NUMERICS_INLINE
    indexed_const_iterator2<C, I> operator ++ (const indexed_const_iterator2<C, I> &it, int) {
        indexed_const_iterator2<C, I> tmp (it);
        ++ tmp;
        return tmp;
    }
    template<class C, class I>
    NUMERICS_INLINE
    indexed_const_iterator2<C, I> operator -- (const indexed_const_iterator2<C, I> &it, int) {
        indexed_const_iterator2<C, I> tmp (it);
        -- tmp;
        return tmp;
    }
    template<class C, class I>
    NUMERICS_INLINE
    indexed_const_iterator2<C, I> operator + (const indexed_const_iterator2<C, I> &it, std::ptrdiff_t n) {
        indexed_const_iterator2<C, I> tmp (it);
        return tmp += n;
    }
    template<class C, class I>
    NUMERICS_INLINE
    indexed_const_iterator2<C, I> operator - (const indexed_const_iterator2<C, I> &it, std::ptrdiff_t n) {
        indexed_const_iterator2<C, I> tmp (it);
        return tmp -= n;
    }
#endif

}}

#endif






