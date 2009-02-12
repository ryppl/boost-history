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

    light_ambient(GL_LIGHT1, 0.6f, 0.6f, 0.6f, 1.0f);
    light_diffuse(GL_LIGHT1, 1.0f, 1.0f, 1.0f, 1.0f);
    light_specular(GL_LIGHT1, 1.0f, 1.0f, 1.0f, 1.0f);
    light_position(GL_LIGHT1, 100, 40, 40, 1);
    light_spot_direction(GL_LIGHT1, -100, -40, -40);
    light_constant_ambient(GL_LIGHT1, 1.5f);
    light_linear_attenuation(GL_LIGHT1, 0.5f);
    light_quadratic_attenuation(GL_LIGHT1, 0.2f);
    light_spot_cutoff(GL_LIGHT1, 180.0f);
    light_spot_exponent(GL_LIGHT1, 2.0f);

    solid_sphere(50, 10, 10);
    glLoadIdentity();
    translate(50.f, 50.f, 50.f);
    wire_sphere(20, 20, 20);

    {
        scoped_matrix m;
        translate_x(-70.0f);
        wire_teapot(20);
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
