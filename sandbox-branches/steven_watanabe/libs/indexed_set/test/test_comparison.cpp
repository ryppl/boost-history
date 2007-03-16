/* Boost.IndexedSet test for comparison functions.
 *
 * Copyright Joaquín M López Muñoz 2003-2004. Use, modification, and distribution
 * are subject to the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/indexed_set for library home page.
 */

#include "test_comparison.hpp"

#include <boost/config.hpp> /* keep it first to prevent some nasty warnings in MSVC */
#include "pre_indexed_set.hpp"
#include "employee.hpp"
#include <boost/test/test_tools.hpp>

using namespace boost::indexed_sets;

void test_comparison()
{
  employee_set              es;
  employee_set_by_name&     i1=get<1>(es);
  employee_set_as_inserted& i3=get<3>(es);
  es.insert(employee(0,"Joe",31));
  es.insert(employee(1,"Robert",27));
  es.insert(employee(2,"John",40));
  es.insert(employee(3,"Albert",20));
  es.insert(employee(4,"John",57));

  employee_set              es2;
  employee_set_by_name&     i12=get<name>(es2);
  employee_set_as_inserted& i32=get<3>(es2);
  es2.insert(employee(0,"Joe",31));
  es2.insert(employee(1,"Robert",27));
  es2.insert(employee(2,"John",40));
  es2.insert(employee(3,"Albert",20));

  BOOST_CHECK(es==es&&es<=es&&es>=es&&
              i12==i12&&i12<=i12&&i12>=i12&&
              i32==i32&&i32<=i32&&i32>=i32);
  BOOST_CHECK(es!=es2&&es2<es&&es>es2&&!(es<=es2)&&!(es2>=es));
  BOOST_CHECK(i1!=i12&&i12<i1&&i1>i12&&!(i1<=i12)&&!(i12>=i1));
  BOOST_CHECK(i3!=i32&&i32<i3&&i3>i32&&!(i3<=i32)&&!(i32>=i3));
}
