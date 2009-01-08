/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
#ifndef GTL_POLYGON_90_SET_VIEW_HPP
#define GTL_POLYGON_90_SET_VIEW_HPP
namespace gtl {
  struct operator_provides_storage {};
  struct operator_requires_copy {};

  template <typename value_type, typename arg_type>
  inline void insert_into_view_arg(value_type& dest, const arg_type& arg, orientation_2d orient);

  template <typename ltype, typename rtype, typename op_type>
  class polygon_90_set_view;

  template <typename ltype, typename rtype, typename op_type>
  struct polygon_90_set_traits<polygon_90_set_view<ltype, rtype, op_type> > {
    typedef typename polygon_90_set_view<ltype, rtype, op_type>::coordinate_type coordinate_type;
    typedef typename polygon_90_set_view<ltype, rtype, op_type>::iterator_type iterator_type;
    typedef typename polygon_90_set_view<ltype, rtype, op_type>::operator_arg_type operator_arg_type;

    static inline iterator_type begin(const polygon_90_set_view<ltype, rtype, op_type>& polygon_set); 
    static inline iterator_type end(const polygon_90_set_view<ltype, rtype, op_type>& polygon_set);

    static inline orientation_2d orient(const polygon_90_set_view<ltype, rtype, op_type>& polygon_set);

    static inline bool clean(const polygon_90_set_view<ltype, rtype, op_type>& polygon_set);

    static inline bool sorted(const polygon_90_set_view<ltype, rtype, op_type>& polygon_set);
  };

  template <typename ltype, typename rtype, typename op_type>
  class polygon_90_set_view {
  public:
    typedef typename polygon_90_set_traits<ltype>::coordinate_type coordinate_type;
    typedef polygon_90_set_data<coordinate_type> value_type;
    typedef typename value_type::iterator_type iterator_type;
    typedef polygon_90_set_view operator_arg_type;
  private:
    const ltype& lvalue_;
    const rtype& rvalue_;
    orientation_2d orient_;
    op_type op_;
    mutable value_type output_;
    mutable bool evaluated_;
  public:
    polygon_90_set_view(const ltype& lvalue,
                     const rtype& rvalue,
                     orientation_2d orient,
                     op_type op) :
      lvalue_(lvalue), rvalue_(rvalue), orient_(orient), op_(op), output_(orient), evaluated_(false) {}

    /// get iterator to begin vertex data
  private:
    const value_type& value() const {
      if(!evaluated_) {
        evaluated_ = true;
        value_type linput_(orient_);
        value_type rinput_(orient_);
        insert_into_view_arg(linput_, lvalue_, orient_);
        insert_into_view_arg(rinput_, rvalue_, orient_);
        output_.applyBooleanBinaryOp(linput_.begin(), linput_.end(),
                                     rinput_.begin(), rinput_.end(), boolean_op::BinaryCount<op_type>()); 
      }
      return output_;
    }
  public:
    iterator_type begin() const { return value().begin(); }
    iterator_type end() const { return value().end(); }

    orientation_2d orient() const { return orient_; }
    bool dirty() const { return false; } //result of a boolean is clean
    bool sorted() const { return true; } //result of a boolean is sorted

//     template <typename input_iterator_type>
//     void set(input_iterator_type input_begin, input_iterator_type input_end, 
//              orientation_2d orient) const {
//       orient_ = orient;
//       output_.clear();
//       output_.insert(output_.end(), input_begin, input_end);
//       std::sort(output_.begin(), output_.end());
//     }
    void sort() const {} //is always sorted
  };

  template <typename ltype, typename rtype, typename op_type>
  struct geometry_concept<polygon_90_set_view<ltype, rtype, op_type> > {
    typedef polygon_90_set_concept type;
  };

  template <typename ltype, typename rtype, typename op_type>
  typename polygon_90_set_view<ltype, rtype, op_type>::iterator_type 
  polygon_90_set_traits<polygon_90_set_view<ltype, rtype, op_type> >::
  begin(const polygon_90_set_view<ltype, rtype, op_type>& polygon_set) {
    return polygon_set.begin();
  }
  template <typename ltype, typename rtype, typename op_type>
  typename polygon_90_set_view<ltype, rtype, op_type>::iterator_type 
  polygon_90_set_traits<polygon_90_set_view<ltype, rtype, op_type> >::
  end(const polygon_90_set_view<ltype, rtype, op_type>& polygon_set) {
    return polygon_set.end();
  }
//   template <typename ltype, typename rtype, typename op_type>
//   template <typename input_iterator_type>
//   void polygon_90_set_traits<polygon_90_set_view<ltype, rtype, op_type> >::
//   set(polygon_90_set_view<ltype, rtype, op_type>& polygon_set, 
//       input_iterator_type input_begin, input_iterator_type input_end,
//       orientation_2d orient) {
//     polygon_set.set(input_begin, input_end, orient);
//   }
  template <typename ltype, typename rtype, typename op_type>
  orientation_2d polygon_90_set_traits<polygon_90_set_view<ltype, rtype, op_type> >::
  orient(const polygon_90_set_view<ltype, rtype, op_type>& polygon_set) { 
    return polygon_set.orient(); }
  template <typename ltype, typename rtype, typename op_type>
  bool polygon_90_set_traits<polygon_90_set_view<ltype, rtype, op_type> >::
  clean(const polygon_90_set_view<ltype, rtype, op_type>& polygon_set) { 
    return true; }
  template <typename ltype, typename rtype, typename op_type>
  bool polygon_90_set_traits<polygon_90_set_view<ltype, rtype, op_type> >::
  sorted(const polygon_90_set_view<ltype, rtype, op_type>& polygon_set) { 
    return true; }

  template <typename value_type, typename arg_type>
  inline void insert_into_view_arg(value_type& dest, const arg_type& arg, orientation_2d orient) {
    typedef typename polygon_90_set_traits<arg_type>::iterator_type literator;
    literator itr1, itr2;
    itr1 = polygon_90_set_traits<arg_type>::begin(arg);
    itr2 = polygon_90_set_traits<arg_type>::end(arg);
    dest.insert(itr1, itr2, orient);
    dest.sort();
  }
  
  template <typename T>
  template <typename ltype, typename rtype, typename op_type>
  inline polygon_90_set_data<T>& polygon_90_set_data<T>::operator=(const polygon_90_set_view<ltype, rtype, op_type>& that) {
    set(that.begin(), that.end(), that.orient());
    dirty_ = false;
    unsorted_ = false;
    return *this;
  }
  
  template <typename geometry_type_1, typename geometry_type_2>
  struct self_assign_operator_lvalue {
    typedef geometry_type_1& type;
  };
    
  template <typename type_1, typename type_2>
  struct by_value_binary_operator {
    typedef type_1 type;
  };
    
  template <typename geometry_type_1, typename geometry_type_2, typename op_type>
  geometry_type_1& self_assignment_boolean_op(geometry_type_1& lvalue_, const geometry_type_2& rvalue_) {
    typedef geometry_type_1 ltype;
    typedef geometry_type_2 rtype;
    typedef typename polygon_90_set_traits<ltype>::coordinate_type coordinate_type;
    typedef polygon_90_set_data<coordinate_type> value_type;
    orientation_2d orient_ = polygon_90_set_traits<ltype>::orient(lvalue_);
    value_type linput_(orient_);
    value_type rinput_(orient_);
    value_type output_(orient_);
    insert_into_view_arg(linput_, lvalue_, orient_);
    insert_into_view_arg(rinput_, rvalue_, orient_);
    output_.applyBooleanBinaryOp(linput_.begin(), linput_.end(),
                                 rinput_.begin(), rinput_.end(), boolean_op::BinaryCount<op_type>()); 
    polygon_90_set_mutable_traits<geometry_type_1>::set(lvalue_, output_.begin(), output_.end(), orient_);
    return lvalue_;
  }
  
  template <typename geometry_type_1, typename geometry_type_2>
  typename requires_2< typename gtl_if<typename is_polygon_90_set_type<geometry_type_1>::type>::type,
                       typename gtl_if<typename is_polygon_90_set_type<geometry_type_2>::type>::type,
                       polygon_90_set_view<geometry_type_1, geometry_type_2, boolean_op::BinaryOr> >::type
  operator|(const geometry_type_1& lvalue, const geometry_type_2& rvalue) {
    return polygon_90_set_view<geometry_type_1, geometry_type_2, boolean_op::BinaryOr> 
      (lvalue, rvalue, 
       polygon_90_set_traits<geometry_type_1>::orient(lvalue),
       boolean_op::BinaryOr());
  }
  
  template <typename geometry_type_1, typename geometry_type_2>
  typename requires_2< typename gtl_if<typename is_polygon_90_set_type<geometry_type_1>::type>::type,
                       typename gtl_if<typename is_polygon_90_set_type<geometry_type_2>::type>::type,
                       polygon_90_set_view<geometry_type_1, geometry_type_2, boolean_op::BinaryOr> >::type
  operator+(const geometry_type_1& lvalue, const geometry_type_2& rvalue) {
    return polygon_90_set_view<geometry_type_1, geometry_type_2, boolean_op::BinaryOr> 
      (lvalue, rvalue, 
       polygon_90_set_traits<geometry_type_1>::orient(lvalue),
       boolean_op::BinaryOr());
  }
  
  template <typename geometry_type_1, typename geometry_type_2>
  typename requires_2< typename gtl_if<typename is_polygon_90_set_type<geometry_type_1>::type>::type, 
                       typename gtl_if<typename is_polygon_90_set_type<geometry_type_2>::type>::type, 
                       polygon_90_set_view<geometry_type_1, geometry_type_2, boolean_op::BinaryAnd> >::type
  operator*(const geometry_type_1& lvalue, const geometry_type_2& rvalue) {
    return polygon_90_set_view<geometry_type_1, geometry_type_2, boolean_op::BinaryAnd> 
      (lvalue, rvalue, 
       polygon_90_set_traits<geometry_type_1>::orient(lvalue),
       boolean_op::BinaryAnd());
  }
  
  template <typename geometry_type_1, typename geometry_type_2>
  typename requires_2< typename gtl_if<typename is_polygon_90_set_type<geometry_type_1>::type>::type, 
                       typename gtl_if<typename is_polygon_90_set_type<geometry_type_2>::type>::type, 
                       polygon_90_set_view<geometry_type_1, geometry_type_2, boolean_op::BinaryAnd> >::type
  operator&(const geometry_type_1& lvalue, const geometry_type_2& rvalue) {
    return polygon_90_set_view<geometry_type_1, geometry_type_2, boolean_op::BinaryAnd> 
      (lvalue, rvalue, 
       polygon_90_set_traits<geometry_type_1>::orient(lvalue),
       boolean_op::BinaryAnd());
  }

  template <typename geometry_type_1, typename geometry_type_2>
  typename requires_2< typename gtl_if<typename is_polygon_90_set_type<geometry_type_1>::type>::type, 
                       typename gtl_if<typename is_polygon_90_set_type<geometry_type_2>::type>::type, 
                       polygon_90_set_view<geometry_type_1, geometry_type_2, boolean_op::BinaryXor> >::type
  operator^(const geometry_type_1& lvalue, const geometry_type_2& rvalue) {
    return polygon_90_set_view<geometry_type_1, geometry_type_2, boolean_op::BinaryXor> 
      (lvalue, rvalue, 
       polygon_90_set_traits<geometry_type_1>::orient(lvalue),
       boolean_op::BinaryXor());
  }
  
  template <typename geometry_type_1, typename geometry_type_2>
  typename requires_2< typename gtl_if<typename is_polygon_90_set_type<geometry_type_1>::type>::type, 
                       typename gtl_if<typename is_polygon_90_set_type<geometry_type_2>::type>::type, 
                       polygon_90_set_view<geometry_type_1, geometry_type_2, boolean_op::BinaryNot> >::type
  operator-(const geometry_type_1& lvalue, const geometry_type_2& rvalue) {
    return polygon_90_set_view<geometry_type_1, geometry_type_2, boolean_op::BinaryNot> 
      (lvalue, rvalue, 
       polygon_90_set_traits<geometry_type_1>::orient(lvalue),
       boolean_op::BinaryNot());
  }
  
  template <typename coordinate_type_1, typename geometry_type_2>
  typename requires_1< typename gtl_if<typename is_polygon_90_set_type<geometry_type_2>::type>::type,
                       polygon_90_set_data<coordinate_type_1> >::type &
  operator+=(polygon_90_set_data<coordinate_type_1>& lvalue, const geometry_type_2& rvalue) {
    lvalue.insert(polygon_90_set_traits<geometry_type_2>::begin(rvalue), polygon_90_set_traits<geometry_type_2>::end(rvalue),
                  polygon_90_set_traits<geometry_type_2>::orient(rvalue));
    return lvalue;
  }
  
  //
  template <typename coordinate_type_1, typename geometry_type_2>
  typename requires_1< typename gtl_if<typename is_polygon_90_set_type<geometry_type_2>::type>::type, 
                       polygon_90_set_data<coordinate_type_1> >::type &
  operator|=(polygon_90_set_data<coordinate_type_1>& lvalue, const geometry_type_2& rvalue) {
    return lvalue += rvalue;
  }

  //normal self assignment boolean operations
  template <typename geometry_type_1, typename geometry_type_2>
  typename requires_2< typename gtl_if<typename is_mutable_polygon_90_set_type<geometry_type_1>::type>::type, 
                       typename gtl_if<typename is_polygon_90_set_type<geometry_type_2>::type>::type, 
                       geometry_type_1>::type &
  operator+=(geometry_type_1& lvalue, const geometry_type_2& rvalue) {
    return self_assignment_boolean_op<geometry_type_1, geometry_type_2, boolean_op::BinaryOr>(lvalue, rvalue);
  }

  template <typename geometry_type_1, typename geometry_type_2>
  typename requires_2< typename gtl_if<typename is_mutable_polygon_90_set_type<geometry_type_1>::type>::type, 
                       typename gtl_if<typename is_polygon_90_set_type<geometry_type_2>::type>::type, 
                       geometry_type_1>::type &
  operator|=(geometry_type_1& lvalue, const geometry_type_2& rvalue) {
    return self_assignment_boolean_op<geometry_type_1, geometry_type_2, boolean_op::BinaryOr>(lvalue, rvalue);
  }

  template <typename geometry_type_1, typename geometry_type_2>
  typename requires_2< typename gtl_if<typename is_mutable_polygon_90_set_type<geometry_type_1>::type>::type, 
                       typename gtl_if<typename is_polygon_90_set_type<geometry_type_2>::type>::type, 
                       geometry_type_1>::type &
  operator*=(geometry_type_1& lvalue, const geometry_type_2& rvalue) {
    return self_assignment_boolean_op<geometry_type_1, geometry_type_2, boolean_op::BinaryAnd>(lvalue, rvalue);
  }
  template <typename geometry_type_1, typename geometry_type_2>
  typename requires_2< typename gtl_if<typename is_mutable_polygon_90_set_type<geometry_type_1>::type>::type, 
                       typename gtl_if<typename is_polygon_90_set_type<geometry_type_2>::type>::type, 
                       geometry_type_1>::type &
  operator&=(geometry_type_1& lvalue, const geometry_type_2& rvalue) {
    return self_assignment_boolean_op<geometry_type_1, geometry_type_2, boolean_op::BinaryAnd>(lvalue, rvalue);
  }

  template <typename geometry_type_1, typename geometry_type_2>
  typename requires_2< typename gtl_if<typename is_mutable_polygon_90_set_type<geometry_type_1>::type>::type, 
                       typename gtl_if<typename is_polygon_90_set_type<geometry_type_2>::type>::type, 
                       geometry_type_1>::type &
  operator^=(geometry_type_1& lvalue, const geometry_type_2& rvalue) {
    return self_assignment_boolean_op<geometry_type_1, geometry_type_2, boolean_op::BinaryXor>(lvalue, rvalue);
  }

  template <typename geometry_type_1, typename geometry_type_2>
  typename requires_2< typename gtl_if<typename is_mutable_polygon_90_set_type<geometry_type_1>::type>::type, 
                       typename gtl_if<typename is_polygon_90_set_type<geometry_type_2>::type>::type, 
                       geometry_type_1>::type &
  operator-=(geometry_type_1& lvalue, const geometry_type_2& rvalue) {
    return self_assignment_boolean_op<geometry_type_1, geometry_type_2, boolean_op::BinaryNot>(lvalue, rvalue);
  }

  template <typename geometry_type_1, typename coordinate_type_1>
  typename requires_2< typename gtl_if<typename is_polygon_90_set_type<geometry_type_1>::type>::type, 
                       typename is_same_type_SFINAE<typename geometry_concept<coordinate_type_1>::type, coordinate_concept>::type,
                       polygon_90_set_data<typename polygon_90_set_traits<geometry_type_1>::coordinate_type> >::type 
  operator+(const geometry_type_1& lvalue, coordinate_type_1 rvalue) {
    polygon_90_set_data<typename polygon_90_set_traits<geometry_type_1>::coordinate_type> ps;
    assign(ps, lvalue);
    resize(ps, rvalue);
    return ps;
  }

  template <typename geometry_type_1, typename coordinate_type_1>
  typename requires_2< typename gtl_if<typename is_polygon_90_set_type<geometry_type_1>::type>::type, 
                       typename is_same_type_SFINAE<typename geometry_concept<coordinate_type_1>::type, coordinate_concept>::type,
                       polygon_90_set_data<typename polygon_90_set_traits<geometry_type_1>::coordinate_type> >::type 
  operator-(const geometry_type_1& lvalue, coordinate_type_1 rvalue) {
    polygon_90_set_data<typename polygon_90_set_traits<geometry_type_1>::coordinate_type> ps;
    assign(ps, lvalue);
    resize(ps, -rvalue);
    return ps;
  }

  template <typename geometry_type_1, typename coordinate_type_1>
  typename requires_2< typename gtl_if<typename is_polygon_90_set_type<geometry_type_1>::type>::type, 
                       typename is_same_type_SFINAE<typename geometry_concept<coordinate_type_1>::type, coordinate_concept>::type,
                       geometry_type_1>::type &
  operator+=(geometry_type_1& lvalue, coordinate_type_1 rvalue) {
    return resize(lvalue, rvalue);
  }

  template <typename geometry_type_1, typename coordinate_type_1>
  typename requires_2< typename gtl_if<typename is_mutable_polygon_90_set_type<geometry_type_1>::type>::type, 
                       typename is_same_type_SFINAE<typename geometry_concept<coordinate_type_1>::type, coordinate_concept>::type,
                       geometry_type_1>::type &
  operator-=(geometry_type_1& lvalue, coordinate_type_1 rvalue) {
    return resize(lvalue, -rvalue);
  }
}
#endif

