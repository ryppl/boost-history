#include <vector>
#include <iostream>
#include <boost/mpl/vector/vector50.hpp>
#include <boost/msm/state_machine.hpp>

using namespace boost::msm;

namespace {
    // events
    struct AnyBtnPressed {};
    struct AlarmBeeps30s {};
    struct P1 {};
    struct P2 {};
    struct P {};
    // Concrete FSM implementation
    struct Clock : public state_machine<Clock>
    {
        struct Displays : public state<> 
        {
            template <class Event>
            void on_entry(Event const& ) {std::cout << "Entering Displays"<< std::endl;}
            template <class Event>
            void on_exit(Event const& ) {std::cout << "Leaving Displays"<< std::endl;}
        };
        struct AlarmBeeps : public state_machine<AlarmBeeps>
        {            
            // states
            struct Alarm1Beeps : public state<>, public explicit_entry<AlarmBeeps,0>
            {
                template <class Event>
                void on_entry(Event const& ) {std::cout << "Entering Alarm1Beeps"<< std::endl;}
                template <class Event>
                void on_exit(Event const& ) {std::cout << "Leaving Alarm1Beeps"<< std::endl;}
            };
            struct Alarm2Beeps : public state<>, public explicit_entry<AlarmBeeps,0>
            {
                template <class Event>
                void on_entry(Event const& ) {std::cout << "Entering Alarm2Beeps"<< std::endl;}
                template <class Event>
                void on_exit(Event const& ) {std::cout << "Leaving Alarm2Beeps"<< std::endl;}
            };
            struct BothBeep : public state<>, public explicit_entry<AlarmBeeps,0>
            {
                template <class Event>
                void on_entry(Event const& ) {std::cout << "Entering BothBeep"<< std::endl;}
                template <class Event>
                void on_exit(Event const& ) {std::cout << "Leaving BothBeep"<< std::endl;}
            };

            // the initial state of the AlarmBeeps SM. Must be defined. Will not be used as we use only explicit entries
            typedef mpl::vector< Alarm1Beeps > initial_state;
            // as we have no transition table, we need to create the states explicitly
            typedef mpl::vector<Alarm1Beeps,Alarm2Beeps,BothBeep> explicit_creation;

            // Transition table for AlarmBeeps. Can be empty as no transition defined
            struct transition_table : mpl::vector<> {};
        };
        // the initial state of the Clock SM. Must be defined
        typedef Displays initial_state;
        // Transition table for Clock
        struct transition_table : mpl::vector<
            //    Start      Event          Next                      Action	Guard
            //  +-----------+-------------+-------------------------+----------+----------------------+
            _row  < Displays  , P1          , AlarmBeeps::Alarm1Beeps                                    >,
            _row  < Displays  , P2          , AlarmBeeps::Alarm2Beeps                                    >,
            _row  < Displays  , P           , AlarmBeeps::BothBeep                                       >,
            _row  < AlarmBeeps,AnyBtnPressed, Displays                                                   >,
            _row  < AlarmBeeps,AlarmBeeps30s, Displays                                                   >
            //  +-----------+-------------+--------------------------+----------+----------------------+
        > {};
    };
}

void test()
{
    Clock sm;
    sm.start();
    std::cout << "Sending P1" << std::endl;
    sm.process_event(P1());
    std::cout << "Sending AnyBtnPressed" << std::endl;
    sm.process_event(AnyBtnPressed());
    std::cout << "Sending P2" << std::endl;
    sm.process_event(P2());
    std::cout << "Sending AnyBtnPressed" << std::endl;
    sm.process_event(AnyBtnPressed());
    std::cout << "Sending P" << std::endl;
    sm.process_event(P());

}
int main()
{
    test();
    return 0;
}


