/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__TYPES_HPP
#define BOOST__GUIGL__TYPES_HPP


#include <boost/gil/utilities.hpp>
#include <boost/gil/typedefs.hpp>

namespace boost { namespace guigl {

typedef gil::point2<double> size_type;
typedef gil::point2<double> position_type;
typedef gil::rgba32f_pixel_t color_type;

//TODO: move 'make_color' to appropriate place
inline color_type make_color(float r, float g, float b, float a = 1.0f)
{
    return color_type(r, g, b, a);
}

}}

#endif // BOOST__GUIGL__TYPES_HPP
