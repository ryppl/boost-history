/* Boost.IndexedSet test for modifier memfuns.
 *
 * Copyright Joaquín M López Muñoz 2003. Use, modification, and distribution
 * are subject to the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/indexed_set for library home page.
 */

#include "test_modifiers.hpp"

#include <boost/config.hpp> /* keep it first to prevent some nasty warnings in MSVC */
#include <vector>
#include "employee.hpp"
#include <boost/test/test_tools.hpp>

using namespace boost::indexed_sets;

void test_modifiers()
{
  employee_set es;
  employee_set_by_age& i2=get<age>(es);

  es.insert(employee(0,"Joe",31));
  BOOST_CHECK(es.insert(employee(0,"Joe",31)).second==false);
  BOOST_CHECK(i2.insert(employee(0,"Joe Jr.",15)).second==false);

  employee_set_by_age::iterator it=i2.find(31);
  i2.insert(it,employee(1,"Joe Jr.",15));
  BOOST_CHECK(es.size()==2);

  es.erase(employee(1,"Joe Jr.",15));
  BOOST_CHECK(i2.size()==1);

  i2.erase(it);
  BOOST_CHECK(es.size()==0);

  es.insert(employee(0,"Joe",31));
  es.insert(employee(1,"Jack",31));
  i2.erase(31);
  BOOST_CHECK(i2.size()==0);

#if !defined(BOOST_NO_MEMBER_TEMPLATES)||defined(BOOST_MSVC6_MEMBER_TEMPLATES)
  std::vector<employee> ve;
  ve.push_back(employee(3,"Anna",31));
  ve.push_back(employee(1,"Rachel",27));
  ve.push_back(employee(2,"Agatha",40));

  i2.insert(ve.begin(),ve.end());
  BOOST_CHECK(es.size()==3);

  es.erase(es.begin(),es.end());
  BOOST_CHECK(i2.size()==0);
#endif

  es.insert(employee(0,"Joe",31));
  es.insert(employee(1,"Robert",27));
  es.insert(employee(2,"John",40));
  es.insert(employee(3,"Albert",20));
  es.insert(employee(4,"John",57));

  employee_set es_backup(es);

  employee_set es2;
  es2.insert(employee(3,"Anna",31));
  es2.insert(employee(1,"Rachel",27));
  es2.insert(employee(2,"Agatha",40));

  employee_set es2_backup(es2);

  i2.swap(get<2>(es2));
  BOOST_CHECK(es==es2_backup&&es2==es_backup);

#if defined(BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP)||defined(BOOST_INTEL_CXX_VERSION)
  ::boost::indexed_sets::detail::swap(i2,get<2>(es2));
#else
  using std::swap;
  swap(i2,get<2>(es2));
#endif

  BOOST_CHECK(es==es_backup&&es2==es2_backup);

  es2.clear();
  BOOST_CHECK(es2.size()==0);
}
