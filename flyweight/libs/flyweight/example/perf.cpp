/* Boost.Flyweight example of performance comparison.
 *
 * Copyright 2006-2007 Joaquín M López Muñoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/flyweight for library home page.
 */

#include <boost/flyweight/flyweight.hpp>
#include <boost/flyweight/hashed_factory.hpp>
#include <boost/flyweight/set_factory.hpp>
#include <boost/flyweight/static_holder.hpp>
#include <boost/flyweight/simple_locking.hpp>
#include <boost/flyweight/refcounted.hpp>
#include <boost/flyweight/no_tracking.hpp>
#include <boost/tokenizer.hpp>
#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#if defined(BOOST_NO_STDC_NAMESPACE)
namespace std{
using ::clock;
using ::clock_t;
using ::exit;
}
#endif

using namespace boost::flyweights;

/* Instrumented allocator family keeping track of the memory in
 * current use.
 */

std::size_t count_allocator_mem=0;

template<typename T>
class count_allocator
{
public:
  typedef std::size_t     size_type;
  typedef std::ptrdiff_t  difference_type;
  typedef T*              pointer;
  typedef const T*        const_pointer;
  typedef T&              reference;
  typedef const T&        const_reference;
  typedef T               value_type;
  template<class U>struct rebind{typedef count_allocator<U> other;};

  count_allocator(){}
  count_allocator(const count_allocator<T>&){}
  template<class U>count_allocator(const count_allocator<U>&,int=0){}

  pointer       address(reference x)const{return &x;}
  const_pointer address(const_reference x)const{return &x;}

  pointer allocate(size_type n,const void* =0)
  {
    pointer p=(T*)(new char[n*sizeof(T)]);
    count_allocator_mem+=n*sizeof(T);
    return p;
  }

  /* Dinkumware stdlib implementation for MSVC++ 6.0/7.0 relies in this
   * non standard extension to the allocator interface to cope with
   * rebinding problems.
   */

  char* _Charalloc(size_type n)
  {
    char* p=new char[n];
    count_allocator_mem+=n;
    return p;
  }

  void deallocate(void* p,size_type n)
  {
    count_allocator_mem-=n*sizeof(T);
    delete [](char *)p;
  }

  size_type max_size() const{return (size_type )(-1);}
  void      construct(pointer p,const T& val){new(p)T(val);}
  void      destroy(pointer p){p->~T();}

  friend bool operator==(const count_allocator&,const count_allocator&)
  {
    return true;
  }

  friend bool operator!=(const count_allocator&,const count_allocator&)
  {
    return false;
  }
};

template<>
class count_allocator<void>
{
public:
  typedef void*           pointer;
  typedef const void*     const_pointer;
  typedef void            value_type;
  template<class U>struct rebind{typedef count_allocator<U> other;};
};

/* Define some count_allocator-based types and Boost.Flyweight components */

typedef std::basic_string<
  char,std::char_traits<char>,count_allocator<char>
> count_string;

#if !defined(BOOST_MPL_CFG_NO_FULL_LAMBDA_SUPPORT)
typedef hashed_factory<
  boost::hash<count_string>,
  std::equal_to<count_string>,
  count_allocator<count_string>
> count_hashed_factory;

typedef set_factory<
  std::less<count_string>,
  count_allocator<count_string>
> count_set_factory;
#else
/* Compilers without proper Boost.MPL lambda support (most notably,
 * MSVC++ 6.0/7.0) can't handle the types count_hashed_factory and
 * count_set_factory defined above because of a clash of the
 * nested symbol rebind in count_allocator with Boost.MPL internal
 * machinery. Instead, we can define the types as factory specifiers.
 */

struct count_hashed_factory:factory_marker
{
  template<typename Entry,typename Value>
  struct apply
  {
    typedef hashed_factory_class<
      Entry,Value,
      boost::hash<Value>,
      std::equal_to<Value>,
      count_allocator<Entry>
    > type;
  };
};

struct count_set_factory:factory_marker
{
  template<typename Entry,typename Value>
  struct apply
  {
    typedef set_factory_class<
      Entry,Value,
      std::less<Value>,
      count_allocator<Entry>
    > type;
  };
};
#endif

/* Measure time and memory performance for a String, which is assumed
 * to be either a plain string type or a string flyweight.
 */

template<typename String>
struct test
{
  static void run(const std::string& file)
  {
    typedef std::vector<String,count_allocator<String> > count_vector;

    /* Define a tokenizer on std::istreambuf. */
  
    typedef std::istreambuf_iterator<char> char_iterator;
    typedef boost::tokenizer<
      boost::char_separator<char>,
      char_iterator
    >                                      tokenizer;

    std::ifstream ifs(file.c_str());
    if(!ifs){
      std::cout<<"can't open "<<file<<std::endl;
      std::exit(EXIT_FAILURE);
    }

    std::clock_t start=std::clock();
  
    /* Tokenize using space and common punctuaction as separators, and
     * keeping the separators.
     */
   
    tokenizer tok=tokenizer(
      char_iterator(ifs),char_iterator(),
      boost::char_separator<char>(
        "",
        "\t\n\r !\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~"));
    count_vector txt;
    for(tokenizer::iterator it=tok.begin();it!=tok.end();++it){
      txt.push_back(String(it->c_str()));
    }

    /* Do some text handling */

    std::sort(txt.begin(),txt.end());
    std::random_shuffle(txt.begin(),txt.end());

    std::cout<<"Time used:  "<<(double)(std::clock()-start)/CLOCKS_PER_SEC
             <<" s\n";
    std::cout<<"Bytes used: "<<count_allocator_mem<<"\n";
  }
};


/* table of test cases for the user to select from */

struct test_case
{
  const char* name;
  void (*test)(const std::string&);
};

/* MSVC++ 6.0/7.0 needs the following explicit instantiations for test<...>
 * to compile correctly.
 */

flyweight<count_string,count_hashed_factory>             dummy1;
flyweight<count_string,count_hashed_factory,no_tracking> dummy2;
flyweight<count_string,count_set_factory>                dummy3;
flyweight<count_string,count_set_factory,no_tracking>    dummy4;

test_case test_table[]=
{
  {
    "simple string",
    test<count_string>::run
  },
  {
    "flyweight, hashed factory",
    test<flyweight<count_string,count_hashed_factory> >::run
  },
  {
    "flyweight, hashed factory, no tracking",
    test<flyweight<count_string,count_hashed_factory,no_tracking> >::run
  },
  {
    "flyweight, set-based factory",
    test<flyweight<count_string,count_set_factory> >::run
  },
  {
    "flyweight, set-based factory, no tracking",
    test<flyweight<count_string,count_set_factory,no_tracking> >::run
  }
};

enum{num_test_cases=sizeof(test_table)/sizeof(test_case)};

int main()
{
  try{
    for(int i=0;i<num_test_cases;++i){
      std::cout<<i+1<<". "<<test_table[i].name<<"\n";
    }
    int option=-1;
    for(;;){
      std::cout<<"select option, enter to exit: ";
      std::string str;
      std::getline(std::cin,str);
      if(str.empty())std::exit(EXIT_SUCCESS);
      std::istringstream istr(str);
      istr>>option;
      if(option>=1&&option<=num_test_cases){
        --option; /* pass from 1-based menu to 0-based test_table */
        break;
      }
    }

    std::cout<<"enter file name: ";
    std::string file;
    std::getline(std::cin,file);
    test_table[option].test(file);
  }
  catch(const std::exception& e){
    std::cout<<"error: "<<e.what()<<"\n";
  }

  return 0;
}
