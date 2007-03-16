// (C) Copyright 2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef OUTFMT_TEST_HDR
#define OUTFMT_TEST_HDR
#  include <boost/test/minimal.hpp>
#  include <sstream>
#  include <algorithm>

#  define IOFMT_WRITE( wr, res ) \
      ss.str( "" );              \
      ss << wr;                  \
      BOOST_TEST( ss.str() == res );

#  define IOFMT_READ( fmt, rd, cmp, res ) \
      ss.str( res );                      \
      ss >> fmt;                          \
      BOOST_TEST( std::equal( rd.begin(), rd.end(), cmp.begin()));

#  define STLIO_WRITE( wr, res )\
      ss.str( "" );              \
      ss << wr;                  \
      BOOST_TEST( ss.str() == res );

#  define STLIO_READ( rd, cmp, res )\
      ss.str( res );                 \
      ss >> rd;                      \
      BOOST_TEST( std::equal( rd.begin(), rd.end(), cmp.begin()));

#  define PAIR_READ( rd, cmp, res )\
      ss.str( res );                \
      ss >> rd;                     \
      BOOST_TEST( rd == cmp );
#endif
