// (C) Copyright 2003-2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_IOFM_FormatObjects_Wrapped_HPP
#define BOOST_IOFM_FormatObjects_Wrapped_HPP
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

#  include <boost/outfmt/formatter.hpp>

   namespace boost { namespace io
   {
      template< typename DelimeterType, class FormatObject >
      class wrappedfmt_t: public openclose_formatter_t< DelimeterType, wrappedfmt_t
                                                                       <
                                                                          DelimeterType,
                                                                          FormatObject
                                                                       > >
      {
         public:
            typedef wrappedfmt_t< DelimeterType, FormatObject >      this_type;
            typedef typename openclose_formatter_t< DelimeterType, this_type >::traits_type
                                                                     traits_type;
            typedef seq_type< wrapped_type >                         formatter_type;
         private:
            FormatObject               fo;
         public:
            template< typename T, class InputStream >
            inline bool                          read( InputStream & is, T & v ) const
            {
               get_typeid< T >::value; // BCB workaround
               return( readex( is, v, seq_type< get_typeid< T >::value >()));
            }
         private:
            template< typename T, class InputStream >
            inline bool                          readex
                                                 (
                                                    InputStream & is,
                                                    T & v,
                                                    ...
                                                 ) const
            {
               return( is.match( open()) && fo.read( is, v ) && is.match( close()));
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
               if( !is.match( open())) return( false );

               typename InputStream::char_type
                                       ch  = '\0';
               typename InputStream::char_type
                                       sch = is.firstch( close());

               is.skipws();
               
               for( ;; )
               {
                  while( is.getch( ch ) && !is.eq( ch, sch ))
                     s.push_back( ch );

                  is.putback( sch );

                  if( is.match( close(), true ))
                     return( true );
                  else if( is.failed())          is.clear(); // lets continue...
                  else                           return( false );
               }
            }
         public:
            template< typename T, class OutputStream >
            inline OutputStream &                write( OutputStream & os, const T & value ) const
            {
               os << open();
               return( fo.write( os, value ) << close());
            }
         public:
            inline           wrappedfmt_t()
            {
            }
            inline           wrappedfmt_t( const wrappedfmt_t & o ):
               openclose_formatter_t< DelimeterType, this_type >( o ),
               fo( o.fo )
            {
            }
            inline           wrappedfmt_t( const FormatObject & o ):
               fo( o )
            {
            }
      };

      template< class DelimeterType >
      inline wrappedfmt_t< DelimeterType >       wrappedfmtex()
      {
         wrappedfmt_t< DelimeterType >  ob;
         return( ob );
      }

      inline wrappedfmt_t< const char * >        wrappedfmt()
      {
         return( wrappedfmtex< const char * >());
      }

      template< class FormatObject >
      inline wrappedfmt_t< typename FormatObject::format_type, FormatObject >
                                                 wrappedfmt
                                                 (
                                                    const FormatObject & fo
                                                 )
      {
         return( wrappedfmt_t< BOOST_DEDUCED_TYPENAME FormatObject::format_type, FormatObject >( fo ));
      }
   }}
#endif
