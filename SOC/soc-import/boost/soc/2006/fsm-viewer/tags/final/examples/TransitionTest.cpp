//////////////////////////////////////////////////////////////////////////////
// (c) Copyright Andreas Huber Doenni 2004-2006
// Distributed under the Boost Software License, Version 1.0. (See accompany-
// ing file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////



#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/null_exception_translator.hpp>
#include <boost/statechart/exception_translator.hpp>
#include <boost/statechart/state.hpp>
#include <boost/statechart/event.hpp>
#include <boost/statechart/transition.hpp>
#include <boost/statechart/custom_reaction.hpp>

#include <boost/mpl/list.hpp>

#include <typeinfo>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <stdexcept>



namespace sc = boost::statechart;
namespace mpl = boost::mpl;



typedef std::string ActionDescription();
typedef ActionDescription * ActionDescriptionPtr;
typedef std::vector< ActionDescriptionPtr > ActionDescriptionSequence;
typedef ActionDescriptionSequence::const_iterator SequenceIterator;
typedef void Action( ActionDescriptionSequence & );
typedef Action * ActionPtr;

template< class State >
std::string EntryDescription()
{
  static const std::string entry = "Entry: ";
  return entry + typeid( State ).name() + "\n";
}

template< class State >
std::string ExitFnDescription()
{
  static const std::string exitFunction = "exit(): ";
  return exitFunction + typeid( State ).name() + "\n";
}

template< class State >
std::string DtorDescription()
{
  static const std::string destructor = "Destructor: ";
  return destructor + typeid( State ).name() + "\n";
}

template< class Context, class Event >
std::string TransDescription()
{
  static const std::string transition = "Transition: ";
  static const std::string event = " with Event: ";
  return transition + typeid( Context ).name() +
    event + typeid( Event ).name() + "\n";
}

template< ActionPtr pAction >
std::string ThrowDescription()
{
  static const std::string throwing = "Throwing exception in ";
  static ActionDescriptionSequence dummy;
  dummy.clear();
  pAction( dummy );
  return throwing + dummy.front()();
}


template< class State >
void Entry( ActionDescriptionSequence & sequence )
{
  sequence.push_back( EntryDescription< State > );
}

template< class State >
void ExitFn( ActionDescriptionSequence & sequence )
{
  sequence.push_back( ExitFnDescription< State > );
}

template< class State >
void Dtor( ActionDescriptionSequence & sequence )
{
  sequence.push_back( DtorDescription< State > );
}

template< class State >
void Exit( ActionDescriptionSequence & sequence )
{
  ExitFn< State >( sequence );
  Dtor< State >( sequence );
}

template< class Context, class Event >
void Trans( ActionDescriptionSequence & sequence )
{
  sequence.push_back( TransDescription< Context, Event > );
}

template< ActionPtr pAction >
void Throw( ActionDescriptionSequence & sequence )
{
  sequence.push_back( ThrowDescription< pAction > );
}

const int arrayLength = 30;
typedef ActionPtr ActionArray[ arrayLength ];


class TransitionTestException : public std::runtime_error
{
  public:
    TransitionTestException() : std::runtime_error( "Oh la la!" ) {}
};


// This test state machine is a beefed-up version of the one presented in
// "Practical Statecharts in C/C++" by Miro Samek, CMP Books 2002
struct A : sc::event< A > {};
struct B : sc::event< B > {};
struct C : sc::event< C > {};
struct D : sc::event< D > {};
struct E : sc::event< E > {};
struct F : sc::event< F > {};
struct G : sc::event< G > {};
struct H : sc::event< H > {};


struct S0;
struct TransitionTest : sc::state_machine<
  TransitionTest, S0, std::allocator< void > >
{
  public:
    //////////////////////////////////////////////////////////////////////////
    TransitionTest() : pThrowAction_( 0 ), unconsumedEventCount_( 0 ) {}

    ~TransitionTest()
    {
      // Since state destructors access the state machine object, we need to
      // make sure that all states are destructed before this subtype
      // portion is destructed.
      this->terminate();
    }

    void ClearActualSequence()
    {
      actualSequence_.clear();
    }

    void ThrowAction( ActionPtr pThrowAction )
    {
      pThrowAction_ = pThrowAction;
    }

    template< class State >
    void ActualEntry()
    {
      StoreActualAction< &::Entry< State > >();
    }

    template< class State >
    void ActualExitFunction()
    {
      StoreActualAction< &::ExitFn< State > >();
    }
    
    template< class State >
    void ActualDestructor()
    {
      StoreActualAction< &::Dtor< State > >();
    }
    
    template< class Context, class Event >
    void ActualTransition()
    {
      StoreActualAction< &::Trans< Context, Event > >();
    }

    void unconsumed_event( const sc::event_base & )
    {
      ++unconsumedEventCount_;
    }

    unsigned int GetUnconsumedEventCount() const
    {
      return unconsumedEventCount_;
    }

  private:
    //////////////////////////////////////////////////////////////////////////
    template< ActionPtr pAction >
    void StoreActualAction()
    {
      if ( pAction == pThrowAction_ )
      {
        Throw< pAction >( actualSequence_ );
        throw TransitionTestException();
      }
      else
      {
        pAction( actualSequence_ );
      }
    }

    ActionPtr pThrowAction_;
    ActionDescriptionSequence actualSequence_;
    ActionDescriptionSequence expectedSequence_;
    unsigned int unconsumedEventCount_;
};

struct S1;
struct S211;

struct S0 : sc::state< S0, TransitionTest, S1 >
{
  typedef sc::state< S0, TransitionTest, S1 > my_base;
  public:
    typedef sc::transition< E, S211 > reactions;

    S0( my_base::my_context ctx ) : my_base( ctx ) {}

    void Transit( const A & evt ) { TransitImpl( evt ); }
    void Transit( const B & evt ) { TransitImpl( evt ); }
    void Transit( const C & evt ) { TransitImpl( evt ); }
    void Transit( const D & evt ) { TransitImpl( evt ); }
    void Transit( const F & evt ) { TransitImpl( evt ); }
    void Transit( const G & evt ) { TransitImpl( evt ); }
    void Transit( const H & evt ) { TransitImpl( evt ); }

  private:
    template< class Event >
    void TransitImpl( const Event & )
    {
      this->outermost_context().template ActualTransition< S0, Event >();
    }
};

  struct S11;
  struct S21;
  
  struct S2 : sc::state< S2, S0, S21 >
  {
    typedef sc::state< S2, S0, S21 > my_base;
    typedef mpl::list<
      sc::transition< C, S1, S0, &S0::Transit >,
      sc::transition< F, S11, S0, &S0::Transit >
    > reactions;

    S2( my_base::my_context ctx ) : my_base( ctx ) {}
  };

    
    struct S21 : sc::state< S21, S2, S211 >
    {
      typedef sc::state< S21, S2, S211 > my_base;
      typedef mpl::list<
        sc::transition< H, S21, S0, &S0::Transit >,
        sc::transition< B, S211, S0, &S0::Transit >
      > reactions;

      S21( my_base::my_context ctx ) : my_base( ctx ) {}
    };

      
      struct S211 : sc::state< S211, S21 >
      {
        typedef sc::state< S211, S21 > my_base;
        typedef mpl::list<
          sc::transition< D, S21, S0, &S0::Transit >,
          sc::transition< G, S0 >
        > reactions;

        S211( my_base::my_context ctx ) : my_base( ctx ) {}
      };

  
  struct S1 : sc::state< S1, S0, S11 >
  {
    typedef sc::state< S1, S0, S11 > my_base;
    typedef mpl::list<
      sc::transition< A, S1, S0, &S0::Transit >,
      sc::transition< B, S11, S0, &S0::Transit >,
      sc::transition< C, S2, S0, &S0::Transit >,
      sc::transition< D, S0 >,
      sc::transition< F, S211, S0, &S0::Transit >
    > reactions;

    S1( my_base::my_context ctx ) : my_base( ctx ) {}
  };

    
    struct S11 : sc::state< S11, S1 >
    {
      typedef sc::state< S11, S1 > my_base;
      typedef mpl::list<
        sc::transition< G, S211, S0, &S0::Transit >,
        sc::custom_reaction< H >
      > reactions;

      S11( my_base::my_context ctx ) : my_base( ctx ) {}

      sc::result react( const H & )
      {
        this->outermost_context().template ActualTransition< S11, H >();
        return this->discard_event();
      }
    };



void TestTransitions( TransitionTest & machine )
{
  machine.initiate();
}


int main()
{
  TransitionTest null_machine;
  TestTransitions( null_machine );

  return 0;
}
