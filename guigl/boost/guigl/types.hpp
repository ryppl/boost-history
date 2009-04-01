/*=================================---------------------------------------------
    Copyright 2008,2009 Stjepan Rajko, Andrey Torba
  
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
struct color_type : public gil::rgba32f_pixel_t
{
    typedef gil::rgba32f_pixel_t base_type;
    typedef gil::bits32f channel_t;
    
    explicit color_type(channel_t v)
        : base_type(v)
    {}
    color_type(channel_t v0, channel_t v1, channel_t v2, channel_t v3 = static_cast<channel_t>(1))
        : base_type(v0,v1,v2,v3)
    {}
    color_type &operator=(const color_type &rhs)
    {
        static_cast<base_type &>(*this) = static_cast<const base_type &>(rhs);
        return *this;
    }
};

}}

#endif // BOOST__GUIGL__TYPES_HPP
