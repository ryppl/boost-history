// (C) Copyright 2003: Reece H. Dunn 

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#ifndef BOOST_IOFM_FormatObjects_WrappedOutput_HPP
#define BOOST_IOFM_FormatObjects_WrappedOutput_HPP
#  include <boost/outfmt/formatter.hpp>

   namespace boost { namespace io
   {
      template< typename FormatType, class Outputter >
      class wrapped_output: public openclose_formatter_t< FormatType, wrapped_output
                                                                      <
                                                                         FormatType,
                                                                         Outputter
                                                                      > >
      {
         public:
            typedef wrapped_output< FormatType, Outputter >          this_type;
            typedef typename openclose_formatter_t< FormatType, this_type >::traits_type
                                                                     traits_type;
         private:
            Outputter                  out;
         public:
            template< typename T, class OutputStream >
            inline OutputStream & operator()( OutputStream & os, const T & value ) const
            {
               os << open();
               return( out( os, value ) << close());
            }
         public:
            template< typename T, class InputStream >
            inline bool                          read( InputStream & is, T & v ) const
            {
               return( is.match( open()) && out.read( is, v ) && is.match( close()));
            }
         public: // special treatment for string types:
            template< class InputStream >
            inline bool                          read
                                                 (
                                                    InputStream & is,
                                                    typename InputStream::string_type & s
                                                 ) const
            {
               s.clear();
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
                  else if( is.rdstate() & InputStream::iosbase_type::failbit ) // failed?
                     is.clear(); // lets continue...
                  else
                     return( false );
               }
            }
         public:
            inline           wrapped_output()
            {
            }
            inline           wrapped_output( const wrapped_output & o ):
               openclose_formatter_t< FormatType, this_type >( o ),
               out( o.out )
            {
            }
            inline           wrapped_output( const Outputter & o ):
               out( o )
            {
            }
      };

      template< class FormatType >
      inline wrapped_output< FormatType >        wrappedfmtex()
      {
         wrapped_output< FormatType >  out;
         return( out );
      }

      inline wrapped_output< char * >            wrappedfmt()
      {
         return( wrappedfmtex< char * >());
      }

      template< class Outputter >
      inline wrapped_output< typename Outputter::format_type, Outputter >
                                                 wrappedfmt
                                                 (
                                                    const Outputter & out
                                                 )
      {
         return( wrapped_output< BOOST_DEDUCED_TYPENAME Outputter::format_type, Outputter >( out ));
      }
   }}
#endif
