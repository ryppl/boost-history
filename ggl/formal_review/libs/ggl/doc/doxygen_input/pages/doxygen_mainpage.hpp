// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef _DOXYGEN_MAINPAGE_HPP
#define _DOXYGEN_MAINPAGE_HPP


/*!
\mainpage Generic Geometry Library

\section header Generic Geometry Library

<em>Copyright © 1995-2009 <b>Barend Gehrels</b>, Geodan, Amsterdam, the Netherlands.\n
Copyright © 2008-2009 <b>Bruno Lalande</b>, Paris, France.\n
Copyright © 2009 <b>Mateusz Loskot</b>, Cadcorp, London, UK.\n
Distributed under the Boost Software License, Version 1.0.\n
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
</em>

\section intro Introduction
The Generic Geometry Library, GGL, currently in \b Formal \b Review, defines \ref concepts "concepts" for geometries and implements some algorithms on such geometries.

GGL contains a dimension-agnostic, coordinate-system-agnostic and scalable kernel,  based on concepts, meta-functions and tag- dispatching.
On top of that kernel, algorithms are built: area, length, perimeter, centroid, convex hull, intersection (clipping), within (point in polygon), distance, envelope (bounding box), simplify, transform, convert, and more. The library is also designed to support high precision arithmetic numbers, such as GMP.

GGL contains instantiable geometry classes, but library users can also use their own. Using registration macros or traits classes their geometries can be adapted to fulfil the GGL-concepts.

The GGL might be used in all domains where geometry plays a role: mapping and GIS, gaming, computer graphics and widgets, robotics, astronomy... The core is designed to be as generic as possible and support those domains. However, for now the development has been mostly GIS-oriented.

GGL supports the extension model, the same way as GIL also applies it. An extension is (mostly) something more specific to domains like mentioned above.

The library follows existing conventions:
- conventions from boost
- conventions from the std library
- conventions and names from one of the OGC standards on Geometry

This Generic Geometry Library (ggl) is now being reviewed by the Boost Community

The library can be downloaded from the Boost Sandbox,
go to the \ref download "Download page" for more information.

A (recently started) GGL Wiki is here: http://trac.osgeo.org/ggl/wiki

\section quickstart Quick start
It is not possible to show the whole library at a glance. A few very small examples are shown below.

It should be possible to use a very small part of the library,
for example only the distance between two points.

\dontinclude doxygen_2.cpp
\skip example_for_main_page()
\skipline int a
\until endl;

Other often used algorithms are point-in-polygon:
\skipline ring_2d
\until endl;

or area:
\skip area
\until endl;

It is possible, by the nature of a template library, to mix the point types declared above:
\skip double d2
\until endl;

The pieces above generate this output:
\image html output_main.png


It is also possible to use non-Cartesian points.
For example: points on a sphere. When then an algorithm such as distance
is used the library "inspects" that it is handling spherical
points and calculates the distance over the sphere, instead of applying the Pythagorean theorem.

Finally an example from a totally different domain: developing window-based applications, for example
using QtWidgets. We check if two rectangles overlap and if so,
move the second one to another place:
\skip QRect
\until }

More examples are on the page \b Examples


*/





#endif // _DOXYGEN_MAINPAGE_HPP
