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

#ifndef NUMERICS_MATRIX_SP_H
#define NUMERICS_MATRIX_SP_H

#include "config.h"
#include "storage.h"
#include "storage_sp.h"
#include "vector_et.h"
#include "matrix_et.h"

// Iterators based on ideas of Jeremy Siek

namespace numerics {

    // Array based sparse matrix class 
    template<class T, class A, class F>
    class sparse_matrix: 
        public matrix_expression<sparse_matrix<T, A, F> > {
    public:      
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef const T &const_reference_type;
        typedef T &reference_type;
        typedef T *pointer_type;
        typedef const A const_array_type;
        typedef A array_type;
        typedef F functor_type;
        typedef sparse_matrix<T, A, F> self_type;
        typedef matrix_const_reference<self_type> const_closure_type;
        typedef matrix_reference<self_type> closure_type;
        typedef matrix_row<self_type> matrix_row_type;
        typedef matrix_column<self_type> matrix_column_type;
        typedef matrix_range<self_type> matrix_range_type;
        typedef typename A::const_iterator const_iterator_type;
        typedef typename A::iterator iterator_type;
        typedef sparse_tag storage_category;
        typedef typename F::orientation_category orientation_category;

        // Construction and destruction
        NUMERICS_INLINE
        sparse_matrix (): 
            size1_ (0), size2_ (0), non_zeroes_ (0), data_ () {}
        NUMERICS_INLINE
        sparse_matrix (size_type size1, size_type size2, size_type non_zeroes): 
            size1_ (size1), size2_ (size2), non_zeroes_ (non_zeroes), data_ () {}
        NUMERICS_INLINE
        sparse_matrix (const sparse_matrix &m): 
            size1_ (m.size1_), size2_ (m.size2_), non_zeroes_ (m.non_zeroes_), data_ (m.data_) {}
        template<class AE>
        NUMERICS_INLINE
        sparse_matrix (size_type non_zeroes, const matrix_expression<AE> &ae): 
            size1_ (ae ().size1 ()), size2_ (ae ().size2 ()), non_zeroes_ (non_zeroes), data_ () { 
            matrix_assign<scalar_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
        }

        // Resizing
        NUMERICS_INLINE
        void resize (size_type size1, size_type size2, size_type non_zeroes) {
            size1_ = size1;
            size2_ = size2;
            non_zeroes_ = non_zeroes;
            data_.resize (size1 * size2);
        }

        NUMERICS_INLINE
        size_type size1 () const { 
            return size1_;
        }
        NUMERICS_INLINE
        size_type size2 () const { 
            return size2_;
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i, size_type j) const {
            const_iterator_type it (data_.find (functor_type::element (i, size1_, j, size2_)));
            if (it == data_.end () || (*it).first != functor_type::element (i, size1_, j, size2_))
                return value_type (0);
            return (*it).second;
        }
        NUMERICS_INLINE
        reference_type operator () (size_type i, size_type j) {
            return data_ [functor_type::element (i, size1_, j, size2_)]; 
        }

        NUMERICS_INLINE
        matrix_row_type operator [] (size_type i) {
            return matrix_row_type (*this, i);
        }
        NUMERICS_INLINE
        matrix_row_type row (size_type i) {
            return matrix_row_type (*this, i);
        }
        NUMERICS_INLINE
        matrix_column_type column (size_type j) {
            return matrix_column_type (*this, j);
        }

        NUMERICS_INLINE
        matrix_range_type project (size_type start1, size_type stop1, size_type start2, size_type stop2) {
            return matrix_range_type (*this, start1, stop1, start2, stop2);
        }
        NUMERICS_INLINE
        matrix_range_type project (const range &r1, const range &r2) {
            return matrix_range_type (*this, r1, r2);
        }

        // Assignment
        NUMERICS_INLINE
        sparse_matrix &operator = (const sparse_matrix &m) { 
            check<bad_size>::precondition (size1_ == m.size1_);
            check<bad_size>::precondition (size2_ == m.size2_);
            check<bad_size>::precondition (non_zeroes_ == m.non_zeroes_);
            data_ = m.data_;
            return *this;
        }
        NUMERICS_INLINE
        sparse_matrix &assign_temporary (sparse_matrix &m) { 
            swap (m);
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        sparse_matrix &operator = (const matrix_expression<AE> &ae) { 
#ifndef USE_GCC
            return assign_temporary (self_type (non_zeroes_, ae));
#else
            return assign (self_type (non_zeroes_, ae));
#endif
        }
        template<class AE>
        NUMERICS_INLINE
        sparse_matrix &assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        sparse_matrix& operator += (const matrix_expression<AE> &ae) {
#ifndef USE_GCC
            return assign_temporary (self_type (non_zeroes_, *this + ae));
#else
            return assign (self_type (non_zeroes_, *this + ae));
#endif
        }
        template<class AE>
        NUMERICS_INLINE
        sparse_matrix &plus_assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_plus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        sparse_matrix& operator -= (const matrix_expression<AE> &ae) {
#ifndef USE_GCC
            return assign_temporary (self_type (non_zeroes_, *this - ae));
#else
            return assign (self_type (non_zeroes_, *this - ae));
#endif
        }
        template<class AE>
        NUMERICS_INLINE
        sparse_matrix &minus_assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_minus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        sparse_matrix& operator *= (const AT &at) {
            matrix_assign_scalar<scalar_multiplies_assign<value_type, AT> > () (*this, at);
            return *this;
        }

        // Swapping
        NUMERICS_INLINE
	    void swap (sparse_matrix &m) {
            check<external_logic>::precondition (this != &m);
            check<bad_size>::precondition (size1_ == m.size1_);
            check<bad_size>::precondition (size2_ == m.size2_);
            check<bad_size>::precondition (non_zeroes_ == m.non_zeroes_);
            std::swap (size1_, m.size1_);
            std::swap (size2_, m.size2_);
            std::swap (non_zeroes_, m.non_zeroes_);
            data_.swap (m.data_);
        }
#ifndef USE_GCC
        NUMERICS_INLINE
	    friend void swap (sparse_matrix &m1, sparse_matrix &m2) {
            m1.swap (m2);
        }
#endif

        // Element insertion
        NUMERICS_INLINE
        void clear () {
            data_.clear ();
        }
        NUMERICS_INLINE
        void insert (size_type i, size_type j, const T &t) {
            data_.insert (data_.end (), std::pair<size_type, T> (functor_type::element (i, size1_, j, size2_), t));
        }

        class const_iterator1;
        class iterator1;
        class const_iterator2;
        class iterator2;

        // Element lookup
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        const_iterator1 find1 (size_type i, size_type j) const {
            difference_type o (functor_type::one1 (size1_, size2_));
            if (o >= 1) {
                const_iterator_type it (data_.lower_bound (functor_type::element (i, size1_, j, size2_)));
                const_iterator_type it_end (data_.end ());
                while (it != it_end && functor_type::index2 ((*it).first, size1_, size2_) < j) {
                    i = functor_type::index1 ((*it).first, size1_, size2_);
                    it = data_.lower_bound (functor_type::element (i, size1_, j, size2_));
                }
                return const_iterator1 (*this, i, j, it);
            } else if (o <= -1) {
                const_iterator_type it (data_.upper_bound (functor_type::element (i, size1_, j, size2_)));
                const_iterator_type it_end (data_.end ());
                while (it != it_end && functor_type::index2 ((*it).first, size1_, size2_) < j) {
                    i = functor_type::index1 ((*it).first, size1_, size2_);
                    it = data_.upper_bound (functor_type::element (i, size1_, j, size2_));
                }
                return const_iterator1 (*this, i, j, it);
            } else
                throw internal_logic ();
        }
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        iterator1 find1 (size_type i, size_type j) {
            difference_type o (functor_type::one1 (size1_, size2_));
            if (o >= 1) {
                iterator_type it (data_.lower_bound (functor_type::element (i, size1_, j, size2_)));
                iterator_type it_end (data_.end ());
                while (it != it_end && functor_type::index2 ((*it).first, size1_, size2_) < j) {
                    i = functor_type::index1 ((*it).first, size1_, size2_);
                    it = data_.lower_bound (functor_type::element (i, size1_, j, size2_));
                }
                return iterator1 (*this, i, j, it);
            } else if (o <= -1) {
                iterator_type it (data_.upper_bound (functor_type::element (i, size1_, j, size2_)));
                iterator_type it_end (data_.end ());
                while (it != it_end && functor_type::index2 ((*it).first, size1_, size2_) < j) {
                    i = functor_type::index1 ((*it).first, size1_, size2_);
                    it = data_.upper_bound (functor_type::element (i, size1_, j, size2_));
                }
                return iterator1 (*this, i, j, it);
            } else
                throw internal_logic ();
        }
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        const_iterator2 find2 (size_type i, size_type j) const {
            difference_type o (functor_type::one2 (size1_, size2_));
            if (o >= 1) {
                const_iterator_type it (data_.lower_bound (functor_type::element (i, size1_, j, size2_)));
                const_iterator_type it_end (data_.end ());
                while (it != it_end && functor_type::index1 ((*it).first, size1_, size2_) < i) {
                    j = functor_type::index2 ((*it).first, size1_, size2_);
                    it = data_.lower_bound (functor_type::element (i, size1_, j, size2_));
                }
                return const_iterator2 (*this, i, j, it);
            } else if (o <= -1) {
                const_iterator_type it (data_.upper_bound (functor_type::element (i, size1_, j, size2_)));
                const_iterator_type it_end (data_.end ());
                while (it != it_end && functor_type::index1 ((*it).first, size1_, size2_) < i) {
                    j = functor_type::index2 ((*it).first, size1_, size2_);
                    it = data_.upper_bound (functor_type::element (i, size1_, j, size2_));
                }
                return const_iterator2 (*this, i, j, it);
            } else
                throw internal_logic ();
        }
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        iterator2 find2 (size_type i, size_type j) {
            difference_type o (functor_type::one2 (size1_, size2_));
            if (o >= 1) {
                iterator_type it (data_.lower_bound (functor_type::element (i, size1_, j, size2_)));
                iterator_type it_end (data_.end ());
                while (it != it_end && functor_type::index1 ((*it).first, size1_, size2_) < i) {
                    j = functor_type::index2 ((*it).first, size1_, size2_);
                    it = data_.lower_bound (functor_type::element (i, size1_, j, size2_));
                }
                return iterator2 (*this, i, j, it);
            } else if (o <= -1) {
                iterator_type it (data_.upper_bound (functor_type::element (i, size1_, j, size2_)));
                iterator_type it_end (data_.end ());
                while (it != it_end && functor_type::index1 ((*it).first, size1_, size2_) < i) {
                    j = functor_type::index2 ((*it).first, size1_, size2_);
                    it = data_.upper_bound (functor_type::element (i, size1_, j, size2_));
                }
                return iterator2 (*this, i, j, it);
            } else
                throw internal_logic ();
        }

        // Iterators simply are pointers.
        // FIXME: are we able to eliminate the switch statement in operator++/--?

        class const_iterator1:
            public container_const_reference<sparse_matrix>,
            public bidirectional_iterator_base<const_iterator1, value_type> {
        public:
            typedef std::bidirectional_iterator_tag iterator_category;
            typedef typename functor_type::functor1_type functor1_type;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator1 (const sparse_matrix &m, size_type i, size_type j, const const_iterator_type &it):
                container_const_reference<sparse_matrix> (m), i_ (i), j_ (j), it_ (it) {}
            NUMERICS_INLINE
            const_iterator1 (const iterator1 &it):
                container_const_reference<sparse_matrix> (it ()), i_ (it.i_), j_ (it.j_), it_ (it.it_) {}

            // Arithmetic
            NUMERICS_INLINE
            const_iterator1 &operator ++ () {
                const sparse_matrix &m = (*this) ();
                difference_type o (functor_type::one1 (m.size1 (), m.size2 ()));
                switch (o) {
                case 1:
                case -1:
                    functor1_type::increment (it_);
                    break;
                default:
                    it_ = m.find1 (index1 () + 1, j_).it_;
                }
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator -- () {
                const sparse_matrix &m = (*this) ();
                difference_type o (functor_type::one1 (m.size1 (), m.size2 ()));
                switch (o) {
                case 1:
                case -1:
                    functor1_type::decrement (it_);
                    break;
                default:
                    it_ = m.find1 (index1 () - 1, j_).it_;
                }
                return *this;
            }

            // Dereference
            NUMERICS_INLINE
            value_type operator * () const {
                check<bad_index>::precondition (index1 () < (*this) ().size1 ());
                check<bad_index>::precondition (index2 () < (*this) ().size2 ());
                return (*it_).second;
            }

            NUMERICS_INLINE
            const_iterator2 begin () const {
                const sparse_matrix &m = (*this) ();
                return m.find2 (index1 (), 0); 
            }
            NUMERICS_INLINE
            const_iterator2 end () const {
                const sparse_matrix &m = (*this) ();
                return m.find2 (index1 (), m.size2 ()); 
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                const sparse_matrix &m = (*this) ();
                return functor_type::index1 ((*it_).first, m.size1 (), m.size2 ());
            }
            NUMERICS_INLINE
            size_type  index2 () const {
                const sparse_matrix &m = (*this) ();
                return functor_type::index2 ((*it_).first, m.size1 (), m.size2 ());
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator1 &it) const {
                check<external_logic>::precondition (&(*this) () == &it ());
                return it_ == it.it_;
            }

        private:
            size_type i_;
            size_type j_;
            const_iterator_type it_;
        };

        NUMERICS_INLINE
        const_iterator1 begin1 () const {
            return find1 (0, 0);
        }
        NUMERICS_INLINE
        const_iterator1 end1 () const {
            return find1 (size1_, 0);
        }

        class iterator1:
            public container_reference<sparse_matrix>,
            public bidirectional_iterator_base<iterator1, value_type> {
        public:
            typedef std::bidirectional_iterator_tag iterator_category;
            typedef typename functor_type::functor1_type functor1_type;

            // Construction and destruction
            NUMERICS_INLINE
            iterator1 (sparse_matrix &m, size_type i, size_type j, const iterator_type &it):
                container_reference<sparse_matrix> (m), i_ (i), j_ (j), it_ (it) {}

            // Arithmetic
            NUMERICS_INLINE
            iterator1 &operator ++ () {
                sparse_matrix &m = (*this) ();
                difference_type o (functor_type::one1 (m.size1 (), m.size2 ()));
                switch (o) {
                case 1:
                case -1:
                    functor1_type::increment (it_);
                    break;
                default:
                    it_ = m.find1 (index1 () + 1, j_).it_;
                }
                return *this;
            }
            NUMERICS_INLINE
            iterator1 &operator -- () {
                sparse_matrix &m = (*this) ();
                difference_type o (functor_type::one1 (m.size1 (), m.size2 ()));
                switch (o) {
                case 1:
                case -1:
                    functor1_type::decrement (it_);
                    break;
                default:
                    it_ = m.find1 (index1 () - 1, j_).it_;
                }
                return *this;
            }

            // Dereference
            NUMERICS_INLINE
            reference_type operator * () const {
                check<bad_index>::precondition (index1 () < (*this) ().size1 ());
                check<bad_index>::precondition (index2 () < (*this) ().size2 ());
                return (*it_).second;
            }

            NUMERICS_INLINE
            iterator2 begin () {
                sparse_matrix &m = (*this) ();
                return m.find2 (index1 (), 0); 
            }
            NUMERICS_INLINE
            iterator2 end () {
                sparse_matrix &m = (*this) ();
                return m.find2 (index1 (), m.size2 ()); 
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                const sparse_matrix &m = (*this) ();
                return functor_type::index1 ((*it_).first, m.size1 (), m.size2 ());
            }
            NUMERICS_INLINE
            size_type  index2 () const {
                const sparse_matrix &m = (*this) ();
                return functor_type::index2 ((*it_).first, m.size1 (), m.size2 ());
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const iterator1 &it) const {
                check<external_logic>::precondition (&(*this) () == &it ());
                return it_ == it.it_;
            }

        private:
            size_type i_;
            size_type j_;
            iterator_type it_;
            
            friend class const_iterator1;
        };

        NUMERICS_INLINE
        iterator1 begin1 () {
            return find1 (0, 0);
        }
        NUMERICS_INLINE
        iterator1 end1 () {
            return find1 (size1_, 0);
        }

        class const_iterator2:
            public container_const_reference<sparse_matrix>,
            public bidirectional_iterator_base<const_iterator2, value_type> {
        public:
            typedef std::bidirectional_iterator_tag iterator_category;
            typedef typename functor_type::functor2_type functor2_type;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator2 (const sparse_matrix &m, size_type i, size_type j, const const_iterator_type &it):
                container_const_reference<sparse_matrix> (m), i_ (i), j_ (j), it_ (it) {}
            NUMERICS_INLINE
            const_iterator2 (const iterator2 &it):
                container_const_reference<sparse_matrix> (it ()), i_ (it.i_), j_ (it.j_), it_ (it.it_) {}

            // Arithmetic
            NUMERICS_INLINE
            const_iterator2 &operator ++ () {
                const sparse_matrix &m = (*this) ();
                difference_type o (functor_type::one2 (m.size1 (), m.size2 ()));
                switch (o) {
                case 1:
                case -1:
                    functor2_type::increment (it_);
                    break;
                default:
                    it_ = m.find2 (i_, index2 () + 1).it_;
                }
                return *this;
            }
            NUMERICS_INLINE
            const_iterator2 &operator -- () {
                const sparse_matrix &m = (*this) ();
                difference_type o (functor_type::one2 (m.size1 (), m.size2 ()));
                switch (o) {
                case 1:
                case -1:
                    functor2_type::decrement (it_);
                    break;
                default:
                    it_ = m.find2 (i_, index2 () - 1).it_;
                }
                return *this;
            }

            // Dereference
            NUMERICS_INLINE
            value_type operator * () const {
                check<bad_index>::precondition (index1 () < (*this) ().size1 ());
                check<bad_index>::precondition (index2 () < (*this) ().size2 ());
                return (*it_).second;
            }

            NUMERICS_INLINE
            const_iterator1 begin () const {
                const sparse_matrix &m = (*this) ();
                return m.find1 (0, index2 ()); 
            }
            NUMERICS_INLINE
            const_iterator1 end () const {
                const sparse_matrix &m = (*this) ();
                return m.find1 (m.size1 (), index2 ()); 
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                const sparse_matrix &m = (*this) ();
                return functor_type::index1 ((*it_).first, m.size1 (), m.size2 ());
            }
            NUMERICS_INLINE
            size_type  index2 () const {
                const sparse_matrix &m = (*this) ();
                return functor_type::index2 ((*it_).first, m.size1 (), m.size2 ());
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator2 &it) const {
                check<external_logic>::precondition (&(*this) () == &it ());
                return it_ == it.it_;
            }

        private:
            size_type i_;
            size_type j_;
            const_iterator_type it_;
        };

        NUMERICS_INLINE
        const_iterator2 begin2 () const {
            return find2 (0, 0);
        }
        NUMERICS_INLINE
        const_iterator2 end2 () const {
            return find2 (0, size2_);
        }

        class iterator2:
            public container_reference<sparse_matrix>,
            public bidirectional_iterator_base<iterator2, value_type> {
        public:
            typedef std::bidirectional_iterator_tag iterator_category;
            typedef typename functor_type::functor2_type functor2_type;

            // Construction and destruction
            NUMERICS_INLINE
            iterator2 (sparse_matrix &m, size_type i, size_type j, const iterator_type &it):
                container_reference<sparse_matrix> (m), i_ (i), j_ (j), it_ (it) {}

            // Arithmetic
            NUMERICS_INLINE
            iterator2 &operator ++ () {
                sparse_matrix &m = (*this) ();
                difference_type o (functor_type::one2 (m.size1 (), m.size2 ()));
                switch (o) {
                case 1:
                case -1:
                    functor2_type::increment (it_);
                    break;
                default:
                    it_ = m.find2 (i_, index2 () + 1).it_;
                }
                return *this;
            }
            NUMERICS_INLINE
            iterator2 &operator -- () {
                sparse_matrix &m = (*this) ();
                difference_type o (functor_type::one2 (m.size1 (), m.size2 ()));
                switch (o) {
                case 1:
                case -1:
                    functor2_type::decrement (it_);
                    break;
                default:
                    it_ = m.find2 (i_, index2 () - 1).it_;
                }
                return *this;
            }

            // Dereference
            NUMERICS_INLINE
            reference_type operator * () const {
                check<bad_index>::precondition (index1 () < (*this) ().size1 ());
                check<bad_index>::precondition (index2 () < (*this) ().size2 ());
                return (*it_).second;
            }

            NUMERICS_INLINE
            iterator1 begin () {
                sparse_matrix &m = (*this) ();
                return m.find1 (0, index2 ()); 
            }
            NUMERICS_INLINE
            iterator1 end () {
                sparse_matrix &m = (*this) ();
                return m.find1 (m.size1 (), index2 ()); 
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                const sparse_matrix &m = (*this) ();
                return functor_type::index1 ((*it_).first, m.size1 (), m.size2 ());
            }
            NUMERICS_INLINE
            size_type  index2 () const {
                const sparse_matrix &m = (*this) ();
                return functor_type::index2 ((*it_).first, m.size1 (), m.size2 ());
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const iterator2 &it) const {
                check<external_logic>::precondition (&(*this) () == &it ());
                return it_ == it.it_;
            }

        private:
            size_type i_;
            size_type j_;
            iterator_type it_;

            friend class const_iterator2;
        };

        NUMERICS_INLINE
        iterator2 begin2 () {
            return find2 (0, 0);
        }
        NUMERICS_INLINE
        iterator2 end2 () {
            return find2 (0, size2_);
        }

    private:
        size_type size1_;
        size_type size2_;
        size_type non_zeroes_;
        array_type data_;
    };

}

#endif 


