/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
struct point_concept {
  point_concept() {}

  template <typename point_type>
  struct registration {
    typedef typename point_traits<point_type>::coordinate_type component_type;
    typedef typename point_traits<point_type>::coordinate_type coordinate_type;
  };

  template <typename T>
  static inline typename point_traits<T>::coordinate_type get(const T& point, orientation_2d orient) {
    return point_traits<T>::get(point, orient);
  }
  
  template <orientation_2d_enum orient, typename T>
  static inline typename point_traits<T>::coordinate_type get(const T& point) {
    return get(point, orient); 
  }
  
  template <typename T, typename coordinate_type>
  static inline void set(T& point, orientation_2d orient, coordinate_type value) {
    point_traits<T>::set(point, orient, value);
  }
  
  template <orientation_2d_enum orient, typename T, typename coordinate_type>
  static inline void set(T& point, coordinate_type value) {
    set(point, orient, value);
  }

  template <typename point_type>
  static typename point_traits<point_type>::coordinate_type 
  x(point_type& point) {
    return get<HORIZONTAL>(point);
  }

  template <typename point_type>
  static typename point_traits<point_type>::coordinate_type 
  y(point_type& point) {
    return get<VERTICAL>(point);
  }

  template <typename point_type, typename coordinate_type>
  static void x(point_type& point, coordinate_type value) {
    set<HORIZONTAL>(point, value);
  }

  template <typename point_type, typename coordinate_type>
  static void y(point_type& point, coordinate_type value) {
    set<VERTICAL>(point, value);
  }

  template <typename T, typename coordinate_type1, typename coordinate_type2>
  static inline T construct(coordinate_type1 x_value, coordinate_type2 y_value) {
    return point_traits<T>::construct(x_value, y_value); 
  }

  template <typename point_type_1, typename point_type_2>
  static point_type_1& assign(point_type_1& lvalue, const point_type_2& rvalue) {
    set(lvalue, HORIZONTAL, get(rvalue, HORIZONTAL));
    set(lvalue, VERTICAL, get(rvalue, VERTICAL));
    return lvalue;
  }

  template <typename T, typename T2>
  static inline bool equivalence(const T& point1, const T2& point2) {
    typename point_traits<T>::coordinate_type x1 = get<HORIZONTAL>(point1);
    typename point_traits<T2>::coordinate_type x2 = get(point2, HORIZONTAL);
    typename point_traits<T>::coordinate_type y1 = get(point1, VERTICAL);
    typename point_traits<T2>::coordinate_type y2 = get<VERTICAL>(point2);
    return x1 == x2 && y1 == y2;
  }

  template <typename point_type_1, typename point_type_2>
  static typename point_traits<point_type_1>::coordinate_type
  manhattan_distance(const point_type_1& point1, const point_type_2& point2) {
    return distance(point1, point2, HORIZONTAL) + distance(point1, point2, VERTICAL);
  }

  template <typename point_type_1, typename point_type_2>
  static typename point_traits<point_type_1>::coordinate_type
  distance(const point_type_1& point1, const point_type_2& point2, orientation_2d orient) {
    typename point_traits<point_type_1>::coordinate_type return_value =
      get(point1, orient) - get(point2, orient);
    return return_value < 0 ? -return_value : return_value;
  }

};


