// (C) Copyright 2003-2004: Reece H. Dunn
// Use, modification, and distribution are subject to the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FORMATTER_HPP
#define BOOST_FORMATTER_HPP
#  include <boost/fixed_string/fixed_string.hpp>
#  include <cstdarg>

   namespace boost
   {
      template< size_t n, typename CharT = char, class StringTraits = string_traits< CharT > >
      class formatterex: public fixed_string< n, CharT, StringTraits >
      {
         public:
            inline const char *   operator()( const char * fs ... ) throw()
            {
               va_list                 args;
               va_start( args, fs );
               format( fs, args );
               va_end( args );
               return( *this );
            }
         public:
            inline           formatterex( const char * fs ... ) throw()
            {
               va_list                 args;
               va_start( args, fs );
               format( fs, args );
               va_end( args );
            }
            inline           formatterex() throw()
            {
            }
      };

      typedef formatterex< 512 >                                     formatter;
   }
#endif
