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

#ifndef SPARSE_MATRIX_HPP
#define SPARSE_MATRIX_HPP

#include <mtl/vector.hpp>
#include <mtl/sparse_pair_vector.hpp>
#include <mtl/external_vector.hpp>
#include <mtl/matrix_adaptor.hpp>
#include <boost/smart_ptr.hpp>

namespace mtl {

  namespace detail {

    template <class T, class Orien>
    class sparse_matrix_impl 
      : public mtl::vector<
          typename mtl::sparse_pair_vector<T, Orien>::type, 
          typename trans_orien<Orien>::type>::type
    {
    public:
      // the 1-D type
      typedef typename mtl::sparse_pair_vector<T, Orien>::type 
        value_type;
    private:
      typedef typename trans_orien<Orien>::type twod_orien;
      typedef typename mtl::vector<value_type, twod_orien>::type Base;
    public:
      typedef dense sparsity;
      typedef linalg_expr<matrix_tag, T, value_type, 
        sparse_matrix_impl<T,Orien>, 
        twod_orien, dense> expr_type;
      typedef Orien orientation;
      typedef T scalar_type;
      typedef typename value_type::value_type::first_type size_type;
      typedef typename Base::iterator iterator;
      typedef typename Base::const_iterator const_iterator;
      typedef typename Base::reference reference;
      typedef typename Base::const_reference const_reference;

      // args are (size_type m, size_type n)
      template <class H1, class H2>
      sparse_matrix_impl(const pair<H1,pair<H2,nil> >& args) 
        : Base(get_nth<0>::apply(args), 
               value_type(get_nth<1>::apply(args))),
	m_nrows(get_nth<0>::apply(args)),
	m_ncols(get_nth<1>::apply(args)) { }

      size_type nrows() const { return m_nrows; }
      size_type ncols() const { return m_ncols; }

    private:
      typedef boost::shared_array<scalar_type> Array;
      typedef typename external_vector<scalar_type, Array, diagonal>::type 
        Diagonal;
    public:
      typedef void diagonal_view;
      typedef const Diagonal const_diagonal_view;

      const_diagonal_view diag() const {
	size_type n = std::min(nrows(), ncols());
	Array d = Array(new scalar_type[n]);
	::detail::copy_diagonal(*this, d);
	return Diagonal(d, n);
      }

      void resize(size_type n) {
        rep.resize(n);
        set_size_dispatch(n, Orien());
      }

    private:
      void set_size_dispatch(size_type n, row) {  m_nrows = n; }
      void set_size_dispatch(size_type n, column) { m_ncols = n; }

      size_type m_nrows;
      size_type m_ncols;
    };

  } // namespace detail

  template <class T, class Orientation = row>
  class sparse_matrix {
    typedef detail::sparse_matrix_impl<T, Orientation> Impl;
  public:
    typedef adaptor_bottom< matrix_adaptor<Impl, Impl> > type;
  };

} // namespace mtl



#endif // SPARSE_MATRIX_HPP
