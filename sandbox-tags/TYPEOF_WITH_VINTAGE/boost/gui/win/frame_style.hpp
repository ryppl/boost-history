// (C) Copyright 2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GUI_WIN_FRAME_STYLE_HPP
#define BOOST_GUI_WIN_FRAME_STYLE_HPP

#include <boost/gui/win/windows.hpp>

namespace boost { namespace gui { namespace win
{

class frame_style
{
   public:
      unsigned int style;
      unsigned int styleEx;
   public:
      inline frame_style & operator|=( unsigned int s );
      inline frame_style & operator|=( const frame_style & fs );
      inline frame_style & operator&=( unsigned int s );
      inline frame_style & operator&=( const frame_style & fs );
   public:
      inline frame_style( unsigned int s = 0, unsigned int se = 0 );
};

// global operators

inline frame_style operator|( const frame_style & fs,  unsigned int s );
inline frame_style operator|( const frame_style & fs1, const frame_style & fs2 );
inline frame_style operator&( const frame_style & fs,  unsigned int s );
inline frame_style operator&( const frame_style & fs1, const frame_style & fs2 );

// implementation

frame_style & frame_style::operator|=( unsigned int s )
{
   style |= s;
   return *this;
}

frame_style & frame_style::operator|=( const frame_style & fs )
{
   style   |= fs.style;
   styleEx |= fs.styleEx;
   return *this;
}

frame_style & frame_style::operator&=( unsigned int s )
{
   style &= s;
   return *this;
}

frame_style & frame_style::operator&=( const frame_style & fs )
{
   style   &= fs.style;
   styleEx &= fs.styleEx;
   return *this;
}

frame_style::frame_style( unsigned int s, unsigned int se ):
   style( s ), styleEx( se )
{
}

// global operators

frame_style operator|( const frame_style & fs,  unsigned int s )
{
   return frame_style( fs.style | s, fs.styleEx );
}

frame_style operator|( const frame_style & fs1, const frame_style & fs2 )
{
   return frame_style( fs1.style | fs2.style, fs1.styleEx | fs2.styleEx );
}

frame_style operator&( const frame_style & fs,  unsigned int s )
{
   return frame_style( fs.style & s, fs.styleEx );
}

frame_style operator&( const frame_style & fs1, const frame_style & fs2 )
{
   return frame_style( fs1.style & fs2.style, fs1.styleEx & fs2.styleEx );
}

}}}

#endif
