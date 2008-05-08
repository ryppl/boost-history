template <typename T>
struct interval_traits {
  typedef typename T::coordinate_type coordinate_type;

  static inline coordinate_type get(const T& interval, direction_1d dir) {
    return interval.get(dir); 
  }

  static inline void set(T& interval, direction_1d dir, coordinate_type value) {
    interval.set(dir, value); 
  }

  static inline T construct(coordinate_type low_value, coordinate_type high_value) {
    return T(low_value, high_value); 
  }
};

