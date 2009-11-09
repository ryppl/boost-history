#include <vector>
#include <iostream>
#include "boost/mpl/vector/vector50.hpp"
#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/front/state_machine_def.hpp>

namespace msm = boost::msm;

namespace  // Concrete FSM implementation
{
    // events
    struct event1 {};
    struct event2 {};
    struct event3 {};
    struct event4 {};
    struct event5 {};
    struct event6 {};
    struct event7 {};
    // Concrete FSM implementation 
    struct Fsm_ : public msm::front::state_machine_def<Fsm_>
    {
        // The list of FSM states
        struct State1 : public msm::front::state<> 
        {
            // every (optional) entry/exit methods get the event passed.
            template <class Event,class FSM>
            void on_entry(Event const&,FSM& ) {std::cout << "entering: State1" << std::endl;}
            template <class Event,class FSM>
            void on_exit(Event const&,FSM& ) {std::cout << "leaving: State1" << std::endl;}
        };
        struct State2 : public msm::front::state<> 
        {
            // every (optional) entry/exit methods get the event passed.
            template <class Event,class FSM>
            void on_entry(Event const&,FSM& ) {std::cout << "entering: State2" << std::endl;}
            template <class Event,class FSM>
            void on_exit(Event const&,FSM& ) {std::cout << "leaving: State2" << std::endl;}
        };
        struct SubFsm2_ : public msm::front::state_machine_def<SubFsm2_>
        {
            template <class Event,class FSM>
            void on_entry(Event const&,FSM& ) {std::cout << "entering: SubFsm2" << std::endl;}
            template <class Event,class FSM>
            void on_exit(Event const&,FSM& ) {std::cout << "leaving: SubFsm2" << std::endl;}

            struct SubState1 : public msm::front::state<>
            {
                template <class Event,class FSM>
                void on_entry(Event const&,FSM& ) {std::cout << "entering: SubFsm2::SubState1" << std::endl;}
                template <class Event,class FSM>
                void on_exit(Event const&,FSM& ) {std::cout << "leaving: SubFsm2::SubState1" << std::endl;}
            };
            struct SubState1b : public msm::front::state<>
            {
                template <class Event,class FSM>
                void on_entry(Event const&,FSM& ) {std::cout << "entering: SubFsm2::SubState1b" << std::endl;}
                template <class Event,class FSM>
                void on_exit(Event const&,FSM& ) {std::cout << "leaving: SubFsm2::SubState1b" << std::endl;}
            };
            struct SubState2 : public msm::front::state<> , public msm::front::explicit_entry<0>
            {
                template <class Event,class FSM>
                void on_entry(Event const&,FSM& ) {std::cout << "entering: SubFsm2::SubState2" << std::endl;}
                template <class Event,class FSM>
                void on_exit(Event const&,FSM& ) {std::cout << "leaving: SubFsm2::SubState2" << std::endl;}
            };
            struct SubState2b : public msm::front::state<> , public msm::front::explicit_entry<1>
            {
                template <class Event,class FSM>
                void on_entry(Event const&,FSM& ) {std::cout << "entering: SubFsm2::SubState2b" << std::endl;}
                template <class Event,class FSM>
                void on_exit(Event const&,FSM& ) {std::cout << "leaving: SubFsm2::SubState2b" << std::endl;}
            };
            // test with a pseudo entry
            struct PseudoEntry1 : public msm::front::entry_pseudo_state<0>
            {
                template <class Event,class FSM>
                void on_entry(Event const&,FSM& ) {std::cout << "entering: SubFsm2::PseudoEntry1" << std::endl;}
                template <class Event,class FSM>
                void on_exit(Event const&,FSM& ) {std::cout << "leaving: SubFsm2::PseudoEntry1" << std::endl;}
            };
            struct SubState3 : public msm::front::state<>
            {
                template <class Event,class FSM>
                void on_entry(Event const&,FSM& ) {std::cout << "entering: SubFsm2::SubState3" << std::endl;}
                template <class Event,class FSM>
                void on_exit(Event const&,FSM& ) {std::cout << "leaving: SubFsm2::SubState3" << std::endl;}
            };
            struct SubState3b : public msm::front::state<>
            {
                template <class Event,class FSM>
                void on_entry(Event const&,FSM& ) {std::cout << "entering: SubFsm2::SubState3b" << std::endl;}
                template <class Event,class FSM>
                void on_exit(Event const&,FSM& ) {std::cout << "leaving: SubFsm2::SubState3b" << std::endl;}
            };
            struct PseudoExit1 : public msm::front::exit_pseudo_state<event5> 
            {
                template <class Event,class FSM>
                void on_entry(Event const&,FSM& ) {std::cout << "entering: SubFsm2::PseudoExit1" << std::endl;}
                template <class Event,class FSM>
                void on_exit(Event const&,FSM& ) {std::cout << "leaving: SubFsm2::PseudoExit1" << std::endl;}
            };
            // action methods
            void entry_action(event4 const&)
            {
                std::cout << "calling entry_action" << std::endl;
            }
            void fake_action(event4 const&)
            {
                std::cout << "calling fake_action" << std::endl;
            }
            bool some_guard(event4 const&)
            {
                bool res = false;
                std::cout << "calling fsm::some_guard with:"<< std::boolalpha << res << std::endl;
                return res;
            }
            // the initial state. Must be defined
            typedef mpl::vector<SubState1,SubState1b> initial_state;

            typedef mpl::vector<SubState2,SubState2b> explicit_creation;

            // Transition table for SubFsm2
            struct transition_table : mpl::vector<
                //    Start          Event         Next         Action				   Guard
                //  +--------------+-------------+------------+------------------------+----------------------+
              a_row < PseudoEntry1 , event4      , SubState3  ,&SubFsm2_::entry_action                         >,
                row < PseudoEntry1 , event4      , SubState3  ,&SubFsm2_::fake_action   , &SubFsm2_::some_guard >,
               _row < SubState3    , event5      , PseudoExit1                                                >
                //  +--------------+-------------+------------+------------------------+----------------------+
            > {};
            // Replaces the default no-transition response.
            template <class FSM,class Event>
            void no_transition(Event const& e, FSM&,int state)
            {
                std::cout << "no transition from state " << state
                    << " on event " << typeid(e).name() << std::endl;
            }
        };
        typedef msm::back::state_machine<SubFsm2_> SubFsm2;
        // the initial state of the player SM. Must be defined
        typedef State1 initial_state;
 
        // transition actions
        void fake_action(event5 const&)
        {
            std::cout << "calling fsm::fake_action" << std::endl;
        }
        // guard conditions
        bool some_guard(event5 const&)
        {
            bool res = false;
            std::cout << "calling fsm::some_guard with:"<< std::boolalpha << res << std::endl;
            return res;
        }
         bool guard_false(event6 const&)
        {
            std::cout << "calling fsm::guard_false"<< std::endl;
            return false;
        }

        // Transition table for Fsm
        struct transition_table : mpl::vector<
            //    Start     Event         Next      Action				Guard
            //  +---------+-------------+---------+---------------------+----------------------+
            _row < State1 , event1      , SubFsm2                                              >,
            _row < State1 , event2      , SubFsm2::direct<SubFsm2_::SubState2>                 >,
            _row < State1 , event3      , mpl::vector<SubFsm2::direct<SubFsm2_::SubState2>,
                                                      SubFsm2::direct<SubFsm2_::SubState2b> >  >,
            _row < State1 , event4      , SubFsm2::entry_pt<SubFsm2_::PseudoEntry1>            >,
            //  +---------+-------------+---------+---------------------+----------------------+
            _row < SubFsm2, event1      , State1                                               >,
            _row < SubFsm2::exit_pt<SubFsm2_::PseudoExit1> , event5  , State2                  >,
           g_row < State2 , event6      , State1                        , &Fsm_::guard_false   >,
           g_row < State2 , event6      , State1                        , &Fsm_::guard_false   >
            //  +---------+-------------+---------+---------------------+----------------------+
        > {};

        // Replaces the default no-transition response.
        template <class FSM,class Event>
        void no_transition(Event const& e, FSM&,int state)
        {
            std::cout << "no transition from state " << state
                << " on event " << typeid(e).name() << std::endl;
        }
    };
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
        // voluntary mistake to check that the transition is only taken if the exit state is active
        std::cout << "Simulate error. event5 is only valid through an exit pseudo state" << std::endl;
        p.process_event(event5()); pstate(p);
        p.process_event(event1()); pstate(p);
        std::cout << "Direct entry into SubFsm2::SubState2 and back" << std::endl;
        p.process_event(event2()); pstate(p);
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
        std::cout << "Simulate error. event5 is not valid" << std::endl;
        p.process_event(event5()); pstate(p);
        std::cout << "2 false guards" << std::endl;
        p.process_event(event6()); pstate(p);
   }
}

int main()
{
    test();
    return 0;
}
