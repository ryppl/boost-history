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

#ifndef MTL_EVAL_EXPR_HPP
#define MTL_EVAL_EXPR_HPP

#include <vector>
#include <mtl/tags.hpp>

  template <class Result, class RHS_Range, class AssignOp, class RHS_Sparsity>
  inline void
  eval_expr(Result& result, RHS_Range& r, AssignOp assign, RHS_Sparsity) {
    typedef typename Result::sparsity LHS_Sparsity;
    eval_dispatch(result, r, assign, LHS_Sparsity(), RHS_Sparsity());
  }

  // dense OP= dense

  template <class Result, class RHS_Range, class AssignOp>
  inline void
  eval_dispatch(Result& result, RHS_Range& r, 
		AssignOp assign, dense, dense) 
  {
    typename Result::iterator out = result.begin();
#if 0
    // this is slower with g++
    for(; !empty(r); ++r, ++out)
      assign(*out, *r);
#elif 1
    // this is faster with KCC and g++, by about 30%
    // but this doesn't work in the case where the RHS
    // is a (sparse + dense) expression, because that
    // returns dense, but doesn't have operator[]!
    // Need to differentiate between "dense without operator[]"
    // and "dense with operator[]".
    typedef typename Result::size_type size_type;
    size_type n = result.size();
    for (size_type i = 0; i != n; ++i)
      assign(out[i], r[i]);
#else
    // experimenting with unrolling
    typedef typename Result::size_type size_type;
    size_type n = result.size();
    size_type mod = n % 4;
    size_type i;
    for (i = 0; i != mod; ++i)
      assign(out[i], r[i]);
    for (; i != n; i += 4) {
      assign(out[i], r[i]);
      assign(out[i+1], r[i+1]);
      assign(out[i+2], r[i+2]);
      assign(out[i+3], r[i+3]);
    }
#endif
  }

  // dense = sparse

  template <class Result, class RHS_Range>
  inline void
  eval_dispatch(Result& result, RHS_Range& r, 
		op_assign assign, dense, sparse) 
  {
    typename Result::iterator out = result.begin();
    typedef typename Result::value_type R_value;
    result = R_value(); // zero out the result
    for(; !empty(r); ++r)
      out[index(*r)] = value(*r);
  }

  // sparse = sparse

  template <class Result, class RHS_Range>
  inline void eval_dispatch(Result& result, RHS_Range& r,
			    op_assign assign, sparse, sparse) 
  {
    result.clear();
    for(; !empty(r); ++r)
      result.push_back(*r);
  }

  // dense += sparse

  template <class Result, class RHS_Range>
  inline void eval_dispatch(Result& result, RHS_Range& r,
			    op_assign_add, dense, sparse) 
  {
    typename Result::iterator out = result.begin();
    for(; !empty(r); ++r)
      out[index(*r)] += value(*r);
  }

  // sparse += sparse

  template <class Result, class RHS_Range>
  inline void eval_dispatch(Result& result, RHS_Range& r,
			    op_assign_add, sparse, sparse) 
  {
    typedef typename Result::size_type size_type;
    typedef typename Result::value_type Pair;
    typedef typename Pair::second_type R_value;

    std::vector<Pair> tmp;
    typename Result::iterator out = result.begin(), out_end = result.end();
    for(; !empty(r) && out != out_end;) {
      if (size_type(index(*r)) == index(*out)) {
        tmp.push_back( Pair(index(*r), value(*out) + value(*r)) );
        ++r; ++out;
      } else if (size_type(index(*r)) < index(*out)) {
        tmp.push_back(*r);
        ++r;
      } else {
        tmp.push_back(*out);
        ++out;
      }
    }
    for(; !empty(r); ++r) 
      tmp.push_back(*r);
    for(; out != out_end; ++out)
      tmp.push_back(*out);

    // would be better to swap...
    result.clear();
    for (typename std::vector<Pair>::iterator i = tmp.begin(); 
         i != tmp.end(); ++i)
      result.push_back(*i);
  }

  // static_sparse means that the index structure of the
  // sparse vector is fixed, and the operation only applies
  // to the stored (usually non-zero) elements.

  // static_sparse op= sparse
  template <class Result, class RHS_Range, class OpAssign>
  inline void eval_dispatch(Result& result, RHS_Range& r,
			    OpAssign assign_op, static_sparse, sparse) 
  {
    typename Result::iterator yi;
    for (yi = result.begin(); yi != result.end(); ++yi)
      for (; !empty(r) && index(*r) <= index(*yi); ++r)
        assign_op(*yi, *r);
  }

  // static_sparse += sparse
  template <class Result, class RHS_Range>
  inline void eval_dispatch(Result& result, RHS_Range& r,
			    op_assign_add, static_sparse, sparse) 
  {
    typename Result::iterator yi;
    for (yi = result.begin(); yi != result.end(); ++yi) {
      while (!empty(r) && index(*r) < index(*yi))
        ++r;
      if (index(*r) == index(*yi))
        (*yi).second += value(*r);
    }
  }

  // static_sparse -= sparse
  template <class Result, class RHS_Range>
  inline void eval_dispatch(Result& result, RHS_Range& r,
			    op_assign_subtract, static_sparse, sparse) 
  {
    typename Result::iterator yi;
    for (yi = result.begin(); yi != result.end(); ++yi) {
      while (!empty(r) && index(*r) < index(*yi))
        ++r;
      if (index(*r) == index(*yi))
        (*yi).second -= value(*r);
    }
  }

  // dense -= sparse
  template <class Result, class RHS_Range>
  inline void eval_dispatch(Result& result, RHS_Range& r,
			    op_assign_subtract, dense, sparse) 
  {
    typename Result::iterator out = result.begin();
    for(; !empty(r); ++r)
      out[index(*r)] -= value(*r);
  }

  // sparse -= sparse  

  template <class Result, class RHS_Range>
  inline void eval_dispatch(Result& result, RHS_Range& r,
			    op_assign_subtract, sparse, sparse)
  {
    typedef typename Result::value_type Pair;
    typedef typename Pair::second_type R_value;

    std::vector<Pair> tmp;
    typename Result::iterator out = result.begin(), out_end = result.end();
    for(; !empty(r) && out != out_end;) {
      if (index(*r) == index(*out)) {
        tmp.push_back( Pair(index(*r), value(*out) - value(*r)) );
        ++r; ++out;
      } else if (index(*r) < index(*out)) {
        tmp.push_back(Pair(index(*r), -value(*r)));
        ++r;
      } else {
        tmp.push_back(*out);
        ++out;
      }
    }
    for(; !empty(r); ++r) 
      tmp.push_back(Pair(index(*r), -value(*r)));
    for(; out != out_end; ++out)
      tmp.push_back(*out);

    result.clear();
    for (typename std::vector<Pair>::iterator i = tmp.begin(); 
         i != tmp.end(); ++i)
      result.push_back(*i);
  }


#endif // MTL_EVAL_EXPR_HPP
