/* (C) Copyright Mariano G. Consoni 2007
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/extension/factory_map.hpp>
#include <boost/extension/factory.hpp>
#include <boost/extension/shared_library.hpp>
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK 1
#include <boost/test/unit_test.hpp>

#include "../examples/parameters/lots_of_parameters_interface.hpp"


using namespace boost::extensions;

BOOST_AUTO_TEST_CASE(parameters_example)
{
  // check if the library can be loaded
  shared_library l((std::string("libParametersLib") + ".extension").c_str());
  BOOST_CHECK_EQUAL( l.open(), true );

  // check if the factory can return the functor
  factory_map fm;
  functor<void, factory_map &> load_func = l.get_functor<void, factory_map &>("extension_export");
  BOOST_CHECK_EQUAL( load_func.is_valid(), true );

  load_func(fm);

  // check if we can get the parameter list
  std::list<factory<lots_of_parameters_interface, int, bool, unsigned int, char, std::string, A, boost::shared_ptr<A> > > & factory_list = 
	  fm.get<lots_of_parameters_interface, int, bool, unsigned int, char, std::string, A, boost::shared_ptr<A> >();  
  BOOST_CHECK_EQUAL( factory_list.size(), 1 );

  // get the interface and construct it
  std::list<factory<lots_of_parameters_interface, int, bool, unsigned int, char, std::string, A, boost::shared_ptr<A> > >::iterator params = 
	  factory_list.begin();

  std::auto_ptr<lots_of_parameters_interface> params_ptr(params->create(true, 4, 'c', "test", A(2), boost::shared_ptr<A>(new A(15))));
  BOOST_CHECK_EQUAL( !params_ptr.get(), 0 );

}
