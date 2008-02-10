/*
 * Boost.Reflection / basic unit test
 *
 * (C) Copyright Jeremy Pack 2007
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/ for latest version.
 */


#include <string>
#include <iostream>

#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK 1
#include <boost/test/unit_test.hpp>
#include <boost/reflection/parameter_map.hpp>

using boost::reflections::parameter_map;
using boost::reflections::generic_parameter;
using boost::reflections::parameter;
BOOST_AUTO_TEST_CASE(construction) {
  parameter<int> i_param(5);
  parameter_map m;
}