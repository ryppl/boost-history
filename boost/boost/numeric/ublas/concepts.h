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

#ifndef NUMERICS_CONCEPTS_H
#define NUMERICS_CONCEPTS_H

namespace numerics {

    template<class T> 
    struct Assignable {
        typedef T value_type;

        static void constraints (value_type t) {
            // Copy Constructor
            value_type c1 (t);
            value_type c2 = t;
            // Assignment
            value_type a = t;
            std::swap (c1, c2);
        }
    };

    template<class T> 
    struct EqualityComparable {
        typedef T value_type;

        static void constraints (const value_type t) {
            bool b;
            // Equality
            b = t == t;
            // Inequality
            b = t != t;
        }
    };

    template<class T> 
    struct DefaultConstructible {
        typedef T value_type;

        static void constraints () {
            // Default Constructor
            static value_type c1 = value_type ();
            static value_type c2;
            c1, c2;
        }
    };

    template<class I, class T = NUMERICS_TYPENAME I::value_type>
    struct BidirectionalIterator {
        typedef I iterator_type;
        typedef T value_type;

        static void constraints () {
#ifdef LATER
            Assignable<iterator_type>::constraints (iterator_type ());
#endif
            EqualityComparable<iterator_type>::constraints (iterator_type ());
            DefaultConstructible<iterator_type>::constraints ();    
            iterator_type it = iterator_type ();
            value_type t = value_type ();
            // Dereference
            t = *it;
            // Member access
            // it->m;
            // Preincrement
            ++ it;
            // Postincrement
            it ++;
            // Predecrement
            -- it;
            // Postdecrement
            it --;
        }
    };

    template<class I, class T = NUMERICS_TYPENAME I::value_type>
    struct MutableBidirectionalIterator {
        typedef I iterator_type;
        typedef T value_type;

        static void constraints () {
            BidirectionalIterator<iterator_type, value_type>::constraints ();
            iterator_type it = iterator_type ();
            value_type t = value_type ();
            // Dereference assignment
            *it = t;
        }
    };

    template<class I, class D = NUMERICS_TYPENAME I::difference_type, class T = NUMERICS_TYPENAME I::value_type>
    struct RandomAccessIterator {
        typedef I iterator_type;
        typedef D difference_type;
        typedef T value_type;

        static void constraints () {
            BidirectionalIterator<iterator_type, value_type>::constraints ();
            iterator_type it = iterator_type (), it1 = iterator_type (), it2 = iterator_type ();
            difference_type n (0);
            value_type t = value_type ();
            // Forward motion
            it += n;
            // Iterator addition
#ifdef LATER
            it = it + n;
#else
            iterator_type itp (it + n);
#endif
            // Backward motion
            it -= n;
            // Iterator subtraction
#ifdef LATER
            it = it - n;
#else
            iterator_type itm (it - n);
#endif
            // Difference
            n = it1 - it2;
            // Element operator
#ifdef LATER
            t = it [n];
#else
            t = *(it + n);
#endif
        }
    };

    template<class I, class D = NUMERICS_TYPENAME I::difference_type, class T = NUMERICS_TYPENAME I::value_type>
    struct MutableRandomAccessIterator {
        typedef I iterator_type;
        typedef D difference_type;
        typedef T value_type;

        static void constraints () {
            MutableBidirectionalIterator<iterator_type, value_type>::constraints ();
            RandomAccessIterator<iterator_type, difference_type, value_type>::constraints ();
            iterator_type it = iterator_type ();
            difference_type n (0);
            value_type t = value_type ();
            // Element assignment
#ifdef LATER
            it [n] = t;
#else
            *(it + n) = t;
#endif
        }
    };

    template<class I>
    struct Indexed1DIterator {
        typedef I iterator_type;

        static void constraints () {
            iterator_type it = iterator_type ();
            // Function call
            it ();
            // Index
            it.index ();
        }
    };

    template<class I>
    struct IndexedBidirectional1DIterator {
        typedef I iterator_type;

        static void constraints () {
            BidirectionalIterator<iterator_type>::constraints ();
            Indexed1DIterator<iterator_type>::constraints ();
        }
    };

    template<class I>
    struct MutableIndexedBidirectional1DIterator {
        typedef I iterator_type;

        static void constraints () {
            MutableBidirectionalIterator<iterator_type>::constraints ();
            Indexed1DIterator<iterator_type>::constraints ();
        }
    };

    template<class I>
    struct IndexedRandomAccess1DIterator {
        typedef I iterator_type;

        static void constraints () {
            RandomAccessIterator<iterator_type>::constraints ();
            Indexed1DIterator<iterator_type>::constraints ();
        }
    };

    template<class I>
    struct MutableIndexedRandomAccess1DIterator {
        typedef I iterator_type;

        static void constraints () {
            MutableRandomAccessIterator<iterator_type>::constraints ();
            Indexed1DIterator<iterator_type>::constraints ();
        }
    };

    template<class I, class O>
    struct Indexed2DIterator {
        typedef I iterator_type;
        typedef O other_iterator_type;

        static void constraints () {
            iterator_type it = iterator_type ();
            // Function call
            it ();
            // Indices
            it.index1 ();
            it.index2 ();
            // Iterator Begin/End
            other_iterator_type it_begin (it.begin ());
            other_iterator_type it_end (it.end ());
        }
    };

    template<class I1, class I2>
    struct IndexedBidirectional2DIterator {
        typedef I1 iterator1_type;
        typedef I2 iterator2_type;

        static void constraints () {
            BidirectionalIterator<iterator1_type>::constraints ();
            BidirectionalIterator<iterator2_type>::constraints ();
            Indexed2DIterator<iterator1_type, iterator2_type>::constraints ();
            Indexed2DIterator<iterator2_type, iterator1_type>::constraints ();
        }
    };

    template<class I1, class I2>
    struct MutableIndexedBidirectional2DIterator {
        typedef I1 iterator1_type;
        typedef I2 iterator2_type;

        static void constraints () {
            MutableBidirectionalIterator<iterator1_type>::constraints ();
            MutableBidirectionalIterator<iterator2_type>::constraints ();
            Indexed2DIterator<iterator1_type, iterator2_type>::constraints ();
            Indexed2DIterator<iterator2_type, iterator1_type>::constraints ();
        }
    };

    template<class I1, class I2>
    struct IndexedRandomAccess2DIterator {
        typedef I1 iterator1_type;
        typedef I2 iterator2_type;

        static void constraints () {
            RandomAccessIterator<iterator1_type>::constraints ();
            RandomAccessIterator<iterator2_type>::constraints ();
            Indexed2DIterator<iterator1_type, iterator2_type>::constraints ();
            Indexed2DIterator<iterator2_type, iterator1_type>::constraints ();
        }
    };

    template<class I1, class I2>
    struct MutableIndexedRandomAccess2DIterator {
        typedef I1 iterator1_type;
        typedef I2 iterator2_type;

        static void constraints () {
            MutableRandomAccessIterator<iterator1_type>::constraints ();
            MutableRandomAccessIterator<iterator2_type>::constraints ();
            Indexed2DIterator<iterator1_type, iterator2_type>::constraints ();
            Indexed2DIterator<iterator2_type, iterator1_type>::constraints ();
        }
    };

    void concept_checks () {
        // Storage
        RandomAccessIterator<bounded_array<double, 1>::const_iterator, std::ptrdiff_t, double>::constraints ();    
        MutableRandomAccessIterator<bounded_array<double, 1>::iterator, std::ptrdiff_t, double>::constraints ();    

        RandomAccessIterator<unbounded_array<double>::const_iterator, std::ptrdiff_t, double>::constraints ();    
        MutableRandomAccessIterator<unbounded_array<double>::iterator, std::ptrdiff_t, double>::constraints ();    

        RandomAccessIterator<range::const_iterator, std::ptrdiff_t, std::size_t>::constraints ();    
        RandomAccessIterator<slice::const_iterator, std::ptrdiff_t, std::size_t>::constraints ();    

        // Sparse Storage
        RandomAccessIterator<compressed_array<std::size_t, double>::const_iterator, std::ptrdiff_t, std::pair<std::size_t, double> >::constraints ();    
        MutableRandomAccessIterator<compressed_array<std::size_t, double>::iterator, std::ptrdiff_t, std::pair<std::size_t, double> >::constraints ();    

        // Vector 
        IndexedRandomAccess1DIterator<vector<double>::const_iterator>::constraints ();
        MutableIndexedRandomAccess1DIterator<vector<double>::iterator>::constraints ();

        IndexedRandomAccess1DIterator<canonical_vector<double>::const_iterator>::constraints ();

        IndexedRandomAccess1DIterator<c_vector<double, 1>::const_iterator>::constraints ();
        MutableIndexedRandomAccess1DIterator<c_vector<double, 1>::iterator>::constraints ();

        // Vector Proxies
        IndexedRandomAccess1DIterator<vector_range<vector<double> >::const_iterator>::constraints ();
        MutableIndexedRandomAccess1DIterator<vector_range<vector<double> >::iterator>::constraints ();

        IndexedRandomAccess1DIterator<vector_slice<vector<double> >::const_iterator>::constraints ();
        MutableIndexedRandomAccess1DIterator<vector_slice<vector<double> >::iterator>::constraints ();

        // Sparse Vector 
        IndexedBidirectional1DIterator<sparse_vector<double>::const_iterator>::constraints ();    
        MutableIndexedBidirectional1DIterator<sparse_vector<double>::iterator>::constraints ();

        // Matrix
        IndexedRandomAccess2DIterator<matrix<double>::const_iterator1, matrix<double>::const_iterator2>::constraints ();
        MutableIndexedRandomAccess2DIterator<matrix<double>::iterator1, matrix<double>::iterator2>::constraints ();

        IndexedRandomAccess2DIterator<vector_of_vector<double>::const_iterator1, vector_of_vector<double>::const_iterator2>::constraints ();
        MutableIndexedRandomAccess2DIterator<vector_of_vector<double>::iterator1, vector_of_vector<double>::iterator2>::constraints ();

        IndexedRandomAccess2DIterator<identity_matrix<double>::const_iterator1, identity_matrix<double>::const_iterator2>::constraints ();

        IndexedRandomAccess2DIterator<c_matrix<double, 1, 1>::const_iterator1, c_matrix<double, 1, 1>::const_iterator2>::constraints ();
        MutableIndexedRandomAccess2DIterator<c_matrix<double, 1, 1>::iterator1, c_matrix<double, 1, 1>::iterator2>::constraints ();

        // Matrix Proxies
        IndexedRandomAccess1DIterator<matrix_row<matrix<double> >::const_iterator>::constraints ();
        MutableIndexedRandomAccess1DIterator<matrix_row<matrix<double> >::iterator>::constraints ();

        IndexedRandomAccess1DIterator<matrix_column<matrix<double> >::const_iterator>::constraints ();
        MutableIndexedRandomAccess1DIterator<matrix_column<matrix<double> >::iterator>::constraints ();

        IndexedRandomAccess1DIterator<matrix_vector_range<matrix<double> >::const_iterator>::constraints ();
        MutableIndexedRandomAccess1DIterator<matrix_vector_range<matrix<double> >::iterator>::constraints ();

        IndexedRandomAccess1DIterator<matrix_vector_slice<matrix<double> >::const_iterator>::constraints ();
        MutableIndexedRandomAccess1DIterator<matrix_vector_slice<matrix<double> >::iterator>::constraints ();

        IndexedRandomAccess2DIterator<matrix_range<matrix<double> >::const_iterator1, matrix_range<matrix<double> >::const_iterator2>::constraints ();
        MutableIndexedRandomAccess2DIterator<matrix_range<matrix<double> >::iterator1, matrix_range<matrix<double> >::iterator2>::constraints ();

        IndexedRandomAccess2DIterator<matrix_slice<matrix<double> >::const_iterator1, matrix_slice<matrix<double> >::const_iterator2>::constraints ();
        MutableIndexedRandomAccess2DIterator<matrix_slice<matrix<double> >::iterator1, matrix_slice<matrix<double> >::iterator2>::constraints ();

        // Banded Matrix
        IndexedBidirectional2DIterator<banded_matrix<double>::const_iterator1, banded_matrix<double>::const_iterator2>::constraints ();
        MutableIndexedBidirectional2DIterator<banded_matrix<double>::iterator1, banded_matrix<double>::iterator2>::constraints ();

        // Sparse Matrix
        IndexedBidirectional2DIterator<sparse_matrix<double>::const_iterator1, sparse_matrix<double>::const_iterator2>::constraints ();
        MutableIndexedBidirectional2DIterator<sparse_matrix<double>::iterator1, sparse_matrix<double>::iterator2>::constraints ();

        IndexedBidirectional2DIterator<sparse_vector_of_sparse_vector<double>::const_iterator1, sparse_vector_of_sparse_vector<double>::const_iterator2>::constraints ();
        MutableIndexedBidirectional2DIterator<sparse_vector_of_sparse_vector<double>::iterator1, sparse_vector_of_sparse_vector<double>::iterator2>::constraints ();
    }

}

#endif

