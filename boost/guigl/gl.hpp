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
    void translate_x(T x);

    template<class T>
    void translate_y(T y);

    template<class T>
    void translate_z(T z);

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

}}}

#include "platform/impl/vertex.hpp"
#include "platform/impl/color.hpp"
#include "platform/impl/rect.hpp"
#include "platform/impl/transform.hpp"
#include "platform/impl/light.hpp"
#include "platform/impl/glut.hpp"

#endif BOOST__GUIGL__GL_HPP
