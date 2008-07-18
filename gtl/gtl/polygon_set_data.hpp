/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
#ifndef GTL_POLYGON_SET_DATA_HPP
#define GTL_POLYGON_SET_DATA_HPP
namespace gtl {

  template <typename T>
  class polygon_set_data {
  public:
    typedef T coordinate_type;
    typedef std::vector<std::pair<coordinate_type, std::pair<coordinate_type, int> > > value_type;
    typedef typename std::vector<std::pair<coordinate_type, std::pair<coordinate_type, int> > >::const_iterator iterator_type;
    typedef polygon_set_data operator_arg_type;
    typedef operator_provides_storage operator_storage_tag;

    /// default constructor
    inline polygon_set_data() : orient_(HORIZONTAL), dirty_(false), unsorted_(false) {}

    /// constructor
    inline polygon_set_data(orientation_2d orient) : orient_(orient), dirty_(false), unsorted_(false) {}

    /// constructor from an iterator pair over vertex data
    template <typename iT>
    inline polygon_set_data(orientation_2d orient, iT input_begin, iT input_end) {
      dirty_ = true;
      unsorted_ = true;
      for( ; input_begin != input_end; ++input_begin) { insert(*input_begin); }
    }

    /// copy constructor
    inline polygon_set_data(const polygon_set_data& that) : 
      orient_(that.orient_), data_(that.data_), dirty_(that.dirty_), unsorted_(that.unsorted_) {}

    /// copy with orientation change constructor
    inline polygon_set_data(orientation_2d orient, const polygon_set_data& that) : 
      orient_(orient), dirty_(false), unsorted_(false) {
      if(that.orient() == orient) { (*this) = that; }
      else if(!that.data_.empty()) {
        dirty_ = unsorted_ = true;
        iterator_vertex_orient_conversion<iterator_type> itr1(that.data_.begin()), iter2(that.data_.end());
        data_.resize(that.data_.size());
        for( ; itr1 != iter2; ++itr1) {
          data_.push_back(*itr1);
        }
      }
    }

    /// destructor
    inline ~polygon_set_data() {}

    /// assignement operator
    inline polygon_set_data& operator=(const polygon_set_data& that) {
      if(this == &that) return *this;
      orient_ = that.orient_;
      data_ = that.data_;
      dirty_ = that.dirty_;
      unsorted_ = that.unsorted_;
      return *this;
    }

    template <typename ltype, typename rtype, typename op_type, typename ltag, typename rtag>
    inline polygon_set_data& operator=(const polygon_set_view<ltype, rtype, op_type, ltag, rtag>& that) {
      orient_ = that.orient();
      data_ = that.value();
      dirty_ = false;
      unsorted_ = false;
      return *this;
    }

    template <typename geometry_object>
    inline polygon_set_data& operator=(const geometry_object& geometry) {
      data_.clear();
      insert(geometry);
      return *this;
    }

    template <typename geometry_object>
    inline polygon_set_data& operator=(const  polygon_set_const_wrapper<geometry_object>& geometry) {
      data_.clear();
      insert(geometry.begin(), geometry.end());
      return *this;
    }

    /// insert iterator range
    template <typename iT>
    inline void insert(iT input_begin, iT input_end) {
      for( ; input_begin != input_end; ++input_begin) {
        insert(*input_begin);
      }
    }

    template <typename geometry_type>
    inline void insert(const geometry_type& geometry_object) {
      iterator_geometry_to_set<typename geometry_concept<geometry_type>::type, geometry_type>
        begin_input(geometry_object, LOW, orient_), end_input(geometry_object, HIGH, orient_);
      insert(begin_input, end_input);
    }

    inline void insert(const std::pair<coordinate_type, std::pair<coordinate_type, int> >& vertex) {
      data_.push_back(vertex);
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
    inline void get(output_container& output) {
      get_dispatch(output, typename geometry_concept<typename output_container::value_type>::type());
    }

    /// equivalence operator 
    inline bool operator==(const polygon_set_data& p) const {
      if(orient_ == p.orient()) {
        clean();
        p.clean();
        return data_ == p.data_;
      } else {
        return false;
      }
    }

    /// inequivalence operator 
    inline bool operator!=(const polygon_set_data& p) const {
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

    /// clear the contents of the polygon_set_data
    inline void clear() { data_.clear(); dirty_ = unsorted_ = false; }

    /// find out if Polygon set is empty
    inline bool empty() const { return data_.empty(); }

    /// find out if Polygon set is sorted
    inline bool sorted() const { return !unsorted_; }

    /// find out if Polygon set is clean
    inline bool dirty() const { return dirty_; }

    /// get the scanline orientation of the polygon set
    inline orientation_2d orient() const { return orient_; }

    void clean() const {
      if(unsorted_) sort();
      if(dirty_) {
        boolean_op::applyBooleanOr(data_);
        dirty_ = false;
      }
    }

    void sort() const{
      std::sort(data_.begin(), data_.end());
      unsorted_ = false;
    }

    template <typename input_iterator_type>
    void set(input_iterator_type input_begin, input_iterator_type input_end, orientation_2d orient) {
      data_.clear();
      data_.insert(data_.end(), input_begin, input_end);
      orient_ = orient;
    }

    void set(const value_type& value, orientation_2d orient) {
      data_ = value; 
      orient_ = orient;
    }
  private:
    orientation_2d orient_;
    mutable value_type data_;
    mutable bool dirty_;
    mutable bool unsorted_;
  
  private:
    //functions
    template <typename output_container>
    void get_dispatch(output_container& output, rectangle_concept tag) {
      clean();
      get_rectangles(output, data_.begin(), data_.end(), orient_, tag);
    }
    template <typename output_container>
    void get_dispatch(output_container& output, polygon_90_concept tag) {
      get_fracture(output, true, tag);
    }
    template <typename output_container>
    void get_dispatch(output_container& output, polygon_90_with_holes_concept tag) {
      get_fracture(output, false, tag);
    }
    template <typename output_container, typename concept_type>
    void get_fracture(output_container& container, bool fracture_holes, concept_type tag) {
      clean();
      get_polygons(container, data_.begin(), data_.end(), orient_, fracture_holes, tag);
    }
  };

  template <typename T>
  struct polygon_set_traits<polygon_set_data<T> > {
    typedef typename polygon_set_data<T>::coordinate_type coordinate_type;
    typedef typename polygon_set_data<T>::iterator_type iterator_type;
    typedef typename polygon_set_data<T>::operator_arg_type operator_arg_type;
    typedef typename polygon_set_data<T>::operator_storage_tag operator_storage_tag;

    static inline iterator_type begin(const polygon_set_data<T>& polygon_set) {
      return polygon_set.begin();
    }

    static inline iterator_type end(const polygon_set_data<T>& polygon_set) {
      return polygon_set.end();
    }

    template <typename input_iterator_type>
    static inline void set(polygon_set_data<T>& polygon_set, 
                           input_iterator_type input_begin, input_iterator_type input_end, 
                           orientation_2d orient) {
      polygon_set.set(input_begin, input_end, orient);
    }

    static inline orientation_2d orient(const polygon_set_data<T>& polygon_set) { return polygon_set.orient(); }

    static inline bool dirty(const polygon_set_data<T>& polygon_set) { return polygon_set.dirty(); }

    static inline bool sorted(const polygon_set_data<T>& polygon_set) { return polygon_set.sorted(); }

  };


  
  template <typename coordinate_type, typename property_type>
  class property_merge {
  private:
    std::vector<std::pair<property_merge_point<coordinate_type>, std::pair<property_type, int> > > pmd_;
  public:
    inline property_merge() {}
    inline property_merge(const property_merge& that) : pmd_(that.pmd_) {}
    inline property_merge& operator=(const property_merge& that) { pmd_ = that.pmd_; }
    inline void insert(const polygon_set_data<coordinate_type>& ps, const property_type& property) {
      merge_scanline<coordinate_type, property_type, polygon_set_data<coordinate_type> >::
        populate_property_merge_data(pmd_, ps.begin(), ps.end(), property, ps.orient());
    }
    template <class GeoObjT>
    inline void insert(const GeoObjT& geoObj, const property_type& property) {
      polygon_set_data<coordinate_type> ps;
      ps.insert(geoObj);
      insert(ps, property);
    }
    //merge properties of input geometries and store the resulting geometries of regions
    //with unique sets of merged properties to polygons sets in a map keyed by sets of properties
    // T = std::map<std::set<property_type>, polygon_set_data<coordiante_type> > or
    // T = std::map<std::vector<property_type>, polygon_set_data<coordiante_type> >
    template <typename ResultType> 
    inline void merge(ResultType& result) {
      merge_scanline<coordinate_type, property_type, polygon_set_data<coordinate_type>, typename ResultType::key_type> ms;
      ms.perform_merge(result, pmd_);
    }
  };

}
#endif

