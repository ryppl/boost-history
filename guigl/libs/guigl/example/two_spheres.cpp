/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#include "two_spheres.hpp"

#include <boost/guigl/view/impl/colored.hpp>
#include <boost/guigl/view/impl/positioned.hpp>
#include <boost/guigl/view/impl/solid_background.hpp>
#include <boost/guigl/view/impl/three_dimensional.hpp>
#include <boost/guigl/platform/glu.hpp>

void two_spheres::draw_prologue()
{
    base_type::draw_prologue();
    glPushMatrix();
    gluSphere(static_cast<GLUquadric *>(sphere()), 10, 4, 4);
    glTranslatef(50,50,50);
    gluSphere(static_cast<GLUquadric *>(sphere()), 10, 4, 4);
    glPopMatrix();
}

void two_spheres::draw()
{
    draw_prologue();
    draw_epilogue();
}

void *two_spheres::sphere()
{
    if(!s_sphere)
        s_sphere = gluNewQuadric();
    return s_sphere;
}

void *two_spheres::s_sphere = 0;
