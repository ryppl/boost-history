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

#ifndef NUMERICS_MATRIX_PR_H
#define NUMERICS_MATRIX_PR_H

#include "config.h"
#include "storage.h"
#include "vector_et.h"
#include "matrix_et.h"

// Iterators based on ideas of Jeremy Siek

namespace numerics {

    // Matrix based row vector class
    template<class M>
    class matrix_row:
		public vector_expression<matrix_row<M> > {
    public:      
        typedef M matrix_type;
        typedef typename M::size_type size_type;
        typedef typename M::difference_type difference_type;
        typedef typename M::value_type value_type;
        typedef typename M::const_reference_type const_reference_type;
        typedef typename M::reference_type reference_type;
        typedef vector_const_reference<matrix_row<M> > const_closure_type;
        typedef vector_reference<matrix_row<M> > closure_type;
        typedef vector_range<matrix_row<M> > vector_range_type;
        typedef typename M::const_iterator2 const_iterator_type;
        typedef typename M::iterator2 iterator_type;
        typedef typename proxy_traits<typename M::storage_category>::storage_category storage_category;

        // Construction and destruction
        NUMERICS_INLINE
        matrix_row (matrix_type &data, size_type i): 
            data_ (data), i_ (i) {}

        NUMERICS_INLINE
        size_type size () const { 
            return data_.size2 (); 
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type j) const {
            // One has do to this, to get the const member dispatched?!
            const matrix_type &data = data_;
            return data (i_, j); 
        }
        NUMERICS_INLINE
        reference_type operator () (size_type j) {
            return data_ (i_, j); 
        }

        NUMERICS_INLINE
        value_type operator [] (size_type j) const { 
            return (*this) (j); 
        }
        NUMERICS_INLINE
        reference_type operator [] (size_type j) { 
            return (*this) (j); 
        }

        NUMERICS_INLINE
        vector_range_type project (size_type start, size_type stop) {
            return vector_range_type (*this, start, stop);
        }
        NUMERICS_INLINE
        vector_range_type project (const range &r) {
            return vector_range_type (*this, r);
        }

        // Assignment
        NUMERICS_INLINE
        matrix_row &operator = (const matrix_row &mr) { 
#ifndef USE_GCC
            std::copy (mr.begin (), mr.end (), begin ());
#else
            detail::copy (mr.begin (), mr.end (), begin ());
#endif
            return *this;
        }
        NUMERICS_INLINE
        matrix_row &assign_temporary (matrix_row &mr) { 
            return *this = mr;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_row &operator = (const vector_expression<AE> &ae) {        
            vector_assign<scalar_assign<value_type, value_type> > () (*this, vector<value_type> (ae));
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_row &assign (const vector_expression<AE> &ae) {
            vector_assign<scalar_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_row &operator += (const vector_expression<AE> &ae) {
            vector_assign<scalar_assign<value_type, value_type> > () (*this, vector<value_type> (*this + ae));
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_row &plus_assign (const vector_expression<AE> &ae) {
            vector_assign<scalar_plus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_row &operator -= (const vector_expression<AE> &ae) {
            vector_assign<scalar_assign<value_type, value_type> > () (*this, vector<value_type> (*this - ae));
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_row &minus_assign (const vector_expression<AE> &ae) {
            vector_assign<scalar_minus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        matrix_row &operator *= (const AT &at) {
            vector_assign_scalar<scalar_multiplies_assign<value_type, AT> > () (*this, at);
            return *this;
        }

        // Swapping
        NUMERICS_INLINE
	    void swap (matrix_row &mr) {
            check<external_logic>::precondition (this != &mr);
            check<bad_size>::precondition (size () == mr.size ());
#ifndef USE_GCC
            std::swap_ranges (begin (), end (), mr.begin ());
#else
            detail::swap_ranges (begin (), end (), mr.begin ());
#endif
        }
#ifndef USE_GCC
        NUMERICS_INLINE
	    friend void swap (matrix_row &mr1, matrix_row &mr2) {
            mr1.swap (mr2);
        }
#endif

        class const_iterator;
        class iterator;

        // Element lookup
        NUMERICS_INLINE
        const_iterator find (size_type j) const {
            return const_iterator (*this, data_.find2 (i_, j));
        }
        NUMERICS_INLINE
        iterator find (size_type j) {
            return iterator (*this, data_.find2 (i_, j));
        }

        // Iterators simply are pointers.

        class const_iterator:
            public container_const_reference<matrix_row>,
            public random_access_iterator_base<const_iterator, value_type> {
        public:
            typedef typename M::const_iterator2::iterator_category iterator_category;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator (const matrix_row &mr, const const_iterator_type &it):
                container_const_reference<matrix_row> (mr), it_ (it) {}
            NUMERICS_INLINE
            const_iterator (const iterator &it):
                container_const_reference<matrix_row> (it ()), it_ (it.it_) {}

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
                check<bad_index>::precondition (index () < (*this) ().size ());
                return *it_;
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                return it_.index2 ();
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator &it) const {
                check<external_logic>::precondition (&(*this) () == &it ());
                return it_ == it.it_;
            }

        private:
            const_iterator_type it_;
        };

        NUMERICS_INLINE
        const_iterator begin () const {
            return find (0);
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return find (size ());
        }

        class iterator:
            public container_reference<matrix_row>,
            public random_access_iterator_base<iterator, value_type> {
        public:
            typedef typename M::iterator2::iterator_category iterator_category;

            // Construction and destruction
            NUMERICS_INLINE
            iterator (matrix_row &mr, const iterator_type &it):
                container_reference<matrix_row> (mr), it_ (it) {}

            // Arithmetic
            NUMERICS_INLINE
            iterator &operator ++ () {
                ++ it_;
                return *this;
            }
            NUMERICS_INLINE
            iterator &operator -- () {
                -- it_;
                return *this;
            }
            NUMERICS_INLINE
            iterator &operator += (difference_type n) {
                it_ += n;
                return *this;
            }
            NUMERICS_INLINE
            iterator &operator -= (difference_type n) {
                it_ -= n;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const iterator &it) const {
                return it_ - it.it_;
            }

            // Dereference
            NUMERICS_INLINE
            reference_type operator * () {
                check<bad_index>::precondition (index () < (*this) ().size ());
                return *it_;
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                return it_.index2 ();
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const iterator &it) const {
                check<external_logic>::precondition (&(*this) () == &it ());
                return it_ == it.it_;
            }

        private:
            iterator_type it_;

            friend class const_iterator;
        };

        NUMERICS_INLINE
        iterator begin () {
            return find (0);
        }
        NUMERICS_INLINE
        iterator end () {
            return find (size ());
        }

    private:
        matrix_type &data_;
        size_type i_;
    };

    // Matrix based column vector class
    template<class M>
    class matrix_column:
		public vector_expression<matrix_column<M> > {
    public:      
        typedef M matrix_type;
        typedef typename M::size_type size_type;
        typedef typename M::difference_type difference_type;
        typedef typename M::value_type value_type;
        typedef typename M::const_reference_type const_reference_type;
        typedef typename M::reference_type reference_type;
        typedef vector_const_reference<matrix_column<M> > const_closure_type;
        typedef vector_reference<matrix_column<M> > closure_type;
        typedef vector_range<matrix_column<M> > vector_range_type;
        typedef typename M::const_iterator1 const_iterator_type;
        typedef typename M::iterator1 iterator_type;
        typedef typename proxy_traits<typename M::storage_category>::storage_category storage_category;

        // Construction and destruction
        NUMERICS_INLINE
        matrix_column (matrix_type &data, size_type j): 
            data_ (data), j_ (j) {}

        NUMERICS_INLINE
        size_type size () const { 
            return data_.size1 (); 
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i) const {
            // One has do to this, to get the const member dispatched?!
            const matrix_type &data = data_;
            return data (i, j_); 
        }
        NUMERICS_INLINE
        reference_type operator () (size_type i) {
            return data_ (i, j_); 
        }

        NUMERICS_INLINE
        value_type operator [] (size_type i) const { 
            return (*this) (i); 
        }
        NUMERICS_INLINE
        reference_type operator [] (size_type i) { 
            return (*this) (i); 
        }

        NUMERICS_INLINE
        vector_range_type project (size_type start, size_type stop) {
            return vector_range_type (*this, start, stop);
        }
        NUMERICS_INLINE
        vector_range_type project (const range &r) {
            return vector_range_type (*this, r);
        }

        // Assignment
        NUMERICS_INLINE
        matrix_column &operator = (const matrix_column &mc) { 
#ifndef USE_GCC
            std::copy (mc.begin (), mc.end (), begin ());
#else
            detail::copy (mc.begin (), mc.end (), begin ());
#endif
            return *this;
        }
        NUMERICS_INLINE
        matrix_column &assign_temporary (matrix_column &mc) { 
            return *this = mc;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_column &operator = (const vector_expression<AE> &ae) {        
            vector_assign<scalar_assign<value_type, value_type> > () (*this, vector<value_type> (ae));
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_column &assign (const vector_expression<AE> &ae) {
            vector_assign<scalar_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_column &operator += (const vector_expression<AE> &ae) {
            vector_assign<scalar_assign<value_type, value_type> > () (*this, vector<value_type> (*this + ae));
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_column &plus_assign (const vector_expression<AE> &ae) {
            vector_assign<scalar_plus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_column &operator -= (const vector_expression<AE> &ae) {
            vector_assign<scalar_assign<value_type, value_type> > () (*this, vector<value_type> (*this - ae));
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_column &minus_assign (const vector_expression<AE> &ae) {
            vector_assign<scalar_minus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        matrix_column &operator *= (const AT &at) {
            vector_assign_scalar<scalar_multiplies_assign<value_type, AT> > () (*this, at);
            return *this;
        }

        // Swapping
        NUMERICS_INLINE
	    void swap (matrix_column &mc) {
            check<external_logic>::precondition (this != &mc);
            check<bad_size>::precondition (size () == mc.size ());
#ifndef USE_GCC
            std::swap_ranges (begin (), end (), mc.begin ());
#else
            detail::swap_ranges (begin (), end (), mc.begin ());
#endif
        }
#ifndef USE_GCC
        NUMERICS_INLINE
	    friend void swap (matrix_column &mc1, matrix_column &mc2) {
            mc1.swap (mc2);
        }
#endif

        class const_iterator;
        class iterator;

        // Element lookup
        NUMERICS_INLINE
        const_iterator find (size_type i) const {
            return const_iterator (*this, data_.find1 (i, j_));
        }
        NUMERICS_INLINE
        iterator find (size_type i) {
            return iterator (*this, data_.find1 (i, j_));
        }

        // Iterators simply are pointers.

        class const_iterator:
            public container_const_reference<matrix_column>,
            public random_access_iterator_base<const_iterator, value_type> {
        public:
            typedef typename M::const_iterator1::iterator_category iterator_category;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator (const matrix_column &mc, const const_iterator_type &it):
                container_const_reference<matrix_column> (mc), it_ (it) {}
            NUMERICS_INLINE
            const_iterator (const iterator &it):
                container_const_reference<matrix_column> (it ()), it_ (it.it_) {}

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
                check<bad_index>::precondition (index () < (*this) ().size ());
                return *it_;
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                return it_.index1 ();
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator &it) const {
                check<external_logic>::precondition (&(*this) () == &it ());
                return it_ == it.it_;
            }

        private:
            const_iterator_type it_;
        };

        NUMERICS_INLINE
        const_iterator begin () const {
            return find (0);
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return find (size ());
        }

        class iterator:
            public container_reference<matrix_column>,
            public random_access_iterator_base<iterator, value_type> {
        public:
            typedef typename M::iterator1::iterator_category iterator_category;

            // Construction and destruction
            NUMERICS_INLINE
            iterator (matrix_column &mc, const iterator_type &it):
                container_reference<matrix_column> (mc), it_ (it) {}

            // Arithmetic
            NUMERICS_INLINE
            iterator &operator ++ () {
                ++ it_;
                return *this;
            }
            NUMERICS_INLINE
            iterator &operator -- () {
                -- it_;
                return *this;
            }
            NUMERICS_INLINE
            iterator &operator += (difference_type n) {
                it_ += n;
                return *this;
            }
            NUMERICS_INLINE
            iterator &operator -= (difference_type n) {
                it_ -= n;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const iterator &it) const {
                return it_ - it.it_;
            }

            // Dereference
            NUMERICS_INLINE
            reference_type operator * () {
                check<bad_index>::precondition (index () < (*this) ().size ());
                return *it_;
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                return it_.index1 ();
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const iterator &it) const {
                check<external_logic>::precondition (&(*this) () == &it ());
                return it_ == it.it_;
            }

        private:
            iterator_type it_;

            friend class const_iterator;
        };

        NUMERICS_INLINE
        iterator begin () {
            return find (0);
        }
        NUMERICS_INLINE
        iterator end () {
            return find (size ());
        }

    private:
        matrix_type &data_;
        size_type j_;
    };

    // Matrix based vector range class
    template<class M>
    class matrix_vector_range:
		public vector_expression<matrix_vector_range<M> > {
    public:      
        typedef M matrix_type;
        typedef typename M::size_type size_type;
        typedef typename M::difference_type difference_type;
        typedef typename M::value_type value_type;
        typedef typename M::const_reference_type const_reference_type;
        typedef typename M::reference_type reference_type;
        typedef vector_const_reference<matrix_vector_range<M> > const_closure_type;
        typedef vector_reference<matrix_vector_range<M> > closure_type;
        typedef range::const_iterator const_iterator1_type;
        typedef range::const_iterator iterator1_type;
        typedef range::const_iterator const_iterator2_type;
        typedef range::const_iterator iterator2_type;
        typedef typename proxy_traits<typename M::storage_category>::storage_category storage_category;

        // Construction and destruction
        NUMERICS_INLINE
        matrix_vector_range (matrix_type &data, const range &r1, const range &r2): 
            data_ (data), r1_ (r1), r2_ (r2) {}
        NUMERICS_INLINE
        matrix_vector_range (matrix_type &data, size_type start1, size_type stop1, size_type start2, size_type stop2): 
            data_ (data), r1_ (start1, stop1), r2_ (start2, stop2) {}

        NUMERICS_INLINE
        size_type size () const { 
            return std::max (r1_.size (), r2_.size ()); 
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i) const {
            // One has do to this, to get the const member dispatched?!
            const matrix_type &data = data_;
            return data (r1_ (i), r2_ (i)); 
        }
        NUMERICS_INLINE
        reference_type operator () (size_type i) {
            return data_ (r1_ (i), r2_ (i)); 
        }

        NUMERICS_INLINE
        value_type operator [] (size_type i) const { 
            return (*this) (i); 
        }
        NUMERICS_INLINE
        reference_type operator [] (size_type i) { 
            return (*this) (i); 
        }

        NUMERICS_INLINE
        matrix_vector_range project (size_type start, size_type stop) {
            return matrix_vector_range (data_, r1_.composite (range (start, stop)), r2_.composite (range (start, stop)));
        }
        NUMERICS_INLINE
        matrix_vector_range project (const range &r) {
            return matrix_vector_range_type (data_, r1_.composite (r), r2_.composite (r));
        }

        // Assignment
        NUMERICS_INLINE
        matrix_vector_range &operator = (const matrix_vector_range &mvr) { 
#ifndef USE_GCC
            std::copy (mvr.begin (), mvr.end (), begin ());
#else
            detail::copy (mvr.begin (), mvr.end (), begin ());
#endif
            return *this;
        }
        NUMERICS_INLINE
        matrix_vector_range &assign_temporary (matrix_vector_range &mvr) { 
            return *this = mvr;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_vector_range &operator = (const vector_expression<AE> &ae) {        
            vector_assign<scalar_assign<value_type, value_type> > () (*this, vector<value_type> (ae));
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_vector_range &assign (const vector_expression<AE> &ae) {
            vector_assign<scalar_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_vector_range &operator += (const vector_expression<AE> &ae) {
            vector_assign<scalar_assign<value_type, value_type> > () (*this, vector<value_type> (*this + ae));
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_vector_range &plus_assign (const vector_expression<AE> &ae) {
            vector_assign<scalar_plus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_vector_range &operator -= (const vector_expression<AE> &ae) {
            vector_assign<scalar_assign<value_type, value_type> > () (*this, vector<value_type> (*this - ae));
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_vector_range &minus_assign (const vector_expression<AE> &ae) {
            vector_assign<scalar_minus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        matrix_vector_range &operator *= (const AT &at) {
            vector_assign_scalar<scalar_multiplies_assign<value_type, AT> > () (*this, at);
            return *this;
        }

        // Swapping
        NUMERICS_INLINE
	    void swap (matrix_vector_range &mvr) {
            check<external_logic>::precondition (this != &mvr);
            check<bad_size>::precondition (size () == mvr.size ());
#ifndef USE_GCC
            std::swap_ranges (begin (), end (), mvr.begin ());
#else
            detail::swap_ranges (begin (), end (), mvr.begin ());
#endif
        }
#ifndef USE_GCC
        NUMERICS_INLINE
	    friend void swap (matrix_vector_range &mvr1, matrix_vector_range &mvr2) {
            mvr1.swap (mvr2);
        }
#endif

        // Iterators simply are indexes.

        class iterator;

        class const_iterator:
            public container_const_reference<matrix_type>,
            public random_access_iterator_base<const_iterator, value_type> {
        public:
            typedef typename restrict_traits<typename M::const_iterator1::iterator_category,
                                             typename M::const_iterator2::iterator_category>::iterator_category iterator_category; 

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator (const matrix_type &m, const const_iterator1_type &it1, const const_iterator2_type &it2):
                container_const_reference<matrix_type> (m), it1_ (it1), it2_ (it2) {}
            NUMERICS_INLINE
            const_iterator (const iterator &it):
                container_const_reference<matrix_type> (it ()), it1_ (it.it1_), it2_ (it.it2_) {}

            // Arithmetic
            NUMERICS_INLINE
            const_iterator &operator ++ () {
                ++ it1_;
                ++ it2_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator &operator -- () {
                -- it1_;
                -- it2_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator &operator += (difference_type n) {
                it1_ += n;
                it2_ += n;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator &operator -= (difference_type n) {
                it1_ -= n;
                it2_ -= n;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const const_iterator &it) const {
                return common (it1_ - it.it1_, it2_ - it.it2_);
            }

            // Dereference
            NUMERICS_INLINE
            value_type operator * () const {
                return (*this) () (*it1_, *it2_); 
            }

            // Index
            NUMERICS_INLINE
            size_type  index () const {
                return common (it1_.index (), it2_.index ());
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator &it) const {
                check<external_logic>::precondition (&(*this) () == &it ());
                return it1_ == it.it1_ && it2_ == it.it2_;
            }

        private:
            const_iterator1_type it1_;
            const_iterator2_type it2_;
        };

        NUMERICS_INLINE
        const_iterator begin () const {
            return const_iterator (data_, r1_.begin (), r2_.begin ()); 
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return const_iterator (data_, r1_.end (), r2_.end ()); 
        }

        class iterator:
            public container_reference<matrix_type>,
            public random_access_iterator_base<iterator, value_type> {
        public:
            typedef typename restrict_traits<typename M::iterator1::iterator_category,
                                             typename M::iterator2::iterator_category>::iterator_category iterator_category; 

            // Construction and destruction
            NUMERICS_INLINE
            iterator (matrix_type &m, const iterator1_type &it1, const iterator2_type &it2):
                container_reference<matrix_type> (m), it1_ (it1), it2_ (it2) {}

            // Arithmetic
            NUMERICS_INLINE
            iterator &operator ++ () {
                ++ it1_;
                ++ it2_;
                return *this;
            }
            NUMERICS_INLINE
            iterator &operator -- () {
                -- it1_;
                -- it2_;
                return *this;
            }
            NUMERICS_INLINE
            iterator &operator += (difference_type n) {
                it1_ += n;
                it2_ += n;
                return *this;
            }
            NUMERICS_INLINE
            iterator &operator -= (difference_type n) {
                it1_ -= n;
                it2_ -= n;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const iterator &it) const {
                return common (it1_ - it.it1_, it2_ - it.it2_);
            }

            // Dereference
            NUMERICS_INLINE
            reference_type operator * () {
                return (*this) () (*it1_, *it2_); 
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                return common (it1_.index (), it2_.index ());
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const iterator &it) const {
                check<external_logic>::precondition (&(*this) () == &it ());
                return it1_ == it.it1_ && it2_ == it.it2_;
            }

        private:
            iterator1_type it1_;
            iterator2_type it2_;

            friend class const_iterator;
        };

        NUMERICS_INLINE
        iterator begin () {
            return iterator (data_, r1_.begin (), r2_.begin ()); 
        }
        NUMERICS_INLINE
        iterator end () {
            return iterator (data_, r1_.end (), r2_.end ()); 
        }

    private:
        matrix_type &data_;
        range r1_;
        range r2_;
    };

    // Matrix based vector slice class
    template<class M>
    class matrix_vector_slice:
		public vector_expression<matrix_vector_slice<M> > {
    public:      
        typedef M matrix_type;
        typedef typename M::size_type size_type;
        typedef typename M::difference_type difference_type;
        typedef typename M::value_type value_type;
        typedef typename M::const_reference_type const_reference_type;
        typedef typename M::reference_type reference_type;
        typedef vector_const_reference<matrix_vector_slice<M> > const_closure_type;
        typedef vector_reference<matrix_vector_slice<M> > closure_type;
        typedef slice::const_iterator const_iterator1_type;
        typedef slice::const_iterator iterator1_type;
        typedef slice::const_iterator const_iterator2_type;
        typedef slice::const_iterator iterator2_type;
        typedef typename proxy_traits<typename M::storage_category>::storage_category storage_category;

        // Construction and destruction
        NUMERICS_INLINE
        matrix_vector_slice (matrix_type &data, const slice &s1, const slice &s2): 
            data_ (data), s1_ (s1), s2_ (s2) {}
        NUMERICS_INLINE
        matrix_vector_slice (matrix_type &data, size_type start1, size_type stride1, size_type size1, size_type start2, size_type stride2, size_type size2): 
            data_ (data), s1_ (start1, stride1, size1), s2_ (start2, stride2, size2) {}

        NUMERICS_INLINE
        size_type size () const { 
            return common (s1_.size (), s2_.size ()); 
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i) const {
            // One has do to this, to get the const member dispatched?!
            const matrix_type &data = data_;
            return data (s1_ (i), s2_ (i)); 
        }
        NUMERICS_INLINE
        reference_type operator () (size_type i) {
            return data_ (s1_ (i), s2_ (i)); 
        }

        NUMERICS_INLINE
        value_type operator [] (size_type i) const { 
            return (*this) (i); 
        }
        NUMERICS_INLINE
        reference_type operator [] (size_type i) { 
            return (*this) (i); 
        }

        NUMERICS_INLINE
        matrix_vector_slice project (const range &r) {
            return matrix_vector_slice (data_, s1_.composite (r), s2_.composite (r));
        }
        NUMERICS_INLINE
        matrix_vector_slice project (const slice &s) {
            return matrix_vector_slice (data_, s1_.composite (s), s2_.composite (s));
        }

        // Assignment
        NUMERICS_INLINE
        matrix_vector_slice &operator = (const matrix_vector_slice &mvs) { 
#ifndef USE_GCC
            std::copy (mvs.begin (), mvs.end (), begin ());
#else
            detail::copy (mvs.begin (), mvs.end (), begin ());
#endif
            return *this;
        }
        NUMERICS_INLINE
        matrix_vector_slice &assign_temporary (matrix_vector_slice &mvs) { 
            return *this = mvs;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_vector_slice &operator = (const vector_expression<AE> &ae) {        
            vector_assign<scalar_assign<value_type, value_type> > () (*this, vector<value_type> (ae));
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_vector_slice &assign (const vector_expression<AE> &ae) {
            vector_assign<scalar_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_vector_slice &operator += (const vector_expression<AE> &ae) {
            vector_assign<scalar_assign<value_type, value_type> > () (*this, vector<value_type> (*this + ae));
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_vector_slice &plus_assign (const vector_expression<AE> &ae) {
            vector_assign<scalar_plus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_vector_slice &operator -= (const vector_expression<AE> &ae) {
            vector_assign<scalar_assign<value_type, value_type> > () (*this, vector<value_type> (*this - ae));
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_vector_slice &minus_assign (const vector_expression<AE> &ae) {
            vector_assign<scalar_minus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        matrix_vector_slice &operator *= (const AT &at) {
            vector_assign_scalar<scalar_multiplies_assign<value_type, AT> > () (*this, at);
            return *this;
        }

        // Swapping
        NUMERICS_INLINE
	    void swap (matrix_vector_slice &mvs) {
            check<external_logic>::precondition (this != &mvs);
            check<bad_size>::precondition (size () == mvs.size ());
#ifndef USE_GCC
            std::swap_ranges (begin (), end (), mvs.begin ());
#else
            detail::swap_ranges (begin (), end (), mvs.begin ());
#endif
        }
#ifndef USE_GCC
        NUMERICS_INLINE
	    friend void swap (matrix_vector_slice &mvs1, matrix_vector_slice &mvs2) {
            mvs1.swap (mvs2);
        }
#endif

        // Iterators simply are indexes.

        class iterator;

        class const_iterator:
            public container_const_reference<matrix_type>,
            public random_access_iterator_base<const_iterator, value_type> {
        public:
            typedef typename restrict_traits<typename M::const_iterator1::iterator_category,
                                             typename M::const_iterator2::iterator_category>::iterator_category iterator_category; 

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator (const matrix_type &m, const const_iterator1_type &it1, const const_iterator2_type &it2):
                container_const_reference<matrix_type> (m), it1_ (it1), it2_ (it2) {}
            NUMERICS_INLINE
            const_iterator (const iterator &it):
                container_const_reference<matrix_type> (it ()), it1_ (it.it1_), it2_ (it.it2_) {}

            // Arithmetic
            NUMERICS_INLINE
            const_iterator &operator ++ () {
                ++ it1_;
                ++ it2_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator &operator -- () {
                -- it1_;
                -- it2_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator &operator += (difference_type n) {
                it1_ += n;
                it2_ += n;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator &operator -= (difference_type n) {
                it1_ -= n;
                it2_ -= n;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const const_iterator &it) const {
                return common (it1_ - it.it1_, it2_ - it.it2_);
            }

            // Dereference
            NUMERICS_INLINE
            value_type operator * () const {
                return (*this) () (*it1_, *it2_); 
            }

            // Index
            NUMERICS_INLINE
            size_type  index () const {
                return common (it1_.index (), it2_.index ());
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator &it) const {
                check<external_logic>::precondition (&(*this) () == &it ());
                return it1_ == it.it1_ && it2_ == it.it2_;
            }

        private:
            const_iterator1_type it1_;
            const_iterator2_type it2_;
        };

        NUMERICS_INLINE
        const_iterator begin () const {
            return const_iterator (data_, s1_.begin (), s2_.begin ()); 
        }
        NUMERICS_INLINE
        const_iterator end () const {
            return const_iterator (data_, s1_.end (), s2_.end ()); 
        }

        class iterator:
            public container_reference<matrix_type>,
            public random_access_iterator_base<iterator, value_type> {
        public:
            typedef typename restrict_traits<typename M::iterator1::iterator_category,
                                             typename M::iterator2::iterator_category>::iterator_category iterator_category; 

            // Construction and destruction
            NUMERICS_INLINE
            iterator (matrix_type &m, const iterator1_type &it1, const iterator2_type &it2):
                container_reference<matrix_type> (m), it1_ (it1), it2_ (it2) {}

            // Arithmetic
            NUMERICS_INLINE
            iterator &operator ++ () {
                ++ it1_;
                ++ it2_;
                return *this;
            }
            NUMERICS_INLINE
            iterator &operator -- () {
                -- it1_;
                -- it2_;
                return *this;
            }
            NUMERICS_INLINE
            iterator &operator += (difference_type n) {
                it1_ += n;
                it2_ += n;
                return *this;
            }
            NUMERICS_INLINE
            iterator &operator -= (difference_type n) {
                it1_ -= n;
                it2_ -= n;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const iterator &it) const {
                return common (it1_ - it.it1_, it2_ - it.it2_);
            }

            // Dereference
            NUMERICS_INLINE
            reference_type operator * () {
                return (*this) () (*it1_, *it2_); 
            }

            // Index
            NUMERICS_INLINE
            size_type index () const {
                return common (it1_.index (), it2_.index ());
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const iterator &it) const {
                check<external_logic>::precondition (&(*this) () == &it ());
                return it1_ == it.it1_ && it2_ == it.it2_;
            }

        private:
            iterator1_type it1_;
            iterator2_type it2_;

            friend class const_iterator;
        };

        NUMERICS_INLINE
        iterator begin () {
            return iterator (data_, s1_.begin (), s2_.begin ()); 
        }
        NUMERICS_INLINE
        iterator end () {
            return iterator (data_, s1_.end (), s2_.end ()); 
        }

    private:
        matrix_type &data_;
        slice s1_;
        slice s2_;
    };

    // Matrix based range class
    template<class M>
    class matrix_range:
		public matrix_expression<matrix_range<M> > {
    public:      
        typedef M matrix_type;
        typedef typename M::size_type size_type;
        typedef typename M::difference_type difference_type;
        typedef typename M::value_type value_type;
        typedef typename M::const_reference_type const_reference_type;
        typedef typename M::reference_type reference_type;
        typedef matrix_const_reference<matrix_range<M> > const_closure_type;
        typedef matrix_reference<matrix_range<M> > closure_type;
        typedef matrix_row<matrix_range<M> > matrix_row_type;
        typedef matrix_column<matrix_range<M> > matrix_column_type;
        typedef typename M::const_iterator1 const_iterator1_type;
        typedef typename M::iterator1 iterator1_type;
        typedef typename M::const_iterator2 const_iterator2_type;
        typedef typename M::iterator2 iterator2_type;
        typedef typename proxy_traits<typename M::storage_category>::storage_category storage_category;
        typedef typename M::orientation_category orientation_category;

        // Construction and destruction
        NUMERICS_INLINE
        matrix_range (matrix_type &data, const range &r1, const range &r2): 
            data_ (data), r1_ (r1), r2_ (r2) {}
        NUMERICS_INLINE
        matrix_range (matrix_type &data, size_type start1, size_type stop1, size_type start2, size_type stop2): 
            data_ (data), r1_ (start1, stop1), r2_ (start2, stop2) {}

        NUMERICS_INLINE
        size_type start1 () const { 
            return r1_.start (); 
        }
        NUMERICS_INLINE
        size_type size1 () const { 
            return r1_.size ();
        }
        NUMERICS_INLINE
        size_type start2() const { 
            return r2_.start (); 
        }
        NUMERICS_INLINE
        size_type size2 () const { 
            return r2_.size ();
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i, size_type j) const {
            // One has do to this, to get the const member dispatched?!
            const matrix_type &data = data_;
            return data (r1_ (i), r2_ (j)); 
        }
        NUMERICS_INLINE
        reference_type operator () (size_type i, size_type j) {
            return data_ (r1_ (i), r2_ (j)); 
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
        matrix_range project (size_type start1, size_type stop1, size_type start2, size_type stop2) {
            return matrix_range (data_, r1_.composite (range (start1, stop1)), r2_.composite (range (start2, stop2)));
        }
        NUMERICS_INLINE
        matrix_range project (const range &r1, const range &r2) {
            return matrix_range (data_, r1_.composite (r1), r2_.composite (r2));
        }

        // Assignment
        NUMERICS_INLINE
        matrix_range &operator = (const matrix_range &mr) { 
            matrix_assign<scalar_assign<value_type, value_type> > () (*this, mr); 
            return *this;
        }
        NUMERICS_INLINE
        matrix_range &assign_temporary (matrix_range &mr) { 
            return *this = mr;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_range &operator = (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_assign<value_type, value_type> > () (*this, matrix<value_type> (ae)); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_range &assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_range& operator += (const matrix_expression<AE> &ae) {
            matrix_assign<scalar_assign<value_type, value_type> > () (*this, matrix<value_type> (*this + ae)); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_range &plus_assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_plus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_range& operator -= (const matrix_expression<AE> &ae) {
            matrix_assign<scalar_assign<value_type, value_type> > () (*this, matrix<value_type> (*this - ae)); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_range &minus_assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_minus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        matrix_range& operator *= (const AT &at) {
            matrix_assign_scalar<scalar_multiplies_assign<value_type, AT> > () (*this, at);
            return *this;
        }

        // Swapping
        NUMERICS_INLINE
	    void swap (matrix_range &mr) {
            check<external_logic>::precondition (this != &mr);
            check<bad_size>::precondition (size1 () == mr.size1 ());
            check<bad_size>::precondition (size2 () == mr.size2 ());
            matrix_swap<scalar_swap<value_type, value_type> > () (*this, mr); 
        }
#ifndef USE_GCC
        NUMERICS_INLINE
	    friend void swap (matrix_range &mr1, matrix_range &mr2) {
            mr1.swap (mr2);
        }
#endif

        class const_iterator1;
        class iterator1;
        class const_iterator2;
        class iterator2;

        // Element lookup
        NUMERICS_INLINE
        const_iterator1 find1 (size_type i, size_type j) const {
            return const_iterator1 (*this, data_.find1 (start1 () + i, start2 () + j));
        }
        NUMERICS_INLINE
        iterator1 find1 (size_type i, size_type j) {
            return iterator1 (*this, data_.find1 (start1 () + i, start2 () + j));
        }
        NUMERICS_INLINE
        const_iterator2 find2 (size_type i, size_type j) const {
            return const_iterator2 (*this, data_.find2 (start1 () + i, start2 () + j));
        }
        NUMERICS_INLINE
        iterator2 find2 (size_type i, size_type j) {
            return iterator2 (*this, data_.find2 (start1 () + i, start2 () + j));
        }

        // Iterators simply are pointers.

        class const_iterator1:
            public container_const_reference<matrix_range>,
            public random_access_iterator_base<const_iterator1, value_type> {
        public:
            typedef typename M::const_iterator1::iterator_category iterator_category;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator1 (const matrix_range &mr, const const_iterator1_type &it):
                container_const_reference<matrix_range> (mr), it_ (it) {}
            NUMERICS_INLINE
            const_iterator1 (const iterator1 &it):
                container_const_reference<matrix_range> (it ()), it_ (it.it_) {}

            // Arithmetic
            NUMERICS_INLINE
            const_iterator1 &operator ++ () {
                ++ it_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator -- () {
                -- it_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator += (difference_type n) {
                it_ += n;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator -= (difference_type n) {
                it_ -= n;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const const_iterator1 &it) const {
                return it_ - it.it_;
            }

            // Dereference
            NUMERICS_INLINE
            value_type operator * () const {
                return *it_; 
            }

            NUMERICS_INLINE
            const_iterator2 begin () const {
                const matrix_range &mr = (*this) ();
                return mr.find2 (index1 (), 0);
            }
            NUMERICS_INLINE
            const_iterator2 end () const {
                const matrix_range &mr = (*this) ();
                return mr.find2 (index1 (), mr.size2 ());
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                return it_.index1 () - (*this) ().start1 ();
            }
            NUMERICS_INLINE
            size_type index2 () const {
                return it_.index2 () - (*this) ().start2 ();
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator1 &it) const {
                check<external_logic>::precondition (&(*this) () == &it ());
                return it_ == it.it_;
            }

        private:
            const_iterator1_type it_;
        };

        NUMERICS_INLINE
        const_iterator1 begin1 () const {
            return find1 (0, 0); 
        }
        NUMERICS_INLINE
        const_iterator1 end1 () const {
            return find1 (size1 (), 0); 
        }

        class iterator1:
            public container_reference<matrix_range>,
            public random_access_iterator_base<iterator1, value_type> {
        public:
            typedef typename M::iterator1::iterator_category iterator_category;

            // Construction and destruction
            NUMERICS_INLINE
            iterator1 (matrix_range &mr, const iterator1_type &it):
                container_reference<matrix_range> (mr), it_ (it) {}

            // Arithmetic
            NUMERICS_INLINE
            iterator1 &operator ++ () {
                ++ it_;
                return *this;
            }
            NUMERICS_INLINE
            iterator1 &operator -- () {
                -- it_;
                return *this;
            }
            NUMERICS_INLINE
            iterator1 &operator += (difference_type n) {
                it_ += n;
                return *this;
            }
            NUMERICS_INLINE
            iterator1 &operator -= (difference_type n) {
                it_ -= n;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const iterator1 &it) const {
                return it_ - it.it_;
            }

            // Dereference
            NUMERICS_INLINE
            reference_type operator * () {
                return *it_; 
            }

            NUMERICS_INLINE
            iterator2 begin () {
                matrix_range &mr = (*this) ();
                return mr.find2 (index1 (), 0);
            }
            NUMERICS_INLINE
            iterator2 end () {
                matrix_range &mr = (*this) ();
                return mr.find2 (index1 (), mr.size2 ());
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                return it_.index1 () - (*this) ().start1 ();
            }
            NUMERICS_INLINE
            size_type index2 () const {
                return it_.index2 () - (*this) ().start2 ();
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const iterator1 &it) const {
                check<external_logic>::precondition (&(*this) () == &it ());
                return it_ == it.it_;
            }

        private:
            iterator1_type it_;

            friend class const_iterator1;
        };

        NUMERICS_INLINE
        iterator1 begin1 () {
            return find1 (0, 0); 
        }
        NUMERICS_INLINE
        iterator1 end1 () {
            return find1 (size1 (), 0); 
        }

        class const_iterator2:
            public container_const_reference<matrix_range>,
            public random_access_iterator_base<const_iterator2, value_type> {
        public:
            typedef typename M::const_iterator2::iterator_category iterator_category;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator2 (const matrix_range &mr, const const_iterator2_type &it):
                container_const_reference<matrix_range> (mr), it_ (it) {}
            NUMERICS_INLINE
            const_iterator2 (const iterator2 &it):
                container_const_reference<matrix_range> (it ()), it_ (it.it_) {}

            // Arithmetic
            NUMERICS_INLINE
            const_iterator2 &operator ++ () {
                ++ it_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator2 &operator -- () {
                -- it_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator2 &operator += (difference_type n) {
                it_ += n;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator2 &operator -= (difference_type n) {
                it_ -= n;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const const_iterator2 &it) const {
                return it_ - it.it_;
            }

            // Dereference
            NUMERICS_INLINE
            value_type operator * () const {
                return *it_; 
            }

            NUMERICS_INLINE
            const_iterator1 begin () const {
                const matrix_range &mr = (*this) ();
                return mr.find1 (0, index2 ());
            }
            NUMERICS_INLINE
            const_iterator1 end () const {
                const matrix_range &mr = (*this) ();
                return mr.find1 (mr.size1 (), index2 ());
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                return it_.index1 () - (*this) ().start1 ();
            }
            NUMERICS_INLINE
            size_type index2 () const {
                return it_.index2 () - (*this) ().start2 ();
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator2 &it) const {
                check<external_logic>::precondition (&(*this) () == &it ());
                return it_ == it.it_;
            }

        private:
            const_iterator2_type it_;
        };

        NUMERICS_INLINE
        const_iterator2 begin2 () const {
            return find2 (0, 0); 
        }
        NUMERICS_INLINE
        const_iterator2 end2 () const {
            return find2 (0, size2 ()); 
        }

        class iterator2:
            public container_reference<matrix_range>,
            public random_access_iterator_base<iterator2, value_type> {
        public:
            typedef typename M::iterator2::iterator_category iterator_category;

            // Construction and destruction
            NUMERICS_INLINE
            iterator2 (matrix_range &mr, const iterator2_type &it):
                container_reference<matrix_range> (mr), it_ (it) {}

            // Arithmetic
            NUMERICS_INLINE
            iterator2 &operator ++ () {
                ++ it_;
                return *this;
            }
            NUMERICS_INLINE
            iterator2 &operator -- () {
                -- it_;
                return *this;
            }
            NUMERICS_INLINE
            iterator2 &operator += (difference_type n) {
                it_ += n;
                return *this;
            }
            NUMERICS_INLINE
            iterator2 &operator -= (difference_type n) {
                it_ -= n;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const iterator2 &it) const {
                return it_ - it.it_;
            }

            // Dereference
            NUMERICS_INLINE
            reference_type operator * () {
                return *it_;
            }

            NUMERICS_INLINE
            iterator1 begin () {
                matrix_range &mr = (*this) ();
                return mr.find1 (0, index2 ());
            }
            NUMERICS_INLINE
            iterator1 end () {
                matrix_range &mr = (*this) ();
                return mr.find1 (mr.size1 (), index2 ());
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                return it_.index1 () - (*this) ().start1 ();
            }
            NUMERICS_INLINE
            size_type index2 () const {
                return it_.index2 () - (*this) ().start2 ();
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const iterator2 &it) const {
                check<external_logic>::precondition (&(*this) () == &it ());
                return it_ == it.it_;
            }

        private:
            iterator2_type it_;

            friend class const_iterator2;
        };

        NUMERICS_INLINE
        iterator2 begin2 () {
            return find2 (0, 0); 
        }
        NUMERICS_INLINE
        iterator2 end2 () {
            return find2 (0, size2 ()); 
        }

    private:
        matrix_type &data_;
        range r1_;
        range r2_;
    };

    // Matrix based slice class
    template<class M>
    class matrix_slice:
		public matrix_expression<matrix_slice<M> > {
    public:      
        typedef M matrix_type;
        typedef typename M::size_type size_type;
        typedef typename M::difference_type difference_type;
        typedef typename M::value_type value_type;
        typedef typename M::const_reference_type const_reference_type;
        typedef typename M::reference_type reference_type;
        typedef matrix_const_reference<matrix_slice<M> > const_closure_type;
        typedef matrix_reference<matrix_slice<M> > closure_type;
        typedef matrix_row<matrix_slice<M> > matrix_row_type;
        typedef matrix_column<matrix_slice<M> > matrix_column_type;
        typedef slice::const_iterator const_iterator1_type;
        typedef slice::const_iterator iterator1_type;
        typedef slice::const_iterator const_iterator2_type;
        typedef slice::const_iterator iterator2_type;
        typedef typename proxy_traits<typename M::storage_category>::storage_category storage_category;
        typedef typename M::orientation_category orientation_category;

        // Construction and destruction
        NUMERICS_INLINE
        matrix_slice (matrix_type &data, const slice &s1, const slice &s2): 
            data_ (data), s1_ (s1), s2_ (s2) {}
        NUMERICS_INLINE
        matrix_slice (matrix_type &data, size_type start1, size_type stride1, size_type size1, size_type start2, size_type stride2, size_type size2): 
            data_ (data), s1_ (start1, stride1, size1), s2_ (start2, stride2, size2) {}

        NUMERICS_INLINE
        size_type size1 () const { 
            return s1_.size ();
        }
        NUMERICS_INLINE
        size_type size2 () const { 
            return s2_.size ();
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i, size_type j) const {
            // One has do to this, to get the const member dispatched?!
            const matrix_type &data = data_;
            return data (s1_ (i), s2_ (j)); 
        }
        NUMERICS_INLINE
        reference_type operator () (size_type i, size_type j) {
            return data_ (s1_ (i), s2_ (j)); 
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
        matrix_slice project (const range &r1, const range &r2) {
            return matrix_slice (data_, s1_.composite (r1), s2_.composite (r2));
        }
        NUMERICS_INLINE
        matrix_slice project (const slice &s1, const slice &s2) {
            return matrix_slice (data_, s1_.composite (s1), s2_.composite (s2));
        }

        // Assignment
        NUMERICS_INLINE
        matrix_slice &operator = (const matrix_slice &ms) { 
            matrix_assign<scalar_assign<value_type, value_type> > () (*this, ms); 
            return *this;
        }
        NUMERICS_INLINE
        matrix_slice &assign_temporary (matrix_slice &ms) { 
            return *this = ms;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_slice &operator = (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_assign<value_type, value_type> > () (*this, matrix<value_type> (ae)); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_slice &assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_slice& operator += (const matrix_expression<AE> &ae) {
            matrix_assign<scalar_assign<value_type, value_type> > () (*this, matrix<value_type> (*this + ae)); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_slice &plus_assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_plus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_slice& operator -= (const matrix_expression<AE> &ae) {
            matrix_assign<scalar_assign<value_type, value_type> > () (*this, matrix<value_type> (*this - ae)); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix_slice &minus_assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_minus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        matrix_slice& operator *= (const AT &at) {
            matrix_assign_scalar<scalar_multiplies_assign<value_type, AT> > () (*this, at);
            return *this;
        }

        // Swapping
        NUMERICS_INLINE
	    void swap (matrix_slice &ms) {
            check<external_logic>::precondition (this != &ms);
            check<bad_size>::precondition (size1 () == ms.size1 ());
            check<bad_size>::precondition (size2 () == ms.size2 ());
            matrix_swap<scalar_swap<value_type, value_type> > () (*this, ms); 
        }
#ifndef USE_GCC
        NUMERICS_INLINE
	    friend void swap (matrix_slice &ms1, matrix_slice &ms2) {
            ms1.swap (ms2);
        }
#endif

        // Iterators simply are indexes.

        class iterator1;

        class const_iterator2;

        class const_iterator1:
            public container_const_reference<matrix_type>,
            public random_access_iterator_base<const_iterator1, value_type> {
        public:
            typedef typename M::const_iterator1::iterator_category iterator_category;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator1 (const matrix_type &m, const const_iterator1_type &it1, const const_iterator2_type &it2):
                container_const_reference<matrix_type> (m), it1_ (it1), it2_ (it2) {}
            NUMERICS_INLINE
            const_iterator1 (const iterator1 &it):
                container_const_reference<matrix_type> (it ()), it1_ (it.it1_), it2_ (it.it2_) {}

            // Arithmetic
            NUMERICS_INLINE
            const_iterator1 &operator ++ () {
                ++ it1_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator -- () {
                -- it1_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator += (difference_type n) {
                it1_ += n;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator -= (difference_type n) {
                it1_ -= n;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const const_iterator1 &it) const {
                return it1_ - it.it1_;
            }

            // Dereference
            NUMERICS_INLINE
            value_type operator * () const {
                return (*this) () (*it1_, *it2_); 
            }

            NUMERICS_INLINE
            const_iterator2 begin () const {
                return const_iterator2 ((*this) (), it1_, it2_ ().begin ());
            }
            NUMERICS_INLINE
            const_iterator2 end () const {
                return const_iterator2 ((*this) (), it1_, it2_ ().end ()); 
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                return it1_.index ();
            }
            NUMERICS_INLINE
            size_type index2 () const {
                return it2_.index ();
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator1 &it) const {
                check<external_logic>::precondition (&(*this) () == &it ());
                return it1_ == it.it1_ && it2_ == it.it2_;
            }

        private:
            const_iterator1_type it1_;
            const_iterator2_type it2_;
        };

        NUMERICS_INLINE
        const_iterator1 begin1 () const {
            return const_iterator1 (data_, s1_.begin (), s2_.begin ()); 
        }
        NUMERICS_INLINE
        const_iterator1 end1 () const {
            return const_iterator1 (data_, s1_.end (), s2_.begin ()); 
        }

        class iterator2;

        class iterator1:
            public container_reference<matrix_type>,
            public random_access_iterator_base<iterator1, value_type> {
        public:
            typedef typename M::iterator1::iterator_category iterator_category;

            // Construction and destruction
            NUMERICS_INLINE
            iterator1 (matrix_type &m, const iterator1_type &it1, const iterator2_type &it2):
                container_reference<matrix_type> (m), it1_ (it1), it2_ (it2) {}

            // Arithmetic
            NUMERICS_INLINE
            iterator1 &operator ++ () {
                ++ it1_;
                return *this;
            }
            NUMERICS_INLINE
            iterator1 &operator -- () {
                -- it1_;
                return *this;
            }
            NUMERICS_INLINE
            iterator1 &operator += (difference_type n) {
                it1_ += n;
                return *this;
            }
            NUMERICS_INLINE
            iterator1 &operator -= (difference_type n) {
                it1_ -= n;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const iterator1 &it) const {
                return it1_ - it.it1_;
            }

            // Dereference
            NUMERICS_INLINE
            reference_type operator * () {
                return (*this) () (*it1_, *it2_); 
            }

            NUMERICS_INLINE
            iterator2 begin () {
                return iterator2 ((*this) (), it1_, it2_ ().begin ());
            }
            NUMERICS_INLINE
            iterator2 end () {
                return iterator2 ((*this) (), it1_, it2_ ().end ()); 
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                return it1_.index ();
            }
            NUMERICS_INLINE
            size_type index2 () const {
                return it2_.index ();
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const iterator1 &it) const {
                check<external_logic>::precondition (&(*this) () == &it ());
                return it1_ == it.it1_ && it2_ == it.it2_;
            }

        private:
            iterator1_type it1_;
            iterator2_type it2_;

            friend class const_iterator1;
        };

        NUMERICS_INLINE
        iterator1 begin1 () {
            return iterator1 (data_, s1_.begin (), s2_.begin ()); 
        }
        NUMERICS_INLINE
        iterator1 end1 () {
            return iterator1 (data_, s1_.end (), s2_.begin ()); 
        }

        class const_iterator2:
            public container_const_reference<matrix_type>,
            public random_access_iterator_base<const_iterator2, value_type> {
        public:
            typedef typename M::const_iterator2::iterator_category iterator_category;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator2 (const matrix_type &m, const const_iterator1_type &it1, const const_iterator2_type &it2):
                container_const_reference<matrix_type> (m), it1_ (it1), it2_ (it2) {}
            NUMERICS_INLINE
            const_iterator2 (const iterator2 &it):
                container_const_reference<matrix_type> (it ()), it1_ (it.it1_), it2_ (it.it2_) {}

            // Arithmetic
            NUMERICS_INLINE
            const_iterator2 &operator ++ () {
                ++ it2_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator2 &operator -- () {
                -- it2_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator2 &operator += (difference_type n) {
                it2_ += n;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator2 &operator -= (difference_type n) {
                it2_ -= n;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const const_iterator2 &it) const {
                return it2_ - it.it2_;
            }

            // Dereference
            NUMERICS_INLINE
            value_type operator * () const {
                return (*this) () (*it1_, *it2_); 
            }

            NUMERICS_INLINE
            const_iterator1 begin () const {
                return const_iterator1 ((*this) (), it1_ ().begin (), it2_); 
            }
            NUMERICS_INLINE
            const_iterator1 end () const {
                return const_iterator1 ((*this) (), it1_ ().end (), it2_); 
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                return it1_.index ();
            }
            NUMERICS_INLINE
            size_type index2 () const {
                return it2_.index ();
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator2 &it) const {
                check<external_logic>::precondition (&(*this) () == &it ());
                return it1_ == it.it1_ && it2_ == it.it2_;
            }

        private:
            const_iterator1_type it1_;
            const_iterator2_type it2_;
        };

        NUMERICS_INLINE
        const_iterator2 begin2 () const {
            return const_iterator2 (data_, s1_.begin (), s2_.begin ()); 
        }
        NUMERICS_INLINE
        const_iterator2 end2 () const {
            return const_iterator2 (data_, s1_.begin (), s2_.end ()); 
        }

        class iterator2:
            public container_reference<matrix_type>,
            public random_access_iterator_base<iterator2, value_type> {
        public:
            typedef typename M::iterator2::iterator_category iterator_category;

            // Construction and destruction
            NUMERICS_INLINE
            iterator2 (matrix_type &m, const iterator1_type &it1, const iterator2_type &it2):
                container_reference<matrix_type> (m), it1_ (it1), it2_ (it2) {}

            // Arithmetic
            NUMERICS_INLINE
            iterator2 &operator ++ () {
                ++ it2_;
                return *this;
            }
            NUMERICS_INLINE
            iterator2 &operator -- () {
                -- it2_;
                return *this;
            }
            NUMERICS_INLINE
            iterator2 &operator += (difference_type n) {
                it2_ += n;
                return *this;
            }
            NUMERICS_INLINE
            iterator2 &operator -= (difference_type n) {
                it2_ -= n;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const iterator2 &it) const {
                return it2_ - it.it2_;
            }

            // Dereference
            NUMERICS_INLINE
            reference_type operator * () {
                return (*this) () (*it1_, *it2_); 
            }

            NUMERICS_INLINE
            iterator1 begin () const {
                return iterator1 ((*this) (), it1_ ().begin (), it2_); 
            }
            NUMERICS_INLINE
            iterator1 end () const {
                return iterator1 ((*this) (), it1_ ().end (), it2_); 
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                return it1_.index ();
            }
            NUMERICS_INLINE
            size_type index2 () const {
                return it2_.index ();
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const iterator2 &it) const {
                check<external_logic>::precondition (&(*this) () == &it ());
                return it1_ == it.it1_ && it2_ == it.it2_;
            }

        private:
            iterator1_type it1_;
            iterator2_type it2_;

            friend class const_iterator2;
        };

        NUMERICS_INLINE
        iterator2 begin2 () {
            return iterator1 (data_, s1_.begin (), s2_.begin ()); 
        }
        NUMERICS_INLINE
        iterator2 end2 () {
            return iterator1 (data_, s1_.begin (), s2_.end ()); 
        }

    private:
        matrix_type &data_;
        slice s1_;
        slice s2_;
    };

}

#endif 


