/* Used in Boost.IndexedSet tests.
 *
 * Copyright Joaquín M López Muñoz 2003. Use, modification, and distribution
 * are subject to the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/indexed_set for library home page.
 */

#ifndef BOOST_INDEXED_SET_TEST_EMPLOYEE_HPP
#define BOOST_INDEXED_SET_TEST_EMPLOYEE_HPP

#define BOOST_INDEXED_SET_ENABLE_INVARIANT_CHECKING
#define BOOST_INDEXED_SET_ENABLE_SAFE_MODE

#include <boost/indexed_set/safe_mode_errors.hpp>

struct safe_mode_exception
{
  safe_mode_exception(boost::indexed_sets::safe_mode::error_code error_code_):
    error_code(error_code_)
  {}

  boost::indexed_sets::safe_mode::error_code error_code;
};

#define BOOST_INDEXED_SET_SAFE_MODE_ASSERT(expr,error_code) \
if(!(expr)){throw safe_mode_exception(error_code);}

#include <boost/config.hpp> /* keep it first to prevent some nasty warnings in MSVC */
#include <boost/indexed_set.hpp>
#include <cstddef>
#include <ostream>
#include <string>

struct employee
{
  int         id;
  std::string name;
  int         age;

  employee(int id_,std::string name_,int age_):id(id_),name(name_),age(age_){}

  bool operator==(const employee& x)const
  {
    return id==x.id&&name==x.name&&age==x.age;
  }

  bool operator<(const employee& x)const
  {
    return id<x.id;
  }

  struct comp_id
  {
    bool operator()(int x,const employee& e2)const{return x<e2.id;}
    bool operator()(const employee& e1,int x)const{return e1.id<x;}
  };

  friend std::ostream& operator<<(std::ostream& os,const employee& e)
  {
    os<<e.id<<" "<<e.name<<" "<<e.age<<std::endl;
    return os;
  }
};

struct name{};
struct by_name{};
struct age{};

typedef
  boost::indexed_sets::indexed_set<
    employee,
    boost::indexed_sets::index_list<
      boost::indexed_sets::unique<
        boost::indexed_sets::identity<employee> >,
      boost::indexed_sets::non_unique<
        boost::indexed_sets::tag<name,by_name>,
        BOOST_INDEXED_SET_MEMBER(employee,std::string,name)>,
      boost::indexed_sets::non_unique<
        boost::indexed_sets::tag<age>,
        BOOST_INDEXED_SET_MEMBER(employee,int,age)> > >
  employee_set;

#if defined(BOOST_NO_MEMBER_TEMPLATES)
typedef boost::indexed_sets::nth_index_type<employee_set,1>::type employee_set_by_name;
#else
typedef employee_set::nth_index_type<1>::type employee_set_by_name;
#endif

typedef boost::indexed_sets::index_type<employee_set,age>::type employee_set_by_age;

#endif
