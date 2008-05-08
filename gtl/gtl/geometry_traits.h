struct no_type {};

template <typename T>
struct geometry_traits { 
  typedef no_type geometry_concept;
};
template <>
struct geometry_traits<point_data<int> > {
  typedef point_concept geometry_concept;
  typedef int point_get_retval;
};
template <>
struct geometry_traits<point_data<long long> > {
  typedef point_concept geometry_concept;
  typedef long long point_get_retval;
};
template <>
struct geometry_traits<rectangle_data<int> > {
  typedef rectangle_concept geometry_concept;
  typedef interval_data<int> rectangle_get_retval;
};
template <>
struct geometry_traits<rectangle_data<long long> > {
  typedef rectangle_concept geometry_concept;
  typedef interval_data<long long> rectangle_get_retval;
};
template <>
struct geometry_traits<interval_data<int> > {
  typedef interval_concept geometry_concept;
};
template <>
struct geometry_traits<interval_data<long long> > {
  typedef interval_concept geometry_concept;
};
template <>
struct geometry_traits<polygon_data<int> > {
  typedef polygon_concept geometry_concept;
};
template <>
struct geometry_traits<polygon_data<long long> > {
  typedef polygon_concept geometry_concept;
};
template <>
struct geometry_traits<point_3d_data<int> > {
  typedef point_3d_concept geometry_concept;
};
template <>
struct geometry_traits<point_3d_data<long long> > {
  typedef point_3d_concept geometry_concept;
};
 

