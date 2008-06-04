/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
#ifndef GTL_POLYGON_45_WITH_HOLES_CONCEPT_HPP
#define GTL_POLYGON_45_WITH_HOLES_CONCEPT_HPP
namespace gtl {

struct polygon_45_with_holes_concept : virtual polygon_45_concept, polygon_90_with_holes_concept {
public:
  inline polygon_45_with_holes_concept() {}

  template <typename polygon_with_holes_type_1, typename polygon_with_holes_type_2>
  static polygon_with_holes_type_1& assign(polygon_with_holes_type_1& lvalue, const polygon_with_holes_type_2& rvalue) {
    polygon_45_concept::assign(lvalue, rvalue);
    set_holes(lvalue, begin_holes(rvalue), end_holes(rvalue));
    return lvalue;
  }

  template <typename polygon_with_holes_type>
  static typename polygon_with_holes_traits<polygon_with_holes_type>::coordinate_type 
  area(const polygon_with_holes_type& polygon) {
    typename polygon_traits<polygon_with_holes_type>::coordinate_type retval = polygon_45_concept::area(polygon);
    typename polygon_with_holes_traits<polygon_with_holes_type>::iterator_holes_type b, e;
    e = end_holes(polygon);
    for(b = begin_holes(polygon); b != e; ++b) {
      retval -= polygon_45_concept::area(*b);
    }
  }

  /// check if point is inside polygon 
  template <typename polygon_with_holes_type, typename point_type>
  static bool contains(const polygon_with_holes_type& polygon, const point_type& point, 
                       bool consider_touch, point_concept pc) {
    typename polygon_with_holes_traits<polygon_with_holes_type>::iterator_holes_type b, e;
    e = end_holes(polygon);
    for(b = begin_holes(polygon); b != e; ++b) {
      if(polygon_45_concept::contains(*b, point, !consider_touch, pc)) {
        return false;
      }
    }
    return polygon_45_concept::contains(polygon, point, consider_touch, pc);
  }

  /// get the perimeter of the polygon
  template <typename polygon_with_holes_type>
  static typename polygon_traits<polygon_with_holes_type>::coordinate_type
  perimeter(const polygon_with_holes_type& polygon) {
    typename polygon_traits<polygon_with_holes_type>::coordinate_type retval = polygon_45_concept::perimeter(polygon);
    typename polygon_with_holes_traits<polygon_with_holes_type>::iterator_holes_type b, e;
    e = end_holes(polygon);
    for(b = begin_holes(polygon); b != e; ++b) {
      retval -= polygon_45_concept::perimeter(*b);
    }
  }

  template <typename polygon_with_holes_type, typename coordinate_type_1, typename coordinate_type_2>
  static polygon_with_holes_type& move(polygon_with_holes_type& polygon, 
                                       coordinate_type_1 x_displacement, coordinate_type_2 y_displacement) {
    typedef typename polygon_with_holes_traits<polygon_with_holes_type>::hole_type hole_type;
    std::vector<hole_type> holes;
    holes.resize(size_holes(polygon));
    typename polygon_with_holes_traits<polygon_with_holes_type>::iterator_holes_type b, e;
    e = end_holes(polygon);
    for(b = begin_holes(polygon); b != e; ++b) {
      holes.push_back(*b);
      polygon_45_concept::move(holes.back(), x_displacement, y_displacement);
    }
    set_holes(polygon, holes.begin(), holes.end());
    polygon_45_concept::move(polygon, x_displacement, y_displacement);
    return polygon;
  }

  /// move polygon by delta in orient
  template <typename polygon_with_holes_type, typename coordinate_type_1>
  static polygon_with_holes_type& move(polygon_with_holes_type& polygon, 
                                       orientation_2d orient, coordinate_type_1 displacement) {
    if(orient == HORIZONTAL) {
      move(polygon, displacement, 0);
    } else {
      move(polygon, 0, displacement);
    }
    return polygon;
  }
};

}
#endif

