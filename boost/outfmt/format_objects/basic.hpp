// (C) Copyright 2003-2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_IOFM_FormatObjects_Basic_HPP
#define BOOST_IOFM_FormatObjects_Basic_HPP
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

#include <boost/outfmt/detail/select.hpp>

   namespace boost { namespace io
   {
      namespace detail
      {
         class simple_object
         {
            public:
               template< typename T, class InputStream >
               static inline bool                read( InputStream & is, T & value )
               {
                  is >> value;
                  return( is.isgood());
               }
               template< typename T, class OutputStream >
               static inline OutputStream &      write( OutputStream & os, const T & value )
               {
                  return( os << value );
               }
         };

         // special handling for string types

         class string_object
         {
            public:
            template< typename T, class InputStream >
            static inline bool                   read( InputStream & is, T & s )
            {
               s.erase();
               is.skipws();
               if( !is.match( '"' )) return( false );

               typename InputStream::char_type
                                       ch  = '\0';
               typename InputStream::char_type
                                       sch = '"';

               while( is.getch( ch ) && !is.eq( ch, sch ))
               {
                  if( is.eq( ch, '\\' ) && is.getch( ch ))
                  {
                     // only escape \ and " characters
                     if( !is.eq( ch, '\\' ) && !is.eq( ch, '"' ))
                        s.push_back( '\\' );

                     s.push_back( ch );
                  }
                  else                 s.push_back( ch );
               }

               return( true );
            }
            template< typename T, class OutputStream >
            static inline OutputStream &         write( OutputStream & os, const T & value )
            {
               os << '"';
               typename T::const_iterator       last = value.end();
               for(typename T::const_iterator i = value.begin(); i != last; ++i )
               {
                  switch( *i ) // only escape \ and " characters
                  {
                     case '"':    os << "\\\""; break;
                     case '\\':   os << "\\\\"; break;
                     default:     os << *i;     break;
                  }
               }
               return( os << '"' );
            }
         };
      }

      class basicfmt_t
      {
         public:
            typedef void                                             traits_type;
            typedef const char *                                     format_type;
            typedef seq_type< basic_type >                           formatter_type;
         public:
            template< typename T, class InputStream >
            inline bool                          read( InputStream & is, T & value ) const
            {
               typedef BOOST_DEDUCED_TYPENAME select
                       <
                           is_std_string< T >, detail::string_object,
                           mpl::true_,         detail::simple_object
                       >::type reader;
               return ( reader::read( is, value ) );
            }
            template< typename T, class OutputStream >
            inline OutputStream &                write( OutputStream & os, const T & value ) const
            {
                return
                ( 
                   BOOST_DEDUCED_TYPENAME select // BCB fix
                   <
                      is_std_string< T >, detail::string_object,
                      mpl::true_,         detail::simple_object
                   >::type::write( os, value )
                );
            }
         public:
            inline           basicfmt_t()
            {
            }
            inline           basicfmt_t( const basicfmt_t & )
            {
            }
      };

      inline basicfmt_t                          basicfmt()
      {
         basicfmt_t                  ob;
         return( ob );
      }
   }}
#endif
