// (C) Copyright 2003: Reece H. Dunn

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#ifndef BOOST_IOFM_DETAIL_SELECTOR_HPP
#define BOOST_IOFM_DETAIL_SELECTOR_HPP
#  include <boost/mpl/if.hpp>
#  include <iostream>

   namespace boost { namespace io { namespace detail
   {
      // mpl-style class to test for equality of two numbers

      template< int a, int b >
      struct eq_int
      {
         BOOST_STATIC_CONSTANT( bool, value = ( a == b ));
      };

      // mpl-style compile-time switch statement

      template< int n,                    // switch( n ){
         int v1,      typename T1,        //    case v1: T1;
         int v2,      typename T2,        //    case v2: T2;
         int v3 = -1, typename T3 = void, //   [case v3: T3;]
         int v4 = -1, typename T4 = void, //   [case v4: T4;]
         int v5 = -1, typename T5 = void, //   [case v5: T5;]
         int v6 = -1, typename T6 = void, //   [case v6: T6;]
         int v7 = -1, typename T7 = void, //   [case v7: T7;]
         typename T8              = void  //   [default: T8;] }
      >
      struct switch_: public boost::mpl::if_
                      <
                         eq_int< n, v1 >, T1, typename boost::mpl::if_
                         <
                            eq_int< n, v2 >, T2, typename boost::mpl::if_
                            <
                               eq_int< n, v3 >, T3, typename boost::mpl::if_
                               <
                                  eq_int< n, v4 >, T4, typename boost::mpl::if_
                                  <
                                     eq_int< n, v5 >, T5, typename boost::mpl::if_
                                     <
                                        eq_int< n, v6 >, T6, typename boost::mpl::if_
                                        <
                                           eq_int< n, v7 >, T7,
                                           T8
                                        >::type
                                     >::type
                                  >::type
                               >::type
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

         typedef typename const switch_
                 <
                    n,
                    1, T1, 2, T2, 3, T3, 4, T4, 5, T5, 6, T6, 7, T7,
                    T8
                 >::type &                                           type;

         typedef typename switch_
                 <
                    n,
                    1, T1, 2, T2, 3, T3, 4, T4, 5, T5, 6, T6, 7, T7,
                    T8
                 >::type &                                           ref_type;

         typedef typename switch_
                 <
                    n,
                    1, get1, 2, get2, 3, get3, 4, get4, 5, get5, 6, get6, 7, get7,
                    get8
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

         // nary< 8 > value selection

         static type                             value
                                                 (
                                                    const T1 & a, const T2 & b, const T3 & c, const T4 & d,
                                                    const T5 & e, const T6 & f, const T7 & g, const T8 & h
                                                 )
         {
            return( getter::value( a, b, c, d, e, f, g, h ));
         }
      };
   }}}
#endif
