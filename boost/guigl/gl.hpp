/*=================================---------------------------------------------
Copyright 2009 Stjepan Rajko, Torba Andrey

Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__GL_HPP
#define BOOST__GUIGL__GL_HPP

#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/guigl/platform/opengl.hpp>
#include <boost/guigl/platform/glut.hpp>

namespace boost{ namespace guigl { namespace gl {

    //////////////////////////////////////////////////////////////////////////
    template<class T>
    void vertex(T x, T y);

    template<class T>
    void vertex(T x, T y, T z);

    template<class T>
    void vertex(T x, T y, T z, T w);

    template<class T>
    void vertex2(T const* const);

    template<class T>
    void vertex3(T const* const);

    template<class T>
    void vertex4(T const* const);

    template<class T>
    void vertex(boost::array<T, 2> const& a);

    template<class T>
    void vertex(boost::array<T, 3> const& a);

    template<class T>
    void vertex(boost::array<T, 4> const& a);

    //////////////////////////////////////////////////////////////////////////
    template<class T>
    void color(T r, T g, T b);

    template<class T>
    void color(T r, T g, T b, T a);

    template<class T>
    void color3(T const* const v);

    template<class T>
    void color4(T const* const v);

    template<class T>
    void color(boost::array<T, 3> const& a);

    template<class T>
    void color(boost::array<T, 4> const& a);

    //////////////////////////////////////////////////////////////////////////
    template<class T>
    void rect(T x1, T y1, T x2, T y2);

    template<class T>
    void rect(T const* v1, T const* v2);

    template<class T>
    void rect(boost::array<T, 2> const& pt1, boost::array<T, 2> const& pt2);

    ////////////////////////////////////////////////////////////////////////////////
    void load_identity();
    void push_matrix();
    void pop_matrix();

    struct scoped_matrix : boost::noncopyable
    {
        scoped_matrix();
        ~scoped_matrix();
    };

    ////////////////////////////////////////////////////////////////////////////////
    template<class T>
    void rotate(T angle, T x, T y, T z);

    template<class T>
    void rotate_x(T angle);

    template<class T>
    void rotate_y(T angle);

    template<class T>
    void rotate_z(T angle);

    ////////////////////////////////////////////////////////////////////////////////
    template<class T>
    void scale(T x, T y, T z);

    template<class T>
    void scale_x(T x);

    template<class T>
    void scale_y(T y);

    template<class T>
    void scale_z(T z);

    ////////////////////////////////////////////////////////////////////////////////
    template<class T>
    void translate(T x, T y, T z);

    template<class T>
    void translate(T x, T y);

    template<class T>
    void translate_x(T x);

    template<class T>
    void translate_y(T y);

    template<class T>
    void translate_z(T z);

    //////////////////////////////////////////////////////////////////////////
    template<class T>
    void light(GLenum i_light, GLenum pname, T param);

    template<class T>
    void light(GLenum i_light, GLenum pname, T const* param);

    template<class T>
    void light_position(GLenum i_light, T x, T y, T z, T w);

    template<class T>
    void light_constant_attenuation(GLenum i_light, T value);

    template<class T>
    void light_linear_attenuation(GLenum i_light, T value);

    template<class T>
    void light_quadratic_attenuation(GLenum i_light, T value);

    template<class T>
    void light_spot_cutoff(GLenum i_light, T value);

    template<class T>
    void light_spot_exponent(GLenum i_light, T value);

    template<class T>
    void light_spot_direction(GLenum i_light, T x, T y, T z);

    template<class T>
    void light_specular(GLenum i_light, T r, T g, T b, T a);

    template<class T>
    void light_diffuse(GLenum i_light, T r, T g, T b, T a);

    template<class T>
    void light_ambient(GLenum i_light, T r, T g, T b, T a);

    //////////////////////////////////////////////////////////////////////////
    class light_setup
    {
    private:
        GLenum m_light;
        light_setup(GLenum i_light):m_light(i_light){};

        friend light_setup light(GLenum i_light);

    public:
        template<class T>
        light_setup const& position(T x, T y, T z, T w) const;

        template<class T>
        light_setup const& constant_attenuation(T value) const;

        template<class T>
        light_setup const& linear_attenuation(T value) const;

        template<class T>
        light_setup const& quadratic_attenuation(T value) const;

        template<class T>
        light_setup const& spot_cutoff(T value) const;

        template<class T>
        light_setup const& spot_exponent(T value) const;

        template<class T>
        light_setup const& spot_direction(T x, T y, T z) const;

        template<class T>
        light_setup const& specular(T r, T g, T b, T a) const;

        template<class T>
        light_setup const& diffuse(T r, T g, T b, T a) const;

        template<class T>
        light_setup const& ambient(T r, T g, T b, T a) const;
    };

    light_setup light(GLenum i_light);

    //////////////////////////////////////////////////////////////////////////
    template<class T>
    void material(GLenum face, GLenum pname, T param);

    template<class T>
    void material(GLenum face, GLenum pname, T const* param);

    template<class T>
    void material_ambient(GLenum face, T r, T g, T b, T a);

    template<class T>
    void material_diffuse(GLenum face, T r, T g, T b, T a);

    template<class T>
    void material_specular(GLenum face, T r, T g, T b, T a);

    template<class T>
    void material_shininess(GLenum face, T value);

    template<class T>
    void material_emission(GLenum face, T r, T g, T b, T a);

    class material_setup
    {
    private:
        GLenum m_face;
        material_setup(GLenum face):m_face(face){};

        friend material_setup material(GLenum i_light);

    public:
        template<class T>
        material_setup const& ambient(T r, T g, T b, T a) const;

        template<class T>
        material_setup const& diffuse(T r, T g, T b, T a) const;

        template<class T>
        material_setup const& specular(T r, T g, T b, T a) const;

        template<class T>
        material_setup const& shininess(T value) const;

        template<class T>
        material_setup const& emission(T r, T g, T b, T a) const;
    };

    material_setup material(GLenum face);

    //////////////////////////////////////////////////////////////////////////
    void wired_cube(GLdouble size);
    void solid_cube(GLdouble size);
    void wire_sphere(GLdouble radius, GLint slices, GLint stacks);
    void solid_sphere(GLdouble radius, GLint slices, GLint stacks);
    void wire_cone(GLdouble base, GLdouble height, GLint slices, GLint stacks);
    void solid_cone(GLdouble base, GLdouble height, GLint slices, GLint stacks);
    void wire_torus(GLdouble innerRadius, GLdouble outerRadius, GLint sides, GLint rings);
    void solid_torus(GLdouble innerRadius, GLdouble outerRadius, GLint sides, GLint rings);
    void wire_dodecahedron();
    void solid_dodecahedron();
    void wire_octahedron();
    void solid_octahedron();
    void wire_tetrahedron();
    void solid_tetrahedron();
    void wire_icosahedron();
    void solid_icosahedron();
    void wire_teapot(GLdouble size);
    void solid_teapot(GLdouble size);

    //////////////////////////////////////////////////////////////////////////
    inline void line_width(float width) {glLineWidth(width);}
    inline void point_size(float width) {glPointSize(width);}

}}}

#include "platform/impl/vertex.hpp"
#include "platform/impl/color.hpp"
#include "platform/impl/rect.hpp"
#include "platform/impl/transform.hpp"
#include "platform/impl/light.hpp"
#include "platform/impl/material.hpp"
#include "platform/impl/glut.hpp"
#include "platform/impl/tess.hpp"

#endif BOOST__GUIGL__GL_HPP
