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

  inline color_type make_color(float r, float g, float b, float a = 1.0f)
    {
    return color_type(r, g, b, a);
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

  inline color_type make_color256(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255)
    {
    return make_color(
      static_cast<float>(r)/255,
      static_cast<float>(g)/255,
      static_cast<float>(b)/255,
      static_cast<float>(a)/255);
    }

  }}

#endif BOOST__GUIGL__COLOR_HPP
