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

  inline void vertex(double x, double y)
  {
    glVertex2d(x, y);
  }

  inline void vertex(const position_type &position)
  {
    vertex(position.x, position.y);
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

  template<GLenum type>
  class vertex_creator : boost::noncopyable
  {
  private:
    begin m_scoped_begin;

  public:
    vertex_creator()
      : m_scoped_begin(type)
    {}

    vertex_creator(double x, double y)
      : m_scoped_begin(type)
    {
      vertex(x, y);
    }

    vertex_creator(const position_type& position)
      : m_scoped_begin(type)
    {
      vertex(position);
    }

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

  typedef vertex_creator<GL_POINTS> point_creator;

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

  typedef vertex_creator<GL_LINE_LOOP> line_loop_creator;
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

  typedef vertex_creator<GL_POLYGON> polygon_creator;

  inline polygon_creator polygon(double x, double y)
  {
    return polygon_creator(x, y);
  }

  inline polygon_creator polygon(const position_type& pos)
  {
    return polygon_creator(pos);
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

  inline void rotate_x(double angle)
  {
    glRotated(angle, 1, 0, 0);
  }

  inline void rotate_y(double angle)
  {
    glRotated(angle, 0, 1, 0);
  }

  inline void rotate_z(double angle)
  {
    glRotated(angle, 0, 0, 1);
  }

  inline void translate(double x, double y, double z)
  {
    glTranslated(x, y, z);
  }

  inline void translate_x(double distance)
  {
    translate(distance, 0, 0);
  }

  inline void translate_y(double distance)
  {
    translate(0, distance, 0);
  }

  inline void translate_z(double distance)
  {
    translate(0, 0, distance);
  }

  inline void scale(double x, double y, double z)
  {
    glScaled(x, y, z);
  }

  inline void scale_x(double x)
  {
    scale(x, 1, 1);
  }

  inline void scale_y(double y)
  {
    scale(1, y, 1);
  }

  inline void scale_z(double z)
  {
    scale(1, 1, z);
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

  inline void line_width(float width)
  {
    glLineWidth(width);
  }

}

}}

#endif // BOOST__GUIGL__DRAW_HPP
