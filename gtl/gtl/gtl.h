/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
//external
#include <vector>
#include <deque>
#include <map>
#include <set>
#include <list>
#include <iostream>
#include <algorithm>
#include <limits>
#include <iterator>

//isotropy types
#include "isotropy.h"

//data types
#include "point_data.h"
#include "interval_data.h"
#include "rectangle_data.h"
#include "point_3d_data.h"
#include "polygon_data.h"
#include "polygon_with_holes_data.h"

//traits types
#include "point_traits.h"
#include "interval_traits.h"
#include "rectangle_traits.h"
#include "point_3d_traits.h"
#include "polygon_traits.h"
#include "polygon_with_holes_traits.h"

//concept types
#include "point_concept.h"
#include "interval_concept.h"
#include "rectangle_concept.h"
#include "point_3d_concept.h"

//algorithms needed by polygon concepts
#include "iterator_points_to_compact.h"
#include "iterator_compact_to_points.h"

//polygon concept types
#include "polygon_concept.h"
#include "polygon_with_holes_concept.h"

//definitions
#include "post_concept_definitions.h"

//manhattan boolean op algorithms
#include "boolean_op.h"
#include "polygon_formation.h"
#include "rectangle_formation.h"

//geometry traits
#include "geometry_traits.h"

//defintions
#include "post_geometry_traits_definitions.h"


template <typename geometry_type_1, typename geometry_type_2>
bool contains(const geometry_type_1& geometry_object, const geometry_type_2& contained_geometry_object, 
              bool consider_touch = true) {
  typename geometry_traits<geometry_type_1>::geometry_concept().contains(geometry_object, contained_geometry_object,
                                                                         consider_touch, typename geometry_traits<geometry_type_2>::geometry_concept());
}

template <typename geometry_type_1, typename geometry_type_2>
bool equivilence(const geometry_type_1& lvalue, const geometry_type_2& rvalue) {
  typename geometry_traits<geometry_type_1>::geometry_concept concept_instantiation;
  return concept_instantiation.equivilence(lvalue, rvalue);
}

template <typename geometry_type>
typename component_type<geometry_type>::type
get(const geometry_type& geometry_object, orientation_2d orient) {
  return geometry_traits<geometry_type>::geometry_concept::get(geometry_object, orient);
}

template <typename geometry_type>
typename component_type<geometry_type>::type
get(const geometry_type& geometry_object, orientation_3d orient) {
  return geometry_traits<geometry_type>::geometry_concept::get(geometry_object, orient);
}

template <typename geometry_type_1, typename geometry_type_2>
geometry_type_1& assign(geometry_type_1& lvalue, geometry_type_2& rvalue) {
  return geometry_traits<geometry_type_1>::geometry_concept::assign(lvalue, rvalue);
}

template <typename geometry_type_1, typename geometry_type_2>
geometry_type_1& operator|(geometry_type_1& lvalue, const geometry_type_2& rvalue) {
  return assign(lvalue, rvalue);
}
 

