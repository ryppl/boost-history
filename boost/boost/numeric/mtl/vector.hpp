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

#ifndef MTL_VECTOR_HPP
#define MTL_VECTOR_HPP

#include <vector>
#include <algorithm> // for std::fill
#include <mtl/vector_fwd.hpp>
#include <mtl/vector_adaptor.hpp>
#include <mtl/adaptor_bottom.hpp>

namespace mtl {

  namespace detail {

    template <class T, class Orientation>
    class vector_impl : public std::vector<T> 
    {
      typedef std::vector<T> Base;
    public:
      typedef typename Base::size_type size_type;
      typedef T scalar_type;
      typedef Orientation orientation;
      typedef dense sparsity;

      vector_impl() { }

      // args are (size_type n)
      template <class H>
      vector_impl(const pair<H,nil>& args) 
	: Base(get_nth<0>::apply(args)) { }

      // args are (size_type n, const T& init)
      template <class H1, class H2>
      vector_impl(const pair<H1,pair<H2,nil> >& args) 
	: Base(get_nth<0>::apply(args), 
	       get_nth<1>::apply(args)) { }

      size_type nrows() const { 
	mtl::pair<size_type, size_type> shape(1, size());
	return ::nrows(shape, Orientation()); 
      }
      size_type ncols() const { 
	mtl::pair<size_type, size_type> shape(1, size());
	return ::ncols(shape, Orientation()); 
      }
    };
  }

  template <class T, class Orientation>
  class vector {
    typedef detail::vector_impl<T, Orientation> Impl;
  public:
    typedef adaptor_bottom< vector_adaptor<Impl, Impl> > type;
  };

} // namespace mtl

#endif // MTL_VECTOR_HPP
