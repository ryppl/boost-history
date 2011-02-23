//Purpose:
//  See if code from the following post works.
/*
From: itaj sherman <itajsherman@gmail.com>
Newsgroups: comp.lang.c++
Subject: Re: Multimethods idioms and library support
Date: Tue, 22 Feb 2011 13:23:05 -0800 (PST)
Organization: http://groups.google.com
Lines: 105
Message-ID: <3f848282-d123-408d-8d25-de12b0f2eca7@u6g2000vbh.googlegroups.com>
 */
//
#include <boost/composite_storage/pack/multiple_dispatch/reify_apply.hpp>
#include <boost/composite_storage/pack/multiple_dispatch/reifier_visitor.hpp>
#include <iostream>

namespace boost
{
namespace composite_storage
{
namespace pack
{
namespace multiple_dispatch
{
namespace testing
{

typedef void ResultType; 

struct object
//Super class of all classes.
{
      virtual 
    std::string 
  i_am_a()const=0
  //Returns the most derived class that
  //object is a member of.
  ;
  object()
  {}
 private:
  object(object const&)
  //Ensure that no copies are made when calling
  //functions. with object as arg.
  {}
}; 

//Predators:
  struct Lion;
  struct Anaconda;
  struct Bear;

struct Predator_abstract;
}//exit testing namespace

template<>
struct hosts_concrete
  < testing::Predator_abstract
  >
  : mpl::package
    < testing::Lion
    , testing::Bear 
    , testing::Anaconda
    >
{
};

template<>
struct host_abstract
  < testing::Lion
  >
{
      typedef
    testing::Predator_abstract
  type
  ;
};

template<>
struct host_abstract
  < testing::Bear
  >
{
      typedef
    testing::Predator_abstract
  type
  ;
};

template<>
struct host_abstract
  < testing::Anaconda
  >
{
      typedef
    testing::Predator_abstract
  type
  ;
};

namespace testing
{

struct Predator_abstract: object
{
  typedef reifier_visit_abstract_seq< ResultType
    , hosts_concrete<Predator_abstract>::type>
  visitor_abstract;

  virtual ResultType accept( visitor_abstract const&)const=0;
};

struct Lion: public Predator_abstract
{
  ResultType accept( visitor_abstract const& a_visitor)const
  {
    return a_visitor.visit(*this);
  }

    std::string
  i_am_a()const
  { return "Lion"
  ;}
};

struct Anaconda: public Predator_abstract
{
  ResultType accept( visitor_abstract const& a_visitor)const
  {
    return a_visitor.visit(*this);
  }
    std::string
  i_am_a()const
  { return "Anaconda"
  ;}
};

struct Bear: public Predator_abstract
{
  ResultType accept( visitor_abstract const& a_visitor)const
  {
    return a_visitor.visit(*this);
  }
    std::string
  i_am_a()const
  { return "Bear"
  ;}
};

//Preys:
  struct Gazelle;
  struct Girrafe;
  
struct Prey_abstract;  
}//exit namespace testing

template<>
struct hosts_concrete
  < testing::Prey_abstract
  >
  : mpl::package
    < testing::Gazelle
    , testing::Girrafe 
    >
{
};

template<>
struct host_abstract
  < testing::Gazelle
  >
{
      typedef
    testing::Prey_abstract
  type
  ;
};

template<>
struct host_abstract
  < testing::Girrafe
  >
{
      typedef
    testing::Prey_abstract
  type
  ;
};

namespace testing
{

struct Prey_abstract: object
{
  typedef reifier_visit_abstract_seq< ResultType
    , hosts_concrete<Prey_abstract>::type>
  visitor_abstract;

  virtual ResultType accept( visitor_abstract const&)const=0;
};

struct Gazelle: public Prey_abstract
{
  ResultType accept( visitor_abstract const& a_visitor)const
  {
    return a_visitor.visit(*this);
  }
    
    std::string
  i_am_a()const
  { return "Gazelle"
  ;}
};

struct Girrafe: public Prey_abstract
{
  ResultType accept( visitor_abstract const& a_visitor)const
  {
    return a_visitor.visit(*this);
  }
    std::string
  i_am_a()const
  { return "Gazelle"
  ;}
};

struct hunt_functor
{
  hunt_functor(){}
  
  typedef ResultType result_type;

  void operator()( Lion const&, Gazelle const& )const
  {
    std::cout<<"Lion jumps on Gazelle and bites its neck.\n";
  }

  void operator()( Lion const&, Girrafe const& )const
  {
    std::cout<<"Lion jumps on Girrafe and bites its ass.\n";
  }

  void operator()( Anaconda const&, Gazelle const& )const
  {
    std::cout<<"Anaconda injects venom into Gazelle.\n";
  }

  void operator()( Anaconda const&, Girrafe const& )const
  {
    std::cout<<"Anaconda ignores Girraffe.\n";
  }

  //Anaconda can't kill girrafes so no override for that one

  template<typename PreyConcrete>
  void operator()( Bear const&, PreyConcrete const& prey )const
  {
    std::cout<<"Bear mauls "<<prey.i_am_a()<<"\n";
  }

};

void test()
{
    hunt_functor const huntor;
    Lion lion;
    Girrafe girrafe;
  #if 0
    Predator_abstract const& pred1=lion;
    Prey_abstract const& prey1=girrafe;
    reify_apply<reifier_visitor>(huntor, pred1, prey1);
  #else
    reify_apply<reifier_visitor>(huntor, Lion(), Girrafe());
  #endif
}

}//exit testing namespace
}//exit multiple_dispatch namespace
}//exit pack namespace
}//exit composite_storage namespace
}//exit boost namespace

int main(void)
{
    boost::composite_storage::pack::multiple_dispatch::testing::test();
    return 0;
}    
