// (C) Copyright 2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GUI_SIZE_HPP
#define BOOST_GUI_SIZE_HPP

#if(( defined(_MSC_VER) && (_MSC_VER >= 1020 )) || defined(__WAVE__))
#  pragma once
#endif

#include <boost/gui/platform.hpp>

namespace boost { namespace gui
{

/** Platform-independant size object.
 * The size object is represented as a vector that contains a dx property for
 * width and a dy property for height. As such, it provides standard vector
 * space operations (+, -, * and /).
 */

class size: public platf::size
{
   public:
      typedef platf::size::native_type native_type;
      typedef platf::size::value_type  value_type;
   public:
      inline size operator-() const;
   public:
      inline size & operator+=( const size & sz );
      inline size & operator-=( const size & sz );
      inline size & operator*=( const size & sz );
      inline size & operator/=( const size & sz );
      inline size & operator =( const native_type & sz );
   public:
      inline size & operator*=( const value_type & sf );
      inline size & operator/=( const value_type & sf );
   public:
      inline size();
      /** construct a size using the platform-native representation. */
      inline size( const native_type & sz );
      /** construct a size specifying its width and height. */
      inline size( const value_type & _w, const value_type & _h );
};

// global operators

inline size operator+( const size & s1, const size & s2 );
inline size operator-( const size & s1, const size & s2 );
inline size operator*( const size & s1, const size & s2 );
inline size operator*( const size & s1, const size::value_type & sf );
inline size operator/( const size & s1, const size & s2 );
inline size operator/( const size & s1, const size::value_type & sf );

inline bool operator==( const size & s1, const size & s2 );
inline bool operator!=( const size & s1, const size & s2 );

// implementation:

size size::operator-() const
{
   return size( -dx, -dy );
}

size & size::operator+=( const size & sz )
{
   *this = size( dx + sz.dx, dy + sz.dy );
   return *this;
}

size & size::operator-=( const size & sz )
{
   *this = size( dx - sz.dx, dy - sz.dy );
   return *this;
}

size & size::operator*=( const size & sz )
{
   *this = size( dx * sz.dx, dy * sz.dy );
   return *this;
}

size & size::operator/=( const size & sz )
{
   *this = size( dx / sz.dx, dy / sz.dy );
   return *this;
}

size & size::operator=( const native_type & sz )
{
   platf::size::operator=( sz );
   return *this;
}

size & size::operator*=( const value_type & sf )
{
   *this = size( dx * sf, dy * sf );
   return *this;
}

size & size::operator/=( const value_type & sf )
{
   *this = size( dx / sf, dy / sf );
   return *this;
}

size::size(): platf::size( 0, 0 )
{
}

size::size( const native_type & sz ):
   platf::size( sz )
{
}

size::size( const value_type & _w, const value_type & _h ):
   platf::size( _w, _h )
{
}

// global operators

size operator+( const size & s1, const size & s2 )
{
   return size( s1.dx + s2.dx, s1.dy + s2.dy );
}

size operator-( const size & s1, const size & s2 )
{
   return size( s1.dx - s2.dx, s1.dy - s2.dy );
}

size operator*( const size & s1, const size & s2 )
{
   return size( s1.dx * s2.dx, s1.dy * s2.dy );
}

size operator*( const size & s1, const size::value_type &  sf )
{
   return size( s1.dx * sf, s1.dy * sf );
}

size operator/( const size & s1, const size & s2 )
{
   return size( s1.dx / s2.dx, s1.dy / s2.dy );
}

size operator/( const size & s1, const size::value_type &  sf )
{
   return size( s1.dx / sf, s1.dy / sf );
}

bool operator==( const size & s1, const size & s2 )
{
   return s1.dx == s2.dx && s1.dy == s2.dy;
}

bool operator!=( const size & s1, const size & s2 )
{
   return !( s1 == s2 );
}

}}

#endif
