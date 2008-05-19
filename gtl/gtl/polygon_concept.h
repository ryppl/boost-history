/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
struct polygon_concept {
  inline polygon_concept() {}

  template <typename polygon_type>
  struct registration {
    typedef typename polygon_traits<polygon_type>::coordinate_type coordinate_type;
  };


  template<typename polygon_type, typename iterator_type>
  static void set(polygon_type& polygon, iterator_type input_begin, iterator_type input_end) {
    polygon_traits<polygon_type>::set(polygon, input_begin, input_end);
  }

  template<typename polygon_type, typename rectangle_type>
  static void set(polygon_type& polygon, const rectangle_type& rect) {
    typename polygon_traits<polygon_type>::coordinate_type coords[4] = 
      {rectangle_concept::xl(rect), rectangle_concept::yl(rect),
       rectangle_concept::xh(rect), rectangle_concept::yh(rect)};
    set(polygon, coords, coords+4);
  }
   
  template<typename polygon_type, typename point_iterator_type>
  static void set_points(polygon_type& polygon, point_iterator_type begin_point, point_iterator_type end_point) {
    return set(iterator_points_to_compact<point_iterator_type, typename point_iterator_type::value_type>(begin_point),
        iterator_points_to_compact<point_iterator_type, typename point_iterator_type::value_type>(end_point));
  }

  template <typename polygon_type>
  static typename polygon_traits<polygon_type>::iterator_type begin(const polygon_type& polygon) {
    return polygon_traits<polygon_type>::begin(polygon);
  }
  
  template <typename polygon_type>
  static typename polygon_traits<polygon_type>::iterator_type end(const polygon_type& polygon) {
    return polygon_traits<polygon_type>::end(polygon);
  }
  
  template <typename polygon_type>
  static iterator_compact_to_points<typename polygon_traits<polygon_type>::iterator_type,
                         point_data<typename polygon_traits<polygon_type>::coordinate_type> > 
  begin_points(const polygon_type& polygon) {
    return iterator_compact_to_points<typename polygon_traits<polygon_type>::iterator_type,
      point_data<typename polygon_traits<polygon_type>::coordinate_type> > (begin(polygon), end(polygon));
  }

  template <typename polygon_type>
  static iterator_compact_to_points<typename polygon_traits<polygon_type>::iterator_type,
                         point_data<typename polygon_traits<polygon_type>::coordinate_type> > 
  end_points(const polygon_type& polygon) {
    return iterator_compact_to_points<typename polygon_traits<polygon_type>::iterator_type,
      point_data<typename polygon_traits<polygon_type>::coordinate_type> > (end(polygon), end(polygon));
  }

  template<typename T, class iT>
  static T construct(iT inputBegin, iT inputEnd) { return polygon_traits<T>::construct(inputBegin, inputEnd); }

  template<typename polygon_type, typename rectangle_type>
  static polygon_type construct_from_rectangle(const rectangle_type& rect) {
    polygon_type poly;
    set(poly, rect);
    return poly;
  }

  template <typename polygon_type, typename polygon_type2>
  static polygon_type copy_construct(const polygon_type2& polygon) { 
    return construct(polygon_concept::begin(polygon), polygon_concept::end(polygon)); 
  }

  template <typename polygon_type>
  static std::size_t size(const polygon_type& polygon) {
    return polygon_traits<polygon_type>::size(polygon);
  }

  template <typename polygon_type_1, typename polygon_type_2>
  static polygon_type_1& assign(polygon_type_1& lvalue, const polygon_type_2& rvalue) {
    set(lvalue, begin(rvalue), end(rvalue));
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
    typedef typename polygon_traits<polygon_type>::iterator_type iterator;
    iterator itr = begin(polygon);
    coordinate_type firstx = *itr;
    coordinate_type minX = firstx;
    ++itr;
    iterator end_itr = end(polygon);
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
    typedef typename polygon_traits<polygon_type>::iterator_type iterator;
    coordinate_type xmin = 0;
    coordinate_type ymin = 0;
    coordinate_type xmax = 0;
    coordinate_type ymax = 0;
    bool first_iteration = true;
    iterator itr_end = end(polygon);
    for(iterator itr = begin(polygon); itr != itr_end; ++itr) {
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
  static typename polygon_traits<polygon_type>::coordinate_type 
  area(const polygon_type& polygon) {
    //for (long i = 2; i < _size; i += 2) res += ((double)_vertex[i-1])*((double)(_vertex[i-2] - _vertex[i]));
    typedef typename polygon_traits<polygon_type>::coordinate_type coordinate_type;
    typedef typename polygon_traits<polygon_type>::iterator_type iterator;
    coordinate_type retval = 0;
    iterator itr = begin(polygon);
    iterator end_itr = end(polygon);
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
    typedef typename polygon_traits<polygon_type>::iterator_type iterator_type;
    typedef iterator_compact_to_points<iterator_type, point_data<coordinate_type> > iterator;
    coordinate_type return_value = 0;
    point_data<coordinate_type> previous_point, first_point;
    iterator itr = begin_points(polygon);
    iterator itr_end = end_points(polygon);
    if(itr == itr_end) return return_value;
    previous_point = first_point = *itr;
    ++itr;
    for( ; itr != itr_end; ++itr) {
      ++itr;
      if(itr == itr_end) break;
      point_data<coordinate_type> current_point = *itr;
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
    typedef typename polygon_traits<polygon_type>::iterator_type iterator_type;
    typedef iterator_compact_to_points<iterator_type, point_data<coordinate_type> > iterator;
    iterator iter, iter_end;
    iter_end = end_points(polygon);
    iter = begin_points(polygon);
    point_data<coordinate_type> prev_pt = *iter;
    unsigned int num = size(polygon);
    unsigned int counts[2] = {0, 0};
    for(unsigned int i = 0; i < num; ++i) {
      if(i == num-1) iter = begin_points(polygon);
      else ++iter;
      point_data<coordinate_type> current_pt = *iter;
      if(point_concept::get<HORIZONTAL>(current_pt) == 
         point_concept::get<HORIZONTAL>(prev_pt)) {
        unsigned int index = point_concept::get<HORIZONTAL>(current_pt) > 
          point_concept::get<HORIZONTAL>(point);
        unsigned int increment = 0;
        interval_data<coordinate_type> ivl = 
          interval_concept::construct<interval_data<coordinate_type> >(point_concept::get<VERTICAL>(current_pt), 
                                                                       point_concept::get<VERTICAL>(prev_pt));
        if(interval_concept::contains(ivl, point_concept::get<VERTICAL>(point), true)) {
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
  static void move(polygon_type& polygon, coordinate_type_1 x_displacement, coordinate_type_2 y_displacement) {
    std::vector<typename polygon_traits<polygon_type>::coordinate_type> coords;
    coords.reserve(size(polygon));
    bool pingpong = true;
    for(typename polygon_traits<polygon_type>::iterator_type iter = begin(polygon); 
        iter != end(polygon); ++iter) {
      coords.push_back((*iter) + (pingpong ? x_displacement : y_displacement));
      pingpong = !pingpong;
    }
    set(polygon, coords.begin(), coords.end());
  }
  
  /// move polygon by delta in orient
  template <typename polygon_type, typename coordinate_type_1>
  static void move(polygon_type& polygon, orientation_2d orient, coordinate_type_1 displacement) {
    if(orient == HORIZONTAL) {
      move(displacement, 0);
    } else {
      move(0, displacement);
    }
  }

};


