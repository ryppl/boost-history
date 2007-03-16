// (C) Copyright 2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GUI_WIN_COLOR_HPP
#define BOOST_GUI_WIN_COLOR_HPP

#if(( defined(_MSC_VER) && (_MSC_VER >= 1020 )) || defined(__WAVE__))
#  pragma once
#endif

#if defined(BOOST_MSVC)
#  pragma warning( push )
#  pragma warning( disable : 4355 ) // 'this' : used in base member initializer list
#endif

#include <boost/gui/win/windows.hpp>
#include <boost/property/scalar_property.hpp>
#include <boost/property/object_property.hpp>

namespace boost { namespace gui { namespace win
{

/** Store an RGBA (Red, Green, Blue, Alpha) color with the form 0xAABBGGRR as
  * a DWORD type (either COLORREF or ARGB).
  */

class color
{
   private:
      DWORD rgba;
   public:
      inline operator DWORD() const; /**< native interoperability */
   public:
      inline int  get_red() const;        /**< get the red color component */
      inline void set_red( const int & ); /**< set the red color component */
      boost::scalar_property< boost::object_property
      <
         int, color, &color::get_red, &color::set_red
      > > red; /**< red color component. */

      inline int  get_green() const;        /**< get the green color component */
      inline void set_green( const int & ); /**< set the green color component */
      boost::scalar_property< boost::object_property
      <
         int, color, &color::get_green, &color::set_green
      > > green; /**< green color component. */

      inline int  get_blue() const;        /**< get the blue color component */
      inline void set_blue( const int & ); /**< set the blue color component */
      boost::scalar_property< boost::object_property
      <
         int, color, &color::get_blue, &color::set_blue
      > > blue; /**< blue color component. */

      inline int  get_alpha() const;        /**< get the alpha color component */
      inline void set_alpha( const int & ); /**< set the alpha color component */
      boost::scalar_property< boost::object_property
      <
         int, color, &color::get_alpha, &color::set_alpha
      > > alpha; /**< alpha color component. */
   public:
      inline color & operator=( const DWORD & c );
      inline color & operator=( const color & c );

      inline color( int r, int g, int b, int a = 0 );
      inline color( const DWORD & c = 0 );
      inline color( const color & c );
};

// native interoperability

color::operator DWORD() const
{
   return rgba;
}

// accessors & modifiers

int color::get_red() const
{
   return rgba & 0xFF;
}

void color::set_red( const int & r )
{
   rgba &= 0xFFFFFF00;
   rgba |= ( r & 0xFF );
}

int color::get_green() const
{
   return ( rgba >> 8 ) & 0xFF;
}

void color::set_green( const int & g )
{
   rgba &= 0xFFFF00FF;
   rgba |= (( g & 0xFF ) << 8 );
}

int color::get_blue() const
{
   return ( rgba >> 16 ) & 0xFF;
}

void color::set_blue( const int & b )
{
   rgba &= 0xFF00FFFF;
   rgba |= (( b & 0xFF ) << 16 );
}

int color::get_alpha() const
{
   return ( rgba >> 24 ) & 0xFF;
}

void color::set_alpha( const int & a )
{
   rgba &= 0x00FFFFFF;
   rgba |= (( a & 0xFF ) << 24 );
}

// assignment operators

color & color::operator=( const DWORD & c )
{
   rgba = c;
   return *this;
}

color & color::operator=( const color & c )
{
   rgba = c.rgba;
   return *this;
}

// constructors

color::color( int r, int g, int b, int a ): rgba( 0 ),
   red( this ), green( this ), blue( this ), alpha( this )
{
   rgba |=  ( r & 0xFF );
   rgba |= (( g & 0xFF ) <<  8 );
   rgba |= (( b & 0xFF ) << 16 );
   rgba |= (( a & 0xFF ) << 24 );
}

color::color( const DWORD & c ): rgba( c ),
   red( this ), green( this ), blue( this ), alpha( this )
{
}

color::color( const color & c ): rgba( c.rgba ),
   red( this ), green( this ), blue( this ), alpha( this )
{
}

}}}

#if defined(BOOST_MSVC)
#  pragma warning( pop )
#endif

#endif
