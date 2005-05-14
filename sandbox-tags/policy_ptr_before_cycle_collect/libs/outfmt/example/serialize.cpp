// (C) Copyright 2003-2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// Examples for text-based serialization

// note: the std::pair and std::string serialization examples are currently broken

#include <iostream>                         // std::cout
#include <fstream>                          // std::fstream
#include <string>                           // std::readline
#include <boost/outfmt/stl/complex.hpp>     // std::complex, operator<<
#include <boost/outfmt/stl/list.hpp>        // std::list, operator<<
#include <boost/outfmt/stl/map.hpp>         // std::map, operator<<
#include <boost/outfmt/stl/vector.hpp>      // std::map, operator<<
#if !defined(BOOST_IOFM_NO_LIB_QUATERNION)
#  include <boost/outfmt/boost/quaternion.hpp>
#endif
#if !defined(BOOST_IOFM_NO_LIB_OCTONIAN)
#  include <boost/outfmt/boost/octonion.hpp>
#endif

#include <boost/outfmt/formatob.hpp>

int main()
{
   // [1]: plain text

   // create the output object used for serialization

   boost::io::pairfmt_t< const char * > textio; // default formatting

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
      // open up the file to read in the serialized data
      std::ifstream                    is( "doc.txt" );
      std::pair< char, int >           p2;

      // retrieve the data from the stream
      std::cout << "pair before reading: " << boost::io::formatob( p2, textio ) << '\n';
      is >> boost::io::formatob( p2, textio );
      std::cout << "pair after reading:  "  << boost::io::formatob( p2, textio ) << '\n';

      // [output]:
      //    pair before reading: (  , 0 )
      //    pair after reading:  ( a, 5 )
   }

   // [2]: XML -- [note]: XML is a text-based system, and can thus be serialized to

   boost::io::pairfmt_t< const char * > xmlio;
   xmlio.format( "<pairob><item>", "</item></pairob>", "</item><item>" );

   // writing to an XML-based serialization stream
   {
      std::ofstream                    out( "doc1.xml" );
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

   boost::io::pairfmt_t
   <
      const char *,
      boost::io::wrappedfmt_t< const char * >,
      boost::io::wrappedfmt_t< const char * >
   >                                   xmlio2 =
      boost::io::pairfmt
      (
         boost::io::wrappedfmt().format( "<i1>", "</i1>" ),
         boost::io::wrappedfmt().format( "<i2>", "</i2>" )
      ).format( "<pairob>", "</pairob>", "" );

   // writing to an XML-based serialization stream
   {
      std::ofstream                    out( "doc2.xml" );
      out << "<?xml version = \"1.0\" encoding = \"UTF-8\"?>\n\n"; // XML signature
      out << boost::io::formatob( p1, xmlio2 ) << '\n';
   }

   // reading data back from a plain text-based serialization stream
   {
      std::ifstream                    is( "doc2.xml" );
      std::pair< char, int >           p2;

      std::cout << "pair before reading: " << boost::io::formatob( p2, textio ) << '\n';
      std::string                      s;
      std::getline( is, s ); // skip XML signature
      is >> boost::io::formatob( p2, xmlio );
      std::cout << "pair after reading:  "  << boost::io::formatob( p2, textio ) << '\n';
   }

   // [4]: plain text -- more advanced types

   // create the output object used for serialization

   boost::io::pairfmt_t
   <
      const char *,
      boost::io::pairfmt_t< const char * >
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
      std::ofstream                    out( "doc2.txt" );
      out << boost::io::formatob( pc1, textio2 ) << '\n';
   }

   // reading data back from a plain text-based serialization stream
   {
      std::ifstream                    in( "doc2.txt" );
      std::pair< std::complex< double >, int >
                                       pc2;

      std::cout << "pair before reading: " << boost::io::formatob( pc2, textio2 ) << '\n';
      in >> boost::io::formatob( pc2, textio2 );
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

      boost::io::naryfmt_t< const char * >
                                       naryio;
      naryio.format( "[ ", " ]" ); // container-compatible formatting

      // create the data to be serialized

      boost::math::quaternion< float > q( 0.12f, 2.75f, -3.3345f, 70.2f );
      boost::math::octonion< double >  o( 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8 );

      // writing to a plain text-based serialization stream
      {
         std::ofstream                 out( "doc3.txt" );
         out << boost::io::formatob( q, naryio ) << '\n';
         out << boost::io::formatob( o, naryio ) << '\n';
      }

      // reading data back from a plain text-based serialization stream
      {
         std::ifstream                 in( "doc3.txt" );
         boost::math::quaternion< float >
                                       q2;
         boost::math::octonion< double >
                                       o2;

         std::cout << "before reading:\n"
                   << "   quaternion = " << boost::io::formatob( q2, naryio ) << '\n'
                   << "   octonion   = " << boost::io::formatob( o2, naryio ) << '\n';

         in >> boost::io::formatob( q2, naryio )
            >> boost::io::formatob( o2, naryio );

         std::cout << "after reading:\n"
                   << "   quaternion = " << boost::io::formatob( q2, naryio ) << '\n'
                   << "   octonion   = " << boost::io::formatob( o2, naryio ) << '\n';

         /* [output]:
            before reading:
               quaternion = ( 0, 0, 0, 0 )
               octonion   = ( 0, 0, 0, 0, 0, 0, 0, 0 )
            after reading:
               quaternion = ( 0.12, 2.75, -3.3345, 70.2 )
               octonion   = ( 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8 )
         */
      }

      // read data into a standard container
      {
         std::ifstream                 in( "doc3.txt" );
         std::list< double >           l;

         std::cout << "before:      " << l << '\n';

         in >> l;
         std::cout << "first read:  " << l << '\n';

         in >> l;
         std::cout << "second read: " << l << '\n';

         /* [output]:
            before:      [  ]
            first read:  [ 0.12, 2.75, -3.3345, 70.2 ]
            second read: [ 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8 ]
         */
      }
#  endif

   // [6]: plain text -- string data

   boost::io::containerfmt_t< const char * >
                                       stlio = boost::io::containerfmt();

   boost::io::containerfmt_t
   <
      const char *,
      boost::io::pairfmt_t< const char * >
   >                                   mapio = boost::io::containerfmt
                                       (
                                          boost::io::pairfmt()
                                       );

   std::list< std::string >            l;
   l.push_back( "1234" );
   l.push_back( "abcdef" );
   l.push_back( "To be or not to be: that is the question!" );

   std::map< std::string, std::string > m;
   m[ "Hallo Welt!"      ] = "German";
   m[ "Boa Noite."       ] = "Portuguese";
   m[ "Como esta Usted?" ] = "Spanish";

   // writing to a plain text-based serialization stream
   {
      std::ofstream                    out( "doc4.txt" );
      out << boost::io::formatob( l, stlio ) << '\n'
          << boost::io::formatob( m, mapio ) << '\n';
   }

   // reading data back from a plain text-based serialization stream
   {
      std::ifstream                    in( "doc4.txt" );
      std::vector< std::string >       v;
      std::map< std::string, std::string >
                                       m;

      std::cout << "before reading:\n"
                << "   vector: " << v << '\n'
                << "   map:    " << boost::io::formatob( m, mapio ) << '\n';

      in >> boost::io::formatob( v, stlio )
         >> boost::io::formatob( m, mapio );

      std::cout << "after reading:\n"
                << "   vector: " << v << '\n'
                << "   map:    " << boost::io::formatob( m, mapio ) << '\n';

      /* [output]:
         before reading:
            vector: [  ]
            map:    [  ]
         after reading:
            vector: [ "1234", "abcdef", "To be or not to be: that is the question!" ]
            map:    [ ( "Boa Noite.", "Portuguese" ), ( "Como esta Usted?", "Spanish" ), ( "Hallo Welt!", "German" ) ]
      */
   }

   return( 0 );
}
