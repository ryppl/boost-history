#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#define BOOST_MPL_LIMIT_VECTOR_SIZE 100
#define BOOST_MPL_LIMIT_MAP_SIZE 100
#define FUSION_MAX_VECTOR_SIZE 80
#include <boost/mpl/vector/vector100.hpp>
#include <boost/fusion/container/vector/vector80.hpp>

#include <boost/msm/back/favor_compile_time.hpp>
#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/front/state_machine_def.hpp>

namespace msm = boost::msm;
namespace mpl = boost::mpl;

#include <iostream>
#ifdef WIN32
#include "windows.h"
#else
#include <sys/time.h>
#endif

namespace test_fsm // Concrete FSM implementation
{
    // events
    struct event1 {};

    // Concrete FSM implementation 
    struct player_ : public msm::front::state_machine_def<player_>
    {
        // no need for exception handling or message queue
        typedef int no_exception_thrown;
        typedef int no_message_queue;

        // The list of FSM states
        struct State1 : public msm::front::state<> {};
        struct State2 : public msm::front::state<> {};
        struct State3 : public msm::front::state<> {};
        struct State4 : public msm::front::state<> {};
        struct State5 : public msm::front::state<> {};
        struct State6 : public msm::front::state<> {};
        struct State7 : public msm::front::state<> {};
        struct State8 : public msm::front::state<> {};
        struct State9 : public msm::front::state<> {};
        struct State10 : public msm::front::state<> {};
        struct State11 : public msm::front::state<> {};
        struct State12 : public msm::front::state<> {};
        struct State13 : public msm::front::state<> {};
        struct State14 : public msm::front::state<> {};
        struct State15 : public msm::front::state<> {};
        struct State16 : public msm::front::state<> {};
        struct State17 : public msm::front::state<> {};
        struct State18 : public msm::front::state<> {};
        struct State19 : public msm::front::state<> {};
        struct State20 : public msm::front::state<> {};
        struct State21 : public msm::front::state<> {};
        struct State22 : public msm::front::state<> {};
        struct State23 : public msm::front::state<> {};
        struct State24 : public msm::front::state<> {};
        struct State25 : public msm::front::state<> {};
        struct State26 : public msm::front::state<> {};
        struct State27 : public msm::front::state<> {};
        struct State28 : public msm::front::state<> {};
        struct State29 : public msm::front::state<> {};
        struct State30 : public msm::front::state<> {};
        struct State31 : public msm::front::state<> {};
        struct State32 : public msm::front::state<> {};
        struct State33 : public msm::front::state<> {};
        struct State34 : public msm::front::state<> {};
        struct State35 : public msm::front::state<> {};
        struct State36 : public msm::front::state<> {};
        struct State37 : public msm::front::state<> {};
        struct State38 : public msm::front::state<> {};
        struct State39 : public msm::front::state<> {};
        struct State40 : public msm::front::state<> {};
        struct State41 : public msm::front::state<> {};
        struct State42 : public msm::front::state<> {};
        struct State43 : public msm::front::state<> {};
        struct State44 : public msm::front::state<> {};
        struct State45 : public msm::front::state<> {};
        struct State46 : public msm::front::state<> {};
        struct State47 : public msm::front::state<> {};
        struct State48 : public msm::front::state<> {};
        struct State49 : public msm::front::state<> {};
        struct State50 : public msm::front::state<> {};
        struct State51 : public msm::front::state<> {};
        struct State52 : public msm::front::state<> {};
        struct State53 : public msm::front::state<> {};
        struct State54 : public msm::front::state<> {};
        struct State55 : public msm::front::state<> {};
        struct State56 : public msm::front::state<> {};
        struct State57 : public msm::front::state<> {};
        struct State58 : public msm::front::state<> {};
        struct State59 : public msm::front::state<> {};
        struct State60 : public msm::front::state<> {};
        struct State61 : public msm::front::state<> {};
        struct State62 : public msm::front::state<> {};
        struct State63 : public msm::front::state<> {};
        struct State64 : public msm::front::state<> {};
        struct State65 : public msm::front::state<> {};
        struct State66 : public msm::front::state<> {};
        struct State67 : public msm::front::state<> {};
        struct State68 : public msm::front::state<> {};
        struct State69 : public msm::front::state<> {};
        struct State70 : public msm::front::state<> {};
        struct State71 : public msm::front::state<> {};
        struct State72 : public msm::front::state<> {};
        struct State73 : public msm::front::state<> {};
        struct State74 : public msm::front::state<> {};
        struct State75 : public msm::front::state<> {};
        struct State76 : public msm::front::state<> {};
        struct State77 : public msm::front::state<> {};
        struct State78 : public msm::front::state<> {};
        struct State79 : public msm::front::state<> {};
        struct State80 : public msm::front::state<> {};


        // the initial state of the player SM. Must be defined
        typedef State1 initial_state;
        // guard conditions

        typedef player_ p; // makes transition table cleaner

        // Transition table for player
        struct transition_table : mpl::vector<
            //    Start     Event         Next      Action				Guard
            //    +---------+-------------+---------+---------------------+----------------------+
             _row < State1  , event1      , State2                                             >,
             _row < State2  , event1      , State3                                             >,
             _row < State3  , event1      , State4                                             >,
             _row < State4  , event1      , State5                                             >,
             _row < State5  , event1      , State6                                             >,
             _row < State6  , event1      , State7                                             >,
             _row < State7  , event1      , State8                                             >,
             _row < State8  , event1      , State9                                             >,
             _row < State9  , event1      , State10                                             >,
             _row < State10 , event1      , State11                                             >,
             _row < State11 , event1      , State12                                             >,
             _row < State12 , event1      , State13                                             >,
             _row < State13 , event1      , State14                                             >,
             _row < State14 , event1      , State15                                             >,
             _row < State15 , event1      , State16                                             >,
             _row < State16 , event1      , State17                                             >,
             _row < State17 , event1      , State18                                             >,
             _row < State18 , event1      , State19                                             >,
             _row < State19 , event1      , State20                                             >,
             _row < State20 , event1      , State21                                             >,
             _row < State21 , event1      , State22                                             >,
             _row < State22 , event1      , State23                                             >,
             _row < State23 , event1      , State24                                             >,
             _row < State24 , event1      , State25                                             >,
             _row < State25 , event1      , State26                                             >,
             _row < State26 , event1      , State27                                             >,
             _row < State27 , event1      , State28                                             >,
             _row < State28 , event1      , State29                                             >,
             _row < State29 , event1      , State30                                             >,
             _row < State30 , event1      , State31                                             >,
             _row < State31 , event1      , State32                                             >,
             _row < State32 , event1      , State33                                             >,
             _row < State33 , event1      , State34                                             >,
             _row < State34 , event1      , State35                                             >,
             _row < State35 , event1      , State36                                             >,
             _row < State36 , event1      , State37                                             >,
             _row < State37 , event1      , State38                                             >,
             _row < State38 , event1      , State39                                             >,
             _row < State39 , event1      , State40                                             >,
             _row < State40 , event1      , State41                                             >,
             _row < State41 , event1      , State42                                             >,
             _row < State42 , event1      , State43                                             >,
             _row < State43 , event1      , State44                                             >,
             _row < State44 , event1      , State45                                             >,
             _row < State45 , event1      , State46                                             >,
             _row < State46 , event1      , State47                                             >,
             _row < State47 , event1      , State48                                             >,
             _row < State48 , event1      , State49                                             >,
             _row < State49 , event1      , State50                                             >,
             _row < State50 , event1      , State51                                             >,
             _row < State51 , event1      , State52                                             >,
             _row < State52 , event1      , State53                                             >,
             _row < State53 , event1      , State54                                             >,
             _row < State54 , event1      , State55                                             >,
             _row < State55 , event1      , State56                                             >,
             _row < State56 , event1      , State57                                             >,
             _row < State57 , event1      , State58                                             >,
             _row < State58 , event1      , State59                                             >,
             _row < State59 , event1      , State60                                             >,
             _row < State60 , event1      , State61                                             >,
             _row < State61 , event1      , State62                                             >,
             _row < State62 , event1      , State63                                             >,
             _row < State63 , event1      , State64                                             >,
             _row < State64 , event1      , State65                                             >,
             _row < State65 , event1      , State66                                             >,
             _row < State66 , event1      , State67                                             >,
             _row < State67 , event1      , State68                                             >,
             _row < State68 , event1      , State69                                             >,
             _row < State69 , event1      , State70                                             >,
             _row < State60 , event1      , State71                                             >,
             _row < State71 , event1      , State72                                             >,
             _row < State72 , event1      , State73                                             >,
             _row < State73 , event1      , State74                                             >,
             _row < State74 , event1      , State75                                             >,
             _row < State75 , event1      , State76                                             >,
             _row < State76 , event1      , State77                                             >,
             _row < State77 , event1      , State78                                             >,
             _row < State78 , event1      , State79                                             >,
             _row < State79 , event1      , State80                                             >,
             _row < State80 , event1      , State80                                             >
            //    +---------+-------------+---------+---------------------+----------------------+
        > {};

        // Replaces the default no-transition response.
        template <class FSM,class Event>
        void no_transition(Event const& e, FSM&,int state)
        {
            std::cout << "no transition from state " << state
                << " on event " << typeid(e).name() << std::endl;
        }
    };
    typedef msm::back::state_machine<player_, 
        ::boost::msm::back::NoHistory, 
        ::boost::msm::back::favor_compile_time> player;

}

#ifndef WIN32
long mtime(struct timeval& tv1,struct timeval& tv2)
{
    return (tv2.tv_sec-tv1.tv_sec) *1000000 + ((tv2.tv_usec-tv1.tv_usec));
}
#endif


int main()
{
    // for timing
#ifdef WIN32
    LARGE_INTEGER res;
    ::QueryPerformanceFrequency(&res);
    LARGE_INTEGER li,li2;
#else
    struct timeval tv1,tv2;
    gettimeofday(&tv1,NULL);
#endif

    test_fsm::player p2;
    p2.start();
    // for timing
#ifdef WIN32
    ::QueryPerformanceCounter(&li);
#else
    gettimeofday(&tv1,NULL);
#endif
    for (int i=0;i<100;++i)
    {
        p2.process_event(test_fsm::event1());
    }
#ifdef WIN32
    ::QueryPerformanceCounter(&li2);
#else
    gettimeofday(&tv2,NULL);
#endif
#ifdef WIN32
    std::cout << "msm took in s:" << (double)(li2.QuadPart-li.QuadPart)/res.QuadPart <<"\n" <<std::endl;
#else
    std::cout << "msm took in us:" <<  mtime(tv1,tv2) <<"\n" <<std::endl;
#endif
    return 0;
}

