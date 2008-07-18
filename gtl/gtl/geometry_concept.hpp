/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
#ifndef GTL_GEOMETRY_CONCEPT_HPP
#define GTL_GEOMETRY_CONCEPT_HPP
namespace gtl {
template <typename T>
struct geometry_concept { }; //has to be empty by default

template <>
struct geometry_concept<int> {
  typedef coordinate_concept type;
};

template <>
struct geometry_concept<long long> {
  typedef coordinate_concept type;
};

template <typename T>
struct geometry_concept<interval_data<T> > {
  typedef interval_concept type;
};

template <typename T>
struct geometry_concept<point_data<T> > {
  typedef point_concept type;
};

template <typename T>
struct geometry_concept<rectangle_data<T> > {
  typedef rectangle_concept type;
};

template <typename T>
struct geometry_concept<point_3d_data<T> > {
  typedef point_3d_concept type;
};

template <typename T>
struct geometry_concept<polygon_data<T> > {
  typedef polygon_concept type;
};

template <typename T>
struct geometry_concept<polygon_with_holes_data<T> > {
  typedef polygon_with_holes_concept type;
};

template <typename T>
struct geometry_concept<polygon_45_data<T> > {
  typedef polygon_45_concept type;
};

template <typename T>
struct geometry_concept<polygon_45_with_holes_data<T> > {
  typedef polygon_45_with_holes_concept type;
};

template <typename T>
struct geometry_concept<polygon_90_data<T> > {
  typedef polygon_90_concept type;
};

template <typename T>
struct geometry_concept<polygon_90_with_holes_data<T> > {
  typedef polygon_90_with_holes_concept type;
};

}
#endif
