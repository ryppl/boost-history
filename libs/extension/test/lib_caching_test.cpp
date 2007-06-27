/*
 * Boost.Extension / hello world unit test
 *
 * (C) Copyright Mariano G. Consoni 2007
 * Distributed under the Boost Software License, Version 1.0. (See             
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/ for latest version.
 */

#include <boost/extension/factory_map.hpp>
#include <boost/extension/factory.hpp>
#include <boost/extension/shared_library.hpp>
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK 1

#include <boost/test/unit_test.hpp>
#include <boost/filesystem/operations.hpp>

#include <iostream>
#include <string>

#include "../examples/word.hpp"

#define BOOST_EXTENSION_LIBS_DIRECTORY "../bin/"

using namespace boost::extensions;

/// tests if the SO caches libraries and if we can remove .extensions files before unloading

BOOST_AUTO_TEST_CASE(lib_caching_test)
{
  if(boost::filesystem::exists(BOOST_EXTENSION_LIBS_DIRECTORY 
                               "libHelloWorldCache.extension")) {
    boost::filesystem::remove(BOOST_EXTENSION_LIBS_DIRECTORY 
                              "libHelloWorldCache.extension");
  }
  boost::filesystem::copy_file(BOOST_EXTENSION_LIBS_DIRECTORY 
                               "libHelloWorldLib.extension", 
                               BOOST_EXTENSION_LIBS_DIRECTORY 
                               "libHelloWorldCache.extension");

  {
    // load the first version
    shared_library l((std::string("libHelloWorldCache") 
                            + ".extension").c_str());
    BOOST_CHECK_EQUAL( l.open(), true );
    {

      // check if the factory can return the functor
      factory_map fm;
      functor<void, factory_map &> load_func = l.get_functor<void, 
        factory_map &>("extension_export_word");
      BOOST_CHECK_EQUAL( load_func.is_valid(), true );
          
      load_func(fm);

      std::list<factory<word, int> > & factory_list = fm.get<word, int>();  
      BOOST_CHECK_EQUAL( factory_list.size(), 2U );

      std::list<factory<word, int> >::iterator current_word = 
        factory_list.begin();

      std::auto_ptr<word> hello_word_ptr(current_word->create());
      BOOST_CHECK_EQUAL( !hello_word_ptr.get(), 0 );

      BOOST_CHECK_EQUAL( hello_word_ptr->get_val(), "hello");
      std::cerr << hello_word_ptr->get_val() << " " << std::endl;

      ++current_word;

      std::auto_ptr<word> world_word_ptr(current_word->create());
      BOOST_CHECK_EQUAL( !world_word_ptr.get(), 0 );

      BOOST_CHECK_EQUAL( world_word_ptr->get_val(), "world!");
      std::cerr << world_word_ptr->get_val() << std::endl << std::endl;
    }
    l.close();
  }

  // replace the loaded library and try to reload
  boost::filesystem::remove(BOOST_EXTENSION_LIBS_DIRECTORY 
                            "libHelloWorldCache.extension");
  boost::filesystem::copy_file(BOOST_EXTENSION_LIBS_DIRECTORY 
                               "libHelloWorldLibv2.extension", 
                               BOOST_EXTENSION_LIBS_DIRECTORY 
                               "libHelloWorldCache.extension");

  {
    // load the second version
    shared_library l((std::string("libHelloWorldCache") 
                      + ".extension").c_str());
    BOOST_CHECK_EQUAL( l.open(), true );

    {
      // check if the factory can return the functor
      factory_map fm;
      functor<void, factory_map &> load_func = l.get_functor<void, 
        factory_map &>("extension_export_word");
      BOOST_CHECK_EQUAL( load_func.is_valid(), true );

      load_func(fm);

      // check if we can get the word list
      std::list<factory<word, int> > & factory_list = fm.get<word, int>();  
      BOOST_CHECK_EQUAL( factory_list.size(), 2U );
          
      // iterate trough the classes and execute get_val method 
      // to obtain the correct words
      std::list<factory<word, int> >::iterator current_word = 
        factory_list.begin();
          
      std::auto_ptr<word> hello_word_ptr(current_word->create());
      BOOST_CHECK_EQUAL( !hello_word_ptr.get(), 0 );

      BOOST_CHECK_EQUAL( hello_word_ptr->get_val(), "| v2 hello");
      std::cerr << hello_word_ptr->get_val() << " " << std::endl;

      ++current_word;

      std::auto_ptr<word> world_word_ptr(current_word->create());
      BOOST_CHECK_EQUAL( !world_word_ptr.get(), 0 );

      BOOST_CHECK_EQUAL( world_word_ptr->get_val(), "world! v2");
      std::cerr << world_word_ptr->get_val() << " " << std::endl;
    }
    l.close();
  }

  {
    // load the library again and remove it when loaded
    shared_library l((std::string("libHelloWorldCache") 
                      + ".extension").c_str());
    BOOST_CHECK_EQUAL( l.open(), true );

    {
      // check if the factory can return the functor
      factory_map fm;
      functor<void, factory_map &> load_func = l.get_functor<void, 
        factory_map &>("extension_export_word");
      BOOST_CHECK_EQUAL( load_func.is_valid(), true );

      load_func(fm);

      // check if we can get the word list
      std::list<factory<word, int> > & factory_list = fm.get<word, int>();  
      BOOST_CHECK_EQUAL( factory_list.size(), 2U );
          
      // iterate trough the classes and execute get_val method 
      // to obtain the correct words
      std::list<factory<word, int> >::iterator current_word = 
        factory_list.begin();
          
      std::auto_ptr<word> hello_word_ptr(current_word->create());
      BOOST_CHECK_EQUAL( !hello_word_ptr.get(), 0 );

      BOOST_CHECK_EQUAL( hello_word_ptr->get_val(), "| v2 hello");
      std::cerr << hello_word_ptr->get_val() << " " << std::endl;

      ++current_word;

      boost::filesystem::remove(BOOST_EXTENSION_LIBS_DIRECTORY 
                                "libHelloWorldCache.extension");

      std::auto_ptr<word> world_word_ptr(current_word->create());
      BOOST_CHECK_EQUAL( !world_word_ptr.get(), 0 );

      BOOST_CHECK_EQUAL( world_word_ptr->get_val(), "world! v2");
      std::cerr << world_word_ptr->get_val() << " " << std::endl;


    }
    l.close();
  }
}

