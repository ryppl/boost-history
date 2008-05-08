template <typename T>
class interval_data {
public:
  typedef T coordinate_type;
  inline interval_data(){} 
  inline interval_data(coordinate_type low, coordinate_type high) {
    coords_[LOW] = low; coords_[HIGH] = high; 
  }
  inline interval_data(const interval_data& that) {
    (*this) = that; 
  }
  inline interval_data& operator=(const interval_data& that) {
    coords_[0] = that.coords_[0]; coords_[1] = that.coords_[1]; return *this; 
  }
  template <typename T2>
  inline interval_data& operator=(const T2& rvalue);
  inline coordinate_type get(direction_1d dir) const {
    return coords_[dir.to_int()]; 
  }
  inline void set(direction_1d dir, coordinate_type value) {
    coords_[dir.to_int()] = value; 
  }
private:
  coordinate_type coords_[2]; 
};

