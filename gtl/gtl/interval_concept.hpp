/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
#ifndef GTL_INTERVAL_CONCEPT_HPP
#define GTL_INTERVAL_CONCEPT_HPP
namespace gtl {
struct interval_concept {
  interval_concept() {}

  template <typename interval_type>
  struct registration {
    typedef typename interval_traits<interval_type>::coordinate_type coordinate_type;
    typedef typename interval_traits<interval_type>::coordinate_type component_type;
    typedef typename interval_traits<interval_type>::coordinate_type center_type;
  };

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
                       typename interval_traits<interval_type>::coordinate_type value, 
                       bool consider_touch, no_type tag) {
    if(consider_touch) {
      return value <= get<HIGH>(interval) && value >= get<LOW>(interval);
    } else {
      return value < get<HIGH>(interval) && value > get<LOW>(interval);
    }
  }

  template <typename interval_type, typename interval_type_2>
  static bool contains(const interval_type& interval,
                       const interval_type_2& value, bool consider_touch, interval_concept tag) {
    return contains(interval, get(value, LOW), consider_touch, no_type()) &&
      contains(interval, get(value, HIGH), consider_touch, no_type());
  }

    /// get the low coordinate
  template <typename interval_type>
  static inline typename interval_traits<interval_type>::coordinate_type 
  low(const interval_type& interval) { return get(interval, LOW); }

  /// get the high coordinate
  template <typename interval_type>
  static inline typename interval_traits<interval_type>::coordinate_type 
  high(const interval_type& interval) { return get(interval, HIGH); }

  /// get the center coordinate
  template <typename interval_type>
  static inline typename interval_traits<interval_type>::coordinate_type 
  center(const interval_type& interval) { return (high(interval) + low(interval))/2; }

  /// set the low coordinate to v
  template <typename interval_type>
  static interval_type& low(interval_type& interval,
                            typename interval_traits<interval_type>::coordinate_type v) { 
    set(interval, LOW, v); return interval; }
  
  /// set the high coordinate to v
  template <typename interval_type>
  static interval_type& high(interval_type& interval,
                             typename interval_traits<interval_type>::coordinate_type v) { 
    set(interval, HIGH, v); return interval; }
  
  /// get the magnitude of the interval
  template <typename interval_type>
  static inline typename interval_traits<interval_type>::coordinate_type 
  delta(const interval_type& interval) { return high(interval) - low(interval); }
    
  /// flip this about coordinate
  template <typename interval_type>
  static interval_type& flip(interval_type& interval,
                             typename interval_traits<interval_type>::coordinate_type axis = 0) {
    typename interval_traits<interval_type>::coordinate_type newLow, newHigh;
    newLow  = axis - high(interval);
    newHigh = axis - low(interval);
    low(interval, newLow);
    high(interval, newHigh);
    return interval;
  }

  /// scale interval by factor
  template <typename interval_type>
  static interval_type& scale(interval_type& interval,
                       double factor) {
    typedef typename interval_traits<interval_type>::coordinate_type Unit;
    Unit newHigh = (Unit)(high(interval) * factor);
    low(interval, (Unit)(low(interval)*factor));
    high(interval, (Unit)(newHigh));
    return interval;
  }
  
  /// move interval by delta
  template <typename interval_type>
  static interval_type& move(interval_type& interval,
                             typename interval_traits<interval_type>::coordinate_type delta) {
    typedef typename interval_traits<interval_type>::coordinate_type Unit;
    Unit len = high(interval) - low(interval);
    low(interval, low(interval) + delta);
    high(interval, low(interval) + len);
    return interval;
  }
  
  /// convolve this with b
  template <typename interval_type>
  static interval_type& convolve(interval_type& interval,
                                 typename interval_traits<interval_type>::coordinate_type b, no_type tag) {
    typedef typename interval_traits<interval_type>::coordinate_type Unit;
    Unit newLow  = low(interval) + b;
    Unit newHigh = high(interval) + b;
    low(interval, newLow);
    high(interval, newHigh);
    return interval;
  }

  /// deconvolve this with b
  template <typename interval_type>
  static interval_type& deconvolve(interval_type& interval,
                                   typename interval_traits<interval_type>::coordinate_type b, no_type tag) {
    typedef typename interval_traits<interval_type>::coordinate_type Unit;
    Unit newLow  = low(interval)  - b;
    Unit newHigh = high(interval) - b;
    low(interval, newLow);
    high(interval, newHigh);
    return interval;
  }

  /// convolve this with b
  template <typename interval_type, typename interval_type_2>
  static interval_type& convolve(interval_type& interval,
                                 const interval_type_2& b, interval_concept tag) {
    typedef typename interval_traits<interval_type>::coordinate_type Unit;
    Unit newLow  = low(interval)  + low(b);
    Unit newHigh = high(interval) + high(b);
    low(interval, newLow);
    high(interval, newHigh);
    return interval;
  }
  
  /// deconvolve this with b
  template <typename interval_type, typename interval_type_2>
  static interval_type& deconvolve(interval_type& interval,
                                   const interval_type_2& b, interval_concept tag) {
    typedef typename interval_traits<interval_type>::coordinate_type Unit;
    Unit newLow  = low(interval)  - low(b);
    Unit newHigh = high(interval) - high(b);
    low(interval, newLow);
    high(interval, newHigh);
    return interval;
  }
  
  /// reflected convolve this with b
  template <typename interval_type, typename interval_type_2>
  static interval_type& reflected_convolve(interval_type& interval,
                                          const interval_type_2& b, interval_concept tag) {
    typedef typename interval_traits<interval_type>::coordinate_type Unit;
    Unit newLow  = low(interval)  - high(b);
    Unit newHigh = high(interval) - low(b);
    low(interval, newLow);
    high(interval, newHigh);
    return interval;
  }
  
  /// reflected deconvolve this with b
  template <typename interval_type, typename interval_type_2>
  static interval_type& reflected_deconvolve(interval_type& interval,
                                            const interval_type_2& b, interval_concept tag) {
    typedef typename interval_traits<interval_type>::coordinate_type Unit;
    Unit newLow  = low(interval)  + high(b);
    Unit newHigh = high(interval) + low(b);
    low(interval, newLow);
    high(interval, newHigh);
    return interval;
  }
  
  /// distance from a coordinate to an interval
  template <typename interval_type>
  static inline typename interval_traits<interval_type>::coordinate_type 
  distance(const interval_type& interval,
           typename interval_traits<interval_type>::coordinate_type position, no_type tag) {
    typedef typename interval_traits<interval_type>::coordinate_type Unit;
    Unit dist[3] = {0, low(interval) - position, position - high(interval)};
    return dist[ (dist[1] > 0) + ((dist[2] > 0) << 1) ];
  }


  /// distance between two intervals
  template <typename interval_type, typename interval_type_2>
  static inline typename interval_traits<interval_type>::coordinate_type 
  distance(const interval_type& interval,
           const interval_type_2& b, interval_concept tag) {
    typedef typename interval_traits<interval_type>::coordinate_type Unit;
    Unit dist[3] = {0, low(interval) - high(b), low(b) - high(interval)};
    return dist[ (dist[1] > 0) + ((dist[2] > 0) << 1) ];
  }

  /// check if Interval b intersects `this` Interval
  template <typename interval_type, typename interval_type_2>
  static bool intersects(const interval_type& interval, const interval_type_2& b, 
                         bool consider_touch, interval_concept tag) {
    return consider_touch ? 
      (low(interval) <= high(b)) & (high(interval) >= low(b)) :
      (low(interval) < high(b)) & (high(interval) > low(b));
  }

  /// check if Interval b partially overlaps `this` Interval
  template <typename interval_type, typename interval_type_2>
  static bool boundaries_intersect(const interval_type& interval, const interval_type_2& b, 
                                   bool consider_touch, interval_concept tag) {
    return (contains(interval, low(b), consider_touch, no_type()) || 
            contains(interval, high(b), consider_touch, no_type())) &&
      (contains(b, low(interval), consider_touch, no_type()) || 
       contains(b, high(interval), consider_touch, no_type()));
  }

  /// check if they are end to end
  template <typename interval_type, typename interval_type_2>
  static bool abuts(const interval_type& interval, const interval_type_2& b, direction_1d dir, interval_concept tag) {
    return dir.to_int() ? low(b) == high(interval) : low(interval) == high(b);
  }

  /// check if they are end to end
  template <typename interval_type, typename interval_type_2>
  static bool abuts(const interval_type& interval, const interval_type_2& b, interval_concept tag) {
    return abuts(interval, b, HIGH, interval_concept()) || abuts(interval, b, LOW, interval_concept());
  } 


  /// set 'this' interval to the intersection of 'this' and b
  template <typename interval_type, typename interval_type_2>
  static bool intersect(interval_type& interval, const interval_type_2& b, bool consider_touch, interval_concept tag) {
    typedef typename interval_traits<interval_type>::coordinate_type Unit;
    Unit lowVal = std::max(low(interval), low(b));
    Unit highVal = std::min(high(interval), high(b));
    bool valid = consider_touch ?
      lowVal <= highVal :
      lowVal < highVal;
    if(valid) {
      low(interval, lowVal);
      high(interval, highVal);
    }
    return valid;
  }

  /// set 'this' interval to the generalized intersection of 'this' and b
  template <typename interval_type, typename interval_type_2>
  static interval_type& generalized_intersect(interval_type& interval, const interval_type_2& b, interval_concept tag) {
    typedef typename interval_traits<interval_type>::coordinate_type Unit;
    Unit coords[4] = {low(interval), high(interval), low(b), high(b)};
    //consider implementing faster sorting of small fixed length range
    std::sort(coords, coords+4);
    low(interval, coords[1]);
    high(interval, coords[2]);
    return interval;
  }

  /// bloat the Interval
  template <typename interval_type>
  static interval_type& bloat(interval_type& interval, typename interval_traits<interval_type>::coordinate_type bloating) {
    low(interval, low(interval)-bloating);
    high(interval, high(interval)+bloating);
    return interval;
  }
  
  /// bloat the specified side of `this` Interval
  template <typename interval_type>
  static interval_type& bloat(interval_type& interval, direction_1d dir, typename interval_traits<interval_type>::coordinate_type bloating) {
    set(interval, dir, get(interval, dir) + dir.get_sign() * bloating);
    return interval;
  }


  /// shrink the Interval
  template <typename interval_type>
  static interval_type& shrink(interval_type& interval, typename interval_traits<interval_type>::coordinate_type shrinking) {
    return bloat(interval, -shrinking);
  }

  /// shrink the specified side of `this` Interval
  template <typename interval_type>
  static interval_type& shrink(interval_type& interval, direction_1d dir, typename interval_traits<interval_type>::coordinate_type shrinking) {
    return bloat(interval, dir, -shrinking);
  }

  /// Enlarge `this` Interval to encompass the specified Interval
  template <typename interval_type, typename interval_type_2>
  static bool encompass(interval_type& interval, const interval_type_2& b, interval_concept tag) {
    bool retval = !contains(interval, b, true, interval_concept());
    low(interval, std::min(low(interval), low(b)));
    high(interval, std::max(high(interval), high(b)));
    return retval;
  }    

  /// gets the half of the interval as an interval
  template <typename interval_type>
  static interval_type get_half(const interval_type& interval, direction_1d d1d) {
    typedef typename interval_traits<interval_type>::coordinate_type Unit;
    Unit c = (get(interval, LOW) + get(interval, HIGH)) / 2;
    return construct<interval_type>((d1d == LOW) ? get(interval, LOW) : c,
                                    (d1d == LOW) ? c : get(interval, HIGH));
  }

  /// returns true if the 2 intervals exactly touch at one value, like in  l1 <= h1 == l2 <= h2
  /// sets the argument to the joined interval
  template <typename interval_type, typename interval_type_2>
  static bool join_with(interval_type& interval, const interval_type_2& b) {
    if(abuts(interval, b, interval_concept())) {
      encompass(interval, b, interval_concept());
      return true;
    }
    return false;
  }
  
};

}
#endif
