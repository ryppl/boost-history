//
//=======================================================================
// Copyright 1997, 1998, 1999, 2000 University of Notre Dame.
// Authors: Andrew Lumsdaine, Lie-Quan Lee, Jeremy G. Siek
//
// This file is part of the Boost Graph Library
//
// You should have received a copy of the License Agreement for the
// Boost Graph Library along with the software; see the file LICENSE.
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
//
#ifndef ADSTL_ARRAY_BINARY_TREE_HPP
#define ADSTL_ARRAY_BINARY_TREE_HPP

#include <iterator>
#include <functional>
#include <boost/config.hpp>

namespace adstl {
  /*
    Note: array_binary_tree is a completey balanced binary tree
   */

#if !defined BOOST_NO_STD_ITERATOR_TRAITS
  template <class RandomAccessIterator, class ID>
#else
  template <class RandomAccessIterator, class ValueType, class ID>
#endif
class array_binary_tree_node {
public:
  typedef RandomAccessIterator rep_iterator;
#if !defined BOOST_NO_STD_ITERATOR_TRAITS
  typedef typename std::iterator_traits<RandomAccessIterator>::difference_type
          difference_type;
  typedef typename std::iterator_traits<RandomAccessIterator>::value_type
          value_type;
#else
  typedef int difference_type;
  typedef ValueType value_type;
#endif
  typedef difference_type size_type;

  struct children_type {
    struct iterator { // replace with iterator_adaptor implementation -JGS
      inline iterator() : i(0), n(0) { }
      inline iterator(const iterator& x) : r(x.r), i(x.i), n(x.n), id(x.id) { }
      inline iterator& operator=(const iterator& x) {
	r = x.r; i = x.i; n = x.n; 
	/*egcs generate a warning*/
	id = x.id; 
	return *this;
      }
      inline iterator(rep_iterator rr, 
		      size_type ii, 
		      size_type nn, 
		      const ID& _id) : r(rr), i(ii), n(nn), id(_id) { }
      inline array_binary_tree_node operator*() { return array_binary_tree_node(r, i, n, id); }
      inline iterator& operator++() { ++i; return *this; }
      inline iterator operator++(int)
        { iterator t = *this; ++(*this); return t; }
      inline bool operator==(const iterator& x) const { return i == x.i; }
      inline bool operator!=(const iterator& x) const 
        { return !(*this == x); }
      rep_iterator r;
      size_type i;
      size_type n;
      ID id;
    };
    inline children_type() : i(0), n(0) { }
    inline children_type(const children_type& x)
      : r(x.r), i(x.i), n(x.n), id(x.id) { }
    inline children_type& operator=(const children_type& x) {
      r = x.r; i = x.i; n = x.n; 
      /*egcs generate a warning*/
      id = x.id; 
      return *this;
    }
    inline children_type(rep_iterator rr,
			 size_type ii, 
			 size_type nn,
			 const ID& _id) : r(rr), i(ii), n(nn), id(_id) { }
    inline iterator begin() { return iterator(r, 2 * i + 1, n, id); }
    inline iterator end() { return iterator(r, 2 * i + 1 + size(), n, id); }
    inline size_type size() const {
      size_type c = 2 * i + 1;
      size_type s;
      if      (c + 1 < n) s = 2;
      else if (c < n)     s = 1;
      else                s = 0;
      return s;
    }
    rep_iterator r;
    size_type i;
    size_type n;
    ID id;
  };
  inline array_binary_tree_node() : i(0), n(0) { }
  inline array_binary_tree_node(const array_binary_tree_node& x) 
    : r(x.r), i(x.i), n(x.n), id(x.id) { }
  inline array_binary_tree_node& operator=(const array_binary_tree_node& x) {
    r = x.r;
    i = x.i; 
    n = x.n; 
    /*egcs generate a warning*/
    id = x.id; 
    return *this;
  }
  inline array_binary_tree_node(rep_iterator start, 
			 rep_iterator end, 
			 rep_iterator pos, const ID& _id)
    : r(start), i(pos - start), n(end - start), id(_id) { }
  inline array_binary_tree_node(rep_iterator rr, 
			 size_type ii, 
			 size_type nn, const ID& _id) 
    : r(rr), i(ii), n(nn), id(_id) { }
  inline value_type& value() { return *(r + i); }
  inline const value_type& value() const { return *(r + i); }
  inline array_binary_tree_node parent() const { return array_binary_tree_node(r, (i - 1) / 2, n, id); }
  inline bool has_parent() const { return i != 0; }
  inline children_type children() { return children_type(r, i, n, id); }
  /*
  inline void swap(array_binary_tree_node x) {
    value_type tmp = x.value();
    x.value() = value();
    value() = tmp;
    i = x.i;
  }
  */
  template <class ExternalData>
  inline void swap(array_binary_tree_node x, ExternalData& edata ) {
    value_type tmp = x.value();

    /*swap external data*/
    edata[ id[tmp] ]     = i;
    edata[ id[value()] ] = x.i;

    x.value() = value();
    value() = tmp;
    i = x.i;
  }
   inline const children_type children() const { 
    return children_type(r, i, n); 
  }
  inline size_type index() const { return i; }
  rep_iterator r;
  size_type i;
  size_type n;
  ID id;
};

template <class RandomAccessContainer, 
       class Compare = std::less<typename RandomAccessContainer::value_type> >
struct compare_array_node {
  typedef typename RandomAccessContainer::value_type value_type;
  compare_array_node(const Compare& x) : comp(x) {}
  compare_array_node(const compare_array_node& x) : comp(x.comp) {}

  template< class node_type >
  inline bool operator()(const node_type& x, const node_type& y) {
    return comp(x.value(), y.value());
  }

  template< class node_type >
  inline bool operator()(const node_type& x, const node_type& y) const {
    return comp(x.value(), y.value());
  }
  Compare comp;
};


} /* namespace adstl */

#endif /* ADSTL_ARRAY_BINARY_TREE_H */
