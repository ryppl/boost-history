/*=================================---------------------------------------------
Copyright 2009 Andrey Torba

Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__GL__TESS_HPP
#define BOOST__GUIGL__GL__TESS_HPP

#include <boost/guigl/types.hpp>
#include <boost/guigl/platform/glu.hpp>

namespace boost{ namespace guigl { namespace gl {

    class tess : boost::noncopyable {
    private:
        GLUtesselator *m_tess;
        friend class polygon;
    public:
        tess();
        ~tess();

        inline bool operator!() const;
        GLUtesselator * get() const;

        void begin_callback(void (CALLBACK *cb)(GLenum which));
        void end_callback(void (CALLBACK *cb)());
        void vertex_callback(void (CALLBACK *cb)(void const* data));
        void error_callback(void (CALLBACK *cb)(GLenum error_code));

        class contour;
        class polygon : boost::noncopyable {
        private:
            GLUtesselator *m_tess;
            friend contour;

        public:
            explicit polygon(tess const& t);
            ~polygon();
        };

        class contour : boost::noncopyable {
        private:
          GLUtesselator *m_tess;

        public:
          explicit contour(polygon const& pg);
          ~contour();

          void operator()(double coord[3], void *data);
          void operator()(double coord[3]);
          };
    };

    ////////////////////////////////////////////////////////////////////////////////
    inline tess::tess()
      {
      m_tess = gluNewTess();
      BOOST_ASSERT(m_tess);
      if(!m_tess) return ;
      }

    inline tess::~tess() { gluDeleteTess(m_tess); }

    inline bool tess::operator!() const {return !m_tess;}

    //inline GLUtesselator * tess::get() const {return m_tess;}

    inline void tess::begin_callback(void (CALLBACK * cb)(GLenum which))
      {
      gluTessCallback(m_tess, GLU_TESS_BEGIN, (void (CALLBACK *)())cb);
      }

    inline void tess::end_callback(void (CALLBACK *cb)())
      {
      gluTessCallback(m_tess, GLU_TESS_END, cb);
      }

    inline void tess::vertex_callback(void (CALLBACK *cb)(void const* data))
      {
      gluTessCallback(m_tess, GLU_TESS_VERTEX, (void (CALLBACK *)())cb);
      }

    inline void tess::error_callback(void (CALLBACK *cb)(GLenum error_code))
      {
      gluTessCallback(m_tess, GLU_TESS_ERROR, (void (CALLBACK *)())cb);
      }

    ////////////////////////////////////////////////////////////////////////////////
    inline tess::polygon::polygon(tess const& t)
      : m_tess(t.m_tess)
      {
      BOOST_ASSERT(m_tess);
      gluTessBeginPolygon(m_tess, 0);
      }
    inline tess::polygon::~polygon(){ gluTessEndPolygon(m_tess); }

    ////////////////////////////////////////////////////////////////////////////////
    inline tess::contour::contour(polygon const& pg)
      : m_tess(pg.m_tess)
      {
      BOOST_ASSERT(m_tess);
      gluTessBeginContour(m_tess);
      }
    inline tess::contour::~contour(){ gluTessEndContour(m_tess); }

    inline void tess::contour::operator()(double coord[3], void *data)
      {
      gluTessVertex(m_tess, coord, data);
      }

    inline void tess::contour::operator()(double coord[3])
      {
      (*this)(coord, coord);
      }
}}}

#endif // BOOST__GUIGL__GL__TESS_HPP
