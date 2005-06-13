/* Boost.IndexedSet test for update(), modify() and modify_key().
 *
 * Copyright Joaquín M López Muñoz 2003-2004. Use, modification, and distribution
 * are subject to the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/indexed_set for library home page.
 */

#include "test_update.hpp"

#include <boost/config.hpp> /* keep it first to prevent some nasty warnings in MSVC */
#include <algorithm>
#include "pre_indexed_set.hpp"
#include "employee.hpp"
#include "pair_of_ints.hpp"
#include <boost/test/test_tools.hpp>

using namespace boost::indexed_sets;

void test_update()
{
  employee_set              es;
  employee_set_as_inserted& i=get<as_inserted>(es);

  es.insert(employee(0,"Joe",31));
  es.insert(employee(1,"Robert",27));
  es.insert(employee(2,"John",40));
  es.insert(employee(3,"Olbert",20));
  es.insert(employee(4,"John",57));

  employee_set::iterator             it=es.find(employee(0,"Joe",31));
  employee_set_as_inserted::iterator it1=
    project<as_inserted>(es,get<name>(es).find("Olbert"));

  BOOST_CHECK(es.update(it,*it));
  BOOST_CHECK(!es.update(it,employee(3,"Joe",31))&&it->id==0);
  BOOST_CHECK(es.update(it,employee(0,"Joe",32))&&it->age==32);
  BOOST_CHECK(i.update(it1,employee(3,"Albert",20))&&it1->name=="Albert");

  typedef indexed_set<
    pair_of_ints,
    index_list<
      unique<BOOST_INDEXED_SET_MEMBER(pair_of_ints,int,first)>,
      unique<BOOST_INDEXED_SET_MEMBER(pair_of_ints,int,second)>,
      sequenced<> > >
  int_int_set;

  int_int_set iis;
  nth_index_type<int_int_set,2>::type& iii=get<2>(iis);
  iis.insert(pair_of_ints(0,0));
  iis.insert(pair_of_ints(5,5));
  iis.insert(pair_of_ints(10,10));

  BOOST_CHECK(!iis.update(iis.begin(),pair_of_ints(5,0)));
  BOOST_CHECK(!iii.update(iii.begin(),pair_of_ints(0,5)));
  BOOST_CHECK(!iis.update(iis.begin(),pair_of_ints(5,11)));
  BOOST_CHECK(!iis.update(iis.begin(),pair_of_ints(11,5)));

#if !defined(BOOST_NO_MEMBER_TEMPLATES)||defined(BOOST_MSVC6_MEMBER_TEMPLATES)
  BOOST_CHECK(iis.modify(iis.begin(),increment_first));
  BOOST_CHECK(iii.modify(iii.begin(),increment_first));
  BOOST_CHECK(iis.modify(iis.begin(),increment_first));
  BOOST_CHECK(iii.modify(iii.begin(),increment_first));

  BOOST_CHECK(!iis.modify(iis.begin(),increment_first));
  BOOST_CHECK(iis.size()==2);

  iis.insert(pair_of_ints(0,0));
  BOOST_CHECK(iii.modify(--iii.end(),increment_second));
  BOOST_CHECK(iis.modify(iis.begin(),increment_second));
  BOOST_CHECK(iii.modify(--iii.end(),increment_second));
  BOOST_CHECK(iis.modify(iis.begin(),increment_second));

  BOOST_CHECK(!iii.modify(--iii.end(),increment_second));
  BOOST_CHECK(iii.size()==2);

  iis.insert(pair_of_ints(0,0));
  BOOST_CHECK(iis.modify_key(iis.begin(),increment_int));
  BOOST_CHECK(iis.modify_key(iis.begin(),increment_int));
  BOOST_CHECK(iis.modify_key(iis.begin(),increment_int));
  BOOST_CHECK(iis.modify_key(iis.begin(),increment_int));

  BOOST_CHECK(!iis.modify_key(iis.begin(),increment_int));
  BOOST_CHECK(iis.size()==2);
#endif
}
