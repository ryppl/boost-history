// (C) Copyright 2003-2004: Reece H. Dunn

#ifndef BOOST_IOFM_FormatObjects_Range_HPP
#define BOOST_IOFM_FormatObjects_Range_HPP
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

#  include <cstddef>
#  include <utility> // std::pair

#  include <boost/outfmt/format_objects/detail/list.hpp>
#  include <boost/outfmt/detail/range.hpp>

   namespace boost { namespace io
   {
      template< typename FormatType, class FmtObject >
      class range_object: public detail::list_object< FormatType, range_object
                                                                  <
                                                                     FormatType,
                                                                     FmtObject
                                                                  >, FmtObject >
      {
         private:
            typedef detail::list_object< FormatType, range_object< FormatType, FmtObject >, FmtObject >
                                                                     base_type;
         public:
            typedef seq_type< range_type >                           formatter_type;
         public:
            template< typename ForwardIterator, class InputStream >
            inline bool                          read
                                                 (
                                                    InputStream & is,
                                                    std::pair< ForwardIterator, ForwardIterator > & ip
                                                 ) const
            {
               const base_type *       self = static_cast< const base_type * >( this );
               return(( *self ).read( is, ip.first, ip.second ));
            }
         public:
            template< typename ForwardIterator, class OutputStream >
            inline OutputStream &                write
                                                 (
                                                    OutputStream & os,
                                                    const std::pair< ForwardIterator, ForwardIterator > & ip
                                                 ) const
            {
               const base_type *       self = static_cast< const base_type * >( this );
               return(( *self ).write( os, ip.first, ip.second ));
            }
         public:
            inline           range_object()
            {
            }
            inline           range_object( const range_object & o ):
               detail::list_object< FormatType, range_object< FormatType, FmtObject >, FmtObject >( o )
            {
            }
            inline           range_object( const FmtObject & o ):
               detail::list_object< FormatType, range_object< FormatType, FmtObject >, FmtObject >( o )
            {
            }
      };

      // range format generators

      template< class FormatType >
      inline range_object< FormatType >          rangefmtex()
      {
         range_object< FormatType >    ob;
         return( ob );
      }

      inline range_object< const char * >              rangefmt()
      {
         return( rangefmtex< const char * >());
      }

      template< class FmtObject >
      inline range_object< BOOST_DEDUCED_TYPENAME FmtObject::format_type, FmtObject >
                                                 rangefmt
                                                 (
                                                    const FmtObject & o
                                                 )
      {
         return( range_object< BOOST_DEDUCED_TYPENAME FmtObject::format_type, FmtObject >( o ));
      }
   }}
#endif
