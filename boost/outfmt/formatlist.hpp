// (C) Copyright 2003: Reece H. Dunn

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#ifndef BOOST_IOFM_FormatList_HPP
#define BOOST_IOFM_FormatList_HPP
#  include <boost/outfmt/formatob.hpp>

   namespace boost { namespace io
   {
      // shorthand manipulators for sequence-style objects

      // list format manipulators -- iterator range variants

      template< typename ForwardIterator >
      inline formatob_t< range_t< ForwardIterator >,
                         char *,
                         range_object< char * >
                       >                         formatlist
                                                 (
                                                    ForwardIterator first,
                                                    ForwardIterator last
                                                 )
      {
         return( formatob( range( first, last ), rangefmt()));
      }

      template< typename FormatType, typename ForwardIterator >
      inline formatob_t< range_t< ForwardIterator >,
                         FormatType,
                         range_object< FormatType >
                       >                         formatlistex
                                                 (
                                                    ForwardIterator first,
                                                    ForwardIterator last
                                                 )
      {
         return( formatob( range( first, last ), rangefmtex< FormatType >()));
      }

      template< typename ForwardIterator, class FmtObject >
      inline formatob_t< range_t< ForwardIterator >,
                         typename FmtObject::format_type,
                         range_object< typename FmtObject::format_type, FmtObject >
                       >                         formatlistout
                                                 (
                                                    ForwardIterator first,
                                                    ForwardIterator last,
                                                    const FmtObject & o
                                                 )
      {
         return( formatob( range( first, last ), rangefmt( o )));
      }

      // list format manipulators -- container variants

      template< class Container >
      inline formatob_t< Container,
                         char *,
                         container_object< char * >
                       >                         format
                                                 (
                                                    Container & c
                                                 )
      {
         return( formatob( c, containerfmt()));
      }

      template< typename FormatType, class Container >
      inline formatob_t< Container,
                         FormatType,
                         container_object< FormatType >
                       >                         formatex
                                                 (
                                                    Container & c
                                                 )
      {
         return( formatob( c, containerfmtex< FormatType >()));
      }

      template< class Container, class FmtObject >
      inline formatob_t< Container,
                         typename FmtObject::format_type,
                         container_object< typename FmtObject::format_type, FmtObject >
                       >                         formatout
                                                 (
                                                    Container & c,
                                                    const FmtObject & o
                                                 )
      {
         return( formatob( c, containerfmt( o )));
      }

      // list format manipulators -- iterator range variants #2

      template< typename ForwardIterator >
      inline formatob_t< std::pair< ForwardIterator, ForwardIterator >,
                         char *,
                         range_object< char * >
                       >                         formatrange
                                                 (
                                                    std::pair< ForwardIterator, ForwardIterator > & ip
                                                 )
      {
         return( formatob( ip, rangefmt()));
      }

      template< typename FormatType, typename ForwardIterator >
      inline formatob_t< std::pair< ForwardIterator, ForwardIterator >,
                         FormatType,
                         range_object< FormatType >
                       >                         formatrangeex
                                                 (
                                                    std::pair< ForwardIterator, ForwardIterator > & ip
                                                 )
      {
         return( formatob( ip, rangefmtex< FormatType >()));
      }

      template< typename ForwardIterator, class FmtObject >
      inline formatob_t< std::pair< ForwardIterator, ForwardIterator >,
                         typename FmtObject::format_type,
                         range_object< typename FmtObject::format_type, FmtObject >
                       >                         formatrangeout
                                                 (
                                                    std::pair< ForwardIterator, ForwardIterator > & ip,
                                                    const FmtObject & o
                                                 )
      {
         return( formatob( ip, rangefmt( o )));
      }
   }}
#endif
