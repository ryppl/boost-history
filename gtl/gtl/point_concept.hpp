/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
#ifndef GTL_POINT_CONCEPT_HPP
#define GTL_POINT_CONCEPT_HPP
#include "isotropy.hpp"
#include "point_data.hpp"
#include "point_traits.hpp"

namespace gtl {
  struct point_concept {};
 
  template <typename T>
  struct is_point_concept {};
  template <>
  struct is_point_concept<point_concept> { typedef void type; };

  struct point_3d_concept;
  template <>
  struct is_point_concept<point_3d_concept> { typedef void type; };

  template <typename T>
  struct is_mutable_point_concept {};
  template <>
  struct is_mutable_point_concept<point_concept> { typedef void type; };


  template <typename T>
  typename requires_1< typename is_point_concept<typename geometry_concept<T>::type>::type, 
                       typename point_traits<T>::coordinate_type >::type 
  get(const T& point, orientation_2d orient) {
    return point_traits<T>::get(point, orient);
  }
  
  template <typename T, typename coordinate_type>
  typename requires_1<typename is_mutable_point_concept<typename geometry_concept<T>::type>::type, void>::type 
  set(T& point, orientation_2d orient, coordinate_type value) {
    point_mutable_traits<T>::set(point, orient, value);
  }
  
  template <typename T, typename coordinate_type1, typename coordinate_type2>
  typename requires_1<typename is_mutable_point_concept<typename geometry_concept<T>::type>::type,
                      T>::type 
  construct(coordinate_type1 x_value, coordinate_type2 y_value) {
    return point_mutable_traits<T>::construct(x_value, y_value); 
  }

  template <typename T1, typename T2>
  typename requires_2<
    typename is_mutable_point_concept<typename geometry_concept<T1>::type>::type,
    typename is_point_concept<typename geometry_concept<T2>::type>::type,
    T1>::type &
  assign(T1& lvalue, const T2& rvalue) {
    set(lvalue, HORIZONTAL, get(rvalue, HORIZONTAL));
    set(lvalue, VERTICAL, get(rvalue, VERTICAL));
    return lvalue;
  }

  template <typename point_type>
  typename requires_1< typename is_point_concept<typename geometry_concept<point_type>::type>::type, 
                       typename point_traits<point_type>::coordinate_type >::type 
  x(const point_type& point) {
    return get(point, HORIZONTAL);
  }

  template <typename point_type>
  typename requires_1< typename is_point_concept<typename geometry_concept<point_type>::type>::type, 
                       typename point_traits<point_type>::coordinate_type >::type 
  y(const point_type& point) {
    return get(point, VERTICAL);
  }

  template <typename point_type, typename coordinate_type>
  typename requires_1<typename is_mutable_point_concept<typename geometry_concept<point_type>::type>::type,
                      void>::type 
  x(point_type& point, coordinate_type value) {
    set(point, HORIZONTAL, value);
  }

  template <typename point_type, typename coordinate_type>
  typename requires_1<typename is_mutable_point_concept<typename geometry_concept<point_type>::type>::type,
                      void>::type 
  y(point_type& point, coordinate_type value) {
    set(point, VERTICAL, value);
  }

  template <typename T, typename T2>
  typename requires_2<typename is_same_type_SFINAE<point_concept, typename geometry_concept<T>::type>::type,
                      typename is_point_concept<typename geometry_concept<T2>::type>::type,
                      bool>::type
  equivalence(const T& point1, const T2& point2) {
    typename point_traits<T>::coordinate_type x1 = x(point1);
    typename point_traits<T2>::coordinate_type x2 = get(point2, HORIZONTAL);
    typename point_traits<T>::coordinate_type y1 = get(point1, VERTICAL);
    typename point_traits<T2>::coordinate_type y2 = y(point2);
    return x1 == x2 && y1 == y2;
  }

  template <typename point_type_1, typename point_type_2>
  typename requires_2<typename is_same_type_SFINAE<point_concept, typename geometry_concept<point_type_1>::type>::type, 
                      typename is_point_concept<typename geometry_concept<point_type_2>::type>::type,
                      typename coordinate_traits<typename point_traits<point_type_1>::coordinate_type>::coordinate_difference>::type
  manhattan_distance(const point_type_1& point1, const point_type_2& point2) {
    return euclidean_distance(point1, point2, HORIZONTAL) + euclidean_distance(point1, point2, VERTICAL);
  }
  
  template <typename point_type_1, typename point_type_2>
  typename requires_2<typename is_point_concept<typename geometry_concept<point_type_1>::type>::type, 
                      typename is_point_concept<typename geometry_concept<point_type_2>::type>::type, 
                      typename coordinate_traits<typename point_traits<point_type_1>::coordinate_type>::coordinate_difference>::type
  euclidean_distance(const point_type_1& point1, const point_type_2& point2, orientation_2d orient) {
    typename coordinate_traits<typename point_traits<point_type_1>::coordinate_type>::coordinate_difference return_value =
      get(point1, orient) - get(point2, orient);
    return return_value < 0 ? -return_value : return_value;
  }
  
  template <typename point_type_1, typename point_type_2>
  typename requires_2<typename is_same_type_SFINAE<point_concept, typename geometry_concept<point_type_1>::type>::type,
                      typename is_same_type_SFINAE<point_concept, typename geometry_concept<point_type_2>::type>::type,
                      typename coordinate_traits<typename point_traits<point_type_1>::coordinate_type>::coordinate_distance>::type
  euclidean_distance(const point_type_1& point1, const point_type_2& point2) {
    typedef typename point_traits<point_type_1>::coordinate_type Unit;
    return sqrt((typename coordinate_traits<Unit>::coordinate_distance)(distance_squared(point1, point2)));
  }
  
  template <typename point_type_1, typename point_type_2>
  typename requires_2<typename is_point_concept<typename geometry_concept<point_type_1>::type>::type,
                      typename is_point_concept<typename geometry_concept<point_type_2>::type>::type,
                      typename coordinate_traits<typename point_traits<point_type_1>::coordinate_type>::coordinate_difference>::type
  distance_squared(const point_type_1& point1, const point_type_2& point2) {
    typedef typename point_traits<point_type_1>::coordinate_type Unit;
    typename coordinate_traits<Unit>::coordinate_difference dx = euclidean_distance(point1, point2, HORIZONTAL);
    typename coordinate_traits<Unit>::coordinate_difference dy = euclidean_distance(point1, point2, VERTICAL);
    dx *= dx;
    dy *= dy;
    return dx + dy;
  }

  template <typename point_type_1, typename point_type_2>
  typename requires_2<typename is_mutable_point_concept<typename geometry_concept<point_type_1>::type>::type, 
                      typename is_point_concept<typename geometry_concept<point_type_2>::type>::type, point_type_1>::type &
  convolve(point_type_1& lvalue, const point_type_2& rvalue) {
    x(lvalue, x(lvalue) + x(rvalue));
    y(lvalue, y(lvalue) + y(rvalue));
    return lvalue;
  }
  
  template <typename point_type_1, typename point_type_2>
  typename requires_2<typename is_mutable_point_concept<typename geometry_concept<point_type_1>::type>::type, 
                      typename is_point_concept<typename geometry_concept<point_type_2>::type>::type, point_type_1>::type &
  deconvolve(point_type_1& lvalue, const point_type_2& rvalue) {
    x(lvalue, x(lvalue) - x(rvalue));
    y(lvalue, y(lvalue) - y(rvalue));
    return lvalue;
  }
  
  template <typename point_type, typename coord_type>
  typename requires_1<typename is_mutable_point_concept<typename geometry_concept<point_type>::type>::type,
                      point_type>::type &
  scale_up(point_type& point, coord_type factor) {
    typedef typename point_traits<point_type>::coordinate_type Unit;
    x(point, x(point) * (Unit)factor);
    y(point, y(point) * (Unit)factor);
    return point;
  }

  template <typename point_type, typename coord_type>
  typename requires_1<typename is_mutable_point_concept<typename geometry_concept<point_type>::type>::type,
                      point_type>::type &
  scale_down(point_type& point, coord_type factor) {
    typedef typename point_traits<point_type>::coordinate_type Unit;
    typedef typename coordinate_traits<Unit>::coordinate_distance dt;
    x(point, scaling_policy<Unit>::round((dt)(x(point)) / (dt)factor)); 
    y(point, scaling_policy<Unit>::round((dt)(y(point)) / (dt)factor)); 
    return point;
  }

  template <typename point_type, typename scaling_type>
  typename requires_1<typename is_mutable_point_concept<typename geometry_concept<point_type>::type>::type,
                      point_type>::type &
  scale(point_type& point, 
        const scaling_type& scaling) {
    typedef typename point_traits<point_type>::coordinate_type Unit;
    Unit x_(x(point)), y_(y(point));
    scaling.scale(x_, y_);
    x(point, x_);
    y(point, y_);
    return point;
  }

  template <typename point_type, typename transformation_type>
  typename requires_1<typename is_mutable_point_concept<typename geometry_concept<point_type>::type>::type,
                      point_type>::type &
  transform(point_type& point, const transformation_type& transformation) {
    typedef typename point_traits<point_type>::coordinate_type Unit;
    Unit x_(x(point)), y_(y(point));
    transformation.transform(x_, y_);
    x(point, x_);
    y(point, y_);
    return point;
  }

  template <typename point_type>
  typename requires_1<typename is_mutable_point_concept<typename geometry_concept<point_type>::type>::type,
                      point_type>::type &
  move(point_type& point, orientation_2d orient,
       typename point_traits<point_type>::coordinate_type displacement) {
    typedef typename point_traits<point_type>::coordinate_type Unit;
    Unit v(get(point, orient));
    set(point, orient, v + displacement);
    return point;
  }

  template <class T>
  template <class T2>
  point_data<T>& point_data<T>::operator=(const T2& rvalue) {
    assign(*this, rvalue);
    return *this;
  }

  template <typename T>
  struct geometry_concept<point_data<T> > {
    typedef point_concept type;
  };
}
#endif

