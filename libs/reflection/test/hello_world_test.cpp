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

#define BOOST_EXTENSION_USE_PP 1

#include <boost/extension/factory_map.hpp>
#include <boost/extension/factory.hpp>
#include <boost/extension/shared_library.hpp>
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK 1
#include <boost/test/unit_test.hpp>

#include "../examples/word.hpp"


using namespace boost::extensions;

BOOST_AUTO_TEST_CASE(hello_world_example)
{
  // check if the library can be loaded
  shared_library l((std::string("../bin/libHelloWorldLib") + ".extension").c_str());
  BOOST_CHECK_EQUAL( l.open(), true );

  // check if the factory can return the functor
  factory_map fm;
  functor<void, factory_map &> load_func = 
          l.get_functor<void, factory_map &>("extension_export_word");
  BOOST_CHECK_EQUAL( load_func.is_valid(), true );

  load_func(fm);

  // check if we can get the word list
  std::list<factory<word, int> > & factory_list = fm.get<word, int>();  
  BOOST_CHECK_EQUAL( factory_list.size(), 2U );

  // iterate trough the classes and execute get_val method 
  // to obtain the correct words
  std::list<factory<word, int> >::iterator current_word = factory_list.begin();

  std::auto_ptr<word> hello_word_ptr(current_word->create());
  BOOST_CHECK_EQUAL( !hello_word_ptr.get(), 0 );

  BOOST_CHECK_EQUAL( hello_word_ptr->get_val(), "hello");

  ++current_word;

  std::auto_ptr<word> world_word_ptr(current_word->create());
  BOOST_CHECK_EQUAL( !world_word_ptr.get(), 0 );

  BOOST_CHECK_EQUAL( world_word_ptr->get_val(), "world!");
}

