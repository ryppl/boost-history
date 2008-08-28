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
typedef gil::rgb32f_pixel_t color_type;

}}

#endif // BOOST__GUIGL__TYPES_HPP
