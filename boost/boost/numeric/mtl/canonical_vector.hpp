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

#ifndef MTL_CANONICAL_VECTOR_HPP
#define MTL_CANONICAL_VECTOR_HPP

#include <iterator>
#include <boost/iterator_adaptors.hpp>

namespace mtl {

#if 0
  template <class T>
  struct canonical_iter_traits {
    typedef mtl::pair<std::size_t, T> value_type;
    typedef value_type reference;
    typedef value_type* pointer;
    typedef std::ptrdiff_t difference_type;
    typedef std::forward_iterator_tag iterator_category;
  };
#endif

  struct canonical_iter_policies : public boost::default_iterator_policies
  {
    canonical_iter_policies() { }
    canonical_iter_policies(std::size_t k) : m_k(k) { }
    template <class Pair>
    Pair dereference(boost::type<Pair>, bool) const { 
      typedef typename Pair::second_type T;
      return Pair(m_k, T(1)); 
    }
    void increment(bool& done) { done = true; }

    std::size_t m_k;
  };

  template <class T>
  class canonical_vector
    : public linalg_expr<vector_tag, T, mtl::pair<std::size_t, T>,
         canonical_vector<T>, column, sparse>
  // use container adaptor
  {
    typedef mtl::pair<std::size_t, T> Pair;
    //typedef canonical_iter_traits<T> Traits;
    typedef canonical_iter_policies Policy;
  public:
    typedef sparse sparsity;
    typedef linalg_expr<vector_tag, T, Pair, 
      canonical_vector<T>, column, sparse> expr_type;

    typedef boost::iterator_adaptor<bool, Policy, 
      Pair, Pair, Pair*, std::input_iterator, std::ptrdiff_t> iterator;
    typedef iterator const_iterator;
    typedef Pair value_type;
    typedef Pair reference;
    typedef Pair const_reference;
    typedef const Pair* pointer;
    typedef const Pair* const_pointer;
    typedef std::size_t size_type;
    
    canonical_vector(std::size_t k, std::size_t n)
      : m_k(k), m_n(n) { }

    ~canonical_vector() { }

    iterator begin() { return iterator(false, k); }
    iterator end() { return iterator(true, k); }
    const_iterator begin() const { return const_iterator(true, m_k); }
    const_iterator end() const { return const_iterator(true, m_k); }

    size_type size() const { return m_n; }
    size_type nrows() const { return m_n; }
    size_type ncols() const { return 1; }
  private:
    std::size_t m_k;
    std::size_t m_n;
  };

  template <class T>
  class create_canonical {
  public:
    create_canonical(std::size_t n) : m_n(n) { }

    inline canonical_vector<T> 
    operator()(std::size_t k) { 
      return canonical_vector<T>(k, m_n); 
    }
  private:
    std::size_t m_n;
  };

} // namespace mtl

#endif //  MTL_CANONICAL_VECTOR_HPP
