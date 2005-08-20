// (C) Copyright 2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GUI_PALMOS_AREA_HPP
#define BOOST_GUI_PALMOS_AREA_HPP

#if(( defined(_MSC_VER) && (_MSC_VER >= 1020 )) || defined(__WAVE__))
#  pragma once
#endif

#include <boost/gui/palmos/palmos.hpp>
#include <boost/property/scalar_property.hpp>
#include <boost/property/aliased_property.hpp>

namespace boost { namespace gui { namespace palmos
{

class area: public RectangleType
{
   public:
      typedef Coord         value_type;
      typedef RectangleType native_type;
      typedef boost::scalar_property< boost::aliased_property< value_type > >
         property_type;
   public:
      property_type xpos;
      property_type ypos;
      property_type width;
      property_type height;
   public:
      inline void inflate(  const size::native_type & sz );
      inline bool contains( const position::native_type & p );
      inline bool contains( const native_type & a );
   public:
      inline area & operator=( const area & a );
      inline area & operator=( const native_type & a );
   public:
      inline area( const native_type & nt );
      inline area
      (
         const value_type & _x, const value_type & _y,
         const value_type & _w, const value_type & _h
      );
};

void area::inflate( const size::native_type & sz )
{
   topLeft.x -= sz.x;
   topLeft.y -= sz.y;
   extent.x  += sz.x;
   extent.y  += sz.y;
}

bool area::contains( const position::native_type & p )
{
   return
   (
      p.x >= topLeft.x && p.x < ( topLeft.x + extent.x ) &&
      p.y >= topLeft.y && p.y < ( topLeft.x + extent.y )
   );
}

bool area::contains( const native_type & a )
{
   return
   (
      a.topLeft.x >= topLeft.x && a.extent.x <= extent.x &&
      a.topLeft.y >= topLeft.y && a.extent.y <= extent.y
   );
}

area & area::operator=( const area & a )
{
   topLeft.x = a.topLeft.x;
   topLeft.y = a.topLeft.y;
   extent.x  = a.extent.x;
   extent.y  = a.extent.y;
   return *this;
}

area & area::operator=( const native_type & a )
{
   topLeft.x = a.topLeft.x;
   topLeft.y = a.topLeft.y;
   extent.x  = a.extent.x;
   extent.y  = a.extent.y;
   return *this;
}

area::area( const native_type & a ):
   xpos( topLeft.x ), ypos(  topLeft.y ),
   width( extent.x ), height( extent.y )
{
   topLeft.x = a.topLeft.x;
   topLeft.y = a.topLeft.y;
   extent.x  = a.extent.x;
   extent.y  = a.extent.y;
}

area::area
(
   const value_type & _x, const value_type & _y,
   const value_type & _w, const value_type & _h
):
   xpos( topLeft.x ), ypos(  topLeft.y ),
   width( extent.x ), height( extent.y )
{
   topLeft.x = _x;
   topLeft.y = _y;
   extent.x  = _w;
   extent.y  = _h;
}

}}}

#endif
