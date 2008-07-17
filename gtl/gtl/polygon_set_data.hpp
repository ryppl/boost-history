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
      typedef typename output_container::value_type rectangle_type;
      typedef typename rectangle_concept::coordinate_type<rectangle_type>::type Unit;
      clean();
      rectangle_data<Unit> model;
      Unit prevPos = std::numeric_limits<Unit>::max();
      rectangle_formation::ScanLineToRects<rectangle_type> scanlineToRects(orient_, model);
      for(iterator_type itr = data_.begin();
          itr != data_.end(); ++ itr) {
        Unit pos = (*itr).first;
        if(pos != prevPos) {
          scanlineToRects.nextMajorCoordinate(pos);
          prevPos = pos;
        }
        Unit lowy = (*itr).second.first;
        ++itr;
        Unit highy = (*itr).second.first;
        scanlineToRects.processEdge(output, interval_data<Unit>(lowy, highy));
        if(abs((*itr).second.second) > 1) --itr; //next edge begins from this vertex
      }
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
      typedef typename output_container::value_type polygon_type;
      polygon_type poly;
      clean();
      unsigned int countPolygons = 0;
      polygon_formation::ScanLineToPolygonItrs<true, coordinate_type> scanlineToPolygonItrsV(fracture_holes);
      polygon_formation::ScanLineToPolygonItrs<false, coordinate_type> scanlineToPolygonItrsH(fracture_holes);
      std::vector<interval_data<coordinate_type> > leftEdges;
      std::vector<interval_data<coordinate_type> > rightEdges;
      coordinate_type prevPos = std::numeric_limits<coordinate_type>::max();
      coordinate_type prevY = std::numeric_limits<coordinate_type>::max();
      int count = 0;
      for(iterator_type itr = data_.begin();
          itr != data_.end(); ++ itr) {
        coordinate_type pos = (*itr).first;
        if(pos != prevPos) {
          if(orient_ == VERTICAL) {
            typename polygon_formation::ScanLineToPolygonItrs<true, coordinate_type>::iterator itrPoly, itrPolyEnd;
            scanlineToPolygonItrsV.processEdges(itrPoly, itrPolyEnd, prevPos, leftEdges, rightEdges);
            for( ; itrPoly != itrPolyEnd; ++ itrPoly) {
              ++countPolygons;
              concept_type::assign(poly, *itrPoly);
              container.insert(container.end(), poly);
            }
          } else {
            typename polygon_formation::ScanLineToPolygonItrs<false, coordinate_type>::iterator itrPoly, itrPolyEnd;
            scanlineToPolygonItrsH.processEdges(itrPoly, itrPolyEnd, prevPos, leftEdges, rightEdges);
            for( ; itrPoly != itrPolyEnd; ++ itrPoly) {
              ++countPolygons;
              concept_type::assign(poly, *itrPoly);
              container.insert(container.end(), poly);
            }
          }
          leftEdges.clear();
          rightEdges.clear();
          prevPos = pos;
          prevY = (*itr).second.first;
          count = (*itr).second.second;
          continue;
        }
        coordinate_type y = (*itr).second.first;
        if(count != 0 && y != prevY) {
          std::pair<interval_data<coordinate_type>, int> element(interval_data<coordinate_type>(prevY, y), count);
          if(element.second == 1) {
            if(leftEdges.size() && leftEdges.back().high() == element.first.low()) {
              interval_concept::encompass(leftEdges.back(), element.first, interval_concept());
            } else {
              leftEdges.push_back(element.first);
            }
          } else {
            if(rightEdges.size() && rightEdges.back().high() == element.first.low()) {
              interval_concept::encompass(rightEdges.back(), element.first, interval_concept());
            } else {
              rightEdges.push_back(element.first);
            }
          }

        }
        prevY = y;
        count += (*itr).second.second;
      }
      if(orient_ == VERTICAL) {
        typename polygon_formation::ScanLineToPolygonItrs<true, coordinate_type>::iterator itrPoly, itrPolyEnd;
        scanlineToPolygonItrsV.processEdges(itrPoly, itrPolyEnd, prevPos, leftEdges, rightEdges);
        for( ; itrPoly != itrPolyEnd; ++ itrPoly) {
          ++countPolygons;
          concept_type::assign(poly, *itrPoly);
          container.insert(container.end(), poly);
        }
      } else {
        typename polygon_formation::ScanLineToPolygonItrs<false, coordinate_type>::iterator itrPoly, itrPolyEnd;
        scanlineToPolygonItrsH.processEdges(itrPoly, itrPolyEnd, prevPos, leftEdges, rightEdges);
        for( ; itrPoly != itrPolyEnd; ++ itrPoly) {
          ++countPolygons;
          concept_type::assign(poly, *itrPoly);
          container.insert(container.end(), poly);
        }
      }
    }
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

