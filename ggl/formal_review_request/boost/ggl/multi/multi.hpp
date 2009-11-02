// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Copyright Mateusz Loskot 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_MULTI_HPP
#define GGL_MULTI_HPP


#include <ggl/multi/core/geometry_id.hpp>
#include <ggl/multi/core/is_multi.hpp>
#include <ggl/multi/core/point_type.hpp>
#include <ggl/multi/core/ring_type.hpp>
#include <ggl/multi/core/tags.hpp>
#include <ggl/multi/core/topological_dimension.hpp>


#include <ggl/multi/algorithms/area.hpp>
#include <ggl/multi/algorithms/centroid.hpp>
#include <ggl/multi/algorithms/convex_hull.hpp>
#include <ggl/multi/algorithms/correct.hpp>
#include <ggl/multi/algorithms/distance.hpp>
#include <ggl/multi/algorithms/envelope.hpp>
#include <ggl/multi/algorithms/for_each.hpp>
#include <ggl/multi/algorithms/get_section.hpp>
#include <ggl/multi/algorithms/intersection.hpp>
#include <ggl/multi/algorithms/length.hpp>
#include <ggl/multi/algorithms/num_points.hpp>
#include <ggl/multi/algorithms/perimeter.hpp>
#include <ggl/multi/algorithms/remove_holes_if.hpp>
#include <ggl/multi/algorithms/sectionalize.hpp>
#include <ggl/multi/algorithms/simplify.hpp>
#include <ggl/multi/algorithms/transform.hpp>
#include <ggl/multi/algorithms/within.hpp>
#include <ggl/multi/algorithms/detail/modify_with_predicate.hpp>
#include <ggl/multi/algorithms/detail/multi_sum.hpp>
#include <ggl/multi/algorithms/overlay/copy_segments.hpp>
#include <ggl/multi/algorithms/overlay/get_intersection_points.hpp>


#include <ggl/multi/geometries/multi_linestring.hpp>
#include <ggl/multi/geometries/multi_point.hpp>
#include <ggl/multi/geometries/multi_polygon.hpp>

#include <ggl/multi/iterators/range_type.hpp>

#include <ggl/multi/strategies/centroid.hpp>
#include <ggl/multi/strategies/cartesian/centroid_average.hpp>

#include <ggl/multi/util/write_dsv.hpp>



#endif // GGL_MULTI_HPP
