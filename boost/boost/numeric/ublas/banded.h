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

#ifndef NUMERICS_BANDED_H
#define NUMERICS_BANDED_H

#include "config.h"
#include "storage.h"
#include "vector_et.h"
#include "matrix_et.h"

// Iterators based on ideas of Jeremy Siek

namespace numerics {

    // Array based band matrix class 
    template<class T, class A, class F>
    class banded_matrix: 
        public matrix_expression<banded_matrix<T, A, F> > {
    public:      
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef T &reference_type;
        typedef T *pointer_type;
        typedef const A const_array_type;
        typedef A array_type;
        typedef F functor_type;
// #ifndef USE_MSVC
        typedef matrix_vector_slice<banded_matrix<T, A, F> > matrix_vector_slice_type;
// #endif

        NUMERICS_EXPLICIT
        NUMERICS_INLINE
        banded_matrix (): 
            size1_ (0), size2_ (0),
            lower_ (0), upper_ (0), data_ (0) {}
        NUMERICS_EXPLICIT
        NUMERICS_INLINE
        banded_matrix (size_type size1, size_type size2, size_type lower, size_type upper): 
            size1_ (size1), size2_ (size2),
            lower_ (lower), upper_ (upper),
            data_ (std::min (size1, size2) * (lower + 1 + upper)) {}
        NUMERICS_EXPLICIT
        NUMERICS_INLINE
        banded_matrix (const banded_matrix &m): 
            size1_ (m.size1 ()), size2_ (m.size2 ()),       
            lower_ (m.lower ()), upper_ (m.upper ()),
            data_ (std::min (m.size1 (), m.size2 ()) * (m.lower () + 1 + m.upper ())) {
            safe_copy (data_, m.data_);
        }
        template<class AE>
        NUMERICS_EXPLICIT
        NUMERICS_INLINE
        banded_matrix (const matrix_expression<AE> &ae, size_type lower, size_type upper): 
            size1_ (ae.size1 ()), size2_ (ae.size2 ()),
            lower_ (lower), upper_ (upper),
            data_ (std::min (ae.size1 (), ae.size2 ()) * (lower + 1 + upper)) {
            assign (ae); 
        }
        NUMERICS_INLINE
        virtual ~banded_matrix () {}

        NUMERICS_INLINE
        void resize (size_type size1, size_type size2, size_type lower, size_type upper) {
            size1_ = size1;
            size2_ = size2;
            lower_ = lower;
            upper_ = upper;
            data_.resize (std::min (size1, size2) * (lower + 1 + upper));
        }

        NUMERICS_INLINE
        array_type &data () {
            return data_;
        }
        NUMERICS_INLINE
        size_type size1 () const { 
            return size1_;
        }
        NUMERICS_INLINE
        size_type size2 () const { 
            return size2_;
        }
        NUMERICS_INLINE
        size_type lower () const {
            return lower_;
        }
        NUMERICS_INLINE
        size_type upper () const {
            return upper_;
        }

        NUMERICS_INLINE
        value_type operator () (size_type i, size_type j) const {
            check<bad_index>::precondition (i < size1 ());
            check<bad_index>::precondition (j < size2 ());
            size_type k = std::min (i, j);
            size_type l = lower_ + j - i;
            if (k < std::min (size1 (), size2 ()) &&
                l < lower_ + 1 + upper_)
                return data_ [functor_type::element (k, std::min (size1 (), size2 ()),
                                                     l, lower_ + 1 + upper_)]; 
            return 0;
        }
        NUMERICS_INLINE
        reference_type operator () (size_type i, size_type j) {
            check<bad_index>::precondition (i < size1 ());
            check<bad_index>::precondition (j < size2 ());
            size_type k = std::min (i, j);
            size_type l = lower_ + j - i;
            return data_ [functor_type::element (k, std::min (size1 (), size2 ()), 
                                                 l, lower_ + 1 + upper_)]; 
        }

        NUMERICS_INLINE
// #ifdef USE_MSVC
//         matrix_vector_slice<banded_matrix> operator [] (size_type i) {
// #else
        matrix_vector_slice_type operator [] (size_type i) {
// #endif
            return matrix_vector_slice<banded_matrix> (*this, slice (i, 0, size2 ()), slice (0, 1, size2 ()));
        }
        NUMERICS_INLINE
// #ifdef USE_MSVC
//         matrix_vector_slice<banded_matrix> row (size_type i) {
// #else
        matrix_vector_slice_type row (size_type i) {
// #endif
            return matrix_vector_slice<banded_matrix> (*this, slice (i, 0, size2 ()), slice (0, 1, size2 ()));
        }
        NUMERICS_INLINE
// #ifdef USE_MSVC
//         matrix_vector_slice<banded_matrix> column (size_type j) {
// #else
        matrix_vector_slice_type column (size_type j) {
// #endif
            return matrix_vector_slice<banded_matrix> (*this, slice (0, 1, size1 ()), slice (j, 0, size2 ()));
        }

        NUMERICS_INLINE
        banded_matrix &operator = (const banded_matrix &m) { 
            safe_copy (data_, m.data_);
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        banded_matrix &operator ^= (const matrix_expression<AE> &ae) { 
            assign (ae); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        banded_matrix &operator = (const matrix_expression<AE> &ae) { 
            banded_matrix e (ae, lower_, upper_);
//            fast_copy (data_, e.data_);
            assign_temporary (data_, e.data_);
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        banded_matrix& operator += (const matrix_expression<AE> &ae) {
            banded_matrix e (*this + ae, lower_, upper_);
//            fast_copy (data_, e.data_);
            assign_temporary (data_, e.data_);
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        banded_matrix& operator -= (const matrix_expression<AE> &ae) {
            banded_matrix e (*this - ae, lower_, upper_);
//            fast_copy (data_, e.data_);
            assign_temporary (data_, e.data_);
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        banded_matrix& operator *= (const AT &at) {
            size_type size1 = this->size1 ();
            size_type size2 = this->size2 ();
            for (size_type i = 0; i < size1; ++ i) {
                for (size_type j = 0; j < size2; ++ j) {
                    size_type k = std::min (i, j);
                    size_type l = lower_ + j - i;
                    if (k < std::min (size1 (), size2 ()) &&
                        l < lower_ + 1 + upper_)
//                        (*this ) (i, j) *= at;
                          data_ [functor_type::element (k, std::min (size1 (), size2 ()),
                                                        l, lower_ + 1 + upper_)] *= at;
                }
            }
            return *this;
        }

    private:
        size_type size1_;
        size_type size2_;
        size_type lower_;
        size_type upper_;
        array_type data_;

        template<class AE>
        NUMERICS_INLINE
        void assign (const matrix_expression<AE> &ae) {
            size_type size1 = common (this->size1 (), ae.size1 ());
            size_type size2 = common (this->size2 (), ae.size2 ());
            for (size_type i = 0; i < size1; ++ i) {
                for (size_type j = 0; j < size2; ++ j) {
                    size_type k = std::min (i, j);
                    size_type l = lower_ + j - i;
                    if (k < std::min (size1 (), size2 ()) &&
                        l < lower_ + 1 + upper_)
//                        (*this) (i, j) = ae (i, j);
                        data_ [functor_type::element (k, std::min (size1 (), size2 ()),
                                                      l, lower_ + 1 + upper_)] = ae (i, j);
                    else 
                        check<non_band>::precondition (ae (i, j) != AT (0));                            
                }
            }
        }
    };

}

#endif 