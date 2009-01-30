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

  inline position_type zero_position()
  {
    return position_type(0, 0);
  }

  inline void color(const color_type &color)
  {
    glColor4f(color[0], color[1], color[2], color[3]);
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

  struct vertex_creator : boost::noncopyable
  {
    inline vertex_creator const& operator()(const position_type &position) const
    {
      vertex(position);
      return *this;
    }

    inline vertex_creator const& operator()(double x, double y) const
    {
      vertex(x, y);
      return *this;
    }
  };

  struct point_creator : begin, vertex_creator
  {

    point_creator()
      : begin(GL_POINTS), vertex_creator()
    {}

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
    {}

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

  class scoped_matrix : boost::noncopyable
  {
  public:
    scoped_matrix(){ glPushMatrix(); }
    ~scoped_matrix(){ glPopMatrix(); }
  };

  // can the rotation be invoked without matrix?
  // if not then move rotate to scoped_matrix interface
  inline void rotate(double angle, double x, double y, double z)
  {
    glRotated(angle, x, y, z);
  }

  inline void rotateX(double angle)
  {
    glRotated(angle, 1, 0, 0);
  }

  inline void rotateY(double angle)
  {
    glRotated(angle, 0, 1, 0);
  }

  inline void rotateZ(double angle)
  {
    glRotated(angle, 0, 0, 1);
  }

  inline void translate(double x, double y, double z)
  {
    glTranslated(x, y, z);
  }

  inline void translateX(double distance)
  {
    translate(distance, 0, 0);
  }

  inline void translateY(double distance)
  {
    translate(0, distance, 0);
  }

  inline void translateZ(double distance)
  {
    translate(0, 0, distance);
  }

  inline void rect(double x1, double y1, double x2, double y2)
  {
    glRectd(x1, y1, x2, y2);
  }

  inline void rect(
    const position_type& pos1,
    const position_type& pos2)
  {
    rect(pos1.x, pos1.y, pos2.x, pos2.y);
  }

  inline void rect(double w, double h)
  {
    rect(0, 0, w, h);
  }

  inline void rect(double edge)
  {
    rect(edge, edge);
  }

  inline void rect(const position_type& pos2)
  {
    rect(zero_position(), pos2);
  }

}

}}

#endif // BOOST__GUIGL__DRAW_HPP
