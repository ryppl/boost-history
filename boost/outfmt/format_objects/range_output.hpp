// (C) Copyright 2003: Reece H. Dunn 

#ifndef BOOST__IOFM__FORMAT_OBJECTS__RANGE_OUTPUT__HPP
#define BOOST__IOFM__FORMAT_OBJECTS__RANGE_OUTPUT__HPP
#  include <boost/outfmt/formatter.hpp>

#  include <utility>                             // std::pair

   namespace boost { namespace io
   {
      template< typename FormatType, class Outputter >
      class range_output: public detail::list_output< FormatType, range_output
                                                                  <
                                                                     FormatType,
                                                                     Outputter
                                                                  >, Outputter >
      {
         public:
            template< typename ForwardIterator, class OutputStream >
            inline OutputStream & operator()
                                  (
                                     OutputStream & os,
                                     const std::pair< ForwardIterator, ForwardIterator > & ip
                                  ) const 
            {
               typedef detail::list_output< FormatType, range_output< FormatType, Outputter >, Outputter >
                                                                     base_type;

               const base_type *       self = static_cast< const base_type * >( this );
               return(( *self )( os, ip.first, ip.second ));
            }
         public:
            inline           range_output()
            {
            }
            inline           range_output( const range_output & o ):
               detail::list_output< FormatType, range_output< FormatType, Outputter >, Outputter >( o )
            {
            }
            inline           range_output( const Outputter & o ):
               detail::list_output< FormatType, range_output< FormatType, Outputter >, Outputter >( o )
            {
            }
      };

      template< class FormatType >
      inline range_output< FormatType >          rangefmtex()
      {
         range_output< FormatType >    out;
         return( out );
      }

      inline range_output< char * >              rangefmt()
      {
         return( rangefmtex< char * >());
      }

      template< class Outputter >
      inline range_output< BOOST_DEDUCED_TYPENAME Outputter::format_type, Outputter >
                                                 rangefmtout
                                                 (
                                                    const Outputter & out
                                                 )
      {
         return( range_output< BOOST_DEDUCED_TYPENAME Outputter::format_type, Outputter >( out ));
      }
   }}
#endif
