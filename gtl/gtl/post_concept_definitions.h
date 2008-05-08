template <typename T>
template <typename interval_type>
inline void rectangle_data<T>::set(orientation_2d orient, const interval_type& interval) {
  ranges_[orient.to_int()].set(LOW, interval_concept::get(interval, LOW));
  ranges_[orient.to_int()].set(HIGH, interval_concept::get(interval, HIGH));
}

template <class T>
template <class T2>
point_data<T>& point_data<T>::operator=(const T2& rvalue) {
  point_concept::assign(*this, rvalue);
  return *this;
}

template <class T>
template <class T2>
interval_data<T>& interval_data<T>::operator=(const T2& rvalue) {
  interval_concept::assign(*this, rvalue);
  return *this;
}

template <class T>
template <class T2>
rectangle_data<T>& rectangle_data<T>::operator=(const T2& rvalue) {
  rectangle_concept::assign(*this, rvalue);
  return *this;
}

template <class T>
template <class T2>
point_3d_data<T>& point_3d_data<T>::operator=(const T2& rvalue) {
  point_3d_concept::assign(*this, rvalue);
  return *this;
}

template <class T>
template <class T2>
polygon_data<T>& polygon_data<T>::operator=(const T2& rvalue) {
  polygon_concept::assign(*this, rvalue);
  return *this;
}

