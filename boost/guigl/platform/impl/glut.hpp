#ifndef BOOST__GUIGL__GL__GLUT_HPP
#define BOOST__GUIGL__GL__GLUT_HPP

#include <boost/noncopyable.hpp>

namespace boost{ namespace guigl { namespace gl {

    inline void wire_cube(GLdouble size)
    {
        glutWireCube(size);
    }

    inline void solid_cube(GLdouble size)
    {
        glutSolidCube(size);
    }

    inline void wire_sphere(GLdouble radius, GLint slices, GLint stacks)
    {
        glutWireSphere(radius, slices, stacks);
    }

    inline void solid_sphere(GLdouble radius, GLint slices, GLint stacks)
    {
        glutSolidSphere(radius, slices, stacks);
    }

    inline void wire_cone(GLdouble base, GLdouble height, GLint slices, GLint stacks)
    {
        glutWireCone(base, height, slices, stacks);
    }

    inline void solid_cone(GLdouble base, GLdouble height, GLint slices, GLint stacks)
    {
        glutSolidCone(base, height, slices, stacks);
    }

    inline void wire_torus(GLdouble innerRadius, GLdouble outerRadius, GLint sides, GLint rings)
    {
        glutWireTorus(innerRadius, outerRadius, sides, rings);
    }

    inline void solid_torus(GLdouble innerRadius, GLdouble outerRadius, GLint sides, GLint rings)
    {
        glutSolidTorus(innerRadius, outerRadius, sides, rings);
    }

    inline void wire_dodecahedron()
    {
        glutWireDodecahedron();
    }

    inline void solid_dodecahedron()
    {
        glutSolidDodecahedron();
    }

    inline void wire_octahedron()
    {
        glutWireOctahedron();
    }

    inline void solid_octahedron()
    {
        glutSolidOctahedron();
    }

    inline void wire_tetrahedron()
    {
        glutWireTetrahedron();
    }

    inline void solid_tetrahedron()
    {
        glutSolidTetrahedron();
    }

    inline void wire_icosahedron()
    {
        glutWireIcosahedron();
    }

    inline void solid_icosahedron()
    {
        glutSolidIcosahedron();
    }

    inline void wire_teapot(GLdouble size)
    {
        glutWireTeapot(size);
    }

    inline void solid_teapot(GLdouble size)
    {
        glutSolidTeapot(size);
    }

}}}

#endif
