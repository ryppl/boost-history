/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
#ifndef GLT_POINT_3D_CONCEPT_HPP
#define GLT_POINT_3D_CONCEPT_HPP
namespace gtl {
  struct point_3d_concept {};
 
  template <typename T>
  struct is_point_3d_concept {};
  template <>
  struct is_point_3d_concept<point_3d_concept> { typedef void type; };
  //template <>
  //struct is_point_concept<point_3d_concept> { typedef void type; };

  template <typename T>
  struct is_mutable_point_3d_concept {};
  template <>
  struct is_mutable_point_3d_concept<point_3d_concept> { typedef void type; };

  template <typename T>
  typename requires_1< typename is_point_3d_concept<typename geometry_concept<T>::type>::type, 
                       typename point_3d_traits<T>::coordinate_type >::type 
  get(const T& point, orientation_3d orient) { return point_3d_traits<T>::get(point, orient); }
  
  template <typename T, typename coordinate_type>
  typename requires_1< typename is_mutable_point_3d_concept<typename geometry_concept<T>::type>::type, void>::type
  set(T& point, orientation_3d orient, coordinate_type value) { point_3d_mutable_traits<T>::set(point, orient, value); }
  template <typename T, typename coordinate_type>
  typename requires_1< typename is_mutable_point_3d_concept<typename geometry_concept<T>::type>::type, void>::type
  set(T& point, orientation_2d orient, coordinate_type value) { point_3d_mutable_traits<T>::set(point, orient, value); }

  template <typename T, typename coordinate_type1, typename coordinate_type2, typename coordinate_type3>
  typename requires_1< typename is_mutable_point_3d_concept<typename geometry_concept<T>::type>::type, T>::type
  construct(coordinate_type1 x_value, coordinate_type2 y_value, coordinate_type3 z_value) {
    return point_3d_mutable_traits<T>::construct(x_value, y_value, z_value); }

  template <typename point_3d_type_1, typename point_3d_type_2>
  typename requires_2< typename is_mutable_point_3d_concept<typename geometry_concept<point_3d_type_1>::type>::type, 
                       typename is_point_3d_concept<typename geometry_concept<point_3d_type_2>::type>::type, 
                       point_3d_type_1>::type &
  assign(point_3d_type_1& lvalue, const point_3d_type_2& rvalue) {
    set(lvalue, HORIZONTAL, get(rvalue, HORIZONTAL));
    set(lvalue, VERTICAL, get(rvalue, VERTICAL));
    set(lvalue, PROXIMAL, get(rvalue, PROXIMAL));
    return lvalue;
  }

  template <typename point_type>
  typename requires_1< typename is_point_3d_concept<typename geometry_concept<point_type>::type>::type, 
                       typename point_3d_traits<point_type>::coordinate_type >::type 
  z(const point_type& point) { return get(point, PROXIMAL); }

  template <typename point_type, typename coordinate_type>
  typename requires_1< typename is_mutable_point_3d_concept<typename geometry_concept<point_type>::type>::type, void>::type
  x(point_type& point, coordinate_type value) { set(point, HORIZONTAL, value); }
  template <typename point_type, typename coordinate_type>
  typename requires_1< typename is_mutable_point_3d_concept<typename geometry_concept<point_type>::type>::type, void>::type
  y(point_type& point, coordinate_type value) { set(point, VERTICAL, value); }
  template <typename point_type, typename coordinate_type>
  typename requires_1< typename is_mutable_point_3d_concept<typename geometry_concept<point_type>::type>::type, void>::type
  z(point_type& point, coordinate_type value) { set(point, PROXIMAL, value); }

  template <typename T, typename T2>
  typename requires_2< typename is_same_type_SFINAE<point_3d_concept, typename geometry_concept<T>::type>::type, 
                       typename is_same_type_SFINAE<point_3d_concept, typename geometry_concept<T2>::type>::type,
                       bool>::type
  equivalence(const T& point1, const T2& point2) {
    return x(point1) == x(point2) && y(point1) == y(point2) && z(point1) == z(point2);
  }

  template <typename point_type_1, typename point_type_2>
  typename requires_2< typename is_same_type_SFINAE<point_3d_concept, typename geometry_concept<point_type_1>::type>::type, 
                       typename is_same_type_SFINAE<point_3d_concept, typename geometry_concept<point_type_2>::type>::type,
                       typename coordinate_traits<typename point_3d_traits<point_type_1>::coordinate_type>::coordinate_difference>::type
  manhattan_distance(const point_type_1& point1, const point_type_2& point2) {
    return euclidean_distance(point1, point2, HORIZONTAL) + euclidean_distance(point1, point2, VERTICAL) 
      + euclidean_distance(point1, point2, PROXIMAL);
  }

  template <typename point_type_1, typename point_type_2>
  typename requires_2< typename is_point_3d_concept<typename geometry_concept<point_type_1>::type>::type, 
                       typename is_point_3d_concept<typename geometry_concept<point_type_2>::type>::type, 
                       typename coordinate_traits<typename point_3d_traits<point_type_1>::coordinate_type>::coordinate_difference>::type
  euclidean_distance(const point_type_1& point1, const point_type_2& point2, orientation_3d orient) {
    typedef typename coordinate_traits<typename point_3d_traits<point_type_1>::coordinate_type>::coordinate_difference return_type;
    return_type return_value =
      (return_type)get(point1, orient) - (return_type)get(point2, orient);
    return return_value < 0 ? -return_value : return_value;
  }

  template <typename point_type_1, typename point_type_2>
  typename requires_2< typename is_same_type_SFINAE<point_3d_concept, typename geometry_concept<point_type_1>::type>::type, 
                       typename is_same_type_SFINAE<point_3d_concept, typename geometry_concept<point_type_2>::type>::type,
                       typename coordinate_traits<typename point_3d_traits<point_type_1>::coordinate_type>::coordinate_distance>::type
  euclidean_distance(const point_type_1& point1, const point_type_2& point2) {
    typedef typename coordinate_traits<typename point_3d_traits<point_type_1>::coordinate_type>::coordinate_distance return_value;
    return_value pdist = (return_value)euclidean_distance(point1, point2, PROXIMAL);
    pdist *= pdist;
    return sqrt((return_value)distance_squared(point1, point2) + pdist);
  }
  
  template <typename point_type_1, typename point_type_2>
  typename requires_2< typename is_mutable_point_3d_concept<typename geometry_concept<point_type_1>::type>::type, 
                       typename is_same_type_SFINAE<point_3d_concept, typename geometry_concept<point_type_2>::type>::type,
                       point_type_1>::type &
  convolve(point_type_1& lvalue, const point_type_2& rvalue) {
    x(lvalue, x(lvalue) + x(rvalue));
    y(lvalue, y(lvalue) + y(rvalue));
    z(lvalue, z(lvalue) + z(rvalue));
    return lvalue;
  }
 
  template <typename point_type_1, typename point_type_2>
  typename requires_2< typename is_mutable_point_3d_concept<typename geometry_concept<point_type_1>::type>::type, 
                       typename is_same_type_SFINAE<point_3d_concept, typename geometry_concept<point_type_2>::type>::type,
                       point_type_1>::type &
  deconvolve(point_type_1& lvalue, const point_type_2& rvalue) {
    x(lvalue, x(lvalue) - x(rvalue));
    y(lvalue, y(lvalue) - y(rvalue));
    z(lvalue, z(lvalue) - z(rvalue));
    return lvalue;
  }

  template <typename point_type>
  typename requires_1< typename is_mutable_point_3d_concept<typename geometry_concept<point_type>::type>::type, 
                       point_type>::type &
  scale_up(point_type& point, 
           typename coordinate_traits<typename point_3d_traits<point_type>::coordinate_type>::unsigned_area_type factor) {
    x(point, x(point) * (typename point_3d_traits<point_type>::coordinate_type)factor);
    y(point, y(point) * (typename point_3d_traits<point_type>::coordinate_type)factor);
    z(point, z(point) * (typename point_3d_traits<point_type>::coordinate_type)factor);
    return point;
  }

  template <typename point_type>
  typename requires_1< typename is_mutable_point_3d_concept<typename geometry_concept<point_type>::type>::type, 
                       point_type>::type &
  scale_down(point_type& point, 
             typename coordinate_traits<typename point_3d_traits<point_type>::coordinate_type>::unsigned_area_type factor) {
    typedef typename point_3d_traits<point_type>::coordinate_type Unit;
    typedef typename coordinate_traits<Unit>::coordinate_distance dt;
    x(point, scaling_policy<Unit>::round((dt)(x(point)) / (dt)factor)); 
    y(point, scaling_policy<Unit>::round((dt)(y(point)) / (dt)factor)); 
    z(point, scaling_policy<Unit>::round((dt)(z(point)) / (dt)factor)); 
    return point;
  }

  template <typename point_type, typename scaling_type>
  typename requires_1< typename is_mutable_point_3d_concept<typename geometry_concept<point_type>::type>::type, 
                       point_type>::type &
  scale(point_type& point, 
        const scaling_type& scaling) {
    typedef typename point_3d_traits<point_type>::coordinate_type Unit;
    Unit x_(x(point)), y_(y(point)), z_(z(point));
    scaling.scale(x_, y_, z_);
    x(point, x_);
    y(point, y_);
    z(point, z_);
    return point;
  }

  template <typename point_type, typename transformation_type>
  typename requires_1< typename is_mutable_point_3d_concept<typename geometry_concept<point_type>::type>::type, 
                       point_type>::type &
  transform(point_type& point, const transformation_type& transformation) {
    typedef typename point_3d_traits<point_type>::coordinate_type Unit;
    Unit x_(x(point)), y_(y(point)), z_(z(point));
    transformation.transform(x_, y_, z_);
    x(point, x_);
    y(point, y_);
    z(point, z_);
    return point;
  }

  template <typename T>
  struct geometry_concept<point_3d_data<T> > {
    typedef point_3d_concept type;
  };

}
#endif

