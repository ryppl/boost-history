// Copyright (C) 2001-2003 Roland Richter <roland@flll.jku.at>
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all copies.
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.
#ifndef BOOST_TUPPLE_DETAIL_CONTAINER_TUPPLE_PART_SPEC_HPP
#define BOOST_TUPPLE_DETAIL_CONTAINER_TUPPLE_PART_SPEC_HPP
#include <boost/tupple/tupple.hpp>
#include <boost/tupple/detail/ref_workaround.hpp>
#include <boost/tupple/detail/iterator_tupple_traits_detail.hpp>
#include <boost/tupple/iterator_tupple.hpp>


namespace boost
{
  namespace tupple
  {
    template < class T0 = null_type, class T1 = null_type, class T2 =
    null_type, class T3 = null_type, class T4 = null_type, class T5 =
    null_type, class T6 = null_type, class T7 = null_type, class T8 =
    null_type, class T9 = null_type >
    struct container_tuple
      {}
    ;
    template < class R0 = null_type, class R1 = null_type, class R2 =
    null_type, class R3 = null_type, class R4 = null_type, class R5 =
    null_type, class R6 = null_type, class R7 = null_type, class R8 =
    null_type, class R9 = null_type >
    struct container_traits_tuple
      {}
    ;



    template <>
    struct container_tuple < null_type, null_type, null_type,
          null_type, null_type, null_type, null_type, null_type, null_type,
          null_type > : public tuple < null_type >
      {}
    ;
    template < class R0 >
    struct container_traits_tuple < R0, null_type,
          null_type, null_type, null_type, null_type, null_type, null_type,
          null_type, null_type >
    {
      typedef tuple < typename R0::value_type > value_type;
      typedef iterator_tuple < typename R0::iterator > iterator;
      typedef iterator_tuple < typename R0::const_iterator > const_iterator;
      typedef tuple < typename R0::reference > reference;
      typedef tuple < typename R0::const_reference > const_reference;
      typedef tuple < typename R0::pointer > pointer;
      typedef tuple < typename R0::difference_type > difference_type;
    };
    template < class T0 >
    struct container_tuple < T0, null_type, null_type,
          null_type, null_type, null_type, null_type, null_type, null_type,
          null_type > : public tuple < T0 >
    {
      typedef container_tuple < T0 > self_type;
      typedef tuple < typename T0::size_type > size_type;
      typedef container_traits_tuple < T0 > traits;
      typedef typename traits::value_type value_type;
      typedef typename traits::iterator iterator;
      typedef typename traits::const_iterator const_iterator;
      typedef typename traits::reference reference;
      typedef typename traits::const_reference const_reference;
      typedef typename traits::pointer pointer;
      typedef typename traits::difference_type difference_type;
      container_tuple( arg_type0 theM0 ) : tuple < T0 > ( theM0 )
      {}
      template < class S0 >
      container_tuple( const container_tuple < S0 >
                       &rhs ) : tuple < T0 > ( rhs )
      {}
      container_tuple( arg_type0 theM0,
                       const tail_type & tail ) : tuple < T0 > ( theM0, tail )
      {}
      tuple < bool > empty( ) const
      {
        return tuple < bool > ( m0.empty( ) );
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
        return reference( m0[ i.m0 ] );
      }
      const_reference operator[] ( const size_type & i ) const
      {
        return const_reference( m0[ i.m0 ] );
      }
    };
    template < class R0, class R1 >
    struct container_traits_tuple < R0, R1,
          null_type, null_type, null_type, null_type, null_type, null_type,
          null_type, null_type >
    {
      typedef tuple < typename R0::value_type,
      typename R1::value_type > value_type;
      typedef iterator_tuple < typename R0::iterator,
      typename R1::iterator > iterator;
      typedef iterator_tuple < typename R0::const_iterator,
      typename R1::const_iterator > const_iterator;
      typedef tuple < typename R0::reference,
      typename R1::reference > reference;
      typedef tuple < typename R0::const_reference,
      typename R1::const_reference > const_reference;
      typedef tuple < typename R0::pointer, typename R1::pointer > pointer;
      typedef tuple < typename R0::difference_type,
      typename R1::difference_type > difference_type;
    };
    template < class T0, class T1 >
    struct container_tuple < T0, T1, null_type,
          null_type, null_type, null_type, null_type, null_type, null_type,
          null_type > : public tuple < T0, T1 >
    {
      typedef container_tuple < T0, T1 > self_type;
      typedef tuple < typename T0::size_type,
      typename T1::size_type > size_type;
      typedef container_traits_tuple < T0, T1 > traits;
      typedef typename traits::value_type value_type;
      typedef typename traits::iterator iterator;
      typedef typename traits::const_iterator const_iterator;
      typedef typename traits::reference reference;
      typedef typename traits::const_reference const_reference;
      typedef typename traits::pointer pointer;
      typedef typename traits::difference_type difference_type;
      container_tuple( arg_type0 theM0, arg_type1 theM1 ) : tuple < T0,
          T1 > ( theM0, theM1 )
      {}
      template < class S0,
      class S1 >
      container_tuple( const container_tuple < S0,
                       S1 > &rhs ) : tuple < T0, T1 > ( rhs )
      {}
      container_tuple( arg_type0 theM0, const tail_type & tail ) : tuple < T0,
          T1 > ( theM0, tail )
      {}
      tuple < bool, bool > empty( ) const
      {
        return tuple < bool, bool > ( m0.empty( ), m1.empty( ) );
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
        return reference( m0[ i.m0 ], m1[ i.m1 ] );
      }
      const_reference operator[] ( const size_type & i ) const
      {
        return const_reference( m0[ i.m0 ], m1[ i.m1 ] );
      }
    };
    template < class R0, class R1,
    class R2 >
    struct container_traits_tuple < R0, R1, R2, null_type,
          null_type, null_type, null_type, null_type, null_type, null_type >
    {
      typedef tuple < typename R0::value_type, typename R1::value_type,
      typename R2::value_type > value_type;
      typedef iterator_tuple < typename R0::iterator, typename R1::iterator,
      typename R2::iterator > iterator;
      typedef iterator_tuple < typename R0::const_iterator,
      typename R1::const_iterator,
      typename R2::const_iterator > const_iterator;
      typedef tuple < typename R0::reference, typename R1::reference,
      typename R2::reference > reference;
      typedef tuple < typename R0::const_reference,
      typename R1::const_reference,
      typename R2::const_reference > const_reference;
      typedef tuple < typename R0::pointer, typename R1::pointer,
      typename R2::pointer > pointer;
      typedef tuple < typename R0::difference_type,
      typename R1::difference_type,
      typename R2::difference_type > difference_type;
    };
    template < class T0, class T1, class T2 >
    struct container_tuple < T0, T1,
          T2, null_type, null_type, null_type, null_type, null_type, null_type,
          null_type > : public tuple < T0, T1, T2 >
    {
      typedef container_tuple < T0, T1, T2 > self_type;
      typedef tuple < typename T0::size_type, typename T1::size_type,
      typename T2::size_type > size_type;
      typedef container_traits_tuple < T0, T1, T2 > traits;
      typedef typename traits::value_type value_type;
      typedef typename traits::iterator iterator;
      typedef typename traits::const_iterator const_iterator;
      typedef typename traits::reference reference;
      typedef typename traits::const_reference const_reference;
      typedef typename traits::pointer pointer;
      typedef typename traits::difference_type difference_type;
      container_tuple( arg_type0 theM0, arg_type1 theM1,
                       arg_type2 theM2 ) : tuple < T0, T1, T2 > ( theM0,
                               theM1,
                               theM2 )
      {}
      template < class S0, class S1,
      class S2 >
      container_tuple( const container_tuple < S0, S1,
                       S2 > &rhs ) : tuple < T0, T1, T2 > ( rhs )
      {}
      container_tuple( arg_type0 theM0, const tail_type & tail ) : tuple < T0,
          T1, T2 > ( theM0, tail )
      {}
      tuple < bool, bool, bool > empty( ) const
      {
        return tuple < bool, bool, bool > ( m0.empty( ), m1.empty( ),
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
        return reference( m0[ i.m0 ], m1[ i.m1 ], m2[ i.m2 ] );
      }
      const_reference operator[] ( const size_type & i ) const
      {
        return const_reference( m0[ i.m0 ], m1[ i.m1 ], m2[ i.m2 ] );
      }
    };
    template < class R0, class R1, class R2,
    class R3 >
    struct container_traits_tuple < R0, R1, R2, R3, null_type,
          null_type, null_type, null_type, null_type, null_type >
    {
      typedef tuple < typename R0::value_type, typename R1::value_type,
      typename R2::value_type, typename R3::value_type > value_type;
      typedef iterator_tuple < typename R0::iterator, typename R1::iterator,
      typename R2::iterator, typename R3::iterator > iterator;
      typedef iterator_tuple < typename R0::const_iterator,
      typename R1::const_iterator, typename R2::const_iterator,
      typename R3::const_iterator > const_iterator;
      typedef tuple < typename R0::reference, typename R1::reference,
      typename R2::reference, typename R3::reference > reference;
      typedef tuple < typename R0::const_reference,
      typename R1::const_reference, typename R2::const_reference,
      typename R3::const_reference > const_reference;
      typedef tuple < typename R0::pointer, typename R1::pointer,
      typename R2::pointer, typename R3::pointer > pointer;
      typedef tuple < typename R0::difference_type,
      typename R1::difference_type, typename R2::difference_type,
      typename R3::difference_type > difference_type;
    };
    template < class T0, class T1, class T2,
    class T3 >
    struct container_tuple < T0, T1, T2, T3, null_type, null_type,
          null_type, null_type, null_type, null_type > : public tuple < T0, T1, T2,
          T3 >
    {
      typedef container_tuple < T0, T1, T2, T3 > self_type;
      typedef tuple < typename T0::size_type, typename T1::size_type,
      typename T2::size_type, typename T3::size_type > size_type;
      typedef container_traits_tuple < T0, T1, T2, T3 > traits;
      typedef typename traits::value_type value_type;
      typedef typename traits::iterator iterator;
      typedef typename traits::const_iterator const_iterator;
      typedef typename traits::reference reference;
      typedef typename traits::const_reference const_reference;
      typedef typename traits::pointer pointer;
      typedef typename traits::difference_type difference_type;
      container_tuple( arg_type0 theM0, arg_type1 theM1, arg_type2 theM2,
                       arg_type3 theM3 ) : tuple < T0, T1, T2, T3 > ( theM0,
                               theM1,
                               theM2,
                               theM3 )
      {}
      template < class S0, class S1, class S2,
      class S3 >
      container_tuple( const container_tuple < S0, S1, S2,
                       S3 > &rhs ) : tuple < T0, T1, T2,
          T3 > ( rhs )
      {}
      container_tuple( arg_type0 theM0, const tail_type & tail ) : tuple < T0,
          T1, T2, T3 > ( theM0, tail )
      {}
      tuple < bool, bool, bool, bool > empty( ) const
      {
        return tuple < bool, bool, bool, bool > ( m0.empty( ), m1.empty( ),
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
        return reference( m0[ i.m0 ], m1[ i.m1 ], m2[ i.m2 ], m3[ i.m3 ] );
      }
      const_reference operator[] ( const size_type & i ) const
      {
        return const_reference( m0[ i.m0 ], m1[ i.m1 ], m2[ i.m2 ], m3[ i.m3 ] );
      }
    };
    template < class R0, class R1, class R2, class R3,
    class R4 >
    struct container_traits_tuple < R0, R1, R2, R3, R4, null_type,
          null_type, null_type, null_type, null_type >
    {
      typedef tuple < typename R0::value_type, typename R1::value_type,
      typename R2::value_type, typename R3::value_type,
      typename R4::value_type > value_type;
      typedef iterator_tuple < typename R0::iterator, typename R1::iterator,
      typename R2::iterator, typename R3::iterator,
      typename R4::iterator > iterator;
      typedef iterator_tuple < typename R0::const_iterator,
      typename R1::const_iterator, typename R2::const_iterator,
      typename R3::const_iterator,
      typename R4::const_iterator > const_iterator;
      typedef tuple < typename R0::reference, typename R1::reference,
      typename R2::reference, typename R3::reference,
      typename R4::reference > reference;
      typedef tuple < typename R0::const_reference,
      typename R1::const_reference, typename R2::const_reference,
      typename R3::const_reference,
      typename R4::const_reference > const_reference;
      typedef tuple < typename R0::pointer, typename R1::pointer,
      typename R2::pointer, typename R3::pointer,
      typename R4::pointer > pointer;
      typedef tuple < typename R0::difference_type,
      typename R1::difference_type, typename R2::difference_type,
      typename R3::difference_type,
      typename R4::difference_type > difference_type;
    };
    template < class T0, class T1, class T2, class T3,
    class T4 >
    struct container_tuple < T0, T1, T2, T3, T4, null_type,
          null_type, null_type, null_type, null_type > : public tuple < T0, T1, T2,
          T3, T4 >
    {
      typedef container_tuple < T0, T1, T2, T3, T4 > self_type;
      typedef tuple < typename T0::size_type, typename T1::size_type,
      typename T2::size_type, typename T3::size_type,
      typename T4::size_type > size_type;
      typedef container_traits_tuple < T0, T1, T2, T3, T4 > traits;
      typedef typename traits::value_type value_type;
      typedef typename traits::iterator iterator;
      typedef typename traits::const_iterator const_iterator;
      typedef typename traits::reference reference;
      typedef typename traits::const_reference const_reference;
      typedef typename traits::pointer pointer;
      typedef typename traits::difference_type difference_type;
      container_tuple( arg_type0 theM0, arg_type1 theM1, arg_type2 theM2,
                       arg_type3 theM3, arg_type4 theM4 ) : tuple < T0, T1,
          T2, T3, T4 > ( theM0, theM1, theM2, theM3, theM4 )
      {}
      template < class S0, class S1, class S2, class S3,
      class S4 >
      container_tuple( const container_tuple < S0, S1, S2, S3,
                       S4 > &rhs ) : tuple < T0, T1, T2, T3,
          T4 > ( rhs )
      {}
      container_tuple( arg_type0 theM0, const tail_type & tail ) : tuple < T0,
          T1, T2, T3, T4 > ( theM0, tail )
      {}
      tuple < bool, bool, bool, bool, bool > empty( ) const
      {
        return tuple < bool, bool, bool, bool, bool > ( m0.empty( ),
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
        return reference( m0[ i.m0 ], m1[ i.m1 ], m2[ i.m2 ], m3[ i.m3 ], m4[ i.m4 ] );
      }
      const_reference operator[] ( const size_type & i ) const
      {
        return const_reference( m0[ i.m0 ], m1[ i.m1 ], m2[ i.m2 ], m3[ i.m3 ],
                                m4[ i.m4 ] );
      }
    };
    template < class R0, class R1, class R2, class R3, class R4,
    class R5 >
    struct container_traits_tuple < R0, R1, R2, R3, R4, R5,
          null_type, null_type, null_type, null_type >
    {
      typedef tuple < typename R0::value_type, typename R1::value_type,
      typename R2::value_type, typename R3::value_type,
      typename R4::value_type, typename R5::value_type > value_type;
      typedef iterator_tuple < typename R0::iterator, typename R1::iterator,
      typename R2::iterator, typename R3::iterator, typename R4::iterator,
      typename R5::iterator > iterator;
      typedef iterator_tuple < typename R0::const_iterator,
      typename R1::const_iterator, typename R2::const_iterator,
      typename R3::const_iterator, typename R4::const_iterator,
      typename R5::const_iterator > const_iterator;
      typedef tuple < typename R0::reference, typename R1::reference,
      typename R2::reference, typename R3::reference,
      typename R4::reference, typename R5::reference > reference;
      typedef tuple < typename R0::const_reference,
      typename R1::const_reference, typename R2::const_reference,
      typename R3::const_reference, typename R4::const_reference,
      typename R5::const_reference > const_reference;
      typedef tuple < typename R0::pointer, typename R1::pointer,
      typename R2::pointer, typename R3::pointer, typename R4::pointer,
      typename R5::pointer > pointer;
      typedef tuple < typename R0::difference_type,
      typename R1::difference_type, typename R2::difference_type,
      typename R3::difference_type, typename R4::difference_type,
      typename R5::difference_type > difference_type;
    };
    template < class T0, class T1, class T2, class T3, class T4,
    class T5 >
    struct container_tuple < T0, T1, T2, T3, T4, T5, null_type,
          null_type, null_type, null_type > : public tuple < T0, T1, T2, T3, T4,
          T5 >
    {
      typedef container_tuple < T0, T1, T2, T3, T4, T5 > self_type;
      typedef tuple < typename T0::size_type, typename T1::size_type,
      typename T2::size_type, typename T3::size_type,
      typename T4::size_type, typename T5::size_type > size_type;
      typedef container_traits_tuple < T0, T1, T2, T3, T4, T5 > traits;
      typedef typename traits::value_type value_type;
      typedef typename traits::iterator iterator;
      typedef typename traits::const_iterator const_iterator;
      typedef typename traits::reference reference;
      typedef typename traits::const_reference const_reference;
      typedef typename traits::pointer pointer;
      typedef typename traits::difference_type difference_type;
      container_tuple( arg_type0 theM0, arg_type1 theM1, arg_type2 theM2,
                       arg_type3 theM3, arg_type4 theM4,
                       arg_type5 theM5 ) : tuple < T0, T1, T2, T3, T4,
          T5 > ( theM0, theM1, theM2, theM3, theM4, theM5 )
      {}
      template < class S0, class S1, class S2, class S3, class S4,
      class S5 >
      container_tuple( const container_tuple < S0, S1, S2, S3,
                       S4, S5 > &rhs ) : tuple < T0, T1, T2, T3,
          T4, T5 > ( rhs )
      {}
      container_tuple( arg_type0 theM0, const tail_type & tail ) : tuple < T0,
          T1, T2, T3, T4, T5 > ( theM0, tail )
      {}
      tuple < bool, bool, bool, bool, bool, bool > empty( ) const
      {
        return tuple < bool, bool, bool, bool, bool, bool > ( m0.empty( ),
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
        return reference( m0[ i.m0 ], m1[ i.m1 ], m2[ i.m2 ], m3[ i.m3 ], m4[ i.m4 ],
                          m5[ i.m5 ] );
      }
      const_reference operator[] ( const size_type & i ) const
      {
        return const_reference( m0[ i.m0 ], m1[ i.m1 ], m2[ i.m2 ], m3[ i.m3 ],
                                m4[ i.m4 ], m5[ i.m5 ] );
      }
    };
    template < class R0, class R1, class R2, class R3, class R4, class R5,
    class R6 >
    struct container_traits_tuple < R0, R1, R2, R3, R4, R5, R6,
          null_type, null_type, null_type >
    {
      typedef tuple < typename R0::value_type, typename R1::value_type,
      typename R2::value_type, typename R3::value_type,
      typename R4::value_type, typename R5::value_type,
      typename R6::value_type > value_type;
      typedef iterator_tuple < typename R0::iterator, typename R1::iterator,
      typename R2::iterator, typename R3::iterator, typename R4::iterator,
      typename R5::iterator, typename R6::iterator > iterator;
      typedef iterator_tuple < typename R0::const_iterator,
      typename R1::const_iterator, typename R2::const_iterator,
      typename R3::const_iterator, typename R4::const_iterator,
      typename R5::const_iterator,
      typename R6::const_iterator > const_iterator;
      typedef tuple < typename R0::reference, typename R1::reference,
      typename R2::reference, typename R3::reference,
      typename R4::reference, typename R5::reference,
      typename R6::reference > reference;
      typedef tuple < typename R0::const_reference,
      typename R1::const_reference, typename R2::const_reference,
      typename R3::const_reference, typename R4::const_reference,
      typename R5::const_reference,
      typename R6::const_reference > const_reference;
      typedef tuple < typename R0::pointer, typename R1::pointer,
      typename R2::pointer, typename R3::pointer, typename R4::pointer,
      typename R5::pointer, typename R6::pointer > pointer;
      typedef tuple < typename R0::difference_type,
      typename R1::difference_type, typename R2::difference_type,
      typename R3::difference_type, typename R4::difference_type,
      typename R5::difference_type,
      typename R6::difference_type > difference_type;
    };
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6 >
    struct container_tuple < T0, T1, T2, T3, T4, T5, T6,
          null_type, null_type, null_type > : public tuple < T0, T1, T2, T3, T4, T5,
          T6 >
    {
      typedef container_tuple < T0, T1, T2, T3, T4, T5, T6 > self_type;
      typedef tuple < typename T0::size_type, typename T1::size_type,
      typename T2::size_type, typename T3::size_type,
      typename T4::size_type, typename T5::size_type,
      typename T6::size_type > size_type;
      typedef container_traits_tuple < T0, T1, T2, T3, T4, T5, T6 > traits;
      typedef typename traits::value_type value_type;
      typedef typename traits::iterator iterator;
      typedef typename traits::const_iterator const_iterator;
      typedef typename traits::reference reference;
      typedef typename traits::const_reference const_reference;
      typedef typename traits::pointer pointer;
      typedef typename traits::difference_type difference_type;
      container_tuple( arg_type0 theM0, arg_type1 theM1, arg_type2 theM2,
                       arg_type3 theM3, arg_type4 theM4, arg_type5 theM5,
                       arg_type6 theM6 ) : tuple < T0, T1, T2, T3, T4, T5,
          T6 > ( theM0, theM1, theM2, theM3, theM4, theM5, theM6 )
      {}
      template < class S0, class S1, class S2, class S3, class S4, class S5,
      class S6 >
      container_tuple( const container_tuple < S0, S1, S2, S3,
                       S4, S5, S6 > &rhs ) : tuple < T0, T1, T2,
          T3, T4, T5, T6 > ( rhs )
      {}
      container_tuple( arg_type0 theM0, const tail_type & tail ) : tuple < T0,
          T1, T2, T3, T4, T5, T6 > ( theM0, tail )
      {}
      tuple < bool, bool, bool, bool, bool, bool, bool > empty( ) const
      {
        return tuple < bool, bool, bool, bool, bool, bool,
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
        return reference( m0[ i.m0 ], m1[ i.m1 ], m2[ i.m2 ], m3[ i.m3 ], m4[ i.m4 ],
                          m5[ i.m5 ], m6[ i.m6 ] );
      }
      const_reference operator[] ( const size_type & i ) const
      {
        return const_reference( m0[ i.m0 ], m1[ i.m1 ], m2[ i.m2 ], m3[ i.m3 ],
                                m4[ i.m4 ], m5[ i.m5 ], m6[ i.m6 ] );
      }
    };
    template < class R0, class R1, class R2, class R3, class R4, class R5,
    class R6, class R7 >
    struct container_traits_tuple < R0, R1, R2, R3, R4,
          R5, R6, R7, null_type, null_type >
    {
      typedef tuple < typename R0::value_type, typename R1::value_type,
      typename R2::value_type, typename R3::value_type,
      typename R4::value_type, typename R5::value_type,
      typename R6::value_type, typename R7::value_type > value_type;
      typedef iterator_tuple < typename R0::iterator, typename R1::iterator,
      typename R2::iterator, typename R3::iterator, typename R4::iterator,
      typename R5::iterator, typename R6::iterator,
      typename R7::iterator > iterator;
      typedef iterator_tuple < typename R0::const_iterator,
      typename R1::const_iterator, typename R2::const_iterator,
      typename R3::const_iterator, typename R4::const_iterator,
      typename R5::const_iterator, typename R6::const_iterator,
      typename R7::const_iterator > const_iterator;
      typedef tuple < typename R0::reference, typename R1::reference,
      typename R2::reference, typename R3::reference,
      typename R4::reference, typename R5::reference,
      typename R6::reference, typename R7::reference > reference;
      typedef tuple < typename R0::const_reference,
      typename R1::const_reference, typename R2::const_reference,
      typename R3::const_reference, typename R4::const_reference,
      typename R5::const_reference, typename R6::const_reference,
      typename R7::const_reference > const_reference;
      typedef tuple < typename R0::pointer, typename R1::pointer,
      typename R2::pointer, typename R3::pointer, typename R4::pointer,
      typename R5::pointer, typename R6::pointer,
      typename R7::pointer > pointer;
      typedef tuple < typename R0::difference_type,
      typename R1::difference_type, typename R2::difference_type,
      typename R3::difference_type, typename R4::difference_type,
      typename R5::difference_type, typename R6::difference_type,
      typename R7::difference_type > difference_type;
    };
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6, class T7 >
    struct container_tuple < T0, T1, T2, T3, T4, T5, T6,
          T7, null_type, null_type > : public tuple < T0, T1, T2, T3, T4, T5, T6,
          T7 >
    {
      typedef container_tuple < T0, T1, T2, T3, T4, T5, T6, T7 > self_type;
      typedef tuple < typename T0::size_type, typename T1::size_type,
      typename T2::size_type, typename T3::size_type,
      typename T4::size_type, typename T5::size_type,
      typename T6::size_type, typename T7::size_type > size_type;
      typedef container_traits_tuple < T0, T1, T2, T3, T4, T5, T6,
      T7 > traits;
      typedef typename traits::value_type value_type;
      typedef typename traits::iterator iterator;
      typedef typename traits::const_iterator const_iterator;
      typedef typename traits::reference reference;
      typedef typename traits::const_reference const_reference;
      typedef typename traits::pointer pointer;
      typedef typename traits::difference_type difference_type;
      container_tuple( arg_type0 theM0, arg_type1 theM1, arg_type2 theM2,
                       arg_type3 theM3, arg_type4 theM4, arg_type5 theM5,
                       arg_type6 theM6, arg_type7 theM7 ) : tuple < T0, T1,
          T2, T3, T4, T5, T6, T7 > ( theM0, theM1, theM2, theM3, theM4, theM5,
                                     theM6, theM7 )
      {}
      template < class S0, class S1, class S2, class S3, class S4, class S5,
      class S6, class S7 >
      container_tuple( const container_tuple < S0, S1,
                       S2, S3, S4, S5, S6,
                       S7 > &rhs ) : tuple < T0, T1, T2,
          T3, T4, T5, T6, T7 > ( rhs )
      {}
      container_tuple( arg_type0 theM0, const tail_type & tail ) : tuple < T0,
          T1, T2, T3, T4, T5, T6, T7 > ( theM0, tail )
      {}
      tuple < bool, bool, bool, bool, bool, bool, bool, bool > empty( ) const
      {
        return tuple < bool, bool, bool, bool, bool, bool, bool,
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
        return reference( m0[ i.m0 ], m1[ i.m1 ], m2[ i.m2 ], m3[ i.m3 ], m4[ i.m4 ],
                          m5[ i.m5 ], m6[ i.m6 ], m7[ i.m7 ] );
      }
      const_reference operator[] ( const size_type & i ) const
      {
        return const_reference( m0[ i.m0 ], m1[ i.m1 ], m2[ i.m2 ], m3[ i.m3 ],
                                m4[ i.m4 ], m5[ i.m5 ], m6[ i.m6 ], m7[ i.m7 ] );
      }
    };
    template < class R0, class R1, class R2, class R3, class R4, class R5,
    class R6, class R7, class R8 >
    struct container_traits_tuple < R0, R1,
          R2, R3, R4, R5, R6, R7, R8, null_type >
    {
      typedef tuple < typename R0::value_type, typename R1::value_type,
      typename R2::value_type, typename R3::value_type,
      typename R4::value_type, typename R5::value_type,
      typename R6::value_type, typename R7::value_type,
      typename R8::value_type > value_type;
      typedef iterator_tuple < typename R0::iterator, typename R1::iterator,
      typename R2::iterator, typename R3::iterator, typename R4::iterator,
      typename R5::iterator, typename R6::iterator, typename R7::iterator,
      typename R8::iterator > iterator;
      typedef iterator_tuple < typename R0::const_iterator,
      typename R1::const_iterator, typename R2::const_iterator,
      typename R3::const_iterator, typename R4::const_iterator,
      typename R5::const_iterator, typename R6::const_iterator,
      typename R7::const_iterator,
      typename R8::const_iterator > const_iterator;
      typedef tuple < typename R0::reference, typename R1::reference,
      typename R2::reference, typename R3::reference,
      typename R4::reference, typename R5::reference,
      typename R6::reference, typename R7::reference,
      typename R8::reference > reference;
      typedef tuple < typename R0::const_reference,
      typename R1::const_reference, typename R2::const_reference,
      typename R3::const_reference, typename R4::const_reference,
      typename R5::const_reference, typename R6::const_reference,
      typename R7::const_reference,
      typename R8::const_reference > const_reference;
      typedef tuple < typename R0::pointer, typename R1::pointer,
      typename R2::pointer, typename R3::pointer, typename R4::pointer,
      typename R5::pointer, typename R6::pointer, typename R7::pointer,
      typename R8::pointer > pointer;
      typedef tuple < typename R0::difference_type,
      typename R1::difference_type, typename R2::difference_type,
      typename R3::difference_type, typename R4::difference_type,
      typename R5::difference_type, typename R6::difference_type,
      typename R7::difference_type,
      typename R8::difference_type > difference_type;
    };
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6, class T7, class T8 >
    struct container_tuple < T0, T1, T2, T3,
          T4, T5, T6, T7, T8, null_type > : public tuple < T0, T1, T2, T3, T4, T5,
          T6, T7, T8 >
    {
      typedef container_tuple < T0, T1, T2, T3, T4, T5, T6, T7,
      T8 > self_type;
      typedef tuple < typename T0::size_type, typename T1::size_type,
      typename T2::size_type, typename T3::size_type,
      typename T4::size_type, typename T5::size_type,
      typename T6::size_type, typename T7::size_type,
      typename T8::size_type > size_type;
      typedef container_traits_tuple < T0, T1, T2, T3, T4, T5, T6, T7,
      T8 > traits;
      typedef typename traits::value_type value_type;
      typedef typename traits::iterator iterator;
      typedef typename traits::const_iterator const_iterator;
      typedef typename traits::reference reference;
      typedef typename traits::const_reference const_reference;
      typedef typename traits::pointer pointer;
      typedef typename traits::difference_type difference_type;
      container_tuple( arg_type0 theM0, arg_type1 theM1, arg_type2 theM2,
                       arg_type3 theM3, arg_type4 theM4, arg_type5 theM5,
                       arg_type6 theM6, arg_type7 theM7,
                       arg_type8 theM8 ) : tuple < T0, T1, T2, T3, T4, T5, T6,
          T7, T8 > ( theM0, theM1, theM2, theM3, theM4, theM5, theM6, theM7,
                     theM8 )
      {}
      template < class S0, class S1, class S2, class S3, class S4, class S5,
      class S6, class S7,
      class S8 >
      container_tuple( const container_tuple < S0, S1, S2, S3,
                       S4, S5, S6, S7, S8 > &rhs ) : tuple < T0,
          T1, T2, T3, T4, T5, T6, T7, T8 > ( rhs )
      {}
      container_tuple( arg_type0 theM0, const tail_type & tail ) : tuple < T0,
          T1, T2, T3, T4, T5, T6, T7, T8 > ( theM0, tail )
      {}
      tuple < bool, bool, bool, bool, bool, bool, bool, bool,
      bool > empty( ) const
      {
        return tuple < bool, bool, bool, bool, bool, bool, bool, bool,
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
        return reference( m0[ i.m0 ], m1[ i.m1 ], m2[ i.m2 ], m3[ i.m3 ], m4[ i.m4 ],
                          m5[ i.m5 ], m6[ i.m6 ], m7[ i.m7 ], m8[ i.m8 ] );
      }
      const_reference operator[] ( const size_type & i ) const
      {
        return const_reference( m0[ i.m0 ], m1[ i.m1 ], m2[ i.m2 ], m3[ i.m3 ],
                                m4[ i.m4 ], m5[ i.m5 ], m6[ i.m6 ], m7[ i.m7 ],
                                m8[ i.m8 ] );
      }
    };




    template < class T, int N >
    struct n_fold_container_tuple
      {}
    ;
    template < class T >
    struct n_fold_container_tuple <T, 0 >
    {
      typedef container_tuple <> type;
      static type make( const T & arg )
      {
        return type( );
      }
    };
    template < class T >
    struct n_fold_container_tuple <T, 1 >
    {
      typedef container_tuple < T > type;
      static type make( const T & arg )
      {
        return type( arg );
      }
    };
    template < class T >
    struct n_fold_container_tuple <T, 2 >
    {
      typedef container_tuple < T, T > type;
      static type make( const T & arg )
      {
        return type( arg, arg );
      }
    };
    template < class T >
    struct n_fold_container_tuple <T, 3 >
    {
      typedef container_tuple < T, T, T > type;
      static type make( const T & arg )
      {
        return type( arg, arg, arg );
      }
    };
    template < class T >
    struct n_fold_container_tuple <T, 4 >
    {
      typedef container_tuple < T, T, T, T > type;
      static type make( const T & arg )
      {
        return type( arg, arg, arg, arg );
      }
    };
    template < class T >
    struct n_fold_container_tuple <T, 5 >
    {
      typedef container_tuple < T, T, T, T, T > type;
      static type make( const T & arg )
      {
        return type( arg, arg, arg, arg, arg );
      }
    };
    template < class T >
    struct n_fold_container_tuple <T, 6 >
    {
      typedef container_tuple < T, T, T, T, T, T > type;
      static type make( const T & arg )
      {
        return type( arg, arg, arg, arg, arg, arg );
      }
    };
    template < class T >
    struct n_fold_container_tuple <T, 7 >
    {
      typedef container_tuple < T, T, T, T, T, T, T > type;
      static type make( const T & arg )
      {
        return type( arg, arg, arg, arg, arg, arg, arg );
      }
    };
    template < class T >
    struct n_fold_container_tuple <T, 8 >
    {
      typedef container_tuple < T, T, T, T, T, T, T, T > type;
      static type make( const T & arg )
      {
        return type( arg, arg, arg, arg, arg, arg, arg, arg );
      }
    };
    template < class T >
    struct n_fold_container_tuple <T, 9 >
    {
      typedef container_tuple < T, T, T, T, T, T, T, T, T > type;
      static type make( const T & arg )
      {
        return type( arg, arg, arg, arg, arg, arg, arg, arg, arg );
      }
    };
  }                             // namespace tupple
}                               // namespace boost


#endif
