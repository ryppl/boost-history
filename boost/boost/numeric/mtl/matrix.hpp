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

#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <mtl/vector.hpp>
#include <mtl/matrix_adaptor.hpp>

namespace mtl {

  namespace detail {

    template <class T, class Orientation>
    class matrix_impl
      : public mtl::vector<typename mtl::vector<T, Orientation>::type, 
          typename trans_orien<Orientation>::type >::type
    {
    public:
      // the 1-D type
      typedef typename mtl::vector<T, Orientation>::type value_type;
    private:
      typedef typename trans_orien<Orientation>::type twod_orien;
      typedef typename mtl::vector<value_type, twod_orien>::type Base;
    public:
      typedef typename Base::size_type size_type;
      typedef dense sparsity;
      typedef Orientation orientation;
      typedef T scalar_type;

      matrix_impl() { }

      // args are (size_type m, size_type n)
      template <class H1, class H2>
      matrix_impl(const pair<H1,pair<H2,nil> >& args) 
        : Base(get_nth<0>::apply(args), 
               value_type(get_nth<1>::apply(args))) { }

      // args are (size_type m, size_type n, const T& value)
      template <class H1, class H2, class H3>
      matrix_impl(const pair<H1,pair<H2,pair<H3,nil> > >& args) 
        : Base(get_nth<0>::apply(args), 
               value_type(get_nth<1>::apply(args), 
                          get_nth<2>::apply(args))) { }
      
      size_type nrows() const { 
	mtl::pair<size_type,size_type> shape(size(), (*this)[0].size());
        return ::nrows(shape, Orientation()); 
      }
      size_type ncols() const { 
	mtl::pair<size_type,size_type> shape(size(), (*this)[0].size());
        return ::ncols(shape, Orientation()); 
      }

      typedef void diagonal_view;
      typedef void const_diagonal_view;

    };

  } // namespace detail

  template <class T, class Orientation = row>
  class matrix {
    typedef detail::matrix_impl<T, Orientation> Impl;
  public:
    typedef adaptor_bottom< matrix_adaptor<Impl, Impl> > type;
  };

} // namespace mtl

#endif // MATRIX_HPP
