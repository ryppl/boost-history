/*=================================---------------------------------------------
Copyright 2009 Stjepan Rajko, Torba Andrey

Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__GL__TRANSFORM_HPP
#define BOOST__GUIGL__GL__TRANSFORM_HPP

#include <boost/noncopyable.hpp>

namespace boost{ namespace guigl { namespace gl {

    inline void load_identity()
    {
        glLoadIdentity();
    }

    inline void push_matrix()
    {
        glPushMatrix();
    }

    inline void pop_matrix()
    {
        glPopMatrix();
    }

    inline scoped_matrix::scoped_matrix() {push_matrix();}
    inline scoped_matrix::~scoped_matrix() {pop_matrix();}

    ////////////////////////////////////////////////////////////////////////////////
    template<>
    inline void rotate<GLdouble>(GLdouble angle, GLdouble x, GLdouble y, GLdouble z)
    {
        glRotated(angle, x, y, z);
    }

    template<>
    inline void rotate<GLfloat>(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
    {
        glRotatef(angle, x, y, z);
    }

    template<>
    inline void rotate_x<GLdouble>(GLdouble angle)
    {
        rotate(angle, 1.0, 0.0, 0.0);
    }

    template<>
    inline void rotate_y<GLdouble>(GLdouble angle)
    {
        rotate(angle, 0.0, 0.1, 0.0);
    }

    template<>
    inline void rotate_z<GLdouble>(GLdouble angle)
    {
        rotate(angle, 0.0, 0.0, 0.1);
    }

    template<>
    inline void rotate_x<GLfloat>(GLfloat angle)
    {
        rotate(angle, 1.0f, 0.0f, 0.0f);
    }

    template<>
    inline void rotate_y<GLfloat>(GLfloat angle)
    {
        rotate(angle, 0.0f, 0.1f, 0.0f);
    }

    template<>
    inline void rotate_z<GLfloat>(GLfloat angle)
    {
        rotate(angle, 0.0f, 0.0f, 0.1f);
    }


    ////////////////////////////////////////////////////////////////////////////////
    template<>
    inline void scale<GLdouble>(GLdouble x, GLdouble y, GLdouble z)
    {
        glScaled(x, y, z);
    }

    template<>
    inline void scale<GLfloat>(GLfloat x, GLfloat y, GLfloat z)
    {
        glScalef(x, y, z);
    }

    template<>
    inline void scale_x<GLdouble>(GLdouble x)
    {
        scale(x, 1.0, 1.0);
    }

    template<>
    inline void scale_y<GLdouble>(GLdouble y)
    {
        scale(1.0, y, 1.0);
    }

    template<>
    inline void scale_z<GLdouble>(GLdouble z)
    {
        scale(1.0, 1.0, z);
    }

    template<>
    inline void scale_x<GLfloat>(GLfloat x)
    {
        scale(x, 1.0f, 1.0f);
    }

    template<>
    inline void scale_y<GLfloat>(GLfloat y)
    {
        scale(1.0f, y, 1.0f);
    }

    template<>
    inline void scale_z<GLfloat>(GLfloat z)
    {
        scale(1.0f, 1.0f, z);
    }

    ////////////////////////////////////////////////////////////////////////////////
    template<>
    inline void translate<GLdouble>(GLdouble x, GLdouble y, GLdouble z)
    {
        glTranslated(x, y, z);
    }

    template<>
    inline void translate<GLfloat>(GLfloat x, GLfloat y, GLfloat z)
    {
        glTranslatef(x, y, z);
    }

    template<>
    inline void translate<GLdouble>(GLdouble x, GLdouble y)
    {
        translate(x, y, 0.0);
    }

    template<>
    inline void translate<GLfloat>(GLfloat x, GLfloat y)
    {
        translate(x, y, 0.0f);
    }

    template<>
    inline void translate_x<GLdouble>(GLdouble x)
    {
        translate(x, 0.0, 0.0);
    }

    template<>
    inline void translate_y<GLdouble>(GLdouble y)
    {
        translate(0.0, y, 0.0);
    }

    template<>
    inline void translate_z<GLdouble>(GLdouble z)
    {
        translate(0.0, 0.0, z);
    }

    template<>
    inline void translate_x<GLfloat>(GLfloat x)
    {
        translate(x, 0.0f, 0.0f);
    }

    template<>
    inline void translate_y<GLfloat>(GLfloat y)
    {
        translate(0.0f, y, 0.0f);
    }

    template<>
    inline void translate_z<GLfloat>(GLfloat z)
    {
        translate(0.0f, 0.0f, z);
    }

}}}

#endif // BOOST__GUIGL__GL__TRANSFORM_HPP
