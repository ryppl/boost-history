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
         class    Outputter = boost::io::basic_output
      >
      class formatlist_t: public detail::list_output< FormatType, formatlist_t
                                                                  <
                                                                     ForwardIterator,
                                                                     FormatType,
                                                                     Outputter
                                                                  >, Outputter >
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
               detail::list_output< FormatType, formatlist_t, Outputter >( fl ),
               first( fl.first ),
               last(  fl.last )
            {
            }
            inline           formatlist_t( ForwardIterator f, ForwardIterator l ):
               first( f ),
               last(  l )
            {
            }
            inline           formatlist_t( ForwardIterator f, ForwardIterator l, Outputter out ):
               detail::list_output< FormatType, formatlist_t, Outputter >( out ),
               first( f ),
               last(  l )
            {
            }
      };

#     if !defined(BOOST_IOFM_NO_BASIC_STREAM)
         template< typename CharT, class TraitsT, typename ForwardIterator, typename FormatType, typename Outputter >
         inline std::basic_ostream< CharT, TraitsT > & operator<<
         (
            std::basic_ostream< CharT, TraitsT > & os,
            const formatlist_t< ForwardIterator, FormatType, Outputter > & fl
         )
         {
            return( fl( os, fl.begin(), fl.end()));
         }

         template< typename CharT, class TraitsT, typename ForwardIterator, typename FormatType, typename Outputter >
         inline std::basic_istream< CharT, TraitsT > & operator>>
         (
            std::basic_istream< CharT, TraitsT > & is,
            const formatlist_t< ForwardIterator, FormatType, Outputter > & fl
         )
         {
            boost::io::detail::input_helper< std::istream >
                                                 in( is );
            fl.read( in,
               const_cast< formatlist_t< ForwardIterator, FormatType, Outputter > & >( fl ).begin(),
               const_cast< formatlist_t< ForwardIterator, FormatType, Outputter > & >( fl ).end()
            );
            return( is );
         }
#     else
         template< typename ForwardIterator, typename FormatType, typename Outputter >
         inline std::ostream & operator<<
         (
            std::ostream & os,
            const formatlist_t< ForwardIterator, FormatType, Outputter > & fl
         )
         {
            return( fl( os, fl.begin(), fl.end()));
         }

         template< typename ForwardIterator, typename FormatType, typename Outputter >
         inline std::istream & operator>>
         (
            std::istream & is,
            const formatlist_t< ForwardIterator, FormatType, Outputter > & fl
         )
         {
            boost::io::detail::input_helper< std::istream >
                                                 in( is );
            fl.read( in,
               const_cast< formatlist_t< ForwardIterator, FormatType, Outputter > & >( fl ).begin(),
               const_cast< formatlist_t< ForwardIterator, FormatType, Outputter > & >( fl ).end()
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

      template< typename ForwardIterator, class Outputter >
      inline formatlist_t< ForwardIterator, typename Outputter::format_type, Outputter >
                                                 formatlistout
                                                 (
                                                    ForwardIterator first,
                                                    ForwardIterator last,
                                                    const Outputter & out
                                                 )
      {
         return( formatlist_t< ForwardIterator, BOOST_DEDUCED_TYPENAME Outputter::format_type, Outputter >
         (
            first,
            last,
            out
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

      template< class Container, class Outputter >
      inline formatlist_t< typename Container::iterator, typename Outputter::format_type, Outputter >
                                                 formatout
                                                 (
                                                    Container & c,
                                                    const Outputter & out 
                                                 )
      {
         return( formatlist_t< BOOST_DEDUCED_TYPENAME Container::iterator, BOOST_DEDUCED_TYPENAME Outputter::format_type, Outputter >
         (
            c.begin(),
            c.end(),
            out
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

      template< typename ForwardIterator, class Outputter >
      inline formatlist_t< ForwardIterator, typename Outputter::format_type, Outputter >
                                                 formatrangeout
                                                 (
                                                    std::pair< ForwardIterator, ForwardIterator > & ip,
                                                    const Outputter & out
                                                 )
      {
         return( formatlist_t< ForwardIterator, BOOST_DEDUCED_TYPENAME Outputter::format_type, Outputter >
         (
            ip.first,
            ip.second,
            out
         ));
      }
   }}
#endif
