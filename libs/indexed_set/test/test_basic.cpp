/* Boost.IndexedSet basic test.
 *
 * Copyright Joaqu�n M L�pez Mu�oz 2003. Use, modification, and distribution
 * are subject to the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/indexed_set for library home page.
 */

#include "test_basic.hpp"

#include <boost/config.hpp> /* keep it first to prevent some nasty warnings in MSVC */
#include <algorithm>
#include <vector>
#include "employee.hpp"
#include <boost/test/test_tools.hpp>

using namespace boost::indexed_sets;

struct less_by_employee_name
{
  bool operator()(const employee& e1,const employee& e2)const{return e1.name<e2.name;}
};

struct less_by_employee_age
{
  bool operator()(const employee& e1,const employee& e2)const{return e1.age<e2.age;}
};

void test_basic()
{
  employee_set          es;
  std::vector<employee> v;

#if defined(BOOST_NO_MEMBER_TEMPLATES)
  employee_set_by_name& i1=get<by_name>(es);
#else
  employee_set_by_name& i1=es.get<by_name>();
#endif

  const employee_set_by_age& i2=get<2>(es);

  es.insert(employee(0,"Joe",31));
  i1.insert(employee(1,"Robert",27));
  es.insert(employee(2,"John",40));
  i1.insert(employee(3,"Albert",20));
  es.insert(employee(4,"John",57));

  v.push_back(employee(0,"Joe",31));
  v.push_back(employee(1,"Robert",27));
  v.push_back(employee(2,"John",40));
  v.push_back(employee(3,"Albert",20));
  v.push_back(employee(4,"John",57));

  {
    /* by id */

    std::sort(v.begin(),v.end());
    BOOST_CHECK(std::equal(es.begin(),es.end(),v.begin()));
  }

  {
    /* by name */

    std::sort(v.begin(),v.end(),less_by_employee_name());
    BOOST_CHECK(std::equal(i1.begin(),i1.end(),v.begin()));
  }

  {
    /* by age */

    std::sort(v.begin(),v.end(),less_by_employee_age());
    BOOST_CHECK(std::equal(i2.begin(),i2.end(),v.begin()));
  }
}
