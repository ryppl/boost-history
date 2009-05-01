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

#ifdef WIN32
#pragma warning( disable: 4996 )
#pragma warning( disable: 4800 )
#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif
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
#include "polygon_45_touch.hpp"
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

/// \mainpage gtl -- Geometry Template Library
/// The geometry template library is a Concepts based typesystem that implements that API and algorithms for a large number of planar (2D) geometry operations.  The primary algorithms provided by gtl are polygon set operations, so-called Booleans, such as polygon intersection and union.  There are three classes of Booleans and associated types, which are axis-parallel (Manhattan), 45-degree restricted and arbitrary angle polygon operations.  This is done because it is significantly more efficient to process geometry data when assumptions about whether it is axis-parallel or restricted to 45-degree can be made.

/// \file "gtl.hpp"
/// \brief Includes all gtl header files in the correct order to use all gtl features.  Include gtl.hpp to treat gtl like a package.

/// \file "isotropy.hpp"
/// \brief Defines abstract ideas such as orientation and direction as data types that are used extensively to aid geometric programming.

//point
/// \file "point_data.hpp"
/// \brief Defines a data structure that models point_concept by satisfying the default point_traits.
/// \file "point_traits.hpp"
/// \brief Defines the point_traits that must be satisfied for an object to model point_concept.
/// \file "point_concept.hpp"
/// \brief Defines behaviors specific to objects that model point_concept.

//point 3d
/// \file "point_3d_data.hpp"
/// \brief Defines a data structure that models point_3d_concept by satisfying the default point_3d_traits.
/// \file "point_3d_traits.hpp"
/// \brief Defines the point_3d_traits that must be satisfied for an object to model point_3d_concept.
/// \file "point_3d_concept.hpp"
/// \brief Defines behaviors specific to objects that model point_3d_concept.

/// \file "transform.hpp"
/// \brief Defines transformations of cartesian coordinate systems through several types
/// \file "transform_detail.hpp"
/// \brief Details of transforms implementation

//interval
/// \file "interval_data.hpp"
/// \brief Defines a data structure that models interval_concept by satisfying the default interval_traits.
/// \file "interval_traits.hpp"
/// \brief Defines the interval_traits that must be satisfied for an object to model interval_concept.
/// \file "interval_concept.hpp"
/// \brief Defines behaviors specific to objects that model interval_concept.

//rectangle
/// \file "rectangle_data.hpp"
/// \brief Defines a data structure that models rectangle_concept by satisfying the default rectangle_traits.
/// \file "rectangle_traits.hpp"
/// \brief Defines the rectangle_traits that must be satisfied for an object to model rectangle_concept.
/// \file "rectangle_concept.hpp"
/// \brief Defines behaviors specific to objects that model rectangle_concept.

//algorithms needed by polygon types
/// \file "iterator_points_to_compact.hpp"
/// \brief Details of an iterator filter that converts a sequence of points of a manhattan polygon to non-redundant coordinate values.
/// \file "iterator_compact_to_points.hpp"
/// \brief Details of an iterator filter that converts a sequence of non-redundant coordinate values to the points of a manhattan polygon.

//polygons
/// \file "polygon_45_data.hpp"
/// \brief Defines a data structure that models polygon_45_concept by satisfying the default rectangle_traits.
/// \file "polygon_data.hpp"
/// \brief Defines a data structure that models polygon_concept by satisfying the default rectangle_traits.
/// \file "polygon_90_data.hpp"
/// \brief Defines a data structure that models polygon_90_concept by satisfying the default rectangle_traits.
/// \file "polygon_90_with_holes_data.hpp"
/// \brief Defines a data structure that models polygon_90_with_holes_concept by satisfying the default rectangle_traits.
/// \file "polygon_45_with_holes_data.hpp"
/// \brief Defines a data structure that models polygon_45_with_holes_concept by satisfying the default rectangle_traits.
/// \file "polygon_with_holes_data.hpp"
/// \brief Defines a data structure that models polygon_with_holes_concept by satisfying the default rectangle_traits.
/// \file "polygon_traits.hpp"
/// \brief Defines the traits that must be satisfied for an object to model any of the polygonal concepts as well as behaviors of those concepts.

//manhattan boolean algorithms
/// \file "boolean_op.hpp"
/// \brief Details of the generic framework for scanline algorithm on manhattan polygonal data.
/// \file "polygon_formation.hpp"
/// \brief Details of the scanline algorithm for forming polygons from manhattan polygonal data and associating holes to outer shells.
/// \file "rectangle_formation.hpp"
/// \brief Details of the scanline algorithm for forming rectangles from manhattan polygonal data. 
/// \file "max_cover.hpp"
/// \brief Details of the visibility algorithm for computing largest enclosed rectangles contained within manhattan polygonal data.
/// \file "property_merge.hpp"
/// \brief Details of the scanline algorithm for merging properties from many manhattan polygonal data inputs. 
/// \file "polygon_90_touch.hpp"
//
/// \file "iterator_geometry_to_set.hpp"
//

//45 boolean op algorithms
/// \file "boolean_op_45.hpp"
//
/// \file "polygon_45_formation.hpp"

//polygon set data types
/// \file "polygon_90_set_data.hpp"
//polygon set trait types
/// \file "polygon_90_set_traits.hpp"
//polygon set concepts
/// \file "polygon_90_set_concept.hpp"
//boolean operator syntax
/// \file "polygon_90_set_view.hpp"
//

//45 boolean op algorithms
/// \file "boolean_op_45.hpp"
//
/// \file "polygon_45_formation.hpp"
//
/// \file "polygon_45_set_data.hpp"
//
/// \file "polygon_45_set_traits.hpp"
//
/// \file "polygon_45_touch.hpp"
//
/// \file "polygon_45_set_concept.hpp"
//
/// \file "polygon_45_set_view.hpp"
//

//arbitrary polygon algorithms
/// \file "polygon_arbitrary_formation.hpp"
//
/// \file "polygon_set_data.hpp"
//

//general scanline
/// \file "scan_arbitrary.hpp"
//
/// \file "polygon_set_traits.hpp"
//
/// \file "polygon_set_view.hpp"
//

/// \file "polygon_set_concept.hpp"
//


/// \defgroup d_concepts Concepts
/// \brief These are the geometry concepts provided by gtl.
/// \details A geometry concept is the idea of a geometry, a given data structure may model a certain geometry concept if it is able to provide a complete concept traits definition for that concept.
/// @{
/// \struct coordinate_concept 
/// \brief coordinate_concept is a numeric
/// \details Behaviors specific to coordinate_concept are defined in \ref isotropy.hpp
/// defgroup d_polygon_concept_functions Polygon Concept Functions

/// \struct interval_concept 
/// \brief interval_concept is one dimensional interval
/// \details Behaviors specific to interval_concept are defined in \ref interval_concept.hpp
/// \par An interval is inclusive of all the values one the number line between its low and high coordinate values.

/// \struct point_concept
/// \brief point_concept is two dimensional point
//
/// \struct point_3d_concept
/// \brief point_3d_concept is two dimensional point
//
/// \struct rectangle_concept
/// \brief rectangle_concept is two dimensional axis-parallel (Manhattan) rectangle often used as a bounding box
//
/// \struct polygon_concept
/// \brief polygon_concept is a two dimensional polygon
//
/// \struct polygon_with_holes_concept
/// \brief polygon_with_holes_concept is a refinement of polygon_concept and extends it with holes that are themselves two dimensional polygons
//
/// \struct polygon_45_concept
/// \brief polygon_45_concept is a refinement of polygon_concept and extends it with the restriction that angles formed at corners are multiples of 45 degrees
//
/// \struct polygon_45_with_holes_concept
/// \brief polygon_45_with_holes_concept is a refinement of polygon_45_concept and polygon_with_holes_concept and extends them with the restriction that holes are of conceptual type polygon_45_concept
//
/// \struct polygon_90_concept
/// \brief polygon_90_concept is a refinement of polygon_45_concept and extends it with the restriction that angles formed at corners are right angles and edges are axis-parallel (Manhattan)
//
/// \struct polygon_90_with_holes_concept
/// \brief polygon_90_with_holes_concept is a refinement of polygon_90_concept and polygon_45_with_holes_concept and extends them with the restriction that holes are of conceptual type polygon_90_concept
//
/// @}
//

#if __ICC
#pragma warning (default:1125)
#endif
#endif
