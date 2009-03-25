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

#include <boost/ggl/geometry/geometries/point.hpp>
#include <boost/ggl/geometry/geometries/box.hpp>
#include <boost/ggl/geometry/geometries/segment.hpp>
#include <boost/ggl/geometry/geometries/linestring.hpp>
#include <boost/ggl/geometry/geometries/linear_ring.hpp>
#include <boost/ggl/geometry/geometries/polygon.hpp>
#include <boost/ggl/geometry/geometries/nsphere.hpp>

#include <boost/ggl/geometry/core/concepts/point_concept.hpp>
#include <boost/ggl/geometry/core/concepts/segment_concept.hpp>
#include <boost/ggl/geometry/core/concepts/box_concept.hpp>
#include <boost/ggl/geometry/core/concepts/linestring_concept.hpp>
#include <boost/ggl/geometry/core/concepts/ring_concept.hpp>
#include <boost/ggl/geometry/core/concepts/polygon_concept.hpp>
#include <boost/ggl/geometry/core/concepts/nsphere_concept.hpp>

#include <boost/ggl/geometry/core/cs.hpp>
#include <boost/ggl/geometry/geometries/register/register_point.hpp>
#include <boost/static_assert.hpp>

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

  namespace dispatch
    {
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
      private:
        template<int D>
        static inline void vertex_(G const& g);

        template<>
        static inline void vertex_<2>(G const& g)
          {
          boost::guigl::gl::vertex(
            geometry::get<0>(g),
            geometry::get<1>(g));
          }

        template<>
        static inline void vertex_<3>(G const& g)
          {
          boost::guigl::gl::vertex(
            geometry::get<0>(g),
            geometry::get<1>(g),
            geometry::get<2>(g));
          }
        template<>
        static inline void vertex_<4>(G const& g)
          {
          boost::guigl::gl::vertex(
            geometry::get<0>(g),
            geometry::get<1>(g),
            geometry::get<2>(g),
            geometry::get<3>(g));
          }

      public:
        static inline
          BOOST_CONCEPT_REQUIRES(((geometry::ConstPoint<G>)),
          (void))
          vertex(G const& g)
          {
          vertex_<geometry::traits::dimension<G>::value>(g);
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

  namespace dispatch
    {

    ////////////////////////////////////////////////////////////////////////////////
    // Segment
    template<typename G>
    struct object_tag<geometry::segment_tag, G>
      {
      private:
        template<int D>
        static inline void vertex_(const G& g);

        template<>
        static inline void vertex_<2>(const G& g)
          {
          boost::guigl::gl::vertex(
            geometry::get<0, 0>(g),
            geometry::get<0, 1>(g));
          boost::guigl::gl::vertex(
            geometry::get<1, 0>(g),
            geometry::get<1, 1>(g));
          }

        template<>
        static inline void vertex_<3>(const G& g)
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

        template<>
        static inline void vertex_<4>(const G& g)
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

      public:
        static inline
          BOOST_CONCEPT_REQUIRES(((geometry::ConstSegment<G>)),
          (void))
          vertex(G const& g)
          {
          typedef typename geometry::traits::point_type<G>::type point_type;
          vertex_<geometry::traits::dimension<point_type>::value>(g);
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

    ////////////////////////////////////////////////////////////////////////////////
    // Box
    template<typename G>
    struct object_tag<geometry::box_tag, G>
      {
      private:
        template<int D>
        static inline void vertex_(const G& g);

        template<>
        static inline void vertex_<2>(const G& g)
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

        // support for 3d

      public:
        static inline
          BOOST_CONCEPT_REQUIRES(((geometry::ConstBox<G>)),
          (void))
          vertex(G const& g)
          {
          typedef typename geometry::traits::point_type<G>::type point_type;
          vertex_<geometry::traits::dimension<point_type>::value>(g);
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
    // polygon is not implemented yet
    template<typename G>
    struct object_tag<geometry::polygon_tag, G>
      {
      public:
        static inline
          BOOST_CONCEPT_REQUIRES(((geometry::ConstPolygon<G>)),
          (void))
          vertex(G const& g)
          {
          ggl::vertex(geometry::exterior_ring(g));

          std::for_each(
            boost::begin(geometry::interior_rings(g)),
            boost::end(geometry::interior_rings(g)),
            vertex_drawer());
          }

        static inline
          BOOST_CONCEPT_REQUIRES(((geometry::ConstPolygon<G>)),
          (void))
          draw(G const& g)
          {
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

#endif BOOST__GUIGL__GGL_HPP
