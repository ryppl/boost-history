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

#ifndef EXTERNAL_MATRIX_HPP
#define EXTERNAL_MATRIX_HPP

#include <mtl/external_vector.hpp>

namespace mtl {

  namespace detail {

    class ext_mat_iter_policy : public boost::defaul_iterator_policies {
    public:
      ext_mat_iter_policy() { }
      
      template <class Reference, class Offset>
      Reference dereference(boost::type<Reference>, Offset p) const {
	return Reference(p, m_minor);
      }

      increment(Offset& p) { p += m_ld; }
      
      size_type m_minor;
      size_type m_ld;
    };

  }


  template <class T, class Orientation = row>
  class external_matrix
    : public linalg_expr<matrix_tag, T, 
        mtl::external_vector<T, Orientation>, 
        external_matrix<T,Orientation>, 
        typename trans_orien<Orientation>::type, 
        dense>
  {
    typedef typename trans_orien<Orientation>::type twod_orien;
  public:
    // the 1-D type
    typedef mtl::external_vector<T, Orientation> value_type;

    typedef dense sparsity;
    typedef linalg_expr<matrix_tag, T, value_type, 
      external_matrix<T,Orientation>, 
      twod_orien, dense> expr_type;
    typedef T scalar_type;

    typedef std::size_t size_type;
    typedef typename Iters::iterator iterator;
    typedef typename Iters::iterator const_iterator;
    typedef value_type reference;
    typedef value_type const_reference;

    external_matrix(Array data, size_type m, size_type n, size_type ld)
      : m_data(data), m_ld(ld), m_nrows(m), m_ncols(n) { }
    ~external_matrix() { }
    template <class E, class Sparsity>
    external_matrix& 
    operator=( const expr<matrix_tag, T,E,twod_orien,Sparsity>& x ) 
      { x.eval(*this); return *this; }

    iterator begin() { return iterator( ); }
    iterator end() { return iterator( ); }
    const_iterator begin() const { return const_iterator( ); }
    const_iterator end() const { return const_iterator( ); }

    size_type size() const { return this->size_dispatch(Orientation()); }
    size_type nrows() const { return m_nrows; }
    size_type ncols() const { return m_ncols; }
  private:
    size_type size_dispatch(row) const { return m_nrows; }
    size_type size_dispatch(column) const { return m_ncols; }

    T* m_data;
    size_type m_ld;
    size_type m_nrows;
    size_type m_ncols;
  };

} // namespace mtl

#endif // EXTERNAL_MATRIX_HPP
