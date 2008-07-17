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
struct geometry_concept { 
  typedef typename T::geometry_type type;
};

template <>
struct geometry_concept<int> {
  typedef coordinate_concept type;
};

template <>
struct geometry_concept<long long> {
  typedef coordinate_concept type;
};

}
#endif
