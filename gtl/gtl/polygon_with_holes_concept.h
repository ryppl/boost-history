/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/

struct polygon_with_holes_concept {
public:

  template<typename polygon_with_holes_type, typename iterator_type>
  static void set(polygon_with_holes_type& polygon, iterator_type input_begin, iterator_type input_end) {
    polygon_traits<polygon_with_holes_type>::set(polygon, input_begin, input_end);
  }

  template<typename polygon_with_holes_type, typename rectangle_type>
  static void set(polygon_with_holes_type& polygon, const rectangle_type& rect) {
    typename polygon_traits<polygon_with_holes_type>::coordinate_type coords[4] = 
      {rectangle_concept::xl(rect), rectangle_concept::yl(rect),
       rectangle_concept::xh(rect), rectangle_concept::yh(rect)};
    set(polygon, coords, coords+4);
  }
   
  template<typename polygon_with_holes_type, typename point_iterator_type>
  static void set_points(polygon_with_holes_type& polygon, point_iterator_type begin_point, point_iterator_type end_point) {
    return set(iterator_points_to_compact<point_iterator_type, typename point_iterator_type::value_type>(begin_point),
               iterator_points_to_compact<point_iterator_type, typename point_iterator_type::value_type>(end_point));
  }

  template<typename polygon_with_holes_type, typename hole_iterator_type>
  static void set_holes(polygon_with_holes_type& polygon, hole_iterator_type holes_begin, hole_iterator_type holes_end) {
    polygon_with_holes_traits<polygon_with_holes_type>::set_holes(polygon, holes_begin, holes_end);
  }

  template <typename polygon_with_holes_type>
  static typename polygon_traits<polygon_with_holes_type>::iterator_type begin(const polygon_with_holes_type& polygon) {
    return polygon_traits<polygon_with_holes_type>::begin(polygon);
  }
  
  template <typename polygon_with_holes_type>
  static typename polygon_traits<polygon_with_holes_type>::iterator_type end(const polygon_with_holes_type& polygon) {
    return polygon_traits<polygon_with_holes_type>::end(polygon);
  }
  
  template <typename polygon_with_holes_type>
  static typename polygon_with_holes_traits<polygon_with_holes_type>::iterator_holes_type begin_holes(const polygon_with_holes_type& polygon) {
    return polygon_with_holes_traits<polygon_with_holes_type>::begin_holes(polygon);
  }
  
  template <typename polygon_with_holes_type>
  static typename polygon_with_holes_traits<polygon_with_holes_type>::iterator_holes_type end_holes(const polygon_with_holes_type& polygon) {
    return polygon_with_holes_traits<polygon_with_holes_type>::end_holes(polygon);
  }
  
  template <typename polygon_with_holes_type>
  static iterator_compact_to_points<typename polygon_traits<polygon_with_holes_type>::iterator_type,
                         point_data<typename polygon_traits<polygon_with_holes_type>::coordinate_type> > 
  begin_points(const polygon_with_holes_type& polygon) {
    return iterator_compact_to_points<typename polygon_traits<polygon_with_holes_type>::iterator_type,
      point_data<typename polygon_traits<polygon_with_holes_type>::coordinate_type> > (begin(polygon), end(polygon));
  }

  template <typename polygon_with_holes_type>
  static iterator_compact_to_points<typename polygon_traits<polygon_with_holes_type>::iterator_type,
                         point_data<typename polygon_traits<polygon_with_holes_type>::coordinate_type> > 
  end_points(const polygon_with_holes_type& polygon) {
    return iterator_compact_to_points<typename polygon_traits<polygon_with_holes_type>::iterator_type,
      point_data<typename polygon_traits<polygon_with_holes_type>::coordinate_type> > (end(polygon), end(polygon));
  }

  template<typename T, class iT>
  static T construct(iT inputBegin, iT inputEnd) { return polygon_traits<T>::construct(inputBegin, inputEnd); }

  template<typename polygon_with_holes_type, typename rectangle_type>
  static polygon_with_holes_type construct_from_rectangle(const rectangle_type& rect) {
    polygon_with_holes_type poly;
    set(poly, rect);
    return poly;
  }

  template <typename polygon_with_holes_type_1, typename polygon_with_holes_type_2>
  static polygon_with_holes_type_1 copy_construct(const polygon_with_holes_type_2& polygon) { 
    polygon_with_holes_type_1 retval;
    set(retval, polygon_concept::begin(polygon), polygon_concept::end(polygon));
    set_holes(retval, polygon_with_holes_concept::begin_holes(polygon), polygon_with_holes_concept::end_holes(polygon));
    return retval;
  }

  template <typename polygon_with_holes_type>
  static std::size_t size(const polygon_with_holes_type& polygon) {
    return polygon_traits<polygon_with_holes_type>::size(polygon);
  }

  template <typename polygon_with_holes_type>
  static std::size_t size_holes(const polygon_with_holes_type& polygon) {
    return polygon_with_holes_traits<polygon_with_holes_type>::size_holes(polygon);
  }

  template <typename polygon_with_holes_type_1, typename polygon_with_holes_type_2>
  static polygon_with_holes_type_1& assign(polygon_with_holes_type_1& lvalue, const polygon_with_holes_type_2& rvalue) {
    set(lvalue, begin(rvalue), end(rvalue));
    set_holes(lvalue, begin_holes(rvalue), end_holes(rvalue));
    return lvalue;
  }

  template <typename polygon_with_holes_type>
  static rectangle_data<typename polygon_traits<polygon_with_holes_type>::coordinate_type> 
  bounding_box(const polygon_with_holes_type& polygon) {
    return polygon_concept::bounding_box(polygon);
  }
  
  template <typename polygon_with_holes_type>
  static direction_1d winding(const polygon_with_holes_type& polygon){
    return polygon_concept::winding(polygon);
  }

  template <typename polygon_with_holes_type>
  static typename polygon_with_holes_traits<polygon_with_holes_type>::coordinate_type 
  area(const polygon_with_holes_type& polygon) {
    typename polygon_traits<polygon_with_holes_type>::coordinate_type retval = polygon_concept::area(polygon);
    typename polygon_with_holes_traits<polygon_with_holes_type>::iterator_holes_type b, e;
    e = end_holes(polygon);
    for(b = begin_holes(polygon); b != e; ++b) {
      retval -= polygon_concept::area(*b);
    }
  }

  /// check if point is inside polygon 
  template <typename polygon_with_holes_type, typename point_type>
  static bool contains(const polygon_with_holes_type& polygon, const point_type& point, 
                       bool consider_touch, point_concept pc) {
    typename polygon_with_holes_traits<polygon_with_holes_type>::iterator_holes_type b, e;
    e = end_holes(polygon);
    for(b = begin_holes(polygon); b != e; ++b) {
      if(polygon_concept::contains(*b, point, !consider_touch, pc)) {
        return false;
      }
    }
    return polygon_concept::contains(polygon, point, consider_touch, pc);
  }

  /// get the perimeter of the polygon
  template <typename polygon_with_holes_type>
  static typename polygon_traits<polygon_with_holes_type>::coordinate_type
  perimeter(const polygon_with_holes_type& polygon) {
    typename polygon_traits<polygon_with_holes_type>::coordinate_type retval = polygon_concept::perimeter(polygon);
    typename polygon_with_holes_traits<polygon_with_holes_type>::iterator_holes_type b, e;
    e = end_holes(polygon);
    for(b = begin_holes(polygon); b != e; ++b) {
      retval -= polygon_concept::perimeter(*b);
    }
  }

  //   //awaiting re-implementation of iterator_edges and edge concept
  //   template <typename polygon_with_holes_type, typename point_type>
  //   static point_type project(const polygon_with_holes_type& polygon, const point_type& point) const {
  //     typedef point_data<typename polygon_with_holes_traits<polygon_with_holes_type>::coordinate_type> point_type_2;
  //     point_type_2 retval = polygon_concept::project(polygon, point);
  //     double dist = point_concept::euclidian_distance(point retval);
  //     polygon_with_holes_traits<polygon_with_holes_type>::iterator_holes_type b, e;
  //     e = end_holes(polygon);
  //     for(b = begin_holes(polygon); b != e; ++b) {
  //       point_type_2 hole_point = polygon_concept::project(*b, point);
  //       double hole_dist = point_concept::euclidian_distance(point hole_point);
  //       if(hole_dist < dist) {
  //         retval = hole_point;
  //         dist = hole_dist;
  //       }
  //     }
  //     return retval;
  //   }

  //   //awaiting re-implementation of iterator_edges and edge concept
  //   template <typename polygon_with_holes_type, typename point_type>
  //   static point_type project(point_type& result, const polygon_with_holes_type& polygon, 
  //                             const point_type& point, direction_2d dir) {
  //     typedef point_data<typename polygon_with_holes_traits<polygon_with_holes_type>::coordinate_type> point_type_2;
  //     point_type_2 retval = polygon_concept::project(polygon, point, dir);
  //     double dist = point_concept::euclidian_distance(point retval);
  //     polygon_with_holes_traits<polygon_with_holes_type>::iterator_holes_type b, e;
  //     e = end_holes(polygon);
  //     for(b = begin_holes(polygon); b != e; ++b) {
  //       point_type_2 hole_point = polygon_concept::project(*b, point, dir);
  //       double hole_dist = point_concept::euclidian_distance(point hole_point);
  //       if(hole_dist < dist) {
  //         retval = hole_point;
  //         dist = hole_dist;
  //       }
  //     }
  //     return retval;
  //   }

  template <typename polygon_with_holes_type, typename coordinate_type_1, typename coordinate_type_2>
  static void move(polygon_with_holes_type& polygon, coordinate_type_1 x_displacement, coordinate_type_2 y_displacement) {
    typedef typename polygon_with_holes_traits<polygon_with_holes_type>::hole_type hole_type;
    std::vector<hole_type> holes;
    holes.resize(size_holes(polygon));
    typename polygon_with_holes_traits<polygon_with_holes_type>::iterator_holes_type b, e;
    e = end_holes(polygon);
    for(b = begin_holes(polygon); b != e; ++b) {
      holes.push_back(*b);
      polygon_concept::move(holes.back(), x_displacement, y_displacement);
    }
    set_holes(polygon, holes.begin(), holes.end());
    polygon_concept::move(polygon, x_displacement, y_displacement);
  }

  /// move polygon by delta in orient
  template <typename polygon_with_holes_type, typename coordinate_type_1>
  static void move(polygon_with_holes_type& polygon, orientation_2d orient, coordinate_type_1 displacement) {
    if(orient == HORIZONTAL) {
      move(polygon, displacement, 0);
    } else {
      move(polygon, 0, displacement);
    }
  }
};

template <typename T>
inline void testPolygonWithHolesImpl() {
  rectangle_data<T> rect = rectangle_concept::construct<rectangle_data<T> >(0, 0, 100, 100);
  polygon_data<T> polyHole = polygon_concept::construct_from_rectangle<polygon_data<T> >(rectangle_concept::construct<rectangle_data<T> >(10, 10, 90, 90));
  polygon_with_holes_data<T> pwh = polygon_with_holes_concept::construct_from_rectangle<polygon_with_holes_data<T> >(rect);
  polygon_with_holes_concept::set_holes(pwh, &polyHole, (&polyHole)+1);
  //std::cout << pwh << std::endl;
  std::cout << "PolygonWithHoles test pattern 1 1 1 1 1 1 0 0 0 0 0 0\n";
  std::cout << "PolygonWithHoles test pattern "; 
  std::cout << polygon_with_holes_concept::contains(pwh, point_data<T>(1, 11), true, point_concept())
            << " " ;
  std::cout <<  polygon_with_holes_concept::contains(pwh, point_data<T>(10, 10), true, point_concept())
            << " " ;
  std::cout <<  polygon_with_holes_concept::contains(pwh, point_data<T>(10, 90), true, point_concept())
            << " " ;
  std::cout <<  polygon_with_holes_concept::contains(pwh, point_data<T>(90, 90), true, point_concept())
            << " " ;
  std::cout <<  polygon_with_holes_concept::contains(pwh, point_data<T>(90, 10), true, point_concept())
            << " " ;
  std::cout <<  polygon_with_holes_concept::contains(pwh, point_data<T>(90, 80), true, point_concept())
            << " " ;
  std::cout <<  polygon_with_holes_concept::contains(pwh, point_data<T>(12, 12), true, point_concept())
            << " " ;
  std::cout <<  polygon_with_holes_concept::contains(pwh, point_data<T>(10, 10), false, point_concept())
            << " " ;
  std::cout <<  polygon_with_holes_concept::contains(pwh, point_data<T>(10, 90), false, point_concept())
            << " " ;
  std::cout <<  polygon_with_holes_concept::contains(pwh, point_data<T>(90, 90), false, point_concept())
            << " " ;
  std::cout <<  polygon_with_holes_concept::contains(pwh, point_data<T>(90, 10), false, point_concept())
            << " " ;
  std::cout <<  polygon_with_holes_concept::contains(pwh, point_data<T>(90, 80), false, point_concept())
            << "\n";
  
  polygon_with_holes_concept::move(pwh, 5, 5);
}
