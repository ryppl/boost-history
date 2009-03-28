/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#include "pch.hpp"

#include "two_spheres.hpp"

#include <boost/guigl/view/impl/colored.hpp>
#include <boost/guigl/view/impl/navigable.hpp>
#include <boost/guigl/view/impl/positioned.hpp>
#include <boost/guigl/view/impl/periodic.hpp>
#include <boost/guigl/view/impl/solid_background.hpp>
#include <boost/guigl/view/impl/three_dimensional.hpp>
#include <boost/guigl/platform/glu.hpp>
#include <boost/guigl/gl.hpp>
#include <boost/guigl/types.hpp>

#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/date_time/local_time/local_time.hpp>

#include <iostream>

//class time_value
//{
//private:
//    double m_speed;
//    double m_phase;
//    boost::posix_time::ptime m_start_time;
//
//public:
//    time_value(
//        double speed,
//        double phase = 0.0,
//        boost::posix_time::ptime const& start_time = boost::posix_time::microsec_clock::local_time())
//        :
//    m_speed(speed),
//        m_phase(phase),
//        m_start_time(start_time)
//    {}
//
//    double operator()(boost::posix_time::ptime const& now
//        = boost::posix_time::microsec_clock::local_time()) const
//    {
//        return m_phase + m_speed *
//            (double)(now - m_start_time).total_milliseconds() /
//            boost::posix_time::seconds(1).total_milliseconds();
//    }
//};
//
//time_value angle(45.0);
//time_value teapot_angle(700.0);
//time_value v(45.0);

void two_spheres::draw_prologue()
{
    using namespace boost::posix_time;
    using namespace boost::guigl::gl;
    base_type::draw_prologue();

    //ptime now = microsec_clock::local_time();

    glMatrixMode(GL_MODELVIEW);
    load_identity();


    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    light(GL_LIGHT0)
        .diffuse(0.5f, 0.5f, 0.5f, 1.0f)
        .specular(1.0f, 1.0f, 1.0f, 1.0f)
        .spot_direction(100, -100, 300)
        .position(200, -150, 0, 0)
        .spot_cutoff(360.0f)
        ;

    light(GL_LIGHT1)
        .diffuse(1.0f, 1.0f, 0.5f, 1.0f)
        .spot_direction(0.0f, -0.5f, -0.5f)
        .position(0, 40, 200, 0)
        .spot_cutoff(90.0f)
        ;

    for(float x = -1000; x < 1000 ; x += 100)
        for(float y = -1000; y < 1000; y += 100)
        {
            scoped_matrix m;
            translate(x, y);
            rotate_z(drag_origin().x);
            rotate_x(drag_origin().y);

            // polished gold
            material(GL_FRONT) 
                .ambient(0.24725f, 0.1995f, 0.0745f, 1.0f)
                .diffuse(0.75164f, 0.60648f, 0.22648f, 1.0f)
                .specular(0.628281f, 0.555802f, 0.366065f, 1.0f)
                .shininess(51.2f)
                ;
            //solid_sphere(45.0, 20, 10);
            solid_cube(90);

            material(GL_FRONT)
                .diffuse(0.0f, 0.0f, 0.0f, 0.0f)
                .ambient(0.0f, 0.0f, 0.0f, 0.0f)
                ;
            wire_cube(90);
        }

}

void two_spheres::draw()
{
    draw_prologue();
    draw_epilogue();
}

void two_spheres::periodic_callback()
{
    if(distance()<300 || distance()>500)
        m_closer = distance()>400;
    set_distance(distance() + 1 - m_closer*2);
}

void *two_spheres::sphere()
{
    if(!s_sphere)
        s_sphere = gluNewQuadric();
    return s_sphere;
}

void *two_spheres::s_sphere = 0;

bool two_spheres::on_event(const boost::guigl::event_type &event_info)
{
    return base_type::on_event(event_info);
}
