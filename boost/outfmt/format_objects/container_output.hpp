// (C) Copyright 2003: Reece H. Dunn 

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#ifndef BOOST_IOFM_FormatObjects_ContainerOutput_HPP
#define BOOST_IOFM_FormatObjects_ContainerOutput_HPP
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
         private:
            typedef detail::list_output< FormatType, container_output< FormatType, Outputter >, Outputter >
                                                                     base_type;
         public:
            template< typename Container, class OutputStream >
            inline OutputStream & operator()
                                  (
                                     OutputStream &    os,
                                     const Container & c
                                  ) const
            {
               const base_type *       self = static_cast< const base_type * >( this );
               return(( *self )( os, c.begin(), c.end()));
            }
         public:
            template< typename Container, class InputStream >
            inline bool                          read( InputStream & is, Container & c ) const
            {
               c.clear();
               get_typeid< Container >::value; // BCB workaround
               return( read( is, c, seq_type< get_typeid< Container >::value >()));
            }
         private:
            template< typename Container, class InputStream >
            inline bool                          read
                                                 (
                                                    InputStream & is,
                                                    Container   & c,
                                                    seq_type< seq_container_type >
                                                 ) const
            {
               const base_type *       self = static_cast< const base_type * >( this );
               typename Container::value_type
                                       value;

               return(( *self ).readc( is, std::back_inserter( c ), value ));
            }
            // [todo]: read( seq_type< assoc_container_type > ) -- map-like containers
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
                                                 containerfmt
                                                 (
                                                    const Outputter & out
                                                 )
      {
         return( container_output< BOOST_DEDUCED_TYPENAME Outputter::format_type, Outputter >( out ));
      }
   }}
#endif
