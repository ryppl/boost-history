/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
#ifndef GTL_POLYGON_45_CONCEPT_HPP
#define GTL_POLYGON_45_CONCEPT_HPP
namespace gtl {


struct polygon_45_concept : polygon_90_concept {
  inline polygon_45_concept() {}

  template <typename iterator_type>
  typename coordinate_traits<
    typename point_traits<
      typename std::iterator_traits<iterator_type>::value_type>::coordinate_type>::area_type
  static point_sequence_area(iterator_type begin_range, iterator_type end_range) {
    typedef typename std::iterator_traits<iterator_type>::value_type point_type;
    typedef typename point_traits<point_type>::coordinate_type Unit;
    typedef typename coordinate_traits<Unit>::area_type area_type;
    if(begin_range == end_range) return area_type(0);
    point_type first = *begin_range;
    point_type previous = first;
    ++begin_range;
    // Initialize trapezoid base line
    area_type y_base = (area_type)point_concept::y(first);
    // Initialize area accumulator

    area_type area(0);
    while (begin_range != end_range) {
      area_type x1 = (area_type)point_concept::x(previous);
      area_type x2 = (area_type)point_concept::x(*begin_range);
      if(x1 != x2) {
        // do trapezoid area accumulation
        area += (x2 - x1) * (((area_type)point_concept::y(*begin_range) - y_base) +
                             ((area_type)point_concept::y(previous) - y_base)) / 2;
      }
      previous = *begin_range;
      // go to next point
      ++begin_range;
    }
    //wrap around to evaluate the edge between first and last if not closed
    if(first != previous) {
      area_type x1 = (area_type)point_concept::x(previous);
      area_type x2 = (area_type)point_concept::x(first);
      area += (x2 - x1) * (((area_type)point_concept::y(first) - y_base) +
                           ((area_type)point_concept::y(previous) - y_base)) / 2;
    }
    return area;
  }

  template <typename polygon_type_1, typename polygon_type_2>
  static polygon_type_1& assign(polygon_type_1& lvalue, const polygon_type_2& rvalue) {
    set(lvalue, begin(rvalue), end(rvalue));
    return lvalue;
  }

  template <typename polygon_type>
  static direction_1d winding(const polygon_type& polygon){
    //rewrite winding algorithm for non-manhattan
    winding_direction wd = polygon_traits<polygon_type>::winding(polygon);
    if(wd != unknown_winding) {
      return wd == clockwise_winding ? CLOCKWISE: COUNTERCLOCKWISE;
    }
    return point_sequence_area(begin(polygon), end(polygon)) < 0 ? COUNTERCLOCKWISE : CLOCKWISE;
  }

  template <typename polygon_type>
  static rectangle_data<typename polygon_traits<polygon_type>::coordinate_type> 
  bounding_box(const polygon_type& polygon) {
    typedef typename polygon_traits<polygon_type>::coordinate_type coordinate_type;
    typedef typename polygon_traits<polygon_type>::iterator_type iterator;
    coordinate_type xmin = 0;
    coordinate_type ymin = 0;
    coordinate_type xmax = 0;
    coordinate_type ymax = 0;
    bool first_iteration = true;
    iterator itr_end = end(polygon);
    for(iterator itr = begin(polygon); itr != itr_end; ++itr) {
      coordinate_type x = point_concept::x(*itr);
      coordinate_type y = point_concept::y(*itr);
      if(first_iteration) {
        xmin = xmax = x;
        ymin = ymax = x;
        first_iteration = false;
      }
      xmin = std::min(xmin, x);
      xmax = std::max(xmax, x);
      ymin = std::min(ymin, y);
      ymax = std::max(ymax, y);
    }
    typedef rectangle_data<coordinate_type> rectangle_type;
    rectangle_type return_value = rectangle_concept::construct<rectangle_type>(xmin, ymin, xmax, ymax);
    return return_value;
  }

  template <typename polygon_type>
  static typename center_type<polygon_type>::type
  center(const polygon_type& polygon) {
    return rectangle_concept::center(bounding_box(polygon));
  }

  template <typename polygon_type>
  static typename coordinate_traits<typename polygon_traits<polygon_type>::coordinate_type>::area_type
  area(const polygon_type& polygon) {
    //rewrite for non-manhattan
    typename polygon_traits<polygon_type>::coordinate_type result = 
      point_sequence_area(begin(polygon), end(polygon));
    if(result < 0) result *= -1;
    return result;
  }

  /// get the perimeter of the polygon
  template <typename polygon_type>
  static typename distance_type<polygon_type>::type 
  perimeter(const polygon_type& polygon) {
    typedef typename polygon_traits<polygon_type>::coordinate_type coordinate_type;
    typedef typename polygon_traits<polygon_type>::iterator_type iterator;
    typedef typename std::iterator_traits<iterator>::value_type point_type;
    typename distance_type<polygon_type>::type return_value = 0;
    point_type previous_point, first_point;
    iterator itr = begin(polygon);
    iterator itr_end = end(polygon);
    if(itr == itr_end) return return_value;
    previous_point = first_point = *itr;
    ++itr;
    for( ; itr != itr_end; ++itr) {
      point_type current_point = *itr;
      return_value += point_concept::distance(current_point, previous_point);
      previous_point = current_point;
    }
    return_value += point_concept::distance(previous_point, first_point);
    return return_value;
  }

  /// check if point is inside polygon 
  template <typename polygon_type, typename point_type>
  static bool contains(const polygon_type& polygon, const point_type& point, 
                       bool consider_touch, point_concept pc) {
    throw(std::string("function not yet implemented\n"));
    return false;
  }

  template <typename polygon_type, typename coordinate_type_1, typename coordinate_type_2>
  static polygon_type& move(polygon_type& polygon, coordinate_type_1 x_displacement, coordinate_type_2 y_displacement) {
    typedef typename polygon_traits<polygon_type>::iterator_type iterator_type;
    std::vector<typename std::iterator_traits<iterator_type>::value_type> points;
    points.reserve(size(polygon));
    for(typename polygon_traits<polygon_type>::iterator_type iter = begin(polygon); 
        iter != end(polygon); ++iter) {
      typename std::iterator_traits<iterator_type>::value_type vertex = *iter;
      point_concept::x(vertex, point_concept::x(vertex) + x_displacement);
      point_concept::y(vertex, point_concept::y(vertex) + y_displacement);
      points.push_back(vertex);
    }
    set(polygon, points.begin(), points.end());
    return polygon;
  }
  
  /// move polygon by delta in orient
  template <typename polygon_type, typename coordinate_type_1>
  static polygon_type& move(polygon_type& polygon, orientation_2d orient, coordinate_type_1 displacement) {
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

