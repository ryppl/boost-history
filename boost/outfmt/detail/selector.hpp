// (C) Copyright 2003-2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_IOFM_DETAIL_SELECTOR_HPP
#define BOOST_IOFM_DETAIL_SELECTOR_HPP
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

#  include <boost/mpl/if.hpp>
#  include <boost/mpl/void.hpp>
#  include <iostream>

   namespace boost { namespace io { namespace detail
   {
      // mpl-style class to test for comparison of two numbers

      template< int a, int b >
      struct eq_int
      {
         BOOST_STATIC_CONSTANT( bool, value = ( a == b ));
      };

      template< int a, int b >
      struct lt_int
      {
         BOOST_STATIC_CONSTANT( bool, value = ( a < b ));
      };

      // mpl-style compile-time switch statement for sequentially numbered objects 

      template
      <
         int n,
         typename T1,              typename T2,
         typename T3 = mpl::void_, typename T4 = mpl::void_,
         typename T5 = mpl::void_, typename T6 = mpl::void_,
         typename T7 = mpl::void_, typename T8 = mpl::void_
      >
      struct sequential_switch_: public boost::mpl::if_
             <
                lt_int< n, 5 >,
                typename boost::mpl::if_
                <
                   lt_int< n, 3 >,
                   typename boost::mpl::if_
                   <
                      eq_int< n, 1 >, T1, T2
                   >::type,
                   typename boost::mpl::if_
                   <
                      eq_int< n, 3 >, T3, T4
                   >::type
                >::type,
                typename boost::mpl::if_
                <
                   lt_int< n, 7 >,
                   typename boost::mpl::if_
                   <
                      eq_int< n, 5 >, T5, T6
                   >::type,
                   typename boost::mpl::if_
                   <
                      eq_int< n, 7 >, T7, T8
                   >::type
                >::type
             >
      {
      };

      // mpl-style function: select the nth value from a list of values

      template
      <
         int n,
         typename T1,      typename T2 = T1, typename T3 = T1, typename T4 = T1,
         typename T5 = T1, typename T6 = T1, typename T7 = T1, typename T8 = T1
      >
      struct selector
      {
         // workaround for compilers that don't like template<> struct get< n >{ ... } in a class

         struct get1
         {
            static const T1 &                    value( const T1 & a, const T2 & )
            {
               return( a );
            }
            static const T1 &                    value( const T1 & a, const T2 &, const T3 &, const T4 & )
            {
               return( a );
            }
            static const T1 &                    value( const T1 & a, const T2 &, const T3 &, const T4 &, const T5 &, const T6 &, const T7 &, const T8 & )
            {
               return( a );
            }
         };

         struct get2
         {
            static const T2 &                    value( const T1 &, const T2 & b )
            {
               return( b );
            }
            static const T2 &                    value( const T1 &, const T2 & b, const T3 &, const T4 & )
            {
               return( b );
            }
            static const T1 &                    value( const T1 &, const T2 & b, const T3 &, const T4 &, const T5 &, const T6 &, const T7 &, const T8 & )
            {
               return( b );
            }
         };

         struct get3
         {
            static const T3 &                    value( const T1 &, const T2 &, const T3 & c, const T4 & )
            {
               return( c );
            }
            static const T1 &                    value( const T1 &, const T2 &, const T3 & c, const T4 &, const T5 &, const T6 &, const T7 &, const T8 & )
            {
               return( c );
            }
         };

         struct get4
         {
            static const T4 &                    value( const T1 &, const T2 &, const T3 &, const T4 & d )
            {
               return( d );
            }
            static const T1 &                    value( const T1 &, const T2 &, const T3 &, const T4 & d, const T5 &, const T6 &, const T7 &, const T8 & )
            {
               return( d );
            }
         };

         struct get5
         {
            static const T5 &                    value( const T1 &, const T2 &, const T3 &, const T4 &, const T5 & e, const T6 &, const T7 &, const T8 & )
            {
               return( e );
            }
         };

         struct get6
         {
            static const T6 &                    value( const T1 &, const T2 &, const T3 &, const T4 &, const T5 &, const T6 & f, const T7 &, const T8 & )
            {
               return( f );
            }
         };

         struct get7
         {
            static const T7 &                    value( const T1 &, const T2 &, const T3 &, const T4 &, const T5 &, const T6 &, const T7 & g, const T8 & )
            {
               return( g );
            }
         };

         struct get8
         {
            static const T8 &                    value( const T1 &, const T2 &, const T3 &, const T4 &, const T5 &, const T6 &, const T7 &, const T8 & h )
            {
               return( h );
            }
         };

         // type selectors

         typedef typename sequential_switch_
                 <
                    n, T1, T2, T3, T4, T5, T6, T7, T8
                 >::type                                             base_type;

         typedef const base_type &                                   type;
         typedef       base_type &                                   ref_type;

         typedef typename sequential_switch_
                 <
                    n, get1, get2, get3, get4, get5, get6, get7, get8
                 >::type                                             getter;

         // nary< 2 > value selection

         static type                             value( const T1 & a, const T2 & b )
         {
            return( getter::value( a, b ));
         }
         static ref_type                         ref( const T1 & a, const T2 & b )
         {
            return( const_cast< ref_type >( getter::value( a, b )));
         }

         // nary< 4 > value selection

         static type                             value
                                                 (
                                                    const T1 & a, const T2 & b, const T3 & c, const T4 & d
                                                 )
         {
            return( getter::value( a, b, c, d ));
         }
         static ref_type                         ref
                                                 (
                                                    const T1 & a, const T2 & b, const T3 & c, const T4 & d
                                                 )
         {
            return( const_cast< ref_type >( getter::value( a, b, c, d )));
         }

         // nary< 8 > value selection

         static type                             value
                                                 (
                                                    const T1 & a, const T2 & b, const T3 & c, const T4 & d,
                                                    const T5 & e, const T6 & f, const T7 & g, const T8 & h
                                                 )
         {
            return( getter::value( a, b, c, d, e, f, g, h ));
         }
         static ref_type                         ref
                                                 (
                                                    const T1 & a, const T2 & b, const T3 & c, const T4 & d,
                                                    const T5 & e, const T6 & f, const T7 & g, const T8 & h
                                                 )
         {
            return( const_cast< ref_type >( getter::value( a, b, c, d, e, f, g, h )));
         }
      };
   }}}
#endif
