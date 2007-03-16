// (C) Copyright 2003-2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_IOFM_FormatObjects_DETAIL_List_HPP
#define BOOST_IOFM_FormatObjects_DETAIL_List_HPP
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

#  include <boost/outfmt/formatter.hpp>
#  include <boost/detail/iterator.hpp>  // boost::detail::iterator_traits

   namespace boost { namespace io { namespace detail
   {
      template< typename DelimeterType, class RetType, typename FormatObject = boost::io::basicfmt_t >
      class list_object: public formatter_t< DelimeterType, RetType >
      {
         public:
            typedef list_object< DelimeterType, RetType, FormatObject > this_type;
            typedef typename formatter_t< DelimeterType, this_type >::traits_type
                                                                     traits_type;
         private:
            FormatObject               fo;
         public:
            template< class InputStream, class Iterator >
            inline bool                          readc
                                                 (
                                                    InputStream & is,
                                                    Iterator      i
                                                 ) const
            {
               if( !is.match( open()))           return( false );

               typename InputStream::char_type
                                       cch = is.firstch( close());
               typename InputStream::char_type
                                       ch  = '\0';
               typename Iterator::value_type
                                       value;

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
               formatter_t< DelimeterType, RetType >( lo ),
               fo( lo.fo )
            {
            }
            inline           list_object( const FormatObject & o ):
               fo( o )
            {
            }
      };
   }}}
#endif
