// (C) Copyright 2003: Reece H. Dunn 

#ifndef BOOST__IOFM__FORMAT_OBJECTS__WRAPPED_OUTPUT__HPP
#define BOOST__IOFM__FORMAT_OBJECTS__WRAPPED_OUTPUT__HPP
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
                                                 wrappedfmtout
                                                 (
                                                    const Outputter & out
                                                 )
      {
         return( wrapped_output< BOOST_DEDUCED_TYPENAME Outputter::format_type, Outputter >( out ));
      }
   }}
#endif
