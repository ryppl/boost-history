/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
#ifndef GLT_POINT_3D_CONCEPT_HPP
#define GLT_POINT_3D_CONCEPT_HPP
namespace gtl {
struct point_3d_concept : point_concept {
  point_3d_concept() {}

  template <orientation_3d_enum orient, typename T>
  static inline typename point_3d_traits<T>::coordinate_type get(const T& point) {
    return point_3d_traits<T>::get(point, orient); }

  template <typename T>
  static inline typename point_3d_traits<T>::coordinate_type get(const T& point, orientation_3d orient) {
    return point_3d_traits<T>::get(point, orient); }
  
  template <orientation_3d_enum orient, typename T, typename coordinate_type>
  static inline void set(T& point, coordinate_type value) {
    point_traits<T>::set(point, orient, value); }
  template <typename T, typename coordinate_type>
  static inline void set(T& point, orientation_3d orient, coordinate_type value) {
    point_3d_traits<T>::set(point, orient, value); }

  template <typename T, typename coordinate_type1, typename coordinate_type2, typename coordinate_type3>
  static inline T construct(coordinate_type1 x_value, 
                            coordinate_type2 y_value, 
                            coordinate_type3 z_value) {
    return point_3d_traits<T>::construct(x_value, y_value, z_value); }

  template <typename point_3d_type_1, typename point_3d_type_2>
  static point_3d_type_1& assign(point_3d_type_1& lvalue, const point_3d_type_2& rvalue) {
    set(lvalue, HORIZONTAL, get(rvalue, HORIZONTAL));
    set(lvalue, VERTICAL, get(rvalue, VERTICAL));
    set(lvalue, PROXIMAL, get(rvalue, PROXIMAL));
    return lvalue;
  }


};

}
#endif

