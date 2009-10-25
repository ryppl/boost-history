// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_STRATEGIES_HPP
#define GGL_STRATEGIES_HPP


#include <ggl/strategies/strategy_traits.hpp>

#include <ggl/strategies/cartesian/area_by_triangles.hpp>
#include <ggl/strategies/cartesian/centroid_bashein_detmer.hpp>
#include <ggl/strategies/cartesian/cart_compare.hpp>
#include <ggl/strategies/cartesian/cart_distance.hpp>
#include <ggl/strategies/cartesian/cart_envelope.hpp>
#include <ggl/strategies/cartesian/cart_side.hpp>
#include <ggl/strategies/cartesian/point_in_poly_franklin.hpp>
#include <ggl/strategies/cartesian/point_in_poly_crossings_multiply.hpp>

#include <ggl/strategies/spherical/distance_haversine.hpp>
#include <ggl/strategies/spherical/distance_cross_track.hpp>

#include <ggl/strategies/spherical/area_huiller.hpp>
#include <ggl/strategies/spherical/sph_envelope.hpp>

#include <ggl/strategies/agnostic/agn_convex_hull.hpp>
#include <ggl/strategies/agnostic/simplify_douglas_peucker.hpp>
#include <ggl/strategies/agnostic/point_in_poly_winding.hpp>

#include <ggl/strategies/strategy_transform.hpp>

#include <ggl/strategies/transform/matrix_transformers.hpp>
#include <ggl/strategies/transform/map_transformer.hpp>
#include <ggl/strategies/transform/inverse_transformer.hpp>


#endif // GGL_STRATEGIES_HPP
