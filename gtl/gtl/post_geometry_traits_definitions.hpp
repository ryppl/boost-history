/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
#ifndef GTL_POST_GEOMETRY_TRAITS_DEFINITIONS_HPP
#define GTL_POST_GEOMETRY_TRAITS_DEFINITIONS_HPP
namespace gtl {

template <typename geometry_type>
struct component_type { 
  typedef typename geometry_concept<geometry_type>::type::template
  component_type<geometry_type>::type type;
};

template <typename geometry_type>
struct coordinate_type { 
  typedef typename geometry_concept<geometry_type>::type::template
  coordinate_type<geometry_type>::type type;
};

template <typename geometry_type>
struct center_type { 
  typedef typename geometry_concept<geometry_type>::type::template
  center_type<geometry_type>::type type;
};

template <typename geometry_type>
struct area_type {
  typedef typename geometry_concept<geometry_type>::type::template
  area_type<geometry_type>::type type;
};

template <typename geometry_type>
struct distance_type {
  typedef typename geometry_concept<geometry_type>::type::template
  distance_type<geometry_type>::type type;
};

template <typename geometry_type>
struct difference_type {
  typedef typename geometry_concept<geometry_type>::type::template
  difference_type<geometry_type>::type type;
};

}
#endif

