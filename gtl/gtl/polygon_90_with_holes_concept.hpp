/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/

#ifndef GTL_POLYGON_90_WITH_HOLES_CONCEPT_HPP
#define GTL_POLYGON_90_WITH_HOLES_CONCEPT_HPP
namespace gtl {

struct polygon_90_with_holes_concept {
public:

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

  template<typename polygon_type, typename compact_iterator_type>
  static void set_compact(polygon_type& polygon, compact_iterator_type input_begin, compact_iterator_type input_end) {
    polygon_traits<polygon_type>::set_compact(polygon, input_begin, input_end);
  }

  template<typename polygon_type, typename rectangle_type>
  static void set_rectangle(polygon_type& polygon, const rectangle_type& rect) {
    typename polygon_traits<polygon_type>::coordinate_type coords[4] = 
      {rectangle_concept::xl(rect), rectangle_concept::yl(rect),
       rectangle_concept::xh(rect), rectangle_concept::yh(rect)};
    set_compact(polygon, coords, coords+4);
  }
   
  template<typename polygon_type, typename point_iterator_type>
  static void set(polygon_type& polygon, point_iterator_type begin_point, point_iterator_type end_point) {
    polygon_traits<polygon_type>::set(polygon, begin_point, end_point);
  }

  template <typename polygon_type>
  static typename polygon_traits<polygon_type>::iterator_type 
  begin(const polygon_type& polygon) {
    return polygon_traits<polygon_type>::begin(polygon);
  }
  
  template <typename polygon_type>
  static typename polygon_traits<polygon_type>::iterator_type 
  end(const polygon_type& polygon) {
    return polygon_traits<polygon_type>::end(polygon);
  }
  
  template <typename polygon_type>
  static typename polygon_traits<polygon_type>::compact_iterator_type 
  begin_compact(const polygon_type& polygon) {
    return polygon_traits<polygon_type>::begin_compact(polygon);
  }
  
  template <typename polygon_type>
  static typename polygon_traits<polygon_type>::compact_iterator_type 
  end_compact(const polygon_type& polygon) {
    return polygon_traits<polygon_type>::end_compact(polygon);
  }
  
  template<typename T, class iT>
  static T construct(iT inputBegin, iT inputEnd) { return polygon_traits<T>::construct(inputBegin, inputEnd); }

  template<typename polygon_type, typename rectangle_type>
  static polygon_type construct_from_rectangle(const rectangle_type& rect) {
    polygon_type poly;
    set_rectangle(poly, rect);
    return poly;
  }

  template <typename polygon_type>
  static std::size_t size(const polygon_type& polygon) {
    return polygon_traits<polygon_type>::size(polygon);
  }

  template <typename polygon_type>
  static direction_1d winding(const polygon_type& polygon){
    winding_direction wd = polygon_traits<polygon_type>::winding(polygon);
    if(wd != unknown_winding) {
      return wd == clockwise_winding ? LOW: HIGH;
    }
    direction_1d dir = HIGH;
    typedef typename polygon_traits<polygon_type>::coordinate_type coordinate_type;
    typedef typename polygon_traits<polygon_type>::compact_iterator_type iterator;
    iterator itr = begin_compact(polygon);
    coordinate_type firstx = *itr;
    coordinate_type minX = firstx;
    ++itr;
    iterator end_itr = end_compact(polygon);
    if(itr == end_itr) return dir; 
    coordinate_type prevy = *itr;
    coordinate_type firsty = *itr;
    ++itr;
    for( ; itr != end_itr; ++itr) {
      coordinate_type x = *itr;
      ++itr;
      if(itr == end_itr) break;
      coordinate_type y = *itr;
      if(x <= minX) {
        minX = x;
        //edge directed downward on left side of figure is counterclockwise
        dir = y < prevy ? HIGH : LOW;
      }
      prevy = y;
    }
    if(firstx <= minX) {
      dir = firsty < prevy ? HIGH : LOW;
    }
    return dir;
  }

  template <typename polygon_type, typename rectangle_type>
  static bool
  extents(rectangle_type& bounding_box, const polygon_type& polygon) {
    typedef typename polygon_traits<polygon_type>::coordinate_type coordinate_type;
    typedef typename polygon_traits<polygon_type>::compact_iterator_type iterator;
    coordinate_type xmin = 0;
    coordinate_type ymin = 0;
    coordinate_type xmax = 0;
    coordinate_type ymax = 0;
    bool first_iteration = true;
    iterator itr_end = end_compact(polygon);
    for(iterator itr = begin_compact(polygon); itr != itr_end; ++itr) {
      coordinate_type x = *itr;
      ++itr;
      if(itr == itr_end) break;
      coordinate_type y = *itr;
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
    if(first_iteration) return false;
    bounding_box = rectangle_concept::construct<rectangle_type>(xmin, ymin, xmax, ymax);
    return true;
  }

  template <typename point_type, typename polygon_type>
  static bool
  center(point_type& center_point, const polygon_type& polygon) {
    rectangle_data<typename coordinate_type<polygon_type>::type> bounding_box;
    if(extents(bounding_box, polygon))
      return rectangle_concept::center(center_point, bounding_box);
    return false;
  }
 
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
    set(retval, polygon_90_concept::begin(polygon), polygon_90_concept::end(polygon));
    set_holes(retval, begin_holes(polygon), end_holes(polygon));
    return retval;
  }

  template <typename polygon_with_holes_type>
  static std::size_t size_holes(const polygon_with_holes_type& polygon) {
    return polygon_with_holes_traits<polygon_with_holes_type>::size_holes(polygon);
  }

  template <typename polygon_with_holes_type_1, typename geometry_type_2>
  static polygon_with_holes_type_1& assign(polygon_with_holes_type_1& lvalue, const geometry_type_2& rvalue) {
    polygon_90_with_holes_concept::assign(lvalue, rvalue, typename geometry_concept<geometry_type_2>::type());
    set_holes(lvalue, begin_holes(rvalue), end_holes(rvalue));
    return lvalue;
  }

  template <typename polygon_with_holes_type_1, typename polygon_with_holes_type_2>
  static polygon_with_holes_type_1& assign(polygon_with_holes_type_1& lvalue, const polygon_with_holes_type_2& rvalue,
                                           polygon_90_with_holes_concept tag) {
    polygon_90_concept::assign(lvalue, rvalue);
    set_holes(lvalue, begin_holes(rvalue), end_holes(rvalue));
    return lvalue;
  }

  template <typename polygon_with_holes_type_1, typename polygon_type_2>
  static polygon_with_holes_type_1& assign(polygon_with_holes_type_1& lvalue, const polygon_type_2& rvalue,
                                           polygon_90_concept tag) {
    polygon_90_concept::assign(lvalue, rvalue);
    return lvalue;
  }

  template <typename polygon_with_holes_type>
  static typename area_type<polygon_with_holes_type>::type
  area(const polygon_with_holes_type& polygon) {
    typename area_type<polygon_with_holes_type>::type retval = polygon_90_concept::area(polygon);
    typename polygon_with_holes_traits<polygon_with_holes_type>::iterator_holes_type b, e;
    e = end_holes(polygon);
    for(b = begin_holes(polygon); b != e; ++b) {
      retval -= polygon_90_concept::area(*b);
    }
    return retval;
  }

  /// check if point is inside polygon 
  template <typename polygon_with_holes_type, typename point_type>
  static bool contains(const polygon_with_holes_type& polygon, const point_type& point, 
                       bool consider_touch, point_concept pc) {
    typename polygon_with_holes_traits<polygon_with_holes_type>::iterator_holes_type b, e;
    e = end_holes(polygon);
    for(b = begin_holes(polygon); b != e; ++b) {
      if(polygon_90_concept::contains(*b, point, !consider_touch, pc)) {
        return false;
      }
    }
    return polygon_90_concept::contains(polygon, point, consider_touch, pc);
  }

  /// get the perimeter of the polygon
  template <typename polygon_with_holes_type>
  static typename coordinate_difference<polygon_with_holes_type>::type
  perimeter(const polygon_with_holes_type& polygon) {
    typename coordinate_difference<polygon_with_holes_type>::type retval = polygon_90_concept::perimeter(polygon);
    typename polygon_with_holes_traits<polygon_with_holes_type>::iterator_holes_type b, e;
    e = end_holes(polygon);
    for(b = begin_holes(polygon); b != e; ++b) {
      retval -= polygon_90_concept::perimeter(*b);
    }
    return retval;
  }

  //   //awaiting re-implementation of iterator_edges and edge concept
  //   template <typename polygon_with_holes_type, typename point_type>
  //   static point_type project(const polygon_with_holes_type& polygon, const point_type& point) const {
  //     typedef point_data<typename polygon_with_holes_traits<polygon_with_holes_type>::coordinate_type> point_type_2;
  //     point_type_2 retval = polygon_90_concept::project(polygon, point);
  //     double dist = point_concept::euclidian_distance(point retval);
  //     polygon_with_holes_traits<polygon_with_holes_type>::iterator_holes_type b, e;
  //     e = end_holes(polygon);
  //     for(b = begin_holes(polygon); b != e; ++b) {
  //       point_type_2 hole_point = polygon_90_concept::project(*b, point);
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
  //     point_type_2 retval = polygon_90_concept::project(polygon, point, dir);
  //     double dist = point_concept::euclidian_distance(point retval);
  //     polygon_with_holes_traits<polygon_with_holes_type>::iterator_holes_type b, e;
  //     e = end_holes(polygon);
  //     for(b = begin_holes(polygon); b != e; ++b) {
  //       point_type_2 hole_point = polygon_90_concept::project(*b, point, dir);
  //       double hole_dist = point_concept::euclidian_distance(point hole_point);
  //       if(hole_dist < dist) {
  //         retval = hole_point;
  //         dist = hole_dist;
  //       }
  //     }
  //     return retval;
  //   }

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
      polygon_90_concept::move(holes.back(), x_displacement, y_displacement);
    }
    set_holes(polygon, holes.begin(), holes.end());
    polygon_90_concept::move(polygon, x_displacement, y_displacement);
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

  template <typename polygon_type>
  static polygon_type& scale_up(polygon_type& polygon, 
                                typename coordinate_traits<typename coordinate_type<polygon_type>::type>::unsigned_area_type factor) {
    typedef typename polygon_with_holes_traits<polygon_type>::hole_type hole_type;
    std::vector<hole_type> holes;
    holes.reserve(size_holes(polygon));
    typename polygon_with_holes_traits<polygon_type>::iterator_holes_type b, e;
    e = end_holes(polygon);
    for(b = begin_holes(polygon); b != e; ++b) {
      holes.push_back(*b);
      polygon_90_concept::scale_up(holes.back(), factor);
    }
    set_holes(polygon, holes.begin(), holes.end());
    polygon_90_concept::scale_up(polygon, factor);
    return polygon;
  }

  template <typename polygon_type>
  static polygon_type& scale_down(polygon_type& polygon, 
                                    typename coordinate_traits<typename coordinate_type<polygon_type>::type>::unsigned_area_type factor) {
    typedef typename polygon_with_holes_traits<polygon_type>::hole_type hole_type;
    std::vector<hole_type> holes;
    holes.reserve(size_holes(polygon));
    typename polygon_with_holes_traits<polygon_type>::iterator_holes_type b, e;
    e = end_holes(polygon);
    for(b = begin_holes(polygon); b != e; ++b) {
      holes.push_back(*b);
      polygon_90_concept::scale_down(holes.back(), factor);
    }
    set_holes(polygon, holes.begin(), holes.end());
    polygon_90_concept::scale_down(polygon, factor);
    return polygon;
  }

  template <typename polygon_type, typename scaling_type>
  static polygon_type& scale(polygon_type& polygon, 
                              const scaling_type& scaling) {
    typedef typename polygon_with_holes_traits<polygon_type>::hole_type hole_type;
    std::vector<hole_type> holes;
    holes.reserve(size_holes(polygon));
    typename polygon_with_holes_traits<polygon_type>::iterator_holes_type b, e;
    e = end_holes(polygon);
    for(b = begin_holes(polygon); b != e; ++b) {
      holes.push_back(*b);
      polygon_90_concept::scale(holes.back(), scaling);
    }
    set_holes(polygon, holes.begin(), holes.end());
    polygon_90_concept::scale(polygon, scaling);
    return polygon;
  }

  template <typename polygon_type, typename transformation_type>
  static polygon_type& transform(polygon_type& polygon,
                               const transformation_type& transformation) {
    typedef typename polygon_with_holes_traits<polygon_type>::hole_type hole_type;
    std::vector<hole_type> holes;
    holes.reserve(size_holes(polygon));
    typename polygon_with_holes_traits<polygon_type>::iterator_holes_type b, e;
    e = end_holes(polygon);
    for(b = begin_holes(polygon); b != e; ++b) {
      holes.push_back(*b);
      polygon_90_concept::transform(holes.back(), transformation);
    }
    set_holes(polygon, holes.begin(), holes.end());
    polygon_90_concept::transform(polygon, transformation);
    return polygon;
  }

};

template <typename T>
inline void testPolygonWithHolesImpl() {
  rectangle_data<T> rect = rectangle_concept::construct<rectangle_data<T> >(0, 0, 100, 100);
  polygon_90_data<T> polyHole = polygon_90_concept::construct_from_rectangle<polygon_90_data<T> >(rectangle_concept::construct<rectangle_data<T> >(10, 10, 90, 90));
  polygon_90_with_holes_data<T> pwh = polygon_90_with_holes_concept::construct_from_rectangle<polygon_90_with_holes_data<T> >(rect);
  polygon_90_with_holes_concept::set_holes(pwh, &polyHole, (&polyHole)+1);
  //std::cout << pwh << std::endl;
  std::cout << "PolygonWithHoles test pattern 1 1 1 1 1 1 0 0 0 0 0 0\n";
  std::cout << "PolygonWithHoles test pattern "; 
  std::cout << polygon_90_with_holes_concept::contains(pwh, point_data<T>(1, 11), true, point_concept())
            << " " ;
  std::cout <<  polygon_90_with_holes_concept::contains(pwh, point_data<T>(10, 10), true, point_concept())
            << " " ;
  std::cout <<  polygon_90_with_holes_concept::contains(pwh, point_data<T>(10, 90), true, point_concept())
            << " " ;
  std::cout <<  polygon_90_with_holes_concept::contains(pwh, point_data<T>(90, 90), true, point_concept())
            << " " ;
  std::cout <<  polygon_90_with_holes_concept::contains(pwh, point_data<T>(90, 10), true, point_concept())
            << " " ;
  std::cout <<  polygon_90_with_holes_concept::contains(pwh, point_data<T>(90, 80), true, point_concept())
            << " " ;
  std::cout <<  polygon_90_with_holes_concept::contains(pwh, point_data<T>(12, 12), true, point_concept())
            << " " ;
  std::cout <<  polygon_90_with_holes_concept::contains(pwh, point_data<T>(10, 10), false, point_concept())
            << " " ;
  std::cout <<  polygon_90_with_holes_concept::contains(pwh, point_data<T>(10, 90), false, point_concept())
            << " " ;
  std::cout <<  polygon_90_with_holes_concept::contains(pwh, point_data<T>(90, 90), false, point_concept())
            << " " ;
  std::cout <<  polygon_90_with_holes_concept::contains(pwh, point_data<T>(90, 10), false, point_concept())
            << " " ;
  std::cout <<  polygon_90_with_holes_concept::contains(pwh, point_data<T>(90, 80), false, point_concept())
            << "\n";
  
  polygon_90_with_holes_concept::move(pwh, 5, 5);
}

}
#endif

