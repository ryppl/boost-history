/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
#ifndef GTL_POLYGON_SET_WRAPPER_HPP
#define GTL_POLYGON_SET_WRAPPER_HPP
namespace gtl {
template <typename T>
class polygon_set_const_wrapper {
private:
  const T& t_;
public:
  typedef typename T::value_type geometry_type;
  typedef typename geometry_concept<geometry_type>::type concept_type;
  typedef typename concept_type::template coordinate_type<geometry_type>::type coordinate_type;
  typedef iterator_geometry_range_to_set<concept_type, typename T::const_iterator> iterator_type;
  typedef polygon_set_const_wrapper operator_arg_type;
  typedef operator_requires_copy operator_storage_tag;

  polygon_set_const_wrapper(const T& t) : t_(t) {}

  inline iterator_type begin() const {
    return iterator_type(t_.begin());
  }

  inline iterator_type end() const {
    return iterator_type(t_.end());
  }

  inline orientation_2d orient() const { return HORIZONTAL; }

  inline bool dirty() const { return true; }

  inline bool sorted() const { return false; }

};

template <typename T>
polygon_set_const_wrapper<T> wrap_const(const T& t) {
  return polygon_set_const_wrapper<T>(t);
}

template <typename T>
class polygon_set_wrapper {
private:
  T& t_;
public:
  typedef typename T::value_type geometry_type;
  typedef typename geometry_concept<geometry_type>::type concept_type;
  typedef typename concept_type::template coordinate_type<geometry_type>::type coordinate_type;
  typedef iterator_geometry_range_to_set<concept_type, typename T::const_iterator> iterator_type;
  typedef polygon_set_wrapper operator_arg_type;
  typedef operator_requires_copy operator_storage_tag;

  inline polygon_set_wrapper(T& t) : t_(t) {}

  inline iterator_type begin() const {
    return iterator_type(t_.begin());
  }

  inline iterator_type end() const {
    return iterator_type(t_.end());
  }

  inline void set(const std::vector<std::pair<coordinate_type, std::pair<coordinate_type, int> > >& value,
           orientation_2d orient) { 
  }

  inline orientation_2d orient() const { return HORIZONTAL; }

  inline bool dirty() const { return true; }

  inline bool sorted() const { return false; }

};

template <typename T>
polygon_set_const_wrapper<T> wrap(const T& t) {
  return polygon_set_wrapper<T>(t);
}

}
#endif

