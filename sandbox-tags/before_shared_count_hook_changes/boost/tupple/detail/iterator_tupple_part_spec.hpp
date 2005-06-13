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
#ifndef BOOST_TUPPLE_DETAIL_ITERATOR_TUPPLE_PART_SPEC_HPP
#define BOOST_TUPPLE_DETAIL_ITERATOR_TUPPLE_PART_SPEC_HPP
#include <boost/tupple/tupple.hpp>
#include <boost/tupple/detail/iterator_tupple_traits_detail.hpp>
#include <boost/tupple/detail/ref_workaround.hpp>

#include <boost/utility.hpp>







namespace boost
{
  namespace tupple
  {
    template < class T0 = null_type, class T1 = null_type, class T2 =
    null_type, class T3 = null_type, class T4 = null_type, class T5 =
    null_type, class T6 = null_type, class T7 = null_type, class T8 =
    null_type, class T9 = null_type >
    struct iterator_tuple
      {}
    ;
    template < class R0 = null_type, class R1 = null_type, class R2 =
    null_type, class R3 = null_type, class R4 = null_type, class R5 =
    null_type, class R6 = null_type, class R7 = null_type, class R8 =
    null_type, class R9 = null_type >
    struct iterator_traits_tuple
      {}
    ;



    template <>
    struct iterator_tuple < null_type, null_type, null_type,
          null_type, null_type, null_type, null_type, null_type, null_type,
          null_type > : public tuple < null_type >
      {}
    ;
    template < class R0 >
    struct iterator_traits_tuple < R0, null_type,
          null_type, null_type, null_type, null_type, null_type, null_type,
          null_type, null_type >
    {
      typedef tuple < typename R0::iterator_category > iterator_category;
      typedef tuple < typename R0::value_type > value_type;
      typedef tuple < typename R0::reference > reference;
      typedef tuple < const typename R0::reference > const_reference;
      typedef tuple < typename R0::pointer > pointer;
      typedef tuple < typename R0::difference_type > difference_type;
    };
    template < class T0 >
    struct iterator_tuple < T0, null_type, null_type,
          null_type, null_type, null_type, null_type, null_type, null_type,
          null_type > : public tuple < T0 >
    {
      typedef iterator_tuple < T0 > self_type;
      typedef tuple < T0 > parent_type;
      typedef iterator_traits_tuple < detail::iterator_traits < T0 > >traits;
      typedef typename traits::iterator_category iterator_category;
      typedef typename traits::value_type value_type;
      typedef typename traits::reference reference;
      typedef typename traits::const_reference const_reference;
      typedef typename traits::pointer pointer;
      typedef typename traits::difference_type difference_type;
      iterator_tuple( )
      {}
      iterator_tuple( arg_type0 theM0 ) : parent_type( theM0 )
      {}
      template < class S0 >
      iterator_tuple( const iterator_tuple < S0 >
                      &rhs ) : parent_type( rhs )
      {}
      iterator_tuple( arg_type0 theM0,
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
        return reference( *m0 );
      }
      const_reference operator*( ) const
      {
        return const_reference( *m0 );
      }
      reference operator[] ( const difference_type & i )
      {
        return reference( m0[ i.m0 ] );
      }
      const_reference operator[] ( const difference_type & i ) const
      {
        return const_reference( m0[ i.m0 ] );
      }
    };
    template < class T0 >
    iterator_tuple < T0 >
    operator+( const iterator_tuple < T0 > &lhs,
               const iterator_tuple < T0 >::difference_type & rhs )
    {
      return iterator_tuple < T0 > ( lhs ) += rhs;
    }
    template < class T0 >
    iterator_tuple < T0 >
    operator-( const iterator_tuple < T0 > &lhs,
               const iterator_tuple < T0 >::difference_type & rhs )
    {
      return iterator_tuple < T0 > ( lhs ) -= rhs;
    }
    template < class T0 >
    iterator_tuple <
    T0 > ::difference_type operator-( const iterator_tuple < T0 > &lhs,
                                      const iterator_tuple < T0 > &rhs )
    {
      return iterator_tuple < T0 >::difference_type( lhs.m0 - rhs.m0 );
    }
    template < class R0, class R1 >
    struct iterator_traits_tuple < R0, R1,
          null_type, null_type, null_type, null_type, null_type, null_type,
          null_type, null_type >
    {
      typedef tuple < typename R0::iterator_category,
      typename R1::iterator_category > iterator_category;
      typedef tuple < typename R0::value_type,
      typename R1::value_type > value_type;
      typedef tuple < typename R0::reference,
      typename R1::reference > reference;
      typedef tuple < const typename R0::reference,
      const typename R1::reference > const_reference;
      typedef tuple < typename R0::pointer, typename R1::pointer > pointer;
      typedef tuple < typename R0::difference_type,
      typename R1::difference_type > difference_type;
    };
    template < class T0, class T1 >
    struct iterator_tuple < T0, T1, null_type,
          null_type, null_type, null_type, null_type, null_type, null_type,
          null_type > : public tuple < T0, T1 >
    {
      typedef iterator_tuple < T0, T1 > self_type;
      typedef tuple < T0, T1 > parent_type;
      typedef iterator_traits_tuple < detail::iterator_traits < T0 >,
      detail::iterator_traits < T1 > > traits;
      typedef typename traits::iterator_category iterator_category;
      typedef typename traits::value_type value_type;
      typedef typename traits::reference reference;
      typedef typename traits::const_reference const_reference;
      typedef typename traits::pointer pointer;
      typedef typename traits::difference_type difference_type;
      iterator_tuple( )
      {}
      iterator_tuple( arg_type0 theM0, arg_type1 theM1 ) : parent_type( theM0,
              theM1 )
      {}
      template < class S0,
      class S1 >
      iterator_tuple( const iterator_tuple < S0,
                      S1 > &rhs ) : parent_type( rhs )
      {}
      iterator_tuple( arg_type0 theM0,
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
        return reference( *m0, *m1 );
      }
      const_reference operator*( ) const
      {
        return const_reference( *m0, *m1 );
      }
      reference operator[] ( const difference_type & i )
      {
        return reference( m0[ i.m0 ], m1[ i.m1 ] );
      }
      const_reference operator[] ( const difference_type & i ) const
      {
        return const_reference( m0[ i.m0 ], m1[ i.m1 ] );
      }
    };
    template < class T0, class T1 >
    iterator_tuple < T0,
    T1 > operator+( const iterator_tuple < T0, T1 > &lhs,
                    const iterator_tuple < T0, T1 >::difference_type & rhs )
    {
      return iterator_tuple < T0, T1 > ( lhs ) += rhs;
    }
    template < class T0, class T1 >
    iterator_tuple < T0,
    T1 > operator-( const iterator_tuple < T0, T1 > &lhs,
                    const iterator_tuple < T0, T1 >::difference_type & rhs )
    {
      return iterator_tuple < T0, T1 > ( lhs ) -= rhs;
    }
    template < class T0, class T1 >
    iterator_tuple < T0,
    T1 > ::difference_type operator-( const iterator_tuple < T0, T1 > &lhs,
                                      const iterator_tuple < T0, T1 > &rhs )
    {
      return iterator_tuple < T0, T1 >::difference_type( lhs.m0 - rhs.m0,
             lhs.m1 - rhs.m1 );
    }
    template < class R0, class R1,
    class R2 >
    struct iterator_traits_tuple < R0, R1, R2, null_type,
          null_type, null_type, null_type, null_type, null_type, null_type >
    {
      typedef tuple < typename R0::iterator_category,
      typename R1::iterator_category,
      typename R2::iterator_category > iterator_category;
      typedef tuple < typename R0::value_type, typename R1::value_type,
      typename R2::value_type > value_type;
      typedef tuple < typename R0::reference, typename R1::reference,
      typename R2::reference > reference;
      typedef tuple < const typename R0::reference,
      const typename R1::reference,
      const typename R2::reference > const_reference;
      typedef tuple < typename R0::pointer, typename R1::pointer,
      typename R2::pointer > pointer;
      typedef tuple < typename R0::difference_type,
      typename R1::difference_type,
      typename R2::difference_type > difference_type;
    };
    template < class T0, class T1, class T2 >
    struct iterator_tuple < T0, T1,
          T2, null_type, null_type, null_type, null_type, null_type, null_type,
          null_type > : public tuple < T0, T1, T2 >
    {
      typedef iterator_tuple < T0, T1, T2 > self_type;
      typedef tuple < T0, T1, T2 > parent_type;
      typedef iterator_traits_tuple < detail::iterator_traits < T0 >,
      detail::iterator_traits < T1 >,
      detail::iterator_traits < T2 > > traits;
      typedef typename traits::iterator_category iterator_category;
      typedef typename traits::value_type value_type;
      typedef typename traits::reference reference;
      typedef typename traits::const_reference const_reference;
      typedef typename traits::pointer pointer;
      typedef typename traits::difference_type difference_type;
      iterator_tuple( )
      {}
      iterator_tuple( arg_type0 theM0, arg_type1 theM1,
                      arg_type2 theM2 ) : parent_type( theM0, theM1, theM2 )
      {}
      template < class S0, class S1,
      class S2 >
      iterator_tuple( const iterator_tuple < S0, S1,
                      S2 > &rhs ) : parent_type( rhs )
      {}
      iterator_tuple( arg_type0 theM0,
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
        return reference( *m0, *m1, *m2 );
      }
      const_reference operator*( ) const
      {
        return const_reference( *m0, *m1, *m2 );
      }
      reference operator[] ( const difference_type & i )
      {
        return reference( m0[ i.m0 ], m1[ i.m1 ], m2[ i.m2 ] );
      }
      const_reference operator[] ( const difference_type & i ) const
      {
        return const_reference( m0[ i.m0 ], m1[ i.m1 ], m2[ i.m2 ] );
      }
    };
    template < class T0, class T1, class T2 >
    iterator_tuple < T0, T1,
    T2 > operator+( const iterator_tuple < T0, T1, T2 > &lhs,
                    const iterator_tuple < T0, T1,
                    T2 >::difference_type & rhs )
    {
      return iterator_tuple < T0, T1, T2 > ( lhs ) += rhs;
    }
    template < class T0, class T1, class T2 >
    iterator_tuple < T0, T1,
    T2 > operator-( const iterator_tuple < T0, T1, T2 > &lhs,
                    const iterator_tuple < T0, T1,
                    T2 >::difference_type & rhs )
    {
      return iterator_tuple < T0, T1, T2 > ( lhs ) -= rhs;
    }
    template < class T0, class T1, class T2 >
    iterator_tuple < T0, T1,
    T2 > ::difference_type operator-( const iterator_tuple < T0, T1,
                                      T2 > &lhs, const iterator_tuple < T0,
                                      T1, T2 > &rhs )
    {
      return iterator_tuple < T0, T1, T2 >::difference_type( lhs.m0 - rhs.m0,
             lhs.m1 - rhs.m1,
             lhs.m2 -
             rhs.m2 );
    }
    template < class R0, class R1, class R2,
    class R3 >
    struct iterator_traits_tuple < R0, R1, R2, R3, null_type,
          null_type, null_type, null_type, null_type, null_type >
    {
      typedef tuple < typename R0::iterator_category,
      typename R1::iterator_category, typename R2::iterator_category,
      typename R3::iterator_category > iterator_category;
      typedef tuple < typename R0::value_type, typename R1::value_type,
      typename R2::value_type, typename R3::value_type > value_type;
      typedef tuple < typename R0::reference, typename R1::reference,
      typename R2::reference, typename R3::reference > reference;
      typedef tuple < const typename R0::reference,
      const typename R1::reference, const typename R2::reference,
      const typename R3::reference > const_reference;
      typedef tuple < typename R0::pointer, typename R1::pointer,
      typename R2::pointer, typename R3::pointer > pointer;
      typedef tuple < typename R0::difference_type,
      typename R1::difference_type, typename R2::difference_type,
      typename R3::difference_type > difference_type;
    };
    template < class T0, class T1, class T2,
    class T3 >
    struct iterator_tuple < T0, T1, T2, T3, null_type, null_type,
          null_type, null_type, null_type, null_type > : public tuple < T0, T1, T2,
          T3 >
    {
      typedef iterator_tuple < T0, T1, T2, T3 > self_type;
      typedef tuple < T0, T1, T2, T3 > parent_type;
      typedef iterator_traits_tuple < detail::iterator_traits < T0 >,
      detail::iterator_traits < T1 >, detail::iterator_traits < T2 >,
      detail::iterator_traits < T3 > > traits;
      typedef typename traits::iterator_category iterator_category;
      typedef typename traits::value_type value_type;
      typedef typename traits::reference reference;
      typedef typename traits::const_reference const_reference;
      typedef typename traits::pointer pointer;
      typedef typename traits::difference_type difference_type;
      iterator_tuple( )
      {}
      iterator_tuple( arg_type0 theM0, arg_type1 theM1, arg_type2 theM2,
                      arg_type3 theM3 ) : parent_type( theM0, theM1, theM2,
                                                           theM3 )
      {}
      template < class S0, class S1, class S2,
      class S3 >
      iterator_tuple( const iterator_tuple < S0, S1, S2,
                      S3 > &rhs ) : parent_type( rhs )
      {}
      iterator_tuple( arg_type0 theM0,
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
        return reference( *m0, *m1, *m2, *m3 );
      }
      const_reference operator*( ) const
      {
        return const_reference( *m0, *m1, *m2, *m3 );
      }
      reference operator[] ( const difference_type & i )
      {
        return reference( m0[ i.m0 ], m1[ i.m1 ], m2[ i.m2 ], m3[ i.m3 ] );
      }
      const_reference operator[] ( const difference_type & i ) const
      {
        return const_reference( m0[ i.m0 ], m1[ i.m1 ], m2[ i.m2 ], m3[ i.m3 ] );
      }
    };
    template < class T0, class T1, class T2, class T3 >
    iterator_tuple < T0,
    T1, T2, T3 > operator+( const iterator_tuple < T0, T1, T2, T3 > &lhs,
                            const iterator_tuple < T0, T1, T2,
                            T3 >::difference_type & rhs )
    {
      return iterator_tuple < T0, T1, T2, T3 > ( lhs ) += rhs;
    }
    template < class T0, class T1, class T2, class T3 >
    iterator_tuple < T0,
    T1, T2, T3 > operator-( const iterator_tuple < T0, T1, T2, T3 > &lhs,
                            const iterator_tuple < T0, T1, T2,
                            T3 >::difference_type & rhs )
    {
      return iterator_tuple < T0, T1, T2, T3 > ( lhs ) -= rhs;
    }
    template < class T0, class T1, class T2, class T3 >
    iterator_tuple < T0,
    T1, T2, T3 > ::difference_type operator-( const iterator_tuple < T0, T1,
        T2, T3 > &lhs,
        const iterator_tuple < T0, T1,
        T2, T3 > &rhs )
    {
      return iterator_tuple < T0, T1, T2,
             T3 > ::difference_type( lhs.m0 - rhs.m0, lhs.m1 - rhs.m1,
                                     lhs.m2 - rhs.m2, lhs.m3 - rhs.m3 );
    }
    template < class R0, class R1, class R2, class R3,
    class R4 >
    struct iterator_traits_tuple < R0, R1, R2, R3, R4, null_type,
          null_type, null_type, null_type, null_type >
    {
      typedef tuple < typename R0::iterator_category,
      typename R1::iterator_category, typename R2::iterator_category,
      typename R3::iterator_category,
      typename R4::iterator_category > iterator_category;
      typedef tuple < typename R0::value_type, typename R1::value_type,
      typename R2::value_type, typename R3::value_type,
      typename R4::value_type > value_type;
      typedef tuple < typename R0::reference, typename R1::reference,
      typename R2::reference, typename R3::reference,
      typename R4::reference > reference;
      typedef tuple < const typename R0::reference,
      const typename R1::reference, const typename R2::reference,
      const typename R3::reference,
      const typename R4::reference > const_reference;
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
    struct iterator_tuple < T0, T1, T2, T3, T4, null_type,
          null_type, null_type, null_type, null_type > : public tuple < T0, T1, T2,
          T3, T4 >
    {
      typedef iterator_tuple < T0, T1, T2, T3, T4 > self_type;
      typedef tuple < T0, T1, T2, T3, T4 > parent_type;
      typedef iterator_traits_tuple < detail::iterator_traits < T0 >,
      detail::iterator_traits < T1 >, detail::iterator_traits < T2 >,
      detail::iterator_traits < T3 >,
      detail::iterator_traits < T4 > > traits;
      typedef typename traits::iterator_category iterator_category;
      typedef typename traits::value_type value_type;
      typedef typename traits::reference reference;
      typedef typename traits::const_reference const_reference;
      typedef typename traits::pointer pointer;
      typedef typename traits::difference_type difference_type;
      iterator_tuple( )
      {}
      iterator_tuple( arg_type0 theM0, arg_type1 theM1, arg_type2 theM2,
                      arg_type3 theM3, arg_type4 theM4 ) : parent_type( theM0,
                              theM1,
                              theM2,
                              theM3,
                              theM4 )
      {}
      template < class S0, class S1, class S2, class S3,
      class S4 >
      iterator_tuple( const iterator_tuple < S0, S1, S2, S3,
                      S4 > &rhs ) : parent_type( rhs )
      {}
      iterator_tuple( arg_type0 theM0,
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
        return reference( *m0, *m1, *m2, *m3, *m4 );
      }
      const_reference operator*( ) const
      {
        return const_reference( *m0, *m1, *m2, *m3, *m4 );
      }
      reference operator[] ( const difference_type & i )
      {
        return reference( m0[ i.m0 ], m1[ i.m1 ], m2[ i.m2 ], m3[ i.m3 ], m4[ i.m4 ] );
      }
      const_reference operator[] ( const difference_type & i ) const
      {
        return const_reference( m0[ i.m0 ], m1[ i.m1 ], m2[ i.m2 ], m3[ i.m3 ],
                                m4[ i.m4 ] );
      }
    };
    template < class T0, class T1, class T2, class T3,
    class T4 >
    iterator_tuple < T0, T1, T2, T3,
    T4 > operator+( const iterator_tuple < T0, T1, T2, T3, T4 > &lhs,
                    const iterator_tuple < T0, T1, T2, T3,
                    T4 >::difference_type & rhs )
    {
      return iterator_tuple < T0, T1, T2, T3, T4 > ( lhs ) += rhs;
    }
    template < class T0, class T1, class T2, class T3,
    class T4 >
    iterator_tuple < T0, T1, T2, T3,
    T4 > operator-( const iterator_tuple < T0, T1, T2, T3, T4 > &lhs,
                    const iterator_tuple < T0, T1, T2, T3,
                    T4 >::difference_type & rhs )
    {
      return iterator_tuple < T0, T1, T2, T3, T4 > ( lhs ) -= rhs;
    }
    template < class T0, class T1, class T2, class T3,
    class T4 >
    iterator_tuple < T0, T1, T2, T3,
    T4 > ::difference_type operator-( const iterator_tuple < T0, T1, T2, T3,
                                      T4 > &lhs, const iterator_tuple < T0,
                                      T1, T2, T3, T4 > &rhs )
    {
      return iterator_tuple < T0, T1, T2, T3,
             T4 > ::difference_type( lhs.m0 - rhs.m0, lhs.m1 - rhs.m1,
                                     lhs.m2 - rhs.m2, lhs.m3 - rhs.m3,
                                     lhs.m4 - rhs.m4 );
    }
    template < class R0, class R1, class R2, class R3, class R4,
    class R5 >
    struct iterator_traits_tuple < R0, R1, R2, R3, R4, R5,
          null_type, null_type, null_type, null_type >
    {
      typedef tuple < typename R0::iterator_category,
      typename R1::iterator_category, typename R2::iterator_category,
      typename R3::iterator_category, typename R4::iterator_category,
      typename R5::iterator_category > iterator_category;
      typedef tuple < typename R0::value_type, typename R1::value_type,
      typename R2::value_type, typename R3::value_type,
      typename R4::value_type, typename R5::value_type > value_type;
      typedef tuple < typename R0::reference, typename R1::reference,
      typename R2::reference, typename R3::reference,
      typename R4::reference, typename R5::reference > reference;
      typedef tuple < const typename R0::reference,
      const typename R1::reference, const typename R2::reference,
      const typename R3::reference, const typename R4::reference,
      const typename R5::reference > const_reference;
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
    struct iterator_tuple < T0, T1, T2, T3, T4, T5, null_type,
          null_type, null_type, null_type > : public tuple < T0, T1, T2, T3, T4,
          T5 >
    {
      typedef iterator_tuple < T0, T1, T2, T3, T4, T5 > self_type;
      typedef tuple < T0, T1, T2, T3, T4, T5 > parent_type;
      typedef iterator_traits_tuple < detail::iterator_traits < T0 >,
      detail::iterator_traits < T1 >, detail::iterator_traits < T2 >,
      detail::iterator_traits < T3 >, detail::iterator_traits < T4 >,
      detail::iterator_traits < T5 > > traits;
      typedef typename traits::iterator_category iterator_category;
      typedef typename traits::value_type value_type;
      typedef typename traits::reference reference;
      typedef typename traits::const_reference const_reference;
      typedef typename traits::pointer pointer;
      typedef typename traits::difference_type difference_type;
      iterator_tuple( )
      {}
      iterator_tuple( arg_type0 theM0, arg_type1 theM1, arg_type2 theM2,
                      arg_type3 theM3, arg_type4 theM4,
                      arg_type5 theM5 ) : parent_type( theM0, theM1, theM2,
                                                           theM3, theM4, theM5 )
      {}
      template < class S0, class S1, class S2, class S3, class S4,
      class S5 >
      iterator_tuple( const iterator_tuple < S0, S1, S2, S3, S4,
                      S5 > &rhs ) : parent_type( rhs )
      {}
      iterator_tuple( arg_type0 theM0,
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
        return reference( *m0, *m1, *m2, *m3, *m4, *m5 );
      }
      const_reference operator*( ) const
      {
        return const_reference( *m0, *m1, *m2, *m3, *m4, *m5 );
      }
      reference operator[] ( const difference_type & i )
      {
        return reference( m0[ i.m0 ], m1[ i.m1 ], m2[ i.m2 ], m3[ i.m3 ], m4[ i.m4 ],
                          m5[ i.m5 ] );
      }
      const_reference operator[] ( const difference_type & i ) const
      {
        return const_reference( m0[ i.m0 ], m1[ i.m1 ], m2[ i.m2 ], m3[ i.m3 ],
                                m4[ i.m4 ], m5[ i.m5 ] );
      }
    };
    template < class T0, class T1, class T2, class T3, class T4,
    class T5 >
    iterator_tuple < T0, T1, T2, T3, T4,
    T5 > operator+( const iterator_tuple < T0, T1, T2, T3, T4, T5 > &lhs,
                    const iterator_tuple < T0, T1, T2, T3, T4,
                    T5 >::difference_type & rhs )
    {
      return iterator_tuple < T0, T1, T2, T3, T4, T5 > ( lhs ) += rhs;
    }
    template < class T0, class T1, class T2, class T3, class T4,
    class T5 >
    iterator_tuple < T0, T1, T2, T3, T4,
    T5 > operator-( const iterator_tuple < T0, T1, T2, T3, T4, T5 > &lhs,
                    const iterator_tuple < T0, T1, T2, T3, T4,
                    T5 >::difference_type & rhs )
    {
      return iterator_tuple < T0, T1, T2, T3, T4, T5 > ( lhs ) -= rhs;
    }
    template < class T0, class T1, class T2, class T3, class T4,
    class T5 >
    iterator_tuple < T0, T1, T2, T3, T4,
    T5 > ::difference_type operator-( const iterator_tuple < T0, T1, T2, T3,
                                      T4, T5 > &lhs,
                                      const iterator_tuple < T0, T1, T2, T3,
                                      T4, T5 > &rhs )
    {
      return iterator_tuple < T0, T1, T2, T3, T4,
             T5 > ::difference_type( lhs.m0 - rhs.m0, lhs.m1 - rhs.m1,
                                     lhs.m2 - rhs.m2, lhs.m3 - rhs.m3,
                                     lhs.m4 - rhs.m4, lhs.m5 - rhs.m5 );
    }
    template < class R0, class R1, class R2, class R3, class R4, class R5,
    class R6 >
    struct iterator_traits_tuple < R0, R1, R2, R3, R4, R5, R6,
          null_type, null_type, null_type >
    {
      typedef tuple < typename R0::iterator_category,
      typename R1::iterator_category, typename R2::iterator_category,
      typename R3::iterator_category, typename R4::iterator_category,
      typename R5::iterator_category,
      typename R6::iterator_category > iterator_category;
      typedef tuple < typename R0::value_type, typename R1::value_type,
      typename R2::value_type, typename R3::value_type,
      typename R4::value_type, typename R5::value_type,
      typename R6::value_type > value_type;
      typedef tuple < typename R0::reference, typename R1::reference,
      typename R2::reference, typename R3::reference,
      typename R4::reference, typename R5::reference,
      typename R6::reference > reference;
      typedef tuple < const typename R0::reference,
      const typename R1::reference, const typename R2::reference,
      const typename R3::reference, const typename R4::reference,
      const typename R5::reference,
      const typename R6::reference > const_reference;
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
    struct iterator_tuple < T0, T1, T2, T3, T4, T5, T6, null_type,
          null_type, null_type > : public tuple < T0, T1, T2, T3, T4, T5, T6 >
    {
      typedef iterator_tuple < T0, T1, T2, T3, T4, T5, T6 > self_type;
      typedef tuple < T0, T1, T2, T3, T4, T5, T6 > parent_type;
      typedef iterator_traits_tuple < detail::iterator_traits < T0 >,
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
      iterator_tuple( )
      {}
      iterator_tuple( arg_type0 theM0, arg_type1 theM1, arg_type2 theM2,
                      arg_type3 theM3, arg_type4 theM4, arg_type5 theM5,
                      arg_type6 theM6 ) : parent_type( theM0, theM1, theM2,
                                                           theM3, theM4, theM5,
                                                           theM6 )
      {}
      template < class S0, class S1, class S2, class S3, class S4, class S5,
      class S6 >
      iterator_tuple( const iterator_tuple < S0, S1, S2, S3, S4,
                      S5, S6 > &rhs ) : parent_type( rhs )
      {}
      iterator_tuple( arg_type0 theM0,
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
        return reference( *m0, *m1, *m2, *m3, *m4, *m5, *m6 );
      }
      const_reference operator*( ) const
      {
        return const_reference( *m0, *m1, *m2, *m3, *m4, *m5, *m6 );
      }
      reference operator[] ( const difference_type & i )
      {
        return reference( m0[ i.m0 ], m1[ i.m1 ], m2[ i.m2 ], m3[ i.m3 ], m4[ i.m4 ],
                          m5[ i.m5 ], m6[ i.m6 ] );
      }
      const_reference operator[] ( const difference_type & i ) const
      {
        return const_reference( m0[ i.m0 ], m1[ i.m1 ], m2[ i.m2 ], m3[ i.m3 ],
                                m4[ i.m4 ], m5[ i.m5 ], m6[ i.m6 ] );
      }
    };
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6 >
    iterator_tuple < T0, T1, T2, T3, T4, T5,
    T6 > operator+( const iterator_tuple < T0, T1, T2, T3, T4, T5,
                    T6 > &lhs, const iterator_tuple < T0, T1, T2, T3, T4,
                    T5, T6 > ::difference_type & rhs )
    {
      return iterator_tuple < T0, T1, T2, T3, T4, T5, T6 > ( lhs ) += rhs;
    }
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6 >
    iterator_tuple < T0, T1, T2, T3, T4, T5,
    T6 > operator-( const iterator_tuple < T0, T1, T2, T3, T4, T5,
                    T6 > &lhs, const iterator_tuple < T0, T1, T2, T3, T4,
                    T5, T6 > ::difference_type & rhs )
    {
      return iterator_tuple < T0, T1, T2, T3, T4, T5, T6 > ( lhs ) -= rhs;
    }
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6 >
    iterator_tuple < T0, T1, T2, T3, T4, T5,
    T6 > ::difference_type operator-( const iterator_tuple < T0, T1, T2, T3,
                                      T4, T5, T6 > &lhs,
                                      const iterator_tuple < T0, T1, T2, T3,
                                      T4, T5, T6 > &rhs )
    {
      return iterator_tuple < T0, T1, T2, T3, T4, T5,
             T6 > ::difference_type( lhs.m0 - rhs.m0, lhs.m1 - rhs.m1,
                                     lhs.m2 - rhs.m2, lhs.m3 - rhs.m3,
                                     lhs.m4 - rhs.m4, lhs.m5 - rhs.m5,
                                     lhs.m6 - rhs.m6 );
    }
    template < class R0, class R1, class R2, class R3, class R4, class R5,
    class R6, class R7 >
    struct iterator_traits_tuple < R0, R1, R2, R3, R4,
          R5, R6, R7, null_type, null_type >
    {
      typedef tuple < typename R0::iterator_category,
      typename R1::iterator_category, typename R2::iterator_category,
      typename R3::iterator_category, typename R4::iterator_category,
      typename R5::iterator_category, typename R6::iterator_category,
      typename R7::iterator_category > iterator_category;
      typedef tuple < typename R0::value_type, typename R1::value_type,
      typename R2::value_type, typename R3::value_type,
      typename R4::value_type, typename R5::value_type,
      typename R6::value_type, typename R7::value_type > value_type;
      typedef tuple < typename R0::reference, typename R1::reference,
      typename R2::reference, typename R3::reference,
      typename R4::reference, typename R5::reference,
      typename R6::reference, typename R7::reference > reference;
      typedef tuple < const typename R0::reference,
      const typename R1::reference, const typename R2::reference,
      const typename R3::reference, const typename R4::reference,
      const typename R5::reference, const typename R6::reference,
      const typename R7::reference > const_reference;
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
    struct iterator_tuple < T0, T1, T2, T3, T4, T5, T6,
          T7, null_type, null_type > : public tuple < T0, T1, T2, T3, T4, T5, T6,
          T7 >
    {
      typedef iterator_tuple < T0, T1, T2, T3, T4, T5, T6, T7 > self_type;
      typedef tuple < T0, T1, T2, T3, T4, T5, T6, T7 > parent_type;
      typedef iterator_traits_tuple < detail::iterator_traits < T0 >,
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
      iterator_tuple( )
      {}
      iterator_tuple( arg_type0 theM0, arg_type1 theM1, arg_type2 theM2,
                      arg_type3 theM3, arg_type4 theM4, arg_type5 theM5,
                      arg_type6 theM6, arg_type7 theM7 ) : parent_type( theM0,
                              theM1,
                              theM2,
                              theM3,
                              theM4,
                              theM5,
                              theM6,
                              theM7 )
      {}
      template < class S0, class S1, class S2, class S3, class S4, class S5,
      class S6, class S7 >
      iterator_tuple( const iterator_tuple < S0, S1,
                      S2, S3, S4, S5, S6,
                      S7 > &rhs ) : parent_type( rhs )
      {}
      iterator_tuple( arg_type0 theM0,
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
        return reference( *m0, *m1, *m2, *m3, *m4, *m5, *m6, *m7 );
      }
      const_reference operator*( ) const
      {
        return const_reference( *m0, *m1, *m2, *m3, *m4, *m5, *m6, *m7 );
      }
      reference operator[] ( const difference_type & i )
      {
        return reference( m0[ i.m0 ], m1[ i.m1 ], m2[ i.m2 ], m3[ i.m3 ], m4[ i.m4 ],
                          m5[ i.m5 ], m6[ i.m6 ], m7[ i.m7 ] );
      }
      const_reference operator[] ( const difference_type & i ) const
      {
        return const_reference( m0[ i.m0 ], m1[ i.m1 ], m2[ i.m2 ], m3[ i.m3 ],
                                m4[ i.m4 ], m5[ i.m5 ], m6[ i.m6 ], m7[ i.m7 ] );
      }
    };
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6, class T7 >
    iterator_tuple < T0, T1, T2, T3, T4, T5, T6,
    T7 > operator+( const iterator_tuple < T0, T1, T2, T3, T4, T5, T6,
                    T7 > &lhs, const iterator_tuple < T0, T1, T2, T3, T4,
                    T5, T6, T7 > ::difference_type & rhs )
    {
      return iterator_tuple < T0, T1, T2, T3, T4, T5, T6, T7 > ( lhs ) += rhs;
    }
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6, class T7 >
    iterator_tuple < T0, T1, T2, T3, T4, T5, T6,
    T7 > operator-( const iterator_tuple < T0, T1, T2, T3, T4, T5, T6,
                    T7 > &lhs, const iterator_tuple < T0, T1, T2, T3, T4,
                    T5, T6, T7 > ::difference_type & rhs )
    {
      return iterator_tuple < T0, T1, T2, T3, T4, T5, T6, T7 > ( lhs ) -= rhs;
    }
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6, class T7 >
    iterator_tuple < T0, T1, T2, T3, T4, T5, T6,
    T7 > ::difference_type operator-( const iterator_tuple < T0, T1, T2, T3,
                                      T4, T5, T6, T7 > &lhs,
                                      const iterator_tuple < T0, T1, T2, T3,
                                      T4, T5, T6, T7 > &rhs )
    {
      return iterator_tuple < T0, T1, T2, T3, T4, T5, T6,
             T7 > ::difference_type( lhs.m0 - rhs.m0, lhs.m1 - rhs.m1,
                                     lhs.m2 - rhs.m2, lhs.m3 - rhs.m3,
                                     lhs.m4 - rhs.m4, lhs.m5 - rhs.m5,
                                     lhs.m6 - rhs.m6, lhs.m7 - rhs.m7 );
    }
    template < class R0, class R1, class R2, class R3, class R4, class R5,
    class R6, class R7, class R8 >
    struct iterator_traits_tuple < R0, R1, R2,
          R3, R4, R5, R6, R7, R8, null_type >
    {
      typedef tuple < typename R0::iterator_category,
      typename R1::iterator_category, typename R2::iterator_category,
      typename R3::iterator_category, typename R4::iterator_category,
      typename R5::iterator_category, typename R6::iterator_category,
      typename R7::iterator_category,
      typename R8::iterator_category > iterator_category;
      typedef tuple < typename R0::value_type, typename R1::value_type,
      typename R2::value_type, typename R3::value_type,
      typename R4::value_type, typename R5::value_type,
      typename R6::value_type, typename R7::value_type,
      typename R8::value_type > value_type;
      typedef tuple < typename R0::reference, typename R1::reference,
      typename R2::reference, typename R3::reference,
      typename R4::reference, typename R5::reference,
      typename R6::reference, typename R7::reference,
      typename R8::reference > reference;
      typedef tuple < const typename R0::reference,
      const typename R1::reference, const typename R2::reference,
      const typename R3::reference, const typename R4::reference,
      const typename R5::reference, const typename R6::reference,
      const typename R7::reference,
      const typename R8::reference > const_reference;
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
    struct iterator_tuple < T0, T1, T2, T3,
          T4, T5, T6, T7, T8, null_type > : public tuple < T0, T1, T2, T3, T4, T5,
          T6, T7, T8 >
    {
      typedef iterator_tuple < T0, T1, T2, T3, T4, T5, T6, T7, T8 > self_type;
      typedef tuple < T0, T1, T2, T3, T4, T5, T6, T7, T8 > parent_type;
      typedef iterator_traits_tuple < detail::iterator_traits < T0 >,
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
      iterator_tuple( )
      {}
      iterator_tuple( arg_type0 theM0, arg_type1 theM1, arg_type2 theM2,
                      arg_type3 theM3, arg_type4 theM4, arg_type5 theM5,
                      arg_type6 theM6, arg_type7 theM7,
                      arg_type8 theM8 ) : parent_type( theM0, theM1, theM2,
                                                           theM3, theM4, theM5,
                                                           theM6, theM7, theM8 )
      {}
      template < class S0, class S1, class S2, class S3, class S4, class S5,
      class S6, class S7,
      class S8 >
      iterator_tuple( const iterator_tuple < S0, S1, S2, S3, S4,
                      S5, S6, S7, S8 > &rhs ) : parent_type( rhs )
      {}
      iterator_tuple( arg_type0 theM0,
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
        return reference( *m0, *m1, *m2, *m3, *m4, *m5, *m6, *m7, *m8 );
      }
      const_reference operator*( ) const
      {
        return const_reference( *m0, *m1, *m2, *m3, *m4, *m5, *m6, *m7, *m8 );
      }
      reference operator[] ( const difference_type & i )
      {
        return reference( m0[ i.m0 ], m1[ i.m1 ], m2[ i.m2 ], m3[ i.m3 ], m4[ i.m4 ],
                          m5[ i.m5 ], m6[ i.m6 ], m7[ i.m7 ], m8[ i.m8 ] );
      }
      const_reference operator[] ( const difference_type & i ) const
      {
        return const_reference( m0[ i.m0 ], m1[ i.m1 ], m2[ i.m2 ], m3[ i.m3 ],
                                m4[ i.m4 ], m5[ i.m5 ], m6[ i.m6 ], m7[ i.m7 ],
                                m8[ i.m8 ] );
      }
    };
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6, class T7, class T8 >
    iterator_tuple < T0, T1, T2, T3, T4, T5,
    T6, T7, T8 > operator+( const iterator_tuple < T0, T1, T2, T3, T4, T5,
                            T6, T7, T8 > &lhs, const iterator_tuple < T0,
                            T1, T2, T3, T4, T5, T6, T7,
                            T8 > ::difference_type & rhs )
    {
      return iterator_tuple < T0, T1, T2, T3, T4, T5, T6, T7, T8 > ( lhs ) +=
               rhs;
    }
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6, class T7, class T8 >
    iterator_tuple < T0, T1, T2, T3, T4, T5,
    T6, T7, T8 > operator-( const iterator_tuple < T0, T1, T2, T3, T4, T5,
                            T6, T7, T8 > &lhs, const iterator_tuple < T0,
                            T1, T2, T3, T4, T5, T6, T7,
                            T8 > ::difference_type & rhs )
    {
      return iterator_tuple < T0, T1, T2, T3, T4, T5, T6, T7, T8 > ( lhs ) -=
               rhs;
    }
    template < class T0, class T1, class T2, class T3, class T4, class T5,
    class T6, class T7, class T8 >
    iterator_tuple < T0, T1, T2, T3, T4, T5,
    T6, T7, T8 > ::difference_type operator-( const iterator_tuple < T0, T1,
        T2, T3, T4, T5, T6, T7,
        T8 > &lhs,
        const iterator_tuple < T0, T1,
        T2, T3, T4, T5, T6, T7,
        T8 > &rhs )
    {
      return iterator_tuple < T0, T1, T2, T3, T4, T5, T6, T7,
             T8 > ::difference_type( lhs.m0 - rhs.m0, lhs.m1 - rhs.m1,
                                     lhs.m2 - rhs.m2, lhs.m3 - rhs.m3,
                                     lhs.m4 - rhs.m4, lhs.m5 - rhs.m5,
                                     lhs.m6 - rhs.m6, lhs.m7 - rhs.m7,
                                     lhs.m8 - rhs.m8 );
    }



    template < class T, int N >
    struct n_fold_iterator_tuple
      {}
    ;
    template < class T >
    struct n_fold_iterator_tuple <T, 0 >
    {
      typedef iterator_tuple <> type;
      static type make( const T & arg )
      {
        return type( );
      }
    };
    template < class T >
    struct n_fold_iterator_tuple <T, 1 >
    {
      typedef iterator_tuple < T > type;
      static type make( const T & arg )
      {
        return type( arg );
      }
    };
    template < class T >
    struct n_fold_iterator_tuple <T, 2 >
    {
      typedef iterator_tuple < T, T > type;
      static type make( const T & arg )
      {
        return type( arg, arg );
      }
    };
    template < class T >
    struct n_fold_iterator_tuple <T, 3 >
    {
      typedef iterator_tuple < T, T, T > type;
      static type make( const T & arg )
      {
        return type( arg, arg, arg );
      }
    };
    template < class T >
    struct n_fold_iterator_tuple <T, 4 >
    {
      typedef iterator_tuple < T, T, T, T > type;
      static type make( const T & arg )
      {
        return type( arg, arg, arg, arg );
      }
    };
    template < class T >
    struct n_fold_iterator_tuple <T, 5 >
    {
      typedef iterator_tuple < T, T, T, T, T > type;
      static type make( const T & arg )
      {
        return type( arg, arg, arg, arg, arg );
      }
    };
    template < class T >
    struct n_fold_iterator_tuple <T, 6 >
    {
      typedef iterator_tuple < T, T, T, T, T, T > type;
      static type make( const T & arg )
      {
        return type( arg, arg, arg, arg, arg, arg );
      }
    };
    template < class T >
    struct n_fold_iterator_tuple <T, 7 >
    {
      typedef iterator_tuple < T, T, T, T, T, T, T > type;
      static type make( const T & arg )
      {
        return type( arg, arg, arg, arg, arg, arg, arg );
      }
    };
    template < class T >
    struct n_fold_iterator_tuple <T, 8 >
    {
      typedef iterator_tuple < T, T, T, T, T, T, T, T > type;
      static type make( const T & arg )
      {
        return type( arg, arg, arg, arg, arg, arg, arg, arg );
      }
    };
    template < class T >
    struct n_fold_iterator_tuple <T, 9 >
    {
      typedef iterator_tuple < T, T, T, T, T, T, T, T, T > type;
      static type make( const T & arg )
      {
        return type( arg, arg, arg, arg, arg, arg, arg, arg, arg );
      }
    };
  }                             // namespace tupple
}                               // namespace boost


#endif
