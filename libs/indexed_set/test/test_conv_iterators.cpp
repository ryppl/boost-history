/* Boost.IndexedSet test for interconvertibilty between const and non-const iterators.
 *
 * Copyright Joaqu�n M L�pez Mu�oz 2003. Use, modification, and distribution
 * are subject to the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/indexed_set for library home page.
 */

#include "test_conv_iterators.hpp"

#include <boost/config.hpp> /* keep it first to prevent some nasty warnings in MSVC */
#include "employee.hpp"
#include <boost/test/test_tools.hpp>

using namespace boost::indexed_sets;

void test_conv_iterators()
{
  employee_set es;
  es.insert(employee(2,"John",40));

  {
    const employee_set& ces=es;
    employee_set::iterator        it=es.find(employee(2,"John",40));
    employee_set::const_iterator it1=es.find(employee(2,"John",40));
    employee_set::const_iterator it2=ces.find(employee(2,"John",40));

    BOOST_CHECK(it==it1&&it1==it2&&it2==it);
    BOOST_CHECK(*it==*it1&&*it1==*it2&&*it2==*it);
  }
  {
    employee_set_by_name&        i1=get<1>(es);
    const employee_set_by_name& ci1=get<1>(es);
    employee_set_by_name::iterator        it=i1.find("John");
    employee_set_by_name::const_iterator it1=i1.find("John");
    employee_set_by_name::const_iterator it2=ci1.find("John");

    BOOST_CHECK(it==it1&&it1==it2&&it2==it);
    BOOST_CHECK(*it==*it1&&*it1==*it2&&*it2==*it);
  }
}
