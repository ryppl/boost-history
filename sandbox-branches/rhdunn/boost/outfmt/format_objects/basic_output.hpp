// (C) Copyright 2003: Reece H. Dunn

#ifndef BOOST__IOFM__FORMAT_OBJECTS__BASIC_OUTPUT__HPP
#define BOOST__IOFM__FORMAT_OBJECTS__BASIC_OUTPUT__HPP
   namespace boost { namespace io
   {
      class basic_output
      {
         public:
            typedef void                                             traits_type;
            typedef char *                                           format_type;
         public:
            template< typename T, class OutputStream >
            inline OutputStream & operator()( OutputStream & os, const T & value ) const
            {
               return( os << value );
            }
         public:
            inline           basic_output()
            {
            }
            inline           basic_output( const basic_output & )
            {
            }
      };

      inline basic_output basicfmt()
      {
         basic_output                  out;
         return( out );
      }
   }}
#endif
