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
      template< typename FormatType, class StateObject, bool pre, class FmtObject >
      class state_object
      {
         public:
            typedef state_object< StateObject, FmtObject, pre >      this_type;
            typedef typename FmtObject::traits_type                  traits_type;
            typedef FormatType                                       format_type;
            typedef seq_type< state_type >                           formatter_type;
         private:
            StateObject                state;
            FmtObject                  fo;
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
            inline           state_object()
            {
            }
            inline           state_object( const state_object & o ):
               state( o.state ),
               fo(    o.fo )
            {
            }
            inline           state_object( const FmtObject & o ):
               fo( o )
            {
            }
            inline           state_object( const StateObject & s ):
               state( s )
            {
            }
      };

      template< typename FormatType, class StateObject, bool pre >
      inline state_object< FormatType, StateObject, pre >
                                                 statefmtex()
      {
         state_object< FormatType, StateObject, pre >
                                       ob;
         return( ob );
      }

      template< class StateObject, bool pre >
      inline state_object< const char *, StateObject, pre >
                                                 statefmt()
      {
         return( statefmtex< const char *, StateObject, pre >());
      }

      template< class StateObject, bool pre, class FmtObject >
      inline state_object< typename FmtObject::format_type, StateObject, pre, FmtObject >
                                                 statefmt
                                                 (
                                                    const FmtObject & fo
                                                 )
      {
         return( state_object< typename FmtObject::format_type, StateObject, pre, FmtObject >( fo ));
      }
   }}
#endif
