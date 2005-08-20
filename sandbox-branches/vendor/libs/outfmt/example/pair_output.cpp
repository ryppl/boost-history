// (C) Copyright 2003-2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>                       // std::cout
#include <string>                         // std::string

#include <boost/outfmt/formatob.hpp>
#include <boost/outfmt/stl/complex.hpp>   // std::complex
#include <boost/outfmt/stl/list.hpp>      // std::list
#include <boost/outfmt/stl/map.hpp>       // std::map

int main()
{
   // std::map - used for the std::pair examples

   std::map< std::string, std::string >
                                       m;
   m[ "Abscheid Nehmen"    ] = "Farewell";
   m[ "Engel"              ] = "Angel";
   m[ "Mas Alla"           ] = "Beyond";
   m[ "Wenn Das Liebe Ist" ] = "If This Is Love";

   // std::list< std::complex > - used for the std::complex examples

   std::list< std::complex< double > > dl;
   dl.push_front( std::complex< double >( 1.0, 2.5 ));
   dl.push_front( std::complex< double >( 7.9, 0.0 ));
   dl.push_front( std::complex< double >( 3.5, 9.7 ));

   // std::pair< std::string, int >

   // JDT: VC6 can't handle 'make_pair( "Hello", 10 )'
   std::pair< std::string, int >       p = std::pair< std::string, int >( "Hello", 10 );

   // formatter configurations

   boost::io::pairfmt_t< const char * > pairfmt1; // [format]: ( a, b )

   boost::io::pairfmt_t< const char * > pairfmt2; // [format]: ( a | b )
   pairfmt2.format( " | " );

   boost::io::pairfmt_t< const char * > pairfmt3; // [format]: a ==> b
   pairfmt3.format( "", "", " ==> " );

   // std::pair output examples

   std::cout << "pair< string, int > = "
             << boost::io::formatob( p, pairfmt3 )
             << '\n';

   // [results]: pair< string, int > = Hello ==> 10

   std::cout << "pair< string, int > = "
             << boost::io::formatob
                (
                   p,
                   boost::io::pairfmt().format( "; " )
                )
             << '\n' << '\n';

   // [results]: pair< string, int > = ( Hello; 10 )

   // std::map output examples

   std::cout << "map< string, string > = "
             << boost::io::formatob( m, boost::io::containerfmt( pairfmt1 ))
             << '\n' << '\n';

   // [results]: map< string, string > = [ ( Abscheid Nehmen, Farewell ), ( Engel, Angel ), ( Mas Alla, Beyond ), ( Wenn Das Liebe Ist, If This Is Love ) ]

   std::cout << "map< string, string > = "
             << boost::io::formatob( m, boost::io::containerfmt( pairfmt1 )) // [note]: pairfmt1 is reused
                .format( "\n", "", "\n" )
             << '\n' << '\n';

   /* [results]:

      map< string, string > =
      ( Abscheid Nehmen, Farewell )
      ( Engel, Angel )
      ( Mas Alla, Beyond )
      ( Wenn Das Liebe Ist, If This Is Love )
   */

   std::cout << "map< string, string > = "
             << boost::io::formatob
                (
                   m,
                   boost::io::containerfmt( boost::io::pairfmt()) // inlined equivalent to pairfmt1
                ).format( "\n", "", "\n" )
             << '\n' << '\n';

   /* [results]:

      map< string, string > =
      ( Abscheid Nehmen, Farewell )
      ( Engel, Angel )
      ( Mas Alla, Beyond )
      ( Wenn Das Liebe Ist, If This Is Love )
   */

   // changing the way std::pair is outputted

   std::cout << "map< string, string > = "
             << boost::io::formatob( m, boost::io::containerfmt( pairfmt2 ))
                .format( "\n==> ", "", "\n==> " )
             << '\n' << '\n';

   /* [results]:

      map< string, string > =
      ==> ( Abscheid Nehmen | Farewell )
      ==> ( Engel | Angel )
      ==> ( Mas Alla | Beyond )
      ==> ( Wenn Das Liebe Ist | If This Is Love )
   */

   std::cout << "map< string, string > = "
             << boost::io::formatob( m, boost::io::containerfmt( pairfmt3 ))
                .format( "\n   ", "", "\n   " )
             << '\n' << '\n';

   /* [results]:

      map< string, string > =
         Abscheid Nehmen ==> Farewell
         Engel ==> Angel
         Mas Alla ==> Beyond
         Wenn Das Liebe Ist ==> If This Is Love
   */

   // std::complex output examples

   std::cout << "list< complex< double > > = "
             << boost::io::formatob( dl ) // use the default (deduced) formatting
             << '\n';

   // [results]: list< complex< double > > = [ ( 3.5, 9.7 ), ( 7.9, 0 ), ( 1, 2.5 ) ]

   std::cout << "list< complex< double > > = "
             << boost::io::formatob( dl, boost::io::containerfmt( pairfmt1 ))
                // custom format - control spacing on complex type
             << '\n';

   // [results]: list< complex< double > > = [ ( 3.5, 9.7 ), ( 7.9, 0 ), ( 1, 2.5 ) ]

   std::cout << "list< complex< double > > = "
             << boost::io::formatob( dl, boost::io::containerfmt( pairfmt2 ))
                // custom fomat - a more radical change!
             << '\n' << '\n';

   // [results]: list< complex< double > > = [ ( 3.5 | 9.7 ), ( 7.9 | 0 ), ( 1 | 2.5 ) ]

   return( 0 );
}
