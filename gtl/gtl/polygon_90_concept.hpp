/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
#ifndef GTL_POLYGON_90_CONCEPT_HPP
#define GTL_POLYGON_90_CONCEPT_HPP
namespace gtl {
struct polygon_90_concept {
  inline polygon_90_concept() {}

  template <typename polygon_type>
  struct registration {
    typedef typename polygon_traits<polygon_type>::coordinate_type coordinate_type;
    typedef point_data<coordinate_type> center_type;
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

  template <typename polygon_type, typename polygon_type2>
  static polygon_type copy_construct(const polygon_type2& polygon) { 
    return construct(begin(polygon), end(polygon)); 
  }

  template <typename polygon_type>
  static std::size_t size(const polygon_type& polygon) {
    return polygon_traits<polygon_type>::size(polygon);
  }

  template <typename polygon_type_1, typename polygon_type_2>
  static polygon_type_1& assign(polygon_type_1& lvalue, const polygon_type_2& rvalue) {
    set_compact(lvalue, begin_compact(rvalue), end_compact(rvalue));
    return lvalue;
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

  template <typename polygon_type>
  static rectangle_data<typename polygon_traits<polygon_type>::coordinate_type> 
  bounding_box(const polygon_type& polygon) {
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
    typedef rectangle_data<coordinate_type> rectangle_type;
    rectangle_type return_value = rectangle_concept::construct<rectangle_type>(xmin, ymin, xmax, ymax);
    return return_value;
  }

  template <typename polygon_type>
  static typename registration<polygon_type>::center_type
  center(const polygon_type& polygon) {
    return rectangle_concept::center(bounding_box(polygon));
  }

  template <typename polygon_type>
  static typename polygon_traits<polygon_type>::coordinate_type 
  area(const polygon_type& polygon) {
    //for (long i = 2; i < _size; i += 2) res += ((double)_vertex[i-1])*((double)(_vertex[i-2] - _vertex[i]));
    typedef typename polygon_traits<polygon_type>::coordinate_type coordinate_type;
    typedef typename polygon_traits<polygon_type>::compact_iterator_type iterator;
    coordinate_type retval = 0;
    iterator itr = begin_compact(polygon);
    iterator end_itr = end_compact(polygon);
    coordinate_type firstx = *itr;
    coordinate_type prevx = *itr;
    ++itr;
    if(itr == end_itr) return 0;
    coordinate_type prevy = *itr;
    ++itr;
    for( ; itr != end_itr; ++itr) {
      coordinate_type x = *itr;
      ++itr;
      if(itr == end_itr) break;
      coordinate_type y = *itr;
      retval += (prevy * (prevx - x));
      prevy = y;
      prevx = x;
    }
    retval += (prevy * (prevx - firstx));
    return retval >= 0 ? retval : -retval;
  }

  /// get the perimeter of the polygon
  template <typename polygon_type>
  static typename polygon_traits<polygon_type>::coordinate_type
  perimeter(const polygon_type& polygon) {
    typedef typename polygon_traits<polygon_type>::coordinate_type coordinate_type;
    typedef typename polygon_traits<polygon_type>::iterator_type iterator;
    typedef typename std::iterator_traits<iterator>::value_type point_type;
    coordinate_type return_value = 0;
    point_type previous_point, first_point;
    iterator itr = begin(polygon);
    iterator itr_end = end(polygon);
    if(itr == itr_end) return return_value;
    previous_point = first_point = *itr;
    ++itr;
    for( ; itr != itr_end; ++itr) {
      ++itr;
      if(itr == itr_end) break;
      point_type current_point = *itr;
      return_value += point_concept::manhattan_distance(current_point, previous_point);
      previous_point = current_point;
    }
    return_value += point_concept::manhattan_distance(previous_point, first_point);
    return return_value;
  }

  /// check if point is inside polygon 
  template <typename polygon_type, typename point_type>
  static bool contains(const polygon_type& polygon, const point_type& point, 
                       bool consider_touch, point_concept pc) {
    typedef typename polygon_traits<polygon_type>::coordinate_type coordinate_type;
    typedef typename polygon_traits<polygon_type>::iterator_type iterator;
    typedef typename std::iterator_traits<iterator>::value_type point_type;
    iterator iter, iter_end;
    iter_end = end(polygon);
    iter = begin(polygon);
    point_type prev_pt = *iter;
    unsigned int num = size(polygon);
    unsigned int counts[2] = {0, 0};
    for(unsigned int i = 0; i < num; ++i) {
      if(i == num-1) iter = begin(polygon);
      else ++iter;
      point_type current_pt = *iter;
      if(point_concept::get<HORIZONTAL>(current_pt) == 
         point_concept::get<HORIZONTAL>(prev_pt)) {
        unsigned int index = point_concept::get<HORIZONTAL>(current_pt) > 
          point_concept::get<HORIZONTAL>(point);
        unsigned int increment = 0;
        interval_data<coordinate_type> ivl = 
          interval_concept::construct<interval_data<coordinate_type> >(point_concept::get<VERTICAL>(current_pt), 
                                                                       point_concept::get<VERTICAL>(prev_pt));
        if(interval_concept::contains(ivl, point_concept::get<VERTICAL>(point), true, no_type())) {
          if(point_concept::get<HORIZONTAL>(current_pt) == 
             point_concept::get<HORIZONTAL>(point)) return consider_touch;
          ++increment;
          if(point_concept::get<VERTICAL>(current_pt) != 
              point_concept::get<VERTICAL>(point) &&
              point_concept::get<VERTICAL>(prev_pt) != 
              point_concept::get<VERTICAL>(point)) {
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

//   //awaiting re-implementation of iterator_edges and edge concept
//   template <typename polygon_type, typename point_type>
//   static point_type project(const polygon_type& polygon, const point_type& point) const {
//     point_type return_value;
//     typedef iterator_edges<typename polygon_traits<polygon_type>::iterator_type,
//       edge_data<typename polygon_traits<polygon_type>::coordinate_type> > iterator;
//     iterator iter = begin_edges();
//     iterator iter_end = end_edges();
//     double dist = 0;
//     bool first_segment = true;
//     for( ; iter != iter_end; ++iter) {
//       point_type segement_point = segment_concept::project_point(*iter, point);
//       double seg_dist = point_concept::euclidian_distance(point, segment_point);
//       if(first_segment || seg_dist < dist) {
//         dist = seg_dist;
//         return_value = segment_point;
//       }
//     }
//     return return_value;
//   }
  
//   //awaiting re-implementation of iterator_edges and edge concept
//   template <typename polygon_type, typename point_type>
//   static point_type project(point_type& result, const polygon_type& polygon, 
//                             const point_type& point, direction_2d dir) {
//     typedef iterator_edges<typename polygon_traits<polygon_type>::iterator_type,
//       edge_data<typename polygon_traits<polygon_type>::coordinate_type> > iterator;
//     iterator iter = begin_edges();
//     iterator iter_end = end_edges();
//     double dist = 0;
//     bool found = false;
//     bool first_segment = true;
//     for( ; iter != iter_end; ++iter) {
//       point_type segement_point;
//       if(segment_concept::project_point(segment_point, *iter, point, dir)) {
//            found = true;
//            double seg_dist = point_concept::euclidian_distance(point, segment_point);
//            if(first_segment || seg_dist < dist) {
//              dist = seg_dist;
//              result = segment_point;
//       }
//     }
//     return found;
//   }

  template <typename polygon_type, typename coordinate_type_1, typename coordinate_type_2>
  static polygon_type& move(polygon_type& polygon, coordinate_type_1 x_displacement, coordinate_type_2 y_displacement) {
    std::vector<typename polygon_traits<polygon_type>::coordinate_type> coords;
    coords.reserve(size(polygon));
    bool pingpong = true;
    for(typename polygon_traits<polygon_type>::compact_iterator_type iter = begin_compact(polygon); 
        iter != end_compact(polygon); ++iter) {
      coords.push_back((*iter) + (pingpong ? x_displacement : y_displacement));
      pingpong = !pingpong;
    }
    set_compact(polygon, coords.begin(), coords.end());
    return polygon;
  }
  
  /// move polygon by delta in orient
  template <typename polygon_type, typename coordinate_type_1>
  static polygon_type& move(polygon_type& polygon, orientation_2d orient, coordinate_type_1 displacement) {
    if(orient == HORIZONTAL) {
      move(displacement, 0);
    } else {
      move(0, displacement);
    }
    return polygon;
  }

};

}
#endif
