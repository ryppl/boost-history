/* Boost.IndexedSet test for projection capabilities.
 *
 * Copyright Joaquín M López Muñoz 2003. Use, modification, and distribution
 * are subject to the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/indexed_set for library home page.
 */

#include "test_projection.hpp"

#include <boost/config.hpp> /* keep it first to prevent some nasty warnings in MSVC */
#include "employee.hpp"
#include <boost/test/test_tools.hpp>

using namespace boost::indexed_sets;

void test_projection()
{
  employee_set          es;
  es.insert(employee(0,"Joe",31));
  es.insert(employee(1,"Robert",27));
  es.insert(employee(2,"John",40));
  es.insert(employee(3,"Albert",20));
  es.insert(employee(4,"John",57));

  employee_set::iterator         it,itbis;
  employee_set_by_name::iterator it1;
  employee_set_by_age::iterator  it2;

  BOOST_STATIC_ASSERT((boost::is_same<
    employee_set::iterator,
    nth_iterator_type<employee_set,0>::type >::value));
  BOOST_STATIC_ASSERT((boost::is_same<
    employee_set_by_name::iterator,
    nth_iterator_type<employee_set,1>::type >::value));
#if defined(BOOST_NO_MEMBER_TEMPLATES)
  BOOST_STATIC_ASSERT((boost::is_same<
    employee_set_by_age::iterator,
    iterator_type<employee_set,age>::type >::value));
#else
  BOOST_STATIC_ASSERT((boost::is_same<
    employee_set_by_age::iterator,
    employee_set::iterator_type<age>::type >::value));
#endif

  it=   es.find(employee(1,"Robert",27));
  it1=  project<name>(es,it);
  it2=  project<age>(es,it1);
#if defined(BOOST_NO_MEMBER_TEMPLATES)
  itbis=project<0>(es,it2);
#else
  itbis=es.project<0>(it2);
#endif

  BOOST_CHECK(*it==*it1&&*it1==*it2&&itbis==it);

  const employee_set& ces=es;

  employee_set::const_iterator         cit,citbis;
  employee_set_by_name::const_iterator cit1;
  employee_set_by_age::const_iterator  cit2;

  BOOST_STATIC_ASSERT((boost::is_same<
    employee_set::const_iterator,
    nth_const_iterator_type<employee_set,0>::type >::value));
  BOOST_STATIC_ASSERT((boost::is_same<
    employee_set_by_name::const_iterator,
    nth_const_iterator_type<employee_set,1>::type >::value));
#if defined(BOOST_NO_MEMBER_TEMPLATES)
  BOOST_STATIC_ASSERT((boost::is_same<
    employee_set_by_age::const_iterator,
    const_iterator_type<employee_set,age>::type >::value));
#else
  BOOST_STATIC_ASSERT((boost::is_same<
    employee_set_by_age::const_iterator,
    employee_set::const_iterator_type<age>::type >::value));
#endif

  BOOST_CHECK(project<age>(es,es.end())==get<age>(es).end());

  cit=   ces.find(employee(4,"John",57));
#if defined(BOOST_NO_MEMBER_TEMPLATES)
  cit1=  project<by_name>(ces,cit);
#else
  cit1=  ces.project<by_name>(cit);
#endif
  cit2=  project<age>(ces,cit1);
  citbis=project<0>(ces,cit2);

  BOOST_CHECK(*cit==*cit1&&*cit1==*cit2&&citbis==cit);
}
