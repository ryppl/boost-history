// (C) Copyright 2003: Reece H. Dunn

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#ifndef BOOST_IOFM_FormatObjects_DETAIL_List_HPP
#define BOOST_IOFM_FormatObjects_DETAIL_List_HPP
#  include <boost/outfmt/formatter.hpp>
#  include <boost/detail/iterator.hpp>  // boost::detail::iterator_traits

   namespace boost { namespace io { namespace detail
   {
      template< typename FormatType, class RetType, typename FmtObject = boost::io::basic_object >
      class list_object: public formatter_t< FormatType, RetType >
      {
         public:
            typedef list_object< FormatType, RetType, FmtObject >    this_type;
            typedef typename formatter_t< FormatType, this_type >::traits_type
                                                                     traits_type;
         private:
            FmtObject                  fo;
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
                  if( !fo.read( is, value )) 
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
                  if( !fo.read( is, value ))
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
            template< typename ForwardIterator, class OutputStream >
            inline OutputStream &                write
                                                 (
                                                    OutputStream &  os,
                                                    ForwardIterator first,
                                                    ForwardIterator last
                                                 ) const 
            {
               os << open();

               while( first != last )
               {
                  fo.write( os, *first );

                  if( ++first != last )
                     os << separator();
               }

               return( os << close());
            }
         public:
            inline           list_object()
            {
            }
            inline           list_object( const list_object & lo ):
               formatter_t< FormatType, RetType >( lo ),
               fo( lo.fo )
            {
            }
            inline           list_object( const FmtObject & o ):
               fo( o )
            {
            }
      };
   }}}
#endif
