// (C) Copyright 2003-2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <iostream> // std::cout
#include <string>   // std::string
#include <boost/outfmt/stl.hpp> // output for STL containers

int main()
{
   // std::vector

   std::vector< int >                  v( 4 );
   v[ 0 ] = 3;
   v[ 1 ] = 4;
   v[ 2 ] = 5;
   v[ 3 ] = 6;

   // std::list

   std::list< char >                   l;
   l.push_back( 'A' );
   l.push_back( 'B' );
   l.push_back( 'C' );

#  if defined(BOOST_HAS_SLIST)
      // std::slist

      BOOST_IOFM_STDEXT::slist< char > sl;
      sl.push_front( 'a' );
      sl.push_front( 'b' );
      sl.push_front( 'c' );
#  endif

   // std::deque

   std::deque< bool >                  dq;
   dq.push_back( true  );
   dq.push_back( true  );
   dq.push_back( false );
   dq.push_back( true  );
   dq.push_back( false );

   // std::map

   std::map< std::string, int >        m;
   m[ "hello" ]      = 5;
   m[ "world" ]      = 5;
   m[ "STL rules!" ] = 11;

   // std::multimap

   // JDT: VC6 can't handle 'make_pair' here.
   std::multimap< std::string, std::string >
                                       mm;

   // JDT: VC6 can't handle 'make_pair' here.
   typedef std::pair< std::string, std::string > string_pair;
   mm.insert( string_pair( "Portishead", "Dummy" ));
   mm.insert( string_pair( "Portishead", "Roseland NYC" ));
   mm.insert( string_pair( "Portishead", "Portishead" ));
   mm.insert( string_pair( "Wonderwall", "Witchcraft" ));
   mm.insert( string_pair( "Tina Arena", "Don't Ask" ));
   mm.insert( string_pair( "Tina Arena", "In Deep" ));
   mm.insert( string_pair( "Tina Arena", "Just Me" ));

   // std::set

   std::set< std::string >             s;
   s.insert( "Portishead" );
   s.insert( "Tricky" );
   s.insert( "GlasHaus" );
   s.insert( "Xavier Naidoo" );
   s.insert( "Marc Anthony" );
   s.insert( "Beth Orton" );
   s.insert( "Melissa Etheridge" );

   // std::multiset

   std::multiset< std::string >        ms;
   ms.insert( "A" );
   ms.insert( "B" );
   ms.insert( "C" );
   ms.insert( "A" );
   ms.insert( "D" );
   ms.insert( "E" );

   // std::pair

   std::pair< int, char >              p( 5, 'b' );

   // STL container examples

   std::cout << "vector< int > = " << v << '\n';

   // [results]: vector< int > = [ 3, 4, 5, 6 ]

   std::cout << "list< char > = " << l << '\n';

   // [results]: list< char > = [ A, B, C ]

#  if defined(BOOST_HAS_SLIST)
      std::cout << "slist< char > = " << sl << '\n';

      // [results]: slist< char > = [ c, b, a ]
#  endif

   std::cout << "deque< bool > = " << dq
             << '\n' << '\n';

   // [results]: deque< bool > = [ 1, 1, 0, 1, 0 ]

   std::cout << "map< string, int > = " << m << '\n';

   // [results]: map< string, int > = [ ( "STL rules!", 11 ), ( "hello", 5 ), ( "world", 5 ) ]

   std::cout << "multimap< string, int > = " << mm
             << '\n' << '\n';

   // [note]: Edited for clarity:

   // [results]: multimap< string, int > =
   //    [ ( "Portishead", "Dummy" ), ( "Portishead", "Roseland NYC" ),
   //      ( "Portishead", "Portishead" ), ( "Tina Arena", "Don't Ask" ), ( "Tina Arena", "In Deep" ),
   //      ( "Tina Arena", "Just Me" ), ( "Wonderwall", "Witchcraft" ) ]

   std::cout << "set< string > = " << s << '\n';

   // [results]: set< string > =
   //    [ "Beth Orton", "GlasHaus", "Marc Anthony", "Melissa Etheridge", "Portishead", "Tricky",
   //    "Xavier Naidoo" ]

   std::cout << "multiset< string > = " << ms
             << '\n' << '\n';

   // [results]: multiset< string > = [ A, A, B, C, D, E ]

   std::cout << "pair< int, char > = " << p << '\n';

   // [results]: pair< int, char > = ( 5, b )

   return( 0 );
}
