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

#ifndef NUMERICS_ITERATOR_H
#define NUMERICS_ITERATOR_H

#include <iterator>

#include "config.h"

#ifdef USE_GCC
// FIXME: could we use BOOST_NO_STD_ITERATOR and <boost/iterator.h> here?
#if __GNUC__ <= 2

namespace std {

    template <class I, class T, class D = std::ptrdiff_t, class P = T *, class R = T &>
    struct iterator {
        typedef I iterator_category;
        typedef T value_type;
        typedef D difference_type;
        typedef P pointer;
        typedef R reference;
    };

}

#endif
#endif

namespace numerics {

    template<class C>
    class container_const_reference {
    public:
        typedef C container_type;

        NUMERICS_INLINE
        container_const_reference ():
            c_ (0) {}
        NUMERICS_INLINE
        container_const_reference (const container_type &c):
            c_ (&c) {}

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
        container_type &operator () () {
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

    template<class I, class T>
    struct forward_iterator_base: 
        public std::iterator<std::forward_iterator_tag, T> {
        typedef I derived_iterator_type;
        typedef T derived_value_type;
        typedef const T &derived_const_reference_type;
        typedef T &derived_reference_type;

        // Arithmetic
        NUMERICS_INLINE
        derived_iterator_type operator ++ (int) {
            derived_iterator_type &d (*static_cast<const derived_iterator_type *> (this));
            derived_iterator_type tmp (d);
            ++ d;
            return tmp;
        }
#ifndef USE_GCC
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

    template<class I, class T>
    struct bidirectional_iterator_base: 
        public std::iterator<std::bidirectional_iterator_tag, T> {
        typedef I derived_iterator_type;
        typedef T derived_value_type;
        typedef const T &derived_const_reference_type;
        typedef T &derived_reference_type;

        // Arithmetic
        NUMERICS_INLINE
        derived_iterator_type operator ++ (int) {
            derived_iterator_type &d (*static_cast<const derived_iterator_type *> (this));
            derived_iterator_type tmp (d);
            ++ d;
            return tmp;
        }
#ifndef USE_GCC
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
#ifndef USE_GCC
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

    template<class I, class T, class D = std::ptrdiff_t>
    struct random_access_iterator_base:
        public std::iterator<std::random_access_iterator_tag, T> {
        typedef I derived_iterator_type;
        typedef T derived_value_type;
        typedef const T &derived_const_reference_type;
        typedef T &derived_reference_type;
        typedef D derived_difference_type;
#ifdef USE_MSVC
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
#ifndef USE_GCC
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
#ifndef USE_GCC
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
#ifndef USE_GCC
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
#ifndef USE_GCC
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

#ifdef USE_MSVC
    template <class I, class T, class R>
    class reverse_iterator:
        public std::reverse_iterator<I, T, R> {
    public:
        typedef std::ptrdiff_t difference_type;
        typedef I iterator_type;
        typedef T value_type;
        typedef R reference_type;
        typedef typename I::container_type container_type;

        // Construction and destruction
        NUMERICS_INLINE
        reverse_iterator ():
            std::reverse_iterator<iterator_type, value_type, reference_type> () {}
        NUMERICS_INLINE NUMERICS_EXPLICIT
        reverse_iterator (const iterator_type &it):
            std::reverse_iterator<iterator_type, value_type, reference_type> (it) {}

        // Arithmetic
        NUMERICS_INLINE
        reverse_iterator &operator += (difference_type n) {
            base () -= n;
            return *this;
        }
        NUMERICS_INLINE
        reverse_iterator &operator -= (difference_type n) {
            base () += n;
            return *this;
        }

        NUMERICS_INLINE
        const container_type &operator () () const {
            return base () ();
        }

        NUMERICS_INLINE
        difference_type index () const {
            return base ().index ();
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

    template <class I, class T, class R>
    class reverse_iterator1:
        public std::reverse_iterator<I, T, R> {
    public:
        typedef std::ptrdiff_t difference_type;
        typedef I iterator_type;
        typedef T value_type;
        typedef R reference_type;
        typedef typename I::container_type container_type;
        typedef typename I::dual_iterator_type dual_iterator_type;
        typedef typename I::dual_reverse_iterator_type dual_reverse_iterator_type;

        // Construction and destruction
        NUMERICS_INLINE
        reverse_iterator1 ():
            std::reverse_iterator<iterator_type, value_type, reference_type> () {}
        NUMERICS_INLINE NUMERICS_EXPLICIT
        reverse_iterator1 (const iterator_type &it):
            std::reverse_iterator<iterator_type, value_type, reference_type> (it) {}

        // Arithmetic
        NUMERICS_INLINE
        reverse_iterator1 &operator += (difference_type n) {
            base () -= n;
            return *this;
        }
        NUMERICS_INLINE
        reverse_iterator1 &operator -= (difference_type n) {
            base () += n;
            return *this;
        }

        NUMERICS_INLINE
        const container_type &operator () () const {
            return base () ();
        }

        NUMERICS_INLINE
        difference_type index1 () const {
            return base ().index1 ();
        }
        NUMERICS_INLINE
        difference_type index2 () const {
            return base ().index2 ();
        }

        NUMERICS_INLINE
        dual_iterator_type begin () const {
            return base ().begin ();
        }
        NUMERICS_INLINE
        dual_iterator_type end () const {
            return base ().end ();
        }
        NUMERICS_INLINE
        dual_reverse_iterator_type rbegin () const {
            return dual_reverse_iterator_type (end ());
        }
        NUMERICS_INLINE
        dual_reverse_iterator_type rend () const {
            return dual_reverse_iterator_type (begin ());
        }
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

    template <class I, class T, class R>
    class reverse_iterator2:
        public std::reverse_iterator<I, T, R> {
    public:
        typedef std::ptrdiff_t difference_type;
        typedef I iterator_type;
        typedef T value_type;
        typedef R reference_type;
        typedef typename I::container_type container_type;
        typedef typename I::dual_iterator_type dual_iterator_type;
        typedef typename I::dual_reverse_iterator_type dual_reverse_iterator_type;

        // Construction and destruction
        NUMERICS_INLINE
        reverse_iterator2 ():
            std::reverse_iterator<iterator_type, value_type, reference_type> () {}
        NUMERICS_INLINE NUMERICS_EXPLICIT
        reverse_iterator2 (const iterator_type &it):
            std::reverse_iterator<iterator_type, value_type, reference_type> (it) {}

        // Arithmetic
        NUMERICS_INLINE
        reverse_iterator2 &operator += (difference_type n) {
            base () -= n;
            return *this;
        }
        NUMERICS_INLINE
        reverse_iterator2 &operator -= (difference_type n) {
            base () += n;
            return *this;
        }

        NUMERICS_INLINE
        const container_type &operator () () const {
            return base () ();
        }

        NUMERICS_INLINE
        difference_type index1 () const {
            return base ().index1 ();
        }
        NUMERICS_INLINE
        difference_type index2 () const {
            return base ().index2 ();
        }

        NUMERICS_INLINE
        dual_iterator_type begin () const {
            return base ().begin ();
        }
        NUMERICS_INLINE
        dual_iterator_type end () const {
            return base ().end ();
        }
        NUMERICS_INLINE
        dual_reverse_iterator_type rbegin () const {
            return dual_reverse_iterator_type (end ());
        }
        NUMERICS_INLINE
        dual_reverse_iterator_type rend () const {
            return dual_reverse_iterator_type (begin ());
        }
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
#else

    template <class I>
    class reverse_iterator:
        public std::reverse_iterator<I> {
    public:
        typedef std::ptrdiff_t difference_type;
        typedef I iterator_type;
        typedef typename I::container_type container_type;

        // Construction and destruction
        NUMERICS_INLINE
        reverse_iterator ():
            std::reverse_iterator<iterator_type> () {}
        NUMERICS_INLINE NUMERICS_EXPLICIT
        reverse_iterator (const iterator_type &it):
            std::reverse_iterator<iterator_type> (it) {}

        // Arithmetic
        NUMERICS_INLINE
        reverse_iterator &operator += (difference_type n) {
            base () -= n;
            return *this;
        }
        NUMERICS_INLINE
        reverse_iterator &operator -= (difference_type n) {
            base () += n;
            return *this;
        }

#ifndef USE_GCC
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
#endif

        NUMERICS_INLINE
        const container_type &operator () () const {
            return base () ();
        }

        NUMERICS_INLINE
        difference_type index () const {
            return base ().index ();
        }
    };

    template <class I>
    class reverse_iterator1:
        public std::reverse_iterator<I> {
    public:
        typedef std::ptrdiff_t difference_type;
        typedef I iterator_type;
        typedef typename I::container_type container_type;
        typedef typename I::dual_iterator_type dual_iterator_type;
        typedef typename I::dual_reverse_iterator_type dual_reverse_iterator_type;

        // Construction and destruction
        NUMERICS_INLINE
        reverse_iterator1 ():
            std::reverse_iterator<iterator_type> () {}
        NUMERICS_INLINE NUMERICS_EXPLICIT
        reverse_iterator1 (const iterator_type &it):
            std::reverse_iterator<iterator_type> (it) {}

        // Arithmetic
        NUMERICS_INLINE
        reverse_iterator1 &operator += (difference_type n) {
            base () -= n;
            return *this;
        }
        NUMERICS_INLINE
        reverse_iterator1 &operator -= (difference_type n) {
            base () += n;
            return *this;
        }

#ifndef USE_GCC
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
#endif

        NUMERICS_INLINE
        const container_type &operator () () const {
            return base () ();
        }

        NUMERICS_INLINE
        difference_type index1 () const {
            return base ().index1 ();
        }
        NUMERICS_INLINE
        difference_type index2 () const {
            return base ().index2 ();
        }

        NUMERICS_INLINE
        dual_iterator_type begin () const {
            return base ().begin ();
        }
        NUMERICS_INLINE
        dual_iterator_type end () const {
            return base ().end ();
        }
        NUMERICS_INLINE
        dual_reverse_iterator_type rbegin () const {
            return dual_reverse_iterator_type (end ());
        }
        NUMERICS_INLINE
        dual_reverse_iterator_type rend () const {
            return dual_reverse_iterator_type (begin ());
        }
    };

    template <class I>
    class reverse_iterator2:
        public std::reverse_iterator<I> {
    public:
        typedef std::ptrdiff_t difference_type;
        typedef I iterator_type;
        typedef typename I::container_type container_type;
        typedef typename I::dual_iterator_type dual_iterator_type;
        typedef typename I::dual_reverse_iterator_type dual_reverse_iterator_type;

        // Construction and destruction
        NUMERICS_INLINE
        reverse_iterator2 ():
            std::reverse_iterator<iterator_type> () {}
        NUMERICS_INLINE NUMERICS_EXPLICIT
        reverse_iterator2 (const iterator_type &it):
            std::reverse_iterator<iterator_type> (it) {}

        // Arithmetic
        NUMERICS_INLINE
        reverse_iterator2 &operator += (difference_type n) {
            base () -= n;
            return *this;
        }
        NUMERICS_INLINE
        reverse_iterator2 &operator -= (difference_type n) {
            base () += n;
            return *this;
        }

#ifndef USE_GCC
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
#endif

        NUMERICS_INLINE
        const container_type &operator () () const {
            return base () ();
        }

        NUMERICS_INLINE
        difference_type index1 () const {
            return base ().index1 ();
        }
        NUMERICS_INLINE
        difference_type index2 () const {
            return base ().index2 ();
        }

        NUMERICS_INLINE
        dual_iterator_type begin () const {
            return base ().begin ();
        }
        NUMERICS_INLINE
        dual_iterator_type end () const {
            return base ().end ();
        }
        NUMERICS_INLINE
        dual_reverse_iterator_type rbegin () const {
            return dual_reverse_iterator_type (end ());
        }
        NUMERICS_INLINE
        dual_reverse_iterator_type rend () const {
            return dual_reverse_iterator_type (begin ());
        }
    };
#endif

#ifdef USE_GCC
    template<class I>
    NUMERICS_INLINE
    I operator ++ (I &it, int) {
        I tmp (it);
        ++ it;
        return tmp;
    }
    template<class I>
    NUMERICS_INLINE
    I operator -- (I &it, int) {
        I tmp (it);
        -- it;
        return tmp;
    }
    template<class I>
    NUMERICS_INLINE
    I operator + (const I &it, std::ptrdiff_t n) {
        I tmp (it);
        return tmp += n;
    }
    template<class I>
    NUMERICS_INLINE
    I operator - (const I &it, std::ptrdiff_t n) {
        I tmp (it);
        return tmp -= n;
    }
#endif

    template<class I1, class I2>
    struct restrict_traits {
        typedef I1 iterator_category;
    };

    template<>
    struct restrict_traits<std::bidirectional_iterator_tag, std::random_access_iterator_tag> {
        typedef std::bidirectional_iterator_tag iterator_category;
    };
    template<>
    struct restrict_traits<std::random_access_iterator_tag, std::bidirectional_iterator_tag> {
        typedef std::bidirectional_iterator_tag iterator_category;
    };

    template<class C>
    class indexed_iterator:
        public container_reference<C>, 
        public random_access_iterator_base<indexed_iterator<C>, 
                                           typename C::value_type,
                                           typename C::difference_type> {
    public:
        typedef C container_type;
        typedef std::random_access_iterator_tag iterator_category;
        typedef typename container_type::size_type size_type;
        typedef typename container_type::difference_type difference_type;
        typedef typename container_type::value_type value_type;
        typedef typename container_type::reference_type reference;
        typedef typename container_type::pointer_type pointer;

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
            return it_ - (*this) ().begin ().it_;
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
            check (&(*this) () == &it (), external_logic ());
            return it_ == it.it_;
        }

    private:
        size_type it_;
    };

#ifdef USE_MSVC
    template<class C>
    NUMERICS_INLINE
    indexed_iterator<C> operator ++ (const indexed_iterator<C> &it, int) {
        indexed_iterator<C> tmp (it);
        ++ tmp;
        return tmp;
    }
    template<class C>
    NUMERICS_INLINE
    indexed_iterator<C> operator -- (const indexed_iterator<C> &it, int) {
        indexed_iterator<C> tmp (it);
        -- tmp;
        return tmp;
    }
    template<class C>
    NUMERICS_INLINE
    indexed_iterator<C> operator + (const indexed_iterator<C> &it, std::ptrdiff_t n) {
        indexed_iterator<C> tmp (it);
        return tmp += n;
    }
    template<class C>
    NUMERICS_INLINE
    indexed_iterator<C> operator - (const indexed_iterator<C> &it, std::ptrdiff_t n) {
        indexed_iterator<C> tmp (it);
        return tmp -= n;
    }
#endif

    template<class C>
    class indexed_const_iterator:
        public container_const_reference<C>, 
        public random_access_iterator_base<indexed_const_iterator<C>, 
                                           typename C::value_type,
                                           typename C::difference_type> {
    public:
        typedef C container_type;
        typedef std::random_access_iterator_tag iterator_category;
        typedef typename container_type::size_type size_type;
        typedef typename container_type::difference_type difference_type;
        typedef typename container_type::value_type value_type;
        typedef typename container_type::value_type reference;
        typedef typename container_type::const_pointer_type pointer;
        typedef indexed_iterator<container_type> iterator_type;

        // Construction and destruction
        NUMERICS_INLINE
        indexed_const_iterator ():
            container_const_reference<container_type> (), it_ () {}
        NUMERICS_INLINE
        indexed_const_iterator (const container_type &c, size_type it):
            container_const_reference<container_type> (c), it_ (it) {}
        NUMERICS_INLINE NUMERICS_EXPLICIT
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
            return it_ - (*this) ().begin ().it_;
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
            check (&(*this) () == &it (), external_logic ());
            return it_ == it.it_;
        }

    private:
        size_type it_;

        friend class indexed_iterator<container_type>;
    };

#ifdef USE_MSVC
    template<class C>
    NUMERICS_INLINE
    indexed_const_iterator<C> operator ++ (const indexed_const_iterator<C> &it, int) {
        indexed_const_iterator<C> tmp (it);
        ++ tmp;
        return tmp;
    }
    template<class C>
    NUMERICS_INLINE
    indexed_const_iterator<C> operator -- (const indexed_const_iterator<C> &it, int) {
        indexed_const_iterator<C> tmp (it);
        -- tmp;
        return tmp;
    }
    template<class C>
    NUMERICS_INLINE
    indexed_const_iterator<C> operator + (const indexed_const_iterator<C> &it, std::ptrdiff_t n) {
        indexed_const_iterator<C> tmp (it);
        return tmp += n;
    }
    template<class C>
    NUMERICS_INLINE
    indexed_const_iterator<C> operator - (const indexed_const_iterator<C> &it, std::ptrdiff_t n) {
        indexed_const_iterator<C> tmp (it);
        return tmp -= n;
    }
#endif

    template<class C>
    class indexed_iterator2;

    template<class C>
    class indexed_iterator1:
        public container_reference<C>, 
        public random_access_iterator_base<indexed_iterator1<C>, 
                                           typename C::value_type,
                                           typename C::reference_type> {
    public:
        typedef C container_type;
        typedef std::random_access_iterator_tag iterator_category;
        typedef typename container_type::size_type size_type;
        typedef typename container_type::difference_type difference_type;
        typedef typename container_type::value_type value_type;
        typedef typename container_type::reference_type reference;
        typedef typename container_type::pointer_type pointer;
        typedef indexed_iterator2<container_type> dual_iterator_type;
#ifdef USE_MSVC
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
            return it1_ - (*this) ().begin1 ().it1_;
        }
        NUMERICS_INLINE
        size_type index2 () const {
            return it2_ - (*this) ().begin1 ().it2_;
        }

        NUMERICS_INLINE
        dual_iterator_type begin () const {
            return (*this) ().find2 (1, index1 (), 0); 
        }
        NUMERICS_INLINE
        dual_iterator_type end () const {
            return (*this) ().find2 (1, index1 (), (*this) ().size2 ()); 
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

#ifdef USE_MSVC
    template<class C>
    NUMERICS_INLINE
    indexed_iterator1<C> operator ++ (const indexed_iterator1<C> &it, int) {
        indexed_iterator1<C> tmp (it);
        ++ tmp;
        return tmp;
    }
    template<class C>
    NUMERICS_INLINE
    indexed_iterator1<C> operator -- (const indexed_iterator1<C> &it, int) {
        indexed_iterator1<C> tmp (it);
        -- tmp;
        return tmp;
    }
    template<class C>
    NUMERICS_INLINE
    indexed_iterator1<C> operator + (const indexed_iterator1<C> &it, std::ptrdiff_t n) {
        indexed_iterator1<C> tmp (it);
        return tmp += n;
    }
    template<class C>
    NUMERICS_INLINE
    indexed_iterator1<C> operator - (const indexed_iterator1<C> &it, std::ptrdiff_t n) {
        indexed_iterator1<C> tmp (it);
        return tmp -= n;
    }
#endif

    template<class C>
    class indexed_const_iterator2;

    template<class C>
    class indexed_const_iterator1:
        public container_const_reference<C>, 
        public random_access_iterator_base<indexed_const_iterator1<C>, 
                                           typename C::value_type,
                                           typename C::const_reference_type> {
    public:
        typedef C container_type;
        typedef std::random_access_iterator_tag iterator_category;
        typedef typename container_type::size_type size_type;
        typedef typename container_type::difference_type difference_type;
        typedef typename container_type::value_type value_type;
        typedef typename container_type::value_type reference;
        typedef typename container_type::const_pointer_type pointer;
        typedef indexed_iterator1<container_type> iterator_type;
        typedef indexed_const_iterator2<container_type> dual_iterator_type;
#ifdef USE_MSVC
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
        NUMERICS_INLINE NUMERICS_EXPLICIT
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
            return it1_ - (*this) ().begin1 ().it1_;
        }
        NUMERICS_INLINE
        size_type index2 () const {
            return it2_ - (*this) ().begin1 ().it2_;
        }

        NUMERICS_INLINE
        dual_iterator_type begin () const {
            return (*this) ().find2 (1, index1 (), 0); 
        }
        NUMERICS_INLINE
        dual_iterator_type end () const {
            return (*this) ().find2 (1, index1 (), (*this) ().size2 ()); 
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

        friend class indexed_iterator1<container_type>;
    };

#ifdef USE_MSVC
    template<class C>
    NUMERICS_INLINE
    indexed_const_iterator1<C> operator ++ (const indexed_const_iterator1<C> &it, int) {
        indexed_const_iterator1<C> tmp (it);
        ++ tmp;
        return tmp;
    }
    template<class C>
    NUMERICS_INLINE
    indexed_const_iterator1<C> operator -- (const indexed_const_iterator1<C> &it, int) {
        indexed_const_iterator1<C> tmp (it);
        -- tmp;
        return tmp;
    }
    template<class C>
    NUMERICS_INLINE
    indexed_const_iterator1<C> operator + (const indexed_const_iterator1<C> &it, std::ptrdiff_t n) {
        indexed_const_iterator1<C> tmp (it);
        return tmp += n;
    }
    template<class C>
    NUMERICS_INLINE
    indexed_const_iterator1<C> operator - (const indexed_const_iterator1<C> &it, std::ptrdiff_t n) {
        indexed_const_iterator1<C> tmp (it);
        return tmp -= n;
    }
#endif

    template<class C>
    class indexed_iterator2:
        public container_reference<C>, 
        public random_access_iterator_base<indexed_iterator2<C>, 
                                           typename C::value_type,
                                           typename C::reference_type> {
    public:
        typedef C container_type;
        typedef std::random_access_iterator_tag iterator_category;
        typedef typename container_type::size_type size_type;
        typedef typename container_type::difference_type difference_type;
        typedef typename container_type::value_type value_type;
        typedef typename container_type::reference_type reference;
        typedef typename container_type::pointer_type pointer;
        typedef indexed_iterator1<container_type> dual_iterator_type;
#ifdef USE_MSVC
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
            return it1_ - (*this) ().begin2 ().it1_;
        }
        NUMERICS_INLINE
        size_type index2 () const {
            return it2_ - (*this) ().begin2 ().it2_;
        }

        NUMERICS_INLINE
        dual_iterator_type begin () const {
            return (*this) ().find1 (1, 0, index2 ()); 
        }
        NUMERICS_INLINE
        dual_iterator_type end () const {
            return (*this) ().find1 (1, (*this) ().size1 (), index2 ()); 
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

#ifdef USE_MSVC
    template<class C>
    NUMERICS_INLINE
    indexed_iterator2<C> operator ++ (const indexed_iterator2<C> &it, int) {
        indexed_iterator2<C> tmp (it);
        ++ tmp;
        return tmp;
    }
    template<class C>
    NUMERICS_INLINE
    indexed_iterator2<C> operator -- (const indexed_iterator2<C> &it, int) {
        indexed_iterator2<C> tmp (it);
        -- tmp;
        return tmp;
    }
    template<class C>
    NUMERICS_INLINE
    indexed_iterator2<C> operator + (const indexed_iterator2<C> &it, std::ptrdiff_t n) {
        indexed_iterator2<C> tmp (it);
        return tmp += n;
    }
    template<class C>
    NUMERICS_INLINE
    indexed_iterator2<C> operator - (const indexed_iterator2<C> &it, std::ptrdiff_t n) {
        indexed_iterator2<C> tmp (it);
        return tmp -= n;
    }
#endif

    template<class C>
    class indexed_const_iterator2:
        public container_const_reference<C>, 
        public random_access_iterator_base<indexed_const_iterator2<C>, 
                                           typename C::value_type,
                                           typename C::const_reference_type> {
    public:
        typedef C container_type;
        typedef std::random_access_iterator_tag iterator_category;
        typedef typename container_type::size_type size_type;
        typedef typename container_type::difference_type difference_type;
        typedef typename container_type::value_type value_type;
        typedef typename container_type::value_type reference;
        typedef typename container_type::const_pointer_type pointer;
        typedef indexed_iterator2<container_type> iterator_type;
        typedef indexed_const_iterator1<container_type> dual_iterator_type;
#ifdef USE_MSVC
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
        NUMERICS_INLINE NUMERICS_EXPLICIT
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
            return it1_ - (*this) ().begin2 ().it1_;
        }
        NUMERICS_INLINE
        size_type index2 () const {
            return it2_ - (*this) ().begin2 ().it2_;
        }

        NUMERICS_INLINE
        dual_iterator_type begin () const {
            return (*this) ().find1 (1, 0, index2 ()); 
        }
        NUMERICS_INLINE
        dual_iterator_type end () const {
            return (*this) ().find1 (1, (*this) ().size1 (), index2 ()); 
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

        friend class indexed_iterator2<container_type>;
    };

#ifdef USE_MSVC
    template<class C>
    NUMERICS_INLINE
    indexed_const_iterator2<C> operator ++ (const indexed_const_iterator2<C> &it, int) {
        indexed_const_iterator2<C> tmp (it);
        ++ tmp;
        return tmp;
    }
    template<class C>
    NUMERICS_INLINE
    indexed_const_iterator2<C> operator -- (const indexed_const_iterator2<C> &it, int) {
        indexed_const_iterator2<C> tmp (it);
        -- tmp;
        return tmp;
    }
    template<class C>
    NUMERICS_INLINE
    indexed_const_iterator2<C> operator + (const indexed_const_iterator2<C> &it, std::ptrdiff_t n) {
        indexed_const_iterator2<C> tmp (it);
        return tmp += n;
    }
    template<class C>
    NUMERICS_INLINE
    indexed_const_iterator2<C> operator - (const indexed_const_iterator2<C> &it, std::ptrdiff_t n) {
        indexed_const_iterator2<C> tmp (it);
        return tmp -= n;
    }
#endif

}

#endif


