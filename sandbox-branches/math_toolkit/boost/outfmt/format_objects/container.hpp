// (C) Copyright 2003-2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_IOFM_FormatObjects_Container_HPP
#define BOOST_IOFM_FormatObjects_Container_HPP
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

#  include <boost/outfmt/format_objects/detail/list.hpp>

   namespace boost { namespace io
   {
      namespace detail
      {
         template< class Container, typename T = typename Container::value_type >
         class insert_iterator: public boost::noncopyable
         {
            public:
               typedef Container                                     container_type;
               typedef std::output_iterator_tag                      category_type;
               typedef void                                          pointer;
               typedef typename Container::reference                 reference;
               typedef T                                             value_type;
            private:
               Container &             c;
            public:
               insert_iterator &  operator=( typename Container::const_reference val )
               {
                  get_typeid< Container >::value; // BCB workaround
                  insert( c, val, seq_type< get_typeid< Container >::value >());
                  return( *this );
               }
               insert_iterator &  operator*(){       return( *this ); }
               insert_iterator &  operator++(){      return( *this ); }
               insert_iterator    operator++( int ){ return( *this ); }
            private:
               void                              insert( Container & cont, typename Container::const_reference val, seq_type< seq_container_type > )
               {
                  cont.insert( cont.end(), val );
               }
               void                              insert( Container & cont, typename Container::const_reference val, seq_type< set_container_type > )
               {
                  cont.insert( val );
               }
               void                              insert( Container & cont, typename Container::const_reference val, seq_type< assoc_container_type > )
               {
                  cont.insert( val );
               }
            public:
               explicit      insert_iterator( Container & x ): c( x )
               {
               }
               inline        insert_iterator( const insert_iterator & i ): c( i.c )
               {
               }
         };

         template< class Container >
         insert_iterator< Container >            inserter( Container & c )
         {
            return( insert_iterator< Container >( c ));
         }

         template< class Container >
         insert_iterator< Container, std::pair< typename Container::key_type, typename Container::mapped_type > >
                                                 inserter_assoc( Container & c ) 
         {
            return( insert_iterator
            <
               Container,
               std::pair< typename Container::key_type, typename Container::mapped_type >
            >( c ));
         }
      }

      template< typename DelimeterType, class FormatObject >
      class containerfmt_t: public detail::list_object< DelimeterType, containerfmt_t
                                                                       <
                                                                          DelimeterType,
                                                                          FormatObject
                                                                       >, FormatObject >
      {
         private:
            typedef detail::list_object< DelimeterType, containerfmt_t< DelimeterType, FormatObject >, FormatObject >
                                                                     base_type;
         public:
            typedef seq_type< seq_container_type >                   formatter_type; // container_type?
         public:
            template< typename Container, class InputStream >
            inline bool                          read( InputStream & is, Container & c ) const
            {
               c.clear();
               get_typeid< Container >::value; // BCB workaround
               return( read( is, c, seq_type< get_typeid< Container >::value >()));
            }
            template< typename Container, class OutputStream >
            inline OutputStream &                write( OutputStream & os, const Container & c ) const
            {
               const base_type *       self = static_cast< const base_type * >( this );
               return(( *self ).write( os, c.begin(), c.end()));
            }
         private:
            template< typename Container, class InputStream >
            inline bool                          read( InputStream & is, Container & c, seq_type< seq_container_type > ) const
            {
               const base_type *       self = static_cast< const base_type * >( this );
               return(( *self ).readc( is, detail::inserter( c )));
            }
            template< typename Container, class InputStream >
            inline bool                          read( InputStream & is, Container & c, seq_type< set_container_type > ) const
            {
               const base_type *       self = static_cast< const base_type * >( this );
               return(( *self ).readc( is, detail::inserter( c )));
            }
            template< typename Container, class InputStream >
            inline bool                          read( InputStream & is, Container & c, seq_type< assoc_container_type > ) const
            {
               const base_type *       self = static_cast< const base_type * >( this );
               return(( *self ).readc( is, detail::inserter_assoc( c )));
            }
         public:
            inline           containerfmt_t()
            {
            }
            inline           containerfmt_t( const containerfmt_t & o ):
               detail::list_object< DelimeterType, containerfmt_t< DelimeterType, FormatObject >, FormatObject >( o )
            {
            }
            inline           containerfmt_t( const FormatObject & o ):
               detail::list_object< DelimeterType, containerfmt_t< DelimeterType, FormatObject >, FormatObject >( o )
            {
            }
      };

      template< class DelimeterType >
      inline containerfmt_t< DelimeterType >     containerfmtex()
      {
         containerfmt_t< DelimeterType > ob;
         return( ob );
      }

      inline containerfmt_t< const char * >      containerfmt()
      {
         return( containerfmtex< const char * >());
      }

      template< class FormatObject >
      inline containerfmt_t< BOOST_DEDUCED_TYPENAME FormatObject::format_type, FormatObject >
                                                 containerfmt
                                                 (
                                                    const FormatObject & o
                                                 )
      {
         return( containerfmt_t< BOOST_DEDUCED_TYPENAME FormatObject::format_type, FormatObject >( o ));
      }
   }}
#endif
