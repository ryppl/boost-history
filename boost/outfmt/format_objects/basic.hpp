// (C) Copyright 2003-2004: Reece H. Dunn

#ifndef BOOST_IOFM_FormatObjects_Basic_HPP
#define BOOST_IOFM_FormatObjects_Basic_HPP
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

   // [todo]: fix std::string output on the Borland compiler

   namespace boost { namespace io
   {
      class basic_object
      {
         public:
            typedef void                                             traits_type;
            typedef char *                                           format_type;
            typedef seq_type< basic_type >                           formatter_type;
         public:
#        if !defined(__BORLANDC__)
            template< typename T, class InputStream >
            inline bool                          read( InputStream & is, T & value ) const
            {
               return( readex( is, value, seq_type< get_typeid< T >::value >()));
            }
         private:
            template< typename T, class InputStream >
            inline bool                          readex
                                                 (
                                                    InputStream & is,
                                                    T & value,
                                                    ...
                                                 ) const
            {
               is >> value;
               return( is.isgood());
            }
            template< typename T, class InputStream >
            inline bool                          readex // special treatment for string types
                                                 (
                                                    InputStream & is,
                                                    T & s,
                                                    seq_type< std_string_type >
                                                 ) const
            {
               s.erase();
               is.skipws();
               if( !is.match( '"' )) return( false );

               typename InputStream::char_type
                                       ch  = '\0';
               typename InputStream::char_type
                                       sch = '"';

               while( is.getch( ch ) && !is.eq( ch, sch ))
                  s.push_back( ch );

               return( true );
            }
         public:
            template< typename T, class OutputStream >
            inline OutputStream &                write( OutputStream & os, const T & value ) const
            {
               return( writeex( os, value, seq_type< get_typeid< T >::value >()));
            }
         private:
            template< typename T, class OutputStream >
            inline OutputStream &                writeex( OutputStream & os, const T & value, ... ) const
            {
               return( os << value );
            }
            template< typename T, class OutputStream >
            inline OutputStream &                writeex( OutputStream & os, const T & value, seq_type< std_string_type > ) const
            {
               return( os << '"' << value << '"' );
            }
#        else
            template< typename T, class InputStream >
            inline bool                          read( InputStream & is, T & value ) const
            {
               is >> value;
               return( is.isgood());
            }
            template< typename T, class OutputStream >
            inline OutputStream &                write( OutputStream & os, const T & value ) const
            {
               return( os << value );
            }
#        endif
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
