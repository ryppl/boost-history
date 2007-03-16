// (C) Copyright 2003-2005: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FSSAFE_WCHAR_HPP
#define BOOST_FSSAFE_WCHAR_HPP

#include <boost/fixed_string/fixed_string.hpp>
#include <cstdarg>
#include <wchar.h>

// wchar.h functions

inline wchar_t * wcscpy( boost::wchar_string & d, const wchar_t * s )
{
   d.assign( s );
   return d.buffer();
}

inline wchar_t * wcscat( boost::wchar_string & d, const wchar_t * s )
{
   d.append( s );
   return d.buffer();
}

inline int wcscmp( boost::wchar_string & s1, boost::wchar_string & s2 )
{
   return s1.compare( s2 );
}

inline int wcscmp( boost::wchar_string & s1, const wchar_t * s2 )
{
   return s1.compare( s2 );
}

inline int wcscmp( const wchar_t * s1, boost::wchar_string & s2 )
{
   return -s2.compare( s1 );
}

inline size_t wcslen( boost::wchar_string & s )
{
   return s.length();
}

inline int vswprintf( boost::wchar_string & s, const wchar_t * fmt, va_list args )
{
   return s.format( fmt, args );
}

inline int swprintf( boost::wchar_string & s, const wchar_t * fmt ... )
{
   va_list args;
   va_start( args, fmt );
   int ret = s.format( fmt, args );
   va_end( args );
   return ret;
}

#endif
