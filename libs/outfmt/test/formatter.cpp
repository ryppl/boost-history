// (C) Copyright 2004: Reece H. Dunn

#include <boost/outfmt/formatlist.hpp>
#include <boost/outfmt/stl/list.hpp>   // std::list support

#include "test_engine.hpp"

int main()
{
   test_engine                         test( "boost/io/formatter" );

   std::list< char >                   l;
   l.push_back( 'A' );
   l.push_back( 'B' );
   l.push_back( 'C' );

   boost::io::openclose_formatter< const char * >
                                       ocfmt( "< ", " >" );
   boost::io::formatter< const char * >      fmt(   "< ", " >", " | " );

   // boost::io::formatter_t< ... >::format

   test_formatter( test, boost::io::format( l ).format( " : " ),             "[ A : B : C ]", "format( sep )" );
   test_formatter( test, boost::io::format( l ).format( "{ ", " }" ),        "{ A, B, C }",   "format( open, close )" );
   test_formatter( test, boost::io::format( l ).format( "{ ", " }", " : " ), "{ A : B : C }", "format( open, close, sep )" );

   // boost::io::openclose_formatter< ... >

   test_formatter( test, boost::io::format( l ).format( boost::io::openclose_formatter< const char * >()),             "[ A, B, C ]", "format( openclose_formatter())" );
   test_formatter( test, boost::io::format( l ).format( boost::io::openclose_formatter< const char * >( "{ ", " }" )), "{ A, B, C }", "format( openclose_formatter( open, close ))" );
   test_formatter( test, boost::io::format( l ).format( boost::io::openclose_formatter< const char * >( ocfmt )),      "< A, B, C >", "format( openclose_formatter( openclose_formatter ))" );

   // boost::io::formatter< ... >

   test_formatter( test, boost::io::format( l ).format( boost::io::formatter< const char * >()),                    "[ A, B, C ]",   "format( formatter())" );
   test_formatter( test, boost::io::format( l ).format( boost::io::formatter< const char * >( " : " )),             "[ A : B : C ]", "format( formatter( sep ))" );
   test_formatter( test, boost::io::format( l ).format( boost::io::formatter< const char * >( "{ ", " }" )),        "{ A, B, C }",   "format( formatter( open, close ))" );
   test_formatter( test, boost::io::format( l ).format( boost::io::formatter< const char * >( "{ ", " }", " : " )), "{ A : B : C }", "format( formatter( open, close, sep ))" );
   test_formatter( test, boost::io::format( l ).format( boost::io::formatter< const char * >( ocfmt )),             "< A, B, C >",   "format( formatter( openclose_formatter ))" );
   test_formatter( test, boost::io::format( l ).format( boost::io::formatter< const char * >( ocfmt, " : " )),      "< A : B : C >", "format( formatter( openclose_formatter, sep ))" );
   test_formatter( test, boost::io::format( l ).format( boost::io::formatter< const char * >( fmt )),               "< A | B | C >", "format( formatter( formatter ))" );

   return( 0 );
}
