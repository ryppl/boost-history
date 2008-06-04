/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
#ifndef GTL_RECTANGLE_CONCEPT_HPP
#define GTL_RECTANGLE_CONCEPT_HPP
namespace gtl {

struct rectangle_concept {
  rectangle_concept() {}

  template <typename rectangle_type>
  struct registration {
    typedef typename rectangle_traits<rectangle_type>::coordinate_type coordinate_type;
    typedef interval_data<coordinate_type> component_type;
    typedef point_data<coordinate_type> center_type;
  };

  template <orientation_2d_enum orient, typename T>
  static inline typename rectangle_traits<T>::interval_type 
  get(const T& rectangle) {
    return rectangle_traits<T>::get(rectangle, orient); 
  }
  
  template <typename T>
  static inline typename rectangle_traits<T>::interval_type 
  get(const T& rectangle, orientation_2d orient) {
    return rectangle_traits<T>::get(rectangle, orient); 
  }

  template <typename T>
  static inline typename rectangle_traits<T>::interval_type 
  horizontal(const T& rectangle) {
    return rectangle_traits<T>::get(rectangle, HORIZONTAL); 
  }

  template <typename T>
  static inline typename rectangle_traits<T>::interval_type 
  vertical(const T& rectangle) {
    return rectangle_traits<T>::get(rectangle, VERTICAL); 
  }

  template <orientation_2d_enum orient, typename T, typename T2>
  static inline void set(T& rectangle, const T2& interval) {
    rectangle_traits<T>::set(rectangle, orient, interval); 
  }

  template <typename T, typename T2>
  static inline void set(T& rectangle, orientation_2d orient, const T2& interval) {
    rectangle_traits<T>::set(rectangle, orient, interval); 
  }

  template <typename T, typename T2>
  static inline void horizontal(T& rectangle, const T2& interval) {
    rectangle_traits<T>::set(rectangle, HORIZONTAL, interval); 
  }

  template <typename T, typename T2>
  static inline void vertical(T& rectangle, const T2& interval) {
    rectangle_traits<T>::set(rectangle, VERTICAL, interval); 
  }

  template <typename T, typename T2, typename T3>
  static inline T construct(const T2& interval_horizontal,
                            const T3& interval_vertical) {
    return rectangle_traits<T>::construct(interval_horizontal, interval_vertical); }

  template <typename T, typename coordinate_type>
  static inline T construct(coordinate_type xl, coordinate_type yl, coordinate_type xh, coordinate_type yh) {
    return rectangle_traits<T>::construct(interval_data<coordinate_type>(xl, xh), 
                                          interval_data<coordinate_type>(yl, yh)); 
  }

  template <typename T, typename T2>
  static T copy_construct(const T2& rectangle) {
    return construct<T>
      (get(rectangle, HORIZONTAL),
       get(rectangle, VERTICAL));
  }

  template <typename rectangle_type_1, typename rectangle_type_2>
  static rectangle_type_1& assign(rectangle_type_1& lvalue, const rectangle_type_2& rvalue) {
    set(lvalue, HORIZONTAL, get(rvalue, HORIZONTAL));
    set(lvalue, VERTICAL, get(rvalue, VERTICAL));
    return lvalue;
  }

  template <typename T, typename T2>
  static bool equivalence(const T& rect1, const T2& rect2) {
    return interval_concept::equivalence(get(rect1, HORIZONTAL),
                                         get(rect2, HORIZONTAL)) &&
      interval_concept::equivalence(get(rect1, VERTICAL),
                                    get(rect2, VERTICAL));
  }

  template <typename rectangle_type>
  static inline typename rectangle_traits<rectangle_type>::coordinate_type 
  get(const rectangle_type& rectangle, orientation_2d orient, direction_1d dir) {
    return interval_concept::get(rectangle_traits<rectangle_type>::get(rectangle, orient), dir); 
  }

  template <direction_1d_enum dir, typename rectangle_type>
  static inline typename rectangle_traits<rectangle_type>::coordinate_type 
  get(const rectangle_type& rectangle, orientation_2d orient) {
    return get(rectangle, orient, dir);
  }

  template <typename rectangle_type>
  static inline void set(rectangle_type& rectangle, orientation_2d orient, direction_1d dir, 
                         typename rectangle_traits<rectangle_type>::coordinate_type value) {
    typename rectangle_traits<rectangle_type>::interval_type ivl = orient(rectangle);
    interval_concept::set(ivl, dir, value);
    set(rectangle, orient, ivl);
  }

  template <direction_1d_enum dir, typename rectangle_type>
  static inline void set(rectangle_type& rectangle, orientation_2d orient,
                         typename rectangle_traits<rectangle_type>::coordinate_type value) {
    typename rectangle_traits<rectangle_type>::interval_type ivl = get(rectangle, orient);
    interval_concept::set(ivl, dir, value);
    set(rectangle, orient, ivl);
  }

  template <typename rectangle_type>
  static typename rectangle_traits<rectangle_type>::coordinate_type
  xl(const rectangle_type& rectangle) {
    return get<LOW>(rectangle, HORIZONTAL);
  }

  template <typename rectangle_type>
  static void xl(rectangle_type& rectangle, typename rectangle_traits<rectangle_type>::coordinate_type value) {
    return set<LOW>(rectangle, HORIZONTAL, value);
  }

  template <typename rectangle_type>
  static typename rectangle_traits<rectangle_type>::coordinate_type
  xh(const rectangle_type& rectangle) {
    return get<HIGH>(rectangle, HORIZONTAL);
  }

  template <typename rectangle_type>
  static void xh(rectangle_type& rectangle, typename rectangle_traits<rectangle_type>::coordinate_type value) {
    return set<HIGH>(rectangle, HORIZONTAL, value);
  }

  template <typename rectangle_type>
  static typename rectangle_traits<rectangle_type>::coordinate_type
  yl(const rectangle_type& rectangle) {
    return get<LOW>(rectangle, VERTICAL);
  }

  template <typename rectangle_type>
  static void yl(rectangle_type& rectangle, typename rectangle_traits<rectangle_type>::coordinate_type value) {
    return set<LOW>(rectangle, VERTICAL, value);
  }

  template <typename rectangle_type>
  static typename rectangle_traits<rectangle_type>::coordinate_type
  yh(const rectangle_type& rectangle) {
    return get<HIGH>(rectangle, VERTICAL);
  }

  template <typename rectangle_type>
  static void yh(rectangle_type& rectangle, typename rectangle_traits<rectangle_type>::coordinate_type value) {
    return set<HIGH>(rectangle, VERTICAL, value);
  }

  template <typename rectangle_type, typename rectangle_type2>
  static bool contains(const rectangle_type& rectangle, const rectangle_type2 rectangle_contained, 
                                bool consider_touch, rectangle_concept tag) {
    return interval_concept::contains(get<HORIZONTAL>(rectangle), get<HORIZONTAL>(rectangle_contained), consider_touch, interval_concept()) &&
      interval_concept::contains(get<VERTICAL>(rectangle), get<VERTICAL>(rectangle_contained), consider_touch, interval_concept());
  }
  template <typename rectangle_type, typename point_type>
  static bool contains(const rectangle_type& rectangle, const point_type point_contained, 
                                bool consider_touch, point_concept tag) {
    return interval_concept::contains(get<HORIZONTAL>(rectangle), point_concept::get<HORIZONTAL>(point_contained), consider_touch, no_type()) &&
      interval_concept::contains(get<VERTICAL>(rectangle), point_concept::get<VERTICAL>(point_contained), consider_touch, no_type());
  }

  /// set all four coordinates based upon two points
  template <typename rectangle_type, typename point_type_1, typename point_type_2>
  static rectangle_type& set_points(rectangle_type& rectangle, const point_type_1& p1,
                                    const point_type_2& p2) {
    typedef typename rectangle_traits<rectangle_type>::coordinate_type Unit;
    Unit x1(point_concept::x(p1));
    Unit x2(point_concept::x(p2));
    Unit y1(point_concept::y(p1));
    Unit y2(point_concept::y(p2));
    horizontal(rectangle, interval_concept::construct<typename rectangle_traits<rectangle_type>::interval_type>(x1, x2));
    vertical(rectangle, interval_concept::construct<typename rectangle_traits<rectangle_type>::interval_type>(y1, y2));
    return rectangle;
  }

  /// move rectangle by delta in orient
  template <typename rectangle_type>
  static rectangle_type& move(rectangle_type& rectangle, orientation_2d orient, 
            typename rectangle_traits<rectangle_type>::coordinate_type delta) {
    typename rectangle_traits<rectangle_type>::interval_type ivl = get(rectangle, orient);
    interval_concept::move(ivl, delta);
    set(rectangle, orient, ivl);
    return rectangle;
  }

  //awaiting transform re-implementation
  //   /// transform rectangle
  //   RectangleImpl& transform(const AxisTransform& atr);
  
  //   /// transform rectangle
  //   RectangleImpl& transform(const Transform& tr);

  /// convolve this with b
  template <typename rectangle_type_1, typename rectangle_type_2>
  static rectangle_type_1& convolve(rectangle_type_1& rectangle,
                                    const rectangle_type_2& convolution_rectangle,
                                    rectangle_concept tag) {
    typename rectangle_traits<rectangle_type_1>::interval_type ivl = horizontal(rectangle);
    horizontal(rectangle, 
               interval_concept::convolve(ivl,
                                          horizontal(convolution_rectangle),
                                          interval_concept()));
    ivl = vertical(rectangle);
    vertical(rectangle, 
             interval_concept::convolve(ivl,
                                        vertical(convolution_rectangle),
                                        interval_concept()));
    return rectangle;
  }
  
  /// deconvolve this with b
  template <typename rectangle_type_1, typename rectangle_type_2>
  static rectangle_type_1& deconvolve(rectangle_type_1& rectangle,
                                      const rectangle_type_2& convolution_rectangle,
                                      rectangle_concept tag) {
    typename rectangle_traits<rectangle_type_1>::interval_type ivl = horizontal(rectangle);
    horizontal(rectangle, 
               interval_concept::deconvolve(ivl,
                                          horizontal(convolution_rectangle),
                                          interval_concept()));
    ivl = vertical(rectangle);
    vertical(rectangle, 
             interval_concept::deconvolve(ivl,
                                        vertical(convolution_rectangle),
                                        interval_concept()));  
    return rectangle;
  }

  /// reflectedConvolve this with b
  template <typename rectangle_type_1, typename rectangle_type_2>
  static rectangle_type_1& reflected_convolve(rectangle_type_1& rectangle,
                                              const rectangle_type_2& convolution_rectangle,
                                              rectangle_concept tag) {
    typename rectangle_traits<rectangle_type_1>::interval_type ivl = horizontal(rectangle);
    horizontal(rectangle, 
               interval_concept::reflected_convolve(ivl,
                                          horizontal(convolution_rectangle),
                                          interval_concept()));
    ivl = vertical(rectangle);
    vertical(rectangle, 
             interval_concept::reflected_convolve(ivl,
                                        vertical(convolution_rectangle),
                                        interval_concept()));   
    return rectangle;
  }

  /// reflectedDeconvolve this with b
  /// deconvolve this with b
  template <typename rectangle_type_1, typename rectangle_type_2>
  static rectangle_type_1& reflected_deconvolve(rectangle_type_1& rectangle,
                                                const rectangle_type_2& convolution_rectangle,
                                                rectangle_concept tag) {
    typename rectangle_traits<rectangle_type_1>::interval_type ivl = horizontal(rectangle);
    horizontal(rectangle, 
               interval_concept::reflected_deconvolve(ivl,
                                                    horizontal(convolution_rectangle),
                                                    interval_concept()));
    ivl = vertical(rectangle);
    vertical(rectangle, 
             interval_concept::reflected_deconvolve(ivl,
                                                  vertical(convolution_rectangle),
                                                  interval_concept()));
    return rectangle;
  }
  
  /// convolve with point
  template <typename rectangle_type, typename point_type>
  static rectangle_type& convolve(rectangle_type& rectangle,
                                  const point_type& convolution_point,
                                  point_concept tag) {
    typename rectangle_traits<rectangle_type>::interval_type ivl = horizontal(rectangle);
    horizontal(rectangle, 
               interval_concept::convolve(ivl,
                                          point_concept::x(convolution_point),
                                          no_type()));
    ivl = vertical(rectangle);
    vertical(rectangle, 
             interval_concept::convolve(ivl,
                                        point_concept::y(convolution_point),
                                        no_type()));
    return rectangle;
  }

  /// deconvolve with point
  template <typename rectangle_type, typename point_type>
  static rectangle_type& deconvolve(rectangle_type& rectangle,
                                    const point_type& convolution_point,
                                    point_concept tag) {
    typename rectangle_traits<rectangle_type>::interval_type ivl = horizontal(rectangle);
    horizontal(rectangle, 
               interval_concept::deconvolve(ivl,
                                            point_concept::x(convolution_point),
                                            no_type()));
    ivl = vertical(rectangle);
    vertical(rectangle, 
             interval_concept::deconvolve(ivl,
                                          point_concept::y(convolution_point),
                                          no_type()));
    return rectangle;
  }

  /// get the magnitude of the interval range depending on orient
  template <typename rectangle_type>
  static typename rectangle_traits<rectangle_type>::coordinate_type
  delta(const rectangle_type& rectangle, orientation_2d orient) {
    return interval_concept::delta(get(rectangle, orient));
  }

  /// get the area of the rectangle
  template <typename rectangle_type>
  static typename rectangle_traits<rectangle_type>::coordinate_type
  area(const rectangle_type& rectangle) {
    return delta(rectangle, HORIZONTAL) * delta(rectangle, VERTICAL);
  }

  /// returns the orientation of the longest side
  template <typename rectangle_type>
  static orientation_2d guess_orientation(const rectangle_type& rectangle) {
    return delta(rectangle, HORIZONTAL) >= delta(rectangle, VERTICAL) ?
      HORIZONTAL : VERTICAL;
  }

  /// get the half perimeter of the rectangle
  template <typename rectangle_type>
  static typename rectangle_traits<rectangle_type>::coordinate_type
  half_perimeter(const rectangle_type& rectangle) {
    return delta(rectangle, HORIZONTAL) + delta(rectangle, VERTICAL);
  }
   
  /// get the perimeter of the rectangle
  template <typename rectangle_type>
  static typename rectangle_traits<rectangle_type>::coordinate_type
  perimeter(const rectangle_type& rectangle) {
    return 2 * half_perimeter(rectangle);
  }

  /// check if Rectangle b intersects `this` Rectangle
  //  [in]     b         Rectangle that will be checked
  //  [in]     considerTouch If true, return true even if b touches the boundary
  //  [ret]    .         true if `t` intersects b
  template <typename rectangle_type_1, typename rectangle_type_2>
  static bool intersects(const rectangle_type_1& rectangle, const rectangle_type_2& b,
                         bool consider_touch, rectangle_concept tag) {
    return interval_concept::intersects(horizontal(rectangle), horizontal(b), consider_touch,
                                        interval_concept()) &&
      interval_concept::intersects(vertical(rectangle), vertical(b), consider_touch,
                                   interval_concept());
  }

  /// Check if boundaries of Rectangle b and `this` Rectangle intersect
  //  [in]     b         Rectangle that will be checked
  //  [in]     considerTouch If true, return true even if p is on the foundary
  //  [ret]    .         true if `t` contains p
  template <typename rectangle_type_1, typename rectangle_type_2>
  static bool boundaries_intersect(const rectangle_type_1& rectangle, const rectangle_type_2& b,
                                   bool consider_touch, rectangle_concept tag) {
    return (intersects(rectangle, b, consider_touch) &&
            !(contains(rectangle, b, !consider_touch)) &&
            !(contains(b, rectangle, !consider_touch)));
  }
    
  /// check if b is touching 'this' on the end specified by dir
  template <typename rectangle_type_1, typename rectangle_type_2>
  static bool abuts(const rectangle_type_1& rectangle, const rectangle_type_2& b,
                                   direction_2d dir, rectangle_concept tag) {
    return 
      interval_concept::abuts(get(rectangle, orientation_2d(dir)),
                              get(b, orientation_2d(dir)),
                              direction_1d(dir), interval_concept()) &&
      interval_concept::intersects(get(rectangle, orientation_2d(dir).get_perpendicular()),
                                   get(b, orientation_2d(dir).get_perpendicular()),
                                   true, interval_concept());
  }
  
  /// check if they are touching in the given orientation
  template <typename rectangle_type_1, typename rectangle_type_2>
  static bool abuts(const rectangle_type_1& rectangle, const rectangle_type_2& b,
                    orientation_2d orient, rectangle_concept tag) {
    return 
      interval_concept::abuts(get(rectangle, orient), get(b, orient), interval_concept()) &&
      interval_concept::intersects(get(rectangle, orient.get_perpendicular()),
                                   get(b, orient.get_perpendicular()),
                                   true, interval_concept());
  }

  /// check if they are touching but not overlapping
  template <typename rectangle_type_1, typename rectangle_type_2>
  static bool abuts(const rectangle_type_1& rectangle, const rectangle_type_2& b,
                    rectangle_concept tag) {
    return abuts(rectangle, b, HORIZONTAL) || abuts(rectangle, b, VERTICAL);
  }

  /// intersect rectangle with interval on orient
  template <typename rectangle_type, typename interval_type>
  static bool intersect(const rectangle_type& rectangle, const interval_type& b,
                        orientation_2d orient, bool consider_touch, interval_concept tag) {
    typename rectangle_traits<rectangle_type>::interval_type ivl = get(rectangle, orient);
    if(interval_concept::intersect(ivl, b, consider_touch, tag)) {
      set(rectangle, orient, ivl);
      return true;
    }
    return false;
  }

  /// clip rectangle to b
  template <typename rectangle_type_1, typename rectangle_type_2>
  static bool intersect(const rectangle_type_1& rectangle, const rectangle_type_2& b,
                        bool consider_touch, rectangle_concept tag) {
    if(intersects(rectangle, b)) {
      intersect(rectangle, horizontal(b), HORIZONTAL, consider_touch, interval_concept());
      intersect(rectangle, vertical(b), VERTICAL, consider_touch, interval_concept());
      return true;
    }
    return false;
  }

  /// Sets this to the generalized intersection of this and the given rectangle
  template <typename rectangle_type_1, typename rectangle_type_2>
  static rectangle_type_1& generalized_intersect(rectangle_type_1& rectangle, 
                                                 const rectangle_type_2& b, rectangle_concept tag) {
    typename rectangle_traits<rectangle_type_1>::interval_type ivl = get(rectangle, HORIZONTAL);
    interval_concept::generalized_intersect(ivl, horizontal(b), interval_concept());
    horizontal(rectangle, ivl);
    ivl = vertical(rectangle);
    interval_concept::generalized_intersect(ivl, vertical(b), interval_concept());
    vertical(rectangle, ivl);
    return rectangle;
  }

  /// bloat the interval specified by orient by bloating
  template <typename rectangle_type>
  static rectangle_type& bloat(rectangle_type& rectangle, orientation_2d orient, 
                               typename rectangle_traits<rectangle_type>::coordinate_type bloating) {
    typename rectangle_traits<rectangle_type>::interval_type ivl = get(rectangle, orient);
    interval_concept::bloat(ivl, bloating);
    set(rectangle, orient, ivl);
    return rectangle;
  }

  /// bloat the Rectangle by bloating
  template <typename rectangle_type>
  static rectangle_type& bloat(rectangle_type& rectangle,
                               typename rectangle_traits<rectangle_type>::coordinate_type bloating) {
    bloat(rectangle, HORIZONTAL, bloating);
    return bloat(rectangle, VERTICAL, bloating);
  }

  /// bloat the interval cooresponding to orient by bloating in dir direction
  template <typename rectangle_type>
  static rectangle_type& bloat(rectangle_type& rectangle, direction_2d dir,
                               typename rectangle_traits<rectangle_type>::coordinate_type bloating) {
    typename rectangle_traits<rectangle_type>::interval_type ivl = get(rectangle, orientation_2d(dir));
    interval_concept::bloat(ivl, direction_1d(dir), bloating);
    set(rectangle, orientation_2d(dir), ivl);
    return rectangle;
  }

  /// shrink the interval specified by orient by bloating
  template <typename rectangle_type>
  static rectangle_type& shrink(rectangle_type& rectangle, orientation_2d orient, 
                                typename rectangle_traits<rectangle_type>::coordinate_type shrinking) {
    return bloat(rectangle, orient, -shrinking);
  }

  /// shrink the Rectangle by bloating
  template <typename rectangle_type>
  static rectangle_type& shrink(rectangle_type& rectangle, 
                                typename rectangle_traits<rectangle_type>::coordinate_type shrinking) {
    return bloat(rectangle, -shrinking);
  }

  /// shrink the interval cooresponding to orient by bloating in dir direction
  template <typename rectangle_type>
  static rectangle_type& shrink(rectangle_type& rectangle, direction_2d dir,
                                typename rectangle_traits<rectangle_type>::coordinate_type shrinking) {
    return bloat(rectangle, dir, -shrinking);
  }

  /// encompass interval on orient
  template <typename rectangle_type, typename interval_type>
  static bool encompass(const rectangle_type& rectangle, const interval_type& b,
                        orientation_2d orient, interval_concept tag) {
    typename rectangle_traits<rectangle_type>::interval_type ivl = get(rectangle, orient);
    if(interval_concept::encompass(ivl, b, tag)) {
      set(rectangle, orient, ivl);
      return true;
    }
    return false;
  }

  /// enlarge rectangle to encompass the Rectangle b
  template <typename rectangle_type_1, typename rectangle_type_2>
  static bool encompass(rectangle_type_1& rectangle, const rectangle_type_2& b,
                        rectangle_concept tag) {
    //note that operator | is intentional because both should be called regardless
    return encompass(rectangle, horizontal(b), HORIZONTAL, interval_concept()) |
      encompass(rectangle, vertical(b), VERTICAL, interval_concept());
  }

  //for consistency we should have encompass(rectangle, point) and encompass(interval, coordinate)

  /// returns the center of the rectangle
  template <typename rectangle_type>
  static inline typename registration<rectangle_type>::center_type
  center(const rectangle_type& rectangle) {
    return typename registration<rectangle_type>::center_type(interval_concept::center(horizontal(rectangle)),
                                                              interval_concept::center(vertical(rectangle)));
  }

  template <typename rectangle_type>
  static typename registration<rectangle_type>::center_type
  get_corner(const rectangle_type& rectangle, direction_2d direction_facing, direction_1d direction_turning) {
    typedef typename rectangle_traits<rectangle_type>::coordinate_type Unit;
    Unit u1 = get(rectangle, direction_facing);
    Unit u2 = get(rectangle, direction_facing.turn(direction_turning));
    if(orientation_2d(direction_facing).to_int()) std::swap(u1, u2);
    return point_concept::construct<typename registration<rectangle_type>::center_type>(u1, u2);
  }

  template <typename rectangle_type>
  static rectangle_type get_half(const rectangle_type& rectangle, direction_2d dir) {
    rectangle_type retval(rectangle);
    set(retval, orientation_2d(dir), 
        interval_concept::get_half(get(rectangle, orientation_2d(dir)), direction_1d(dir)));
    return retval;
  }

  template <typename rectangle_type_1, typename rectangle_type_2>
  static bool join_with(rectangle_type_1& rectangle, const rectangle_type_2& b) {
    typedef typename rectangle_traits<rectangle_type_1>::interval_type Interval1;
    typedef typename rectangle_traits<rectangle_type_2>::interval_type Interval2;
    Interval1 hi1 = get(rectangle, HORIZONTAL);
    Interval1 vi1 = get(rectangle, VERTICAL);
    Interval2 hi2 = get(b, HORIZONTAL), vi2 = get(b, VERTICAL);
    Interval1 temp;
    if (interval_concept::equivalence(hi1, hi2) && interval_concept::join_with(vi1, vi2)) {
      vertical(rectangle, vi1);
      return true;
    }
    if (interval_concept::equivalence(vi1, vi2) && interval_concept::join_with(hi1, hi2)) {
      horizontal(rectangle, hi1);
      return true;
    }
    return false;
  }

  //TODO: Add distance_type<geometry_type>::type and difference_type<geometry_type>::type
  
//   /// Returns the euclidian distance between the edge of the
//   /// rectangle and the point. Returns 0 if point is inside the
//   /// rectangle
//   template <class T2>
//   double
//   euclidianDistance(const PointImpl<T2> & p) const;

//   /// Returns the square of the euclidian distance between the edge of the
//   /// rectangle and the point. Returns 0 if point is inside the
//   /// rectangle
//   template <class T2>
//   UnsignedLongUnit
//   squareEuclidianDistance(const PointImpl<T2> & p) const;

//   /// Returns the manhattan distance (deltax + deltay) between the
//   /// edge of the rectangle and the point. Returns 0 if point is
//   /// inside the rectangle
//   template <class T2>
//   double
//   manhattanDistance(const PointImpl<T2> & p) const;

//   /// Returns the distance between this rectangle and a unit or a point
//   /// returns 0 if point or coordinate lies within
//   /// the range of the rectangle
//   Unit distance(Unit u, Orientation2D o) const;
//   template <class T2>
//   Unit distance(const PointImpl<T2> & p, Orientation2D o) const;

//   /// Returns the distance between two rectangles along the given orientation
//   template <class T2>
//   Unit distance(const RectangleImpl<T2> & r2, Orientation2D o) const;

//   /// Returns closest euclidian distance between the edges of the two rectangles.
//   /// Returns 0 if they overlap or touch
//   template <class T2>
//   double euclidianDistance(const RectangleImpl<T2> & r2) const;

//   /// Returns the square of the closest euclidian distance between the edges of the two rectangles.
//   /// Returns 0 if they overlap or touch
//   template <class T2>
//   UnsignedLongUnit squareEuclidianDistance(const RectangleImpl<T2> & r2) const;

//   /// Returns manhattan distance (deltax + deltay) between the two rectangles
//   /// Returns 0 if they overlap or touch
//   template <class T2>
//   Unit manhattanDistance(const RectangleImpl<T2> & r2) const;

  template <typename rectangle_type_1, typename rectangle_type_2>
  class less : public std::binary_function<const rectangle_type_1&, const rectangle_type_2&, bool> {
  private:
    orientation_2d orient_;
  public:
    inline less(orientation_2d orient = VERTICAL) : orient_(orient) {}
    inline bool operator () (const rectangle_type_1& a,
                             const rectangle_type_2& b) const {
      typedef typename rectangle_traits<rectangle_type_1>::coordinate_type Unit;
      Unit vl1 = interval_concept::get(get(a, orient_), LOW); 
      Unit vl2 = interval_concept::get(get(b, orient_), LOW); 
      if(vl1 > vl2) return false;
      if(vl1 == vl2) {
        orientation_2d perp = orient_.get_perpendicular();
        Unit hl1 = interval_concept::get(get(a, perp), LOW);
        Unit hl2 = interval_concept::get(get(b, perp), LOW);
        if(hl1 > hl2) return false;
        if(hl1 == hl2) {
          Unit vh1 = interval_concept::get(get(a, orient_), HIGH); 
          Unit vh2 = interval_concept::get(get(b, orient_), HIGH); 
          if(vh1 > vh2) return false;
          if(vh1 == vh2) {
            Unit hh1 = interval_concept::get(get(a, perp), HIGH);
            Unit hh2 = interval_concept::get(get(b, perp), HIGH);
            return hh1 < hh2;
          }
        }
      }
      return true;
    }
    
  };
};

}
#endif

