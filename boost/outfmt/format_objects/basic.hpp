// (C) Copyright 2003: Reece H. Dunn

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#ifndef BOOST_IOFM_FormatObjects_Basic_HPP
#define BOOST_IOFM_FormatObjects_Basic_HPP
#  include <typeinfo>

   namespace boost { namespace io
   {
      class basic_object
      {
         public:
            typedef void                                             traits_type;
            typedef char *                                           format_type;
         public:
            template< typename T, class InputStream >
            inline bool                          read( InputStream & is, T & value ) const
            {
               is >> value;
               return( is.isgood());
            }
         public:
            template< typename T, class OutputStream >
            inline OutputStream &                write( OutputStream & os, const T & value ) const
            {
               return( os << value );
            }
         public:
            inline           basic_object()
            {
            }
            inline           basic_object( const basic_object & )
            {
            }
      };

      inline basic_object basicfmt()
      {
         basic_object                  ob;
         return( ob );
      }
   }}
#endif
