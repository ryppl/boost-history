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

#ifndef MTL_SPARSE_MATRIX_ITERATOR_HPP
#define MTL_SPARSE_MATRIX_ITERATOR_HPP

#include <boost/iterator_adaptors.hpp>
#include <mtl/external_sparse_vector.hpp>

namespace mtl {

  template <class IdxIter, class ValIter, class Idx>
  struct sparse_matrix_iter_policy : public boost::default_iterator_policies
  {
    sparse_matrix_iter_policy() { }

    sparse_matrix_iter_policy(ValIter value, IdxIter index, 
                              IdxIter ptr, Idx minor)
      : m_value(value), m_index(index), m_ptr(ptr), m_minor(minor) { }

    template <class Reference, class Pos>
    Reference dereference(boost::type<Reference>, Pos pos) const {
      Idx i = m_ptr[pos];
      Idx nnz = m_ptr[pos+1] - i;
      return Reference(m_index + i, m_value + i, nnz, m_minor);
    }

    ValIter m_value;
    IdxIter m_index;
    IdxIter m_ptr;
    Idx m_minor;
  };

#if 0
  template <class ValueType>
  struct sparse_matrix_iter_traits {
    typedef ValueType value_type;
    typedef value_type reference;
    typedef value_type* pointer;
    typedef std::ptrdiff_t difference_type;
    typedef std::random_access_iterator_tag iterator_category;
  };

  template <class ValueType>
  struct const_sparse_matrix_iter_traits {
    typedef ValueType value_type;
    typedef const value_type reference;
    typedef const value_type* pointer;
    typedef std::ptrdiff_t difference_type;
    typedef std::random_access_iterator_tag iterator_category;
  };
#endif

  template <class IdxIter, class ValIter, class Idx, class T, class Orien>
  class sparse_matrix_iterators
  {
    typedef typename 
      external_sparse_vector<IdxIter,ValIter, Orien, Idx, T>::type ValueType;
    typedef sparse_matrix_iter_policy<IdxIter, ValIter, Idx> Policy;
    typedef ValueType value_type;
    typedef value_type reference;
    typedef const value_type const reference;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef std::ptrdiff_t diff_type;
    typedef std::random_access_iterator_tag category;
  public:
    typedef Policy policy;
    typedef boost::iterator_adaptor<std::size_t, Policy, value_type,
      reference, pointer, category, diff_type> iterator;
    typedef boost::iterator_adaptor<std::size_t, Policy, value_type,
      const_reference, const_pointer, category, diff_type> const_iterator;
  };

} // namespace mtl

#endif // MTL_SPARSE_MATRIX_ITERATOR_HPP
