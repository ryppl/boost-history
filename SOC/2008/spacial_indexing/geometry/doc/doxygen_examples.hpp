// Geometry Library
//
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef _DOXYGEN_EXAMPLES_HPP
#define _DOXYGEN_EXAMPLES_HPP


/*!
\example 1_point_example.cpp
In most cases the documentation gives small examples of how to use the algorithms or classes.
The point example is a slightly larger example giving the an idea of how to use different
algorithms from the library, related to points. It shows
- the usage of include files
- how to declare points, using different coordinate types
- how to construct points, specifying coordinates, initializing to zero or to infinite
- how to compare points to each other
- how points can be streamed as OGC text
- calculating the distance from point to point
*/


//---------------------------------------------------------------------------------------------------

/*!
\example 2_point_ll_example.cpp
This example explains how to use point_ll latlong points. Those points can be specified in
degrees or in radians. Calculations are usually done using radians. Storage is usually in
degrees. There is a conversion method available.

Furthermore, there are specific measures to avoid confusion between lat/lon, lat with east/west, etc:
- There is a constructor using latitude / longitude
- There is a constructor using longitude / latitude
- There is \b no constructor using two floating point values
- The latitude using DMS class should have north or south
- The longitude using DMS class should have east or west
*/

//---------------------------------------------------------------------------------------------------

/*!
\example 3_linestring_example.cpp
The linestring example shows how linestrings can be declared and used and shows some more algorithms.
One of the important concepts of the geometry library is that it is totally built upon the standard
library, using the standard containers such as std::vector.

A linestring is, as explained elsewhere in this documentation, not much more than a vector of points.
Most algorithms run on linestrings, but can also run on any iterator pair. And all algorithms
on std::vector can be used on geometry::linestring.

The sample shows this, shows some algorithms:
- geometry::envelope
- geometry::length
- geometry::distance
- geometry::simplify
- geometry::for_each
- geometry::intersection

This documentation illustrates the simplify algorithm and the intersection algorithm with some pictures.

The simplify algorithm simplifies a linestring. Simplification means that the less important points
are removed from the line and that the points that are most important for the shape of a line are
kept. Simplification is done using the well known Douglas Peucker algorithm. The library user can
specify the distance or tolerance, which indicates how much the linestring should be simplified.

The image below shows the original and simplified linestring:
\image html simplify_example.png
The red line is the original linestring; the blue line is the simplified line which has one point less.
In geographical applications simplification can reduce a linestring to its basic form containing only
10% of its original points.

The intersection algorithm intersects two geometries which each other, delivering a third geometry.
In the case of the example a linestring is intersected with a box. Intersection with a box is often
called a clip. The image below illustrates the intersection.
\image html intersection_linestring_example.png
The red line is intersected with the blue box. The intersection result, painted in black, is a multi_linestring containing three linestrings.
*/

//---------------------------------------------------------------------------------------------------

/*!
\example 4_polygon_example.cpp
The polygon example shows some examples of what can be done with polygons in the Geometry Library:
* the outer ring and the inner rings
* how to calculate the area of a polygon
* how to get the centroid, and how to get an often more interesting label point
* how to correct the polygon such that it is clockwise and closed
* within: the well-known point in polygon algorithm
* how to use polygons which use another container, or which use different containers for points and for inner rings
* how polygons can be intersected, or clipped, using a clipping box

The illustrations below show the usage of the within algorithm and the intersection algorithm.

The within algorithm results in true if a point lies completly within a polygon. If it lies exactly
on a border it is not considered as within and if it is inside a hole it is also not within the
polygon. This is illustrated below, where only the point in the middle is within the polygon.

\image html within_polygon_example.png

The clipping algorithm, called intersection, is illustrated below:

\image html clip_polygon_example.png

The yellow polygon, containing a hole, is clipped with the blue rectangle, resulting in a
multi_polygon of three polygons, drawn in red. The hole is vanished.

include polygon_example.cpp
*/


//---------------------------------------------------------------------------------------------------

/*!
\example 5_distance_example.cpp
The distance example shows how the distance algorithms can be used for point_xy points, for
point_ll points, with our without explicitly specified strategies
*/

//---------------------------------------------------------------------------------------------------
 /*!
\example 6_transformation_example.cpp
This sample demonstrates the usage of the Geometry Library in combination with the
uBLAS matrix/vector library.

The Geometry Library does not define vector/matrix calculations. The reason for that is they are 
already there. Boost contains one in uBLAS. Furthermore there is a MTL. And there are many more 
available.

So the Geometry Library can be used in combination with matrix/vector libraries. It supports the
algorithm \b for_each_point which iterates through all points of a 
container, of a polygon, and it can visit a point as well (for consistancy). for_each_point can be 
used for transformations or projections.

The Geometry Library also uses simple matrix/vector calculations in its algorithms. For example
the dot-product is used, a determinant is calculated in the intersection calculation of segments,
and a cross-product will be used.

At this moment this those aritmethics calculated in the library. Until now we didn't want
to introduce a library for this, because it was not completely clear which library. That might change.
*/


#endif // _DOXYGEN_EXAMPLES_HPP
