// (C) Copyright 2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/outfmt/stl/vector.hpp>     // std::vector
#include <boost/outfmt/stl/list.hpp>       // std::list
#include <boost/outfmt/stl/deque.hpp>      // std::deque
#include <boost/outfmt/stl/map.hpp>        // std::map; std::multimap
#include <boost/outfmt/stl/set.hpp>        // std::set; std::multiset
#include <boost/outfmt/stl/pair.hpp>       // std::pair

#if defined(BOOST_IOFM_HASH_CONTAINERS)
#  include <boost/outfmt/stl/hash_map.hpp> // stdext::hash_map; stdext::hash_multimap
#  include <boost/outfmt/stl/hash_set.hpp> // stdext::hash_set; stdext::hash_multiset
#endif

#if defined(BOOST_HAS_SLIST)
#   include <boost/outfmt/stl/slist.hpp>   // stdext::slist
#endif

#include "test.hpp"

int test_main( int, char *[] )
{
   std::stringstream                   ss;

   // std::vector

   {
      const char *                     res = "[ 3, 6, 9 ]";
      std::vector< int >               vr( 3 );
      std::vector< int >               vw( 3 );
      vw[ 0 ] = 3;
      vw[ 1 ] = 6;
      vw[ 2 ] = 9;

      STLIO_READ(  vr, vw, res );
      STLIO_WRITE( vw, res );
   }

   // std::list

   {
      const char *                     res = "[ A, B, C ]";
      std::list< char >                lr;
      std::list< char >                lw;
      lw.push_back( 'A' );
      lw.push_back( 'B' );
      lw.push_back( 'C' );

      STLIO_READ(  lr, lw, res );
      STLIO_WRITE( lw, res );
   }


   // std::deque

   {
      const char *                     res = "[ A, B, C ]";
      std::deque< char >               dqr;
      std::deque< char >               dqw;
      dqw.push_back( 'A' );
      dqw.push_back( 'B' );
      dqw.push_back( 'C' );

      STLIO_READ(  dqr, dqw, res );
      STLIO_WRITE( dqw, res );
   }

   // std::map

   {
      const char *                     res = "[ ( A, 3 ), ( L, 7 ), ( S, -5 ) ]";
      std::map< char, int >            mr;
      std::map< char, int >            mw;
      mw[ 'A' ] =  3;
      mw[ 'L' ] =  7;
      mw[ 'S' ] = -5;

      STLIO_READ(  mr, mw, res );
      STLIO_WRITE( mw, res );
   }


   // std::multimap

   {
      const char *                     res = "[ ( A, 3 ), ( L, 7 ), ( S, -5 ) ]";
      std::multimap< char, int >       mmr;
      std::multimap< char, int >       mmw;
      mmw.insert( std::pair< char, int >( 'A',  3 ));
      mmw.insert( std::pair< char, int >( 'L',  7 ));
      mmw.insert( std::pair< char, int >( 'S', -5 ));

      STLIO_READ(  mmr, mmw, res );
      STLIO_WRITE( mmw, res );
   }

   // std::set

   {
      const char *                     res = "[ A, B, C, D, E ]";
      std::set< char >                 sr;
      std::set< char >                 sw;
      sw.insert( 'A' );
      sw.insert( 'B' );
      sw.insert( 'C' );
      sw.insert( 'A' );
      sw.insert( 'D' );
      sw.insert( 'E' );

      STLIO_READ(  sr, sw, res );
      STLIO_WRITE( sw, res );
   }

   // std::multiset

   {
      const char *                     res = "[ A, A, B, C, D, E ]";
      std::multiset< char >            msr;
      std::multiset< char >            msw;
      msw.insert( 'A' );
      msw.insert( 'B' );
      msw.insert( 'C' );
      msw.insert( 'A' );
      msw.insert( 'D' );
      msw.insert( 'E' );

      STLIO_READ(  msr, msw, res );
      STLIO_WRITE( msw, res );
   }

   // std::pair

   {
      const char *                     res = "( A, 2.759 )";
      std::pair< char, float >         pr( '\0', 0.0f );
      std::pair< char, float >         pw( 'A',  2.759f );

      PAIR_READ(   pr, pw, res );
      STLIO_WRITE( pw, res );
   }

   // std::string

   {
      const char *                     res = "( \"Abschied Nehmen\", \"Farewell\" )";
      std::pair< std::string, std::string > pr;
      std::pair< std::string, std::string > pw( "Abschied Nehmen", "Farewell" );

      PAIR_READ(   pr, pw, res );
      STLIO_WRITE( pw, res );
   }

   // std::string (character escaping)

   {
      const char *                     res = "( \"message\", \"quote: \\\"Hello\\\\ World!\\\"\" )";
      std::pair< std::string, std::string > pr;
      std::pair< std::string, std::string > pw( "message", "quote: \"Hello\\ World!\"" );

      PAIR_READ(   pr, pw, res );
      STLIO_WRITE( pw, res );
   }

#  if defined(BOOST_IOFM_HASH_CONTAINERS)
      // hash_map

      {
         const char *                  res = "[ ( A, 3 ), ( L, 7 ), ( S, -5 ) ]";
         BOOST_IOFM_STDEXT::hash_map< char, int > hmr;
         BOOST_IOFM_STDEXT::hash_map< char, int > hmw;
         hmw[ 'A' ] = 3;
         hmw[ 'L' ] = 7;
         hmw[ 'S' ] = -5;

         STLIO_READ(  hmr, hmw, res );
         STLIO_WRITE( hmw, res );
      }

      // hash_multimap

      {
         const char *                  res = "[ ( A, 3 ), ( L, 7 ), ( S, -5 ) ]";
         BOOST_IOFM_STDEXT::hash_multimap< char, int > hmmr;
         BOOST_IOFM_STDEXT::hash_multimap< char, int > hmmw;
         hmmw.insert( std::pair< char, int >( 'A',  3 ));
         hmmw.insert( std::pair< char, int >( 'L',  7 ));
         hmmw.insert( std::pair< char, int >( 'S', -5 ));

         STLIO_READ(  hmmr, hmmw, res );
         STLIO_WRITE( hmmw, res );
      }

      // hash_set

      {
         const char *                  res = // [hack]: is there a better way to test this?
#        if defined(BOOST_DINKUMWARE_STDLIB) && (BOOST_DINKUMWARE_STDLIB >= 313)
            "[ A, C, E, B, D ]";
#        elif defined(BOOST_DINKUMWARE_STDLIB) && (BOOST_DINKUMWARE_STDLIB < 313)
            "[ B, D, A, C, E ]";
#        else
            "[ A, B, C, D, E ]";
#        endif
         BOOST_IOFM_STDEXT::hash_set< char > hsr;
         BOOST_IOFM_STDEXT::hash_set< char > hsw;
         hsw.insert( 'A' );
         hsw.insert( 'B' );
         hsw.insert( 'C' );
         hsw.insert( 'A' );
         hsw.insert( 'D' );
         hsw.insert( 'E' );

         STLIO_READ(  hsr, hsw, res );
         STLIO_WRITE( hsw, res );
      }

      // hash_multiset

      {
         const char *                  res = // [hack]: is there a better way to test this?
#        if defined(BOOST_DINKUMWARE_STDLIB) && (BOOST_DINKUMWARE_STDLIB >= 313)
            "[ A, A, C, E, B, D ]";
#        elif defined(BOOST_DINKUMWARE_STDLIB) && (BOOST_DINKUMWARE_STDLIB < 313)
            "[ B, D, A, A, C, E ]";
#        else
            "[ A, A, B, C, D, E ]";
#        endif
         BOOST_IOFM_STDEXT::hash_multiset< char > hmsr;
         BOOST_IOFM_STDEXT::hash_multiset< char > hmsw;
         hmsw.insert( 'A' );
         hmsw.insert( 'B' );
         hmsw.insert( 'C' );
         hmsw.insert( 'A' );
         hmsw.insert( 'D' );
         hmsw.insert( 'E' );

         STLIO_READ(  hmsr, hmsw, res );
         STLIO_WRITE( hmsw, res );
      }
#  endif

#  if defined(BOOST_HAS_SLIST) // slist
   {
      const char *                     res = "[ 3, 6, 9 ]";
      BOOST_IOFM_STDEXT::slist< char > slr;
      BOOST_IOFM_STDEXT::slist< char > slw;
      slw.push_front( 'C' );
      slw.push_front( 'B' );
      slw.push_front( 'A' );

      STLIO_READ(  slr, slw, res );
      STLIO_WRITE( slw, res );
   }
#  endif

   return( 0 );
}
