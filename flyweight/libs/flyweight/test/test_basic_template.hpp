/* Boost.Flyweight basic test template.
 *
 * Copyright 2006-2007 Joaquín M López Muñoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/flyweight for library home page.
 */

#ifndef BOOST_FLYWEIGHT_TEST_BASIC_TEMPLATE_HPP
#define BOOST_FLYWEIGHT_TEST_BASIC_TEMPLATE_HPP

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#include <boost/config.hpp> /* keep it first to prevent nasty warns in MSVC */
#include <boost/detail/lightweight_test.hpp>
#include <boost/mpl/apply.hpp>
#include <string>
#include <sstream>

#define LENGTHOF(array) (sizeof(array)/sizeof((array)[0]))

template<typename Flyweight,typename ForwardIterator>
void test_basic_template(
  ForwardIterator first,ForwardIterator last
  BOOST_APPEND_EXPLICIT_TEMPLATE_TYPE(Flyweight))
{
  typedef typename Flyweight::value_type value_type;

  ForwardIterator it;

  for(it=first;it!=last;++it){
    /* construct/copy/destroy */

    Flyweight f1(*it);
    Flyweight f2;
    Flyweight c1(f1);
    Flyweight c2(static_cast<const Flyweight&>(f2));
    BOOST_TEST((f1==f2)==(f1.get()==value_type()));
    BOOST_TEST(f1==c1);
    BOOST_TEST(f2==c2);

    f1=f1;
    BOOST_TEST(f1==f1);

    c1=f2;
    BOOST_TEST(c1==f2);

    c1=f1;
    BOOST_TEST(c1==f1);

    /* convertibility to underlying type */

    value_type v1(f1);
    BOOST_TEST(v1==f1.get());

    /* identity of reference */

    BOOST_TEST(&f1.get()==&c1.get());

    /* modifiers */

    f1.swap(f1);
    BOOST_TEST(f1==c1);

    f1.swap(f2);
    BOOST_TEST(f1==c2);
    BOOST_TEST(f2==c1);

    boost::flyweights::swap(f1,f2);
    BOOST_TEST(f1==c1);
    BOOST_TEST(f2==c2);

    /* specialized algorithms */

    std::ostringstream oss1;
    oss1<<f1;
    std::ostringstream oss2;
    oss2<<f1.get();
    BOOST_TEST(oss1.str()==oss2.str());

    std::istringstream iss1(oss1.str());
    Flyweight f3;
    iss1>>f3;
    std::istringstream iss2(oss2.str());
    value_type v3;
    iss2>>v3;
    BOOST_TEST(f3.get()==v3);
  }
}

template<
  typename Flyweight1,typename Flyweight2,
  typename ForwardIterator1,typename ForwardIterator2
>
void test_basic_comparison_template(
  ForwardIterator1 first1,ForwardIterator1 last1,
  ForwardIterator2 first2
  BOOST_APPEND_EXPLICIT_TEMPLATE_TYPE(Flyweight1)
  BOOST_APPEND_EXPLICIT_TEMPLATE_TYPE(Flyweight2))
{
  typedef typename Flyweight1::value_type value_type1;
  typedef typename Flyweight2::value_type value_type2;

  for(;first1!=last1;++first1,++first2){
    value_type1 v1=*first1;
    value_type2 v2=*first2;
    Flyweight1  f1(v1);
    Flyweight2  f2(v2);

    BOOST_TEST((f1==f2)==(f1.get()==v2));
    BOOST_TEST((f1< f2)==(f1.get()< v2));
    BOOST_TEST((f1!=f2)==(f1.get()!=v2));
    BOOST_TEST((f1> f2)==(f1.get()> v2));
    BOOST_TEST((f1>=f2)==(f1.get()>=v2));
    BOOST_TEST((f1<=f2)==(f1.get()<=v2));

#if !defined(BOOST_NO_FUNCTION_TEMPLATE_ORDERING)
    BOOST_TEST((f1==v2)==(f1.get()==v2));
    BOOST_TEST((f1< v2)==(f1.get()< v2));
    BOOST_TEST((f1!=v2)==(f1.get()!=v2));
    BOOST_TEST((f1> v2)==(f1.get()> v2));
    BOOST_TEST((f1>=v2)==(f1.get()>=v2));
    BOOST_TEST((f1<=v2)==(f1.get()<=v2));

    BOOST_TEST((v1==f2)==(f1.get()==v2));
    BOOST_TEST((v1< f2)==(f1.get()< v2));
    BOOST_TEST((v1!=f2)==(f1.get()!=v2));
    BOOST_TEST((v1> f2)==(f1.get()> v2));
    BOOST_TEST((v1>=f2)==(f1.get()>=v2));
    BOOST_TEST((v1<=f2)==(f1.get()<=v2));
#endif

  }
}

template<typename FlyweightSpecifier>
void test_basic_template(BOOST_EXPLICIT_TEMPLATE_TYPE(FlyweightSpecifier))
{
  typedef typename boost::mpl::apply1<
    FlyweightSpecifier,int
  >::type int_flyweight;

  typedef typename boost::mpl::apply1<
    FlyweightSpecifier,std::string
  >::type string_flyweight;

  typedef typename boost::mpl::apply1<
    FlyweightSpecifier,char
  >::type char_flyweight;

  int ints[]={0,1,1,0,1,2,3,4,3,4,0,0};
  test_basic_template<int_flyweight>(&ints[0],&ints[0]+LENGTHOF(ints));

  const char* words[]={"hello","boost","flyweight","boost","bye","c++","c++"};
  test_basic_template<string_flyweight>(&words[0],&words[0]+LENGTHOF(words));

  char chars[]={0,2,4,5,1,1,1,3,4,1,1,0};
  test_basic_comparison_template<int_flyweight,char_flyweight>(
    &ints[0],&ints[0]+LENGTHOF(ints),&chars[0]);

  test_basic_comparison_template<string_flyweight,string_flyweight>(
    &words[0],&words[0]+LENGTHOF(words),&words[0]);
}

#define BOOST_FLYWEIGHT_TEST_BASIC(FlyweightSpecifier)              \
{                                                                   \
  /* Without these explicit instantiations, MSVC++ 6.5/7.0 does not \
   * find some friend operators in certain contexts.                \
   */                                                               \
                                                                    \
  typedef boost::mpl::apply1<                                       \
    FlyweightSpecifier,int                                          \
  >::type int_flyweight;                                            \
                                                                    \
  typedef boost::mpl::apply1<                                       \
    FlyweightSpecifier,std::string                                  \
  >::type string_flyweight;                                         \
                                                                    \
  typedef boost::mpl::apply1<                                       \
    FlyweightSpecifier,char                                         \
  >::type char_flyweight;                                           \
                                                                    \
  int_flyweight    f1;f1=f1;                                        \
  string_flyweight f2;f2=f2;                                        \
  char_flyweight   f3;f3=f3;                                        \
                                                                    \
  test_basic_template<FlyweightSpecifier>();                        \
}

#undef LENGTHOF

#endif
