// (C) Copyright 2003: Reece H. Dunn 

#ifndef BOOST__IOFM__FORMAT_OBJECTS__CONTAINER_OUTPUT__HPP
#define BOOST__IOFM__FORMAT_OBJECTS__CONTAINER_OUTPUT__HPP
#  include <boost/outfmt/format_objects/detail/list_output.hpp>

   namespace boost { namespace io
   {
      template< typename FormatType, class Outputter >
      class container_output: public detail::list_output< FormatType, container_output
                                                                      <
                                                                         FormatType,
                                                                         Outputter
                                                                      >, Outputter >
      {
         public:
            template< typename Container, class OutputStream >
            inline OutputStream & operator()
                                  (
                                     OutputStream &    os,
                                     const Container & c
                                  ) const 
            {
               typedef detail::list_output< FormatType, container_output< FormatType, Outputter >, Outputter >
                                                                     base_type;

               const base_type *       self = static_cast< const base_type * >( this );
               return(( *self )( os, c.begin(), c.end()));
            }
         public:
            inline           container_output()
            {
            }
            inline           container_output( const container_output & o ):
               detail::list_output< FormatType, container_output< FormatType, Outputter >, Outputter >( o )
            {
            }
            inline           container_output( const Outputter & o ):
               detail::list_output< FormatType, container_output< FormatType, Outputter >, Outputter >( o )
            {
            }
      };

      template< class FormatType >
      inline container_output< FormatType >      containerfmtex()
      {
         container_output< FormatType > out;
         return( out );
      }

      inline container_output< char * >          containerfmt()
      {
         return( containerfmtex< char * >());
      }

      template< class Outputter >
      inline container_output< BOOST_DEDUCED_TYPENAME Outputter::format_type, Outputter >
                                                 containerfmtout
                                                 (
                                                    const Outputter & out
                                                 )
      {
         return( container_output< BOOST_DEDUCED_TYPENAME Outputter::format_type, Outputter >( out ));
      }
   }}
#endif
