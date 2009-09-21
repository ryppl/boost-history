/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
#ifndef BOOST_POLYGON_POLYGON_SET_DATA_HPP
#define BOOST_POLYGON_POLYGON_SET_DATA_HPP
#include "polygon_45_set_data.hpp"
#include "polygon_45_set_concept.hpp"
#include "detail/polygon_arbitrary_formation.hpp"
namespace boost { namespace polygon{

  //foward declare view
  template <typename ltype, typename rtype, int op_type> class polygon_set_view;

  template <typename T>
  class polygon_set_data {
  public:
    typedef T coordinate_type;
    typedef point_data<T> point_type;
    typedef std::pair<point_type, point_type> edge_type;
    typedef std::pair<edge_type, int> element_type;
    typedef std::vector<element_type> value_type;
    typedef typename value_type::const_iterator iterator_type;
    typedef polygon_set_data operator_arg_type;

    // default constructor
    inline polygon_set_data() : data_(), dirty_(false), unsorted_(false), is_45_(true) {}

    // constructor from an iterator pair over edge data
    template <typename iT>
    inline polygon_set_data(iT input_begin, iT input_end) : data_(), dirty_(false), unsorted_(false), is_45_(true) {
      for( ; input_begin != input_end; ++input_begin) { insert(*input_begin); }
    }

    // copy constructor
    inline polygon_set_data(const polygon_set_data& that) : 
      data_(that.data_), dirty_(that.dirty_), unsorted_(that.unsorted_), is_45_(that.is_45_) {}

    // copy constructor
    template <typename ltype, typename rtype, int op_type> 
    inline polygon_set_data(const polygon_set_view<ltype, rtype, op_type>& that);

    // destructor
    inline ~polygon_set_data() {}

    // assignement operator
    inline polygon_set_data& operator=(const polygon_set_data& that) {
      if(this == &that) return *this;
      data_ = that.data_;
      dirty_ = that.dirty_;
      unsorted_ = that.unsorted_;
      is_45_ = that.is_45_;
      return *this;
    }

    template <typename ltype, typename rtype, int op_type>
    inline polygon_set_data& operator=(const polygon_set_view<ltype, rtype, op_type>& geometry) {
      (*this) = geometry.value();
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


    // insert iterator range
    inline void insert(iterator_type input_begin, iterator_type input_end) {
      if(input_begin == input_end || (!data_.empty() && &(*input_begin) == &(*(data_.begin())))) return;
      dirty_ = true;
      unsorted_ = true;
      while(input_begin != input_end) {
        insert(*input_begin);
        ++input_begin;
      }
    }

    // insert iterator range
    template <typename iT>
    inline void insert(iT input_begin, iT input_end) {
      if(input_begin == input_end) return;
      for(; input_begin != input_end; ++input_begin) {
        insert(*input_begin);
      }
    }

    template <typename geometry_type>
    inline void insert(const geometry_type& geometry_object, bool is_hole = false) {
      insert(geometry_object, is_hole, typename geometry_concept<geometry_type>::type());
    }

    template <typename polygon_type>
    inline void insert(const polygon_type& polygon_object, bool is_hole, polygon_concept ) {
      bool first_iteration = true;
      point_type first_point;
      point_type previous_point;
      point_type current_point;
      direction_1d winding_dir = winding(polygon_object);
      int multiplier = winding_dir == COUNTERCLOCKWISE ? 1 : -1;
      if(is_hole) multiplier *= -1;
      for(typename polygon_traits<polygon_type>::iterator_type itr = begin_points(polygon_object);
          itr != end_points(polygon_object); ++itr) {
        assign(current_point, *itr);
        if(first_iteration) {
          first_iteration = false;
          first_point = previous_point = current_point;
        } else {
          if(previous_point != current_point) {
            element_type elem(edge_type(previous_point, current_point), 
                              ( previous_point.get(HORIZONTAL) == current_point.get(HORIZONTAL) ? -1 : 1) * multiplier);
            insert_clean(elem);
          }
        }
        previous_point = current_point;
      }
      current_point = first_point;
      if(!first_iteration) {
        if(previous_point != current_point) {
          element_type elem(edge_type(previous_point, current_point), 
                            ( previous_point.get(HORIZONTAL) == current_point.get(HORIZONTAL) ? -1 : 1) * multiplier);
          insert_clean(elem);
        }
        dirty_ = true;
        unsorted_ = true;
      }
    }

    template <typename coordinate_type_2>
    inline void insert(const polygon_45_set_data<coordinate_type_2>& ps) {
      std::vector<polygon_45_with_holes_data<coordinate_type_2> > polys;
      assign(polys, ps);
      insert(polys.begin(), polys.end());
    }

    template <typename coordinate_type_2>
    inline void insert(const polygon_90_set_data<coordinate_type_2>& ps) {
      std::vector<polygon_90_with_holes_data<coordinate_type_2> > polys;
      assign(polys, ps);
      insert(polys.begin(), polys.end());
    }

    template <typename polygon_type>
    inline void insert(const polygon_type& polygon_object, bool is_hole, polygon_45_concept ) {
      insert(polygon_object, is_hole, polygon_concept()); }

    template <typename polygon_type>
    inline void insert(const polygon_type& polygon_object, bool is_hole, polygon_90_concept ) {
      insert(polygon_object, is_hole, polygon_concept()); }

    template <typename polygon_with_holes_type>
    inline void insert(const polygon_with_holes_type& polygon_with_holes_object, bool is_hole, 
                       polygon_with_holes_concept ) {
      insert(polygon_with_holes_object, is_hole, polygon_concept());
      for(typename polygon_with_holes_traits<polygon_with_holes_type>::iterator_holes_type itr = 
            begin_holes(polygon_with_holes_object);
          itr != end_holes(polygon_with_holes_object); ++itr) {
        insert(*itr, !is_hole, polygon_concept());
      }
    }

    template <typename polygon_with_holes_type>
    inline void insert(const polygon_with_holes_type& polygon_with_holes_object, bool is_hole, 
                       polygon_45_with_holes_concept ) {
      insert(polygon_with_holes_object, is_hole, polygon_with_holes_concept()); }

    template <typename polygon_with_holes_type>
    inline void insert(const polygon_with_holes_type& polygon_with_holes_object, bool is_hole, 
                       polygon_90_with_holes_concept ) {
      insert(polygon_with_holes_object, is_hole, polygon_with_holes_concept()); }

    template <typename rectangle_type>
    inline void insert(const rectangle_type& rectangle_object, bool is_hole, rectangle_concept ) {
      polygon_90_data<coordinate_type> poly;
      assign(poly, rectangle_object);
      insert(poly, is_hole, polygon_concept());
    }

    inline void insert_clean(const element_type& edge) {
      if( ! scanline_base<coordinate_type>::is_45_degree(edge.first) &&
          ! scanline_base<coordinate_type>::is_horizontal(edge.first) &&
          ! scanline_base<coordinate_type>::is_vertical(edge.first) ) is_45_ = false;
      data_.push_back(edge);
      if(data_.back().first.second < data_.back().first.first) {
        std::swap(data_.back().first.second, data_.back().first.first);
        data_.back().second *= -1;
      }
    }

    inline void insert(const element_type& edge) {
      insert_clean(edge);
      dirty_ = true;
      unsorted_ = true;
    }

    template <typename output_container>
    inline void get(output_container& output) {
      get_dispatch(output, typename geometry_concept<typename output_container::value_type>::type());
    }

    // append to the container cT with polygons of three or four verticies
    // slicing orientation is vertical
    template <class cT>
    void get_trapezoids(cT& container) const {
      clean();
      trapezoid_arbitrary_formation<coordinate_type> pf;
      typedef typename polygon_arbitrary_formation<coordinate_type>::vertex_half_edge vertex_half_edge;
      std::vector<vertex_half_edge> data;
      for(iterator_type itr = data_.begin(); itr != data_.end(); ++itr){
        data.push_back(vertex_half_edge((*itr).first.first, (*itr).first.second, (*itr).second));
        data.push_back(vertex_half_edge((*itr).first.second, (*itr).first.first, -1 * (*itr).second));
      }
      std::sort(data.begin(), data.end());
      pf.scan(container, data.begin(), data.end());
      //std::cout << "DONE FORMING POLYGONS\n";
    }

    // append to the container cT with polygons of three or four verticies
    template <class cT>
    void get_trapezoids(cT& container, orientation_2d slicing_orientation) const {
      if(slicing_orientation == VERTICAL) {
        get_trapezoids(container);
      } else {
        polygon_set_data<T> ps(*this);
        ps.transform(axis_transformation(axis_transformation::SWAP_XY));
        cT result;
        ps.get_trapezoids(result);
        for(typename cT::iterator itr = result.begin(); itr != result.end(); ++itr) {
          ::boost::polygon::transform(*itr, axis_transformation(axis_transformation::SWAP_XY));
        }
        container.insert(container.end(), result.begin(), result.end());
      }
    }

    // equivalence operator 
    inline bool operator==(const polygon_set_data& p) const {
      clean();
      p.clean();
      return data_ == p.data_;
    }

    // inequivalence operator 
    inline bool operator!=(const polygon_set_data& p) const {
      return !((*this) == p);
    }

    // get iterator to begin vertex data
    inline iterator_type begin() const {
      return data_.begin();
    }

    // get iterator to end vertex data
    inline iterator_type end() const {
      return data_.end();
    }

    const value_type& value() const {
      return data_;
    }

    // clear the contents of the polygon_set_data
    inline void clear() { data_.clear(); dirty_ = unsorted_ = false; }

    // find out if Polygon set is empty
    inline bool empty() const { return data_.empty(); }

    // get the Polygon set size in vertices
    inline std::size_t size() const { clean(); return data_.size(); }

    // get the current Polygon set capacity in vertices
    inline std::size_t capacity() const { return data_.capacity(); }

    // reserve size of polygon set in vertices
    inline void reserve(std::size_t size) { return data_.reserve(size); }

    // find out if Polygon set is sorted
    inline bool sorted() const { return !unsorted_; }

    // find out if Polygon set is clean
    inline bool dirty() const { return dirty_; }

    void clean() const;

    void sort() const{
      if(unsorted_) {
        std::sort(data_.begin(), data_.end());
        unsorted_ = false;
      }
    }

    template <typename input_iterator_type>
    void set(input_iterator_type input_begin, input_iterator_type input_end) {
      clear();
      insert(input_begin, input_end);
      dirty_ = true;
      unsorted_ = true;
    }

    void set(const value_type& value) {
      data_ = value; 
      dirty_ = true;
      unsorted_ = true;
    }

    template <typename rectangle_type>
    bool extents(rectangle_type& rect) {
      clean();
      if(empty()) return false;
      bool first_iteration = true;
      for(iterator_type itr = begin();
          itr != end(); ++itr) {
        rectangle_type edge_box;
        set_points(edge_box, (*itr).first.first, (*itr).first.second);
        if(first_iteration)
          rect = edge_box;
        else
          encompass(rect, edge_box);
        first_iteration = false;
      }
      return true;
    }

    template <typename transform_type>
    inline polygon_set_data& 
    transform(const transform_type& tr) {
      std::vector<polygon_with_holes_data<T> > polys;
      get(polys);
      clear();
      for(std::size_t i = 0 ; i < polys.size(); ++i) {
        ::boost::polygon::transform(polys[i], tr);
        insert(polys[i]);
      }
      unsorted_ = true;
      dirty_ = true;
      return *this;
    }

    inline polygon_set_data& 
    scale_up(typename coordinate_traits<coordinate_type>::unsigned_area_type factor) {
      for(typename value_type::iterator itr = data_.begin(); itr != data_.end(); ++itr) {
        ::boost::polygon::scale_up((*itr).first.first, factor);
        ::boost::polygon::scale_up((*itr).first.second, factor);
      }
      return *this;
    }
    
    inline polygon_set_data& 
    scale_down(typename coordinate_traits<coordinate_type>::unsigned_area_type factor) {
      for(typename value_type::iterator itr = data_.begin(); itr != data_.end(); ++itr) {
        ::boost::polygon::scale_down((*itr).first.first, factor);
        ::boost::polygon::scale_down((*itr).first.second, factor);
      }
      unsorted_ = true;
      dirty_ = true;
      return *this;
    }
    
    template <typename scaling_type>
    inline polygon_set_data& scale(polygon_set_data& polygon_set, 
                                   const scaling_type& scaling) {
      for(typename value_type::iterator itr = begin(); itr != end(); ++itr) {
        ::boost::polygon::scale((*itr).first.first, scaling);
        ::boost::polygon::scale((*itr).first.second, scaling);
      }
      unsorted_ = true;
      dirty_ = true;
      return *this;
    }

    inline polygon_set_data&
    interact(const polygon_set_data& that); 

    inline bool downcast(polygon_45_set_data<coordinate_type>& result) const {
      if(!is_45_) return false;
      for(iterator_type itr = begin(); itr != end(); ++itr) {
        const element_type& elem = *itr;
        int count = elem.second;
        int rise = 1; //up sloping 45
        if(scanline_base<coordinate_type>::is_horizontal(elem.first)) rise = 0;
        else if(scanline_base<coordinate_type>::is_vertical(elem.first)) rise = 2;
        else {
          if(!scanline_base<coordinate_type>::is_45_degree(elem.first)) {
            is_45_ = false;
            return false; //consider throwing because is_45_ has be be wrong
          }
          if(elem.first.first.y() > elem.first.second.y()) rise = -1; //down sloping 45
        }
        typename polygon_45_set_data<coordinate_type>::Vertex45Compact vertex(elem.first.first, rise, count);
        result.insert(vertex);
        typename polygon_45_set_data<coordinate_type>::Vertex45Compact vertex2(elem.first.second, rise, -count);
        result.insert(vertex2);
      }
      return true;
    }

    inline GEOMETRY_CONCEPT_ID concept_downcast() const {
      typedef typename coordinate_traits<coordinate_type>::coordinate_difference delta_type;
      bool is_45 = false;
      for(iterator_type itr = begin(); itr != end(); ++itr) {
        const element_type& elem = *itr;
        delta_type h_delta = euclidean_distance(elem.first.first, elem.first.second, HORIZONTAL);
        delta_type v_delta = euclidean_distance(elem.first.first, elem.first.second, VERTICAL);
        if(h_delta != 0 || v_delta != 0) {
          //neither delta is zero and the edge is not MANHATTAN
          if(v_delta != h_delta && v_delta != -h_delta) return POLYGON_SET_CONCEPT;
          else is_45 = true;
        }
      }
      if(is_45) return POLYGON_45_SET_CONCEPT;
      return POLYGON_90_SET_CONCEPT;
    }

  private:
    mutable value_type data_;
    mutable bool dirty_;
    mutable bool unsorted_;
    mutable bool is_45_;

  private:
    //functions

    //TODO write trapezoidization function, object, concept and hook up
    //template <typename output_container>
    //void get_dispatch(output_container& output, rectangle_concept tag) {
    //  clean();
    //  get_rectangles(output, data_.begin(), data_.end(), orient_, tag);
    //}
    template <typename output_container>
    void get_dispatch(output_container& output, polygon_concept tag) const {
      get_fracture(output, true, tag);
    }
    template <typename output_container>
    void get_dispatch(output_container& output, polygon_with_holes_concept tag) const {
      get_fracture(output, false, tag);
    }
    template <typename output_container, typename concept_type>
    void get_fracture(output_container& container, bool fracture_holes, concept_type ) const {
      clean();
      polygon_arbitrary_formation<coordinate_type> pf(fracture_holes);
      typedef typename polygon_arbitrary_formation<coordinate_type>::vertex_half_edge vertex_half_edge;
      std::vector<vertex_half_edge> data;
      for(iterator_type itr = data_.begin(); itr != data_.end(); ++itr){
        data.push_back(vertex_half_edge((*itr).first.first, (*itr).first.second, (*itr).second));
        data.push_back(vertex_half_edge((*itr).first.second, (*itr).first.first, -1 * (*itr).second));
      }
      std::sort(data.begin(), data.end());
      pf.scan(container, data.begin(), data.end());
    }
  };

  struct polygon_set_concept;
  template <typename T>
  struct geometry_concept<polygon_set_data<T> > {
    typedef polygon_set_concept type;
  };
}
}
#include "detail/scan_arbitrary.hpp"

namespace boost { namespace polygon {
  //ConnectivityExtraction computes the graph of connectivity between rectangle, polygon and
  //polygon set graph nodes where an edge is created whenever the geometry in two nodes overlap
  template <typename coordinate_type>
  class connectivity_extraction{
  private:
    typedef arbitrary_connectivity_extraction<coordinate_type, int> ce;
    ce ce_;
    unsigned int nodeCount_;
  public:
    inline connectivity_extraction() : ce_(), nodeCount_(0) {}
    inline connectivity_extraction(const connectivity_extraction& that) : ce_(that.ce_),
                                                                          nodeCount_(that.nodeCount_) {}
    inline connectivity_extraction& operator=(const connectivity_extraction& that) { 
      ce_ = that.ce_; 
      nodeCount_ = that.nodeCount_; {}
      return *this;
    }
    
    //insert a polygon set graph node, the value returned is the id of the graph node
    inline unsigned int insert(const polygon_set_data<coordinate_type>& ps) {
      ps.clean();
      ce_.populateTouchSetData(ps.begin(), ps.end(), nodeCount_);
      return nodeCount_++;
    }
    template <class GeoObjT>
    inline unsigned int insert(const GeoObjT& geoObj) {
      polygon_set_data<coordinate_type> ps;
      ps.insert(geoObj);
      return insert(ps);
    }
    
    //extract connectivity and store the edges in the graph
    //graph must be indexable by graph node id and the indexed value must be a std::set of
    //graph node id
    template <class GraphT>
    inline void extract(GraphT& graph) {
      ce_.execute(graph);
    }
  };

  template <typename T>
  polygon_set_data<T>&
  polygon_set_data<T>::interact(const polygon_set_data<T>& that) {
    connectivity_extraction<coordinate_type> ce;
    std::vector<polygon_with_holes_data<T> > polys;
    get(polys);
    clear();
    for(std::size_t i = 0; i < polys.size(); ++i) {
      ce.insert(polys[i]);
    }
    int id = ce.insert(that);
    std::vector<std::set<int> > graph(id+1);
    ce.extract(graph);
    for(std::set<int>::iterator itr = graph[id].begin();
        itr != graph[id].end(); ++itr) {
      insert(polys[*itr]);
    }
    return *this;
  }
}
}

#include "polygon_set_traits.hpp"
#include "detail/polygon_set_view.hpp"

#include "polygon_set_concept.hpp"
#endif

