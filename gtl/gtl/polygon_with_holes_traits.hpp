/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
#ifndef GTL_POLYGON_WITH_HOLES_TRAITS_HPP
#define GTL_POLYGON_WITH_HOLES_TRAITS_HPP
namespace gtl {

template <class T>
struct polygon_with_holes_traits {
  typedef typename T::iterator_holes_type iterator_holes_type;
  typedef typename T::hole_type hole_type;

  /// Get the begin iterator
  static inline iterator_holes_type begin_holes(const T& t) {
    return t.begin_holes();
  }

  /// Get the end iterator
  static inline iterator_holes_type end_holes(const T& t) {
    return t.end_holes();
  }

  /// Set the data of a polygon with the unique coordinates in an iterator, starting with an x
  template <typename iT>
  static inline T& set_holes(T& t, iT inputBegin, iT inputEnd) {
    t.set_holes(inputBegin, inputEnd);
    return t;
  }

  /// Get the number of holes 
  static inline unsigned int size_holes(const T& t) {
    return t.size_holes();
  }

};

}
#endif

