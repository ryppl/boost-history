/*=================================---------------------------------------------
Copyright 2008 Torba Andrey

Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__COLOR_HPP
#define BOOST__GUIGL__COLOR_HPP

#include <boost/guigl/types.hpp>

namespace boost { namespace guigl {

inline color_type make_color(float r, float g, float b, float a)
{
  return color_type(r, g, b, a);
}

inline color_type make_color(float r, float g, float b)
{
  return make_color(r, g, b, 1);
}

inline color_type red(float alpha = 1)
{
  return make_color(1, 0, 0, alpha);
}

inline color_type green(float alpha = 1)
{
  return make_color(0, 1, 0, alpha);
}

inline color_type blue(float alpha = 1)
{
  return make_color(0, 0, 1, alpha);
}

inline color_type yellow(float alpha = 1)
{
  return make_color(1, 1, 0, alpha);
}

inline color_type white(float alpha = 1)
{
  return make_color(1, 1, 1, alpha);
}

inline color_type black(float alpha = 1)
{
  return make_color(0, 0, 0, alpha);
}

}}

#endif BOOST__GUIGL__COLOR_HPP
