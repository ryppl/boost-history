/*=================================---------------------------------------------
Copyright 2009 Stjepan Rajko, Torba Andrey

Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__GL__VERTEX_HPP
#define BOOST__GUIGL__GL__VERTEX_HPP

namespace boost { namespace guigl { namespace gl {

    //////////////////////////////////////////////////////////////////////////
#   define BOOST_GUIGL_GL_DECLARE(type_, pref_)\
    template<>\
    inline void vertex<type_>(type_ x, type_ y)\
    {\
    glVertex2##pref_(x, y);\
    }

    BOOST_GUIGL_GL_DECLARE(GLdouble, d);
    BOOST_GUIGL_GL_DECLARE(GLfloat, f);
    BOOST_GUIGL_GL_DECLARE(GLint, i);
    BOOST_GUIGL_GL_DECLARE(GLshort, s);
#   undef BOOST_GUIGL_GL_DECLARE

    //////////////////////////////////////////////////////////////////////////
#   define BOOST_GUIGL_GL_DECLARE(type_, pref_)\
    template<>\
    inline void vertex<type_>(type_ x, type_ y, type_ z)\
    {\
    glVertex3##pref_(x, y, z);\
    }

    BOOST_GUIGL_GL_DECLARE(GLdouble, d);
    BOOST_GUIGL_GL_DECLARE(GLfloat, f);
    BOOST_GUIGL_GL_DECLARE(GLint, i);
    BOOST_GUIGL_GL_DECLARE(GLshort, s);
#   undef BOOST_GUIGL_GL_DECLARE

    //////////////////////////////////////////////////////////////////////////
#   define BOOST_GUIGL_GL_DECLARE(type_, pref_)\
    template<>\
    inline void vertex<type_>(type_ x, type_ y, type_ z, type_ w)\
    {\
    glVertex4##pref_(x, y, z, w);\
    }

    BOOST_GUIGL_GL_DECLARE(GLdouble, d);
    BOOST_GUIGL_GL_DECLARE(GLfloat, f);
    BOOST_GUIGL_GL_DECLARE(GLint, i);
    BOOST_GUIGL_GL_DECLARE(GLshort, s);
#   undef BOOST_GUIGL_GL_DECLARE

    //////////////////////////////////////////////////////////////////////////
#   define BOOST_GUIGL_GL_DECLARE(type_, pref_, dimension_)\
    template<>\
    inline void vertex##dimension_<type_>(type_ const* const v)\
    {\
    glVertex##dimension_##pref_##v(v);\
    }

    BOOST_GUIGL_GL_DECLARE(GLdouble, d, 2);
    BOOST_GUIGL_GL_DECLARE(GLfloat, f, 2);
    BOOST_GUIGL_GL_DECLARE(GLint, i, 2);
    BOOST_GUIGL_GL_DECLARE(GLshort, s, 2);

    BOOST_GUIGL_GL_DECLARE(GLdouble, d, 3);
    BOOST_GUIGL_GL_DECLARE(GLfloat, f, 3);
    BOOST_GUIGL_GL_DECLARE(GLint, i, 3);
    BOOST_GUIGL_GL_DECLARE(GLshort, s, 3);

    BOOST_GUIGL_GL_DECLARE(GLdouble, d, 4);
    BOOST_GUIGL_GL_DECLARE(GLfloat, f, 4);
    BOOST_GUIGL_GL_DECLARE(GLint, i, 4);
    BOOST_GUIGL_GL_DECLARE(GLshort, s, 4);
#   undef BOOST_GUIGL_GL_DECLARE

    //////////////////////////////////////////////////////////////////////////
#   define BOOST_GUIGL_GL_DECLARE(type_, pref_, dimension_)\
    template<>\
    inline void vertex<type_>(boost::array<type_, dimension_> const& a)\
    {\
    vertex##dimension_(&a[0]);\
    }

    BOOST_GUIGL_GL_DECLARE(GLdouble, d, 2);
    BOOST_GUIGL_GL_DECLARE(GLfloat, f, 2);
    BOOST_GUIGL_GL_DECLARE(GLint, i, 2);
    BOOST_GUIGL_GL_DECLARE(GLshort, s, 2);

    BOOST_GUIGL_GL_DECLARE(GLdouble, d, 3);
    BOOST_GUIGL_GL_DECLARE(GLfloat, f, 3);
    BOOST_GUIGL_GL_DECLARE(GLint, i, 3);
    BOOST_GUIGL_GL_DECLARE(GLshort, s, 3);

    BOOST_GUIGL_GL_DECLARE(GLdouble, d, 4);
    BOOST_GUIGL_GL_DECLARE(GLfloat, f, 4);
    BOOST_GUIGL_GL_DECLARE(GLint, i, 4);
    BOOST_GUIGL_GL_DECLARE(GLshort, s, 4);
#   undef BOOST_GUIGL_GL_DECLARE

}}} // gl

#endif BOOST__GUIGL__GL__VERTEX_HPP
