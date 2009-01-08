/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
#ifndef GTL_POLYGON_TRAITS_HPP
#define GTL_POLYGON_TRAITS_HPP
namespace gtl {

  template <typename T, typename enable = void>
  struct polygon_90_traits {
    typedef typename T::coordinate_type coordinate_type;
    typedef typename T::compact_iterator_type compact_iterator_type;

    /// Get the begin iterator
    static inline compact_iterator_type begin_compact(const T& t) {
      return t.begin_compact();
    }
  
    /// Get the end iterator
    static inline compact_iterator_type end_compact(const T& t) {
      return t.end_compact();
    }
  
    /// Get the number of sides of the polygon
    static inline unsigned int size(const T& t) {
      return t.size();
    }
  
    /// Get the winding direction of the polygon
    static inline winding_direction winding(const T& t) {
      return unknown_winding;
    }
  };

  template <typename T, typename enable = void>
  struct polygon_traits {
    typedef typename T::coordinate_type coordinate_type;
    typedef typename T::iterator_type iterator_type;
    typedef typename T::point_type point_type;

    /// Get the begin iterator
    static inline iterator_type begin_points(const T& t) {
      return t.begin();
    }
  
    /// Get the end iterator
    static inline iterator_type end_points(const T& t) {
      return t.end();
    }
  
    /// Get the number of sides of the polygon
    static inline unsigned int size(const T& t) {
      return t.size();
    }
  
    /// Get the winding direction of the polygon
    static inline winding_direction winding(const T& t) {
      return unknown_winding;
    }
  };

  template <typename T>
  struct polygon_traits<T, typename is_same_type_SFINAE<polygon_90_concept, typename geometry_concept<T>::type>::type> {
    typedef typename polygon_90_traits<T>::coordinate_type coordinate_type;
    typedef iterator_compact_to_points<typename polygon_90_traits<T>::compact_iterator_type, point_data<coordinate_type> > iterator_type;

    /// Get the begin iterator
    static inline iterator_type begin_points(const T& t) {
      return iterator_type(polygon_90_traits<T>::begin_compact(t),
                           polygon_90_traits<T>::end_compact(t));
    }
  
    /// Get the end iterator
    static inline iterator_type end_points(const T& t) {
      return iterator_type(polygon_90_traits<T>::end_compact(t),
                           polygon_90_traits<T>::end_compact(t));
    }
  
    /// Get the number of sides of the polygon
    static inline unsigned int size(const T& t) {
      return polygon_90_traits<T>::size(t);
    }
  
    /// Get the winding direction of the polygon
    static inline winding_direction winding(const T& t) {
      return polygon_90_traits<T>::winding(t);
    }
  };

  template <typename T, typename enable = void>
  struct polygon_with_holes_traits {
    typedef typename T::iterator_holes_type iterator_holes_type;
    typedef typename T::hole_type hole_type;

    /// Get the begin iterator
    static inline iterator_holes_type begin_holes(const T& t) {
      return t.begin_holes();
    }

    /// Get the end iterator
    static inline iterator_holes_type end_holes(const T& t) {
      return t.end_holes();
    }

    /// Get the number of holes 
    static inline unsigned int size_holes(const T& t) {
      return t.size_holes();
    }
  };

  template <typename T, typename enable = void>
  struct polygon_90_mutable_traits {
  
    /// Set the data of a polygon with the unique coordinates in an iterator, starting with an x
    template <typename iT>
    static inline T& set_compact(T& t, iT input_begin, iT input_end) {
      t.set_compact(input_begin, input_end);
      return t;
    }
  
  };

  template <typename T>
  struct polygon_90_mutable_traits<T, typename is_same_type_SFINAE<polygon_concept, typename geometry_concept<T>::type>::type> {
    /// Set the data of a polygon with the unique coordinates in an iterator, starting with an x
    template <typename iT>
    static inline T& set_compact(T& t, iT input_begin, iT input_end) {
      typedef iterator_points_to_compact<iT, typename polygon_traits<T>::point_type> iTp;
      t.set_points(iTp(polygon_traits<T>::begin_points(t)), iTp(polygon_traits<T>::end_points(t)));
      return t;
    }
  };

  template <typename T, typename enable = void>
  struct polygon_mutable_traits {

    /// Set the data of a polygon with the unique coordinates in an iterator, starting with an x
    template <typename iT>
    static inline T& set_points(T& t, iT input_begin, iT input_end) {
      t.set(input_begin, input_end);
      return t;
    }
  
  };

  template <typename T, typename enable = void>
  struct polygon_with_holes_mutable_traits {

    /// Set the data of a polygon with the unique coordinates in an iterator, starting with an x
    template <typename iT>
    static inline T& set_holes(T& t, iT inputBegin, iT inputEnd) {
      t.set_holes(inputBegin, inputEnd);
      return t;
    }

  };
}
#include "isotropy.hpp"

//point
#include "point_data.hpp"
#include "point_traits.hpp"
#include "point_concept.hpp"

//interval
#include "interval_data.hpp"
#include "interval_traits.hpp"
#include "interval_concept.hpp"

//rectangle
#include "rectangle_data.hpp"
#include "rectangle_traits.hpp"
#include "rectangle_concept.hpp"

//algorithms needed by polygon types
#include "iterator_points_to_compact.hpp"
#include "iterator_compact_to_points.hpp"

namespace gtl {
  struct polygon_concept {};
  struct polygon_with_holes_concept {};
  struct polygon_45_concept {};
  struct polygon_45_with_holes_concept {};
  struct polygon_90_concept {};
  struct polygon_90_with_holes_concept {};
  

  template <typename T>
  struct is_polygon_90_type {
    typedef typename geometry_concept<T>::type GC;
    typedef typename gtl_same_type<polygon_90_concept, GC>::type type;
  };

  template <typename T>
  struct is_polygon_45_type {
    typedef typename geometry_concept<T>::type GC;
    typedef typename gtl_or<typename is_polygon_90_type<T>::type,
                            typename gtl_same_type<polygon_45_concept, GC>::type>::type type;
  };

  template <typename T>
  struct is_polygon_type {
    typedef typename geometry_concept<T>::type GC;
    typedef typename gtl_or<typename is_polygon_45_type<T>::type,
                            typename gtl_same_type<polygon_concept, GC>::type>::type type;
  };
  
  template <typename T>
  struct is_polygon_90_with_holes_type {
    typedef typename geometry_concept<T>::type GC;
    typedef typename gtl_or<typename is_polygon_90_type<T>::type,
                            typename gtl_same_type<polygon_90_with_holes_concept, GC>::type>::type type;
  };

  template <typename T>
  struct is_polygon_45_with_holes_type {
    typedef typename geometry_concept<T>::type GC;
    typedef typename gtl_or_3<typename is_polygon_90_with_holes_type<T>::type,
                              typename is_polygon_45_type<T>::type, 
                              typename gtl_same_type<polygon_45_with_holes_concept, GC>::type>::type type;
  };

  template <typename T>
  struct is_polygon_with_holes_type {
    typedef typename geometry_concept<T>::type GC;
    typedef typename gtl_or_3<typename is_polygon_45_with_holes_type<T>::type,
                              typename is_polygon_type<T>::type, 
                              typename gtl_same_type<polygon_with_holes_concept, GC>::type>::type type;
  };

  template <typename T>
  struct is_mutable_polygon_90_type {
    typedef typename geometry_concept<T>::type GC;
    typedef typename gtl_same_type<polygon_90_concept, GC>::type type;
  };
  
  template <typename T>
  struct is_mutable_polygon_45_type {
    typedef typename geometry_concept<T>::type GC;
    typedef typename gtl_same_type<polygon_45_concept, GC>::type type;
  };

  template <typename T>
  struct is_mutable_polygon_type {
    typedef typename geometry_concept<T>::type GC;
    typedef typename gtl_same_type<polygon_concept, GC>::type type;
  };
  
  template <typename T>
  struct is_mutable_polygon_90_with_holes_type {
    typedef typename geometry_concept<T>::type GC;
    typedef typename gtl_same_type<polygon_90_with_holes_concept, GC>::type type;
  };

  template <typename T>
  struct is_mutable_polygon_45_with_holes_type {
    typedef typename geometry_concept<T>::type GC;
    typedef typename gtl_same_type<polygon_45_with_holes_concept, GC>::type type;
  };

  template <typename T>
  struct is_mutable_polygon_with_holes_type {
    typedef typename geometry_concept<T>::type GC;
    typedef typename gtl_same_type<polygon_with_holes_concept, GC>::type type;
  };

  template <typename T>
  struct is_any_mutable_polygon_with_holes_type {
    typedef typename gtl_or_3<typename is_mutable_polygon_90_with_holes_type<T>::type,
                              typename is_mutable_polygon_45_with_holes_type<T>::type,
                              typename is_mutable_polygon_with_holes_type<T>::type>::type type;
  };
  template <typename T>
  struct is_any_mutable_polygon_without_holes_type {
    typedef typename gtl_or_3<typename is_mutable_polygon_90_type<T>::type, 
                              typename is_mutable_polygon_45_type<T>::type, 
                              typename is_mutable_polygon_type<T>::type>::type type; };
  
  template <typename T>
  struct is_any_mutable_polygon_type {
    typedef typename gtl_or<typename is_any_mutable_polygon_with_holes_type<T>::type,
                            typename is_any_mutable_polygon_without_holes_type<T>::type>::type type;
  };

  template <>
  struct geometry_domain<polygon_45_concept> { typedef forty_five_domain type; };
  template <>
  struct geometry_domain<polygon_45_with_holes_concept> { typedef forty_five_domain type; };
  template <>
  struct geometry_domain<polygon_90_concept> { typedef manhattan_domain type; };
  template <>
  struct geometry_domain<polygon_90_with_holes_concept> { typedef manhattan_domain type; };

  template <typename domain_type, typename coordinate_type>
  struct distance_type_by_domain { typedef typename coordinate_traits<coordinate_type>::coordinate_distance type; };
  template <typename coordinate_type>
  struct distance_type_by_domain<manhattan_domain, coordinate_type> { 
    typedef typename coordinate_traits<coordinate_type>::coordinate_difference type; };

  template <typename T, typename iT>
  typename requires_1 <typename gtl_if<typename is_any_mutable_polygon_type<T>::type>::type, T>::type &
  set_points(T& t, iT begin_points, iT end_points) {
    polygon_mutable_traits<T>::set_points(t, begin_points, end_points);
    return t;
  }

  template <typename T, typename iT>
  typename requires_1 <typename gtl_if<typename gtl_or< typename is_mutable_polygon_90_type<T>::type, 
                                        typename is_mutable_polygon_90_with_holes_type<T>::type>::type>::type, T>::type & 
  set_compact(T& t, iT begin_compact_coordinates, iT end_compact_coordinates) {
    polygon_90_mutable_traits<T>::set_compact(t, begin_compact_coordinates, end_compact_coordinates);
    return t;
  }

  template <typename T, typename iT>
  typename requires_2 <typename gtl_if<typename is_any_mutable_polygon_with_holes_type<T>::type>::type,
                       typename is_different_type_SFINAE<typename geometry_domain<typename geometry_concept<T>::type>::type, manhattan_domain>::type,
                       T>::type &
  set_compact(T& t, iT begin_compact_coordinates, iT end_compact_coordinates) {
    iterator_compact_to_points<iT, point_data<typename polygon_traits<T>::coordinate_type> >
      itrb(begin_compact_coordinates, end_compact_coordinates),
      itre(end_compact_coordinates, end_compact_coordinates);
    return set_points(t, itrb, itre);
  }

  template <typename T, typename iT>
  typename requires_1 <typename gtl_if<typename is_any_mutable_polygon_with_holes_type<T>::type>::type, T>::type &
  set_holes(T& t, iT begin_holes, iT end_holes) {
    polygon_with_holes_mutable_traits<T>::set_holes(t, begin_holes, end_holes);
    return t;
  }

  template <typename T>
  typename requires_2 <typename gtl_if<typename is_polygon_with_holes_type<T>::type>::type, 
                       typename is_same_type_SFINAE<typename geometry_domain<typename geometry_concept<T>::type>::type, manhattan_domain>::type, 
                       typename polygon_90_traits<T>::compact_iterator_type>::type
  begin_compact(const T& polygon) {
    return polygon_90_traits<T>::begin_compact(polygon);
  }
  
  template <typename T>
  typename requires_2 <typename gtl_if<typename is_polygon_with_holes_type<T>::type>::type, 
                       typename is_same_type_SFINAE<typename geometry_domain<typename geometry_concept<T>::type>::type, manhattan_domain>::type, 
                       typename polygon_90_traits<T>::compact_iterator_type>::type
  end_compact(const T& polygon) {
    return polygon_90_traits<T>::end_compact(polygon);
  }
  
  template <typename T>
  typename requires_1 <typename gtl_if<typename is_polygon_with_holes_type<T>::type>::type, 
                       typename polygon_traits<T>::iterator_type>::type
  begin_points(const T& polygon) {
    return polygon_traits<T>::begin_points(polygon);
  }

  template <typename T>
  typename requires_1 <typename gtl_if<typename is_polygon_with_holes_type<T>::type>::type, 
                       typename polygon_traits<T>::iterator_type>::type
  end_points(const T& polygon) {
    return polygon_traits<T>::end_points(polygon);
  }

  template <typename T>
  typename requires_1 <typename gtl_if<typename is_polygon_with_holes_type<T>::type>::type, 
                       unsigned int>::type
  size(const T& polygon) {
    return polygon_traits<T>::size(polygon);
  }

  template <typename T>
  typename requires_1 <typename gtl_if<typename is_polygon_with_holes_type<T>::type>::type, 
                       typename polygon_with_holes_traits<T>::iterator_holes_type>::type
  begin_holes(const T& polygon) {
    return polygon_with_holes_traits<T>::begin_holes(polygon);
  }

  template <typename T>
  typename requires_1 <typename gtl_if<typename is_polygon_with_holes_type<T>::type>::type, 
                       typename polygon_with_holes_traits<T>::iterator_holes_type>::type
  end_holes(const T& polygon) {
    return polygon_with_holes_traits<T>::end_holes(polygon);
  }

  template <typename T>
  typename requires_1 <typename gtl_if<typename is_polygon_with_holes_type<T>::type>::type, 
                       unsigned int>::type
  size_holes(const T& polygon) {
    return polygon_with_holes_traits<T>::size_holes(polygon);
  }

  template <typename T1, typename T2>
  typename requires_2< typename gtl_if<typename is_mutable_polygon_type<T1>::type>::type,
                       typename gtl_if<typename is_polygon_type<T2>::type>::type, T1>::type &
  assign(T1& lvalue, const T2& rvalue) {
    polygon_mutable_traits<T1>::set_points(lvalue, polygon_traits<T2>::begin_points(rvalue),
                                           polygon_traits<T2>::end_points(rvalue));
    return lvalue;
  }

  template <typename T1, typename T2>
  typename requires_2< typename gtl_if<typename is_mutable_polygon_with_holes_type<T1>::type>::type,
                       typename gtl_if<typename is_polygon_with_holes_type<T2>::type>::type, T1>::type &
  assign(T1& lvalue, const T2& rvalue) {
    polygon_mutable_traits<T1>::set_points(lvalue, polygon_traits<T2>::begin_points(rvalue),
                                           polygon_traits<T2>::end_points(rvalue));
    polygon_with_holes_mutable_traits<T1>::set_holes(lvalue, polygon_with_holes_traits<T2>::begin_holes(rvalue),
                                                     polygon_with_holes_traits<T2>::end_holes(rvalue));
    return lvalue;
  }

  template <typename T1, typename T2>
  typename requires_2< typename gtl_if<typename is_mutable_polygon_45_type<T1>::type>::type,
                       typename gtl_if<typename is_polygon_45_type<T2>::type>::type,
                       T1>::type &
  assign(T1& lvalue, const T2& rvalue) {
    polygon_mutable_traits<T1>::set_points(lvalue, polygon_traits<T2>::begin_points(rvalue),
                                           polygon_traits<T2>::end_points(rvalue));
    return lvalue;
  }

  template <typename T1, typename T2>
  typename requires_2< typename gtl_if<typename is_mutable_polygon_45_with_holes_type<T1>::type>::type,
                       typename gtl_if<typename is_polygon_45_with_holes_type<T2>::type>::type, T1>::type &
  assign(T1& lvalue, const T2& rvalue) {
    polygon_mutable_traits<T1>::set_points(lvalue, polygon_traits<T2>::begin_points(rvalue),
                                           polygon_traits<T2>::end_points(rvalue));
    polygon_with_holes_mutable_traits<T1>::set_holes(lvalue, polygon_with_holes_traits<T2>::begin_holes(rvalue),
                                                     polygon_with_holes_traits<T2>::end_holes(rvalue));
    return lvalue;
  }
  
  template <typename T1, typename T2>
  typename requires_2< typename gtl_if<typename is_mutable_polygon_90_type<T1>::type>::type,
                       typename gtl_if<typename is_polygon_90_type<T2>::type>::type, T1>::type &
  assign(T1& lvalue, const T2& rvalue) {
    polygon_90_mutable_traits<T1>::set_compact(lvalue, polygon_90_traits<T2>::begin_compact(rvalue),
                                               polygon_90_traits<T2>::end_compact(rvalue));
    return lvalue;
  }
  
  template <typename T1, typename T2>
  typename requires_2< typename gtl_if<typename is_mutable_polygon_90_with_holes_type<T1>::type>::type,
                      typename gtl_if< typename is_polygon_90_with_holes_type<T2>::type>::type, T1>::type &
  assign(T1& lvalue, const T2& rvalue) {
    polygon_90_mutable_traits<T1>::set_compact(lvalue, polygon_90_traits<T2>::begin_compact(rvalue),
                                               polygon_90_traits<T2>::end_compact(rvalue));
    polygon_with_holes_mutable_traits<T1>::set_holes(lvalue, polygon_with_holes_traits<T2>::begin_holes(rvalue),
                                                     polygon_with_holes_traits<T2>::end_holes(rvalue));
    return lvalue;
  }

  template <typename T1, typename T2>
  typename requires_2< typename gtl_if<typename is_any_mutable_polygon_type<T1>::type>::type,
                       typename is_rectangle_concept<typename geometry_concept<T2>::type>::type, T1>::type &
  assign(T1& polygon, const T2& rect) {
    typedef point_data<typename polygon_traits<T1>::coordinate_type> PT;
    PT points[4] = {PT(xl(rect), yl(rect)), PT(xh(rect), yl(rect)), PT(xh(rect), yh(rect)), PT(xl(rect), yh(rect))};
    set_points(polygon, points, points+4);
    return polygon;
  }

  template <typename polygon_type, typename point_type>
  typename requires_2< typename gtl_if<typename is_mutable_polygon_90_type<polygon_type>::type>::type, 
                       typename is_point_concept<typename geometry_concept<point_type>::type>::type,
                       polygon_type>::type &
  convolve(polygon_type& polygon, const point_type& point) {
    std::vector<typename polygon_90_traits<polygon_type>::coordinate_type> coords;
    coords.reserve(size(polygon));
    bool pingpong = true;
    for(typename polygon_90_traits<polygon_type>::compact_iterator_type iter = begin_compact(polygon); 
        iter != end_compact(polygon); ++iter) {
      coords.push_back((*iter) + (pingpong ? x(point) : y(point)));
      pingpong = !pingpong;
    }
    polygon_90_mutable_traits<polygon_type>::set_compact(polygon, coords.begin(), coords.end());
    return polygon;
  }
  template <typename polygon_type, typename point_type>
  typename requires_2< typename gtl_if<typename gtl_or<typename is_mutable_polygon_45_type<polygon_type>::type, 
                                       typename is_mutable_polygon_type<polygon_type>::type>::type>::type, 
                       typename is_point_concept<typename geometry_concept<point_type>::type>::type,
                       polygon_type>::type &
  convolve(polygon_type& polygon, const point_type& point) {
    std::vector<point_data<typename polygon_traits<polygon_type>::coordinate_type> > points;
    points.reserve(size(polygon));
    for(typename polygon_traits<polygon_type>::iterator_type iter = begin_points(polygon); 
        iter != end_points(polygon); ++iter) {
      points.push_back(*iter);
      convolve(points.back(), point);
    }
    polygon_mutable_traits<polygon_type>::set_points(polygon, points.begin(), points.end());
    return polygon;
  }
  
  template <typename polygon_type, typename point_type>
  typename requires_2< typename gtl_if<typename is_any_mutable_polygon_with_holes_type<polygon_type>::type>::type, 
                       typename is_point_concept<typename geometry_concept<point_type>::type>::type,
                       polygon_type>::type &
  convolve(polygon_type& polygon, const point_type& point) {
    typedef typename polygon_with_holes_traits<polygon_type>::hole_type hole_type;
    hole_type h;
    set_points(h, begin_points(polygon), end_points(polygon));
    convolve(h, point);
    std::vector<hole_type> holes;
    holes.reserve(size_holes(polygon));
    for(typename polygon_with_holes_traits<polygon_type>::iterator_holes_type itr = begin_holes(polygon);
        itr != end_holes(polygon); ++itr) {
      holes.push_back(*itr);
      convolve(holes.back(), point);
    }
    assign(polygon, h);
    set_holes(polygon, holes.begin(), holes.end());
    return polygon;
  }

  template <typename T>
  typename requires_1< typename gtl_if<typename is_any_mutable_polygon_type<T>::type>::type, T>::type &
  move(T& polygon, orientation_2d orient, typename polygon_traits<T>::coordinate_type displacement) {
    typedef typename polygon_traits<T>::coordinate_type Unit;
    if(orient == HORIZONTAL) return convolve(polygon, point_data<Unit>(displacement, Unit(0)));
    return convolve(polygon, point_data<Unit>(Unit(0), displacement));
  }                              

  template <typename polygon_type, typename transform_type>
  typename requires_1< typename gtl_if<typename is_mutable_polygon_90_type<polygon_type>::type>::type, polygon_type>::type &
  transform(polygon_type& polygon, const transform_type& tr) {
    std::vector<typename polygon_90_traits<polygon_type>::coordinate_type> coords;
    coords.reserve(size(polygon));
    bool pingpong = true;
    for(typename polygon_90_traits<polygon_type>::compact_iterator_type iter = begin_compact(polygon); 
        iter != end_compact(polygon); ++iter) {
      typename polygon_90_traits<polygon_type>::coordinate_type dummy(0);
      coords.push_back(*iter);
      if(pingpong)
        tr.transform(coords.back(), dummy);
      else
        tr.transform(dummy, coords.back());
      pingpong = !pingpong;
    }
    polygon_90_mutable_traits<polygon_type>::set_compact(polygon, coords.begin(), coords.end());
    return polygon;
  }
  template <typename polygon_type, typename transform_type>
  typename requires_1< typename gtl_if<typename gtl_or<typename is_mutable_polygon_45_type<polygon_type>::type, 
                                       typename is_mutable_polygon_type<polygon_type>::type>::type>::type, polygon_type>::type &
  transform(polygon_type& polygon, const transform_type& tr) {
    std::vector<point_data<typename polygon_traits<polygon_type>::coordinate_type> > points;
    points.reserve(size(polygon));
    for(typename polygon_traits<polygon_type>::iterator_type iter = begin_points(polygon); 
        iter != end_points(polygon); ++iter) {
      points.push_back(*iter);
      transform(points.back(), tr);
    }
    polygon_mutable_traits<polygon_type>::set_points(polygon, points.begin(), points.end());
    return polygon;
  }

  template <typename T, typename transform_type>
  typename requires_1< typename gtl_if<typename is_any_mutable_polygon_with_holes_type<T>::type>::type, T>::type &
  transform(T& polygon, const transform_type& tr) {
    typedef typename polygon_with_holes_traits<T>::hole_type hole_type;
    hole_type h;
    set_points(h, begin_points(polygon), end_points(polygon));
    transform(h, tr);
    std::vector<hole_type> holes;
    holes.reserve(size_holes(polygon));
    for(typename polygon_with_holes_traits<T>::iterator_holes_type itr = begin_holes(polygon);
        itr != end_holes(polygon); ++itr) {
      holes.push_back(*itr);
      transform(holes.back(), tr);
    }
    assign(polygon, h);
    set_holes(polygon, holes.begin(), holes.end());
    return polygon;
  }

  template <typename polygon_type>
  typename requires_1< typename gtl_if<typename is_any_mutable_polygon_without_holes_type<polygon_type>::type>::type, polygon_type>::type &
  scale_up(polygon_type& polygon, typename coordinate_traits<typename polygon_traits<polygon_type>::coordinate_type>::unsigned_area_type factor) {
    std::vector<point_data<typename polygon_traits<polygon_type>::coordinate_type> > points;
    points.reserve(size(polygon));
    for(typename polygon_traits<polygon_type>::iterator_type iter = begin_points(polygon); 
        iter != end_points(polygon); ++iter) {
      points.push_back(*iter);
      scale_up(points.back(), factor);
    }
    polygon_mutable_traits<polygon_type>::set_points(polygon, points.begin(), points.end());
    return polygon;
  }

  template <typename T>
  typename requires_1< typename gtl_if<typename is_any_mutable_polygon_with_holes_type<T>::type>::type, T>::type &
  scale_up(T& polygon, typename coordinate_traits<typename polygon_traits<T>::coordinate_type>::unsigned_area_type factor) {
    typedef typename polygon_with_holes_traits<T>::hole_type hole_type;
    hole_type h;
    set_points(h, begin_points(polygon), end_points(polygon));
    scale_up(h, factor);
    std::vector<hole_type> holes;
    holes.reserve(size_holes(polygon));
    for(typename polygon_with_holes_traits<T>::iterator_holes_type itr = begin_holes(polygon);
        itr != end_holes(polygon); ++itr) {
      holes.push_back(*itr);
      scale_up(holes.back(), factor);
    }
    assign(polygon, h);
    set_holes(polygon, holes.begin(), holes.end());
    return polygon;
  }

  //scale non-45 down
  template <typename polygon_type>
  typename requires_2< typename gtl_if<typename is_any_mutable_polygon_without_holes_type<polygon_type>::type>::type, 
                       typename gtl_if<typename gtl_not<typename gtl_same_type
                                        < forty_five_domain, 
                                          typename geometry_domain<typename geometry_concept<polygon_type>::type>::type>::type>::type>::type,
                       polygon_type>::type &
  scale_down(polygon_type& polygon, typename coordinate_traits<typename polygon_traits<polygon_type>::coordinate_type>::unsigned_area_type factor) {
    std::vector<point_data<typename polygon_traits<polygon_type>::coordinate_type> > points;
    points.reserve(size(polygon));
    for(typename polygon_traits<polygon_type>::iterator_type iter = begin_points(polygon); 
        iter != end_points(polygon); ++iter) {
      points.push_back(*iter);
      scale_down(points.back(), factor);
    }
    polygon_mutable_traits<polygon_type>::set_points(polygon, points.begin(), points.end());
    return polygon;
  }

  template <typename Unit>
  Unit abs(Unit value) { return value < 0 ? -value : value; }

  template <typename Unit>
  void snap_point_vector_to_45(std::vector<point_data<Unit> >& pts) {
    typedef point_data<Unit> Point;
    if(pts.size() < 3) { pts.clear(); return; }
    Point firstPt = pts.front();
    Point prevPt = firstPt;
    std::unique(pts.begin(), pts.end());
    if(pts.back() == pts[0]) pts.pop_back();
    //iterate over point triplets
    int numPts = pts.size();
    bool wrap_around = false;
    for(int i = 0; i < numPts; ++i) {
      Point& pt1 = pts[i];
      Point& pt2 = pts[(i + 1) % numPts];
      Point& pt3 = pts[(i + 2) % numPts];
      //check if non-45 edge
      Unit deltax = x(pt2) - x(pt1);
      Unit deltay = y(pt2) - y(pt1);
      if(deltax && deltay &&
         abs(deltax) != abs(deltay)) {
        //adjust the middle point
        Unit ndx = x(pt3) - x(pt2);
        Unit ndy = y(pt3) - y(pt2);
        if(ndx && ndy) {
          Unit diff = abs(abs(deltax) - abs(deltay));
          Unit halfdiff = diff/2;
          if((deltax > 0 && deltay > 0) ||
             (deltax < 0 && deltay < 0)) {
            //previous edge is rising slope
            if(abs(deltax + halfdiff + (diff % 2)) ==
               abs(deltay - halfdiff)) {
              x(pt2, x(pt2) + halfdiff + (diff % 2));
              y(pt2, y(pt2) - halfdiff);
            } else if(abs(deltax - halfdiff - (diff % 2)) ==
                      abs(deltay + halfdiff)) {
              x(pt2, x(pt2) - halfdiff - (diff % 2));
              y(pt2, y(pt2) + halfdiff);
            } else{
              std::cout << "fail1\n";
            }
          } else {
            //previous edge is falling slope
            if(abs(deltax + halfdiff + (diff % 2)) ==
               abs(deltay + halfdiff)) {
              x(pt2, x(pt2) + halfdiff + (diff % 2));
              y(pt2, y(pt2) + halfdiff);
            } else if(abs(deltax - halfdiff - (diff % 2)) ==
                      abs(deltay - halfdiff)) {
              x(pt2, x(pt2) - halfdiff - (diff % 2));
              y(pt2, y(pt2) - halfdiff);
            } else {
              std::cout << "fail2\n";
            }
          }
          if(i == numPts - 1 && (diff % 2)) {
            //we have a wrap around effect
            if(!wrap_around) {
              wrap_around = true;
              i = -1;
            }
          }
        } else if(ndx) {
          //next edge is horizontal
          //find the x value for pt1 that would make the abs(deltax) == abs(deltay)
          Unit newDeltaX = abs(deltay);
          if(deltax < 0) newDeltaX *= -1;
          x(pt2, x(pt1) + newDeltaX);
        } else { //ndy
          //next edge is vertical
          //find the y value for pt1 that would make the abs(deltax) == abs(deltay)
          Unit newDeltaY = abs(deltax);
          if(deltay < 0) newDeltaY *= -1;
          y(pt2, y(pt1) + newDeltaY);
        }
      }
    }
  }

  template <typename polygon_type>
  typename requires_1< typename gtl_if<typename is_any_mutable_polygon_without_holes_type<polygon_type>::type>::type, polygon_type>::type &
  snap_to_45(polygon_type& polygon) {
    std::vector<point_data<typename polygon_traits<polygon_type>::coordinate_type> > points;
    points.reserve(size(polygon));
    for(typename polygon_traits<polygon_type>::iterator_type iter = begin_points(polygon); 
        iter != end_points(polygon); ++iter) {
      points.push_back(*iter);
    }
    snap_point_vector_to_45(points);
    polygon_mutable_traits<polygon_type>::set_points(polygon, points.begin(), points.end());
    return polygon;
  }

  template <typename polygon_type>
  typename requires_1< typename gtl_if<typename is_any_mutable_polygon_with_holes_type<polygon_type>::type>::type, polygon_type>::type &
  snap_to_45(polygon_type& polygon) {
    typedef typename polygon_with_holes_traits<polygon_type>::hole_type hole_type;
    hole_type h;
    set_points(h, begin_points(polygon), end_points(polygon));
    snap_to_45(h);
    std::vector<hole_type> holes;
    holes.reserve(size_holes(polygon));
    for(typename polygon_with_holes_traits<polygon_type>::iterator_holes_type itr = begin_holes(polygon);
        itr != end_holes(polygon); ++itr) {
      holes.push_back(*itr);
      snap_to_45(holes.back());
    }
    assign(polygon, h);
    set_holes(polygon, holes.begin(), holes.end());
    return polygon;
  }

  //scale specifically 45 down
  template <typename polygon_type>
  typename requires_2< typename gtl_if<typename is_any_mutable_polygon_without_holes_type<polygon_type>::type>::type, 
                       typename gtl_if<typename gtl_same_type
                       < forty_five_domain, 
                         typename geometry_domain<typename geometry_concept<polygon_type>::type>::type>::type>::type,
                       polygon_type>::type &
  scale_down(polygon_type& polygon, typename coordinate_traits<typename polygon_traits<polygon_type>::coordinate_type>::unsigned_area_type factor) {
    std::vector<point_data<typename polygon_traits<polygon_type>::coordinate_type> > points;
    points.reserve(size(polygon));
    for(typename polygon_traits<polygon_type>::iterator_type iter = begin_points(polygon); 
        iter != end_points(polygon); ++iter) {
      points.push_back(*iter);
      scale_down(points.back(), factor);
    }
    snap_point_vector_to_45(points);
    polygon_mutable_traits<polygon_type>::set_points(polygon, points.begin(), points.end());
    return polygon;
  }

  template <typename T>
  typename requires_1< typename gtl_if<typename is_any_mutable_polygon_with_holes_type<T>::type>::type, T>::type &
  scale_down(T& polygon, typename coordinate_traits<typename polygon_traits<T>::coordinate_type>::unsigned_area_type factor) {
    typedef typename polygon_with_holes_traits<T>::hole_type hole_type;
    hole_type h;
    set_points(h, begin_points(polygon), end_points(polygon));
    scale_down(h, factor);
    std::vector<hole_type> holes;
    holes.reserve(size_holes(polygon));
    for(typename polygon_with_holes_traits<T>::iterator_holes_type itr = begin_holes(polygon);
        itr != end_holes(polygon); ++itr) {
      holes.push_back(*itr);
      scale_down(holes.back(), factor);
    }
    assign(polygon, h);
    set_holes(polygon, holes.begin(), holes.end());
    return polygon;
  }

  template <typename iterator_type, typename area_type>
  static area_type
  point_sequence_area(iterator_type begin_range, iterator_type end_range) {
    typedef typename std::iterator_traits<iterator_type>::value_type point_type;
    typedef typename point_traits<point_type>::coordinate_type Unit;
    if(begin_range == end_range) return area_type(0);
    point_type first = *begin_range;
    point_type previous = first;
    ++begin_range;
    // Initialize trapezoid base line
    area_type y_base = (area_type)y(first);
    // Initialize area accumulator

    area_type area(0);
    while (begin_range != end_range) {
      area_type x1 = (area_type)x(previous);
      area_type x2 = (area_type)x(*begin_range);
#ifdef __ICC
#pragma warning (disable:1572)
#endif
      if(x1 != x2) {
#ifdef __ICC
#pragma warning (default:1572)
#endif
        // do trapezoid area accumulation
        area += (x2 - x1) * (((area_type)y(*begin_range) - y_base) +
                             ((area_type)y(previous) - y_base)) / 2;
      }
      previous = *begin_range;
      // go to next point
      ++begin_range;
    }
    //wrap around to evaluate the edge between first and last if not closed
    if(first != previous) {
      area_type x1 = (area_type)x(previous);
      area_type x2 = (area_type)x(first);
      area += (x2 - x1) * (((area_type)y(first) - y_base) +
                           ((area_type)y(previous) - y_base)) / 2;
    }
    return area;
  }

  template <typename T>
  typename requires_1< typename gtl_if<typename is_polygon_with_holes_type<T>::type>::type,
                       typename area_type_by_domain< typename geometry_domain<typename geometry_concept<T>::type>::type,
                                                     typename polygon_traits<T>::coordinate_type>::type>::type
  area(const T& polygon) {
    typedef typename area_type_by_domain< typename geometry_domain<typename geometry_concept<T>::type>::type,
      typename polygon_traits<T>::coordinate_type>::type area_type;
    area_type retval = point_sequence_area<typename polygon_traits<T>::iterator_type, area_type>
      (begin_points(polygon), end_points(polygon));
    if(retval < 0) retval *= -1;
    for(typename polygon_with_holes_traits<T>::iterator_holes_type itr =
          polygon_with_holes_traits<T>::begin_holes(polygon);
        itr != polygon_with_holes_traits<T>::end_holes(polygon); ++itr) {
      area_type tmp_area = point_sequence_area
        <typename polygon_traits<typename polygon_with_holes_traits<T>::hole_type>::iterator_type, area_type>
        (begin_points(*itr), end_points(*itr));
      if(tmp_area < 0) tmp_area *= -1;
      retval -= tmp_area;
    }
    return retval;
  }

  template <typename iT>
  bool point_sequence_is_45(iT itr, iT itr_end) {
    typedef typename iT::value_type Point;
    typedef typename point_traits<Point>::coordinate_type Unit;
    if(itr == itr_end) return true;
    Point firstPt = *itr;
    Point prevPt = firstPt;
    ++itr;
    while(itr != itr_end) {
      Point pt = *itr;
      Unit deltax = x(pt) - x(prevPt);
      Unit deltay = y(pt) - y(prevPt);
      if(deltax && deltay &&
         abs(deltax) != abs(deltay))
        return false;
      prevPt = pt;
      ++itr;
    }
    Unit deltax = x(firstPt) - x(prevPt);
    Unit deltay = y(firstPt) - y(prevPt);
    if(deltax && deltay &&
       abs(deltax) != abs(deltay))
      return false;
    return true;
  }

  template <typename polygon_type>
  typename requires_1< typename gtl_if<typename is_polygon_with_holes_type<polygon_type>::type>::type, bool>::type
  is_45(const polygon_type& polygon) {
    typename polygon_traits<polygon_type>::iterator_type itr = begin_points(polygon), itr_end = end_points(polygon);
    if(!point_sequence_is_45(itr, itr_end)) return false;
    typename polygon_with_holes_traits<polygon_type>::iterator_holes_type itrh = begin_holes(polygon), itrh_end = end_holes(polygon);
    typedef typename polygon_with_holes_traits<polygon_type>::hole_type hole_type;
    for(; itrh != itrh_end; ++ itrh) {
      typename polygon_traits<hole_type>::iterator_type itr1 = begin_points(polygon), itr1_end = end_points(polygon);
      if(!point_sequence_is_45(itr1, itr1_end)) return false;
    }
    return true;
  }

  template <typename distance_type, typename iterator_type>
  distance_type point_sequence_distance(iterator_type itr, iterator_type itr_end) {
    typedef distance_type Unit;
    typedef iterator_type iterator;
    typedef typename std::iterator_traits<iterator>::value_type point_type;
    Unit return_value = Unit(0);
    point_type previous_point, first_point;
    if(itr == itr_end) return return_value;
    previous_point = first_point = *itr;
    ++itr;
    for( ; itr != itr_end; ++itr) {
      point_type current_point = *itr;
      return_value += (Unit)euclidean_distance(current_point, previous_point);
      previous_point = current_point;
    }
    return_value += (Unit)euclidean_distance(previous_point, first_point);
    return return_value;
  }

  template <typename T>
  typename requires_1< typename gtl_if<typename is_polygon_with_holes_type<T>::type>::type, 
                       typename distance_type_by_domain<typename geometry_domain<typename geometry_concept<T>::type>::type, 
                                                        typename polygon_traits<T>::coordinate_type>::type>::type
  perimeter(const T& polygon) {
    typedef typename distance_type_by_domain
      <typename geometry_domain<typename geometry_concept<T>::type>::type, typename polygon_traits<T>::coordinate_type>::type Unit;
    typedef typename polygon_traits<T>::iterator_type iterator;
    iterator itr = begin_points(polygon);
    iterator itr_end = end_points(polygon);
    Unit return_value = point_sequence_distance<Unit, iterator>(itr, itr_end);
    for(typename polygon_with_holes_traits<T>::iterator_holes_type itr_holes = begin_holes(polygon);
        itr_holes != end_holes(polygon); ++itr_holes) {
      typedef typename polygon_traits<typename polygon_with_holes_traits<T>::hole_type>::iterator_type hitertype;
      return_value += point_sequence_distance<Unit, hitertype>(begin_points(*itr_holes), end_points(*itr_holes));
    }
    return return_value;
  }

  template <typename T>
  typename requires_1 <typename gtl_if<typename is_polygon_with_holes_type<T>::type>::type, 
                       direction_1d>::type
  winding(const T& polygon) {
    winding_direction wd = polygon_traits<T>::winding(polygon);
    if(wd != unknown_winding) {
      return wd == clockwise_winding ? CLOCKWISE: COUNTERCLOCKWISE;
    }
    typedef typename area_type_by_domain< typename geometry_domain<typename geometry_concept<T>::type>::type,
      typename polygon_traits<T>::coordinate_type>::type area_type;
    return point_sequence_area<typename polygon_traits<T>::iterator_type, area_type>(begin_points(polygon), end_points(polygon)) < 0 ?
      COUNTERCLOCKWISE : CLOCKWISE;
  }

  template <typename T, typename input_point_type>
  typename requires_3< typename gtl_if<typename is_polygon_with_holes_type<T>::type>::type, 
                       typename is_same_type_SFINAE<typename geometry_domain<typename geometry_concept<T>::type>::type, manhattan_domain>::type, 
                       typename is_same_type_SFINAE<typename geometry_concept<input_point_type>::type, point_concept>::type, 
  bool>::type
  contains(const T& polygon, const input_point_type& point, bool consider_touch = true) {
    typedef T polygon_type;
    typedef typename polygon_traits<polygon_type>::coordinate_type coordinate_type;
    typedef typename polygon_traits<polygon_type>::iterator_type iterator;
    typedef typename std::iterator_traits<iterator>::value_type point_type;
    iterator iter, iter_end;
    iter_end = end_points(polygon);
    iter = begin_points(polygon);
    point_type prev_pt = *iter;
    unsigned int num = size(polygon);
    unsigned int counts[2] = {0, 0};
    for(unsigned int i = 0; i < num; ++i) {
      if(i == num-1) iter = begin_points(polygon);
      else ++iter;
      point_type current_pt = *iter;
      if(x(current_pt) == 
         x(prev_pt)) {
        unsigned int index = x(current_pt) > 
          x(point);
        unsigned int increment = 0;
        interval_data<coordinate_type> ivl(y(current_pt), 
                                           y(prev_pt));
        if(contains(ivl, y(point), true)) {
          if(x(current_pt) == 
             x(point)) return consider_touch;
          ++increment;
          if(y(current_pt) != 
              y(point) &&
              y(prev_pt) != 
              y(point)) {
            ++increment;
          } 
          counts[index] += increment;
        }
      }
      prev_pt = current_pt;
    }
    //odd count implies boundary condition
    if(counts[0] % 2 || counts[1] % 2) return consider_touch;
    //an odd number of edges to the left implies interior pt
    return counts[0] % 4; 
  }
           
  template <typename T, typename input_point_type>
  typename requires_3< typename gtl_if<typename is_polygon_with_holes_type<T>::type>::type, 
                       typename is_different_type_SFINAE<typename geometry_domain<typename geometry_concept<T>::type>::type, manhattan_domain>::type, 
                       typename is_same_type_SFINAE<typename geometry_concept<input_point_type>::type, point_concept>::type, bool>::type
  contains(const T& polygon, const input_point_type& point, bool consider_touch = true) {
    std::cout << "not implemented\n"; //awaiting arrival of general edge concept
    return false;
  }

  template <typename T1, typename T2>
  typename requires_2< typename is_mutable_point_concept<typename geometry_concept<T1>::type>::type,
                       typename gtl_if<typename is_polygon_with_holes_type<T2>::type>::type,
                       bool>::type 
  center(T1& center_point, const T2& polygon) {
    typedef typename polygon_traits<T2>::coordinate_type coordinate_type;
    rectangle_data<coordinate_type> bbox;
    extents(bbox, polygon);
    return center(center_point, bbox);
  }
   
  template <typename T1, typename T2>
  typename requires_2< typename is_mutable_rectangle_concept<typename geometry_concept<T1>::type>::type,
                       typename gtl_if<typename is_polygon_with_holes_type<T2>::type>::type,
                       bool>::type 
  extents(T1& bounding_box, const T2& polygon) {
    typedef typename polygon_traits<T2>::iterator_type iterator;
    bool first_iteration = true;
    iterator itr_end = end_points(polygon);
    for(iterator itr = begin_points(polygon); itr != itr_end; ++itr) {
      if(first_iteration) {
        set_points(bounding_box, *itr, *itr);
        first_iteration = false;
      } else {
        encompass(bounding_box, *itr);
      }
    }
    if(first_iteration) return false;
    return true;
  }

  template <class T>
  template <class T2>
  polygon_90_data<T>& polygon_90_data<T>::operator=(const T2& rvalue) {
    assign(*this, rvalue);
    return *this;
  }

  template <typename T>
  struct geometry_concept<polygon_data<T> > {
    typedef polygon_concept type;
  };
  template <typename T>
  struct geometry_concept<polygon_45_data<T> > {
    typedef polygon_45_concept type;
  };
  template <typename T>
  struct geometry_concept<polygon_90_data<T> > {
    typedef polygon_90_concept type;
  };
  template <typename T>
  struct geometry_concept<polygon_with_holes_data<T> > {
    typedef polygon_with_holes_concept type;
  };
  template <typename T>
  struct geometry_concept<polygon_45_with_holes_data<T> > {
    typedef polygon_45_with_holes_concept type;
  };
  template <typename T>
  struct geometry_concept<polygon_90_with_holes_data<T> > {
    typedef polygon_90_with_holes_concept type;
  };

  template <typename T> struct polygon_with_holes_traits<polygon_90_data<T> > {
    typedef polygon_90_data<T> hole_type;
    typedef const hole_type* iterator_holes_type;
    static inline iterator_holes_type begin_holes(const hole_type& t) { return &t; }
    static inline iterator_holes_type end_holes(const hole_type& t) { return &t; }
    static inline unsigned int size_holes(const hole_type& t) { return 0; }
  };
  template <typename T> struct polygon_with_holes_traits<polygon_45_data<T> > {
    typedef polygon_45_data<T> hole_type;
    typedef const hole_type* iterator_holes_type;
    static inline iterator_holes_type begin_holes(const hole_type& t) { return &t; }
    static inline iterator_holes_type end_holes(const hole_type& t) { return &t; }
    static inline unsigned int size_holes(const hole_type& t) { return 0; }
  };
  template <typename T> struct polygon_with_holes_traits<polygon_data<T> > {
    typedef polygon_data<T> hole_type;
    typedef const hole_type* iterator_holes_type;
    static inline iterator_holes_type begin_holes(const hole_type& t) { return &t; }
    static inline iterator_holes_type end_holes(const hole_type& t) { return &t; }
    static inline unsigned int size_holes(const hole_type& t) { return 0; }
  };

}

#endif

