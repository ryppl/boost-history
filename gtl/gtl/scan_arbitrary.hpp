
/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
#ifndef GTL_SCAN_ARBITRARY_HPP
#define GTL_SCAN_ARBITRARY_HPP
namespace gtl {
  
  template <typename user_coordinate, typename internal_coordinate, typename property_type>
  class scan_intersect {
  public: //change to private when done testing
    //definitions
    typedef point_data<user_coordinate> user_point;
    typedef point_data<internal_coordinate> internal_point;

    typedef std::pair<user_point, user_point> user_edge;
    typedef std::pair<internal_point, internal_point> internal_edge;

    class less_scanline_edge;

    typedef std::map<property_type, int> property_count;
    typedef std::pair<internal_point, property_count> half_edge;
    typedef std::vector<half_edge> half_edge_vector;
    typedef std::pair<internal_point, half_edge_vector> vertex_data;
    typedef std::map<property_type, int> property_data;
    typedef std::pair<internal_edge, property_data> scanline_elment;
    typedef std::map<internal_edge, property_data, less_scanline_edge> scanline_data;
    typedef std::set<property_type> property_combination;
    typedef std::pair<property_combination, internal_edge> output_element;
    typedef std::set<internal_point> intersection_event_queue;
    typedef std::vector<vertex_data> vertex_vector;
    
  private:
    //data memebers
    internal_coordinate current_scanline_stop_;
    bool handling_intersections_;
    vertex_vector input_vertices_; //stores the input
    vertex_vector output_vertices_; //stores the output
    scanline_data scanline_; 
    intersection_event_queue intersections_; //intersection points that need to be processed
    vertex_vector intersection_vertices_; //vertex events created by handling intersections
    property_count vertical_count_; //count coming in from below due to vertical edges
  public:
    //interfaces
    scan_intersect() : scanline_(less_scanline_edge(&current_scanline_stop_, &handling_intersections_)) {}

    template <typename geometry_type>
    void insert(const geometry_type& geometry_object, const property_type& property_value, bool is_hole = false) {
      insert(geometry_object, property_value, is_hole, typename geometry_concept<geometry_type>::type());
    }

    void scan() {
      sort_input();
      typename vertex_vector::iterator input_begin = input_vertices_.begin();
      typename vertex_vector::iterator input_end = input_vertices_.end();
      handling_intersections_ = false;
      while(input_begin != input_end) {
        internal_coordinate next_scanline_stop = point_concept::get((*input_begin).first, HORIZONTAL);
        if(next_scanline_stop != current_scanline_stop_) {
          //this call modifies current_scanline_stop to be the coordinate of the first intersection if any before the next input
          if(handle_intersections(next_scanline_stop)) {
            vertex_merge_iterator<typename vertex_vector::iterator, typename vertex_vector::const_iterator> 
              merge_begin(input_begin, input_end, intersection_vertices_.begin(), intersection_vertices_.end());
            vertex_merge_iterator<typename vertex_vector::iterator, typename vertex_vector::const_iterator> 
              merge_end(input_begin, input_end, intersection_vertices_.begin(), intersection_vertices_.end());
            while(merge_begin != merge_end && point_concept::get((*merge_begin).first, HORIZONTAL) == current_scanline_stop_) {
              scan_element(*merge_begin); 
              ++merge_begin;
            }
          } else {
            //no pending intersection events, so advance the scanline to the new stop
            current_scanline_stop_ = next_scanline_stop;
          }
        } else {
          scan_element(*input_begin);
          ++input_begin;
        }
      }
    }

    //output is std::map<std::set<property_type>, back_intertable_container<geometry_type> >
    //geometry_type is either polygon_type or polygon_with_holes_type
    //back_insertable_container is any container that supports container.insert(container.end(), value_type)
    template <typename T>
    void getResult(T& output) {
      //TODO
    }

  public: //change to private when done testing
    //non-static member functions for internal use

    inline void sort_input() {
      less_vertex_data lessF;
      std::sort(input_vertices_.begin(), input_vertices_.end(), lessF);
    }

    template <typename polygon_type>
    void insert(const polygon_type& polygon_object, const property_type& property_value, bool is_hole, 
                polygon_concept tag) {
      bool first_iteration = true;
      bool second_iteration = true;
      internal_point first_point;
      internal_point second_point;
      internal_point previous_previous_point;
      internal_point previous_point;
      internal_point current_point;
      vertex_data current_vertex;
      direction_1d winding = polygon_concept::winding(polygon_object);
      for(typename polygon_traits<polygon_type>::iterator_type itr = polygon_concept::begin(polygon_object);
          itr != polygon_concept::end(polygon_object); ++itr) {
        point_concept::assign(current_point, *itr);
        if(first_iteration) {
          first_iteration = false;
          first_point = previous_point = current_point;
        } else if(second_iteration) {
          if(previous_point != current_point) {
            second_iteration = false;
            previous_previous_point = previous_point;
            second_point = previous_point = current_point;
          }
        } else {
          if(previous_point != current_point) {
            create_vertex(current_vertex, previous_previous_point, previous_point, current_point, winding,
                          is_hole, property_value);
            input_vertices_.push_back(current_vertex);
            previous_previous_point = previous_point;
            previous_point = current_point;
          }
        }
      }
      current_point = first_point;
      if(!first_iteration && !second_iteration) {
        if(previous_point != current_point) {
          create_vertex(current_vertex, previous_previous_point, previous_point, current_point, winding,
                        is_hole, property_value);
          input_vertices_.push_back(current_vertex);
          previous_previous_point = previous_point;
          previous_point = current_point;
        }
        current_point = second_point;
        create_vertex(current_vertex, previous_previous_point, previous_point, current_point, winding,
                      is_hole, property_value);
        input_vertices_.push_back(current_vertex);
        previous_previous_point = previous_point;
        previous_point = current_point;
      }
    }

    template <typename polygon_with_holes_type>
    void insert(const polygon_with_holes_type& polygon_with_holes_object, const property_type& property_value, bool is_hole, 
                polygon_with_holes_concept tag) {
      insert(polygon_with_holes_object, property_value, is_hole, polygon_concept());
      for(typename polygon_with_holes_traits<polygon_with_holes_type>::iterator_holes_type itr = 
            polygon_with_holes_concept::begin_holes(polygon_with_holes_object);
          itr != polygon_with_holes_concept::end_holes(polygon_with_holes_object); ++itr) {
        insert(*itr, property_value, !is_hole, polygon_concept());
      }
    }

    template <typename rectangle_type>
    void insert(const rectangle_type& rectangle_object, const property_type& property_value, bool is_hole, 
                rectangle_concept tag) {
      polygon_90_data<user_coordinate> poly;
      polygon_90_concept::set_rectangle(poly, rectangle_object);
      insert(poly, property_value, is_hole, polygon_concept());
    }

    void scan_element(const vertex_data& vertex) {
      for(typename half_edge_vector::const_iterator itr = vertex.second.begin(); itr != vertex.second.end(); ++itr) {
        //scan_half_edge(vertex.first, *itr);
      }
    }

    void handle_intersection(internal_coordinate intersection_intercept) {}

    bool handle_intersections(internal_coordinate next_scanline_stop) {
      if(intersections_.empty() || point_concept::get(*(intersections_.begin()), HORIZONTAL) > next_scanline_stop)
        return false;
      current_scanline_stop_ = point_concept::get(*(intersections_.begin()), HORIZONTAL);
      handling_intersections_ = true;
      do {
        handle_intersection(point_concept::get(*(intersections_.begin()), VERTICAL));
        intersections_.erase(intersections_.begin());
      } while(current_scanline_stop_ == point_concept::get(*(intersections_.begin()), HORIZONTAL));
      handling_intersections_ = false;
      return true;
    }
      


  public: //change to private when done testing
    //static helper functions and member classes

    static inline void create_vertex(vertex_data& current_vertex, 
                                     const internal_point& previous_point, 
                                     const internal_point& current_point, 
                                     const internal_point& next_point, 
                                     direction_1d winding,
                                     bool is_hole, const property_type& property) {
      current_vertex.first = current_point;
      current_vertex.second.clear();
      int multiplier = 1;
      if(winding == CLOCKWISE) 
        multiplier = -1;
      if(is_hole)
        multiplier *= -1;
      half_edge he;
      he.first = next_point;
      he.second[property] = multiplier * (point_concept::distance(next_point, current_point, HORIZONTAL) == 0 ? -1: 1);
      current_vertex.second.push_back(he);
      he.second.clear();
      he.first = previous_point;
      he.second[property] = -1 * multiplier * (point_concept::distance(previous_point, current_point, HORIZONTAL) == 0 ? -1: 1);
      current_vertex.second.push_back(he);
      sort_vertex_half_edges(current_vertex);
    }

    static inline void sort_vertex_half_edges(vertex_data& vertex) {
      less_half_edge_pair lessF(vertex.first);
      std::sort(vertex.second.begin(), vertex.second.end(), lessF);
    }

    static inline internal_coordinate compute_intercept(const internal_coordinate& dy2,
                                                        const internal_coordinate& dx1,
                                                        const internal_coordinate& dx2) {
      //intercept = dy2 * dx1 / dx2
      typedef typename coordinate_traits<internal_coordinate>::area_type area_type;
      //try to protect against potential overflow by casting to area type for multiplication computation
      return (internal_coordinate)(((area_type)dy2 * (area_type)dx1) / (area_type)dx2);
    }
    
    static inline internal_coordinate compute_intercept(const internal_edge& edge, internal_coordinate& coordinate) {
      const internal_point* pts[2] = {edge.first, edge.second};
      if(*(pts[0]) > *(pts[1]))
        std::swap(pts[0], pts[1]);
      internal_coordinate dx = point_concept::distance(*(pts[0]), *(pts[1]), HORIZONTAL);
      internal_coordinate dy = point_concept::distance(*(pts[0]), *(pts[1]), VERTICAL);
      internal_coordinate dx_intercept = coordinate - point_concept::get(*(pts[0]), HORIZONTAL);
      return point_concept::get(*(pts[0]), VERTICAL) + compute_intercept(dy, dx_intercept, dx);
    }
  
    static inline bool less_slope(const internal_coordinate& x, const internal_coordinate& y,
                                  const internal_point& pt1, const internal_point& pt2) {
      const internal_point* pts[2] = {&pt1, &pt2};
      //compute y value on edge from pt_ to pts[1] at the x value of pts[0]
      internal_coordinate dy2 = pts[1]->get(VERTICAL) - y;
      internal_coordinate dy1 = pts[0]->get(VERTICAL) - y;
      internal_coordinate dx2 = pts[1]->get(HORIZONTAL) - x;
      internal_coordinate dx1 = pts[0]->get(HORIZONTAL) - x;
      if(dx1 < 0) {
        dy1 *= -1;
        dx1 *= -1;
      }
      if(dx2 < 0) {
        dy2 *= -1;
        dx2 *= -1;
      }
      if(dx1 < dx2)
        return (dy1 < compute_intercept(dy2, dx1, dx2));
      else
        return (dy2 > compute_intercept(dy1, dx2, dx1));
    }

    class less_vertex_data {
    public:
      less_vertex_data() {}
      bool operator()(const vertex_data& lvalue, const vertex_data& rvalue) {
        return lvalue.first < rvalue.first;
      }
    };

    class less_scanline_edge {
    private:
      internal_coordinate* scanline_stop;
      bool* processing_intersections;
    public:
      less_scanline_edge(internal_coordinate* stop, bool* flag) :
        scanline_stop(stop), processing_intersections(flag) {}
      bool operator()(const internal_edge& lvalue, const internal_edge& rvalue) {
        internal_coordinate ly1 = lvalue.first.get(VERTICAL);
        internal_coordinate ly2 = lvalue.second.get(VERTICAL);
        internal_coordinate ry1 = rvalue.first.get(VERTICAL);
        internal_coordinate ry2 = rvalue.second.get(VERTICAL);
        if(std::max(ly1, ly2) < std::min(ry1, ry2))
          return true;
        if(std::min(ly1, ly2) > std::max(ry1, ry2))
          return false;
        //the edges have overlapping range in y
        //it is assumed that the edges overlap the scanline stop
        //it is futher assumed that neither edge is vertical
        internal_coordinate ly = compute_intercept(lvalue, *scanline_stop);
        internal_coordinate ry = compute_intercept(lvalue, *scanline_stop);
        if(ly < ry) return true;
        if(ly > ry) return false;
        //the two edges have the same y value at the scanline stop
        return *processing_intersections ^ 
          less_slope(*scanline_stop, ly, lvalue.first, rvalue.first);
      }
    };

    static inline void merge_count(property_count& lvalue, const property_count& rvalue) {
      for(typename property_count::const_iterator itr = rvalue.begin(); itr != rvalue.end(); ++itr) {
        //double check that default initialization of element of map to int results in zero value
        lvalue[(*itr).first] += (*itr).second;
      }
    }

    //checks if two property counts that are adjacenet in the scanline are equivilent
    //for the purposes of determining if there should be an output generated along their boundary or not
    static inline bool equivalent(const property_count& lvalue, const property_count& rvalue) {
      typename property_count::const_iterator itr1 = lvalue.begin();
      typename property_count::const_iterator itr2 = rvalue.begin();
      while(itr1 != lvalue.end() || itr2 != rvalue.end()) {
        while(itr1 != lvalue.end() && (*itr1).second <= 0) ++itr1;
        while(itr2 != rvalue.end() && (*itr2).second <= 0) ++itr2;
        if(itr1 != lvalue.end()) {
          if(itr2 != rvalue.end() && *itr1.first != *itr2.first) return false;
        } else if(itr2 != rvalue.end())
          return false;
      }
    }

    static inline void set_unqiue_property(property_combination& unqiue_property, const property_count& property) {
      unqiue_property.clear();
      for(typename property_count::const_iterator itr = property.begin(); itr != property.end(); ++itr) {
        if(*itr.second > 0)
          unqiue_property.insert(unqiue_property.end(), *itr.first);
      }
    }

    class less_half_edge_pair {
    private:
      internal_point pt_;
    public:
      less_half_edge_pair(const internal_point& pt) : pt_(pt) {}
      bool operator()(const half_edge& e1, const half_edge& e2) {
        const internal_point& pt1 = e1.first;
        const internal_point& pt2 = e2.first;
        if(point_concept::get(pt1, HORIZONTAL) == 
           point_concept::get(pt_, HORIZONTAL)) {
          //vertical edge is always largest
          return false;
        }
        if(point_concept::get(pt2, HORIZONTAL) == 
           point_concept::get(pt_, HORIZONTAL)) {
          //if half edge 1 is not vertical its slope is less than that of half edge 2
          return point_concept::get(pt1, HORIZONTAL) != point_concept::get(pt2, HORIZONTAL);
        }
        return less_slope(point_concept::get(pt_, HORIZONTAL),
                          point_concept::get(pt_, VERTICAL), pt1, pt2);
      }
    };

    template <typename iterator1, typename iterator2>
    class vertex_merge_iterator {
    private:
      iterator1 begin1_, end1_;
      iterator2 begin2_, end2_;
      vertex_data merged_vertex_;
      unsigned int state_;
    public:
      typedef std::forward_iterator_tag iterator_category;
      typedef vertex_data value_type;
      typedef std::ptrdiff_t difference_type;
      typedef const value_type* pointer; //immutable
      typedef const value_type& reference; //immutable
      vertex_merge_iterator(iterator1 begin1, iterator1 end1, iterator2 begin2, iterator2 end2) :
        begin1_(begin1), end1_(end1), begin2_(begin2), end2_(end2) {
        set_state();
        if(state_ == 0)
          handle_equal();
      }
      
      bool operator==(const vertex_merge_iterator& that) {
        return begin1_ == that.begin1_ && begin2_ == that.begin2_;
      }
      bool operator!=(const vertex_merge_iterator& that) {
        return !((*this) == that);
      }

      reference operator*() {
        if(state_ == 1) return *begin1_;
        if(state_ == 2) return *begin2_;
        if(state_ == 3) return merged_vertex_;
        //assert this return statement should not be used
        return merged_vertex_;
      }

      inline vertex_merge_iterator& operator++() {
        if(state_ == 1) ++begin1_;
        if(state_ == 2) ++begin1_;
        set_state();
        if(state_ == 0)
          handle_equal();
        return *this;
      }
      inline vertex_merge_iterator operator++(int) {
        vertex_merge_iterator tmp(*this);
        ++(*this);
        return tmp;
      }

      void handle_equal() {
        half_edge_vector half_edges;
        merged_vertex_.first = (*begin1_).first;
        while(begin1_ != end1_ && (*begin1_).first == merged_vertex_.first) {
          half_edges.insert(half_edges.end(), (*begin1_).second.begin(), (*begin1_).second.end());
          ++begin1_;
        }
        while(begin2_ != end2_ && (*begin2_).first == merged_vertex_.first) {
          half_edges.insert(half_edges.end(), (*begin2_).second.begin(), (*begin2_).second.end());
          ++begin2_;
        }
        less_half_edge_pair lessF(merged_vertex_.first);
        std::sort(half_edges.begin(), half_edges.end(), lessF);
        merged_vertex_.second.push_back(half_edges.front());
        for(unsigned int i = 1; i < half_edges.size(); ++i) {
          if(!lessF(merged_vertex_.second.back(), half_edges[i])) {
            merge_count(merged_vertex_.second.back().second, half_edges[i].second);
          } else {
            merged_vertex_.second.push_back(half_edges[i]);
          }
        }
        state_ = 3;
      }

      void set_state() {
        if(begin1_ == end1_) {
          if(begin2_ == end2_) {
            state_ = 4;
          } else {
            state_ = 2;
          }
        } else if(begin2_ == end2_) {
          state_ = 1;
        } else {
          if((*begin1_).first == (*begin2_).first) {
            state_ = 0;
          } else if((*begin1_).first < (*begin2_).first) {
            state_ = 1;
          } else {
            state_ = 2;
          }
        }
      }
    };
    
  public:
    //test functions
    static std::ostream& print (std::ostream& o, const property_count& c)
    {
      o << "count: {";
      for(typename property_count::const_iterator itr = c.begin(); itr != c.end(); ++itr) {
        o << ((*itr).first) << ":" << ((*itr).second) << " ";
      }
      return o << "} ";
    }

    static std::ostream& print (std::ostream& o, const half_edge& he)
    {
      return print(o << "half edge: (" << (he.first) << ", ", (he.second)) << ") ";
    }

    static std::ostream& print (std::ostream& o, const half_edge_vector& hev)
    {
      o << "half edges: {";
      for(unsigned int i = 0; i < hev.size(); ++i) {
        print(o, (hev[i])) << " ";
      }
      return o << "} ";
    }

    static std::ostream& print (std::ostream& o, const vertex_data& v)
    {
      return print(o << "vertex: <" << (v.first) << ", ", (v.second)) << "> ";
    }

    static std::ostream& print (std::ostream& o, const vertex_vector& vv)
    {
      o << "vertices: {";
      for(unsigned int i = 0; i < vv.size(); ++i) {
        print(o, (vv[i])) << " ";
      }
      return o << "} ";
    }



    static inline bool test_insertion() {
      scan_intersect si;
      rectangle_data<user_coordinate> rect;
      rectangle_concept::xl(rect, 0);
      rectangle_concept::yl(rect, 1);
      rectangle_concept::xh(rect, 10);
      rectangle_concept::yh(rect, 11);

      si.insert(rect, 333);
      print(std::cout, si.input_vertices_) << std::endl;
      
      user_point pts[4] = {user_point(0, 0), user_point(10,-3), user_point(13, 8), user_point(0, 0) };
      polygon_data<user_coordinate> poly;
      scan_intersect si2;
      poly.set(pts, pts+3);
      si2.insert(poly, 444);
      si2.sort_input();
      print(std::cout, si2.input_vertices_) << std::endl;
      scan_intersect si3;
      poly.set(pts, pts+4);
      si3.insert(poly, 444);
      si3.sort_input();
      std::cout << (si2.input_vertices_ == si3.input_vertices_) << std::endl;
      std::reverse(pts, pts+4);
      scan_intersect si4;
      poly.set(pts, pts+4);
      si4.insert(poly, 444);
      si4.sort_input();
      std::cout << (si2.input_vertices_ == si4.input_vertices_) << std::endl;
      std::reverse(pts, pts+3);
      scan_intersect si5;
      poly.set(pts, pts+4);
      si5.insert(poly, 444);
      si5.sort_input();
      std::cout << (si2.input_vertices_ == si5.input_vertices_) << std::endl;
      
      return true;
    }



    static inline bool test_less_slope() {
      internal_coordinate x = 0;
      internal_coordinate y = 0;
      internal_point pt1(10, 10);
      internal_point pt2(10, 10);
      if(less_slope(x, y, pt1, pt2)) {
        std::cout << "fail1\n";
        return false;
      }
      if(less_slope(x, y, pt2, pt1)) {
        std::cout << "fail2\n";
        return false;
      }
      pt1.set(HORIZONTAL, 11);
      if(!less_slope(x, y, pt1, pt2)) {
        std::cout << "fail3\n";
        return false;
      }
      if(less_slope(x, y, pt2, pt1)) {
        std::cout << "fail4\n";
        return false;
      }
      pt1 = internal_point(-10, -10);
      if(less_slope(x, y, pt1, pt2)) {
        std::cout << "fail5\n";
        return false;
      }
      if(less_slope(x, y, pt2, pt1)) {
        std::cout << "fail6\n";
        return false;
      }
      pt1.set(HORIZONTAL, -11);
      if(!less_slope(x, y, pt1, pt2)) {
        std::cout << "fail7\n";
        return false;
      }
      if(less_slope(x, y, pt2, pt1)) {
        std::cout << "fail8\n";
        return false;
      }
      return true;
    }

  };

}
#endif

