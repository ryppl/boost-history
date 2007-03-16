// Copyright (C) 2001-2003 Roland Richter <roland@flll.jku.at>
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all copies.
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.
#ifndef BOOST_TUPPLE_DETAIL_TUPPLE_NO_PART_SPEC_HPP
#define BOOST_TUPPLE_DETAIL_TUPPLE_NO_PART_SPEC_HPP
#include <boost/tupple/detail/tupple_detail.hpp>
#include <algorithm>


#include <boost/type_traits.hpp>
#include <boost/mpl/if.hpp>
#include <boost/static_assert.hpp>


namespace boost
{
  namespace tupple
  {
    template < class T >
    struct tuple0
    {
      typedef tuple0 < T > self_type;
      int size( ) const
      {
        return 0;
      }
    };


    template < class T0 >
    struct tuple1
    {
      typedef T0 type0;
      typedef typename access_traits < T0 >::arg_type arg_type0;
      typedef typename access_traits < T0 >::non_const_type get_type0;
      typedef typename access_traits < T0 >::const_type const_get_type0;
      typedef tuple1 < T0 > self_type;
      typedef tuple0 < null_type > tail_type;
      tuple1( )
      {}
      tuple1( arg_type0 theM0 ) : m0( theM0 )
      {}
      template < class S0 >
      tuple1( const tuple1 < S0 > &rhs ) : m0( rhs.m0 )
      {}
      tuple1( arg_type0 theM0, const tail_type & tail ) : m0( theM0 )
      {}
      template < class S0 >
      self_type & operator=( const tuple1 < S0 >
                             &rhs )
      {
        if ( this != ( self_type * ) ( &rhs ) )
        {
          m0 = rhs.m0;
        }
        return *this;
      }
      int size( ) const
      {
        return 1;
      }
      void swap( self_type & rhs )
      {
        std::swap( m0, rhs.m0 );
      }
      get_type0 head( )
      {
        return m0;
      }
      const_get_type0 head( ) const
      {
        return m0;
      }
      tail_type tail( ) const
      {
        return tail_type( );
      }
      get_type0 first( )
      {
        return m0;
      }
      const_get_type0 first( ) const
      {
        return m0;
      }
      get_type0 last( )
      {
        return m0;
      }
      const_get_type0 last( ) const
      {
        return m0;
      }
      const_get_type0 get0( ) const
      {
        return m0;
      }
      get_type0 get0( )
      {
        return m0;
      }
      T0 m0;
    };
    template < class T0, class T1 >
    struct tuple2
    {
      typedef T0 type0;
      typedef T1 type1;
      typedef typename access_traits < T0 >::arg_type arg_type0;
      typedef typename access_traits < T1 >::arg_type arg_type1;
      typedef typename access_traits < T0 >::non_const_type get_type0;
      typedef typename access_traits < T1 >::non_const_type get_type1;
      typedef typename access_traits < T0 >::const_type const_get_type0;
      typedef typename access_traits < T1 >::const_type const_get_type1;
      typedef tuple2 < T0, T1 > self_type;
      typedef tuple1 < T1 > tail_type;
      tuple2( )
      {}
      tuple2( arg_type0 theM0, arg_type1 theM1 ) : m0( theM0 ), m1( theM1 )
      {}
      template < class S0, class S1 >
      tuple2( const tuple2 < S0,
              S1 > &rhs ) : m0( rhs.m0 ),
          m1( rhs.m1 )
      {}
      tuple2( arg_type0 theM0, const tail_type & tail ) : m0( theM0 ),
          m1( tail.m0 )
      {}
      template < class S0,
      class S1 >
      self_type & operator=( const tuple2 < S0, S1 > &rhs )
      {
        if ( this != ( self_type * ) ( &rhs ) )
        {
          m0 = rhs.m0;
          m1 = rhs.m1;
        }
        return *this;
      }
      template < class U1,
      class U2 >
      self_type & operator=( const std::pair < U1, U2 > &p )
      {
        m0 = p.first;
        m1 = p.second;
        return *this;
      }
      int size( ) const
      {
        return 2;
      }
      void swap( self_type & rhs )
      {
        std::swap( m0, rhs.m0 );
        std::swap( m1, rhs.m1 );
      }
      get_type0 head( )
      {
        return m0;
      }
      const_get_type0 head( ) const
      {
        return m0;
      }
      tail_type tail( ) const
      {
        return tail_type( m1 );
      }
      get_type0 first( )
      {
        return m0;
      }
      const_get_type0 first( ) const
      {
        return m0;
      }
      get_type1 last( )
      {
        return m1;
      }
      const_get_type1 last( ) const
      {
        return m1;
      }
      const_get_type0 get0( ) const
      {
        return m0;
      }
      const_get_type1 get1( ) const
      {
        return m1;
      }
      get_type0 get0( )
      {
        return m0;
      }
      get_type1 get1( )
      {
        return m1;
      }
      T0 m0;
      T1 m1;
    };
    template < class T0, class T1, class T2 >
    struct tuple3
    {
      typedef T0 type0;
      typedef T1 type1;
      typedef T2 type2;
      typedef typename access_traits < T0 >::arg_type arg_type0;
      typedef typename access_traits < T1 >::arg_type arg_type1;
      typedef typename access_traits < T2 >::arg_type arg_type2;
      typedef typename access_traits < T0 >::non_const_type get_type0;
      typedef typename access_traits < T1 >::non_const_type get_type1;
      typedef typename access_traits < T2 >::non_const_type get_type2;
      typedef typename access_traits < T0 >::const_type const_get_type0;
      typedef typename access_traits < T1 >::const_type const_get_type1;
      typedef typename access_traits < T2 >::const_type const_get_type2;
      typedef tuple3 < T0, T1, T2 > self_type;
      typedef tuple2 < T1, T2 > tail_type;
      tuple3( )
      {}
      tuple3( arg_type0 theM0, arg_type1 theM1,
              arg_type2 theM2 ) : m0( theM0 ), m1( theM1 ), m2( theM2 )
      {}
      template < class S0, class S1, class S2 >
      tuple3( const tuple3 < S0,
              S1,
              S2 >
              &rhs ) : m0( rhs.m0 ),
          m1( rhs.m1 ), m2( rhs.m2 )
      {}
      tuple3( arg_type0 theM0, const tail_type & tail ) : m0( theM0 ),
          m1( tail.m0 ), m2( tail.m1 )
      {}
      template < class S0, class S1,
      class S2 >
      self_type & operator=( const tuple3 < S0, S1, S2 > &rhs )
      {
        if ( this != ( self_type * ) ( &rhs ) )
        {
          m0 = rhs.m0;
          m1 = rhs.m1;
          m2 = rhs.m2;
        }
        return *this;
      }
      int size( ) const
      {
        return 3;
      }
      void swap( self_type & rhs )
      {
        std::swap( m0, rhs.m0 );
        std::swap( m1, rhs.m1 );
        std::swap( m2, rhs.m2 );
      }
      get_type0 head( )
      {
        return m0;
      }
      const_get_type0 head( ) const
      {
        return m0;
      }
      tail_type tail( ) const
      {
        return tail_type( m1, m2 );
      }
      get_type0 first( )
      {
        return m0;
      }
      const_get_type0 first( ) const
      {
        return m0;
      }
      get_type2 last( )
      {
        return m2;
      }
      const_get_type2 last( ) const
      {
        return m2;
      }
      const_get_type0 get0( ) const
      {
        return m0;
      }
      const_get_type1 get1( ) const
      {
        return m1;
      }
      const_get_type2 get2( ) const
      {
        return m2;
      }
      get_type0 get0( )
      {
        return m0;
      }
      get_type1 get1( )
      {
        return m1;
      }
      get_type2 get2( )
      {
        return m2;
      }
      T0 m0;
      T1 m1;
      T2 m2;
    };
    template < class T0, class T1, class T2, class T3 >
    struct tuple4
    {
      typedef T0 type0;
      typedef T1 type1;
      typedef T2 type2;
      typedef T3 type3;
      typedef typename access_traits < T0 >::arg_type arg_type0;
      typedef typename access_traits < T1 >::arg_type arg_type1;
      typedef typename access_traits < T2 >::arg_type arg_type2;
      typedef typename access_traits < T3 >::arg_type arg_type3;
      typedef typename access_traits < T0 >::non_const_type get_type0;
      typedef typename access_traits < T1 >::non_const_type get_type1;
      typedef typename access_traits < T2 >::non_const_type get_type2;
      typedef typename access_traits < T3 >::non_const_type get_type3;
      typedef typename access_traits < T0 >::const_type const_get_type0;
      typedef typename access_traits < T1 >::const_type const_get_type1;
      typedef typename access_traits < T2 >::const_type const_get_type2;
      typedef typename access_traits < T3 >::const_type const_get_type3;
      typedef tuple4 < T0, T1, T2, T3 > self_type;
      typedef tuple3 < T1, T2, T3 > tail_type;
      tuple4( )
      {}
      tuple4( arg_type0 theM0, arg_type1 theM1, arg_type2 theM2,
              arg_type3 theM3 ) : m0( theM0 ), m1( theM1 ), m2( theM2 ),
          m3( theM3 )
      {}
      template < class S0, class S1, class S2,
      class S3 >
      tuple4( const tuple4 < S0, S1, S2,
              S3 > &rhs ) : m0( rhs.m0 ), m1( rhs.m1 ),
          m2( rhs.m2 ), m3( rhs.m3 )
      {}
      tuple4( arg_type0 theM0, const tail_type & tail ) : m0( theM0 ),
          m1( tail.m0 ), m2( tail.m1 ), m3( tail.m2 )
      {}
      template < class S0, class S1, class S2,
      class S3 >
      self_type & operator=( const tuple4 < S0, S1, S2,
                             S3 > &rhs )
      {
        if ( this != ( self_type * ) ( &rhs ) )
        {
          m0 = rhs.m0;
          m1 = rhs.m1;
          m2 = rhs.m2;
          m3 = rhs.m3;
        }
        return *this;
      }
      int size( ) const
      {
        return 4;
      }
      void swap( self_type & rhs )
      {
        std::swap( m0, rhs.m0 );
        std::swap( m1, rhs.m1 );
        std::swap( m2, rhs.m2 );
        std::swap( m3, rhs.m3 );
      }
      get_type0 head( )
      {
        return m0;
      }
      const_get_type0 head( ) const
      {
        return m0;
      }
      tail_type tail( ) const
      {
        return tail_type( m1, m2, m3 );
      }
      get_type0 first( )
      {
        return m0;
      }
      const_get_type0 first( ) const
      {
        return m0;
      }
      get_type3 last( )
      {
        return m3;
      }
      const_get_type3 last( ) const
      {
        return m3;
      }
      const_get_type0 get0( ) const
      {
        return m0;
      }
      const_get_type1 get1( ) const
      {
        return m1;
      }
      const_get_type2 get2( ) const
      {
        return m2;
      }
      const_get_type3 get3( ) const
      {
        return m3;
      }
      get_type0 get0( )
      {
        return m0;
      }
      get_type1 get1( )
      {
        return m1;
      }
      get_type2 get2( )
      {
        return m2;
      }
      get_type3 get3( )
      {
        return m3;
      }
      T0 m0;
      T1 m1;
      T2 m2;
      T3 m3;
    };
    template < class T0, class T1, class T2, class T3, class T4 >
    struct tuple5
    {
      typedef T0 type0;
      typedef T1 type1;
      typedef T2 type2;
      typedef T3 type3;
      typedef T4 type4;
      typedef typename access_traits < T0 >::arg_type arg_type0;
      typedef typename access_traits < T1 >::arg_type arg_type1;
      typedef typename access_traits < T2 >::arg_type arg_type2;
      typedef typename access_traits < T3 >::arg_type arg_type3;
      typedef typename access_traits < T4 >::arg_type arg_type4;
      typedef typename access_traits < T0 >::non_const_type get_type0;
      typedef typename access_traits < T1 >::non_const_type get_type1;
      typedef typename access_traits < T2 >::non_const_type get_type2;
      typedef typename access_traits < T3 >::non_const_type get_type3;
      typedef typename access_traits < T4 >::non_const_type get_type4;
      typedef typename access_traits < T0 >::const_type const_get_type0;
      typedef typename access_traits < T1 >::const_type const_get_type1;
      typedef typename access_traits < T2 >::const_type const_get_type2;
      typedef typename access_traits < T3 >::const_type const_get_type3;
      typedef typename access_traits < T4 >::const_type const_get_type4;
      typedef tuple5 < T0, T1, T2, T3, T4 > self_type;
      typedef tuple4 < T1, T2, T3, T4 > tail_type;
      tuple5( )
      {}
      tuple5( arg_type0 theM0, arg_type1 theM1, arg_type2 theM2,
              arg_type3 theM3, arg_type4 theM4 ) : m0( theM0 ), m1( theM1 ),
          m2( theM2 ), m3( theM3 ), m4( theM4 )
      {}
      template < class S0, class S1, class S2, class S3,
      class S4 >
      tuple5( const tuple5 < S0, S1, S2, S3,
              S4 > &rhs ) : m0( rhs.m0 ), m1( rhs.m1 ),
          m2( rhs.m2 ), m3( rhs.m3 ), m4( rhs.m4 )
      {}
      tuple5( arg_type0 theM0, const tail_type & tail ) : m0( theM0 ),
          m1( tail.m0 ), m2( tail.m1 ), m3( tail.m2 ), m4( tail.m3 )
      {}
      template < class S0, class S1, class S2, class S3,
      class S4 >
      self_type & operator=( const tuple5 < S0, S1, S2, S3,
                             S4 > &rhs )
      {
        if ( this != ( self_type * ) ( &rhs ) )
        {
          m0 = rhs.m0;
          m1 = rhs.m1;
          m2 = rhs.m2;
          m3 = rhs.m3;
          m4 = rhs.m4;
        }
        return *this;
      }
      int size( ) const
      {
        return 5;
      }
      void swap( self_type & rhs )
      {
        std::swap( m0, rhs.m0 );
        std::swap( m1, rhs.m1 );
        std::swap( m2, rhs.m2 );
        std::swap( m3, rhs.m3 );
        std::swap( m4, rhs.m4 );
      }
      get_type0 head( )
      {
        return m0;
      }
      const_get_type0 head( ) const
      {
        return m0;
      }
      tail_type tail( ) const
      {
        return tail_type( m1, m2, m3, m4 );
      }
      get_type0 first( )
      {
        return m0;
      }
      const_get_type0 first( ) const
      {
        return m0;
      }
      get_type4 last( )
      {
        return m4;
      }
      const_get_type4 last( ) const
      {
        return m4;
      }
      const_get_type0 get0( ) const
      {
        return m0;
      }
      const_get_type1 get1( ) const
      {
        return m1;
      }
      const_get_type2 get2( ) const
      {
        return m2;
      }
      const_get_type3 get3( ) const
      {
        return m3;
      }
      const_get_type4 get4( ) const
      {
        return m4;
      }
      get_type0 get0( )
      {
        return m0;
      }
      get_type1 get1( )
      {
        return m1;
      }
      get_type2 get2( )
      {
        return m2;
      }
      get_type3 get3( )
      {
        return m3;
      }
      get_type4 get4( )
      {
        return m4;
      }
      T0 m0;
      T1 m1;
      T2 m2;
      T3 m3;
      T4 m4;
    };
    template < class T0, class T1, class T2, class T3, class T4, class T5 >
    struct tuple6
    {
      typedef T0 type0;
      typedef T1 type1;
      typedef T2 type2;
      typedef T3 type3;
      typedef T4 type4;
      typedef T5 type5;
      typedef typename access_traits < T0 >::arg_type arg_type0;
      typedef typename access_traits < T1 >::arg_type arg_type1;
      typedef typename access_traits < T2 >::arg_type arg_type2;
      typedef typename access_traits < T3 >::arg_type arg_type3;
      typedef typename access_traits < T4 >::arg_type arg_type4;
      typedef typename access_traits < T5 >::arg_type arg_type5;
      typedef typename access_traits < T0 >::non_const_type get_type0;
      typedef typename access_traits < T1 >::non_const_type get_type1;
      typedef typename access_traits < T2 >::non_const_type get_type2;
      typedef typename access_traits < T3 >::non_const_type get_type3;
      typedef typename access_traits < T4 >::non_const_type get_type4;
      typedef typename access_traits < T5 >::non_const_type get_type5;
      typedef typename access_traits < T0 >::const_type const_get_type0;
      typedef typename access_traits < T1 >::const_type const_get_type1;
      typedef typename access_traits < T2 >::const_type const_get_type2;
      typedef typename access_traits < T3 >::const_type const_get_type3;
      typedef typename access_traits < T4 >::const_type const_get_type4;
      typedef typename access_traits < T5 >::const_type const_get_type5;
      typedef tuple6 < T0, T1, T2, T3, T4, T5 > self_type;
      typedef tuple5 < T1, T2, T3, T4, T5 > tail_type;
      tuple6( )
      {}
      tuple6( arg_type0 theM0, arg_type1 theM1, arg_type2 theM2,
              arg_type3 theM3, arg_type4 theM4,
              arg_type5 theM5 ) : m0( theM0 ), m1( theM1 ), m2( theM2 ),
          m3( theM3 ), m4( theM4 ), m5( theM5 )
      {}
      template < class S0, class S1, class S2, class S3, class S4,
      class S5 >
      tuple6( const tuple6 < S0, S1, S2, S3, S4,
              S5 > &rhs ) : m0( rhs.m0 ), m1( rhs.m1 ),
          m2( rhs.m2 ), m3( rhs.m3 ), m4( rhs.m4 ), m5( rhs.m5 )
      {}
      tuple6( arg_type0 theM0, const tail_type & tail ) : m0( theM0 ),
          m1( tail.m0 ), m2( tail.m1 ), m3( tail.m2 ), m4( tail.m3 ),
          m5( tail.m4 )
      {}
      template < class S0, class S1, class S2, class S3, class S4,
      class S5 >
      self_type & operator=( const tuple6 < S0, S1, S2, S3, S4,
                             S5 > &rhs )
      {
        if ( this != ( self_type * ) ( &rhs ) )
        {
          m0 = rhs.m0;
          m1 = rhs.m1;
          m2 = rhs.m2;
          m3 = rhs.m3;
          m4 = rhs.m4;
          m5 = rhs.m5;
        }
        return *this;
      }
      int size( ) const
      {
        return 6;
      }
      void swap( self_type & rhs )
      {
        std::swap( m0, rhs.m0 );
        std::swap( m1, rhs.m1 );
        std::swap( m2, rhs.m2 );
        std::swap( m3, rhs.m3 );
        std::swap( m4, rhs.m4 );
        std::swap( m5, rhs.m5 );
      }
      get_type0 head( )
      {
        return m0;
      }
      const_get_type0 head( ) const
      {
        return m0;
      }
      tail_type tail( ) const
      {
        return tail_type( m1, m2, m3, m4, m5 );
      }
      get_type0 first( )
      {
        return m0;
      }
      const_get_type0 first( ) const
      {
        return m0;
      }
      get_type5 last( )
      {
        return m5;
      }
      const_get_type5 last( ) const
      {
        return m5;
      }
      const_get_type0 get0( ) const
      {
        return m0;
      }
      const_get_type1 get1( ) const
      {
        return m1;
      }
      const_get_type2 get2( ) const
      {
        return m2;
      }
      const_get_type3 get3( ) const
      {
        return m3;
      }
      const_get_type4 get4( ) const
      {
        return m4;
      }
      const_get_type5 get5( ) const
      {
        return m5;
      }
      get_type0 get0( )
      {
        return m0;
      }
      get_type1 get1( )
      {
        return m1;
      }
      get_type2 get2( )
      {
        return m2;
      }
      get_type3 get3( )
      {
        return m3;
      }
      get_type4 get4( )
      {
        return m4;
      }
      get_type5 get5( )
      {
        return m5;
      }
      T0 m0;
      T1 m1;
      T2 m2;
      T3 m3;
      T4 m4;
      T5 m5;
    };
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6 >
    struct tuple7
    {
      typedef T0 type0;
      typedef T1 type1;
      typedef T2 type2;
      typedef T3 type3;
      typedef T4 type4;
      typedef T5 type5;
      typedef T6 type6;
      typedef typename access_traits < T0 >::arg_type arg_type0;
      typedef typename access_traits < T1 >::arg_type arg_type1;
      typedef typename access_traits < T2 >::arg_type arg_type2;
      typedef typename access_traits < T3 >::arg_type arg_type3;
      typedef typename access_traits < T4 >::arg_type arg_type4;
      typedef typename access_traits < T5 >::arg_type arg_type5;
      typedef typename access_traits < T6 >::arg_type arg_type6;
      typedef typename access_traits < T0 >::non_const_type get_type0;
      typedef typename access_traits < T1 >::non_const_type get_type1;
      typedef typename access_traits < T2 >::non_const_type get_type2;
      typedef typename access_traits < T3 >::non_const_type get_type3;
      typedef typename access_traits < T4 >::non_const_type get_type4;
      typedef typename access_traits < T5 >::non_const_type get_type5;
      typedef typename access_traits < T6 >::non_const_type get_type6;
      typedef typename access_traits < T0 >::const_type const_get_type0;
      typedef typename access_traits < T1 >::const_type const_get_type1;
      typedef typename access_traits < T2 >::const_type const_get_type2;
      typedef typename access_traits < T3 >::const_type const_get_type3;
      typedef typename access_traits < T4 >::const_type const_get_type4;
      typedef typename access_traits < T5 >::const_type const_get_type5;
      typedef typename access_traits < T6 >::const_type const_get_type6;
      typedef tuple7 < T0, T1, T2, T3, T4, T5, T6 > self_type;
      typedef tuple6 < T1, T2, T3, T4, T5, T6 > tail_type;
      tuple7( )
      {}
      tuple7( arg_type0 theM0, arg_type1 theM1, arg_type2 theM2,
              arg_type3 theM3, arg_type4 theM4, arg_type5 theM5,
              arg_type6 theM6 ) : m0( theM0 ), m1( theM1 ), m2( theM2 ),
          m3( theM3 ), m4( theM4 ), m5( theM5 ), m6( theM6 )
      {}
      template < class S0, class S1, class S2, class S3, class S4, class S5,
      class S6 >
      tuple7( const tuple7 < S0, S1, S2, S3, S4, S5,
              S6 > &rhs ) : m0( rhs.m0 ), m1( rhs.m1 ),
          m2( rhs.m2 ), m3( rhs.m3 ), m4( rhs.m4 ), m5( rhs.m5 ), m6( rhs.m6 )
      {}
      tuple7( arg_type0 theM0, const tail_type & tail ) : m0( theM0 ),
          m1( tail.m0 ), m2( tail.m1 ), m3( tail.m2 ), m4( tail.m3 ),
          m5( tail.m4 ), m6( tail.m5 )
      {}
      template < class S0, class S1, class S2, class S3, class S4, class S5,
      class S6 >
      self_type & operator=( const tuple7 < S0, S1, S2, S3, S4,
                             S5, S6 > &rhs )
      {
        if ( this != ( self_type * ) ( &rhs ) )
        {
          m0 = rhs.m0;
          m1 = rhs.m1;
          m2 = rhs.m2;
          m3 = rhs.m3;
          m4 = rhs.m4;
          m5 = rhs.m5;
          m6 = rhs.m6;
        }
        return *this;
      }
      int size( ) const
      {
        return 7;
      }
      void swap( self_type & rhs )
      {
        std::swap( m0, rhs.m0 );
        std::swap( m1, rhs.m1 );
        std::swap( m2, rhs.m2 );
        std::swap( m3, rhs.m3 );
        std::swap( m4, rhs.m4 );
        std::swap( m5, rhs.m5 );
        std::swap( m6, rhs.m6 );
      }
      get_type0 head( )
      {
        return m0;
      }
      const_get_type0 head( ) const
      {
        return m0;
      }
      tail_type tail( ) const
      {
        return tail_type( m1, m2, m3, m4, m5, m6 );
      }
      get_type0 first( )
      {
        return m0;
      }
      const_get_type0 first( ) const
      {
        return m0;
      }
      get_type6 last( )
      {
        return m6;
      }
      const_get_type6 last( ) const
      {
        return m6;
      }
      const_get_type0 get0( ) const
      {
        return m0;
      }
      const_get_type1 get1( ) const
      {
        return m1;
      }
      const_get_type2 get2( ) const
      {
        return m2;
      }
      const_get_type3 get3( ) const
      {
        return m3;
      }
      const_get_type4 get4( ) const
      {
        return m4;
      }
      const_get_type5 get5( ) const
      {
        return m5;
      }
      const_get_type6 get6( ) const
      {
        return m6;
      }
      get_type0 get0( )
      {
        return m0;
      }
      get_type1 get1( )
      {
        return m1;
      }
      get_type2 get2( )
      {
        return m2;
      }
      get_type3 get3( )
      {
        return m3;
      }
      get_type4 get4( )
      {
        return m4;
      }
      get_type5 get5( )
      {
        return m5;
      }
      get_type6 get6( )
      {
        return m6;
      }
      T0 m0;
      T1 m1;
      T2 m2;
      T3 m3;
      T4 m4;
      T5 m5;
      T6 m6;
    };
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6, class T7 >
    struct tuple8
    {
      typedef T0 type0;
      typedef T1 type1;
      typedef T2 type2;
      typedef T3 type3;
      typedef T4 type4;
      typedef T5 type5;
      typedef T6 type6;
      typedef T7 type7;
      typedef typename access_traits < T0 >::arg_type arg_type0;
      typedef typename access_traits < T1 >::arg_type arg_type1;
      typedef typename access_traits < T2 >::arg_type arg_type2;
      typedef typename access_traits < T3 >::arg_type arg_type3;
      typedef typename access_traits < T4 >::arg_type arg_type4;
      typedef typename access_traits < T5 >::arg_type arg_type5;
      typedef typename access_traits < T6 >::arg_type arg_type6;
      typedef typename access_traits < T7 >::arg_type arg_type7;
      typedef typename access_traits < T0 >::non_const_type get_type0;
      typedef typename access_traits < T1 >::non_const_type get_type1;
      typedef typename access_traits < T2 >::non_const_type get_type2;
      typedef typename access_traits < T3 >::non_const_type get_type3;
      typedef typename access_traits < T4 >::non_const_type get_type4;
      typedef typename access_traits < T5 >::non_const_type get_type5;
      typedef typename access_traits < T6 >::non_const_type get_type6;
      typedef typename access_traits < T7 >::non_const_type get_type7;
      typedef typename access_traits < T0 >::const_type const_get_type0;
      typedef typename access_traits < T1 >::const_type const_get_type1;
      typedef typename access_traits < T2 >::const_type const_get_type2;
      typedef typename access_traits < T3 >::const_type const_get_type3;
      typedef typename access_traits < T4 >::const_type const_get_type4;
      typedef typename access_traits < T5 >::const_type const_get_type5;
      typedef typename access_traits < T6 >::const_type const_get_type6;
      typedef typename access_traits < T7 >::const_type const_get_type7;
      typedef tuple8 < T0, T1, T2, T3, T4, T5, T6, T7 > self_type;
      typedef tuple7 < T1, T2, T3, T4, T5, T6, T7 > tail_type;
      tuple8( )
      {}
      tuple8( arg_type0 theM0, arg_type1 theM1, arg_type2 theM2,
              arg_type3 theM3, arg_type4 theM4, arg_type5 theM5,
              arg_type6 theM6, arg_type7 theM7 ) : m0( theM0 ), m1( theM1 ),
          m2( theM2 ), m3( theM3 ), m4( theM4 ), m5( theM5 ), m6( theM6 ),
          m7( theM7 )
      {}
      template < class S0, class S1, class S2, class S3, class S4, class S5,
      class S6, class S7 >
      tuple8( const tuple8 < S0, S1, S2, S3, S4, S5,
              S6, S7 > &rhs ) : m0( rhs.m0 ),
          m1( rhs.m1 ), m2( rhs.m2 ), m3( rhs.m3 ), m4( rhs.m4 ), m5( rhs.m5 ),
          m6( rhs.m6 ), m7( rhs.m7 )
      {}
      tuple8( arg_type0 theM0, const tail_type & tail ) : m0( theM0 ),
          m1( tail.m0 ), m2( tail.m1 ), m3( tail.m2 ), m4( tail.m3 ),
          m5( tail.m4 ), m6( tail.m5 ), m7( tail.m6 )
      {}
      template < class S0, class S1, class S2, class S3, class S4, class S5,
      class S6, class S7 >
      self_type & operator=( const tuple8 < S0, S1, S2,
                             S3, S4, S5, S6,
                             S7 > &rhs )
      {
        if ( this != ( self_type * ) ( &rhs ) )
        {
          m0 = rhs.m0;
          m1 = rhs.m1;
          m2 = rhs.m2;
          m3 = rhs.m3;
          m4 = rhs.m4;
          m5 = rhs.m5;
          m6 = rhs.m6;
          m7 = rhs.m7;
        }
        return *this;
      }
      int size( ) const
      {
        return 8;
      }
      void swap( self_type & rhs )
      {
        std::swap( m0, rhs.m0 );
        std::swap( m1, rhs.m1 );
        std::swap( m2, rhs.m2 );
        std::swap( m3, rhs.m3 );
        std::swap( m4, rhs.m4 );
        std::swap( m5, rhs.m5 );
        std::swap( m6, rhs.m6 );
        std::swap( m7, rhs.m7 );
      }
      get_type0 head( )
      {
        return m0;
      }
      const_get_type0 head( ) const
      {
        return m0;
      }
      tail_type tail( ) const
      {
        return tail_type( m1, m2, m3, m4, m5, m6, m7 );
      }
      get_type0 first( )
      {
        return m0;
      }
      const_get_type0 first( ) const
      {
        return m0;
      }
      get_type7 last( )
      {
        return m7;
      }
      const_get_type7 last( ) const
      {
        return m7;
      }
      const_get_type0 get0( ) const
      {
        return m0;
      }
      const_get_type1 get1( ) const
      {
        return m1;
      }
      const_get_type2 get2( ) const
      {
        return m2;
      }
      const_get_type3 get3( ) const
      {
        return m3;
      }
      const_get_type4 get4( ) const
      {
        return m4;
      }
      const_get_type5 get5( ) const
      {
        return m5;
      }
      const_get_type6 get6( ) const
      {
        return m6;
      }
      const_get_type7 get7( ) const
      {
        return m7;
      }
      get_type0 get0( )
      {
        return m0;
      }
      get_type1 get1( )
      {
        return m1;
      }
      get_type2 get2( )
      {
        return m2;
      }
      get_type3 get3( )
      {
        return m3;
      }
      get_type4 get4( )
      {
        return m4;
      }
      get_type5 get5( )
      {
        return m5;
      }
      get_type6 get6( )
      {
        return m6;
      }
      get_type7 get7( )
      {
        return m7;
      }
      T0 m0;
      T1 m1;
      T2 m2;
      T3 m3;
      T4 m4;
      T5 m5;
      T6 m6;
      T7 m7;
    };
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6, class T7, class T8 >
    struct tuple9
    {
      typedef T0 type0;
      typedef T1 type1;
      typedef T2 type2;
      typedef T3 type3;
      typedef T4 type4;
      typedef T5 type5;
      typedef T6 type6;
      typedef T7 type7;
      typedef T8 type8;
      typedef typename access_traits < T0 >::arg_type arg_type0;
      typedef typename access_traits < T1 >::arg_type arg_type1;
      typedef typename access_traits < T2 >::arg_type arg_type2;
      typedef typename access_traits < T3 >::arg_type arg_type3;
      typedef typename access_traits < T4 >::arg_type arg_type4;
      typedef typename access_traits < T5 >::arg_type arg_type5;
      typedef typename access_traits < T6 >::arg_type arg_type6;
      typedef typename access_traits < T7 >::arg_type arg_type7;
      typedef typename access_traits < T8 >::arg_type arg_type8;
      typedef typename access_traits < T0 >::non_const_type get_type0;
      typedef typename access_traits < T1 >::non_const_type get_type1;
      typedef typename access_traits < T2 >::non_const_type get_type2;
      typedef typename access_traits < T3 >::non_const_type get_type3;
      typedef typename access_traits < T4 >::non_const_type get_type4;
      typedef typename access_traits < T5 >::non_const_type get_type5;
      typedef typename access_traits < T6 >::non_const_type get_type6;
      typedef typename access_traits < T7 >::non_const_type get_type7;
      typedef typename access_traits < T8 >::non_const_type get_type8;
      typedef typename access_traits < T0 >::const_type const_get_type0;
      typedef typename access_traits < T1 >::const_type const_get_type1;
      typedef typename access_traits < T2 >::const_type const_get_type2;
      typedef typename access_traits < T3 >::const_type const_get_type3;
      typedef typename access_traits < T4 >::const_type const_get_type4;
      typedef typename access_traits < T5 >::const_type const_get_type5;
      typedef typename access_traits < T6 >::const_type const_get_type6;
      typedef typename access_traits < T7 >::const_type const_get_type7;
      typedef typename access_traits < T8 >::const_type const_get_type8;
      typedef tuple9 < T0, T1, T2, T3, T4, T5, T6, T7, T8 > self_type;
      typedef tuple8 < T1, T2, T3, T4, T5, T6, T7, T8 > tail_type;
      tuple9( )
      {}
      tuple9( arg_type0 theM0, arg_type1 theM1, arg_type2 theM2,
              arg_type3 theM3, arg_type4 theM4, arg_type5 theM5,
              arg_type6 theM6, arg_type7 theM7,
              arg_type8 theM8 ) : m0( theM0 ), m1( theM1 ), m2( theM2 ),
          m3( theM3 ), m4( theM4 ), m5( theM5 ), m6( theM6 ), m7( theM7 ),
          m8( theM8 )
      {}
      template < class S0, class S1, class S2, class S3, class S4, class S5,
      class S6, class S7, class S8 >
      tuple9( const tuple9 < S0, S1, S2, S3,
              S4, S5, S6, S7,
              S8 > &rhs ) : m0( rhs.m0 ),
          m1( rhs.m1 ), m2( rhs.m2 ), m3( rhs.m3 ), m4( rhs.m4 ), m5( rhs.m5 ),
          m6( rhs.m6 ), m7( rhs.m7 ), m8( rhs.m8 )
      {}
      tuple9( arg_type0 theM0, const tail_type & tail ) : m0( theM0 ),
          m1( tail.m0 ), m2( tail.m1 ), m3( tail.m2 ), m4( tail.m3 ),
          m5( tail.m4 ), m6( tail.m5 ), m7( tail.m6 ), m8( tail.m7 )
      {}
      template < class S0, class S1, class S2, class S3, class S4, class S5,
      class S6, class S7,
      class S8 >
      self_type & operator=( const tuple9 < S0, S1, S2, S3, S4,
                             S5, S6, S7, S8 > &rhs )
      {
        if ( this != ( self_type * ) ( &rhs ) )
        {
          m0 = rhs.m0;
          m1 = rhs.m1;
          m2 = rhs.m2;
          m3 = rhs.m3;
          m4 = rhs.m4;
          m5 = rhs.m5;
          m6 = rhs.m6;
          m7 = rhs.m7;
          m8 = rhs.m8;
        }
        return *this;
      }
      int size( ) const
      {
        return 9;
      }
      void swap( self_type & rhs )
      {
        std::swap( m0, rhs.m0 );
        std::swap( m1, rhs.m1 );
        std::swap( m2, rhs.m2 );
        std::swap( m3, rhs.m3 );
        std::swap( m4, rhs.m4 );
        std::swap( m5, rhs.m5 );
        std::swap( m6, rhs.m6 );
        std::swap( m7, rhs.m7 );
        std::swap( m8, rhs.m8 );
      }
      get_type0 head( )
      {
        return m0;
      }
      const_get_type0 head( ) const
      {
        return m0;
      }
      tail_type tail( ) const
      {
        return tail_type( m1, m2, m3, m4, m5, m6, m7, m8 );
      }
      get_type0 first( )
      {
        return m0;
      }
      const_get_type0 first( ) const
      {
        return m0;
      }
      get_type8 last( )
      {
        return m8;
      }
      const_get_type8 last( ) const
      {
        return m8;
      }
      const_get_type0 get0( ) const
      {
        return m0;
      }
      const_get_type1 get1( ) const
      {
        return m1;
      }
      const_get_type2 get2( ) const
      {
        return m2;
      }
      const_get_type3 get3( ) const
      {
        return m3;
      }
      const_get_type4 get4( ) const
      {
        return m4;
      }
      const_get_type5 get5( ) const
      {
        return m5;
      }
      const_get_type6 get6( ) const
      {
        return m6;
      }
      const_get_type7 get7( ) const
      {
        return m7;
      }
      const_get_type8 get8( ) const
      {
        return m8;
      }
      get_type0 get0( )
      {
        return m0;
      }
      get_type1 get1( )
      {
        return m1;
      }
      get_type2 get2( )
      {
        return m2;
      }
      get_type3 get3( )
      {
        return m3;
      }
      get_type4 get4( )
      {
        return m4;
      }
      get_type5 get5( )
      {
        return m5;
      }
      get_type6 get6( )
      {
        return m6;
      }
      get_type7 get7( )
      {
        return m7;
      }
      get_type8 get8( )
      {
        return m8;
      }
      T0 m0;
      T1 m1;
      T2 m2;
      T3 m3;
      T4 m4;
      T5 m5;
      T6 m6;
      T7 m7;
      T8 m8;
    };
    namespace detail
    {
      template < class T >
      struct n_fold_helper
      {
        template < int N >
        struct select
          {}
        ;
        template <>
        struct select <0 >
        {
          typedef tuple0 < null_type > type;
        };
        template <>
        struct select <1 >
        {
          typedef tuple1 < T > type;
          static type make( const T & arg )
          {
            return type( arg );
          }
        };
        template <>
        struct select <2 >
        {
          typedef tuple2 < T, T > type;
          static type make( const T & arg )
          {
            return type( arg, arg );
          }
        };
        template <>
        struct select <3 >
        {
          typedef tuple3 < T, T, T > type;
          static type make( const T & arg )
          {
            return type( arg, arg, arg );
          }
        };
        template <>
        struct select <4 >
        {
          typedef tuple4 < T, T, T, T > type;
          static type make( const T & arg )
          {
            return type( arg, arg, arg, arg );
          }
        };
        template <>
        struct select <5 >
        {
          typedef tuple5 < T, T, T, T, T > type;
          static type make( const T & arg )
          {
            return type( arg, arg, arg, arg, arg );
          }
        };
        template <>
        struct select <6 >
        {
          typedef tuple6 < T, T, T, T, T, T > type;
          static type make( const T & arg )
          {
            return type( arg, arg, arg, arg, arg, arg );
          }
        };
        template <>
        struct select <7 >
        {
          typedef tuple7 < T, T, T, T, T, T, T > type;
          static type make( const T & arg )
          {
            return type( arg, arg, arg, arg, arg, arg, arg );
          }
        };
        template <>
        struct select <8 >
        {
          typedef tuple8 < T, T, T, T, T, T, T, T > type;
          static type make( const T & arg )
          {
            return type( arg, arg, arg, arg, arg, arg, arg, arg );
          }
        };
        template <>
        struct select <9 >
        {
          typedef tuple9 < T, T, T, T, T, T, T, T, T > type;
          static type make( const T & arg )
          {
            return type( arg, arg, arg, arg, arg, arg, arg, arg, arg );
          }
        };
      };
    }                           // namespace detail

    template < class T, int N >
    struct n_fold_tuple
    {
      typedef detail::n_fold_helper < T >::select < N >::type type;

      static type make( const T & arg )
      {
        return detail::n_fold_helper < T >::select < N >::make( arg );
      }
    };




    template < class TupleT >
    typename TupleT::get_type0 get0( TupleT & t )
    {
      return t.m0;
    }
    template < class TupleT >
    typename TupleT::const_get_type0 get0( const TupleT & t )
    {
      return t.m0;
    }
    template < class T0 >
    tuple1 < typename make_tuple_traits < T0 >::type >
    make_tuple( const T0 & theM0 )
    {
      return tuple1 < typename make_tuple_traits < T0 >::type > ( theM0 );
    }
    template < class T0 >
    struct make_tuple_function1
    {
      typedef tuple1 < typename make_tuple_traits < T0 >::type > result_type;
      result_type operator( ) ( const T0 & theM0 ) const
      {
        return result_type( theM0 );
      }
    };
    template < class T0 >
    tuple1 < detail::assign_to_pointee < T0 >
    > tie( T0 & theM0 )
    {
      return make_tuple( detail::assign_to_pointee < T0 > ( &theM0 ) );
    }
    template < class TupleT >
    typename TupleT::get_type1 get1( TupleT & t )
    {
      return t.m1;
    }
    template < class TupleT >
    typename TupleT::const_get_type1 get1( const TupleT & t )
    {
      return t.m1;
    }
    template < class T0,
    class T1 >
    tuple2 < typename make_tuple_traits < T0 >::type,
    typename make_tuple_traits < T1 >::type > make_tuple( const T0 & theM0,
        const T1 & theM1 )
    {
      return tuple2 < typename make_tuple_traits < T0 >::type,
             typename make_tuple_traits < T1 >::type > ( theM0, theM1 );
    }
    template < class T0, class T1 >
    struct make_tuple_function2
    {
      typedef tuple2 < typename make_tuple_traits < T0 >::type,
      typename make_tuple_traits < T1 >::type > result_type;
      result_type operator( ) ( const T0 & theM0, const T1 & theM1 ) const
      {
        return result_type( theM0, theM1 );
      }
    };
    template < class T0, class T1 >
    tuple2 < detail::assign_to_pointee < T0 >,
    detail::assign_to_pointee < T1 > > tie( T0 & theM0, T1 & theM1 )
    {
      return make_tuple( detail::assign_to_pointee < T0 > ( &theM0 ),
                         detail::assign_to_pointee < T1 > ( &theM1 ) );
    }
    template < class TupleT >
    typename TupleT::get_type2 get2( TupleT & t )
    {
      return t.m2;
    }
    template < class TupleT >
    typename TupleT::const_get_type2 get2( const TupleT & t )
    {
      return t.m2;
    }
    template < class T0, class T1,
    class T2 >
    tuple3 < typename make_tuple_traits < T0 >::type,
    typename make_tuple_traits < T1 >::type,
    typename make_tuple_traits < T2 >::type > make_tuple( const T0 & theM0,
        const T1 & theM1,
        const T2 & theM2 )
    {
      return tuple3 < typename make_tuple_traits < T0 >::type,
             typename make_tuple_traits < T1 >::type,
             typename make_tuple_traits < T2 >::type > ( theM0, theM1, theM2 );
    }
    template < class T0, class T1, class T2 >
    struct make_tuple_function3
    {
      typedef tuple3 < typename make_tuple_traits < T0 >::type,
      typename make_tuple_traits < T1 >::type,
      typename make_tuple_traits < T2 >::type > result_type;
      result_type operator( ) ( const T0 & theM0, const T1 & theM1,
                                const T2 & theM2 ) const
      {
        return result_type( theM0, theM1, theM2 );
      }
    };
    template < class T0, class T1,
    class T2 >
    tuple3 < detail::assign_to_pointee < T0 >,
    detail::assign_to_pointee < T1 >,
    detail::assign_to_pointee < T2 > > tie( T0 & theM0, T1 & theM1,
                                            T2 & theM2 )
    {
      return make_tuple( detail::assign_to_pointee < T0 > ( &theM0 ),
                         detail::assign_to_pointee < T1 > ( &theM1 ),
                         detail::assign_to_pointee < T2 > ( &theM2 ) );
    }
    template < class TupleT >
    typename TupleT::get_type3 get3( TupleT & t )
    {
      return t.m3;
    }
    template < class TupleT >
    typename TupleT::const_get_type3 get3( const TupleT & t )
    {
      return t.m3;
    }
    template < class T0, class T1, class T2,
    class T3 >
    tuple4 < typename make_tuple_traits < T0 >::type,
    typename make_tuple_traits < T1 >::type,
    typename make_tuple_traits < T2 >::type,
    typename make_tuple_traits < T3 >::type > make_tuple( const T0 & theM0,
        const T1 & theM1,
        const T2 & theM2,
        const T3 & theM3 )
    {
      return tuple4 < typename make_tuple_traits < T0 >::type,
             typename make_tuple_traits < T1 >::type,
             typename make_tuple_traits < T2 >::type,
             typename make_tuple_traits < T3 >::type > ( theM0, theM1, theM2,
                 theM3 );
    }
    template < class T0, class T1, class T2, class T3 >
    struct make_tuple_function4
    {
      typedef tuple4 < typename make_tuple_traits < T0 >::type,
      typename make_tuple_traits < T1 >::type,
      typename make_tuple_traits < T2 >::type,
      typename make_tuple_traits < T3 >::type > result_type;
      result_type operator( ) ( const T0 & theM0, const T1 & theM1,
                                const T2 & theM2, const T3 & theM3 ) const
      {
        return result_type( theM0, theM1, theM2, theM3 );
      }
    };
    template < class T0, class T1, class T2,
    class T3 >
    tuple4 < detail::assign_to_pointee < T0 >,
    detail::assign_to_pointee < T1 >, detail::assign_to_pointee < T2 >,
    detail::assign_to_pointee < T3 > > tie( T0 & theM0, T1 & theM1,
                                            T2 & theM2, T3 & theM3 )
    {
      return make_tuple( detail::assign_to_pointee < T0 > ( &theM0 ),
                         detail::assign_to_pointee < T1 > ( &theM1 ),
                         detail::assign_to_pointee < T2 > ( &theM2 ),
                         detail::assign_to_pointee < T3 > ( &theM3 ) );
    }
    template < class TupleT >
    typename TupleT::get_type4 get4( TupleT & t )
    {
      return t.m4;
    }
    template < class TupleT >
    typename TupleT::const_get_type4 get4( const TupleT & t )
    {
      return t.m4;
    }
    template < class T0, class T1, class T2, class T3,
    class T4 >
    tuple5 < typename make_tuple_traits < T0 >::type,
    typename make_tuple_traits < T1 >::type,
    typename make_tuple_traits < T2 >::type,
    typename make_tuple_traits < T3 >::type,
    typename make_tuple_traits < T4 >::type > make_tuple( const T0 & theM0,
        const T1 & theM1,
        const T2 & theM2,
        const T3 & theM3,
        const T4 & theM4 )
    {
      return tuple5 < typename make_tuple_traits < T0 >::type,
             typename make_tuple_traits < T1 >::type,
             typename make_tuple_traits < T2 >::type,
             typename make_tuple_traits < T3 >::type,
             typename make_tuple_traits < T4 >::type > ( theM0, theM1, theM2,
                 theM3, theM4 );
    }
    template < class T0, class T1, class T2, class T3, class T4 >
    struct make_tuple_function5
    {
      typedef tuple5 < typename make_tuple_traits < T0 >::type,
      typename make_tuple_traits < T1 >::type,
      typename make_tuple_traits < T2 >::type,
      typename make_tuple_traits < T3 >::type,
      typename make_tuple_traits < T4 >::type > result_type;
      result_type operator( ) ( const T0 & theM0, const T1 & theM1,
                                const T2 & theM2, const T3 & theM3,
                                const T4 & theM4 ) const
      {
        return result_type( theM0, theM1, theM2, theM3, theM4 );
      }
    };
    template < class T0, class T1, class T2, class T3,
    class T4 >
    tuple5 < detail::assign_to_pointee < T0 >,
    detail::assign_to_pointee < T1 >, detail::assign_to_pointee < T2 >,
    detail::assign_to_pointee < T3 >,
    detail::assign_to_pointee < T4 > > tie( T0 & theM0, T1 & theM1,
                                            T2 & theM2, T3 & theM3,
                                            T4 & theM4 )
    {
      return make_tuple( detail::assign_to_pointee < T0 > ( &theM0 ),
                         detail::assign_to_pointee < T1 > ( &theM1 ),
                         detail::assign_to_pointee < T2 > ( &theM2 ),
                         detail::assign_to_pointee < T3 > ( &theM3 ),
                         detail::assign_to_pointee < T4 > ( &theM4 ) );
    }
    template < class TupleT >
    typename TupleT::get_type5 get5( TupleT & t )
    {
      return t.m5;
    }
    template < class TupleT >
    typename TupleT::const_get_type5 get5( const TupleT & t )
    {
      return t.m5;
    }
    template < class T0, class T1, class T2, class T3, class T4,
    class T5 >
    tuple6 < typename make_tuple_traits < T0 >::type,
    typename make_tuple_traits < T1 >::type,
    typename make_tuple_traits < T2 >::type,
    typename make_tuple_traits < T3 >::type,
    typename make_tuple_traits < T4 >::type,
    typename make_tuple_traits < T5 >::type > make_tuple( const T0 & theM0,
        const T1 & theM1,
        const T2 & theM2,
        const T3 & theM3,
        const T4 & theM4,
        const T5 & theM5 )
    {
      return tuple6 < typename make_tuple_traits < T0 >::type,
             typename make_tuple_traits < T1 >::type,
             typename make_tuple_traits < T2 >::type,
             typename make_tuple_traits < T3 >::type,
             typename make_tuple_traits < T4 >::type,
             typename make_tuple_traits < T5 >::type > ( theM0, theM1, theM2,
                 theM3, theM4, theM5 );
    }
    template < class T0, class T1, class T2, class T3, class T4, class T5 >
    struct make_tuple_function6
    {
      typedef tuple6 < typename make_tuple_traits < T0 >::type,
      typename make_tuple_traits < T1 >::type,
      typename make_tuple_traits < T2 >::type,
      typename make_tuple_traits < T3 >::type,
      typename make_tuple_traits < T4 >::type,
      typename make_tuple_traits < T5 >::type > result_type;
      result_type operator( ) ( const T0 & theM0, const T1 & theM1,
                                const T2 & theM2, const T3 & theM3,
                                const T4 & theM4, const T5 & theM5 ) const
      {
        return result_type( theM0, theM1, theM2, theM3, theM4, theM5 );
      }
    };
    template < class T0, class T1, class T2, class T3, class T4,
    class T5 >
    tuple6 < detail::assign_to_pointee < T0 >,
    detail::assign_to_pointee < T1 >, detail::assign_to_pointee < T2 >,
    detail::assign_to_pointee < T3 >, detail::assign_to_pointee < T4 >,
    detail::assign_to_pointee < T5 > > tie( T0 & theM0, T1 & theM1,
                                            T2 & theM2, T3 & theM3,
                                            T4 & theM4, T5 & theM5 )
    {
      return make_tuple( detail::assign_to_pointee < T0 > ( &theM0 ),
                         detail::assign_to_pointee < T1 > ( &theM1 ),
                         detail::assign_to_pointee < T2 > ( &theM2 ),
                         detail::assign_to_pointee < T3 > ( &theM3 ),
                         detail::assign_to_pointee < T4 > ( &theM4 ),
                         detail::assign_to_pointee < T5 > ( &theM5 ) );
    }
    template < class TupleT >
    typename TupleT::get_type6 get6( TupleT & t )
    {
      return t.m6;
    }
    template < class TupleT >
    typename TupleT::const_get_type6 get6( const TupleT & t )
    {
      return t.m6;
    }
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6 >
    tuple7 < typename make_tuple_traits < T0 >::type,
    typename make_tuple_traits < T1 >::type,
    typename make_tuple_traits < T2 >::type,
    typename make_tuple_traits < T3 >::type,
    typename make_tuple_traits < T4 >::type,
    typename make_tuple_traits < T5 >::type,
    typename make_tuple_traits < T6 >::type > make_tuple( const T0 & theM0,
        const T1 & theM1,
        const T2 & theM2,
        const T3 & theM3,
        const T4 & theM4,
        const T5 & theM5,
        const T6 & theM6 )
    {
      return tuple7 < typename make_tuple_traits < T0 >::type,
             typename make_tuple_traits < T1 >::type,
             typename make_tuple_traits < T2 >::type,
             typename make_tuple_traits < T3 >::type,
             typename make_tuple_traits < T4 >::type,
             typename make_tuple_traits < T5 >::type,
             typename make_tuple_traits < T6 >::type > ( theM0, theM1, theM2,
                 theM3, theM4, theM5,
                 theM6 );
    }
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6 >
    struct make_tuple_function7
    {
      typedef tuple7 < typename make_tuple_traits < T0 >::type,
      typename make_tuple_traits < T1 >::type,
      typename make_tuple_traits < T2 >::type,
      typename make_tuple_traits < T3 >::type,
      typename make_tuple_traits < T4 >::type,
      typename make_tuple_traits < T5 >::type,
      typename make_tuple_traits < T6 >::type > result_type;
      result_type operator( ) ( const T0 & theM0, const T1 & theM1,
                                const T2 & theM2, const T3 & theM3,
                                const T4 & theM4, const T5 & theM5,
                                const T6 & theM6 ) const
      {
        return result_type( theM0, theM1, theM2, theM3, theM4, theM5, theM6 );
      }
    };
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6 >
    tuple7 < detail::assign_to_pointee < T0 >,
    detail::assign_to_pointee < T1 >, detail::assign_to_pointee < T2 >,
    detail::assign_to_pointee < T3 >, detail::assign_to_pointee < T4 >,
    detail::assign_to_pointee < T5 >,
    detail::assign_to_pointee < T6 > > tie( T0 & theM0, T1 & theM1,
                                            T2 & theM2, T3 & theM3,
                                            T4 & theM4, T5 & theM5,
                                            T6 & theM6 )
    {
      return make_tuple( detail::assign_to_pointee < T0 > ( &theM0 ),
                         detail::assign_to_pointee < T1 > ( &theM1 ),
                         detail::assign_to_pointee < T2 > ( &theM2 ),
                         detail::assign_to_pointee < T3 > ( &theM3 ),
                         detail::assign_to_pointee < T4 > ( &theM4 ),
                         detail::assign_to_pointee < T5 > ( &theM5 ),
                         detail::assign_to_pointee < T6 > ( &theM6 ) );
    }
    template < class TupleT >
    typename TupleT::get_type7 get7( TupleT & t )
    {
      return t.m7;
    }
    template < class TupleT >
    typename TupleT::const_get_type7 get7( const TupleT & t )
    {
      return t.m7;
    }
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6, class T7 >
    tuple8 < typename make_tuple_traits < T0 >::type,
    typename make_tuple_traits < T1 >::type,
    typename make_tuple_traits < T2 >::type,
    typename make_tuple_traits < T3 >::type,
    typename make_tuple_traits < T4 >::type,
    typename make_tuple_traits < T5 >::type,
    typename make_tuple_traits < T6 >::type,
    typename make_tuple_traits < T7 >::type > make_tuple( const T0 & theM0,
        const T1 & theM1,
        const T2 & theM2,
        const T3 & theM3,
        const T4 & theM4,
        const T5 & theM5,
        const T6 & theM6,
        const T7 & theM7 )
    {
      return tuple8 < typename make_tuple_traits < T0 >::type,
             typename make_tuple_traits < T1 >::type,
             typename make_tuple_traits < T2 >::type,
             typename make_tuple_traits < T3 >::type,
             typename make_tuple_traits < T4 >::type,
             typename make_tuple_traits < T5 >::type,
             typename make_tuple_traits < T6 >::type,
             typename make_tuple_traits < T7 >::type > ( theM0, theM1, theM2,
                 theM3, theM4, theM5,
                 theM6, theM7 );
    }
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6, class T7 >
    struct make_tuple_function8
    {
      typedef tuple8 < typename make_tuple_traits < T0 >::type,
      typename make_tuple_traits < T1 >::type,
      typename make_tuple_traits < T2 >::type,
      typename make_tuple_traits < T3 >::type,
      typename make_tuple_traits < T4 >::type,
      typename make_tuple_traits < T5 >::type,
      typename make_tuple_traits < T6 >::type,
      typename make_tuple_traits < T7 >::type > result_type;
      result_type operator( ) ( const T0 & theM0, const T1 & theM1,
                                const T2 & theM2, const T3 & theM3,
                                const T4 & theM4, const T5 & theM5,
                                const T6 & theM6, const T7 & theM7 ) const
      {
        return result_type( theM0, theM1, theM2, theM3, theM4, theM5, theM6,
                            theM7 );
      }
    };
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6, class T7 >
    tuple8 < detail::assign_to_pointee < T0 >,
    detail::assign_to_pointee < T1 >, detail::assign_to_pointee < T2 >,
    detail::assign_to_pointee < T3 >, detail::assign_to_pointee < T4 >,
    detail::assign_to_pointee < T5 >, detail::assign_to_pointee < T6 >,
    detail::assign_to_pointee < T7 > > tie( T0 & theM0, T1 & theM1,
                                            T2 & theM2, T3 & theM3,
                                            T4 & theM4, T5 & theM5,
                                            T6 & theM6, T7 & theM7 )
    {
      return make_tuple( detail::assign_to_pointee < T0 > ( &theM0 ),
                         detail::assign_to_pointee < T1 > ( &theM1 ),
                         detail::assign_to_pointee < T2 > ( &theM2 ),
                         detail::assign_to_pointee < T3 > ( &theM3 ),
                         detail::assign_to_pointee < T4 > ( &theM4 ),
                         detail::assign_to_pointee < T5 > ( &theM5 ),
                         detail::assign_to_pointee < T6 > ( &theM6 ),
                         detail::assign_to_pointee < T7 > ( &theM7 ) );
    }
    template < class TupleT >
    typename TupleT::get_type8 get8( TupleT & t )
    {
      return t.m8;
    }
    template < class TupleT >
    typename TupleT::const_get_type8 get8( const TupleT & t )
    {
      return t.m8;
    }
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6, class T7,
    class T8 >
    tuple9 < typename make_tuple_traits < T0 >::type,
    typename make_tuple_traits < T1 >::type,
    typename make_tuple_traits < T2 >::type,
    typename make_tuple_traits < T3 >::type,
    typename make_tuple_traits < T4 >::type,
    typename make_tuple_traits < T5 >::type,
    typename make_tuple_traits < T6 >::type,
    typename make_tuple_traits < T7 >::type,
    typename make_tuple_traits < T8 >::type > make_tuple( const T0 & theM0,
        const T1 & theM1,
        const T2 & theM2,
        const T3 & theM3,
        const T4 & theM4,
        const T5 & theM5,
        const T6 & theM6,
        const T7 & theM7,
        const T8 & theM8 )
    {
      return tuple9 < typename make_tuple_traits < T0 >::type,
             typename make_tuple_traits < T1 >::type,
             typename make_tuple_traits < T2 >::type,
             typename make_tuple_traits < T3 >::type,
             typename make_tuple_traits < T4 >::type,
             typename make_tuple_traits < T5 >::type,
             typename make_tuple_traits < T6 >::type,
             typename make_tuple_traits < T7 >::type,
             typename make_tuple_traits < T8 >::type > ( theM0, theM1, theM2,
                 theM3, theM4, theM5,
                 theM6, theM7, theM8 );
    }
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6, class T7, class T8 >
    struct make_tuple_function9
    {
      typedef tuple9 < typename make_tuple_traits < T0 >::type,
      typename make_tuple_traits < T1 >::type,
      typename make_tuple_traits < T2 >::type,
      typename make_tuple_traits < T3 >::type,
      typename make_tuple_traits < T4 >::type,
      typename make_tuple_traits < T5 >::type,
      typename make_tuple_traits < T6 >::type,
      typename make_tuple_traits < T7 >::type,
      typename make_tuple_traits < T8 >::type > result_type;
      result_type operator( ) ( const T0 & theM0, const T1 & theM1,
                                const T2 & theM2, const T3 & theM3,
                                const T4 & theM4, const T5 & theM5,
                                const T6 & theM6, const T7 & theM7,
                                const T8 & theM8 ) const
      {
        return result_type( theM0, theM1, theM2, theM3, theM4, theM5, theM6,
                            theM7, theM8 );
      }
    };
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6, class T7,
    class T8 >
    tuple9 < detail::assign_to_pointee < T0 >,
    detail::assign_to_pointee < T1 >, detail::assign_to_pointee < T2 >,
    detail::assign_to_pointee < T3 >, detail::assign_to_pointee < T4 >,
    detail::assign_to_pointee < T5 >, detail::assign_to_pointee < T6 >,
    detail::assign_to_pointee < T7 >,
    detail::assign_to_pointee < T8 > > tie( T0 & theM0, T1 & theM1,
                                            T2 & theM2, T3 & theM3,
                                            T4 & theM4, T5 & theM5,
                                            T6 & theM6, T7 & theM7,
                                            T8 & theM8 )
    {
      return make_tuple( detail::assign_to_pointee < T0 > ( &theM0 ),
                         detail::assign_to_pointee < T1 > ( &theM1 ),
                         detail::assign_to_pointee < T2 > ( &theM2 ),
                         detail::assign_to_pointee < T3 > ( &theM3 ),
                         detail::assign_to_pointee < T4 > ( &theM4 ),
                         detail::assign_to_pointee < T5 > ( &theM5 ),
                         detail::assign_to_pointee < T6 > ( &theM6 ),
                         detail::assign_to_pointee < T7 > ( &theM7 ),
                         detail::assign_to_pointee < T8 > ( &theM8 ) );
    }
    template < class F, class R, class T0 >
    struct function_object1
    {
      typedef tuple1 < T0 > argument_type;
      typedef R result_type;
      function_object1( )
      {}
      function_object1( const F & theF ) : f( theF )
      {}
      R operator( ) ( const tuple1 < T0 > &arg )
      {
        return f( arg.m0 );
      }
protected:
      F f;
    };
    template < class R, class T0 >
    struct pointer_to_function1
    {
      typedef tuple1 < T0 > argument_type;
      typedef R result_type;
      pointer_to_function1( )
      {}
      pointer_to_function1( R( *theF ) ( T0 ) ) : f( theF )
      {}
      R operator( ) ( const tuple1 < T0 > &arg )
      {
        return f( arg.m0 );
      }
protected:
      R( *f ) ( T0 );
    };
    template < class R, class T0 >
    inline pointer_to_function1 < R,
    T0 > ptr_fct( R( *theF ) ( T0 ) )
    {
      return pointer_to_function1 < R, T0 > ( theF );
    }
    template < class F, class R, class T0, class T1 >
    struct function_object2
    {
      typedef tuple2 < T0, T1 > argument_type;
      typedef R result_type;
      function_object2( )
      {}
      function_object2( const F & theF ) : f( theF )
      {}
      R operator( ) ( const tuple2 < T0, T1 > &arg )
      {
        return f( arg.m0, arg.m1 );
      }
protected:
      F f;
    };
    template < class R, class T0, class T1 >
    struct pointer_to_function2
    {
      typedef tuple2 < T0, T1 > argument_type;
      typedef R result_type;
      pointer_to_function2( )
      {}
      pointer_to_function2( R( *theF ) ( T0, T1 ) ) : f( theF )
      {}
      R operator( ) ( const tuple2 < T0, T1 > &arg )
      {
        return f( arg.m0, arg.m1 );
      }
protected:
      R( *f ) ( T0, T1 );
    };
    template < class R, class T0, class T1 >
    inline pointer_to_function2 < R,
    T0, T1 > ptr_fct( R( *theF ) ( T0, T1 ) )
    {
      return pointer_to_function2 < R, T0, T1 > ( theF );
    }
    template < class F, class R, class T0, class T1, class T2 >
    struct function_object3
    {
      typedef tuple3 < T0, T1, T2 > argument_type;
      typedef R result_type;
      function_object3( )
      {}
      function_object3( const F & theF ) : f( theF )
      {}
      R operator( ) ( const tuple3 < T0, T1, T2 > &arg )
      {
        return f( arg.m0, arg.m1, arg.m2 );
      }
protected:
      F f;
    };
    template < class R, class T0, class T1, class T2 >
    struct pointer_to_function3
    {
      typedef tuple3 < T0, T1, T2 > argument_type;
      typedef R result_type;
      pointer_to_function3( )
      {}
      pointer_to_function3( R( *theF ) ( T0, T1, T2 ) ) : f( theF )
      {}
      R operator( ) ( const tuple3 < T0, T1, T2 > &arg )
      {
        return f( arg.m0, arg.m1, arg.m2 );
      }
protected:
      R( *f ) ( T0, T1, T2 );
    };
    template < class R, class T0, class T1,
    class T2 >
    inline pointer_to_function3 < R, T0, T1,
    T2 > ptr_fct( R( *theF ) ( T0, T1, T2 ) )
    {
      return pointer_to_function3 < R, T0, T1, T2 > ( theF );
    }
    template < class F, class R, class T0, class T1, class T2, class T3 >
    struct function_object4
    {
      typedef tuple4 < T0, T1, T2, T3 > argument_type;
      typedef R result_type;
      function_object4( )
      {}
      function_object4( const F & theF ) : f( theF )
      {}
      R operator( ) ( const tuple4 < T0, T1, T2, T3 > &arg )
      {
        return f( arg.m0, arg.m1, arg.m2, arg.m3 );
      }
protected:
      F f;
    };
    template < class R, class T0, class T1, class T2, class T3 >
    struct pointer_to_function4
    {
      typedef tuple4 < T0, T1, T2, T3 > argument_type;
      typedef R result_type;
      pointer_to_function4( )
      {}
      pointer_to_function4( R( *theF ) ( T0, T1, T2, T3 ) ) : f( theF )
      {}
      R operator( ) ( const tuple4 < T0, T1, T2, T3 > &arg )
      {
        return f( arg.m0, arg.m1, arg.m2, arg.m3 );
      }
protected:
      R( *f ) ( T0, T1, T2, T3 );
    };
    template < class R, class T0, class T1, class T2,
    class T3 >
    inline pointer_to_function4 < R, T0, T1, T2,
    T3 > ptr_fct( R( *theF ) ( T0, T1, T2, T3 ) )
    {
      return pointer_to_function4 < R, T0, T1, T2, T3 > ( theF );
    }
    template < class F, class R, class T0, class T1, class T2, class T3,
    class T4 >
    struct function_object5
    {
      typedef tuple5 < T0, T1, T2, T3, T4 > argument_type;
      typedef R result_type;
      function_object5( )
      {}
      function_object5( const F & theF ) : f( theF )
      {}
      R operator( ) ( const tuple5 < T0, T1, T2, T3, T4 > &arg )
      {
        return f( arg.m0, arg.m1, arg.m2, arg.m3, arg.m4 );
      }
protected:
      F f;
    };
    template < class R, class T0, class T1, class T2, class T3, class T4 >
    struct pointer_to_function5
    {
      typedef tuple5 < T0, T1, T2, T3, T4 > argument_type;
      typedef R result_type;
      pointer_to_function5( )
      {}
      pointer_to_function5( R( *theF ) ( T0, T1, T2, T3, T4 ) ) : f( theF )
      {}
      R operator( ) ( const tuple5 < T0, T1, T2, T3, T4 > &arg )
      {
        return f( arg.m0, arg.m1, arg.m2, arg.m3, arg.m4 );
      }
protected:
      R( *f ) ( T0, T1, T2, T3, T4 );
    };
    template < class R, class T0, class T1, class T2, class T3,
    class T4 >
    inline pointer_to_function5 < R, T0, T1, T2, T3,
    T4 > ptr_fct( R( *theF ) ( T0, T1, T2, T3, T4 ) )
    {
      return pointer_to_function5 < R, T0, T1, T2, T3, T4 > ( theF );
    }
    template < class F, class R, class T0, class T1, class T2, class T3,
    class T4, class T5 >
    struct function_object6
    {
      typedef tuple6 < T0, T1, T2, T3, T4, T5 > argument_type;
      typedef R result_type;
      function_object6( )
      {}
      function_object6( const F & theF ) : f( theF )
      {}
      R operator( ) ( const tuple6 < T0, T1, T2, T3, T4, T5 > &arg )
      {
        return f( arg.m0, arg.m1, arg.m2, arg.m3, arg.m4, arg.m5 );
      }
protected:
      F f;
    };
    template < class R, class T0, class T1, class T2, class T3, class T4,
    class T5 >
    struct pointer_to_function6
    {
      typedef tuple6 < T0, T1, T2, T3, T4, T5 > argument_type;
      typedef R result_type;
      pointer_to_function6( )
      {}
      pointer_to_function6( R( *theF ) ( T0, T1, T2, T3, T4, T5 ) ) : f
          ( theF )
      {}
      R operator( ) ( const tuple6 < T0, T1, T2, T3, T4, T5 > &arg )
      {
        return f( arg.m0, arg.m1, arg.m2, arg.m3, arg.m4, arg.m5 );
      }
protected:
      R( *f ) ( T0, T1, T2, T3, T4, T5 );
    };
    template < class R, class T0, class T1, class T2, class T3, class T4,
    class T5 >
    inline pointer_to_function6 < R, T0, T1, T2, T3, T4,
    T5 > ptr_fct( R( *theF ) ( T0, T1, T2, T3, T4, T5 ) )
    {
      return pointer_to_function6 < R, T0, T1, T2, T3, T4, T5 > ( theF );
    }
    template < class F, class R, class T0, class T1, class T2, class T3,
    class T4, class T5, class T6 >
    struct function_object7
    {
      typedef tuple7 < T0, T1, T2, T3, T4, T5, T6 > argument_type;
      typedef R result_type;
      function_object7( )
      {}
      function_object7( const F & theF ) : f( theF )
      {}
      R operator( ) ( const tuple7 < T0, T1, T2, T3, T4, T5, T6 > &arg )
      {
        return f( arg.m0, arg.m1, arg.m2, arg.m3, arg.m4, arg.m5, arg.m6 );
      }
protected:
      F f;
    };
    template < class R, class T0, class T1, class T2, class T3, class T4,
    class T5, class T6 >
    struct pointer_to_function7
    {
      typedef tuple7 < T0, T1, T2, T3, T4, T5, T6 > argument_type;
      typedef R result_type;
      pointer_to_function7( )
      {}
      pointer_to_function7( R( *theF ) ( T0, T1, T2, T3, T4, T5, T6 ) ) : f
          ( theF )
      {}
      R operator( ) ( const tuple7 < T0, T1, T2, T3, T4, T5, T6 > &arg )
      {
        return f( arg.m0, arg.m1, arg.m2, arg.m3, arg.m4, arg.m5, arg.m6 );
      }
protected:
      R( *f ) ( T0, T1, T2, T3, T4, T5, T6 );
    };
    template < class R, class T0, class T1, class T2, class T3, class T4,
    class T5, class T6 >
    inline pointer_to_function7 < R, T0, T1, T2, T3,
    T4, T5, T6 > ptr_fct( R( *theF ) ( T0, T1, T2, T3, T4, T5, T6 ) )
    {
      return pointer_to_function7 < R, T0, T1, T2, T3, T4, T5, T6 > ( theF );
    }
    template < class F, class R, class T0, class T1, class T2, class T3,
    class T4, class T5, class T6, class T7 >
    struct function_object8
    {
      typedef tuple8 < T0, T1, T2, T3, T4, T5, T6, T7 > argument_type;
      typedef R result_type;
      function_object8( )
      {}
      function_object8( const F & theF ) : f( theF )
      {}
      R operator( ) ( const tuple8 < T0, T1, T2, T3, T4, T5, T6,
                      T7 > &arg )
      {
        return f( arg.m0, arg.m1, arg.m2, arg.m3, arg.m4, arg.m5, arg.m6,
                  arg.m7 );
      }
protected:
      F f;
    };
    template < class R, class T0, class T1, class T2, class T3, class T4,
    class T5, class T6, class T7 >
    struct pointer_to_function8
    {
      typedef tuple8 < T0, T1, T2, T3, T4, T5, T6, T7 > argument_type;
      typedef R result_type;
      pointer_to_function8( )
      {}
      pointer_to_function8( R( *theF )
                            ( T0, T1, T2, T3, T4, T5, T6, T7 ) ) : f( theF )
      {}
      R operator( ) ( const tuple8 < T0, T1, T2, T3, T4, T5, T6,
                      T7 > &arg )
      {
        return f( arg.m0, arg.m1, arg.m2, arg.m3, arg.m4, arg.m5, arg.m6,
                  arg.m7 );
      }
protected:
      R( *f ) ( T0, T1, T2, T3, T4, T5, T6, T7 );
    };
    template < class R, class T0, class T1, class T2, class T3, class T4,
    class T5, class T6, class T7 >
    inline pointer_to_function8 < R, T0, T1,
    T2, T3, T4, T5, T6,
    T7 > ptr_fct( R( *theF ) ( T0, T1, T2, T3, T4, T5, T6, T7 ) )
    {
      return pointer_to_function8 < R, T0, T1, T2, T3, T4, T5, T6,
             T7 > ( theF );
    }
    template < class F, class R, class T0, class T1, class T2, class T3,
    class T4, class T5, class T6, class T7,
    class T8 >
    struct function_object9
    {
      typedef tuple9 < T0, T1, T2, T3, T4, T5, T6, T7, T8 > argument_type;
      typedef R result_type;
      function_object9( )
      {}
      function_object9( const F & theF ) : f( theF )
      {}
      R operator( ) ( const tuple9 < T0, T1, T2, T3, T4, T5, T6, T7,
                      T8 > &arg )
      {
        return f( arg.m0, arg.m1, arg.m2, arg.m3, arg.m4, arg.m5, arg.m6,
                  arg.m7, arg.m8 );
      }
protected:
      F f;
    };
    template < class R, class T0, class T1, class T2, class T3, class T4,
    class T5, class T6, class T7, class T8 >
    struct pointer_to_function9
    {
      typedef tuple9 < T0, T1, T2, T3, T4, T5, T6, T7, T8 > argument_type;
      typedef R result_type;
      pointer_to_function9( )
      {}
      pointer_to_function9( R( *theF )
                            ( T0, T1, T2, T3, T4, T5, T6, T7,
                              T8 ) ) : f( theF )
      {}
      R operator( ) ( const tuple9 < T0, T1, T2, T3, T4, T5, T6, T7,
                      T8 > &arg )
      {
        return f( arg.m0, arg.m1, arg.m2, arg.m3, arg.m4, arg.m5, arg.m6,
                  arg.m7, arg.m8 );
      }
protected:
      R( *f ) ( T0, T1, T2, T3, T4, T5, T6, T7, T8 );
    };
    template < class R, class T0, class T1, class T2, class T3, class T4,
    class T5, class T6, class T7,
    class T8 >
    inline pointer_to_function9 < R, T0, T1, T2, T3, T4, T5, T6,
    T7, T8 > ptr_fct( R( *theF ) ( T0, T1, T2, T3, T4, T5, T6, T7, T8 ) )
    {
      return pointer_to_function9 < R, T0, T1, T2, T3, T4, T5, T6, T7,
             T8 > ( theF );
    }
    inline bool operator==( const tuple0 < null_type > &lhs,
                            const tuple0 < null_type > &rhs )
    {
      return true;
    }
    inline bool operator<( const tuple0 < null_type > &lhs,
                           const tuple0 < null_type > &rhs )
    {
      return false;
    }
    template < class T0, class S0 >
    bool operator==( const tuple1 < T0 > &lhs,
                     const tuple1 < S0 >
                     &rhs )
    {
      return ( lhs.m0 == rhs.m0 );
    }
    template < class T0, class S0 >
    bool operator<( const tuple1 < T0 > &lhs,
                    const tuple1 < S0 > &rhs )
    {
      return ( lhs.head( ) < rhs.head( )
               || ( !( rhs.head( ) < lhs.head( ) )
                    && lhs.tail( ) < rhs.tail( ) ) );
    }
    template < class T0, class S0 >
    bool operator<=( const tuple1 < T0 > &lhs,
                     const tuple1 < S0 >
                     &rhs )
    {
      return !( rhs < lhs );
    }
    template < class T0, class S0 >
    bool operator>( const tuple1 < T0 > &lhs,
                    const tuple1 < S0 > &rhs )
    {
      return ( rhs < lhs );
    }
    template < class T0, class S0 >
    bool operator>=( const tuple1 < T0 > &lhs,
                     const tuple1 < S0 >
                     &rhs )
    {
      return !( lhs < rhs );
    }
    template < class T0, class T1, class S0,
    class S1 >
    bool operator==( const tuple2 < T0, T1 > &lhs,
                     const tuple2 < S0, S1 > &rhs )
    {
      return ( lhs.m0 == rhs.m0 && lhs.m1 == rhs.m1 );
    }
    template < class T0, class T1, class S0,
    class S1 >
    bool operator<( const tuple2 < T0, T1 > &lhs,
                    const tuple2 < S0, S1 > &rhs )
    {
      return ( lhs.head( ) < rhs.head( )
               || ( !( rhs.head( ) < lhs.head( ) )
                    && lhs.tail( ) < rhs.tail( ) ) );
    }
    template < class T0, class T1, class S0,
    class S1 >
    bool operator<=( const tuple2 < T0, T1 > &lhs,
                     const tuple2 < S0, S1 > &rhs )
    {
      return !( rhs < lhs );
    }
    template < class T0, class T1, class S0,
    class S1 >
    bool operator>( const tuple2 < T0, T1 > &lhs,
                    const tuple2 < S0, S1 > &rhs )
    {
      return ( rhs < lhs );
    }
    template < class T0, class T1, class S0,
    class S1 >
    bool operator>=( const tuple2 < T0, T1 > &lhs,
                     const tuple2 < S0, S1 > &rhs )
    {
      return !( lhs < rhs );
    }
    template < class T0, class T1, class T2, class S0, class S1,
    class S2 >
    bool operator==( const tuple3 < T0, T1, T2 > &lhs,
                     const tuple3 < S0, S1, S2 > &rhs )
    {
      return ( lhs.m0 == rhs.m0 && lhs.m1 == rhs.m1 && lhs.m2 == rhs.m2 );
    }
    template < class T0, class T1, class T2, class S0, class S1,
    class S2 >
    bool operator<( const tuple3 < T0, T1, T2 > &lhs,
                    const tuple3 < S0, S1, S2 > &rhs )
    {
      return ( lhs.head( ) < rhs.head( )
               || ( !( rhs.head( ) < lhs.head( ) )
                    && lhs.tail( ) < rhs.tail( ) ) );
    }
    template < class T0, class T1, class T2, class S0, class S1,
    class S2 >
    bool operator<=( const tuple3 < T0, T1, T2 > &lhs,
                     const tuple3 < S0, S1, S2 > &rhs )
    {
      return !( rhs < lhs );
    }
    template < class T0, class T1, class T2, class S0, class S1,
    class S2 >
    bool operator>( const tuple3 < T0, T1, T2 > &lhs,
                    const tuple3 < S0, S1, S2 > &rhs )
    {
      return ( rhs < lhs );
    }
    template < class T0, class T1, class T2, class S0, class S1,
    class S2 >
    bool operator>=( const tuple3 < T0, T1, T2 > &lhs,
                     const tuple3 < S0, S1, S2 > &rhs )
    {
      return !( lhs < rhs );
    }
    template < class T0, class T1, class T2, class T3, class S0, class S1,
    class S2, class S3 >
    bool operator==( const tuple4 < T0, T1, T2,
                     T3 > &lhs, const tuple4 < S0, S1,
                     S2, S3 > &rhs )
    {
      return ( lhs.m0 == rhs.m0 && lhs.m1 == rhs.m1 && lhs.m2 == rhs.m2
               && lhs.m3 == rhs.m3 );
    }
    template < class T0, class T1, class T2, class T3, class S0, class S1,
    class S2, class S3 >
    bool operator<( const tuple4 < T0, T1, T2,
                    T3 > &lhs, const tuple4 < S0, S1,
                    S2, S3 > &rhs )
    {
      return ( lhs.head( ) < rhs.head( )
               || ( !( rhs.head( ) < lhs.head( ) )
                    && lhs.tail( ) < rhs.tail( ) ) );
    }
    template < class T0, class T1, class T2, class T3, class S0, class S1,
    class S2, class S3 >
    bool operator<=( const tuple4 < T0, T1, T2,
                     T3 > &lhs, const tuple4 < S0, S1,
                     S2, S3 > &rhs )
    {
      return !( rhs < lhs );
    }
    template < class T0, class T1, class T2, class T3, class S0, class S1,
    class S2, class S3 >
    bool operator>( const tuple4 < T0, T1, T2,
                    T3 > &lhs, const tuple4 < S0, S1,
                    S2, S3 > &rhs )
    {
      return ( rhs < lhs );
    }
    template < class T0, class T1, class T2, class T3, class S0, class S1,
    class S2, class S3 >
    bool operator>=( const tuple4 < T0, T1, T2,
                     T3 > &lhs, const tuple4 < S0, S1,
                     S2, S3 > &rhs )
    {
      return !( lhs < rhs );
    }
    template < class T0, class T1, class T2, class T3, class T4, class S0,
    class S1, class S2, class S3,
    class S4 >
    bool operator==( const tuple5 < T0, T1, T2, T3, T4 > &lhs,
                     const tuple5 < S0, S1, S2, S3, S4 > &rhs )
    {
      return ( lhs.m0 == rhs.m0 && lhs.m1 == rhs.m1 && lhs.m2 == rhs.m2
               && lhs.m3 == rhs.m3 && lhs.m4 == rhs.m4 );
    }
    template < class T0, class T1, class T2, class T3, class T4, class S0,
    class S1, class S2, class S3,
    class S4 >
    bool operator<( const tuple5 < T0, T1, T2, T3, T4 > &lhs,
                    const tuple5 < S0, S1, S2, S3, S4 > &rhs )
    {
      return ( lhs.head( ) < rhs.head( )
               || ( !( rhs.head( ) < lhs.head( ) )
                    && lhs.tail( ) < rhs.tail( ) ) );
    }
    template < class T0, class T1, class T2, class T3, class T4, class S0,
    class S1, class S2, class S3,
    class S4 >
    bool operator<=( const tuple5 < T0, T1, T2, T3, T4 > &lhs,
                     const tuple5 < S0, S1, S2, S3, S4 > &rhs )
    {
      return !( rhs < lhs );
    }
    template < class T0, class T1, class T2, class T3, class T4, class S0,
    class S1, class S2, class S3,
    class S4 >
    bool operator>( const tuple5 < T0, T1, T2, T3, T4 > &lhs,
                    const tuple5 < S0, S1, S2, S3, S4 > &rhs )
    {
      return ( rhs < lhs );
    }
    template < class T0, class T1, class T2, class T3, class T4, class S0,
    class S1, class S2, class S3,
    class S4 >
    bool operator>=( const tuple5 < T0, T1, T2, T3, T4 > &lhs,
                     const tuple5 < S0, S1, S2, S3, S4 > &rhs )
    {
      return !( lhs < rhs );
    }
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class S0, class S1, class S2, class S3, class S4,
    class S5 >
    bool operator==( const tuple6 < T0, T1, T2, T3, T4,
                     T5 > &lhs, const tuple6 < S0, S1, S2, S3,
                     S4, S5 > &rhs )
    {
      return ( lhs.m0 == rhs.m0 && lhs.m1 == rhs.m1 && lhs.m2 == rhs.m2
               && lhs.m3 == rhs.m3 && lhs.m4 == rhs.m4 && lhs.m5 == rhs.m5 );
    }
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class S0, class S1, class S2, class S3, class S4,
    class S5 >
    bool operator<( const tuple6 < T0, T1, T2, T3, T4, T5 > &lhs,
                    const tuple6 < S0, S1, S2, S3, S4,
                    S5 > &rhs )
    {
      return ( lhs.head( ) < rhs.head( )
               || ( !( rhs.head( ) < lhs.head( ) )
                    && lhs.tail( ) < rhs.tail( ) ) );
    }
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class S0, class S1, class S2, class S3, class S4,
    class S5 >
    bool operator<=( const tuple6 < T0, T1, T2, T3, T4,
                     T5 > &lhs, const tuple6 < S0, S1, S2, S3,
                     S4, S5 > &rhs )
    {
      return !( rhs < lhs );
    }
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class S0, class S1, class S2, class S3, class S4,
    class S5 >
    bool operator>( const tuple6 < T0, T1, T2, T3, T4, T5 > &lhs,
                    const tuple6 < S0, S1, S2, S3, S4,
                    S5 > &rhs )
    {
      return ( rhs < lhs );
    }
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class S0, class S1, class S2, class S3, class S4,
    class S5 >
    bool operator>=( const tuple6 < T0, T1, T2, T3, T4,
                     T5 > &lhs, const tuple6 < S0, S1, S2, S3,
                     S4, S5 > &rhs )
    {
      return !( lhs < rhs );
    }
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6, class S0, class S1, class S2, class S3, class S4, class S5,
    class S6 >
    bool operator==( const tuple7 < T0, T1, T2, T3, T4, T5,
                     T6 > &lhs, const tuple7 < S0, S1, S2, S3,
                     S4, S5, S6 > &rhs )
    {
      return ( lhs.m0 == rhs.m0 && lhs.m1 == rhs.m1 && lhs.m2 == rhs.m2
               && lhs.m3 == rhs.m3 && lhs.m4 == rhs.m4 && lhs.m5 == rhs.m5
               && lhs.m6 == rhs.m6 );
    }
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6, class S0, class S1, class S2, class S3, class S4, class S5,
    class S6 >
    bool operator<( const tuple7 < T0, T1, T2, T3, T4, T5,
                    T6 > &lhs, const tuple7 < S0, S1, S2, S3, S4,
                    S5, S6 > &rhs )
    {
      return ( lhs.head( ) < rhs.head( )
               || ( !( rhs.head( ) < lhs.head( ) )
                    && lhs.tail( ) < rhs.tail( ) ) );
    }
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6, class S0, class S1, class S2, class S3, class S4, class S5,
    class S6 >
    bool operator<=( const tuple7 < T0, T1, T2, T3, T4, T5,
                     T6 > &lhs, const tuple7 < S0, S1, S2, S3,
                     S4, S5, S6 > &rhs )
    {
      return !( rhs < lhs );
    }
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6, class S0, class S1, class S2, class S3, class S4, class S5,
    class S6 >
    bool operator>( const tuple7 < T0, T1, T2, T3, T4, T5,
                    T6 > &lhs, const tuple7 < S0, S1, S2, S3, S4,
                    S5, S6 > &rhs )
    {
      return ( rhs < lhs );
    }
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6, class S0, class S1, class S2, class S3, class S4, class S5,
    class S6 >
    bool operator>=( const tuple7 < T0, T1, T2, T3, T4, T5,
                     T6 > &lhs, const tuple7 < S0, S1, S2, S3,
                     S4, S5, S6 > &rhs )
    {
      return !( lhs < rhs );
    }
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6, class T7, class S0, class S1, class S2, class S3, class S4,
    class S5, class S6, class S7 >
    bool operator==( const tuple8 < T0, T1,
                     T2, T3, T4, T5, T6,
                     T7 > &lhs,
                     const tuple8 < S0, S1,
                     S2, S3, S4, S5, S6,
                     S7 > &rhs )
    {
      return ( lhs.m0 == rhs.m0 && lhs.m1 == rhs.m1 && lhs.m2 == rhs.m2
               && lhs.m3 == rhs.m3 && lhs.m4 == rhs.m4 && lhs.m5 == rhs.m5
               && lhs.m6 == rhs.m6 && lhs.m7 == rhs.m7 );
    }
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6, class T7, class S0, class S1, class S2, class S3, class S4,
    class S5, class S6, class S7 >
    bool operator<( const tuple8 < T0, T1,
                    T2, T3, T4, T5, T6,
                    T7 > &lhs,
                    const tuple8 < S0, S1,
                    S2, S3, S4, S5, S6,
                    S7 > &rhs )
    {
      return ( lhs.head( ) < rhs.head( )
               || ( !( rhs.head( ) < lhs.head( ) )
                    && lhs.tail( ) < rhs.tail( ) ) );
    }
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6, class T7, class S0, class S1, class S2, class S3, class S4,
    class S5, class S6, class S7 >
    bool operator<=( const tuple8 < T0, T1,
                     T2, T3, T4, T5, T6,
                     T7 > &lhs,
                     const tuple8 < S0, S1,
                     S2, S3, S4, S5, S6,
                     S7 > &rhs )
    {
      return !( rhs < lhs );
    }
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6, class T7, class S0, class S1, class S2, class S3, class S4,
    class S5, class S6, class S7 >
    bool operator>( const tuple8 < T0, T1,
                    T2, T3, T4, T5, T6,
                    T7 > &lhs,
                    const tuple8 < S0, S1,
                    S2, S3, S4, S5, S6,
                    S7 > &rhs )
    {
      return ( rhs < lhs );
    }
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6, class T7, class S0, class S1, class S2, class S3, class S4,
    class S5, class S6, class S7 >
    bool operator>=( const tuple8 < T0, T1,
                     T2, T3, T4, T5, T6,
                     T7 > &lhs,
                     const tuple8 < S0, S1,
                     S2, S3, S4, S5, S6,
                     S7 > &rhs )
    {
      return !( lhs < rhs );
    }
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6, class T7, class T8, class S0, class S1, class S2, class S3,
    class S4, class S5, class S6, class S7,
    class S8 >
    bool operator==( const tuple9 < T0, T1, T2, T3, T4, T5, T6,
                     T7, T8 > &lhs, const tuple9 < S0, S1, S2,
                     S3, S4, S5, S6, S7, S8 > &rhs )
    {
      return ( lhs.m0 == rhs.m0 && lhs.m1 == rhs.m1 && lhs.m2 == rhs.m2
               && lhs.m3 == rhs.m3 && lhs.m4 == rhs.m4 && lhs.m5 == rhs.m5
               && lhs.m6 == rhs.m6 && lhs.m7 == rhs.m7 && lhs.m8 == rhs.m8 );
    }
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6, class T7, class T8, class S0, class S1, class S2, class S3,
    class S4, class S5, class S6, class S7,
    class S8 >
    bool operator<( const tuple9 < T0, T1, T2, T3, T4, T5, T6,
                    T7, T8 > &lhs, const tuple9 < S0, S1, S2, S3,
                    S4, S5, S6, S7, S8 > &rhs )
    {
      return ( lhs.head( ) < rhs.head( )
               || ( !( rhs.head( ) < lhs.head( ) )
                    && lhs.tail( ) < rhs.tail( ) ) );
    }
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6, class T7, class T8, class S0, class S1, class S2, class S3,
    class S4, class S5, class S6, class S7,
    class S8 >
    bool operator<=( const tuple9 < T0, T1, T2, T3, T4, T5, T6,
                     T7, T8 > &lhs, const tuple9 < S0, S1, S2,
                     S3, S4, S5, S6, S7, S8 > &rhs )
    {
      return !( rhs < lhs );
    }
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6, class T7, class T8, class S0, class S1, class S2, class S3,
    class S4, class S5, class S6, class S7,
    class S8 >
    bool operator>( const tuple9 < T0, T1, T2, T3, T4, T5, T6,
                    T7, T8 > &lhs, const tuple9 < S0, S1, S2, S3,
                    S4, S5, S6, S7, S8 > &rhs )
    {
      return ( rhs < lhs );
    }
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6, class T7, class T8, class S0, class S1, class S2, class S3,
    class S4, class S5, class S6, class S7,
    class S8 >
    bool operator>=( const tuple9 < T0, T1, T2, T3, T4, T5, T6,
                     T7, T8 > &lhs, const tuple9 < S0, S1, S2,
                     S3, S4, S5, S6, S7, S8 > &rhs )
    {
      return !( lhs < rhs );
    }

    // operator!= seems to be defined within STL, sometimes,
    // for instance in file stl_relops.h of SGI's STL
#ifndef TUPPLE_SKIP_UNEQUAL
    template < class T0, class S0 >
    bool operator!=( const tuple1 < T0 > &lhs,
                     const tuple1 < S0 >
                     &rhs )
    {
      return !( lhs == rhs );
    }
    template < class T0, class T1, class S0,
    class S1 >
    bool operator!=( const tuple2 < T0, T1 > &lhs,
                     const tuple2 < S0, S1 > &rhs )
    {
      return !( lhs == rhs );
    }
    template < class T0, class T1, class T2, class S0, class S1,
    class S2 >
    bool operator!=( const tuple3 < T0, T1, T2 > &lhs,
                     const tuple3 < S0, S1, S2 > &rhs )
    {
      return !( lhs == rhs );
    }
    template < class T0, class T1, class T2, class T3, class S0, class S1,
    class S2, class S3 >
    bool operator!=( const tuple4 < T0, T1, T2,
                     T3 > &lhs, const tuple4 < S0, S1,
                     S2, S3 > &rhs )
    {
      return !( lhs == rhs );
    }
    template < class T0, class T1, class T2, class T3, class T4, class S0,
    class S1, class S2, class S3,
    class S4 >
    bool operator!=( const tuple5 < T0, T1, T2, T3, T4 > &lhs,
                     const tuple5 < S0, S1, S2, S3, S4 > &rhs )
    {
      return !( lhs == rhs );
    }
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class S0, class S1, class S2, class S3, class S4,
    class S5 >
    bool operator!=( const tuple6 < T0, T1, T2, T3, T4,
                     T5 > &lhs, const tuple6 < S0, S1, S2, S3,
                     S4, S5 > &rhs )
    {
      return !( lhs == rhs );
    }
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6, class S0, class S1, class S2, class S3, class S4, class S5,
    class S6 >
    bool operator!=( const tuple7 < T0, T1, T2, T3, T4, T5,
                     T6 > &lhs, const tuple7 < S0, S1, S2, S3,
                     S4, S5, S6 > &rhs )
    {
      return !( lhs == rhs );
    }
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6, class T7, class S0, class S1, class S2, class S3, class S4,
    class S5, class S6, class S7 >
    bool operator!=( const tuple8 < T0, T1,
                     T2, T3, T4, T5, T6,
                     T7 > &lhs,
                     const tuple8 < S0, S1,
                     S2, S3, S4, S5, S6,
                     S7 > &rhs )
    {
      return !( lhs == rhs );
    }
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6, class T7, class T8, class S0, class S1, class S2, class S3,
    class S4, class S5, class S6, class S7,
    class S8 >
    bool operator!=( const tuple9 < T0, T1, T2, T3, T4, T5, T6,
                     T7, T8 > &lhs, const tuple9 < S0, S1, S2,
                     S3, S4, S5, S6, S7, S8 > &rhs )
    {
      return !( lhs == rhs );
    }
#endif
    template < class T >
    T minimal( const tuple1 < T > &t )
    {
      return t.head( );
    }
    template < class T >
    T maximal( const tuple1 < T > &t )
    {
      return t.head( );
    }

    template < class T >
    T minimal( const tuple2 < T, T > &t )
    {
      return std::min( t.head( ), minimal( t.tail( ) ) );
    }
    template < class T >
    T maximal( const tuple2 < T, T > &t )
    {
      return std::max( t.head( ), maximal( t.tail( ) ) );
    }
    template < class T >
    T minimal( const tuple3 < T, T, T > &t )
    {
      return std::min( t.head( ), minimal( t.tail( ) ) );
    }
    template < class T >
    T maximal( const tuple3 < T, T, T > &t )
    {
      return std::max( t.head( ), maximal( t.tail( ) ) );
    }
    template < class T >
    T minimal( const tuple4 < T, T, T, T > &t )
    {
      return std::min( t.head( ), minimal( t.tail( ) ) );
    }
    template < class T >
    T maximal( const tuple4 < T, T, T, T > &t )
    {
      return std::max( t.head( ), maximal( t.tail( ) ) );
    }
    template < class T >
    T minimal( const tuple5 < T, T, T, T, T > &t )
    {
      return std::min( t.head( ), minimal( t.tail( ) ) );
    }
    template < class T >
    T maximal( const tuple5 < T, T, T, T, T > &t )
    {
      return std::max( t.head( ), maximal( t.tail( ) ) );
    }
    template < class T >
    T minimal( const tuple6 < T, T, T, T, T, T > &t )
    {
      return std::min( t.head( ), minimal( t.tail( ) ) );
    }
    template < class T >
    T maximal( const tuple6 < T, T, T, T, T, T > &t )
    {
      return std::max( t.head( ), maximal( t.tail( ) ) );
    }
    template < class T >
    T minimal( const tuple7 < T, T, T, T, T, T, T > &t )
    {
      return std::min( t.head( ), minimal( t.tail( ) ) );
    }
    template < class T >
    T maximal( const tuple7 < T, T, T, T, T, T, T > &t )
    {
      return std::max( t.head( ), maximal( t.tail( ) ) );
    }
    template < class T >
    T minimal( const tuple8 < T, T, T, T, T, T, T,
               T > &t )
    {
      return std::min( t.head( ), minimal( t.tail( ) ) );
    }
    template < class T >
    T maximal( const tuple8 < T, T, T, T, T, T, T,
               T > &t )
    {
      return std::max( t.head( ), maximal( t.tail( ) ) );
    }
    template < class T >
    T minimal( const tuple9 < T, T, T, T, T, T, T, T,
               T > &t )
    {
      return std::min( t.head( ), minimal( t.tail( ) ) );
    }
    template < class T >
    T maximal( const tuple9 < T, T, T, T, T, T, T, T,
               T > &t )
    {
      return std::max( t.head( ), maximal( t.tail( ) ) );
    }
    namespace detail
    {
      // The initial case.
      template < class T0 >
      struct tuple_base_type_selector1
      {
        typedef::boost::mpl::if_c < ::boost::is_same < T0, null_type >::value,
        tuple0 < null_type >, tuple1 < T0 > > ::type type;
      };

      template < class T0, class T1 >
      struct tuple_base_type_selector2
      {
        typedef::boost::mpl::if_c < ::boost::is_same < T1, null_type >::value,
        tuple_base_type_selector1 < T0 >::type, tuple2 < T0,
        T1 > > ::type type;
      };
      template < class T0, class T1, class T2 >
      struct tuple_base_type_selector3
      {
        typedef::boost::mpl::if_c < ::boost::is_same < T2, null_type >::value,
        tuple_base_type_selector2 < T0, T1 >::type, tuple3 < T0, T1,
        T2 > > ::type type;
      };
      template < class T0, class T1, class T2, class T3 >
      struct tuple_base_type_selector4
      {
        typedef::boost::mpl::if_c < ::boost::is_same < T3, null_type >::value,
        tuple_base_type_selector3 < T0, T1, T2 >::type, tuple4 < T0, T1, T2,
        T3 > > ::type type;
      };
      template < class T0, class T1, class T2, class T3, class T4 >
      struct tuple_base_type_selector5
      {
        typedef::boost::mpl::if_c < ::boost::is_same < T4, null_type >::value,
        tuple_base_type_selector4 < T0, T1, T2, T3 >::type, tuple5 < T0, T1,
        T2, T3, T4 > > ::type type;
      };
      template < class T0, class T1, class T2, class T3, class T4,
      class T5 >
      struct tuple_base_type_selector6
      {
        typedef::boost::mpl::if_c < ::boost::is_same < T5, null_type >::value,
        tuple_base_type_selector5 < T0, T1, T2, T3, T4 >::type, tuple6 < T0,
        T1, T2, T3, T4, T5 > > ::type type;
      };
      template < class T0, class T1, class T2, class T3, class T4, class T5,
      class T6 >
      struct tuple_base_type_selector7
      {
        typedef::boost::mpl::if_c < ::boost::is_same < T6, null_type >::value,
        tuple_base_type_selector6 < T0, T1, T2, T3, T4, T5 >::type,
        tuple7 < T0, T1, T2, T3, T4, T5, T6 > > ::type type;
      };
      template < class T0, class T1, class T2, class T3, class T4, class T5,
      class T6, class T7 >
      struct tuple_base_type_selector8
      {
        typedef::boost::mpl::if_c < ::boost::is_same < T7, null_type >::value,
        tuple_base_type_selector7 < T0, T1, T2, T3, T4, T5, T6 >::type,
        tuple8 < T0, T1, T2, T3, T4, T5, T6, T7 > > ::type type;
      };
      template < class T0, class T1, class T2, class T3, class T4, class T5,
      class T6, class T7, class T8 >
      struct tuple_base_type_selector9
      {
        typedef::boost::mpl::if_c < ::boost::is_same < T8, null_type >::value,
        tuple_base_type_selector8 < T0, T1, T2, T3, T4, T5, T6, T7 >::type,
        tuple9 < T0, T1, T2, T3, T4, T5, T6, T7, T8 > > ::type type;
      };

    }                           // namespace detail
    template < class T0 = null_type, class T1 = null_type, class T2 =
    null_type, class T3 = null_type, class T4 = null_type, class T5 =
    null_type, class T6 = null_type, class T7 = null_type, class T8 =
    null_type >
    struct tuple: public detail::tuple_base_type_selector9 < T0,
          T1, T2, T3, T4, T5, T6, T7, T8 > ::type
    {
      typedef detail::tuple_base_type_selector9 < T0, T1, T2, T3, T4, T5, T6,
      T7, T8 > ::type base_type;

      tuple( ) : self_type( )
      {}

      template < class V0 >
      tuple( V0 v0 ) : self_type( v0 )
      {}
      template < class V0, class V1 >
      tuple( V0 v0, V1 v1 ) : self_type( v0,
                                             v1 )
      {}
      template < class V0, class V1, class V2 >
      tuple( V0 v0, V1 v1, V2 v2 ) : self_type( v0, v1,
              v2 )
      {}
      template < class V0, class V1, class V2, class V3 >
      tuple( V0 v0, V1 v1, V2 v2, V3 v3 ) : self_type( v0, v1, v2,
              v3 )
      {}
      template < class V0, class V1, class V2, class V3, class V4 >
      tuple( V0 v0, V1 v1, V2 v2, V3 v3, V4 v4 ) : self_type( v0, v1, v2, v3,
              v4 )
      {}
      template < class V0, class V1, class V2, class V3, class V4, class V5 >
      tuple( V0 v0, V1 v1, V2 v2, V3 v3, V4 v4, V5 v5 ) : self_type( v0, v1, v2, v3, v4,
              v5 )
      {}
      template < class V0, class V1, class V2, class V3, class V4, class V5, class V6 >
      tuple( V0 v0, V1 v1, V2 v2, V3 v3, V4 v4, V5 v5, V6 v6 ) : self_type( v0, v1, v2, v3, v4, v5,
              v6 )
      {}
      template < class V0, class V1, class V2, class V3, class V4, class V5, class V6, class V7 >
      tuple( V0 v0, V1 v1, V2 v2, V3 v3, V4 v4, V5 v5, V6 v6, V7 v7 ) : self_type( v0, v1, v2, v3, v4, v5, v6,
              v7 )
      {}
      template < class V0, class V1, class V2, class V3, class V4, class V5, class V6, class V7, class V8 >
      tuple( V0 v0, V1 v1, V2 v2, V3 v3, V4 v4, V5 v5, V6 v6, V7 v7, V8 v8 ) : self_type( v0, v1, v2, v3, v4, v5, v6, v7,
              v8 )
      {}

      template < class S0, class S1, class S2, class S3, class S4, class S5,
      class S6, class S7,
      class S8 >
      base_type & operator=( const tuple < S0, S1, S2, S3, S4,
                             S5, S6, S7, S8 > &rhs )
      {
        //BOOST_STATIC_ASSERT(  );
        return ( base_type::operator=( rhs ) );
      }
    };



  }                             // namespace tupple
}                               // namespace boost


#endif
