/*
 * Boost.Extension / testing of adapter class
 *
 * (C) Copyright Jeremy Pack 2007
 * Distributed under the Boost Software License, Version 1.0. (See             
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/ for latest version.
 */

#include <boost/extension/adaptable_factory.hpp>
#include <boost/extension/shared_library.hpp>
#include <boost/reflection/parameter_map.hpp>
#include <boost/function.hpp>
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK 1
#include <boost/test/unit_test.hpp>


using namespace boost::extensions;
using namespace boost::reflections;

class adaptable_class_base {
public:
  virtual ~adaptable_class_base() {}
  virtual int get() {
    return 5;
  }
};

class adaptable_class : public adaptable_class_base {
public:
  virtual int get() {
    return my_value;
  }
  adaptable_class()
    : my_value(1) {
  }
  adaptable_class(int new_val)
    : my_value(new_val) {
  }
  adaptable_class(int first_new_val, int second_new_val)
    : my_value(first_new_val * second_new_val) {
  }
private:
  int my_value;
};

BOOST_AUTO_TEST_CASE(noArgTest) {
  parameter_map params;
  adaptable_factory<adaptable_class_base> current_factory;
  current_factory.set<adaptable_class>();
  adaptable_class_base* a = current_factory.create(params);
  BOOST_CHECK_EQUAL(a->get(), 1);
}

BOOST_AUTO_TEST_CASE(oneArgTest) {
  parameter_map params;
  parameter<int>* p1 = new parameter<int>(5);
  params.insert(std::make_pair("first value", p1));

  generic_parameter<>* g = params.get_first<int>("first value");
  BOOST_CHECK(g->can_cast<int>());
  BOOST_CHECK_EQUAL(5, g->cast<int>());

  adaptable_factory<adaptable_class_base> current_factory;
  current_factory.set<adaptable_class, int>("first value");
  BOOST_CHECK(current_factory.get_missing_parameter_list(params).empty());
  adaptable_class_base* a = current_factory.create(params);
  BOOST_CHECK_EQUAL(a->get(), 5);
}

BOOST_AUTO_TEST_CASE(twoArgTest) {
  parameter_map params;
  parameter<int>* p1 = new parameter<int>(5);
  parameter<int>* p2 = new parameter<int>(2);
  params.insert(std::make_pair("first value", p1));
  params.insert(std::make_pair("second value", p2));

  adaptable_factory<adaptable_class_base> current_factory;
  current_factory.set<adaptable_class, int, int>("first value", "second value");
  BOOST_CHECK(current_factory.get_missing_parameter_list(params).empty());
  adaptable_class_base* a = current_factory.create(params);
  BOOST_CHECK_EQUAL(a->get(), 10);
}
