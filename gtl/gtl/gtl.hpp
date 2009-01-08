/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
#ifndef GTL_GTL_HPP
#define GTL_GTL_HPP

//external
#include <math.h>
#include <vector>
#include <deque>
#include <map>
#include <set>
#include <list>
#include <iostream>
#include <algorithm>
#include <limits>
#include <iterator>

#ifdef __ICC
#pragma warning (disable:1125)
#endif

#include "isotropy.hpp"

//point
#include "point_data.hpp"
#include "point_traits.hpp"
#include "point_concept.hpp"

//point 3d
#include "point_3d_data.hpp"
#include "point_3d_traits.hpp"
#include "point_3d_concept.hpp"

#include "transform.hpp"
#include "transform_detail.hpp"

//interval
#include "interval_data.hpp"
#include "interval_traits.hpp"
#include "interval_concept.hpp"

//rectangle
#include "rectangle_data.hpp"
#include "rectangle_traits.hpp"
#include "rectangle_concept.hpp"

//algorithms needed by polygon types
#include "iterator_points_to_compact.hpp"
#include "iterator_compact_to_points.hpp"

//polygons
#include "polygon_45_data.hpp"
#include "polygon_data.hpp"
#include "polygon_90_data.hpp"
#include "polygon_90_with_holes_data.hpp"
#include "polygon_45_with_holes_data.hpp"
#include "polygon_with_holes_data.hpp"
#include "polygon_traits.hpp"

//manhattan boolean algorithms
#include "boolean_op.hpp"
#include "polygon_formation.hpp"
#include "rectangle_formation.hpp"
#include "max_cover.hpp"
#include "property_merge.hpp"
#include "polygon_90_touch.hpp"
#include "iterator_geometry_to_set.hpp"

//45 boolean op algorithms
#include "boolean_op_45.hpp"
#include "polygon_45_formation.hpp"

//polygon set data types
#include "polygon_90_set_data.hpp"
//polygon set trait types
#include "polygon_90_set_traits.hpp"
//polygon set concepts
#include "polygon_90_set_concept.hpp"
//boolean operator syntax
#include "polygon_90_set_view.hpp"

//45 boolean op algorithms
#include "boolean_op_45.hpp"
#include "polygon_45_formation.hpp"
#include "polygon_45_set_data.hpp"
#include "polygon_45_set_traits.hpp"
#include "polygon_45_set_concept.hpp"
#include "polygon_45_set_view.hpp"

//arbitrary polygon algorithms
#include "polygon_arbitrary_formation.hpp"
#include "polygon_set_data.hpp"

//general scanline
#include "scan_arbitrary.hpp"
#include "polygon_set_traits.hpp"
#include "polygon_set_view.hpp"

#include "polygon_set_concept.hpp"

#if __ICC
#pragma warning (default:1125)
#endif
#endif
