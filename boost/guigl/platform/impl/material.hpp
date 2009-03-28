/*=================================---------------------------------------------
Copyright 2009 Stjepan Rajko, Torba Andrey

Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__GL__MATERIAL_HPP
#define BOOST__GUIGL__GL__MATERIAL_HPP

namespace boost{ namespace guigl { namespace gl {

    //////////////////////////////////////////////////////////////////////////
    template<>
    inline void material<GLfloat>(GLenum face, GLenum pname, GLfloat param)
    {
        glMaterialf(face, pname, param);
    }

    template<>
    inline void material<GLint>(GLenum face, GLenum pname, GLint param)
    {
        glMateriali(face, pname, param);
    }

    //////////////////////////////////////////////////////////////////////////
    template<>
    inline void material<GLfloat>(GLenum face, GLenum pname, GLfloat const* params)
    {
        glMaterialfv(face, pname, params);
    }

    template<>
    inline void material<GLint>(GLenum face, GLenum pname, GLint const* params)
    {
        glMaterialiv(face, pname, params);
    }

    //////////////////////////////////////////////////////////////////////////
    template<>
    inline void material_ambient<GLfloat>(GLenum face,
        GLfloat r, GLfloat g, GLfloat b, GLfloat a)
    {
        GLfloat const ambient_color[] = {r, g, b, a};
        material(face, GL_AMBIENT, ambient_color);
    }

    template<>
    inline void material_ambient<GLint>(GLenum face,
        GLint r, GLint g, GLint b, GLint a)
    {
        GLint const ambient_color[] = {r, g, b, a};
        material(face, GL_AMBIENT, ambient_color);
    }

    //////////////////////////////////////////////////////////////////////////
    template<>
    inline void material_diffuse<GLfloat>(GLenum face,
        GLfloat r, GLfloat g, GLfloat b, GLfloat a)
    {
        GLfloat const diffuse_color[] = {r, g, b, a};
        material(face, GL_DIFFUSE, diffuse_color);
    }

    template<>
    inline void material_diffuse<GLint>(GLenum face,
        GLint r, GLint g, GLint b, GLint a)
    {
        GLint const diffuse_color[] = {r, g, b, a};
        material(face, GL_DIFFUSE, diffuse_color);
    }

    //////////////////////////////////////////////////////////////////////////
    template<>
    inline void material_specular<GLfloat>(GLenum face,
        GLfloat r, GLfloat g, GLfloat b, GLfloat a)
    {
        GLfloat const specular_color[] = {r, g, b, a};
        material(face, GL_SPECULAR, specular_color);
    }

    template<>
    inline void material_specular<GLint>(GLenum face,
        GLint r, GLint g, GLint b, GLint a)
    {
        GLint const specular_color[] = {r, g, b, a};
        material(face, GL_SPECULAR, specular_color);
    }

    //////////////////////////////////////////////////////////////////////////
    template<>
    inline void material_shininess<GLfloat>(GLenum face, GLfloat value)
    {
        GLfloat const values[] = {value};
        material(face, GL_SHININESS, values);
    }

    template<>
    inline void material_shininess<GLint>(GLenum face, GLint value)
    {
        GLint const values[] = {value};
        material(face, GL_SHININESS, values);
    }

    //////////////////////////////////////////////////////////////////////////
    template<>
    inline void material_emission<GLfloat>(GLenum face,
        GLfloat r, GLfloat g, GLfloat b,  GLfloat a)
    {
        GLfloat const color[] = {r, g, b, a};
        material(face, GL_EMISSION, color);
    }

    template<>
    inline void material_emission<GLint>(GLenum face,
        GLint r, GLint g, GLint b,  GLint a)
    {
        GLint const color[] = {r, g, b, a};
        material(face, GL_EMISSION, color);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    template<class T>
    inline material_setup const& material_setup::ambient(T r, T g, T b, T a) const
    {
        material_ambient(m_face, r, g, b, a);
        return *this;
    }

    template<class T>
    inline material_setup const& material_setup::diffuse(T r, T g, T b, T a) const
    {
        material_diffuse(m_face, r, g, b, a);
        return *this;
    }

    template<class T>
    inline material_setup const& material_setup::specular(T r, T g, T b, T a) const
    {
        material_specular(m_face, r, g, b, a);
        return *this;
    }

    template<class T>
    inline material_setup const& material_setup::shininess(T value) const
    {
        material_shininess(m_face, value);
        return *this;
    }

    template<class T>
    inline material_setup const& material_setup::emission(T r, T g, T b, T a) const
    {
        material_emission(m_face, r, g, b, a);
        return *this;
    }

    inline material_setup material(GLenum face)
    {
        return material_setup(face);
    }
}}}

#endif // BOOST__GUIGL__GL__MATERIAL_HPP
