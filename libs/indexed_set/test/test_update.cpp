/* Boost.IndexedSet test for update(), modify() and modify_key().
 *
 * Copyright Joaquín M López Muñoz 2003. Use, modification, and distribution
 * are subject to the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/indexed_set for library home page.
 */

#include "test_update.hpp"

#include <boost/config.hpp> /* keep it first to prevent some nasty warnings in MSVC */
#include <algorithm>
#include "employee.hpp"
#include "pair_of_ints.hpp"
#include <boost/test/test_tools.hpp>

using namespace boost::indexed_sets;

void test_update()
{
  employee_set          es;
  employee_set_by_name& i1=get<1>(es);

  es.insert(employee(0,"Joe",31));
  es.insert(employee(1,"Robert",27));
  es.insert(employee(2,"John",40));
  es.insert(employee(3,"Olbert",20));
  es.insert(employee(4,"John",57));

  employee_set::iterator          it=es.find(employee(0,"Joe",31));
  employee_set_by_name::iterator it1=i1.find("Olbert");

  BOOST_CHECK(es.update(it,*it));
  BOOST_CHECK(!es.update(it,employee(3,"Joe",31))&&it->id==0);
  BOOST_CHECK(es.update(it,employee(0,"Joe",32))&&it->age==32);
  BOOST_CHECK(i1.update(it1,employee(3,"Albert",20))&&it1->name=="Albert");

  typedef indexed_set<
    pair_of_ints,
    index_list<
      unique<BOOST_INDEXED_SET_MEMBER(pair_of_ints,int,first)>,
      unique<BOOST_INDEXED_SET_MEMBER(pair_of_ints,int,second)> > >
  int_int_set;

  int_int_set iis;
  iis.insert(pair_of_ints(0,0));
  iis.insert(pair_of_ints(5,5));

  BOOST_CHECK(!iis.update(iis.begin(),pair_of_ints(5,0)));
  BOOST_CHECK(!iis.update(iis.begin(),pair_of_ints(0,5)));

#if !defined(BOOST_NO_MEMBER_TEMPLATES)||defined(BOOST_MSVC6_MEMBER_TEMPLATES)
  BOOST_CHECK(iis.modify(iis.begin(),increment_first));
  BOOST_CHECK(iis.modify(iis.begin(),increment_first));
  BOOST_CHECK(iis.modify(iis.begin(),increment_first));
  BOOST_CHECK(iis.modify(iis.begin(),increment_first));

  BOOST_CHECK(!iis.modify(iis.begin(),increment_first));
  BOOST_CHECK(iis.size()==1);

  iis.insert(pair_of_ints(0,0));
  BOOST_CHECK(iis.modify(iis.begin(),increment_second));
  BOOST_CHECK(iis.modify(iis.begin(),increment_second));
  BOOST_CHECK(iis.modify(iis.begin(),increment_second));
  BOOST_CHECK(iis.modify(iis.begin(),increment_second));

  BOOST_CHECK(!iis.modify(iis.begin(),increment_second));
  BOOST_CHECK(iis.size()==1);

  iis.insert(pair_of_ints(0,0));
  BOOST_CHECK(iis.modify_key(iis.begin(),increment_int));
  BOOST_CHECK(iis.modify_key(iis.begin(),increment_int));
  BOOST_CHECK(iis.modify_key(iis.begin(),increment_int));
  BOOST_CHECK(iis.modify_key(iis.begin(),increment_int));

  BOOST_CHECK(!iis.modify_key(iis.begin(),increment_int));
  BOOST_CHECK(iis.size()==1);
#endif
}
