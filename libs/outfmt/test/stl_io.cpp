// (C) Copyright 2004: Reece H. Dunn

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

#include "test_engine.hpp"

int main()
{
   test_engine                         test( "boost/io/stl_io" );

   // std::vector
   {
      std::vector< int >               v( 3 );
      v[ 0 ] = 3;
      v[ 1 ] = 6;
      v[ 2 ] = 9;

      test_stlio( test, v, "[ 3, 6, 9 ]", "std::vector" );
   }

   // std::list
   {
      std::list< char >                l;
      l.push_back( 'A' );
      l.push_back( 'B' );
      l.push_back( 'C' );

      test_stlio( test, l, "[ A, B, C ]", "std::list" );
   }

   // std::deque
   {
      std::deque< char >               dq;
      dq.push_back( 'A' );
      dq.push_back( 'B' );
      dq.push_back( 'C' );

      test_stlio( test, dq, "[ A, B, C ]", "std::deque" );
   }

   // std::map
   {
      std::map< char, int >            m;
      m[ 'A' ] =  3;
      m[ 'L' ] =  7;
      m[ 'S' ] = -5;

      test_stlio( test, m, "[ ( A, 3 ), ( L, 7 ), ( S, -5 ) ]", "std::map" );
   }

   // std::multimap
   {
      std::multimap< char, int >       mm;
      mm.insert( std::pair< char, int >( 'A',  3 ));
      mm.insert( std::pair< char, int >( 'L',  7 ));
      mm.insert( std::pair< char, int >( 'S', -5 ));

      test_stlio( test, mm, "[ ( A, 3 ), ( L, 7 ), ( S, -5 ) ]", "std::multimap" );
   }

   // std::set
   {
      std::set< char >                 s;
      s.insert( 'A' );
      s.insert( 'B' );
      s.insert( 'C' );
      s.insert( 'A' );
      s.insert( 'D' );
      s.insert( 'E' );

      test_stlio( test, s, "[ A, B, C, D, E ]", "std::set" );
   }

   // std::multiset
   {
      std::multiset< char >            ms;
      ms.insert( 'A' );
      ms.insert( 'B' );
      ms.insert( 'C' );
      ms.insert( 'A' );
      ms.insert( 'D' );
      ms.insert( 'E' );

      test_stlio( test, ms, "[ A, A, B, C, D, E ]", "std::multiset" );
   }

   // std::pair
   {
      std::pair< char, float >         p( 'A', 2.759f );
      test_stlio( test, p, "( A, 2.759 )", "std::pair" );
   }

   // std::string
   {
      std::pair< std::string, std::string >
                                       p( "Abschied Nehmen", "Farewell" );
      test_stlio( test, p, "( \"Abschied Nehmen\", \"Farewell\" )", "std::string" );
   }

   // std::string (character escaping)
   {
      // [note]: results shown in escaped form

      std::pair< std::string, std::string >
                                       p( "message", "quote: \"Hello\\ World!\"" );
      test_stlio( test, p, "( \"message\", \"quote: \\\"Hello\\\\ World!\\\"\" )", "std::string (character escaping)" );
   }

#  if defined(BOOST_IOFM_HASH_CONTAINERS)
      // hash_map
      {
         BOOST_IOFM_STDEXT::hash_map< char, int >
                                       hm;
         hm[ 'A' ] = 3;
         hm[ 'L' ] = 7;
         hm[ 'S' ] = -5;

         test_stlio( test, hm, "[ ( A, 3 ), ( L, 7 ), ( S, -5 ) ]", "hash_map" );
      }

      // hash_multimap
      {
         BOOST_IOFM_STDEXT::hash_multimap< char, int >
                                       hmm;
         hmm.insert( std::pair< char, int >( 'A',  3 ));
         hmm.insert( std::pair< char, int >( 'L',  7 ));
         hmm.insert( std::pair< char, int >( 'S', -5 ));

         test_stlio( test, hmm, "[ ( A, 3 ), ( L, 7 ), ( S, -5 ) ]", "hash_multimap" );
      }

      // hash_set
      {
         BOOST_IOFM_STDEXT::hash_set< char >
                                       hs;
         hs.insert( 'A' );
         hs.insert( 'B' );
         hs.insert( 'C' );
         hs.insert( 'A' );
         hs.insert( 'D' );
         hs.insert( 'E' );

         // [hack]: is there a better way to test this?
#        if defined(BOOST_DINKUMWARE_STDLIB) && (BOOST_DINKUMWARE_STDLIB == 313)
            test_stlio( test, hs, "[ A, C, E, B, D ]", "hash_set" );
#        elif defined(BOOST_DINKUMWARE_STDLIB) && (BOOST_DINKUMWARE_STDLIB < 313)
            test_stlio( test, hs, "[ B, D, A, C, E ]", "hash_set" );
#        else
            test_stlio( test, hs, "[ A, B, C, D, E ]", "hash_set" );
#        endif
      }

      // hash_multiset
      {
         BOOST_IOFM_STDEXT::hash_multiset< char >
                                       hms;
         hms.insert( 'A' );
         hms.insert( 'B' );
         hms.insert( 'C' );
         hms.insert( 'A' );
         hms.insert( 'D' );
         hms.insert( 'E' );

         // [hack]: is there a better way to test this?
#        if defined(BOOST_DINKUMWARE_STDLIB) && (BOOST_DINKUMWARE_STDLIB == 313)
            test_stlio( test, hms, "[ A, A, C, E, B, D ]", "hash_multiset" );
#        elif defined(BOOST_DINKUMWARE_STDLIB) && (BOOST_DINKUMWARE_STDLIB < 313)
            test_stlio( test, hms, "[ B, D, A, A, C, E ]", "hash_multiset" );
#        else
            test_stlio( test, hms, "[ A, A, B, C, D, E ]", "hash_multiset" );
#        endif
      }
#  else
      test.unsupported( "hash_map",      "library does not support 'hash_map' extension class" );
      test.unsupported( "hash_multimap", "library does not support 'hash_multimap' extension class" );
      test.unsupported( "hash_set",      "library does not support 'hash_set' extension class" );
      test.unsupported( "hash_multiset", "library does not support 'hash_multiset' extension class" );
#  endif

#  if defined(BOOST_HAS_SLIST) // slist
   {
      BOOST_IOFM_STDEXT::slist< char >
                                       sl;
      sl.push_front( 'C' );
      sl.push_front( 'B' );
      sl.push_front( 'A' );

      test_stlio( test, sl, "[ A, B, C ]", "slist" );
   }
#  else
      test.unsupported( "slist", "library does not support 'slist' extension class" );
#  endif

   return( 0 );
}
