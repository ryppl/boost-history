/*
 * Boost.Extension / construction test case
 *
 * (C) Copyright Jeremy Pack 2007
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
#include <boost/test/unit_test.hpp>



using namespace boost::extensions;

BOOST_AUTO_TEST_CASE(zone_construction)
{
  factory_map z;

}
BOOST_AUTO_TEST_CASE(factory_construction)
{
  //factory f;

}
BOOST_AUTO_TEST_CASE(linked_library_construction)
{
  shared_library l((std::string("liblocal") + ".extension").c_str());

}
