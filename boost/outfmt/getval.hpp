// (C) Copyright 2003: Reece H. Dunn

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#ifndef BOOST_IOFM_GETVAL_HPP
#define BOOST_IOFM_GETVAL_HPP
#  include <boost/outfmt/detail/selector.hpp>

#  include <utility>                             // std::pair
#  include <complex>                             // std::complex
#  include <boost/compressed_pair.hpp>           // boost::compressed_pair
#  if !defined(BOOST_IOFM_NO_LIB_INTERVAL)
#     include <boost/numeric/interval/interval.hpp> // boost::numeric::interval
#  endif
#  if !defined(BOOST_IOFM_NO_LIB_RATIONAL)
#     include <boost/rational.hpp>               // boost::rational
#  endif

#  if !defined(BOOST_IOFM_NO_LIB_QUATERNION)
#     include <boost/math/quaternion.hpp>        // boost::math::quaternion
#  endif

#  if !defined(BOOST_IOFM_NO_LIB_OCTONION)
#     include <boost/math/octonion.hpp>          // boost::math::octonion
#  endif

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

      // boost::compressed_pair

      BOOST_IO_NARY_PARAM2( boost::compressed_pair, seperable_pair )

      template< int n, typename T1, typename T2 >
      typename detail::selector< n, T1, T2 >::type
                                                 getval( const boost::compressed_pair< T1, T2 > & cp )
      {
         return( detail::selector< n, T1, T2 >::value( cp.first(), cp.second()));
      }

      template< int n, typename T1, typename T2 >
      typename detail::selector< n, T1, T2 >::ref_type
                                                 refval( boost::compressed_pair< T1, T2 > & p )
      {
         return( detail::selector< n, T1, T2 >::ref( p.first(), p.second()));
      }

      // std::complex

      BOOST_IO_NARY_PARAM1( std::complex, inseperable_pair< T > )

      template< int n, typename T >
      T getval( const std::complex< T > & c )
      {
         return( detail::selector< n, T >::value( c.real(), c.imag()));
      }

      template< typename T >
      void assignval( std::complex< T > & c, const T & a, const T & b )
      {
         c = std::complex< T >( a, b );
      }

      // boost::numeric::interval

#     if !defined(BOOST_IOFM_NO_LIB_INTERVAL)
         BOOST_IO_NARY_PARAM2( boost::numeric::interval, inseperable_pair< T > )

         template< int n, typename T, class Traits >
         T getval( const boost::numeric::interval< T, Traits > & i )
         {
            return( detail::selector< n, T >::value( i.lower(), i.upper()));
         }

         template< typename T, class Traits >
         void assignval( boost::numeric::interval< T, Traits > & i, const T & a, const T & b )
         {
            i = boost::numeric::interval< T, Traits >( a, b );
         }
#     endif

      // boost::rational

#     if !defined(BOOST_IOFM_NO_LIB_RATIONAL)
         BOOST_IO_NARY_PARAM1( boost::rational, inseperable_pair< T > )

         template< int n, typename T >
         T getval( const boost::rational< T > & r )
         {
            return( detail::selector< n, T >::value( r.numerator(), r.denominator()));
         }

         template< typename T >
         void assignval( boost::rational< T > & r, const T & a, const T & b )
         {
            r = boost::rational< T >( a, b );
         }
#     endif

      // boost::math::quaternion

#     if !defined(BOOST_IOFM_NO_LIB_QUATERNION)
         BOOST_IO_NARY_PARAM1( boost::math::quaternion, nary4_type< T > )

         template< int n, typename T >
         T getval( const boost::math::quaternion< T > & h )
         {
            return( detail::selector< n, T >::value
            (
               h.R_component_1(), h.R_component_2(),
               h.R_component_3(), h.R_component_4()
            ));
         }

         template< typename T >
         void assignval( boost::math::quaternion< T > & q, const T & a, const T & b, const T & c, const T & d )
         {
            q = boost::math::quaternion< T >( a, b, c, d );
         }
#     endif

      // boost::math::octonion

#     if !defined(BOOST_IOFM_NO_LIB_OCTONION)
         BOOST_IO_NARY_PARAM1( boost::math::octonion, nary8_type< T > )

         template< int n, typename T >
         T getval( const boost::math::octonion< T > & o )
         {
            return( detail::selector< n, T >::value
            (
               o.R_component_1(), o.R_component_2(), o.R_component_3(), o.R_component_4(),
               o.R_component_5(), o.R_component_6(), o.R_component_7(), o.R_component_8()
            ));
         }

         template< typename T >
         void assignval( boost::math::octonion< T > & o, const T & a, const T & b, const T & c, const T & d, const T & e, const T & f, const T & g, const T & h )
         {
            o = boost::math::octonion< T >( a, b, c, d, e, f, g, h );
         }
#     endif
   }}
#endif
