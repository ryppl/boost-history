template <typename T>
class point_data {
public:
  typedef T coordinate_type;
  inline point_data(){} 
  inline point_data(coordinate_type x, coordinate_type y) {
    coords_[HORIZONTAL] = x; coords_[VERTICAL] = y; 
  }
  inline point_data(const point_data& that) { (*this) = that; }
  inline point_data& operator=(const point_data& that) {
    coords_[0] = that.coords_[0]; coords_[1] = that.coords_[1]; return *this; 
  }
  template <typename T2>
  inline point_data& operator=(const T2& rvalue);
  inline coordinate_type get(orientation_2d orient) const {
    return coords_[orient.to_int()]; 
  }
  inline void set(orientation_2d orient, coordinate_type value) {
    coords_[orient.to_int()] = value; 
  }
private:
  coordinate_type coords_[2]; 
};

