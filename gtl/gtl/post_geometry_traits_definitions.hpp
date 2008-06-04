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
  registration<geometry_type>::component_type type;
};

template <typename geometry_type>
struct coordinate_type { 
  typedef typename geometry_concept<geometry_type>::type::template
  registration<geometry_type>::coordinate_type type;
};

template <typename geometry_type>
struct center_type { 
  typedef typename geometry_concept<geometry_type>::type::template
  registration<geometry_type>::center_type type;
};

}
#endif

