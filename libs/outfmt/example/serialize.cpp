// (C) Copyright 2003: Reece H. Dunn

// Examples for text-based serialization

#include <iostream> // std::cout
#include <fstream>  // std::fstream
#include <list>     // std::list

#include <boost/outfmt/formatob.hpp>
#include <boost/outfmt/readob.hpp>

int main()
{
   // [1]: plain text

   // create the output object used for serialization

   boost::io::pair_output< char * >    textio; // default formatting

   // create the data to be serialized
   std::pair< char, int >              p1 = std::pair< char, int >( 'a', 5 );

   // writing to a plain text-based serialization stream
   {
      // open the file to serialize the pair to
      std::ofstream                    out( "doc.txt" );

      // serialize to the stream
      out << boost::io::formatobout( p1, textio ) << '\n';
   }

   // reading data back from a plain text-based serialization stream
   {
      // [note]: the mechanism for reading data in is currently incomplete and experimental:
      //    it is only currently supported for basic_output and pair_output/std::pair.

      std::ifstream                    is( "doc.txt" );
      std::pair< char, int >           p2;

      std::cout << "pair before reading: " << boost::io::formatobout( p2, textio ) << '\n';
      is >> boost::io::readobout( p2, textio );
      std::cout << "pair after reading:  "  << boost::io::formatobout( p2, textio ) << '\n';

      // [output]:
      //    pair before reading: (  , 0 )
      //    pair after reading:  ( a, 5 )
   }

   // [2]: XML -- [note]: XML is a text-based system, and can thus be serialized to

   boost::io::pair_output< char * >    xmlio;
   xmlio.format( "<pairob><item>", "</item></pairob>", "</item><item>" );

   // writing to an XML-based serialization stream
   {
      // open the file to serialize the pair to
      std::ofstream                    out( "doc1.xml" );
//    out << "<?xml version = \"1.0\" encoding = \"UTF-8\"?>\n\n"; // XML signature

      // serialize to the stream
      out << boost::io::formatobout( p1, xmlio ) << '\n';
   }

   // reading data back from a plain text-based serialization stream
   {
      // [note]: the mechanism for reading data in is currently incomplete and experimental:
      //    it is only currently supported for basic_output and pair_output/std::pair.

      std::ifstream                    is( "doc1.xml" );
      std::pair< char, int >           p2;

      std::cout << "pair before reading: " << boost::io::formatobout( p2, textio ) << '\n';
      is >> boost::io::readobout( p2, xmlio );
      std::cout << "pair after reading:  "  << boost::io::formatobout( p2, textio ) << '\n';

      // [output]:
      //    pair before reading: (  , 0 )
      //    pair after reading:  ( a, 5 )
   }

   // [3]: XML -- [note]: use wrapped output to simplify output specification

   boost::io::pair_output
   <
      char *,
      boost::io::wrapped_output< char * >,
      boost::io::wrapped_output< char * >
   >                                   xmlio2 =
      boost::io::pairfmtout
      (
         boost::io::wrappedfmt().format( "<i1>", "</i1>" ),
         boost::io::wrappedfmt().format( "<i2>", "</i2>" )
      ).format( "<pairob>", "</pairob>", "" );

   // writing to an XML-based serialization stream
   {
      // open the file to serialize the pair to
      std::ofstream                    out( "doc2.xml" );
      out << "<?xml version = \"1.0\" encoding = \"UTF-8\"?>\n\n"; // XML signature

      // serialize to the stream
      out << boost::io::formatobout( p1, xmlio2 ) << '\n';
   }

   // reading data back from a plain text-based serialization stream
   {
      // [todo]
      // in >> boost::io::readobject( p1, xmlio2 ); ?
   }

   // [4]: plain text -- more advanced type

   // create the output object used for serialization

   boost::io::pair_output
   <
      char *,
      boost::io::pair_output< char * >
   >                                   textio2;

   // create the data to be serialized
   std::pair< std::complex< float >, int >
                                         pc1 = std::pair< std::complex< float >, int >
                                         (
                                            std::complex< float >( 0.5f, 1.345f ),
                                            5
                                         );

   // writing to a plain text-based serialization stream
   {
      // open the file to serialize the pair to
      std::ofstream                    out( "doc2.txt" );

      // serialize to the stream
      out << boost::io::formatobout( pc1, textio2 ) << '\n';
   }

   // reading data back from a plain text-based serialization stream
   {
      // [note]: the mechanism for reading data in is currently incomplete and experimental:
      //    it is only currently supported for basic_output and pair_output/std::pair.

      std::ifstream                    is( "doc2.txt" );
      std::pair< std::complex< float >, int >
                                       pc2;

      std::cout << "pair before reading: " << boost::io::formatobout( pc2, textio2 ) << '\n';
      is >> boost::io::readobout( pc2, textio2 );
      std::cout << "pair after reading:  "  << boost::io::formatobout( pc2, textio2 ) << '\n';

      // [output]:
      // pair before reading: ( ( 0, 0 ), 0 )
      // pair after reading:  ( ( 0.5, 1.345 ), 5 )
   }
   
   // reading data back using a different (but compatible) data type 
   {
      // [note]: the mechanism for reading data in is currently incomplete and experimental:
      //    it is only currently supported for basic_output and pair_output/std::pair.

      std::ifstream                    is( "doc2.txt" );
      std::pair< std::pair< float, double >, int >
                                       pc2;

      std::cout << "pair before reading: " << boost::io::formatobout( pc2, textio2 ) << '\n';
      is >> boost::io::readobout( pc2, textio2 );
      std::cout << "pair after reading:  "  << boost::io::formatobout( pc2, textio2 ) << '\n';

      // [output]:
      // pair before reading: ( ( 0, 0 ), 0 )
      // pair after reading:  ( ( 0.5, 1.345 ), 5 )
   }

   return( 0 );
}
