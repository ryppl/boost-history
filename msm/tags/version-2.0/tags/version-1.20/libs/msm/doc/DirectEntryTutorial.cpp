#include <vector>
#include <iostream>
#include "boost/mpl/vector/vector50.hpp"
#include <boost/msm/state_machine.hpp>

using namespace boost::msm;

namespace  // Concrete FSM implementation
{
    // events
    struct event1 {};
    struct event2 {};
    struct event3 {};
    struct event4 {};
    struct event5 {};
    struct event6 {};
    // Concrete FSM implementation 
    struct Fsm : public state_machine<Fsm>
    {
        // The list of FSM states
        struct State1 : public state<> 
        {
            // every (optional) entry/exit methods get the event passed.
            template <class Event>
            void on_entry(Event const& ) {std::cout << "entering: State1" << std::endl;}
            template <class Event>
            void on_exit(Event const& ) {std::cout << "leaving: State1" << std::endl;}
        };
        struct State2 : public state<> 
        {
            // every (optional) entry/exit methods get the event passed.
            template <class Event>
            void on_entry(Event const& ) {std::cout << "entering: State2" << std::endl;}
            template <class Event>
            void on_exit(Event const& ) {std::cout << "leaving: State2" << std::endl;}
        };
        struct SubFsm2 : public state_machine<SubFsm2>
        {
            //a fsm having exit pseudo states need a template constructor calling the one of the basic class
            template <class ContainingSM>
            SubFsm2(ContainingSM* sm):state_machine<SubFsm2>(sm){}

            template <class Event>
            void on_entry(Event const& ) {std::cout << "entering: SubFsm2" << std::endl;}
            template <class Event>
            void on_exit(Event const& ) {std::cout << "leaving: SubFsm2" << std::endl;}

            struct SubState1 : public state<>
            {
                template <class Event>
                void on_entry(Event const& ) {std::cout << "entering: SubFsm2::SubState1" << std::endl;}
                template <class Event>
                void on_exit(Event const& ) {std::cout << "leaving: SubFsm2::SubState1" << std::endl;}
            };
            struct SubState1b : public state<>
            {
                template <class Event>
                void on_entry(Event const& ) {std::cout << "entering: SubFsm2::SubState1b" << std::endl;}
                template <class Event>
                void on_exit(Event const& ) {std::cout << "leaving: SubFsm2::SubState1b" << std::endl;}
            };
            struct SubState2 : public state<> , public explicit_entry<SubFsm2,0>
            {
                template <class Event>
                void on_entry(Event const& ) {std::cout << "entering: SubFsm2::SubState2" << std::endl;}
                template <class Event>
                void on_exit(Event const& ) {std::cout << "leaving: SubFsm2::SubState2" << std::endl;}
            };
            struct SubState2b : public state<> , public explicit_entry<SubFsm2,1>
            {
                template <class Event>
                void on_entry(Event const& ) {std::cout << "entering: SubFsm2::SubState2b" << std::endl;}
                template <class Event>
                void on_exit(Event const& ) {std::cout << "leaving: SubFsm2::SubState2b" << std::endl;}
            };
            // test with a pseudo entry
            struct PseudoEntry1 : public entry_pseudo_state<SubFsm2,0>
            {
                template <class Event>
                void on_entry(Event const& ) {std::cout << "entering: SubFsm2::PseudoEntry1" << std::endl;}
                template <class Event>
                void on_exit(Event const& ) {std::cout << "leaving: SubFsm2::PseudoEntry1" << std::endl;}
            };
            struct SubState3 : public state<>
            {
                template <class Event>
                void on_entry(Event const& ) {std::cout << "entering: SubFsm2::SubState3" << std::endl;}
                template <class Event>
                void on_exit(Event const& ) {std::cout << "leaving: SubFsm2::SubState3" << std::endl;}
            };
            struct SubState3b : public state<>
            {
                template <class Event>
                void on_entry(Event const& ) {std::cout << "entering: SubFsm2::SubState3b" << std::endl;}
                template <class Event>
                void on_exit(Event const& ) {std::cout << "leaving: SubFsm2::SubState3b" << std::endl;}
            };
            struct PseudoExit1 : public exit_pseudo_state<SubFsm2,event5> 
            {
                template <class Event>
                void on_entry(Event const& ) {std::cout << "entering: SubFsm2::PseudoExit1" << std::endl;}
                template <class Event>
                void on_exit(Event const& ) {std::cout << "leaving: SubFsm2::PseudoExit1" << std::endl;}
            };
            // action methods
            void entry_action(event4 const&)
            {
                std::cout << "calling entry_action" << std::endl;
            }
            // the initial state. Must be defined
            typedef mpl::vector<SubState1,SubState1b> initial_state;

            typedef mpl::vector<SubState2b> explicit_creation;

            // Transition table for SubFsm2
            //TODO should not be necessary
            struct transition_table : mpl::vector<
                //    Start          Event         Next         Action				   Guard
                //  +--------------+-------------+------------+------------------------+----------------------+
                a_row < PseudoEntry1 , event4      , SubState3  ,&SubFsm2::entry_action                         >,
                _row < SubState2    , event6      , SubState1                                                  >,
                _row < SubState3    , event5      , PseudoExit1                                                >
                //  +--------------+-------------+------------+------------------------+----------------------+
            > {};
            // Replaces the default no-transition response.
            template <class Event>
            int no_transition(int state, Event const& e)
            {
                std::cout << "no transition in SubFsm2 from state " << state << " type:" << typeid(*get_state_by_id(state)).name()
                    << " on event " << typeid(e).name() << std::endl;
                return state;
            }
        };
        // the initial state of the player SM. Must be defined
        typedef State1 initial_state;

#ifdef __MWERKS__
    public: // Codewarrior bug workaround.  Tested at 0x3202
#endif 
        // transition actions
        // guard conditions


#ifdef __MWERKS__
    private:
#endif 

        // Transition table for Fsm
        struct transition_table : mpl::vector<
            //    Start                 Event         Next                                 Action  Guard
            //   +---------------------+-------------+------------------------------------+-------+-------+
            _row < State1              , event1       , SubFsm2                                           >,
            _row < State1              , event2       , SubFsm2::SubState2                                >,
            _row < State1              , event3       , mpl::vector<SubFsm2::SubState2,
            SubFsm2::SubState2b>                  >,
            _row < State1              , event4       , SubFsm2::PseudoEntry1                             >,
            //   +---------------------+-------------+------------------------------------+-------+-------+
            _row < SubFsm2             , event1       , State1                                            >,
            _row < SubFsm2::PseudoExit1, event5       , State2                                            >
            //   +---------------------+-------------+------------------------------------+-------+-------+
        > {};

        // Replaces the default no-transition response.
        template <class Event>
        int no_transition(int state, Event const& e)
        {
            std::cout << "no transition in Fsm from state " << state << " type:" << typeid(*get_state_by_id(state)).name()
                << " on event " << typeid(e).name() << std::endl;
            return state;
        }
    };

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
        std::cout << "processing fork to SubFsm2::SubState2 and SubFsm2::SubState2b" << std::endl;
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
