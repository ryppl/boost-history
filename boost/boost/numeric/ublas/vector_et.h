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

#ifndef VECTOR_ET_H
#define VECTOR_ET_H

#include "config.h"
#include "exception.h"
#include "functional.h"

// Expression templates based on ideas of Todd Veldhuizen and Geoffrey Furnish 
// Iterators based on ideas of Jeremy Siek

namespace numerics {

    template<class T>
    struct scalar_expression {
        typedef T value_type;
    };

    template<class T>
    class scalar_value:
        public scalar_expression<T> {
    public:
        typedef T value_type;
        
        // Construction and destruction
        NUMERICS_INLINE
        scalar_value ():
            t_ () {}
        NUMERICS_INLINE
        scalar_value (const value_type &t):
            t_ (t) {}

        NUMERICS_INLINE
        operator value_type () const {
            return t_;
        }

    private:
        value_type t_;
    };

    template<class T>
    class scalar_const_reference:
        public scalar_expression<T> {
    public:
        typedef T value_type;
        
        // Construction and destruction
        NUMERICS_INLINE
        scalar_const_reference ():
            t_ (nil_) {}
        NUMERICS_INLINE
        scalar_const_reference (const value_type &t):
            t_ (t) {}

        NUMERICS_INLINE
        operator value_type () const {
            return t_;
        }

    private:
        const value_type &t_;
        static value_type nil_;
    };

    template<class T>
    scalar_const_reference<T>::value_type scalar_const_reference<T>::nil_;

    // Base class for the Barton Nackman trick
    template<class E>
    struct vector_expression {
        typedef E expression_type;
        typedef vector_tag type_category;

        // This class could define an common interface for all 
        // statically derived expression type classes.
        // Due to a compiler deficiency - one can not reference class typedefs of E 
        // on MSVC 6.0 (error C2027) - we only implement the casts.

        NUMERICS_INLINE
        const expression_type &operator () () const {
            return *static_cast<const expression_type *> (this);
        }
        NUMERICS_INLINE
        expression_type &operator () () {
            return *static_cast<expression_type *> (this);
        }
    };

    template<class E>
    class vector_const_reference:
        public vector_expression<vector_const_reference<E> > {
    public:
        typedef E expression_type;
        typedef typename E::size_type size_type;
        typedef typename E::difference_type difference_type;
        typedef typename E::value_type value_type;
        typedef typename E::const_reference const_reference;
        typedef const_reference reference;
        typedef typename E::const_pointer const_pointer;
        typedef const_pointer pointer;
        typedef typename E::const_iterator const_iterator_type;
#ifdef NUMERICS_DEPRECATED
#ifdef NUMERICS_DEPRECATED
        typedef const vector_expression_range<const E> const_vector_range_type;
#else
        typedef const vector_range<const E> const_vector_range_type;
#endif
#endif
        typedef unknown_storage_tag storage_category;

        // Construction and destruction
        NUMERICS_INLINE
        vector_const_reference ():
            e_ (nil_) {}
        NUMERICS_INLINE
        vector_const_reference (const expression_type &e):
            e_ (e) {}

        NUMERICS_INLINE
        size_type size () const { 
            return e_.size (); 
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i) const { 
            return e_ (i); 
        }

        NUMERICS_INLINE
        value_type operator [] (size_type i) const { 
            return e_ [i]; 
        }

#ifdef NUMERICS_DEPRECATED
        NUMERICS_INLINE
        const_vector_range_type project (size_type start, size_type stop) const {
            return const_vector_range_type (e_, start, stop);
        }
        NUMERICS_INLINE
        const_vector_range_type project (const range &r) const {
            return const_vector_range_type (e_, r);
        }
#endif

        typedef const_iterator_type const_iterator;
        typedef const_iterator iterator;

        // Element lookup
        NUMERICS_INLINE
        const_iterator find (size_type i) const {
            return const_iterator (e_.find (i));
        }

        // Iterator is the iterator of the referenced expression.

        NUMERICS_INLINE
        const_iterator begin () const {
            return e_.begin ();
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return e_.end ();
        }

        // Reverse iterator

#ifdef USE_MSVC
        typedef reverse_iterator<const_iterator, value_type, value_type> const_reverse_iterator;
#else
        typedef reverse_iterator<const_iterator> const_reverse_iterator;
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
        const expression_type &e_;
        static expression_type nil_;
    };

    template<class E>
    vector_const_reference<E>::expression_type vector_const_reference<E>::nil_;

    template<class E>
    class vector_reference: 
        public vector_expression<vector_reference<E> > {
    public:
        typedef E expression_type;
        typedef typename E::size_type size_type;
        typedef typename E::difference_type difference_type;
        typedef typename E::value_type value_type;
        typedef typename E::const_reference const_reference;
        typedef typename E::reference reference;
        typedef typename E::const_pointer const_pointer;
        typedef typename E::pointer pointer;
        typedef typename E::const_iterator const_iterator_type;
        typedef typename E::iterator iterator_type;
#ifdef NUMERICS_DEPRECATED
#ifdef NUMERICS_DEPRECATED
        typedef const vector_expression_range<const E> const_vector_range_type;
        typedef vector_expression_range<E> vector_range_type;
#else
        typedef const vector_range<const E> const_vector_range_type;
        typedef vector_range<E> vector_range_type;
#endif
#endif
        typedef unknown_storage_tag storage_category;

        // Construction and destruction
        NUMERICS_INLINE
        vector_reference ():
            e_ (nil_) {}
        NUMERICS_INLINE
        vector_reference (expression_type &e):
            e_ (e) {}

        // Resizing
        NUMERICS_INLINE
        void resize (size_type size) {
            e_.resize (size);
        }

        NUMERICS_INLINE
        size_type size () const { 
            return e_.size (); 
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i) const { 
            return e_ (i); 
        }
        NUMERICS_INLINE
        reference operator () (size_type i) { 
            return e_ (i); 
        }

        NUMERICS_INLINE
        value_type operator [] (size_type i) const { 
            return e_ [i]; 
        }
        NUMERICS_INLINE
        reference operator [] (size_type i) { 
            return e_ [i]; 
        }

#ifdef NUMERICS_DEPRECATED
        NUMERICS_INLINE
        const_vector_range_type project (size_type start, size_type stop) const {
            return const_vector_range_type (e_, start, stop);
        }
        NUMERICS_INLINE
        const_vector_range_type project (const range &r) const {
            return const_vector_range_type (e_, r);
        }
        NUMERICS_INLINE
        vector_range_type project (size_type start, size_type stop) {
            return vector_range_type (e_, start, stop);
        }
        NUMERICS_INLINE
        vector_range_type project (const range &r) {
            return vector_range_type (e_, r);
        }
#endif

        typedef const_iterator_type const_iterator;
        typedef iterator_type iterator;

        // Element lookup
        NUMERICS_INLINE
        const_iterator find (size_type i) const {
            return const_iterator (e_.find (i));
        }
        NUMERICS_INLINE
        iterator find (size_type i) {
            return iterator (e_.find (i));
        }

        // Iterator is the iterator of the referenced expression.

        NUMERICS_INLINE
        const_iterator begin () const {
            return e_.begin ();
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return e_.end (); 
        }

        NUMERICS_INLINE
        iterator begin () {
            return e_.begin ();
        }
        NUMERICS_INLINE
        iterator end () {
            return e_.end (); 
        }

        // Reverse iterator

#ifdef USE_MSVC
        typedef reverse_iterator<const_iterator, value_type, value_type> const_reverse_iterator;
#else
        typedef reverse_iterator<const_iterator> const_reverse_iterator;
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
        typedef reverse_iterator<iterator, value_type, reference> reverse_iterator;
#else
        typedef reverse_iterator<iterator> reverse_iterator;
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
        expression_type &e_;
        static expression_type nil_;
    };

    template<class E>
    vector_reference<E>::expression_type vector_reference<E>::nil_;

    template<class E, class F>
    class vector_unary: 
        public vector_expression<vector_unary<E, F> > {
    public:
        typedef E expression_type;
        typedef F functor_type;
        typedef typename E::size_type size_type;
        typedef typename E::difference_type difference_type;
        typedef typename F::result_type value_type;
        typedef const value_type &const_reference;
        typedef const_reference reference;
        typedef const value_type *const_pointer;
        typedef const_pointer pointer;
        typedef const vector_unary<E, F> const_closure_type;
        typedef typename E::const_iterator const_iterator_type;
#ifdef NUMERICS_DEPRECATED
#ifdef NUMERICS_DEPRECATED
        typedef const vector_expression_range<const_closure_type> const_vector_range_type;
#else
        typedef const vector_range<const_closure_type> const_vector_range_type;
#endif
#endif
        typedef unknown_storage_tag storage_category;

        // Construction and destruction
        NUMERICS_INLINE
        vector_unary (): 
            e_ () {}
        NUMERICS_INLINE
        vector_unary (const expression_type &e): 
            e_ (e) {}

        NUMERICS_INLINE
        size_type size () const { 
            return e_.size (); 
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i) const { 
            return functor_type () (e_ (i)); 
        }

        NUMERICS_INLINE
        value_type operator [] (size_type i) const { 
            return functor_type () (e_ [i]); 
        }

#ifdef NUMERICS_DEPRECATED
        NUMERICS_INLINE
        const_vector_range_type project (size_type start, size_type stop) const {
            return const_vector_range_type (*this, start, stop);
        }
        NUMERICS_INLINE
        const_vector_range_type project (const range &r) const {
            return const_vector_range_type (*this, r);
        }
#endif

        class const_iterator;
        typedef const_iterator iterator;

        // Element lookup
        NUMERICS_INLINE
        const_iterator find (size_type i) const {
            return const_iterator (*this, e_.find (i));
        }

        // Iterator enhances the iterator of the referenced expression 
        // with the unary functor.

        class const_iterator:
            public container_const_reference<vector_unary>, 
            public random_access_iterator_base<const_iterator, value_type> {
        public:
            typedef typename E::const_iterator::iterator_category iterator_category;
#ifndef USE_MSVC
            typedef typename E::const_iterator::difference_type difference_type;
            typedef typename E::const_iterator::value_type value_type;
            typedef typename E::const_iterator::value_type reference;
            typedef typename E::const_iterator::pointer pointer;
#endif

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator ():
                container_const_reference<vector_unary> (), it_ () {}
            NUMERICS_INLINE
            const_iterator (const vector_unary &vu, const const_iterator_type &it):
                container_const_reference<vector_unary> (vu), it_ (it) {}

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
                return functor_type () (*it_);
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                return it_.index ();
            }

            // Assignment 
            NUMERICS_INLINE
            const_iterator &operator = (const const_iterator &it) {
                container_const_reference<vector_unary>::assign (&it ());
                it_ = it.it_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it_ == it.it_;
            }

        private:
            const_iterator_type it_;
        };

        NUMERICS_INLINE
        const_iterator begin () const {
            return const_iterator (*this, e_.begin ()); 
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return const_iterator (*this, e_.end ()); 
        }

        // Reverse iterator

#ifdef USE_MSVC
        typedef reverse_iterator<const_iterator, value_type, value_type> const_reverse_iterator;
#else
        typedef reverse_iterator<const_iterator> const_reverse_iterator;
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
#ifdef NUMERICS_ET_VALUE
        expression_type e_; 
#endif
#ifdef NUMERICS_ET_REFERENCE
        const expression_type &e_; 
#endif
    };

    template<class E, class F>
    struct vector_unary_traits {
        typedef vector_unary<typename E::const_closure_type, F> expression_type;
#ifdef NUMERICS_USE_ET
        typedef expression_type result_type; 
#else
        typedef vector<typename E::value_type> result_type;
#endif
    };

    // (- v) [i] = - v [i]
    template<class E> 
    NUMERICS_INLINE
    typename vector_unary_traits<E, scalar_negate<typename E::value_type> >::result_type
    operator - (const vector_expression<E> &e) {
        typedef NUMERICS_TYPENAME vector_unary_traits<E, scalar_negate<NUMERICS_TYPENAME E::value_type> >::expression_type expression_type;
        return expression_type (e ());
    }

    // (conj v) [i] = conj (v [i])
    template<class E> 
    NUMERICS_INLINE
    typename vector_unary_traits<E, scalar_conj<typename E::value_type> >::result_type
    conj (const vector_expression<E> &e) {
        typedef NUMERICS_TYPENAME vector_unary_traits<E, scalar_conj<NUMERICS_TYPENAME E::value_type> >::expression_type expression_type;
        return expression_type (e ());
    }

    // (trans v) [i] = v [i]
    template<class E> 
    NUMERICS_INLINE
    typename vector_unary_traits<E, scalar_identity<typename E::value_type> >::result_type
    trans (const vector_expression<E> &e) {
        typedef NUMERICS_TYPENAME vector_unary_traits<E, scalar_identity<NUMERICS_TYPENAME E::value_type> >::expression_type expression_type;
        return expression_type (e ());
    }

    // (herm v) [i] = conj (v [i])
    template<class E> 
    NUMERICS_INLINE
    typename vector_unary_traits<E, scalar_conj<typename E::value_type> >::result_type
    herm (const vector_expression<E> &e) {
        typedef NUMERICS_TYPENAME vector_unary_traits<E, scalar_conj<NUMERICS_TYPENAME E::value_type> >::expression_type expression_type;
        return expression_type (e ());
    }

    template<class E1, class E2, class F>
    class vector_binary:
        public vector_expression<vector_binary<E1, E2, F> > {
    public:
        typedef E1 expression1_type;
        typedef E2 expression2_type;
        typedef F functor_type;
        typedef typename promote_traits<typename E1::size_type, typename E2::size_type>::promote_type size_type;
        typedef typename promote_traits<typename E1::difference_type, typename E2::difference_type>::promote_type difference_type;
        typedef typename F::result_type value_type;
        typedef const value_type &const_reference;
        typedef const_reference reference;
        typedef const value_type *const_pointer;
        typedef const_pointer pointer;
        typedef const vector_binary<E1, E2, F> const_closure_type;
        typedef typename E1::const_iterator const_iterator1_type;
        typedef typename E2::const_iterator const_iterator2_type;
#ifdef NUMERICS_DEPRECATED
#ifdef NUMERICS_DEPRECATED
        typedef const vector_expression_range<const_closure_type> const_vector_range_type;
#else
        typedef const vector_range<const_closure_type> const_vector_range_type;
#endif
#endif
        typedef unknown_storage_tag storage_category;

        // Construction and destruction
        NUMERICS_INLINE
        vector_binary ():
            e1_ (), e2_ () {}
        NUMERICS_INLINE
        vector_binary (const expression1_type &e1, const expression2_type &e2):
            e1_ (e1), e2_ (e2) {}

        NUMERICS_INLINE
        size_type size () const { 
            return common (e1_.size (), e2_.size ()); 
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i) const { 
            return functor_type () (e1_ (i), e2_ (i)); 
        }

        NUMERICS_INLINE
        value_type operator [] (size_type i) const { 
            return functor_type () (e1_ [i], e2_ [i]); 
        }

#ifdef NUMERICS_DEPRECATED
        NUMERICS_INLINE
        const_vector_range_type project (size_type start, size_type stop) const {
            return const_vector_range_type (*this, start, stop);
        }
        NUMERICS_INLINE
        const_vector_range_type project (const range &r) const {
            return const_vector_range_type (*this, r);
        }
#endif

        class const_iterator;
        typedef const_iterator iterator;

        // Element lookup
        NUMERICS_INLINE
        const_iterator find (size_type i) const {
            return const_iterator (*this, i, e1_.find (i), e2_.find (i));
        }

        // Iterator merges the iterators of the referenced expressions and  
        // enhances them with the binary functor.

        class const_iterator:
            public container_const_reference<vector_binary>, 
            public random_access_iterator_base<const_iterator, value_type> {
        public:
            typedef typename restrict_traits<typename E1::const_iterator::iterator_category, 
                                             typename E2::const_iterator::iterator_category>::iterator_category iterator_category;
#ifndef USE_MSVC
            typedef typename vector_binary::difference_type difference_type;
            typedef typename vector_binary::value_type value_type;
            typedef typename vector_binary::value_type reference;
            typedef typename vector_binary::const_pointer pointer;
#endif

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator ():
                container_const_reference<vector_binary> (), i_ (), it1_ (), it2_ () {}
            NUMERICS_INLINE
            const_iterator (const vector_binary &vb, size_type i, const const_iterator1_type &it1, const const_iterator2_type &it2):
                container_const_reference<vector_binary> (vb), i_ (i), it1_ (it1), it2_ (it2) {}
            
            // Random access specializations
            NUMERICS_INLINE
            void increment (std::random_access_iterator_tag) {
                ++ i_, ++ it1_, ++ it2_;
            }
            NUMERICS_INLINE
            void decrement (std::random_access_iterator_tag) {
                -- i_, -- it1_, -- it2_;
            }
            NUMERICS_INLINE
            value_type dereference (std::random_access_iterator_tag) const {
                return functor_type () (*it1_, *it2_); 
            }

            // Bidirectional specializations
            NUMERICS_INLINE
            void increment (std::bidirectional_iterator_tag) {
                if (it1_.index () <= i_) 
                    ++ it1_;
                if (it2_.index () <= i_) 
                    ++ it2_;
                i_ = std::min (it1_.index (), it2_.index ());
            }
            NUMERICS_INLINE
            void decrement (std::bidirectional_iterator_tag) {
                if (i_ <= it1_.index ()) 
                    -- it1_;
                if (i_ <= it2_.index ()) 
                    -- it2_;
                i_ = std::max (it1_.index (), it2_.index ());
            }
            NUMERICS_INLINE
            value_type dereference (std::bidirectional_iterator_tag) const {
#ifndef USE_MSVC
                value_type t1 = i_ - it1_.index () ? value_type () : *it1_;
                value_type t2 = i_ - it2_.index () ? value_type () : *it2_;
#else
                value_type t1, t2;
                if (i_ - it1_.index ()) t1 = value_type (); else t1 = *it1_;
                if (i_ - it2_.index ()) t2 = value_type (); else t2 = *it2_;
#endif
                return functor_type () (t1, t2);
            }

            // Arithmetic
            NUMERICS_INLINE
            const_iterator &operator ++ () {
                increment (iterator_category ());
                return *this;
            }
            NUMERICS_INLINE
            const_iterator &operator -- () {
                decrement (iterator_category ());
                return *this;
            }
            NUMERICS_INLINE
            const_iterator &operator += (difference_type n) {
                i_ += n, it1_ += n, it2_ += n;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator &operator -= (difference_type n) {
                i_ -= n, it1_ -= n, it2_ -= n;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const const_iterator &it) const {
                return common (it1_ - it.it1_, it2_ - it.it2_);
            }

            // Dereference
            NUMERICS_INLINE
            value_type operator * () const {
                return dereference (iterator_category ()); 
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                return i_;
            }

            // Assignment 
            NUMERICS_INLINE
            const_iterator &operator = (const const_iterator &it) {
                container_const_reference<vector_binary>::assign (&it ());
                it1_ = it.it1_;
                it2_ = it.it2_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it1_ == it.it1_ && it2_ == it.it2_; 
            }

        private:
            size_type i_;
            const_iterator1_type it1_;
            const_iterator2_type it2_;
        };

        NUMERICS_INLINE
        const_iterator begin () const {
//            return const_iterator (*this, 0, e1_.begin (), e2_.begin ()); 
            const_iterator1_type e1_begin (e1_.begin ());
            const_iterator2_type e2_begin (e2_.begin ());
            return const_iterator (*this, std::min (e1_begin.index (), e2_begin.index ()), 
                                   e1_begin, e2_begin);
        }
        NUMERICS_INLINE
        const_iterator end () const {
//            return const_iterator (*this, size (), e1_.end (), e2_.end ()); 
            const_iterator1_type e1_end (e1_.end ());
            const_iterator2_type e2_end (e2_.end ());
            return const_iterator (*this, std::max (e1_end.index (), e2_end.index ()), 
                                   e1_end, e2_end);
        }

        // Reverse iterator

#ifdef USE_MSVC
        typedef reverse_iterator<const_iterator, value_type, value_type> const_reverse_iterator;
#else
        typedef reverse_iterator<const_iterator> const_reverse_iterator;
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
#ifdef NUMERICS_ET_VALUE
        expression1_type e1_; 
        expression2_type e2_; 
#endif
#ifdef NUMERICS_ET_REFERENCE
        const expression1_type &e1_; 
        const expression2_type &e2_; 
#endif
    };

    template<class E1, class E2, class F>
    struct vector_binary_traits {
        typedef vector_binary<typename E1::const_closure_type, 
                              typename E2::const_closure_type, F> expression_type;
#ifdef NUMERICS_USE_ET
        typedef expression_type result_type; 
#else
        typedef vector<typename promote_traits<typename E1::value_type, 
                                               typename E2::value_type>::promote_type> result_type;
#endif
    };

    // (v1 + v2) [i] = v1 [i] + v2 [i]
    template<class E1, class E2>
    NUMERICS_INLINE
    typename vector_binary_traits<E1, E2, scalar_plus<typename E1::value_type, 
                                                      typename E2::value_type> >::result_type
    operator + (const vector_expression<E1> &e1, 
                const vector_expression<E2> &e2) {
        typedef NUMERICS_TYPENAME vector_binary_traits<E1, E2, scalar_plus<NUMERICS_TYPENAME E1::value_type, 
                                                                           NUMERICS_TYPENAME E2::value_type> >::expression_type expression_type;
        return expression_type (e1 (), e2 ());
    }

    // (v1 - v2) [i] = v1 [i] - v2 [i]
    template<class E1, class E2>
    NUMERICS_INLINE
    typename vector_binary_traits<E1, E2, scalar_minus<typename E1::value_type, 
                                                       typename E2::value_type> >::result_type
    operator - (const vector_expression<E1> &e1, 
                const vector_expression<E2> &e2) {
        typedef NUMERICS_TYPENAME vector_binary_traits<E1, E2, scalar_minus<NUMERICS_TYPENAME E1::value_type, 
                                                                            NUMERICS_TYPENAME E2::value_type> >::expression_type expression_type;
        return expression_type (e1 (), e2 ());
    }

    template<class E1, class E2, class F>
    class vector_binary_scalar:
        public vector_expression<vector_binary_scalar<E1, E2, F> > {
    public:
        typedef E1 expression1_type;
        typedef E2 expression2_type;
        typedef F functor_type;
        typedef typename E2::size_type size_type;
        typedef typename E2::difference_type difference_type;
        typedef typename F::result_type value_type;
        typedef const value_type &const_reference;
        typedef const_reference reference;
        typedef const value_type *const_pointer;
        typedef const_pointer pointer;
        typedef const vector_binary_scalar<E1, E2, F> const_closure_type;
        typedef typename E1::value_type const_iterator1_type;
        typedef typename E2::const_iterator const_iterator2_type;
#ifdef NUMERICS_DEPRECATED
#ifdef NUMERICS_DEPRECATED
        typedef const vector_expression_range<const_closure_type> const_vector_range_type;
#else
        typedef const vector_range<const_closure_type> const_vector_range_type;
#endif
#endif
        typedef unknown_storage_tag storage_category;

        // Construction and destruction
        NUMERICS_INLINE
        vector_binary_scalar ():
            e1_ (), e2_ () {}
        NUMERICS_INLINE
        vector_binary_scalar (const expression1_type &e1, const expression2_type &e2):
            e1_ (e1), e2_ (e2) {}

        NUMERICS_INLINE
        size_type size () const { 
            return e2_.size (); 
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i) const { 
            return functor_type () (e1_, e2_ (i)); 
        }

        NUMERICS_INLINE
        value_type operator [] (size_type i) const { 
            return functor_type () (e1_, e2_ [i]); 
        }

#ifdef NUMERICS_DEPRECATED
        NUMERICS_INLINE
        const_vector_range_type project (size_type start, size_type stop) const {
            return const_vector_range_type (*this, start, stop);
        }
        NUMERICS_INLINE
        const_vector_range_type project (const range &r) const {
            return const_vector_range_type (*this, r);
        }
#endif

        class const_iterator;
        typedef const_iterator iterator;

        // Element lookup
        NUMERICS_INLINE
        const_iterator find (size_type i) const {
            return const_iterator (*this, e1_, e2_.find (i));
        }

        // Iterator enhances the iterator of the referenced vector expression
        // with the binary functor.

        class const_iterator:
            public container_const_reference<vector_binary_scalar>, 
            public random_access_iterator_base<const_iterator, value_type> {
        public:
            typedef typename E2::const_iterator::iterator_category iterator_category;
#ifndef USE_MSVC
            typedef typename E2::const_iterator::difference_type difference_type;
            typedef typename E2::const_iterator::value_type value_type;
            typedef typename E2::const_iterator::value_type reference;
            typedef typename E2::const_iterator::pointer pointer;
#endif

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator ():
                container_const_reference<vector_binary_scalar> (), it1_ (), it2_ () {}
            NUMERICS_INLINE
            const_iterator (const vector_binary_scalar &vbs, const const_iterator1_type &it1, const const_iterator2_type &it2):
                container_const_reference<vector_binary_scalar> (vbs), it1_ (it1), it2_ (it2) {}

            // Arithmetic
            NUMERICS_INLINE
            const_iterator &operator ++ () {
                ++ it2_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator &operator -- () {
                -- it2_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator &operator += (difference_type n) {
                it2_ += n;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator &operator -= (difference_type n) {
                it2_ -= n;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const const_iterator &it) const {
                return it2_ - it.it2_;
            }

            // Dereference
            NUMERICS_INLINE
            value_type operator * () const {
                return functor_type () (it1_, *it2_); 
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                return it2_.index ();
            }

            // Assignment 
            NUMERICS_INLINE
            const_iterator &operator = (const const_iterator &it) {
                container_const_reference<vector_binary_scalar>::assign (&it ());
                it1_ = it.it1_;
                it2_ = it.it2_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator &it) const {
                check (&(*this) () == &it (), external_logic ());
                // FIXME: we shouldn't compare floats
                // check (it1_ == it.it1_, external_logic ());
                return it2_ == it.it2_; 
            }

        private:
            const_iterator1_type it1_;
            const_iterator2_type it2_;
        };

        NUMERICS_INLINE
        const_iterator begin () const {
            return const_iterator (*this, e1_, e2_.begin ()); 
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return const_iterator (*this, e1_, e2_.end ()); 
        }

        // Reverse iterator

#ifdef USE_MSVC
        typedef reverse_iterator<const_iterator, value_type, value_type> const_reverse_iterator;
#else
        typedef reverse_iterator<const_iterator> const_reverse_iterator;
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
#ifdef NUMERICS_ET_VALUE
        expression1_type e1_; 
        expression2_type e2_; 
#endif
#ifdef NUMERICS_ET_REFERENCE
        const expression1_type &e1_; 
        const expression2_type &e2_; 
#endif
    };

    template<class T1, class E2, class F>
    struct vector_binary_scalar_traits {
        typedef vector_binary_scalar<scalar_const_reference<T1>, 
                                     typename E2::const_closure_type, F> expression_type;
#ifdef NUMERICS_USE_ET
        typedef expression_type result_type; 
#else
        typedef vector<typename promote_traits<T1, typename E2::value_type>::promote_type> result_type;
#endif
    };

    // (t * v) [i] = t * v [i]
    template<class T1, class E2>
    NUMERICS_INLINE
    typename vector_binary_scalar_traits<T1, E2, scalar_multiplies<T1, typename E2::value_type> >::result_type
    operator * (const T1 &e1, 
                const vector_expression<E2> &e2) {
        typedef NUMERICS_TYPENAME vector_binary_scalar_traits<T1, E2, scalar_multiplies<T1, NUMERICS_TYPENAME E2::value_type> >::expression_type expression_type;
        return expression_type (e1, e2 ());
    }

    template<class E, class F>
    class vector_scalar_unary:
        public scalar_expression<typename F::result_type> {
    public:
        typedef E expression_type;
        typedef F functor_type;
        typedef typename F::size_type size_type;
        typedef typename F::difference_type difference_type;
        typedef typename F::result_type value_type;
        typedef typename E::const_iterator::iterator_category iterator_category;
        typedef unknown_storage_tag storage_category;

        // Construction and destruction
        NUMERICS_INLINE
        vector_scalar_unary (): 
            e_ () {}
        NUMERICS_INLINE
        vector_scalar_unary (const expression_type &e): 
            e_ (e) {}

        // Dense random access specialization
        NUMERICS_INLINE
        value_type evaluate (dense_random_access_iterator_tag) const {
            difference_type size = e_.size ();
            if (size >= NUMERICS_ITERATOR_THRESHOLD)
                return functor_type () (size, e_.begin ()); 
            else 
                return functor_type () (e_); 
        }

        // Packed bidirectional specialization
        NUMERICS_INLINE
        value_type evaluate (packed_bidirectional_iterator_tag) const {
            return functor_type () (e_.begin (), e_.end ()); 
        }

        // Sparse bidirectional specialization
        NUMERICS_INLINE
        value_type evaluate (sparse_bidirectional_iterator_tag) const {
            return functor_type () (e_.begin (), e_.end ()); 
        }

        NUMERICS_INLINE
        operator value_type () const { 
            return evaluate (iterator_category ());
        }

    private:
#ifdef NUMERICS_ET_VALUE
        expression_type e_;
#endif
#ifdef NUMERICS_ET_REFERENCE
        const expression_type &e_;
#endif
    };
    
    template<class E, class F>
    struct vector_scalar_unary_traits {
        typedef vector_scalar_unary<typename E::const_closure_type, F> expression_type;
#ifdef NUMERICS_USE_ET
         typedef expression_type result_type; 
#else
         typedef typename F::result_type result_type;
#endif
    };

    // sum v = sum (v [i])
    template<class E>
    NUMERICS_INLINE
    typename vector_scalar_unary_traits<E, vector_sum<typename E::value_type> >::result_type
    sum (const vector_expression<E> &e) {
        typedef NUMERICS_TYPENAME vector_scalar_unary_traits<E, vector_sum<NUMERICS_TYPENAME E::value_type> >::expression_type expression_type;
        return expression_type (e ());
    }

    // norm_1 v = sum (abs (v [i]))
    template<class E>
    NUMERICS_INLINE
    typename vector_scalar_unary_traits<E, vector_norm_1<typename E::value_type> >::result_type
    norm_1 (const vector_expression<E> &e) {
        typedef NUMERICS_TYPENAME vector_scalar_unary_traits<E, vector_norm_1<NUMERICS_TYPENAME E::value_type> >::expression_type expression_type;
        return expression_type (e ());
    }

    // norm_2 v = sqrt (sum (v [i] * v [i]))
    template<class E>
    NUMERICS_INLINE
    typename vector_scalar_unary_traits<E, vector_norm_2<typename E::value_type> >::result_type
    norm_2 (const vector_expression<E> &e) {
        typedef NUMERICS_TYPENAME vector_scalar_unary_traits<E, vector_norm_2<NUMERICS_TYPENAME E::value_type> >::expression_type expression_type;
        return expression_type (e ());
    }

    // norm_inf v = max (abs (v [i]))
    template<class E>
    NUMERICS_INLINE
    typename vector_scalar_unary_traits<E, vector_norm_inf<typename E::value_type> >::result_type
    norm_inf (const vector_expression<E> &e) {
        typedef NUMERICS_TYPENAME vector_scalar_unary_traits<E, vector_norm_inf<NUMERICS_TYPENAME E::value_type> >::expression_type expression_type;
        return expression_type (e ());
    }

    // index_norm_inf v = min (i: abs (v [i]) == max (abs (v [i])))
    template<class E>
    NUMERICS_INLINE
    typename vector_scalar_unary_traits<E, vector_index_norm_inf<typename E::value_type> >::result_type
    index_norm_inf (const vector_expression<E> &e) {
        typedef NUMERICS_TYPENAME vector_scalar_unary_traits<E, vector_index_norm_inf<NUMERICS_TYPENAME E::value_type> >::expression_type expression_type;
        return expression_type (e ());
    }

    template<class E1, class E2, class F>
    class vector_scalar_binary:
        public scalar_expression<typename F::result_type> {
    public:
        typedef E1 expression1_type;
        typedef E2 expression2_type;
        typedef F functor_type;
        typedef typename F::size_type size_type;
        typedef typename F::difference_type difference_type;
        typedef typename F::result_type value_type;
        typedef typename restrict_traits<typename E1::const_iterator::iterator_category,
                                         typename E2::const_iterator::iterator_category>::iterator_category iterator_category;
        typedef unknown_storage_tag storage_category;

        // Construction and destruction
        NUMERICS_INLINE
        vector_scalar_binary (): 
            e1_ (), e2_ () {}
        NUMERICS_INLINE
        vector_scalar_binary (const expression1_type &e1, const expression2_type  &e2): 
            e1_ (e1), e2_ (e2) {}

        // Dense random access specialization
        NUMERICS_INLINE
        value_type evaluate (dense_random_access_iterator_tag) const {
            difference_type size = common (e1_.size (), e2_.size ());
            if (size >= NUMERICS_ITERATOR_THRESHOLD)
                return functor_type () (size, e1_.begin (), e2_.begin ()); 
            else 
                return functor_type () (e1_, e2_); 
        }

        // Packed bidirectional specialization
        NUMERICS_INLINE
        value_type evaluate (packed_bidirectional_iterator_tag) const {
            return functor_type () (e1_.begin (), e1_.end (), e2_.begin (), e2_.end ()); 
        }

        // Sparse bidirectional specialization
        NUMERICS_INLINE
        value_type evaluate (sparse_bidirectional_iterator_tag) const {
            return functor_type () (e1_.begin (), e1_.end (), e2_.begin (), e2_.end (), sparse_bidirectional_iterator_tag ()); 
        }

        NUMERICS_INLINE
        operator value_type () const { 
            return evaluate (iterator_category ());
        }

    private:
#ifdef NUMERICS_ET_VALUE
        expression1_type e1_;
        expression2_type e2_;
#endif
#ifdef NUMERICS_ET_REFERENCE
        const expression1_type &e1_;
        const expression2_type &e2_;
#endif
    };

    template<class E1, class E2, class F>
    struct vector_scalar_binary_traits {
        typedef vector_scalar_binary<typename E1::const_closure_type, 
                                     typename E2::const_closure_type, F> expression_type;
#ifdef NUMERICS_USE_ET
        typedef expression_type result_type; 
#else
        typedef typename F::result_type result_type;
#endif
    };

    // inner_prod (v1, v2) = sum (v1 [i] * v2 [i]
    template<class E1, class E2>
    NUMERICS_INLINE
    typename vector_scalar_binary_traits<E1, E2, vector_inner_prod<typename E1::value_type, 
                                                                   typename E2::value_type,
                                                                   typename promote_traits<typename E1::value_type, 
                                                                                           typename E2::value_type>::promote_type> >::result_type 
    inner_prod (const vector_expression<E1> &e1, 
                const vector_expression<E2> &e2) {
        typedef NUMERICS_TYPENAME vector_scalar_binary_traits<E1, E2, vector_inner_prod<NUMERICS_TYPENAME E1::value_type, 
                                                                                        NUMERICS_TYPENAME E2::value_type,
                                                                                        NUMERICS_TYPENAME promote_traits<NUMERICS_TYPENAME E1::value_type, 
                                                                                                                         NUMERICS_TYPENAME E2::value_type>::promote_type> >::expression_type expression_type;
        return expression_type (e1 (), e2 ());
    }

    template<class E1, class E2>
    NUMERICS_INLINE
    typename vector_scalar_binary_traits<E1, E2, vector_inner_prod<typename E1::value_type, 
                                                                   typename E2::value_type,
                                                                   typename type_traits<typename promote_traits<typename E1::value_type, 
                                                                                                                typename E2::value_type>::promote_type>::precision_type> >::result_type 
    prec_inner_prod (const vector_expression<E1> &e1, 
                     const vector_expression<E2> &e2) {
        typedef NUMERICS_TYPENAME vector_scalar_binary_traits<E1, E2, vector_inner_prod<NUMERICS_TYPENAME E1::value_type, 
                                                                                        NUMERICS_TYPENAME E2::value_type,
                                                                                        NUMERICS_TYPENAME type_traits<NUMERICS_TYPENAME promote_traits<NUMERICS_TYPENAME E1::value_type, 
                                                                                                                                                       NUMERICS_TYPENAME E2::value_type>::promote_type>::precision_type> >::expression_type expression_type;
        return expression_type (e1 (), e2 ());
    }

#ifdef NUMERICS_DEPRECATED

    template<class E>
    class vector_expression_range:
        public vector_expression<vector_expression_range<E> > {
    public:
        typedef E expression_type;
        typedef typename E::size_type size_type;
        typedef typename E::difference_type difference_type;
        typedef typename E::value_type value_type;
        typedef typename E::const_reference const_reference;
        typedef const_reference reference;
        typedef typename E::const_pointer const_pointer;
        typedef const_pointer pointer;
        typedef const vector_expression_range<E> const_closure_type;
        typedef range::const_iterator const_iterator_type;
        typedef unknown_storage_tag storage_category;

        // Construction and destruction
        NUMERICS_INLINE
        vector_expression_range (): 
            e_ (), r_ () {}
        NUMERICS_INLINE
        vector_expression_range (const expression_type &e, const range &r): 
            e_ (e), r_ (r) {}
        NUMERICS_INLINE
        vector_expression_range (const expression_type &e, size_type start, size_type stop): 
            e_ (e), r_ (start, stop) {}

        NUMERICS_INLINE
        size_type size () const { 
            return r_.size (); 
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i) const { 
            return e_ (r_ (i)); 
        }

        NUMERICS_INLINE
        value_type operator [] (size_type i) const { 
            return e_ [r_ (i)]; 
        }

#ifdef NUMERICS_DEPRECATED
        NUMERICS_INLINE
        vector_expression_range project (size_type start, size_type stop) const {
            return vector_expression_range (e_, r_.composite (start, stop));
        }
        NUMERICS_INLINE
        vector_expression_range project (const range &r) const {
            return vector_expression_range (e_, r_.composite (r));
        }
#endif

        class const_iterator;
        typedef const_iterator iterator;

        // Element lookup
        NUMERICS_INLINE
        const_iterator find (size_type i) const {
            return const_iterator (e_, const_iterator_type (r_, i));
        }

        // Iterator simply is an index.
        // FIXME: is it possible to rearrange this using pointers?

        class const_iterator:
            public container_const_reference<expression_type>,
            public random_access_iterator_base<const_iterator, value_type> {
        public:
            typedef typename E::const_iterator::iterator_category iterator_category;
#ifndef USE_MSVC
            typedef typename E::const_iterator::difference_type difference_type;
            typedef typename E::const_iterator::value_type value_type;
            typedef typename E::const_iterator::value_type reference;
            typedef typename E::const_iterator::pointer pointer;
#endif

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator ():
                container_const_reference<expression_type> (), it_ () {}
            NUMERICS_INLINE
            const_iterator (const expression_type &e, const const_iterator_type &it):
                container_const_reference<expression_type> (e), it_ (it) {}

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
                return (*this) () (*it_);
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                return it_.index ();
            }

            // Assignment 
            NUMERICS_INLINE
            const_iterator &operator = (const const_iterator &it) {
                container_const_reference<expression_type>::assign (&it ());
                it_ = it.it_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it_ == it.it_;
            }

        private:
            const_iterator_type it_;
        };

        NUMERICS_INLINE
        const_iterator begin () const {
            return const_iterator (e_, r_.begin ());
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return const_iterator (e_, r_.end ());
        }

        // Reverse iterator

#ifdef USE_MSVC
        typedef reverse_iterator<const_iterator, value_type, value_type> const_reverse_iterator;
#else
        typedef reverse_iterator<const_iterator> const_reverse_iterator;
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
#ifdef NUMERICS_ET_VALUE
        expression_type e_;
        range r_;
#endif
#ifdef NUMERICS_ET_REFERENCE
        const expression_type &e_;
        const range &r_;
#endif
    };

    template<class E>
    struct vector_expression_range_traits {
        typedef vector_expression_range<typename E::const_closure_type> expression_type;
#ifdef NUMERICS_USE_ET
        typedef expression_type result_type; 
#else
        typedef vector<typename E::value_type> result_type;
#endif
    };

    template<class E>
    NUMERICS_INLINE
    typename vector_expression_range_traits<E>::result_type
    project (const vector_expression<E> &e, 
             const range &r) {
        typedef NUMERICS_TYPENAME vector_expression_range_traits<E>::expression_type expression_type;
        return expression_type (e (), r);
    }
    template<class E>
    NUMERICS_INLINE
    typename vector_expression_range_traits<E>::result_type
    project (const vector_expression<E> &e, 
             typename E::size_type start,
             typename E::size_type stop) {
        typedef NUMERICS_TYPENAME vector_expression_range_traits<E>::expression_type expression_type;
        return expression_type (e (), start, stop);
    }

    template<class E>
    class vector_expression_slice:
        public vector_expression<vector_expression_slice<E> > {
    public:
        typedef E expression_type;
        typedef typename E::size_type size_type;
        typedef typename E::difference_type difference_type;
        typedef typename E::value_type value_type;
        typedef typename E::const_reference const_reference;
        typedef const_reference reference;
        typedef typename E::const_pointer const_pointer;
        typedef const_pointer pointer;
        typedef const vector_expression_slice<E> const_closure_type;
        typedef slice::const_iterator const_iterator_type;
        typedef unknown_storage_tag storage_category;

        // Construction and destruction
        NUMERICS_INLINE
        vector_expression_slice (): 
            e_ (), s_ () {}
        NUMERICS_INLINE
        vector_expression_slice (const expression_type &e, const slice &s): 
            e_ (e), s_ (s) {}
        NUMERICS_INLINE
        vector_expression_slice (const expression_type &e, size_type start, size_type stride, size_type size): 
            e_ (e), s_ (start, stride, size) {}

        NUMERICS_INLINE
        size_type size () const { 
            return s_.size (); 
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i) const { 
            return e_ (s_ (i)); 
        }

        NUMERICS_INLINE
        value_type operator [] (size_type i) const { 
            return e_ [s_ (i)]; 
        }

#ifdef NUMERICS_DEPRECATED
        NUMERICS_INLINE
        vector_expression_slice project (const range &r) const {
            return vector_expression_slice (e_, s_.composite (r));
        }
        NUMERICS_INLINE
        vector_expression_slice project (const slice &s) const {
            return vector_expression_slice (e_, s_.composite (s));
        }
#endif

        class const_iterator;
        typedef const_iterator iterator;

        // Element lookup
        NUMERICS_INLINE
        const_iterator find (size_type i) const {
            return const_iterator (e_, const_iterator_type (s_, i));
        }

        // Iterator simply is an index.

        class const_iterator:
            public container_const_reference<expression_type>,
            public random_access_iterator_base<const_iterator, value_type> {
        public:
            typedef typename E::const_iterator::iterator_category iterator_category;
#ifndef USE_MSVC
            typedef typename E::const_iterator::difference_type difference_type;
            typedef typename E::const_iterator::value_type value_type;
            typedef typename E::const_iterator::value_type reference;
            typedef typename E::const_iterator::pointer pointer;
#endif

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator ():
                container_const_reference<expression_type> (), it_ () {}
            NUMERICS_INLINE
            const_iterator (const expression_type &e, const const_iterator_type &it):
                container_const_reference<expression_type> (e), it_ (it) {}

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
                return (*this) () (*it_);
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                return it_.index ();
            }

            // Assignment 
            NUMERICS_INLINE
            const_iterator &operator = (const const_iterator &it) {
                container_const_reference<expression_type>::assign (&it ());
                it_ = it.it_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it_ == it.it_;
            }

        private:
            const_iterator_type it_;
        };

        NUMERICS_INLINE
        const_iterator begin () const {
            return const_iterator (e_, s_.begin ());
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return const_iterator (e_, s_.end ());
        }

        // Reverse iterator

#ifdef USE_MSVC
        typedef reverse_iterator<const_iterator, value_type, value_type> const_reverse_iterator;
#else
        typedef reverse_iterator<const_iterator> const_reverse_iterator;
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
#ifdef NUMERICS_ET_VALUE
        expression_type e_;
        slice s_;
#endif
#ifdef NUMERICS_ET_REFERENCE
        const expression_type &e_;
        const slice &s_;
#endif
    };

    template<class E>
    struct vector_expression_slice_traits {
        typedef vector_expression_slice<typename E::const_closure_type> expression_type;
#ifdef NUMERICS_USE_ET
        typedef expression_type result_type; 
#else
        typedef vector<typename E::value_type> result_type;
#endif
    };

    template<class E>
    NUMERICS_INLINE
    typename vector_expression_slice_traits<E>::result_type
    project (const vector_expression<E> &e, 
             const slice &s) {
        typedef NUMERICS_TYPENAME vector_expression_slice_traits<E>::expression_type expression_type;
        return expression_type (e (), s);
    }
    template<class E>
    NUMERICS_INLINE
    typename vector_expression_slice_traits<E>::result_type
    project (const vector_expression<E> &e, 
             typename E::size_type start,
             typename E::size_type stride,
             typename E::size_type size) {
        typedef NUMERICS_TYPENAME vector_expression_slice_traits<E>::expression_type expression_type;
        return expression_type (e (), start, stride, size);
    }

#endif

}

#endif




