// (C) Copyright 2003: Reece H. Dunn

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#ifndef BOOST_IOFM_FormatObjects_DETAIL_ListOutput_HPP
#define BOOST_IOFM_FormatObjects_DETAIL_ListOutput_HPP
#  include <boost/outfmt/formatter.hpp>
#  include <boost/detail/iterator.hpp>  // boost::detail::iterator_traits

   namespace boost { namespace io { namespace detail
   {
      template< typename FormatType, class RetType, typename Outputter = boost::io::basic_output >
      class list_output: public formatter_t< FormatType, RetType >
      {
         public:
            typedef list_output< FormatType, RetType, Outputter >    this_type;
            typedef typename formatter_t< FormatType, this_type >::traits_type
                                                                     traits_type;
         private:
            Outputter                  outputter;
         public:
            template< typename ForwardIterator, class OutputStream >
            inline OutputStream & operator()
                                  (
                                     OutputStream &  os,
                                     ForwardIterator first,
                                     ForwardIterator last
                                  ) const 
            {
               os << open();

               while( first != last )
               {
                  outputter( os, *first );

                  if( ++first != last )
                     os << separator();
               }

               return( os << close());
            }
         public:
            template< class InputStream, class OutputIterator, typename T >
            inline bool                          readc
                                                 (
                                                    InputStream  & is,
                                                    OutputIterator i,
                                                    T            & value
                                                 ) const
            {
               if( !is.match( open()))           return( false );

               typename InputStream::char_type
                                       cch = is.firstch( close());
               typename InputStream::char_type
                                       ch  = '\0';

               while( is.readfirstch( ch ) && !is.eq( ch, cch ))
               {
                  if( !outputter.read( is, value ))
                     return( false );

                  *i++ = value;

                  if( is.readfirstch( ch ) && !is.eq( ch, cch ))
                  {
                     if( !is.match( separator()))
                        return( false );
                  }
               }

               return( is.match( close()));
            }
         public:
            template< class InputStream, class Iterator >
            inline bool                          read
                                                 (
                                                    InputStream & is,
                                                    Iterator      first,
                                                    Iterator      last
                                                 ) const
            {
               if( !is.match( open()))           return( false );

               typename InputStream::char_type
                                       cch = is.firstch( close());
               typename InputStream::char_type
                                       ch  = '\0';
               typename boost::detail::iterator_traits< Iterator >::value_type
                                       value;

               while(( first != last ) && is.readfirstch( ch ) && !is.eq( ch, cch ))
               {
                  if( !outputter.read( is, value ))
                     return( false );

                  *first++ = value;

                  if( is.readfirstch( ch ) && !is.eq( ch, cch ))
                  {
                     if( !is.match( separator()))
                        return( false );
                  }
               }

               return( is.match( close()));
            }
         public:
            inline           list_output()
            {
            }
            inline           list_output( const list_output & lo ):
               formatter_t< FormatType, RetType >( lo ),
               outputter( lo.outputter )
            {
            }
            inline           list_output( const Outputter & o ):
               outputter( o )
            {
            }
      };
   }}}
#endif
