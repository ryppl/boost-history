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
#include <boost/bind.hpp>
#include <iostream>

namespace bmpl = boost::mpl;
namespace mdis = boost::composite_storage::pack::multiple_dispatch;
using namespace mdis;

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
#define OBJECT_IS_COPYABLE
#ifndef OBJECT_IS_COPYABLE
 private:
  object(object const&)
  //Ensure that no copies are made when calling
  //functions. with object as arg.
  {}
#endif  
}; 

//Predators:
  struct Lion;
  struct Anaconda;
  struct Bear;

struct Predator_abstract;

//Preys:
  struct Gazelle;
  struct Girrafe;
  
struct Prey_abstract;  

#define ACCEPT_CONSTANCY
#define VISIT_CONSTANCY const

namespace boost{
namespace composite_storage{
namespace pack{
namespace multiple_dispatch{

template<>
struct hosts_concrete
  < Predator_abstract
  >
  : bmpl::package
    < Lion
    , Bear
    , Anaconda
    >
{
};

template<>
struct hosts_concrete
  < Prey_abstract
  >
  : bmpl::package
    < Gazelle
    , Girrafe
    >
{
};

}}}}//exit boost::composite_storage::pack::multiple_dispatch namespace

struct Predator_abstract: object
{
      typedef 
    reifier_visit_abstract_seq
    < ResultType
    , hosts_concrete<Predator_abstract>::type
    >
  visitor_abstract
  ;

  virtual ResultType accept( visitor_abstract VISIT_CONSTANCY& a_visitor)ACCEPT_CONSTANCY=0;
};

struct Lion: public Predator_abstract
{
  ResultType accept( visitor_abstract VISIT_CONSTANCY& a_visitor)ACCEPT_CONSTANCY
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
  ResultType accept( visitor_abstract VISIT_CONSTANCY& a_visitor)ACCEPT_CONSTANCY
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
  ResultType accept( visitor_abstract VISIT_CONSTANCY& a_visitor)ACCEPT_CONSTANCY
  {
    return a_visitor.visit(*this);
  }
    std::string
  i_am_a()const
  { return "Bear"
  ;}
};
//--------------------------
struct Prey_abstract: object
{
      typedef 
    reifier_visit_abstract_seq
    < ResultType
    , hosts_concrete<Prey_abstract>::type
    >
  visitor_abstract
  ;

  virtual ResultType accept( visitor_abstract VISIT_CONSTANCY& a_visitor)ACCEPT_CONSTANCY=0;
};

struct Gazelle: public Prey_abstract
{
  ResultType accept( visitor_abstract VISIT_CONSTANCY& a_visitor)ACCEPT_CONSTANCY
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
  ResultType accept( visitor_abstract VISIT_CONSTANCY& a_visitor)ACCEPT_CONSTANCY
  {
    return a_visitor.visit(*this);
  }
    std::string
  i_am_a()const
  { return "Gazelle"
  ;}
};
//----------------------
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
    Predator_abstract ACCEPT_CONSTANCY& pred1=lion;
    Prey_abstract ACCEPT_CONSTANCY& prey1=girrafe;
    reify_apply<reifier_visitor>(huntor, pred1, prey1);
  #ifdef OBJECT_IS_COPYABLE
    auto huntor_lion_prey=boost::bind<hunt_functor::result_type>(huntor,Anaconda(),_1);
    reify_apply<reifier_visitor>(huntor_lion_prey,prey1);
    auto huntor_lion_girrafe=boost::bind<hunt_functor::result_type>(huntor,Bear(),Girrafe());
    reify_apply<reifier_visitor>(huntor_lion_girrafe);
    auto huntor_pred_girrafe=boost::bind<hunt_functor::result_type>(huntor,_1,Gazelle());
    reify_apply<reifier_visitor>(huntor_pred_girrafe,pred1);
  #endif
}

int main(void)
{
    test();
    return 0;
}    
