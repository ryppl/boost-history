// (C) Copyright 2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GUI_POSITION_HPP
#define BOOST_GUI_POSITION_HPP

#if(( defined(_MSC_VER) && (_MSC_VER >= 1020 )) || defined(__WAVE__))
#  pragma once
#endif

#include <boost/gui/platform.hpp>
#include <boost/gui/size.hpp>

namespace boost { namespace gui
{

/** Platform-independant position object.
 * A position object is a location in 2D Euclidian geometry at (xpos, ypos),
 * where xpos and ypos are properties.
 */

class position: public platf::position
{
   public:
      typedef platf::position::native_type native_type;
      typedef platf::position::value_type  value_type;
   public:
      /** offset the position by <code>sz</code>. */
      inline position & operator+=( const size & sz );
      /** offset the position by <code>-sz</code>. */
      inline position & operator-=( const size & sz );
      inline position & operator =( const native_type & p );
   public:
      inline position();
      /** construct a position using the platform-native representation. */
      inline position( const native_type & p );
      /** construct a position specifying the x and y coordinates. */
      inline position( const value_type & _x, const value_type & _y );
};

// global operators

/** get the distance between two points. */
inline size operator-( const position & p1, const position & p2 );
/** offset the position <code>p</code> by <code>sz</code>. */
inline position operator+( const position & p, const size & sz );
/** offset the position <code>p</code> by <code>-sz</code>. */
inline position operator-( const position & p, const size & sz );

inline bool operator==( const position & p1, const position & p2 );
inline bool operator!=( const position & p1, const position & p2 );

// implementation:

position & position::operator+=( const size & sz )
{
   *this = position( xpos + sz.dx, ypos + sz.dy );
   return *this;
}

position & position::operator-=( const size & sz )
{
   *this = position( xpos - sz.dx, ypos - sz.dy );
   return *this;
}

position & position::operator=( const native_type & p )
{
   platf::position::operator=( p );
   return *this;
}

position::position(): platf::position( 0, 0 )
{
}

position::position( const native_type & p ):
   platf::position( p )
{
}

position::position( const value_type & _x, const value_type & _y ):
   platf::position( _x, _y )
{
}

// global operators

size operator-( const position & p1, const position & p2 )
{
   return size( p2.xpos - p1.xpos, p2.ypos - p1.ypos );
}

position operator+( const position & p, const size & sz )
{
   return position( p.xpos + sz.dx, p.ypos + sz.dy );
}

position operator-( const position & p, const size & sz )
{
   return position( p.xpos - sz.dx, p.ypos - sz.dy );
}

bool operator==( const position & p1, const position & p2 )
{
   return p1.xpos == p2.xpos && p1.ypos == p2.ypos;
}

bool operator!=( const position & p1, const position & p2 )
{
   return !( p1 == p2 );
}

}}

#endif
