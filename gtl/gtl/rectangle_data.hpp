/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
#ifndef GTL_RECTANGLE_DATA_HPP
#define GTL_RECTANGLE_DATA_HPP
namespace gtl {

struct rectangle_concept;

template <typename T>
class rectangle_data {
public:
  typedef rectangle_concept geometry_type;
  typedef T coordinate_type;
  inline rectangle_data() {}
  template <typename interval_type_1, typename interval_type_2>
  inline rectangle_data(const interval_type_1& hrange,
                        const interval_type_2& vrange) {
    set(HORIZONTAL, hrange); set(VERTICAL, vrange); }

  inline rectangle_data(const rectangle_data& that) { (*this) = that; }

  inline rectangle_data& operator=(const rectangle_data& that) {
    ranges_[0] = that.ranges_[0]; ranges_[1] = that.ranges_[1]; return *this;
  }
  template <typename T2>
  inline rectangle_data& operator=(const T2& rvalue);

  template <typename T2>
  inline bool operator==(const T2& rvalue) const;
  template <typename T2>
  inline bool operator!=(const T2& rvalue) const { return !((*this) == rvalue); }

  inline interval_data<coordinate_type> get(orientation_2d orient) const {
    return ranges_[orient.to_int()]; }
  inline coordinate_type get(direction_2d dir) const {
    return ranges_[orientation_2d(dir).to_int()].get(direction_1d(dir));
  }
  inline void set(direction_2d dir, coordinate_type value) {
    return ranges_[orientation_2d(dir).to_int()].set(direction_1d(dir), value);
  }
  template <typename interval_type>
  inline void set(orientation_2d orient, const interval_type& interval); 
private:
  interval_data<coordinate_type> ranges_[2]; 
};

template <class T>
std::ostream& operator << (std::ostream& o, const rectangle_data<T>& r)
{
  return o << r.get(HORIZONTAL) << ' ' << r.get(VERTICAL);
}

}
#endif

