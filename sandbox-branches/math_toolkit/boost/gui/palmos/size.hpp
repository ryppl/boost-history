// (C) Copyright 2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GUI_PALMOS_SIZE_HPP
#define BOOST_GUI_PALMOS_SIZE_HPP

#if(( defined(_MSC_VER) && (_MSC_VER >= 1020 )) || defined(__WAVE__))
#  pragma once
#endif

#include <boost/gui/palmos/palmos.hpp>
#include <boost/property/scalar_property.hpp>
#include <boost/property/aliased_property.hpp>

namespace boost { namespace gui { namespace palmos
{

class size: public PointType
{
   public:
      typedef Coord     value_type;
      typedef PointType native_type;
      typedef boost::scalar_property< boost::aliased_property< value_type > >
         property_type;
   public:
      property_type dx;
      property_type dy;
   public:
      inline size & operator=( const size & sz );
      inline size & operator=( const native_type & sz );
   public:
      inline size( const native_type & sz );
      inline size( const value_type & _w, const value_type & _h );
};

size & size::operator=( const size & sz )
{
   x = sz.x;
   y = sz.y;
   return *this;
}

size & size::operator=( const native_type & sz )
{
   x = sz.x;
   y = sz.y;
   return *this;
}

size::size( const native_type & sz ):
   dx( x ), dy( y )
{
   x = sz.x;
   y = sz.y;
}

size::size( const value_type & _w, const value_type & _h ):
   dx( x ), dy( y )
{
   x = _w;
   y = _h;
}

}}}

#endif
