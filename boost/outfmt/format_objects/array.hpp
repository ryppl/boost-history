// (C) Copyright 2003-2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_IOFM_FormatObjects_Array_HPP
#define BOOST_IOFM_FormatObjects_Array_HPP
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

#  include <boost/outfmt/format_objects/detail/list.hpp>

   namespace boost { namespace io
   {
      template< typename FormatType, class FmtObject >
      class array_object: public detail::list_object< FormatType, array_object
                                                                  <
                                                                     FormatType,
                                                                     FmtObject
                                                                  >, FmtObject >
      {
         private:
            long                       length;
            long                       offset;
         private:
            typedef detail::list_object< FormatType, array_object< FormatType, FmtObject >, FmtObject >
                                                                     base_type;
         public:
            typedef seq_type< array_type >                           formatter_type;
         public:
            template< typename T, class InputStream >
            inline bool                          read( InputStream & is, const T * array ) const
            {
               const base_type *       self = static_cast< const base_type * >( this );
               return(( *self ).read( is, array + offset, array + offset + length ));
            }
         public:
            template< typename T, class OutputStream >
            inline OutputStream &                write
                                                 (
                                                    OutputStream & os,
                                                    const T      * array
                                                 ) const
            {

               const base_type *       self = static_cast< const base_type * >( this );
               return(( *self ).write( os, array + offset, array + offset + length ));
            }
         public:
            inline array_object & operator()( long len )
            {
               offset = 0;
               length = len;
               return( *this );
            }
            inline array_object & operator()( long off, long len )
            {
               offset = off;
               length = len;
               return( *this );
            }
         public:
            inline           array_object( const array_object & ao ):
               detail::list_object< FormatType, array_object< FormatType, FmtObject >, FmtObject >( ao ),
               length( ao.length ),
               offset( ao.offset )
            {
            }
            inline           array_object( long len = 0 ):
               length( len ),
               offset( 0 )
            {
            }
            inline           array_object( long off, long len ):
               length( len ),
               offset( off )
            {
            }
            inline           array_object( const FmtObject & o, long len = 0 ):
               detail::list_object< FormatType, array_object< FormatType, FmtObject >, FmtObject >( o ),
               length( len ),
               offset( 0 )
            {
            }
            inline           array_object( const FmtObject & o, long off, long len ):
               detail::list_object< FormatType, array_object< FormatType, FmtObject >, FmtObject >( o ),
               length( len ),
               offset( off )
            {
            }
      };

      template< class FormatType >
      inline array_object< FormatType >          arrayfmtex( long off, long len )
      {
         array_object< FormatType > ob( off, len );
         return( ob );
      }
      template< class FormatType >
      inline array_object< FormatType >          arrayfmtex( long len = 0 )
      {
         return( arrayfmtex< FormatType >( 0, len ));
      }

      inline array_object< const char * >              arrayfmt( long off, long len )
      {
         return( arrayfmtex< const char * >( off, len ));
      }
      inline array_object< const char * >              arrayfmt( long len = 0 )
      {
         return( arrayfmtex< const char * >( 0, len ));
      }

      template< class FmtObject >
      inline array_object< typename FmtObject::format_type, FmtObject >
                                                 arrayfmtout
                                                 (
                                                    const FmtObject & o,
                                                    long              off,
                                                    long              len
                                                 )
      {
         return( array_object< BOOST_DEDUCED_TYPENAME FmtObject::format_type, FmtObject >( o, off, len ));
      }
      template< class FmtObject >
      inline array_object< typename FmtObject::format_type, FmtObject >
                                                 arrayfmtout
                                                 (
                                                    const FmtObject & o,
                                                    long              len = 0
                                                 )
      {
         return( array_object< BOOST_DEDUCED_TYPENAME FmtObject::format_type, FmtObject >( o, len ));
      }
   }}
#endif
