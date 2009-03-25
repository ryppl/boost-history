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

using namespace boost::guigl;

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

      gl::color(blue());
      glLineWidth(1);
      ggl::draw(segment<VL>());
      ggl::draw(segment<HT>());
      ggl::draw(segment<VR>());
      ggl::draw(segment<HB>());

      gl::color(green(0.5));
      glLineWidth(2);
      ggl::draw(segment<HC>());
      ggl::draw(segment<VC>());

      gl::color(red(0.2));
      glLineWidth(5);
      ggl::draw(segment<D1>());
      ggl::draw(segment<D2>());

      }

    BOOST_GUIGL_WIDGET_DRAW_IMPL(my_widget);
  };

int main()
  {
  window wnd((
    _label = "custom example",
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
