/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
struct no_type {};

template <typename T>
struct geometry_traits { 
  typedef no_type geometry_concept;
};

template <>
struct geometry_traits<point_data<int> > {
  typedef point_concept geometry_concept;
};
template <>
struct geometry_traits<point_data<long long> > {
  typedef point_concept geometry_concept;
};
template <>
struct geometry_traits<rectangle_data<int> > {
  typedef rectangle_concept geometry_concept;
};
template <>
struct geometry_traits<rectangle_data<long long> > {
  typedef rectangle_concept geometry_concept;
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
struct geometry_traits<point_3d_data<int> > {
  typedef point_3d_concept geometry_concept;
};
template <>
struct geometry_traits<point_3d_data<long long> > {
  typedef point_3d_concept geometry_concept;
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
struct geometry_traits<polygon_with_holes_data<int> > {
  typedef polygon_with_holes_concept geometry_concept;
};
template <>
struct geometry_traits<polygon_with_holes_data<long long> > {
  typedef polygon_with_holes_concept geometry_concept;
};
template<>
struct geometry_traits<polygon_formation::PolyLinePolygonData<true, int> > {
  typedef polygon_with_holes_concept geometry_concept;
};
template<>
struct geometry_traits<polygon_formation::PolyLinePolygonData<false, int> > {
  typedef polygon_with_holes_concept geometry_concept;
};
template<>
struct geometry_traits<polygon_formation::PolyLinePolygonData<true, long long> > {
  typedef polygon_with_holes_concept geometry_concept;
};
template<>
struct geometry_traits<polygon_formation::PolyLinePolygonData<false, long long> > {
  typedef polygon_with_holes_concept geometry_concept;
};
template <>
struct geometry_traits<polygon_formation::PolyLineHoleData<true, int> > {
  typedef polygon_concept geometry_concept;
};
template <>
struct geometry_traits<polygon_formation::PolyLineHoleData<false, int> > {
  typedef polygon_concept geometry_concept;
};
template <>
struct geometry_traits<polygon_formation::PolyLineHoleData<true, long long> > {
  typedef polygon_concept geometry_concept;
};
template <>
struct geometry_traits<polygon_formation::PolyLineHoleData<false, long long> > {
  typedef polygon_concept geometry_concept;
};

