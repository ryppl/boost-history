// (C) Copyright 2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GUI_COCOA_AREA_HPP
#define BOOST_GUI_COCOA_AREA_HPP

#if(( defined(_MSC_VER) && (_MSC_VER >= 1020 )) || defined(__WAVE__))
#  pragma once
#endif

#include <boost/gui/cocoa/cocoa.hpp>
#include <boost/property/scalar_property.hpp>
#include <boost/property/aliased_property.hpp>

namespace boost { namespace gui { namespace cocoa
{

class area: public NSRect
{
   public:
      typedef float  value_type;
      typedef NSRect native_type;
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
      inline area( const native_type & a );
      inline area
      (
         const value_type & _x, const value_type & _y,
         const value_type & _w, const value_type & _h
      );
};

void area::inflate( const size::native_type & sz )
{
   origin.x    -= sz.width;
   origin.y    -= sz.height;
   size.width  += sz.width;
   size.height += sz.height;
}

bool area::contains( const position::native_type & p )
{
   return
   (
      p.x >= origin.x && p.x < ( origin.x + size.width  ) &&
      p.y >= origin.y && p.y < ( origin.x + size.height )
   );
}

bool area::contains( const native_type & a )
{
   return
   (
      a.origin.x >= origin.x && a.size.width  <= size.width &&
      a.origin.y >= origin.y && a.size.height <= size.height
   );
}

area & area::operator=( const area & a )
{
   origin.x    = a.origin.x;
   origin.y    = a.origin.y;
   size.width  = a.size.width;
   size.height = a.size.height;
   return *this;
}

area & area::operator=( const native_type & a )
{
   origin.x    = a.origin.x;
   origin.y    = a.origin.y;
   size.width  = a.size.width;
   size.height = a.size.height;
   return *this;
}

area::area( const native_type & a ):
   xpos( origin.x ), ypos( origin.y ),
   width( size.width ), height( size.height )
{
   origin.x    = a.origin.x;
   origin.y    = a.origin.y;
   size.width  = a.size.width;
   size.height = a.size.height;
}

area::area
(
   const value_type & _x, const value_type & _y,
   const value_type & _w, const value_type & _h
):
   xpos( origin.x ), ypos( origin.y ),
   width( size.width ), height( size.height )
{
   origin.x    = _x;
   origin.y    = _y;
   size.width  = _w;
   size.height = _h;
}

}}}

#endif
