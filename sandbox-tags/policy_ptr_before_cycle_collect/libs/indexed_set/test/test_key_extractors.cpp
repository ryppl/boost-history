/* Boost.IndexedSet test for key extractors.
 *
 * Copyright Joaquín M López Muñoz 2003-2004. Use, modification, and distribution
 * are subject to the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/indexed_set for library home page.
 */

#include "test_key_extractors.hpp"

#include <boost/config.hpp> /* keep it first to prevent some nasty warnings in MSVC */
#include "pre_indexed_set.hpp"
#include <boost/indexed_set/identity.hpp>
#include <boost/indexed_set/member.hpp>
#include <boost/indexed_set/mem_fun.hpp>
#include <boost/ref.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/test/test_tools.hpp>
#include <list>
#include <memory>

using namespace boost::indexed_sets;

struct test_class
{
  int       int_member;
  const int int_cmember;

  bool bool_mem_fun_const()const{return true;}
  bool bool_mem_fun(){return false;}

  test_class(int i=0):int_member(i),int_cmember(i){}
};

BOOST_BROKEN_COMPILER_TYPE_TRAITS_SPECIALIZATION(test_class)

typedef identity<test_class>                                                idn;
typedef identity<const test_class>                                          cidn;
typedef BOOST_INDEXED_SET_MEMBER(test_class,int,int_member)                 key_m;
typedef BOOST_INDEXED_SET_MEMBER(test_class,const int,int_member)           ckey_m;
typedef BOOST_INDEXED_SET_MEMBER(test_class,const int,int_cmember)          key_cm;
typedef BOOST_INDEXED_SET_CONST_MEM_FUN(test_class,bool,bool_mem_fun_const) key_cmf;
typedef BOOST_INDEXED_SET_MEM_FUN(test_class,bool,bool_mem_fun)             key_mf;

void test_key_extractors()
{
  idn     id;
  cidn    cid;
  key_m   k_m;
  ckey_m  ck_m;
  key_cm  k_cm;
  key_cmf k_cmf;
  key_mf  k_mf;

  test_class                                 t;
  const test_class&                          ctr=t;

#if !defined(BOOST_NO_MEMBER_TEMPLATES)||defined(BOOST_MSVC6_MEMBER_TEMPLATES)
  test_class*                                tp=&t;
  const test_class*                          ctp=&t;

  test_class**                               tpp=&tp;
  const test_class**                         ctpp=&ctp;

  std::auto_ptr<test_class*>                 tap(new test_class*(tp));
  std::auto_ptr<const test_class*>           ctap(new const test_class*(ctp));
#endif

  boost::reference_wrapper<test_class>       tw(t);
  boost::reference_wrapper<const test_class> ctw(t);

#if !defined(BOOST_NO_MEMBER_TEMPLATES)||defined(BOOST_MSVC6_MEMBER_TEMPLATES)

#endif
  
  id(t).int_member=0;
  BOOST_CHECK(id(t).int_member==0);
  BOOST_CHECK(cid(t).int_member==0);
  BOOST_CHECK(k_m(t)==0);
  BOOST_CHECK(ck_m(t)==0);
  BOOST_CHECK(id(ctr).int_member==0);
  BOOST_CHECK(cid(ctr).int_member==0);
  BOOST_CHECK(k_m(ctr)==0);
  BOOST_CHECK(ck_m(ctr)==0);

#if !defined(BOOST_NO_MEMBER_TEMPLATES)||defined(BOOST_MSVC6_MEMBER_TEMPLATES)
  k_m(t)=1;
  BOOST_CHECK(id(tp).int_member==1);
  BOOST_CHECK(cid(tp).int_member==1);
  BOOST_CHECK(k_m(tp)==1);
  BOOST_CHECK(ck_m(tp)==1);
  BOOST_CHECK(cid(ctp).int_member==1);
  BOOST_CHECK(ck_m(ctp)==1);

  k_m(tp)=2;
  BOOST_CHECK(id(tpp).int_member==2);
  BOOST_CHECK(cid(tpp).int_member==2);
  BOOST_CHECK(k_m(tpp)==2);
  BOOST_CHECK(ck_m(tpp)==2);
  BOOST_CHECK(cid(ctpp).int_member==2);
  BOOST_CHECK(ck_m(ctpp)==2);

  k_m(tpp)=3;
  BOOST_CHECK(id(tap).int_member==3);
  BOOST_CHECK(cid(tap).int_member==3);
  BOOST_CHECK(k_m(tap)==3);
  BOOST_CHECK(ck_m(tap)==3);
  BOOST_CHECK(cid(ctap).int_member==3);
  BOOST_CHECK(ck_m(ctap)==3);

  k_m(tap)=4;
  BOOST_CHECK(id(tw).int_member==4);
  BOOST_CHECK(cid(tw).int_member==4);
  BOOST_CHECK(k_m(tw)==4);
  BOOST_CHECK(ck_m(tw)==4);
#endif

  k_m(tw)=5;
  BOOST_CHECK(id(ctw).int_member==5);
  BOOST_CHECK(cid(ctw).int_member==5);
  BOOST_CHECK(k_m(ctw)==5);
  BOOST_CHECK(ck_m(ctw)==5);

  BOOST_CHECK(k_cm(t)==0);

#if !defined(BOOST_NO_MEMBER_TEMPLATES)||defined(BOOST_MSVC6_MEMBER_TEMPLATES)
  BOOST_CHECK(k_cm(tp)==0);
  BOOST_CHECK(k_cm(ctp)==0);
  BOOST_CHECK(k_cm(tpp)==0);
  BOOST_CHECK(k_cm(ctpp)==0);
  BOOST_CHECK(k_cm(tap)==0);
  BOOST_CHECK(k_cm(ctap)==0);
#endif

  BOOST_CHECK(k_cm(tw)==0);
  BOOST_CHECK(k_cm(ctw)==0);

  BOOST_CHECK(k_cmf(t));

#if !defined(BOOST_NO_MEMBER_TEMPLATES)||defined(BOOST_MSVC6_MEMBER_TEMPLATES)
  BOOST_CHECK(k_cmf(tp));
  BOOST_CHECK(k_cmf(ctp));
  BOOST_CHECK(k_cmf(tpp));
  BOOST_CHECK(k_cmf(ctpp));
  BOOST_CHECK(k_cmf(tap));
  BOOST_CHECK(k_cmf(ctap));
#endif

  BOOST_CHECK(k_cmf(tw));
  BOOST_CHECK(k_cmf(ctw));

  BOOST_CHECK(!k_mf(t));

#if !defined(BOOST_NO_MEMBER_TEMPLATES)||defined(BOOST_MSVC6_MEMBER_TEMPLATES)
  BOOST_CHECK(!k_mf(tp));
  BOOST_CHECK(!k_mf(tpp));
  BOOST_CHECK(!k_mf(tap));
  BOOST_CHECK(!k_mf(tw));
#endif

#if !defined(BOOST_NO_MEMBER_TEMPLATES)||defined(BOOST_MSVC6_MEMBER_TEMPLATES)
  std::list<test_class> tl;
  for(int i=0;i<20;++i)tl.push_back(test_class(i));

  int j=0;
  for(std::list<test_class>::iterator it=tl.begin();it!=tl.end();++it){
    BOOST_CHECK(k_m(it)==j);
    BOOST_CHECK(k_cm(it)==j);
    BOOST_CHECK(k_cmf(it));
    BOOST_CHECK(!k_mf(it));
    ++j;
  }
#endif
}
