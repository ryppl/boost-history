#ifndef BOOST__GUIGL__GL__LIGHT_HPP
#define BOOST__GUIGL__GL__LIGHT_HPP

namespace boost{ namespace guigl { namespace gl {

    //////////////////////////////////////////////////////////////////////////
    template<class T>
    inline void light(GLenum i_light, GLenum pname, T param);

    template<>
    inline void light<GLfloat>(GLenum i_light, GLenum pname, GLfloat param)
    {
        glLightf(i_light, pname, param);
    }

    template<>
    inline void light<GLint>(GLenum i_light, GLenum pname, GLint param)
    {
        glLighti(i_light, pname, param);
    }

    //////////////////////////////////////////////////////////////////////////
    template<class T>
    inline void light(GLenum i_light, GLenum pname, T const* param);

    template<>
    inline void light<GLfloat>(GLenum i_light, GLenum pname, GLfloat const* params)
    {
        glLightfv(i_light, pname, params);
    }

    template<>
    inline void light<GLint>(GLenum i_light, GLenum pname, GLint const* params)
    {
        glLightiv(i_light, pname, params);
    }

    //////////////////////////////////////////////////////////////////////////
    template<class T>
    inline void light_position(GLenum i_light, T x, T y, T z, T w);

    template<>
    inline void light_position<GLfloat>(
        GLenum i_light,
        GLfloat x, GLfloat y, GLfloat z, GLfloat w)
    {
        GLfloat const position[] = {x, y, z, w};
        light(i_light, GL_POSITION, position);
    }

    template<>
    inline void light_position<GLint>(
        GLenum i_light,
        GLint x, GLint y, GLint z, GLint w)
    {
        GLint const position[] = {x, y, z, w};
        light(i_light, GL_POSITION, position);
    }

    //////////////////////////////////////////////////////////////////////////
    template<class T>
    void light_constant_ambient(GLenum i_light, T value);

    template<>
    inline void light_constant_ambient<GLfloat>(GLenum i_light, GLfloat value)
    {
        light(i_light, GL_CONSTANT_ATTENUATION, value);
    }

    template<>
    inline void light_constant_ambient<GLint>(GLenum i_light, GLint value)
    {
        light(i_light, GL_CONSTANT_ATTENUATION, value);
    }

    //////////////////////////////////////////////////////////////////////////
    template<class T>
    void light_linear_attenuation(GLenum i_light, T value);

    template<>
    inline void light_linear_attenuation<GLfloat>(GLenum i_light, GLfloat value)
    {
        light(i_light, GL_LINEAR_ATTENUATION, value);
    }

    template<>
    inline void light_linear_attenuation<GLint>(GLenum i_light, GLint value)
    {
        light(i_light, GL_LINEAR_ATTENUATION, value);
    }

    //////////////////////////////////////////////////////////////////////////
    template<class T>
    void light_quadratic_attenuation(GLenum i_light, T value);

    template<>
    inline void light_quadratic_attenuation<GLfloat>(GLenum i_light, GLfloat value)
    {
        light(i_light, GL_QUADRATIC_ATTENUATION, value);
    }

    template<>
    inline void light_quadratic_attenuation<GLint>(GLenum i_light, GLint value)
    {
        light(i_light, GL_QUADRATIC_ATTENUATION, value);
    }

    //////////////////////////////////////////////////////////////////////////
    template<class T>
    void light_spot_cutoff(GLenum i_light, T value);

    template<>
    inline void light_spot_cutoff<GLfloat>(GLenum i_light, GLfloat value)
    {
        light(i_light, GL_SPOT_CUTOFF, value);
    }

    template<>
    inline void light_spot_cutoff<GLint>(GLenum i_light, GLint value)
    {
        light(i_light, GL_SPOT_CUTOFF, value);
    }

    //////////////////////////////////////////////////////////////////////////
    template<class T>
    void light_spot_exponent(GLenum i_light, T value);

    template<>
    inline void light_spot_exponent<GLfloat>(GLenum i_light, GLfloat value)
    {
        light(i_light, GL_SPOT_EXPONENT, value);
    }

    template<>
    inline void light_spot_exponent<GLint>(GLenum i_light, GLint value)
    {
        light(i_light, GL_SPOT_EXPONENT, value);
    }

    //////////////////////////////////////////////////////////////////////////
    template<class T>
    void light_spot_direction(GLenum i_light, T x, T y, T z);

    template<>
    inline void light_spot_direction<GLfloat>(GLenum i_light, GLfloat x, GLfloat y, GLfloat z)
    {
        GLfloat const direction[] = {x, y, z};
        light(i_light, GL_SPOT_DIRECTION, direction);
    }

    template<>
    inline void light_spot_direction<GLint>(GLenum i_light, GLint x, GLint y, GLint z)
    {
        GLint const direction[] = {x, y, z};
        light(i_light, GL_SPOT_DIRECTION, direction);
    }

    //////////////////////////////////////////////////////////////////////////
    template<class T>
    void light_specular(GLenum i_light, T r, T g, T b, T a);

    template<>
    inline void light_specular<GLfloat>(GLenum i_light,
        GLfloat r, GLfloat g, GLfloat b, GLfloat a)
    {
        GLfloat const specular_color[] = {r, g, b, a};
        light(i_light, GL_SPECULAR, specular_color);
    }

    template<>
    inline void light_specular<GLint>(GLenum i_light,
        GLint r, GLint g, GLint b, GLint a)
    {
        GLint const specular_color[] = {r, g, b, a};
        light(i_light, GL_SPECULAR, specular_color);
    }

    //////////////////////////////////////////////////////////////////////////
    template<class T>
    void light_diffuse(GLenum i_light, T r, T g, T b, T a);

    template<>
    inline void light_diffuse<GLfloat>(GLenum i_light,
        GLfloat r, GLfloat g, GLfloat b, GLfloat a)
    {
        GLfloat const diffuse_color[] = {r, g, b, a};
        light(i_light, GL_DIFFUSE, diffuse_color);
    }

    template<>
    inline void light_diffuse<GLint>(GLenum i_light,
        GLint r, GLint g, GLint b, GLint a)
    {
        GLint const diffuse_color[] = {r, g, b, a};
        light(i_light, GL_DIFFUSE, diffuse_color);
    }

    //////////////////////////////////////////////////////////////////////////
    template<class T>
    void light_ambient(GLenum i_light, T r, T g, T b, T a);

    template<>
    inline void light_ambient<GLfloat>(GLenum i_light,
        GLfloat r, GLfloat g, GLfloat b, GLfloat a)
    {
        GLfloat const ambient_color[] = {r, g, b, a};
        light(i_light, GL_AMBIENT, ambient_color);
    }

    template<>
    inline void light_ambient<GLint>(GLenum i_light,
        GLint r, GLint g, GLint b, GLint a)
    {
        GLint const ambient_color[] = {r, g, b, a};
        light(i_light, GL_AMBIENT, ambient_color);
    }

}}}

#endif BOOST__GUIGL__GL__LIGHT_HPP
