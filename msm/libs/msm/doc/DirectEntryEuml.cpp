#include <vector>
#include <iostream>

#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/front/euml/euml.hpp>

using namespace std;
using namespace boost::msm::front::euml;
namespace msm = boost::msm;


namespace  // Concrete FSM implementation
{
    // events
    struct event1 : euml_event<event1> {};
    struct event2 : euml_event<event2> {};
    struct event3 : euml_event<event3> {};
    struct event4 : euml_event<event4> {};
    struct event5 : euml_event<event5> {};
    struct event6 : euml_event<event6>
    {
        event6(){}
        template <class Event>
        event6(Event const&){}
    };

    //Sub fsm state definition
	struct SubState1_Entry : euml_action<SubState1_Entry>
	{
		template <class Event,class FSM,class STATE>
		void operator()(Event const&,FSM&,STATE& )
		{
			std::cout << "entering: SubFsm2::SubState1" << std::endl;
		}
	};
	struct SubState1_Exit : euml_action<SubState1_Exit>
	{
		template <class Event,class FSM,class STATE>
		void operator()(Event const&,FSM&,STATE& )
		{
			std::cout << "leaving: SubFsm2::SubState1" << std::endl;
		}
	};
    typedef BOOST_TYPEOF(build_state(SubState1_Entry(),SubState1_Exit())) SubState1;

	struct SubState1b_Entry : euml_action<SubState1b_Entry>
	{
		template <class Event,class FSM,class STATE>
		void operator()(Event const&,FSM&,STATE& )
		{
			std::cout << "entering: SubFsm2::SubState1b" << std::endl;
		}
	};
	struct SubState1b_Exit : euml_action<SubState1b_Exit>
	{
		template <class Event,class FSM,class STATE>
		void operator()(Event const&,FSM&,STATE& )
		{
			std::cout << "leaving: SubFsm2::SubState1b" << std::endl;
		}
	};
    typedef BOOST_TYPEOF(build_state(SubState1b_Entry(),SubState1b_Exit())) SubState1b;

	struct SubState1c_Entry : euml_action<SubState1c_Entry>
	{
		template <class Event,class FSM,class STATE>
		void operator()(Event const&,FSM&,STATE& )
		{
			std::cout << "entering: SubFsm2::SubState1c" << std::endl;
		}
	};
	struct SubState1c_Exit : euml_action<SubState1c_Exit>
	{
		template <class Event,class FSM,class STATE>
		void operator()(Event const&,FSM&,STATE& )
		{
			std::cout << "leaving: SubFsm2::SubState1c" << std::endl;
		}
	};
    typedef BOOST_TYPEOF(build_state(SubState1c_Entry(),SubState1c_Exit())) SubState1c;

	struct SubState2_Entry : euml_action<SubState2_Entry>
	{
		template <class Event,class FSM,class STATE>
		void operator()(Event const&,FSM&,STATE& )
		{
			std::cout << "entering: SubFsm2::SubState2" << std::endl;
		}
	};
	struct SubState2_Exit : euml_action<SubState2_Exit>
	{
		template <class Event,class FSM,class STATE>
		void operator()(Event const&,FSM&,STATE& )
		{
			std::cout << "leaving: SubFsm2::SubState2" << std::endl;
		}
	};
    typedef BOOST_TYPEOF(build_explicit_entry_state<0>(SubState2_Entry(),SubState2_Exit())) SubState2;

	struct SubState2b_Entry : euml_action<SubState2b_Entry>
	{
		template <class Event,class FSM,class STATE>
		void operator()(Event const&,FSM&,STATE& )
		{
			std::cout << "entering: SubFsm2::SubState2b" << std::endl;
		}
	};
    struct SubState2b_Exit : euml_action<SubState2b_Exit>
	{
		template <class Event,class FSM,class STATE>
		void operator()(Event const&,FSM&,STATE& )
		{
			std::cout << "leaving: SubFsm2::SubState2b" << std::endl;
		}
	};
    typedef BOOST_TYPEOF(build_explicit_entry_state<1>(SubState2b_Entry(),SubState2b_Exit())) SubState2b;

	struct SubState2c_Entry : euml_action<SubState2c_Entry>
	{
		template <class Event,class FSM,class STATE>
		void operator()(Event const&,FSM&,STATE& )
		{
			std::cout << "entering: SubFsm2::SubState2c" << std::endl;
		}
	};
    struct SubState2c_Exit : euml_action<SubState2c_Exit>
	{
		template <class Event,class FSM,class STATE>
		void operator()(Event const&,FSM&,STATE& )
		{
			std::cout << "leaving: SubFsm2::SubState2c" << std::endl;
		}
	};
    typedef BOOST_TYPEOF(build_explicit_entry_state<2>(SubState2c_Entry(),SubState2c_Exit())) SubState2c;

	struct PseudoEntry1_Entry : euml_action<PseudoEntry1_Entry>
	{
		template <class Event,class FSM,class STATE>
		void operator()(Event const&,FSM&,STATE& )
		{
			std::cout << "entering: SubFsm2::PseudoEntry1" << std::endl;
		}
	};
    struct PseudoEntry1_Exit : euml_action<PseudoEntry1_Exit>
	{
		template <class Event,class FSM,class STATE>
		void operator()(Event const&,FSM&,STATE& )
		{
			std::cout << "leaving: SubFsm2::PseudoEntry1" << std::endl;
		}
	};
    typedef BOOST_TYPEOF(build_entry_state<0>(PseudoEntry1_Entry(),PseudoEntry1_Exit())) PseudoEntry1;

	struct SubState3_Entry : euml_action<SubState3_Entry>
	{
		template <class Event,class FSM,class STATE>
		void operator()(Event const&,FSM&,STATE& )
		{
			std::cout << "entering: SubFsm2::SubState3" << std::endl;
		}
	};
	struct SubState3_Exit : euml_action<SubState3_Exit>
	{
		template <class Event,class FSM,class STATE>
		void operator()(Event const&,FSM&,STATE& )
		{
			std::cout << "leaving: SubFsm2::SubState3" << std::endl;
		}
	};
    typedef BOOST_TYPEOF(build_state(SubState3_Entry(),SubState3_Exit())) SubState3;

	struct SubState3b_Entry : euml_action<SubState3b_Entry>
	{
		template <class Event,class FSM,class STATE>
		void operator()(Event const&,FSM&,STATE& )
		{
			std::cout << "entering: SubFsm2::SubState3b" << std::endl;
		}
	};
	struct SubState3b_Exit : euml_action<SubState3b_Exit>
	{
		template <class Event,class FSM,class STATE>
		void operator()(Event const&,FSM&,STATE& )
		{
			std::cout << "leaving: SubFsm2::SubState3b" << std::endl;
		}
	};
    typedef BOOST_TYPEOF(build_state(SubState3b_Entry(),SubState3b_Exit())) SubState3b;

	struct PseudoExit1_Entry : euml_action<PseudoExit1_Entry>
	{
		template <class Event,class FSM,class STATE>
		void operator()(Event const&,FSM&,STATE& )
		{
			std::cout << "entering: SubFsm2::PseudoExit1" << std::endl;
		}
	};
	struct PseudoExit1_Exit : euml_action<PseudoExit1_Exit>
	{
		template <class Event,class FSM,class STATE>
		void operator()(Event const&,FSM&,STATE& )
		{
			std::cout << "leaving: SubFsm2::PseudoExit1" << std::endl;
		}
	};
    typedef BOOST_TYPEOF(build_exit_state(event6(),PseudoExit1_Entry(),PseudoExit1_Exit())) PseudoExit1;

    // actions
	struct entry_action : euml_action<entry_action>
	{
		template <class FSM,class EVT,class SourceState,class TargetState>
		void operator()(FSM& ,EVT const& ,SourceState& ,TargetState& )
		{
			cout << "calling entry_action" << endl;
		}
	};
    // SubFsm definition
	struct SubFsm2_Entry : euml_action<SubFsm2_Entry>
	{
		template <class Event,class FSM,class STATE>
		void operator()(Event const&,FSM&,STATE& )
		{
			std::cout << "entering: SubFsm2" << std::endl;
		}
	};
	struct SubFsm2_Exit : euml_action<SubFsm2_Exit>
	{
		template <class Event,class FSM,class STATE>
		void operator()(Event const&,FSM&,STATE& )
		{
			std::cout << "leaving: SubFsm2" << std::endl;
		}
	};
    typedef BOOST_TYPEOF(build_stt(
        //  +------------------------------------------------------------------------------+
        (   PseudoEntry1() + event4()  == SubState3()  / entry_action() ,
            SubState2()    + event6()  == SubState1()                   ,
            SubState3()    + event5()  == PseudoExit1()
        //  +------------------------------------------------------------------------------+
        ) ) ) SubFsm2_transition_table;

    struct SubFsm2_ : public BOOST_TYPEOF(build_sm(  SubFsm2_transition_table(), //STT
                                                     init_ << SubState1() << SubState1b() << SubState1c(), // Init State
                                                     SubFsm2_Entry(), // Entry
                                                     SubFsm2_Exit()
                                                    )) 
    {
        typedef mpl::vector<SubState2b,SubState2c> explicit_creation;
    };


    // back-end
    typedef msm::back::state_machine<SubFsm2_> SubFsm2;

    // Fsm state definitions
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
    typedef BOOST_TYPEOF(build_state(State1_Entry(),State1_Exit())) State1;

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
    typedef BOOST_TYPEOF(build_state(State2_Entry(),State2_Exit())) State2;

    // Fsm definition
    typedef BOOST_TYPEOF(build_stt(
        //  +------------------------------------------------------------------------------+
        (   State1()                + event1()  == SubFsm2()                        ,
            State1()                + event2()  == SubFsm2::direct<SubState2>()     ,
            State1()                + event3()  == (SubFsm2::direct<SubState2>(),SubFsm2::direct<SubState2b>(),
                                                    SubFsm2::direct<SubState2c>()),
            State1()                + event4()  == SubFsm2::entry_pt<PseudoEntry1>(),
            SubFsm2()               + event1()  == State1()                         ,
            SubFsm2::exit_pt
                    <PseudoExit1>() + event6()  == State2()
        //  +------------------------------------------------------------------------------+
        ) ) ) transition_table;

	struct Log_No_Transition : euml_action<Log_No_Transition>
	{
		template <class Event,class FSM,class STATE>
		void operator()(Event const& e,FSM&,STATE& )
		{
            std::cout << "no transition in Fsm"
                << " on event " << typeid(e).name() << std::endl;
		}
	};
    typedef BOOST_TYPEOF(build_sm(  transition_table(), //STT
                                    init_ << State1(), // Init State
                                    NoAction(), // Entry
                                    NoAction(), // Exit
                                    attributes_ << no_attributes_, // Attributes
                                    configure_<< no_configure_, // Flags, Deferred events, configuration
                                    Log_No_Transition() // no_transition handler
                                    )) Fsm_;
    //back-end
    typedef msm::back::state_machine<Fsm_> Fsm;

    //
    // Testing utilities.
    //
    static char const* const state_names[] = { "State1", "SubFsm2","State2" };
    void pstate(Fsm const& p)
    {
        std::cout << " -> " << state_names[p.current_state()[0]] << std::endl;
    }

    void test()
    {
        Fsm p;
        // needed to start the highest-level SM. This will call on_entry and mark the start of the SM
        p.start(); 
        std::cout << "Simply move in and out of the composite, activate init states" << std::endl;
        p.process_event(event1()); pstate(p);
        p.process_event(event1()); pstate(p);
        std::cout << "Direct entry into SubFsm2::SubState2, then transition to SubState1 and back to State1" << std::endl;
        p.process_event(event2()); pstate(p);
        p.process_event(event6()); pstate(p);
        p.process_event(event1()); pstate(p);
        std::cout << "processing fork to SubFsm2::SubState2, SubFsm2::SubState2b and SubFsm2::SubState2c" << std::endl;
        p.process_event(event3()); pstate(p);
        p.process_event(event1()); pstate(p);
        std::cout << "processing entry pseudo state" << std::endl;
        p.process_event(event4()); pstate(p);
        p.process_event(event1()); pstate(p);
        std::cout << "processing entry + exit pseudo state" << std::endl;
        p.process_event(event4()); pstate(p);
        std::cout << "using exit pseudo state" << std::endl;
        p.process_event(event5()); pstate(p);
    }
}

int main()
{
    test();
    return 0;
}
