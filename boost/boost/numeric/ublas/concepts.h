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

#ifndef NUMERICS_CONCEPTS_H
#define NUMERICS_CONCEPTS_H

// #define INTERNAL_STORAGE
// #define INTERNAL_STORAGE_SPARSE
// #define INTERNAL_VECTOR
// #define INTERNAL_VECTOR_PROXY
// #define INTERNAL_VECTOR_SPARSE
// #define INTERNAL_MATRIX
// #define INTERNAL_MATRIX_PROXY
// #define INTERNAL_BANDED
// #define INTERNAL_TRIANGULAR
// #define INTERNAL_SYMMETRIC
// #define INTERNAL_HERMITEAN
// #define INTERNAL_MATRIX_SPARSE
// #define INTERNAL_VECTOR_EXPRESSION
// #define INTERNAL_MATRIX_EXPRESSION

// #define EXTERNAL

// Concept checks based on ideas of Jeremy Siek

namespace boost { namespace numerics {

    template<class T>
    void ignore_unused_variable_warning (const T &t) {}

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
            ignore_unused_variable_warning (a);
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
            ignore_unused_variable_warning (b);
        }
    };

    template<class T>
    struct LessThanComparable {
        typedef T value_type;

        static void constraints (const value_type t) {
            bool b;
            b = t < t;
            b = t <= t;
            b = t >= t;
            b = t > t;
            ignore_unused_variable_warning (b);
        }
    };

    template<class T>
    struct DefaultConstructible {
        typedef T value_type;

        static void constraints () {
            // Default Constructor
            static value_type c1 = value_type ();
            static value_type c2;
            ignore_unused_variable_warning (c1);
            ignore_unused_variable_warning (c2);
        }
    };

    template<class I, class T = NUMERICS_TYPENAME I::value_type>
    struct BidirectionalIterator {
        typedef I iterator_type;
        typedef T value_type;

        static void constraints () {
#define NUMERICS_ITERATOR_IS_ASSIGNABLE
#ifdef NUMERICS_ITERATOR_IS_ASSIGNABLE
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
            ignore_unused_variable_warning (t);
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
#ifdef NUMERICS_NON_STD
            // Dereference assignment
            *it = t;
#else
            ignore_unused_variable_warning (it);
#endif
            ignore_unused_variable_warning (t);
        }
    };

    template<class I, class D = NUMERICS_TYPENAME I::difference_type, class T = NUMERICS_TYPENAME I::value_type>
    struct RandomAccessIterator {
        typedef I iterator_type;
        typedef D difference_type;
        typedef T value_type;

        static void constraints () {
            LessThanComparable<iterator_type>::constraints (iterator_type ());
            BidirectionalIterator<iterator_type, value_type>::constraints ();
            iterator_type it = iterator_type (), it1 = iterator_type (), it2 = iterator_type ();
            difference_type n (0);
            value_type t = value_type ();
            // Forward motion
            it += n;
            // Iterator addition
#ifdef NUMERICS_ITERATOR_IS_ASSIGNABLE
            it = it + n;
#else
            iterator_type itp (it + n);
#endif
            // Backward motion
            it -= n;
            // Iterator subtraction
#ifdef NUMERICS_ITERATOR_IS_ASSIGNABLE
            it = it - n;
#else
            iterator_type itm (it - n);
#endif
            // Difference
            n = it1 - it2;
            // Element operator
#ifdef NUMERICS_ITERATOR_IS_INDEXABLE
            t = it [n];
#else
            t = *(it + n);
#endif
            ignore_unused_variable_warning (t);
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
#ifdef NUMERICS_ITERATOR_IS_INDEXABLE
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

    template<class I>
    struct Indexed2DIterator {
        typedef I iterator_type;
#ifndef NUMERICS_USE_CANONICAL_ITERATOR
        typedef typename I::dual_iterator_type dual_iterator_type;
        typedef typename I::dual_reverse_iterator_type dual_reverse_iterator_type;
#endif

        static void constraints () {
            iterator_type it = iterator_type ();
            // Function call
            it ();
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            // Indices
            it.index ();
#else
            // Indices
            it.index1 ();
            it.index2 ();
            // Iterator begin/end
            dual_iterator_type it_begin (it.begin ());
            dual_iterator_type it_end (it.end ());
            // Reverse iterator begin/end
            dual_reverse_iterator_type it_rbegin (it.rbegin ());
            dual_reverse_iterator_type it_rend (it.rend ());
            ignore_unused_variable_warning (it_begin);
            ignore_unused_variable_warning (it_end);
            ignore_unused_variable_warning (it_rbegin);
            ignore_unused_variable_warning (it_rend);
#endif
        }
    };

    template<class I1, class I2>
    struct IndexedBidirectional2DIterator {
        typedef I1 iterator1_type;
        typedef I2 iterator2_type;

        static void constraints () {
            BidirectionalIterator<iterator1_type>::constraints ();
            BidirectionalIterator<iterator2_type>::constraints ();
            Indexed2DIterator<iterator1_type>::constraints ();
            Indexed2DIterator<iterator2_type>::constraints ();
        }
    };

    template<class I1, class I2>
    struct MutableIndexedBidirectional2DIterator {
        typedef I1 iterator1_type;
        typedef I2 iterator2_type;

        static void constraints () {
            MutableBidirectionalIterator<iterator1_type>::constraints ();
            MutableBidirectionalIterator<iterator2_type>::constraints ();
            Indexed2DIterator<iterator1_type>::constraints ();
            Indexed2DIterator<iterator2_type>::constraints ();
        }
    };

    template<class I1, class I2>
    struct IndexedRandomAccess2DIterator {
        typedef I1 iterator1_type;
        typedef I2 iterator2_type;

        static void constraints () {
            RandomAccessIterator<iterator1_type>::constraints ();
            RandomAccessIterator<iterator2_type>::constraints ();
            Indexed2DIterator<iterator1_type>::constraints ();
            Indexed2DIterator<iterator2_type>::constraints ();
        }
    };

    template<class I1, class I2>
    struct MutableIndexedRandomAccess2DIterator {
        typedef I1 iterator1_type;
        typedef I2 iterator2_type;

        static void constraints () {
            MutableRandomAccessIterator<iterator1_type>::constraints ();
            MutableRandomAccessIterator<iterator2_type>::constraints ();
            Indexed2DIterator<iterator1_type>::constraints ();
            Indexed2DIterator<iterator2_type>::constraints ();
        }
    };

    template<class C>
    struct Container {
        typedef C container_type;
        typedef typename C::size_type size_type;
        typedef typename C::const_iterator const_iterator_type;
        
        static void constraints () {
            DefaultConstructible<container_type>::constraints ();    
            container_type c = container_type ();
            size_type n (0);
            // Beginning of range
            const_iterator_type cit_begin (c.begin ());
            // End of range
            const_iterator_type cit_end (c.end ());
            // Size
            n = c.size ();
            ignore_unused_variable_warning (cit_end);
            ignore_unused_variable_warning (cit_begin);
            ignore_unused_variable_warning (n);
        }
    };

    template<class C>
    struct MutableContainer {
        typedef C container_type;
        typedef typename C::iterator iterator_type;
        
        static void constraints () {
            Assignable<container_type>::constraints (container_type ());
            Container<container_type>::constraints ();
            container_type c = container_type (), c1 = container_type (), c2 = container_type ();
            // Beginning of range
            iterator_type it_begin (c.begin ());
            // End of range
            iterator_type it_end (c.end ());
            // Swap
            c1.swap (c2);
            ignore_unused_variable_warning (it_end);
            ignore_unused_variable_warning (it_begin);
        }
    };

    template<class C>
    struct ReversibleContainer {
        typedef C container_type;
        typedef typename C::const_reverse_iterator const_reverse_iterator_type;

        static void constraints () {
            Container<container_type>::constraints ();
#ifndef NUMERICS_LWG_ISSUE_280
            const container_type cc = container_type ();
#else
            container_type c = container_type ();
#endif
            // Beginning of reverse range
#ifndef NUMERICS_LWG_ISSUE_280
            const_reverse_iterator_type crit_begin (cc.rbegin ());
#else
            const_reverse_iterator_type crit_begin (c.rbegin ());
#endif
            // End of reverse range
#ifndef NUMERICS_LWG_ISSUE_280
            const_reverse_iterator_type crit_end (cc.rend ());
#else
            const_reverse_iterator_type crit_end (c.rend ());
#endif
            ignore_unused_variable_warning (crit_end);
            ignore_unused_variable_warning (crit_begin);
        }
    };

    template<class C>
    struct MutableReversibleContainer {
        typedef C container_type;
        typedef typename C::reverse_iterator reverse_iterator_type;
        
        static void constraints () {
            MutableContainer<container_type>::constraints ();
            ReversibleContainer<container_type>::constraints ();
            container_type c = container_type ();
            // Beginning of reverse range
            reverse_iterator_type rit_begin (c.rbegin ());
            // End of reverse range
            reverse_iterator_type rit_end (c.rend ());
            ignore_unused_variable_warning (rit_end);
            ignore_unused_variable_warning (rit_begin);
        }
    };

    template<class C>
    struct RandomAccessContainer {
        typedef C container_type;
        typedef typename C::size_type size_type;
        typedef typename C::value_type value_type;
        
        static void constraints () {
            ReversibleContainer<container_type>::constraints ();
            container_type c = container_type ();
            size_type n (0);
            value_type t = value_type ();
            // Element access
            t = c [n];
            ignore_unused_variable_warning (t);
        }
    };

    template<class C>
    struct MutableRandomAccessContainer {
        typedef C container_type;
        typedef typename C::size_type size_type;
        typedef typename C::value_type value_type;
        
        static void constraints () {
            MutableReversibleContainer<container_type>::constraints ();
            RandomAccessContainer<container_type>::constraints ();
            container_type c = container_type ();
            size_type n (0);
            value_type t = value_type ();
            // Element access
            c [n] = t;
        }
    };

    template<class C>
    struct StorageContainer {
        typedef C container_type;
        typedef typename C::size_type size_type;
        
        static void constraints () {
            RandomAccessContainer<container_type>::constraints ();
            size_type n (0);
            // Sizing constructor
            container_type c = container_type (n);
            ignore_unused_variable_warning (c);
        }
    };

    template<class C>
    struct MutableStorageContainer {
        typedef C container_type;
        typedef typename C::size_type size_type;
        typedef typename C::value_type value_type;
        typedef typename C::iterator iterator_type;
        
        static void constraints () {
            MutableRandomAccessContainer<container_type>::constraints ();
            size_type n (0);
            // Sizing constructor
            container_type c = container_type (n);
            value_type t = value_type ();
            iterator_type it = iterator_type (), it1 = iterator_type (), it2 = iterator_type ();
            // Insert 
            c.insert (it, t);
            // Range insert
            c.insert (it, it1, it2);
            // Erase
            c.erase (it);
            // Range erase
            c.erase (it1, it2);
            // Clear
            c.clear ();
            // Resize 
            c.resize (n);
        }
    };

    template<class C>
    struct SparseStorageContainer {
        typedef C container_type;
        typedef typename C::size_type size_type;
        
        static void constraints () {
            ReversibleContainer<container_type>::constraints ();
        }
    };

    template<class C>
    struct MutableSparseStorageContainer {
        typedef C container_type;
        typedef typename C::size_type size_type;
        typedef typename C::value_type value_type;
        typedef typename C::iterator iterator_type;
        
        static void constraints () {
            MutableReversibleContainer<container_type>::constraints ();
            container_type c = container_type ();
            value_type t = value_type ();
            iterator_type it = iterator_type (), it1 = iterator_type (), it2 = iterator_type ();
            // Insert 
            c.insert (it, t);
#ifdef NUMERICS_NON_STD
            // Range insert
            c.insert (it, it1, it2);
#endif
            // Erase
            c.erase (it);
            // Range erase
            c.erase (it1, it2);
            // Clear 
            c.clear ();
        }
    };

    template<class G>
    struct IndexSet {
        typedef G generator_type;
        typedef typename G::size_type size_type;
        typedef typename G::value_type value_type;

        static void constraints () {
            DefaultConstructible<generator_type>::constraints ();    
            ReversibleContainer<generator_type>::constraints ();
            generator_type g = generator_type ();
            size_type n (0);
            value_type t = value_type ();
            // Element access 
            t = g (n);
            ignore_unused_variable_warning (t);
        }
    };

    template<class S>
    struct ScalarExpression {
        typedef S scalar_type;
        typedef typename S::value_type value_type;
        
        static void constraints () {
            DefaultConstructible<scalar_type>::constraints ();
            scalar_type s = scalar_type ();
            value_type t = value_type ();
            // Conversion
            t = s;
            ignore_unused_variable_warning (t);
        }
    };

    template<class V>
    struct VectorExpression {
        typedef V vector_type;
        typedef typename V::type_category type_category;
        typedef typename V::size_type size_type;
        typedef typename V::value_type value_type;
        typedef typename V::const_iterator const_iterator_type;
        typedef typename V::const_reverse_iterator const_reverse_iterator_type;

        static void constraints () {
            DefaultConstructible<vector_type>::constraints ();
            vector_type v = vector_type ();
            size_type n (0), i (0);
            value_type t = value_type ();
            // Find (internal?)
            const_iterator_type cit_lower (v.find_first (i));
            const_iterator_type cit_upper (v.find_last (i));
            // Beginning of range
            const_iterator_type cit_begin (v.begin ());
            // End of range
            const_iterator_type cit_end (v.end ());
            // Size
            n = v.size ();
            // Beginning of reverse range
#ifndef NUMERICS_LWG_ISSUE_280
            const vector_type cv = vector_type ();
            const_reverse_iterator_type crit_begin (cv.rbegin ());
#else
            const_reverse_iterator_type crit_begin (v.rbegin ());
#endif
            // End of reverse range
#ifndef NUMERICS_LWG_ISSUE_280
            const_reverse_iterator_type crit_end (cv.rend ());
#else
            const_reverse_iterator_type crit_end (v.rend ());
#endif
            // Element access
            t = v (i);
            ignore_unused_variable_warning (n);
            ignore_unused_variable_warning (cit_lower);
            ignore_unused_variable_warning (cit_upper);
            ignore_unused_variable_warning (cit_begin);
            ignore_unused_variable_warning (cit_end);
            ignore_unused_variable_warning (crit_begin);
            ignore_unused_variable_warning (crit_end);
            ignore_unused_variable_warning (t);
        }
    };

    template<class V>
    struct MutableVectorExpression {
        typedef V vector_type;
        typedef typename V::size_type size_type;
        typedef typename V::value_type value_type;
        typedef typename V::iterator iterator_type;
        typedef typename V::reverse_iterator reverse_iterator_type;

        static void constraints () {
            Assignable<vector_type>::constraints (vector_type ());
            VectorExpression<vector_type>::constraints ();
            vector_type v = vector_type (), v1 = vector_type (), v2 = vector_type ();
            size_type i (0);
            value_type t = value_type ();
            // Find (internal?)
            iterator_type it_lower (v.find_first (i));
            iterator_type it_upper (v.find_last (i));
            // Beginning of range
            iterator_type it_begin (v.begin ());
            // End of range
            iterator_type it_end (v.end ());
            // Swap
            v1.swap (v2);
            // Beginning of reverse range
            reverse_iterator_type rit_begin (v.rbegin ());
            // End of reverse range
            reverse_iterator_type rit_end (v.rend ());
            // Assignments
            v2 = v1;
            v2.assign (v1);
            v2 += v1;
            v2.plus_assign (v1);
            v2 -= v1;
            v2.minus_assign (v1);
            v *= t;
            ignore_unused_variable_warning (it_lower);
            ignore_unused_variable_warning (it_upper);
            ignore_unused_variable_warning (it_begin);
            ignore_unused_variable_warning (it_end);
            ignore_unused_variable_warning (rit_begin);
            ignore_unused_variable_warning (rit_end);
        }
    };

    template<class M>
    struct MatrixExpression {
        typedef M matrix_type;
        typedef typename M::type_category type_category;
        typedef typename M::size_type size_type;
        typedef typename M::value_type value_type;
        typedef typename M::const_iterator1 const_iterator1_type;
        typedef typename M::const_iterator2 const_iterator2_type;
        typedef typename M::const_reverse_iterator1 const_reverse_iterator1_type;
        typedef typename M::const_reverse_iterator2 const_reverse_iterator2_type;

        static void constraints () {
            DefaultConstructible<matrix_type>::constraints ();
            matrix_type m = matrix_type ();
            size_type n (0), i (0), j (0);
            value_type t = value_type ();
            // Find (internal?)
            const_iterator1_type cit1_lower (m.find_first1 (0, i, j));
            const_iterator1_type cit1_upper (m.find_last1 (0, i, j));
            const_iterator2_type cit2_lower (m.find_first2 (0, i, j));
            const_iterator2_type cit2_upper (m.find_last2 (0, i, j));
            // Beginning of range
            const_iterator1_type cit1_begin (m.begin1 ());
            const_iterator2_type cit2_begin (m.begin2 ());
            // End of range
            const_iterator1_type cit1_end (m.end1 ());
            const_iterator2_type cit2_end (m.end2 ());
            // Size
            n = m.size1 ();
            n = m.size2 ();
            // Beginning of reverse range
#ifndef NUMERICS_LWG_ISSUE_280
            const matrix_type cm = matrix_type ();
            const_reverse_iterator1_type crit1_begin (cm.rbegin1 ());
            const_reverse_iterator2_type crit2_begin (cm.rbegin2 ());
#else
            const_reverse_iterator1_type crit1_begin (m.rbegin1 ());
            const_reverse_iterator2_type crit2_begin (m.rbegin2 ());
#endif
            // End of reverse range
#ifndef NUMERICS_LWG_ISSUE_280
            const_reverse_iterator1_type crit1_end (cm.rend1 ());
            const_reverse_iterator2_type crit2_end (cm.rend2 ());
#else
            const_reverse_iterator1_type crit1_end (m.rend1 ());
            const_reverse_iterator2_type crit2_end (m.rend2 ());
#endif
            // Element access
            t = m (i, j);
            ignore_unused_variable_warning (n);
            ignore_unused_variable_warning (cit1_lower);
            ignore_unused_variable_warning (cit1_upper);
            ignore_unused_variable_warning (cit2_lower);
            ignore_unused_variable_warning (cit2_upper);
            ignore_unused_variable_warning (cit1_begin);
            ignore_unused_variable_warning (cit2_begin);
            ignore_unused_variable_warning (cit1_end);
            ignore_unused_variable_warning (cit2_end);
            ignore_unused_variable_warning (crit1_begin);
            ignore_unused_variable_warning (crit2_begin);
            ignore_unused_variable_warning (crit1_end);
            ignore_unused_variable_warning (crit2_end);
            ignore_unused_variable_warning (t);
        }
    };

    template<class M>
    struct MutableMatrixExpression {
        typedef M matrix_type;
        typedef typename M::size_type size_type;
        typedef typename M::value_type value_type;
        typedef typename M::iterator1 iterator1_type;
        typedef typename M::iterator2 iterator2_type;
        typedef typename M::reverse_iterator1 reverse_iterator1_type;
        typedef typename M::reverse_iterator2 reverse_iterator2_type;

        static void constraints () {
            Assignable<matrix_type>::constraints (matrix_type ());
            MatrixExpression<matrix_type>::constraints ();
            matrix_type m = matrix_type (), m1 = matrix_type (), m2 = matrix_type ();
            size_type i (0), j (0);
            value_type t = value_type ();
            // Find (internal?)
            iterator1_type it1_lower (m.find_first1 (0, i, j));
            iterator1_type it1_upper (m.find_last1 (0, i, j));
            iterator2_type it2_lower (m.find_first2 (0, i, j));
            iterator2_type it2_upper (m.find_last2 (0, i, j));
            // Beginning of range
            iterator1_type it1_begin (m.begin1 ());
            iterator2_type it2_begin (m.begin2 ());
            // End of range
            iterator1_type it1_end (m.end1 ());
            iterator2_type it2_end (m.end2 ());
            // Swap
            m1.swap (m2);
            // Beginning of reverse range
            reverse_iterator1_type rit1_begin (m.rbegin1 ());
            reverse_iterator2_type rit2_begin (m.rbegin2 ());
            // End of reverse range
            reverse_iterator1_type rit1_end (m.rend1 ());
            reverse_iterator2_type rit2_end (m.rend2 ());
            // Assignments
            m2 = m1;
            m2.assign (m1);
            m2 += m1;
            m2.plus_assign (m1);
            m2 -= m1;
            m2.minus_assign (m1);
            m *= t;
            ignore_unused_variable_warning (it1_lower);
            ignore_unused_variable_warning (it1_upper);
            ignore_unused_variable_warning (it2_lower);
            ignore_unused_variable_warning (it2_upper);
            ignore_unused_variable_warning (it1_begin);
            ignore_unused_variable_warning (it2_begin);
            ignore_unused_variable_warning (it1_end);
            ignore_unused_variable_warning (it2_end);
            ignore_unused_variable_warning (rit1_begin);
            ignore_unused_variable_warning (rit2_begin);
            ignore_unused_variable_warning (rit1_end);
            ignore_unused_variable_warning (rit2_end);
        }
    };

    template<class V>
    struct Vector {
        typedef V vector_type;

        static void constraints () {
            VectorExpression<vector_type>::constraints ();
        }
    };

    template<class V>
    struct MutableVector {
        typedef V vector_type;
        typedef typename V::size_type size_type;
        typedef typename V::value_type value_type;
        
        static void constraints () {
            MutableVectorExpression<vector_type>::constraints ();    
            Vector<vector_type>::constraints ();    
            size_type n (0);
            // Sizing constructor
            vector_type v = vector_type (n);
            value_type t = value_type ();
            size_type i (0);
            // Insert 
            v.insert (i, t);
            // Erase
            v.erase (i);
            // Clear 
            v.clear ();
            // Resize 
            v.resize (n);
        }
    };

    template<class M>
    struct Matrix {
        typedef M matrix_type;
        
        static void constraints () {
            MatrixExpression<matrix_type>::constraints ();
        }
    };

    template<class M>
    struct MutableMatrix {
        typedef M matrix_type;
        typedef typename M::size_type size_type;
        typedef typename M::value_type value_type;

        static void constraints () {
            MutableMatrixExpression<matrix_type>::constraints ();    
            Matrix<matrix_type>::constraints ();    
            size_type n (0);
            // Sizing constructor
            matrix_type m = matrix_type (n, n);
            value_type t = value_type ();
            size_type i (0), j (0);
            // Insert 
            m.insert (i, j, t);
            // Erase
            m.erase (i, j);
            // Clear 
            m.clear ();
            // Resize 
            m.resize (n, n);
        }
    };

    template<class T>
    T 
    zero (T);
    template<>
    float 
    zero (float) {
        return 0.f;
    }
    template<>
    double 
    zero (double) {
        return 0.;
    }
    template<>
    std::complex<float>
    zero (std::complex<float>) {
        return std::complex<float> (0.f);
    }
    template<>
    std::complex<double>
    zero (std::complex<double>) {
        return std::complex<float> (0.);
    }
    template<>
    vector<float> 
    zero (vector<float>) {
        return zero_vector<float> ();
    }
    template<>
    vector<double> 
    zero (vector<double>) {
        return zero_vector<double> ();
    }
    template<>
    vector<std::complex<float> > 
    zero (vector<std::complex<float> >) {
        return zero_vector<std::complex<float> > ();
    }
    template<>
    vector<std::complex<double> > 
    zero (vector<std::complex<double> >) {
        return zero_vector<std::complex<double> > ();
    }
    template<>
    matrix<float> 
    zero (matrix<float>) {
        return zero_matrix<float> ();
    }
    template<>
    matrix<double> 
    zero (matrix<double>) {
        return zero_matrix<double> ();
    }
    template<>
    matrix<std::complex<float> > 
    zero (matrix<std::complex<float> >) {
        return zero_matrix<std::complex<float> > ();
    }
    template<>
    matrix<std::complex<double> > 
    zero (matrix<std::complex<double> >) {
        return zero_matrix<std::complex<double> > ();
    }

    template<class T>
    T 
    one (T);
    template<>
    float
    one (float) {
        return 1.f;
    }
    template<>
    double 
    one (double) {
        return 1.;
    }
    template<>
    std::complex<float>
    one (std::complex<float>) {
        return std::complex<float> (1.f);
    }
    template<>
    std::complex<double>
    one (std::complex<double>) {
        return std::complex<double> (1.);
    }
    template<>
    matrix<float> 
    one (matrix<float>) {
        return identity_matrix<float> ();
    }
    template<>
    matrix<double> 
    one (matrix<double>) {
        return identity_matrix<double> ();
    }
    template<>
    matrix<std::complex<float> > 
    one (matrix<std::complex<float> >) {
        return identity_matrix<std::complex<float> > ();
    }
    template<>
    matrix<std::complex<double> > 
    one (matrix<std::complex<double> >) {
        return identity_matrix<std::complex<double> > ();
    }

    template<class E1, class E2>
    bool 
    operator == (const vector_expression<E1> &e1, const vector_expression<E2> &e2) {
        typedef NUMERICS_TYPENAME promote_traits<NUMERICS_TYPENAME E1::value_type, 
                                                 NUMERICS_TYPENAME E2::value_type>::promote_type value_type;
        typedef NUMERICS_TYPENAME type_traits<value_type>::real_type real_type;
        return norm_inf (e1 - e2) == real_type ();
    }
    template<class E1, class E2>
    bool
    operator == (const matrix_expression<E1> &e1, const matrix_expression<E2> &e2) {
        typedef NUMERICS_TYPENAME promote_traits<NUMERICS_TYPENAME E1::value_type,
                                                 NUMERICS_TYPENAME E2::value_type>::promote_type value_type;
        typedef NUMERICS_TYPENAME type_traits<value_type>::real_type real_type;
        return norm_inf (e1 - e2) == real_type ();
    }

    template<class T>
    struct AdditiveAbelianGroup {
        typedef T value_type;

        static void constraints () {
            bool r;
            value_type a = value_type (), b = value_type (), c = value_type ();
            r = (a + b) + c == a + (b + c);
            r = zero (value_type ()) + a == a;
            r = a + zero (value_type ()) == a;
            r = a + (- a) == zero (value_type ());
            r = (- a) + a == zero (value_type ());
            r = a + b == b + a;
            ignore_unused_variable_warning (r);
        }
    };

    template<class T>
    struct MultiplicativeAbelianGroup {
        typedef T value_type;

        static void constraints () {
            bool r;
            value_type a = value_type (), b = value_type (), c = value_type ();
            r = (a * b) * c == a * (b * c);
            r = one (value_type ()) * a == a;
            r = a * one (value_type ()) == a;
            r = a * (one (value_type ()) / a) == a;
            r = (one (value_type ()) / a) * a == a;
            r = a * b == b * a;
            ignore_unused_variable_warning (r);
        }
    };

    template<class T>
    struct RingWithIdentity {
        typedef T value_type;

        static void constraints () {
            AdditiveAbelianGroup<value_type>::constraints ();
            bool r;
            value_type a = value_type (), b = value_type (), c = value_type ();
            r = (a * b) * c == a * (b * c);
            r = (a + b) * c == a * c + b * c;
            r = one (value_type ()) * a == a;
            r = a * one (value_type ()) == a;
            ignore_unused_variable_warning (r);
        }
        static void constraints (int) {
            AdditiveAbelianGroup<value_type>::constraints ();
            bool r;
            value_type a = value_type (), b = value_type (), c = value_type ();
            r = prod (prod (a, b), c) == prod (a, prod (b, c)) ;
            r = prod (a + b, c) == prod (a, c) + prod (b, c);
            r = prod (one (value_type ()), a) == a;
            r = prod (a, one (value_type ())) == a;
            ignore_unused_variable_warning (r);
        }
    };

    template<class T>
    struct CommutativeRingWithIdentity {
        typedef T value_type;

        static void constraints () {
            RingWithIdentity<value_type>::constraints ();
            bool r;
            value_type a = value_type (), b = value_type ();
            r = a * b == b * a;
            ignore_unused_variable_warning (r);
        }
    };

    template<class T>
    struct Field {
        typedef T value_type;

        static void constraints () {
            CommutativeRingWithIdentity<value_type>::constraints ();
            bool r;
            value_type a = value_type ();
            r = a == zero (value_type ()) || a * (one (value_type ()) / a) == a;
            r = a == zero (value_type ()) || (one (value_type ()) / a) * a == a;
            ignore_unused_variable_warning (r);
        }
    };

    template<class T, class V>
    struct VectorSpace {
        typedef T value_type;
        typedef V vector_type;

        static void constraints () {
            Field<value_type>::constraints ();
            AdditiveAbelianGroup<vector_type>::constraints ();
            bool r;
            value_type alpha = value_type (), beta = value_type ();
            vector_type a = vector_type (), b = vector_type ();
            r = alpha * (a + b) == alpha * a + alpha * b;
            r = (alpha + beta) * a == alpha * a + beta * a;
            r = (alpha * beta) * a == alpha * (beta * a);
            r = one (value_type ()) * a == a;
            ignore_unused_variable_warning (r);
        }
    };

    template<class T, class V, class M>
    struct LinearOperator {
        typedef T value_type;
        typedef V vector_type;
        typedef M matrix_type;

        static void constraints () {
            VectorSpace<value_type, vector_type>::constraints ();
            bool r;
            value_type alpha = value_type (), beta = value_type ();
            vector_type a = vector_type (), b = vector_type ();
            matrix_type A = matrix_type ();
            r = prod (A, alpha * a + beta * b) == alpha * prod (A, a) + beta * prod (A, b);
            ignore_unused_variable_warning (r);
        }
    };
 
    void concept_checks () {

        // Storage
#if defined (INTERNAL) || defined (INTERNAL_STORAGE)
        StorageContainer<const std::vector<double> >::constraints ();
        MutableStorageContainer<std::vector<double> >::constraints ();
        RandomAccessIterator<std::vector<double>::const_iterator, std::ptrdiff_t, double>::constraints ();
        MutableRandomAccessIterator<std::vector<double>::iterator, std::ptrdiff_t, double>::constraints ();

        StorageContainer<const bounded_array<double, 1> >::constraints ();
        MutableStorageContainer<bounded_array<double, 1> >::constraints ();
        RandomAccessIterator<bounded_array<double, 1>::const_iterator, std::ptrdiff_t, double>::constraints ();
        MutableRandomAccessIterator<bounded_array<double, 1>::iterator, std::ptrdiff_t, double>::constraints ();

        StorageContainer<const unbounded_array<double> >::constraints ();
        MutableStorageContainer<unbounded_array<double> >::constraints ();
        RandomAccessIterator<unbounded_array<double>::const_iterator, std::ptrdiff_t, double>::constraints ();
        MutableRandomAccessIterator<unbounded_array<double>::iterator, std::ptrdiff_t, double>::constraints ();

        StorageContainer<const array_adaptor<double> >::constraints ();
        MutableStorageContainer<array_adaptor<double> >::constraints ();
        RandomAccessIterator<array_adaptor<double>::const_iterator, std::ptrdiff_t, double>::constraints ();
        MutableRandomAccessIterator<array_adaptor<double>::iterator, std::ptrdiff_t, double>::constraints ();

        IndexSet<range>::constraints ();
        RandomAccessIterator<range::const_iterator, std::ptrdiff_t, std::size_t>::constraints ();

        IndexSet<slice>::constraints ();
        RandomAccessIterator<slice::const_iterator, std::ptrdiff_t, std::size_t>::constraints ();

        IndexSet<indirect_array<> >::constraints ();
        RandomAccessIterator<indirect_array<>::const_iterator, std::ptrdiff_t, std::size_t>::constraints ();
#endif

        // Sparse Storage
#if defined (INTERNAL) || defined (INTERNAL_STORAGE_SPARSE)
#ifndef NUMERICS_NON_STD
        SparseStorageContainer<const std::map<std::size_t, double> >::constraints ();
        MutableSparseStorageContainer<std::map<std::size_t, double> >::constraints ();
        BidirectionalIterator<std::map<std::size_t, double>::const_iterator, std::pair<std::size_t, double> >::constraints ();
        MutableBidirectionalIterator<std::map<std::size_t, double>::iterator, std::pair<std::size_t, double> >::constraints ();
#endif

        SparseStorageContainer<const map_array<std::size_t, double> >::constraints ();
        MutableSparseStorageContainer<map_array<std::size_t, double> >::constraints ();
        RandomAccessIterator<map_array<std::size_t, double>::const_iterator, std::ptrdiff_t, std::pair<std::size_t, double> >::constraints ();    
        MutableRandomAccessIterator<map_array<std::size_t, double>::iterator, std::ptrdiff_t, std::pair<std::size_t, double> >::constraints ();    

#ifndef NUMERICS_NON_STD
        SparseStorageContainer<const std::set<std::size_t> >::constraints ();
        MutableSparseStorageContainer<std::set<std::size_t> >::constraints ();
        BidirectionalIterator<std::set<std::size_t>::const_iterator, std::size_t>::constraints ();
        MutableBidirectionalIterator<std::set<std::size_t>::iterator, std::size_t>::constraints ();
#endif

        SparseStorageContainer<const set_array<std::size_t> >::constraints ();
        MutableSparseStorageContainer<set_array<std::size_t> >::constraints ();
        RandomAccessIterator<set_array<std::size_t>::const_iterator, std::ptrdiff_t, std::size_t>::constraints ();    
        MutableRandomAccessIterator<set_array<std::size_t>::iterator, std::ptrdiff_t, std::size_t>::constraints ();    
#endif

        // Vector
#if defined (INTERNAL) || defined (INTERNAL_VECTOR)
        Vector<const vector<double> >::constraints ();
        MutableVector<vector<double> >::constraints ();
        IndexedRandomAccess1DIterator<vector<double>::const_iterator>::constraints ();
        MutableIndexedRandomAccess1DIterator<vector<double>::iterator>::constraints ();
        IndexedRandomAccess1DIterator<vector<double>::const_reverse_iterator>::constraints ();
        MutableIndexedRandomAccess1DIterator<vector<double>::reverse_iterator>::constraints ();

        Vector<unit_vector<double> >::constraints ();
        IndexedRandomAccess1DIterator<unit_vector<double>::const_iterator>::constraints ();
        IndexedRandomAccess1DIterator<unit_vector<double>::const_reverse_iterator>::constraints ();

        Vector<zero_vector<double> >::constraints ();
        IndexedBidirectional1DIterator<zero_vector<double>::const_iterator>::constraints ();
        IndexedBidirectional1DIterator<zero_vector<double>::const_reverse_iterator>::constraints ();

        Vector<scalar_vector<double> >::constraints ();
        IndexedRandomAccess1DIterator<scalar_vector<double>::const_iterator>::constraints ();
        IndexedRandomAccess1DIterator<scalar_vector<double>::const_reverse_iterator>::constraints ();

        Vector<const c_vector<double, 1> >::constraints ();
        MutableVector<c_vector<double, 1> >::constraints ();
        IndexedRandomAccess1DIterator<c_vector<double, 1>::const_iterator>::constraints ();
        MutableIndexedRandomAccess1DIterator<c_vector<double, 1>::iterator>::constraints ();
        IndexedRandomAccess1DIterator<c_vector<double, 1>::const_reverse_iterator>::constraints ();
        MutableIndexedRandomAccess1DIterator<c_vector<double, 1>::reverse_iterator>::constraints ();
#endif

        // Vector Proxies
#if defined (INTERNAL) || defined (INTERNAL_VECTOR_PROXY)
        VectorExpression<const vector_range<const vector<double> > >::constraints ();
        MutableVectorExpression<vector_range<vector<double> > >::constraints ();
        IndexedRandomAccess1DIterator<vector_range<vector<double> >::const_iterator>::constraints ();
        MutableIndexedRandomAccess1DIterator<vector_range<vector<double> >::iterator>::constraints ();
        IndexedRandomAccess1DIterator<vector_range<vector<double> >::const_reverse_iterator>::constraints ();
        MutableIndexedRandomAccess1DIterator<vector_range<vector<double> >::reverse_iterator>::constraints ();

        VectorExpression<const vector_slice<const vector<double> > >::constraints ();
        MutableVectorExpression<vector_slice<vector<double> > >::constraints ();
        IndexedRandomAccess1DIterator<vector_slice<vector<double> >::const_iterator>::constraints ();
        MutableIndexedRandomAccess1DIterator<vector_slice<vector<double> >::iterator>::constraints ();
        IndexedRandomAccess1DIterator<vector_slice<vector<double> >::const_reverse_iterator>::constraints ();
        MutableIndexedRandomAccess1DIterator<vector_slice<vector<double> >::reverse_iterator>::constraints ();

        VectorExpression<const vector_indirect<const vector<double> > >::constraints ();
        MutableVectorExpression<vector_indirect<vector<double> > >::constraints ();
        IndexedRandomAccess1DIterator<vector_indirect<vector<double> >::const_iterator>::constraints ();
        MutableIndexedRandomAccess1DIterator<vector_indirect<vector<double> >::iterator>::constraints ();
        IndexedRandomAccess1DIterator<vector_indirect<vector<double> >::const_reverse_iterator>::constraints ();
        MutableIndexedRandomAccess1DIterator<vector_indirect<vector<double> >::reverse_iterator>::constraints ();
#endif

        // Sparse Vector
#if defined (INTERNAL) || defined (INTERNAL_VECTOR_SPARSE)
        Vector<const sparse_vector<double> >::constraints ();
        MutableVector<sparse_vector<double> >::constraints ();
        IndexedBidirectional1DIterator<sparse_vector<double>::const_iterator>::constraints ();
        MutableIndexedBidirectional1DIterator<sparse_vector<double>::iterator>::constraints ();
        IndexedBidirectional1DIterator<sparse_vector<double>::const_reverse_iterator>::constraints ();
        MutableIndexedBidirectional1DIterator<sparse_vector<double>::reverse_iterator>::constraints ();

        Vector<const compressed_vector<double> >::constraints ();
        MutableVector<compressed_vector<double> >::constraints ();
        IndexedBidirectional1DIterator<compressed_vector<double>::const_iterator>::constraints ();
        MutableIndexedBidirectional1DIterator<compressed_vector<double>::iterator>::constraints ();
        IndexedBidirectional1DIterator<compressed_vector<double>::const_reverse_iterator>::constraints ();
        MutableIndexedBidirectional1DIterator<compressed_vector<double>::reverse_iterator>::constraints ();
#endif

        // Matrix
#if defined (INTERNAL) || defined (INTERNAL_MATRIX)
        Matrix<const matrix<double> >::constraints ();
        MutableMatrix<matrix<double> >::constraints ();
        IndexedRandomAccess2DIterator<matrix<double>::const_iterator1,
                                      matrix<double>::const_iterator2>::constraints ();
        MutableIndexedRandomAccess2DIterator<matrix<double>::iterator1,
                                             matrix<double>::iterator2>::constraints ();
        IndexedRandomAccess2DIterator<matrix<double>::const_reverse_iterator1,
                                      matrix<double>::const_reverse_iterator2>::constraints ();
        MutableIndexedRandomAccess2DIterator<matrix<double>::reverse_iterator1, 
                                             matrix<double>::reverse_iterator2>::constraints ();

        Matrix<const vector_of_vector<double> >::constraints ();
        MutableMatrix<vector_of_vector<double> >::constraints ();
        IndexedRandomAccess2DIterator<vector_of_vector<double>::const_iterator1, 
                                      vector_of_vector<double>::const_iterator2>::constraints ();
        MutableIndexedRandomAccess2DIterator<vector_of_vector<double>::iterator1, 
                                             vector_of_vector<double>::iterator2>::constraints ();
        IndexedRandomAccess2DIterator<vector_of_vector<double>::const_reverse_iterator1, 
                                      vector_of_vector<double>::const_reverse_iterator2>::constraints ();
        MutableIndexedRandomAccess2DIterator<vector_of_vector<double>::reverse_iterator1, 
                                             vector_of_vector<double>::reverse_iterator2>::constraints ();

        Matrix<identity_matrix<double> >::constraints ();
        IndexedRandomAccess2DIterator<identity_matrix<double>::const_iterator1,
                                      identity_matrix<double>::const_iterator2>::constraints ();
        IndexedRandomAccess2DIterator<identity_matrix<double>::const_reverse_iterator1,
                                      identity_matrix<double>::const_reverse_iterator2>::constraints ();

        Matrix<zero_matrix<double> >::constraints ();
        IndexedBidirectional2DIterator<zero_matrix<double>::const_iterator1,
                                       zero_matrix<double>::const_iterator2>::constraints ();
        IndexedBidirectional2DIterator<zero_matrix<double>::const_reverse_iterator1,
                                       zero_matrix<double>::const_reverse_iterator2>::constraints ();

        Matrix<scalar_matrix<double> >::constraints ();
        IndexedRandomAccess2DIterator<scalar_matrix<double>::const_iterator1, 
                                      scalar_matrix<double>::const_iterator2>::constraints ();
        IndexedRandomAccess2DIterator<scalar_matrix<double>::const_reverse_iterator1, 
                                      scalar_matrix<double>::const_reverse_iterator2>::constraints ();

        Matrix<const c_matrix<double, 1, 1> >::constraints ();
        MutableMatrix<c_matrix<double, 1, 1> >::constraints ();
        IndexedRandomAccess2DIterator<c_matrix<double, 1, 1>::const_iterator1, 
                                      c_matrix<double, 1, 1>::const_iterator2>::constraints ();
        MutableIndexedRandomAccess2DIterator<c_matrix<double, 1, 1>::iterator1,
                                             c_matrix<double, 1, 1>::iterator2>::constraints ();
        IndexedRandomAccess2DIterator<c_matrix<double, 1, 1>::const_reverse_iterator1,
                                      c_matrix<double, 1, 1>::const_reverse_iterator2>::constraints ();
        MutableIndexedRandomAccess2DIterator<c_matrix<double, 1, 1>::reverse_iterator1,
                                             c_matrix<double, 1, 1>::reverse_iterator2>::constraints ();
#endif

        // Matrix Proxies
#if defined (INTERNAL) || defined (INTERNAL_MATRIX_PROXY)
        VectorExpression<const matrix_row<const matrix<double> > >::constraints ();
        MutableVectorExpression<matrix_row<matrix<double> > >::constraints ();
        IndexedRandomAccess1DIterator<matrix_row<matrix<double> >::const_iterator>::constraints ();
        MutableIndexedRandomAccess1DIterator<matrix_row<matrix<double> >::iterator>::constraints ();
        IndexedRandomAccess1DIterator<matrix_row<matrix<double> >::const_reverse_iterator>::constraints ();
        MutableIndexedRandomAccess1DIterator<matrix_row<matrix<double> >::reverse_iterator>::constraints ();

        VectorExpression<const matrix_column<const matrix<double> > >::constraints ();
        MutableVectorExpression<matrix_column<matrix<double> > >::constraints ();
        IndexedRandomAccess1DIterator<matrix_column<matrix<double> >::const_iterator>::constraints ();
        MutableIndexedRandomAccess1DIterator<matrix_column<matrix<double> >::iterator>::constraints ();
        IndexedRandomAccess1DIterator<matrix_column<matrix<double> >::const_reverse_iterator>::constraints ();
        MutableIndexedRandomAccess1DIterator<matrix_column<matrix<double> >::reverse_iterator>::constraints ();

#ifndef NUMERICS_ITERATOR_IS_INDEXABLE
        VectorExpression<const matrix_vector_range<const matrix<double> > >::constraints ();
        MutableVectorExpression<matrix_vector_range<matrix<double> > >::constraints ();
        IndexedRandomAccess1DIterator<matrix_vector_range<matrix<double> >::const_iterator>::constraints ();
        MutableIndexedRandomAccess1DIterator<matrix_vector_range<matrix<double> >::iterator>::constraints ();
        IndexedRandomAccess1DIterator<matrix_vector_range<matrix<double> >::const_reverse_iterator>::constraints ();
        MutableIndexedRandomAccess1DIterator<matrix_vector_range<matrix<double> >::reverse_iterator>::constraints ();

        VectorExpression<const matrix_vector_slice<const matrix<double> > >::constraints ();
        MutableVectorExpression<matrix_vector_slice<matrix<double> > >::constraints ();
        IndexedRandomAccess1DIterator<matrix_vector_slice<matrix<double> >::const_iterator>::constraints ();
        MutableIndexedRandomAccess1DIterator<matrix_vector_slice<matrix<double> >::iterator>::constraints ();
        IndexedRandomAccess1DIterator<matrix_vector_slice<matrix<double> >::const_reverse_iterator>::constraints ();
        MutableIndexedRandomAccess1DIterator<matrix_vector_slice<matrix<double> >::reverse_iterator>::constraints ();

        VectorExpression<const matrix_vector_indirect<const matrix<double> > >::constraints ();
        MutableVectorExpression<matrix_vector_indirect<matrix<double> > >::constraints ();
        IndexedRandomAccess1DIterator<matrix_vector_indirect<matrix<double> >::const_iterator>::constraints ();
        MutableIndexedRandomAccess1DIterator<matrix_vector_indirect<matrix<double> >::iterator>::constraints ();
        IndexedRandomAccess1DIterator<matrix_vector_indirect<matrix<double> >::const_reverse_iterator>::constraints ();
        MutableIndexedRandomAccess1DIterator<matrix_vector_indirect<matrix<double> >::reverse_iterator>::constraints ();
#endif

        MatrixExpression<const matrix_range<const matrix<double> > >::constraints ();
        MutableMatrixExpression<matrix_range<matrix<double> > >::constraints ();
        IndexedRandomAccess2DIterator<matrix_range<matrix<double> >::const_iterator1,
                                      matrix_range<matrix<double> >::const_iterator2>::constraints ();
        MutableIndexedRandomAccess2DIterator<matrix_range<matrix<double> >::iterator1,
                                             matrix_range<matrix<double> >::iterator2>::constraints ();
        IndexedRandomAccess2DIterator<matrix_range<matrix<double> >::const_reverse_iterator1,
                                      matrix_range<matrix<double> >::const_reverse_iterator2>::constraints ();
        MutableIndexedRandomAccess2DIterator<matrix_range<matrix<double> >::reverse_iterator1,
                                             matrix_range<matrix<double> >::reverse_iterator2>::constraints ();

        MatrixExpression<const matrix_slice<const matrix<double> > >::constraints ();
        MutableMatrixExpression<matrix_slice<matrix<double> > >::constraints ();
        IndexedRandomAccess2DIterator<matrix_slice<matrix<double> >::const_iterator1,
                                      matrix_slice<matrix<double> >::const_iterator2>::constraints ();
        MutableIndexedRandomAccess2DIterator<matrix_slice<matrix<double> >::iterator1,
                                             matrix_slice<matrix<double> >::iterator2>::constraints ();
        IndexedRandomAccess2DIterator<matrix_slice<matrix<double> >::const_reverse_iterator1,
                                      matrix_slice<matrix<double> >::const_reverse_iterator2>::constraints ();
        MutableIndexedRandomAccess2DIterator<matrix_slice<matrix<double> >::reverse_iterator1,
                                             matrix_slice<matrix<double> >::reverse_iterator2>::constraints ();

        MatrixExpression<const matrix_indirect<const matrix<double> > >::constraints ();
        MutableMatrixExpression<matrix_indirect<matrix<double> > >::constraints ();
        IndexedRandomAccess2DIterator<matrix_indirect<matrix<double> >::const_iterator1,
                                      matrix_indirect<matrix<double> >::const_iterator2>::constraints ();
        MutableIndexedRandomAccess2DIterator<matrix_indirect<matrix<double> >::iterator1,
                                             matrix_indirect<matrix<double> >::iterator2>::constraints ();
        IndexedRandomAccess2DIterator<matrix_indirect<matrix<double> >::const_reverse_iterator1,
                                      matrix_indirect<matrix<double> >::const_reverse_iterator2>::constraints ();
        MutableIndexedRandomAccess2DIterator<matrix_indirect<matrix<double> >::reverse_iterator1,
                                             matrix_indirect<matrix<double> >::reverse_iterator2>::constraints ();
#endif

        // Banded Matrix
#if defined (INTERNAL) || defined (INTERNAL_BANDED)
        Matrix<const banded_matrix<double> >::constraints ();
        MutableMatrix<banded_matrix<double> >::constraints ();
        IndexedBidirectional2DIterator<banded_matrix<double>::const_iterator1,
                                       banded_matrix<double>::const_iterator2>::constraints ();
        MutableIndexedBidirectional2DIterator<banded_matrix<double>::iterator1,
                                              banded_matrix<double>::iterator2>::constraints ();
        IndexedBidirectional2DIterator<banded_matrix<double>::const_reverse_iterator1,
                                       banded_matrix<double>::const_reverse_iterator2>::constraints ();
        MutableIndexedBidirectional2DIterator<banded_matrix<double>::reverse_iterator1,
                                              banded_matrix<double>::reverse_iterator2>::constraints ();

        MatrixExpression<const banded_adaptor<const matrix<double> > >::constraints ();
        MutableMatrixExpression<banded_adaptor<matrix<double> > >::constraints ();
        IndexedBidirectional2DIterator<banded_adaptor<matrix<double> >::const_iterator1,
                                       banded_adaptor<matrix<double> >::const_iterator2>::constraints ();
        MutableIndexedBidirectional2DIterator<banded_adaptor<matrix<double> >::iterator1,
                                              banded_adaptor<matrix<double> >::iterator2>::constraints ();
        IndexedBidirectional2DIterator<banded_adaptor<matrix<double> >::const_reverse_iterator1,
                                       banded_adaptor<matrix<double> >::const_reverse_iterator2>::constraints ();
        MutableIndexedBidirectional2DIterator<banded_adaptor<matrix<double> >::reverse_iterator1,
                                              banded_adaptor<matrix<double> >::reverse_iterator2>::constraints ();
#endif

        // Triangular Matrix
#if defined (INTERNAL) || defined (INTERNAL_TRIANGULAR)
        Matrix<const triangular_matrix<double> >::constraints ();
        MutableMatrix<triangular_matrix<double> >::constraints ();
        IndexedBidirectional2DIterator<triangular_matrix<double>::const_iterator1,
                                       triangular_matrix<double>::const_iterator2>::constraints ();
        MutableIndexedBidirectional2DIterator<triangular_matrix<double>::iterator1,
                                              triangular_matrix<double>::iterator2>::constraints ();
        IndexedBidirectional2DIterator<triangular_matrix<double>::const_reverse_iterator1, 
                                       triangular_matrix<double>::const_reverse_iterator2>::constraints ();
        MutableIndexedBidirectional2DIterator<triangular_matrix<double>::reverse_iterator1, 
                                              triangular_matrix<double>::reverse_iterator2>::constraints ();

        MatrixExpression<const triangular_adaptor<const matrix<double> > >::constraints ();
        MutableMatrixExpression<triangular_adaptor<matrix<double> > >::constraints ();
        IndexedBidirectional2DIterator<triangular_adaptor<matrix<double> >::const_iterator1, 
                                       triangular_adaptor<matrix<double> >::const_iterator2>::constraints ();
        MutableIndexedBidirectional2DIterator<triangular_adaptor<matrix<double> >::iterator1, 
                                              triangular_adaptor<matrix<double> >::iterator2>::constraints ();
        IndexedBidirectional2DIterator<triangular_adaptor<matrix<double> >::const_reverse_iterator1,
                                       triangular_adaptor<matrix<double> >::const_reverse_iterator2>::constraints ();
        MutableIndexedBidirectional2DIterator<triangular_adaptor<matrix<double> >::reverse_iterator1, 
                                              triangular_adaptor<matrix<double> >::reverse_iterator2>::constraints ();
#endif

        // Symmetric Matrix
#if defined (INTERNAL) || defined (INTERNAL_SYMMETRIC)
        Matrix<const symmetric_matrix<double> >::constraints ();
        MutableMatrix<symmetric_matrix<double> >::constraints ();
        IndexedBidirectional2DIterator<symmetric_matrix<double>::const_iterator1,
                                       symmetric_matrix<double>::const_iterator2>::constraints ();
        MutableIndexedBidirectional2DIterator<symmetric_matrix<double>::iterator1,
                                              symmetric_matrix<double>::iterator2>::constraints ();
        IndexedBidirectional2DIterator<symmetric_matrix<double>::const_reverse_iterator1,
                                       symmetric_matrix<double>::const_reverse_iterator2>::constraints ();
        MutableIndexedBidirectional2DIterator<symmetric_matrix<double>::reverse_iterator1,
                                              symmetric_matrix<double>::reverse_iterator2>::constraints ();

        MatrixExpression<const symmetric_adaptor<const matrix<double> > >::constraints ();
        MutableMatrixExpression<symmetric_adaptor<matrix<double> > >::constraints ();
        IndexedBidirectional2DIterator<symmetric_adaptor<matrix<double> >::const_iterator1,
                                       symmetric_adaptor<matrix<double> >::const_iterator2>::constraints ();
        MutableIndexedBidirectional2DIterator<symmetric_adaptor<matrix<double> >::iterator1,
                                              symmetric_adaptor<matrix<double> >::iterator2>::constraints ();
        IndexedBidirectional2DIterator<symmetric_adaptor<matrix<double> >::const_reverse_iterator1, 
                                       symmetric_adaptor<matrix<double> >::const_reverse_iterator2>::constraints ();
        MutableIndexedBidirectional2DIterator<symmetric_adaptor<matrix<double> >::reverse_iterator1, 
                                              symmetric_adaptor<matrix<double> >::reverse_iterator2>::constraints ();
#endif

        // Hermitean Matrix
#if defined (INTERNAL) || defined (INTERNAL_HERMITEAN)
        Matrix<const hermitean_matrix<double> >::constraints ();
        MutableMatrix<hermitean_matrix<double> >::constraints ();
        IndexedBidirectional2DIterator<hermitean_matrix<double>::const_iterator1,
                                       hermitean_matrix<double>::const_iterator2>::constraints ();
        MutableIndexedBidirectional2DIterator<hermitean_matrix<double>::iterator1,
                                              hermitean_matrix<double>::iterator2>::constraints ();
        IndexedBidirectional2DIterator<hermitean_matrix<double>::const_reverse_iterator1,
                                       hermitean_matrix<double>::const_reverse_iterator2>::constraints ();
        MutableIndexedBidirectional2DIterator<hermitean_matrix<double>::reverse_iterator1,
                                              hermitean_matrix<double>::reverse_iterator2>::constraints ();

        MatrixExpression<const hermitean_adaptor<const matrix<double> > >::constraints ();
        MutableMatrixExpression<hermitean_adaptor<matrix<double> > >::constraints ();
        IndexedBidirectional2DIterator<hermitean_adaptor<matrix<double> >::const_iterator1,
                                       hermitean_adaptor<matrix<double> >::const_iterator2>::constraints ();
        MutableIndexedBidirectional2DIterator<hermitean_adaptor<matrix<double> >::iterator1,
                                              hermitean_adaptor<matrix<double> >::iterator2>::constraints ();
        IndexedBidirectional2DIterator<hermitean_adaptor<matrix<double> >::const_reverse_iterator1,
                                       hermitean_adaptor<matrix<double> >::const_reverse_iterator2>::constraints ();
        MutableIndexedBidirectional2DIterator<hermitean_adaptor<matrix<double> >::reverse_iterator1,
                                              hermitean_adaptor<matrix<double> >::reverse_iterator2>::constraints ();
#endif

        // Sparse Matrix
#if defined (INTERNAL) || defined (INTERNAL_MATRIX_SPARSE)
        Matrix<const sparse_matrix<double> >::constraints ();
        MutableMatrix<sparse_matrix<double> >::constraints ();
        IndexedBidirectional2DIterator<sparse_matrix<double>::const_iterator1, 
                                       sparse_matrix<double>::const_iterator2>::constraints ();
        MutableIndexedBidirectional2DIterator<sparse_matrix<double>::iterator1, 
                                              sparse_matrix<double>::iterator2>::constraints ();
        IndexedBidirectional2DIterator<sparse_matrix<double>::const_reverse_iterator1, 
                                       sparse_matrix<double>::const_reverse_iterator2>::constraints ();
        MutableIndexedBidirectional2DIterator<sparse_matrix<double>::reverse_iterator1, 
                                              sparse_matrix<double>::reverse_iterator2>::constraints ();

        Matrix<const sparse_vector_of_sparse_vector<double> >::constraints ();
        MutableMatrix<sparse_vector_of_sparse_vector<double> >::constraints ();
        IndexedBidirectional2DIterator<sparse_vector_of_sparse_vector<double>::const_iterator1, 
                                       sparse_vector_of_sparse_vector<double>::const_iterator2>::constraints ();
        MutableIndexedBidirectional2DIterator<sparse_vector_of_sparse_vector<double>::iterator1, 
                                              sparse_vector_of_sparse_vector<double>::iterator2>::constraints ();
        IndexedBidirectional2DIterator<sparse_vector_of_sparse_vector<double>::const_reverse_iterator1, 
                                       sparse_vector_of_sparse_vector<double>::const_reverse_iterator2>::constraints ();
        MutableIndexedBidirectional2DIterator<sparse_vector_of_sparse_vector<double>::reverse_iterator1, 
                                              sparse_vector_of_sparse_vector<double>::reverse_iterator2>::constraints ();

        Matrix<const compressed_matrix<double> >::constraints ();
        MutableMatrix<compressed_matrix<double> >::constraints ();
        IndexedBidirectional2DIterator<compressed_matrix<double>::const_iterator1,
                                       compressed_matrix<double>::const_iterator2>::constraints ();
        MutableIndexedBidirectional2DIterator<compressed_matrix<double>::iterator1, 
                                              compressed_matrix<double>::iterator2>::constraints ();
        IndexedBidirectional2DIterator<compressed_matrix<double>::const_reverse_iterator1, 
                                       compressed_matrix<double>::const_reverse_iterator2>::constraints ();
        MutableIndexedBidirectional2DIterator<compressed_matrix<double>::reverse_iterator1, 
                                              compressed_matrix<double>::reverse_iterator2>::constraints ();
#endif

        // Scalar Expressions
#if defined (INTERNAL) || defined (INTERNAL_VECTOR_EXPRESSION)
        ScalarExpression<scalar_value<double > >::constraints ();
        ScalarExpression<scalar_const_reference<double > >::constraints ();

        // Vector Expressions
        VectorExpression<vector_const_reference<vector<double> > >::constraints ();
        IndexedRandomAccess1DIterator<vector_const_reference<vector<double> >::const_iterator>::constraints ();
        IndexedRandomAccess1DIterator<vector_const_reference<vector<double> >::const_reverse_iterator>::constraints ();

        VectorExpression<vector_reference<vector<double> > >::constraints ();
        // MutableVectorExpression<vector_reference<vector<double> > >::constraints ();
        IndexedRandomAccess1DIterator<vector_reference<vector<double> >::const_iterator>::constraints ();
        MutableIndexedRandomAccess1DIterator<vector_reference<vector<double> >::iterator>::constraints ();
        IndexedRandomAccess1DIterator<vector_reference<vector<double> >::const_reverse_iterator>::constraints ();
        MutableIndexedRandomAccess1DIterator<vector_reference<vector<double> >::reverse_iterator>::constraints ();

        VectorExpression<vector_unary<vector<double>, scalar_identity<double> > >::constraints ();
        IndexedRandomAccess1DIterator<vector_unary<vector<double>, scalar_identity<double>  >::const_iterator>::constraints ();
        IndexedRandomAccess1DIterator<vector_unary<vector<double>, scalar_identity<double>  >::const_reverse_iterator>::constraints ();

        VectorExpression<vector_binary<vector<double>, vector<double>, scalar_plus<double, double> > >::constraints ();
        IndexedRandomAccess1DIterator<vector_binary<vector<double>, vector<double>, scalar_plus<double, double> >::const_iterator>::constraints ();
        IndexedRandomAccess1DIterator<vector_binary<vector<double>, vector<double>, scalar_plus<double, double> >::const_reverse_iterator>::constraints ();

        VectorExpression<vector_binary_scalar1<scalar_value<double>, vector<double>, scalar_multiplies<double, double> > >::constraints ();
        IndexedRandomAccess1DIterator<vector_binary_scalar1<scalar_value<double>, vector<double>, scalar_multiplies<double, double> >::const_iterator>::constraints ();
        IndexedRandomAccess1DIterator<vector_binary_scalar1<scalar_value<double>, vector<double>, scalar_multiplies<double, double> >::const_reverse_iterator>::constraints ();

        VectorExpression<vector_binary_scalar2<vector<double>, scalar_value<double>, scalar_multiplies<double, double> > >::constraints ();
        IndexedRandomAccess1DIterator<vector_binary_scalar2<vector<double>, scalar_value<double>, scalar_multiplies<double, double> >::const_iterator>::constraints ();
        IndexedRandomAccess1DIterator<vector_binary_scalar2<vector<double>, scalar_value<double>, scalar_multiplies<double, double> >::const_reverse_iterator>::constraints ();

        ScalarExpression<vector_scalar_unary<vector<double>, vector_sum<double> > >::constraints ();
        ScalarExpression<vector_scalar_unary<vector<double>, vector_norm_1<double> > >::constraints ();
        ScalarExpression<vector_scalar_unary<vector<double>, vector_norm_2<double> > >::constraints ();
        ScalarExpression<vector_scalar_unary<vector<double>, vector_norm_inf<double> > >::constraints ();

        ScalarExpression<vector_scalar_binary<vector<double>, vector<double>, vector_inner_prod<double, double, double> > >::constraints ();
#endif

        // Matrix Expressions
#if defined (INTERNAL) || defined (INTERNAL_MATRIX_EXPRESSION)
        MatrixExpression<matrix_const_reference<matrix<double> > >::constraints ();
        IndexedRandomAccess2DIterator<matrix_const_reference<matrix<double> >::const_iterator1,
                                      matrix_const_reference<matrix<double> >::const_iterator2>::constraints ();
        IndexedRandomAccess2DIterator<matrix_const_reference<matrix<double> >::const_reverse_iterator1,
                                      matrix_const_reference<matrix<double> >::const_reverse_iterator2>::constraints ();

        MatrixExpression<matrix_reference<matrix<double> > >::constraints ();
        // MutableMatrixExpression<matrix_reference<matrix<double> > >::constraints ();
        IndexedRandomAccess2DIterator<matrix_reference<matrix<double> >::const_iterator1,
                                      matrix_reference<matrix<double> >::const_iterator2>::constraints ();
        MutableIndexedRandomAccess2DIterator<matrix_reference<matrix<double> >::iterator1,
                                             matrix_reference<matrix<double> >::iterator2>::constraints ();
        IndexedRandomAccess2DIterator<matrix_reference<matrix<double> >::const_reverse_iterator1,
                                      matrix_reference<matrix<double> >::const_reverse_iterator2>::constraints ();
        MutableIndexedRandomAccess2DIterator<matrix_reference<matrix<double> >::reverse_iterator1,
                                             matrix_reference<matrix<double> >::reverse_iterator2>::constraints ();

        MatrixExpression<vector_matrix_binary<vector<double>, vector<double>, scalar_multiplies<double, double> > >::constraints ();
        IndexedRandomAccess2DIterator<vector_matrix_binary<vector<double>, vector<double>, scalar_multiplies<double, double> >::const_iterator1,
                                      vector_matrix_binary<vector<double>, vector<double>, scalar_multiplies<double, double> >::const_iterator2>::constraints ();
        IndexedRandomAccess2DIterator<vector_matrix_binary<vector<double>, vector<double>, scalar_multiplies<double, double> >::const_reverse_iterator1,
                                      vector_matrix_binary<vector<double>, vector<double>, scalar_multiplies<double, double> >::const_reverse_iterator2>::constraints ();

        MatrixExpression<matrix_unary1<matrix<double>, scalar_identity<double> > >::constraints ();
        IndexedRandomAccess2DIterator<matrix_unary1<matrix<double>, scalar_identity<double> >::const_iterator1,
                                      matrix_unary1<matrix<double>, scalar_identity<double> >::const_iterator2>::constraints ();
        IndexedRandomAccess2DIterator<matrix_unary1<matrix<double>, scalar_identity<double> >::const_reverse_iterator1,
                                      matrix_unary1<matrix<double>, scalar_identity<double> >::const_reverse_iterator2>::constraints ();

        MatrixExpression<matrix_unary2<matrix<double>, scalar_identity<double> > >::constraints ();
        IndexedRandomAccess2DIterator<matrix_unary2<matrix<double>, scalar_identity<double> >::const_iterator1,
                                      matrix_unary2<matrix<double>, scalar_identity<double> >::const_iterator2>::constraints ();
        IndexedRandomAccess2DIterator<matrix_unary2<matrix<double>, scalar_identity<double> >::const_reverse_iterator1,
                                      matrix_unary2<matrix<double>, scalar_identity<double> >::const_reverse_iterator2>::constraints ();

        MatrixExpression<matrix_binary<matrix<double>, matrix<double>, scalar_plus<double, double> > >::constraints ();
        IndexedRandomAccess2DIterator<matrix_binary<matrix<double>, matrix<double>, scalar_plus<double, double> >::const_iterator1,
                                      matrix_binary<matrix<double>, matrix<double>, scalar_plus<double, double> >::const_iterator2>::constraints ();
        IndexedRandomAccess2DIterator<matrix_binary<matrix<double>, matrix<double>, scalar_plus<double, double> >::const_reverse_iterator1,
                                      matrix_binary<matrix<double>, matrix<double>, scalar_plus<double, double> >::const_reverse_iterator2>::constraints ();

        MatrixExpression<matrix_binary_scalar1<scalar_value<double>, matrix<double>, scalar_multiplies<double, double> > >::constraints ();
        IndexedRandomAccess2DIterator<matrix_binary_scalar1<scalar_value<double>, matrix<double>, scalar_multiplies<double, double> >::const_iterator1,
                                      matrix_binary_scalar1<scalar_value<double>, matrix<double>, scalar_multiplies<double, double> >::const_iterator2>::constraints ();
        IndexedRandomAccess2DIterator<matrix_binary_scalar1<scalar_value<double>, matrix<double>, scalar_multiplies<double, double> >::const_reverse_iterator1,
                                      matrix_binary_scalar1<scalar_value<double>, matrix<double>, scalar_multiplies<double, double> >::const_reverse_iterator2>::constraints ();

        MatrixExpression<matrix_binary_scalar2<matrix<double>, scalar_value<double>, scalar_multiplies<double, double> > >::constraints ();
        IndexedRandomAccess2DIterator<matrix_binary_scalar2<matrix<double>, scalar_value<double>, scalar_multiplies<double, double> >::const_iterator1,
                                      matrix_binary_scalar2<matrix<double>, scalar_value<double>, scalar_multiplies<double, double> >::const_iterator2>::constraints ();
        IndexedRandomAccess2DIterator<matrix_binary_scalar2<matrix<double>, scalar_value<double>, scalar_multiplies<double, double> >::const_reverse_iterator1,
                                      matrix_binary_scalar2<matrix<double>, scalar_value<double>, scalar_multiplies<double, double> >::const_reverse_iterator2>::constraints ();

        VectorExpression<matrix_vector_binary1<matrix<double>, vector<double>, matrix_vector_prod1<double, double, double> > >::constraints ();
        IndexedRandomAccess1DIterator<matrix_vector_binary1<matrix<double>, vector<double>, matrix_vector_prod1<double, double, double> >::const_iterator>::constraints ();
        IndexedRandomAccess1DIterator<matrix_vector_binary1<matrix<double>, vector<double>, matrix_vector_prod1<double, double, double> >::const_reverse_iterator>::constraints ();

        VectorExpression<matrix_vector_binary2<vector<double>, matrix<double>, matrix_vector_prod2<double, double, double> > >::constraints ();
        IndexedRandomAccess1DIterator<matrix_vector_binary2<vector<double>, matrix<double>, matrix_vector_prod2<double, double, double> >::const_iterator>::constraints ();
        IndexedRandomAccess1DIterator<matrix_vector_binary2<vector<double>, matrix<double>, matrix_vector_prod2<double, double, double> >::const_reverse_iterator>::constraints ();

        MatrixExpression<matrix_matrix_binary<matrix<double>, matrix<double>, matrix_matrix_prod<double, double, double> > >::constraints ();
        IndexedRandomAccess2DIterator<matrix_matrix_binary<matrix<double>, matrix<double>, matrix_matrix_prod<double, double, double> >::const_iterator1,
                                      matrix_matrix_binary<matrix<double>, matrix<double>, matrix_matrix_prod<double, double, double> >::const_iterator2>::constraints ();
        IndexedRandomAccess2DIterator<matrix_matrix_binary<matrix<double>, matrix<double>, matrix_matrix_prod<double, double, double> >::const_reverse_iterator1,
                                      matrix_matrix_binary<matrix<double>, matrix<double>, matrix_matrix_prod<double, double, double> >::const_reverse_iterator2>::constraints ();

        ScalarExpression<matrix_scalar_unary<matrix<double>, matrix_norm_1<double> > >::constraints ();
        ScalarExpression<matrix_scalar_unary<matrix<double>, matrix_norm_2<double> > >::constraints ();
        ScalarExpression<matrix_scalar_unary<matrix<double>, matrix_norm_inf<double> > >::constraints ();
#endif

#ifdef EXTERNAL
        AdditiveAbelianGroup<float>::constraints ();
        CommutativeRingWithIdentity<float>::constraints ();
        Field<float>::constraints ();
        VectorSpace<float, vector<float> >::constraints ();
        RingWithIdentity<matrix<float> >::constraints (0);
        VectorSpace<float, matrix<float> >::constraints ();
        LinearOperator<float, vector<float>, matrix<float> >::constraints ();

        AdditiveAbelianGroup<double>::constraints ();
        CommutativeRingWithIdentity<double>::constraints ();
        Field<double>::constraints ();
        VectorSpace<double, vector<double> >::constraints ();
        RingWithIdentity<matrix<double> >::constraints (0);
        VectorSpace<double, matrix<double> >::constraints ();
        LinearOperator<double, vector<double>, matrix<double> >::constraints ();

        AdditiveAbelianGroup<std::complex<float> >::constraints ();
        CommutativeRingWithIdentity<std::complex<float> >::constraints ();
        Field<std::complex<float> >::constraints ();
        VectorSpace<std::complex<float>, vector<std::complex<float> > >::constraints ();
        RingWithIdentity<matrix<std::complex<float> > >::constraints (0);
        VectorSpace<std::complex<float>, matrix<std::complex<float> > >::constraints ();
        LinearOperator<std::complex<float>, vector<std::complex<float> >, matrix<std::complex<float> > >::constraints ();

        AdditiveAbelianGroup<std::complex<double> >::constraints ();
        CommutativeRingWithIdentity<std::complex<double> >::constraints ();
        Field<std::complex<double> >::constraints ();
        VectorSpace<std::complex<double>, vector<std::complex<double> > >::constraints ();
        RingWithIdentity<matrix<std::complex<double> > >::constraints (0);
        VectorSpace<std::complex<double>, matrix<std::complex<double> > >::constraints ();
        LinearOperator<std::complex<double>, vector<std::complex<double> >, matrix<std::complex<double> > >::constraints ();
#endif
    }

}}

#endif



