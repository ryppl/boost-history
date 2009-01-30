/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__DRAW_HPP
#define BOOST__GUIGL__DRAW_HPP


#include <boost/guigl/platform/opengl.hpp>
#include <boost/guigl/types.hpp>


namespace boost { namespace guigl {

namespace draw {

  inline color_type red()
    {
    return color_type(1, 0, 0);
    }

  inline color_type green()
    {
    return color_type(0, 1, 0);
    }

  inline color_type blue()
    {
    return color_type(0, 0, 1);
    }

  class begin : boost::noncopyable
    {
    public:
      explicit begin(int operation)
        {
        glBegin(operation);
        }

      ~begin()
        {
        glEnd();
        }
    };

  inline void vertex(double x, double y)
    {
    glVertex2d(x, y);
    }

  inline void vertex(const position_type &position)
    {
    vertex(position.x, position.y);
    }

  inline void color(const color_type &color)
    {
    glColor3d(color[0], color[1], color[2]);
    }

  struct vertex_creator : boost::noncopyable
    {
    inline vertex_creator& operator()(const position_type &position)
      {
      vertex(position);
      return *this;
      }

    inline vertex_creator& operator()(double x, double y)
      {
      vertex(x, y);
      return *this;
      }
    };

  struct point_creator : begin, vertex_creator
    {

    point_creator()
      : begin(GL_POINTS), vertex_creator()
      {
      }

    point_creator(double x, double y)
      : begin(GL_POINTS), vertex_creator()
      {
      vertex(x, y);
      }
    };

  inline void point(double x, double y)
    {
    point_creator pt(x, y);
    }

  inline void point(const position_type &position)
    {
    point(position.x, position.y);
    }

  inline void point(const color_type& cl, double x, double y)
    {
    color(cl);
    point(x, y);
    }

  inline void point(const color_type& cl, const position_type &position)
    {
    color(cl);
    point(position);
    }

  inline point_creator points(double x, double y)
    {
    return point_creator(x, y);
    }

  inline point_creator points(const position_type &position)
    {
    return points(position.x, position.y);
    }

  inline point_creator points(const color_type& cl)
    {
    color(cl);
    return point_creator();
    }

  inline void line(
    const position_type &first_point,
    const position_type &second_point)
    {
    begin bounds(GL_LINES);
    vertex(first_point);
    vertex(second_point);
    }

  inline void line(
    const color_type& cl,
    const position_type &first_point,
    const position_type &second_point)
    {
    color(cl);
    line(first_point, second_point);
    }

  //template<class ForwardIterator>
  //inline void line_loop(
  //  ForwardIterator begin_it,
  //  ForwardIterator end_it)
  //  {
  //  begin bounds(GL_LINE_LOOP);
  //  std::for_each(begin_it, end_it, &vertex);
  //  }

  struct line_loop_creator : begin, vertex_creator
    {

    line_loop_creator()
      : begin(GL_LINE_LOOP), vertex_creator()
      {
      }

    line_loop_creator(double x, double y)
      : begin(GL_LINE_LOOP), vertex_creator()
      {
      vertex(x, y);
      }
    };

  //template<class ForwardIterator>
  //inline void line_loop(
  //  const color_type& cl,
  //  ForwardIterator begin_it,
  //  ForwardIterator end_it)
  //  {
  //  color(cl);
  //  line_loop(begin_it, end_it);
  //  }

  inline line_loop_creator line_loop(double x, double y)
    {
    return line_loop_creator(x, y);
    }

  inline line_loop_creator line_loop(const position_type& position)
    {
    return line_loop(position.x, position.y);
    }

}

}}

#endif // BOOST__GUIGL__DRAW_HPP
