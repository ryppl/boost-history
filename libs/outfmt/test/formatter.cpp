// (C) Copyright 2004: Reece H. Dunn

#include <iostream>
#include <boost/outfmt/formatlist.hpp>

#include "test_engine.hpp"

int main()
{
   test_engine                         test( "boost/io/formatter" );

   std::list< char >                   l;
   l.push_back( 'A' );
   l.push_back( 'B' );
   l.push_back( 'C' );

   boost::io::openclose_formatter< char * >
                                       ocfmt( "< ", " >" );
   boost::io::formatter< char * >      fmt(   "< ", " >", " | " );

   // boost::io::formatter_t< ... >::format

   test.formatter( boost::io::format( l ).format( " : " ),             "[ A : B : C ]", "format( sep )" );
   test.formatter( boost::io::format( l ).format( "{ ", " }" ),        "{ A, B, C }",   "format( open, close )" );
   test.formatter( boost::io::format( l ).format( "{ ", " }", " : " ), "{ A : B : C }", "format( open, close, sep )" );

   // boost::io::openclose_formatter< ... >

   test.formatter( boost::io::format( l ).format( boost::io::openclose_formatter< char * >()),             "[ A, B, C ]", "format( openclose_formatter())" );
   test.formatter( boost::io::format( l ).format( boost::io::openclose_formatter< char * >( "{ ", " }" )), "{ A, B, C }", "format( openclose_formatter( open, close ))" );
   test.formatter( boost::io::format( l ).format( boost::io::openclose_formatter< char * >( ocfmt )),      "< A, B, C >", "format( openclose_formatter( openclose_formatter ))" );

   // boost::io::formatter< ... >

   test.formatter( boost::io::format( l ).format( boost::io::formatter< char * >()),                    "[ A, B, C ]",   "format( formatter())" );
   test.formatter( boost::io::format( l ).format( boost::io::formatter< char * >( " : " )),             "[ A : B : C ]", "format( formatter( sep ))" );
   test.formatter( boost::io::format( l ).format( boost::io::formatter< char * >( "{ ", " }" )),        "{ A, B, C }",   "format( formatter( open, close ))" );
   test.formatter( boost::io::format( l ).format( boost::io::formatter< char * >( "{ ", " }", " : " )), "{ A : B : C }", "format( formatter( open, close, sep ))" );
   test.formatter( boost::io::format( l ).format( boost::io::formatter< char * >( ocfmt )),             "< A, B, C >",   "format( formatter( openclose_formatter ))" );
   test.formatter( boost::io::format( l ).format( boost::io::formatter< char * >( ocfmt, " : " )),      "< A : B : C >", "format( formatter( openclose_formatter, sep ))" );
   test.formatter( boost::io::format( l ).format( boost::io::formatter< char * >( fmt )),               "< A | B | C >", "format( formatter( formatter ))" );

   return( 0 );
}
