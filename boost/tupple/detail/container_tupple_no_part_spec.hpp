// Copyright (C) 2001-2003 Roland Richter <roland@flll.jku.at>
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all copies.
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.
#ifndef BOOST_TUPPLE_DETAIL_CONTAINER_TUPPLE_NO_PART_SPEC_HPP
#define BOOST_TUPPLE_DETAIL_CONTAINER_TUPPLE_NO_PART_SPEC_HPP
#include <boost/tupple/tupple.hpp>
#include <boost/tupple/detail/ref_workaround.hpp>
#include <boost/tupple/detail/iterator_tupple_traits_detail.hpp>
#include <boost/tupple/iterator_tupple.hpp>


namespace boost
{
  namespace tupple
  {
    //#  define CONTTRAITSTYPE(k) TYPE( CONTTRAITS(k),k,R )
    //#  define TRAITSTEMPLATE(k,a,b) template<BOOST_PP_ENUM_PARAMS(k,class a),BOOST_PP_ENUM(k,b,_)>
    template < class T >
    struct container_tuple0
    {
      typedef container_tuple0 < T > self_type;
    };



    template < class R0 >
    struct container_traits_tuple1
    {
      typedef tuple1 < typename R0::value_type > value_type;
      typedef iterator_tuple1 < typename R0::iterator > iterator;
      typedef iterator_tuple1 < typename R0::const_iterator > const_iterator;
      typedef tuple1 < detail::reference_wrapper < typename R0::value_type >
      > reference;
      typedef tuple1 < typename R0::value_type > const_reference;
      typedef tuple1 < typename R0::pointer > pointer;
      typedef tuple1 < typename R0::difference_type > difference_type;
    };
    template < class T0 >
    struct container_tuple1: public tuple1 < T0 >
    {
      typedef container_tuple1 < T0 > self_type;
      typedef tuple1 < typename T0::size_type > size_type;
      typedef container_traits_tuple1 < T0 > traits;
      typedef typename traits::value_type value_type;
      typedef typename traits::iterator iterator;
      typedef typename traits::const_iterator const_iterator;
      typedef typename traits::reference reference;
      typedef typename traits::const_reference const_reference;
      typedef typename traits::pointer pointer;
      typedef typename traits::difference_type difference_type;
      container_tuple1( arg_type0 theM0 ) : tuple1 < T0 > ( theM0 )
      {}
      template < class S0 >
      container_tuple1( const container_tuple1 < S0 >
                        &rhs ) : tuple1 < T0 > ( rhs )
      {}
      container_tuple1( arg_type0 theM0,
                        const tail_type & tail ) : tuple1 < T0 > ( theM0,
                                tail )
      {}
      tuple1 < bool > empty( ) const
      {
        return tuple1 < bool > ( m0.empty( ) );
      }
      size_type size( ) const
      {
        return size_type( m0.size( ) );
      }
      iterator begin( )
      {
        return iterator( m0.begin( ) );
      }
      const_iterator begin( ) const
      {
        return const_iterator( m0.begin( ) );
      }
      iterator end( )
      {
        return iterator( m0.end( ) );
      }
      const_iterator end( ) const
      {
        return const_iterator( m0.end( ) );
      }
      reference operator[] ( const size_type & i )
      {
        return reference( detail::ref( m0[ i.m0 ] ) );
      }
      const_reference operator[] ( const size_type & i ) const
      {
        return const_reference( detail::cref( m0[ i.m0 ] ) );
      }
    };
    template < class R0, class R1 >
    struct container_traits_tuple2
    {
      typedef tuple2 < typename R0::value_type,
      typename R1::value_type > value_type;
      typedef iterator_tuple2 < typename R0::iterator,
      typename R1::iterator > iterator;
      typedef iterator_tuple2 < typename R0::const_iterator,
      typename R1::const_iterator > const_iterator;
      typedef tuple2 < detail::reference_wrapper < typename R0::value_type >,
      detail::reference_wrapper < typename R1::value_type > > reference;
      typedef tuple2 < typename R0::value_type,
      typename R1::value_type > const_reference;
      typedef tuple2 < typename R0::pointer, typename R1::pointer > pointer;
      typedef tuple2 < typename R0::difference_type,
      typename R1::difference_type > difference_type;
    };
    template < class T0,
    class T1 >
    struct container_tuple2: public tuple2 < T0, T1 >
    {
      typedef container_tuple2 < T0, T1 > self_type;
      typedef tuple2 < typename T0::size_type,
      typename T1::size_type > size_type;
      typedef container_traits_tuple2 < T0, T1 > traits;
      typedef typename traits::value_type value_type;
      typedef typename traits::iterator iterator;
      typedef typename traits::const_iterator const_iterator;
      typedef typename traits::reference reference;
      typedef typename traits::const_reference const_reference;
      typedef typename traits::pointer pointer;
      typedef typename traits::difference_type difference_type;
      container_tuple2( arg_type0 theM0, arg_type1 theM1 ) : tuple2 < T0,
          T1 > ( theM0, theM1 )
      {}
      template < class S0,
      class S1 >
      container_tuple2( const container_tuple2 < S0,
                        S1 > &rhs ) : tuple2 < T0, T1 > ( rhs )
      {}
      container_tuple2( arg_type0 theM0,
                        const tail_type & tail ) : tuple2 < T0, T1 > ( theM0,
                                tail )
      {}
      tuple2 < bool, bool > empty( ) const
      {
        return tuple2 < bool, bool > ( m0.empty( ), m1.empty( ) );
      }
      size_type size( ) const
      {
        return size_type( m0.size( ), m1.size( ) );
      }
      iterator begin( )
      {
        return iterator( m0.begin( ), m1.begin( ) );
      }
      const_iterator begin( ) const
      {
        return const_iterator( m0.begin( ), m1.begin( ) );
      }
      iterator end( )
      {
        return iterator( m0.end( ), m1.end( ) );
      }
      const_iterator end( ) const
      {
        return const_iterator( m0.end( ), m1.end( ) );
      }
      reference operator[] ( const size_type & i )
      {
        return reference( detail::ref( m0[ i.m0 ] ), detail::ref( m1[ i.m1 ] ) );
      }
      const_reference operator[] ( const size_type & i ) const
      {
        return const_reference( detail::cref( m0[ i.m0 ] ),
                                detail::cref( m1[ i.m1 ] ) );
      }
    };
    template < class R0, class R1, class R2 >
    struct container_traits_tuple3
    {
      typedef tuple3 < typename R0::value_type, typename R1::value_type,
      typename R2::value_type > value_type;
      typedef iterator_tuple3 < typename R0::iterator, typename R1::iterator,
      typename R2::iterator > iterator;
      typedef iterator_tuple3 < typename R0::const_iterator,
      typename R1::const_iterator,
      typename R2::const_iterator > const_iterator;
      typedef tuple3 < detail::reference_wrapper < typename R0::value_type >,
      detail::reference_wrapper < typename R1::value_type >,
      detail::reference_wrapper < typename R2::value_type > > reference;
      typedef tuple3 < typename R0::value_type, typename R1::value_type,
      typename R2::value_type > const_reference;
      typedef tuple3 < typename R0::pointer, typename R1::pointer,
      typename R2::pointer > pointer;
      typedef tuple3 < typename R0::difference_type,
      typename R1::difference_type,
      typename R2::difference_type > difference_type;
    };
    template < class T0, class T1,
    class T2 >
    struct container_tuple3: public tuple3 < T0, T1, T2 >
    {
      typedef container_tuple3 < T0, T1, T2 > self_type;
      typedef tuple3 < typename T0::size_type, typename T1::size_type,
      typename T2::size_type > size_type;
      typedef container_traits_tuple3 < T0, T1, T2 > traits;
      typedef typename traits::value_type value_type;
      typedef typename traits::iterator iterator;
      typedef typename traits::const_iterator const_iterator;
      typedef typename traits::reference reference;
      typedef typename traits::const_reference const_reference;
      typedef typename traits::pointer pointer;
      typedef typename traits::difference_type difference_type;
      container_tuple3( arg_type0 theM0, arg_type1 theM1,
                        arg_type2 theM2 ) : tuple3 < T0, T1, T2 > ( theM0,
                                theM1,
                                theM2 )
      {}
      template < class S0, class S1,
      class S2 >
      container_tuple3( const container_tuple3 < S0, S1,
                        S2 > &rhs ) : tuple3 < T0, T1, T2 > ( rhs )
      {}
      container_tuple3( arg_type0 theM0,
                        const tail_type & tail ) : tuple3 < T0, T1,
          T2 > ( theM0, tail )
      {}
      tuple3 < bool, bool, bool > empty( ) const
      {
        return tuple3 < bool, bool, bool > ( m0.empty( ), m1.empty( ),
                                             m2.empty( ) );
      }
      size_type size( ) const
      {
        return size_type( m0.size( ), m1.size( ), m2.size( ) );
      }
      iterator begin( )
      {
        return iterator( m0.begin( ), m1.begin( ), m2.begin( ) );
      }
      const_iterator begin( ) const
      {
        return const_iterator( m0.begin( ), m1.begin( ), m2.begin( ) );
      }
      iterator end( )
      {
        return iterator( m0.end( ), m1.end( ), m2.end( ) );
      }
      const_iterator end( ) const
      {
        return const_iterator( m0.end( ), m1.end( ), m2.end( ) );
      }
      reference operator[] ( const size_type & i )
      {
        return reference( detail::ref( m0[ i.m0 ] ), detail::ref( m1[ i.m1 ] ),
                          detail::ref( m2[ i.m2 ] ) );
      }
      const_reference operator[] ( const size_type & i ) const
      {
        return const_reference( detail::cref( m0[ i.m0 ] ),
                                detail::cref( m1[ i.m1 ] ),
                                detail::cref( m2[ i.m2 ] ) );
      }
    };
    template < class R0, class R1, class R2, class R3 >
    struct container_traits_tuple4
    {
      typedef tuple4 < typename R0::value_type, typename R1::value_type,
      typename R2::value_type, typename R3::value_type > value_type;
      typedef iterator_tuple4 < typename R0::iterator, typename R1::iterator,
      typename R2::iterator, typename R3::iterator > iterator;
      typedef iterator_tuple4 < typename R0::const_iterator,
      typename R1::const_iterator, typename R2::const_iterator,
      typename R3::const_iterator > const_iterator;
      typedef tuple4 < detail::reference_wrapper < typename R0::value_type >,
      detail::reference_wrapper < typename R1::value_type >,
      detail::reference_wrapper < typename R2::value_type >,
      detail::reference_wrapper < typename R3::value_type > > reference;
      typedef tuple4 < typename R0::value_type, typename R1::value_type,
      typename R2::value_type, typename R3::value_type > const_reference;
      typedef tuple4 < typename R0::pointer, typename R1::pointer,
      typename R2::pointer, typename R3::pointer > pointer;
      typedef tuple4 < typename R0::difference_type,
      typename R1::difference_type, typename R2::difference_type,
      typename R3::difference_type > difference_type;
    };
    template < class T0, class T1, class T2,
    class T3 >
    struct container_tuple4: public tuple4 < T0, T1, T2, T3 >
    {
      typedef container_tuple4 < T0, T1, T2, T3 > self_type;
      typedef tuple4 < typename T0::size_type, typename T1::size_type,
      typename T2::size_type, typename T3::size_type > size_type;
      typedef container_traits_tuple4 < T0, T1, T2, T3 > traits;
      typedef typename traits::value_type value_type;
      typedef typename traits::iterator iterator;
      typedef typename traits::const_iterator const_iterator;
      typedef typename traits::reference reference;
      typedef typename traits::const_reference const_reference;
      typedef typename traits::pointer pointer;
      typedef typename traits::difference_type difference_type;
      container_tuple4( arg_type0 theM0, arg_type1 theM1, arg_type2 theM2,
                        arg_type3 theM3 ) : tuple4 < T0, T1, T2, T3 > ( theM0,
                                theM1,
                                theM2,
                                theM3 )
      {}
      template < class S0, class S1, class S2,
      class S3 >
      container_tuple4( const container_tuple4 < S0, S1, S2,
                        S3 > &rhs ) : tuple4 < T0, T1, T2,
          T3 > ( rhs )
      {}
      container_tuple4( arg_type0 theM0,
                        const tail_type & tail ) : tuple4 < T0, T1, T2,
          T3 > ( theM0, tail )
      {}
      tuple4 < bool, bool, bool, bool > empty( ) const
      {
        return tuple4 < bool, bool, bool, bool > ( m0.empty( ), m1.empty( ),
               m2.empty( ),
               m3.empty( ) );
      }
      size_type size( ) const
      {
        return size_type( m0.size( ), m1.size( ), m2.size( ),
                          m3.size( ) );
      }
      iterator begin( )
      {
        return iterator( m0.begin( ), m1.begin( ), m2.begin( ),
                         m3.begin( ) );
      }
      const_iterator begin( ) const
      {
        return const_iterator( m0.begin( ), m1.begin( ), m2.begin( ),
                               m3.begin( ) );
      }
      iterator end( )
      {
        return iterator( m0.end( ), m1.end( ), m2.end( ), m3.end( ) );
      }
      const_iterator end( ) const
      {
        return const_iterator( m0.end( ), m1.end( ), m2.end( ),
                               m3.end( ) );
      }
      reference operator[] ( const size_type & i )
      {
        return reference( detail::ref( m0[ i.m0 ] ), detail::ref( m1[ i.m1 ] ),
                          detail::ref( m2[ i.m2 ] ), detail::ref( m3[ i.m3 ] ) );
      }
      const_reference operator[] ( const size_type & i ) const
      {
        return const_reference( detail::cref( m0[ i.m0 ] ),
                                detail::cref( m1[ i.m1 ] ),
                                detail::cref( m2[ i.m2 ] ),
                                detail::cref( m3[ i.m3 ] ) );
      }
    };
    template < class R0, class R1, class R2, class R3, class R4 >
    struct container_traits_tuple5
    {
      typedef tuple5 < typename R0::value_type, typename R1::value_type,
      typename R2::value_type, typename R3::value_type,
      typename R4::value_type > value_type;
      typedef iterator_tuple5 < typename R0::iterator, typename R1::iterator,
      typename R2::iterator, typename R3::iterator,
      typename R4::iterator > iterator;
      typedef iterator_tuple5 < typename R0::const_iterator,
      typename R1::const_iterator, typename R2::const_iterator,
      typename R3::const_iterator,
      typename R4::const_iterator > const_iterator;
      typedef tuple5 < detail::reference_wrapper < typename R0::value_type >,
      detail::reference_wrapper < typename R1::value_type >,
      detail::reference_wrapper < typename R2::value_type >,
      detail::reference_wrapper < typename R3::value_type >,
      detail::reference_wrapper < typename R4::value_type > > reference;
      typedef tuple5 < typename R0::value_type, typename R1::value_type,
      typename R2::value_type, typename R3::value_type,
      typename R4::value_type > const_reference;
      typedef tuple5 < typename R0::pointer, typename R1::pointer,
      typename R2::pointer, typename R3::pointer,
      typename R4::pointer > pointer;
      typedef tuple5 < typename R0::difference_type,
      typename R1::difference_type, typename R2::difference_type,
      typename R3::difference_type,
      typename R4::difference_type > difference_type;
    };
    template < class T0, class T1, class T2, class T3,
    class T4 >
    struct container_tuple5: public tuple5 < T0, T1, T2, T3, T4 >
    {
      typedef container_tuple5 < T0, T1, T2, T3, T4 > self_type;
      typedef tuple5 < typename T0::size_type, typename T1::size_type,
      typename T2::size_type, typename T3::size_type,
      typename T4::size_type > size_type;
      typedef container_traits_tuple5 < T0, T1, T2, T3, T4 > traits;
      typedef typename traits::value_type value_type;
      typedef typename traits::iterator iterator;
      typedef typename traits::const_iterator const_iterator;
      typedef typename traits::reference reference;
      typedef typename traits::const_reference const_reference;
      typedef typename traits::pointer pointer;
      typedef typename traits::difference_type difference_type;
      container_tuple5( arg_type0 theM0, arg_type1 theM1, arg_type2 theM2,
                        arg_type3 theM3, arg_type4 theM4 ) : tuple5 < T0, T1,
          T2, T3, T4 > ( theM0, theM1, theM2, theM3, theM4 )
      {}
      template < class S0, class S1, class S2, class S3,
      class S4 >
      container_tuple5( const container_tuple5 < S0, S1, S2, S3,
                        S4 > &rhs ) : tuple5 < T0, T1, T2, T3,
          T4 > ( rhs )
      {}
      container_tuple5( arg_type0 theM0,
                        const tail_type & tail ) : tuple5 < T0, T1, T2, T3,
          T4 > ( theM0, tail )
      {}
      tuple5 < bool, bool, bool, bool, bool > empty( ) const
      {
        return tuple5 < bool, bool, bool, bool, bool > ( m0.empty( ),
               m1.empty( ),
               m2.empty( ),
               m3.empty( ),
               m4.empty( ) );
      }
      size_type size( ) const
      {
        return size_type( m0.size( ), m1.size( ), m2.size( ), m3.size( ),
                          m4.size( ) );
      }
      iterator begin( )
      {
        return iterator( m0.begin( ), m1.begin( ), m2.begin( ),
                         m3.begin( ), m4.begin( ) );
      }
      const_iterator begin( ) const
      {
        return const_iterator( m0.begin( ), m1.begin( ), m2.begin( ),
                               m3.begin( ), m4.begin( ) );
      }
      iterator end( )
      {
        return iterator( m0.end( ), m1.end( ), m2.end( ), m3.end( ),
                         m4.end( ) );
      }
      const_iterator end( ) const
      {
        return const_iterator( m0.end( ), m1.end( ), m2.end( ), m3.end( ),
                               m4.end( ) );
      }
      reference operator[] ( const size_type & i )
      {
        return reference( detail::ref( m0[ i.m0 ] ), detail::ref( m1[ i.m1 ] ),
                          detail::ref( m2[ i.m2 ] ), detail::ref( m3[ i.m3 ] ),
                          detail::ref( m4[ i.m4 ] ) );
      }
      const_reference operator[] ( const size_type & i ) const
      {
        return const_reference( detail::cref( m0[ i.m0 ] ),
                                detail::cref( m1[ i.m1 ] ),
                                detail::cref( m2[ i.m2 ] ),
                                detail::cref( m3[ i.m3 ] ),
                                detail::cref( m4[ i.m4 ] ) );
      }
    };
    template < class R0, class R1, class R2, class R3, class R4, class R5 >
    struct container_traits_tuple6
    {
      typedef tuple6 < typename R0::value_type, typename R1::value_type,
      typename R2::value_type, typename R3::value_type,
      typename R4::value_type, typename R5::value_type > value_type;
      typedef iterator_tuple6 < typename R0::iterator, typename R1::iterator,
      typename R2::iterator, typename R3::iterator, typename R4::iterator,
      typename R5::iterator > iterator;
      typedef iterator_tuple6 < typename R0::const_iterator,
      typename R1::const_iterator, typename R2::const_iterator,
      typename R3::const_iterator, typename R4::const_iterator,
      typename R5::const_iterator > const_iterator;
      typedef tuple6 < detail::reference_wrapper < typename R0::value_type >,
      detail::reference_wrapper < typename R1::value_type >,
      detail::reference_wrapper < typename R2::value_type >,
      detail::reference_wrapper < typename R3::value_type >,
      detail::reference_wrapper < typename R4::value_type >,
      detail::reference_wrapper < typename R5::value_type > > reference;
      typedef tuple6 < typename R0::value_type, typename R1::value_type,
      typename R2::value_type, typename R3::value_type,
      typename R4::value_type, typename R5::value_type > const_reference;
      typedef tuple6 < typename R0::pointer, typename R1::pointer,
      typename R2::pointer, typename R3::pointer, typename R4::pointer,
      typename R5::pointer > pointer;
      typedef tuple6 < typename R0::difference_type,
      typename R1::difference_type, typename R2::difference_type,
      typename R3::difference_type, typename R4::difference_type,
      typename R5::difference_type > difference_type;
    };
    template < class T0, class T1, class T2, class T3, class T4,
    class T5 >
    struct container_tuple6: public tuple6 < T0, T1, T2, T3, T4,
          T5 >
    {
      typedef container_tuple6 < T0, T1, T2, T3, T4, T5 > self_type;
      typedef tuple6 < typename T0::size_type, typename T1::size_type,
      typename T2::size_type, typename T3::size_type,
      typename T4::size_type, typename T5::size_type > size_type;
      typedef container_traits_tuple6 < T0, T1, T2, T3, T4, T5 > traits;
      typedef typename traits::value_type value_type;
      typedef typename traits::iterator iterator;
      typedef typename traits::const_iterator const_iterator;
      typedef typename traits::reference reference;
      typedef typename traits::const_reference const_reference;
      typedef typename traits::pointer pointer;
      typedef typename traits::difference_type difference_type;
      container_tuple6( arg_type0 theM0, arg_type1 theM1, arg_type2 theM2,
                        arg_type3 theM3, arg_type4 theM4,
                        arg_type5 theM5 ) : tuple6 < T0, T1, T2, T3, T4,
          T5 > ( theM0, theM1, theM2, theM3, theM4, theM5 )
      {}
      template < class S0, class S1, class S2, class S3, class S4,
      class S5 >
      container_tuple6( const container_tuple6 < S0, S1, S2, S3,
                        S4, S5 > &rhs ) : tuple6 < T0, T1, T2, T3,
          T4, T5 > ( rhs )
      {}
      container_tuple6( arg_type0 theM0,
                        const tail_type & tail ) : tuple6 < T0, T1, T2, T3,
          T4, T5 > ( theM0, tail )
      {}
      tuple6 < bool, bool, bool, bool, bool, bool > empty( ) const
      {
        return tuple6 < bool, bool, bool, bool, bool, bool > ( m0.empty( ),
               m1.empty( ),
               m2.empty( ),
               m3.empty( ),
               m4.empty( ),
               m5.empty( ) );
      }
      size_type size( ) const
      {
        return size_type( m0.size( ), m1.size( ), m2.size( ), m3.size( ),
                          m4.size( ), m5.size( ) );
      }
      iterator begin( )
      {
        return iterator( m0.begin( ), m1.begin( ), m2.begin( ),
                         m3.begin( ), m4.begin( ), m5.begin( ) );
      }
      const_iterator begin( ) const
      {
        return const_iterator( m0.begin( ), m1.begin( ), m2.begin( ),
                               m3.begin( ), m4.begin( ), m5.begin( ) );
      }
      iterator end( )
      {
        return iterator( m0.end( ), m1.end( ), m2.end( ), m3.end( ),
                         m4.end( ), m5.end( ) );
      }
      const_iterator end( ) const
      {
        return const_iterator( m0.end( ), m1.end( ), m2.end( ), m3.end( ),
                               m4.end( ), m5.end( ) );
      }
      reference operator[] ( const size_type & i )
      {
        return reference( detail::ref( m0[ i.m0 ] ), detail::ref( m1[ i.m1 ] ),
                          detail::ref( m2[ i.m2 ] ), detail::ref( m3[ i.m3 ] ),
                          detail::ref( m4[ i.m4 ] ), detail::ref( m5[ i.m5 ] ) );
      }
      const_reference operator[] ( const size_type & i ) const
      {
        return const_reference( detail::cref( m0[ i.m0 ] ),
                                detail::cref( m1[ i.m1 ] ),
                                detail::cref( m2[ i.m2 ] ),
                                detail::cref( m3[ i.m3 ] ),
                                detail::cref( m4[ i.m4 ] ),
                                detail::cref( m5[ i.m5 ] ) );
      }
    };
    template < class R0, class R1, class R2, class R3, class R4, class R5,
    class R6 >
    struct container_traits_tuple7
    {
      typedef tuple7 < typename R0::value_type, typename R1::value_type,
      typename R2::value_type, typename R3::value_type,
      typename R4::value_type, typename R5::value_type,
      typename R6::value_type > value_type;
      typedef iterator_tuple7 < typename R0::iterator, typename R1::iterator,
      typename R2::iterator, typename R3::iterator, typename R4::iterator,
      typename R5::iterator, typename R6::iterator > iterator;
      typedef iterator_tuple7 < typename R0::const_iterator,
      typename R1::const_iterator, typename R2::const_iterator,
      typename R3::const_iterator, typename R4::const_iterator,
      typename R5::const_iterator,
      typename R6::const_iterator > const_iterator;
      typedef tuple7 < detail::reference_wrapper < typename R0::value_type >,
      detail::reference_wrapper < typename R1::value_type >,
      detail::reference_wrapper < typename R2::value_type >,
      detail::reference_wrapper < typename R3::value_type >,
      detail::reference_wrapper < typename R4::value_type >,
      detail::reference_wrapper < typename R5::value_type >,
      detail::reference_wrapper < typename R6::value_type > > reference;
      typedef tuple7 < typename R0::value_type, typename R1::value_type,
      typename R2::value_type, typename R3::value_type,
      typename R4::value_type, typename R5::value_type,
      typename R6::value_type > const_reference;
      typedef tuple7 < typename R0::pointer, typename R1::pointer,
      typename R2::pointer, typename R3::pointer, typename R4::pointer,
      typename R5::pointer, typename R6::pointer > pointer;
      typedef tuple7 < typename R0::difference_type,
      typename R1::difference_type, typename R2::difference_type,
      typename R3::difference_type, typename R4::difference_type,
      typename R5::difference_type,
      typename R6::difference_type > difference_type;
    };
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6 >
    struct container_tuple7: public tuple7 < T0, T1, T2, T3, T4,
          T5, T6 >
    {
      typedef container_tuple7 < T0, T1, T2, T3, T4, T5, T6 > self_type;
      typedef tuple7 < typename T0::size_type, typename T1::size_type,
      typename T2::size_type, typename T3::size_type,
      typename T4::size_type, typename T5::size_type,
      typename T6::size_type > size_type;
      typedef container_traits_tuple7 < T0, T1, T2, T3, T4, T5, T6 > traits;
      typedef typename traits::value_type value_type;
      typedef typename traits::iterator iterator;
      typedef typename traits::const_iterator const_iterator;
      typedef typename traits::reference reference;
      typedef typename traits::const_reference const_reference;
      typedef typename traits::pointer pointer;
      typedef typename traits::difference_type difference_type;
      container_tuple7( arg_type0 theM0, arg_type1 theM1, arg_type2 theM2,
                        arg_type3 theM3, arg_type4 theM4, arg_type5 theM5,
                        arg_type6 theM6 ) : tuple7 < T0, T1, T2, T3, T4, T5,
          T6 > ( theM0, theM1, theM2, theM3, theM4, theM5, theM6 )
      {}
      template < class S0, class S1, class S2, class S3, class S4, class S5,
      class S6 >
      container_tuple7( const container_tuple7 < S0, S1, S2, S3,
                        S4, S5, S6 > &rhs ) : tuple7 < T0, T1, T2,
          T3, T4, T5, T6 > ( rhs )
      {}
      container_tuple7( arg_type0 theM0,
                        const tail_type & tail ) : tuple7 < T0, T1, T2, T3,
          T4, T5, T6 > ( theM0, tail )
      {}
      tuple7 < bool, bool, bool, bool, bool, bool, bool > empty( ) const
      {
        return tuple7 < bool, bool, bool, bool, bool, bool,
               bool > ( m0.empty( ), m1.empty( ), m2.empty( ), m3.empty( ),
                        m4.empty( ), m5.empty( ), m6.empty( ) );
      }
      size_type size( ) const
      {
        return size_type( m0.size( ), m1.size( ), m2.size( ), m3.size( ),
                          m4.size( ), m5.size( ), m6.size( ) );
      }
      iterator begin( )
      {
        return iterator( m0.begin( ), m1.begin( ), m2.begin( ),
                         m3.begin( ), m4.begin( ), m5.begin( ),
                         m6.begin( ) );
      }
      const_iterator begin( ) const
      {
        return const_iterator( m0.begin( ), m1.begin( ), m2.begin( ),
                               m3.begin( ), m4.begin( ), m5.begin( ),
                               m6.begin( ) );
      }
      iterator end( )
      {
        return iterator( m0.end( ), m1.end( ), m2.end( ), m3.end( ),
                         m4.end( ), m5.end( ), m6.end( ) );
      }
      const_iterator end( ) const
      {
        return const_iterator( m0.end( ), m1.end( ), m2.end( ), m3.end( ),
                               m4.end( ), m5.end( ), m6.end( ) );
      }
      reference operator[] ( const size_type & i )
      {
        return reference( detail::ref( m0[ i.m0 ] ), detail::ref( m1[ i.m1 ] ),
                          detail::ref( m2[ i.m2 ] ), detail::ref( m3[ i.m3 ] ),
                          detail::ref( m4[ i.m4 ] ), detail::ref( m5[ i.m5 ] ),
                          detail::ref( m6[ i.m6 ] ) );
      }
      const_reference operator[] ( const size_type & i ) const
      {
        return const_reference( detail::cref( m0[ i.m0 ] ),
                                detail::cref( m1[ i.m1 ] ),
                                detail::cref( m2[ i.m2 ] ),
                                detail::cref( m3[ i.m3 ] ),
                                detail::cref( m4[ i.m4 ] ),
                                detail::cref( m5[ i.m5 ] ),
                                detail::cref( m6[ i.m6 ] ) );
      }
    };
    template < class R0, class R1, class R2, class R3, class R4, class R5,
    class R6, class R7 >
    struct container_traits_tuple8
    {
      typedef tuple8 < typename R0::value_type, typename R1::value_type,
      typename R2::value_type, typename R3::value_type,
      typename R4::value_type, typename R5::value_type,
      typename R6::value_type, typename R7::value_type > value_type;
      typedef iterator_tuple8 < typename R0::iterator, typename R1::iterator,
      typename R2::iterator, typename R3::iterator, typename R4::iterator,
      typename R5::iterator, typename R6::iterator,
      typename R7::iterator > iterator;
      typedef iterator_tuple8 < typename R0::const_iterator,
      typename R1::const_iterator, typename R2::const_iterator,
      typename R3::const_iterator, typename R4::const_iterator,
      typename R5::const_iterator, typename R6::const_iterator,
      typename R7::const_iterator > const_iterator;
      typedef tuple8 < detail::reference_wrapper < typename R0::value_type >,
      detail::reference_wrapper < typename R1::value_type >,
      detail::reference_wrapper < typename R2::value_type >,
      detail::reference_wrapper < typename R3::value_type >,
      detail::reference_wrapper < typename R4::value_type >,
      detail::reference_wrapper < typename R5::value_type >,
      detail::reference_wrapper < typename R6::value_type >,
      detail::reference_wrapper < typename R7::value_type > > reference;
      typedef tuple8 < typename R0::value_type, typename R1::value_type,
      typename R2::value_type, typename R3::value_type,
      typename R4::value_type, typename R5::value_type,
      typename R6::value_type, typename R7::value_type > const_reference;
      typedef tuple8 < typename R0::pointer, typename R1::pointer,
      typename R2::pointer, typename R3::pointer, typename R4::pointer,
      typename R5::pointer, typename R6::pointer,
      typename R7::pointer > pointer;
      typedef tuple8 < typename R0::difference_type,
      typename R1::difference_type, typename R2::difference_type,
      typename R3::difference_type, typename R4::difference_type,
      typename R5::difference_type, typename R6::difference_type,
      typename R7::difference_type > difference_type;
    };
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6, class T7 >
    struct container_tuple8: public tuple8 < T0, T1, T2,
          T3, T4, T5, T6, T7 >
    {
      typedef container_tuple8 < T0, T1, T2, T3, T4, T5, T6, T7 > self_type;
      typedef tuple8 < typename T0::size_type, typename T1::size_type,
      typename T2::size_type, typename T3::size_type,
      typename T4::size_type, typename T5::size_type,
      typename T6::size_type, typename T7::size_type > size_type;
      typedef container_traits_tuple8 < T0, T1, T2, T3, T4, T5, T6,
      T7 > traits;
      typedef typename traits::value_type value_type;
      typedef typename traits::iterator iterator;
      typedef typename traits::const_iterator const_iterator;
      typedef typename traits::reference reference;
      typedef typename traits::const_reference const_reference;
      typedef typename traits::pointer pointer;
      typedef typename traits::difference_type difference_type;
      container_tuple8( arg_type0 theM0, arg_type1 theM1, arg_type2 theM2,
                        arg_type3 theM3, arg_type4 theM4, arg_type5 theM5,
                        arg_type6 theM6, arg_type7 theM7 ) : tuple8 < T0, T1,
          T2, T3, T4, T5, T6, T7 > ( theM0, theM1, theM2, theM3, theM4, theM5,
                                     theM6, theM7 )
      {}
      template < class S0, class S1, class S2, class S3, class S4, class S5,
      class S6, class S7 >
      container_tuple8( const container_tuple8 < S0,
                        S1, S2, S3, S4, S5, S6,
                        S7 > &rhs ) : tuple8 < T0, T1,
          T2, T3, T4, T5, T6, T7 > ( rhs )
      {}
      container_tuple8( arg_type0 theM0,
                        const tail_type & tail ) : tuple8 < T0, T1, T2, T3,
          T4, T5, T6, T7 > ( theM0, tail )
      {}
      tuple8 < bool, bool, bool, bool, bool, bool, bool, bool > empty( ) const
      {
        return tuple8 < bool, bool, bool, bool, bool, bool, bool,
               bool > ( m0.empty( ), m1.empty( ), m2.empty( ), m3.empty( ),
                        m4.empty( ), m5.empty( ), m6.empty( ), m7.empty( ) );
      }
      size_type size( ) const
      {
        return size_type( m0.size( ), m1.size( ), m2.size( ), m3.size( ),
                          m4.size( ), m5.size( ), m6.size( ),
                          m7.size( ) );
      }
      iterator begin( )
      {
        return iterator( m0.begin( ), m1.begin( ), m2.begin( ),
                         m3.begin( ), m4.begin( ), m5.begin( ),
                         m6.begin( ), m7.begin( ) );
      }
      const_iterator begin( ) const
      {
        return const_iterator( m0.begin( ), m1.begin( ), m2.begin( ),
                               m3.begin( ), m4.begin( ), m5.begin( ),
                               m6.begin( ), m7.begin( ) );
      }
      iterator end( )
      {
        return iterator( m0.end( ), m1.end( ), m2.end( ), m3.end( ),
                         m4.end( ), m5.end( ), m6.end( ), m7.end( ) );
      }
      const_iterator end( ) const
      {
        return const_iterator( m0.end( ), m1.end( ), m2.end( ), m3.end( ),
                               m4.end( ), m5.end( ), m6.end( ),
                               m7.end( ) );
      }
      reference operator[] ( const size_type & i )
      {
        return reference( detail::ref( m0[ i.m0 ] ), detail::ref( m1[ i.m1 ] ),
                          detail::ref( m2[ i.m2 ] ), detail::ref( m3[ i.m3 ] ),
                          detail::ref( m4[ i.m4 ] ), detail::ref( m5[ i.m5 ] ),
                          detail::ref( m6[ i.m6 ] ), detail::ref( m7[ i.m7 ] ) );
      }
      const_reference operator[] ( const size_type & i ) const
      {
        return const_reference( detail::cref( m0[ i.m0 ] ),
                                detail::cref( m1[ i.m1 ] ),
                                detail::cref( m2[ i.m2 ] ),
                                detail::cref( m3[ i.m3 ] ),
                                detail::cref( m4[ i.m4 ] ),
                                detail::cref( m5[ i.m5 ] ),
                                detail::cref( m6[ i.m6 ] ),
                                detail::cref( m7[ i.m7 ] ) );
      }
    };
    template < class R0, class R1, class R2, class R3, class R4, class R5,
    class R6, class R7, class R8 >
    struct container_traits_tuple9
    {
      typedef tuple9 < typename R0::value_type, typename R1::value_type,
      typename R2::value_type, typename R3::value_type,
      typename R4::value_type, typename R5::value_type,
      typename R6::value_type, typename R7::value_type,
      typename R8::value_type > value_type;
      typedef iterator_tuple9 < typename R0::iterator, typename R1::iterator,
      typename R2::iterator, typename R3::iterator, typename R4::iterator,
      typename R5::iterator, typename R6::iterator, typename R7::iterator,
      typename R8::iterator > iterator;
      typedef iterator_tuple9 < typename R0::const_iterator,
      typename R1::const_iterator, typename R2::const_iterator,
      typename R3::const_iterator, typename R4::const_iterator,
      typename R5::const_iterator, typename R6::const_iterator,
      typename R7::const_iterator,
      typename R8::const_iterator > const_iterator;
      typedef tuple9 < detail::reference_wrapper < typename R0::value_type >,
      detail::reference_wrapper < typename R1::value_type >,
      detail::reference_wrapper < typename R2::value_type >,
      detail::reference_wrapper < typename R3::value_type >,
      detail::reference_wrapper < typename R4::value_type >,
      detail::reference_wrapper < typename R5::value_type >,
      detail::reference_wrapper < typename R6::value_type >,
      detail::reference_wrapper < typename R7::value_type >,
      detail::reference_wrapper < typename R8::value_type > > reference;
      typedef tuple9 < typename R0::value_type, typename R1::value_type,
      typename R2::value_type, typename R3::value_type,
      typename R4::value_type, typename R5::value_type,
      typename R6::value_type, typename R7::value_type,
      typename R8::value_type > const_reference;
      typedef tuple9 < typename R0::pointer, typename R1::pointer,
      typename R2::pointer, typename R3::pointer, typename R4::pointer,
      typename R5::pointer, typename R6::pointer, typename R7::pointer,
      typename R8::pointer > pointer;
      typedef tuple9 < typename R0::difference_type,
      typename R1::difference_type, typename R2::difference_type,
      typename R3::difference_type, typename R4::difference_type,
      typename R5::difference_type, typename R6::difference_type,
      typename R7::difference_type,
      typename R8::difference_type > difference_type;
    };
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6, class T7,
    class T8 >
    struct container_tuple9: public tuple9 < T0, T1, T2, T3, T4,
          T5, T6, T7, T8 >
    {
      typedef container_tuple9 < T0, T1, T2, T3, T4, T5, T6, T7,
      T8 > self_type;
      typedef tuple9 < typename T0::size_type, typename T1::size_type,
      typename T2::size_type, typename T3::size_type,
      typename T4::size_type, typename T5::size_type,
      typename T6::size_type, typename T7::size_type,
      typename T8::size_type > size_type;
      typedef container_traits_tuple9 < T0, T1, T2, T3, T4, T5, T6, T7,
      T8 > traits;
      typedef typename traits::value_type value_type;
      typedef typename traits::iterator iterator;
      typedef typename traits::const_iterator const_iterator;
      typedef typename traits::reference reference;
      typedef typename traits::const_reference const_reference;
      typedef typename traits::pointer pointer;
      typedef typename traits::difference_type difference_type;
      container_tuple9( arg_type0 theM0, arg_type1 theM1, arg_type2 theM2,
                        arg_type3 theM3, arg_type4 theM4, arg_type5 theM5,
                        arg_type6 theM6, arg_type7 theM7,
                        arg_type8 theM8 ) : tuple9 < T0, T1, T2, T3, T4, T5,
          T6, T7, T8 > ( theM0, theM1, theM2, theM3, theM4, theM5, theM6, theM7,
                         theM8 )
      {}
      template < class S0, class S1, class S2, class S3, class S4, class S5,
      class S6, class S7,
      class S8 >
      container_tuple9( const container_tuple9 < S0, S1, S2, S3,
                        S4, S5, S6, S7, S8 > &rhs ) : tuple9 < T0,
          T1, T2, T3, T4, T5, T6, T7, T8 > ( rhs )
      {}
      container_tuple9( arg_type0 theM0,
                        const tail_type & tail ) : tuple9 < T0, T1, T2, T3,
          T4, T5, T6, T7, T8 > ( theM0, tail )
      {}
      tuple9 < bool, bool, bool, bool, bool, bool, bool, bool,
      bool > empty( ) const
      {
        return tuple9 < bool, bool, bool, bool, bool, bool, bool, bool,
               bool > ( m0.empty( ), m1.empty( ), m2.empty( ), m3.empty( ),
                        m4.empty( ), m5.empty( ), m6.empty( ), m7.empty( ),
                        m8.empty( ) );
      }
      size_type size( ) const
      {
        return size_type( m0.size( ), m1.size( ), m2.size( ), m3.size( ),
                          m4.size( ), m5.size( ), m6.size( ), m7.size( ),
                          m8.size( ) );
      }
      iterator begin( )
      {
        return iterator( m0.begin( ), m1.begin( ), m2.begin( ),
                         m3.begin( ), m4.begin( ), m5.begin( ),
                         m6.begin( ), m7.begin( ), m8.begin( ) );
      }
      const_iterator begin( ) const
      {
        return const_iterator( m0.begin( ), m1.begin( ), m2.begin( ),
                               m3.begin( ), m4.begin( ), m5.begin( ),
                               m6.begin( ), m7.begin( ), m8.begin( ) );
      }
      iterator end( )
      {
        return iterator( m0.end( ), m1.end( ), m2.end( ), m3.end( ),
                         m4.end( ), m5.end( ), m6.end( ), m7.end( ),
                         m8.end( ) );
      }
      const_iterator end( ) const
      {
        return const_iterator( m0.end( ), m1.end( ), m2.end( ), m3.end( ),
                               m4.end( ), m5.end( ), m6.end( ), m7.end( ),
                               m8.end( ) );
      }
      reference operator[] ( const size_type & i )
      {
        return reference( detail::ref( m0[ i.m0 ] ), detail::ref( m1[ i.m1 ] ),
                          detail::ref( m2[ i.m2 ] ), detail::ref( m3[ i.m3 ] ),
                          detail::ref( m4[ i.m4 ] ), detail::ref( m5[ i.m5 ] ),
                          detail::ref( m6[ i.m6 ] ), detail::ref( m7[ i.m7 ] ),
                          detail::ref( m8[ i.m8 ] ) );
      }
      const_reference operator[] ( const size_type & i ) const
      {
        return const_reference( detail::cref( m0[ i.m0 ] ),
                                detail::cref( m1[ i.m1 ] ),
                                detail::cref( m2[ i.m2 ] ),
                                detail::cref( m3[ i.m3 ] ),
                                detail::cref( m4[ i.m4 ] ),
                                detail::cref( m5[ i.m5 ] ),
                                detail::cref( m6[ i.m6 ] ),
                                detail::cref( m7[ i.m7 ] ),
                                detail::cref( m8[ i.m8 ] ) );
      }
    };
    namespace detail
    {
      template < class T >
      struct n_fold_container_helper
      {
        template < int N >
        struct cont_select
          {}
        ;
        template <>
        struct cont_select <0 >
        {
          typedef container_tuple0 < null_type > type;
        };
        template <>
        struct cont_select <1 >
        {
          typedef container_tuple1 < T > type;
          static type make( const T & arg )
          {
            return type( arg );
          }
        };
        template <>
        struct cont_select <2 >
        {
          typedef container_tuple2 < T, T > type;
          static type make( const T & arg )
          {
            return type( arg, arg );
          }
        };
        template <>
        struct cont_select <3 >
        {
          typedef container_tuple3 < T, T, T > type;
          static type make( const T & arg )
          {
            return type( arg, arg, arg );
          }
        };
        template <>
        struct cont_select <4 >
        {
          typedef container_tuple4 < T, T, T, T > type;
          static type make( const T & arg )
          {
            return type( arg, arg, arg, arg );
          }
        };
        template <>
        struct cont_select <5 >
        {
          typedef container_tuple5 < T, T, T, T, T > type;
          static type make( const T & arg )
          {
            return type( arg, arg, arg, arg, arg );
          }
        };
        template <>
        struct cont_select <6 >
        {
          typedef container_tuple6 < T, T, T, T, T, T > type;
          static type make( const T & arg )
          {
            return type( arg, arg, arg, arg, arg, arg );
          }
        };
        template <>
        struct cont_select <7 >
        {
          typedef container_tuple7 < T, T, T, T, T, T, T > type;
          static type make( const T & arg )
          {
            return type( arg, arg, arg, arg, arg, arg, arg );
          }
        };
        template <>
        struct cont_select <8 >
        {
          typedef container_tuple8 < T, T, T, T, T, T, T, T > type;
          static type make( const T & arg )
          {
            return type( arg, arg, arg, arg, arg, arg, arg, arg );
          }
        };
        template <>
        struct cont_select <9 >
        {
          typedef container_tuple9 < T, T, T, T, T, T, T, T, T > type;
          static type make( const T & arg )
          {
            return type( arg, arg, arg, arg, arg, arg, arg, arg, arg );
          }
        };
      };
    }                           // namespace detail

    template < class T, int N >
    struct n_fold_container_tuple
    {
      typedef detail::n_fold_container_helper < T >::cont_select <
      N > ::type type;

      static type make( const T & arg )
      {
        return detail::n_fold_container_helper < T >::cont_select <
               N > ::make( arg );
      }
    };







    namespace detail
    {
      // The initial case.
      template < class T0 >
      struct container_tuple_base_type_selector1
      {
        typedef::boost::mpl::if_c < ::boost::is_same < T0, null_type >::value,
        container_tuple0 < null_type >,
        container_tuple1 < T0 > > ::type type;
      };

      template < class T0, class T1 >
      struct container_tuple_base_type_selector2
      {
        typedef::boost::mpl::if_c < ::boost::is_same < T1, null_type >::value,
        container_tuple_base_type_selector1 < T0 >::type,
        container_tuple2 < T0, T1 > > ::type type;
      };
      template < class T0, class T1, class T2 >
      struct container_tuple_base_type_selector3
      {
        typedef::boost::mpl::if_c < ::boost::is_same < T2, null_type >::value,
        container_tuple_base_type_selector2 < T0, T1 >::type,
        container_tuple3 < T0, T1, T2 > > ::type type;
      };
      template < class T0, class T1, class T2, class T3 >
      struct container_tuple_base_type_selector4
      {
        typedef::boost::mpl::if_c < ::boost::is_same < T3, null_type >::value,
        container_tuple_base_type_selector3 < T0, T1, T2 >::type,
        container_tuple4 < T0, T1, T2, T3 > > ::type type;
      };
      template < class T0, class T1, class T2, class T3, class T4 >
      struct container_tuple_base_type_selector5
      {
        typedef::boost::mpl::if_c < ::boost::is_same < T4, null_type >::value,
        container_tuple_base_type_selector4 < T0, T1, T2, T3 >::type,
        container_tuple5 < T0, T1, T2, T3, T4 > > ::type type;
      };
      template < class T0, class T1, class T2, class T3, class T4,
      class T5 >
      struct container_tuple_base_type_selector6
      {
        typedef::boost::mpl::if_c < ::boost::is_same < T5, null_type >::value,
        container_tuple_base_type_selector5 < T0, T1, T2, T3, T4 >::type,
        container_tuple6 < T0, T1, T2, T3, T4, T5 > > ::type type;
      };
      template < class T0, class T1, class T2, class T3, class T4, class T5,
      class T6 >
      struct container_tuple_base_type_selector7
      {
        typedef::boost::mpl::if_c < ::boost::is_same < T6, null_type >::value,
        container_tuple_base_type_selector6 < T0, T1, T2, T3, T4,
        T5 > ::type, container_tuple7 < T0, T1, T2, T3, T4, T5,
        T6 > > ::type type;
      };
      template < class T0, class T1, class T2, class T3, class T4, class T5,
      class T6, class T7 >
      struct container_tuple_base_type_selector8
      {
        typedef::boost::mpl::if_c < ::boost::is_same < T7, null_type >::value,
        container_tuple_base_type_selector7 < T0, T1, T2, T3, T4, T5,
        T6 > ::type, container_tuple8 < T0, T1, T2, T3, T4, T5, T6,
        T7 > > ::type type;
      };
      template < class T0, class T1, class T2, class T3, class T4, class T5,
      class T6, class T7,
      class T8 >
      struct container_tuple_base_type_selector9
      {
        typedef::boost::mpl::if_c < ::boost::is_same < T8, null_type >::value,
        container_tuple_base_type_selector8 < T0, T1, T2, T3, T4, T5, T6,
        T7 > ::type, container_tuple9 < T0, T1, T2, T3, T4, T5, T6, T7,
        T8 > > ::type type;
      };

    }                           // namespace detail
    template < class T0 = null_type, class T1 = null_type, class T2 =
    null_type, class T3 = null_type, class T4 = null_type, class T5 =
    null_type, class T6 = null_type, class T7 = null_type, class T8 =
    null_type >
    struct container_tuple: public detail::
          container_tuple_base_type_selector9 < T0, T1, T2, T3, T4, T5, T6, T7,
          T8 > ::type
    {
      typedef detail::container_tuple_base_type_selector9 < T0, T1, T2, T3,
      T4, T5, T6, T7, T8 > ::type base_type;

      container_tuple( ) : self_type( )
      {}

      template < class V0 >
      container_tuple( V0 v0 ) : self_type( v0 )
      {}
      template < class V0, class V1 >
      container_tuple( V0 v0, V1 v1 ) : self_type( v0,
              v1 )
      {}
      template < class V0, class V1, class V2 >
      container_tuple( V0 v0, V1 v1, V2 v2 ) : self_type( v0, v1,
              v2 )
      {}
      template < class V0, class V1, class V2, class V3 >
      container_tuple( V0 v0, V1 v1, V2 v2, V3 v3 ) : self_type( v0, v1, v2,
              v3 )
      {}
      template < class V0, class V1, class V2, class V3, class V4 >
      container_tuple( V0 v0, V1 v1, V2 v2, V3 v3, V4 v4 ) : self_type( v0, v1, v2, v3,
              v4 )
      {}
      template < class V0, class V1, class V2, class V3, class V4, class V5 >
      container_tuple( V0 v0, V1 v1, V2 v2, V3 v3, V4 v4, V5 v5 ) : self_type( v0, v1, v2, v3, v4,
              v5 )
      {}
      template < class V0, class V1, class V2, class V3, class V4, class V5, class V6 >
      container_tuple( V0 v0, V1 v1, V2 v2, V3 v3, V4 v4, V5 v5, V6 v6 ) : self_type( v0, v1, v2, v3, v4, v5,
              v6 )
      {}
      template < class V0, class V1, class V2, class V3, class V4, class V5, class V6, class V7 >
      container_tuple( V0 v0, V1 v1, V2 v2, V3 v3, V4 v4, V5 v5, V6 v6, V7 v7 ) : self_type( v0, v1, v2, v3, v4, v5, v6,
              v7 )
      {}
      template < class V0, class V1, class V2, class V3, class V4, class V5, class V6, class V7, class V8 >
      container_tuple( V0 v0, V1 v1, V2 v2, V3 v3, V4 v4, V5 v5, V6 v6, V7 v7, V8 v8 ) : self_type( v0, v1, v2, v3, v4, v5, v6, v7,
              v8 )
      {}

      template < class S0, class S1, class S2, class S3, class S4, class S5,
      class S6, class S7,
      class S8 >
      base_type & operator=( const container_tuple < S0, S1, S2,
                             S3, S4, S5, S6, S7, S8 > &rhs )
      {
        //BOOST_STATIC_ASSERT(  );
        return ( base_type::operator=( rhs ) );
      }
    };



  }                             // namespace tupple
}                               // namespace boost


#endif
