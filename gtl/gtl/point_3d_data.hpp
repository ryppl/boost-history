/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
#ifndef GTL_POINT_3D_DATA_HPP
#define GTL_POINT_3D_DATA_HPP
namespace gtl {
template <typename T>
class point_3d_data {
public:
  typedef T coordinate_type;
  inline point_3d_data(){} 
  inline point_3d_data(coordinate_type x, coordinate_type y) {
    coords_[HORIZONTAL] = x; coords_[VERTICAL] = y; coords_[PROXIMAL] = 0; }
  inline point_3d_data(coordinate_type x, coordinate_type y, coordinate_type z) {
    coords_[HORIZONTAL] = x; coords_[VERTICAL] = y; coords_[PROXIMAL] = z; }
  inline point_3d_data(const point_3d_data& that) { (*this) = that; }
  inline point_3d_data& operator=(const point_3d_data& that) {
    coords_[0] = that.coords_[0]; coords_[1] = that.coords_[1]; 
    coords_[2] = that.coords_[2]; return *this; }
  template <typename T2>
  inline point_3d_data& operator=(const T2& rvalue);
  inline coordinate_type get(orientation_3d orient) const {
    return coords_[orient.to_int()]; }
  inline void set(orientation_3d orient, coordinate_type value) {
    coords_[orient.to_int()] = value; }
private:
  coordinate_type coords_[3]; 
};

}
#endif


