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

#ifndef MTL_MATRIX_ADAPTOR_HPP
#define MTL_MATRIX_ADAPTOR_HPP

#include <mtl/vector_adaptor.hpp>
#include <mtl/expr.hpp>

namespace mtl {

  template <class Traits>
  class default_matrix_policies : public default_vector_policies<Traits> {
    typedef typename Traits::size_type size_type;
    typedef typename Traits::diagonal_view diagonal_view;
    typedef typename Traits::const_diagonal_view const_diagonal_view;
  public:
    default_matrix_policies() { }

    template <class C>
    const_diagonal_view diag(const C& c) const { return c.diag(); }
    
    template <class C>
    diagonal_view diag(C& c) { return c.diag(); }

    template <class C>
    size_type nnz(const C& c) const { return c.nnz(); }
  };

  template <class V, class Traits,
            class Policies = default_matrix_policies<Traits> >  
  class matrix_adaptor :
    public container_adaptor<V, Traits, Policies>,
    public linalg_expr<matrix_tag,
      typename Traits::scalar_type,
      typename Traits::value_type,
      matrix_adaptor<V, Traits, Policies>,
      typename trans_orien<typename Traits::orientation>::type,
      typename Traits::sparsity>
  {
    typedef container_adaptor<V, Traits, Policies> ContainerBase;
    typedef linalg_expr<matrix_tag,
      typename Traits::scalar_type,
      typename Traits::value_type,
      matrix_adaptor<V, Traits, Policies>,
      typename trans_orien<typename Traits::orientation>::type,
      typename Traits::sparsity> LinalgBase;
  public:
    typedef typename Traits::orientation orientation;
    typedef typename Traits::sparsity sparsity;
    typedef typename Traits::scalar_type scalar_type;
    typedef typename Traits::diagonal_view diagonal_view;
    typedef typename Traits::const_diagonal_view const_diagonal_view;

    typedef typename Traits::value_type value_type;
    typedef typename Traits::size_type size_type;
    typedef typename Traits::difference_type difference_type;
    typedef typename Traits::reference reference;
    typedef typename Traits::const_reference const_reference;
    typedef typename Traits::pointer pointer;
    typedef typename Traits::const_pointer const_pointer;

    matrix_adaptor() { }
    
    template <class H, class T>
    matrix_adaptor(const pair<H,T>& args)
      : ContainerBase(args) { }

    template <class X>
    matrix_adaptor& operator=(const X& x) {
      LinalgBase::operator=(x);
      return *this;
    }

    size_type nrows() const { return policies().nrows(c()); }
    size_type ncols() const { return policies().ncols(c()); }
    size_type nnz() const { return policies().nnz(c()); }

    diagonal_view diag() { return policies().diag(c()); }
    const_diagonal_view diag() const { return policies().diag(c()); }

    // for matrices with external data
    template <class Data>
    void set_data(const Data& d) { c().set_data(d); }

    template <class Data>
    void get_data(Data& d) { c().get_data(d); }
    
  };
  
} // namespace mtl

#endif // MTL_MATRIX_ADAPTOR_HPP

