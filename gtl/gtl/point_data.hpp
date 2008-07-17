/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
#ifndef GTLPOINT_DATA_HPP
#define GTLPOINT_DATA_HPP
namespace gtl {

struct point_concept;

template <typename T>
class point_data {
public:
  typedef point_concept geometry_type;
  typedef T coordinate_type;
  inline point_data(){} 
  inline point_data(coordinate_type x, coordinate_type y) {
    coords_[HORIZONTAL] = x; coords_[VERTICAL] = y; 
  }
  inline point_data(const point_data& that) { (*this) = that; }
  inline point_data& operator=(const point_data& that) {
    coords_[0] = that.coords_[0]; coords_[1] = that.coords_[1]; return *this; 
  }
  template <typename T2>
  inline point_data& operator=(const T2& rvalue);
  inline bool operator==(const point_data& that) const {
    return coords_[0] == that.coords_[0] && coords_[1] == that.coords_[1];
  }
  inline bool operator!=(const point_data& that) const {
    return !((*this) == that);
  }
  inline bool operator<(const point_data& that) const {
    return coords_[0] < that.coords_[0] ||
      (coords_[0] == that.coords_[0] && coords_[1] < that.coords_[1]);
  }
  inline coordinate_type get(orientation_2d orient) const {
    return coords_[orient.to_int()]; 
  }
  inline void set(orientation_2d orient, coordinate_type value) {
    coords_[orient.to_int()] = value; 
  }
private:
  coordinate_type coords_[2]; 
};

template <class T>
std::ostream& operator << (std::ostream& o, const point_data<T>& r)
{
  return o << r.get(HORIZONTAL) << ' ' << r.get(VERTICAL);
}

}
#endif

