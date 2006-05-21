// (C) Copyright 2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GUI_WIN_SIZE_HPP
#define BOOST_GUI_WIN_SIZE_HPP

#if(( defined(_MSC_VER) && (_MSC_VER >= 1020 )) || defined(__WAVE__))
#  pragma once
#endif

#include <boost/gui/win/windows.hpp>
#include <boost/property/scalar_property.hpp>
#include <boost/property/aliased_property.hpp>

namespace boost { namespace gui { namespace win
{

class size: public SIZE
{
   public:
      typedef long value_type;
      typedef SIZE native_type;
      typedef boost::scalar_property< boost::aliased_property< value_type > >
         property_type;
   public:
      property_type dx;
      property_type dy;
   public:
      inline size & operator=( const size & sz );
      inline size & operator=( const native_type & sz );
   public:
      inline size( const size & sz );
      inline size( const native_type & sz );
      inline size( const value_type & _w, const value_type & _h );
};

size & size::operator=( const size & sz )
{
   cx = sz.cx;
   cy = sz.cy;
   return *this;
}

size & size::operator=( const native_type & sz )
{
   cx = sz.cx;
   cy = sz.cy;
   return *this;
}

size::size( const size & sz ):
   dx( cx ), dy( cy )
{
   cx = sz.cx;
   cy = sz.cy;
}

size::size( const native_type & sz ):
   dx( cx ), dy( cy )
{
   cx = sz.cx;
   cy = sz.cy;
}

size::size( const value_type & _w, const value_type & _h ):
   dx( cx ), dy( cy )
{
   cx = _w;
   cy = _h;
}

}}}

#endif
