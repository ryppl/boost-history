// (C) Copyright 2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

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
         void                                    unsupported(      const char *, const char * );
         bool                                    save_testresults( bool, bool, const char *, const char *, const std::string &, const std::string & );
      private:
         xmldom::element_ptr                     get_testset( xmldom::element_ptr );
         xmldom::element_ptr                     get_group(   xmldom::element_ptr, const char * );
      public:
                             test_engine( const char * );
                            ~test_engine();
   };

   template< class Formatter >
   bool test_formatter( test_engine & te, const Formatter & fmt, const char * results, const char * name )
   {
      // write:

      std::ostringstream               ss;
      ss << fmt;

      // read:

      std::istringstream               ss2( results );
      ss2  >> fmt;
      std::ostringstream               ssrd;
      ssrd << fmt;

      return( te.save_testresults(
         ( !ss2.fail() && ssrd.str() == results ), ( ss.str() == results ),
         name, results, ssrd.str(), ss.str()
      ));
   }

   template< class Object >
   bool test_stlio( test_engine & te, Object & o, const char * results, const char * name )
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

      return( te.save_testresults( 
         ( !ss2.fail() && ssrd.str() == results ), ( ss.str() == results ),
         name, results, ssrd.str(), ss.str()
      ));
   }
#endif
