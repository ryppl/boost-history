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

#ifndef MTL_COMPRESSED_ITERATOR_HPP
#define MTL_COMPRESSED_ITERATOR_HPP

#include <mtl/promote.hpp> // for to_expr
#include <boost/iterator_adaptors.hpp>

namespace mtl {

  template <class IdxIter, class ValIter>
  struct comp_iter_policies : public boost::default_iterator_policies
  {
    comp_iter_policies() { }
    comp_iter_policies(ValIter v, IdxIter i) : m_value(v), m_index(i) { }

    template <class Reference, class Pos>
    Reference dereference(boost::type<Reference>, Pos pos) const {
      return Reference(m_index[pos], m_value[pos]);
    }
    ValIter m_value;
    IdxIter m_index;
  };

#if 0
  template <class T, class I, class IdxIter, class ValIter>
  struct comp_iter_traits {
    typedef mtl::pair<I&, T&> reference;
    typedef mtl::pair<I,T> value_type;
    typedef value_type* pointer;
    typedef typename std::iterator_traits<ValIter>::difference_type
      difference_type;
    typedef std::random_access_iterator_tag iterator_category;
  };
  template <class T, class I, class IdxIter, class ValIter>
  struct const_comp_iter_traits {
    typedef mtl::pair<const I&,const T&> reference;
    typedef mtl::pair<I,T> value_type;
    typedef value_type* pointer;
    typedef typename std::iterator_traits<ValIter>::difference_type
      difference_type;
    typedef std::random_access_iterator_tag iterator_category;
  };
#endif

  template <class IdxIter, class ValIter, class I, class T>
  class compressed_iterators {
    typedef comp_iter_policies<IdxIter,ValIter> Policy;
    typedef mtl::pair<I&, T&> reference;
    typedef mtl::pair<const I&,const T&> const_reference;
    typedef mtl::pair<I,T> value_type;
    typedef value_type* pointer;
    typedef typename std::iterator_traits<ValIter>::difference_type
      difference_type;
    //    typedef std::random_access_iterator_tag iterator_category;
    typedef std::input_iterator_tag iterator_category;
  public:
    typedef Policy policy;

    typedef boost::iterator_adaptor<std::size_t, Policy, value_type,
      reference, pointer, iterator_category, difference_type> iterator;

    typedef boost::iterator_adaptor<std::size_t, Policy, value_type,
      const_reference, pointer, iterator_category, difference_type> 
      const_iterator;
  };


} // namespace mtl

#endif // MTL_COMPRESSED_ITERATOR_HPP
