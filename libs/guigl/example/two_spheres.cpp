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

#include <iostream>

void two_spheres::draw_prologue()
{
    using namespace boost::guigl::gl;
    base_type::draw_prologue();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //glEnable(GL_DEPTH_TEST);
    //glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);

    light(GL_LIGHT1)
        .ambient(0.3f, 0.3f, 0.3f, 1.0f)
        .diffuse(1.0f, 1.0f, 1.0f, 1.0f)
        .specular(1.0f, 1.0f, 1.0f, 1.0f)
        .spot_direction(100, 100, 100)
        .position(-100, -100, -100, 0)
        .constant_attenuation(1.5f)
        .linear_attenuation(0.5f)
        .quadratic_attenuation(0.2f)
        .spot_cutoff(180.0f)
        .spot_exponent(2.0f);

    light(GL_LIGHT2)
        .ambient(0.3f, 0.3f, 0.3f, 1.0f)
        .diffuse(1.0f, 1.0f, 1.0f, 1.0f)
        .specular(1.0f, 0.0f, 1.0f, 1.0f)
        .spot_direction(-100, 100, 100)
        .position(100, -100, -100, 0)
        .constant_attenuation(1.2f)
        .linear_attenuation(0.5f)
        .quadratic_attenuation(0.2f)
        .spot_cutoff(180.0f)
        .spot_exponent(2.0f);

    solid_sphere(50, 50, 50);
    glLoadIdentity();
    translate_x(50.f);
    boost::guigl::gl::color(1.0, 1.0, 0.0, 0.5);
    solid_sphere(50, 100, 100);

    {
        scoped_matrix m;
        translate_x(-100.0f);
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
