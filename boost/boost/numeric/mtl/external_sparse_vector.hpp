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

#ifndef MTL_EXTERNAL_SPARSE_VECTOR_HPP
#define MTL_EXTERNAL_SPARSE_VECTOR_HPP

#include <mtl/compressed_iterator.hpp>
#include <mtl/vector_adaptor.hpp>
#include <mtl/adaptor_bottom.hpp>
#include <mtl/expr.hpp>

namespace mtl {

  namespace detail {

    template <class IdxIter, class ValIter, class Orien, class I, class T>
    class ext_sparse_vector_impl
    {
      typedef compressed_iterators<IdxIter, ValIter, I, T> Iters;
      typedef typename Iters::policy Policy;
    public:
      typedef Orien orientation;
      typedef sparse sparsity;
      typedef T scalar_type;

      typedef typename Iters::iterator iterator;
      typedef typename Iters::const_iterator const_iterator;
      typedef typename iterator::value_type value_type;
      typedef typename iterator::reference reference;
      typedef typename const_iterator::reference const_reference;
      typedef typename iterator::pointer pointer;
      typedef typename const_iterator::pointer const_pointer;
      typedef typename iterator::difference_type difference_type;
      typedef difference_type size_type;

      ext_sparse_vector_impl() { }

      // args are (IdxIter index_array, ValIter value_array,
      //           size_type nnz, size_type n)
      template <class Head, class Tail>
      ext_sparse_vector_impl(const pair<Head,Tail>& args) 
        : m_index(IdxIter(get_nth<0>::apply(args))),
          m_value(ValIter(get_nth<1>::apply(args))), 
          m_nnz(get_nth<2>::apply(args)),
          m_size(get_nth<3>::apply(args))
      { }

      iterator begin() { return iterator(0, Policy(m_value, m_index)); }
      iterator end() { return iterator(m_nnz, Policy(m_value, m_index)); }

      const_iterator begin() const { 
        return const_iterator(0, Policy(m_value, m_index)); 
      }
      const_iterator end() const { 
        return const_iterator(m_nnz, Policy(m_value, m_index)); 
      }
      size_type size() const { return m_size; }
    private:
      I* m_index;
      T* m_value;
      I m_nnz;
      I m_size;
    };

  } // namespace detail   

  template <class IdxIter, class ValIter, class Orientation,
            class I = typename std::iterator_traits<IdxIter>::value_type, 
            class T = typename std::iterator_traits<ValIter>::value_type>
  class external_sparse_vector
  {
    typedef detail::ext_sparse_vector_impl<IdxIter,ValIter, Orientation,I,T>
      Impl;
  public:
    typedef adaptor_bottom< vector_adaptor<Impl, Impl> > type;
  };

} // namespace mtl

#endif // MTL_EXTERNAL_SPARSE_VECTOR_HPP
