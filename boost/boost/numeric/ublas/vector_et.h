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

namespace numerics {

    template<class T>
    struct scalar_expression {
        typedef T value_type;

        NUMERICS_INLINE
        virtual ~scalar_expression () {}
    };

    template<class T>
    class scalar_const_reference:
        public scalar_expression<T> {
    public:
        typedef T value_type;
        
        NUMERICS_INLINE
        scalar_const_reference (const value_type &t):
            t_ (t) {}

        NUMERICS_INLINE
        operator value_type () const {
            return t_;
        }

    private:
        const value_type &t_;
    };

    template<class T, class E>
    struct vector_expression {
        typedef unsigned size_type;
        typedef T value_type;
        typedef T &reference_type;
        typedef E expression_type;

        NUMERICS_INLINE
        virtual ~vector_expression () {}

        NUMERICS_INLINE
        void resize (size_type size) {
            expression_type *e = static_cast<expression_type *> (this);
            e->resize (size); 
        }

        NUMERICS_INLINE
        size_type size () const { 
            const expression_type *e = static_cast<const expression_type *> (this);
            return e->size (); 
        }

        NUMERICS_INLINE
        value_type operator () (size_type i) const { 
            const expression_type *e = static_cast<const expression_type *> (this);
            return (*e) (i); 
        }
        NUMERICS_INLINE
        reference_type operator () (size_type i)  { 
            expression_type *e = static_cast<expression_type *> (this);
            return (*e) (i); 
        }

#ifdef NUMERICS_USE_ITERATOR
        const expression_type &expression () const {
            const expression_type *e = static_cast<const expression_type *> (this);
            return *e; 
        }
        expression_type &expression () {
            expression_type *e = static_cast<expression_type *> (this);
            return *e; 
        }
#endif // NUMERICS_USE_ITERATOR
    };

    template<class E>
    class vector_const_reference:
        public vector_expression<typename E::value_type, vector_const_reference<E> > {
    public:
        typedef E expression_type;
        typedef typename E::size_type size_type;
        typedef typename E::value_type value_type;
#ifdef NUMERICS_USE_ITERATOR
        typedef typename E::expression_type::const_iterator const_iterator_type;
#endif // NUMERICS_USE_ITERATOR

        NUMERICS_INLINE
        vector_const_reference (const expression_type &e):
            e_ (e) {}

        NUMERICS_INLINE
        size_type size () const { 
            return e_.size (); 
        }

        NUMERICS_INLINE
        value_type operator () (size_type i) const { 
            return e_ (i); 
        }

#ifdef NUMERICS_USE_ITERATOR
        class const_iterator {
        public:
            NUMERICS_INLINE 
            const_iterator (const const_iterator_type &it): 
                it_ (it) {}

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
            value_type operator * () const {
                return *it_;
            }

            NUMERICS_INLINE
            friend bool operator == (const const_iterator &it1, const const_iterator &it2) {
                return it1.it_ == it2.it_; 
            }
            NUMERICS_INLINE
            friend bool operator != (const const_iterator &it1, const const_iterator &it2) {
                return ! (it1 == it2);
            }

        protected:
            const_iterator_type it_;
        };

        NUMERICS_INLINE
        const_iterator begin () const {
            return const_iterator (e_.expression ().begin ());
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return const_iterator (e_.expression ().end ()); 
        }
#endif // NUMERICS_USE_ITERATOR

    private:
        const expression_type &e_;
    };

    template<class E>
    class vector_reference: 
        public vector_expression<typename E::value_type, vector_reference<E> > {
    public:
        typedef E expression_type;
        typedef typename E::size_type size_type;
        typedef typename E::value_type value_type;
        typedef typename E::reference_type reference_type;
#ifdef NUMERICS_USE_ITERATOR
        typedef typename E::expression_type::const_iterator const_iterator_type;
        typedef typename E::expression_type::iterator iterator_type;
#endif // NUMERICS_USE_ITERATOR

        NUMERICS_INLINE
        vector_reference (expression_type &e):
            e_ (e) {}

        NUMERICS_INLINE
        void resize (size_type size) {
            e_.resize (size);
        }

        NUMERICS_INLINE
        size_type size () const { 
            return e_.size (); 
        }

        NUMERICS_INLINE
        value_type operator () (size_type i) const { 
            return e_ (i); 
        }
        NUMERICS_INLINE
        reference_type operator () (size_type i) { 
            return e_ (i); 
        }

#ifdef NUMERICS_USE_ITERATOR
        class const_iterator {
        public:
            NUMERICS_INLINE 
            const_iterator (const const_iterator_type &it): 
                it_ (it) {}

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
            value_type operator * () const {
                return *it_;
            }

            NUMERICS_INLINE
            friend bool operator == (const const_iterator &it1, const const_iterator &it2) {
                return it1.it_ == it2.it_; 
            }
            NUMERICS_INLINE
            friend bool operator != (const const_iterator &it1, const const_iterator &it2) {
                return ! (it1 == it2);
            }

        protected:
            const_iterator_type it_;
        };

        NUMERICS_INLINE
        const_iterator begin () const {
            return const_iterator (e_.expression ().begin ());
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return const_iterator (e_.expression ().end ()); 
        }

        class iterator:
            public const_iterator {
        public:
            NUMERICS_INLINE
            iterator (const iterator_type &it): 
                const_iterator (it) {}

            NUMERICS_INLINE
            reference_type operator * () {
                return *it_;
            }
        };

        NUMERICS_INLINE
        iterator begin () {
            return iterator (e_.expression ().begin ());
        }
        NUMERICS_INLINE
        iterator end () {
            return iterator (e_.expression ().end ()); 
        }
#endif // NUMERICS_USE_ITERATOR

    private:
        expression_type &e_;
    };

    template<class E, class F>
    class vector_unary: 
        public vector_expression<typename F::value_type, vector_unary<E, F> > {
    public:
        typedef E expression_type;
        typedef F functor_type;
        typedef typename E::size_type size_type;
        typedef typename F::value_type value_type;
#ifdef NUMERICS_USE_ITERATOR
        typedef typename E::const_iterator const_iterator_type;
#endif // NUMERICS_USE_ITERATOR

        NUMERICS_INLINE
        vector_unary (const expression_type &e): 
            e_ (e) {}

        NUMERICS_INLINE
        size_type size () const { 
            return e_.size (); 
        }

        NUMERICS_INLINE
        value_type operator () (size_type i) const { 
            return functor_type () (e_ (i)); 
        }

#ifdef NUMERICS_USE_ITERATOR
        class const_iterator {
        public:
            NUMERICS_INLINE
            const_iterator (const const_iterator_type &it): 
                it_ (it) {}

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
            value_type operator * () const {
                return functor_type () (*it_); 
            }

            NUMERICS_INLINE
            friend bool operator == (const const_iterator &it1, const const_iterator &it2) {
                return it1.it_ == it2.it_; 
            }
            NUMERICS_INLINE
            friend bool operator != (const const_iterator &it1, const const_iterator &it2) {
                return ! (it1 == it2);
            }

        protected:
            const_iterator_type it_;
        };

        NUMERICS_INLINE
        const_iterator begin () const {
            return const_iterator (e_.begin ()); 
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return const_iterator (e_.end ()); 
        }
#endif // NUMERICS_USE_ITERATOR

    private:
        expression_type e_; 
    };

#ifdef NUMERICS_USE_ET

    template<class T, class E> 
    NUMERICS_INLINE
    vector_unary<vector_const_reference<vector_expression<T, E> >, 
                 scalar_negate<T> > 
    operator - (const vector_expression<T, E> &e) {
        return vector_unary<vector_const_reference<vector_expression<T, E> >, 
                            scalar_negate<T> > (e);
    }

    template<class T, class E> 
    NUMERICS_INLINE
    vector_unary<vector_const_reference<vector_expression<T, E> >, 
                 scalar_conj<T> > 
    conj (const vector_expression<T, E> &e) {
        return vector_unary<vector_const_reference<vector_expression<T, E> >, 
                            scalar_conj<T> > (e);
    }

#else // NUMERICS_USE_ET

    template<class T, class A, class F> 
    NUMERICS_INLINE
    vector<T>
    operator - (const vector<T, A, F> &e) {
        vector<T> r (e.size ());
        scalar_negate<T> f;
#ifndef NUMERICS_USE_ITERATOR
        for (size_type i = 0; i < r.size (); ++ i) 
            r (i) = f (e [i]);
#else // NUMERICS_USE_ITERATOR
        vector<T>::iterator itr (r.begin ());
        vector<T, A, F>::const_iterator ite (e.begin ());
        for (size_type i = 0; i < r.size (); ++ i, ++ itr, ++ ite) 
            *itr  = f (*ite);
#endif // NUMERICS_USE_ITERATOR
        return r;
    }

    template<class T, class A, class F> 
    NUMERICS_INLINE
    vector<T>
    conj (const vector<T, A, F> &e) {
        vector<T> r (e.size ());
        scalar_conj<T> f;
#ifndef NUMERICS_USE_ITERATOR
        for (size_type i = 0; i < r.size (); ++ i) 
            r (i) = f (e [i]);
#else // NUMERICS_USE_ITERATOR
        vector<T>::iterator itr (r.begin ());
        vector<T, A, F>::const_iterator ite (e.begin ());
        for (; itr != r.end (); ++ itr, ++ ite) 
            *itr  = f (*ite);
#endif // NUMERICS_USE_ITERATOR
        return r;
    }

#endif // NUMERICS_USE_ET

    template<class E1, class E2, class F>
    class vector_binary:
        public vector_expression<typename F::value_type, vector_binary<E1, E2, F> > {
    public:
        typedef E1 expression1_type;
        typedef E2 expression2_type;
        typedef F functor_type;
        typedef promote_traits<typename E1::size_type, typename E2::size_type>::promote_type size_type;
        typedef typename F::value_type value_type;
#ifdef NUMERICS_USE_ITERATOR
        typedef typename E1::const_iterator const_iterator1_type;
        typedef typename E2::const_iterator const_iterator2_type;
#endif // NUMERICS_USE_ITERATOR

        NUMERICS_INLINE
        vector_binary (const expression1_type &e1, const expression2_type &e2):
            e1_ (e1), e2_ (e2) {}

        NUMERICS_INLINE
        size_type size () const { 
            return common (e1_.size (), e2_.size ()); 
        }

        NUMERICS_INLINE
        value_type operator () (size_type i) const { 
            return functor_type () (e1_ (i), e2_ (i)); 
        }

#ifdef NUMERICS_USE_ITERATOR
        class const_iterator {
        public:
            NUMERICS_INLINE
            const_iterator (const const_iterator1_type &it1, const const_iterator2_type &it2):
                it1_ (it1), it2_ (it2) {}

            NUMERICS_INLINE
            const_iterator &operator ++ () {
                ++ it1_, ++ it2_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator &operator -- () {
                -- it1_, -- it2_;
                return *this;
            }
            NUMERICS_INLINE
            value_type operator * () const {
                return functor_type () (*it1_, *it2_); 
            }

            NUMERICS_INLINE
            friend bool operator == (const const_iterator &it1, const const_iterator &it2) {
                return it1.it1_ == it2.it1_ && it1.it2_ == it2.it2_; 
            }
            NUMERICS_INLINE
            friend bool operator != (const const_iterator &it1, const const_iterator &it2) {
                return ! (it1 == it2);
            }

        protected:
            const_iterator1_type it1_;
            const_iterator2_type it2_;
        };

        NUMERICS_INLINE
        const_iterator begin () const {
            return const_iterator (e1_.begin (), e2_.begin ()); 
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return const_iterator (e1_.end (), e2_.end ()); 
        }
#endif // NUMERICS_USE_ITERATOR

    private:
        expression1_type e1_; 
        expression2_type e2_; 
    };

#ifdef NUMERICS_USE_ET

    template<class T1, class E1, class T2, class E2>
    NUMERICS_INLINE
    vector_binary<vector_const_reference<vector_expression<T1, E1> >, 
                  vector_const_reference<vector_expression<T2, E2> >, 
                  scalar_plus<T1, T2> > 
    operator + (const vector_expression<T1, E1> &e1, 
                const vector_expression<T2, E2> &e2) {
        return vector_binary<vector_const_reference<vector_expression<T1, E1> >, 
                             vector_const_reference<vector_expression<T2, E2> >, 
                             scalar_plus<T1, T2> > (e1, e2);
    }

    template<class T1, class E1, class T2, class E2>
    NUMERICS_INLINE
    vector_binary<vector_const_reference<vector_expression<T1, E1> >, 
                  vector_const_reference<vector_expression<T2, E2> >, 
                  scalar_minus<T1, T2> > 
    operator - (const vector_expression<T1, E1> &e1, 
                const vector_expression<T2, E2> &e2) {
        return vector_binary<vector_const_reference<vector_expression<T1, E1> >, 
                             vector_const_reference<vector_expression<T2, E2> >, 
                             scalar_minus<T1, T2> > (e1, e2);
    }

#else // NUMERICS_USE_ET

    template<class T1, class A1, class F1, class T2, class A2, class F2> 
    NUMERICS_INLINE
    vector<promote_traits<T1, T2>::promote_type>
    operator + (const vector<T1, A1, F1> &e1, 
                const vector<T2, A2, F2> &e2) {
        vector<promote_traits<T1, T2>::promote_type> r (common (e1.size (), e2.size ()));
        scalar_plus<T1, T2> f;
#ifndef NUMERICS_USE_ITERATOR
        for (size_type i = 0; i < r.size (); ++ i) 
            r (i) = f (e1 [i], e2 [i]);
#else // NUMERICS_USE_ITERATOR
        vector<promote_traits<T1, T2>::promote_type>::iterator itr (r.begin ());
        vector<T1, A1, F1>::const_iterator ite1 (e1.begin ());
        vector<T2, A2, F2>::const_iterator ite2 (e2.begin ());
        for (size_type i = 0; i < r.size (); ++ i, ++ itr, ++ ite1, ++ ite2) 
            *itr = f (*ite1, *ite2);
#endif // NUMERICS_USE_ITERATOR
        return r;
    }

    template<class T1, class A1, class F1, class T2, class A2, class F2> 
    NUMERICS_INLINE
    vector<promote_traits<T1, T2>::promote_type>
    operator - (const vector<T1, A1, F1> &e1, 
                const vector<T2, A2, F2> &e2) {
        vector<promote_traits<T1, T2>::promote_type> r (common (e1.size (), e2.size ()));
        scalar_minus<T1, T2> f;
#ifndef NUMERICS_USE_ITERATOR
        for (size_type i = 0; i < r.size (); ++ i) 
            r (i) = f (e1 [i], e2 [i]);
#else // NUMERICS_USE_ITERATOR
        vector<promote_traits<T1, T2>::promote_type>::iterator itr (r.begin ());
        vector<T1, A1, F1>::const_iterator ite1 (e1.begin ());
        vector<T2, A2, F2>::const_iterator ite2 (e2.begin ());
        for (; itr != r.end (); ++ itr, ++ ite1, ++ ite2) 
            *itr = f (*ite1, *ite2);
#endif // NUMERICS_USE_ITERATOR
        return r;
    }

#endif // NUMERICS_USE_ET

    template<class E1, class E2, class F>
    class vector_binary_scalar:
        public vector_expression<typename F::value_type, vector_binary_scalar<E1, E2, F> > {
    public:
        typedef E1 expression1_type;
        typedef E2 expression2_type;
        typedef F functor_type;
        typedef typename E2::size_type size_type;
        typedef typename F::value_type value_type;
#ifdef NUMERICS_USE_ITERATOR
        typedef typename E1::value_type const_iterator1_type;
        typedef typename E2::const_iterator const_iterator2_type;
#endif

        NUMERICS_INLINE
        vector_binary_scalar (const expression1_type &e1, const expression2_type &e2):
            e1_ (e1), e2_ (e2) {}

        NUMERICS_INLINE
        size_type size () const { 
            return e2_.size (); 
        }

        NUMERICS_INLINE
        value_type operator () (size_type i) const { 
            return functor_type () (e1_, e2_ (i)); 
        }

#ifdef NUMERICS_USE_ITERATOR
        class const_iterator {
        public:
            NUMERICS_INLINE
            const_iterator (const const_iterator1_type &it1, const const_iterator2_type &it2):
                it1_ (it1), it2_ (it2) {}

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
            value_type operator * () const {
                return functor_type () (it1_, *it2_); 
            }

            NUMERICS_INLINE
            friend bool operator == (const const_iterator &it1, const const_iterator &it2) {
                return it1.it1_ == it2.it1_ && it1.it2_ == it2.it2_; 
            }
            NUMERICS_INLINE
            friend bool operator != (const const_iterator &it1, const const_iterator &it2) {
                return ! (it1 == it2);
            }

        protected:
            const_iterator1_type it1_;
            const_iterator2_type it2_;
        };

        NUMERICS_INLINE
        const_iterator begin () const {
            return const_iterator (e1_, e2_.begin ()); 
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return const_iterator (e1_, e2_.end ()); 
        }
#endif // NUMERICS_USE_ITERATOR

    private:
        expression1_type e1_; 
        expression2_type e2_; 
    };

#ifdef NUMERICS_USE_ET

    template<class T1, class T2, class E2>
    NUMERICS_INLINE
    vector_binary_scalar<scalar_const_reference<T1>, 
                         vector_const_reference<vector_expression<T2, E2> >, 
                         scalar_multiplies<T1, T2> > 
    operator * (const T1 &e1, 
                const vector_expression<T2, E2> &e2) {
        return vector_binary_scalar<scalar_const_reference<T1>, 
                                    vector_const_reference<vector_expression<T2, E2> >, 
                                    scalar_multiplies<T1, T2> > (e1, e2);
    }

#else // NUMERICS_USE_ET

    template<class T1, class T2, class A2, class F2> 
    NUMERICS_INLINE
    vector<promote_traits<T1, T2>::promote_type>
    operator * (const T1 &e1, 
                const vector<T2, A2, F2> &e2) {
        vector<promote_traits<T1, T2>::promote_type> r (e2.size ());
        scalar_multiplies<T1, T2> f;
#ifndef NUMERICS_USE_ITERATOR
        for (size_type i = 0; i < r.size (); ++ i) 
            r (i) = f (e1, e2 [i]);
#else // NUMERICS_USE_ITERATOR
        vector<promote_traits<T1, T2>::promote_type>::iterator itr (r.begin ());
        vector<T2, A2, F2>::const_iterator ite2 (e2.begin ());
        for (; itr != r.end (); ++ itr, ++ ite2) 
            *itr = f (e1, *ite2);
#endif // NUMERICS_USE_ITERATOR
        return r;
    }

#endif // NUMERICS_USE_ET

    template<class E, class F>
    class vector_scalar_unary:
        public scalar_expression<typename F::value_type> {
    public:
        typedef E expression_type;
        typedef F functor_type;
        typedef typename F::value_type value_type;

        NUMERICS_INLINE
        vector_scalar_unary (const expression_type &e): 
            e_ (e) {}

        NUMERICS_INLINE
        operator value_type () const { 
#ifndef NUMERICS_USE_ITERATOR
            return functor_type () (e_); 
#else // NUMERICS_USE_ITERATOR
            return functor_type () (e_.size (), e_.begin ()); 
#endif // NUMERICS_USE_ITERATOR
        }

    private:
        expression_type e_;
    };

#ifdef NUMERICS_USE_ET

    template<class T, class E>
    NUMERICS_INLINE
    vector_scalar_unary<vector_const_reference<vector_expression<T, E> >, 
                        vector_sum<T> > 
    sum (const vector_expression<T, E> &e) {
        return vector_scalar_unary<vector_const_reference<vector_expression<T, E> >, 
                                   vector_sum<T> > (e);
    }

    template<class T, class E>
    NUMERICS_INLINE
    vector_scalar_unary<vector_const_reference<vector_expression<T, E> >, 
                        vector_norm_1<T> > 
    norm_1 (const vector_expression<T, E> &e) {
        return vector_scalar_unary<vector_const_reference<vector_expression<T, E> >, 
                                   vector_norm_1<T> > (e);
    }

    template<class T, class E>
    NUMERICS_INLINE
    vector_scalar_unary<vector_const_reference<vector_expression<T, E> >, 
                        vector_norm_2<T> > 
    norm_2 (const vector_expression<T, E> &e) {
        return vector_scalar_unary<vector_const_reference<vector_expression<T, E> >, 
                                    vector_norm_2<T> > (e);
    }

    template<class T, class E>
    NUMERICS_INLINE
    vector_scalar_unary<vector_const_reference<vector_expression<T, E> >, 
                        vector_norm_inf<T> > 
    norm_inf (const vector_expression<T, E> &e) {
        return vector_scalar_unary<vector_const_reference<vector_expression<T, E> >, 
                                   vector_norm_inf<T> > (e);
    }

#else // NUMERICS_USE_ET

    template<class T, class A, class F> 
    NUMERICS_INLINE
    T
    sum (const vector<T, A, F> &e) {
        vector_sum<T> f;
        return f (e);
    }

    template<class T, class A, class F> 
    NUMERICS_INLINE
    type_traits<T>::norm_type
    norm_1 (const vector<T, A, F> &e) {
        vector_norm_1<T> f;
        return f (e);
    }

    template<class T, class A, class F> 
    NUMERICS_INLINE
    type_traits<T>::norm_type
    norm_2 (const vector<T, A, F> &e) {
        vector_norm_2<T> f;
        return f (e);
    }

    template<class T, class A, class F> 
    NUMERICS_INLINE
    type_traits<T>::norm_type
    norm_inf (const vector<T, A, F> &e) {
        vector_norm_inf<T> f;
        return f (e);
    }

#endif // NUMERICS_USE_ET

    template<class E1, class E2, class F>
    class vector_scalar_binary:
        public scalar_expression<typename F::value_type> {
    public:
        typedef E1 expression1_type;
        typedef E2 expression2_type;
        typedef F functor_type;
        typedef typename F::value_type value_type;

        NUMERICS_INLINE
        vector_scalar_binary (const expression1_type &e1, const expression2_type  &e2): 
            e1_ (e1), e2_ (e2) {}

        NUMERICS_INLINE
        operator value_type () const { 
#ifndef NUMERICS_USE_ITERATOR
            return functor_type () (e1_, e2_); 
#else // NUMERICS_USE_ITERATOR
            return functor_type () (common (e1_.size (), e2_.size ()), e1_.begin (), e2_.begin ()); 
#endif // NUMERICS_USE_ITERATOR
        }

    private:
        expression1_type e1_;
        expression2_type e2_;
    };

#ifdef NUMERICS_USE_ET

    template<class T1, class E1, class T2, class E2>
    NUMERICS_INLINE
    vector_scalar_binary<vector_const_reference<vector_expression<T1, E1> >, 
                         vector_const_reference<vector_expression<T2, E2> >, 
                         vector_inner_prod<T1, T2> > 
    inner_prod (const vector_expression<T1, E1> &e1, 
                const vector_expression<T2, E2> &e2) {
        return vector_scalar_binary<vector_const_reference<vector_expression<T1, E1> >, 
                                    vector_const_reference<vector_expression<T2, E2> >, 
                                    vector_inner_prod<T1, T2> > (e1, e2);
    }
    template<class T1, class E1, class T2, class E2>
    NUMERICS_INLINE
    vector_scalar_binary<vector_const_reference<vector_expression<T1, E1> >, 
                         vector_const_reference<vector_expression<T2, E2> >, 
                         vector_prec_inner_prod<T1, T2> > 
    prec_inner_prod (const vector_expression<T1, E1> &e1, 
                     const vector_expression<T2, E2> &e2) {
        return vector_scalar_binary<vector_const_reference<vector_expression<T1, E1> >, 
                                    vector_const_reference<vector_expression<T2, E2> >, 
                                    vector_prec_inner_prod<T1, T2> > (e1, e2);
    }

#else // NUMERICS_USE_ET

    template<class T1, class A1, class F1, class T2, class A2, class F2> 
    NUMERICS_INLINE
    promote_traits<T1, T2>::promote_type
    inner_prod (const vector<T1, A1, F1> &e1, 
                const vector<T2, A2, F2> &e2) {
        vector_inner_prod<T1, T2> f;
        return f (e1, e2);
    }
    template<class T1, class A1, class F1, class T2, class A2, class F2> 
    NUMERICS_INLINE
    type_traits<promote_traits<T1, T2>::promote_type>::norm_type
    prec_inner_prod (const vector<T1, A1, F1> &e1, 
                     const vector<T2, A2, F2> &e2) {
        vector_prec_inner_prod<T1, T2> f;
        return f (e1, e2);
    }

#endif // NUMERICS_USE_ET

    template<class E>
    class vector_expr_slice:
        public vector_expression<typename E::value_type, vector_expr_slice<E> > {
    public:
        typedef E expression_type;
        typedef typename E::size_type size_type;
        typedef typename E::value_type value_type;

        NUMERICS_INLINE
        vector_expr_slice (const expression_type &e, const slice &s): 
            e_ (e), s_ (s) {}

        NUMERICS_INLINE
        size_type size () const { 
            return s_.size (); 
        }

        NUMERICS_INLINE
        value_type operator () (size_type i) const { 
            return e_ (s_ (i)); 
        }

    private:
        expression_type e_;
        slice s_;
    };

}

#endif