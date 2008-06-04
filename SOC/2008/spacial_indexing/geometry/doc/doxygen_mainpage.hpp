// Geometry Library
//
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef _DOXYGEN_MAINPAGE_HPP
#define _DOXYGEN_MAINPAGE_HPP


/*!
\mainpage Geometry Library

\section header Geometry Library

This Geometry Library is considered to be proposed to Boost, 
and is prepared for Boost until a certain point.
It is not part of Boost.

<em>Copyright © 1995-2008 <b>Barend Gehrels</b>, Geodan Holding B.V. Amsterdam, the Netherlands.\n
Distributed under the Boost Software License, Version 1.0.\n
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
</em>

\section intro Introduction
The Geometry Library provides a generic implementation of geometry algorithms. Besides the
algorithms it defines the basic geometry classes (point, linestring, polygon) which are kept
small and simple. The algorithms include some well-known and often needed algorithms as
point-in-polygon, polygon area, point-to-point distance, point-to-line distance, line clipping and
polygon clipping.

The implementation is as generic as possible. The algorithms work, of course, on the geometry
classes provided by the library, but they also work on geometry classes defined by the library
user.

The library follows conventions used in the std:: library and in the boost:: libraries. It also
follows, where possible, the OGC names and conventions. OGC is the Open Geospatial Consortium,
the standardization committee on GIS (Geographical Information Systems). OGC geometries are used
in many environments, among others in databases (Oracle, MySQL, PostgreSQL). OGC specifies a library
based on inheritance, as opposed to this library, which is a template library where data classes are
strictly separated from the algorithms. Therefore it is not an OGC implementation (but an
OGC implementation could be built with this library).

\section platforms Platforms

The library currently compiles successfully on the following platforms:
- MS Visual Studio 2005 (Express Edition)
- MS Visual Studio 2003
- gcc version 3.4.4

It doesn't compile on the following platform:
- MS Visual Studio 6 (because of nested templates)

\section history History
Geodan started in 1995 with a Geographic Library, called geolib. Since then hundreds of projects have
been done using this geolib, and the geolib have been extended to fit the needs of its users. Geolib
can be used in different environments: in Windows applications, as a DLL (ggl32.dll), in
Web Map Servers (SclMapServer), Web Feature Servers or more specific programs.

In 2007 the Geometry part was revised and completely templatized. Now that it is clear and well
designed we consider proposing the geometry part to boost, and the boost community has expressed
their interest.

The library can be downloaded on "Download" in "Related pages".
*/





#endif // _DOXYGEN_MAINPAGE_HPP
