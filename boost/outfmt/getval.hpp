// (C) Copyright 2003: Reece H. Dunn

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
      template< int n, typename T1, typename T2 >
      typename detail::selector< n, T1, T2 >::type
                                                 getval( const std::pair< T1, T2 > & p )
      {
         return( detail::selector< n, T1, T2 >::value( p.first, p.second ));
      }

      template< int n, typename T1, typename T2 >
      typename detail::selector< n, T1, T2 >::type
                                                 getval( const boost::compressed_pair< T1, T2 > & cp )
      {
         return( detail::selector< n, T1, T2 >::value( cp.first(), cp.second()));
      }

      template< int n, typename T >
      typename detail::selector< n, T >::type
                                                 getval( const std::complex< T > & c )
      {
         return( detail::selector< n, T >::value( c.real(), c.imag()));
      }

#     if !defined(BOOST_IOFM_NO_LIB_INTERVAL)
         template< int n, typename T, class Traits >
         typename detail::selector< n, T >::type
                                                 getval( const boost::numeric::interval< T, Traits > & i )
         {
            return( detail::selector< n, T >::value( i.lower(), i.upper()));
         }
#     endif
#     if !defined(BOOST_IOFM_NO_LIB_RATIONAL)
         template< int n, typename T >
         typename detail::selector< n, T >::type
                                                 getval( const boost::rational< T > & r )
         {
            return( detail::selector< n, T >::value( r.numerator(), r.denominator()));
         }
#     endif

#     if !defined(BOOST_IOFM_NO_LIB_QUATERNION)
         template< int n, typename T >
         typename detail::selector< n, T >::type
                                                 getval( const boost::math::quaternion< T > & h )
         {
            return( detail::selector< n, T >::value
            (
               h.R_component_1(), h.R_component_2(),
               h.R_component_3(), h.R_component_4()
            ));
         }
#     endif

#     if !defined(BOOST_IOFM_NO_LIB_OCTONION)
         template< int n, typename T >
         typename detail::selector< n, T >::type
                                                 getval( const boost::math::octonion< T > & o )
         {
            return( detail::selector< n, T >::value
            (
               o.R_component_1(), o.R_component_2(), o.R_component_3(), o.R_component_4(),
               o.R_component_5(), o.R_component_6(), o.R_component_7(), o.R_component_8()
            ));
         }
#     endif
   }}
#endif
