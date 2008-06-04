/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
#ifndef GTL_POLYGON_SET_DATA_HPP
#define GTL_POLYGON_SET_DATA_HPP
namespace gtl {
struct operator_provides_storage {};
struct operator_requires_copy {};

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
  template <class iT>
  inline polygon_set_data(orientation_2d orient, iT input_begin, iT input_end) {
    dirty_ = true;
    unsorted_ = true;
    for( ; input_begin != input_end; ++input_begin) { data_.push_back(*input_begin); }
  }

  /// copy constructor
  inline polygon_set_data(const polygon_set_data& that) : 
    orient_(that.orient_), data_(that.data_), dirty_(that.dirty), unsorted_(that.unsorted_) {}

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
      applyBooleanOr(data_);
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

};

}
#endif

