/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#include "two_spheres.hpp"

#include <boost/guigl/view/impl/colored.hpp>
#include <boost/guigl/view/impl/navigable.hpp>
#include <boost/guigl/view/impl/positioned.hpp>
#include <boost/guigl/view/impl/periodic.hpp>
#include <boost/guigl/view/impl/solid_background.hpp>
#include <boost/guigl/view/impl/three_dimensional.hpp>
#include <boost/guigl/platform/glu.hpp>
#include <boost/guigl/gl.hpp>

#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/date_time/local_time/local_time.hpp>

#include <iostream>

class time_value
{
private:
    double m_speed;
    double m_phase;
    boost::posix_time::ptime m_start_time;

public:
    time_value(
        double speed,
        double phase = 0.0,
        boost::posix_time::ptime const& start_time = boost::posix_time::microsec_clock::local_time())
        :
    m_speed(speed),
        m_phase(phase),
        m_start_time(start_time)
    {}

    double operator()(boost::posix_time::ptime const& now
        = boost::posix_time::microsec_clock::local_time()) const
    {
        return m_phase + m_speed *
            (double)(now - m_start_time).total_milliseconds() /
            boost::posix_time::seconds(1).total_milliseconds();
    }
};

time_value angle(45.0);
time_value teapot_angle(700.0);
time_value v(45.0);

void two_spheres::draw_prologue()
{
    using namespace boost::posix_time;
    using namespace boost::guigl::gl;
    base_type::draw_prologue();

    ptime now = microsec_clock::local_time();

    glMatrixMode(GL_MODELVIEW);
    load_identity();

    scoped_matrix m;
    rotate_y(angle(now));


    //glEnable(GL_COLOR_MATERIAL);

    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_BLEND); 
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
    //glShadeModel(GL_SMOOTH); 
    //glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST); 
    //glEnable(GL_POLYGON_SMOOTH);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    light(GL_LIGHT0)
        .ambient(0.3f, 0.3f, 0.3f, 1.0f)
        .diffuse(0.5f, 0.5f, 0.5f, 1.0f)
        .specular(1.0f, 0.0f, 1.0f, 1.0f)
        .spot_direction(100, -100, 300)
        .position(-100, 100, -300, 0)
        .constant_attenuation(1.5f)
        .linear_attenuation(0.5f)
        .quadratic_attenuation(0.2f)
        .spot_cutoff(180.0f)
        .spot_exponent(2.0f);

    light(GL_LIGHT1)
        .ambient(0.3f, 0.3f, 0.3f, 1.0f)
        .diffuse(0.5f, 0.5f, 0.5f, 1.0f)
        .specular(1.0f, 1.0f, 1.0f, 1.0f)
        .spot_direction(-100, 100, -300)
        .position(100, -100, 300, 0)
        .constant_attenuation(1.2f)
        .linear_attenuation(0.5f)
        .quadratic_attenuation(0.2f)
        .spot_cutoff(180.0f)
        .spot_exponent(2.0f);

    double offset = 30;
    material(GL_FRONT).ambient(1.0f, 0.0f, 0.0f, 0.0f);
    {   scoped_matrix m;
        translate(offset, offset, 0.0);
        solid_sphere(50, 30, 30);
    }
    rect(0.0, 0.0, 200.0, -200.0);

    material(GL_FRONT).ambient(1.0f, 1.0f, 0.0f, 0.0f);
    {   scoped_matrix m;
        translate(-offset, offset, 0.0);
        solid_sphere(50, 30, 30);
    }
    material(GL_FRONT).ambient(0.0f, 1.0f, 0.0f, 0.0f);
    rect(0.0, 0.0, -200.0, 200.0);

    material(GL_FRONT).ambient(1.0f, 0.0f, 1.0f, 0.0f);
    {   scoped_matrix m;
        translate(-offset, -offset, 0.0);
        solid_sphere(50, 30, 30);
    }
    rect(0.0, 0.0, 200.0, 200.0);

    material(GL_FRONT).emission(0.3f, 0.0f, 0.3f, 1.0f);
    material(GL_FRONT).ambient(0.0f, 0.0f, 1.0f, 0.0f);
    {   scoped_matrix m;
        translate(offset, -offset, 0.0);
        solid_sphere(50, 30, 30);
    }
    rect(0.0, 0.0, -200.0, -200.0);

    material(GL_FRONT).ambient(1.0f, 1.0f, 1.0f, 1.0f);
    glLineWidth(5.0);
    glBegin(GL_LINE_LOOP);
    vertex(200.0, 200.0);
    vertex(-200.0, 200.0);
    vertex(-200.0, -200.0);
    vertex(200.0, -200.0);
    glEnd();
    {
        scoped_matrix m;
        material(GL_FRONT)
            .shininess(20.0f)
            .specular(0.3f, 0.3f, 0.3f, 0.3f)
            .ambient(0.0f, 0.0f, 0.0f, 0.0f)
            .diffuse(0.0f, 1.0f, 0.0f, 0.0f);

        translate_z(100.0f);
        rotate_y(teapot_angle(now));
        solid_teapot(20);
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
