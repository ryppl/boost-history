/* Boost.IndexedSet test for capacity memfuns.
 *
 * Copyright Joaquín M López Muñoz 2003-2004. Use, modification, and distribution
 * are subject to the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/indexed_set for library home page.
 */

#include "test_capacity.hpp"

#include <boost/config.hpp> /* keep it first to prevent some nasty warnings in MSVC */
#include "pre_indexed_set.hpp"
#include "employee.hpp"
#include <boost/test/test_tools.hpp>

using namespace boost::indexed_sets;

void test_capacity()
{
  employee_set es;

  es.insert(employee(0,"Joe",31));
  es.insert(employee(1,"Robert",27));
  es.insert(employee(2,"John",40));
  es.insert(employee(3,"Albert",20));
  es.insert(employee(4,"John",57));

  BOOST_CHECK(!es.empty());
  BOOST_CHECK(es.size()==5);

  es.erase(es.begin());
  BOOST_CHECK(get<name>(es).size()==4);

  es.erase(es.begin());
  BOOST_CHECK(!get<as_inserted>(es).empty());
  BOOST_CHECK(get<as_inserted>(es).size()==3);

  indexed_set<int,index_list<sequenced<> > > ss;

  ss.resize(10);
  BOOST_CHECK(ss.size()==10);

  ss.resize(20);
  BOOST_CHECK(ss.size()==20);

  ss.resize(5);
  BOOST_CHECK(ss.size()==5);
}
