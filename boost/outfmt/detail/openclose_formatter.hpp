// (C) Copyright 2003: Reece H. Dunn

#ifndef BOOST__IOFM__DETAIL__OPENCLOSE_FORMATTER__HPP
#define BOOST__IOFM__DETAIL__OPENCLOSE_FORMATTER__HPP
   namespace boost { namespace io
   {
      template
      <
         typename FormatType,
         class    ReferenceType,
         class    FormatTraits = boost::io::detail::format_traits< FormatType >
      >
      class openclose_formatter_t
      {
         public:
            typedef FormatType                             format_type;
            typedef FormatTraits                           traits_type;
         private: // format control
            format_type                fmt_open;
            format_type                fmt_close;
         public:
            inline format_type                   open() const throw()
            {
               return( fmt_open );
            }
            inline format_type                   close() const throw()
            {
               return( fmt_close );
            }
         public:
            inline ReferenceType &               format
                                                 (
                                                    format_type o,
                                                    format_type c
                                                 ) throw()
            {
               fmt_open  = o;
               fmt_close = c;
               return( *static_cast< ReferenceType * >( this ));
            }
            template< typename RT2, class FormatTraits2 >
            inline ReferenceType &               format
                                                 (
                                                    const openclose_formatter_t< FormatType, RT2, FormatTraits2 > & fmt
                                                 )
            {
               return( format( fmt.open(), fmt.close()));
            }
         public: // constructors
            inline           openclose_formatter_t() throw():
               fmt_open(  traits_type::open_default  ),
               fmt_close( traits_type::close_default )
            {
            }
            inline           openclose_formatter_t( format_type o, format_type c ) throw():
               fmt_open(  o ),
               fmt_close( c )
            {
            }
            template< typename RT2, class FormatTraits2 >
            inline           openclose_formatter_t( const openclose_formatter_t< FormatType, RT2, FormatTraits2 > & fmt ) throw():
               fmt_open(  fmt.open()),
               fmt_close( fmt.close())
            {
            }
      };

      template
      <
         typename FormatType,
         class    FormatTraits = boost::io::detail::format_traits< FormatType >
      >
      class openclose_formatter: public openclose_formatter_t
                                        <
                                           FormatType,
                                           openclose_formatter< FormatType, FormatTraits >,
                                           FormatTraits
                                        >
      {
         public:
            typedef openclose_formatter_t< FormatType, openclose_formatter< FormatType, FormatTraits >, FormatTraits >
                                                           base_type;
            typedef typename base_type::format_type        format_type;
            typedef typename base_type::traits_type        traits_type;
         public:
            inline           openclose_formatter() throw():
               openclose_formatter_t< FormatType, openclose_formatter, FormatTraits >()
            {
            }
            inline           openclose_formatter( format_type o, format_type c ) throw():
               openclose_formatter_t< FormatType, openclose_formatter, FormatTraits >( o, c )
            {
            }
            template< typename RT2, class FormatTraits2 >
            inline           openclose_formatter( const openclose_formatter_t< FormatType, RT2, FormatTraits2 > & fmt ) throw():
               openclose_formatter_t< FormatType, openclose_formatter, FormatTraits >( fmt )
            {
            }
      };
   }}
#endif
