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

#ifndef MTL_VECTOR_ADAPTOR_HPP
#define MTL_VECTOR_ADAPTOR_HPP

#include <mtl/container_adaptor.hpp>
#include <mtl/expr.hpp>

namespace mtl {

  template <class Traits>
  class default_vector_policies : public default_container_policies<Traits> {
    typedef typename Traits::size_type size_type;
    typedef typename Traits::const_reference const_reference;
  public:
    default_vector_policies() { }

    template <class C>
    size_type nrows(const C& c) const { return c.nrows(); }

    template <class C>
    size_type ncols(const C& c) const { return c.ncols(); }

    // for sparse vectors
    template <class C>
    void push_back(C& c, const_reference p) const { c.push_back(p); }

    template <class C>
    void clear(C& c) const { c.clear(); }

    template <class C>
    void sort(C& c) const { c.sort(); }
  };

  template <class V, class Traits,
            class Policies = default_vector_policies<Traits> >  
  class vector_adaptor :
    public container_adaptor<V, Traits, Policies>,
    public linalg_expr<vector_tag,
      typename Traits::scalar_type,
      typename Traits::value_type,
      vector_adaptor<V, Traits, Policies>,
      typename Traits::orientation,
      typename Traits::sparsity>
  {
    typedef container_adaptor<V, Traits, Policies> ContainerBase;
    typedef linalg_expr<vector_tag,
      typename Traits::scalar_type,
      typename Traits::value_type,
      vector_adaptor<V, Traits, Policies>,
      typename Traits::orientation,
      typename Traits::sparsity> LinalgBase;
  public:
    typedef typename Traits::scalar_type scalar_type;
    typedef typename Traits::value_type value_type;
    typedef typename Traits::size_type size_type;
    typedef typename Traits::difference_type difference_type;
    typedef typename Traits::reference reference;
    typedef typename Traits::const_reference const_reference;
    typedef typename Traits::pointer pointer;
    typedef typename Traits::const_pointer const_pointer;

    vector_adaptor() { }
    
    template <class H, class T>
    vector_adaptor(const pair<H,T>& args)
      : ContainerBase(args) { }

    template <class X>
    vector_adaptor& operator=(const X& x) {
      LinalgBase::operator=(x);
      return *this;
    }

    size_type nrows() const { return policies().nrows(c()); }
    size_type ncols() const { return policies().ncols(c()); }

    // for sparse vectors
    void push_back(const_reference p) { 
      return policies().push_back(c(), p); 
    }
    void clear() { return policies().clear(c()); }
    void sort() { return policies().sort(c()); }
  };
  
} // namespace mtl

#endif // MTL_VECTOR_ADAPTOR_HPP
