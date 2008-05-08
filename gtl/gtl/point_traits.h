template <typename T>
struct point_traits {
  typedef typename T::coordinate_type coordinate_type;
  
  static inline coordinate_type get(const T& point, orientation_2d orient) {
    return point.get(orient); 
  }
  static inline void set(T& point, orientation_2d orient, coordinate_type value) {
    point.set(orient, value); 
  }
  static inline T construct(coordinate_type x_value, coordinate_type y_value) {
    return T(x_value, y_value); 
  }
};

