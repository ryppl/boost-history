#ifndef BOOST__GUIGL__GL__RECT_HPP
#define BOOST__GUIGL__GL__RECT_HPP

namespace boost{ namespace guigl { namespace gl {

#   define BOOST_GUIGL_GL_DECLARE(type_, pref_)\
    template<>\
    inline void rect<type_>(type_ x1, type_ y1, type_ x2, type_ y2)\
    {\
        glRect##pref_(x1, y1, x2, y2);\
    }

    BOOST_GUIGL_GL_DECLARE(GLdouble, d);
    BOOST_GUIGL_GL_DECLARE(GLfloat, f);
    BOOST_GUIGL_GL_DECLARE(GLint, i);
    BOOST_GUIGL_GL_DECLARE(GLshort, s);
#   undef BOOST_GUIGL_GL_DECLARE

#   define BOOST_GUIGL_GL_DECLARE(type_, pref_)\
    template<>\
    inline void rect<type_>(type_ const* v1, type_ const* v2)\
    {\
        glRect##pref_##v(v1, v2);\
    }

    BOOST_GUIGL_GL_DECLARE(GLdouble, d);
    BOOST_GUIGL_GL_DECLARE(GLfloat, f);
    BOOST_GUIGL_GL_DECLARE(GLint, i);
    BOOST_GUIGL_GL_DECLARE(GLshort, s);

#   undef BOOST_GUIGL_GL_DECLARE

#   define BOOST_GUIGL_GL_DECLARE(type_, pref_)\
    template<>\
    inline void rect<type_>(\
    boost::array<type_, 2> const& pt1,\
    boost::array<type_, 2> const& pt2)\
    {\
        rect(&pt1[0], &pt2[0]);\
    }

    BOOST_GUIGL_GL_DECLARE(GLdouble, d);
    BOOST_GUIGL_GL_DECLARE(GLfloat, f);
    BOOST_GUIGL_GL_DECLARE(GLint, i);
    BOOST_GUIGL_GL_DECLARE(GLshort, s);

#   undef BOOST_GUIGL_GL_DECLARE

}}}

#endif BOOST__GUIGL__GL__RECT_HPP
