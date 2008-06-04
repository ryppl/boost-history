/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
#ifndef GTL_POLYGON_SET_TRAITS_HPP
#define GTL_POLYGON_SET_TRAITS_HPP
namespace gtl {

template <typename T>
struct polygon_set_traits {
  typedef typename T::coordinate_type coordinate_type;
  typedef typename T::iterator_type iterator_type;
  typedef typename T::operator_arg_type operator_arg_type;

  static inline iterator_type begin(const T& polygon_set) {
    return polygon_set.begin();
  }

  static inline iterator_type end(const T& polygon_set) {
    return polygon_set.end();
  }

  static inline void set(T& polygon_set, const std::vector<std::pair<coordinate_type, std::pair<coordinate_type, int> > >& value,
                         orientation_2d orient) {
    polygon_set.set(value, orient);
  }

  static inline orientation_2d orient(const T& polygon_set) { return polygon_set.orient(); }

  static inline bool dirty(const T& polygon_set) { return polygon_set.dirty(); }

  static inline bool sorted(const T& polygon_set) { return polygon_set.sorted(); }

};

}
#endif

