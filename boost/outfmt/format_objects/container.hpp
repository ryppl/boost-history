// (C) Copyright 2003-2004: Reece H. Dunn 

#ifndef BOOST_IOFM_FormatObjects_Container_HPP
#define BOOST_IOFM_FormatObjects_Container_HPP
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

#  include <boost/outfmt/format_objects/detail/list.hpp>

   namespace boost { namespace io
   {
      template< typename FormatType, class FmtObject >
      class container_object: public detail::list_object< FormatType, container_object
                                                                      <
                                                                         FormatType,
                                                                         FmtObject
                                                                      >, FmtObject >
      {
         private:
            typedef detail::list_object< FormatType, container_object< FormatType, FmtObject >, FmtObject >
                                                                     base_type;
         public:
            typedef seq_type< seq_container_type >                   formatter_type; // container_type?
         public:
            template< typename Container, class InputStream >
            inline bool                          read( InputStream & is, Container & c ) const
            {
               c.clear();
               get_typeid< Container >::value; // BCB workaround
               return( read( is, c, seq_type< get_typeid< Container >::value >()));
            }
         public:
            template< typename Container, class OutputStream >
            inline OutputStream &                write( OutputStream & os, const Container & c ) const
            {
               const base_type *       self = static_cast< const base_type * >( this );
               return(( *self ).write( os, c.begin(), c.end()));
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
            inline           container_object()
            {
            }
            inline           container_object( const container_object & o ):
               detail::list_object< FormatType, container_object< FormatType, FmtObject >, FmtObject >( o )
            {
            }
            inline           container_object( const FmtObject & o ):
               detail::list_object< FormatType, container_object< FormatType, FmtObject >, FmtObject >( o )
            {
            }
      };

      template< class FormatType >
      inline container_object< FormatType >      containerfmtex()
      {
         container_object< FormatType > ob;
         return( ob );
      }

      inline container_object< char * >          containerfmt()
      {
         return( containerfmtex< char * >());
      }

      template< class FmtObject >
      inline container_object< BOOST_DEDUCED_TYPENAME FmtObject::format_type, FmtObject >
                                                 containerfmt
                                                 (
                                                    const FmtObject & o
                                                 )
      {
         return( container_object< BOOST_DEDUCED_TYPENAME FmtObject::format_type, FmtObject >( o ));
      }
   }}
#endif
