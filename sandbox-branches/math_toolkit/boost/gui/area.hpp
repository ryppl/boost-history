// (C) Copyright 2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GUI_AREA_HPP
#define BOOST_GUI_AREA_HPP

#if(( defined(_MSC_VER) && (_MSC_VER >= 1020 )) || defined(__WAVE__))
#  pragma once
#endif

#if defined(BOOST_MSVC)
#  pragma warning( push )
#  pragma warning( disable : 4355 ) // 'this' : used in base member initializer list
#endif

#include <boost/gui/platform.hpp>
#include <boost/gui/position.hpp>
#include <boost/gui/size.hpp>

namespace boost { namespace gui
{

/** Platform-independant area object.
 * Represents a rectangle area that combines a position object (locating either
 * the top-left or bottom-left corner, depending on the platform) and a size
 * object (defining the size of the area from that position).
 */

class area: public platf::area
{
   public:
      typedef platf::area::native_type native_type;
      typedef platf::area::value_type  value_type;
   public:
      inline gui::size get_size() const;
      boost::scalar_property< boost::readonly_object_property
      <
         gui::size, area, &area::get_size
      > > size; /**< the width and height of the area. */

      inline position get_center() const;
      boost::scalar_property< boost::readonly_object_property
      <
         position, area, &area::get_center
      > > center; /**< the central coordinates of the area. */

      inline bool empty() const; /**< does the area have zero size? */
   public:
      /** translate the area by <code>sz</code>. */
      inline area & offset(  const gui::size & sz );
      /** decrease the top-left position and increase the bottom-right position
        * by <code>sz</code>. This retains the areas central position and
        * increases its size by <code>2*sz</code>
        */
      inline area & inflate( const gui::size & sz );
      /** increase the top-left position and decrease the bottom-right position
        * by <code>sz</code>. This retains the areas central position and
        * decreases its size by <code>2*sz</code>
        */
      inline area & deflate( const gui::size & sz );
   public:
      /** offset the area by <code>sz</code> */
      inline area & operator+=( const gui::size & sz );
      /** offset the area by <code>-sz</code> */
      inline area & operator-=( const gui::size & sz );
      inline area & operator =( const area & a );
      inline area & operator =( const native_type & a );
   public:
      inline area();
      /** construct an area using the platform-native representation. */
      inline area( const area & a );
      inline area( const native_type & a );
      /** construct an area by specifying its position and size. */
      inline area( const position & p, const gui::size & sz );
      /** construct an area by specifying its position and size as separate
        * values.
        */
      inline area
      (
         const value_type & _x, const value_type & _y,
         const value_type & _w, const value_type & _h
      );
};

// implementation:

size area::get_size() const
{
   return gui::size( width, height );
}

position area::get_center() const
{
   return position( xpos + ( width / 2 ), ypos + ( height / 2 ));
}

bool area::empty() const
{
   return width == 0 && height == 0;
}

area & area::offset( const gui::size & sz )
{
   *this = area( xpos + sz.dx, ypos + sz.dy, width + sz.dx, height + sz.dy );
   return *this;
}

area & area::inflate( const gui::size & sz )
{
   platf::area::inflate( sz );
   return *this;
}

area & area::deflate( const gui::size & sz )
{
   platf::area::inflate( -sz );
   return *this;
}

area & area::operator+=( const gui::size & sz )
{
   *this = area( xpos, ypos, width + sz.dx, height + sz.dy );
   return *this;
}

area & area::operator-=( const gui::size & sz )
{
   *this = area( xpos, ypos, width - sz.dx, height - sz.dy );
   return *this;
}

area & area::operator=( const area & a )
{
   platf::area::operator=( a );
   return *this;
}

area & area::operator=( const native_type & a )
{
   platf::area::operator=( a );
   return *this;
}

area::area():
   platf::area( 0, 0, 0, 0 ),
   size( this ), center( this )
{
}

area::area( const area & a ):
   platf::area( a ),
   size( this ), center( this )
{
}

area::area( const native_type & a ):
   platf::area( a ),
   size( this ), center( this )
{
}

area::area( const position & p, const gui::size & sz ):
   platf::area( p.xpos, p.ypos, sz.dx, sz.dy ),
   size( this ), center( this )
{
}

area::area
(
   const value_type & _x, const value_type & _y,
   const value_type & _w, const value_type & _h
):
   platf::area( _x, _y, _w, _h ),
   size( this ), center( this )
{
}

}}

#if defined(BOOST_MSVC)
#  pragma warning( pop )
#endif

#endif
