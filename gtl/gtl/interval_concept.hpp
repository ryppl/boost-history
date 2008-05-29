/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
struct interval_concept {
  interval_concept() {}

  template <direction_1d_enum dir, typename T>
  static inline typename interval_traits<T>::coordinate_type 
  get(const T& interval) {
    return interval_traits<T>::get(interval, dir);
  }
  
  template <typename T>
  static inline typename interval_traits<T>::coordinate_type 
  get(const T& interval, direction_1d dir) {
    return interval_traits<T>::get(interval, dir); 
  }

  template <direction_1d_enum dir, typename T, typename coordinate_type>
  static inline void set(T& interval, coordinate_type value) {
    set(interval, dir, value); 
  }
  
  template <typename T, typename coordinate_type>
  static inline void set(T& interval, direction_1d dir, coordinate_type value) {
    interval_traits<T>::set(interval, dir, value); 
    if(get<HIGH>(interval) < get<LOW>(interval))
      interval_traits<T>::set(interval, dir.backward(), value);
  }
  
  template <typename T, typename coordinate_type1, typename coordinate_type2>
  static inline T construct(coordinate_type1 low_value, coordinate_type2 high_value) {
    if(low_value > high_value) std::swap(low_value, high_value);
    return interval_traits<T>::construct(low_value, high_value); 
  }
  
  template <typename T, typename T2>
  static T copy_construct(const T2& interval) {
    return construct<T>
      (get(interval, LOW ),
       get(interval, HIGH));
  }

  template <typename interval_type_1, typename interval_type_2>
  static interval_type_1& assign(interval_type_1& lvalue, const interval_type_2& rvalue) {
    set(lvalue, LOW, get(rvalue, LOW));
    set(lvalue, HIGH, get(rvalue, HIGH));
    return lvalue;
  }

  template <typename T, typename T2>
  static bool equivalence(const T& interval1, const T2& interval2) {
    return get(interval1, LOW) ==
      get(interval2, LOW) &&
      get(interval1, HIGH) ==
      get(interval2, HIGH); 
  }

  template <typename interval_type>
  static bool contains(const interval_type& interval,
                       typename interval_traits<interval_type>::coordinate_type value, bool consider_touch) {
    if(consider_touch) {
      return value <= get<HIGH>(interval) && value >= get<LOW>(interval);
    } else {
      return value < get<HIGH>(interval) && value > get<LOW>(interval);
    }
  }

  template <typename interval_type, typename interval_type_2>
  static bool contains(const interval_type& interval,
                       const interval_type_2& value, bool consider_touch, interval_concept tag) {
    return contains(interval, get(value, LOW), consider_touch) &&
      contains(interval, get(value, HIGH), consider_touch);
  }

};


