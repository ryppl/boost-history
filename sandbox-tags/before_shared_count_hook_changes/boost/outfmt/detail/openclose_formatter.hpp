// (C) Copyright 2003-2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_IOFM_DETAIL_OpenCloseFormatter_HPP
#define BOOST_IOFM_DETAIL_OpenCloseFormatter_HPP
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

   namespace boost { namespace io
   {
      template
      <
         typename DelimeterType,
         class    ReferenceType,
         class    FormatTraits = boost::io::detail::default_sequence_traits< DelimeterType >
      >
      class openclose_formatter_t
      {
         public:
            typedef DelimeterType                          format_type;
            typedef FormatTraits                           traits_type;
         private: // format control
            format_type                fmt_open;
            format_type                fmt_close;
         public:
            inline format_type                   open() const
            {
               return( fmt_open );
            }
            inline format_type                   close() const
            {
               return( fmt_close );
            }
         public:
            inline ReferenceType &               format
                                                 (
                                                    format_type o,
                                                    format_type c
                                                 ) 
            {
               fmt_open  = o;
               fmt_close = c;
               return( *static_cast< ReferenceType * >( this ));
            }
            template< typename RefT2, class FormatTraits2 >
            inline ReferenceType &               format
                                                 (
                                                    const openclose_formatter_t< DelimeterType, RefT2, FormatTraits2 > & fmt
                                                 )
            {
               return( format( fmt.open(), fmt.close()));
            }
         public: // constructors
            inline           openclose_formatter_t():
               fmt_open(  traits_type::open_default()  ),
               fmt_close( traits_type::close_default() )
            {
            }
            inline           openclose_formatter_t( format_type o, format_type c ):
               fmt_open(  o ),
               fmt_close( c )
            {
            }
            template< typename RefT2, class FormatTraits2 >
            inline           openclose_formatter_t( const openclose_formatter_t< DelimeterType, RefT2, FormatTraits2 > & fmt ):
               fmt_open(  fmt.open()),
               fmt_close( fmt.close())
            {
            }
      };

      template
      <
         typename DelimeterType,
         class    FormatTraits = boost::io::detail::default_sequence_traits< DelimeterType >
      >
      class openclose_formatter: public openclose_formatter_t
                                        <
                                           DelimeterType,
                                           openclose_formatter< DelimeterType, FormatTraits >,
                                           FormatTraits
                                        >
      {
         public:
            typedef openclose_formatter_t< DelimeterType, openclose_formatter< DelimeterType, FormatTraits >, FormatTraits >
                                                           base_type;
            typedef typename base_type::format_type        format_type;
            typedef typename base_type::traits_type        traits_type;
         public:
            inline           openclose_formatter():
               openclose_formatter_t< DelimeterType, openclose_formatter, FormatTraits >()
            {
            }
            inline           openclose_formatter( format_type o, format_type c ):
               openclose_formatter_t< DelimeterType, openclose_formatter, FormatTraits >( o, c )
            {
            }
            template< typename RefT2, class FormatTraits2 >
            inline           openclose_formatter( const openclose_formatter_t< DelimeterType, RefT2, FormatTraits2 > & fmt ):
               openclose_formatter_t< DelimeterType, openclose_formatter, FormatTraits >( fmt )
            {
            }
      };
   }}
#endif
