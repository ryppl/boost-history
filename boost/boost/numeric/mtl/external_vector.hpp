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

#ifndef MTL_EXTERNAL_VECTOR_HPP
#define MTL_EXTERNAL_VECTOR_HPP

#include <mtl/assert.hpp>
#include <algorithm> // for std::fill
#include <mtl/expr.hpp>
#include <mtl/external_vector_fwd.hpp>
#include <mtl/vector_adaptor.hpp>
#include <mtl/adaptor_bottom.hpp>

namespace mtl {

  template <class T, class Array, class Orien>
  class ext_vec_impl {
  public:
    // Container typedefs
    typedef T value_type;
    typedef std::size_t size_type;
    typedef T* iterator;
    typedef const T* const_iterator;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef std::ptrdiff_t difference_type;

    // Linalg typedefs
    typedef T scalar_type;
    typedef Orien orientation;
    typedef dense sparsity;

    ext_vec_impl() { }

    // args are (Array data, size_type n)
    template <class Head, class Tail>
    ext_vec_impl(const pair<Head,Tail>& args)
      : m_data(get_nth<0>::apply(args)), 
        m_size(get_nth<1>::apply(args)) { }

    iterator begin() { return &m_data[0]; }
    iterator end() { return &m_data[m_size]; }
    const_iterator begin() const { return &m_data[0]; }
    const_iterator end() const { return &m_data[m_size]; }

    size_type size() const { return m_size; }
    size_type nrows() const { return nrows(make_pair(1, m_size), Orien()); }
    size_type ncols() const { return ncols(make_pair(1, m_size), Orien()); }
  private:
    Array m_data;
    size_type m_size;    
  };

  template <class T, class Array, class Orientation>
  class external_vector {
    typedef ext_vec_impl<T, Array, Orientation> Impl;
  public:
    typedef adaptor_bottom< vector_adaptor<Impl, Impl> > type;
  };

} // namespace mtl

#endif // MTL_EXTERNAL_VECTOR_HPP
