/* Boost.IndexedSet test for copying and assignment.
 *
 * Copyright Joaquín M López Muñoz 2003. Use, modification, and distribution
 * are subject to the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/indexed_set for library home page.
 */

#include "test_copy_assignment.hpp"

#include <boost/config.hpp> /* keep it first to prevent some nasty warnings in MSVC */
#include <algorithm>
#include <list>
#include <vector>
#include "employee.hpp"
#include <boost/test/test_tools.hpp>

using namespace boost::indexed_sets;

void test_copy_assignment()
{
  employee_set es;

  es.insert(employee(0,"Joe",31));
  es.insert(employee(1,"Robert",27));
  es.insert(employee(2,"John",40));
  es.insert(employee(2,"Aristotle",2387));
  es.insert(employee(3,"Albert",20));
  es.insert(employee(4,"John",57));

  employee_set es2(es);

  BOOST_CHECK(es==es2);

  employee_set es3;
  employee_set_by_age& v=get<age>(es3);
  v=get<2>(es);

  BOOST_CHECK(v==get<2>(es));

#if !defined(BOOST_NO_MEMBER_TEMPLATES)||defined(BOOST_MSVC6_MEMBER_TEMPLATES)
  std::list<employee> l;
  l.push_back(employee(3,"Anna",31));
  l.push_back(employee(1,"Rachel",27));
  l.push_back(employee(2,"Agatha",40));

  employee_set es4(l.begin(),l.end());

  l.sort();

  BOOST_CHECK(es4.size()==l.size()&&std::equal(es4.begin(),es4.end(),l.begin()));
#endif
}
