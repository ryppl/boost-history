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

#ifndef MTL_EXTERNAL_SPARSE_MATRIX_HPP
#define MTL_EXTERNAL_SPARSE_MATRIX_HPP

#include <mtl/sparse_matrix_iterator.hpp>
#include <mtl/external_vector.hpp>
#include <mtl/matrix_adaptor.hpp>
#include <mtl/algobase.hpp>

namespace mtl {

  template <class IdxIter, class ValIter, class Size>
  struct sparse_matrix_data
  {
    sparse_matrix_data() { }

    template <class Head, class Tail>
    sparse_matrix_data(const mtl::pair<Head,Tail>& args)
      : index(get_nth<0>::apply(args)),
        value(get_nth<1>::apply(args)), 
        ptrs(get_nth<2>::apply(args)),
        nrows(get_nth<3>::apply(args)), 
        ncols(get_nth<4>::apply(args)), 
        nnz(get_nth<5>::apply(args)) { }
    
    IdxIter index;
    ValIter value;
    IdxIter ptrs;
    Size nrows;
    Size ncols;
    Size nnz;
  };

  namespace detail {

    template <class IdxIter, class ValIter, class Orien,
              class I, class T>
    class ext_sparse_matrix_impl
    {
      typedef sparse_matrix_iterators<IdxIter, ValIter, I, T, Orien> Iters;
      typedef typename Iters::policy Policy;
      typedef typename trans_orien<Orien>::type twod_orien;
    public:
      typedef T scalar_type;
      typedef dense sparsity;
      typedef Orien orientation;
      typedef typename Iters::iterator iterator;
      typedef typename Iters::const_iterator const_iterator;
      typedef typename iterator::value_type value_type;
      typedef typename iterator::reference reference;
      typedef typename const_iterator::reference const_reference;
      typedef typename iterator::difference_type difference_type;
      typedef I size_type;
      typedef typename iterator::pointer pointer;
      typedef typename const_iterator::pointer const_pointer;

      ext_sparse_matrix_impl() { }

      // args are (IdxIter index_array, ValIter value_array,
      //           IdxIter oned_ptrs, size_type m, size_type n,
      //           size_type nnz)
      template <class Head, class Tail>
      ext_sparse_matrix_impl(const mtl::pair<Head,Tail>& args)
        : m_data(args) { }

      template <class D>
      void set_data(const D& d) { m_data = d; }

      template <class D>
      void get_data(D& d) const { d = m_data; }

      iterator begin() { 
        return iterator(0, Policy(m_data.value, m_data.index, 
                                  m_data.ptrs, this->oned_size())); 
      }
      iterator end() { 
        return iterator(this->size(),
                        Policy(m_data.value, m_data.index, 
                               m_data.ptrs, this->oned_size())); 
      }
      const_iterator begin() const {
        return iterator(0, Policy(m_data.value, m_data.index, 
                                  m_data.ptrs, this->oned_size())); 
      }
      const_iterator end() const {
        return iterator(this->size(),
                        Policy(m_data.value, m_data.index, 
                               m_data.ptrs, this->oned_size())); 
      }

      size_type size() const { return this->size_dispatch(Orien()); }
      size_type oned_size() const { 
        return this->oned_size_dispatch(Orien()); 
      }

      size_type nrows() const { return m_data.nrows; }
      size_type ncols() const { return m_data.ncols; }
      size_type nnz() const { return m_data.nnz; }

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

    protected:
      size_type size_dispatch(row) const { return m_data.nrows; }
      size_type size_dispatch(column) const { return m_data.ncols; }
      size_type oned_size_dispatch(row) const { return m_data.ncols; }
      size_type oned_size_dispatch(column) const { return m_data.nrows; }

      typedef sparse_matrix_data<IdxIter,ValIter,size_type> Data;
      Data m_data;
    };

  } // namespace detail

  template <class IdxIter, class ValIter, class Orientation,
            class I = typename std::iterator_traits<IdxIter>::value_type, 
            class T = typename std::iterator_traits<ValIter>::value_type>
  class external_sparse_matrix {
    typedef detail::ext_sparse_matrix_impl<IdxIter, ValIter,
      Orientation, I, T> Impl;
  public:
    typedef adaptor_bottom< matrix_adaptor<Impl, Impl> > type;
  };

} // namespace mtl

#endif // MTL_EXTERNAL_SPARSE_MATRIX_HPP
