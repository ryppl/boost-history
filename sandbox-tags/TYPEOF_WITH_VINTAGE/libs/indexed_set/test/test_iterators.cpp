/* Boost.IndexedSet test for iterators.
 *
 * Copyright Joaquín M López Muñoz 2003-2004. Use, modification, and distribution
 * are subject to the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/indexed_set for library home page.
 */

#include "test_iterators.hpp"

#include <boost/config.hpp> /* keep it first to prevent some nasty warnings in MSVC */
#include "pre_indexed_set.hpp"
#include "employee.hpp"
#include <boost/test/test_tools.hpp>

using namespace boost::indexed_sets;

void test_iterators()
{
  employee_set es;

  es.insert(employee(0,"Joe",31));
  es.insert(employee(1,"Robert",27));
  es.insert(employee(2,"John",40));
  es.insert(employee(3,"Albert",20));
  es.insert(employee(4,"John",57));

  {
    int n=0;
    for(employee_set::const_iterator it=es.begin();it!=es.end();++it){
      n+=it->id;
    }
    int m=0;
    for(employee_set::reverse_iterator rit=es.rbegin();rit!=es.rend();++rit){
      m+=rit->id;
    }
    BOOST_CHECK(n==0+1+2+3+4&&n==m);
  }

  {
    int n=0;
    employee_set_by_name& i1=get<by_name>(es);
    for(employee_set_by_name::iterator it=i1.begin();it!=i1.end();++it){
      n+=it->id;
    }
    int m=0;
    const employee_set_by_age& i2=get<2>(es);
    for(employee_set_by_age::const_reverse_iterator rit=i2.rbegin();rit!=i2.rend();++rit){
      m+=rit->id;
    }
    int p=0;
    const employee_set_as_inserted& i3=get<3>(es);
    for(
      employee_set_as_inserted::const_reverse_iterator rit2=i3.rbegin();
      rit2!=i3.rend();++rit2){
      p+=rit2->id;
    }
    BOOST_CHECK(n==0+1+2+3+4&&n==m&&n==p);
  }
}
