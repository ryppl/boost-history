// (C) Copyright 2004: Reece H. Dunn

#ifndef BOOST_IOFM_BOOST_CompressedPair_HPP
#define BOOST_IOFM_BOOST_CompressedPair_HPP
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

#  include <boost/compressed_pair.hpp>

   namespace boost { namespace io
   {
      // type deduction:

      namespace detail
      {
         BOOST_IO_CLASSIFY_TYPE_2( boost::compressed_pair, boost::io::pair_type );
      }

      // naryval:

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
   }}
#endif
