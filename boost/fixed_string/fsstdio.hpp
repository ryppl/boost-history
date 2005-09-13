// (C) Copyright 2003-2005: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FSSAFE_STDIO_HPP
#define BOOST_FSSAFE_STDIO_HPP

#include <boost/fixed_string/fixed_string.hpp>
#include <cstdarg>
#include <stdio.h>

// stdio.h functions

inline int vsprintf( boost::char_string & s, const char * fmt, va_list args )
{
   return s.format( fmt, args );
}

inline int sprintf( boost::char_string & s, const char * fmt ... )
{
   va_list args;
   va_start( args, fmt );
   int ret = s.format( fmt, args );
   va_end( args );

   return ret;
}

#endif
