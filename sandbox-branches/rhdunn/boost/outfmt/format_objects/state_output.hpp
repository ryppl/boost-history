// (C) Copyright 2003: Reece H. Dunn 

#ifndef BOOST__IOFM__FORMAT_OBJECTS__STATE_OUTPUT__HPP
#define BOOST__IOFM__FORMAT_OBJECTS__STATE_OUTPUT__HPP
#  include <boost/outfmt/formatter.hpp>

   namespace boost { namespace io
   {
      template< typename FormatType, class StateObject, bool pre, class Outputter >
      class state_output
      {
         public:
            typedef state_output< StateObject, Outputter, pre >      this_type;
            typedef typename Outputter::traits_type                  traits_type;
            typedef FormatType                                       format_type;
         private:
            StateObject                state;
            Outputter                  out;
         public:
            template< typename T, class OutputStream >
            inline OutputStream & operator()( OutputStream & os, const T & value ) const
            {
#              if defined(_MSC_VER)
#                 pragma warning( push )
#                 pragma warning( disable : 4127 ) // conditional expression is constant
#              endif
               if( pre )               const_cast< StateObject & >( state )( os );
               out( os, value );
               if( !pre )              const_cast< StateObject & >( state )( os );
#              if defined(_MSC_VER)
#                 pragma warning( pop )
#              endif

               return( os );
            }
         public:
            inline           state_output()
            {
            }
            inline           state_output( const state_output & o ):
               state( o.state ),
               out(   o.out )
            {
            }
            inline           state_output( const Outputter & o ):
               out( o )
            {
            }
            inline           state_output( const StateObject & s ):
               state( s )
            {
            }
      };

      template< class StateObject, bool pre >
      inline state_output< char *, StateObject, pre >
                                                 statefmt()
      {
         state_output< char *, StateObject, pre >
                                       out;
         return( out );
      }

      template< typename FormatType, class StateObject, bool pre >
      inline state_output< FormatType, StateObject, pre >
                                                 statefmtex()
      {
         state_output< FormatType, StateObject, pre >
                                       out;
         return( out );
      }

      template< class StateObject, bool pre, class Outputter >
      inline state_output< typename Outputter::format_type, StateObject, pre, Outputter >
                                                 statefmtout
                                                 (
                                                    const Outputter & out
                                                 )
      {
         return( state_output< typename Outputter::format_type, StateObject, pre, Outputter >( out ));
      }
   }}
#endif
