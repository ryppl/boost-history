enum orientation_2d_enum { HORIZONTAL = 0, VERTICAL = 1 };
enum direction_1d_enum { LOW = 0, HIGH = 1 };
enum orientation_3d_enum { PROXIMAL = 2 };
enum winding_direction {
  counterclockwise_winding = 0,
  clockwise_winding = 1,
  unknown_winding = 2
};


class orientation_2d {
private:
  unsigned int val_;
  explicit orientation_2d(int o);
public:
  orientation_2d() : val_(HORIZONTAL) {}
  orientation_2d(const orientation_2d& ori) : val_(ori.val_) {}
  orientation_2d(const orientation_2d_enum val) : val_(val) {}
  const orientation_2d& operator=(const orientation_2d& ori) {
    val_ = ori.val_; return * this; }
  bool operator==(orientation_2d that) const { return (val_ == that.val_); }
  bool operator!=(orientation_2d that) const { return (val_ != that.val_); }
  unsigned int to_int() const { return (val_); }
  void turn_90() { val_ = val_^ 1; }
};

class direction_1d {
private:
  unsigned int val_;
  explicit direction_1d(int d);
public:
  direction_1d() { val_ = LOW; }
  direction_1d(const direction_1d& that) : val_(that.val_) {}
  direction_1d(const direction_1d_enum val) : val_(val) {}
  const direction_1d& operator = (const direction_1d& d) { 
    val_ = d.val_; return * this; }
  bool operator==(direction_1d d) const { return (val_ == d.val_); }
  bool operator!=(direction_1d d) const { return !((*this) == d); }
  unsigned int to_int(void) const { return val_; }
  direction_1d& backward() { val_ ^ 1; }
};

class orientation_3d {
private:
  unsigned int val_;
  explicit orientation_3d(int o);
public:
  orientation_3d() : val_((int)HORIZONTAL) {}
  orientation_3d(const orientation_3d& ori) : val_(ori.val_) {}
  orientation_3d(const orientation_2d& ori) { val_ = ori.to_int(); }
  orientation_3d(const orientation_3d_enum val) : val_(val) {}
  ~orientation_3d() {  }
  const orientation_3d& operator=(const orientation_3d& ori) { 
    val_ = ori.val_; return * this; }
  bool operator==(orientation_3d that) const { return (val_ == that.val_); }
  bool operator!=(orientation_3d that) const { return (val_ != that.val_); }
  unsigned int to_int() const { return (val_); }
};

