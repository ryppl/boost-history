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

namespace std {

    template<class I, class T> 
    struct iterator {};

}

#endif

namespace numerics {

    template<class C>
    class container_const_reference {
    public:
        typedef C container_type;

        NUMERICS_INLINE
        container_const_reference (const container_type &c):
            c_ (c) {}

        NUMERICS_INLINE
        const container_type &operator () () const {
            return c_; 
        }

    private:
        const container_type &c_;
    };

    template<class C>
    class container_reference {
    public:
        typedef C container_type;

        NUMERICS_INLINE
        container_reference (container_type &c):
            c_ (c) {}

        NUMERICS_INLINE
        const container_type &operator () () const {
            return c_; 
        }
        NUMERICS_INLINE
        container_type &operator () () {
            return c_; 
        }

    private:
        container_type &c_;
    };

    template<class D, class T>
    struct forward_iterator_base: 
        public std::iterator<std::forward_iterator_tag, T> {
        typedef D derived_iterator_type;
        typedef T value_type;
        typedef const T &const_reference_type;
        typedef T &reference_type;

        // Arithmetic
        NUMERICS_INLINE
        derived_iterator_type operator ++ (int) {
            derived_iterator_type &d (*static_cast<const derived_iterator_type *> (this));
            derived_iterator_type tmp (d);
            ++ d;
            return tmp;
        }
        NUMERICS_INLINE
        friend derived_iterator_type operator ++ (derived_iterator_type &d, int) {
            derived_iterator_type tmp (d);
            ++ d;
            return tmp;
        }

        // Comparison
        NUMERICS_INLINE
        bool operator != (const derived_iterator_type &it) const {
            const derived_iterator_type *d = static_cast<const derived_iterator_type *> (this);
            return ! (*d == it);
        }
    };

    template<class D, class T>
    struct bidirectional_iterator_base: 
        public std::iterator<std::bidirectional_iterator_tag, T> {
        typedef D derived_iterator_type;
        typedef T value_type;
        typedef const T &const_reference_type;
        typedef T &reference_type;

        // Arithmetic
        NUMERICS_INLINE
        derived_iterator_type operator ++ (int) {
            derived_iterator_type &d (*static_cast<const derived_iterator_type *> (this));
            derived_iterator_type tmp (d);
            ++ d;
            return tmp;
        }
        NUMERICS_INLINE
        friend derived_iterator_type operator ++ (derived_iterator_type &d, int) {
            derived_iterator_type tmp (d);
            ++ d;
            return tmp;
        }
        NUMERICS_INLINE
        derived_iterator_type operator -- (int) {
            derived_iterator_type &d (*static_cast<const derived_iterator_type *> (this));
            derived_iterator_type tmp (d);
            -- d;
            return tmp;
        }
        NUMERICS_INLINE
        friend derived_iterator_type operator -- (derived_iterator_type &d, int) {
            derived_iterator_type tmp (d);
            -- d;
            return tmp;
        }

        // Comparison
        NUMERICS_INLINE
        bool operator != (const derived_iterator_type &it) const {
            const derived_iterator_type *d = static_cast<const derived_iterator_type *> (this);
            return ! (*d == it);
        }
    };

    template<class D, class T>
    struct random_access_iterator_base:
        public std::iterator<std::random_access_iterator_tag, T> {
        typedef D derived_iterator_type;
        typedef T value_type;
        typedef const T &const_reference_type;
        typedef T &reference_type;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;

        // Arithmetic
        NUMERICS_INLINE
        derived_iterator_type operator ++ (int) {
            derived_iterator_type &d (*static_cast<const derived_iterator_type *> (this));
            derived_iterator_type tmp (d);
            ++ d;
            return tmp;
        }
        NUMERICS_INLINE
        friend derived_iterator_type operator ++ (derived_iterator_type &d, int) {
            derived_iterator_type tmp (d);
            ++ d;
            return tmp;
        }
        NUMERICS_INLINE
        derived_iterator_type operator -- (int) {
            derived_iterator_type &d (*static_cast<const derived_iterator_type *> (this));
            derived_iterator_type tmp (d);
            -- d;
            return tmp;
        }
        NUMERICS_INLINE
        friend derived_iterator_type operator -- (derived_iterator_type &d, int) {
            derived_iterator_type tmp (d);
            -- d;
            return tmp;
        }
        NUMERICS_INLINE
        derived_iterator_type operator + (difference_type n) const {
            derived_iterator_type d (*static_cast<const derived_iterator_type *> (this));
            return d += n;
        }
        NUMERICS_INLINE
        friend derived_iterator_type operator + (const derived_iterator_type &d, difference_type n) {
            derived_iterator_type tmp (d);
            return tmp += n;
        }
        NUMERICS_INLINE
        derived_iterator_type operator - (difference_type n) const {
            derived_iterator_type d (*static_cast<const derived_iterator_type *> (this));
            return d -= n;
        }
        NUMERICS_INLINE
        friend derived_iterator_type operator - (const derived_iterator_type &d, difference_type n) {
            derived_iterator_type tmp (d);
            return tmp -= n;
        }

        NUMERICS_INLINE
        value_type operator [] (difference_type n) const {
            const derived_iterator_type *d = static_cast<const derived_iterator_type *> (this);
            return *((*d) + n);
        }
        NUMERICS_INLINE
        reference_type operator [] (difference_type n) {
            const derived_iterator_type *d = static_cast<const derived_iterator_type *> (this);
            return *((*d) + n);
        }

        // Comparison
        NUMERICS_INLINE
        bool operator != (const derived_iterator_type &it) const {
            const derived_iterator_type *d = static_cast<const derived_iterator_type *> (this);
            return ! (*d == it);
        }
    };

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

}

#endif


