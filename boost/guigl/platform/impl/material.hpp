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

    ////////////////////////////////////////////////////////////////////////////
    //template<>
    //inline void light_position<GLfloat>(
    //    GLenum i_light,
    //    GLfloat x, GLfloat y, GLfloat z, GLfloat w)
    //{
    //    GLfloat const position[] = {x, y, z, w};
    //    light(i_light, GL_POSITION, position);
    //}

    //template<>
    //inline void light_position<GLint>(
    //    GLenum i_light,
    //    GLint x, GLint y, GLint z, GLint w)
    //{
    //    GLint const position[] = {x, y, z, w};
    //    light(i_light, GL_POSITION, position);
    //}

    ////////////////////////////////////////////////////////////////////////////
    //template<>
    //inline void light_constant_attenuation<GLfloat>(GLenum i_light, GLfloat value)
    //{
    //    light(i_light, GL_CONSTANT_ATTENUATION, value);
    //}

    //template<>
    //inline void light_constant_attenuation<GLint>(GLenum i_light, GLint value)
    //{
    //    light(i_light, GL_CONSTANT_ATTENUATION, value);
    //}

    ////////////////////////////////////////////////////////////////////////////
    //template<>
    //inline void light_linear_attenuation<GLfloat>(GLenum i_light, GLfloat value)
    //{
    //    light(i_light, GL_LINEAR_ATTENUATION, value);
    //}

    //template<>
    //inline void light_linear_attenuation<GLint>(GLenum i_light, GLint value)
    //{
    //    light(i_light, GL_LINEAR_ATTENUATION, value);
    //}

    ////////////////////////////////////////////////////////////////////////////
    //template<>
    //inline void light_quadratic_attenuation<GLfloat>(GLenum i_light, GLfloat value)
    //{
    //    light(i_light, GL_QUADRATIC_ATTENUATION, value);
    //}

    //template<>
    //inline void light_quadratic_attenuation<GLint>(GLenum i_light, GLint value)
    //{
    //    light(i_light, GL_QUADRATIC_ATTENUATION, value);
    //}

    ////////////////////////////////////////////////////////////////////////////
    //template<>
    //inline void light_spot_cutoff<GLfloat>(GLenum i_light, GLfloat value)
    //{
    //    light(i_light, GL_SPOT_CUTOFF, value);
    //}

    //template<>
    //inline void light_spot_cutoff<GLint>(GLenum i_light, GLint value)
    //{
    //    light(i_light, GL_SPOT_CUTOFF, value);
    //}

    ////////////////////////////////////////////////////////////////////////////
    //template<>
    //inline void light_spot_exponent<GLfloat>(GLenum i_light, GLfloat value)
    //{
    //    light(i_light, GL_SPOT_EXPONENT, value);
    //}

    //template<>
    //inline void light_spot_exponent<GLint>(GLenum i_light, GLint value)
    //{
    //    light(i_light, GL_SPOT_EXPONENT, value);
    //}

    ////////////////////////////////////////////////////////////////////////////
    //template<>
    //inline void light_spot_direction<GLfloat>(GLenum i_light, GLfloat x, GLfloat y, GLfloat z)
    //{
    //    GLfloat const direction[] = {x, y, z};
    //    light(i_light, GL_SPOT_DIRECTION, direction);
    //}

    //template<>
    //inline void light_spot_direction<GLint>(GLenum i_light, GLint x, GLint y, GLint z)
    //{
    //    GLint const direction[] = {x, y, z};
    //    light(i_light, GL_SPOT_DIRECTION, direction);
    //}

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

    ////////////////////////////////////////////////////////////////////////////
    //template<class T>
    //inline light_setup& light_setup::position(T x, T y, T z, T w)
    //{
    //    light_position(m_light, x, y, z, w);
    //    return *this;
    //}

    //template<class T>
    //inline light_setup& light_setup::constant_attenuation(T value)
    //{
    //    light_constant_attenuation(m_light, value);
    //    return *this;
    //}

    //template<class T>
    //inline light_setup& light_setup::linear_attenuation(T value)
    //{
    //    light_linear_attenuation(m_light, value);
    //    return *this;
    //}

    //template<class T>
    //inline light_setup& light_setup::quadratic_attenuation(T value)
    //{
    //    light_quadratic_attenuation(m_light, value);
    //    return *this;
    //}

    //template<class T>
    //inline light_setup& light_setup::spot_cutoff(T value)
    //{
    //    light_spot_cutoff(m_light, value);
    //    return *this;
    //}

    //template<class T>
    //inline light_setup& light_setup::spot_exponent(T value)
    //{
    //    light_spot_exponent(m_light, value);
    //    return *this;
    //}

    //template<class T>
    //inline light_setup& light_setup::spot_direction(T x, T y, T z)
    //{
    //    light_spot_direction(m_light, x, y, z);
    //    return *this;
    //}

    //template<class T>
    //inline light_setup& light_setup::specular(T r, T g, T b, T a)
    //{
    //    light_specular(m_light, r, g, b, a);
    //    return *this;
    //}

    //template<class T>
    //inline light_setup& light_setup::diffuse(T r, T g, T b, T a)
    //{
    //    light_diffuse(m_light, r, g, b, a);
    //    return *this;
    //}

    //template<class T>
    //inline light_setup& light_setup::ambient(T r, T g, T b, T a)
    //{
    //    light_ambient(m_light, r, g, b, a);
    //    return *this;
    //}

    //inline light_setup light(GLenum i_light)
    //{
    //    glEnable(i_light);
    //    return light_setup(i_light);
    //}

}}}

#endif BOOST__GUIGL__GL__MATERIAL_HPP
