// (C) Copyright 2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GUI_WIN_AREA_HPP
#define BOOST_GUI_WIN_AREA_HPP

#if(( defined(_MSC_VER) && (_MSC_VER >= 1020 )) || defined(__WAVE__))
#  pragma once
#endif

#if defined(BOOST_MSVC)
#  pragma warning( push )
#  pragma warning( disable : 4355 ) // 'this' : used in base member initializer list
#endif

#include <boost/gui/win/windows.hpp>
#include <boost/property/scalar_property.hpp>
#include <boost/property/readonly_aliased_property.hpp>
#include <boost/property/readonly_object_property.hpp>

namespace boost { namespace gui { namespace win
{

class area: public RECT
{
   public:
      typedef long value_type;
      typedef RECT native_type;
   public:
      boost::scalar_property< boost::readonly_aliased_property< value_type > >
         xpos;
      boost::scalar_property< boost::readonly_aliased_property< value_type > >
         ypos;

      inline value_type get_width() const;
      boost::scalar_property< boost::readonly_object_property
      <
         value_type, area, &area::get_width
      > > width;

      inline value_type get_height() const;
      boost::scalar_property< boost::readonly_object_property
      <
         value_type, area, &area::get_height
      > > height;
   public:
      inline void inflate(  const size::native_type     & sz );
      inline bool contains( const position::native_type & p );
      inline bool contains( const native_type & a );
   public:
      inline area & operator=( const area & a );
      inline area & operator=( const native_type & a );
   public:
      inline area( const area & a );
      inline area( const native_type & a );
      inline area
      (
         const value_type & _x, const value_type & _y,
         const value_type & _w, const value_type & _h
      );
};

area::value_type area::get_width() const
{
   return right - left;
}

area::value_type area::get_height() const
{
   return bottom - top;
}

void area::inflate( const size::native_type & sz )
{
   left   -= sz.cx;
   top    -= sz.cy;
   right  += sz.cx;
   bottom += sz.cy;
}

bool area::contains( const position::native_type & p )
{
   return p.x >= left && p.x < right && p.y >= top && p.y < bottom;
}

bool area::contains( const native_type & a )
{
   return
   (
      a.left >= left && a.right  <= right &&
      a.top  >= top  && a.bottom <= bottom
   );
}

area & area::operator=( const area & a )
{
   left   = a.left;
   top    = a.top;
   right  = a.right;
   bottom = a.bottom;
   return *this;
}

area & area::operator=( const native_type & a )
{
   left   = a.left;
   top    = a.top;
   right  = a.right;
   bottom = a.bottom;
   return *this;
}

area::area( const area & a ):
   xpos( left ), ypos( top ), width( this ), height( this )
{
   left   = a.left;
   top    = a.top;
   right  = a.right;
   bottom = a.bottom;
}

area::area( const native_type & a ):
   xpos( left ), ypos( top ), width( this ), height( this )
{
   left   = a.left;
   top    = a.top;
   right  = a.right;
   bottom = a.bottom;
}

area::area
(
   const value_type & _x, const value_type & _y,
   const value_type & _w, const value_type & _h
):
   xpos( left ), ypos( top ), width( this ), height( this )
{
   left   = _x;
   top    = _y;
   right  = _x + _w;
   bottom = _y + _h;
}

}}}

#if defined(BOOST_MSVC)
#  pragma warning( pop )
#endif

#endif
