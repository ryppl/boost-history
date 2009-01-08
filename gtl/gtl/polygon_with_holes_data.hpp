/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
#ifndef GTL_POLYGON_WITH_HOLES_DATA_HPP
#define GTL_POLYGON_WITH_HOLES_DATA_HPP
namespace gtl {
  struct polygon_with_holes_concept;
  template <typename T>
  class polygon_with_holes_data : public polygon_45_with_holes_data<T> {
  public:
    typedef polygon_with_holes_concept geometry_type;
    //inherits everything
  };
  
  template <typename T>
  std::ostream& operator<<(std::ostream& o, const polygon_90_with_holes_data<T>& poly) {
    o << "Polygon With Holes { ";
    for(typename polygon_90_with_holes_data<T>::iterator_type itr = poly.begin(); 
        itr != poly.end(); ++itr) {
      if(itr != poly.begin()) o << ", ";
      o << (*itr).get(HORIZONTAL) << " " << (*itr).get(VERTICAL);
    } o << " { ";
    for(typename polygon_90_with_holes_data<T>::iterator_holes_type itr = poly.begin_holes();
        itr != poly.end_holes(); ++itr) {
      o << (*itr);
    }
    o << " } } ";
    return o;
  }
  template <typename T>
  std::ostream& operator<<(std::ostream& o, const polygon_45_with_holes_data<T>& poly) {
    o << "Polygon With Holes { ";
    for(typename polygon_45_with_holes_data<T>::iterator_type itr = poly.begin(); 
        itr != poly.end(); ++itr) {
      if(itr != poly.begin()) o << ", ";
      o << (*itr).get(HORIZONTAL) << " " << (*itr).get(VERTICAL);
    } o << " { ";
    for(typename polygon_45_with_holes_data<T>::iterator_holes_type itr = poly.begin_holes();
        itr != poly.end_holes(); ++itr) {
      o << (*itr);
    }
    o << " } } ";
    return o;
  }

}
#endif

