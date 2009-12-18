#include <iostream>
// back-end
#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/front/euml/euml.hpp>

namespace msm = boost::msm;
using namespace boost::msm::front::euml;

namespace
{
    // events
    struct event1 : euml_event<event1> {};

    struct State1_Entry : euml_action<State1_Entry>
    {
        template <class Event,class FSM,class STATE>
        void operator()(Event const&,FSM&,STATE& )
        {
            std::cout << "entering: State1" << std::endl;
        }
    };
    struct State1_Exit : euml_action<State1_Exit>
    {
        template <class Event,class FSM,class STATE>
        void operator()(Event const&,FSM&,STATE& )
        {
            std::cout << "leaving: State1" << std::endl;
        }
    };
    struct State2_Entry : euml_action<State2_Entry>
    {
        template <class Event,class FSM,class STATE>
        void operator()(Event const&,FSM&,STATE& )
        {
            std::cout << "entering: State2" << std::endl;
        }
    };
    struct State2_Exit : euml_action<State2_Exit>
    {
        template <class Event,class FSM,class STATE>
        void operator()(Event const&,FSM&,STATE& )
        {
            std::cout << "leaving: State2" << std::endl;
        }
    };
    struct State3_Entry : euml_action<State3_Entry>
    {
        template <class Event,class FSM,class STATE>
        void operator()(Event const&,FSM&,STATE& )
        {
            std::cout << "entering: State3" << std::endl;
        }
    };
    struct State3_Exit : euml_action<State3_Exit>
    {
        template <class Event,class FSM,class STATE>
        void operator()(Event const&,FSM&,STATE& )
        {
            std::cout << "leaving: State3" << std::endl;
        }
    };
    struct State4_Entry : euml_action<State4_Entry>
    {
        template <class Event,class FSM,class STATE>
        void operator()(Event const&,FSM&,STATE& )
        {
            std::cout << "entering: State4" << std::endl;
        }
    };
    struct State4_Exit : euml_action<State4_Exit>
    {
        template <class Event,class FSM,class STATE>
        void operator()(Event const&,FSM&,STATE& )
        {
            std::cout << "leaving: State4" << std::endl;
        }
    };

    // The list of FSM states
    typedef BOOST_TYPEOF(build_state(State1_Entry(),State1_Exit())) State1;
    typedef BOOST_TYPEOF(build_state(State2_Entry(),State2_Exit())) State2;
    typedef BOOST_TYPEOF(build_state(State3_Entry(),State3_Exit())) State3;
    typedef BOOST_TYPEOF(build_state(State4_Entry(),State4_Exit())) State4;

    // transition actions
    struct State2ToState3 : euml_action<State2ToState3>
    {
        template <class FSM,class EVT,class SourceState,class TargetState>
        void operator()(EVT const& ,FSM&,SourceState& ,TargetState& )
        {
            std::cout << "my_machine::State2ToState3" << std::endl;
        }
    };
    struct State3ToState4 : euml_action<State3ToState4>
    {
        template <class FSM,class EVT,class SourceState,class TargetState>
        void operator()(EVT const& ,FSM&,SourceState& ,TargetState& )
        {
            std::cout << "my_machine::State3ToState4" << std::endl;
        }
    };
    // guard conditions
    struct always_true : euml_action<always_true>
    {
        template <class FSM,class EVT,class SourceState,class TargetState>
        bool operator()(EVT const& evt,FSM&,SourceState& ,TargetState& )
        {
            std::cout << "always_true" << std::endl;
            return true;
        }
    };
    struct always_false : euml_action<always_false>
    {
        template <class FSM,class EVT,class SourceState,class TargetState>
        bool operator()(EVT const& evt,FSM&,SourceState& ,TargetState& )
        {
            std::cout << "always_false" << std::endl;
            return false;
        }
    };
    // replaces the old transition table
    typedef BOOST_TYPEOF(build_stt
        ((State1()                   == State2()  ,
          State2()                   == State3() / State2ToState3(),
          State3()                   == State4() [always_true()] / State3ToState4(),
          State3()                   == State4() [always_false()],
          State4() + event1()        == State1()     
          //  +------------------------------------------------------------------------------+
                    ) ) ) transition_table;

    // create a state machine "on the fly"
    typedef BOOST_TYPEOF(build_sm(  transition_table(), //STT
                                    init_ << State1() // Init State
                                    )) my_machine_;

    // Pick a back-end
    typedef msm::back::state_machine<my_machine_> my_machine;

    //
    // Testing utilities.
    //
    static char const* const state_names[] = { "State1", "State2", "State3", "State4" };
    void pstate(my_machine const& p)
    {
        std::cout << " -> " << state_names[p.current_state()[0]] << std::endl;
    }

    void test()
    {        
		my_machine p;

        // needed to start the highest-level SM. This will call on_entry and mark the start of the SM
        // in this case it will also immediately trigger all anonymous transitions
        p.start(); 
        // this event will bring us back to the initial state and thus, a new "loop" will be started
        p.process_event(event1());
    }
}

int main()
{
    test();
    return 0;
}
