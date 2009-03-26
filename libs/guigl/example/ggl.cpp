/*=================================---------------------------------------------
Copyright 2009 Andrey Torba

Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#include <boost/guigl/application.hpp>
#include <boost/guigl/window.hpp>
#include <boost/guigl/view/positioned.hpp>
#include <boost/guigl/view/impl/positioned.hpp>

#include <boost/guigl/ggl.hpp>
#include <boost/guigl/gl.hpp>

#include <boost/foreach.hpp>

#include <boost/assign/std.hpp>

#include <geometry/geometries/geometries.hpp>
#include <geometry/algorithms/correct.hpp>

namespace geometry { namespace traits{
    template<class T>
    struct point_type<std::vector<T> >
    {
        typedef T type;
    };
}}

using namespace boost::guigl;

void CALLBACK tess_begin_cb(GLenum which) {
  //std::cout << __FUNCTION__ << ": " << which << std::endl;
  glBegin(which);
  }
void CALLBACK tess_end_cb() {
  //std::cout << __FUNCTION__ << std::endl;
  glEnd();
  }
void CALLBACK tess_vertex_cb(const GLvoid *data) {
  //std::cout << __FUNCTION__ << std::endl;
  glVertex3dv((const GLdouble*)data);
  }
void CALLBACK tess_error_cb(GLenum errorCode)
{
    const GLubyte *errorStr;
    errorStr = gluErrorString(errorCode);
    std::cerr << "[ERROR]: " << errorStr << std::endl;
}

class tess : boost::noncopyable {
public:
    GLUtesselator *m_tess;

public:
    tess() {
        m_tess = gluNewTess(); // create a tessellator
        if(!m_tess) throw std::runtime_error("failed to create tessellation object");
    }
    ~tess() { gluDeleteTess(m_tess); }

    class polygon : boost::noncopyable {
    public:
        GLUtesselator *m_tess;
    public:
        explicit polygon(tess const& t)
            : m_tess(t.m_tess)
        { gluTessBeginPolygon(m_tess, 0); }
        ~polygon(){ gluTessEndPolygon(m_tess); }

        class contour : boost::noncopyable {
        public:
            GLUtesselator *m_tess;

        public:
            explicit contour(polygon const& pg)
                : m_tess(pg.m_tess)
            { gluTessBeginContour(m_tess); }
            ~contour(){ gluTessEndContour(m_tess); }

            inline void operator()(double coord[3], void *data) const {
                gluTessVertex(m_tess, coord, data);
            }

            inline void operator()(double coord[3]) const {
                (*this)(coord, coord);
            }

            inline void operator()(position_type const& pos) const {
                double coord[3] = {pos[0], pos[1], 0.0};
                (*this)(coord);
            }
            
        };
    };
};

GLuint tessellate1()
{
    glColor3f(1,0,0);

    tess tt;
    GLUtesselator* t = tt.m_tess;
    GLdouble quad1[4][3] = { {-10,30,0}, {0,0,0}, {10,30,0}, {0,20,0} };

    // register callback functions
    gluTessCallback(t, GLU_TESS_BEGIN, (void (CALLBACK *)())tess_begin_cb);
    gluTessCallback(t, GLU_TESS_END, (void (CALLBACK *)())tess_end_cb);
    gluTessCallback(t, GLU_TESS_ERROR, (void (CALLBACK *)())tess_error_cb);
    gluTessCallback(t, GLU_TESS_VERTEX, (void (CALLBACK *)())tess_vertex_cb);

//    glNewList(id, GL_COMPILE);
    {
        tess::polygon p(tt);
        tess::polygon::contour c(p);
        c(quad1[0]);
        c(quad1[1]);
        c(quad1[2]);
        c(quad1[3]);
    }

    return 0;//id;      // return handle ID of a display list
}

typedef view::positioned<> my_widget_base_type;
class my_widget : public my_widget_base_type
{
public:
    typedef my_widget_base_type base_type;

    template<typename ArgumentPack>
    my_widget(const ArgumentPack &args)
        : base_type(args)
    {}

    void draw_prologue()
    {
        base_type::draw_prologue();
        using namespace boost::assign;

        geometry::linear_ring<position_type> r;
        r.push_back(point<LT>());
        r.push_back(point<RB>());
        r.push_back(point<RT>());
        r.push_back(point<LB>());

        // linear_ring
        gl::color(yellow());
        glLineWidth(7);
        ggl::draw(r);

        // box
        gl::color(blue());
        glLineWidth(1);
        geometry::box<position_type> b(
            point<LT>(),
            point<RB>());
        ggl::draw(b);

        // segment
        gl::color(green(0.5f));
        glLineWidth(2);
        ggl::draw(segment<HC>());
        ggl::draw(segment<VC>());

        gl::color(red(0.2f));
        glLineWidth(5);
        ggl::draw(segment<D1>());
        ggl::draw(segment<D2>());

        // std::vector as a ring
        gl::color(black());
        std::vector<position_type> v;
        v +=
            point<LC>(),
            point<CT>(),
            point<RC>(),
            point<CB>();
        ggl::draw<geometry::ring_tag>(v);

        // std::vector as a linestring
        glLineWidth(0.5);
        gl::color(white());
        ggl::draw<geometry::linestring_tag>(v);

        // polygon
        geometry::polygon<position_type> pg;
        pg.outer() +=
            point<LT>(),
            point<RT>(),
            point<RB>(),
            point<LB>();
        pg.inners().resize(1);
        std::copy(v.begin(), v.end(), std::back_inserter(pg.inners().back()));
        geometry::correct(pg);

        //glPointSize(10);
        //gl::color(red());
        //glBegin(GL_POINTS);
        //ggl::vertex(pg);
        //glEnd();

        //GLuint id = glGenLists(1);  // create a display list
        gl::color(red(0.5));
        //{
        //    GLUtesselator *t = gluNewTess(); // create a tessellator
        //    //tess t;
        //    // register callback functions
        //    gluTessCallback(t, GLU_TESS_BEGIN, (void (__stdcall*)(void))tess_begin_cb);
        //    gluTessCallback(t, GLU_TESS_END, (void (__stdcall*)(void))tess_end_cb);
        //    gluTessCallback(t, GLU_TESS_ERROR, (void (__stdcall*)(void))tess_error_cb);
        //    gluTessCallback(t, GLU_TESS_VERTEX, (void (__stdcall*)())tess_vertex_cb);

        //    glNewList(id, GL_COMPILE);
        //    gluTessBeginPolygon(t, 0);
        //    //tess::polygon p(t);
        //    {
        //        {
        //            gluTessBeginContour(t);
        //            //tess::polygon::contour c(t);
        //            BOOST_FOREACH(position_type const& pos, pg.outer())
        //            {
        //                double coord[3] = {pos[0], pos[1], 0.0};
        //                gluTessVertex(t, coord, coord);
        //                //c(pos);
        //            }
        //            gluTessEndContour(t);
        //        }

        //        //BOOST_FOREACH(
        //        //    geometry::linear_ring<position_type> const& ring,
        //        //    pg.inners())
        //        //{
        //        //    tess::polygon::contour c(t);
        //        //    BOOST_FOREACH(position_type const& pos, ring)
        //        //    { c(pos); }
        //        //}
        //    }
        //    gluTessEndPolygon(t);
        //    glEndList();
        //    gluDeleteTess(t);
        //}

        //std::cout << "tess-begin" << std::endl;
        tessellate1();
        //std::cout << "tess-end" << std::endl;
//        glCallList(tessellate1());
    }

    BOOST_GUIGL_WIDGET_DRAW_IMPL(my_widget);
};

int main()
{
    window wnd((
        _label = "GGL example",
        _size = size_type(500, 500),
        _background = white()
        ));

    wnd << new my_widget((
        _size = size_type(300, 300),
        _position = position_type(100, 100)
        ));

    application::run();
    return 0;
}
