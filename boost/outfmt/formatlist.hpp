// (C) Copyright 2003: Reece H. Dunn 

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#ifndef BOOST_IOFM_FormatList_HPP
#define BOOST_IOFM_FormatList_HPP
#  include <boost/outfmt/formatob.hpp>

   namespace boost { namespace io
   {
      template
      <
         typename ForwardIterator, typename FormatType = char *,
         class    FmtObject = boost::io::basic_object
      >
      class formatlist_t: public detail::list_object< FormatType, formatlist_t
                                                                  <
                                                                     ForwardIterator,
                                                                     FormatType,
                                                                     FmtObject
                                                                  >, FmtObject >
      {
         private:
            ForwardIterator            first;
            ForwardIterator            last;
         public:
            inline ForwardIterator               begin() const 
            {
               return( first );
            }
            inline ForwardIterator               end() const 
            {
               return( last );
            }
         public:
            typedef ForwardIterator                        iterator;
         public: // constructors
            inline           formatlist_t( const formatlist_t & fl ):
               detail::list_object< FormatType, formatlist_t, FmtObject >( fl ),
               first( fl.first ),
               last(  fl.last )
            {
            }
            inline           formatlist_t( ForwardIterator f, ForwardIterator l ):
               first( f ),
               last(  l )
            {
            }
            inline           formatlist_t( ForwardIterator f, ForwardIterator l, FmtObject o ):
               detail::list_object< FormatType, formatlist_t, FmtObject >( o ),
               first( f ),
               last(  l )
            {
            }
      };

#     if !defined(BOOST_IOFM_NO_BASIC_STREAM)
         template< typename CharT, class TraitsT, typename ForwardIterator, typename FormatType, typename FmtObject >
         inline std::basic_ostream< CharT, TraitsT > & operator<<
         (
            std::basic_ostream< CharT, TraitsT > & os,
            const formatlist_t< ForwardIterator, FormatType, FmtObject > & fl
         )
         {
            return( fl.write( os, fl.begin(), fl.end()));
         }

         template< typename CharT, class TraitsT, typename ForwardIterator, typename FormatType, typename FmtObject >
         inline std::basic_istream< CharT, TraitsT > & operator>>
         (
            std::basic_istream< CharT, TraitsT > & is,
            const formatlist_t< ForwardIterator, FormatType, FmtObject > & fl
         )
         {
            boost::io::detail::input_helper< std::istream >
                                                 in( is );
            fl.read( in,
               const_cast< formatlist_t< ForwardIterator, FormatType, FmtObject > & >( fl ).begin(),
               const_cast< formatlist_t< ForwardIterator, FormatType, FmtObject > & >( fl ).end()
            );
            return( is );
         }
#     else
         template< typename ForwardIterator, typename FormatType, typename FmtObject >
         inline std::ostream & operator<<
         (
            std::ostream & os,
            const formatlist_t< ForwardIterator, FormatType, FmtObject > & fl
         )
         {
            return( fl.write( os, fl.begin(), fl.end()));
         }

         template< typename ForwardIterator, typename FormatType, typename FmtObject >
         inline std::istream & operator>>
         (
            std::istream & is,
            const formatlist_t< ForwardIterator, FormatType, FmtObject > & fl
         )
         {
            boost::io::detail::input_helper< std::istream >
                                                 in( is );
            fl.read( in,
               const_cast< formatlist_t< ForwardIterator, FormatType, FmtObject > & >( fl ).begin(),
               const_cast< formatlist_t< ForwardIterator, FormatType, FmtObject > & >( fl ).end()
            );
            return( is );
         }
#     endif

      // list format manipulators -- iterator range variants

      template< typename ForwardIterator >
      inline formatlist_t< ForwardIterator >
                                                 formatlist
                                                 (
                                                    ForwardIterator first,
                                                    ForwardIterator last
                                                 )
      {
         return( formatlist_t< ForwardIterator >( first, last ));
      }

      template< typename FormatType, typename ForwardIterator >
      inline formatlist_t< ForwardIterator, FormatType >
                                                 formatlistex
                                                 (
                                                    ForwardIterator first,
                                                    ForwardIterator last
                                                 )
      {
         return( formatlist_t< ForwardIterator, FormatType >( first, last ));
      }

      template< typename ForwardIterator, class FmtObject >
      inline formatlist_t< ForwardIterator, typename FmtObject::format_type, FmtObject >
                                                 formatlistout
                                                 (
                                                    ForwardIterator first,
                                                    ForwardIterator last,
                                                    const FmtObject & o
                                                 )
      {
         return( formatlist_t< ForwardIterator, BOOST_DEDUCED_TYPENAME FmtObject::format_type, FmtObject >
         (
            first,
            last,
            o
         ));
      }

      // list format manipulators -- container variants

      template< class Container >
      inline formatlist_t< typename Container::iterator > // unknown error in VC7
                                                 format
                                                 (
                                                    Container & c
                                                 )
      {
         return( formatlist_t< BOOST_DEDUCED_TYPENAME Container::iterator >( c.begin(), c.end()));
      }

      template< typename FormatType, class Container >
      inline formatlist_t< typename Container::iterator, FormatType >
                                                 formatex
                                                 (
                                                    Container & c
                                                 )
      {
         return( formatlist_t< BOOST_DEDUCED_TYPENAME Container::iterator, FormatType >( c.begin(), c.end()));
      }

      template< class Container, class FmtObject >
      inline formatlist_t< typename Container::iterator, typename FmtObject::format_type, FmtObject >
                                                 formatout
                                                 (
                                                    Container & c,
                                                    const FmtObject & o
                                                 )
      {
         return( formatlist_t< BOOST_DEDUCED_TYPENAME Container::iterator, BOOST_DEDUCED_TYPENAME FmtObject::format_type, FmtObject >
         (
            c.begin(),
            c.end(),
            o
         ));
      }

      // list format manipulators -- iterator range variants #2

      template< typename ForwardIterator >
      inline formatlist_t< ForwardIterator >
                                                 formatrange
                                                 (
                                                    std::pair< ForwardIterator, ForwardIterator > & ip
                                                 )
      {
         return( formatlist_t< ForwardIterator >( ip.first, ip.second ));
      }

      template< typename FormatType, typename ForwardIterator >
      inline formatlist_t< ForwardIterator, FormatType >
                                                 formatrangeex
                                                 (
                                                    std::pair< ForwardIterator, ForwardIterator > & ip
                                                 )
      {
         return( formatlist_t< ForwardIterator, FormatType >( ip.first, ip.second ));
      }

      template< typename ForwardIterator, class FmtObject >
      inline formatlist_t< ForwardIterator, typename FmtObject::format_type, FmtObject >
                                                 formatrangeout
                                                 (
                                                    std::pair< ForwardIterator, ForwardIterator > & ip,
                                                    const FmtObject & o
                                                 )
      {
         return( formatlist_t< ForwardIterator, BOOST_DEDUCED_TYPENAME FmtObject::format_type, FmtObject >
         (
            ip.first,
            ip.second,
            o
         ));
      }
   }}
#endif
