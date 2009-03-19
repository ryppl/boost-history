/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


#include <boost/guigl/application.hpp>
#include <boost/guigl/window.hpp>
//#include <boost/guigl/widget/button.hpp>

#include <boost/guigl/view/impl/clickable.hpp>
#include <boost/guigl/view/impl/positioned.hpp>
#include <boost/guigl/view/impl/active_colored.hpp>

#include <boost/guigl/platform/opengl.hpp>

#include <boost/guigl/gl.hpp>

using namespace boost::guigl;

struct circle2d_generator {
  position_type center;
  double radius;

  circle2d_generator(
    position_type center_,
    double radius_):
  center(center_),
    radius(radius_)
    {}

  typedef position_type result_type;
  result_type operator()(double angle) const
    {
    return position_type(
      center.x + std::cos(angle) * radius,
      center.y + std::sin(angle) * radius);
    }
  };

template<class ColorKey, class BaseType>
class color_set {
private:
  typedef std::map<ColorKey, color_type> color_map_type;
  color_map_type m_color_set;

public:
  typedef BaseType base_type;

  template<typename ArgumentPack>
  color_set(const ArgumentPack &args)
    : base_type(args)
    {}

  color_set(const color_set &rhs)
    : base_type(rhs), m_color_set(rhs.m_color_set)
    {}

  void set_color(ColorKey const& key, color_type const& color)
    {
    m_color_set[key] = color;
    }

  bool use_color(ColorKey const& key)
    {
    color_map_type::const_iterator it = m_color_set.find(key);
    if(it == m_color_set.end())
      return false;
    gl::color((*it)[0], (*it)[1], (*it)[2], (*it)[3]);
    return true;
    }

  boost::optional<color_type> get_color(ColorKey const& key) const
    {
    color_map_type::const_iterator it = m_color_set.find(key);
    if(it != m_color_set.end())
      return *it;
    else
      return boost::optional<color_type>();
    }

  };

class sexy_button;

typedef
color_set<int,
//view::solid_background<
//view::clickable<sexy_button,button::left_type,
view::positioned<>
/*>*/   /*>*/   > button_base_type;

class sexy_button : public button_base_type {
private:
  color_type m_bg_color;
  color_type m_highlight_color;

public:
  typedef button_base_type base_type;

  template<typename ArgumentPack>
  sexy_button(const ArgumentPack &args)
    : base_type(args)
    {}

  void draw_circle_segment(circle2d_generator const& circle, int phase, int angle)
    {
    position_type pos;
    for(int i = phase; i < phase+angle; i += 10)
      {
      pos = circle(double(i)/180*3.1425926);
      gl::vertex(pos.x, pos.y);
      }
    position_type last_pos = circle(double(phase+angle)/180*3.1425926);
    if(last_pos != pos)
      gl::vertex(pos.x, pos.y);
    }

  void draw_rounded_rect()
    {
    double radius = (std::min)(10., (std::min)(size().x, size().y)/2);

    draw_circle_segment(
      circle2d_generator(position_type(radius, radius), radius),
      180,
      90);

    draw_circle_segment(
      circle2d_generator(position_type(size().x - radius, radius), radius),
      270,
      90);

    draw_circle_segment(
      circle2d_generator(position_type(size().x - radius, size().y - radius), radius),
      0,
      90);

    draw_circle_segment(
      circle2d_generator(position_type(radius, size().y - radius), radius),
      90,
      90);
    }

  void draw()
    {
    base_type::draw_prologue();

    use_color(0);

    glEnable (GL_LINE_SMOOTH);
    //glEnable (GL_POLYGON_SMOOTH);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);    //glEnable(GL_POLYGON_SMOOTH);
    //glHint (GL_POLYGON_SMOOTH_HINT, GL_NICEST);    //glEnable(GL_POLYGON_SMOOTH);

    glLineWidth(0.1);

    glBegin(GL_POLYGON);
    draw_rounded_rect();
    glEnd();

    glBegin(GL_LINE_LOOP);
    draw_rounded_rect();
    glEnd();

    base_type::draw_epilogue();
    }

  };

int main()
{
    window test_window1(( _label = "custom example", _size=size_type(300,300) ));
    
    test_window1
      << new sexy_button((
      _size=size_type(100, 50),
      _position = position_type(10, 10),
      _active_color = make_color256(12, 114, 163) ))

      << new sexy_button((
      _size=size_type(100, 50),
      _position = position_type(150, 10),
      _active_color = make_color256(12, 114, 163) ))
      ;

    application::run();
}
