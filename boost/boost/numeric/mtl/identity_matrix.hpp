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

#ifndef MTL_IDENTITY_MATRIX_HPP
#define MTL_IDENTITY_MATRIX_HPP

#include <mtl/canonical_vector.hpp>
#include <mtl/expr.hpp>

// identity matrix is a column matrix of canonical vectors
// nah, that's a bad idea. Need to implement expressions
// with diagonal orientation

template <class T>
class identity_matrix
  : public linalg_expr<matrix_tag, T, canonical_vector<T>,
      identity_matrix<T>, column, sparse>
{

public:


  identity_matrix(size_type n) : m_size(n) { }
  
private:
  size_type m_size;
};


#endif // MTL_IDENTITY_MATRIX_HPP
