// (C) Copyright 2004: Reece H. Dunn

#ifndef BOOST_IOFM_TEST_ENGINE_CPP
#define BOOST_IOFM_TEST_ENGINE_CPP
#  include <boost/version.hpp>
#  include "../test_engine.hpp"
#  include <libs/date_time/src/gregorian/greg_month.cpp>

   void test_engine::unsupported( const char * name, const char * reason )
   {
      xmldom::element_ptr              elem = testset -> append( "test" );
      elem -> attr[ "name"   ] = name;
      elem -> attr[ "read"   ] = "unsupported";
      elem -> attr[ "write"  ] = "unsupported";
      elem -> attr[ "reason" ] = reason;
   }

   bool test_engine::save_testresults( bool read, bool write, const char * name, const char * results, const std::string & readstr, const std::string & writestr )
   {
      xmldom::element_ptr              elem = testset -> append( "test" );

      if( name != 0 )                  elem -> attr[ "name"  ] = name;
      elem -> attr[ "read"  ] = ( read  ? "passed" : "failed" );
      elem -> attr[ "write" ] = ( write ? "passed" : "failed" );

      elem -> append( "results" ) -> append( results,  xmldom::nodetype::CDATASection );
      elem -> append( "write"   ) -> append( writestr, xmldom::nodetype::CDATASection );
      elem -> append( "read"    ) -> append( readstr,  xmldom::nodetype::CDATASection );

      return( read && write );
   }

   xmldom::element_ptr test_engine::get_testset( xmldom::element_ptr node )
   {
      xmldom::element_ptr              ret;
      for( xmldom::element::collection::iterator i = node -> begin(); i != node -> end() && !ret; ++i )
      {
         if
         (
            (( *i ) -> attr[ "platform" ] == BOOST_PLATFORM ) &&
            (( *i ) -> attr[ "compiler" ] == BOOST_COMPILER ) &&
            (( *i ) -> attr[ "stdlib"   ] == BOOST_STDLIB   ) &&
            (( *i ) -> attr[ "boostver" ] == BOOST_STRINGIZE( BOOST_VERSION ))
         )
            ret = ( *i );
      }
      if( !ret )
      {
         ret = node -> append( "test-set" );
         ret -> attr[ "platform" ] = BOOST_PLATFORM;
         ret -> attr[ "compiler" ] = BOOST_COMPILER;
         ret -> attr[ "stdlib"   ] = BOOST_STDLIB;
         ret -> attr[ "boostver" ] = BOOST_STRINGIZE( BOOST_VERSION );
      }
      return( ret );
   }

   xmldom::element_ptr test_engine::get_group( xmldom::element_ptr node, const char * name )
   {
      xmldom::element_ptr              ret;
      for( xmldom::element::collection::iterator i = node -> begin(); i != node -> end() && !ret; ++i )
      {
         if(( *i ) -> attr[ "name" ] == name )
            ret = ( *i );
      }
      if( !ret )
      {
         ret = node -> append( "group" );
         ret -> attr[ "name" ] = name;
      }
      return( ret );
   }

   test_engine::test_engine( const char * name )
   {
      bool                             loaded = res.load( "results.xml" ) && !( res.root -> empty());
      if( !loaded )
      {
         xmldom::element_ptr           xml = res.root -> append( "xml", xmldom::nodetype::ProcessingInstruction );
         xml -> attr[ "version" ] = "1.0";

         xmldom::element_ptr           xsl = res.root -> append( "xml-stylesheet", xmldom::nodetype::ProcessingInstruction );
         xsl -> attr[ "href" ] = "view.xsl";
         xsl -> attr[ "type" ] = "text/xsl";
      }

      xmldom::element_ptr              root  = loaded ? res.root -> back() : res.root -> append( "data" );
      testset = get_group( get_testset( root ), name ) -> append( "run" );
      testset -> attr[ "date" ] = boost::posix_time::to_simple_string
                                  (
                                     boost::posix_time::second_clock::local_time()
                                  );
   }

   test_engine::~test_engine()
   {
      res.save( "results.xml" );
   }
#endif
