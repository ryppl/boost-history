// Copyright (C) 2001,2002 Roland Richter <roland@flll.jku.at>
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all copies.
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.
#ifndef BOOST_TUPPLE_DETAIL_CONTAINER_TUPPLE_NO_PART_SPEC_HPP
#define BOOST_TUPPLE_DETAIL_CONTAINER_TUPPLE_NO_PART_SPEC_HPP
#include "boost/tupple/tupple.hpp"
#include "boost/tupple/detail/container_tupple_traits_detail.hpp"


namespace boost
{
  namespace tupple
  {
    template < class R0 > struct iterator_traits_tuple1
    {
      typedef tuple1 < typename R0::iterator_category > iterator_category;
      typedef tuple1 < typename R0::value_type > value_type;
      typedef tuple1 < detail::assign_to_pointee < typename R0::value_type >
        >reference;
      typedef tuple1 < typename R0::value_type > const_reference;
      typedef tuple1 < typename R0::pointer > pointer;
      typedef tuple1 < typename R0::difference_type > difference_type;
    };
      template < class T0, class R0 =
      detail::iterator_traits < T0 > >struct iterator_tuple1:public tuple1 <
      T0 >
    {
      typedef iterator_tuple1 < T0, R0 > self_type;
      typedef iterator_traits_tuple1 < R0 > traits;
      typedef typename traits::iterator_category iterator_category;
      typedef typename traits::value_type value_type;
      typedef typename traits::reference reference;
      typedef typename traits::const_reference const_reference;
      typedef typename traits::pointer pointer;
      typedef typename traits::difference_type difference_type;
        iterator_tuple1( arg_type0 theM0 ):tuple1 < T0 > ( theM0 )
      {
      }
      template < class S0 > iterator_tuple1( const iterator_tuple1 < S0 >
                                             &rhs ):tuple1 < T0 > ( rhs )
      {
      }
      iterator_tuple1( arg_type0 theM0,
                       const tail_type & tail ):tuple1 < T0 > ( theM0, tail )
      {
      }
      self_type & operator++(  )
      {
        ++m0;
        return *this;
      }
      const self_type operator++( int )
      {
        self_type old = *this;
        ++( *this );
        return old;
      }
      self_type & operator+=( const difference_type & rhs )
      {
        m0 += rhs.m0;
        return *this;
      }
      self_type & operator--(  )
      {
        --m0;
        return *this;
      }
      const self_type operator--( int )
      {
        self_type old = *this;
        --( *this );
        return old;
      }
      self_type & operator-=( const difference_type & rhs )
      {
        m0 -= rhs.m0;
        return *this;
      }
      reference operator*(  )
      {
        return reference( m0 );
      }
      const_reference operator*(  ) const
      {
        return const_reference( *m0 );
      }
      reference operator[] ( const difference_type & i )
      {
        return reference( &( m0[i.m0] ) );
      }
      const_reference operator[] ( const difference_type & i ) const
      {
        return const_reference( m0[i.m0] );
      }
    };
    template < class T0 > iterator_tuple1 < T0 >
      operator+( const iterator_tuple1 < T0 > &lhs,
                 const iterator_tuple1 < T0 >::difference_type & rhs )
    {
      return iterator_tuple1 < T0 > ( lhs ) += rhs;
    }
    template < class T0 > iterator_tuple1 < T0 >
      operator-( const iterator_tuple1 < T0 > &lhs,
                 const iterator_tuple1 < T0 >::difference_type & rhs )
    {
      return iterator_tuple1 < T0 > ( lhs ) -= rhs;
    }
    template < class T > struct n_fold_iterator1
    {
      typedef iterator_tuple1 < T > type;
    };
    template < class R0, class R1 > struct iterator_traits_tuple2
    {
      typedef tuple2 < typename R0::iterator_category,
        typename R1::iterator_category > iterator_category;
      typedef tuple2 < typename R0::value_type,
        typename R1::value_type > value_type;
      typedef tuple2 < detail::assign_to_pointee < typename R0::value_type >,
        detail::assign_to_pointee < typename R1::value_type > >reference;
      typedef tuple2 < typename R0::value_type,
        typename R1::value_type > const_reference;
      typedef tuple2 < typename R0::pointer, typename R1::pointer > pointer;
      typedef tuple2 < typename R0::difference_type,
        typename R1::difference_type > difference_type;
    };
    template < class T0, class T1, class R0 =
      detail::iterator_traits < T0 >, class R1 =
      detail::iterator_traits < T1 > >struct iterator_tuple2:public tuple2 <
      T0, T1 >
    {
      typedef iterator_tuple2 < T0, T1, R0, R1 > self_type;
      typedef iterator_traits_tuple2 < R0, R1 > traits;
      typedef typename traits::iterator_category iterator_category;
      typedef typename traits::value_type value_type;
      typedef typename traits::reference reference;
      typedef typename traits::const_reference const_reference;
      typedef typename traits::pointer pointer;
      typedef typename traits::difference_type difference_type;
        iterator_tuple2( arg_type0 theM0, arg_type1 theM1 ):tuple2 < T0,
        T1 > ( theM0, theM1 )
      {
      }
      template < class S0,
        class S1 > iterator_tuple2( const iterator_tuple2 < S0,
                                    S1 > &rhs ):tuple2 < T0, T1 > ( rhs )
      {
      }
      iterator_tuple2( arg_type0 theM0, const tail_type & tail ):tuple2 < T0,
        T1 > ( theM0, tail )
      {
      }
      self_type & operator++(  )
      {
        ++m0;
        ++m1;
        return *this;
      }
      const self_type operator++( int )
      {
        self_type old = *this;
        ++( *this );
        return old;
      }
      self_type & operator+=( const difference_type & rhs )
      {
        m0 += rhs.m0;
        m1 += rhs.m1;
        return *this;
      }
      self_type & operator--(  )
      {
        --m0;
        --m1;
        return *this;
      }
      const self_type operator--( int )
      {
        self_type old = *this;
        --( *this );
        return old;
      }
      self_type & operator-=( const difference_type & rhs )
      {
        m0 -= rhs.m0;
        m1 -= rhs.m1;
        return *this;
      }
      reference operator*(  )
      {
        return reference( m0, m1 );
      }
      const_reference operator*(  ) const
      {
        return const_reference( *m0, *m1 );
      }
      reference operator[] ( const difference_type & i )
      {
        return reference( &( m0[i.m0] ), &( m1[i.m1] ) );
      }
      const_reference operator[] ( const difference_type & i ) const
      {
        return const_reference( m0[i.m0], m1[i.m1] );
      }
    };
    template < class T0, class T1 > iterator_tuple2 < T0,
      T1 > operator+( const iterator_tuple2 < T0, T1 > &lhs,
                      const iterator_tuple2 < T0,
                      T1 >::difference_type & rhs )
    {
      return iterator_tuple2 < T0, T1 > ( lhs ) += rhs;
    }
    template < class T0, class T1 > iterator_tuple2 < T0,
      T1 > operator-( const iterator_tuple2 < T0, T1 > &lhs,
                      const iterator_tuple2 < T0,
                      T1 >::difference_type & rhs )
    {
      return iterator_tuple2 < T0, T1 > ( lhs ) -= rhs;
    }
    template < class T > struct n_fold_iterator2
    {
      typedef iterator_tuple2 < T, T > type;
    };
    template < class R0, class R1, class R2 > struct iterator_traits_tuple3
    {
      typedef tuple3 < typename R0::iterator_category,
        typename R1::iterator_category,
        typename R2::iterator_category > iterator_category;
      typedef tuple3 < typename R0::value_type, typename R1::value_type,
        typename R2::value_type > value_type;
      typedef tuple3 < detail::assign_to_pointee < typename R0::value_type >,
        detail::assign_to_pointee < typename R1::value_type >,
        detail::assign_to_pointee < typename R2::value_type > >reference;
      typedef tuple3 < typename R0::value_type, typename R1::value_type,
        typename R2::value_type > const_reference;
      typedef tuple3 < typename R0::pointer, typename R1::pointer,
        typename R2::pointer > pointer;
      typedef tuple3 < typename R0::difference_type,
        typename R1::difference_type,
        typename R2::difference_type > difference_type;
    };
    template < class T0, class T1, class T2, class R0 =
      detail::iterator_traits < T0 >, class R1 =
      detail::iterator_traits < T1 >, class R2 =
      detail::iterator_traits < T2 > >struct iterator_tuple3:public tuple3 <
      T0, T1, T2 >
    {
      typedef iterator_tuple3 < T0, T1, T2, R0, R1, R2 > self_type;
      typedef iterator_traits_tuple3 < R0, R1, R2 > traits;
      typedef typename traits::iterator_category iterator_category;
      typedef typename traits::value_type value_type;
      typedef typename traits::reference reference;
      typedef typename traits::const_reference const_reference;
      typedef typename traits::pointer pointer;
      typedef typename traits::difference_type difference_type;
        iterator_tuple3( arg_type0 theM0, arg_type1 theM1,
                         arg_type2 theM2 ):tuple3 < T0, T1, T2 > ( theM0,
                                                                   theM1,
                                                                   theM2 )
      {
      }
      template < class S0, class S1,
        class S2 > iterator_tuple3( const iterator_tuple3 < S0, S1,
                                    S2 > &rhs ):tuple3 < T0, T1, T2 > ( rhs )
      {
      }
      iterator_tuple3( arg_type0 theM0, const tail_type & tail ):tuple3 < T0,
        T1, T2 > ( theM0, tail )
      {
      }
      self_type & operator++(  )
      {
        ++m0;
        ++m1;
        ++m2;
        return *this;
      }
      const self_type operator++( int )
      {
        self_type old = *this;
        ++( *this );
        return old;
      }
      self_type & operator+=( const difference_type & rhs )
      {
        m0 += rhs.m0;
        m1 += rhs.m1;
        m2 += rhs.m2;
        return *this;
      }
      self_type & operator--(  )
      {
        --m0;
        --m1;
        --m2;
        return *this;
      }
      const self_type operator--( int )
      {
        self_type old = *this;
        --( *this );
        return old;
      }
      self_type & operator-=( const difference_type & rhs )
      {
        m0 -= rhs.m0;
        m1 -= rhs.m1;
        m2 -= rhs.m2;
        return *this;
      }
      reference operator*(  )
      {
        return reference( m0, m1, m2 );
      }
      const_reference operator*(  ) const
      {
        return const_reference( *m0, *m1, *m2 );
      }
      reference operator[] ( const difference_type & i )
      {
        return reference( &( m0[i.m0] ), &( m1[i.m1] ), &( m2[i.m2] ) );
      }
      const_reference operator[] ( const difference_type & i ) const
      {
        return const_reference( m0[i.m0], m1[i.m1], m2[i.m2] );
      }
    };
    template < class T0, class T1, class T2 > iterator_tuple3 < T0, T1,
      T2 > operator+( const iterator_tuple3 < T0, T1, T2 > &lhs,
                      const iterator_tuple3 < T0, T1,
                      T2 >::difference_type & rhs )
    {
      return iterator_tuple3 < T0, T1, T2 > ( lhs ) += rhs;
    }
    template < class T0, class T1, class T2 > iterator_tuple3 < T0, T1,
      T2 > operator-( const iterator_tuple3 < T0, T1, T2 > &lhs,
                      const iterator_tuple3 < T0, T1,
                      T2 >::difference_type & rhs )
    {
      return iterator_tuple3 < T0, T1, T2 > ( lhs ) -= rhs;
    }
    template < class T > struct n_fold_iterator3
    {
      typedef iterator_tuple3 < T, T, T > type;
    };
    template < class R0, class R1, class R2, class R3 > struct iterator_traits_tuple4
    {
      typedef tuple4 < typename R0::iterator_category,
        typename R1::iterator_category, typename R2::iterator_category,
        typename R3::iterator_category > iterator_category;
      typedef tuple4 < typename R0::value_type, typename R1::value_type,
        typename R2::value_type, typename R3::value_type > value_type;
      typedef tuple4 < detail::assign_to_pointee < typename R0::value_type >,
        detail::assign_to_pointee < typename R1::value_type >,
        detail::assign_to_pointee < typename R2::value_type >,
        detail::assign_to_pointee < typename R3::value_type > >reference;
      typedef tuple4 < typename R0::value_type, typename R1::value_type,
        typename R2::value_type, typename R3::value_type > const_reference;
      typedef tuple4 < typename R0::pointer, typename R1::pointer,
        typename R2::pointer, typename R3::pointer > pointer;
      typedef tuple4 < typename R0::difference_type,
        typename R1::difference_type, typename R2::difference_type,
        typename R3::difference_type > difference_type;
    };
    template < class T0, class T1, class T2, class T3, class R0 =
      detail::iterator_traits < T0 >, class R1 =
      detail::iterator_traits < T1 >, class R2 =
      detail::iterator_traits < T2 >, class R3 =
      detail::iterator_traits < T3 > >struct iterator_tuple4:public tuple4 <
      T0, T1, T2, T3 >
    {
      typedef iterator_tuple4 < T0, T1, T2, T3, R0, R1, R2, R3 > self_type;
      typedef iterator_traits_tuple4 < R0, R1, R2, R3 > traits;
      typedef typename traits::iterator_category iterator_category;
      typedef typename traits::value_type value_type;
      typedef typename traits::reference reference;
      typedef typename traits::const_reference const_reference;
      typedef typename traits::pointer pointer;
      typedef typename traits::difference_type difference_type;
        iterator_tuple4( arg_type0 theM0, arg_type1 theM1, arg_type2 theM2,
                         arg_type3 theM3 ):tuple4 < T0, T1, T2, T3 > ( theM0,
                                                                       theM1,
                                                                       theM2,
                                                                       theM3 )
      {
      }
      template < class S0, class S1, class S2,
        class S3 > iterator_tuple4( const iterator_tuple4 < S0, S1, S2,
                                    S3 > &rhs ):tuple4 < T0, T1, T2,
        T3 > ( rhs )
      {
      }
      iterator_tuple4( arg_type0 theM0, const tail_type & tail ):tuple4 < T0,
        T1, T2, T3 > ( theM0, tail )
      {
      }
      self_type & operator++(  )
      {
        ++m0;
        ++m1;
        ++m2;
        ++m3;
        return *this;
      }
      const self_type operator++( int )
      {
        self_type old = *this;
        ++( *this );
        return old;
      }
      self_type & operator+=( const difference_type & rhs )
      {
        m0 += rhs.m0;
        m1 += rhs.m1;
        m2 += rhs.m2;
        m3 += rhs.m3;
        return *this;
      }
      self_type & operator--(  )
      {
        --m0;
        --m1;
        --m2;
        --m3;
        return *this;
      }
      const self_type operator--( int )
      {
        self_type old = *this;
        --( *this );
        return old;
      }
      self_type & operator-=( const difference_type & rhs )
      {
        m0 -= rhs.m0;
        m1 -= rhs.m1;
        m2 -= rhs.m2;
        m3 -= rhs.m3;
        return *this;
      }
      reference operator*(  )
      {
        return reference( m0, m1, m2, m3 );
      }
      const_reference operator*(  ) const
      {
        return const_reference( *m0, *m1, *m2, *m3 );
      }
      reference operator[] ( const difference_type & i )
      {
        return reference( &( m0[i.m0] ), &( m1[i.m1] ), &( m2[i.m2] ),
                          &( m3[i.m3] ) );
      }
      const_reference operator[] ( const difference_type & i ) const
      {
        return const_reference( m0[i.m0], m1[i.m1], m2[i.m2], m3[i.m3] );
      }
    };
    template < class T0, class T1, class T2, class T3 > iterator_tuple4 < T0,
      T1, T2, T3 > operator+( const iterator_tuple4 < T0, T1, T2, T3 > &lhs,
                              const iterator_tuple4 < T0, T1, T2,
                              T3 >::difference_type & rhs )
    {
      return iterator_tuple4 < T0, T1, T2, T3 > ( lhs ) += rhs;
    }
    template < class T0, class T1, class T2, class T3 > iterator_tuple4 < T0,
      T1, T2, T3 > operator-( const iterator_tuple4 < T0, T1, T2, T3 > &lhs,
                              const iterator_tuple4 < T0, T1, T2,
                              T3 >::difference_type & rhs )
    {
      return iterator_tuple4 < T0, T1, T2, T3 > ( lhs ) -= rhs;
    }
    template < class T > struct n_fold_iterator4
    {
      typedef iterator_tuple4 < T, T, T, T > type;
    };
    template < class R0, class R1, class R2, class R3, class R4 > struct iterator_traits_tuple5
    {
      typedef tuple5 < typename R0::iterator_category,
        typename R1::iterator_category, typename R2::iterator_category,
        typename R3::iterator_category,
        typename R4::iterator_category > iterator_category;
      typedef tuple5 < typename R0::value_type, typename R1::value_type,
        typename R2::value_type, typename R3::value_type,
        typename R4::value_type > value_type;
      typedef tuple5 < detail::assign_to_pointee < typename R0::value_type >,
        detail::assign_to_pointee < typename R1::value_type >,
        detail::assign_to_pointee < typename R2::value_type >,
        detail::assign_to_pointee < typename R3::value_type >,
        detail::assign_to_pointee < typename R4::value_type > >reference;
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
    template < class T0, class T1, class T2, class T3, class T4, class R0 =
      detail::iterator_traits < T0 >, class R1 =
      detail::iterator_traits < T1 >, class R2 =
      detail::iterator_traits < T2 >, class R3 =
      detail::iterator_traits < T3 >, class R4 =
      detail::iterator_traits < T4 > >struct iterator_tuple5:public tuple5 <
      T0, T1, T2, T3, T4 >
    {
      typedef iterator_tuple5 < T0, T1, T2, T3, T4, R0, R1, R2, R3,
        R4 > self_type;
      typedef iterator_traits_tuple5 < R0, R1, R2, R3, R4 > traits;
      typedef typename traits::iterator_category iterator_category;
      typedef typename traits::value_type value_type;
      typedef typename traits::reference reference;
      typedef typename traits::const_reference const_reference;
      typedef typename traits::pointer pointer;
      typedef typename traits::difference_type difference_type;
        iterator_tuple5( arg_type0 theM0, arg_type1 theM1, arg_type2 theM2,
                         arg_type3 theM3, arg_type4 theM4 ):tuple5 < T0, T1,
        T2, T3, T4 > ( theM0, theM1, theM2, theM3, theM4 )
      {
      }
      template < class S0, class S1, class S2, class S3,
        class S4 > iterator_tuple5( const iterator_tuple5 < S0, S1, S2, S3,
                                    S4 > &rhs ):tuple5 < T0, T1, T2, T3,
        T4 > ( rhs )
      {
      }
      iterator_tuple5( arg_type0 theM0, const tail_type & tail ):tuple5 < T0,
        T1, T2, T3, T4 > ( theM0, tail )
      {
      }
      self_type & operator++(  )
      {
        ++m0;
        ++m1;
        ++m2;
        ++m3;
        ++m4;
        return *this;
      }
      const self_type operator++( int )
      {
        self_type old = *this;
        ++( *this );
        return old;
      }
      self_type & operator+=( const difference_type & rhs )
      {
        m0 += rhs.m0;
        m1 += rhs.m1;
        m2 += rhs.m2;
        m3 += rhs.m3;
        m4 += rhs.m4;
        return *this;
      }
      self_type & operator--(  )
      {
        --m0;
        --m1;
        --m2;
        --m3;
        --m4;
        return *this;
      }
      const self_type operator--( int )
      {
        self_type old = *this;
        --( *this );
        return old;
      }
      self_type & operator-=( const difference_type & rhs )
      {
        m0 -= rhs.m0;
        m1 -= rhs.m1;
        m2 -= rhs.m2;
        m3 -= rhs.m3;
        m4 -= rhs.m4;
        return *this;
      }
      reference operator*(  )
      {
        return reference( m0, m1, m2, m3, m4 );
      }
      const_reference operator*(  ) const
      {
        return const_reference( *m0, *m1, *m2, *m3, *m4 );
      }
      reference operator[] ( const difference_type & i )
      {
        return reference( &( m0[i.m0] ), &( m1[i.m1] ), &( m2[i.m2] ),
                          &( m3[i.m3] ), &( m4[i.m4] ) );
      }
      const_reference operator[] ( const difference_type & i ) const
      {
        return const_reference( m0[i.m0], m1[i.m1], m2[i.m2], m3[i.m3],
                                m4[i.m4] );
      }
    };
    template < class T0, class T1, class T2, class T3,
      class T4 > iterator_tuple5 < T0, T1, T2, T3,
      T4 > operator+( const iterator_tuple5 < T0, T1, T2, T3, T4 > &lhs,
                      const iterator_tuple5 < T0, T1, T2, T3,
                      T4 >::difference_type & rhs )
    {
      return iterator_tuple5 < T0, T1, T2, T3, T4 > ( lhs ) += rhs;
    }
    template < class T0, class T1, class T2, class T3,
      class T4 > iterator_tuple5 < T0, T1, T2, T3,
      T4 > operator-( const iterator_tuple5 < T0, T1, T2, T3, T4 > &lhs,
                      const iterator_tuple5 < T0, T1, T2, T3,
                      T4 >::difference_type & rhs )
    {
      return iterator_tuple5 < T0, T1, T2, T3, T4 > ( lhs ) -= rhs;
    }
    template < class T > struct n_fold_iterator5
    {
      typedef iterator_tuple5 < T, T, T, T, T > type;
    };
    template < class R0, class R1, class R2, class R3, class R4, class R5 > struct iterator_traits_tuple6
    {
      typedef tuple6 < typename R0::iterator_category,
        typename R1::iterator_category, typename R2::iterator_category,
        typename R3::iterator_category, typename R4::iterator_category,
        typename R5::iterator_category > iterator_category;
      typedef tuple6 < typename R0::value_type, typename R1::value_type,
        typename R2::value_type, typename R3::value_type,
        typename R4::value_type, typename R5::value_type > value_type;
      typedef tuple6 < detail::assign_to_pointee < typename R0::value_type >,
        detail::assign_to_pointee < typename R1::value_type >,
        detail::assign_to_pointee < typename R2::value_type >,
        detail::assign_to_pointee < typename R3::value_type >,
        detail::assign_to_pointee < typename R4::value_type >,
        detail::assign_to_pointee < typename R5::value_type > >reference;
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
    template < class T0, class T1, class T2, class T3, class T4, class T5,
      class R0 = detail::iterator_traits < T0 >, class R1 =
      detail::iterator_traits < T1 >, class R2 =
      detail::iterator_traits < T2 >, class R3 =
      detail::iterator_traits < T3 >, class R4 =
      detail::iterator_traits < T4 >, class R5 =
      detail::iterator_traits < T5 > >struct iterator_tuple6:public tuple6 <
      T0, T1, T2, T3, T4, T5 >
    {
      typedef iterator_tuple6 < T0, T1, T2, T3, T4, T5, R0, R1, R2, R3, R4,
        R5 > self_type;
      typedef iterator_traits_tuple6 < R0, R1, R2, R3, R4, R5 > traits;
      typedef typename traits::iterator_category iterator_category;
      typedef typename traits::value_type value_type;
      typedef typename traits::reference reference;
      typedef typename traits::const_reference const_reference;
      typedef typename traits::pointer pointer;
      typedef typename traits::difference_type difference_type;
        iterator_tuple6( arg_type0 theM0, arg_type1 theM1, arg_type2 theM2,
                         arg_type3 theM3, arg_type4 theM4,
                         arg_type5 theM5 ):tuple6 < T0, T1, T2, T3, T4,
        T5 > ( theM0, theM1, theM2, theM3, theM4, theM5 )
      {
      }
      template < class S0, class S1, class S2, class S3, class S4,
        class S5 > iterator_tuple6( const iterator_tuple6 < S0, S1, S2, S3,
                                    S4, S5 > &rhs ):tuple6 < T0, T1, T2, T3,
        T4, T5 > ( rhs )
      {
      }
      iterator_tuple6( arg_type0 theM0, const tail_type & tail ):tuple6 < T0,
        T1, T2, T3, T4, T5 > ( theM0, tail )
      {
      }
      self_type & operator++(  )
      {
        ++m0;
        ++m1;
        ++m2;
        ++m3;
        ++m4;
        ++m5;
        return *this;
      }
      const self_type operator++( int )
      {
        self_type old = *this;
        ++( *this );
        return old;
      }
      self_type & operator+=( const difference_type & rhs )
      {
        m0 += rhs.m0;
        m1 += rhs.m1;
        m2 += rhs.m2;
        m3 += rhs.m3;
        m4 += rhs.m4;
        m5 += rhs.m5;
        return *this;
      }
      self_type & operator--(  )
      {
        --m0;
        --m1;
        --m2;
        --m3;
        --m4;
        --m5;
        return *this;
      }
      const self_type operator--( int )
      {
        self_type old = *this;
        --( *this );
        return old;
      }
      self_type & operator-=( const difference_type & rhs )
      {
        m0 -= rhs.m0;
        m1 -= rhs.m1;
        m2 -= rhs.m2;
        m3 -= rhs.m3;
        m4 -= rhs.m4;
        m5 -= rhs.m5;
        return *this;
      }
      reference operator*(  )
      {
        return reference( m0, m1, m2, m3, m4, m5 );
      }
      const_reference operator*(  ) const
      {
        return const_reference( *m0, *m1, *m2, *m3, *m4, *m5 );
      }
      reference operator[] ( const difference_type & i )
      {
        return reference( &( m0[i.m0] ), &( m1[i.m1] ), &( m2[i.m2] ),
                          &( m3[i.m3] ), &( m4[i.m4] ), &( m5[i.m5] ) );
      }
      const_reference operator[] ( const difference_type & i ) const
      {
        return const_reference( m0[i.m0], m1[i.m1], m2[i.m2], m3[i.m3],
                                m4[i.m4], m5[i.m5] );
      }
    };
    template < class T0, class T1, class T2, class T3, class T4,
      class T5 > iterator_tuple6 < T0, T1, T2, T3, T4,
      T5 > operator+( const iterator_tuple6 < T0, T1, T2, T3, T4, T5 > &lhs,
                      const iterator_tuple6 < T0, T1, T2, T3, T4,
                      T5 >::difference_type & rhs )
    {
      return iterator_tuple6 < T0, T1, T2, T3, T4, T5 > ( lhs ) += rhs;
    }
    template < class T0, class T1, class T2, class T3, class T4,
      class T5 > iterator_tuple6 < T0, T1, T2, T3, T4,
      T5 > operator-( const iterator_tuple6 < T0, T1, T2, T3, T4, T5 > &lhs,
                      const iterator_tuple6 < T0, T1, T2, T3, T4,
                      T5 >::difference_type & rhs )
    {
      return iterator_tuple6 < T0, T1, T2, T3, T4, T5 > ( lhs ) -= rhs;
    }
    template < class T > struct n_fold_iterator6
    {
      typedef iterator_tuple6 < T, T, T, T, T, T > type;
    };
    template < class R0, class R1, class R2, class R3, class R4, class R5,
      class R6 > struct iterator_traits_tuple7
    {
      typedef tuple7 < typename R0::iterator_category,
        typename R1::iterator_category, typename R2::iterator_category,
        typename R3::iterator_category, typename R4::iterator_category,
        typename R5::iterator_category,
        typename R6::iterator_category > iterator_category;
      typedef tuple7 < typename R0::value_type, typename R1::value_type,
        typename R2::value_type, typename R3::value_type,
        typename R4::value_type, typename R5::value_type,
        typename R6::value_type > value_type;
      typedef tuple7 < detail::assign_to_pointee < typename R0::value_type >,
        detail::assign_to_pointee < typename R1::value_type >,
        detail::assign_to_pointee < typename R2::value_type >,
        detail::assign_to_pointee < typename R3::value_type >,
        detail::assign_to_pointee < typename R4::value_type >,
        detail::assign_to_pointee < typename R5::value_type >,
        detail::assign_to_pointee < typename R6::value_type > >reference;
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
      class T6, class R0 = detail::iterator_traits < T0 >, class R1 =
      detail::iterator_traits < T1 >, class R2 =
      detail::iterator_traits < T2 >, class R3 =
      detail::iterator_traits < T3 >, class R4 =
      detail::iterator_traits < T4 >, class R5 =
      detail::iterator_traits < T5 >, class R6 =
      detail::iterator_traits < T6 > >struct iterator_tuple7:public tuple7 <
      T0, T1, T2, T3, T4, T5, T6 >
    {
      typedef iterator_tuple7 < T0, T1, T2, T3, T4, T5, T6, R0, R1, R2, R3,
        R4, R5, R6 > self_type;
      typedef iterator_traits_tuple7 < R0, R1, R2, R3, R4, R5, R6 > traits;
      typedef typename traits::iterator_category iterator_category;
      typedef typename traits::value_type value_type;
      typedef typename traits::reference reference;
      typedef typename traits::const_reference const_reference;
      typedef typename traits::pointer pointer;
      typedef typename traits::difference_type difference_type;
        iterator_tuple7( arg_type0 theM0, arg_type1 theM1, arg_type2 theM2,
                         arg_type3 theM3, arg_type4 theM4, arg_type5 theM5,
                         arg_type6 theM6 ):tuple7 < T0, T1, T2, T3, T4, T5,
        T6 > ( theM0, theM1, theM2, theM3, theM4, theM5, theM6 )
      {
      }
      template < class S0, class S1, class S2, class S3, class S4, class S5,
        class S6 > iterator_tuple7( const iterator_tuple7 < S0, S1, S2, S3,
                                    S4, S5, S6 > &rhs ):tuple7 < T0, T1, T2,
        T3, T4, T5, T6 > ( rhs )
      {
      }
      iterator_tuple7( arg_type0 theM0, const tail_type & tail ):tuple7 < T0,
        T1, T2, T3, T4, T5, T6 > ( theM0, tail )
      {
      }
      self_type & operator++(  )
      {
        ++m0;
        ++m1;
        ++m2;
        ++m3;
        ++m4;
        ++m5;
        ++m6;
        return *this;
      }
      const self_type operator++( int )
      {
        self_type old = *this;
        ++( *this );
        return old;
      }
      self_type & operator+=( const difference_type & rhs )
      {
        m0 += rhs.m0;
        m1 += rhs.m1;
        m2 += rhs.m2;
        m3 += rhs.m3;
        m4 += rhs.m4;
        m5 += rhs.m5;
        m6 += rhs.m6;
        return *this;
      }
      self_type & operator--(  )
      {
        --m0;
        --m1;
        --m2;
        --m3;
        --m4;
        --m5;
        --m6;
        return *this;
      }
      const self_type operator--( int )
      {
        self_type old = *this;
        --( *this );
        return old;
      }
      self_type & operator-=( const difference_type & rhs )
      {
        m0 -= rhs.m0;
        m1 -= rhs.m1;
        m2 -= rhs.m2;
        m3 -= rhs.m3;
        m4 -= rhs.m4;
        m5 -= rhs.m5;
        m6 -= rhs.m6;
        return *this;
      }
      reference operator*(  )
      {
        return reference( m0, m1, m2, m3, m4, m5, m6 );
      }
      const_reference operator*(  ) const
      {
        return const_reference( *m0, *m1, *m2, *m3, *m4, *m5, *m6 );
      }
      reference operator[] ( const difference_type & i )
      {
        return reference( &( m0[i.m0] ), &( m1[i.m1] ), &( m2[i.m2] ),
                          &( m3[i.m3] ), &( m4[i.m4] ), &( m5[i.m5] ),
                          &( m6[i.m6] ) );
      }
      const_reference operator[] ( const difference_type & i ) const
      {
        return const_reference( m0[i.m0], m1[i.m1], m2[i.m2], m3[i.m3],
                                m4[i.m4], m5[i.m5], m6[i.m6] );
      }
    };
    template < class T0, class T1, class T2, class T3, class T4, class T5,
      class T6 > iterator_tuple7 < T0, T1, T2, T3, T4, T5,
      T6 > operator+( const iterator_tuple7 < T0, T1, T2, T3, T4, T5,
                      T6 > &lhs, const iterator_tuple7 < T0, T1, T2, T3, T4,
                      T5, T6 >::difference_type & rhs )
    {
      return iterator_tuple7 < T0, T1, T2, T3, T4, T5, T6 > ( lhs ) += rhs;
    }
    template < class T0, class T1, class T2, class T3, class T4, class T5,
      class T6 > iterator_tuple7 < T0, T1, T2, T3, T4, T5,
      T6 > operator-( const iterator_tuple7 < T0, T1, T2, T3, T4, T5,
                      T6 > &lhs, const iterator_tuple7 < T0, T1, T2, T3, T4,
                      T5, T6 >::difference_type & rhs )
    {
      return iterator_tuple7 < T0, T1, T2, T3, T4, T5, T6 > ( lhs ) -= rhs;
    }
    template < class T > struct n_fold_iterator7
    {
      typedef iterator_tuple7 < T, T, T, T, T, T, T > type;
    };
    template < class R0, class R1, class R2, class R3, class R4, class R5,
      class R6, class R7 > struct iterator_traits_tuple8
    {
      typedef tuple8 < typename R0::iterator_category,
        typename R1::iterator_category, typename R2::iterator_category,
        typename R3::iterator_category, typename R4::iterator_category,
        typename R5::iterator_category, typename R6::iterator_category,
        typename R7::iterator_category > iterator_category;
      typedef tuple8 < typename R0::value_type, typename R1::value_type,
        typename R2::value_type, typename R3::value_type,
        typename R4::value_type, typename R5::value_type,
        typename R6::value_type, typename R7::value_type > value_type;
      typedef tuple8 < detail::assign_to_pointee < typename R0::value_type >,
        detail::assign_to_pointee < typename R1::value_type >,
        detail::assign_to_pointee < typename R2::value_type >,
        detail::assign_to_pointee < typename R3::value_type >,
        detail::assign_to_pointee < typename R4::value_type >,
        detail::assign_to_pointee < typename R5::value_type >,
        detail::assign_to_pointee < typename R6::value_type >,
        detail::assign_to_pointee < typename R7::value_type > >reference;
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
      class T6, class T7, class R0 =
      detail::iterator_traits < T0 >, class R1 =
      detail::iterator_traits < T1 >, class R2 =
      detail::iterator_traits < T2 >, class R3 =
      detail::iterator_traits < T3 >, class R4 =
      detail::iterator_traits < T4 >, class R5 =
      detail::iterator_traits < T5 >, class R6 =
      detail::iterator_traits < T6 >, class R7 =
      detail::iterator_traits < T7 > >struct iterator_tuple8:public tuple8 <
      T0, T1, T2, T3, T4, T5, T6, T7 >
    {
      typedef iterator_tuple8 < T0, T1, T2, T3, T4, T5, T6, T7, R0, R1, R2,
        R3, R4, R5, R6, R7 > self_type;
      typedef iterator_traits_tuple8 < R0, R1, R2, R3, R4, R5, R6,
        R7 > traits;
      typedef typename traits::iterator_category iterator_category;
      typedef typename traits::value_type value_type;
      typedef typename traits::reference reference;
      typedef typename traits::const_reference const_reference;
      typedef typename traits::pointer pointer;
      typedef typename traits::difference_type difference_type;
        iterator_tuple8( arg_type0 theM0, arg_type1 theM1, arg_type2 theM2,
                         arg_type3 theM3, arg_type4 theM4, arg_type5 theM5,
                         arg_type6 theM6, arg_type7 theM7 ):tuple8 < T0, T1,
        T2, T3, T4, T5, T6, T7 > ( theM0, theM1, theM2, theM3, theM4, theM5,
                                   theM6, theM7 )
      {
      }
      template < class S0, class S1, class S2, class S3, class S4, class S5,
        class S6, class S7 > iterator_tuple8( const iterator_tuple8 < S0, S1,
                                              S2, S3, S4, S5, S6,
                                              S7 > &rhs ):tuple8 < T0, T1, T2,
        T3, T4, T5, T6, T7 > ( rhs )
      {
      }
      iterator_tuple8( arg_type0 theM0, const tail_type & tail ):tuple8 < T0,
        T1, T2, T3, T4, T5, T6, T7 > ( theM0, tail )
      {
      }
      self_type & operator++(  )
      {
        ++m0;
        ++m1;
        ++m2;
        ++m3;
        ++m4;
        ++m5;
        ++m6;
        ++m7;
        return *this;
      }
      const self_type operator++( int )
      {
        self_type old = *this;
        ++( *this );
        return old;
      }
      self_type & operator+=( const difference_type & rhs )
      {
        m0 += rhs.m0;
        m1 += rhs.m1;
        m2 += rhs.m2;
        m3 += rhs.m3;
        m4 += rhs.m4;
        m5 += rhs.m5;
        m6 += rhs.m6;
        m7 += rhs.m7;
        return *this;
      }
      self_type & operator--(  )
      {
        --m0;
        --m1;
        --m2;
        --m3;
        --m4;
        --m5;
        --m6;
        --m7;
        return *this;
      }
      const self_type operator--( int )
      {
        self_type old = *this;
        --( *this );
        return old;
      }
      self_type & operator-=( const difference_type & rhs )
      {
        m0 -= rhs.m0;
        m1 -= rhs.m1;
        m2 -= rhs.m2;
        m3 -= rhs.m3;
        m4 -= rhs.m4;
        m5 -= rhs.m5;
        m6 -= rhs.m6;
        m7 -= rhs.m7;
        return *this;
      }
      reference operator*(  )
      {
        return reference( m0, m1, m2, m3, m4, m5, m6, m7 );
      }
      const_reference operator*(  ) const
      {
        return const_reference( *m0, *m1, *m2, *m3, *m4, *m5, *m6, *m7 );
      }
      reference operator[] ( const difference_type & i )
      {
        return reference( &( m0[i.m0] ), &( m1[i.m1] ), &( m2[i.m2] ),
                          &( m3[i.m3] ), &( m4[i.m4] ), &( m5[i.m5] ),
                          &( m6[i.m6] ), &( m7[i.m7] ) );
      }
      const_reference operator[] ( const difference_type & i ) const
      {
        return const_reference( m0[i.m0], m1[i.m1], m2[i.m2], m3[i.m3],
                                m4[i.m4], m5[i.m5], m6[i.m6], m7[i.m7] );
      }
    };
    template < class T0, class T1, class T2, class T3, class T4, class T5,
      class T6, class T7 > iterator_tuple8 < T0, T1, T2, T3, T4, T5, T6,
      T7 > operator+( const iterator_tuple8 < T0, T1, T2, T3, T4, T5, T6,
                      T7 > &lhs, const iterator_tuple8 < T0, T1, T2, T3, T4,
                      T5, T6, T7 >::difference_type & rhs )
    {
      return iterator_tuple8 < T0, T1, T2, T3, T4, T5, T6, T7 > ( lhs ) +=
        rhs;
    }
    template < class T0, class T1, class T2, class T3, class T4, class T5,
      class T6, class T7 > iterator_tuple8 < T0, T1, T2, T3, T4, T5, T6,
      T7 > operator-( const iterator_tuple8 < T0, T1, T2, T3, T4, T5, T6,
                      T7 > &lhs, const iterator_tuple8 < T0, T1, T2, T3, T4,
                      T5, T6, T7 >::difference_type & rhs )
    {
      return iterator_tuple8 < T0, T1, T2, T3, T4, T5, T6, T7 > ( lhs ) -=
        rhs;
    }
    template < class T > struct n_fold_iterator8
    {
      typedef iterator_tuple8 < T, T, T, T, T, T, T, T > type;
    };
    template < class R0, class R1, class R2, class R3, class R4, class R5,
      class R6, class R7, class R8 > struct iterator_traits_tuple9
    {
      typedef tuple9 < typename R0::iterator_category,
        typename R1::iterator_category, typename R2::iterator_category,
        typename R3::iterator_category, typename R4::iterator_category,
        typename R5::iterator_category, typename R6::iterator_category,
        typename R7::iterator_category,
        typename R8::iterator_category > iterator_category;
      typedef tuple9 < typename R0::value_type, typename R1::value_type,
        typename R2::value_type, typename R3::value_type,
        typename R4::value_type, typename R5::value_type,
        typename R6::value_type, typename R7::value_type,
        typename R8::value_type > value_type;
      typedef tuple9 < detail::assign_to_pointee < typename R0::value_type >,
        detail::assign_to_pointee < typename R1::value_type >,
        detail::assign_to_pointee < typename R2::value_type >,
        detail::assign_to_pointee < typename R3::value_type >,
        detail::assign_to_pointee < typename R4::value_type >,
        detail::assign_to_pointee < typename R5::value_type >,
        detail::assign_to_pointee < typename R6::value_type >,
        detail::assign_to_pointee < typename R7::value_type >,
        detail::assign_to_pointee < typename R8::value_type > >reference;
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
      class T6, class T7, class T8, class R0 =
      detail::iterator_traits < T0 >, class R1 =
      detail::iterator_traits < T1 >, class R2 =
      detail::iterator_traits < T2 >, class R3 =
      detail::iterator_traits < T3 >, class R4 =
      detail::iterator_traits < T4 >, class R5 =
      detail::iterator_traits < T5 >, class R6 =
      detail::iterator_traits < T6 >, class R7 =
      detail::iterator_traits < T7 >, class R8 =
      detail::iterator_traits < T8 > >struct iterator_tuple9:public tuple9 <
      T0, T1, T2, T3, T4, T5, T6, T7, T8 >
    {
      typedef iterator_tuple9 < T0, T1, T2, T3, T4, T5, T6, T7, T8, R0, R1,
        R2, R3, R4, R5, R6, R7, R8 > self_type;
      typedef iterator_traits_tuple9 < R0, R1, R2, R3, R4, R5, R6, R7,
        R8 > traits;
      typedef typename traits::iterator_category iterator_category;
      typedef typename traits::value_type value_type;
      typedef typename traits::reference reference;
      typedef typename traits::const_reference const_reference;
      typedef typename traits::pointer pointer;
      typedef typename traits::difference_type difference_type;
        iterator_tuple9( arg_type0 theM0, arg_type1 theM1, arg_type2 theM2,
                         arg_type3 theM3, arg_type4 theM4, arg_type5 theM5,
                         arg_type6 theM6, arg_type7 theM7,
                         arg_type8 theM8 ):tuple9 < T0, T1, T2, T3, T4, T5,
        T6, T7, T8 > ( theM0, theM1, theM2, theM3, theM4, theM5, theM6, theM7,
                       theM8 )
      {
      }
      template < class S0, class S1, class S2, class S3, class S4, class S5,
        class S6, class S7,
        class S8 > iterator_tuple9( const iterator_tuple9 < S0, S1, S2, S3,
                                    S4, S5, S6, S7, S8 > &rhs ):tuple9 < T0,
        T1, T2, T3, T4, T5, T6, T7, T8 > ( rhs )
      {
      }
      iterator_tuple9( arg_type0 theM0, const tail_type & tail ):tuple9 < T0,
        T1, T2, T3, T4, T5, T6, T7, T8 > ( theM0, tail )
      {
      }
      self_type & operator++(  )
      {
        ++m0;
        ++m1;
        ++m2;
        ++m3;
        ++m4;
        ++m5;
        ++m6;
        ++m7;
        ++m8;
        return *this;
      }
      const self_type operator++( int )
      {
        self_type old = *this;
        ++( *this );
        return old;
      }
      self_type & operator+=( const difference_type & rhs )
      {
        m0 += rhs.m0;
        m1 += rhs.m1;
        m2 += rhs.m2;
        m3 += rhs.m3;
        m4 += rhs.m4;
        m5 += rhs.m5;
        m6 += rhs.m6;
        m7 += rhs.m7;
        m8 += rhs.m8;
        return *this;
      }
      self_type & operator--(  )
      {
        --m0;
        --m1;
        --m2;
        --m3;
        --m4;
        --m5;
        --m6;
        --m7;
        --m8;
        return *this;
      }
      const self_type operator--( int )
      {
        self_type old = *this;
        --( *this );
        return old;
      }
      self_type & operator-=( const difference_type & rhs )
      {
        m0 -= rhs.m0;
        m1 -= rhs.m1;
        m2 -= rhs.m2;
        m3 -= rhs.m3;
        m4 -= rhs.m4;
        m5 -= rhs.m5;
        m6 -= rhs.m6;
        m7 -= rhs.m7;
        m8 -= rhs.m8;
        return *this;
      }
      reference operator*(  )
      {
        return reference( m0, m1, m2, m3, m4, m5, m6, m7, m8 );
      }
      const_reference operator*(  ) const
      {
        return const_reference( *m0, *m1, *m2, *m3, *m4, *m5, *m6, *m7, *m8 );
      }
      reference operator[] ( const difference_type & i )
      {
        return reference( &( m0[i.m0] ), &( m1[i.m1] ), &( m2[i.m2] ),
                          &( m3[i.m3] ), &( m4[i.m4] ), &( m5[i.m5] ),
                          &( m6[i.m6] ), &( m7[i.m7] ), &( m8[i.m8] ) );
      }
      const_reference operator[] ( const difference_type & i ) const
      {
        return const_reference( m0[i.m0], m1[i.m1], m2[i.m2], m3[i.m3],
                                m4[i.m4], m5[i.m5], m6[i.m6], m7[i.m7],
                                m8[i.m8] );
      }
    };
    template < class T0, class T1, class T2, class T3, class T4, class T5,
      class T6, class T7, class T8 > iterator_tuple9 < T0, T1, T2, T3, T4, T5,
      T6, T7, T8 > operator+( const iterator_tuple9 < T0, T1, T2, T3, T4, T5,
                              T6, T7, T8 > &lhs, const iterator_tuple9 < T0,
                              T1, T2, T3, T4, T5, T6, T7,
                              T8 >::difference_type & rhs )
    {
      return iterator_tuple9 < T0, T1, T2, T3, T4, T5, T6, T7, T8 > ( lhs ) +=
        rhs;
    }
    template < class T0, class T1, class T2, class T3, class T4, class T5,
      class T6, class T7, class T8 > iterator_tuple9 < T0, T1, T2, T3, T4, T5,
      T6, T7, T8 > operator-( const iterator_tuple9 < T0, T1, T2, T3, T4, T5,
                              T6, T7, T8 > &lhs, const iterator_tuple9 < T0,
                              T1, T2, T3, T4, T5, T6, T7,
                              T8 >::difference_type & rhs )
    {
      return iterator_tuple9 < T0, T1, T2, T3, T4, T5, T6, T7, T8 > ( lhs ) -=
        rhs;
    }
    template < class T > struct n_fold_iterator9
    {
      typedef iterator_tuple9 < T, T, T, T, T, T, T, T, T > type;
    };







    template < class R0 > struct container_traits_tuple1
    {
      typedef tuple1 < typename R0::value_type > value_type;
      typedef iterator_tuple1 < typename R0::iterator, R0 > iterator;
      typedef iterator_tuple1 < typename R0::const_iterator,
        R0 > const_iterator;
      typedef tuple1 < detail::assign_to_pointee < typename R0::value_type >
        >reference;
      typedef tuple1 < typename R0::value_type > const_reference;
      typedef tuple1 < typename R0::pointer > pointer;
      typedef tuple1 < typename R0::difference_type > difference_type;
    };
    template < class T0, class R0 =
      detail::iterator_traits < typename T0::iterator >
      >struct container_tuple1:public tuple1 < T0 >
    {
      typedef container_tuple1 < T0 > self_type;
      typedef tuple1 < typename T0::size_type > size_type;
      typedef container_traits_tuple1 < R0 > traits;
      typedef typename traits::value_type value_type;
      typedef typename traits::iterator iterator;
      typedef typename traits::const_iterator const_iterator;
      typedef typename traits::reference reference;
      typedef typename traits::const_reference const_reference;
      typedef typename traits::pointer pointer;
      typedef typename traits::difference_type difference_type;
        container_tuple1( arg_type0 theM0 ):tuple1 < T0 > ( theM0 )
      {
      }
      template < class S0 > container_tuple1( const iterator_tuple1 < S0 >
                                              &rhs ):tuple1 < T0 > ( rhs )
      {
      }
      container_tuple1( arg_type0 theM0,
                        const tail_type & tail ):tuple1 < T0 > ( theM0, tail )
      {
      }
      tuple1 < bool > empty(  )const
      {
        return tuple1 < bool > ( m0.empty(  ) );
      }
      size_type size(  ) const
      {
        return size_type( m0.size(  ) );
      }
      iterator begin(  )
      {
        return iterator( m0.begin(  ) );
      }
      const_iterator begin(  ) const
      {
        return const_iterator( m0.begin(  ) );
      }
      iterator end(  )
      {
        return iterator( m0.end(  ) );
      }
      const_iterator end(  ) const
      {
        return const_iterator( m0.end(  ) );
      }
      reference operator[] ( const size_type & i )
      {
        return reference( &( m0[i.m0] ) );
      }
      const_reference operator[] ( const size_type & i ) const
      {
        return const_reference( m0[i.m0] );
      }
    };
    template < class T > struct n_fold_container1
    {
      typedef container_tuple1 < T > type;
    };
    template < class R0, class R1 > struct container_traits_tuple2
    {
      typedef tuple2 < typename R0::value_type,
        typename R1::value_type > value_type;
      typedef iterator_tuple2 < typename R0::iterator, typename R1::iterator,
        R0, R1 > iterator;
      typedef iterator_tuple2 < typename R0::const_iterator,
        typename R1::const_iterator, R0, R1 > const_iterator;
      typedef tuple2 < detail::assign_to_pointee < typename R0::value_type >,
        detail::assign_to_pointee < typename R1::value_type > >reference;
      typedef tuple2 < typename R0::value_type,
        typename R1::value_type > const_reference;
      typedef tuple2 < typename R0::pointer, typename R1::pointer > pointer;
      typedef tuple2 < typename R0::difference_type,
        typename R1::difference_type > difference_type;
    };
    template < class T0, class T1, class R0 =
      detail::iterator_traits < typename T0::iterator >, class R1 =
      detail::iterator_traits < typename T1::iterator >
      >struct container_tuple2:public tuple2 < T0, T1 >
    {
      typedef container_tuple2 < T0, T1 > self_type;
      typedef tuple2 < typename T0::size_type,
        typename T1::size_type > size_type;
      typedef container_traits_tuple2 < R0, R1 > traits;
      typedef typename traits::value_type value_type;
      typedef typename traits::iterator iterator;
      typedef typename traits::const_iterator const_iterator;
      typedef typename traits::reference reference;
      typedef typename traits::const_reference const_reference;
      typedef typename traits::pointer pointer;
      typedef typename traits::difference_type difference_type;
        container_tuple2( arg_type0 theM0, arg_type1 theM1 ):tuple2 < T0,
        T1 > ( theM0, theM1 )
      {
      }
      template < class S0,
        class S1 > container_tuple2( const iterator_tuple2 < S0,
                                     S1 > &rhs ):tuple2 < T0, T1 > ( rhs )
      {
      }
      container_tuple2( arg_type0 theM0, const tail_type & tail ):tuple2 < T0,
        T1 > ( theM0, tail )
      {
      }
      tuple2 < bool, bool > empty(  )const
      {
        return tuple2 < bool, bool > ( m0.empty(  ), m1.empty(  ) );
      }
      size_type size(  ) const
      {
        return size_type( m0.size(  ), m1.size(  ) );
      }
      iterator begin(  )
      {
        return iterator( m0.begin(  ), m1.begin(  ) );
      }
      const_iterator begin(  ) const
      {
        return const_iterator( m0.begin(  ), m1.begin(  ) );
      }
      iterator end(  )
      {
        return iterator( m0.end(  ), m1.end(  ) );
      }
      const_iterator end(  ) const
      {
        return const_iterator( m0.end(  ), m1.end(  ) );
      }
      reference operator[] ( const size_type & i )
      {
        return reference( &( m0[i.m0] ), &( m1[i.m1] ) );
      }
      const_reference operator[] ( const size_type & i ) const
      {
        return const_reference( m0[i.m0], m1[i.m1] );
      }
    };
    template < class T > struct n_fold_container2
    {
      typedef container_tuple2 < T, T > type;
    };
    template < class R0, class R1, class R2 > struct container_traits_tuple3
    {
      typedef tuple3 < typename R0::value_type, typename R1::value_type,
        typename R2::value_type > value_type;
      typedef iterator_tuple3 < typename R0::iterator, typename R1::iterator,
        typename R2::iterator, R0, R1, R2 > iterator;
      typedef iterator_tuple3 < typename R0::const_iterator,
        typename R1::const_iterator, typename R2::const_iterator, R0, R1,
        R2 > const_iterator;
      typedef tuple3 < detail::assign_to_pointee < typename R0::value_type >,
        detail::assign_to_pointee < typename R1::value_type >,
        detail::assign_to_pointee < typename R2::value_type > >reference;
      typedef tuple3 < typename R0::value_type, typename R1::value_type,
        typename R2::value_type > const_reference;
      typedef tuple3 < typename R0::pointer, typename R1::pointer,
        typename R2::pointer > pointer;
      typedef tuple3 < typename R0::difference_type,
        typename R1::difference_type,
        typename R2::difference_type > difference_type;
    };
    template < class T0, class T1, class T2, class R0 =
      detail::iterator_traits < typename T0::iterator >, class R1 =
      detail::iterator_traits < typename T1::iterator >, class R2 =
      detail::iterator_traits < typename T2::iterator >
      >struct container_tuple3:public tuple3 < T0, T1, T2 >
    {
      typedef container_tuple3 < T0, T1, T2 > self_type;
      typedef tuple3 < typename T0::size_type, typename T1::size_type,
        typename T2::size_type > size_type;
      typedef container_traits_tuple3 < R0, R1, R2 > traits;
      typedef typename traits::value_type value_type;
      typedef typename traits::iterator iterator;
      typedef typename traits::const_iterator const_iterator;
      typedef typename traits::reference reference;
      typedef typename traits::const_reference const_reference;
      typedef typename traits::pointer pointer;
      typedef typename traits::difference_type difference_type;
        container_tuple3( arg_type0 theM0, arg_type1 theM1,
                          arg_type2 theM2 ):tuple3 < T0, T1, T2 > ( theM0,
                                                                    theM1,
                                                                    theM2 )
      {
      }
      template < class S0, class S1,
        class S2 > container_tuple3( const iterator_tuple3 < S0, S1,
                                     S2 > &rhs ):tuple3 < T0, T1, T2 > ( rhs )
      {
      }
      container_tuple3( arg_type0 theM0, const tail_type & tail ):tuple3 < T0,
        T1, T2 > ( theM0, tail )
      {
      }
      tuple3 < bool, bool, bool > empty(  )const
      {
        return tuple3 < bool, bool, bool > ( m0.empty(  ), m1.empty(  ),
                                             m2.empty(  ) );
      }
      size_type size(  ) const
      {
        return size_type( m0.size(  ), m1.size(  ), m2.size(  ) );
      }
      iterator begin(  )
      {
        return iterator( m0.begin(  ), m1.begin(  ), m2.begin(  ) );
      }
      const_iterator begin(  ) const
      {
        return const_iterator( m0.begin(  ), m1.begin(  ), m2.begin(  ) );
      }
      iterator end(  )
      {
        return iterator( m0.end(  ), m1.end(  ), m2.end(  ) );
      }
      const_iterator end(  ) const
      {
        return const_iterator( m0.end(  ), m1.end(  ), m2.end(  ) );
      }
      reference operator[] ( const size_type & i )
      {
        return reference( &( m0[i.m0] ), &( m1[i.m1] ), &( m2[i.m2] ) );
      }
      const_reference operator[] ( const size_type & i ) const
      {
        return const_reference( m0[i.m0], m1[i.m1], m2[i.m2] );
      }
    };
    template < class T > struct n_fold_container3
    {
      typedef container_tuple3 < T, T, T > type;
    };
    template < class R0, class R1, class R2, class R3 > struct container_traits_tuple4
    {
      typedef tuple4 < typename R0::value_type, typename R1::value_type,
        typename R2::value_type, typename R3::value_type > value_type;
      typedef iterator_tuple4 < typename R0::iterator, typename R1::iterator,
        typename R2::iterator, typename R3::iterator, R0, R1, R2,
        R3 > iterator;
      typedef iterator_tuple4 < typename R0::const_iterator,
        typename R1::const_iterator, typename R2::const_iterator,
        typename R3::const_iterator, R0, R1, R2, R3 > const_iterator;
      typedef tuple4 < detail::assign_to_pointee < typename R0::value_type >,
        detail::assign_to_pointee < typename R1::value_type >,
        detail::assign_to_pointee < typename R2::value_type >,
        detail::assign_to_pointee < typename R3::value_type > >reference;
      typedef tuple4 < typename R0::value_type, typename R1::value_type,
        typename R2::value_type, typename R3::value_type > const_reference;
      typedef tuple4 < typename R0::pointer, typename R1::pointer,
        typename R2::pointer, typename R3::pointer > pointer;
      typedef tuple4 < typename R0::difference_type,
        typename R1::difference_type, typename R2::difference_type,
        typename R3::difference_type > difference_type;
    };
    template < class T0, class T1, class T2, class T3, class R0 =
      detail::iterator_traits < typename T0::iterator >, class R1 =
      detail::iterator_traits < typename T1::iterator >, class R2 =
      detail::iterator_traits < typename T2::iterator >, class R3 =
      detail::iterator_traits < typename T3::iterator >
      >struct container_tuple4:public tuple4 < T0, T1, T2, T3 >
    {
      typedef container_tuple4 < T0, T1, T2, T3 > self_type;
      typedef tuple4 < typename T0::size_type, typename T1::size_type,
        typename T2::size_type, typename T3::size_type > size_type;
      typedef container_traits_tuple4 < R0, R1, R2, R3 > traits;
      typedef typename traits::value_type value_type;
      typedef typename traits::iterator iterator;
      typedef typename traits::const_iterator const_iterator;
      typedef typename traits::reference reference;
      typedef typename traits::const_reference const_reference;
      typedef typename traits::pointer pointer;
      typedef typename traits::difference_type difference_type;
        container_tuple4( arg_type0 theM0, arg_type1 theM1, arg_type2 theM2,
                          arg_type3 theM3 ):tuple4 < T0, T1, T2, T3 > ( theM0,
                                                                        theM1,
                                                                        theM2,
                                                                        theM3 )
      {
      }
      template < class S0, class S1, class S2,
        class S3 > container_tuple4( const iterator_tuple4 < S0, S1, S2,
                                     S3 > &rhs ):tuple4 < T0, T1, T2,
        T3 > ( rhs )
      {
      }
      container_tuple4( arg_type0 theM0, const tail_type & tail ):tuple4 < T0,
        T1, T2, T3 > ( theM0, tail )
      {
      }
      tuple4 < bool, bool, bool, bool > empty(  )const
      {
        return tuple4 < bool, bool, bool, bool > ( m0.empty(  ), m1.empty(  ),
                                                   m2.empty(  ),
                                                   m3.empty(  ) );
      }
      size_type size(  ) const
      {
        return size_type( m0.size(  ), m1.size(  ), m2.size(  ),
                          m3.size(  ) );
      }
      iterator begin(  )
      {
        return iterator( m0.begin(  ), m1.begin(  ), m2.begin(  ),
                         m3.begin(  ) );
      }
      const_iterator begin(  ) const
      {
        return const_iterator( m0.begin(  ), m1.begin(  ), m2.begin(  ),
                               m3.begin(  ) );
      }
      iterator end(  )
      {
        return iterator( m0.end(  ), m1.end(  ), m2.end(  ), m3.end(  ) );
      }
      const_iterator end(  ) const
      {
        return const_iterator( m0.end(  ), m1.end(  ), m2.end(  ),
                               m3.end(  ) );
      }
      reference operator[] ( const size_type & i )
      {
        return reference( &( m0[i.m0] ), &( m1[i.m1] ), &( m2[i.m2] ),
                          &( m3[i.m3] ) );
      }
      const_reference operator[] ( const size_type & i ) const
      {
        return const_reference( m0[i.m0], m1[i.m1], m2[i.m2], m3[i.m3] );
      }
    };
    template < class T > struct n_fold_container4
    {
      typedef container_tuple4 < T, T, T, T > type;
    };
    template < class R0, class R1, class R2, class R3, class R4 > struct container_traits_tuple5
    {
      typedef tuple5 < typename R0::value_type, typename R1::value_type,
        typename R2::value_type, typename R3::value_type,
        typename R4::value_type > value_type;
      typedef iterator_tuple5 < typename R0::iterator, typename R1::iterator,
        typename R2::iterator, typename R3::iterator, typename R4::iterator,
        R0, R1, R2, R3, R4 > iterator;
      typedef iterator_tuple5 < typename R0::const_iterator,
        typename R1::const_iterator, typename R2::const_iterator,
        typename R3::const_iterator, typename R4::const_iterator, R0, R1, R2,
        R3, R4 > const_iterator;
      typedef tuple5 < detail::assign_to_pointee < typename R0::value_type >,
        detail::assign_to_pointee < typename R1::value_type >,
        detail::assign_to_pointee < typename R2::value_type >,
        detail::assign_to_pointee < typename R3::value_type >,
        detail::assign_to_pointee < typename R4::value_type > >reference;
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
    template < class T0, class T1, class T2, class T3, class T4, class R0 =
      detail::iterator_traits < typename T0::iterator >, class R1 =
      detail::iterator_traits < typename T1::iterator >, class R2 =
      detail::iterator_traits < typename T2::iterator >, class R3 =
      detail::iterator_traits < typename T3::iterator >, class R4 =
      detail::iterator_traits < typename T4::iterator >
      >struct container_tuple5:public tuple5 < T0, T1, T2, T3, T4 >
    {
      typedef container_tuple5 < T0, T1, T2, T3, T4 > self_type;
      typedef tuple5 < typename T0::size_type, typename T1::size_type,
        typename T2::size_type, typename T3::size_type,
        typename T4::size_type > size_type;
      typedef container_traits_tuple5 < R0, R1, R2, R3, R4 > traits;
      typedef typename traits::value_type value_type;
      typedef typename traits::iterator iterator;
      typedef typename traits::const_iterator const_iterator;
      typedef typename traits::reference reference;
      typedef typename traits::const_reference const_reference;
      typedef typename traits::pointer pointer;
      typedef typename traits::difference_type difference_type;
        container_tuple5( arg_type0 theM0, arg_type1 theM1, arg_type2 theM2,
                          arg_type3 theM3, arg_type4 theM4 ):tuple5 < T0, T1,
        T2, T3, T4 > ( theM0, theM1, theM2, theM3, theM4 )
      {
      }
      template < class S0, class S1, class S2, class S3,
        class S4 > container_tuple5( const iterator_tuple5 < S0, S1, S2, S3,
                                     S4 > &rhs ):tuple5 < T0, T1, T2, T3,
        T4 > ( rhs )
      {
      }
      container_tuple5( arg_type0 theM0, const tail_type & tail ):tuple5 < T0,
        T1, T2, T3, T4 > ( theM0, tail )
      {
      }
      tuple5 < bool, bool, bool, bool, bool > empty(  )const
      {
        return tuple5 < bool, bool, bool, bool, bool > ( m0.empty(  ),
                                                         m1.empty(  ),
                                                         m2.empty(  ),
                                                         m3.empty(  ),
                                                         m4.empty(  ) );
      }
      size_type size(  ) const
      {
        return size_type( m0.size(  ), m1.size(  ), m2.size(  ), m3.size(  ),
                          m4.size(  ) );
      }
      iterator begin(  )
      {
        return iterator( m0.begin(  ), m1.begin(  ), m2.begin(  ),
                         m3.begin(  ), m4.begin(  ) );
      }
      const_iterator begin(  ) const
      {
        return const_iterator( m0.begin(  ), m1.begin(  ), m2.begin(  ),
                               m3.begin(  ), m4.begin(  ) );
      }
      iterator end(  )
      {
        return iterator( m0.end(  ), m1.end(  ), m2.end(  ), m3.end(  ),
                         m4.end(  ) );
      }
      const_iterator end(  ) const
      {
        return const_iterator( m0.end(  ), m1.end(  ), m2.end(  ), m3.end(  ),
                               m4.end(  ) );
      }
      reference operator[] ( const size_type & i )
      {
        return reference( &( m0[i.m0] ), &( m1[i.m1] ), &( m2[i.m2] ),
                          &( m3[i.m3] ), &( m4[i.m4] ) );
      }
      const_reference operator[] ( const size_type & i ) const
      {
        return const_reference( m0[i.m0], m1[i.m1], m2[i.m2], m3[i.m3],
                                m4[i.m4] );
      }
    };
    template < class T > struct n_fold_container5
    {
      typedef container_tuple5 < T, T, T, T, T > type;
    };
    template < class R0, class R1, class R2, class R3, class R4, class R5 > struct container_traits_tuple6
    {
      typedef tuple6 < typename R0::value_type, typename R1::value_type,
        typename R2::value_type, typename R3::value_type,
        typename R4::value_type, typename R5::value_type > value_type;
      typedef iterator_tuple6 < typename R0::iterator, typename R1::iterator,
        typename R2::iterator, typename R3::iterator, typename R4::iterator,
        typename R5::iterator, R0, R1, R2, R3, R4, R5 > iterator;
      typedef iterator_tuple6 < typename R0::const_iterator,
        typename R1::const_iterator, typename R2::const_iterator,
        typename R3::const_iterator, typename R4::const_iterator,
        typename R5::const_iterator, R0, R1, R2, R3, R4, R5 > const_iterator;
      typedef tuple6 < detail::assign_to_pointee < typename R0::value_type >,
        detail::assign_to_pointee < typename R1::value_type >,
        detail::assign_to_pointee < typename R2::value_type >,
        detail::assign_to_pointee < typename R3::value_type >,
        detail::assign_to_pointee < typename R4::value_type >,
        detail::assign_to_pointee < typename R5::value_type > >reference;
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
    template < class T0, class T1, class T2, class T3, class T4, class T5,
      class R0 = detail::iterator_traits < typename T0::iterator >, class R1 =
      detail::iterator_traits < typename T1::iterator >, class R2 =
      detail::iterator_traits < typename T2::iterator >, class R3 =
      detail::iterator_traits < typename T3::iterator >, class R4 =
      detail::iterator_traits < typename T4::iterator >, class R5 =
      detail::iterator_traits < typename T5::iterator >
      >struct container_tuple6:public tuple6 < T0, T1, T2, T3, T4, T5 >
    {
      typedef container_tuple6 < T0, T1, T2, T3, T4, T5 > self_type;
      typedef tuple6 < typename T0::size_type, typename T1::size_type,
        typename T2::size_type, typename T3::size_type,
        typename T4::size_type, typename T5::size_type > size_type;
      typedef container_traits_tuple6 < R0, R1, R2, R3, R4, R5 > traits;
      typedef typename traits::value_type value_type;
      typedef typename traits::iterator iterator;
      typedef typename traits::const_iterator const_iterator;
      typedef typename traits::reference reference;
      typedef typename traits::const_reference const_reference;
      typedef typename traits::pointer pointer;
      typedef typename traits::difference_type difference_type;
        container_tuple6( arg_type0 theM0, arg_type1 theM1, arg_type2 theM2,
                          arg_type3 theM3, arg_type4 theM4,
                          arg_type5 theM5 ):tuple6 < T0, T1, T2, T3, T4,
        T5 > ( theM0, theM1, theM2, theM3, theM4, theM5 )
      {
      }
      template < class S0, class S1, class S2, class S3, class S4,
        class S5 > container_tuple6( const iterator_tuple6 < S0, S1, S2, S3,
                                     S4, S5 > &rhs ):tuple6 < T0, T1, T2, T3,
        T4, T5 > ( rhs )
      {
      }
      container_tuple6( arg_type0 theM0, const tail_type & tail ):tuple6 < T0,
        T1, T2, T3, T4, T5 > ( theM0, tail )
      {
      }
      tuple6 < bool, bool, bool, bool, bool, bool > empty(  )const
      {
        return tuple6 < bool, bool, bool, bool, bool, bool > ( m0.empty(  ),
                                                               m1.empty(  ),
                                                               m2.empty(  ),
                                                               m3.empty(  ),
                                                               m4.empty(  ),
                                                               m5.empty(  ) );
      }
      size_type size(  ) const
      {
        return size_type( m0.size(  ), m1.size(  ), m2.size(  ), m3.size(  ),
                          m4.size(  ), m5.size(  ) );
      }
      iterator begin(  )
      {
        return iterator( m0.begin(  ), m1.begin(  ), m2.begin(  ),
                         m3.begin(  ), m4.begin(  ), m5.begin(  ) );
      }
      const_iterator begin(  ) const
      {
        return const_iterator( m0.begin(  ), m1.begin(  ), m2.begin(  ),
                               m3.begin(  ), m4.begin(  ), m5.begin(  ) );
      }
      iterator end(  )
      {
        return iterator( m0.end(  ), m1.end(  ), m2.end(  ), m3.end(  ),
                         m4.end(  ), m5.end(  ) );
      }
      const_iterator end(  ) const
      {
        return const_iterator( m0.end(  ), m1.end(  ), m2.end(  ), m3.end(  ),
                               m4.end(  ), m5.end(  ) );
      }
      reference operator[] ( const size_type & i )
      {
        return reference( &( m0[i.m0] ), &( m1[i.m1] ), &( m2[i.m2] ),
                          &( m3[i.m3] ), &( m4[i.m4] ), &( m5[i.m5] ) );
      }
      const_reference operator[] ( const size_type & i ) const
      {
        return const_reference( m0[i.m0], m1[i.m1], m2[i.m2], m3[i.m3],
                                m4[i.m4], m5[i.m5] );
      }
    };
    template < class T > struct n_fold_container6
    {
      typedef container_tuple6 < T, T, T, T, T, T > type;
    };
    template < class R0, class R1, class R2, class R3, class R4, class R5,
      class R6 > struct container_traits_tuple7
    {
      typedef tuple7 < typename R0::value_type, typename R1::value_type,
        typename R2::value_type, typename R3::value_type,
        typename R4::value_type, typename R5::value_type,
        typename R6::value_type > value_type;
      typedef iterator_tuple7 < typename R0::iterator, typename R1::iterator,
        typename R2::iterator, typename R3::iterator, typename R4::iterator,
        typename R5::iterator, typename R6::iterator, R0, R1, R2, R3, R4, R5,
        R6 > iterator;
      typedef iterator_tuple7 < typename R0::const_iterator,
        typename R1::const_iterator, typename R2::const_iterator,
        typename R3::const_iterator, typename R4::const_iterator,
        typename R5::const_iterator, typename R6::const_iterator, R0, R1, R2,
        R3, R4, R5, R6 > const_iterator;
      typedef tuple7 < detail::assign_to_pointee < typename R0::value_type >,
        detail::assign_to_pointee < typename R1::value_type >,
        detail::assign_to_pointee < typename R2::value_type >,
        detail::assign_to_pointee < typename R3::value_type >,
        detail::assign_to_pointee < typename R4::value_type >,
        detail::assign_to_pointee < typename R5::value_type >,
        detail::assign_to_pointee < typename R6::value_type > >reference;
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
      class T6, class R0 =
      detail::iterator_traits < typename T0::iterator >, class R1 =
      detail::iterator_traits < typename T1::iterator >, class R2 =
      detail::iterator_traits < typename T2::iterator >, class R3 =
      detail::iterator_traits < typename T3::iterator >, class R4 =
      detail::iterator_traits < typename T4::iterator >, class R5 =
      detail::iterator_traits < typename T5::iterator >, class R6 =
      detail::iterator_traits < typename T6::iterator >
      >struct container_tuple7:public tuple7 < T0, T1, T2, T3, T4, T5, T6 >
    {
      typedef container_tuple7 < T0, T1, T2, T3, T4, T5, T6 > self_type;
      typedef tuple7 < typename T0::size_type, typename T1::size_type,
        typename T2::size_type, typename T3::size_type,
        typename T4::size_type, typename T5::size_type,
        typename T6::size_type > size_type;
      typedef container_traits_tuple7 < R0, R1, R2, R3, R4, R5, R6 > traits;
      typedef typename traits::value_type value_type;
      typedef typename traits::iterator iterator;
      typedef typename traits::const_iterator const_iterator;
      typedef typename traits::reference reference;
      typedef typename traits::const_reference const_reference;
      typedef typename traits::pointer pointer;
      typedef typename traits::difference_type difference_type;
        container_tuple7( arg_type0 theM0, arg_type1 theM1, arg_type2 theM2,
                          arg_type3 theM3, arg_type4 theM4, arg_type5 theM5,
                          arg_type6 theM6 ):tuple7 < T0, T1, T2, T3, T4, T5,
        T6 > ( theM0, theM1, theM2, theM3, theM4, theM5, theM6 )
      {
      }
      template < class S0, class S1, class S2, class S3, class S4, class S5,
        class S6 > container_tuple7( const iterator_tuple7 < S0, S1, S2, S3,
                                     S4, S5, S6 > &rhs ):tuple7 < T0, T1, T2,
        T3, T4, T5, T6 > ( rhs )
      {
      }
      container_tuple7( arg_type0 theM0, const tail_type & tail ):tuple7 < T0,
        T1, T2, T3, T4, T5, T6 > ( theM0, tail )
      {
      }
      tuple7 < bool, bool, bool, bool, bool, bool, bool > empty(  )const
      {
        return tuple7 < bool, bool, bool, bool, bool, bool,
          bool > ( m0.empty(  ), m1.empty(  ), m2.empty(  ), m3.empty(  ),
                   m4.empty(  ), m5.empty(  ), m6.empty(  ) );
      }
      size_type size(  ) const
      {
        return size_type( m0.size(  ), m1.size(  ), m2.size(  ), m3.size(  ),
                          m4.size(  ), m5.size(  ), m6.size(  ) );
      }
      iterator begin(  )
      {
        return iterator( m0.begin(  ), m1.begin(  ), m2.begin(  ),
                         m3.begin(  ), m4.begin(  ), m5.begin(  ),
                         m6.begin(  ) );
      }
      const_iterator begin(  ) const
      {
        return const_iterator( m0.begin(  ), m1.begin(  ), m2.begin(  ),
                               m3.begin(  ), m4.begin(  ), m5.begin(  ),
                               m6.begin(  ) );
      }
      iterator end(  )
      {
        return iterator( m0.end(  ), m1.end(  ), m2.end(  ), m3.end(  ),
                         m4.end(  ), m5.end(  ), m6.end(  ) );
      }
      const_iterator end(  ) const
      {
        return const_iterator( m0.end(  ), m1.end(  ), m2.end(  ), m3.end(  ),
                               m4.end(  ), m5.end(  ), m6.end(  ) );
      }
      reference operator[] ( const size_type & i )
      {
        return reference( &( m0[i.m0] ), &( m1[i.m1] ), &( m2[i.m2] ),
                          &( m3[i.m3] ), &( m4[i.m4] ), &( m5[i.m5] ),
                          &( m6[i.m6] ) );
      }
      const_reference operator[] ( const size_type & i ) const
      {
        return const_reference( m0[i.m0], m1[i.m1], m2[i.m2], m3[i.m3],
                                m4[i.m4], m5[i.m5], m6[i.m6] );
      }
    };
    template < class T > struct n_fold_container7
    {
      typedef container_tuple7 < T, T, T, T, T, T, T > type;
    };
    template < class R0, class R1, class R2, class R3, class R4, class R5,
      class R6, class R7 > struct container_traits_tuple8
    {
      typedef tuple8 < typename R0::value_type, typename R1::value_type,
        typename R2::value_type, typename R3::value_type,
        typename R4::value_type, typename R5::value_type,
        typename R6::value_type, typename R7::value_type > value_type;
      typedef iterator_tuple8 < typename R0::iterator, typename R1::iterator,
        typename R2::iterator, typename R3::iterator, typename R4::iterator,
        typename R5::iterator, typename R6::iterator, typename R7::iterator,
        R0, R1, R2, R3, R4, R5, R6, R7 > iterator;
      typedef iterator_tuple8 < typename R0::const_iterator,
        typename R1::const_iterator, typename R2::const_iterator,
        typename R3::const_iterator, typename R4::const_iterator,
        typename R5::const_iterator, typename R6::const_iterator,
        typename R7::const_iterator, R0, R1, R2, R3, R4, R5, R6,
        R7 > const_iterator;
      typedef tuple8 < detail::assign_to_pointee < typename R0::value_type >,
        detail::assign_to_pointee < typename R1::value_type >,
        detail::assign_to_pointee < typename R2::value_type >,
        detail::assign_to_pointee < typename R3::value_type >,
        detail::assign_to_pointee < typename R4::value_type >,
        detail::assign_to_pointee < typename R5::value_type >,
        detail::assign_to_pointee < typename R6::value_type >,
        detail::assign_to_pointee < typename R7::value_type > >reference;
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
      class T6, class T7, class R0 =
      detail::iterator_traits < typename T0::iterator >, class R1 =
      detail::iterator_traits < typename T1::iterator >, class R2 =
      detail::iterator_traits < typename T2::iterator >, class R3 =
      detail::iterator_traits < typename T3::iterator >, class R4 =
      detail::iterator_traits < typename T4::iterator >, class R5 =
      detail::iterator_traits < typename T5::iterator >, class R6 =
      detail::iterator_traits < typename T6::iterator >, class R7 =
      detail::iterator_traits < typename T7::iterator >
      >struct container_tuple8:public tuple8 < T0, T1, T2, T3, T4, T5, T6,
      T7 >
    {
      typedef container_tuple8 < T0, T1, T2, T3, T4, T5, T6, T7 > self_type;
      typedef tuple8 < typename T0::size_type, typename T1::size_type,
        typename T2::size_type, typename T3::size_type,
        typename T4::size_type, typename T5::size_type,
        typename T6::size_type, typename T7::size_type > size_type;
      typedef container_traits_tuple8 < R0, R1, R2, R3, R4, R5, R6,
        R7 > traits;
      typedef typename traits::value_type value_type;
      typedef typename traits::iterator iterator;
      typedef typename traits::const_iterator const_iterator;
      typedef typename traits::reference reference;
      typedef typename traits::const_reference const_reference;
      typedef typename traits::pointer pointer;
      typedef typename traits::difference_type difference_type;
        container_tuple8( arg_type0 theM0, arg_type1 theM1, arg_type2 theM2,
                          arg_type3 theM3, arg_type4 theM4, arg_type5 theM5,
                          arg_type6 theM6, arg_type7 theM7 ):tuple8 < T0, T1,
        T2, T3, T4, T5, T6, T7 > ( theM0, theM1, theM2, theM3, theM4, theM5,
                                   theM6, theM7 )
      {
      }
      template < class S0, class S1, class S2, class S3, class S4, class S5,
        class S6, class S7 > container_tuple8( const iterator_tuple8 < S0, S1,
                                               S2, S3, S4, S5, S6,
                                               S7 > &rhs ):tuple8 < T0, T1,
        T2, T3, T4, T5, T6, T7 > ( rhs )
      {
      }
      container_tuple8( arg_type0 theM0, const tail_type & tail ):tuple8 < T0,
        T1, T2, T3, T4, T5, T6, T7 > ( theM0, tail )
      {
      }
      tuple8 < bool, bool, bool, bool, bool, bool, bool, bool > empty(  )const
      {
        return tuple8 < bool, bool, bool, bool, bool, bool, bool,
          bool > ( m0.empty(  ), m1.empty(  ), m2.empty(  ), m3.empty(  ),
                   m4.empty(  ), m5.empty(  ), m6.empty(  ), m7.empty(  ) );
      }
      size_type size(  ) const
      {
        return size_type( m0.size(  ), m1.size(  ), m2.size(  ), m3.size(  ),
                          m4.size(  ), m5.size(  ), m6.size(  ),
                          m7.size(  ) );
      }
      iterator begin(  )
      {
        return iterator( m0.begin(  ), m1.begin(  ), m2.begin(  ),
                         m3.begin(  ), m4.begin(  ), m5.begin(  ),
                         m6.begin(  ), m7.begin(  ) );
      }
      const_iterator begin(  ) const
      {
        return const_iterator( m0.begin(  ), m1.begin(  ), m2.begin(  ),
                               m3.begin(  ), m4.begin(  ), m5.begin(  ),
                               m6.begin(  ), m7.begin(  ) );
      }
      iterator end(  )
      {
        return iterator( m0.end(  ), m1.end(  ), m2.end(  ), m3.end(  ),
                         m4.end(  ), m5.end(  ), m6.end(  ), m7.end(  ) );
      }
      const_iterator end(  ) const
      {
        return const_iterator( m0.end(  ), m1.end(  ), m2.end(  ), m3.end(  ),
                               m4.end(  ), m5.end(  ), m6.end(  ),
                               m7.end(  ) );
      }
      reference operator[] ( const size_type & i )
      {
        return reference( &( m0[i.m0] ), &( m1[i.m1] ), &( m2[i.m2] ),
                          &( m3[i.m3] ), &( m4[i.m4] ), &( m5[i.m5] ),
                          &( m6[i.m6] ), &( m7[i.m7] ) );
      }
      const_reference operator[] ( const size_type & i ) const
      {
        return const_reference( m0[i.m0], m1[i.m1], m2[i.m2], m3[i.m3],
                                m4[i.m4], m5[i.m5], m6[i.m6], m7[i.m7] );
      }
    };
    template < class T > struct n_fold_container8
    {
      typedef container_tuple8 < T, T, T, T, T, T, T, T > type;
    };
    template < class R0, class R1, class R2, class R3, class R4, class R5,
      class R6, class R7, class R8 > struct container_traits_tuple9
    {
      typedef tuple9 < typename R0::value_type, typename R1::value_type,
        typename R2::value_type, typename R3::value_type,
        typename R4::value_type, typename R5::value_type,
        typename R6::value_type, typename R7::value_type,
        typename R8::value_type > value_type;
      typedef iterator_tuple9 < typename R0::iterator, typename R1::iterator,
        typename R2::iterator, typename R3::iterator, typename R4::iterator,
        typename R5::iterator, typename R6::iterator, typename R7::iterator,
        typename R8::iterator, R0, R1, R2, R3, R4, R5, R6, R7, R8 > iterator;
      typedef iterator_tuple9 < typename R0::const_iterator,
        typename R1::const_iterator, typename R2::const_iterator,
        typename R3::const_iterator, typename R4::const_iterator,
        typename R5::const_iterator, typename R6::const_iterator,
        typename R7::const_iterator, typename R8::const_iterator, R0, R1, R2,
        R3, R4, R5, R6, R7, R8 > const_iterator;
      typedef tuple9 < detail::assign_to_pointee < typename R0::value_type >,
        detail::assign_to_pointee < typename R1::value_type >,
        detail::assign_to_pointee < typename R2::value_type >,
        detail::assign_to_pointee < typename R3::value_type >,
        detail::assign_to_pointee < typename R4::value_type >,
        detail::assign_to_pointee < typename R5::value_type >,
        detail::assign_to_pointee < typename R6::value_type >,
        detail::assign_to_pointee < typename R7::value_type >,
        detail::assign_to_pointee < typename R8::value_type > >reference;
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
      class T6, class T7, class T8, class R0 =
      detail::iterator_traits < typename T0::iterator >, class R1 =
      detail::iterator_traits < typename T1::iterator >, class R2 =
      detail::iterator_traits < typename T2::iterator >, class R3 =
      detail::iterator_traits < typename T3::iterator >, class R4 =
      detail::iterator_traits < typename T4::iterator >, class R5 =
      detail::iterator_traits < typename T5::iterator >, class R6 =
      detail::iterator_traits < typename T6::iterator >, class R7 =
      detail::iterator_traits < typename T7::iterator >, class R8 =
      detail::iterator_traits < typename T8::iterator >
      >struct container_tuple9:public tuple9 < T0, T1, T2, T3, T4, T5, T6, T7,
      T8 >
    {
      typedef container_tuple9 < T0, T1, T2, T3, T4, T5, T6, T7,
        T8 > self_type;
      typedef tuple9 < typename T0::size_type, typename T1::size_type,
        typename T2::size_type, typename T3::size_type,
        typename T4::size_type, typename T5::size_type,
        typename T6::size_type, typename T7::size_type,
        typename T8::size_type > size_type;
      typedef container_traits_tuple9 < R0, R1, R2, R3, R4, R5, R6, R7,
        R8 > traits;
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
                          arg_type8 theM8 ):tuple9 < T0, T1, T2, T3, T4, T5,
        T6, T7, T8 > ( theM0, theM1, theM2, theM3, theM4, theM5, theM6, theM7,
                       theM8 )
      {
      }
      template < class S0, class S1, class S2, class S3, class S4, class S5,
        class S6, class S7,
        class S8 > container_tuple9( const iterator_tuple9 < S0, S1, S2, S3,
                                     S4, S5, S6, S7, S8 > &rhs ):tuple9 < T0,
        T1, T2, T3, T4, T5, T6, T7, T8 > ( rhs )
      {
      }
      container_tuple9( arg_type0 theM0, const tail_type & tail ):tuple9 < T0,
        T1, T2, T3, T4, T5, T6, T7, T8 > ( theM0, tail )
      {
      }
      tuple9 < bool, bool, bool, bool, bool, bool, bool, bool,
        bool > empty(  )const
      {
        return tuple9 < bool, bool, bool, bool, bool, bool, bool, bool,
          bool > ( m0.empty(  ), m1.empty(  ), m2.empty(  ), m3.empty(  ),
                   m4.empty(  ), m5.empty(  ), m6.empty(  ), m7.empty(  ),
                   m8.empty(  ) );
      }
      size_type size(  ) const
      {
        return size_type( m0.size(  ), m1.size(  ), m2.size(  ), m3.size(  ),
                          m4.size(  ), m5.size(  ), m6.size(  ), m7.size(  ),
                          m8.size(  ) );
      }
      iterator begin(  )
      {
        return iterator( m0.begin(  ), m1.begin(  ), m2.begin(  ),
                         m3.begin(  ), m4.begin(  ), m5.begin(  ),
                         m6.begin(  ), m7.begin(  ), m8.begin(  ) );
      }
      const_iterator begin(  ) const
      {
        return const_iterator( m0.begin(  ), m1.begin(  ), m2.begin(  ),
                               m3.begin(  ), m4.begin(  ), m5.begin(  ),
                               m6.begin(  ), m7.begin(  ), m8.begin(  ) );
      }
      iterator end(  )
      {
        return iterator( m0.end(  ), m1.end(  ), m2.end(  ), m3.end(  ),
                         m4.end(  ), m5.end(  ), m6.end(  ), m7.end(  ),
                         m8.end(  ) );
      }
      const_iterator end(  ) const
      {
        return const_iterator( m0.end(  ), m1.end(  ), m2.end(  ), m3.end(  ),
                               m4.end(  ), m5.end(  ), m6.end(  ), m7.end(  ),
                               m8.end(  ) );
      }
      reference operator[] ( const size_type & i )
      {
        return reference( &( m0[i.m0] ), &( m1[i.m1] ), &( m2[i.m2] ),
                          &( m3[i.m3] ), &( m4[i.m4] ), &( m5[i.m5] ),
                          &( m6[i.m6] ), &( m7[i.m7] ), &( m8[i.m8] ) );
      }
      const_reference operator[] ( const size_type & i ) const
      {
        return const_reference( m0[i.m0], m1[i.m1], m2[i.m2], m3[i.m3],
                                m4[i.m4], m5[i.m5], m6[i.m6], m7[i.m7],
                                m8[i.m8] );
      }
    };
    template < class T > struct n_fold_container9
    {
      typedef container_tuple9 < T, T, T, T, T, T, T, T, T > type;
    };
  }                             // namespace tupple
}                               // namespace boost


#endif
