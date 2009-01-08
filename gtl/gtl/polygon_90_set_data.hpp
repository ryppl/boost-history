/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
#ifndef GTL_POLYGON_90_SET_DATA_HPP
#define GTL_POLYGON_90_SET_DATA_HPP
namespace gtl {
  template <typename ltype, typename rtype, typename op_type>
  class polygon_90_set_view;

  template <typename T>
  class polygon_90_set_data {
  public:
    typedef T coordinate_type;
    typedef std::vector<std::pair<coordinate_type, std::pair<coordinate_type, int> > > value_type;
    typedef typename std::vector<std::pair<coordinate_type, std::pair<coordinate_type, int> > >::const_iterator iterator_type;
    typedef polygon_90_set_data operator_arg_type;

    /// default constructor
    inline polygon_90_set_data() : orient_(HORIZONTAL), dirty_(false), unsorted_(false) {}

    /// constructor
    inline polygon_90_set_data(orientation_2d orient) : orient_(orient), dirty_(false), unsorted_(false) {}

    /// constructor from an iterator pair over vertex data
    template <typename iT>
    inline polygon_90_set_data(orientation_2d orient, iT input_begin, iT input_end) {
      dirty_ = true;
      unsorted_ = true;
      for( ; input_begin != input_end; ++input_begin) { insert(*input_begin); }
    }

    /// copy constructor
    inline polygon_90_set_data(const polygon_90_set_data& that) : 
      orient_(that.orient_), data_(that.data_), dirty_(that.dirty_), unsorted_(that.unsorted_) {}

    /// copy with orientation change constructor
    inline polygon_90_set_data(orientation_2d orient, const polygon_90_set_data& that) : 
      orient_(orient), dirty_(false), unsorted_(false) {
      insert(that, false, that.orient_);
    }

    /// destructor
    inline ~polygon_90_set_data() {}

    /// assignement operator
    inline polygon_90_set_data& operator=(const polygon_90_set_data& that) {
      if(this == &that) return *this;
      orient_ = that.orient_;
      data_ = that.data_;
      dirty_ = that.dirty_;
      unsorted_ = that.unsorted_;
      return *this;
    }

    template <typename ltype, typename rtype, typename op_type>
    inline polygon_90_set_data& operator=(const polygon_90_set_view<ltype, rtype, op_type>& that);

    template <typename geometry_object>
    inline polygon_90_set_data& operator=(const geometry_object& geometry) {
      data_.clear();
      insert(geometry);
      return *this;
    }

//     template <typename geometry_object>
//     inline polygon_90_set_data& operator=(const  polygon_90_set_const_wrapper<geometry_object>& geometry) {
//       data_.clear();
//       insert(geometry.begin(), geometry.end(), geometry.orient());
//       return *this;
//     }

    /// insert iterator range
    template <typename iT>
    inline void insert(iT input_begin, iT input_end, orientation_2d orient) {
      if(input_begin == input_end) return;
      dirty_ = true;
      unsorted_ = true;
      for( ; input_begin != input_end; ++input_begin) {
        insert(*input_begin, false, orient);
      }
    }

    inline void insert(const polygon_90_set_data& polygon_set) {
      insert(polygon_set.begin(), polygon_set.end(), polygon_set.orient());
    }

    template <typename geometry_type>
    inline void insert(const geometry_type& geometry_object, bool is_hole = false, orientation_2d orient = HORIZONTAL) {
      iterator_geometry_to_set<typename geometry_concept<geometry_type>::type, geometry_type>
        begin_input(geometry_object, LOW, orient_, is_hole), end_input(geometry_object, HIGH, orient_, is_hole);
      insert(begin_input, end_input, orient_);
    }

    inline void insert(const std::pair<coordinate_type, std::pair<coordinate_type, int> >& vertex, bool is_hole = false, 
                       orientation_2d orient = HORIZONTAL) {
      data_.push_back(vertex);
      if(orient != orient_) std::swap(data_.back().first, data_.back().second.first);
      if(is_hole) data_.back().second.second *= -1;
      dirty_ = true;
      unsorted_ = true;
    }

    inline void insert(coordinate_type major_coordinate, const std::pair<interval_data<coordinate_type>, int>& edge) {
      std::pair<coordinate_type, std::pair<coordinate_type, int> > vertex;
      vertex.first = major_coordinate;
      vertex.second.first = edge.first.get(LOW);
      vertex.second.second = edge.second;
      insert(vertex);
      vertex.second.first = edge.first.get(HIGH);
      vertex.second.second *= -1;
      insert(vertex);
    }

    template <typename output_container>
    inline void get(output_container& output) const {
      get_dispatch(output, typename geometry_concept<typename output_container::value_type>::type());
    }

    /// equivalence operator 
    inline bool operator==(const polygon_90_set_data& p) const {
      if(orient_ == p.orient()) {
        clean();
        p.clean();
        return data_ == p.data_;
      } else {
        return false;
      }
    }

    /// inequivalence operator 
    inline bool operator!=(const polygon_90_set_data& p) const {
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

    /// clear the contents of the polygon_90_set_data
    inline void clear() { data_.clear(); dirty_ = unsorted_ = false; }

    /// find out if Polygon set is empty
    inline bool empty() const { clean(); return data_.empty(); }

    /// find out if Polygon set is sorted
    inline bool sorted() const { return !unsorted_; }

    /// find out if Polygon set is clean
    inline bool dirty() const { return dirty_; }

    /// get the scanline orientation of the polygon set
    inline orientation_2d orient() const { return orient_; }

    void clean() const {
      sort();
      if(dirty_) {
        boolean_op::applyBooleanOr(data_);
        dirty_ = false;
      }
    }

    void sort() const{
      if(unsorted_) {
        std::sort(data_.begin(), data_.end());
        unsorted_ = false;
      }
    }

    template <typename input_iterator_type>
    void set(input_iterator_type input_begin, input_iterator_type input_end, orientation_2d orient) {
      data_.clear();
      data_.insert(data_.end(), input_begin, input_end);
      orient_ = orient;
      dirty_ = true;
      unsorted_ = true;
    }

    void set(const value_type& value, orientation_2d orient) {
      data_ = value; 
      orient_ = orient;
      dirty_ = true;
      unsorted_ = true;
    }

    template <class T2, typename iterator_type_1, typename iterator_type_2>
    void applyBooleanBinaryOp(iterator_type_1 itr1, iterator_type_1 itr1_end,
                              iterator_type_2 itr2, iterator_type_2 itr2_end,
                              T2 defaultCount) {
      data_.clear();
      boolean_op::applyBooleanBinaryOp(data_, itr1, itr1_end, itr2, itr2_end, defaultCount);
    }

  private:
    orientation_2d orient_;
    mutable value_type data_;
    mutable bool dirty_;
    mutable bool unsorted_;
  
  private:
    //functions
    template <typename output_container>
    void get_dispatch(output_container& output, rectangle_concept tag) const {
      clean();
      get_rectangles(output, data_.begin(), data_.end(), orient_, tag);
    }
    template <typename output_container>
    void get_dispatch(output_container& output, polygon_90_concept tag) const {
      get_fracture(output, true, tag);
    }
    template <typename output_container>
    void get_dispatch(output_container& output, polygon_90_with_holes_concept tag) const {
      get_fracture(output, false, tag);
    }
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
      get_polygons(container, data_.begin(), data_.end(), orient_, fracture_holes, tag);
    }
  };

  template <typename coordinate_type, typename property_type>
  class property_merge_90 {
  private:
    std::vector<std::pair<property_merge_point<coordinate_type>, std::pair<property_type, int> > > pmd_;
  public:
    inline property_merge_90() {}
    inline property_merge_90(const property_merge_90& that) : pmd_(that.pmd_) {}
    inline property_merge_90& operator=(const property_merge_90& that) { pmd_ = that.pmd_; }
    inline void insert(const polygon_90_set_data<coordinate_type>& ps, const property_type& property) {
      merge_scanline<coordinate_type, property_type, polygon_90_set_data<coordinate_type> >::
        populate_property_merge_data(pmd_, ps.begin(), ps.end(), property, ps.orient());
    }
    template <class GeoObjT>
    inline void insert(const GeoObjT& geoObj, const property_type& property) {
      polygon_90_set_data<coordinate_type> ps;
      ps.insert(geoObj);
      insert(ps, property);
    }
    //merge properties of input geometries and store the resulting geometries of regions
    //with unique sets of merged properties to polygons sets in a map keyed by sets of properties
    // T = std::map<std::set<property_type>, polygon_90_set_data<coordiante_type> > or
    // T = std::map<std::vector<property_type>, polygon_90_set_data<coordiante_type> >
    template <typename ResultType> 
    inline void merge(ResultType& result) {
      merge_scanline<coordinate_type, property_type, polygon_90_set_data<coordinate_type>, typename ResultType::key_type> ms;
      ms.perform_merge(result, pmd_);
    }
  };

  //ConnectivityExtraction computes the graph of connectivity between rectangle, polygon and
  //polygon set graph nodes where an edge is created whenever the geometry in two nodes overlap
  template <typename coordinate_type>
  class connectivity_extraction {
  private:
    typedef typename touch_90_operation<coordinate_type>::TouchSetData tsd;
    tsd tsd_;
    unsigned int nodeCount_;
  public:
    inline connectivity_extraction() : nodeCount_(0) {}
    inline connectivity_extraction(const connectivity_extraction& that) : tsd_(that.tsd_),
                                                                          nodeCount_(that.nodeCount_) {}
    inline connectivity_extraction& operator=(const connectivity_extraction& that) { 
      tsd_ = that.tsd_; 
      nodeCount_ = that.nodeCount_; {}
      return *this;
    }
    
    //insert a polygon set graph node, the value returned is the id of the graph node
    inline unsigned int insert(const polygon_90_set_data<coordinate_type>& ps) {
      ps.clean();
      touch_90_operation<coordinate_type>::populateTouchSetData(tsd_, ps.begin(), ps.end(), nodeCount_);
      return nodeCount_++;
    }
    template <class GeoObjT>
    inline unsigned int insert(const GeoObjT& geoObj) {
      polygon_90_set_data<coordinate_type> ps;
      ps.insert(geoObj);
      return insert(ps);
    }
    
    //extract connectivity and store the edges in the graph
    //graph must be indexable by graph node id and the indexed value must be a std::set of
    //graph node id
    template <class GraphT>
    inline void extract(GraphT& graph) {
      touch_90_operation<coordinate_type>::performTouch(graph, tsd_);
    }
  };

}
#endif

