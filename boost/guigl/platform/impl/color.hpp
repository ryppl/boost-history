#ifndef BOOST__GUIGL__GL__COLOR_HPP
#define BOOST__GUIGL__GL__COLOR_HPP

namespace boost{ namespace guigl { namespace gl {

    //////////////////////////////////////////////////////////////////////////
#   define BOOST_GUIGL_GL_DECLARE(type_, pref_)\
    template<>\
    inline void color<type_>(type_ r, type_ g, type_ b)\
    {\
    glColor3##pref_(r, g, b);\
    }

    BOOST_GUIGL_GL_DECLARE(GLbyte, b);
    BOOST_GUIGL_GL_DECLARE(GLdouble, d);
    BOOST_GUIGL_GL_DECLARE(GLfloat, f);
    BOOST_GUIGL_GL_DECLARE(GLint, i);
    BOOST_GUIGL_GL_DECLARE(GLshort, s);
    BOOST_GUIGL_GL_DECLARE(GLubyte, ub);
    BOOST_GUIGL_GL_DECLARE(GLuint, ui);
    BOOST_GUIGL_GL_DECLARE(GLushort, us);

#   undef BOOST_GUIGL_GL_DECLARE

    //////////////////////////////////////////////////////////////////////////
#   define BOOST_GUIGL_GL_DECLARE(type_, pref_)\
    template<>\
    inline void color<type_>(type_ r, type_ g, type_ b, type_ a)\
    {\
    glColor4##pref_(r, g, b, a);\
    }

    BOOST_GUIGL_GL_DECLARE(GLbyte, b);
    BOOST_GUIGL_GL_DECLARE(GLdouble, d);
    BOOST_GUIGL_GL_DECLARE(GLfloat, f);
    BOOST_GUIGL_GL_DECLARE(GLint, i);
    BOOST_GUIGL_GL_DECLARE(GLshort, s);
    BOOST_GUIGL_GL_DECLARE(GLubyte, ub);
    BOOST_GUIGL_GL_DECLARE(GLuint, ui);
    BOOST_GUIGL_GL_DECLARE(GLushort, us);

#   undef BOOST_GUIGL_GL_DECLARE

    //////////////////////////////////////////////////////////////////////////
#   define BOOST_GUIGL_GL_DECLARE(type_, pref_, dimension_)\
    template<>\
    inline void color##dimension_<type_>(type_ const* const v)\
    {\
    glColor##dimension_##pref_##v(v);\
    }

    BOOST_GUIGL_GL_DECLARE(GLbyte, b, 3);
    BOOST_GUIGL_GL_DECLARE(GLdouble, d, 3);
    BOOST_GUIGL_GL_DECLARE(GLfloat, f, 3);
    BOOST_GUIGL_GL_DECLARE(GLint, i, 3);
    BOOST_GUIGL_GL_DECLARE(GLshort, s, 3);
    BOOST_GUIGL_GL_DECLARE(GLubyte, ub, 3);
    BOOST_GUIGL_GL_DECLARE(GLuint, ui, 3);
    BOOST_GUIGL_GL_DECLARE(GLushort, us, 3);

    BOOST_GUIGL_GL_DECLARE(GLbyte, b, 4);
    BOOST_GUIGL_GL_DECLARE(GLdouble, d, 4);
    BOOST_GUIGL_GL_DECLARE(GLfloat, f, 4);
    BOOST_GUIGL_GL_DECLARE(GLint, i, 4);
    BOOST_GUIGL_GL_DECLARE(GLshort, s, 4);
    BOOST_GUIGL_GL_DECLARE(GLubyte, ub, 4);
    BOOST_GUIGL_GL_DECLARE(GLuint, ui, 4);
    BOOST_GUIGL_GL_DECLARE(GLushort, us, 4);

#   undef BOOST_GUIGL_GL_DECLARE

    //////////////////////////////////////////////////////////////////////////
#   define BOOST_GUIGL_GL_DECLARE(type_, pref_, dimension_)\
    template<>\
    inline void color<type_>(boost::array<type_, dimension_> const& a)\
    {\
    color##dimension_(&a[0]);\
    }

    BOOST_GUIGL_GL_DECLARE(GLbyte, b, 3);
    BOOST_GUIGL_GL_DECLARE(GLdouble, d, 3);
    BOOST_GUIGL_GL_DECLARE(GLfloat, f, 3);
    BOOST_GUIGL_GL_DECLARE(GLint, i, 3);
    BOOST_GUIGL_GL_DECLARE(GLshort, s, 3);
    BOOST_GUIGL_GL_DECLARE(GLubyte, ub, 3);
    BOOST_GUIGL_GL_DECLARE(GLuint, ui, 3);
    BOOST_GUIGL_GL_DECLARE(GLushort, us, 3);

    BOOST_GUIGL_GL_DECLARE(GLbyte, b, 4);
    BOOST_GUIGL_GL_DECLARE(GLdouble, d, 4);
    BOOST_GUIGL_GL_DECLARE(GLfloat, f, 4);
    BOOST_GUIGL_GL_DECLARE(GLint, i, 4);
    BOOST_GUIGL_GL_DECLARE(GLshort, s, 4);
    BOOST_GUIGL_GL_DECLARE(GLubyte, ub, 4);
    BOOST_GUIGL_GL_DECLARE(GLuint, ui, 4);
    BOOST_GUIGL_GL_DECLARE(GLushort, us, 4);

#   undef BOOST_GUIGL_GL_DECLARE

}}}

#endif BOOST__GUIGL__GL__COLOR_HPP
