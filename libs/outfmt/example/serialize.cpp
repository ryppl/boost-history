// (C) Copyright 2003: Reece H. Dunn

// Examples for text-based serialization

#include <iostream> // std::cout
#include <fstream>  // std::fstream
#include <list>     // std::list

#include <boost/outfmt/formatob.hpp>

int main()
{
   // [note]: the mechanism for reading data in is currently incomplete and experimental:
   //    it is only currently supported for basic_output, pair_output and static_nary_output
   //    formatter types.

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
      out << boost::io::formatob( p1, textio ) << '\n';
   }

   // reading data back from a plain text-based serialization stream
   {
      std::ifstream                    is( "doc.txt" );
      std::pair< char, int >           p2;

      std::cout << "pair before reading: " << boost::io::formatob( p2, textio ) << '\n';
      is >> boost::io::formatob( p2, textio );
      std::cout << "pair after reading:  "  << boost::io::formatob( p2, textio ) << '\n';

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
      // out << "<?xml version = \"1.0\" encoding = \"UTF-8\"?>\n\n"; // XML signature

      // serialize to the stream
      out << boost::io::formatob( p1, xmlio ) << '\n';
   }

   // reading data back from a plain text-based serialization stream
   {
      std::ifstream                    is( "doc1.xml" );
      std::pair< char, int >           p2;

      std::cout << "pair before reading: " << boost::io::formatob( p2, textio ) << '\n';
      is >> boost::io::formatob( p2, xmlio );
      std::cout << "pair after reading:  "  << boost::io::formatob( p2, textio ) << '\n';

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
      boost::io::pairfmt
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
      out << boost::io::formatob( p1, xmlio2 ) << '\n';
   }

   // reading data back from a plain text-based serialization stream
   {
      // [todo]
      // in >> boost::io::readobject( p1, xmlio2 ); ?
   }

   // [4]: plain text -- more advanced types

   // create the output object used for serialization

   boost::io::pair_output
   <
      char *,
      boost::io::pair_output< char * >
   >                                   textio2;

   // create the data to be serialized
   std::pair< std::complex< double >, int >
                                       pc1 = std::pair< std::complex< double >, int >
                                       (
                                          std::complex< double >( 0.5, 1.345 ),
                                          5
                                       );

   // writing to a plain text-based serialization stream
   {
      // open the file to serialize the pair to
      std::ofstream                    out( "doc2.txt" );

      // serialize to the stream
      out << boost::io::formatob( pc1, textio2 ) << '\n';
   }

   // reading data back from a plain text-based serialization stream
   {
      std::ifstream                    is( "doc2.txt" );
      std::pair< std::complex< double >, int >
                                       pc2;

      std::cout << "pair before reading: " << boost::io::formatob( pc2, textio2 ) << '\n';
      is >> boost::io::formatob( pc2, textio2 );
      std::cout << "pair after reading:  "  << boost::io::formatob( pc2, textio2 ) << '\n';

      // [output]:
      // pair before reading: ( ( 0, 0 ), 0 )
      // pair after reading:  ( ( 0.5, 1.345 ), 5 )
   }

   // reading data back using a different (but compatible) data type
   {
      std::ifstream                    is( "doc2.txt" );
      std::pair< std::pair< float, double >, int >
                                       pc2;

      std::cout << "pair before reading: " << boost::io::formatob( pc2, textio2 ) << '\n';
      is >> boost::io::formatob( pc2, textio2 );
      std::cout << "pair after reading:  "  << boost::io::formatob( pc2, textio2 ) << '\n';

      // [output]:
      // pair before reading: ( ( 0, 0 ), 0 )
      // pair after reading:  ( ( 0.5, 1.345 ), 5 )
   }

#  if !defined(BOOST_IOFM_NO_LIB_QUATERNION) && !defined(BOOST_IOFM_NO_LIB_OCTONION)
      // [5]: plain text -- 4-ary and 8-ary types

      // create the output object used for serialization

      boost::io::static_nary_output< char * >
                                       naryio;

      // create the data to be serialized

      boost::math::quaternion< float > q( 0.12f, 2.75f, -3.3345f, 70.2f );
      boost::math::octonion< double >  o( 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8 );

      // writing to a plain text-based serialization stream
      {
         // open the file to serialize the pair to
         std::ofstream                 out( "doc3.txt" );

         // serialize to the stream
         out << boost::io::formatob( q, naryio ) << '\n';
         out << boost::io::formatob( o, naryio ) << '\n';
      }

      // reading data back from a plain text-based serialization stream
      {
         std::ifstream                 is( "doc3.txt" );
         boost::math::quaternion< float >
                                       q2;
         boost::math::octonion< double >
                                       o2;

         std::cout << "before reading:\n"
                   << "   quaternion = " << boost::io::formatob( q2, naryio ) << '\n'
                   << "   octonion   = " << boost::io::formatob( o2, naryio ) << '\n';

         is >> boost::io::formatob( q2, naryio )
            >> boost::io::formatob( o2, naryio );

         std::cout << "after reading:\n"
                   << "   quaternion = " << boost::io::formatob( q2, naryio ) << '\n'
                   << "   octonion   = " << boost::io::formatob( o2, naryio ) << '\n';

         /* [output]:
            before reading:
               quaternion = ( 0, 0, 0, 0 )
               octonion   = ( 0, 0, 0, 0, 0, 0, 0, 0 )
            after reading:
               quaternion = ( 0.12, 2.75, 3.3345, 70.2 )
               octonion   = ( 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8 )
         */
      }
#  endif

   return( 0 );
}
