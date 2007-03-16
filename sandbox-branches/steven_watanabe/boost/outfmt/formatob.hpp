// (C) Copyright 2003-2004: Reece H. Dunn 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_IOFM_FormatOb_HPP
#define BOOST_IOFM_FormatOb_HPP
#  include <boost/outfmt/detail/config.hpp>
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

#  include <boost/outfmt/format_objects.hpp>
#  include <boost/outfmt/detail/config.hpp>
#  if !defined(BOOST_IOFM_NO_OUTPUT_DEDUCTION) // automatic type deduction
#     include <boost/outfmt/deducer.hpp>
#  endif

#  include <iostream>
#  include <boost/noncopyable.hpp>
#  include <boost/call_traits.hpp>

   namespace boost { namespace io
   {
      template< typename T, typename DelimeterType = const char *, class FormatObject = boost::io::basicfmt_t >
      class formatob_t: public FormatObject, public boost::noncopyable
      {
         public:
            typename mpl::if_
            <
               mpl::or_
               <
                  is_range< T >, is_formatter< FormatObject, range_type >,
                  is_basic< T >, is_formatter< FormatObject, basic_type >
               >,
               typename boost::call_traits< T >::value_type,
               typename boost::call_traits< T >::const_reference
            >::type                    ob;
         public:
            typedef typename FormatObject::format_type     format_type;
            typedef typename FormatObject::traits_type     traits_type;
         public:
            inline formatob_t &                  format
                                                 (
                                                    format_type o,
                                                    format_type c,
                                                    format_type s
                                                 )
            {
               FormatObject::format( o, c, s );
               return( *this );
            }
            inline formatob_t &                  format
                                                 (
                                                    format_type o,
                                                    format_type c
                                                 )
            {
               FormatObject::format( o, c );
               return( *this );
            }
            inline formatob_t &                  format
                                                 (
                                                    format_type s
                                                 )
            {
               FormatObject::format( s );
               return( *this );
            }

            template< typename RT2, class FormatTraits2 >
            inline formatob_t &                  format
                                                 (
                                                    const openclose_formatter_t< DelimeterType, RT2, FormatTraits2 > & fmt
                                                 )
            {
               FormatObject::format( fmt );
               return( *this );
            }

            template< typename RT2, class FormatTraits2 >
            inline formatob_t &                  format
                                                 (
                                                    const formatter_t< DelimeterType, RT2, FormatTraits2 > & fmt
                                                 )
            {
               FormatObject::format( fmt );
               return( *this );
            }
         private:
            inline           formatob_t();
         public: // constructors
            inline           formatob_t( const formatob_t & fo ):
               FormatObject( fo ),
               ob( fo.ob )
            {
            }
            inline           formatob_t( const T & val ): ob( val )
            {
            }
            inline           formatob_t( const T & val, const FormatObject & o ):
               FormatObject( o ),
               ob( val )
            {
            }
      };

#     if !defined(BOOST_IOFM_NO_BASIC_STREAM)
         template< typename CharT, class TraitsT, typename T, typename DelimeterType, typename FormatObject >
         inline std::basic_ostream< CharT, TraitsT > & operator<<
         (
            std::basic_ostream< CharT, TraitsT > & os,
            const formatob_t< T, DelimeterType, FormatObject > & fo
         )
         {
            return( fo.write( os, fo.ob ));
         }

         template< typename CharT, class TraitsT, typename T, typename DelimeterType, typename FormatObject >
         inline std::basic_istream< CharT, TraitsT > & operator>>
         (
            std::basic_istream< CharT, TraitsT >         & is,
            const formatob_t< T, DelimeterType, FormatObject > & fo
         )
         {
            boost::io::detail::input_helper< std::basic_istream< CharT, TraitsT > >
                                                 in( is );
            fo.read( in, const_cast< T & >( fo.ob ));
            return( is );
         }
#     else
         template< typename T, typename DelimeterType, typename FormatObject >
         inline std::ostream & operator<<
         (
            std::ostream & os,
            const formatob_t< T, DelimeterType, FormatObject > & fo
         )
         {
            return( fo.write( os, fo.ob ));
         }

         template< typename T, typename DelimeterType, typename FormatObject >
         inline std::istream & operator>>
         (
            std::istream & is,
            const formatob_t< T, DelimeterType, FormatObject > & fo
         )
         {
            boost::io::detail::input_helper< std::istream >
                                                 in( is );
            fo.read( in, const_cast< T & >( fo.ob ));
            return( is );
         }
#     endif

      // format object generators

#     if !defined(BOOST_IOFM_NO_OUTPUT_DEDUCTION) // automatic type deduction
         template< typename T >
         inline formatob_t< T, const char *, typename deducer< T, const char * >::type::outputter >
                                                 formatob( const T & t )
         {
            return( formatob_t< T, const char *, BOOST_DEDUCED_TYPENAME deducer< T, const char * >::type::outputter >
            (
               t,
               deducer< T, const char * >::type::generate()
            ));
         }

         template< typename DelimeterType, typename T >
         inline formatob_t< T, DelimeterType, typename deducer< T, DelimeterType >::type::outputter >
                                                 formatobex( const T & t )
         {
            return( formatob_t< T, DelimeterType, BOOST_DEDUCED_TYPENAME deducer< T, DelimeterType >::type::outputter >
            (
               t,
               deducer< T, DelimeterType >::type::generate()
            ));
         }
#     else // no automatic deduction: use basic_output instead
         template< typename T >
         inline formatob_t< T >                  formatob( const T & t )
         {
            return( formatob_t< T >( t ));
         }

         template< typename DelimeterType, typename T >
         inline formatob_t< T, DelimeterType >      formatobex( const T & t )
         {
            return( formatob_t< T, DelimeterType >( t ));
         }
#     endif

      template< typename T, class FormatObject >
      inline formatob_t< T, typename FormatObject::format_type, FormatObject >
                                                 formatob( const T & t, const FormatObject & o )
      {
         return( formatob_t< T, BOOST_DEDUCED_TYPENAME FormatObject::format_type, FormatObject >( t, o ));
      }
   }}
#endif
