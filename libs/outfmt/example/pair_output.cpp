// (C) Copyright 2003: Reece H. Dunn

#include <iostream>  // std::cout
#include <string>    // std::string
#include <list>      // std::list
#include <map>       // std::map

#include <boost/outfmt/formatlist.hpp>
#include <boost/outfmt/formatob.hpp>

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

   boost::io::pair_output< char * >    pairfmt1; // [format]: ( a, b )
   
   boost::io::pair_output< char * >    pairfmt2; // [format]: ( a | b )
   pairfmt2.format( " | " );

   boost::io::pair_output< char * >    pairfmt3; // [format]: a ==> b
   pairfmt3.format( "", "", " ==> " );

   // std::pair output examples

   std::cout << "pair< string, int > = "
             << boost::io::formatobout( p, pairfmt3 )
             << '\n';

   // [results]: pair< string, int > = Hello ==> 10

   std::cout << "pair< string, int > = "
             << boost::io::formatobout
                (
                   p,
                   boost::io::pairfmt().format( "; " )
                )
             << '\n' << '\n';

   // [results]: pair< string, int > = ( Hello; 10 )

   // std::map output examples

   std::cout << "map< string, string > = "
             << boost::io::formatlistout( m, pairfmt1 )
             << '\n' << '\n';

   // [results]: map< string, string > = [ ( Abscheid Nehmen, Farewell ), ( Engel, Angel ), ( Mas Alla, Beyond ), ( Wenn Das Liebe Ist, If This Is Love ) ]

   std::cout << "map< string, string > = "
             << boost::io::formatlistout( m, pairfmt1 ) // [note]: pairfmt1 is reused
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
             << boost::io::formatlistout
                (
                   m,
                   boost::io::pairfmt() // inlined equivalent to pairfmt1
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
             << boost::io::formatlistout( m, pairfmt2 )
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
             << boost::io::formatlistout( m, pairfmt3 )
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
             << boost::io::formatlist( dl ) // use the build in complex number formatting
             << '\n';

   // [results]: list< complex< double > > = [ (3.5,9.7), (7.9,0), (1,2.5) ]

   std::cout << "list< complex< double > > = "
             << boost::io::formatlistout( dl, pairfmt1 ) // custom format - more control on spacing
             << '\n';

   // [results]: list< complex< double > > = [ ( 3.5, 9.7 ), ( 7.9, 0 ), ( 1, 2.5 ) ]

   std::cout << "list< complex< double > > = "
             << boost::io::formatlistout( dl, pairfmt2 ) // custom fomat - a more radical change!
             << '\n' << '\n';

   // [results]: list< complex< double > > = [ ( 3.5 | 9.7 ), ( 7.9 | 0 ), ( 1 | 2.5 ) ]

   return( 0 );
}
