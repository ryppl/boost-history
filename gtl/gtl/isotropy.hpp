/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
enum direction_1d_enum { LOW = 0, HIGH = 1 };
enum orientation_2d_enum { HORIZONTAL = 0, VERTICAL = 1 };
enum direction_2d_enum { WEST = 0, EAST = 1, SOUTH = 2, NORTH = 3 };
enum orientation_3d_enum { PROXIMAL = 2 };
enum winding_direction {
  counterclockwise_winding = 0,
  clockwise_winding = 1,
  unknown_winding = 2
};

class direction_2d;
class orientation_2d;

class direction_1d {
private:
  unsigned int val_;
  explicit direction_1d(int d);
public:
  direction_1d() { val_ = LOW; }
  direction_1d(const direction_1d& that) : val_(that.val_) {}
  direction_1d(const direction_1d_enum val) : val_(val) {}
  explicit direction_1d(const direction_2d& that);
  const direction_1d& operator = (const direction_1d& d) { 
    val_ = d.val_; return * this; }
  bool operator==(direction_1d d) const { return (val_ == d.val_); }
  bool operator!=(direction_1d d) const { return !((*this) == d); }
  unsigned int to_int(void) const { return val_; }
  direction_1d& backward() { val_ ^= 1; return *this; }
};

class direction_2d;

class orientation_2d {
private:
  unsigned int val_;
  explicit orientation_2d(int o);
public:
  orientation_2d() : val_(HORIZONTAL) {}
  orientation_2d(const orientation_2d& ori) : val_(ori.val_) {}
  orientation_2d(const orientation_2d_enum val) : val_(val) {}
  explicit orientation_2d(const direction_2d& that);
  const orientation_2d& operator=(const orientation_2d& ori) {
    val_ = ori.val_; return * this; }
  bool operator==(orientation_2d that) const { return (val_ == that.val_); }
  bool operator!=(orientation_2d that) const { return (val_ != that.val_); }
  unsigned int to_int() const { return (val_); }
  void turn_90() { val_ = val_^ 1; }
  orientation_2d get_perpendicular() const {
    orientation_2d retval = *this;
    retval.turn_90();
    return retval;
  }
  direction_2d get_direction(direction_1d dir) const;
};

class direction_2d {
private:
  int val_;

public:

  direction_2d() { val_ = WEST; }

  direction_2d(const direction_2d& that) : val_(that.val_) {}
  
  direction_2d(const direction_2d_enum val) : val_(val) {}

  const direction_2d& operator=(const direction_2d& d) {
    val_ = d.val_;
    return * this;
  }

  ~direction_2d() { }

  bool operator==(direction_2d d) const { return (val_ == d.val_); }
  bool operator!=(direction_2d d) const { return !((*this) == d); }
  bool operator< (direction_2d d) const { return (val_ < d.val_); }
  bool operator<=(direction_2d d) const { return (val_ <= d.val_); }
  bool operator> (direction_2d d) const { return (val_ > d.val_); }
  bool operator>=(direction_2d d) const { return (val_ >= d.val_); }

  /// Casting to int
  unsigned int to_int(void) const { return val_; }

  direction_2d backward() const {
    // flip the LSB, toggles 0 - 1   and 2 - 3
    return direction_2d(direction_2d_enum(val_ ^ 1));
  }

  /// Returns a direction 90 degree left (LOW) or right(HIGH) to this one
  direction_2d turn(direction_1d t) const {
    return direction_2d(direction_2d_enum(val_ ^ 3 ^ (val_ >> 1) ^ t.to_int()));
  }

  /// Returns a direction 90 degree left to this one
  direction_2d left() const {return turn(HIGH);}

  /// Returns a direction 90 degree right to this one
  direction_2d right() const {return turn(LOW);}

  /// N, E are positive, S, W are negative
  bool is_positive() const {return (val_ & 1);}
  bool is_negative() const {return !is_positive();}
  int get_sign() const {return ((is_positive()) << 1) -1;}

};

direction_1d::direction_1d(const direction_2d& that) {
  val_ = that.to_int() & 1; 
}
orientation_2d::orientation_2d(const direction_2d& that) {
  val_ = that.to_int() >> 1; 
}

direction_2d orientation_2d::get_direction(direction_1d dir) const {
  return direction_2d(direction_2d_enum((val_ << 1) + dir.to_int()));
}

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

