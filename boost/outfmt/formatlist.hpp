// (C) Copyright 2003: Reece H. Dunn 

#ifndef BOOST__IOFM__FORMATLIST__HPP
#define BOOST__IOFM__FORMATLIST__HPP
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
            inline ForwardIterator               begin() const throw()
            {
               return( first );
            }
            inline ForwardIterator               end() const throw()
            {
               return( last );
            }
         public:
            typedef ForwardIterator                        iterator;
         public: // constructors
            inline           formatlist_t( const formatlist_t & fl ) throw():
               detail::list_output< FormatType, formatlist_t, Outputter >( fl ),
               first( fl.first ),
               last(  fl.last )
            {
            }
            inline           formatlist_t( ForwardIterator f, ForwardIterator l ) throw():
               first( f ),
               last(  l )
            {
            }
            inline           formatlist_t( ForwardIterator f, ForwardIterator l, Outputter out ) throw():
               detail::list_output< FormatType, formatlist_t, Outputter >( out ),
               first( f ),
               last(  l )
            {
            }
      };

#     if !defined(BOOST_IOFM__NO_BASIC_STREAM)
         template< typename CharT, class TraitsT, typename ForwardIterator, typename FormatType, typename Outputter >
         inline std::basic_ostream< CharT, TraitsT > & operator<<
         (
            std::basic_ostream< CharT, TraitsT > & os,
            const formatlist_t< ForwardIterator, FormatType, Outputter > & fl
         )
         {
            return( fl( os, fl.begin(), fl.end()));
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

         // [todo]: check and add support for std::wstream
#     endif

      // list format manipulators

      // [todo]: return( formatob( range( first, last ), rangefmt()));
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

      // [todo]: return( formatob( range( c ), rangefmt()));
      template< class Container >
      inline formatlist_t< typename Container::iterator >
                                                 formatlist
                                                 (
                                                    Container & c
                                                 )
      {
         return( formatlist_t< BOOST_DEDUCED_TYPENAME Container::iterator >( c.begin(), c.end()));
      }

      // [todo]: return( formatobout( ip, rangefmt()));
      template< typename ForwardIterator >
      inline formatlist_t< ForwardIterator >
                                                 formatlist
                                                 (
                                                    std::pair< ForwardIterator, ForwardIterator > & ip
                                                 )
      {
         return( formatlist_t< ForwardIterator >( ip.first, ip.second ));
      }

      // extended format manipulator:

      // [todo]: return( formatob( range( first, last ), rangefmtex< FormatType >()));
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

      // [todo]: return( formatob( range( c ), rangefmtex< FormatType >()));
      template< typename FormatType, class Container >
      inline formatlist_t< typename Container::iterator, FormatType >
                                                 formatlistex
                                                 (
                                                    Container & c
                                                 )
      {
         return( formatlist_t< BOOST_DEDUCED_TYPENAME Container::iterator, FormatType >( c.begin(), c.end()));
      }

      // [todo]: return( formatobout( ip, rangefmtex< FormatType >()));
      template< typename FormatType, typename ForwardIterator >
      inline formatlist_t< ForwardIterator, FormatType >
                                                 formatlistex
                                                 (
                                                    std::pair< ForwardIterator, ForwardIterator > & ip
                                                 )
      {
         return( formatlist_t< ForwardIterator, FormatType >( ip.first, ip.second ));
      }

      // format manipulator -- output functor versions:

      template< typename ForwardIterator, class Outputter >
      inline formatlist_t< ForwardIterator, typename Outputter::format_type, Outputter >
                                                 formatlist
                                                 (
                                                    ForwardIterator first,
                                                    ForwardIterator last,
                                                    Outputter out
                                                 )
      {
         return( formatlist_t< ForwardIterator, BOOST_DEDUCED_TYPENAME Outputter::format_type, Outputter >
         (
            first,
            last,
            out
         ));
      }

      template< class Container, class Outputter >
      inline formatlist_t< typename Container::iterator, typename Outputter::format_type, Outputter >
                                                 formatlist
                                                 (
                                                    Container & c,
                                                    Outputter out
                                                 )
      {
         return( formatlist_t< BOOST_DEDUCED_TYPENAME Container::iterator, BOOST_DEDUCED_TYPENAME Outputter::format_type, Outputter >
         (
            c.begin(),
            c.end(),
            out
         ));
      }

      template< typename ForwardIterator, class Outputter >
      inline formatlist_t< ForwardIterator, typename Outputter::format_type, Outputter >
                                                 formatlist
                                                 (
                                                    std::pair< ForwardIterator, ForwardIterator > & ip,
                                                    Outputter out
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
