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

#ifndef SPARSE_PAIR_VECTOR_HPP
#define SPARSE_PAIR_VECTOR_HPP

#include <vector>
#include <algorithm> // for std::sort
#include <mtl/index_value_pair.hpp>
#include <mtl/sparse_pair_vector_fwd.hpp>
#include <mtl/vector_adaptor.hpp>
#include <mtl/adaptor_bottom.hpp>

// issue: sort elements by index?

namespace mtl {

  namespace detail {

    template <class Pair>
    struct compare1st {
      typename Pair::first_type operator()(const Pair& a, const Pair& b) {
	return a.first < b.first;
      }
    };

    template <class T, class Orien>
    class sparse_pair_vector_impl 
      : public std::vector< mtl::pair<std::size_t, T> >
    {
      typedef std::size_t Idx;
      typedef mtl::pair<Idx, T> Pair;
      typedef std::vector<Pair> Base;
    public:
      typedef typename Base::size_type size_type;
      typedef sparse sparsity;
      typedef Orien orientation;
      typedef T scalar_type;

      sparse_pair_vector_impl() : m_size(size_type()) { }

      // args are (size_type n)
      template <class H>
      sparse_pair_vector_impl(const pair<H,nil>& args) 
	: m_size(get_nth<0>::apply(args)) { }

      void resize(size_type n) { m_size = n; }
      size_type size() const { return m_size; }

      size_type nrows() const { return nrows(make_pair(1, m_size), Orien()); }
      size_type ncols() const { return ncols(make_pair(1, m_size), Orien()); }
 
      void sort() {
	std::sort(rep.begin(), rep.end(), detail::compare1st<Pair>());
      }
    private:
      size_type m_size;
    };

  } // namespace detail

  template <class T, class Orientation>
  class sparse_pair_vector
  {
    typedef detail::sparse_pair_vector_impl<T,Orientation> Impl;
  public:
    typedef adaptor_bottom< vector_adaptor<Impl, Impl> > type;
  };

} // namespace mtl

#endif // SPARSE_PAIR_VECTOR_HPP
