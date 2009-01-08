/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
#ifndef GTL_POLYGON_SET_VIEW_HPP
#define GTL_POLYGON_SET_VIEW_HPP
namespace gtl {
  
  template <typename coordinate_type>
  void polygon_set_data<coordinate_type>::clean() const {
    if(dirty_) {
      polygon_45_set_data<coordinate_type> tmp;
      if(downcast(tmp) ) {
        tmp.clean();
        data_.clear();
        is_45_ = true;
        polygon_set_data<coordinate_type> tmp2;
        tmp2.insert(tmp);
        data_.swap(tmp2.data_);
        dirty_ = false;
        sort();
      } else {
        sort();
        arbitrary_boolean_op<coordinate_type> abo;
        polygon_set_data<coordinate_type> tmp;
        abo.execute(tmp, begin(), end(), end(), end(), 0);
        data_.swap(tmp.data_);
        is_45_ = tmp.is_45_;
        dirty_ = false;
      }
    }
  }

  template <typename value_type, typename arg_type>
  inline void insert_into_view_arg(value_type& dest, const arg_type& arg);

  template <typename ltype, typename rtype, int op_type>
  class polygon_set_view;

  template <typename ltype, typename rtype, int op_type>
  struct polygon_set_traits<polygon_set_view<ltype, rtype, op_type> > {
    typedef typename polygon_set_view<ltype, rtype, op_type>::coordinate_type coordinate_type;
    typedef typename polygon_set_view<ltype, rtype, op_type>::iterator_type iterator_type;
    typedef typename polygon_set_view<ltype, rtype, op_type>::operator_arg_type operator_arg_type;

    static inline iterator_type begin(const polygon_set_view<ltype, rtype, op_type>& polygon_set); 
    static inline iterator_type end(const polygon_set_view<ltype, rtype, op_type>& polygon_set);

    static inline bool clean(const polygon_set_view<ltype, rtype, op_type>& polygon_set);

    static inline bool sort(const polygon_set_view<ltype, rtype, op_type>& polygon_set);
  };

  template <typename value_type, typename geometry_type_1, typename geometry_type_2>
  void execute_boolean_op(value_type& output_, const geometry_type_1& lvalue_, const geometry_type_2& rvalue_, int op_type) {
    typedef geometry_type_1 ltype;
    typedef geometry_type_2 rtype;
    typedef typename polygon_set_traits<ltype>::coordinate_type coordinate_type;
    value_type linput_;
    value_type rinput_;
    insert_into_view_arg(linput_, lvalue_);
    insert_into_view_arg(rinput_, rvalue_);
    polygon_45_set_data<coordinate_type> l45, r45, o45;
    if(linput_.downcast(l45) && rinput_.downcast(r45)) {
      l45.applyAdaptiveBoolean_(o45, op_type, r45);
      output_.insert(o45);
    } else {
      arbitrary_boolean_op<coordinate_type> abo;
      abo.execute(output_, linput_.begin(), linput_.end(),
                  rinput_.begin(), rinput_.end(), op_type);
    }
  }

  template <typename ltype, typename rtype, int op_type>
  class polygon_set_view {
  public:
    typedef typename polygon_set_traits<ltype>::coordinate_type coordinate_type;
    typedef polygon_set_data<coordinate_type> value_type;
    typedef typename value_type::iterator_type iterator_type;
    typedef polygon_set_view operator_arg_type;
  private:
    const ltype& lvalue_;
    const rtype& rvalue_;
    mutable value_type output_;
    mutable bool evaluated_;
  public:
    polygon_set_view(const ltype& lvalue,
                     const rtype& rvalue ) :
      lvalue_(lvalue), rvalue_(rvalue), evaluated_(false) {}

    /// get iterator to begin vertex data
  public:
    const value_type& value() const {
      if(!evaluated_) {
        evaluated_ = true;
        execute_boolean_op(output_, lvalue_, rvalue_, op_type);
      }
      return output_;
    }
  public:
    iterator_type begin() const { return value().begin(); }
    iterator_type end() const { return value().end(); }

    bool dirty() const { return false; } //result of a boolean is clean
    bool sorted() const { return true; } //result of a boolean is sorted

    void sort() const {} //is always sorted
  };

  template <typename ltype, typename rtype, int op_type>
  typename polygon_set_view<ltype, rtype, op_type>::iterator_type 
  polygon_set_traits<polygon_set_view<ltype, rtype, op_type> >::
  begin(const polygon_set_view<ltype, rtype, op_type>& polygon_set) {
    return polygon_set.begin();
  }
  template <typename ltype, typename rtype, int op_type>
  typename polygon_set_view<ltype, rtype, op_type>::iterator_type 
  polygon_set_traits<polygon_set_view<ltype, rtype, op_type> >::
  end(const polygon_set_view<ltype, rtype, op_type>& polygon_set) {
    return polygon_set.end();
  }
  template <typename ltype, typename rtype, int op_type>
  bool polygon_set_traits<polygon_set_view<ltype, rtype, op_type> >::
  clean(const polygon_set_view<ltype, rtype, op_type>& polygon_set) { 
    return true; }
  template <typename ltype, typename rtype, int op_type>
  bool polygon_set_traits<polygon_set_view<ltype, rtype, op_type> >::
  sort(const polygon_set_view<ltype, rtype, op_type>& polygon_set) { 
    return true; }

  template <typename value_type, typename arg_type>
  inline void insert_into_view_arg(value_type& dest, const arg_type& arg) {
    typedef typename polygon_set_traits<arg_type>::iterator_type literator;
    literator itr1, itr2;
    itr1 = polygon_set_traits<arg_type>::begin(arg);
    itr2 = polygon_set_traits<arg_type>::end(arg);
    dest.insert(itr1, itr2);
  }
  
  template <typename geometry_type_1, typename geometry_type_2, int op_type>
  geometry_type_1& self_assignment_boolean_op(geometry_type_1& lvalue_, const geometry_type_2& rvalue_) {
    typedef geometry_type_1 ltype;
    typedef typename polygon_set_traits<ltype>::coordinate_type coordinate_type;
    typedef polygon_set_data<coordinate_type> value_type;
    value_type output_;
    execute_boolean_op(output_, lvalue_, rvalue_, op_type);
    polygon_set_mutable_traits<geometry_type_1>::set(lvalue_, output_.begin(), output_.end());
    return lvalue_;
  }

  template <typename ltype, typename rtype, int op_type>
  struct geometry_concept<polygon_set_view<ltype, rtype, op_type> > { typedef polygon_set_concept type; };

}
#endif

