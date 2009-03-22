/*=================================---------------------------------------------
Copyright 2009 Andrey Torba

Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__COLOR_HPP
#define BOOST__GUIGL__COLOR_HPP

#include <boost/guigl/types.hpp>

namespace boost { namespace guigl {

    template<class T>
    color_type make_color(T r, T g, T b, T a);

    template<>
    inline color_type make_color<float>(float r, float g, float b, float a)
    {
        return color_type(r, g, b, a);
    }

    template<>
    inline color_type make_color<double>(double r, double g, double b, double a)
    {
        return make_color(
            static_cast<float>(r),
            static_cast<float>(g),
            static_cast<float>(b),
            static_cast<float>(a));
    }

    template<>
    inline color_type make_color<int>(int r, int g, int b, int a)
    {
        return make_color(
            static_cast<float>(r)/255,
            static_cast<float>(g)/255,
            static_cast<float>(b)/255,
            static_cast<float>(a)/255);
    }

    template<class T>
    color_type make_color(T r, T g, T b);

    template<>
    inline color_type make_color<float>(float r, float g, float b)
    {
        return make_color(r, g, b, 1.0f);
    }

    template<>
    inline color_type make_color<double>(double r, double g, double b)
    {
        return make_color(r, g, b, 1.0);
    }

    template<>
    inline color_type make_color<int>(int r, int g, int b)
    {
        return make_color(r, g, b, 255);
    }

#define BOOST_GUIGL_COLOR(name, red, green, blue)   \
    inline color_type name(float alpha = 1.0f)      \
    {                                               \
    return make_color(red, green, blue, alpha); \
    }

    BOOST_GUIGL_COLOR(red,      1.0f, 0.0f, 0.0f);
    BOOST_GUIGL_COLOR(green,    0.0f, 1.0f, 0.0f);
    BOOST_GUIGL_COLOR(blue,     0.0f, 0.0f, 1.0f);
    BOOST_GUIGL_COLOR(black,    0.0f, 0.0f, 0.0f);
    BOOST_GUIGL_COLOR(white,    1.0f, 1.0f, 1.0f);
    BOOST_GUIGL_COLOR(yellow,   1.0f, 1.0f, 0.0f);

    inline color_type grey(float brightness = 0.5f, float alpha = 1.0f)
    {
        return make_color(brightness, brightness, brightness, alpha);
    }

#undef BOOST_GUIGL_COLOR

}}

#include <boost/guigl/gl.hpp>
// TODO: move to another place
namespace boost { namespace guigl { namespace gl {
    inline void color(color_type const& clr)
    {
        // TODO: use gil concepts to access channels
        color(
            static_cast<float>(clr[0]),
            static_cast<float>(clr[1]),
            static_cast<float>(clr[2]),
            static_cast<float>(clr[3]));
    }
}}}

#endif BOOST__GUIGL__COLOR_HPP
