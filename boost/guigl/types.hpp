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

// temporary place for color functions
inline color_type red(float alpha = 1)
  {
  return color_type(1, 0, 0, alpha);
  }

inline color_type green(float alpha = 1)
  {
  return color_type(0, 1, 0, alpha);
  }

inline color_type blue(float alpha = 1)
  {
  return color_type(0, 0, 1, alpha);
  }

inline color_type yellow(float alpha = 1)
  {
  return color_type(1, 1, 0, alpha);
  }

inline color_type white(float alpha = 1)
  {
  return color_type(1, 1, 1, alpha);
  }

inline color_type black(float alpha = 1)
  {
  return color_type(0, 0, 0, alpha);
  }

}}

#endif // BOOST__GUIGL__TYPES_HPP
