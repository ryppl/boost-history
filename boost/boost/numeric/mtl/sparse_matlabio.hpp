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

#ifndef MTL_SPARSE_MATLABIO_HPP
#define MTL_SPARSE_MATLABIO_HPP

#include <stdexcept>
#include <string>
#include <mat.h> // matlab header file
#include <mtl/external_sparse_matrix.hpp>

namespace mtl {

  class sparse_matlabio_matrix
    : public external_sparse_matrix<int*,double*, column, int, double>::type
  {
  public:
    sparse_matlabio_matrix(std::string matrix_name, std::string file)
    {
      m_fp = matOpen(file.c_str(), "r");
      if (m_fp == NULL)
	throw std::invalid_argument("could not open file " + file);

      m_a = matGetArray(m_fp, matrix_name.c_str());
      if (m_a == NULL)
	throw std::invalid_argument("could not find matrix " + matrix_name);

      if (mxIsSparse(m_a) != true)
	throw std::invalid_argument("matrix was not m");

      sparse_matrix_data<int*,double*,int> d;
      d.nrows = mxGetM(m_a);
      d.ncols = mxGetN(m_a);
      d.nnz = mxGetNzmax(m_a); /* maybe use mxGetNumberOfElements */
      d.index = mxGetIr(m_a);
      d.ptrs = mxGetJc(m_a);
      d.value = (double*)mxGetData(m_a);
      this->set_data(d);

      matClose(m_fp);
    }
    ~sparse_matlabio_matrix() {
      mxDestroyArray(m_a);
    }
  private:
    // Do not copy this matrix!
    sparse_matlabio_matrix(const sparse_matlabio_matrix&) { }

    MATFile* m_fp;
    mxArray* m_a;
  };

} // namespace mtl

#endif // MTL_SPARSE_MATLABIO_HPP
