/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
#ifndef GTL_RECTANGLE_CONCEPT_HPP
#define GTL_RECTANGLE_CONCEPT_HPP

#include "isotropy.hpp"

//point
#include "point_data.hpp"
#include "point_traits.hpp"
#include "point_concept.hpp"

//interval
#include "interval_data.hpp"
#include "interval_traits.hpp"
#include "interval_concept.hpp"

#include "rectangle_data.hpp"
#include "rectangle_traits.hpp"

namespace gtl {
  struct rectangle_concept {};
 
  template <typename T>
  struct is_rectangle_concept {};
  template <>
  struct is_rectangle_concept<rectangle_concept> { typedef void type; };

  template <typename T>
  struct is_mutable_rectangle_concept {};
  template <>
  struct is_mutable_rectangle_concept<rectangle_concept> { typedef void type; };

  template <>
  struct geometry_domain<rectangle_concept> { typedef manhattan_domain type; };

  template <typename T, orientation_2d_enum orient>
  typename requires_1< typename is_rectangle_concept<typename geometry_concept<T>::type>::type,
                       typename rectangle_traits<T>::interval_type>::type
  get(const T& rectangle) {
    return rectangle_traits<T>::get(rectangle, orient); 
  }

  template <typename T>
  typename requires_1< typename is_rectangle_concept<typename geometry_concept<T>::type>::type,
                       typename rectangle_traits<T>::interval_type>::type
  get(const T& rectangle, orientation_2d orient) {
    return rectangle_traits<T>::get(rectangle, orient); 
  }

  template <typename T>
  typename requires_1< typename is_rectangle_concept<typename geometry_concept<T>::type>::type,
                       typename rectangle_traits<T>::interval_type>::type
  horizontal(const T& rectangle) {
    return rectangle_traits<T>::get(rectangle, HORIZONTAL); 
  }

  template <typename T>
  typename requires_1< typename is_rectangle_concept<typename geometry_concept<T>::type>::type,
                       typename rectangle_traits<T>::interval_type>::type
  vertical(const T& rectangle) {
    return rectangle_traits<T>::get(rectangle, VERTICAL); 
  }

  template <orientation_2d_enum orient, typename T, typename T2>
  typename requires_2<typename is_mutable_rectangle_concept<typename geometry_concept<T>::type>::type, 
                      typename is_interval_concept<typename geometry_concept<T2>::type>::type,
                      void>::type 
  set(T& rectangle, const T2& interval) {
    rectangle_mutable_traits<T>::set(rectangle, orient, interval); 
  }

  template <typename T, typename T2>
  typename requires_2<typename is_mutable_rectangle_concept<typename geometry_concept<T>::type>::type, 
                      typename is_interval_concept<typename geometry_concept<T2>::type>::type,
                      void>::type 
  set(T& rectangle, orientation_2d orient, const T2& interval) {
    rectangle_mutable_traits<T>::set(rectangle, orient, interval); 
  }

  template <typename T, typename T2>
  typename requires_2<typename is_mutable_rectangle_concept<typename geometry_concept<T>::type>::type, 
                      typename is_interval_concept<typename geometry_concept<T2>::type>::type,
                      void>::type 
  horizontal(T& rectangle, const T2& interval) {
    rectangle_mutable_traits<T>::set(rectangle, HORIZONTAL, interval); 
  }

  template <typename T, typename T2>
  typename requires_2<typename is_mutable_rectangle_concept<typename geometry_concept<T>::type>::type, 
                      typename is_interval_concept<typename geometry_concept<T2>::type>::type,
                      void>::type 
  vertical(T& rectangle, const T2& interval) {
    rectangle_mutable_traits<T>::set(rectangle, VERTICAL, interval); 
  }

  template <typename T, typename T2, typename T3>
  typename requires_1<
    typename is_mutable_rectangle_concept<typename geometry_concept<T>::type>::type,
    T>::type 
  construct(const T2& interval_horizontal,
            const T3& interval_vertical) {
    return rectangle_mutable_traits<T>::construct(interval_horizontal, interval_vertical); }

  template <typename T, typename coord_type>
  typename requires_1<
    typename is_mutable_rectangle_concept<typename geometry_concept<T>::type>::type,
    T>::type 
  construct(coord_type xl, coord_type yl, coord_type xh, coord_type yh) {
    return rectangle_mutable_traits<T>::construct(interval_data<coord_type>(xl, xh), 
                                          interval_data<coord_type>(yl, yh)); 
  }

  template <typename T, typename T2>
  typename requires_2<
    typename is_mutable_rectangle_concept<typename geometry_concept<T>::type>::type,
    typename is_rectangle_concept<typename geometry_concept<T2>::type>::type,
    T>::type
  copy_construct(const T2& rectangle) {
    return construct<T> (get(rectangle, HORIZONTAL), get(rectangle, VERTICAL));
  }

  template <typename rectangle_type_1, typename rectangle_type_2>
  typename requires_2<
    typename is_mutable_rectangle_concept<typename geometry_concept<rectangle_type_1>::type>::type,
    typename is_rectangle_concept<typename geometry_concept<rectangle_type_2>::type>::type,
    rectangle_type_1>::type &
  assign(rectangle_type_1& lvalue, const rectangle_type_2& rvalue) {
    set(lvalue, HORIZONTAL, get(rvalue, HORIZONTAL));
    set(lvalue, VERTICAL, get(rvalue, VERTICAL));
    return lvalue;
  }

  template <typename T, typename T2>
  typename requires_2<
    typename is_rectangle_concept<typename geometry_concept<T>::type>::type,
    typename is_rectangle_concept<typename geometry_concept<T2>::type>::type,
    bool>::type 
  equivalence(const T& rect1, const T2& rect2) {
    return equivalence(get(rect1, HORIZONTAL), get(rect2, HORIZONTAL)) &&
      equivalence(get(rect1, VERTICAL), get(rect2, VERTICAL));
  }

  template <typename rectangle_type>
  typename requires_1< typename is_rectangle_concept<typename geometry_concept<rectangle_type>::type>::type,
                       typename rectangle_traits<rectangle_type>::coordinate_type>::type
  get(const rectangle_type& rectangle, orientation_2d orient, direction_1d dir) {
    return get(rectangle_traits<rectangle_type>::get(rectangle, orient), dir); 
  }

  template <typename rectangle_type>
  typename requires_1<typename is_mutable_rectangle_concept<typename geometry_concept<rectangle_type>::type>::type, void>::type 
  set(rectangle_type& rectangle, orientation_2d orient, direction_1d dir, 
      typename rectangle_traits<rectangle_type>::coordinate_type value) {
    typename rectangle_traits<rectangle_type>::interval_type ivl = get(rectangle, orient);
    set(ivl, dir, value);
    set(rectangle, orient, ivl);
  }

  template <typename rectangle_type>
  typename requires_1< typename is_rectangle_concept<typename geometry_concept<rectangle_type>::type>::type,
                       typename rectangle_traits<rectangle_type>::coordinate_type>::type
  xl(const rectangle_type& rectangle) {
    return get(rectangle, HORIZONTAL, LOW);
  }

  template <typename rectangle_type>
  typename requires_1<typename is_mutable_rectangle_concept<typename geometry_concept<rectangle_type>::type>::type, void>::type 
  xl(rectangle_type& rectangle, typename rectangle_traits<rectangle_type>::coordinate_type value) {
    return set(rectangle, HORIZONTAL, LOW, value);
  }

  template <typename rectangle_type>
  typename requires_1< typename is_rectangle_concept<typename geometry_concept<rectangle_type>::type>::type,
                       typename rectangle_traits<rectangle_type>::coordinate_type>::type
  xh(const rectangle_type& rectangle) {
    return get(rectangle, HORIZONTAL, HIGH);
  }

  template <typename rectangle_type>
  typename requires_1<typename is_mutable_rectangle_concept<typename geometry_concept<rectangle_type>::type>::type, void>::type 
  xh(rectangle_type& rectangle, typename rectangle_traits<rectangle_type>::coordinate_type value) {
    return set(rectangle, HORIZONTAL, HIGH, value);
  }

  template <typename rectangle_type>
  typename requires_1< typename is_rectangle_concept<typename geometry_concept<rectangle_type>::type>::type,
                       typename rectangle_traits<rectangle_type>::coordinate_type>::type
  yl(const rectangle_type& rectangle) {
    return get(rectangle, VERTICAL, LOW);
  }

  template <typename rectangle_type>
  typename requires_1<typename is_mutable_rectangle_concept<typename geometry_concept<rectangle_type>::type>::type, void>::type 
  yl(rectangle_type& rectangle, typename rectangle_traits<rectangle_type>::coordinate_type value) {
    return set(rectangle, VERTICAL, LOW, value);
  }

  template <typename rectangle_type>
  typename requires_1< typename is_rectangle_concept<typename geometry_concept<rectangle_type>::type>::type,
                       typename rectangle_traits<rectangle_type>::coordinate_type>::type
  yh(const rectangle_type& rectangle) {
    return get(rectangle, VERTICAL, HIGH);
  }

  template <typename rectangle_type>
  typename requires_1<typename is_mutable_rectangle_concept<typename geometry_concept<rectangle_type>::type>::type, void>::type 
  yh(rectangle_type& rectangle, typename rectangle_traits<rectangle_type>::coordinate_type value) {
    return set(rectangle, VERTICAL, HIGH, value);
  }

  template <typename rectangle_type>
  typename requires_1< typename is_rectangle_concept<typename geometry_concept<rectangle_type>::type>::type,
                       point_data<typename rectangle_traits<rectangle_type>::coordinate_type> >::type
  ll(const rectangle_type& rectangle) {
    return point_data<typename rectangle_traits<rectangle_type>::coordinate_type> (xl(rectangle), yl(rectangle));
  }

  template <typename rectangle_type>
  typename requires_1< typename is_rectangle_concept<typename geometry_concept<rectangle_type>::type>::type,
                       point_data<typename rectangle_traits<rectangle_type>::coordinate_type> >::type
  lr(const rectangle_type& rectangle) {
    return point_data<typename rectangle_traits<rectangle_type>::coordinate_type> (xh(rectangle), yl(rectangle));
  }

  template <typename rectangle_type>
  typename requires_1< typename is_rectangle_concept<typename geometry_concept<rectangle_type>::type>::type,
                       point_data<typename rectangle_traits<rectangle_type>::coordinate_type> >::type
  ul(const rectangle_type& rectangle) {
    return point_data<typename rectangle_traits<rectangle_type>::coordinate_type> (xl(rectangle), yh(rectangle));
  }

  template <typename rectangle_type>
  typename requires_1< typename is_rectangle_concept<typename geometry_concept<rectangle_type>::type>::type,
                       point_data<typename rectangle_traits<rectangle_type>::coordinate_type> >::type
  ur(const rectangle_type& rectangle) {
    return point_data<typename rectangle_traits<rectangle_type>::coordinate_type> (xh(rectangle), yh(rectangle));
  }

  template <typename rectangle_type, typename rectangle_type_2>
  typename requires_2<
    typename is_rectangle_concept<typename geometry_concept<rectangle_type>::type>::type,
    typename is_rectangle_concept<typename geometry_concept<rectangle_type_2>::type>::type,
    bool>::type 
  contains(const rectangle_type& rectangle, const rectangle_type_2 rectangle_contained, 
                       bool consider_touch = true) {
    return contains(horizontal(rectangle), horizontal(rectangle_contained), consider_touch) &&
      contains(vertical(rectangle), vertical(rectangle_contained), consider_touch);
  }
  template <typename rectangle_type, typename point_type>
  typename requires_2<
    typename is_rectangle_concept<typename geometry_concept<rectangle_type>::type>::type,
    typename is_point_concept<typename geometry_concept<point_type>::type>::type,
    bool>::type 
  contains(const rectangle_type& rectangle, const point_type point_contained, 
                       bool consider_touch = true) {
    return contains(horizontal(rectangle), x(point_contained), consider_touch) &&
      contains(vertical(rectangle), y(point_contained), consider_touch);
  }

  /// set all four coordinates based upon two points
  template <typename rectangle_type, typename point_type_1, typename point_type_2>
  typename requires_3< typename is_mutable_rectangle_concept<typename geometry_concept<rectangle_type>::type>::type, 
                       typename is_point_concept<typename geometry_concept<point_type_1>::type>::type, 
                       typename is_point_concept<typename geometry_concept<point_type_2>::type>::type, 
                       rectangle_type>::type &
  set_points(rectangle_type& rectangle, const point_type_1& p1,
             const point_type_2& p2) {
    typedef typename rectangle_traits<rectangle_type>::coordinate_type Unit;
    Unit x1(x(p1));
    Unit x2(x(p2));
    Unit y1(y(p1));
    Unit y2(y(p2));
    horizontal(rectangle, construct<typename rectangle_traits<rectangle_type>::interval_type>(x1, x2));
    vertical(rectangle, construct<typename rectangle_traits<rectangle_type>::interval_type>(y1, y2));
    return rectangle;
  }
  
  /// move rectangle by delta in orient
  template <typename rectangle_type>
  typename requires_1<typename is_mutable_rectangle_concept<typename geometry_concept<rectangle_type>::type>::type, rectangle_type>::type &
  move(rectangle_type& rectangle, orientation_2d orient, 
       typename coordinate_traits<typename rectangle_traits<rectangle_type>::coordinate_type>::coordinate_difference delta) {
    typename rectangle_traits<rectangle_type>::interval_type ivl = get(rectangle, orient);
    move(ivl, delta);
    set(rectangle, orient, ivl);
    return rectangle;
  }

  /// convolve this with b
  template <typename rectangle_type_1, typename rectangle_type_2>
  typename requires_2< typename is_mutable_rectangle_concept<typename geometry_concept<rectangle_type_1>::type>::type, 
                       typename is_rectangle_concept<typename geometry_concept<rectangle_type_2>::type>::type, 
                       rectangle_type_1>::type &
  convolve(rectangle_type_1& rectangle,
           const rectangle_type_2& convolution_rectangle) {
    typename rectangle_traits<rectangle_type_1>::interval_type ivl = horizontal(rectangle);
    horizontal(rectangle, convolve(ivl, horizontal(convolution_rectangle)));
    ivl = vertical(rectangle);
    vertical(rectangle, convolve(ivl, vertical(convolution_rectangle)));
    return rectangle;
  }
  
  /// deconvolve this with b
  template <typename rectangle_type_1, typename rectangle_type_2>
  typename requires_2< typename is_mutable_rectangle_concept<typename geometry_concept<rectangle_type_1>::type>::type,
                       typename is_rectangle_concept<typename geometry_concept<rectangle_type_2>::type>::type,
                       rectangle_type_1>::type &
  deconvolve(rectangle_type_1& rectangle, const rectangle_type_2& convolution_rectangle) {
    typename rectangle_traits<rectangle_type_1>::interval_type ivl = horizontal(rectangle);
    horizontal(rectangle, deconvolve(ivl, horizontal(convolution_rectangle)));
    ivl = vertical(rectangle);
    vertical(rectangle, deconvolve(ivl, vertical(convolution_rectangle)));
    return rectangle;
  }
  
  /// reflectedConvolve this with b
  template <typename rectangle_type_1, typename rectangle_type_2>
  typename requires_2< typename is_mutable_rectangle_concept<typename geometry_concept<rectangle_type_1>::type>::type,
                       typename is_rectangle_concept<typename geometry_concept<rectangle_type_2>::type>::type,
                       rectangle_type_1>::type &
  reflected_convolve(rectangle_type_1& rectangle, const rectangle_type_2& convolution_rectangle) {
    typename rectangle_traits<rectangle_type_1>::interval_type ivl = horizontal(rectangle);
    horizontal(rectangle, reflected_convolve(ivl, horizontal(convolution_rectangle)));
    ivl = vertical(rectangle);
    vertical(rectangle, reflected_convolve(ivl, vertical(convolution_rectangle)));
    return rectangle;
  }
  
  /// reflectedDeconvolve this with b
  /// deconvolve this with b
  template <typename rectangle_type_1, typename rectangle_type_2>
  typename requires_2< typename is_mutable_rectangle_concept<typename geometry_concept<rectangle_type_1>::type>::type,
                       typename is_rectangle_concept<typename geometry_concept<rectangle_type_2>::type>::type,
                       rectangle_type_1>::type &
  reflected_deconvolve(rectangle_type_1& rectangle, const rectangle_type_2& convolution_rectangle) {
    typename rectangle_traits<rectangle_type_1>::interval_type ivl = horizontal(rectangle);
    horizontal(rectangle, reflected_deconvolve(ivl, horizontal(convolution_rectangle)));
    ivl = vertical(rectangle);
    vertical(rectangle, reflected_deconvolve(ivl, vertical(convolution_rectangle)));
    return rectangle;
  }
  
  /// convolve with point
  template <typename rectangle_type, typename point_type>
  typename requires_2< typename is_mutable_rectangle_concept<typename geometry_concept<rectangle_type>::type>::type,
                       typename is_point_concept<typename geometry_concept<point_type>::type>::type,
                       rectangle_type>::type &
  convolve(rectangle_type& rectangle, const point_type& convolution_point) {
    typename rectangle_traits<rectangle_type>::interval_type ivl = horizontal(rectangle);
    horizontal(rectangle, convolve(ivl, x(convolution_point)));
    ivl = vertical(rectangle);
    vertical(rectangle, convolve(ivl, y(convolution_point)));
    return rectangle;
  }

  /// deconvolve with point
  template <typename rectangle_type, typename point_type>
  typename requires_2< typename is_mutable_rectangle_concept<typename geometry_concept<rectangle_type>::type>::type,
                       typename is_point_concept<typename geometry_concept<point_type>::type>::type,
                       rectangle_type>::type &
  deconvolve(rectangle_type& rectangle, const point_type& convolution_point) {
    typename rectangle_traits<rectangle_type>::interval_type ivl = horizontal(rectangle);
    horizontal(rectangle, deconvolve(ivl, x(convolution_point)));
    ivl = vertical(rectangle);
    vertical(rectangle, deconvolve(ivl, y(convolution_point)));
    return rectangle;
  }

  /// get the magnitude of the interval range depending on orient
  template <typename rectangle_type>
  typename coordinate_traits<typename rectangle_traits<rectangle_type>::coordinate_type>::coordinate_difference
  delta(const rectangle_type& rectangle, orientation_2d orient) {
    return delta(get(rectangle, orient));
  }

  /// get the area of the rectangle
  template <typename rectangle_type>
  typename requires_1< typename is_rectangle_concept<typename geometry_concept<rectangle_type>::type>::type,
                       typename coordinate_traits<typename rectangle_traits<rectangle_type>::coordinate_type>::manhattan_area_type>::type
  area(const rectangle_type& rectangle) {
    typedef typename coordinate_traits<typename rectangle_traits<rectangle_type>::coordinate_type>::manhattan_area_type area_type;
    return (area_type)delta(rectangle, HORIZONTAL) * (area_type)delta(rectangle, VERTICAL);
  }

  /// returns the orientation of the longest side
  template <typename rectangle_type>
  typename requires_1<typename is_rectangle_concept<typename geometry_concept<rectangle_type>::type>::type, 
                      orientation_2d>::type 
  guess_orientation(const rectangle_type& rectangle) {
    return delta(rectangle, HORIZONTAL) >= delta(rectangle, VERTICAL) ?
      HORIZONTAL : VERTICAL;
  }

  /// get the half perimeter of the rectangle
  template <typename rectangle_type>
  typename requires_1< typename is_rectangle_concept<typename geometry_concept<rectangle_type>::type>::type,
                       typename coordinate_traits<typename rectangle_traits<rectangle_type>::coordinate_type>::coordinate_difference>::type
  half_perimeter(const rectangle_type& rectangle) {
    return delta(rectangle, HORIZONTAL) + delta(rectangle, VERTICAL);
  }
   
  /// get the perimeter of the rectangle
  template <typename rectangle_type>
  typename requires_1< typename is_rectangle_concept<typename geometry_concept<rectangle_type>::type>::type,
                       typename coordinate_traits<typename rectangle_traits<rectangle_type>::coordinate_type>::coordinate_difference>::type
  perimeter(const rectangle_type& rectangle) {
    return 2 * half_perimeter(rectangle);
  }

  /// check if Rectangle b intersects `this` Rectangle
  //  [in]     b         Rectangle that will be checked
  //  [in]     considerTouch If true, return true even if b touches the boundary
  //  [ret]    .         true if `t` intersects b
  template <typename rectangle_type_1, typename rectangle_type_2>
  typename requires_2<
    typename is_rectangle_concept<typename geometry_concept<rectangle_type_1>::type>::type,
    typename is_rectangle_concept<typename geometry_concept<rectangle_type_2>::type>::type,
    bool>::type 
  intersects(const rectangle_type_1& rectangle, const rectangle_type_2& b, bool consider_touch = true) {
    return intersects(horizontal(rectangle), horizontal(b), consider_touch) &&
      intersects(vertical(rectangle), vertical(b), consider_touch);
  }

  /// Check if boundaries of Rectangle b and `this` Rectangle intersect
  //  [in]     b         Rectangle that will be checked
  //  [in]     considerTouch If true, return true even if p is on the foundary
  //  [ret]    .         true if `t` contains p
  template <typename rectangle_type_1, typename rectangle_type_2>
  typename requires_2<
    typename is_rectangle_concept<typename geometry_concept<rectangle_type_1>::type>::type,
    typename is_rectangle_concept<typename geometry_concept<rectangle_type_2>::type>::type,
    bool>::type 
  boundaries_intersect(const rectangle_type_1& rectangle, const rectangle_type_2& b,
                       bool consider_touch = true) {
    return (intersects(rectangle, b, consider_touch) &&
            !(contains(rectangle, b, !consider_touch)) &&
            !(contains(b, rectangle, !consider_touch)));
  }
    
  /// check if b is touching 'this' on the end specified by dir
  template <typename rectangle_type_1, typename rectangle_type_2>
  typename requires_2<
    typename is_rectangle_concept<typename geometry_concept<rectangle_type_1>::type>::type,
    typename is_rectangle_concept<typename geometry_concept<rectangle_type_2>::type>::type,
    bool>::type 
  abuts(const rectangle_type_1& rectangle, const rectangle_type_2& b,
                    direction_2d dir) {
    return 
      abuts(get(rectangle, orientation_2d(dir)),
            get(b, orientation_2d(dir)),
            direction_1d(dir)) &&
      intersects(get(rectangle, orientation_2d(dir).get_perpendicular()),
                 get(b, orientation_2d(dir).get_perpendicular()), true);
  }
  
  /// check if they are touching in the given orientation
  template <typename rectangle_type_1, typename rectangle_type_2>
  typename requires_2<
    typename is_rectangle_concept<typename geometry_concept<rectangle_type_1>::type>::type,
    typename is_rectangle_concept<typename geometry_concept<rectangle_type_2>::type>::type,
    bool>::type 
  abuts(const rectangle_type_1& rectangle, const rectangle_type_2& b,
                    orientation_2d orient) {
    return 
      abuts(get(rectangle, orient), get(b, orient)) &&
      intersects(get(rectangle, orient.get_perpendicular()),
                 get(b, orient.get_perpendicular()), true);
  }

  /// check if they are touching but not overlapping
  template <typename rectangle_type_1, typename rectangle_type_2>
  typename requires_2<
    typename is_rectangle_concept<typename geometry_concept<rectangle_type_1>::type>::type,
    typename is_rectangle_concept<typename geometry_concept<rectangle_type_2>::type>::type,
    bool>::type 
  abuts(const rectangle_type_1& rectangle, const rectangle_type_2& b) {
    return abuts(rectangle, b, HORIZONTAL) || abuts(rectangle, b, VERTICAL);
  }

  /// intersect rectangle with interval on orient
  template <typename rectangle_type, typename interval_type>
  typename requires_2< typename is_mutable_rectangle_concept<typename geometry_concept<rectangle_type>::type>::type,
                       typename is_interval_concept<typename geometry_concept<interval_type>::type>::type,
                       bool>::type 
  intersect(rectangle_type& rectangle, const interval_type& b,
            orientation_2d orient, bool consider_touch = true) {
    typename rectangle_traits<rectangle_type>::interval_type ivl = get(rectangle, orient);
    if(intersect(ivl, b, consider_touch)) {
      set(rectangle, orient, ivl);
      return true;
    }
    return false;
  }

  /// clip rectangle to b
  template <typename rectangle_type_1, typename rectangle_type_2>
  typename requires_2< typename is_mutable_rectangle_concept<typename geometry_concept<rectangle_type_1>::type>::type,
                       typename is_rectangle_concept<typename geometry_concept<rectangle_type_2>::type>::type,
                       bool>::type 
  intersect(rectangle_type_1& rectangle, const rectangle_type_2& b, bool consider_touch = true) {
    if(intersects(rectangle, b)) {
      intersect(rectangle, horizontal(b), HORIZONTAL, consider_touch);
      intersect(rectangle, vertical(b), VERTICAL, consider_touch);
      return true;
    }
    return false;
  }

  /// Sets this to the generalized intersection of this and the given rectangle
  template <typename rectangle_type_1, typename rectangle_type_2>
  typename requires_2< typename is_mutable_rectangle_concept<typename geometry_concept<rectangle_type_1>::type>::type,
                       typename is_rectangle_concept<typename geometry_concept<rectangle_type_2>::type>::type,
                       rectangle_type_1>::type &
  generalized_intersect(rectangle_type_1& rectangle, const rectangle_type_2& b) {
    typename rectangle_traits<rectangle_type_1>::interval_type ivl = get(rectangle, HORIZONTAL);
    generalized_intersect(ivl, horizontal(b));
    horizontal(rectangle, ivl);
    ivl = vertical(rectangle);
    generalized_intersect(ivl, vertical(b));
    vertical(rectangle, ivl);
    return rectangle;
  }

  /// bloat the interval specified by orient by bloating
  template <typename rectangle_type>
  typename requires_1<typename is_rectangle_concept<typename geometry_concept<rectangle_type>::type>::type, 
                      rectangle_type>::type &
  bloat(rectangle_type& rectangle, orientation_2d orient, 
                        typename rectangle_traits<rectangle_type>::coordinate_type bloating) {
    typename rectangle_traits<rectangle_type>::interval_type ivl = get(rectangle, orient);
    bloat(ivl, bloating);
    set(rectangle, orient, ivl);
    return rectangle;
  }

  /// bloat the Rectangle by bloating
  template <typename rectangle_type>
  typename requires_1<typename is_rectangle_concept<typename geometry_concept<rectangle_type>::type>::type, 
                      rectangle_type>::type &
  bloat(rectangle_type& rectangle,
                        typename rectangle_traits<rectangle_type>::coordinate_type bloating) {
    bloat(rectangle, HORIZONTAL, bloating);
    return bloat(rectangle, VERTICAL, bloating);
  }

  /// bloat the interval cooresponding to orient by bloating in dir direction
  template <typename rectangle_type>
  typename requires_1<typename is_rectangle_concept<typename geometry_concept<rectangle_type>::type>::type, 
                      rectangle_type>::type &
  bloat(rectangle_type& rectangle, direction_2d dir,
                        typename rectangle_traits<rectangle_type>::coordinate_type bloating) {
    typename rectangle_traits<rectangle_type>::interval_type ivl = get(rectangle, orientation_2d(dir));
    bloat(ivl, direction_1d(dir), bloating);
    set(rectangle, orientation_2d(dir), ivl);
    return rectangle;
  }

  /// shrink the interval specified by orient by bloating
  template <typename rectangle_type>
  typename requires_1<typename is_rectangle_concept<typename geometry_concept<rectangle_type>::type>::type, 
                      rectangle_type>::type &
  shrink(rectangle_type& rectangle, orientation_2d orient, 
                         typename rectangle_traits<rectangle_type>::coordinate_type shrinking) {
    return bloat(rectangle, orient, -shrinking);
  }

  /// shrink the Rectangle by bloating
  template <typename rectangle_type>
  typename requires_1<typename is_rectangle_concept<typename geometry_concept<rectangle_type>::type>::type, 
                      rectangle_type>::type &
  shrink(rectangle_type& rectangle, 
                         typename rectangle_traits<rectangle_type>::coordinate_type shrinking) {
    return bloat(rectangle, -shrinking);
  }

  /// shrink the interval cooresponding to orient by bloating in dir direction
  template <typename rectangle_type>
  typename requires_1<typename is_rectangle_concept<typename geometry_concept<rectangle_type>::type>::type, 
                      rectangle_type>::type &
  shrink(rectangle_type& rectangle, direction_2d dir,
                         typename rectangle_traits<rectangle_type>::coordinate_type shrinking) {
    return bloat(rectangle, dir, -shrinking);
  }

  /// encompass interval on orient
  template <typename rectangle_type, typename interval_type>
  typename requires_2< typename is_mutable_rectangle_concept<typename geometry_concept<rectangle_type>::type>::type,
                       typename is_interval_concept<typename geometry_concept<interval_type>::type>::type,
                       bool>::type 
  encompass(rectangle_type& rectangle, const interval_type& b,
            orientation_2d orient) {
    typename rectangle_traits<rectangle_type>::interval_type ivl = get(rectangle, orient);
    if(encompass(ivl, b)) {
      set(rectangle, orient, ivl);
      return true;
    }
    return false;
  }

  /// enlarge rectangle to encompass the Rectangle b
  template <typename rectangle_type_1, typename rectangle_type_2>
  typename requires_2< typename is_mutable_rectangle_concept<typename geometry_concept<rectangle_type_1>::type>::type,
                       typename is_rectangle_concept<typename geometry_concept<rectangle_type_2>::type>::type,
                       bool>::type 
  encompass(rectangle_type_1& rectangle, const rectangle_type_2& b) {
    //note that operator | is intentional because both should be called regardless
    return encompass(rectangle, horizontal(b), HORIZONTAL) |
      encompass(rectangle, vertical(b), VERTICAL);
  }

 /// enlarge rectangle to encompass the point b
  template <typename rectangle_type_1, typename point_type>
  typename requires_2< typename is_mutable_rectangle_concept<typename geometry_concept<rectangle_type_1>::type>::type,
                       typename is_point_concept<typename geometry_concept<point_type>::type>::type,
                       bool>::type 
  encompass(rectangle_type_1& rectangle, const point_type& b) {
    typename rectangle_traits<rectangle_type_1>::interval_type hivl, vivl;
    hivl = horizontal(rectangle);
    vivl = vertical(rectangle);
    //note that operator | is intentional because both should be called regardless
    bool retval = encompass(hivl, x(b)) | encompass(vivl, y(b));
    if(retval) {
      horizontal(rectangle, hivl);
      vertical(rectangle, vivl);
    }
    return retval;
  }

  /// returns the center of the rectangle
  template <typename point_type, typename rectangle_type>
  typename requires_2< typename is_mutable_point_concept<typename geometry_concept<point_type>::type>::type,
                       typename is_rectangle_concept<typename geometry_concept<rectangle_type>::type>::type,
                       bool>::type 
  center(point_type& center_point, const rectangle_type& rectangle) {
    center_point = construct<point_type>(center(horizontal(rectangle)),
                                         center(vertical(rectangle)));
    return true;
  }

  template <typename point_type, typename rectangle_type>
  typename requires_2< typename is_mutable_point_concept<typename geometry_concept<point_type>::type>::type,
                       typename is_rectangle_concept<typename geometry_concept<rectangle_type>::type>::type,
                       bool>::type 
  get_corner(point_type& corner_point, const rectangle_type& rectangle, direction_2d direction_facing, direction_1d direction_turning) {
    typedef typename rectangle_traits<rectangle_type>::coordinate_type Unit;
    Unit u1 = get(rectangle, direction_facing);
    Unit u2 = get(rectangle, direction_facing.turn(direction_turning));
    if(orientation_2d(direction_facing).to_int()) std::swap(u1, u2);
    corner_point = construct<point_type>(u1, u2);
    return true;
  }

  template <typename rectangle_type>
  typename requires_1<typename is_mutable_rectangle_concept<typename geometry_concept<rectangle_type>::type>::type, rectangle_type>::type 
  get_half(const rectangle_type& rectangle, direction_2d dir) {
    rectangle_type retval(rectangle);
    set(retval, orientation_2d(dir), get_half(get(rectangle, orientation_2d(dir)), direction_1d(dir)));
    return retval;
  }

  template <typename rectangle_type_1, typename rectangle_type_2>
  typename requires_2< typename is_mutable_rectangle_concept<typename geometry_concept<rectangle_type_1>::type>::type,
                       typename is_rectangle_concept<typename geometry_concept<rectangle_type_2>::type>::type,
                       bool>::type 
  join_with(rectangle_type_1& rectangle, const rectangle_type_2& b) {
    typedef typename rectangle_traits<rectangle_type_1>::interval_type Interval1;
    typedef typename rectangle_traits<rectangle_type_2>::interval_type Interval2;
    Interval1 hi1 = get(rectangle, HORIZONTAL);
    Interval1 vi1 = get(rectangle, VERTICAL);
    Interval2 hi2 = get(b, HORIZONTAL), vi2 = get(b, VERTICAL);
    Interval1 temp;
    if (equivalence(hi1, hi2) && join_with(vi1, vi2)) {
      vertical(rectangle, vi1);
      return true;
    }
    if (equivalence(vi1, vi2) && join_with(hi1, hi2)) {
      horizontal(rectangle, hi1);
      return true;
    }
    return false;
  }

  template <typename rectangle_type, typename point_type>
  typename requires_2< typename is_rectangle_concept<typename geometry_concept<rectangle_type>::type>::type,
                       typename is_point_concept<typename geometry_concept<point_type>::type>::type,
                       typename coordinate_traits<typename rectangle_traits<rectangle_type>::coordinate_type>::coordinate_difference>::type 
  euclidean_distance(const rectangle_type& lvalue, const point_type& rvalue, orientation_2d orient) {
    return euclidean_distance(get(lvalue, orient), get(rvalue, orient));
  }

  template <typename rectangle_type, typename rectangle_type_2>
  typename requires_2< typename is_rectangle_concept<typename geometry_concept<rectangle_type>::type>::type,
                       typename is_rectangle_concept<typename geometry_concept<rectangle_type_2>::type>::type,
                       typename coordinate_traits<typename rectangle_traits<rectangle_type>::coordinate_type>::coordinate_difference>::type 
  euclidean_distance(const rectangle_type& lvalue, const rectangle_type_2& rvalue, orientation_2d orient) {
    return euclidean_distance(get(lvalue, orient), get(rvalue, orient));
  }

  template <typename rectangle_type, typename point_type>
  typename requires_2< typename is_rectangle_concept<typename geometry_concept<rectangle_type>::type>::type,
                       typename is_point_concept<typename geometry_concept<point_type>::type>::type,
                       typename coordinate_traits<typename rectangle_traits<rectangle_type>::coordinate_type>::coordinate_difference>::type 
  square_euclidean_distance(rectangle_type& lvalue, const point_type& rvalue) {
    typename coordinate_traits<typename rectangle_traits<rectangle_type>::coordinate_type>::coordinate_difference xdist, ydist;
    xdist = euclidean_distance(lvalue, rvalue, HORIZONTAL);
    ydist = euclidean_distance(lvalue, rvalue, VERTICAL);
    return (xdist * xdist) + (ydist * ydist);
  }

  template <typename rectangle_type, typename rectangle_type_2>
  typename requires_2< typename is_rectangle_concept<typename geometry_concept<rectangle_type>::type>::type, 
                       typename is_rectangle_concept<typename geometry_concept<rectangle_type_2>::type>::type, 
                       typename coordinate_traits<typename rectangle_traits<rectangle_type>::coordinate_type>::coordinate_difference>::type 
  square_euclidean_distance(const rectangle_type& lvalue, const rectangle_type_2& rvalue) {
    typename coordinate_traits<typename rectangle_traits<rectangle_type>::coordinate_type>::coordinate_difference xdist, ydist;
    xdist = euclidean_distance(lvalue, rvalue, HORIZONTAL);
    ydist = euclidean_distance(lvalue, rvalue, VERTICAL);
    return (xdist * xdist) + (ydist * ydist);
  }

  template <typename rectangle_type, typename point_type>
  typename requires_2< typename is_rectangle_concept<typename geometry_concept<rectangle_type>::type>::type,
                       typename is_point_concept<typename geometry_concept<point_type>::type>::type,
                       typename coordinate_traits<typename rectangle_traits<rectangle_type>::coordinate_type>::coordinate_distance>::type 
  euclidean_distance(rectangle_type& lvalue, const point_type& rvalue) {
    return sqrt((typename coordinate_traits<typename rectangle_traits<rectangle_type>::coordinate_type>::coordinate_distance)
                (square_euclidean_distance(lvalue, rvalue)));
  }

  template <typename rectangle_type, typename rectangle_type_2>
  typename requires_2< typename is_rectangle_concept<typename geometry_concept<rectangle_type>::type>::type,
                       typename is_rectangle_concept<typename geometry_concept<rectangle_type_2>::type>::type,
                       typename coordinate_traits<typename rectangle_traits<rectangle_type>::coordinate_type>::coordinate_distance>::type 
  euclidean_distance(const rectangle_type& lvalue, const rectangle_type_2& rvalue) {
    return sqrt((typename coordinate_traits<typename rectangle_traits<rectangle_type>::coordinate_type>::coordinate_distance)
                (square_euclidean_distance(lvalue, rvalue)));
  }

  template <typename rectangle_type, typename point_type>
  typename requires_2< typename is_rectangle_concept<typename geometry_concept<rectangle_type>::type>::type,
                       typename is_point_concept<typename geometry_concept<point_type>::type>::type,
                       typename coordinate_traits<typename rectangle_traits<rectangle_type>::coordinate_type>::coordinate_difference>::type 
  manhattan_distance(rectangle_type& lvalue, const point_type& rvalue) {
    typename coordinate_traits<typename rectangle_traits<rectangle_type>::coordinate_type>::coordinate_difference xdist, ydist;
    xdist = euclidean_distance(lvalue, rvalue, HORIZONTAL);
    ydist = euclidean_distance(lvalue, rvalue, VERTICAL);
    return xdist + ydist;
  }

  template <typename rectangle_type, typename rectangle_type_2>
  typename requires_2< typename is_rectangle_concept<typename geometry_concept<rectangle_type>::type>::type,
                       typename is_rectangle_concept<typename geometry_concept<rectangle_type_2>::type>::type,
                       typename coordinate_traits<typename rectangle_traits<rectangle_type>::coordinate_type>::coordinate_difference>::type 
  manhattan_distance(const rectangle_type& lvalue, const rectangle_type_2& rvalue) {
    typename coordinate_traits<typename rectangle_traits<rectangle_type>::coordinate_type>::coordinate_difference xdist, ydist;
    xdist = euclidean_distance(lvalue, rvalue, HORIZONTAL);
    ydist = euclidean_distance(lvalue, rvalue, VERTICAL);
    return xdist + ydist;
  }

  template <typename rectangle_type>
  typename requires_1<typename is_mutable_rectangle_concept<typename geometry_concept<rectangle_type>::type>::type, rectangle_type>::type &
  scale_up(rectangle_type& rectangle, 
                           typename coordinate_traits<typename rectangle_traits<rectangle_type>::coordinate_type>::unsigned_area_type factor) {
    horizontal(rectangle, scale_up(horizontal(rectangle), factor));
    vertical(rectangle, scale_up(vertical(rectangle), factor));
    return rectangle;
  }

  template <typename rectangle_type>
  typename requires_1<typename is_mutable_rectangle_concept<typename geometry_concept<rectangle_type>::type>::type, rectangle_type>::type &
  scale_down(rectangle_type& rectangle, 
                             typename coordinate_traits<typename rectangle_traits<rectangle_type>::coordinate_type>::unsigned_area_type factor) {
    horizontal(rectangle, scale_down(horizontal(rectangle), factor));
    vertical(rectangle, scale_down(vertical(rectangle), factor));
    return rectangle;
  }

  template <typename rectangle_type, typename scaling_type>
  typename requires_1<typename is_mutable_rectangle_concept<typename geometry_concept<rectangle_type>::type>::type, rectangle_type>::type &
  scale(rectangle_type& rectangle, const scaling_type& scaling) {
    point_data<typename rectangle_traits<rectangle_type>::coordinate_type> llp(xl(rectangle), yl(rectangle));
    point_data<typename rectangle_traits<rectangle_type>::coordinate_type> urp(xl(rectangle), yl(rectangle));
    scale(llp, scaling);
    scale(urp, scaling);
    set_points(rectangle, llp, urp);
    return rectangle;
  }

  template <typename rectangle_type, typename transformation_type>
  typename requires_1<typename is_mutable_rectangle_concept<typename geometry_concept<rectangle_type>::type>::type, rectangle_type>::type &
  transform(rectangle_type& rectangle, const transformation_type& transformation) {
    point_data<typename rectangle_traits<rectangle_type>::coordinate_type> llp(xl(rectangle), yl(rectangle));
    point_data<typename rectangle_traits<rectangle_type>::coordinate_type> urp(xl(rectangle), yl(rectangle));
    transform(llp, transformation);
    transform(urp, transformation);
    set_points(rectangle, llp, urp);
    return rectangle;
  }
  
  template <typename rectangle_type_1, typename rectangle_type_2>
  class less_rectangle_concept {
  private:
    orientation_2d orient_;
  public:
    inline less_rectangle_concept(orientation_2d orient = VERTICAL) : orient_(orient) {}
    typename requires_2< typename is_rectangle_concept<typename geometry_concept<rectangle_type_1>::type>::type,
                         typename is_rectangle_concept<typename geometry_concept<rectangle_type_2>::type>::type,
                         bool>::type 
    operator () (const rectangle_type_1& a,
                 const rectangle_type_2& b) const {
      typedef typename rectangle_traits<rectangle_type_1>::coordinate_type Unit;
      Unit vl1 = get(get(a, orient_), LOW); 
      Unit vl2 = get(get(b, orient_), LOW); 
      if(vl1 > vl2) return false;
      if(vl1 == vl2) {
        orientation_2d perp = orient_.get_perpendicular();
        Unit hl1 = get(get(a, perp), LOW);
        Unit hl2 = get(get(b, perp), LOW);
        if(hl1 > hl2) return false;
        if(hl1 == hl2) {
          Unit vh1 = get(get(a, orient_), HIGH); 
          Unit vh2 = get(get(b, orient_), HIGH); 
          if(vh1 > vh2) return false;
          if(vh1 == vh2) {
            Unit hh1 = get(get(a, perp), HIGH);
            Unit hh2 = get(get(b, perp), HIGH);
            return hh1 < hh2;
          }
        }
      }
      return true;
    }
    
  };

  template <typename T>
  template <typename interval_type_1>
  inline void rectangle_data<T>::set(orientation_2d orient, const interval_type_1& interval) {
    assign(ranges_[orient.to_int()], interval);
  }

  template <class T>
  template <class T2>
  rectangle_data<T>& rectangle_data<T>::operator=(const T2& rvalue) {
    assign(*this, rvalue);
    return *this;
  }
  
  template <class T>
  template <class T2>
  bool rectangle_data<T>::operator==(const T2& rvalue) const {
    return equivalence(*this, rvalue);
  }

  template <typename T>
  struct geometry_concept<rectangle_data<T> > {
    typedef rectangle_concept type;
  };

}
#endif

