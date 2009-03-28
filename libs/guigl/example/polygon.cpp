/*=================================---------------------------------------------
Copyright 2009 Andrey Torba

Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#include "pch.hpp"

#include <boost/guigl/application.hpp>
#include <boost/guigl/window.hpp>
#include <boost/guigl/view/mouse_tracking.hpp>
#include <boost/guigl/view/impl/mouse_tracking.hpp>
#include <boost/guigl/view/positioned.hpp>
#include <boost/guigl/view/impl/positioned.hpp>
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

typedef view::mouse_tracking<view::positioned<> > my_widget_base_type;
class my_widget : public my_widget_base_type
{
private:
    typedef polygon_2d Polygon;
    typedef box_2d Box;
    typedef std::vector<polygon_2d> PV;

    Polygon poly;
    Polygon hull;
    Box cb;
    PV v;

    void init()
    {
        box_2d b(make<point_2d>(150, 150), make<point_2d>(450, 250));
        cb = b;

        const double coor[][2] = {
            {200, 130}, {240, 170}, {280, 180}, {340, 120}, {370, 160},
            {340, 200}, {410, 300}, {530, 260}, {540, 120}, {490, 80}, {290, 70},
            {200, 130} // closing point is opening point
        };
        assign(poly, coor);

        {
            poly.inners().resize(1);
            const double coor[][2] = { {400, 200}, {420, 140}, {480, 190}, {440, 220}, {400, 200} };
            assign(poly.inners().back(), coor);
        }

        correct(poly);

        convex_hull(poly, std::back_inserter(hull.outer()));

    }

    void on_mouse_move()
    {
        box_2d box(
            make<point_2d>(mouse_state().position.x - 100, mouse_state().position.y - 50),
            make<point_2d>(mouse_state().position.x + 100, mouse_state().position.y + 50));
        cb = box;

        v.clear();
        intersection(cb, poly, std::back_inserter(v));

        window::redraw(*this);
    }

public:
    typedef my_widget_base_type base_type;

    template<typename ArgumentPack>
    my_widget(const ArgumentPack &args)
        : base_type(args)
    {
        init();
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

    void draw_prologue()
    {
        base_type::draw_prologue();

        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_POINT_SMOOTH);

        // hull
        draw_polygon(hull, yellow(0.3), red(0.4));

        // polygon
        draw_polygon(poly, yellow(0.8), red(0.8));

        // box
        gl::color(blue(0.2));
        ggl::rect(cb);

        gl::point_size(10);
        // intersection
        BOOST_FOREACH(polygon_2d const& pg, v)
        {
            draw_polygon(pg, red(0.4), black(0.8));
            geometry::for_each_point(pg, ggl::drawer());
        }

    }

    BOOST_GUIGL_WIDGET_DRAW_IMPL(my_widget);
};

int main()
{
    window wnd((
        _label = "tess example",
        _size=size_type(600, 600),
        _background = white() ));

    wnd << new my_widget((
        _size=size_type(600, 600)
        ));

    application::run();
    return 0;
}
