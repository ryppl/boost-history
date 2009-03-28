/*=================================---------------------------------------------
Copyright 2009 Andrey Torba

Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__GGL_HPP
#define BOOST__GUIGL__GGL_HPP

#include <boost/guigl/gl.hpp>
#include <boost/guigl/types.hpp>

#include <geometry/geometries/point.hpp>
#include <geometry/geometries/box.hpp>
#include <geometry/geometries/segment.hpp>
#include <geometry/geometries/linestring.hpp>
#include <geometry/geometries/linear_ring.hpp>
#include <geometry/geometries/polygon.hpp>
#include <geometry/geometries/nsphere.hpp>

#include <geometry/core/concepts/point_concept.hpp>
#include <geometry/core/concepts/segment_concept.hpp>
#include <geometry/core/concepts/box_concept.hpp>
#include <geometry/core/concepts/linestring_concept.hpp>
#include <geometry/core/concepts/ring_concept.hpp>
#include <geometry/core/concepts/polygon_concept.hpp>
#include <geometry/core/concepts/nsphere_concept.hpp>

#include <geometry/core/cs.hpp>
#include <geometry/geometries/register/register_point.hpp>
#include <geometry/algorithms/foreach.hpp>
#include <boost/static_assert.hpp>
#include <boost/foreach.hpp>

// GEOMETRY_REGISTER_POINT_2D needs type without '::'
typedef boost::guigl::position_type boost__guigl__position_type;
GEOMETRY_REGISTER_POINT_2D( boost__guigl__position_type, double, geometry::cs::cartesian, operator[](0), operator[](1) );

// GGL doesn't have a GEOMETRY_REGISTER_SEGMENT_2D macro
namespace geometry { namespace traits {

    // tag trait
    template<>
    struct tag<boost::guigl::segment_type>
    {
        typedef
            geometry::segment_tag
            type;
    };

    // point_type trait
    template<>
    struct point_type<boost::guigl::segment_type>
    {
        typedef
            boost::guigl::segment_type::first_type
            type;
    };

    // indexed_access trait
    template<int I, int D>
    struct indexed_access<boost::guigl::segment_type,I,D>
    {
        BOOST_STATIC_ASSERT( (0 == I) || (1 == I) );

        typedef
            geometry::traits::point_type<boost::guigl::segment_type>::type
            point_type;

        typedef
            geometry::traits::coordinate_type<point_type>::type
            coordinate_type;

        static inline coordinate_type get(boost::guigl::segment_type const& segment)
        {
            return (I == 0)
                ? geometry::get<D>(segment.first)
                : geometry::get<D>(segment.second);
        }

        static inline void set(boost::guigl::segment_type const& segment, coordinate_type const& x)
        {
            return (I == 0)
                ? geometry::set<D>(segment.first, x)
                : geometry::set<D>(segment.second, x);
        }

    };
}}

namespace boost{ namespace guigl { namespace ggl {

  template<class T>
  inline
  BOOST_CONCEPT_REQUIRES(((geometry::ConstBox<T>)),
    (void))
    rect(T const& g)
    {
    gl::rect(
      geometry::get<geometry::min_corner, 0>(g),
      geometry::get<geometry::min_corner, 1>(g),
      geometry::get<geometry::max_corner, 0>(g),
      geometry::get<geometry::max_corner, 1>(g));
    }

    namespace dispatch
    {
        template<int N>
        struct pvertex_;
        
        template<>
        struct pvertex_<2>
        {
            template<typename G>
            inline void operator()(G const& g)
            {
                boost::guigl::gl::vertex(
                    geometry::get<0>(g),
                    geometry::get<1>(g));
            }
        };

        template<>
        struct pvertex_<3>
        {
            template<typename G>
            inline void operator()(G const& g)
            {
                boost::guigl::gl::vertex(
                    geometry::get<0>(g),
                    geometry::get<1>(g),
                    geometry::get<2>(g));
            }
        };

        template<>
        struct pvertex_<4>
        {
            template<typename G>
            inline void operator()(G const& g)
            {
                boost::guigl::gl::vertex(
                    geometry::get<0>(g),
                    geometry::get<1>(g),
                    geometry::get<2>(g),
                    geometry::get<3>(g));
            }
        };
        
        template <typename Tag, typename G>
        struct object_tag
        {
            //static inline void vertex(G const& ) {}
            //static inline void draw(G const& g) {}
        };

        ////////////////////////////////////////////////////////////////////////////////
        // Point
        template<typename G>
        struct object_tag<geometry::point_tag, G>
        {
        public:
            static inline
                BOOST_CONCEPT_REQUIRES(((geometry::ConstPoint<G>)),
                (void))
                vertex(G const& g)
            {
                pvertex_<geometry::traits::dimension<G>::value>()(g);
            }

            static inline
                BOOST_CONCEPT_REQUIRES(((geometry::ConstPoint<G>)),
                (void))
                draw(G const& g)
            {
                glBegin(GL_POINTS);
                vertex(g);
                glEnd();
            }
        };
    }

    template<class Tag, class G>
    void vertex(G const& g)
    {
        dispatch::object_tag<Tag, G>::vertex(g);
    }

    template<class G>
    void vertex(G const& g)
    {
        vertex<typename geometry::traits::tag<G>::type>(g);
    }

    struct vertex_drawer {
        template<class G>
        void operator()(G const& g) const {vertex(g);}
    };

  template<class Point>
  class tess : public boost::guigl::gl::tess
    {
    public:
      typedef boost::guigl::gl::tess base_type;
      typedef Point point_type;

    private:

      static void CALLBACK begin_cb(GLenum which)
        {
        glBegin(which);
        }

      static void CALLBACK end_cb()
        {
        glEnd();
        }
      static void CALLBACK vertex_cb(const GLvoid *data)
        {
        point_type const& pt =
          *((point_type const*)data);

        boost::guigl::ggl::vertex(pt);
        }

      static void CALLBACK error_cb(GLenum error_code)
        {
        BOOST_ASSERT(!gluGetString(error_code));
        }

    public:
      tess() {
        if(base_type::operator !())
          throw std::runtime_error("invalid tess");

        begin_callback(begin_cb);
        end_callback(end_cb);
        vertex_callback(vertex_cb);
        error_callback(error_cb);
        }

      class contour : public base_type::contour
        {
        private:

          inline void init_coordinates(double coord[3], point_type const& pt, boost::mpl::int_<2>)
            {
            coord[0] = geometry::get<0>(pt);
            coord[1] = geometry::get<1>(pt);
            coord[2] = 0.0;
            }

          inline void init_coordinates(double coord[3], point_type const& pt, boost::mpl::int_<3>)
            {
            coord[0] = geometry::get<0>(pt);
            coord[1] = geometry::get<1>(pt);
            coord[2] = geometry::get<2>(pt);
            }

        public:
          inline explicit contour(polygon const& pg)
          : base_type::contour(pg) {}

          inline void operator()(point_type const& pt)
            {
            double coord[3];
            init_coordinates(coord, pt, geometry::dimension<point_type>());
            base_type::contour::operator()(coord, (void*)&pt);
            }
        
        };
    };

    namespace dispatch
    {
        template<int N>
        struct svertex_;
        
        template<>
        struct svertex_<2>
        {
            template<typename G>
            inline void operator()(G const& g)
            {
                boost::guigl::gl::vertex(
                    geometry::get<0, 0>(g),
                    geometry::get<0, 1>(g));
                boost::guigl::gl::vertex(
                    geometry::get<1, 0>(g),
                    geometry::get<1, 1>(g));
            }
        };

        template<>
        struct svertex_<3>
        {
            template<typename G>
            inline void operator()(G const& g)
            {
                boost::guigl::gl::vertex(
                    geometry::get<0, 0>(g),
                    geometry::get<0, 1>(g),
                    geometry::get<0, 2>(g));
                boost::guigl::gl::vertex(
                    geometry::get<1, 0>(g),
                    geometry::get<1, 1>(g),
                    geometry::get<1, 2>(g));
            }
        };

        template<>
        struct svertex_<4>
        {
            template<typename G>
            inline void operator()(G const& g)
            {
                boost::guigl::gl::vertex(
                    geometry::get<0, 0>(g),
                    geometry::get<0, 1>(g),
                    geometry::get<0, 2>(g),
                    geometry::get<0, 3>(g));
                boost::guigl::gl::vertex(
                    geometry::get<1, 0>(g),
                    geometry::get<1, 1>(g),
                    geometry::get<1, 2>(g),
                    geometry::get<1, 3>(g));
            }
        };


        ////////////////////////////////////////////////////////////////////////////////
        // Segment
        template<typename G>
        struct object_tag<geometry::segment_tag, G>
        {
        public:
            static inline
                BOOST_CONCEPT_REQUIRES(((geometry::ConstSegment<G>)),
                (void))
                vertex(G const& g)
            {
                typedef typename geometry::traits::point_type<G>::type point_type;
                svertex_<geometry::traits::dimension<point_type>::value>()(g);
            }

            static inline
                BOOST_CONCEPT_REQUIRES(((geometry::ConstSegment<G>)),
                (void))
                draw(G const& g)
            {
                glBegin(GL_LINES);
                vertex(g);
                glEnd();
            }
        };

        template<int N>
        struct bvertex_;
        
        template<>
        struct bvertex_<2>
        {
            template<typename G>
            inline void operator()(G const& g)
            {
                boost::guigl::gl::vertex(
                    geometry::get<geometry::min_corner, 0>(g),
                    geometry::get<geometry::min_corner, 1>(g));
                boost::guigl::gl::vertex(
                    geometry::get<geometry::min_corner, 0>(g),
                    geometry::get<geometry::max_corner, 1>(g));
                boost::guigl::gl::vertex(
                    geometry::get<geometry::max_corner, 0>(g),
                    geometry::get<geometry::max_corner, 1>(g));
                boost::guigl::gl::vertex(
                    geometry::get<geometry::max_corner, 0>(g),
                    geometry::get<geometry::min_corner, 1>(g));
            }
        };
        
        ////////////////////////////////////////////////////////////////////////////////
        // Box
        template<typename G>
        struct object_tag<geometry::box_tag, G>
        {
            // support for 3d

        public:
            static inline
                BOOST_CONCEPT_REQUIRES(((geometry::ConstBox<G>)),
                (void))
                vertex(G const& g)
            {
                typedef typename geometry::traits::point_type<G>::type point_type;
                bvertex_<geometry::traits::dimension<point_type>::value>()(g);
            }

            static inline
                BOOST_CONCEPT_REQUIRES(((geometry::ConstBox<G>)),
                (void))
                draw(G const& g)
            {
                glBegin(GL_LINE_LOOP);
                vertex(g);
                glEnd();
            }
        };

        ////////////////////////////////////////////////////////////////////////////////
        // Point Set
        template<typename G, int GLMode, template<typename T> class Concept>
        struct point_set
        {
        public:
            static inline
                BOOST_CONCEPT_REQUIRES(((Concept<G>)),
                (void))
                vertex(G const& g)
            {
                std::for_each(
                    boost::begin(g), boost::end(g),
                    vertex_drawer());
            }

            static inline
                BOOST_CONCEPT_REQUIRES(((Concept<G>)),
                (void))
                draw(G const& g)
            {
                glBegin(GLMode);
                vertex(g);
                glEnd();
            }
        };

        ////////////////////////////////////////////////////////////////////////////////
        // Linestring
        template<typename G>
        struct object_tag<geometry::linestring_tag, G>
            : public point_set<G, GL_LINE_STRIP, geometry::ConstLinestring>
        {
        };

        ////////////////////////////////////////////////////////////////////////////////
        // Ring
        template<typename G>
        struct object_tag<geometry::ring_tag, G>
            : public point_set<G, GL_LINE_LOOP, geometry::ConstRing>
        {
        };

        ////////////////////////////////////////////////////////////////////////////////
        // polygon_tag
        template<typename G>
        struct object_tag<geometry::polygon_tag, G>
        {
        public:
            static inline
                BOOST_CONCEPT_REQUIRES(((geometry::ConstPolygon<G>)),
                (void))
                vertex(G const& g)
            {
            geometry::for_each_point(g, vertex_drawer());
            }

            static inline
                BOOST_CONCEPT_REQUIRES(((geometry::ConstPolygon<G>)),
                (void))
                draw(G const& g)
            {
            typedef
              typename geometry::point_type<G>::type
              point_type;
            typedef
              typename geometry::ring_type<G>::type
              ring_type;
            typedef
              ::boost::guigl::ggl::tess<point_type>
              tess_type;

            tess_type t;
            typename tess_type::polygon p(t);
              {
              typename tess_type::contour c(p);
              BOOST_FOREACH(point_type const& pt, geometry::exterior_ring(g))
                c(pt);
              }
              BOOST_FOREACH(ring_type const& ring, geometry::interior_rings(g))
                {
                typename tess_type::contour c(p);
                BOOST_FOREACH(point_type const& pt, ring)
                  c(pt);
                }
            }
        };

    }

    template<class Tag, class G>
    void draw(G const& g)
    {
        dispatch::object_tag<Tag, G>::draw(g);
    }

    template<class G>
    void draw(G const& g)
    {
        draw<typename geometry::traits::tag<G>::type>(g);
    }

    struct drawer {
        template<class G>
        void operator()(G const& g) const {draw(g);}
    };

}}}

#endif // BOOST__GUIGL__GGL_HPP
