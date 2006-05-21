// (C) Copyright 2003-2004: Reece H. Dunn 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_IOFM_FormatObjects_State_HPP
#define BOOST_IOFM_FormatObjects_State_HPP
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

#  include <boost/outfmt/formatter.hpp>

   namespace boost { namespace io
   {
      template< typename DelimeterType, class StateObject, bool pre, class FormatObject >
      class statefmt_t
      {
         public:
            typedef statefmt_t< StateObject, FormatObject, pre >     this_type;
            typedef typename FormatObject::traits_type               traits_type;
            typedef DelimeterType                                    format_type;
            typedef seq_type< state_type >                           formatter_type;
         private:
            StateObject                state;
            FormatObject               fo;
         public:
            template< typename T, class InputStream >
            inline bool                          read( InputStream & is, T & v ) const
            {
               if( pre && !const_cast< StateObject & >( state ).read( is ))
                  return( false );

               if( !fo.read( is, v )) return( false );

               if( !pre && !const_cast< StateObject & >( state ).read( is ))
                  return( false );

               return( true );
            }
         public:
            template< typename T, class OutputStream >
            inline OutputStream &                write( OutputStream & os, const T & value ) const
            {
#              if defined(BOOST_MSVC)
#                 pragma warning( push )
#                 pragma warning( disable : 4127 ) // conditional expression is constant
#              endif
               if( pre )               const_cast< StateObject & >( state )( os );
               fo.write( os, value );
               if( !pre )              const_cast< StateObject & >( state )( os );
#              if defined(BOOST_MSVC)
#                 pragma warning( pop )
#              endif

               return( os );
            }
         public:
            inline           statefmt_t()
            {
            }
            inline           statefmt_t( const statefmt_t & o ):
               state( o.state ),
               fo(    o.fo )
            {
            }
            inline           statefmt_t( const FormatObject & o ):
               fo( o )
            {
            }
            inline           statefmt_t( const StateObject & s ):
               state( s )
            {
            }
      };

      template< typename DelimeterType, class StateObject, bool pre >
      inline statefmt_t< DelimeterType, StateObject, pre >
                                                 statefmtex()
      {
         statefmt_t< DelimeterType, StateObject, pre >
                                       ob;
         return( ob );
      }

      template< class StateObject, bool pre >
      inline statefmt_t< const char *, StateObject, pre >
                                                 statefmt()
      {
         return( statefmtex< const char *, StateObject, pre >());
      }

      template< class StateObject, bool pre, class FormatObject >
      inline statefmt_t< typename FormatObject::format_type, StateObject, pre, FormatObject >
                                                 statefmt
                                                 (
                                                    const FormatObject & fo
                                                 )
      {
         return( statefmt_t< typename FormatObject::format_type, StateObject, pre, FormatObject >( fo ));
      }
   }}
#endif
