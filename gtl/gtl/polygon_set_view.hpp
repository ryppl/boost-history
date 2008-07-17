/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
#ifndef GTL_POLYGON_SET_VIEW_HPP
#define GTL_POLYGON_SET_VIEW_HPP
namespace gtl {
  struct operator_provides_storage {};
  struct operator_requires_copy {};


template <typename value_type, typename arg_type>
inline void insert_into_view_arg(value_type& dest, const arg_type& arg, orientation_2d orient) {
  typedef typename polygon_set_traits<arg_type>::iterator_type literator;
  literator itr1, itr2;
  itr1 = polygon_set_traits<arg_type>::begin(arg);
  itr2 = polygon_set_traits<arg_type>::end(arg);
  if(polygon_set_traits<arg_type>::orient(arg) == orient) {
    for( ; itr1 != itr2; ++itr1) {
      dest.push_back(*itr1);
    }
    if(!polygon_set_traits<arg_type>::sorted(arg)) std::sort(dest.begin(), dest.end());
  } else {
    iterator_vertex_orient_conversion<literator> citr2(itr2);
    for(iterator_vertex_orient_conversion<literator> citr1(itr1); citr1 != citr2; ++citr1) {
      dest.push_back(*citr1);
    }
    std::sort(dest.begin(), dest.end());
  }
}


template <typename ltype, typename rtype, typename op_type, typename ltag, typename rtag>
class polygon_set_view {
public:
  typedef typename polygon_set_traits<ltype>::coordinate_type coordinate_type;
  typedef std::vector<std::pair<coordinate_type, std::pair<coordinate_type, int> > > value_type;
  typedef typename value_type::const_iterator iterator_type;
  typedef polygon_set_view operator_arg_type;
  typedef operator_provides_storage operator_storage_tag;
private:
  const ltype& lvalue_;
  const rtype& rvalue_;
  op_type op_;
  orientation_2d orient_;
  mutable value_type output_;
public:
  polygon_set_view(const ltype& lvalue,
                   const rtype& rvalue,
                   orientation_2d orient,
                   op_type op) :
    lvalue_(lvalue), rvalue_(rvalue), orient_(orient), op_(op) {}

  /// get iterator to begin vertex data
  const value_type& value() const {
    if(output_.empty()) {
      value_type linput_;
      value_type rinput_;
      insert_into_view_arg(linput_, lvalue_, orient_);
      insert_into_view_arg(rinput_, rvalue_, orient_);
      boolean_op::applyBooleanBinaryOp(output_, linput_, rinput_, boolean_op::BinaryCount<op_type>()); 
    }
    return output_;
  }

  iterator_type begin() const { return value().begin(); }
  iterator_type end() const { return value().begin(); }

  orientation_2d orient() const { return orient_; }
  bool dirty() const { return false; } //result of a boolean is clean
  bool sorted() const { return true; } //result of a boolean is sorted

  //insert is not inteded to be called because a view is read only
  void insert(const value_type& value,
              orientation_2d orient) const {
    //throw a helpful exception
  }
  void sort() const {} //is always sorted
};

template <typename ltype, typename rtype, typename op_type>
class polygon_set_view<ltype, rtype, op_type, operator_provides_storage, operator_provides_storage> {
public:
  typedef typename polygon_set_traits<ltype>::coordinate_type coordinate_type;
  typedef std::vector<std::pair<coordinate_type, std::pair<coordinate_type, int> > > value_type;
  typedef typename value_type::const_iterator iterator_type;
  typedef polygon_set_view operator_arg_type;
  typedef operator_provides_storage operator_storage_tag;
private:
  const ltype& lvalue_;
  const rtype& rvalue_;
  op_type op_;
  orientation_2d orient_;
  mutable value_type output_;
public:
  polygon_set_view(const ltype& lvalue,
                   const rtype& rvalue,
                   orientation_2d orient,
                   op_type op) :
    lvalue_(lvalue), rvalue_(rvalue), op_(op), orient_(orient) {}

  /// get iterator to begin vertex data
  const value_type& value() const {
    if(output_.empty()) {
      value_type linput_;
      value_type rinput_;
      if(orient_ != lvalue_.orient()) {
        insert_into_view_arg(linput_, lvalue_, orient_);
        if(orient_ != rvalue_.orient()) {
          insert_into_view_arg(rinput_, rvalue_, orient_);
          boolean_op::applyBooleanBinaryOp(output_, linput_, rinput_, boolean_op::BinaryCount<op_type>()); 
        } else {
          boolean_op::applyBooleanBinaryOp(output_, linput_, rvalue_.value(), boolean_op::BinaryCount<op_type>()); 
        }
      } else {
        if(!lvalue_.sorted()) lvalue_.sort();
        if(orient_ != rvalue_.orient()) {
          insert_into_view_arg(rinput_, rvalue_, orient_);
          boolean_op::applyBooleanBinaryOp(output_, lvalue_.value(), rinput_, boolean_op::BinaryCount<op_type>()); 
        } else {
          if(!rvalue_.sorted()) rvalue_.sort();
          boolean_op::applyBooleanBinaryOp(output_, lvalue_.value(), rvalue_.value(), boolean_op::BinaryCount<op_type>()); 
        }
      }
    }
    return output_;
  }

  iterator_type begin() const { return value().begin(); }
  iterator_type end() const { return value().begin(); }
  orientation_2d orient() const { return orient_; }
  bool dirty() const { return false; } //result of a boolean is clean
  bool sorted() const { return true; } //result of a boolean is sorted

  //insert is not inteded to be called because a view is read only
  void insert(const value_type& value,
              orientation_2d orient) const {
    //throw a helpful exception
  }
  void sort() const {} //is always sorted
};

template <typename ltype, typename rtype, typename op_type, typename rtag>
class polygon_set_view<ltype, rtype, op_type, operator_provides_storage, rtag> {
public:
  typedef typename polygon_set_traits<ltype>::coordinate_type coordinate_type;
  typedef std::vector<std::pair<coordinate_type, std::pair<coordinate_type, int> > > value_type;
  typedef typename value_type::const_iterator iterator_type;
  typedef polygon_set_view operator_arg_type;
  typedef operator_provides_storage operator_storage_tag;
private:
  const ltype& lvalue_;
  const rtype& rvalue_;
  op_type op_;
  orientation_2d orient_;
  mutable value_type output_;
public:
  polygon_set_view(const ltype& lvalue,
                   const rtype& rvalue,
                   orientation_2d orient,
                   op_type op) :
    lvalue_(lvalue), rvalue_(rvalue), op_(op) , orient_(orient) {}

  /// get iterator to begin vertex data
  const value_type& value() const {
    if(output_.empty()) {
      value_type linput_;
      value_type rinput_;
      insert_into_view_arg(rinput_, rvalue_, orient_);
      if(orient_ != lvalue_.orient()) {
        insert_into_view_arg(linput_, lvalue_, orient_);
        boolean_op::applyBooleanBinaryOp(output_, linput_, rinput_, boolean_op::BinaryCount<op_type>()); 
      } else {
        if(!lvalue_.sorted()) lvalue_.sort();
        boolean_op::applyBooleanBinaryOp(output_, lvalue_.value(), rinput_, boolean_op::BinaryCount<op_type>()); 
      }
    }
    return output_;
  }

  iterator_type begin() const { return value().begin(); }
  iterator_type end() const { return value().begin(); }
  orientation_2d orient() const { return orient_; }
  bool dirty() const { return false; } //result of a boolean is clean
  bool sorted() const { return true; } //result of a boolean is sorted

  //insert is not inteded to be called because a view is read only
  void insert(const value_type& value,
              orientation_2d orient) const {
    //throw a helpful exception
  }
  void sort() const {} //is always sorted
};

template <typename ltype, typename rtype, typename op_type, typename ltag>
class polygon_set_view<ltype, rtype, op_type, ltag, operator_provides_storage> {
public:
  typedef typename polygon_set_traits<ltype>::coordinate_type coordinate_type;
  typedef std::vector<std::pair<coordinate_type, std::pair<coordinate_type, int> > > value_type;
  typedef typename value_type::const_iterator iterator_type;
  typedef polygon_set_view operator_arg_type;
private:
  const ltype& lvalue_;
  const rtype& rvalue_;
  op_type op_;
  orientation_2d orient_;
  mutable value_type output_;
  mutable value_type linput_;
public:
  polygon_set_view(const ltype& lvalue,
                   const rtype& rvalue,
                   orientation_2d orient,
                   op_type op) :
    lvalue_(lvalue), rvalue_(rvalue), op_(op), orient_(orient) {}

  /// get iterator to begin vertex data
  const value_type& value() const {
    if(output_.empty()) {
      value_type linput_;
      value_type rinput_;
      insert_into_view_arg(linput_, lvalue_, orient_);
      if(orient_ != lvalue_.orient()) {
        insert_into_view_arg(rinput_, rvalue_, orient_);
        boolean_op::applyBooleanBinaryOp(output_, linput_, rinput_, boolean_op::BinaryCount<op_type>()); 
      } else {
        if(!rvalue_.sorted()) rvalue_.sort();
        boolean_op::applyBooleanBinaryOp(output_, linput_, rvalue_.value(), boolean_op::BinaryCount<op_type>()); 
      }
    }
    return output_;
  }

  iterator_type begin() const { return value().begin(); }
  iterator_type end() const { return value().begin(); }
  orientation_2d orient() const { return orient_; }
  bool dirty() const { return false; } //result of a boolean is clean
  bool sorted() const { return true; } //result of a boolean is sorted

  //insert is not inteded to be called because a view is read only
  void insert(const value_type& value,
              orientation_2d orient) const {
    //throw a helpful exception
  }
  void sort() const {} //is always sorted
};

}
#endif

