// (C) Copyright 2003: Reece H. Dunn 

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#ifndef BOOST_IOFM_READOB_HPP
#define BOOST_IOFM_READOB_HPP
#  include <boost/outfmt/format_objects.hpp>
#  include <boost/outfmt/detail/config.hpp>

#  include <iostream>
#  include <boost/noncopyable.hpp>

   namespace boost { namespace io
   {
      template< typename T, typename FormatType = char *, class Inputter = boost::io::basic_output >
      class readob_t: public Inputter, public boost::noncopyable
      {
         public:
            T &                        ob;
         public: // constructors
            inline           readob_t( const readob_t & ro ):
               Inputter( ro ),
               ob( ro.ob )
            {
            }
            inline           readob_t( T & val ): ob( val )
            {
            }
            inline           readob_t( T & val, const Inputter & i ):
               Inputter( i ),
               ob( val )
            {
            }
      };

#     if !defined(BOOST_IOFM_NO_BASIC_STREAM)
         template< typename CharT, class TraitsT, typename T, typename FormatType, typename Inputter >
         inline std::basic_istream< CharT, TraitsT > & operator>>
         (
            std::basic_istream< CharT, TraitsT > & is,
            const readob_t< T, FormatType, Inputter >  & ro
         )
         {
            boost::io::detail::input_helper< std::basic_istream< CharT, TraitsT > >
                                                 in( is );
            ro.read( in, ro.ob );
            return( is );
         }
#     else
         template< typename T, typename FormatType, typename Inputter >
         inline std::istream & operator>>
         (
            std::istream & is,
            const readob_t< T, FormatType, Inputter > & ro
         )
         {
            boost::io::detail::input_helper< std::istream >
                                                 in( is );
            ro.read( in, ro.ob );
            return( is );
         }
#     endif

      // read object generators

      template< typename T, class Inputter >
      inline readob_t< T, typename Inputter::format_type, Inputter >
                                                 readobout( T & t, const Inputter & i )
      {
         return( readob_t< T, BOOST_DEDUCED_TYPENAME Inputter::format_type, Inputter >( t, i ));
      }
   }}
#endif
