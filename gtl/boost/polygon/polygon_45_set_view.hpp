/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
#ifndef GTL_POLYGON_45_SET_VIEW_HPP
#define GTL_POLYGON_45_SET_VIEW_HPP
namespace boost { namespace polygon{

  template <typename ltype, typename rtype, int op_type>
  class polygon_45_set_view;

  template <typename ltype, typename rtype, int op_type>
  struct polygon_45_set_traits<polygon_45_set_view<ltype, rtype, op_type> > {
    typedef typename polygon_45_set_view<ltype, rtype, op_type>::coordinate_type coordinate_type;
    typedef typename polygon_45_set_view<ltype, rtype, op_type>::iterator_type iterator_type;
    typedef typename polygon_45_set_view<ltype, rtype, op_type>::operator_arg_type operator_arg_type;

    static inline iterator_type begin(const polygon_45_set_view<ltype, rtype, op_type>& polygon_45_set); 
    static inline iterator_type end(const polygon_45_set_view<ltype, rtype, op_type>& polygon_45_set);

    template <typename input_iterator_type>
    static inline void set(polygon_45_set_view<ltype, rtype, op_type>& polygon_45_set, 
                           input_iterator_type input_begin, input_iterator_type input_end);

    static inline bool clean(const polygon_45_set_view<ltype, rtype, op_type>& polygon_45_set);

  };

  template <typename value_type, typename ltype, typename rtype, int op_type>
  struct compute_45_set_value {
    static
    void value(value_type& output_, const ltype& lvalue_, const rtype& rvalue_) {
      output_.set(polygon_45_set_traits<ltype>::begin(lvalue_),
                  polygon_45_set_traits<ltype>::end(lvalue_));
      value_type rinput_;
      rinput_.set(polygon_45_set_traits<rtype>::begin(rvalue_),
                  polygon_45_set_traits<rtype>::end(rvalue_));
      if(op_type == 0)
        output_ |= rinput_;
      else if(op_type == 1)
        output_ &= rinput_;
      else if(op_type == 2)
        output_ ^= rinput_;
      else
        output_ -= rinput_;
    }
  };

  template <typename value_type, typename ltype, typename rcoord, int op_type>
  struct compute_45_set_value<value_type, ltype, polygon_45_set_data<rcoord>, op_type> {
    static
    void value(value_type& output_, const ltype& lvalue_, const polygon_45_set_data<rcoord>& rvalue_) {
      output_.set(polygon_45_set_traits<ltype>::begin(lvalue_),
                  polygon_45_set_traits<ltype>::end(lvalue_));
      if(op_type == 0)
        output_ |= rvalue_;
      else if(op_type == 1)
        output_ &= rvalue_;
      else if(op_type == 2)
        output_ ^= rvalue_;
      else
        output_ -= rvalue_;
    }
  };

  template <typename ltype, typename rtype, int op_type>
  class polygon_45_set_view {
  public:
    typedef typename polygon_45_set_traits<ltype>::coordinate_type coordinate_type;
    typedef polygon_45_set_data<coordinate_type> value_type;
    typedef typename value_type::iterator_type iterator_type;
    typedef polygon_45_set_view operator_arg_type;
  private:
    const ltype& lvalue_;
    const rtype& rvalue_;
    mutable value_type output_;
    mutable bool evaluated_;
  public:
    polygon_45_set_view(const ltype& lvalue,
                        const rtype& rvalue ) :
      lvalue_(lvalue), rvalue_(rvalue), output_(), evaluated_(false) {}

    // get iterator to begin vertex data
  public:
    const value_type& value() const {
      if(!evaluated_) {
        evaluated_ = true;
        compute_45_set_value<value_type, ltype, rtype, op_type>::value(output_, lvalue_, rvalue_);
      }
      return output_;
    }
  public:
    iterator_type begin() const { return value().begin(); }
    iterator_type end() const { return value().end(); }

    bool dirty() const { return value().dirty(); } //result of a boolean is clean
    bool sorted() const { return value().sorted(); } //result of a boolean is sorted

    //     template <typename input_iterator_type>
    //     void set(input_iterator_type input_begin, input_iterator_type input_end, 
    //              orientation_2d orient) const {
    //       orient_ = orient;
    //       output_.clear();
    //       output_.insert(output_.end(), input_begin, input_end);
    //       std::sort(output_.begin(), output_.end());
    //     }
  };

  template <typename ltype, typename rtype, int op_type>
  typename polygon_45_set_view<ltype, rtype, op_type>::iterator_type 
  polygon_45_set_traits<polygon_45_set_view<ltype, rtype, op_type> >::
  begin(const polygon_45_set_view<ltype, rtype, op_type>& polygon_45_set) {
    return polygon_45_set.begin();
  }
  template <typename ltype, typename rtype, int op_type>
  typename polygon_45_set_view<ltype, rtype, op_type>::iterator_type 
  polygon_45_set_traits<polygon_45_set_view<ltype, rtype, op_type> >::
  end(const polygon_45_set_view<ltype, rtype, op_type>& polygon_45_set) {
    return polygon_45_set.end();
  }
  template <typename ltype, typename rtype, int op_type>
  bool polygon_45_set_traits<polygon_45_set_view<ltype, rtype, op_type> >::
  clean(const polygon_45_set_view<ltype, rtype, op_type>& polygon_45_set) { 
    return polygon_45_set.value().clean(); }

  template <typename geometry_type_1, typename geometry_type_2, int op_type>
  geometry_type_1& self_assignment_boolean_op_45(geometry_type_1& lvalue_, const geometry_type_2& rvalue_) {
    typedef geometry_type_1 ltype;
    typedef geometry_type_2 rtype;
    typedef typename polygon_45_set_traits<ltype>::coordinate_type coordinate_type;
    typedef polygon_45_set_data<coordinate_type> value_type;
    value_type output_;
    value_type rinput_;
    output_.set(polygon_45_set_traits<ltype>::begin(lvalue_),
                polygon_45_set_traits<ltype>::end(lvalue_));
    rinput_.set(polygon_45_set_traits<rtype>::begin(rvalue_),
                polygon_45_set_traits<rtype>::end(rvalue_));
    if(op_type == 0)
      output_ |= rinput_;
    else if(op_type == 1)
      output_ &= rinput_;
    else if(op_type == 2)
      output_ ^= rinput_;
    else
      output_ -= rinput_;
    polygon_45_set_mutable_traits<geometry_type_1>::set(lvalue_, output_.begin(), output_.end());
    return lvalue_;
  }

  template <typename concept_type>
  struct fracture_holes_option_by_type {
    static const bool value = true;
  };
  template <>
  struct fracture_holes_option_by_type<polygon_45_with_holes_concept> {
    static const bool value = false;
  };
  template <>
  struct fracture_holes_option_by_type<polygon_with_holes_concept> {
    static const bool value = false;
  };

  template <typename ltype, typename rtype, int op_type>
  struct geometry_concept<polygon_45_set_view<ltype, rtype, op_type> > { typedef polygon_45_set_concept type; };

  template <typename geometry_type_1, typename geometry_type_2>
  typename enable_if< typename gtl_and_3< 
    typename is_polygon_45_or_90_set_type<geometry_type_1>::type,
    typename is_polygon_45_or_90_set_type<geometry_type_2>::type,
    typename is_either_polygon_45_set_type<geometry_type_1, geometry_type_2>::type>::type,
                       polygon_45_set_view<geometry_type_1, geometry_type_2, 0> >::type 
  operator|(const geometry_type_1& lvalue, const geometry_type_2& rvalue) {
    return polygon_45_set_view<geometry_type_1, geometry_type_2, 0>
      (lvalue, rvalue);
  }
  
  template <typename geometry_type_1, typename geometry_type_2>
  typename enable_if< typename gtl_and_3< typename is_polygon_45_or_90_set_type<geometry_type_1>
#ifdef __ICC 
      ::type
#endif
  ::type, typename is_polygon_45_or_90_set_type<geometry_type_2>
#ifdef __ICC 
  ::type
#endif
  ::type, typename is_either_polygon_45_set_type<geometry_type_1, geometry_type_2>::type>
#ifdef __ICC 
  ::type
#endif
  ::type, polygon_45_set_view<geometry_type_1, geometry_type_2, 0> >::type 
  operator+(const geometry_type_1& lvalue, const geometry_type_2& rvalue) {
    return polygon_45_set_view<geometry_type_1, geometry_type_2, 0>
      (lvalue, rvalue);
  }
  
  template <typename geometry_type_1, typename geometry_type_2>
  typename enable_if< typename gtl_and_3< typename is_polygon_45_or_90_set_type<geometry_type_1>::type,
                                           typename is_polygon_45_or_90_set_type<geometry_type_2>::type,
                                           typename is_either_polygon_45_set_type<geometry_type_1, geometry_type_2>::type>::type,
                       polygon_45_set_view<geometry_type_1, geometry_type_2, 1> >::type 
  operator*(const geometry_type_1& lvalue, const geometry_type_2& rvalue) {
    return polygon_45_set_view<geometry_type_1, geometry_type_2, 1>
      (lvalue, rvalue);
  }
  
  template <typename geometry_type_1, typename geometry_type_2>
  typename enable_if< typename gtl_and_3< typename is_polygon_45_or_90_set_type<geometry_type_1>::type,
                                           typename is_polygon_45_or_90_set_type<geometry_type_2>::type,
                                           typename is_either_polygon_45_set_type<geometry_type_1, geometry_type_2>::type>::type,
                       polygon_45_set_view<geometry_type_1, geometry_type_2, 2> >::type 
  operator^(const geometry_type_1& lvalue, const geometry_type_2& rvalue) {
    return polygon_45_set_view<geometry_type_1, geometry_type_2, 2>
      (lvalue, rvalue);
  }
  
  template <typename geometry_type_1, typename geometry_type_2>
  typename enable_if< typename gtl_and_3< typename is_polygon_45_or_90_set_type<geometry_type_1>
#ifdef __ICC 
      ::type
#endif
  ::type, typename is_polygon_45_or_90_set_type<geometry_type_2>
#ifdef __ICC 
  ::type
#endif
  ::type, typename is_either_polygon_45_set_type<geometry_type_1, geometry_type_2>::type>
#ifdef __ICC 
  ::type
#endif
  ::type, polygon_45_set_view<geometry_type_1, geometry_type_2, 3> >::type 
  operator-(const geometry_type_1& lvalue, const geometry_type_2& rvalue) {
    return polygon_45_set_view<geometry_type_1, geometry_type_2, 3>
      (lvalue, rvalue);
  }
  
  template <typename geometry_type_1, typename geometry_type_2>
  typename enable_if< typename gtl_and< typename is_mutable_polygon_45_set_type<geometry_type_1>::type, 
                                         typename is_polygon_45_or_90_set_type<geometry_type_2>::type>::type, 
                       geometry_type_1>::type &
  operator+=(geometry_type_1& lvalue, const geometry_type_2& rvalue) {
    return self_assignment_boolean_op_45<geometry_type_1, geometry_type_2, 0>(lvalue, rvalue);
  }

  template <typename geometry_type_1, typename geometry_type_2>
  typename enable_if< typename gtl_and< typename is_mutable_polygon_45_set_type<geometry_type_1>::type, 
                                         typename is_polygon_45_or_90_set_type<geometry_type_2>::type>::type, 
                       geometry_type_1>::type &
  operator|=(geometry_type_1& lvalue, const geometry_type_2& rvalue) {
    return self_assignment_boolean_op_45<geometry_type_1, geometry_type_2, 0>(lvalue, rvalue);
  }

  template <typename geometry_type_1, typename geometry_type_2>
  typename enable_if< typename gtl_and< 
    typename is_mutable_polygon_45_set_type<geometry_type_1>::type, 
    typename is_polygon_45_or_90_set_type<geometry_type_2>::type>::type, 
                       geometry_type_1>::type &
  operator*=(geometry_type_1& lvalue, const geometry_type_2& rvalue) {
    return self_assignment_boolean_op_45<geometry_type_1, geometry_type_2, 1>(lvalue, rvalue);
  }

  template <typename geometry_type_1, typename geometry_type_2>
  typename enable_if< typename gtl_and< typename is_mutable_polygon_45_set_type<geometry_type_1>::type, 
                                         typename is_polygon_45_or_90_set_type<geometry_type_2>::type>::type, 
                       geometry_type_1>::type &
  operator&=(geometry_type_1& lvalue, const geometry_type_2& rvalue) {
    return self_assignment_boolean_op_45<geometry_type_1, geometry_type_2, 1>(lvalue, rvalue);
  }

  template <typename geometry_type_1, typename geometry_type_2>
  typename enable_if< 
    typename gtl_and< typename is_mutable_polygon_45_set_type<geometry_type_1>::type, 
                      typename is_polygon_45_or_90_set_type<geometry_type_2>::type>::type, 
    geometry_type_1>::type &
  operator^=(geometry_type_1& lvalue, const geometry_type_2& rvalue) {
    return self_assignment_boolean_op_45<geometry_type_1, geometry_type_2, 2>(lvalue, rvalue);
  }

  template <typename geometry_type_1, typename geometry_type_2>
  typename enable_if< typename gtl_and< typename is_mutable_polygon_45_set_type<geometry_type_1>::type, 
                                         typename is_polygon_45_or_90_set_type<geometry_type_2>::type>::type, 
                       geometry_type_1>::type &
  operator-=(geometry_type_1& lvalue, const geometry_type_2& rvalue) {
    return self_assignment_boolean_op_45<geometry_type_1, geometry_type_2, 3>(lvalue, rvalue);
  }

  template <typename geometry_type_1, typename coordinate_type_1>
  typename enable_if< typename gtl_and< typename is_mutable_polygon_45_set_type<geometry_type_1>::type, 
                                         typename gtl_same_type<typename geometry_concept<coordinate_type_1>::type, 
                                                                coordinate_concept>::type>::type,
                       geometry_type_1>::type &
  operator+=(geometry_type_1& lvalue, coordinate_type_1 rvalue) {
    return resize(lvalue, rvalue);
  }

  template <typename geometry_type_1, typename coordinate_type_1>
  typename enable_if< typename gtl_and< typename gtl_if<typename is_mutable_polygon_45_set_type<geometry_type_1>::type>::type, 
                                         typename gtl_same_type<typename geometry_concept<coordinate_type_1>::type, 
                                                                coordinate_concept>::type>::type,
                       geometry_type_1>::type &
  operator-=(geometry_type_1& lvalue, coordinate_type_1 rvalue) {
    return resize(lvalue, -rvalue);
  }

  template <typename geometry_type_1, typename coordinate_type_1>
  typename enable_if< typename gtl_and< typename gtl_if<typename is_mutable_polygon_45_set_type<geometry_type_1>::type>
#ifdef __ICC 
  ::type
#endif
  ::type, typename gtl_same_type<typename geometry_concept<coordinate_type_1>::type, 
                                 coordinate_concept>::type>
#ifdef __ICC 
  ::type
#endif
  ::type, geometry_type_1>::type
  operator+(const geometry_type_1& lvalue, coordinate_type_1 rvalue) {
    geometry_type_1 retval(lvalue);
    retval += rvalue;
    return retval;
  }

  template <typename geometry_type_1, typename coordinate_type_1>
  typename enable_if< typename gtl_and< typename gtl_if<typename is_mutable_polygon_45_set_type<geometry_type_1>::type>
#ifdef __ICC 
  ::type
#endif
  ::type, typename gtl_same_type<typename geometry_concept<coordinate_type_1>::type, 
                                 coordinate_concept>::type>
#ifdef __ICC 
  ::type
#endif
  ::type, geometry_type_1>::type
  operator-(const geometry_type_1& lvalue, coordinate_type_1 rvalue) {
    geometry_type_1 retval(lvalue);
    retval -= rvalue;
    return retval;
  }
}
}
#endif

