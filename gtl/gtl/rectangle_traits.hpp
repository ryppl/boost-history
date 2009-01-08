/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
#ifndef GTL_RECTANGLE_TRAITS_HPP
#define GTL_RECTANGLE_TRAITS_HPP
namespace gtl {

  template <typename T>
  struct rectangle_traits {
    typedef typename T::coordinate_type coordinate_type;
    typedef typename T::interval_type interval_type;
    static inline interval_type get(const T& rectangle, orientation_2d orient) {
      return rectangle.get(orient); }
  };

  template <typename T>
  struct rectangle_mutable_traits {
    template <typename T2>
    static inline void set(T& rectangle, orientation_2d orient, const T2& interval) {
      rectangle.set(orient, interval); }
    template <typename T2, typename T3>
    static inline T construct(const T2& interval_horizontal,
                              const T3& interval_vertical) {
      return T(interval_horizontal, interval_vertical); }
  };

}
#endif

