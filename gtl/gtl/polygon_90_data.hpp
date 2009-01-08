/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
#ifndef GTL_POLYGON_90_DATA_HPP
#define GTL_POLYGON_90_DATA_HPP
namespace gtl {
struct polygon_90_concept;
template <typename T>
class polygon_90_data {
public:
  typedef polygon_90_concept geometry_type;
  typedef T coordinate_type;
  typedef typename std::vector<coordinate_type>::const_iterator compact_iterator_type;
  typedef iterator_compact_to_points<compact_iterator_type, point_data<coordinate_type> > iterator_type;
  typedef typename coordinate_traits<T>::area_type area_type;

  inline polygon_90_data(){;} //do nothing default constructor

  /// initialize a polygon from x,y values, it is assumed that the first is an x
  /// and that the input is a well behaved polygon
  template<class iT>
  inline polygon_90_data& set(iT begin_point, iT end_point) {
    return set_compact(iterator_points_to_compact<iT, typename std::iterator_traits<iT>::value_type>(begin_point),
        iterator_points_to_compact<iT, typename std::iterator_traits<iT>::value_type>(end_point));
  }

  template<class iT>
  inline polygon_90_data& set_compact(iT input_begin, iT input_end) {
    coords_.clear();  //just in case there was some old data there
    while(input_begin != input_end) {
       coords_.insert(coords_.end(), *input_begin);
       ++input_begin;
    }
    return *this;
  }

  /// copy constructor (since we have dynamic memory)
  inline polygon_90_data(const polygon_90_data& that) : coords_(that.coords_) {}
  
  /// assignment operator (since we have dynamic memory do a deep copy)
  inline polygon_90_data& operator=(const polygon_90_data& that) {
    coords_ = that.coords_;
    return *this;
  }

  template <typename T2>
  inline polygon_90_data& operator=(const T2& rvalue);

  /// assignment operator (since we have dynamic memory do a deep copy)
  inline bool operator==(const polygon_90_data& that) const {
    return coords_ == that.coords_;
  }

  /// get begin iterator, returns a pointer to a const Unit
  inline iterator_type begin() const { return iterator_type(coords_.begin(), coords_.end()); }

  /// get end iterator, returns a pointer to a const Unit
  inline iterator_type end() const { return iterator_type(coords_.end(), coords_.end()); }

  /// get begin iterator, returns a pointer to a const Unit
  inline compact_iterator_type begin_compact() const { return coords_.begin(); }
  
  /// get end iterator, returns a pointer to a const Unit
  inline compact_iterator_type end_compact() const { return coords_.end(); }

  inline std::size_t size() const { return coords_.size(); }
  
  inline void swap(polygon_90_data& that) {
    int INSTANTIATED = 0;
    coords_.swap(that.coords_);
  }
 
private:
  std::vector<coordinate_type> coords_; 
};

template <typename T>
std::ostream& operator << (std::ostream& o, const polygon_90_data<T>& r)
{
  o << "Polygon { ";
  for(typename polygon_90_data<T>::iterator_type itr = r.begin(); itr != r.end(); ++itr) {
    o << *itr << ", ";
  }
  return o << "} ";
}

template <typename T>
std::istream& operator >> (std::istream& i, polygon_90_data<T>& r)
{
  unsigned int size;
  i >> size; 
  std::vector<T> vec;
  vec.reserve(size);
  for(unsigned int ii = 0; ii < size; ++ii) {
    T coord;
    i >> coord;
    vec.push_back(coord);
  }
  r.set_compact(vec.begin(), vec.end());
  return i;
}
  
template <typename T>
std::ostream& operator << (std::ostream& o, const std::vector<polygon_90_data<T> >& r) {
  o << r.size() << ' ';
  for(unsigned int ii = 0; ii < r.size(); ++ii) {
    o << (r[ii]); 
  }
  return o;
}
template <typename T>
std::istream& operator >> (std::istream& i, std::vector<polygon_90_data<T> >& r) {
  unsigned int size;
  i >> size;
  r.clear();
  r.reserve(size);
  for(unsigned int ii = 0; ii < size; ++ii) {
    polygon_90_data<T> tmp;
    i >> tmp;
    r.push_back(tmp);
  }
  return i;
}

}

namespace std {
template <typename T>
void swap(gtl::polygon_90_data<T>& l, gtl::polygon_90_data<T>& r) {
  l.swap(r);
}

}
#endif

