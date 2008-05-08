template <typename T>
class rectangle_data {
public:
  typedef T coordinate_type;
  inline rectangle_data() {}
  template <typename interval_type_1, typename interval_type_2>
  inline rectangle_data(const interval_type_1& hrange,
                        const interval_type_2& vrange) {
    set(HORIZONTAL, hrange); set(VERTICAL, hrange); }

  inline rectangle_data(const rectangle_data& that) { (*this) = that; }

  inline rectangle_data& operator=(const rectangle_data& that) {
    ranges_[0] = that.ranges_[0]; ranges_[1] = that.ranges_[1]; return *this;
  }
  template <typename T2>
  inline rectangle_data& operator=(const T2& rvalue);

  inline interval_data<coordinate_type> get(orientation_2d orient) const {
    return ranges_[orient.to_int()]; }
  template <typename interval_type>
  inline void set(orientation_2d orient, const interval_type& interval); 
private:
  interval_data<coordinate_type> ranges_[2]; 
};

