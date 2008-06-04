/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
#ifndef GTL_POLYGON_TRAITS_HPP
#define GTL_POLYGON_TRAITS_HPP
namespace gtl {
template <typename T>
struct polygon_traits {
  typedef typename T::coordinate_type coordinate_type;
  typedef typename T::iterator_type iterator_type;
  typedef typename T::compact_iterator_type compact_iterator_type;

  /// Get the begin iterator
  static inline iterator_type begin(const T& t) {
    return t.begin();
  }
  
  /// Get the end iterator
  static inline iterator_type end(const T& t) {
    return t.end();
  }
  
  /// Set the data of a polygon with the unique coordinates in an iterator, starting with an x
  template <typename iT>
  static inline T& set(T& t, iT input_begin, iT input_end) {
    t.set(input_begin, input_end);
    return t;
  }
  
  /// Get the begin iterator
  static inline compact_iterator_type begin_compact(const T& t) {
    return t.begin_compact();
  }
  
  /// Get the end iterator
  static inline compact_iterator_type end_compact(const T& t) {
    return t.end_compact();
  }
  
  /// Set the data of a polygon with the unique coordinates in an iterator, starting with an x
  template <typename iT>
  static inline T& set_compact(T& t, iT input_begin, iT input_end) {
    t.set_compact(input_begin, input_end);
    return t;
  }
  
  /// Get the number of sides of the polygon
  static inline unsigned int size(const T& t) {
    return t.size();
  }
  
  /// Get the winding direction of the polygon
  static inline winding_direction winding(const T& t) {
    return unknown_winding;
  }
};

}
#endif

