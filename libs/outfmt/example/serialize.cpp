// (C) Copyright 2003: Reece H. Dunn

// Examples for text-based serialization

#include <iostream> // std::cout
#include <fstream>  // std::fstream
#include <list>     // std::list

#include <boost/outfmt/formatob.hpp>

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
      // [todo]
      // in >> boost::io::readobject( p1, textio ); ?
   }

   // [2]: XML -- [note]: XML is a text-based system, and can thus be serialized to

   boost::io::pair_output< char * >    xmlio;
   xmlio.format( "<pairob><item>", "</item></pairob>", "</item><item>" );

   // writing to an XML-based serialization stream
   {
      // open the file to serialize the pair to
      std::ofstream                    out( "doc1.xml" );
      out << "<?xml version = \"1.0\" encoding = \"UTF-8\"?>\n\n"; // XML signature

      // serialize to the stream
      out << boost::io::formatobout( p1, xmlio ) << '\n';
   }

   // reading data back from a plain text-based serialization stream
   {
      // [todo]
      // in >> boost::io::readobject( p1, xmlio ); ?
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

   return( 0 );
}
