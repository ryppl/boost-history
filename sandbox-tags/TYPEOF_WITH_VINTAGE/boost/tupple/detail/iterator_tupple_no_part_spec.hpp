// Copyright (C) 2003 Roland Richter <roland@flll.jku.at>
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all copies.
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.



// Copyright (C) 2003 Roland Richter <roland@flll.jku.at>
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all copies.
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.
#ifndef BOOST_TUPPLE_DETAIL_ITERATOR_TUPPLE_NO_PART_SPEC_HPP
#define BOOST_TUPPLE_DETAIL_ITERATOR_TUPPLE_NO_PART_SPEC_HPP
#include <boost/tupple/tupple.hpp>
#include <boost/tupple/detail/iterator_tupple_traits_detail.hpp>
#include <boost/tupple/detail/ref_workaround.hpp>

#include <boost/utility.hpp>


#include <boost/type_traits.hpp>
#include <boost/mpl/if.hpp>
#include <boost/static_assert.hpp>


namespace boost
{
  namespace tupple
  {
    template < class T >
    struct iterator_tuple0
    {
      typedef iterator_tuple0 < T > self_type;
    };



    template < class R0 >
    struct iterator_traits_tuple1
    {
      typedef tuple1 < typename R0::iterator_category > iterator_category;
      typedef tuple1 < typename R0::value_type > value_type;
      typedef tuple1 < detail::reference_wrapper < typename R0::value_type >
      > reference;
      typedef tuple1 < detail::reference_wrapper <
      typename R0::value_type const > > const_reference;
      typedef tuple1 < typename R0::pointer > pointer;
      typedef tuple1 < typename R0::difference_type > difference_type;
    };
    template < class T0 >
    struct iterator_tuple1: public tuple1 < T0 >
    {
      typedef iterator_tuple1 < T0 > self_type;
      typedef tuple1 < T0 > parent_type;
      typedef iterator_traits_tuple1 < detail::iterator_traits < T0 > >traits;
      typedef typename traits::iterator_category iterator_category;
      typedef typename traits::value_type value_type;
      typedef typename traits::reference reference;
      typedef typename traits::const_reference const_reference;
      typedef typename traits::pointer pointer;
      typedef typename traits::difference_type difference_type;
      iterator_tuple1( )
      {}
      iterator_tuple1( arg_type0 theM0 ) : parent_type( theM0 )
      {}
      template < class S0 >
      iterator_tuple1( const iterator_tuple1 < S0 >
                       &rhs ) : parent_type( rhs )
      {}
      iterator_tuple1( arg_type0 theM0,
                       const tail_type & tail ) : parent_type( theM0, tail )
      {}
      self_type & operator++( )
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
      self_type & operator--( )
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
      reference operator*( )
      {
        return reference( detail::ref( *m0 ) );
      }
      const_reference operator*( ) const
      {
        return const_reference( detail::cref( *m0 ) );
      }
      reference operator[] ( const difference_type & i )
      {
        return reference( detail::ref( m0[ i.m0 ] ) );
      }
      const_reference operator[] ( const difference_type & i ) const
      {
        return const_reference( detail::cref( m0[ i.m0 ] ) );
      }
    };
    template < class T0 >
    iterator_tuple1 < T0 >
    operator+( const iterator_tuple1 < T0 > &lhs,
               const iterator_tuple1 < T0 >::difference_type & rhs )
    {
      return iterator_tuple1 < T0 > ( lhs ) += rhs;
    }
    template < class T0 >
    iterator_tuple1 < T0 >
    operator-( const iterator_tuple1 < T0 > &lhs,
               const iterator_tuple1 < T0 >::difference_type & rhs )
    {
      return iterator_tuple1 < T0 > ( lhs ) -= rhs;
    }
    template < class T0 >
    iterator_tuple1 <
    T0 > ::difference_type operator-( const iterator_tuple1 < T0 > &lhs,
                                      const iterator_tuple1 < T0 > &rhs )
    {
      return iterator_tuple1 < T0 >::difference_type( lhs.m0 - rhs.m0 );
    }
    template < class R0, class R1 >
    struct iterator_traits_tuple2
    {
      typedef tuple2 < typename R0::iterator_category,
      typename R1::iterator_category > iterator_category;
      typedef tuple2 < typename R0::value_type,
      typename R1::value_type > value_type;
      typedef tuple2 < detail::reference_wrapper < typename R0::value_type >,
      detail::reference_wrapper < typename R1::value_type > > reference;
      typedef tuple2 < detail::reference_wrapper <
      typename R0::value_type const > ,
      detail::reference_wrapper <
      typename R1::value_type const > > const_reference;
      typedef tuple2 < typename R0::pointer, typename R1::pointer > pointer;
      typedef tuple2 < typename R0::difference_type,
      typename R1::difference_type > difference_type;
    };
    template < class T0, class T1 >
    struct iterator_tuple2: public tuple2 < T0,
          T1 >
    {
      typedef iterator_tuple2 < T0, T1 > self_type;
      typedef tuple2 < T0, T1 > parent_type;
      typedef iterator_traits_tuple2 < detail::iterator_traits < T0 >,
      detail::iterator_traits < T1 > > traits;
      typedef typename traits::iterator_category iterator_category;
      typedef typename traits::value_type value_type;
      typedef typename traits::reference reference;
      typedef typename traits::const_reference const_reference;
      typedef typename traits::pointer pointer;
      typedef typename traits::difference_type difference_type;
      iterator_tuple2( )
      {}
      iterator_tuple2( arg_type0 theM0,
                       arg_type1 theM1 ) : parent_type( theM0, theM1 )
      {}
      template < class S0,
      class S1 >
      iterator_tuple2( const iterator_tuple2 < S0,
                       S1 > &rhs ) : parent_type( rhs )
      {}
      iterator_tuple2( arg_type0 theM0,
                       const tail_type & tail ) : parent_type( theM0, tail )
      {}
      self_type & operator++( )
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
      self_type & operator--( )
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
      reference operator*( )
      {
        return reference( detail::ref( *m0 ), detail::ref( *m1 ) );
      }
      const_reference operator*( ) const
      {
        return const_reference( detail::cref( *m0 ), detail::cref( *m1 ) );
      }
      reference operator[] ( const difference_type & i )
      {
        return reference( detail::ref( m0[ i.m0 ] ), detail::ref( m1[ i.m1 ] ) );
      }
      const_reference operator[] ( const difference_type & i ) const
      {
        return const_reference( detail::cref( m0[ i.m0 ] ),
                                detail::cref( m1[ i.m1 ] ) );
      }
    };
    template < class T0, class T1 >
    iterator_tuple2 < T0,
    T1 > operator+( const iterator_tuple2 < T0, T1 > &lhs,
                    const iterator_tuple2 < T0,
                    T1 >::difference_type & rhs )
    {
      return iterator_tuple2 < T0, T1 > ( lhs ) += rhs;
    }
    template < class T0, class T1 >
    iterator_tuple2 < T0,
    T1 > operator-( const iterator_tuple2 < T0, T1 > &lhs,
                    const iterator_tuple2 < T0,
                    T1 >::difference_type & rhs )
    {
      return iterator_tuple2 < T0, T1 > ( lhs ) -= rhs;
    }
    template < class T0, class T1 >
    iterator_tuple2 < T0,
    T1 > ::difference_type operator-( const iterator_tuple2 < T0, T1 > &lhs,
                                      const iterator_tuple2 < T0, T1 > &rhs )
    {
      return iterator_tuple2 < T0, T1 >::difference_type( lhs.m0 - rhs.m0,
             lhs.m1 - rhs.m1 );
    }
    template < class R0, class R1, class R2 >
    struct iterator_traits_tuple3
    {
      typedef tuple3 < typename R0::iterator_category,
      typename R1::iterator_category,
      typename R2::iterator_category > iterator_category;
      typedef tuple3 < typename R0::value_type, typename R1::value_type,
      typename R2::value_type > value_type;
      typedef tuple3 < detail::reference_wrapper < typename R0::value_type >,
      detail::reference_wrapper < typename R1::value_type >,
      detail::reference_wrapper < typename R2::value_type > > reference;
      typedef tuple3 < detail::reference_wrapper <
      typename R0::value_type const > ,
      detail::reference_wrapper < typename R1::value_type const >,
      detail::reference_wrapper <
      typename R2::value_type const > > const_reference;
      typedef tuple3 < typename R0::pointer, typename R1::pointer,
      typename R2::pointer > pointer;
      typedef tuple3 < typename R0::difference_type,
      typename R1::difference_type,
      typename R2::difference_type > difference_type;
    };
    template < class T0, class T1,
    class T2 >
    struct iterator_tuple3: public tuple3 < T0, T1, T2 >
    {
      typedef iterator_tuple3 < T0, T1, T2 > self_type;
      typedef tuple3 < T0, T1, T2 > parent_type;
      typedef iterator_traits_tuple3 < detail::iterator_traits < T0 >,
      detail::iterator_traits < T1 >,
      detail::iterator_traits < T2 > > traits;
      typedef typename traits::iterator_category iterator_category;
      typedef typename traits::value_type value_type;
      typedef typename traits::reference reference;
      typedef typename traits::const_reference const_reference;
      typedef typename traits::pointer pointer;
      typedef typename traits::difference_type difference_type;
      iterator_tuple3( )
      {}
      iterator_tuple3( arg_type0 theM0, arg_type1 theM1,
                       arg_type2 theM2 ) : parent_type( theM0, theM1, theM2 )
      {}
      template < class S0, class S1,
      class S2 >
      iterator_tuple3( const iterator_tuple3 < S0, S1,
                       S2 > &rhs ) : parent_type( rhs )
      {}
      iterator_tuple3( arg_type0 theM0,
                       const tail_type & tail ) : parent_type( theM0, tail )
      {}
      self_type & operator++( )
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
      self_type & operator--( )
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
      reference operator*( )
      {
        return reference( detail::ref( *m0 ), detail::ref( *m1 ),
                          detail::ref( *m2 ) );
      }
      const_reference operator*( ) const
      {
        return const_reference( detail::cref( *m0 ), detail::cref( *m1 ),
                                detail::cref( *m2 ) );
      }
      reference operator[] ( const difference_type & i )
      {
        return reference( detail::ref( m0[ i.m0 ] ), detail::ref( m1[ i.m1 ] ),
                          detail::ref( m2[ i.m2 ] ) );
      }
      const_reference operator[] ( const difference_type & i ) const
      {
        return const_reference( detail::cref( m0[ i.m0 ] ),
                                detail::cref( m1[ i.m1 ] ),
                                detail::cref( m2[ i.m2 ] ) );
      }
    };
    template < class T0, class T1, class T2 >
    iterator_tuple3 < T0, T1,
    T2 > operator+( const iterator_tuple3 < T0, T1, T2 > &lhs,
                    const iterator_tuple3 < T0, T1,
                    T2 >::difference_type & rhs )
    {
      return iterator_tuple3 < T0, T1, T2 > ( lhs ) += rhs;
    }
    template < class T0, class T1, class T2 >
    iterator_tuple3 < T0, T1,
    T2 > operator-( const iterator_tuple3 < T0, T1, T2 > &lhs,
                    const iterator_tuple3 < T0, T1,
                    T2 >::difference_type & rhs )
    {
      return iterator_tuple3 < T0, T1, T2 > ( lhs ) -= rhs;
    }
    template < class T0, class T1, class T2 >
    iterator_tuple3 < T0, T1,
    T2 > ::difference_type operator-( const iterator_tuple3 < T0, T1,
                                      T2 > &lhs, const iterator_tuple3 < T0,
                                      T1, T2 > &rhs )
    {
      return iterator_tuple3 < T0, T1, T2 >::difference_type( lhs.m0 - rhs.m0,
             lhs.m1 - rhs.m1,
             lhs.m2 -
             rhs.m2 );
    }
    template < class R0, class R1, class R2, class R3 >
    struct iterator_traits_tuple4
    {
      typedef tuple4 < typename R0::iterator_category,
      typename R1::iterator_category, typename R2::iterator_category,
      typename R3::iterator_category > iterator_category;
      typedef tuple4 < typename R0::value_type, typename R1::value_type,
      typename R2::value_type, typename R3::value_type > value_type;
      typedef tuple4 < detail::reference_wrapper < typename R0::value_type >,
      detail::reference_wrapper < typename R1::value_type >,
      detail::reference_wrapper < typename R2::value_type >,
      detail::reference_wrapper < typename R3::value_type > > reference;
      typedef tuple4 < detail::reference_wrapper <
      typename R0::value_type const > ,
      detail::reference_wrapper < typename R1::value_type const >,
      detail::reference_wrapper < typename R2::value_type const >,
      detail::reference_wrapper <
      typename R3::value_type const > > const_reference;
      typedef tuple4 < typename R0::pointer, typename R1::pointer,
      typename R2::pointer, typename R3::pointer > pointer;
      typedef tuple4 < typename R0::difference_type,
      typename R1::difference_type, typename R2::difference_type,
      typename R3::difference_type > difference_type;
    };
    template < class T0, class T1, class T2,
    class T3 >
    struct iterator_tuple4: public tuple4 < T0, T1, T2, T3 >
    {
      typedef iterator_tuple4 < T0, T1, T2, T3 > self_type;
      typedef tuple4 < T0, T1, T2, T3 > parent_type;
      typedef iterator_traits_tuple4 < detail::iterator_traits < T0 >,
      detail::iterator_traits < T1 >, detail::iterator_traits < T2 >,
      detail::iterator_traits < T3 > > traits;
      typedef typename traits::iterator_category iterator_category;
      typedef typename traits::value_type value_type;
      typedef typename traits::reference reference;
      typedef typename traits::const_reference const_reference;
      typedef typename traits::pointer pointer;
      typedef typename traits::difference_type difference_type;
      iterator_tuple4( )
      {}
      iterator_tuple4( arg_type0 theM0, arg_type1 theM1, arg_type2 theM2,
                       arg_type3 theM3 ) : parent_type( theM0, theM1, theM2,
                                                            theM3 )
      {}
      template < class S0, class S1, class S2,
      class S3 >
      iterator_tuple4( const iterator_tuple4 < S0, S1, S2,
                       S3 > &rhs ) : parent_type( rhs )
      {}
      iterator_tuple4( arg_type0 theM0,
                       const tail_type & tail ) : parent_type( theM0, tail )
      {}
      self_type & operator++( )
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
      self_type & operator--( )
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
      reference operator*( )
      {
        return reference( detail::ref( *m0 ), detail::ref( *m1 ),
                          detail::ref( *m2 ), detail::ref( *m3 ) );
      }
      const_reference operator*( ) const
      {
        return const_reference( detail::cref( *m0 ), detail::cref( *m1 ),
                                detail::cref( *m2 ), detail::cref( *m3 ) );
      }
      reference operator[] ( const difference_type & i )
      {
        return reference( detail::ref( m0[ i.m0 ] ), detail::ref( m1[ i.m1 ] ),
                          detail::ref( m2[ i.m2 ] ), detail::ref( m3[ i.m3 ] ) );
      }
      const_reference operator[] ( const difference_type & i ) const
      {
        return const_reference( detail::cref( m0[ i.m0 ] ),
                                detail::cref( m1[ i.m1 ] ),
                                detail::cref( m2[ i.m2 ] ),
                                detail::cref( m3[ i.m3 ] ) );
      }
    };
    template < class T0, class T1, class T2, class T3 >
    iterator_tuple4 < T0,
    T1, T2, T3 > operator+( const iterator_tuple4 < T0, T1, T2, T3 > &lhs,
                            const iterator_tuple4 < T0, T1, T2,
                            T3 >::difference_type & rhs )
    {
      return iterator_tuple4 < T0, T1, T2, T3 > ( lhs ) += rhs;
    }
    template < class T0, class T1, class T2, class T3 >
    iterator_tuple4 < T0,
    T1, T2, T3 > operator-( const iterator_tuple4 < T0, T1, T2, T3 > &lhs,
                            const iterator_tuple4 < T0, T1, T2,
                            T3 >::difference_type & rhs )
    {
      return iterator_tuple4 < T0, T1, T2, T3 > ( lhs ) -= rhs;
    }
    template < class T0, class T1, class T2, class T3 >
    iterator_tuple4 < T0,
    T1, T2, T3 > ::difference_type operator-( const iterator_tuple4 < T0, T1,
        T2, T3 > &lhs,
        const iterator_tuple4 < T0, T1,
        T2, T3 > &rhs )
    {
      return iterator_tuple4 < T0, T1, T2,
             T3 > ::difference_type( lhs.m0 - rhs.m0, lhs.m1 - rhs.m1,
                                     lhs.m2 - rhs.m2, lhs.m3 - rhs.m3 );
    }
    template < class R0, class R1, class R2, class R3, class R4 >
    struct iterator_traits_tuple5
    {
      typedef tuple5 < typename R0::iterator_category,
      typename R1::iterator_category, typename R2::iterator_category,
      typename R3::iterator_category,
      typename R4::iterator_category > iterator_category;
      typedef tuple5 < typename R0::value_type, typename R1::value_type,
      typename R2::value_type, typename R3::value_type,
      typename R4::value_type > value_type;
      typedef tuple5 < detail::reference_wrapper < typename R0::value_type >,
      detail::reference_wrapper < typename R1::value_type >,
      detail::reference_wrapper < typename R2::value_type >,
      detail::reference_wrapper < typename R3::value_type >,
      detail::reference_wrapper < typename R4::value_type > > reference;
      typedef tuple5 < detail::reference_wrapper <
      typename R0::value_type const > ,
      detail::reference_wrapper < typename R1::value_type const >,
      detail::reference_wrapper < typename R2::value_type const >,
      detail::reference_wrapper < typename R3::value_type const >,
      detail::reference_wrapper <
      typename R4::value_type const > > const_reference;
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
    struct iterator_tuple5: public tuple5 < T0, T1, T2, T3, T4 >
    {
      typedef iterator_tuple5 < T0, T1, T2, T3, T4 > self_type;
      typedef tuple5 < T0, T1, T2, T3, T4 > parent_type;
      typedef iterator_traits_tuple5 < detail::iterator_traits < T0 >,
      detail::iterator_traits < T1 >, detail::iterator_traits < T2 >,
      detail::iterator_traits < T3 >,
      detail::iterator_traits < T4 > > traits;
      typedef typename traits::iterator_category iterator_category;
      typedef typename traits::value_type value_type;
      typedef typename traits::reference reference;
      typedef typename traits::const_reference const_reference;
      typedef typename traits::pointer pointer;
      typedef typename traits::difference_type difference_type;
      iterator_tuple5( )
      {}
      iterator_tuple5( arg_type0 theM0, arg_type1 theM1, arg_type2 theM2,
                       arg_type3 theM3,
                       arg_type4 theM4 ) : parent_type( theM0, theM1, theM2,
                                                            theM3, theM4 )
      {}
      template < class S0, class S1, class S2, class S3,
      class S4 >
      iterator_tuple5( const iterator_tuple5 < S0, S1, S2, S3,
                       S4 > &rhs ) : parent_type( rhs )
      {}
      iterator_tuple5( arg_type0 theM0,
                       const tail_type & tail ) : parent_type( theM0, tail )
      {}
      self_type & operator++( )
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
      self_type & operator--( )
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
      reference operator*( )
      {
        return reference( detail::ref( *m0 ), detail::ref( *m1 ),
                          detail::ref( *m2 ), detail::ref( *m3 ),
                          detail::ref( *m4 ) );
      }
      const_reference operator*( ) const
      {
        return const_reference( detail::cref( *m0 ), detail::cref( *m1 ),
                                detail::cref( *m2 ), detail::cref( *m3 ),
                                detail::cref( *m4 ) );
      }
      reference operator[] ( const difference_type & i )
      {
        return reference( detail::ref( m0[ i.m0 ] ), detail::ref( m1[ i.m1 ] ),
                          detail::ref( m2[ i.m2 ] ), detail::ref( m3[ i.m3 ] ),
                          detail::ref( m4[ i.m4 ] ) );
      }
      const_reference operator[] ( const difference_type & i ) const
      {
        return const_reference( detail::cref( m0[ i.m0 ] ),
                                detail::cref( m1[ i.m1 ] ),
                                detail::cref( m2[ i.m2 ] ),
                                detail::cref( m3[ i.m3 ] ),
                                detail::cref( m4[ i.m4 ] ) );
      }
    };
    template < class T0, class T1, class T2, class T3,
    class T4 >
    iterator_tuple5 < T0, T1, T2, T3,
    T4 > operator+( const iterator_tuple5 < T0, T1, T2, T3, T4 > &lhs,
                    const iterator_tuple5 < T0, T1, T2, T3,
                    T4 >::difference_type & rhs )
    {
      return iterator_tuple5 < T0, T1, T2, T3, T4 > ( lhs ) += rhs;
    }
    template < class T0, class T1, class T2, class T3,
    class T4 >
    iterator_tuple5 < T0, T1, T2, T3,
    T4 > operator-( const iterator_tuple5 < T0, T1, T2, T3, T4 > &lhs,
                    const iterator_tuple5 < T0, T1, T2, T3,
                    T4 >::difference_type & rhs )
    {
      return iterator_tuple5 < T0, T1, T2, T3, T4 > ( lhs ) -= rhs;
    }
    template < class T0, class T1, class T2, class T3,
    class T4 >
    iterator_tuple5 < T0, T1, T2, T3,
    T4 > ::difference_type operator-( const iterator_tuple5 < T0, T1, T2, T3,
                                      T4 > &lhs, const iterator_tuple5 < T0,
                                      T1, T2, T3, T4 > &rhs )
    {
      return iterator_tuple5 < T0, T1, T2, T3,
             T4 > ::difference_type( lhs.m0 - rhs.m0, lhs.m1 - rhs.m1,
                                     lhs.m2 - rhs.m2, lhs.m3 - rhs.m3,
                                     lhs.m4 - rhs.m4 );
    }
    template < class R0, class R1, class R2, class R3, class R4, class R5 >
    struct iterator_traits_tuple6
    {
      typedef tuple6 < typename R0::iterator_category,
      typename R1::iterator_category, typename R2::iterator_category,
      typename R3::iterator_category, typename R4::iterator_category,
      typename R5::iterator_category > iterator_category;
      typedef tuple6 < typename R0::value_type, typename R1::value_type,
      typename R2::value_type, typename R3::value_type,
      typename R4::value_type, typename R5::value_type > value_type;
      typedef tuple6 < detail::reference_wrapper < typename R0::value_type >,
      detail::reference_wrapper < typename R1::value_type >,
      detail::reference_wrapper < typename R2::value_type >,
      detail::reference_wrapper < typename R3::value_type >,
      detail::reference_wrapper < typename R4::value_type >,
      detail::reference_wrapper < typename R5::value_type > > reference;
      typedef tuple6 < detail::reference_wrapper <
      typename R0::value_type const > ,
      detail::reference_wrapper < typename R1::value_type const >,
      detail::reference_wrapper < typename R2::value_type const >,
      detail::reference_wrapper < typename R3::value_type const >,
      detail::reference_wrapper < typename R4::value_type const >,
      detail::reference_wrapper <
      typename R5::value_type const > > const_reference;
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
    struct iterator_tuple6: public tuple6 < T0, T1, T2, T3, T4,
          T5 >
    {
      typedef iterator_tuple6 < T0, T1, T2, T3, T4, T5 > self_type;
      typedef tuple6 < T0, T1, T2, T3, T4, T5 > parent_type;
      typedef iterator_traits_tuple6 < detail::iterator_traits < T0 >,
      detail::iterator_traits < T1 >, detail::iterator_traits < T2 >,
      detail::iterator_traits < T3 >, detail::iterator_traits < T4 >,
      detail::iterator_traits < T5 > > traits;
      typedef typename traits::iterator_category iterator_category;
      typedef typename traits::value_type value_type;
      typedef typename traits::reference reference;
      typedef typename traits::const_reference const_reference;
      typedef typename traits::pointer pointer;
      typedef typename traits::difference_type difference_type;
      iterator_tuple6( )
      {}
      iterator_tuple6( arg_type0 theM0, arg_type1 theM1, arg_type2 theM2,
                       arg_type3 theM3, arg_type4 theM4,
                       arg_type5 theM5 ) : parent_type( theM0, theM1, theM2,
                                                            theM3, theM4, theM5 )
      {}
      template < class S0, class S1, class S2, class S3, class S4,
      class S5 >
      iterator_tuple6( const iterator_tuple6 < S0, S1, S2, S3,
                       S4, S5 > &rhs ) : parent_type( rhs )
      {}
      iterator_tuple6( arg_type0 theM0,
                       const tail_type & tail ) : parent_type( theM0, tail )
      {}
      self_type & operator++( )
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
      self_type & operator--( )
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
      reference operator*( )
      {
        return reference( detail::ref( *m0 ), detail::ref( *m1 ),
                          detail::ref( *m2 ), detail::ref( *m3 ),
                          detail::ref( *m4 ), detail::ref( *m5 ) );
      }
      const_reference operator*( ) const
      {
        return const_reference( detail::cref( *m0 ), detail::cref( *m1 ),
                                detail::cref( *m2 ), detail::cref( *m3 ),
                                detail::cref( *m4 ), detail::cref( *m5 ) );
      }
      reference operator[] ( const difference_type & i )
      {
        return reference( detail::ref( m0[ i.m0 ] ), detail::ref( m1[ i.m1 ] ),
                          detail::ref( m2[ i.m2 ] ), detail::ref( m3[ i.m3 ] ),
                          detail::ref( m4[ i.m4 ] ), detail::ref( m5[ i.m5 ] ) );
      }
      const_reference operator[] ( const difference_type & i ) const
      {
        return const_reference( detail::cref( m0[ i.m0 ] ),
                                detail::cref( m1[ i.m1 ] ),
                                detail::cref( m2[ i.m2 ] ),
                                detail::cref( m3[ i.m3 ] ),
                                detail::cref( m4[ i.m4 ] ),
                                detail::cref( m5[ i.m5 ] ) );
      }
    };
    template < class T0, class T1, class T2, class T3, class T4,
    class T5 >
    iterator_tuple6 < T0, T1, T2, T3, T4,
    T5 > operator+( const iterator_tuple6 < T0, T1, T2, T3, T4, T5 > &lhs,
                    const iterator_tuple6 < T0, T1, T2, T3, T4,
                    T5 >::difference_type & rhs )
    {
      return iterator_tuple6 < T0, T1, T2, T3, T4, T5 > ( lhs ) += rhs;
    }
    template < class T0, class T1, class T2, class T3, class T4,
    class T5 >
    iterator_tuple6 < T0, T1, T2, T3, T4,
    T5 > operator-( const iterator_tuple6 < T0, T1, T2, T3, T4, T5 > &lhs,
                    const iterator_tuple6 < T0, T1, T2, T3, T4,
                    T5 >::difference_type & rhs )
    {
      return iterator_tuple6 < T0, T1, T2, T3, T4, T5 > ( lhs ) -= rhs;
    }
    template < class T0, class T1, class T2, class T3, class T4,
    class T5 >
    iterator_tuple6 < T0, T1, T2, T3, T4,
    T5 > ::difference_type operator-( const iterator_tuple6 < T0, T1, T2, T3,
                                      T4, T5 > &lhs,
                                      const iterator_tuple6 < T0, T1, T2, T3,
                                      T4, T5 > &rhs )
    {
      return iterator_tuple6 < T0, T1, T2, T3, T4,
             T5 > ::difference_type( lhs.m0 - rhs.m0, lhs.m1 - rhs.m1,
                                     lhs.m2 - rhs.m2, lhs.m3 - rhs.m3,
                                     lhs.m4 - rhs.m4, lhs.m5 - rhs.m5 );
    }
    template < class R0, class R1, class R2, class R3, class R4, class R5,
    class R6 >
    struct iterator_traits_tuple7
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
      typedef tuple7 < detail::reference_wrapper < typename R0::value_type >,
      detail::reference_wrapper < typename R1::value_type >,
      detail::reference_wrapper < typename R2::value_type >,
      detail::reference_wrapper < typename R3::value_type >,
      detail::reference_wrapper < typename R4::value_type >,
      detail::reference_wrapper < typename R5::value_type >,
      detail::reference_wrapper < typename R6::value_type > > reference;
      typedef tuple7 < detail::reference_wrapper <
      typename R0::value_type const > ,
      detail::reference_wrapper < typename R1::value_type const >,
      detail::reference_wrapper < typename R2::value_type const >,
      detail::reference_wrapper < typename R3::value_type const >,
      detail::reference_wrapper < typename R4::value_type const >,
      detail::reference_wrapper < typename R5::value_type const >,
      detail::reference_wrapper <
      typename R6::value_type const > > const_reference;
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
    struct iterator_tuple7: public tuple7 < T0, T1, T2, T3, T4,
          T5, T6 >
    {
      typedef iterator_tuple7 < T0, T1, T2, T3, T4, T5, T6 > self_type;
      typedef tuple7 < T0, T1, T2, T3, T4, T5, T6 > parent_type;
      typedef iterator_traits_tuple7 < detail::iterator_traits < T0 >,
      detail::iterator_traits < T1 >, detail::iterator_traits < T2 >,
      detail::iterator_traits < T3 >, detail::iterator_traits < T4 >,
      detail::iterator_traits < T5 >,
      detail::iterator_traits < T6 > > traits;
      typedef typename traits::iterator_category iterator_category;
      typedef typename traits::value_type value_type;
      typedef typename traits::reference reference;
      typedef typename traits::const_reference const_reference;
      typedef typename traits::pointer pointer;
      typedef typename traits::difference_type difference_type;
      iterator_tuple7( )
      {}
      iterator_tuple7( arg_type0 theM0, arg_type1 theM1, arg_type2 theM2,
                       arg_type3 theM3, arg_type4 theM4, arg_type5 theM5,
                       arg_type6 theM6 ) : parent_type( theM0, theM1, theM2,
                                                            theM3, theM4, theM5,
                                                            theM6 )
      {}
      template < class S0, class S1, class S2, class S3, class S4, class S5,
      class S6 >
      iterator_tuple7( const iterator_tuple7 < S0, S1, S2, S3,
                       S4, S5, S6 > &rhs ) : parent_type( rhs )
      {}
      iterator_tuple7( arg_type0 theM0,
                       const tail_type & tail ) : parent_type( theM0, tail )
      {}
      self_type & operator++( )
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
      self_type & operator--( )
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
      reference operator*( )
      {
        return reference( detail::ref( *m0 ), detail::ref( *m1 ),
                          detail::ref( *m2 ), detail::ref( *m3 ),
                          detail::ref( *m4 ), detail::ref( *m5 ),
                          detail::ref( *m6 ) );
      }
      const_reference operator*( ) const
      {
        return const_reference( detail::cref( *m0 ), detail::cref( *m1 ),
                                detail::cref( *m2 ), detail::cref( *m3 ),
                                detail::cref( *m4 ), detail::cref( *m5 ),
                                detail::cref( *m6 ) );
      }
      reference operator[] ( const difference_type & i )
      {
        return reference( detail::ref( m0[ i.m0 ] ), detail::ref( m1[ i.m1 ] ),
                          detail::ref( m2[ i.m2 ] ), detail::ref( m3[ i.m3 ] ),
                          detail::ref( m4[ i.m4 ] ), detail::ref( m5[ i.m5 ] ),
                          detail::ref( m6[ i.m6 ] ) );
      }
      const_reference operator[] ( const difference_type & i ) const
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
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6 >
    iterator_tuple7 < T0, T1, T2, T3, T4, T5,
    T6 > operator+( const iterator_tuple7 < T0, T1, T2, T3, T4, T5,
                    T6 > &lhs, const iterator_tuple7 < T0, T1, T2, T3, T4,
                    T5, T6 > ::difference_type & rhs )
    {
      return iterator_tuple7 < T0, T1, T2, T3, T4, T5, T6 > ( lhs ) += rhs;
    }
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6 >
    iterator_tuple7 < T0, T1, T2, T3, T4, T5,
    T6 > operator-( const iterator_tuple7 < T0, T1, T2, T3, T4, T5,
                    T6 > &lhs, const iterator_tuple7 < T0, T1, T2, T3, T4,
                    T5, T6 > ::difference_type & rhs )
    {
      return iterator_tuple7 < T0, T1, T2, T3, T4, T5, T6 > ( lhs ) -= rhs;
    }
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6 >
    iterator_tuple7 < T0, T1, T2, T3, T4, T5,
    T6 > ::difference_type operator-( const iterator_tuple7 < T0, T1, T2, T3,
                                      T4, T5, T6 > &lhs,
                                      const iterator_tuple7 < T0, T1, T2, T3,
                                      T4, T5, T6 > &rhs )
    {
      return iterator_tuple7 < T0, T1, T2, T3, T4, T5,
             T6 > ::difference_type( lhs.m0 - rhs.m0, lhs.m1 - rhs.m1,
                                     lhs.m2 - rhs.m2, lhs.m3 - rhs.m3,
                                     lhs.m4 - rhs.m4, lhs.m5 - rhs.m5,
                                     lhs.m6 - rhs.m6 );
    }
    template < class R0, class R1, class R2, class R3, class R4, class R5,
    class R6, class R7 >
    struct iterator_traits_tuple8
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
      typedef tuple8 < detail::reference_wrapper < typename R0::value_type >,
      detail::reference_wrapper < typename R1::value_type >,
      detail::reference_wrapper < typename R2::value_type >,
      detail::reference_wrapper < typename R3::value_type >,
      detail::reference_wrapper < typename R4::value_type >,
      detail::reference_wrapper < typename R5::value_type >,
      detail::reference_wrapper < typename R6::value_type >,
      detail::reference_wrapper < typename R7::value_type > > reference;
      typedef tuple8 < detail::reference_wrapper <
      typename R0::value_type const > ,
      detail::reference_wrapper < typename R1::value_type const >,
      detail::reference_wrapper < typename R2::value_type const >,
      detail::reference_wrapper < typename R3::value_type const >,
      detail::reference_wrapper < typename R4::value_type const >,
      detail::reference_wrapper < typename R5::value_type const >,
      detail::reference_wrapper < typename R6::value_type const >,
      detail::reference_wrapper <
      typename R7::value_type const > > const_reference;
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
    struct iterator_tuple8: public tuple8 < T0, T1, T2,
          T3, T4, T5, T6, T7 >
    {
      typedef iterator_tuple8 < T0, T1, T2, T3, T4, T5, T6, T7 > self_type;
      typedef tuple8 < T0, T1, T2, T3, T4, T5, T6, T7 > parent_type;
      typedef iterator_traits_tuple8 < detail::iterator_traits < T0 >,
      detail::iterator_traits < T1 >, detail::iterator_traits < T2 >,
      detail::iterator_traits < T3 >, detail::iterator_traits < T4 >,
      detail::iterator_traits < T5 >, detail::iterator_traits < T6 >,
      detail::iterator_traits < T7 > > traits;
      typedef typename traits::iterator_category iterator_category;
      typedef typename traits::value_type value_type;
      typedef typename traits::reference reference;
      typedef typename traits::const_reference const_reference;
      typedef typename traits::pointer pointer;
      typedef typename traits::difference_type difference_type;
      iterator_tuple8( )
      {}
      iterator_tuple8( arg_type0 theM0, arg_type1 theM1, arg_type2 theM2,
                       arg_type3 theM3, arg_type4 theM4, arg_type5 theM5,
                       arg_type6 theM6,
                       arg_type7 theM7 ) : parent_type( theM0, theM1, theM2,
                                                            theM3, theM4, theM5,
                                                            theM6, theM7 )
      {}
      template < class S0, class S1, class S2, class S3, class S4, class S5,
      class S6, class S7 >
      iterator_tuple8( const iterator_tuple8 < S0, S1,
                       S2, S3, S4, S5, S6,
                       S7 > &rhs ) : parent_type( rhs )
      {}
      iterator_tuple8( arg_type0 theM0,
                       const tail_type & tail ) : parent_type( theM0, tail )
      {}
      self_type & operator++( )
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
      self_type & operator--( )
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
      reference operator*( )
      {
        return reference( detail::ref( *m0 ), detail::ref( *m1 ),
                          detail::ref( *m2 ), detail::ref( *m3 ),
                          detail::ref( *m4 ), detail::ref( *m5 ),
                          detail::ref( *m6 ), detail::ref( *m7 ) );
      }
      const_reference operator*( ) const
      {
        return const_reference( detail::cref( *m0 ), detail::cref( *m1 ),
                                detail::cref( *m2 ), detail::cref( *m3 ),
                                detail::cref( *m4 ), detail::cref( *m5 ),
                                detail::cref( *m6 ), detail::cref( *m7 ) );
      }
      reference operator[] ( const difference_type & i )
      {
        return reference( detail::ref( m0[ i.m0 ] ), detail::ref( m1[ i.m1 ] ),
                          detail::ref( m2[ i.m2 ] ), detail::ref( m3[ i.m3 ] ),
                          detail::ref( m4[ i.m4 ] ), detail::ref( m5[ i.m5 ] ),
                          detail::ref( m6[ i.m6 ] ), detail::ref( m7[ i.m7 ] ) );
      }
      const_reference operator[] ( const difference_type & i ) const
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
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6, class T7 >
    iterator_tuple8 < T0, T1, T2, T3, T4, T5, T6,
    T7 > operator+( const iterator_tuple8 < T0, T1, T2, T3, T4, T5, T6,
                    T7 > &lhs, const iterator_tuple8 < T0, T1, T2, T3, T4,
                    T5, T6, T7 > ::difference_type & rhs )
    {
      return iterator_tuple8 < T0, T1, T2, T3, T4, T5, T6, T7 > ( lhs ) +=
               rhs;
    }
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6, class T7 >
    iterator_tuple8 < T0, T1, T2, T3, T4, T5, T6,
    T7 > operator-( const iterator_tuple8 < T0, T1, T2, T3, T4, T5, T6,
                    T7 > &lhs, const iterator_tuple8 < T0, T1, T2, T3, T4,
                    T5, T6, T7 > ::difference_type & rhs )
    {
      return iterator_tuple8 < T0, T1, T2, T3, T4, T5, T6, T7 > ( lhs ) -=
               rhs;
    }
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6, class T7 >
    iterator_tuple8 < T0, T1, T2, T3, T4, T5, T6,
    T7 > ::difference_type operator-( const iterator_tuple8 < T0, T1, T2, T3,
                                      T4, T5, T6, T7 > &lhs,
                                      const iterator_tuple8 < T0, T1, T2, T3,
                                      T4, T5, T6, T7 > &rhs )
    {
      return iterator_tuple8 < T0, T1, T2, T3, T4, T5, T6,
             T7 > ::difference_type( lhs.m0 - rhs.m0, lhs.m1 - rhs.m1,
                                     lhs.m2 - rhs.m2, lhs.m3 - rhs.m3,
                                     lhs.m4 - rhs.m4, lhs.m5 - rhs.m5,
                                     lhs.m6 - rhs.m6, lhs.m7 - rhs.m7 );
    }
    template < class R0, class R1, class R2, class R3, class R4, class R5,
    class R6, class R7, class R8 >
    struct iterator_traits_tuple9
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
      typedef tuple9 < detail::reference_wrapper < typename R0::value_type >,
      detail::reference_wrapper < typename R1::value_type >,
      detail::reference_wrapper < typename R2::value_type >,
      detail::reference_wrapper < typename R3::value_type >,
      detail::reference_wrapper < typename R4::value_type >,
      detail::reference_wrapper < typename R5::value_type >,
      detail::reference_wrapper < typename R6::value_type >,
      detail::reference_wrapper < typename R7::value_type >,
      detail::reference_wrapper < typename R8::value_type > > reference;
      typedef tuple9 < detail::reference_wrapper <
      typename R0::value_type const > ,
      detail::reference_wrapper < typename R1::value_type const >,
      detail::reference_wrapper < typename R2::value_type const >,
      detail::reference_wrapper < typename R3::value_type const >,
      detail::reference_wrapper < typename R4::value_type const >,
      detail::reference_wrapper < typename R5::value_type const >,
      detail::reference_wrapper < typename R6::value_type const >,
      detail::reference_wrapper < typename R7::value_type const >,
      detail::reference_wrapper <
      typename R8::value_type const > > const_reference;
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
    struct iterator_tuple9: public tuple9 < T0, T1, T2, T3, T4,
          T5, T6, T7, T8 >
    {
      typedef iterator_tuple9 < T0, T1, T2, T3, T4, T5, T6, T7,
      T8 > self_type;
      typedef tuple9 < T0, T1, T2, T3, T4, T5, T6, T7, T8 > parent_type;
      typedef iterator_traits_tuple9 < detail::iterator_traits < T0 >,
      detail::iterator_traits < T1 >, detail::iterator_traits < T2 >,
      detail::iterator_traits < T3 >, detail::iterator_traits < T4 >,
      detail::iterator_traits < T5 >, detail::iterator_traits < T6 >,
      detail::iterator_traits < T7 >,
      detail::iterator_traits < T8 > > traits;
      typedef typename traits::iterator_category iterator_category;
      typedef typename traits::value_type value_type;
      typedef typename traits::reference reference;
      typedef typename traits::const_reference const_reference;
      typedef typename traits::pointer pointer;
      typedef typename traits::difference_type difference_type;
      iterator_tuple9( )
      {}
      iterator_tuple9( arg_type0 theM0, arg_type1 theM1, arg_type2 theM2,
                       arg_type3 theM3, arg_type4 theM4, arg_type5 theM5,
                       arg_type6 theM6, arg_type7 theM7,
                       arg_type8 theM8 ) : parent_type( theM0, theM1, theM2,
                                                            theM3, theM4, theM5,
                                                            theM6, theM7, theM8 )
      {}
      template < class S0, class S1, class S2, class S3, class S4, class S5,
      class S6, class S7,
      class S8 >
      iterator_tuple9( const iterator_tuple9 < S0, S1, S2, S3,
                       S4, S5, S6, S7,
                       S8 > &rhs ) : parent_type( rhs )
      {}
      iterator_tuple9( arg_type0 theM0,
                       const tail_type & tail ) : parent_type( theM0, tail )
      {}
      self_type & operator++( )
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
      self_type & operator--( )
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
      reference operator*( )
      {
        return reference( detail::ref( *m0 ), detail::ref( *m1 ),
                          detail::ref( *m2 ), detail::ref( *m3 ),
                          detail::ref( *m4 ), detail::ref( *m5 ),
                          detail::ref( *m6 ), detail::ref( *m7 ),
                          detail::ref( *m8 ) );
      }
      const_reference operator*( ) const
      {
        return const_reference( detail::cref( *m0 ), detail::cref( *m1 ),
                                detail::cref( *m2 ), detail::cref( *m3 ),
                                detail::cref( *m4 ), detail::cref( *m5 ),
                                detail::cref( *m6 ), detail::cref( *m7 ),
                                detail::cref( *m8 ) );
      }
      reference operator[] ( const difference_type & i )
      {
        return reference( detail::ref( m0[ i.m0 ] ), detail::ref( m1[ i.m1 ] ),
                          detail::ref( m2[ i.m2 ] ), detail::ref( m3[ i.m3 ] ),
                          detail::ref( m4[ i.m4 ] ), detail::ref( m5[ i.m5 ] ),
                          detail::ref( m6[ i.m6 ] ), detail::ref( m7[ i.m7 ] ),
                          detail::ref( m8[ i.m8 ] ) );
      }
      const_reference operator[] ( const difference_type & i ) const
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
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6, class T7, class T8 >
    iterator_tuple9 < T0, T1, T2, T3, T4, T5,
    T6, T7, T8 > operator+( const iterator_tuple9 < T0, T1, T2, T3, T4, T5,
                            T6, T7, T8 > &lhs, const iterator_tuple9 < T0,
                            T1, T2, T3, T4, T5, T6, T7,
                            T8 > ::difference_type & rhs )
    {
      return iterator_tuple9 < T0, T1, T2, T3, T4, T5, T6, T7, T8 > ( lhs ) +=
               rhs;
    }
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6, class T7, class T8 >
    iterator_tuple9 < T0, T1, T2, T3, T4, T5,
    T6, T7, T8 > operator-( const iterator_tuple9 < T0, T1, T2, T3, T4, T5,
                            T6, T7, T8 > &lhs, const iterator_tuple9 < T0,
                            T1, T2, T3, T4, T5, T6, T7,
                            T8 > ::difference_type & rhs )
    {
      return iterator_tuple9 < T0, T1, T2, T3, T4, T5, T6, T7, T8 > ( lhs ) -=
               rhs;
    }
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6, class T7, class T8 >
    iterator_tuple9 < T0, T1, T2, T3, T4, T5,
    T6, T7, T8 > ::difference_type operator-( const iterator_tuple9 < T0, T1,
        T2, T3, T4, T5, T6, T7,
        T8 > &lhs,
        const iterator_tuple9 < T0, T1,
        T2, T3, T4, T5, T6, T7,
        T8 > &rhs )
    {
      return iterator_tuple9 < T0, T1, T2, T3, T4, T5, T6, T7,
             T8 > ::difference_type( lhs.m0 - rhs.m0, lhs.m1 - rhs.m1,
                                     lhs.m2 - rhs.m2, lhs.m3 - rhs.m3,
                                     lhs.m4 - rhs.m4, lhs.m5 - rhs.m5,
                                     lhs.m6 - rhs.m6, lhs.m7 - rhs.m7,
                                     lhs.m8 - rhs.m8 );
    }
    namespace detail
    {
      template < class T >
      struct n_fold_iterator_helper
      {
        template < int N >
        struct iter_select
          {}
        ;
        template <>
        struct iter_select <0 >
        {
          typedef iterator_tuple0 < null_type > type;
        };
        template <>
        struct iter_select <1 >
        {
          typedef iterator_tuple1 < T > type;
          static type make( const T & arg )
          {
            return type( arg );
          }
        };
        template <>
        struct iter_select <2 >
        {
          typedef iterator_tuple2 < T, T > type;
          static type make( const T & arg )
          {
            return type( arg, arg );
          }
        };
        template <>
        struct iter_select <3 >
        {
          typedef iterator_tuple3 < T, T, T > type;
          static type make( const T & arg )
          {
            return type( arg, arg, arg );
          }
        };
        template <>
        struct iter_select <4 >
        {
          typedef iterator_tuple4 < T, T, T, T > type;
          static type make( const T & arg )
          {
            return type( arg, arg, arg, arg );
          }
        };
        template <>
        struct iter_select <5 >
        {
          typedef iterator_tuple5 < T, T, T, T, T > type;
          static type make( const T & arg )
          {
            return type( arg, arg, arg, arg, arg );
          }
        };
        template <>
        struct iter_select <6 >
        {
          typedef iterator_tuple6 < T, T, T, T, T, T > type;
          static type make( const T & arg )
          {
            return type( arg, arg, arg, arg, arg, arg );
          }
        };
        template <>
        struct iter_select <7 >
        {
          typedef iterator_tuple7 < T, T, T, T, T, T, T > type;
          static type make( const T & arg )
          {
            return type( arg, arg, arg, arg, arg, arg, arg );
          }
        };
        template <>
        struct iter_select <8 >
        {
          typedef iterator_tuple8 < T, T, T, T, T, T, T, T > type;
          static type make( const T & arg )
          {
            return type( arg, arg, arg, arg, arg, arg, arg, arg );
          }
        };
        template <>
        struct iter_select <9 >
        {
          typedef iterator_tuple9 < T, T, T, T, T, T, T, T, T > type;
          static type make( const T & arg )
          {
            return type( arg, arg, arg, arg, arg, arg, arg, arg, arg );
          }
        };
      };
    }                           // namespace detail

    template < class T, int N >
    struct n_fold_iterator_tuple
    {
      typedef detail::n_fold_iterator_helper < T >::iter_select <
      N > ::type type;

      static type make( const T & arg )
      {
        return detail::n_fold_iterator_helper < T >::iter_select <
               N > ::make( arg );
      }
    };






    namespace detail
    {
      // The initial case.
      template < class T0 >
      struct iterator_tuple_base_type_selector1
      {
        typedef::boost::mpl::if_c < ::boost::is_same < T0, null_type >::value,
        iterator_tuple0 < null_type >, iterator_tuple1 < T0 > > ::type type;
      };

      template < class T0, class T1 >
      struct iterator_tuple_base_type_selector2
      {
        typedef::boost::mpl::if_c < ::boost::is_same < T1, null_type >::value,
        iterator_tuple_base_type_selector1 < T0 >::type,
        iterator_tuple2 < T0, T1 > > ::type type;
      };
      template < class T0, class T1, class T2 >
      struct iterator_tuple_base_type_selector3
      {
        typedef::boost::mpl::if_c < ::boost::is_same < T2, null_type >::value,
        iterator_tuple_base_type_selector2 < T0, T1 >::type,
        iterator_tuple3 < T0, T1, T2 > > ::type type;
      };
      template < class T0, class T1, class T2, class T3 >
      struct iterator_tuple_base_type_selector4
      {
        typedef::boost::mpl::if_c < ::boost::is_same < T3, null_type >::value,
        iterator_tuple_base_type_selector3 < T0, T1, T2 >::type,
        iterator_tuple4 < T0, T1, T2, T3 > > ::type type;
      };
      template < class T0, class T1, class T2, class T3, class T4 >
      struct iterator_tuple_base_type_selector5
      {
        typedef::boost::mpl::if_c < ::boost::is_same < T4, null_type >::value,
        iterator_tuple_base_type_selector4 < T0, T1, T2, T3 >::type,
        iterator_tuple5 < T0, T1, T2, T3, T4 > > ::type type;
      };
      template < class T0, class T1, class T2, class T3, class T4,
      class T5 >
      struct iterator_tuple_base_type_selector6
      {
        typedef::boost::mpl::if_c < ::boost::is_same < T5, null_type >::value,
        iterator_tuple_base_type_selector5 < T0, T1, T2, T3, T4 >::type,
        iterator_tuple6 < T0, T1, T2, T3, T4, T5 > > ::type type;
      };
      template < class T0, class T1, class T2, class T3, class T4, class T5,
      class T6 >
      struct iterator_tuple_base_type_selector7
      {
        typedef::boost::mpl::if_c < ::boost::is_same < T6, null_type >::value,
        iterator_tuple_base_type_selector6 < T0, T1, T2, T3, T4, T5 >::type,
        iterator_tuple7 < T0, T1, T2, T3, T4, T5, T6 > > ::type type;
      };
      template < class T0, class T1, class T2, class T3, class T4, class T5,
      class T6, class T7 >
      struct iterator_tuple_base_type_selector8
      {
        typedef::boost::mpl::if_c < ::boost::is_same < T7, null_type >::value,
        iterator_tuple_base_type_selector7 < T0, T1, T2, T3, T4, T5,
        T6 > ::type, iterator_tuple8 < T0, T1, T2, T3, T4, T5, T6,
        T7 > > ::type type;
      };
      template < class T0, class T1, class T2, class T3, class T4, class T5,
      class T6, class T7,
      class T8 >
      struct iterator_tuple_base_type_selector9
      {
        typedef::boost::mpl::if_c < ::boost::is_same < T8, null_type >::value,
        iterator_tuple_base_type_selector8 < T0, T1, T2, T3, T4, T5, T6,
        T7 > ::type, iterator_tuple9 < T0, T1, T2, T3, T4, T5, T6, T7,
        T8 > > ::type type;
      };

    }                           // namespace detail
    template < class T0 = null_type, class T1 = null_type, class T2 =
    null_type, class T3 = null_type, class T4 = null_type, class T5 =
    null_type, class T6 = null_type, class T7 = null_type, class T8 =
    null_type >
    struct iterator_tuple: public detail::
          iterator_tuple_base_type_selector9 < T0, T1, T2, T3, T4, T5, T6, T7,
          T8 > ::type
    {
      typedef detail::iterator_tuple_base_type_selector9 < T0, T1, T2, T3, T4,
      T5, T6, T7, T8 > ::type base_type;

      iterator_tuple( ) : self_type( )
      {}

      typedef typename base_type::iterator_category iterator_category;
      typedef typename base_type::value_type value_type;
      typedef typename base_type::reference reference;
      typedef typename base_type::const_reference const_reference;
      typedef typename base_type::pointer pointer;
      typedef typename base_type::difference_type difference_type;

      template < class V0 >
      iterator_tuple( V0 v0 ) : self_type( v0 )
      {}
      template < class V0, class V1 >
      iterator_tuple( V0 v0, V1 v1 ) : self_type( v0,
              v1 )
      {}
      template < class V0, class V1, class V2 >
      iterator_tuple( V0 v0, V1 v1, V2 v2 ) : self_type( v0, v1,
              v2 )
      {}
      template < class V0, class V1, class V2, class V3 >
      iterator_tuple( V0 v0, V1 v1, V2 v2, V3 v3 ) : self_type( v0, v1, v2,
              v3 )
      {}
      template < class V0, class V1, class V2, class V3, class V4 >
      iterator_tuple( V0 v0, V1 v1, V2 v2, V3 v3, V4 v4 ) : self_type( v0, v1, v2, v3,
              v4 )
      {}
      template < class V0, class V1, class V2, class V3, class V4, class V5 >
      iterator_tuple( V0 v0, V1 v1, V2 v2, V3 v3, V4 v4, V5 v5 ) : self_type( v0, v1, v2, v3, v4,
              v5 )
      {}
      template < class V0, class V1, class V2, class V3, class V4, class V5, class V6 >
      iterator_tuple( V0 v0, V1 v1, V2 v2, V3 v3, V4 v4, V5 v5, V6 v6 ) : self_type( v0, v1, v2, v3, v4, v5,
              v6 )
      {}
      template < class V0, class V1, class V2, class V3, class V4, class V5, class V6, class V7 >
      iterator_tuple( V0 v0, V1 v1, V2 v2, V3 v3, V4 v4, V5 v5, V6 v6, V7 v7 ) : self_type( v0, v1, v2, v3, v4, v5, v6,
              v7 )
      {}
      template < class V0, class V1, class V2, class V3, class V4, class V5, class V6, class V7, class V8 >
      iterator_tuple( V0 v0, V1 v1, V2 v2, V3 v3, V4 v4, V5 v5, V6 v6, V7 v7, V8 v8 ) : self_type( v0, v1, v2, v3, v4, v5, v6, v7,
              v8 )
      {}

      template < class S0, class S1, class S2, class S3, class S4, class S5,
      class S6, class S7,
      class S8 >
      base_type & operator=( const iterator_tuple < S0, S1, S2,
                             S3, S4, S5, S6, S7, S8 > &rhs )
      {
        //BOOST_STATIC_ASSERT(  );
        return ( base_type::operator=( rhs ) );
      }
    };
  }                             // namespace tupple
}                               // namespace boost


#endif
