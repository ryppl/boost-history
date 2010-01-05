#include <vector>
#include <iostream>

#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/front/euml/euml.hpp>

using namespace std;
using namespace boost::msm::front::euml;
namespace msm = boost::msm;

// how long the timer will ring when countdown elapsed.
#define RINGING_TIME 5
// the different attribute indexes (more readable than just 0)
#define m_timer 0
#define m_tick 0
#define m_counter 0
#define m_ringing_cpt 0

namespace  // Concrete FSM implementation
{
    // events
    typedef BOOST_TYPEOF(build_attributes(attributes_ << int() /*m_timer*/ )) start_timer_attr;
    struct start_timer : euml_event<start_timer>, start_timer_attr
    {
        start_timer(int counter=0)
        {
            get_attribute<m_timer>()=counter;
        }
    };
    struct stop_timer : euml_event<stop_timer>{};

    typedef BOOST_TYPEOF(build_attributes(attributes_ << int() /*m_tick*/ )) tick_attr;
    struct tick : euml_event<tick>,tick_attr
    {
        tick(int counter=0)
        {
            get_attribute<m_tick>()=counter;
        }
    };
    struct start_ringing : euml_event<start_ringing>{};

    // Concrete FSM implementation 

    // The list of FSM states
    struct Stopped_Entry : euml_action<Stopped_Entry>
    {
        template <class Event,class FSM,class STATE>
        void operator()(Event const&,FSM&,STATE& )
        {
            std::cout << "entering: Stopped" << std::endl;
        }
    };
    typedef BOOST_TYPEOF(build_state( Stopped_Entry() )) Stopped;

    struct Started_Entry : euml_action<Started_Entry>
    {
        template <class Event,class FSM,class STATE>
        void operator()(Event const&,FSM&,STATE& )
        {
            std::cout << "entering: Started" << std::endl;
        }
    };
    typedef BOOST_TYPEOF(build_state( Started_Entry(),NoAction(), attributes_ << int()/*m_counter*/ )) Started;

    struct Ringing_Entry : euml_action<Ringing_Entry>
    {
        template <class Event,class FSM,class STATE>
        void operator()(Event const&,FSM&,STATE& )
        {
            std::cout << "entering: Ringing" << std::endl;
        }
    };
    typedef BOOST_TYPEOF(build_state( Ringing_Entry(), NoAction(), attributes_ << int()/*M_RINGING_CPT*/ )) Ringing;

    // replaces the old transition table
    typedef BOOST_TYPEOF(build_stt((
        //  +------------------------------------------------------------------------------+
          // When we start the countdown, the countdown value is not hardcoded but contained in the start_timer event.
          // We copy this value into Started
          Started()     == Stopped() + start_timer() / (Target_<m_counter>()= Event_<m_timer>())        ,
          Stopped()     == Started() + stop_timer()  ,
          // internal transition
          Started() + tick()
                    // we here use the message queue to move to Started when the countdown is finished
                    // to do this we put start_ringing into the message queue
                    / if_then_( (Source_<m_counter>() -= Event_<m_tick>()) <= Int_<0>(),
                                 process_(start_ringing()) )                                    ,
          // when we start ringing, we give to the state its hard-coded ringing time.
          Ringing()     == Started() + start_ringing() / (Target_<m_ringing_cpt>() = Int_<RINGING_TIME>()),
          // to change a bit, we now do not use the message queue but a transition conflict to solve the same problem.
          // When tick is fired, we have an internal transition Ringing -> Ringing, as long as Counter > 0
          Ringing() + tick() [ Source_<m_ringing_cpt>() - Event_<m_tick>() > Int_<0>() ] 
                             / (Source_<m_ringing_cpt>() -= Event_<m_tick>() )        ,
          // And we move to Stopped when the counter is 0
          Stopped()     == Ringing() + tick() [ Source_<m_ringing_cpt>()-Event_<m_tick>() <= Int_<0>() ],
          // we let the user manually stop the ringing by pressing any button
          Stopped()     == Ringing() + stop_timer()                                                     ,
          Stopped()     == Ringing() + start_timer()
          //  +------------------------------------------------------------------------------+
                    ) ) ) transition_table;

    // create a state machine "on the fly"
    typedef BOOST_TYPEOF(build_sm(  transition_table(), //STT
                                    init_ << Stopped() // Init
                                    )) SimpleTimer_;


    // choice of back-end
    typedef msm::back::state_machine<SimpleTimer_> SimpleTimer;

    //
    // Testing utilities.
    //
    static char const* const state_names[] = { "Stopped", "Started","Ringing" };
    void pstate(SimpleTimer const& p)
    {
        std::cout << " -> " << state_names[p.current_state()[0]] << std::endl;
    }

    void test()
    {        
		SimpleTimer p;
        // needed to start the highest-level SM. This will call on_entry and mark the start of the SM
        p.start();

        p.process_event(start_timer(5));pstate(p); //timer set to 5 ticks
        p.process_event(tick(2));pstate(p);
        p.process_event(tick(1));pstate(p);
        p.process_event(tick(1));pstate(p);
        p.process_event(tick(1));pstate(p);
        // we are now ringing, let it ring a bit
        p.process_event(tick(2));pstate(p);
        p.process_event(tick(1));pstate(p);
        p.process_event(tick(1));pstate(p);
        p.process_event(tick(1));pstate(p);
    }
}

int main()
{
    test();
    return 0;
}
