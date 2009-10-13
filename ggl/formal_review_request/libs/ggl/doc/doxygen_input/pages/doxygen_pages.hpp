// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef _DOXYGEN_PAGES_HPP
#define _DOXYGEN_PAGES_HPP


//---------------------------------------------------------------------------------------------------

/*!
\page concepts Concepts

All geometries are handled as concepts. They are specified by a set of traits classes in the traits namespace.

This means that geometries provided by library users, or legacy geometries, or plain arrays, or boost tuples,
all can be handled by the Generic Geometry Library. That is why it is called generic. Also std::vector of points
or tuples can be handled as either a linestring or a linear ring (polygon without holes).

There are concepts for
- points
- box
- n-sphere (meaning circle or sphere)
- linestring
- ring (linear ring, a polygon without holes)
- polygon
- segment

The library uses the generic functionality internally. So it should work with any geometry, as long as the
necessary traits classes are provided.

For a valid point five traits classes are necessary:
- there should be a tag traits class indicating that it is a point
- there should be a coordinate_type traits class indicating what arithmetic type its coordinates are
- there should be a dimension traits class indicating the number of its dimensions (coordinate axes)
- there should be a coordinate system traits class defining in which coordinate system it lives
- there should be an access traits class, having a get (for a ConstPoint) and a set (for a mutable Point) function

These traits class can be implemented in the code, but easier is it to use the registration macro's as
shown in example c01 (custom points).

For a valid linestring there is only one traits class required:
- there should be a tag traits class indicating that it is a linestring

For the rest a linestring is modelled as a range, so a std::vector, std::deque, boost::array, ggl::linestring,
iterator pair or c-array will do. If a linestring is modified it is using the standard library (push_back). If that
is not intended, optionally another traits class can be defined:
- there might be a use_std traits class indicating that it does not use the standard library (for modifications)

If that traits class is defined, there should also be two other traits classs
- there should then be clear traits class to make a linestring empty
- there should then be a append_point traits class to add a poitn to a linestring

There are also concepts for boxes, n-spheres, segments, rings and polygons.


The Generic Geometry Library uses Boost Concept Check Library (BCCL) and Meta Programming Language (MPL) to define
and check concepts. The Generic Geometry Library uses the Boost Range Library to iterate through standard containers,
boost arrays, c-arrays.

*/






//---------------------------------------------------------------------------------------------------

/*!
\page compiling Compiling

\section compiling_intro Compiling the Generic Geometry Library

The Generic Geometry Library is a Header Only library. So just including the headerfiles is enough
to use the algorithms. Nothing has to be linked.

The Generic Geometry Library is only dependant on (header only) Boost libraries. Download the Boost
Library Collection from www.boost.org, adapt include path to include Boost.

\section platforms Platforms

The library currently compiles successfully on the following platforms:
- MS Visual Studio 2008 (Express Edition)
- MS Visual Studio 2005 (Express Edition)
- gcc version 3.4
- gcc version 4 (tested are 4.1, 4.2, 4.4)


\section convenient_includes Convenient includes

This section concentrates on how to include the Generic Geometry Library.

The most convenient headerfile including all algorithms and strategies is geometry.hpp:
- \#include <ggl/ggl.hpp> It is not absolutely necessary to include this headerfile, it is also possible to
  include the algorithm headerfiles separately. However, it is convenient and used in most cases.


An often used headerfile is geometries.hpp:
- \#include <ggl/geometries/geometries.hpp> This includes all default geometries: point, linestring,
  polygon, linear_ring, box, circle/sphere. It is not included in the "geometry.hpp" headerfile because
  users are allowed to use their own geometries. However, for library users who want to use the provided
  geometries it is most useful.


For users using only Cartesian points, with floating point coordinates (double), in 2D or 3D, you can use instead:
- \#include <ggl/geometries/cartesian2d.hpp> This includes all 2D Cartesian geometries: point_2d, linestring_2d,
  etc. Using this headerfile the library seems to be a non-template library, so it is convenient for users that
  are not so into the template world.




\section advanced_includes Advanced includes

This section is for users who have their own geometries and want to use algorithms from the Generic Geometry Library.


If you want to use your own points it makes sense to use the registration macro's:
- \#include <ggl/geometries/register/point.hpp> macro's for point registration
- \#include <ggl/geometries/register/box.hpp> macro's for box registration


If you are using standard containers containing points and want to handle them as a linestring
- \#include <ggl/geometries/adapted/std_as_linestring.hpp> allows you to use things like \c std::vector<point_2d>
  and put them as parameters into algorithms.


If you are using boost tuples and want to handle them as Cartesian points
- \#include <ggl/geometries/adapted/tuple_cartesian.hpp>


\section performance Performance findtuning
The enumeration below is not exhaustive but can contain hints to improve the performance
- For Microsoft, set the define _SECURE_SCL=0
- For Microsoft, set the define _HAS_ITERATOR_DEBUGGING=0
- Barend's measurements indicate that MSVC 2005 generates faster code than MSVC 2008
- Using StlPort results in significant faster code than Microsoft's standard library
- Of course turn on compiler optimizations, compile in release mode

*/


//---------------------------------------------------------------------------------------------------

/*!
\page performance Performance

\section performance_comparison Performance results
The performance has been tested for some algorithms and it seems that the Generic Geometry Library performs well,
indicating that the template-based approach and consequent usage of inline functions and the std:: library
is a sound approach. Measurements and comparisons (still in concept) are here:
http://trac.osgeo.org/ggl/wiki/Performance

\section performance-notes Performance notes
In the page about compiling the library there are some hints provided which might improve the performance.

Furthermore it is important to realize that if you're about to do calculations with two geometries, for
example a point-in-polygon or an intersection, it is very useful to first calculate and store all bounding
boxes (envelopes), and then before doing a point-in-polygon check if the point is in the bounding box. Checking if a
point is within a box is of course much faster than visiting all vertices of a polygon.

The storage of bounding boxes is, on purpose, not done within the library because it would break the possibility
to use standard vectors of points for linestrings or rings. The library might get a traits system in the future
where geometries might tell their boundaries to the algorithms, this however would be an optional system.
*/





//---------------------------------------------------------------------------------------------------

/*!
\page DSV DSV (Delimiter Separated Values)
DSV is a text representation of a geometry, explained here: http://en.wikipedia.org/wiki/Delimiter-separated_values
DSV can represent a point, a linestring, a polygon, or multi versions of those.

It is currently just a utility in the library, used in samples and tests, to get some textual output
of a geometry.

*/

//---------------------------------------------------------------------------------------------------


/*!
\page OGC OGC (Open Geospatial Consortium)
OGC is the Open Geospatial Consortium, the standardization committee
on Geospatial Interoperability and GIS (Geographical Information Systems).
OGC geometries are used in many environments and databases.

The Generic Geometry Library uses OGC conventions
for algorithms and for geometry type names. Note that though OGC is concentrating on GIS,
the conventions followed here are 'geometry only', the so-called 'simple features'
(simple in the sense that polygons are not allowed to have self-intersections).

OGC specifies a library based on inheritance, as opposed to this library, which is a generic template based library
where data classes are separated from the algorithms.
Therefore this library is not an OGC implementation in the strict sense.

\section Classes
OGC defines the following geometry classes,
which are implemented as concepts (and as geometries) the Generic Geometry Library:
- \ref ggl::point "point": a point. The point defined here is dimensionally agnostic.
Library users does not have to use this point, they might also use their own points as long as it meets the concepts.
- \ref ggl::linestring "linestring": Sequence of point values with linear interpolation
between points. Note that library users does not have to use this type. Algorithms works on iterators, so
all algorithms also accept iterators on a vector (or other container) of points.
- \ref ggl::linear_ring "linear_ring": Sequence of point values with linear interpolation
between points, which is closed and not self-intersecting
- \ref ggl::polygon "polygon": Plane figure, consisting of an outer ring and zero or more
inner rings. So basically a polygon which might have holes.
<em>Note that this definition is different from several other polygon definitions and libraries,
where polygons are not allowed to have holes. These polygons are comparable to the linear_ring above</em>

The following geometries were in the first preview, because they are defined in OGC, but not in the current preview
(they will be added again).
- \b multi_point: collection of points
- \b multi_linestring: collection of linestrings
- \b multi_polygon: collection of polygons

The naming of these classes is used in:
- WKT (Well-Known Text)
- KML (Google Maps)
- GML
- many GIS/geometry libraries

and in many databases:
- Oracle Spatial
- SQL Server 2008
- PostGreSQL
- MySQL
- MonetDB

Besides this the Generic Geometry Library provides the following additional classes:
- \ref ggl::box "box": Box, used for selections and for envelopes (bounding boxes)
- \ref ggl::nsphere "n-sphere", meaning circle" and "sphere". Circle is convenient for selections
- \ref ggl::segment "segment": Segment, helper class, used for e.g. intersections


\section Algorithms
The Generic Geometry Library implements the following OGC algorithms, which are applicable to most or all geometries
- \b area: Returns the area of surface-geometries
- \b as_text: Represents the geometry as a Well Known Text (named "wkt")
- \b centroid: Returns a point geometry: the centroid on surface-geometries
- \b convex_hull: Returns true if the geometry is simple (doesn't intersect itself)
- \b coordinate_dimension: Returns the number of geometry coordinate values
- \b distance: Returns the distance between two geometries
- \b envelope: Returns the bounding box of a geometry
- \b intersection: Returns a geometry containing the intersection between two geometries (currently only with boxes)
- \b length: Returns the length of curve-type geometries
- \b num_points: Returns the number of points
- \b topological_dimension: Returns the topological dimension
- \b within: Returns true if one geometry falls completely within another geometry

Readers might notice that many OGC algorithms, especially the more complex ones, are not yet implemented and that is true.
Much work has been done on the generic structure of the library, algorithms still have to be worked on.
On the other hand many algorithms are implemented for different coordinate systems (distance is implemented using
four strategies: Pythagoras, Andoyer, Vincenty and haversine, there is the "spherical polygon area").

\section Differences
The Generic Geometry Library does not implement the OGC Simple Feature interface exactly and completely.
There are many differences. Below the most important differences are listed.
- In OGC all operations are class methods. The Generic  Geometry Library is a template library and defines the algorithms
as generic functions.
- In OGC a point is defined by an x-coordinate value, a y-coordinate value and possibly a z-coordinate value
and a measured value. In the Generic Geometry Library the basic point defines coordinates in a neutral way,
so there is no x, no y.
- in OGC all geometries have additional members, such as SRID, measured. These
properties are not defined in the Generic Geometry Library. Library users can implement them, if necessary,
in derived classes.
- In OGC the envelope returns a geometry, in the Generic Geometry Library it returns a box
- The OGC algorithm asText is named "wkt" and to stream manipulators in streamwkt


More information on OGC can be found on their website, http://www.opengeospatial.org
and on Wikipedia http://en.wikipedia.org/wiki/Open_Geospatial_Consortium
*/


//---------------------------------------------------------------------------------------------------

/*!
\page status Status and preview

\section introduction Introduction

The Generic Geometry Library is currently in its fourth preview.

The first preview was a template 2D geometry library providing its own geometries and algorithms working on
those geometries. Those geometries are still provided but not essential anymore.

The second preview didn't asume points with .x() and .y() anymore. It provided strategies,
operating on specific point types. So point_xy (cartesian points) were
handled different from point_ll (latlong points). That is still the case (now using a coordinate system meta-function)

The third preview introducted the point concept, library users could use their own points with the algorithms
provided by the library.

So this is the fourth preview and implements concepts for all geometries. Besides that tag dispatching is
introduced internally. All algorithms are now really generic, there are only two distance functions, one with two
geometries and one with two geometries and a strategy. Those two will do. Internally they are first splitted
per geometry, so point-to-point distance and point-to-linestring distance, etc. Then they are splitted per
coordinate system, so Cartesian points have another distance calculation then spherical points. Finally those
strategies, as they are called, are applied.

\section changes4 Changes since preview 3
- there are now typedefs for the more common geometries, such as point_2d or ring_ll_deg
- all geometries are optional, everything is working with concepts
- the convex hull algorithm has been added and can be calculated for linestrings (ranges, point sequences), polygons
- map projections (92 !) have been added, they are converted to C++ template classes from PROJ4
- transformations have been added, to go from one coordinate system to another
- conversions have been added, to go from one geometry type to another (e.g. BOX to POLYGON)
- edit functionality have been added, to edit coordinate values or to add points
- parsing of latitude longitude coordinate values have been added
- the "selected" algorithm have been added
- many examples have been added
- many tests have been added

\b Breaking \b changes
Because the library was in preview, and it still is, there are some major changes which might influence library user's
code. Although there are many changes internally, the changes for users should be relatively small:
- all algorithms accepting linestrings are now modelled to get the linestring itself. In the previous version
  the .begin(), end() had to be specified, That is now not necessary anymore because the Boost Range Library is used
  internally, and tag dispatching is used to distinguish geometries internally
- the "grow" utility is now splitted into buffer (growing a box with a value was in fact a buffer) and a combine.
- there was a generic "get" function with a const ref and a non const ref. This is splitted into "get" and "set"
- there might be more changes, please contact if anything is unclear

\section changes5 Changes since preview 4
The change from preview 3 to preview 4 was major and many things have been addressed. Concepts are everywhere,
algorithms are really generic, not dependant on geometry type. The library is coordinate agnostic and dimensional
agnostic.So the design is stable, in that sense.
However, there are changes:
- implementation of spatial set relations (intersection, union)
- move of GIS-specific code to extension/gis
- move of map projections to extensions/gis/projections
- implementation of SVG in extensions/SVG
- review of all code, conform Boost code guidelines, no tabs, template parameters, etc.
- other small changes.



If people are interested in helping with the library, be it by coding, by testing, by commenting or otherwise, they
are very welcome.

\section history History
Geodan started in 1995 with a Geographic Library, called geolib or also GGL (Geodan Geographic Library).
Since then hundreds of projects have
been done using this geolib, and the geolib have been extended to fit the needs of its users. Geolib
can be used in different environments: in Windows applications, as a DLL (ggl32.dll), in
Web Map Servers (SclMapServer), Web Feature Servers or more specific programs.

In 2007-2009 the Geometry part has been revised and completely templatized.
From 2008 Bruno Lalande, already a Boost contributor, joined and helped
to make the library more generic and to define clear concepts.
It is now called Generic Geometry Library, abbreviated to GGL.
From 2009 Mateusz Loskot, an active member of Geospatial Open Source communities,
joined and helped with reviewing code, guidelines, WKB, iterators, and the Wiki and ticket system

The library is now called Generic Geometry Library, abbreviated to GGL.
The Generic Geometry Library is Open Source and is proposed to the Boost community.


*/


//---------------------------------------------------------------------------------------------------

/*!
\page download Download the Generic Geometry Library

The library can be downloaded from Boost SVN. There is anonymous SVN access.

The command

<tt>svn co https://svn.boost.org/svn/boost/sandbox/ggl ggl</tt>

will download the library to your machine.

Current "formal review request" version is here:
https://svn.boost.org/svn/boost/sandbox/ggl/formal_review_request


*/





#endif // _DOXYGEN_PAGES_HPP
