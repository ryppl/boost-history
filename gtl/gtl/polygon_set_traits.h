/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/

template <typename T>
struct polygon_set_traits {
  typedef typename T::coordinate_type coordinate_type;
  typedef typename T::iterator_type iterator_type;
  typedef typename T::operator_arg_type operator_arg_type;

  static inline iterator_type begin(const T& polygon_set) {
    return polygon_set.value.begin();
  }

  static inline iterator_type end(const T& polygon_set) {
    return polygon_set.value.end();
  }

  static inline void insert(T& polygon_set, const std::vector<std::pair<coordinate_type, std::pair<coordinate_type, int> > >& value,
                            orientation_2d orient) {
    polygon_set.insert(value, orient);
  }

  static inline orientation_2d orient(const T& polygon_set) { return polygon_set.orient(); }

  static inline bool dirty(const T& polygon_set) { return polygon_set.dirty(); }

  static inline bool sorted(const T& polygon_set) { return polygon_set.sorted(); }

};

