// Geometry Library
//
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef _DOXYGEN_PAGES_HPP
#define _DOXYGEN_PAGES_HPP


/*!
\page points Points, point types, strategies, and algorithms
This page explains the design of the library with respect to the generic point,
derived point types, and strategies and algorithms.
Note that, for clarity, the
listings do not list the complete classes. Constructors, for example, are skipped.
Listed methods are public methods.
\n\n \section point class point
The class point is templatized by coordinate-type and dimension. It is generic and coordinate
access is implemented in a neutral way, using \b value methods.
\dontinclude geometry.hpp
\skipline template<typename T, size_t D>
\until {
There are two member function templates to get and set values:
\skipline template <size_t K>
\until }
\skipline template <size_t K>
\until }
In loops, compile-time access doesn't work. So for use in loops, there are also member
functions with an index to get and set values:
\skipline void ivalue
\until }
\skipline ivalue
\until }
The coordinates themselves are stored using an array
\skipline private
\until };

\n \section point_point_xy class point_xy for Cartesian 2D points
Library users will normally work with derived points. The point_xy point is derived from the
generic point and exposes its coordinates as x- and y-values. (<em>The this->template
constructs are standard and required for some compilers.</em>)
\skipline template<typename T>
\until {
\skipline inline const T& x()
\until value<1>(v); }
\skip };
\line };

\n \section xystrategy Strategy for point_xy for distance calculation
\n For each point type, there are strategies for primitive operations. Take, for example, the
distance algorithm. There is a strategy for point_xy points implementing this algorithm
(which resides in namespace strategy::distance):
\dontinclude strategies_point_xy.hpp
\skipline template <typename P1
\until {
\skipline distance_result
\until Pythagoras
\skipline }
\until };

\n \section strategy class strategy_traits
\n The \b strategy_traits class matches \b point-types with \b strategies.
\n For each point-type there is a specialization
which defines which strategy should be used. <em>(Detail: because the library can calculate the distance of two different
types, the strategy_traits classes use actually two types).</em>
The following specialization defines that for point_xy points, the Pythagoras strategy should be used.
\dontinclude strategy_traits.hpp
\skipline template <typename T1, typename T2>
\until pythagoras
\skip };
\line };

\n \section point_point_ll class point_ll for lat long points
However, there are also latlong points, \b point_ll. They don't have x/y, but they have lat/lon methods.
\dontinclude latlong.hpp
\skipline template <dr_selector D,
\until {
\skipline lon()
\until void lat
\skip };
\line };
For these points totally different strategies are implemented then for xy-points.
For distance calculation there are three different strategies provided:
- \b haversine: fast, simple and often used, but not so accurate for calculations on the Earth
- \b Andoyer: not well-known but fast and accurate
- \b Vincenty: well-known, slower but very accurate.

The strategy_traits class defines only one to be the default distance calculation strategy.
\dontinclude strategy_traits.hpp
\skipline template <dr_selector D1
\until haversine
\skip };
\line };

\n \section distance The distance algorithm
Finally, the distance algorithm selects the right strategy using the strategy_traits class:
\dontinclude distance.hpp
\skipline template <typename P1, typename P2>
\until {
It then calls the overloaded method where a strategy can specified
\skipline distance
\until }
Alternatively, the library user can call this overloaded method directly:
- to specify another (for example more accurate) strategy
- to specify a value (e.g. Earth radius) in the constructor of the strategy
- because there is no default strategy implemented for the specified point type or
point type combination

\n \section other Other algorithms
The explanation above concentrates on distance algorithms. However, the design is used for other
algorithms as well: within, centroid, area, distance to a line segment, etc.

\n \section other_geometries Other geometries
All other geometry classes in the Geometry Library are class templates and the point type
(point_xy, point_ll or a user defined point type) can be specified as a template argument.

\n \section Summary
- there are different point types
- for each point type (or point-type-combination) there are strategies
- the strategy_traits class selects the default strategy
- the algorithms use the default strategy
- the library user can select another strategy
*/

//---------------------------------------------------------------------------------------------------

/*!
\page status Status and preview
The revised Geometry Library is successfully reapplied to the original geolib. All original
functionality is reimplemented. However, many OGC algorithms or geometry combinations have never
been implemented by us. For example, the distance between two polygons has never been necessary
in our environments.

If the boost community is enthousastic about the library we consider to implement more algorithms,
or more algorithms could be implemented because it is an Open Source process then.

The preview doesn't contain all implemented functionality, but probably gives
enough view on the design.

Besides this, things might still be adapted because of changed insight and/or because of comments
from the boost community.

Finally, the accompanying Projection Library, converting between latlong-points and
xy-points, is also considered to be made open source.

See also section "Changes since preview 1" 
*/

//---------------------------------------------------------------------------------------------------

/*!
\page changes Changes since preview 1
\section Structure
In preview 1 there were two layers: algorithms and implementations.

In this preview 2 there are three layers:
- algorithms, working on the Geometry Library geometry data structures
- implementation, implementations of algorithms, working on a more abstract base on any geometry
which satisfies the concept
- strategies: classes operating on a specific point type

\section Documentation
The documentation has been changed. Doxygen is now used to generate all documentation. There
are many short examples.

\section Points
The point type is changed completely. In preview 1 all points were required to have members as x() and y().
There were many comments on this design, already before the preview was posted.

The new geometry::point:
- has now a dimension which is templatized (this makes also 3D points possible)
- they are coordinate neutral, implemented internally as an array

There are currently the following derived points:
- geometry::point_xy with members x() and y()
- geometry::point_ll with members lat() and lon()
- more derived points are possible, for example point_xyz or point_rgb

Derived points should have corresponding strategies (primitive algorithms). See for more information
the page \ref points.

This change is based on suggestions made on the boost mailinglist by Hervé Brönnimann.

\section oi Output iterators
In preview 1 the intersection of a linestring with a box resulted in a multi-linestring.
This has been changed. The intersection algorithms now produces intersected linestrings which are
sent to output iterators.
This suggestion was made on the boost mailinglist by Phil Endecott.

\section mg Multi geometries
In preview 1 the intersection algorithms resulted in a multi-linstring. This was one of the reasons
to have the multi_polygon / multi_linestring. Other reasons are the OGC model, and that it is a
standard GIS concept.

\section ip Iterator pairs
In preview 1 all linestring-algorithms worked on the whole linestring. Now, in addition, they also
work on an interator pair. This suggestion was made on the boost mailinglist by Phil Endecott.

\section dr Distance results
In preview 1 the primary distance algorithms were named "squared_" and resulted in square distances,
for performance reasons. This is valid for xy-points but not for some other points, such as latlong
points. Therefore the distance algorithms now return "distance_result" structures, containing the
distance and a boolean indicating if it is squared.

\section ns Namespaces
In preview 1 all code was located within namespace geometry. This has been changed. The design,
which in preview 1 had already two layers, has now three layers (based on changes in points).
The layers are reflected in namespaces:
- namespace geometry for OGC and non OGC algorithms, working on geometry:: geometry-types
- namespace impl for algorithms, working on template geometry types G (where G are types
implemented with the required concepts). They are called by the algorithms from namespace geometry.
- namespace strategy for more primitive algorithms working specificly on points,
for example on point_xy or on point_ll. They are called by the impl-algorithms.

This change in namespace has an additional positive effect. There were some comments on naming of some
of the primary algorithms, for example within_point_in_polygon. This is now changed in point_in_polygon,
residing in namespace impl::within

\section wkt WKT
The well-known-text implementation has been changed completely. In preview 1 all geometries could be
streamed as well-known-text. This made streaming of other formats inconvenient. Now the library user can
choose how and what he wants to be streamed.

Besides that, there is a new algorithm from_wkt which parses WKT.
*/

//---------------------------------------------------------------------------------------------------

/*!
\page WKT WKT (Well-Known Text)
Well-Known Text is a text representation of a geometry, designed by \ref OGC.
Well-Known Text can represent a point, a linestring, a polygon, or multi versions of those.

Well-Known Text is used in the databases:
- PostGreSQL
- MySQL
- SQL Server 2008

WKT is explained in Wikipedia: http://en.wikipedia.org/wiki/Well-known_text
\note WKT points are comma-separated, coordinates are space-separated.
*/

//---------------------------------------------------------------------------------------------------


/*!
\page OGC OGC (Open Geospatial Consortium)
OGC is a standardizing committee on Geospatial Interoperability. The Geometry Library uses OGC conventions
for \ref WKT, for class names and for algorithms.

\section Classes
OGC defines the following geometry classes, implemented by the Geometry Library:
- \ref geometry::point "point": Point
- \ref geometry::linestring "linestring": Sequence of point values with linear interpolation 
between points
- \ref geometry::linear_ring "linear_ring": Sequence of point values with linear interpolation 
between points, which is closed and
not self-intersecting
- \ref geometry::polygon "polygon": Plane figure, consisting of an outer ring and zero or more 
inner rings. 
<em>Note that this definition is different from several other polygon definitions and libraries,
where polygons are not allowed to have holes. These polygons are comparable to the linear_ring above</em>
- \b multi_point: collection of points
- \b multi_linestring: collection of linestrings
- \b multi_polygon: collection of polygons

The naming of these classes is used in:
- WKT (Well-Known Text)
- KML (Google Maps)
- GML
- many GIS/geometry libraries 

Besides this the Geometry Library provides the following additional classes:
- \ref geometry::box "box": Box, used for selections and for envelopes (bounding boxes)
- \ref geometry::circle "circle": Circle, used for selections
- \ref geometry::segment "segment": Segment, helper class, used for e.g. intersections


\section Algorithms
The Geometry Library implements the following OGC algorithms, which are applicable to most or all geometries
- \b area: Returns the area of surface-geometries
- \b as_text: Represents the geometry as a Well Known Text
- \b centroid: Returns a point geometry: the centroid on surface-geometries
- \b distance: Returns the distance between two geometries
- \b envelope: Returns the bounding box of a geometry
- \b intersection: Returns a geometry containing the intersection between two geometries
- \b is_simple: Returns true if the geometry is simple (doesn't intersect itself)
- \b length: Returns the length of curve-type geometries
- \b within: Returns true if one geometry falls completely within another geometry

\section Differences
The Geometry Library does not implement the OGC Simple Feature interface exactly and completely.
There are many differences. Below the most important differences are listed.
- In OGC all operations are class methods. The Geometry Library is a template library and defines the operations
as functions.
- In OGC a point is defined by an x-coordinate value, a y-coordinate value and possibly a z-coordinate value
and a measured value. In the Geometry Library the basic point defines coordinates in a neutral way,
so there is no x, no y. In addition there are two derived point classes: 
\ref geometry::point_ll "point_ll" and \ref geometry::point_xy "point_xy".
- in OGC all geometries have additional members, such as SRID, type, dimension information. These
properties are not defined in the Geometry Library. Library users can implement them, if necessary,
in derived classes.
- In OGC the envelope returns a geometry, in the Geometry Library it returns a box
- The OGC algorithm asText is renamed to the class as_wkt and the manipulators in streamwkt


More information on OGC can be found on their website, http://www.opengeospatial.org
and on Wikipedia http://en.wikipedia.org/wiki/Open_Geospatial_Consortium
*/

//---------------------------------------------------------------------------------------------------

/*!
\page download Download the Geometry Library
The geometry library preview 2 can be downloaded here: 
http://geometrylibrary.geodan.nl/geometry_library_preview_2.zip
*/





#endif // _DOXYGEN_PAGES_HPP
