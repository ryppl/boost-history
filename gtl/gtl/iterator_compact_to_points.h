/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
template <class iterator_type, class point_type>
class iterator_compact_to_points {
private:
  iterator_type iter_;
  iterator_type iter_end_;
  point_type pt_;
  typename point_traits<point_type>::coordinate_type firstX_;
  orientation_2d orient_;
public:
  inline iterator_compact_to_points() {}
  inline iterator_compact_to_points(iterator_type iter, iterator_type iter_end) : 
    iter_(iter), iter_end_(iter_end), orient_(HORIZONTAL) {
    if(iter_ != iter_end_) {
      firstX_ = *iter_;
      point_concept::set<HORIZONTAL>(pt_, firstX_);
      ++iter_;
      if(iter_ != iter_end_) {
        point_concept::set<VERTICAL>(pt_, *iter_);
      }
    }
  }
  //use bitwise copy and assign provided by the compiler
  inline iterator_compact_to_points& operator++() {
    ++iter_;
    if(iter_ == iter_end_) {
      if(point_concept::get<HORIZONTAL>(pt_) != firstX_) {
        --iter_;
        point_concept::set<HORIZONTAL>(pt_, firstX_);
      }
    } else {
      point_concept::set(pt_, orient_, *iter_);
      orient_.turn_90();
    }
    return *this;
  }
  inline iterator_compact_to_points operator++(int) {
    iterator_compact_to_points tmp(*this);
    ++(*this);
    return tmp;
  }
  inline bool operator==(const iterator_compact_to_points& that) const {
    return (iter_ == that.iter_);
  }
  inline bool operator!=(const iterator_compact_to_points& that) const {
    return (iter_ != that.iter_);
  }
  inline const point_type& operator*() const { return pt_; }
};
