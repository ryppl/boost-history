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
      template< typename DelimeterType, class FormatObject >
      class arrayfmt_t: public detail::list_object< DelimeterType, arrayfmt_t
                                                                   <
                                                                      DelimeterType,
                                                                      FormatObject
                                                                   >, FormatObject >
      {
         private:
            long                       length;
            long                       offset;
         private:
            typedef detail::list_object< DelimeterType, arrayfmt_t< DelimeterType, FormatObject >, FormatObject >
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
            inline arrayfmt_t & operator()( long len )
            {
               offset = 0;
               length = len;
               return( *this );
            }
            inline arrayfmt_t & operator()( long off, long len )
            {
               offset = off;
               length = len;
               return( *this );
            }
         public:
            inline           arrayfmt_t( const arrayfmt_t & ao ):
               detail::list_object< DelimeterType, arrayfmt_t< DelimeterType, FormatObject >, FormatObject >( ao ),
               length( ao.length ),
               offset( ao.offset )
            {
            }
            inline           arrayfmt_t( long len = 0 ):
               length( len ),
               offset( 0 )
            {
            }
            inline           arrayfmt_t( long off, long len ):
               length( len ),
               offset( off )
            {
            }
            inline           arrayfmt_t( const FormatObject & o, long len = 0 ):
               detail::list_object< DelimeterType, arrayfmt_t< DelimeterType, FormatObject >, FormatObject >( o ),
               length( len ),
               offset( 0 )
            {
            }
            inline           arrayfmt_t( const FormatObject & o, long off, long len ):
               detail::list_object< DelimeterType, arrayfmt_t< DelimeterType, FormatObject >, FormatObject >( o ),
               length( len ),
               offset( off )
            {
            }
      };

      template< class DelimeterType >
      inline arrayfmt_t< DelimeterType >         arrayfmtex( long off, long len )
      {
         arrayfmt_t< DelimeterType > ob( off, len );
         return( ob );
      }
      template< class DelimeterType >
      inline arrayfmt_t< DelimeterType >         arrayfmtex( long len )
      {
         return( arrayfmtex< DelimeterType >( 0, len ));
      }

      inline arrayfmt_t< const char * >          arrayfmt( long off, long len )
      {
         return( arrayfmtex< const char * >( off, len ));
      }
      inline arrayfmt_t< const char * >          arrayfmt( long len )
      {
         return( arrayfmtex< const char * >( 0, len ));
      }

      template< class FormatObject >
      inline arrayfmt_t< typename FormatObject::format_type, FormatObject >
                                                 arrayfmtout
                                                 (
                                                    const FormatObject & o,
                                                    long              off,
                                                    long              len
                                                 )
      {
         return( arrayfmt_t< BOOST_DEDUCED_TYPENAME FormatObject::format_type, FormatObject >( o, off, len ));
      }
      template< class FormatObject >
      inline arrayfmt_t< typename FormatObject::format_type, FormatObject >
                                                 arrayfmtout
                                                 (
                                                    const FormatObject & o,
                                                    long              len
                                                 )
      {
         return( arrayfmt_t< BOOST_DEDUCED_TYPENAME FormatObject::format_type, FormatObject >( o, len ));
      }
   }}
#endif
