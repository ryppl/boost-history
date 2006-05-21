// (C) Copyright 2003-2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

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
      template< typename DelimeterType, class FormatObject >
      class rangefmt_t: public detail::list_object< DelimeterType, rangefmt_t
                                                                   <
                                                                      DelimeterType,
                                                                      FormatObject
                                                                   >, FormatObject >
      {
         private:
            typedef detail::list_object< DelimeterType, rangefmt_t< DelimeterType, FormatObject >, FormatObject >
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
            inline           rangefmt_t()
            {
            }
            inline           rangefmt_t( const rangefmt_t & o ):
               detail::list_object< DelimeterType, rangefmt_t< DelimeterType, FormatObject >, FormatObject >( o )
            {
            }
            inline           rangefmt_t( const FormatObject & o ):
               detail::list_object< DelimeterType, rangefmt_t< DelimeterType, FormatObject >, FormatObject >( o )
            {
            }
      };

      // range format generators

      template< class DelimeterType >
      inline rangefmt_t< DelimeterType >         rangefmtex()
      {
         rangefmt_t< DelimeterType >    ob;
         return( ob );
      }

      inline rangefmt_t< const char * >          rangefmt()
      {
         return( rangefmtex< const char * >());
      }

      template< class FormatObject >
      inline rangefmt_t< BOOST_DEDUCED_TYPENAME FormatObject::format_type, FormatObject >
                                                 rangefmt
                                                 (
                                                    const FormatObject & o
                                                 )
      {
         return( rangefmt_t< BOOST_DEDUCED_TYPENAME FormatObject::format_type, FormatObject >( o ));
      }
   }}
#endif
