/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
#ifndef GTL_INTERVAL_CONCEPT_HPP
#define GTL_INTERVAL_CONCEPT_HPP
#include "isotropy.hpp"
#include "interval_data.hpp"
#include "interval_traits.hpp"

namespace gtl {
  struct interval_concept {};
 
  template <typename T>
  struct is_interval_concept {};
  template <>
  struct is_interval_concept<interval_concept> { typedef void type; };

  template <typename T>
  struct is_mutable_interval_concept {};
  template <>
  struct is_mutable_interval_concept<interval_concept> { typedef void type; };

  template <typename T>
  static inline typename interval_traits<T>::coordinate_type 
  get(const T& interval, direction_1d dir) {
    return interval_traits<T>::get(interval, dir); 
  }

  template <typename T, typename coordinate_type>
  typename requires_1<typename is_mutable_interval_concept<typename geometry_concept<T>::type>::type, void>::type 
  set(T& interval, direction_1d dir, coordinate_type value) {
    //this may need to be refined
    interval_mutable_traits<T>::set(interval, dir, value); 
    if(high(interval) < low(interval))
      interval_mutable_traits<T>::set(interval, dir.backward(), value);
  }
  
  template <typename T>
  typename requires_1<
    typename is_mutable_interval_concept<typename geometry_concept<T>::type>::type,
    T>::type 
  construct(typename interval_traits<T>::coordinate_type low_value, typename interval_traits<T>::coordinate_type high_value) {
    if(low_value > high_value) std::swap(low_value, high_value);
    return interval_mutable_traits<T>::construct(low_value, high_value); 
  }
  
  template <typename T, typename T2>
  typename requires_2< typename is_mutable_interval_concept<typename geometry_concept<T>::type>::type,
                       typename is_interval_concept<typename geometry_concept<T2>::type>::type,
                       T>::type
  copy_construct(const T2& interval) {
    return construct<T>
      (get(interval, LOW ),
       get(interval, HIGH));
  }

  template <typename T1, typename T2>
  typename requires_2< typename is_mutable_interval_concept<typename geometry_concept<T1>::type>::type,
                       typename is_interval_concept<typename geometry_concept<T2>::type>::type,
                       T1>::type &
  assign(T1& lvalue, const T2& rvalue) {
    lvalue = copy_construct<T1>(rvalue);
    return lvalue;
  }

  template <typename T, typename T2>
  typename requires_2< typename is_interval_concept<typename geometry_concept<T>::type>::type,
                       typename is_interval_concept<typename geometry_concept<T2>::type>::type,
                       bool>::type 
  equivalence(const T& interval1, const T2& interval2) {
    return get(interval1, LOW) ==
      get(interval2, LOW) &&
      get(interval1, HIGH) ==
      get(interval2, HIGH); 
  }
  
  template <typename interval_type>
  typename requires_1< typename is_interval_concept<typename geometry_concept<interval_type>::type>::type,
                       bool>::type 
  contains(const interval_type& interval,
           typename interval_traits<interval_type>::coordinate_type value, 
           bool consider_touch = true) {
    if(consider_touch) {
      return value <= high(interval) && value >= low(interval);
    } else {
      return value < high(interval) && value > low(interval);
    }
  }
  
  template <typename interval_type, typename interval_type_2>
  typename requires_2< typename is_interval_concept<typename geometry_concept<interval_type>::type>::type,
                       typename is_interval_concept<typename geometry_concept<interval_type_2>::type>::type,
                       bool>::type 
  contains(const interval_type& interval,
           const interval_type_2& value, bool consider_touch = true) {
    return contains(interval, get(value, LOW), consider_touch) &&
      contains(interval, get(value, HIGH), consider_touch);
  }
  
  /// get the low coordinate
  template <typename interval_type>
  typename requires_1< typename is_interval_concept<typename geometry_concept<interval_type>::type>::type,
                       typename interval_traits<interval_type>::coordinate_type >::type
  low(const interval_type& interval) { return get(interval, LOW); }

  /// get the high coordinate
  template <typename interval_type>
  typename requires_1< typename is_interval_concept<typename geometry_concept<interval_type>::type>::type,
                       typename interval_traits<interval_type>::coordinate_type >::type
  high(const interval_type& interval) { return get(interval, HIGH); }

  /// get the center coordinate
  template <typename interval_type>
  typename requires_1< typename is_interval_concept<typename geometry_concept<interval_type>::type>::type,
                       typename interval_traits<interval_type>::coordinate_type >::type
  center(const interval_type& interval) { return (high(interval) + low(interval))/2; }

  /// set the low coordinate to v
  template <typename interval_type>
  typename requires_1<typename is_mutable_interval_concept<typename geometry_concept<interval_type>::type>::type, void>::type 
  low(interval_type& interval,
      typename interval_traits<interval_type>::coordinate_type v) { 
    set(interval, LOW, v); }
  
  /// set the high coordinate to v
  template <typename interval_type>
  typename requires_1<typename is_mutable_interval_concept<typename geometry_concept<interval_type>::type>::type, void>::type 
  high(interval_type& interval,
       typename interval_traits<interval_type>::coordinate_type v) { 
    set(interval, HIGH, v); }
  
  /// get the magnitude of the interval
  template <typename interval_type>
  typename coordinate_traits<typename interval_traits<interval_type>::coordinate_type>::coordinate_difference
  delta(const interval_type& interval) { 
    typedef typename coordinate_traits<typename interval_traits<interval_type>::coordinate_type>::coordinate_difference diffT;
    return (diffT)high(interval) - (diffT)low(interval); }
  
  /// flip this about coordinate
  template <typename interval_type>
  typename requires_1<typename is_mutable_interval_concept<typename geometry_concept<interval_type>::type>::type, interval_type>::type &
  flip(interval_type& interval,
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
  typename requires_1<typename is_mutable_interval_concept<typename geometry_concept<interval_type>::type>::type, interval_type>::type &
  scale_up(interval_type& interval, 
           typename coordinate_traits<typename interval_traits<interval_type>::coordinate_type>::unsigned_area_type factor) {
    typedef typename interval_traits<interval_type>::coordinate_type Unit;
    Unit newHigh = high(interval) * (Unit)factor;
    low(interval, low(interval) * (Unit)factor);
    high(interval, (newHigh));
    return interval;
  }

  template <typename interval_type>
  typename requires_1<typename is_mutable_interval_concept<typename geometry_concept<interval_type>::type>::type, interval_type>::type &
  scale_down(interval_type& interval, 
             typename coordinate_traits<typename interval_traits<interval_type>::coordinate_type>::unsigned_area_type factor) {
    typedef typename interval_traits<interval_type>::coordinate_type Unit;
    typedef typename coordinate_traits<Unit>::coordinate_distance dt;
    Unit newHigh = scaling_policy<Unit>::round((dt)(high(interval)) / (dt)factor); 
    low(interval, scaling_policy<Unit>::round((dt)(low(interval)) / (dt)factor)); 
    high(interval, (newHigh));
    return interval;
  }

  template <typename interval_type>
  typename requires_1<typename is_mutable_interval_concept<typename geometry_concept<interval_type>::type>::type, interval_type>::type &
  scale(interval_type& interval, double factor) {
    typedef typename interval_traits<interval_type>::coordinate_type Unit;
    Unit newHigh = scaling_policy<Unit>::round((double)(high(interval)) * factor);
    low(interval, scaling_policy<Unit>::round((double)low(interval)* factor));
    high(interval, (newHigh));
    return interval;
  }
  
  /// move interval by delta
  template <typename interval_type>
  typename requires_1<typename is_mutable_interval_concept<typename geometry_concept<interval_type>::type>::type, interval_type>::type &
  move(interval_type& interval,
       typename coordinate_traits<typename interval_traits<interval_type>::coordinate_type>::coordinate_difference displacement) {
    typedef typename coordinate_traits<typename interval_traits<interval_type>::coordinate_type>::coordinate_difference Unit;
    Unit len = delta(interval);
    low(interval, (Unit)low(interval) + displacement);
    high(interval, (Unit)low(interval) + len);
    return interval;
  }
  
  /// convolve this with b
  template <typename interval_type>
  typename requires_1<typename is_mutable_interval_concept<typename geometry_concept<interval_type>::type>::type, interval_type>::type &
  convolve(interval_type& interval,
           typename interval_traits<interval_type>::coordinate_type b) {
    typedef typename interval_traits<interval_type>::coordinate_type Unit;
    Unit newLow  = low(interval) + b;
    Unit newHigh = high(interval) + b;
    low(interval, newLow);
    high(interval, newHigh);
    return interval;
  }

  /// deconvolve this with b
  template <typename interval_type>
  typename requires_1<typename is_mutable_interval_concept<typename geometry_concept<interval_type>::type>::type, interval_type>::type &
  deconvolve(interval_type& interval,
             typename interval_traits<interval_type>::coordinate_type b) {
    typedef typename interval_traits<interval_type>::coordinate_type Unit;
    Unit newLow  = low(interval)  - b;
    Unit newHigh = high(interval) - b;
    low(interval, newLow);
    high(interval, newHigh);
    return interval;
  }

  /// convolve this with b
  template <typename interval_type, typename interval_type_2>
  typename requires_2< typename is_mutable_interval_concept<typename geometry_concept<interval_type>::type>::type, 
                       typename is_interval_concept<typename geometry_concept<interval_type_2>::type>::type, interval_type>::type &
  convolve(interval_type& interval,
           const interval_type_2& b) {
    typedef typename interval_traits<interval_type>::coordinate_type Unit;
    Unit newLow  = low(interval)  + low(b);
    Unit newHigh = high(interval) + high(b);
    low(interval, newLow);
    high(interval, newHigh);
    return interval;
  }
  
  /// deconvolve this with b
  template <typename interval_type, typename interval_type_2>
  typename requires_2< typename is_mutable_interval_concept<typename geometry_concept<interval_type>::type>::type,
                       typename is_interval_concept<typename geometry_concept<interval_type_2>::type>::type,
                       interval_type>::type &
  deconvolve(interval_type& interval,
             const interval_type_2& b) {
    typedef typename interval_traits<interval_type>::coordinate_type Unit;
    Unit newLow  = low(interval)  - low(b);
    Unit newHigh = high(interval) - high(b);
    low(interval, newLow);
    high(interval, newHigh);
    return interval;
  }
  
  /// reflected convolve this with b
  template <typename interval_type, typename interval_type_2>
  typename requires_2< typename is_mutable_interval_concept<typename geometry_concept<interval_type>::type>::type,
                       typename is_interval_concept<typename geometry_concept<interval_type_2>::type>::type,
                       interval_type>::type &
  reflected_convolve(interval_type& interval,
                     const interval_type_2& b) {
    typedef typename interval_traits<interval_type>::coordinate_type Unit;
    Unit newLow  = low(interval)  - high(b);
    Unit newHigh = high(interval) - low(b);
    low(interval, newLow);
    high(interval, newHigh);
    return interval;
  }
  
  /// reflected deconvolve this with b
  template <typename interval_type, typename interval_type_2>
  typename requires_2< typename is_mutable_interval_concept<typename geometry_concept<interval_type>::type>::type, 
                       typename is_interval_concept<typename geometry_concept<interval_type_2>::type>::type, interval_type>::type &
  reflected_deconvolve(interval_type& interval,
                       const interval_type_2& b) {
    typedef typename interval_traits<interval_type>::coordinate_type Unit;
    Unit newLow  = low(interval)  + high(b);
    Unit newHigh = high(interval) + low(b);
    low(interval, newLow);
    high(interval, newHigh);
    return interval;
  }
  
  /// distance from a coordinate to an interval
  template <typename interval_type>
  typename requires_1< typename is_interval_concept<typename geometry_concept<interval_type>::type>::type,
                       typename coordinate_traits<typename interval_traits<interval_type>::coordinate_type>::coordinate_difference>::type
  euclidean_distance(const interval_type& interval,
                     typename interval_traits<interval_type>::coordinate_type position) {
    typedef typename coordinate_traits<typename interval_traits<interval_type>::coordinate_type>::coordinate_difference Unit;
    Unit dist[3] = {0, (Unit)low(interval) - (Unit)position, (Unit)position - (Unit)high(interval)};
    return dist[ (dist[1] > 0) + ((dist[2] > 0) << 1) ];
  }
  
  
  /// distance between two intervals
  template <typename interval_type, typename interval_type_2>
  typename requires_2< typename is_interval_concept<typename geometry_concept<interval_type>::type>::type,
                       typename is_interval_concept<typename geometry_concept<interval_type_2>::type>::type,
                       typename coordinate_traits<typename interval_traits<interval_type>::coordinate_type>::coordinate_difference>::type
  euclidean_distance(const interval_type& interval,
                     const interval_type_2& b) {
    typedef typename coordinate_traits<typename interval_traits<interval_type>::coordinate_type>::coordinate_difference Unit;
    Unit dist[3] = {0, (Unit)low(interval) - (Unit)high(b), (Unit)low(b) - (Unit)high(interval)};
    return dist[ (dist[1] > 0) + ((dist[2] > 0) << 1) ];
  }
  
  /// check if Interval b intersects `this` Interval
  template <typename interval_type, typename interval_type_2>
  typename requires_2< typename is_interval_concept<typename geometry_concept<interval_type>::type>::type,
                       typename is_interval_concept<typename geometry_concept<interval_type_2>::type>::type,
                       bool>::type 
  intersects(const interval_type& interval, const interval_type_2& b, 
             bool consider_touch = true) {
    return consider_touch ? 
      (low(interval) <= high(b)) & (high(interval) >= low(b)) :
      (low(interval) < high(b)) & (high(interval) > low(b));
  }

  /// check if Interval b partially overlaps `this` Interval
  template <typename interval_type, typename interval_type_2>
  typename requires_2< typename is_interval_concept<typename geometry_concept<interval_type>::type>::type,
                       typename is_interval_concept<typename geometry_concept<interval_type_2>::type>::type,
                       bool>::type 
  boundaries_intersect(const interval_type& interval, const interval_type_2& b, 
                       bool consider_touch = true) {
    return (contains(interval, low(b), consider_touch) || 
            contains(interval, high(b), consider_touch)) &&
      (contains(b, low(interval), consider_touch) || 
       contains(b, high(interval), consider_touch));
  }

  /// check if they are end to end
  template <typename interval_type, typename interval_type_2>
  typename requires_2< typename is_interval_concept<typename geometry_concept<interval_type>::type>::type,
                       typename is_interval_concept<typename geometry_concept<interval_type_2>::type>::type,
                       bool>::type 
  abuts(const interval_type& interval, const interval_type_2& b, direction_1d dir) {
    return dir.to_int() ? low(b) == high(interval) : low(interval) == high(b);
  }

  /// check if they are end to end
  template <typename interval_type, typename interval_type_2>
  typename requires_2< typename is_interval_concept<typename geometry_concept<interval_type>::type>::type,
                       typename is_interval_concept<typename geometry_concept<interval_type_2>::type>::type,
                       bool>::type 
  abuts(const interval_type& interval, const interval_type_2& b) {
    return abuts(interval, b, HIGH) || abuts(interval, b, LOW);
  } 


  /// set 'this' interval to the intersection of 'this' and b
  template <typename interval_type, typename interval_type_2>
  typename requires_2< typename is_mutable_interval_concept<typename geometry_concept<interval_type>::type>::type,
                       typename is_interval_concept<typename geometry_concept<interval_type_2>::type>::type,
                       bool>::type 
  intersect(interval_type& interval, const interval_type_2& b, bool consider_touch = true) {
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
  typename requires_2< typename is_mutable_interval_concept<typename geometry_concept<interval_type>::type>::type,
                       typename is_interval_concept<typename geometry_concept<interval_type_2>::type>::type,
                       interval_type>::type &
  generalized_intersect(interval_type& interval, const interval_type_2& b) {
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
  typename requires_1< typename is_mutable_interval_concept<typename geometry_concept<interval_type>::type>::type,
                       interval_type>::type &
  bloat(interval_type& interval, typename interval_traits<interval_type>::coordinate_type bloating) {
    low(interval, low(interval)-bloating);
    high(interval, high(interval)+bloating);
    return interval;
  }
  
  /// bloat the specified side of `this` Interval
  template <typename interval_type>
  typename requires_1< typename is_mutable_interval_concept<typename geometry_concept<interval_type>::type>::type,
                       interval_type>::type &
  bloat(interval_type& interval, direction_1d dir, typename interval_traits<interval_type>::coordinate_type bloating) {
    set(interval, dir, get(interval, dir) + dir.get_sign() * bloating);
    return interval;
  }


  /// shrink the Interval
  template <typename interval_type>
  typename requires_1< typename is_mutable_interval_concept<typename geometry_concept<interval_type>::type>::type,
                       interval_type>::type &
  shrink(interval_type& interval, typename interval_traits<interval_type>::coordinate_type shrinking) {
    return bloat(interval, -shrinking);
  }

  /// shrink the specified side of `this` Interval
  template <typename interval_type>
  typename requires_1< typename is_mutable_interval_concept<typename geometry_concept<interval_type>::type>::type,
                       interval_type>::type &
  shrink(interval_type& interval, direction_1d dir, typename interval_traits<interval_type>::coordinate_type shrinking) {
    return bloat(interval, dir, -shrinking);
  }

  /// Enlarge `this` Interval to encompass the specified Interval
  template <typename interval_type, typename interval_type_2>
  typename requires_2< typename is_mutable_interval_concept<typename geometry_concept<interval_type>::type>::type,
                       typename is_interval_concept<typename geometry_concept<interval_type_2>::type>::type,
                       bool>::type
  encompass(interval_type& interval, const interval_type_2& b) {
    bool retval = !contains(interval, b, true);
    low(interval, std::min(low(interval), low(b)));
    high(interval, std::max(high(interval), high(b)));
    return retval;
  }    

  /// Enlarge `this` Interval to encompass the specified Interval
  template <typename interval_type>
  typename requires_1< typename is_mutable_interval_concept<typename geometry_concept<interval_type>::type>::type,
                       bool>::type
  encompass(interval_type& interval, typename interval_traits<interval_type>::coordinate_type b) {
    bool retval = !contains(interval, b, true);
    low(interval, std::min(low(interval), b));
    high(interval, std::max(high(interval), b));
    return retval;
  }    

  /// gets the half of the interval as an interval
  template <typename interval_type>
  typename requires_1<typename is_mutable_interval_concept<typename geometry_concept<interval_type>::type>::type, interval_type>::type 
  get_half(const interval_type& interval, direction_1d d1d) {
    typedef typename interval_traits<interval_type>::coordinate_type Unit;
    Unit c = (get(interval, LOW) + get(interval, HIGH)) / 2;
    return construct<interval_type>((d1d == LOW) ? get(interval, LOW) : c,
                                    (d1d == LOW) ? c : get(interval, HIGH));
  }

  /// returns true if the 2 intervals exactly touch at one value, like in  l1 <= h1 == l2 <= h2
  /// sets the argument to the joined interval
  template <typename interval_type, typename interval_type_2>
  typename requires_2< typename is_mutable_interval_concept<typename geometry_concept<interval_type>::type>::type,
                       typename is_interval_concept<typename geometry_concept<interval_type_2>::type>::type,
                       bool>::type 
  join_with(interval_type& interval, const interval_type_2& b) {
    if(abuts(interval, b)) {
      encompass(interval, b);
      return true;
    }
    return false;
  }

  template <class T>
  template <class T2>
  interval_data<T>& interval_data<T>::operator=(const T2& rvalue) {
    assign(*this, rvalue);
    return *this;
  }

  template <typename T>
  struct geometry_concept<interval_data<T> > {
    typedef interval_concept type;
  };

}
#endif
