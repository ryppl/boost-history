// (C) Copyright 2003-2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_IOFM_NAryVal_HPP
#define BOOST_IOFM_NAryVal_HPP
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

#  include <boost/outfmt/detail/selector.hpp>

   namespace boost { namespace io
   {
      // type classification:

      struct nary2_type{};

      struct seperable_pair: public nary2_type{};

      template< typename T1, typename T2 = T1 >
      struct inseperable_pair: public nary2_type
      {
         typedef T1                    base1_type;
         typedef T2                    base2_type;
      };

      template< typename T >
      struct nary4_type
      {
         typedef T                     base_type;
      };

      template< typename T >
      struct nary8_type
      {
         typedef T                     base_type;
      };

      // macro helpers:

#     if defined(__GNUC__)
#        define BOOST_IO_NARY_PARAM1( otype, ctype )\
            template< typename T >                  \
            ctype narytype( const otype< T > & )    \
            {                                       \
               ctype ret;                           \
               return( ret );                       \
            }
#        define BOOST_IO_NARY_PARAM2( otype, ctype )\
            template< typename T, typename U >      \
            ctype narytype( const otype< T, U > & ) \
            {                                       \
               ctype ret;                           \
               return( ret );                       \
            }
#     else
#        define BOOST_IO_NARY_PARAM1( otype, ctype )\
            template< typename T >                  \
            ctype narytype( const otype< T > & )    \
            {                                       \
               return( ctype());                    \
            }
#        define BOOST_IO_NARY_PARAM2( otype, ctype )\
            template< typename T, typename U >      \
            ctype narytype( const otype< T, U > & ) \
            {                                       \
               return( ctype());                    \
            }
#     endif

      // std::pair   

      BOOST_IO_NARY_PARAM2( std::pair, seperable_pair )

      template< int n, typename T1, typename T2 >
      typename detail::selector< n, T1, T2 >::type
                                                 getval( const std::pair< T1, T2 > & p )
      {
         return( detail::selector< n, T1, T2 >::value( p.first, p.second ));
      }

      template< int n, typename T1, typename T2 >
      typename detail::selector< n, T1, T2 >::ref_type
                                                 refval( std::pair< T1, T2 > & p )
      {
         return( detail::selector< n, T1, T2 >::ref( p.first, p.second ));
      }
   }}
#endif
