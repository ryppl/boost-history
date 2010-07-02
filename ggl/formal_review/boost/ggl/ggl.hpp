// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Copyright (c) 2009 Mateusz Loskot <mateusz@loskot.net>
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_GGL_HPP
#define GGL_GGL_HPP

// Shortcut to include all header files
#if defined(_MSC_VER) || defined(__BORLANDC__) || defined(__DMC__)
#  pragma message ("Warning: Obsolete! Please use http://svn.boost.org/svn/boost/sandbox/geometry/")
#elif defined(__GNUC__) || defined(__HP_aCC) || defined(__SUNPRO_CC) || defined(__IBMCPP__)
#  warning "Warning: Obsolete! Please use http://svn.boost.org/svn/boost/sandbox/geometry/"
#endif


#include <ggl/core/cs.hpp>
#include <ggl/core/tag.hpp>
#include <ggl/core/tags.hpp>

#include <ggl/core/access.hpp>
#include <ggl/core/radian_access.hpp>
#include <ggl/core/topological_dimension.hpp>

#include <ggl/core/replace_point_type.hpp>

#include <ggl/arithmetic/arithmetic.hpp>
#include <ggl/arithmetic/dot_product.hpp>

#include <ggl/strategies/strategies.hpp>

#include <ggl/algorithms/append.hpp>
#include <ggl/algorithms/area.hpp>
#include <ggl/algorithms/assign.hpp>
#include <ggl/algorithms/centroid.hpp>
#include <ggl/algorithms/clear.hpp>
#include <ggl/algorithms/convert.hpp>
#include <ggl/algorithms/convex_hull.hpp>
#include <ggl/algorithms/correct.hpp>
#include <ggl/algorithms/distance.hpp>
#include <ggl/algorithms/envelope.hpp>
#include <ggl/algorithms/for_each.hpp>
#include <ggl/algorithms/intersection.hpp>
#include <ggl/algorithms/intersects.hpp>
#include <ggl/algorithms/length.hpp>
#include <ggl/algorithms/make.hpp>
#include <ggl/algorithms/num_points.hpp>
#include <ggl/algorithms/perimeter.hpp>
#include <ggl/algorithms/sectionalize.hpp>
#include <ggl/algorithms/simplify.hpp>
#include <ggl/algorithms/transform.hpp>
#include <ggl/algorithms/within.hpp>

// check includes all concepts
#include <ggl/geometries/concepts/check.hpp>

#include <ggl/util/copy.hpp>
#include <ggl/util/for_each_coordinate.hpp>
#include <ggl/util/math.hpp>
#include <ggl/util/select_most_precise.hpp>
#include <ggl/util/select_coordinate_type.hpp>
#include <ggl/util/write_dsv.hpp>

#endif // GGL_GGL_HPP