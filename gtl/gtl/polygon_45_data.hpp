/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
#ifndef GTL_POLYGON_45_DATA_HPP
#define GTL_POLYGON_45_DATA_HPP
namespace gtl {
struct polygon_45_concept;
template <typename T>
class polygon_45_data {
public:
  typedef polygon_45_concept geometry_type;
  typedef T coordinate_type;
  typedef typename std::vector<point_data<coordinate_type> >::const_iterator iterator_type;
  typedef iterator_points_to_compact<iterator_type, point_data<coordinate_type> > compact_iterator_type;

  inline polygon_45_data(){;} //do nothing default constructor

  template<class iT>
  inline polygon_45_data& set(iT input_begin, iT input_end) {
    coords_.clear();  //just in case there was some old data there
    coords_.insert(coords_.end(), input_begin, input_end);
    return *this;
  }

  template<typename iT>
  inline polygon_45_data& set_compact(iT input_begin, iT input_end) {
    typedef iterator_compact_to_points<iT, point_data<coordinate_type> > iTC;
    return set(iTC(input_begin, input_end), iTC(input_end, input_end));
  }

  /// copy constructor (since we have dynamic memory)
  inline polygon_45_data(const polygon_45_data& that) : coords_(that.coords_) {}
  
  /// assignment operator (since we have dynamic memory do a deep copy)
  inline polygon_45_data& operator=(const polygon_45_data& that) {
    coords_ = that.coords_;
    return *this;
  }

  template <typename T2>
  inline polygon_45_data& operator=(const T2& rvalue);

  /// get begin iterator, returns a pointer to a const Unit
  inline iterator_type begin() const { return coords_.begin(); }

  /// get end iterator, returns a pointer to a const Unit
  inline iterator_type end() const { return coords_.end(); }

  /// get begin iterator, returns a pointer to a const Unit
  inline compact_iterator_type begin_compact() const { return compact_iterator_type(begin()); }

  /// get end iterator, returns a pointer to a const Unit
  inline compact_iterator_type end_compact() const { return compact_iterator_type(begin()); }

  inline std::size_t size() const { return coords_.size(); }

private:
  std::vector<point_data<coordinate_type> > coords_; 
};

}
#endif

