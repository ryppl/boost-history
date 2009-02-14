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
    load_identity();

    scoped_matrix m;
    //translate_y(-100.0f);
    rotate_y(20.0f);


    //glEnable(GL_DEPTH_TEST);
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
    material_ambient(GL_FRONT, 1.0f, 0.0f, 0.0f, 0.0f);
    {   scoped_matrix m;
        translate(offset, offset, 0.0);
        solid_sphere(50, 30, 30);
    }

    material_ambient(GL_FRONT, 1.0f, 1.0f, 0.0f, 0.0f);
    {   scoped_matrix m;
        translate(-offset, offset, 0.0);
        solid_sphere(50, 30, 30);
    }

    material_ambient(GL_FRONT, 1.0f, 0.0f, 1.0f, 0.0f);
    {   scoped_matrix m;
        translate(-offset, -offset, 0.0);
        solid_sphere(50, 100, 100);
    }

    material_ambient(GL_FRONT, 0.0f, 0.0f, 1.0f, 0.0f);
    {   scoped_matrix m;
        translate(offset, -offset, 0.0);
        solid_sphere(50, 30, 30);
    }

    {
        scoped_matrix m;
        material_shininess(GL_FRONT, 20.0f);
        material_specular(GL_FRONT, 0.3f, 0.3f, 0.3f, 0.3f);
        material_ambient(GL_FRONT, 0.0f, 0.0f, 0.0f, 0.0f);
        material_diffuse(GL_FRONT, 0.0f, 1.0f, 0.0f, 0.0f);

        translate_z(100.0f);
        rotate_y(20.0f);
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
