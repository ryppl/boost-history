/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
#ifndef GTL_POLYGON_45_SET_DATA_HPP
#define GTL_POLYGON_45_SET_DATA_HPP
namespace gtl {

  enum RoundingOption { CLOSEST = 0, OVERSIZE = 1, UNDERSIZE = 2, SQRT2 = 3 };
  enum CornerOption { INTERSECTION = 0, ORTHOGINAL = 1 };

  template <typename ltype, typename rtype, int op_type>
  class polygon_45_set_view;
  
  struct polygon_45_set_concept {};

  template <typename Unit>
  class polygon_45_set_data {
  public:
    typedef typename polygon_45_formation<Unit>::Vertex45Compact Vertex45Compact;
    typedef std::vector<Vertex45Compact> Polygon45VertexData;

    typedef Unit coordinate_type;
    typedef Polygon45VertexData value_type;
    typedef typename value_type::const_iterator iterator_type;
    typedef polygon_45_set_data operator_arg_type;

    /// default constructor
    inline polygon_45_set_data() : dirty_(false), unsorted_(false), is_manhattan_(true) {}

    /// constructor from a geometry object
    template <typename geometry_type>
    inline polygon_45_set_data(const geometry_type& that) : dirty_(false), unsorted_(false), is_manhattan_(true) {
      insert(that);
    }

    /// copy constructor
    inline polygon_45_set_data(const polygon_45_set_data& that) : 
      data_(that.data_), dirty_(that.dirty_), unsorted_(that.unsorted_), is_manhattan_(that.is_manhattan_) {}

    /// destructor
    inline ~polygon_45_set_data() {}

    /// assignement operator
    inline polygon_45_set_data& operator=(const polygon_45_set_data& that) {
      if(this == &that) return *this;
      data_ = that.data_;
      dirty_ = that.dirty_;
      unsorted_ = that.unsorted_;
      is_manhattan_ = that.is_manhattan_;
      return *this;
    }

    template <typename ltype, typename rtype, int op_type>
    inline polygon_45_set_data& operator=(const polygon_45_set_view<ltype, rtype, op_type>& that) {
      set(that.begin(), that.end());
      dirty_ = that.dirty();
      unsorted_ = !that.sorted();
      return *this;
    }

    template <typename geometry_object>
    inline polygon_45_set_data& operator=(const geometry_object& geometry) {
      data_.clear();
      insert(geometry);
      return *this;
    }

    /// insert iterator range
    template <typename iT>
    inline void insert(iT input_begin, iT input_end, bool is_hole = false) {
      if(input_begin == input_end) return;
      dirty_ = true;
      unsorted_ = true;
      while(input_begin != input_end) {
        insert(*input_begin, is_hole);
        ++input_begin;
      }
    }

    inline void insert(const polygon_45_set_data& polygon_set, bool is_hole = false);

    template <typename geometry_type>
    inline void insert(const geometry_type& geometry_object, bool is_hole = false) {
      insert_dispatch(geometry_object, is_hole, typename geometry_concept<geometry_type>::type());
    }

    inline void insert_clean(const Vertex45Compact& vertex_45, bool is_hole = false) {
      if(vertex_45.count.is_45()) is_manhattan_ = false;
      data_.push_back(vertex_45);
      if(is_hole) data_.back().count.invert();
    }

    inline void insert(const Vertex45Compact& vertex_45, bool is_hole = false) {
      dirty_ = true;
      unsorted_ = true;
      insert_clean(vertex_45, is_hole);
    }
    
    template <typename coordinate_type_2>
    inline void insert(const polygon_90_set_data<coordinate_type_2>& polygon_set, bool is_hole = false) {
      if(polygon_set.orient() == VERTICAL) {
        for(typename polygon_90_set_data<coordinate_type_2>::iterator_type itr = polygon_set.begin();
            itr != polygon_set.end(); ++itr) {
          Vertex45Compact vertex_45(point_data<Unit>((*itr).first, (*itr).second.first), 2, (*itr).second.second);
          vertex_45.count[1] = (*itr).second.second;
          insert_clean(vertex_45);
        }
      } else {
        for(typename polygon_90_set_data<coordinate_type_2>::iterator_type itr = polygon_set.begin();
            itr != polygon_set.end(); ++itr) {
          Vertex45Compact vertex_45(point_data<Unit>((*itr).second.first, (*itr).first), 2, (*itr).second.second);
          vertex_45.count[1] = (*itr).second.second;
          insert_clean(vertex_45);
        }
      }
    }

    template <typename output_container>
    inline void get(output_container& output) const {
      get_dispatch(output, typename geometry_concept<typename output_container::value_type>::type());
    }

    /// equivalence operator 
    inline bool operator==(const polygon_45_set_data& p) const {
      clean();
      p.clean();
      return data_ == p.data_;
    }

    /// inequivalence operator 
    inline bool operator!=(const polygon_45_set_data& p) const {
      return !((*this) == p);
    }

    /// get iterator to begin vertex data
    inline iterator_type begin() const {
      return data_.begin();
    }

    /// get iterator to end vertex data
    inline iterator_type end() const {
      return data_.end();
    }

    const value_type& value() const {
      return data_;
    }

    /// clear the contents of the polygon_45_set_data
    inline void clear() { data_.clear(); dirty_ = unsorted_ = false; is_manhattan_ = true; }

    /// find out if Polygon set is empty
    inline bool empty() const { return data_.empty(); }

    /// find out if Polygon set is sorted
    inline bool sorted() const { return !unsorted_; }

    /// find out if Polygon set is clean
    inline bool dirty() const { return dirty_; }

    /// find out if Polygon set is clean
    inline bool is_manhattan() const { return is_manhattan_; }

    bool clean() const;

    void sort() const{
      if(unsorted_) {
        std::sort(data_.begin(), data_.end());
        unsorted_ = false;
      }
    }

    template <typename input_iterator_type>
    void set(input_iterator_type input_begin, input_iterator_type input_end) {
      data_.clear();
      insert(input_begin, input_end);
      dirty_ = true;
      unsorted_ = true;
    }

    void set_clean(const value_type& value) {
      data_ = value;
      dirty_ = false;
      unsorted_ = false;
    }

    void set(const value_type& value) {
      data_ = value; 
      dirty_ = true;
      unsorted_ = true;
    }

    /// append to the container cT with polygons (holes will be fractured vertically)
    template <class cT>
    void get_polygons(cT& container) const {
      get_dispatch(container, polygon_45_concept());
    }
    
    /// append to the container cT with PolygonWithHoles objects
    template <class cT>
    void get_polygons_with_holes(cT& container) const {
      get_dispatch(container, polygon_45_with_holes_concept());
    }
    
    /// append to the container cT with polygons of three or four verticies
    template <class cT>
    void get_trapezoids(cT& container) const;

    /// insert vertex sequence
    template <class iT>
    void insert_vertex_sequence(iT begin_vertex, iT end_vertex,
                                direction_1d winding, bool is_hole = false);

    /// get the external boundary rectangle
    template <typename rectangle_type>
    bool extents(rectangle_type& rect) const;

    /// snap verticies of set to even,even or odd,odd coordinates
    void snap() const;
    
    /// |= &= += *= -= ^= binary operators
    polygon_45_set_data& operator|=(const polygon_45_set_data& b);
    polygon_45_set_data& operator&=(const polygon_45_set_data& b);
    polygon_45_set_data& operator+=(const polygon_45_set_data& b);
    polygon_45_set_data& operator*=(const polygon_45_set_data& b);
    polygon_45_set_data& operator-=(const polygon_45_set_data& b);
    polygon_45_set_data& operator^=(const polygon_45_set_data& b);

    /// resizing operations
    polygon_45_set_data& operator+=(Unit delta);
    polygon_45_set_data& operator-=(Unit delta);
    
    /// shrink the Polygon45Set by shrinking
    polygon_45_set_data& resize(coordinate_type resizing, RoundingOption rounding = CLOSEST,
                                CornerOption corner = INTERSECTION);

    /// transform set
    template <typename transformation_type>
    polygon_45_set_data& transform(const transformation_type& tr);

    /// scale set
    polygon_45_set_data& scale_up(typename coordinate_traits<Unit>::unsigned_area_type factor);
    polygon_45_set_data& scale_down(typename coordinate_traits<Unit>::unsigned_area_type factor);

    /// accumulate the bloated polygon
    template <typename geometry_type>
    polygon_45_set_data& insert_with_resize(const geometry_type& poly,
                                            coordinate_type resizing, RoundingOption rounding = CLOSEST,
                                            CornerOption corner = INTERSECTION,
                                            bool hole = false) {
      return insert_with_resize_dispatch(poly, resizing, rounding, corner, hole, typename geometry_concept<geometry_type>::type());
    }
    
  private:
    mutable value_type data_;
    mutable bool dirty_;
    mutable bool unsorted_;
    mutable bool is_manhattan_;
  
  private:
    //functions
    template <typename output_container>
    void get_dispatch(output_container& output, polygon_45_concept tag) const {
      get_fracture(output, true, tag);
    }
    template <typename output_container>
    void get_dispatch(output_container& output, polygon_45_with_holes_concept tag) const {
      get_fracture(output, false, tag);
    }
    template <typename output_container>
    void get_dispatch(output_container& output, polygon_concept tag) const {
      get_fracture(output, true, tag);
    }
    template <typename output_container>
    void get_dispatch(output_container& output, polygon_with_holes_concept tag) const {
      get_fracture(output, false, tag);
    }
    template <typename output_container, typename concept_type>
    void get_fracture(output_container& container, bool fracture_holes, concept_type tag) const {
      clean();
      typename polygon_45_formation<Unit>::Polygon45Formation pf(fracture_holes);
      //std::cout << "FORMING POLYGONS\n";
      pf.scan(container, data_.begin(), data_.end());
    }

    template <typename geometry_type>
    void insert_dispatch(const geometry_type& geometry_object, bool is_hole, undefined_concept tag) {
      insert(geometry_object.begin(), geometry_object.end(), is_hole);
    }
    template <typename geometry_type>
    void insert_dispatch(const geometry_type& geometry_object, bool is_hole, rectangle_concept tag) {
      polygon_45_data<Unit> poly;
      assign(poly, geometry_object);
      insert_dispatch(poly, is_hole, polygon_45_concept());
    }
    template <typename geometry_type>
    void insert_dispatch(const geometry_type& geometry_object, bool is_hole, polygon_90_concept tag) {
      insert_vertex_sequence(begin_points(geometry_object), end_points(geometry_object), winding(geometry_object), is_hole);
    }
    template <typename geometry_type>
    void insert_dispatch(const geometry_type& geometry_object, bool is_hole, polygon_90_with_holes_concept tag) {
      insert_vertex_sequence(begin_points(geometry_object), end_points(geometry_object), winding(geometry_object), is_hole);
      for(typename polygon_with_holes_traits<geometry_type>::iterator_holes_type itr =
            begin_holes(geometry_object); itr != end_holes(geometry_object);
          ++itr) {
        insert_vertex_sequence(begin_points(*itr), end_points(*itr), winding(*itr), !is_hole);
      }
    }
    template <typename geometry_type>
    void insert_dispatch(const geometry_type& geometry_object, bool is_hole, polygon_45_concept tag) {
      insert_vertex_sequence(begin_points(geometry_object), end_points(geometry_object), winding(geometry_object), is_hole);
    }
    template <typename geometry_type>
    void insert_dispatch(const geometry_type& geometry_object, bool is_hole, polygon_45_with_holes_concept tag) {
      insert_vertex_sequence(begin_points(geometry_object), end_points(geometry_object), winding(geometry_object), is_hole);
      for(typename polygon_with_holes_traits<geometry_type>::iterator_holes_type itr =
            begin_holes(geometry_object); itr != end_holes(geometry_object);
          ++itr) {
        insert_vertex_sequence(begin_points(*itr), end_points(*itr), winding(*itr), !is_hole);
      }
    }
    template <typename geometry_type>
    void insert_dispatch(const geometry_type& geometry_object, bool is_hole, polygon_45_set_concept tag) {
      polygon_45_set_data ps;
      assign(ps, geometry_object);
      insert(ps, is_hole);
    }
    template <typename geometry_type>
    void insert_dispatch(const geometry_type& geometry_object, bool is_hole, polygon_90_set_concept tag) {
      std::list<polygon_90_data<coordinate_type> > pl;
      assign(pl, geometry_object);
      insert(pl.begin(), pl.end(), is_hole);
    }

    void insert_vertex_half_edge_45_pair(const point_data<Unit>& pt1, point_data<Unit>& pt2, 
                                         const point_data<Unit>& pt3, direction_1d wdir);

    template <typename geometry_type>
    polygon_45_set_data& insert_with_resize_dispatch(const geometry_type& poly,
                                                     coordinate_type resizing, RoundingOption rounding,
                                                     CornerOption corner, bool hole, polygon_45_concept tag);
    
    /// accumulate the bloated polygon with holes
    template <typename geometry_type>
    polygon_45_set_data& insert_with_resize_dispatch(const geometry_type& poly,
                                                     coordinate_type resizing, RoundingOption rounding,
                                                     CornerOption corner, bool hole, polygon_45_with_holes_concept tag); 
    
    static void snap_vertex_45(Vertex45Compact& vertex);

  public:
    void applyAdaptiveBoolean_(int op, const polygon_45_set_data& rvalue) const;
    void applyAdaptiveBoolean_(polygon_45_set_data& result, int op, const polygon_45_set_data& rvalue) const;
    //void applyBoolean_(int op, value_type& rvalue) const;
    //void applyBoolean_(value_type& result, int op, 
    //                   value_type& rvalue) const;
    //void applyBooleanException_(value_type& result, int op, value_type& rvalue) const;
  };

  template <typename T>
  struct geometry_concept<polygon_45_set_data<T> > {
    typedef polygon_45_set_concept type;
  };
 

  template <typename Unit>
  template <class cT>
  inline void polygon_45_set_data<Unit>::get_trapezoids(cT& container) const {
    clean();
    typename polygon_45_formation<Unit>::Polygon45Tiling pf;
    //std::cout << "FORMING POLYGONS\n";
    pf.scan(container, data_.begin(), data_.end());
    //std::cout << "DONE FORMING POLYGONS\n";
  }

  template <typename Unit>
  inline std::pair<int, int> characterizeEdge45(const point_data<Unit>& pt1, const point_data<Unit>& pt2) {
    std::pair<int, int> retval(0, 1);
    if(pt1.x() == pt2.x()) {
      retval.first = 3;
      retval.second = -1;
      return retval;
    }
    //retval.second = pt1.x() < pt2.x() ? -1 : 1;
    retval.second = 1;
    if(pt1.y() == pt2.y()) {
      retval.first = 1;
    } else if(pt1.x() < pt2.x()) {
      if(pt1.y() < pt2.y()) {
        retval.first = 2;
      } else {
        retval.first = 0;
      }
    } else {
      if(pt1.y() < pt2.y()) {
        retval.first = 0;
      } else {
        retval.first = 2;
      }
    }
    return retval;
  }

  template <typename Unit>
  inline void polygon_45_set_data<Unit>::insert_vertex_half_edge_45_pair(const point_data<Unit>& pt1, point_data<Unit>& pt2, 
                                                                         const point_data<Unit>& pt3, 
                                                                         direction_1d wdir) {
    int multiplier = wdir == LOW ? -1 : 1;
    Vertex45Compact vertex(pt2, 0, 0);
    //std::cout << pt1 << " " << pt2 << " " << pt3 << std::endl;
    std::pair<int, int> check;
    check = characterizeEdge45(pt1, pt2); 
    //std::cout << "index " << check.first << " " << check.second * -multiplier << std::endl;
    vertex.count[check.first] += check.second * -multiplier;
    check = characterizeEdge45(pt2, pt3); 
    //std::cout << "index " << check.first << " " << check.second * multiplier << std::endl;
    vertex.count[check.first] += check.second * multiplier;
    data_.push_back(vertex);
    if(vertex.count.is_45()) is_manhattan_ = false;
  }

  template <typename Unit>
  template <class iT>
  inline void polygon_45_set_data<Unit>::insert_vertex_sequence(iT begin_vertex, iT end_vertex,
                                                                direction_1d winding, bool is_hole) {
    if(begin_vertex == end_vertex) return;
    if(is_hole) winding = winding.backward();
    iT itr = begin_vertex;
    if(itr == end_vertex) return;
    point_data<Unit> firstPt = *itr;
    ++itr;
    point_data<Unit> secondPt(firstPt);
    //skip any duplicate points
    do {
      if(itr == end_vertex) return;
      secondPt = *itr;
      ++itr;
    } while(secondPt == firstPt);
    point_data<Unit> prevPt = secondPt;
    point_data<Unit> prevPrevPt = firstPt;
    while(itr != end_vertex) {
      point_data<Unit> pt = *itr;
      //skip any duplicate points
      if(pt == prevPt) {
        ++itr;
        continue;
      }
      //operate on the three points
      insert_vertex_half_edge_45_pair(prevPrevPt, prevPt, pt, winding);
      prevPrevPt = prevPt;
      prevPt = pt;
      ++itr;
    }
    if(prevPt != firstPt) {
      insert_vertex_half_edge_45_pair(prevPrevPt, prevPt, firstPt, winding);
      insert_vertex_half_edge_45_pair(prevPt, firstPt, secondPt, winding);
    } else {
      insert_vertex_half_edge_45_pair(prevPrevPt, firstPt, secondPt, winding);
    }
    dirty_ = true;
    unsorted_ = true;
  }

  /// insert polygon set
  template <typename Unit>
  inline void polygon_45_set_data<Unit>::insert(const polygon_45_set_data<Unit>& polygon_set, bool is_hole) {
    if(is_hole) {
      (*this) -= polygon_set;
      return;
    }
    if(empty()) {
      (*this) = polygon_set;
      return;
    }
    data_.insert(data_.end(), polygon_set.data_.begin(), polygon_set.data_.end());
    dirty_ = true;
    unsorted_ = true;
    if(polygon_set.is_manhattan_ == false) is_manhattan_ = false;
    return;
  }

  /// get the external boundary rectangle
  template <typename Unit>
  template <typename rectangle_type>
  inline bool polygon_45_set_data<Unit>::extents(rectangle_type& rect) const{
    if(empty()) {
      return false;
    }
    Unit low = std::numeric_limits<Unit>::max();
    Unit high = std::numeric_limits<Unit>::min();
    interval_data<Unit> xivl(low, high); 
    interval_data<Unit> yivl(low, high);
    for(typename value_type::const_iterator itr = data_.begin();
        itr != data_.end(); ++ itr) {
      if((*itr).pt.x() > xivl.get(HIGH))
        xivl.set(HIGH, (*itr).pt.x());
      if((*itr).pt.x() < xivl.get(LOW))
        xivl.set(LOW, (*itr).pt.x());
      if((*itr).pt.y() > yivl.get(HIGH))
        yivl.set(HIGH, (*itr).pt.y());
      if((*itr).pt.y() < yivl.get(LOW))
        yivl.set(LOW, (*itr).pt.y());
    }
    rect = construct<rectangle_type>(xivl, yivl);
    return true;
  }

  //this function snaps the vertex and two half edges
  //to be both even or both odd coordinate values if one of the edges is 45
  //and throws an excpetion if an edge is non-manhattan, non-45.
  template <typename Unit>
  inline void polygon_45_set_data<Unit>::snap_vertex_45(typename polygon_45_set_data<Unit>::Vertex45Compact& vertex) {
    bool plus45 = vertex.count[2] != 0;
    bool minus45 = vertex.count[0] != 0;
    if(plus45 || minus45) {
      if(abs(vertex.pt.x()) % 2 != abs(vertex.pt.y()) % 2) {
        if(vertex.count[1] != 0 ||
           (plus45 && minus45)) {
          //move right
          vertex.pt.x(vertex.pt.x() + 1);
        } else {
          //assert that vertex.count[3] != 0
          Unit modifier = plus45 ? -1 : 1;
          vertex.pt.y(vertex.pt.y() + modifier);
        }
      }
    }
  }

  template <typename Unit>
  inline void polygon_45_set_data<Unit>::snap() const {
    for(typename value_type::iterator itr = data_.begin();
        itr != data_.end(); ++itr) {
      snap_vertex_45(*itr);
    }
  }

  /// |= &= += *= -= ^= binary operators
  template <typename Unit>
  inline polygon_45_set_data<Unit>& polygon_45_set_data<Unit>::operator|=(const polygon_45_set_data<Unit>& b) {
    insert(b);
    return *this;
  }
  template <typename Unit>
  inline polygon_45_set_data<Unit>& polygon_45_set_data<Unit>::operator&=(const polygon_45_set_data<Unit>& b) {
    //b.sort();
    //sort();
    applyAdaptiveBoolean_(1, b);
    dirty_ = false;
    unsorted_ = false;
    return *this;
  }
  template <typename Unit>
  inline polygon_45_set_data<Unit>& polygon_45_set_data<Unit>::operator+=(const polygon_45_set_data<Unit>& b) {
    return (*this) |= b;
  }
  template <typename Unit>
  inline polygon_45_set_data<Unit>& polygon_45_set_data<Unit>::operator*=(const polygon_45_set_data<Unit>& b) {
    return (*this) &= b;
  }
  template <typename Unit>
  inline polygon_45_set_data<Unit>& polygon_45_set_data<Unit>::operator-=(const polygon_45_set_data<Unit>& b) {
    //b.sort();
    //sort();
    applyAdaptiveBoolean_(2, b);   
    dirty_ = false;
    unsorted_ = false;
    return *this;
  }
  template <typename Unit>
  inline polygon_45_set_data<Unit>& polygon_45_set_data<Unit>::operator^=(const polygon_45_set_data<Unit>& b) {
    //b.sort();
    //sort();
    applyAdaptiveBoolean_(3, b);   
    dirty_ = false;
    unsorted_ = false;
    return *this;
  }

  template <typename Unit>
  inline polygon_45_set_data<Unit>& polygon_45_set_data<Unit>::operator+=(Unit delta) {
    return resize(delta);
  }
  template <typename Unit>
  inline polygon_45_set_data<Unit>& polygon_45_set_data<Unit>::operator-=(Unit delta) {
    return (*this) += -delta;
  }

  template <typename Unit>
  inline polygon_45_set_data<Unit>& 
  polygon_45_set_data<Unit>::resize(Unit resizing, RoundingOption rounding, CornerOption corner) {
    if(resizing == 0) return *this;
    std::list<polygon_45_with_holes_data<Unit> > pl;
    get_polygons_with_holes(pl);
    clear();
    for(typename std::list<polygon_45_with_holes_data<Unit> >::iterator itr = pl.begin(); itr != pl.end(); ++itr) {
      insert_with_resize(*itr, resizing, rounding, corner);
    }
    //perterb 45 edges to prevent non-integer intersection errors upon boolean op
    //snap();
    return *this;
  }

  //distance is assumed to be positive
  inline int roundClosest(double distance) {
    int f = (int)distance;
    if(distance - (double)f < 0.5) return f;
    return f+1;
  }

  //distance is assumed to be positive
  template <typename Unit>
  inline Unit roundWithOptions(double distance, RoundingOption rounding) {
    if(rounding == CLOSEST) {
      return roundClosest(distance);
    } else if(rounding == OVERSIZE) {
      return (Unit)distance + 1;
    } else { //UNDERSIZE
      return (Unit)distance;
    }
  }

  // 0 is east, 1 is northeast, 2 is north, 3 is northwest, 4 is west, 5 is southwest, 6 is south
  // 7 is southwest
  template <typename Unit>
  inline point_data<Unit> bloatVertexInDirWithOptions(const point_data<Unit>& point, unsigned int dir,
                                                      Unit bloating, RoundingOption rounding) {
    const double sqrt2 = 1.4142135623730950488016887242097;
    if(dir & 1) {
      Unit unitDistance = (Unit)bloating;
      if(rounding != SQRT2) {
        //45 degree bloating
        double distance = (double)bloating;
        distance /= sqrt2;  // multiply by 1/sqrt2
        unitDistance = roundWithOptions<Unit>(distance, rounding);
      }
      int xMultiplier = 1;
      int yMultiplier = 1;
      if(dir == 3 || dir == 5) xMultiplier = -1;
      if(dir == 5 || dir == 7) yMultiplier = -1;
      return point_data<Unit>(point.x()+xMultiplier*unitDistance,
                   point.y()+yMultiplier*unitDistance);
    } else {
      if(dir == 0)
        return point_data<Unit>(point.x()+bloating, point.y());
      if(dir == 2)
        return point_data<Unit>(point.x(), point.y()+bloating);
      if(dir == 4)
        return point_data<Unit>(point.x()-bloating, point.y());
      if(dir == 6)
        return point_data<Unit>(point.x(), point.y()-bloating);
      return point_data<Unit>();
    }
  }

  template <typename Unit>
  inline unsigned int getEdge45Direction(const point_data<Unit>& pt1, const point_data<Unit>& pt2) {
    if(pt1.x() == pt2.x()) {
      if(pt1.y() < pt2.y()) return 2;
      return 6;
    }
    if(pt1.y() == pt2.y()) {
      if(pt1.x() < pt2.x()) return 0;
      return 4;
    }
    if(pt2.y() > pt1.y()) {
      if(pt2.x() > pt1.x()) return 1;
      return 3;
    }
    if(pt2.x() > pt1.x()) return 7;
    return 5;
  }

  inline unsigned int getEdge45NormalDirection(unsigned int dir, int multiplier) {
    if(multiplier < 0)
      return (dir + 2) % 8;
    return (dir + 4 + 2) % 8;
  }

  template <typename Unit>
  inline point_data<Unit> getIntersectionPoint(const point_data<Unit>& pt1, unsigned int slope1,
                                               const point_data<Unit>& pt2, unsigned int slope2) {
    //the intention here is to use all integer arithmetic without causing overflow
    //turncation error or divide by zero error
    //I don't use floating point arithmetic because its precision may not be high enough
    //at the extremes of the integer range
    typedef typename coordinate_traits<Unit>::area_type LongUnit;
    const Unit rises[8] = {0, 1, 1, 1, 0, -1, -1, -1};
    const Unit runs[8] = {1, 1, 0, -1, -1, -1, 0, 1};
    LongUnit rise1 = rises[slope1];
    LongUnit rise2 = rises[slope2];
    LongUnit run1 = runs[slope1];
    LongUnit run2 = runs[slope2];
    LongUnit x1 = (LongUnit)pt1.x();
    LongUnit x2 = (LongUnit)pt2.x();
    LongUnit y1 = (LongUnit)pt1.y();
    LongUnit y2 = (LongUnit)pt2.y();
    Unit x = 0;
    Unit y = 0;
    if(run1 == 0) {
      x = pt1.x();
      y = (Unit)(((x1 - x2) * rise2) / run2) + pt2.y(); 
    } else if(run2 == 0) {
      x = pt2.x();
      y = (Unit)(((x2 - x1) * rise1) / run1) + pt1.y();
    } else {
      // y - y1 = (rise1/run1)(x - x1)
      // y - y2 = (rise2/run2)(x - x2)
      // y = (rise1/run1)(x - x1) + y1 = (rise2/run2)(x - x2) + y2
      // (rise1/run1 - rise2/run2)x = y2 - y1 + rise1/run1 x1 - rise2/run2 x2
      // x = (y2 - y1 + rise1/run1 x1 - rise2/run2 x2)/(rise1/run1 - rise2/run2)
      // x = (y2 - y1 + rise1/run1 x1 - rise2/run2 x2)(rise1 run2 - rise2 run1)/(run1 run2)
      x = (Unit)((y2 - y1 + ((rise1 * x1) / run1) - ((rise2 * x2) / run2)) * 
                 (run1 * run2) / (rise1 * run2 - rise2 * run1));
      if(rise1 == 0) {
        y = pt1.y();
      } else if(rise2 == 0) {
        y = pt2.y();
      } else {
        // y - y1 = (rise1/run1)(x - x1)
        // (run1/rise1)(y - y1) = x - x1
        // x = (run1/rise1)(y - y1) + x1 = (run2/rise2)(y - y2) + x2
        y = (Unit)((x2 - x1 + ((run1 * y1) / rise1) - ((run2 * y2) / rise2)) * 
                   (rise1 * rise2) / (run1 * rise2 - run2 * rise1));
      }
    }
    return point_data<Unit>(x, y);
  }

  template <typename Unit>
  inline
  void handleResizingEdge45(polygon_45_set_data<Unit>& sizingSet, point_data<Unit> first, 
                            point_data<Unit> second, Unit resizing, RoundingOption rounding) {
    if(first.x() == second.x()) {
      sizingSet += rectangle_data<int>(first.x() - resizing, first.y(), first.x() + resizing, second.y());
      return;
    }
    if(first.y() == second.y()) {
      sizingSet += rectangle_data<int>(first.x(), first.y() - resizing, second.x(), first.y() + resizing);
      return;
    }
    //edge is 45
    std::vector<point_data<Unit> > pts;
    Unit bloating = resizing < 0 ? -resizing : resizing;
    if(second.x() < first.x()) std::swap(first, second);
    if(first.y() < second.y()) {
      pts.push_back(bloatVertexInDirWithOptions(first, 3, bloating, rounding));
      pts.push_back(bloatVertexInDirWithOptions(first, 7, bloating, rounding));
      pts.push_back(bloatVertexInDirWithOptions(second, 7, bloating, rounding));
      pts.push_back(bloatVertexInDirWithOptions(second, 3, bloating, rounding));
      sizingSet.insert_vertex_sequence(pts.begin(), pts.end(), HIGH, false);
    } else {
      pts.push_back(bloatVertexInDirWithOptions(first, 1, bloating, rounding));
      pts.push_back(bloatVertexInDirWithOptions(first, 5, bloating, rounding));
      pts.push_back(bloatVertexInDirWithOptions(second, 5, bloating, rounding));
      pts.push_back(bloatVertexInDirWithOptions(second, 1, bloating, rounding));
      sizingSet.insert_vertex_sequence(pts.begin(), pts.end(), HIGH, false);
    }
  }

  template <typename Unit>
  inline
  void handleResizingVertex45(polygon_45_set_data<Unit>& sizingSet, const point_data<Unit>& first, 
                              const point_data<Unit>& second, const point_data<Unit>& third, Unit resizing, 
                              RoundingOption rounding, CornerOption corner, 
                              int multiplier) {
    unsigned int edge1 = getEdge45Direction(first, second);
    unsigned int edge2 = getEdge45Direction(second, third);
    unsigned int diffAngle;
    if(multiplier < 0) 
      diffAngle = (edge2 + 8 - edge1) % 8;
    else
      diffAngle = (edge1 + 8 - edge2) % 8;
    if(diffAngle < 4) {
      if(resizing > 0) return; //accute interior corner
      else multiplier *= -1; //make it appear to be an accute exterior angle
    }
    unsigned int normal1 = getEdge45NormalDirection(edge1, multiplier);
    unsigned int normal2 = getEdge45NormalDirection(edge2, multiplier);
    Unit bloating = abs(resizing);
    point_data<Unit> edgePoint1 = bloatVertexInDirWithOptions(second, normal1, bloating, rounding);
    point_data<Unit> edgePoint2 = bloatVertexInDirWithOptions(second, normal2, bloating, rounding);
    //if the change in angle is 135 degrees it is an accute exterior corner
    if((edge1+ multiplier * 3) % 8 == edge2) {
      if(corner == ORTHOGINAL) {
        rectangle_data<int> insertion_rect;
        set_points(insertion_rect, edgePoint1, edgePoint2);
        sizingSet.insert(insertion_rect);
        return;
      }
    } 
    std::vector<point_data<Unit> > pts;
    pts.push_back(edgePoint1);
    pts.push_back(second);
    pts.push_back(edgePoint2);
    pts.push_back(getIntersectionPoint(edgePoint1, edge1, edgePoint2, edge2));
    polygon_45_data<Unit> poly(pts.begin(), pts.end());
    sizingSet += poly;
  }

  template <typename Unit>
  template <typename geometry_type>
  inline polygon_45_set_data<Unit>& 
  polygon_45_set_data<Unit>::insert_with_resize_dispatch(const geometry_type& poly,
                                                         coordinate_type resizing, 
                                                         RoundingOption rounding,
                                                         CornerOption corner,
                                                         bool hole, polygon_45_concept tag) {
    direction_1d wdir = winding(poly);
    int multiplier = wdir == LOW ? -1 : 1;
    if(hole) multiplier *= -1;
    //if(resizing < 0) {
    //multiplier *= -1;
    //resizing *= -1;
    //}
    typedef typename polygon_45_data<Unit>::iterator_type piterator;
    piterator first, second, third, end, real_end;
    real_end = end_points(poly);
    third = begin_points(poly);
    first = third;
    if(first == real_end) return *this;
    ++third;
    if(third == real_end) return *this;
    second = end = third;
    ++third;
    if(third == real_end) return *this;
    polygon_45_set_data<Unit> sizingSet;
    //insert minkofski shapes on edges and corners
    do {
      handleResizingEdge45(sizingSet, *first, *second, resizing, rounding);
      handleResizingVertex45(sizingSet, *first, *second, *third, resizing, rounding, corner, multiplier);
      first = second;
      second = third;
      ++third;
      if(third == real_end) {
        third = begin_points(poly);
        if(*second == *third) {
          ++third; //skip first point if it is duplicate of last point
        }
      }
    } while(second != end);
    //sizingSet.snap();
    polygon_45_set_data<Unit> tmp;
    //insert original shape
    tmp.insert(poly, hole);
    if(hole) {
      std::vector<point_data<Unit> > pts;
      pts.reserve(4);
      Unit UnitMax = std::numeric_limits<Unit>::max();
      Unit UnitMin = std::numeric_limits<Unit>::min();
      pts.push_back(point_data<Unit> (UnitMin, UnitMin));
      pts.push_back(point_data<Unit> (UnitMin, UnitMax));
      pts.push_back(point_data<Unit> (UnitMax, UnitMax));
      pts.push_back(point_data<Unit> (UnitMax, UnitMin));
      tmp.insert_vertex_sequence(pts.begin(), pts.end(), LOW, false);
      //tmp.snap();
      if(resizing < 0) tmp -= sizingSet;
      else tmp += sizingSet;
      tmp.clean();
      tmp.insert_vertex_sequence(pts.begin(), pts.end(), LOW, true);
      //tmp.snap();
    } else {
      if(resizing < 0) tmp -= sizingSet;
      else tmp += sizingSet;
      tmp.clean();
    }
    return (*this) += tmp;
  }

  /// accumulate the bloated polygon with holes
  template <typename Unit>
  template <typename geometry_type>
  inline polygon_45_set_data<Unit>&
  polygon_45_set_data<Unit>::insert_with_resize_dispatch(const geometry_type& poly,
                                                         coordinate_type resizing, 
                                                         RoundingOption rounding,
                                                         CornerOption corner, 
                                                         bool hole, polygon_45_with_holes_concept tag) {
    insert_with_resize_dispatch(poly, resizing, rounding, corner, hole, polygon_45_concept());
    for(typename polygon_with_holes_traits<geometry_type>::iterator_holes_type itr =
          begin_holes(poly); itr != end_holes(poly);
        ++itr) {
      insert_with_resize_dispatch(*itr, resizing, rounding, corner, !hole, polygon_45_concept());
    }
    return *this;
  }

  /// transform set
  template <typename Unit>
  template <typename transformation_type>
  inline polygon_45_set_data<Unit>& polygon_45_set_data<Unit>::transform(const transformation_type& tr){
    clean();
    std::vector<polygon_45_with_holes_data<Unit> > polys;
    get(polys);
    for(typename std::vector<polygon_45_with_holes_data<Unit> >::iterator itr = polys.begin();
        itr != polys.end(); ++itr) {
      gtl::transform(*itr, tr);
    }
    clear();
    insert(polys.begin(), polys.end());
    dirty_ = true; 
    unsorted_ = true;
    return *this;
  }
    
  template <typename Unit>
  inline polygon_45_set_data<Unit>& polygon_45_set_data<Unit>::scale_up(typename coordinate_traits<Unit>::unsigned_area_type factor) {
    clean();
    std::vector<polygon_45_with_holes_data<Unit> > polys;
    get_polygons_with_holes(polys);
    for(typename std::vector<polygon_45_with_holes_data<Unit> >::iterator itr = polys.begin();
        itr != polys.end(); ++itr) {
      gtl::scale_up(*itr, factor);
    }
    clear();
    insert(polys.begin(), polys.end());
    dirty_ = true; 
    unsorted_ = true;
    return *this;
  }

  template <typename Unit>
  inline polygon_45_set_data<Unit>& polygon_45_set_data<Unit>::scale_down(typename coordinate_traits<Unit>::unsigned_area_type factor) {
    clean();
    std::vector<polygon_45_with_holes_data<Unit> > polys;
    get_polygons_with_holes(polys);
    for(typename std::vector<polygon_45_with_holes_data<Unit> >::iterator itr = polys.begin();
        itr != polys.end(); ++itr) {
      gtl::scale_down(*itr, factor);
    }
    clear();
    insert(polys.begin(), polys.end());
    dirty_ = true; 
    unsorted_ = true;
    return *this;
  }

  template <typename Unit>
  inline bool polygon_45_set_data<Unit>::clean() const {
    if(unsorted_) sort();
    if(dirty_) {
      polygon_45_set_data<Unit> empty;
      applyAdaptiveBoolean_(0, empty);
      dirty_ = false;
    }
    return true;
  }

  template <typename Unit>
  inline void polygon_45_set_data<Unit>::applyAdaptiveBoolean_(int op, const polygon_45_set_data<Unit>& rvalue) const {
    polygon_45_set_data<Unit> tmp;
    applyAdaptiveBoolean_(tmp, op, rvalue);
    data_.swap(tmp.data_); //swapping vectors should be constant time operation
    is_manhattan_ = tmp.is_manhattan_;
    unsorted_ = false;
    dirty_ = false;
  }

  template <typename Unit>
  inline void polygon_45_set_data<Unit>::applyAdaptiveBoolean_(polygon_45_set_data<Unit>& result,
                                                               int op, const polygon_45_set_data<Unit>& rvalue) const {
    if(is_manhattan() && rvalue.is_manhattan()) {
      //convert each into polygon_90_set data and call boolean operations
      polygon_90_set_data<Unit> l90sd(VERTICAL), r90sd(VERTICAL), output(VERTICAL);
      for(typename value_type::const_iterator itr = data_.begin(); itr != data_.end(); ++itr) {
        if((*itr).count[3] == 0) continue; //skip all non vertical edges
        l90sd.insert(std::make_pair((*itr).pt.x(), std::make_pair((*itr).pt.y(), (*itr).count[3])), false, VERTICAL);
      }
          for(typename value_type::const_iterator itr = rvalue.data_.begin(); itr != rvalue.data_.end(); ++itr) {
            if((*itr).count[3] == 0) continue; //skip all non vertical edges
        r90sd.insert(std::make_pair((*itr).pt.x(), std::make_pair((*itr).pt.y(), (*itr).count[3])), false, VERTICAL);
      }
      l90sd.sort();
      r90sd.sort();
      if(op == 0) {
        output.applyBooleanBinaryOp(l90sd.begin(), l90sd.end(),
                                    r90sd.begin(), r90sd.end(), boolean_op::BinaryCount<boolean_op::BinaryOr>()); 
      } else if (op == 1) {
        output.applyBooleanBinaryOp(l90sd.begin(), l90sd.end(),
                                    r90sd.begin(), r90sd.end(), boolean_op::BinaryCount<boolean_op::BinaryAnd>()); 
      } else if (op == 2) {
        output.applyBooleanBinaryOp(l90sd.begin(), l90sd.end(),
                                    r90sd.begin(), r90sd.end(), boolean_op::BinaryCount<boolean_op::BinaryNot>()); 
      } else if (op == 3) {
        output.applyBooleanBinaryOp(l90sd.begin(), l90sd.end(),
                                    r90sd.begin(), r90sd.end(), boolean_op::BinaryCount<boolean_op::BinaryXor>()); 
      }
      result.clear();
      result.insert(output);
    } else {
      sort();
      rvalue.sort();
      typename boolean_op_45<Unit>::Scan45 scan45(op);
      std::vector<typename boolean_op_45<Unit>::Vertex45> eventOut;
      std::vector<typename boolean_op_45<Unit>::Scan45Vertex> eventIn;
      typename value_type::const_iterator iter1 = data_.begin();
      typename value_type::const_iterator iter2 = rvalue.data_.begin();
      typename value_type::const_iterator end1 = data_.end();
      typename value_type::const_iterator end2 = rvalue.data_.end();
      const Unit UnitMax = std::numeric_limits<Unit>::max();
      Unit x = UnitMax;
      while(iter1 != end1 || iter2 != end2) {
        Unit currentX = UnitMax;
        if(iter1 != end1) currentX = iter1->pt.x();
        if(iter2 != end2) currentX = std::min(currentX, iter2->pt.x());
        if(currentX != x) {
          //std::cout << "SCAN " << currentX << "\n";
          //scan event
          scan45.scan(eventOut, eventIn.begin(), eventIn.end());
          std::sort(eventOut.begin(), eventOut.end());
          for(unsigned int i = 0; i < eventOut.size(); ++i) {
            if(!result.data_.empty() &&
               result.data_.back().pt == eventOut[i].pt) {
              result.data_.back().count += eventOut[i];
            } else {
              if(!result.data_.empty() &&
                 result.data_.back().count.is_45()) {
                result.is_manhattan_ = false;
              }
              result.data_.push_back(eventOut[i]);
            }
            if(result.data_.back().count == (typename polygon_45_formation<Unit>::Vertex45Count(0, 0, 0, 0))) {
              result.data_.pop_back();
            }
          }
          eventOut.clear();
          eventIn.clear();
          x = currentX;
        }
        //std::cout << "get next\n";
        if(iter2 != end2 && (iter1 == end1 || iter2->pt.x() < iter1->pt.x() || 
                             (iter2->pt.x() == iter1->pt.x() &&
                              iter2->pt.y() < iter1->pt.y()) )) {
          //std::cout << "case1 next\n";
          eventIn.push_back(typename boolean_op_45<Unit>::Scan45Vertex(iter2->pt, 
                                                                       typename polygon_45_formation<Unit>::
                                                                       Scan45Count(typename polygon_45_formation<Unit>::Count2(0, iter2->count[0]),
                                                                                   typename polygon_45_formation<Unit>::Count2(0, iter2->count[1]),
                                                                                   typename polygon_45_formation<Unit>::Count2(0, iter2->count[2]),
                                                                                   typename polygon_45_formation<Unit>::Count2(0, iter2->count[3]))));
          ++iter2;
        } else if(iter1 != end1 && (iter2 == end2 || iter1->pt.x() < iter2->pt.x() || 
                                    (iter1->pt.x() == iter2->pt.x() &&
                                     iter1->pt.y() < iter2->pt.y()) )) {
          //std::cout << "case2 next\n";
          eventIn.push_back(typename boolean_op_45<Unit>::Scan45Vertex(iter1->pt, 
                                                                       typename polygon_45_formation<Unit>::
                                                                       Scan45Count(
                                                                                   typename polygon_45_formation<Unit>::Count2(iter1->count[0], 0),
                                                                                   typename polygon_45_formation<Unit>::Count2(iter1->count[1], 0),
                                                                                   typename polygon_45_formation<Unit>::Count2(iter1->count[2], 0),
                                                                                   typename polygon_45_formation<Unit>::Count2(iter1->count[3], 0))));
          ++iter1;
        } else {
          //std::cout << "case3 next\n";
          eventIn.push_back(typename boolean_op_45<Unit>::Scan45Vertex(iter2->pt, 
                                                                       typename polygon_45_formation<Unit>::
                                                                       Scan45Count(typename polygon_45_formation<Unit>::Count2(iter1->count[0], 
                                                                                                                               iter2->count[0]),
                                                                                   typename polygon_45_formation<Unit>::Count2(iter1->count[1], 
                                                                                                                               iter2->count[1]),
                                                                                   typename polygon_45_formation<Unit>::Count2(iter1->count[2], 
                                                                                                                               iter2->count[2]),
                                                                                   typename polygon_45_formation<Unit>::Count2(iter1->count[3], 
                                                                                                                               iter2->count[3]))));
          ++iter1;
          ++iter2;
        }
      }
      scan45.scan(eventOut, eventIn.begin(), eventIn.end());
      std::sort(eventOut.begin(), eventOut.end());
      for(unsigned int i = 0; i < eventOut.size(); ++i) {
        if(!result.data_.empty() &&
           result.data_.back().pt == eventOut[i].pt) {
          result.data_.back().count += eventOut[i];
        } else {
          if(!result.data_.empty() &&
             result.data_.back().count.is_45()) {
            result.is_manhattan_ = false;
          }
          result.data_.push_back(eventOut[i]);
        }
        if(result.data_.back().count == typename polygon_45_formation<Unit>::Vertex45Count(0, 0, 0, 0)) {
          result.data_.pop_back();
        }
      }
      if(!result.data_.empty() &&
         result.data_.back().count.is_45()) {
        result.is_manhattan_ = false;
      }
      //std::cout << "DONE SCANNING\n";
    }
  }

//   template <typename Unit>
//   inline void polygon_45_set_data<Unit>::applyBoolean_(int op, value_type& rvalue) const {
//     value_type tmp;
//     applyBoolean_(tmp, op, rvalue);
//     data_.swap(tmp); //swapping vectors should be constant time operation
//     unsorted_ = false;
//     dirty_ = false;
//   }

//   template <typename Unit>
//   inline void polygon_45_set_data<Unit>::applyBoolean_(value_type& result, int op, 
//                                                        value_type& rvalue) const {
//     unsigned int originalSize = result.size();
//     try {
//       applyBooleanException_(result, op, rvalue);
//     } catch (std::string str) {
//       std::string msg = "GTL 45 Boolean error, precision insufficient to represent edge intersection coordinate value.";
//       if(str == msg) {
//         result.resize(originalSize);
//         snap();
//         std::sort(data_.begin(), data_.end());
//         for(typename value_type::iterator itr = rvalue.begin();
//             itr != rvalue.end(); ++itr) {
//           snap_vertex_45(*itr);
//         }
//         std::sort(rvalue.begin(), rvalue.end());
//         applyBooleanException_(result, op, rvalue);
//       } else { throw str; }
//     }
//   }

//   template <typename Unit>
//   inline void polygon_45_set_data<Unit>::applyBooleanException_(value_type& result, int op, 
//                                                                 value_type& rvalue) const {
//     typename boolean_op_45<Unit>::Scan45 scan45(op);
//     std::vector<typename boolean_op_45<Unit>::Vertex45> eventOut;
//     std::vector<typename boolean_op_45<Unit>::Scan45Vertex> eventIn;
//     typename value_type::const_iterator iter1 = data_.begin();
//     typename value_type::const_iterator iter2 = rvalue.begin();
//     typename value_type::const_iterator end1 = data_.end();
//     typename value_type::const_iterator end2 = rvalue.end();
//     const Unit UnitMax = std::numeric_limits<Unit>::max();
//     Unit x = UnitMax;
//     while(iter1 != end1 || iter2 != end2) {
//       Unit currentX = UnitMax;
//       if(iter1 != end1) currentX = iter1->pt.x();
//       if(iter2 != end2) currentX = std::min(currentX, iter2->pt.x());
//       if(currentX != x) {
//         //std::cout << "SCAN " << currentX << "\n";
//         //scan event
//         scan45.scan(eventOut, eventIn.begin(), eventIn.end());
//         std::sort(eventOut.begin(), eventOut.end());
//         for(unsigned int i = 0; i < eventOut.size(); ++i) {
//           if(!result.empty() &&
//              result.back().pt == eventOut[i].pt) {
//             result.back().count += eventOut[i];
//           } else {
//             result.push_back(eventOut[i]);
//           }
//           if(result.back().count == (typename polygon_45_formation<Unit>::Vertex45Count(0, 0, 0, 0))) {
//             result.pop_back();
//           }
//         }
//         eventOut.clear();
//         eventIn.clear();
//         x = currentX;
//       }
//       //std::cout << "get next\n";
//       if(iter2 != end2 && (iter1 == end1 || iter2->pt.x() < iter1->pt.x() || 
//                            (iter2->pt.x() == iter1->pt.x() &&
//                             iter2->pt.y() < iter1->pt.y()) )) {
//         //std::cout << "case1 next\n";
//         eventIn.push_back(typename boolean_op_45<Unit>::Scan45Vertex(iter2->pt, 
//                                                                      typename polygon_45_formation<Unit>::
//                                                                      Scan45Count(typename polygon_45_formation<Unit>::Count2(0, iter2->count[0]),
//                                                                                  typename polygon_45_formation<Unit>::Count2(0, iter2->count[1]),
//                                                                                  typename polygon_45_formation<Unit>::Count2(0, iter2->count[2]),
//                                                                                  typename polygon_45_formation<Unit>::Count2(0, iter2->count[3]))));
//         ++iter2;
//       } else if(iter1 != end1 && (iter2 == end2 || iter1->pt.x() < iter2->pt.x() || 
//                                   (iter1->pt.x() == iter2->pt.x() &&
//                                    iter1->pt.y() < iter2->pt.y()) )) {
//         //std::cout << "case2 next\n";
//         eventIn.push_back(typename boolean_op_45<Unit>::Scan45Vertex(iter1->pt, 
//                                                                      typename polygon_45_formation<Unit>::
//                                                                      Scan45Count(
//                                                                                  typename polygon_45_formation<Unit>::Count2(iter1->count[0], 0),
//                                                                                  typename polygon_45_formation<Unit>::Count2(iter1->count[1], 0),
//                                                                                  typename polygon_45_formation<Unit>::Count2(iter1->count[2], 0),
//                                                                                  typename polygon_45_formation<Unit>::Count2(iter1->count[3], 0))));
//         ++iter1;
//       } else {
//         //std::cout << "case3 next\n";
//         eventIn.push_back(typename boolean_op_45<Unit>::Scan45Vertex(iter2->pt, 
//                                                                      typename polygon_45_formation<Unit>::
//                                                                      Scan45Count(typename polygon_45_formation<Unit>::Count2(iter1->count[0], 
//                                                                                                                              iter2->count[0]),
//                                                                                  typename polygon_45_formation<Unit>::Count2(iter1->count[1], 
//                                                                                                                              iter2->count[1]),
//                                                                                  typename polygon_45_formation<Unit>::Count2(iter1->count[2], 
//                                                                                                                              iter2->count[2]),
//                                                                                  typename polygon_45_formation<Unit>::Count2(iter1->count[3], 
//                                                                                                                              iter2->count[3]))));
//         ++iter1;
//         ++iter2;
//       }
//     }
//     scan45.scan(eventOut, eventIn.begin(), eventIn.end());
//     std::sort(eventOut.begin(), eventOut.end());
//     for(unsigned int i = 0; i < eventOut.size(); ++i) {
//       if(!result.empty() &&
//          result.back().pt == eventOut[i].pt) {
//         result.back().count += eventOut[i];
//       } else {
//         result.push_back(eventOut[i]);
//       }
//       if(result.back().count == typename polygon_45_formation<Unit>::Vertex45Count(0, 0, 0, 0)) {
//         result.pop_back();
//       }
//     }
//     //std::cout << "DONE SCANNING\n";
//   }


  template <typename Unit>
  inline std::ostream& operator<< (std::ostream& o, const polygon_45_set_data<Unit>& p) {
    o << "Polygon45Set ";
    o << " " << !p.sorted() << " " << p.dirty() << " { ";
    for(typename polygon_45_set_data<Unit>::iterator_type itr = p.begin();
        itr != p.end(); ++itr) {
      o << (*itr).pt << ":";
      for(unsigned int i = 0; i < 4; ++i) {
        o << (*itr).count[i] << ",";
      } o << " ";
      //o << (*itr).first << ":" <<  (*itr).second << "; ";
    }
    o << "} ";
    return o;
  }

  template <typename Unit>
  inline std::istream& operator>> (std::istream& i, polygon_45_set_data<Unit>& p) {
    //TODO
    return i;
  }


}
#endif

