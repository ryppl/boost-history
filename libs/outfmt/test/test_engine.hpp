// (C) Copyright 2004: Reece H. Dunn

#ifndef BOOST_IOFM_TEST_ENGINE_HPP
#define BOOST_IOFM_TEST_ENGINE_HPP
#  include <boost/config.hpp>
#  include <sstream>

#  include "xmldom/xmldom-lite.hpp"
#  include "date_time.hpp"

   class test_engine: public boost::noncopyable
   {
      private:
         xmldom::document              res;
         xmldom::element_ptr           testset;
      public:
         template< class Formatter >
         bool                                    formatter( const Formatter &, const char *, const char * = 0 );
         template< class Object >
         bool                                    stlio( Object &, const char *, const char * = 0 );
         void                                    unsupported(     const char *, const char * );
      private:
         bool                                    save_testresults( xmldom::element_ptr, bool, bool, const char *, const char *, const std::string &, const std::string & );
      private:
         xmldom::element_ptr                     get_testset( xmldom::element_ptr );
         xmldom::element_ptr                     get_group(   xmldom::element_ptr, const char * );
      public:
                             test_engine( const char * );
                            ~test_engine();
   };

   template< class Formatter >
   bool test_engine::formatter( const Formatter & fmt, const char * results, const char * name )
   {
      // write:

      std::ostringstream               ss;
      ss << fmt;

      // read:

      std::istringstream               ss2( results );
      ss2  >> fmt;
      std::ostringstream               ssrd;
      ssrd << fmt;

      return( save_testresults( testset,
         ( !ss2.fail() && ssrd.str() == results ), ( ss.str() == results ),
         name, results, ssrd.str(), ss.str()
      ));
   }

   template< class Object >
   bool test_engine::stlio( Object & o, const char * results, const char * name )
   {
      // write:

      std::ostringstream               ss;
      ss << o;

      // read:
      
      Object                           ob;
      std::istringstream               ss2( results );
      ss2  >> ob;
      std::ostringstream               ssrd;
      ssrd << ob;

      return( save_testresults( testset,
         ( !ss2.fail() && ssrd.str() == results ), ( ss.str() == results ),
         name, results, ssrd.str(), ss.str()
      ));
   }
#endif
