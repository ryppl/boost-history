/*=================================---------------------------------------------
Copyright 2009 Andrey Torba

Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__GL__TESS_HPP
#define BOOST__GUIGL__GL__TESS_HPP

#include <boost/guigl/types.hpp>

namespace boost{ namespace guigl { namespace gl {

    class tess : boost::noncopyable {
    private:
        GLUtesselator *m_tess;
        friend class polygon;

        static void CALLBACK begin_cb(GLenum which)
        {
            glBegin(which);
        }

        static void CALLBACK end_cb() {
            glEnd();
        }
        static void CALLBACK vertex_cb(const GLvoid *data)
        {
            boost::guigl::position_type* pos =
                (boost::guigl::position_type*)data;
            glVertex2d(pos->x, pos->y);
        }

        static void CALLBACK error_cb(GLenum errorCode)
        {
            BOOST_ASSERT(!gluErrorString(errorCode));
            //const GLubyte *errorStr;
            //errorStr = gluErrorString(errorCode);
            //std::cerr << "[ERROR]: " << errorStr << std::endl;
        }

    public:
        tess() {
            m_tess = gluNewTess(); // create a tessellator
            BOOST_ASSERT(m_tess);
            if(!m_tess) return ;
            gluTessCallback(m_tess, GLU_TESS_BEGIN, (void (CALLBACK *)())&tess::begin_cb);
            gluTessCallback(m_tess, GLU_TESS_END, (void (CALLBACK *)())&tess::end_cb);
            gluTessCallback(m_tess, GLU_TESS_ERROR, (void (CALLBACK *)())&tess::error_cb);
            gluTessCallback(m_tess, GLU_TESS_VERTEX, (void (CALLBACK *)())&tess::vertex_cb);
        }
        ~tess() { gluDeleteTess(m_tess); }

        bool operator!() const {return !m_tess;}

        class polygon : boost::noncopyable {
        private:
            GLUtesselator *m_tess;
            friend class contour;

        public:
            explicit polygon(tess const& t)
                : m_tess(t.m_tess)
            {
                BOOST_ASSERT(m_tess);
                gluTessBeginPolygon(m_tess, 0);
            }
            ~polygon(){ gluTessEndPolygon(m_tess); }

            class contour : boost::noncopyable {
            private:
                GLUtesselator *m_tess;

            public:
                explicit contour(polygon const& pg)
                    : m_tess(pg.m_tess)
                {
                    BOOST_ASSERT(m_tess);
                    gluTessBeginContour(m_tess);
                }
                ~contour(){ gluTessEndContour(m_tess); }

                inline void operator()(double coord[3], void *data) const {
                    gluTessVertex(m_tess, coord, data);
                }

                inline void operator()(double coord[3]) const {
                    (*this)(coord, coord);
                }

                inline void operator()(boost::guigl::position_type const& pos) const {
                    double coord[3] = {pos.x, pos.y, 0.};
                    (*this)(coord, (void*)&pos);
                }
            };
        };
    };


}}}

#endif // BOOST__GUIGL__GL__TESS_HPP
