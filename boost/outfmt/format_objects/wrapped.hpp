// (C) Copyright 2003-2004: Reece H. Dunn

#ifndef BOOST_IOFM_FormatObjects_Wrapped_HPP
#define BOOST_IOFM_FormatObjects_Wrapped_HPP
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

#  include <boost/outfmt/formatter.hpp>

   namespace boost { namespace io
   {
      template< typename FormatType, class FmtObject >
      class wrapped_object: public openclose_formatter_t< FormatType, wrapped_object
                                                                      <
                                                                         FormatType,
                                                                         FmtObject
                                                                      > >
      {
         public:
            typedef wrapped_object< FormatType, FmtObject >          this_type;
            typedef typename openclose_formatter_t< FormatType, this_type >::traits_type
                                                                     traits_type;
            typedef seq_type< wrapped_type >                         formatter_type;
         private:
            FmtObject                  fo;
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
            inline           wrapped_object()
            {
            }
            inline           wrapped_object( const wrapped_object & o ):
               openclose_formatter_t< FormatType, this_type >( o ),
               fo( o.fo )
            {
            }
            inline           wrapped_object( const FmtObject & o ):
               fo( o )
            {
            }
      };

      template< class FormatType >
      inline wrapped_object< FormatType >        wrappedfmtex()
      {
         wrapped_object< FormatType >  ob;
         return( ob );
      }

      inline wrapped_object< const char * >            wrappedfmt()
      {
         return( wrappedfmtex< const char * >());
      }

      template< class FmtObject >
      inline wrapped_object< typename FmtObject::format_type, FmtObject >
                                                 wrappedfmt
                                                 (
                                                    const FmtObject & fo
                                                 )
      {
         return( wrapped_object< BOOST_DEDUCED_TYPENAME FmtObject::format_type, FmtObject >( fo ));
      }
   }}
#endif
