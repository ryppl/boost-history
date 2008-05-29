/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
template <class iT, typename point_type>
class iterator_points_to_compact {
private:
  iT iter_;
  orientation_2d orient_;
public:
  typedef typename point_traits<point_type>::coordinate_type coordinate_type;
  typedef std::forward_iterator_tag iterator_category;
  typedef coordinate_type value_type;
  typedef std::ptrdiff_t difference_type;
  typedef const coordinate_type* pointer; //immutable
  typedef const coordinate_type& reference; //immutable

  inline iterator_points_to_compact() {}
  inline iterator_points_to_compact(iT iter) : iter_(iter), orient_(HORIZONTAL) {}
  inline iterator_points_to_compact(const iterator_points_to_compact& that) : 
    iter_(that.iter_), orient_(that.orient_) {}
  //use bitwise copy and assign provided by the compiler
  inline iterator_points_to_compact& operator++() {
    ++iter_;
    orient_.turn_90();
    return *this;
  }
  inline iterator_points_to_compact operator++(int) {
    iT tmp(*this);
    ++(*this);
    return tmp;
  }
  inline bool operator==(const iterator_points_to_compact& that) const {
    return (iter_ == that.iter_);
  }
  inline bool operator!=(const iterator_points_to_compact& that) const {
    return (iter_ != that.iter_);
  }
  inline reference operator*() const { return point_concept::get(*iter_, orient_); }
};
