// (C) Copyright 2003-2005: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FORMATTER_HPP
#define BOOST_FORMATTER_HPP

#include <boost/fixed_string/fixed_string.hpp>
#include <cstdarg>

namespace boost
{

template
<
   size_t n, typename CharT = char,
   class CharStringTraits = std::char_traits< CharT >,
   class FmtPolicy = detail::format_policy< CharT >
>
struct formatterex: public fixed_string< n, CharT, CharStringTraits, FmtPolicy >
{
   formatterex & operator()( const char * fs ... )
   {
      va_list args;
      va_start( args, fs );
      format( fs, args );
      va_end( args );

      return *this;
   }

   formatterex( const char * fs ... )
   {
      va_list args;
      va_start( args, fs );
      format( fs, args );
      va_end( args );
   }

   formatterex() throw()
   {
   }
};

typedef formatterex< 512 > formatter;

}

#endif
