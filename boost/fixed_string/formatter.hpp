// (C) Copyright 2003-2004: Reece H. Dunn

#ifndef BOOST_FORMATTER_HPP
#define BOOST_FORMATTER_HPP
#  include <boost/char_string.hpp>
#  include <cstdarg>

   namespace boost
   {
      template< size_t n, bool nt = true, typename CharT = char, class StringTraits = string_traits< CharT > >
      class formatterex: public char_string< n, nt, CharT, StringTraits >
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
