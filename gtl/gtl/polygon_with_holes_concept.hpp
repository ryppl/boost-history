/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/

#ifndef GTL_POLYGON_WITH_HOLES_CONCEPT_HPP
#define GTL_POLYGON_WITH_HOLES_CONCEPT_HPP
namespace gtl {

struct polygon_with_holes_concept : public polygon_concept {
public:
  //inline polygon_with_holes_concept() {}

  template <typename T>
  struct coordinate_type {
    typedef typename polygon_traits<T>::coordinate_type type;
  };

  template <typename T>
  struct area_type {
    typedef typename polygon_traits<T>::area_type type;
  };

  template <typename T>
  struct coordinate_difference {
    typedef typename coordinate_traits<typename coordinate_type<T>::type>::coordinate_difference type;
  };

  template <typename T>
  struct coordinate_distance {
    typedef typename coordinate_traits<typename coordinate_type<T>::type>::coordinate_distance type;
  };

  template <typename T>
  struct point_type {
    typedef typename polygon_traits<T>::iterator_type iterator;
    typedef typename std::iterator_traits<iterator>::value_type type;
  };

  template<typename polygon_with_holes_type, typename hole_iterator_type>
  static void set_holes(polygon_with_holes_type& polygon, hole_iterator_type holes_begin, hole_iterator_type holes_end) {
    polygon_with_holes_traits<polygon_with_holes_type>::set_holes(polygon, holes_begin, holes_end);
  }

  template <typename polygon_with_holes_type>
  static typename polygon_with_holes_traits<polygon_with_holes_type>::iterator_holes_type 
  begin_holes(const polygon_with_holes_type& polygon) {
    return polygon_with_holes_traits<polygon_with_holes_type>::begin_holes(polygon);
  }
  
  template <typename polygon_with_holes_type>
  static typename polygon_with_holes_traits<polygon_with_holes_type>::iterator_holes_type 
  end_holes(const polygon_with_holes_type& polygon) {
    return polygon_with_holes_traits<polygon_with_holes_type>::end_holes(polygon);
  }
  
  template <typename polygon_with_holes_type_1, typename polygon_with_holes_type_2>
  static polygon_with_holes_type_1 copy_construct(const polygon_with_holes_type_2& polygon) { 
    polygon_with_holes_type_1 retval;
    set(retval, polygon_concept::begin(polygon), polygon_concept::end(polygon));
    set_holes(retval, begin_holes(polygon), end_holes(polygon));
    return retval;
  }

  template <typename polygon_with_holes_type>
  static std::size_t size_holes(const polygon_with_holes_type& polygon) {
    return polygon_with_holes_traits<polygon_with_holes_type>::size_holes(polygon);
  }

  //needed to disambiguate between multiple base class implementations of same function
  template <typename polygon_with_holes_type_1, typename polygon_with_holes_type_2>
  static polygon_with_holes_type_1& assign(polygon_with_holes_type_1& lvalue, const polygon_with_holes_type_2& rvalue) {
    return polygon_45_with_holes_concept::assign(lvalue, rvalue);
  }

  //scaling down differs from the implementation in 45 polygons by not snapping the result to 45
  template <typename polygon_type>
  static polygon_type& scale_down(polygon_type& polygon, 
                                  typename coordinate_traits<typename coordinate_type<polygon_type>::type>::unsigned_area_type factor) {
    polygon_90_with_holes_concept::scale_down(polygon, factor);
    return polygon;
  }

  template <typename polygon_with_holes_type>
  static typename area_type<polygon_with_holes_type>::type
  area(const polygon_with_holes_type& polygon) {
    return polygon_45_with_holes_concept::area(polygon);
  }

  /// check if point is inside polygon 
  template <typename polygon_with_holes_type, typename point_type>
  static bool contains(const polygon_with_holes_type& polygon, const point_type& point, 
                       bool consider_touch, point_concept pc) {
    return polygon_45_with_holes_concept::contains(polygon, point, consider_touch, pc);
  }

  /// get the perimeter of the polygon
  template <typename polygon_with_holes_type>
  static typename coordinate_distance<polygon_with_holes_type>::type 
  perimeter(const polygon_with_holes_type& polygon) {
    return polygon_45_with_holes_concept::perimeter(polygon);
  }

  template <typename polygon_with_holes_type, typename coordinate_type_1, typename coordinate_type_2>
  static polygon_with_holes_type& move(polygon_with_holes_type& polygon, 
                                       coordinate_type_1 x_displacement, coordinate_type_2 y_displacement) {
    return polygon_45_with_holes_concept::move(polygon, x_displacement, y_displacement);
  }

  /// move polygon by delta in orient
  template <typename polygon_with_holes_type, typename coordinate_type_1>
  static polygon_with_holes_type& move(polygon_with_holes_type& polygon, 
                                       orientation_2d orient, coordinate_type_1 displacement) {
    return polygon_45_with_holes_concept::move(polygon, orient, displacement);
  }

  template <typename polygon_type>
  static direction_1d winding(const polygon_type& polygon) {
    return polygon_45_concept::winding(polygon);
  }

  template <typename point_type, typename polygon_type>
  static bool
  center(point_type& center_point, const polygon_type& polygon) {
    return polygon_45_concept::center(center_point, polygon);
  }

  template <typename polygon_type, typename rectangle_type>
  static bool
  extents(rectangle_type& bounding_box, const polygon_type& polygon) {
    return polygon_45_concept::extents(bounding_box, polygon);
  }

  template <typename polygon_type>
  static polygon_type& scale_up(polygon_type& polygon, 
                                typename coordinate_traits<typename coordinate_type<polygon_type>::type>::unsigned_area_type factor) {
    return polygon_90_with_holes_concept::scale_up(polygon, factor);
  }

  template <typename polygon_type, typename scaling_type>
  static polygon_type& scale(polygon_type& polygon, 
                              const scaling_type& scaling) {
    return polygon_90_with_holes_concept::scale(polygon, scaling);
  }

  template <typename polygon_type, typename transformation_type>
  static polygon_type& transform(polygon_type& polygon,
                               const transformation_type& transformation) {
    return polygon_90_with_holes_concept::transform(polygon, transformation);
  }

  template <typename polygon_type>
  static polygon_type&
  snap_to_45(polygon_type& polygon) {
    return polygon_45_with_holes_concept::snap_to_45(polygon);
  }

  template <typename polygon_type>
  static bool
  is_45(const polygon_type& polygon) {
    return polygon_45_with_holes_concept::is_45(polygon);
  }

};

}
#endif
