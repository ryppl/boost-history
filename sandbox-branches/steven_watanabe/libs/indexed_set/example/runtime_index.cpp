/* Boost.IndexedSet example of an index specified at run-time.
 *
 * Copyright Joaquín M López Muñoz 2003-2004. Use, modification, and distribution
 * are subject to the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/indexed_set for library home page.
 */

#if !defined(NDEBUG)
#define BOOST_INDEXED_SET_ENABLE_INVARIANT_CHECKING
#define BOOST_INDEXED_SET_ENABLE_SAFE_MODE
#endif

#include <boost/config.hpp> /* keep it first to prevent some nasty warnings in MSVC */
#include <boost/detail/workaround.hpp>
#include <boost/indexed_set.hpp>
#include <boost/function.hpp>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

using namespace boost::indexed_sets;

/* Some (many) compilers do not cope with Boost.Lambda. If yours does,
 * you can take advantage of it by defining the macro
 *  BOOST_INDEXED_SET_EXAMPLE_RUNTIME_INDEX_DONT_USE_BOOST_LAMBDA
 * macro for them.
 */

#if BOOST_WORKAROUND(BOOST_MSVC,<1300)
#undef BOOST_INDEXED_SET_EXAMPLE_RUNTIME_INDEX_DONT_USE_BOOST_LAMBDA
#define BOOST_INDEXED_SET_EXAMPLE_RUNTIME_INDEX_DONT_USE_BOOST_LAMBDA
#endif

#if !defined(BOOST_INDEXED_SET_EXAMPLE_RUNTIME_INDEX_DONT_USE_BOOST_LAMBDA)
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>

using namespace boost::lambda;
#endif

/* This function computes a similarity distance between two strings,
 * where 0 means total disagreement and 1 is equality.
 * The Jaro-Winkler comparator is defined in:
 *   Winkler, W. E., Thibadeau, Y.: "An application of the Fellegi-
 *   Sunter model of record linkage to the 1990 U.S. decennial census",
 *   section 2.5, Report Number RR91/09, U.S. Census Bureau, 1987
 *   http://www.census.gov/srd/papers/pdf/rr91-9.pdf
 */

static double jaro_winkler(const std::string& str1_,const std::string& str2_)
{
  static const double JW_W1=1.0/3.0;
  static const double JW_W2=1.0/3.0;
  static const double JW_WT=1.0/3.0;

  std::string::size_type d=str1_.size();    /* length of first string */
  std::string::size_type r=str2_.size();    /* lenght of second string */
  std::string::size_type m=d>r?d:r;
  std::string::size_type m2=m>1?(m/2)-1:0; /* vicinity for matching common characters */

  /* The code is simpler is the first argument is guaranteed to be no
   * longer than the second. We get this effect trough additional references.
   */

  const std::string& str1=(d<=r)?str1_:str2_;
  const std::string& str2=(d<=r)?str2_:str1_;

  std::vector<char> ass1(m); /* table of assigned characters in str1 */
  std::vector<char> ass2(m); /* same for str2 */

  std::string::size_type c=0;  /* characters in common */
  std::string::size_type t=0;  /* transpositions */
  std::string::size_type a=0;  /* agreements in initial characters */


  for(std::string::size_type i=0;i<d;++i){
    int ch=str1[i];
    if(!ass2[i]&&ch==str2[i]){
      ass1[i]=ass2[i]=ch;
      ++c;
    }
    else{
      for(std::string::size_type j=1;j<=m2;++j)
      {
        if(i<r-j&&!ass2[i+j]&&ch==str2[i+j]){
          ass1[i]=ass2[i+j]=ch;
          ++c;
          break;
        }
        if(i>=j&&!ass2[i-j]&&ch==str2[i-j]){
          ass1[i]=ass2[i-j]=ch;
          ++c;
          break;
        }
      }
    }
  }

  std::string::size_type n1=0,n2=0;
  for(std::string::size_type n=0;n<c;++n){
    while(!ass1[n1])++n1;
    while(!ass2[n2])++n2;
    if(ass1[n1++]!=ass2[n2++]){
      ++t;
    }
  }
  t/=2;

  double phi=c?(JW_W1*c/d)+(JW_W2*c/r)+(JW_WT*(c-t)/c):0.0; /* Jaro comparator */

  for(a=0;a<4&&a<d&&str1[a]==str2[a];++a);

  return phi+a*0.1*(1-phi);
}

/* indexed_set of strings with a run-time index. FALTA MAS INFO
 */

typedef indexed_set<
  std::string,
  index_list<
    unique<
      identity<std::string>,
      boost::function2<bool,const std::string&,const std::string&>
    >
  >
> name_record_set;

/* if we don't have lambda support, use a hand-made comparer */

#if defined(BOOST_INDEXED_SET_EXAMPLE_RUNTIME_INDEX_DONT_USE_BOOST_LAMBDA)
struct jaro_winkler_comparer
{
  jaro_winkler_comparer(const std::string& ref_str_):ref_str(ref_str_){}

  bool operator()(const std::string& str1,const std::string& str2)const
  {
    return jaro_winkler(ref_str,str1)>jaro_winkler(ref_str,str2);
  }

private:
  std::string ref_str;
};
#endif

int main()
{
  std::cout<<"enter a name (e.g \"Robert Brown\")"<<std::endl;
  std::string name;
  std::getline(std::cin,name);

  /* Compute the index and construct the indexed_set.
   * FALTA MAS INFO
   */

  name_record_set::ctor_args_list args_list;
  name_record_set::ctor_args& args=boost::tuples::get<0>(args_list);

  /* fill the comparer */

#if !defined(BOOST_INDEXED_SET_EXAMPLE_RUNTIME_INDEX_DONT_USE_BOOST_LAMBDA)
  boost::tuples::get<0>(args)=bind(jaro_winkler,name,_1)>bind(jaro_winkler,name,_2);
#else
  boost::tuples::get<1>(args)=jaro_winkler_comparer(name);
#endif

  name_record_set ns(args_list);

  ns.insert("Joe Smith");
  ns.insert("Robert Nightingale");
  ns.insert("Robert Brown");
  ns.insert("Marc Tuxedo");

  std::cout<<"select a similary measure (1=perfect match, 0="<<std::endl;
  double similarity;
  std::cin>>similarity;
  if(similarity<0.0)similarity=0;else if(similarity>1.0)similarity=1.0;

  /* FALTA */

  std::copy(
    get<0>(ns).begin(),get<0>(ns).end(),
    std::ostream_iterator<std::string>(std::cout,"\n"));

  return 0;
}
