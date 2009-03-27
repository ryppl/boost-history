/*=================================---------------------------------------------
Copyright 2009 Andrey Torba

Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#include <boost/guigl/application.hpp>
#include <boost/guigl/window.hpp>
#include <boost/guigl/widget/custom.hpp>
#include <boost/guigl/ggl.hpp>

#include <boost/assign/std.hpp>
#include <boost/foreach.hpp>

#include <algorithm> // for reverse, unique

#include <geometry/geometry.hpp>
#include <geometry/geometries/cartesian2d.hpp>
#include <geometry/geometries/adapted/c_array_cartesian.hpp>
#include <geometry/geometries/adapted/std_as_linestring.hpp>
#include <geometry/io/wkt/streamwkt.hpp>

using namespace boost::guigl;
using namespace geometry;

struct draw_stuff {
  typedef polygon_2d Polygon;
  typedef box_2d Box;
  typedef std::vector<polygon_2d> PV;

  Polygon poly;
  Polygon hull;
  Box cb;
  PV v;

  draw_stuff()
    : cb(make<point_2d>(1.5, 1.5), make<point_2d>(4.5, 2.5))
    {
    const double coor[][2] = {
      {2.0, 1.3}, {2.4, 1.7}, {2.8, 1.8}, {3.4, 1.2}, {3.7, 1.6},
      {3.4, 2.0}, {4.1, 3.0}, {5.3, 2.6}, {5.4, 1.2}, {4.9, 0.8}, {2.9, 0.7},
      {2.0, 1.3} // closing point is opening point
      };
    assign(poly, coor);

      {
      poly.inners().resize(1);
      const double coor[][2] = { {4.0, 2.0}, {4.2, 1.4}, {4.8, 1.9}, {4.4, 2.2}, {4.0, 2.0} };
      assign(poly.inners().back(), coor);
      }

    correct(poly);

    convex_hull(poly, std::back_inserter(hull.outer()));

    intersection(cb, poly, std::back_inserter(v));
    }

  template<class T>
  static void draw_polygon(
    T const& g,
    color_type const& body,
    color_type const& border)
    {
    // body of polygon
    gl::color(body);
    ggl::draw(g);

    // contours of polygon
    gl::color(border);
    ggl::draw(exterior_ring(g));

    typedef typename ring_type<T>::type R;

    BOOST_FOREACH(R const& r, interior_rings(g))
      ggl::draw(r);
    }

  void operator()() const {
    gl::scoped_matrix m;
    //gl::translate(100., 100.);
    gl::scale(80., 80., 80.);

    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);

    // hull
    draw_polygon(hull, yellow(0.3), red(0.4));

    // polygon
    draw_polygon(poly, yellow(0.8), red(0.8));

    // box
    gl::color(blue(0.2));
    ggl::rect(cb);

    // intersection
    BOOST_FOREACH(polygon_2d const& pg, v)
      draw_polygon(pg, red(0.4), black(0.8));
    }

  };

int main()
  {
  window wnd((
    _label = "tess example",
    _size=size_type(500, 500),
    _background = white() ));

  wnd << new widget::custom((
    _size=size_type(500,500),
    _draw_prologue = draw_stuff()
    ));

  application::run();
  return 0;
  }
