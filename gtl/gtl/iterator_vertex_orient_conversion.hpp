/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
template <typename iterator_type>
class iterator_vertex_orient_conversion {
private:
  iterator_type iter_;
  typename iterator_type::value_type vertex_;
public:
  typedef std::forward_iterator_tag iterator_category;
  typedef typename iterator_type::value_type value_type;
  typedef std::ptrdiff_t difference_type;
  typedef const value_type* pointer; //immutable
  typedef const value_type& reference; //immutable
  typedef typename value_type::first_type coordinate_type;

  inline iterator_vertex_orient_conversion() {}
  inline iterator_vertex_orient_conversion(iterator_type iter) : 
    iter_(iter) {}
  inline iterator_vertex_orient_conversion& operator++() {
    ++iter_;
    return *this;
  }
  inline iterator_vertex_orient_conversion operator++(int) {
    iterator_vertex_orient_conversion tmp(*this);
    ++(*this);
    return tmp;
  }
  inline bool operator==(const iterator_vertex_orient_conversion& that) const {
    return (iter_ == that.iter_);
  }
  inline bool operator!=(const iterator_vertex_orient_conversion& that) const {
    return (iter_ != that.iter_);
  }
  inline reference operator*() const { 
    vertex_.first = (*iter_).second.first;
    vertex_.second = std::pair<coordinate_type, int>((*iter_).first, (*iter_).second.second); 
    return vertex_; 
  }
};

