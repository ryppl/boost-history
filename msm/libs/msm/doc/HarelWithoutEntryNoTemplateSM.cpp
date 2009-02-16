#include <vector>
#include <iostream>
#include <boost/mpl/vector/vector50.hpp>
#include <boost/msm/state_machine.hpp>

using namespace boost::msm;

namespace {
    // events
    struct AnyBtnPressed {};
    struct AlarmBeeps30s {};
    struct P 
    {
        P(int id):index(id){}
        // the bit(s) of the corresponding alarm
        int index;
    };
    // Concrete FSM implementation
    struct Clock : public state_machine<Clock>
    {
        struct Displays : public state<> {};
        class AlarmBeeps : public state_machine<AlarmBeeps>
        {            
        public:
            template <class Event>
            void on_entry(Event const& evt) 
            {
                // let each orthogonal zone handle the event (can be handled by more than 1)
                process_event(evt);
            }
            // g++ refuses to compile if the Alarm submachines are template
            struct AlarmImpl : public state_machine<AlarmImpl >
            {
                AlarmImpl(int index):Index(index){}
                struct NoBeep : public state<> {};
                struct Beeps : public state<> {};
                typedef AlarmImpl A; // makes transition table cleaner
                // the initial state of the AlarmBeeps SM. Must be defined
                typedef NoBeep initial_state;
                // guard
                bool check_beep(const P& evt)
                {
                    bool beep = ((evt.index & Index)!=0);
                    if (beep)
                        std::cout << "Beeping alarm:"<< Index  << std::endl;
                    // check if our bit is set in the event
                    return beep;
                }
                // Transition table for Alarm
                struct transition_table : mpl::vector<
                    //    Start     Event         Next      Action				Guard
                    //  +---------+-------------+---------+------------------------+----------------------+
                    g_row  < NoBeep  , P           , Beeps                            ,&A::check_beep        >
                    //  +---------+-------------+---------+------------------------+----------------------+
                > {};
            private:
                int Index;
            };
            struct Alarm1 : public AlarmImpl
            {
                Alarm1():AlarmImpl(1){}
            }; 
            struct Alarm2 : public AlarmImpl
            {
                Alarm2():AlarmImpl(2){}
            }; 

            // the initial state of the AlarmBeeps SM. Must be defined
            typedef mpl::vector<Alarm1,Alarm2 > initial_state;

            // Transition table for AlarmBeeps. Can be empty as no transition defined
            struct transition_table : mpl::vector<> {};
        };
        // the initial state of the Clock SM. Must be defined
        typedef Displays initial_state;
        // Transition table for Clock
        struct transition_table : mpl::vector<
            //    Start     Event         Next      Action				Guard
            //  +-----------+-------------+------------+------------------------+----------------------+
            _row  < Displays  , P           , AlarmBeeps                                                 >,
            _row  < AlarmBeeps,AnyBtnPressed, Displays                                                   >,
            _row  < AlarmBeeps,AlarmBeeps30s, Displays                                                   >
            //  +-----------+-------------+------------+------------------------+----------------------+
        > {};
    };
}

void test()
{
    Clock sm;
    sm.start();
    std::cout << "Sending P(1)" << std::endl;
    sm.process_event(P(1));
    std::cout << "Sending AnyBtnPressed" << std::endl;
    sm.process_event(AnyBtnPressed());
    std::cout << "Sending P(2)" << std::endl;
    sm.process_event(P(2));
    std::cout << "Sending AnyBtnPressed" << std::endl;
    sm.process_event(AnyBtnPressed());
    std::cout << "Sending P(3)" << std::endl;
    sm.process_event(P(3));

}
int main()
{
    test();
    return 0;
}


