//=======================================================================
// Copyright 2001 University of Notre Dame.
// Authors: Jeremy G. Siek, Andrew Lumsdaine
//
// This file is part of the Matrix Template Library
//
// You should have received a copy of the License Agreement for the
// Matrix Template Library along with the software; see the file LICENSE.
// If not, contact Office of Research, University of Notre Dame, Notre
// Dame, IN 46556.
//
// Permission to modify the code and to distribute modified code is
// granted, provided the text of this NOTICE is retained, a notice that
// the code was modified is included with the above COPYRIGHT NOTICE and
// with the COPYRIGHT NOTICE in the LICENSE file, and that the LICENSE
// file is distributed with the modified code.
//
// LICENSOR MAKES NO REPRESENTATIONS OR WARRANTIES, EXPRESS OR IMPLIED.
// By way of example, but not limitation, Licensor MAKES NO
// REPRESENTATIONS OR WARRANTIES OF MERCHANTABILITY OR FITNESS FOR ANY
// PARTICULAR PURPOSE OR THAT THE USE OF THE LICENSED SOFTWARE COMPONENTS
// OR DOCUMENTATION WILL NOT INFRINGE ANY PATENTS, COPYRIGHTS, TRADEMARKS
// OR OTHER RIGHTS.
//=======================================================================

#ifndef MTL_ALGOBASE_HPP
#define MTL_ALGOBASE_HPP

#include <mtl/vector_fwd.hpp>
#include <mtl/op_functors.hpp>

namespace detail {

  // No efficient way to implement diag() for some sparse matrices :(
  template <class SparseMatrix, class Vector>
  inline void
  copy_diagonal(const SparseMatrix& A, Vector& diag)
  {
    typedef typename SparseMatrix::value_type::const_iterator iter1D;
    typename SparseMatrix::size_type k = 0;
    for (typename SparseMatrix::const_iterator i = A.begin();
	 i != A.end(); ++i, ++k)
      for (iter1D j = (*i).begin(); j != (*i).end(); ++j)
	if (index(*j) == k) {
	  diag[k] = value(*j);
	  break;
	}
  }

  // Note: must initialize result to zero if that's what you want
  // as starting value
  
  template <class RangeA, class RangeB, class size_type, class T, 
            class AssignOp, class BinaryOp, class Combine>
  void inner_product(RangeA a, RangeB b, size_type n, T& result, dense, dense,
		     AssignOp assign_op, BinaryOp binary_op, Combine)
  {
    for(; !empty(a); ++a, ++b)
      assign_op(result, binary_op(*a, *b));
  }

  template <class RangeA, class RangeB, class size_type, class T, 
            class AssignOp, class BinaryOp>
  void inner_product(RangeA a, RangeB b, size_type, T& result, 
		     sparse, sparse,
		     AssignOp assign_op, BinaryOp binary_op, and_op)
  {
#if 1
    for(; !empty(a); ++a) {
      while (!empty(b) && index(*b) < index(*a)) 
        ++b;
      if (index(*a) == index(*b))
        assign_op(result, binary_op(value(*a), value(*b)));
    }
#else
    // This version should be faster
    typename mtl::vector<T>::type work(n);
    for(; !empty(a); ++a)
      work[index(*a)] = value(*a);
    for(; !empty(b); ++b) {
      T w = work[index(*b)];
      if (w != T())
	assign_op(result, binary_op(w, value(*b)));
    }
#endif
  }

  template <class RangeA, class RangeB, class size_type, class T, 
            class AssignOp, class BinaryOp>
  void inner_product(RangeA a, RangeB b, size_type, T& result, 
		     sparse, sparse,
		     AssignOp assign_op, BinaryOp binary_op, or_op)
  {
    //Hmm, or should these be identity elements for the binary op?
    typename range_traits<RangeA>::value_type A_zero;
    typename range_traits<RangeB>::value_type B_zero;
    while (!empty(a) || !empty(b)) {
      for (; !empty(a) && index(*a) < index(*b); ++a)
        assign_op(result, binary_op(value(*a), B_zero));

      for (; !empty(b) && index(*b) < index(*a); ++b)
        assign_op(result, binary_op(A_zero, value(*b)));
	
      if (!empty(a) && !empty(b) && (index(*a) == index(*b)))
        assign_op(result, binary_op(value(*a), value(*b)));
    }
  }

  template <class RangeA, class RangeB, class size_type, class T, 
            class AssignOp, class BinaryOp>
  void inner_product(RangeA a, RangeB b, size_type, T& result, dense, sparse,
		     AssignOp assign_op, BinaryOp binary_op, and_op)
  {
    for(; !empty(b); ++b)
      assign_op(result, binary_op(a[index(*b)], value(*b)));
  }

  template <class RangeA, class RangeB, class size_type, class T, 
            class AssignOp, class BinaryOp>
  void inner_product(RangeA a, RangeB b, size_type, T& result, sparse, dense,
		     AssignOp assign_op, BinaryOp binary_op, and_op)
  {
    for(; !empty(a); ++a)
      assign_op(result, binary_op(value(*a), b[index(*a)]));
  }

  template <class RangeA, class RangeB, class size_type, class T, 
            class LS, class DS,
            class AssignOp, class BinaryOp>
  void inner_product(RangeA a, RangeB b, size_type n, T& result, LS ls, DS ds,
		     AssignOp assign_op, BinaryOp binary_op)
  {
    typedef typename BinaryOp::index_combine Combine;
    inner_product(a, b, n, result, ls, ds, assign_op, binary_op, Combine());
  }

  template <class RangeA, class RangeB, class size_type, class T, 
            class RS, class LS>
  void inner_product(RangeA a, RangeB b, size_type n, T& result, RS rs, LS ls)
  {
    inner_product(a, b, n, result, rs, ls, op_assign_add(), op_multiply());
  }
  
} // namespace detail

#endif // MTL_ALGOBASE_HPP
