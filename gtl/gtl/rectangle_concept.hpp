/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
struct rectangle_concept {
  rectangle_concept() {}

  template <typename rectangle_type>
  struct registration {
    typedef typename rectangle_traits<rectangle_type>::coordinate_type coordinate_type;
    typedef interval_data<coordinate_type> component_type;
  };

  template <orientation_2d_enum orient, typename T>
  static inline typename rectangle_traits<T>::interval_type 
  get(const T& rectangle) {
    return rectangle_traits<T>::get(rectangle, orient); 
  }
  
  template <typename T>
  static inline typename rectangle_traits<T>::interval_type 
  get(const T& rectangle, orientation_2d orient) {
    return rectangle_traits<T>::get(rectangle, orient); 
  }

  template <orientation_2d_enum orient, typename T, typename T2>
  static inline void set(T& rectangle, const T2& interval) {
    rectangle_traits<T>::set(rectangle, orient, interval); 
  }

  template <typename T, typename T2>
  static inline void set(T& rectangle, orientation_2d orient, const T2& interval) {
    rectangle_traits<T>::set(rectangle, orient, interval); 
  }

  template <typename T, typename T2, typename T3>
  static inline T construct(const T2& interval_horizontal,
                            const T3& interval_vertical) {
    return rectangle_traits<T>::construct(interval_horizontal, interval_vertical); }

  template <typename T, typename coordinate_type>
  static inline T construct(coordinate_type xl, coordinate_type yl, coordinate_type xh, coordinate_type yh) {
    return rectangle_traits<T>::construct(interval_data<coordinate_type>(xl, xh), 
                                          interval_data<coordinate_type>(yl, yh)); 
  }

  template <typename T, typename T2>
  static T copy_construct(const T2& rectangle) {
    return construct<T>
      (get(rectangle, HORIZONTAL),
       get(rectangle, VERTICAL));
  }

  template <typename rectangle_type_1, typename rectangle_type_2>
  static rectangle_type_1& assign(rectangle_type_1& lvalue, const rectangle_type_2& rvalue) {
    set(lvalue, HORIZONTAL, get(rvalue, HORIZONTAL));
    set(lvalue, VERTICAL, get(rvalue, VERTICAL));
    return lvalue;
  }

  template <typename T, typename T2>
  static bool equivalence(const T& rect1, const T2& rect2) {
    return interval_concept::equivalence(get(rect1, HORIZONTAL),
                                         get(rect2, HORIZONTAL)) &&
      interval_concept::equivalence(get(rect1, VERTICAL),
                                    get(rect2, VERTICAL));
  }

  template <typename rectangle_type>
  static typename rectangle_traits<rectangle_type>::coordinate_type
  xl(const rectangle_type& rectangle) {
    return interval_concept::get<LOW>(get<HORIZONTAL>(rectangle));
  }

  template <typename rectangle_type>
  static typename rectangle_traits<rectangle_type>::coordinate_type
  xh(const rectangle_type& rectangle) {
    return interval_concept::get<HIGH>(get<HORIZONTAL>(rectangle));
  }

  template <typename rectangle_type>
  static typename rectangle_traits<rectangle_type>::coordinate_type
  yl(const rectangle_type& rectangle) {
    return interval_concept::get<LOW>(get<VERTICAL>(rectangle));
  }

  template <typename rectangle_type>
  static typename rectangle_traits<rectangle_type>::coordinate_type
  yh(const rectangle_type& rectangle) {
    return interval_concept::get<HIGH>(get<VERTICAL>(rectangle));
  }

  template <typename rectangle_type, typename rectangle_type2>
  static bool contains(const rectangle_type& rectangle, const rectangle_type2 rectangle_contained, 
                                bool consider_touch, rectangle_concept tag) {
    return interval_concept::contains(get<HORIZONTAL>(rectangle), get<HORIZONTAL>(rectangle_contained), consider_touch, interval_concept()) &&
      interval_concept::contains(get<VERTICAL>(rectangle), get<VERTICAL>(rectangle_contained), consider_touch, interval_concept());
  }
  template <typename rectangle_type, typename point_type>
  static bool contains(const rectangle_type& rectangle, const point_type point_contained, 
                                bool consider_touch, point_concept tag) {
    return interval_concept::contains(get<HORIZONTAL>(rectangle), point_concept::get<HORIZONTAL>(point_contained), consider_touch) &&
      interval_concept::contains(get<VERTICAL>(rectangle), point_concept::get<VERTICAL>(point_contained), consider_touch);
  }

  template <typename rectangle_type_1, typename rectangle_type_2>
  class less : public std::binary_function<const rectangle_type_1&, const rectangle_type_2&, bool> {
  private:
    orientation_2d orient_;
  public:
    inline less(orientation_2d orient = VERTICAL) : orient_(orient) {}
    inline bool operator () (const rectangle_type_1& a,
                             const rectangle_type_2& b) const {
      typedef typename rectangle_traits<rectangle_type_1>::coordinate_type Unit;
      Unit vl1 = interval_concept::get(get(a, orient_), LOW); 
      Unit vl2 = interval_concept::get(get(b, orient_), LOW); 
      if(vl1 > vl2) return false;
      if(vl1 == vl2) {
        orientation_2d perp = orient_.get_perpendicular();
        Unit hl1 = interval_concept::get(get(a, perp), LOW);
        Unit hl2 = interval_concept::get(get(b, perp), LOW);
        if(hl1 > hl2) return false;
        if(hl1 == hl2) {
          Unit vh1 = interval_concept::get(get(a, orient_), HIGH); 
          Unit vh2 = interval_concept::get(get(b, orient_), HIGH); 
          if(vh1 > vh2) return false;
          if(vh1 == vh2) {
            Unit hh1 = interval_concept::get(get(a, perp), HIGH);
            Unit hh2 = interval_concept::get(get(b, perp), HIGH);
            return hh1 < hh2;
          }
        }
      }
      return true;
    }
    
  };
};


