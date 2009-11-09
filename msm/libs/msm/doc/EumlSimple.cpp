// MsmSimple.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/front/euml/euml.hpp>
#include <boost/msm/front/euml/stl.hpp>

namespace msm = boost::msm;
namespace mpl = boost::mpl;
using namespace boost::msm::front::euml;

#include <iostream>
#ifdef WIN32
#include "windows.h"
#else
#include <sys/time.h>
#endif

namespace test_fsm // Concrete FSM implementation
{
    // events
    struct play : euml_event<play>{};
    struct end_pause : euml_event<end_pause>{};
    struct stop : euml_event<stop>{};
    struct pause : euml_event<pause>{};
    struct open_close : euml_event<open_close>{};
    struct cd_detected : euml_event<cd_detected>{};

    struct start_playback : euml_action<start_playback>
    {
        template <class FSM,class EVT,class SourceState,class TargetState>
        void operator()(EVT const& ,FSM&,SourceState& ,TargetState& )
        {
        }
    };
    struct open_drawer : euml_action<open_drawer>
    {
        template <class FSM,class EVT,class SourceState,class TargetState>
        void operator()(EVT const& ,FSM&,SourceState& ,TargetState& )
        {
        }
    };
    struct close_drawer : euml_action<close_drawer>
    {
        template <class FSM,class EVT,class SourceState,class TargetState>
        void operator()(EVT const& ,FSM&,SourceState& ,TargetState& )
        {
        }
    };
    struct store_cd_info : euml_action<store_cd_info>
    {
        template <class FSM,class EVT,class SourceState,class TargetState>
        void operator()(EVT const&, FSM& fsm ,SourceState& ,TargetState& )
        {
        }
    };
    struct stop_playback : euml_action<stop_playback>
    {
        template <class FSM,class EVT,class SourceState,class TargetState>
        void operator()(EVT const& ,FSM&,SourceState& ,TargetState& )
        {
        }
    };
    struct pause_playback : euml_action<pause_playback>
    {
        template <class FSM,class EVT,class SourceState,class TargetState>
        void operator()(EVT const& ,FSM&,SourceState& ,TargetState& )
        {
        }
    };
    struct resume_playback : euml_action<resume_playback>
    {
        template <class FSM,class EVT,class SourceState,class TargetState>
        void operator()(EVT const& ,FSM&,SourceState& ,TargetState& )
        {
        }
    };
    struct stop_and_open : euml_action<stop_and_open>
    {
        template <class FSM,class EVT,class SourceState,class TargetState>
        void operator()(EVT const& ,FSM&,SourceState& ,TargetState& )
        {
        }
    };
    struct stopped_again : euml_action<stopped_again>
    {
        template <class FSM,class EVT,class SourceState,class TargetState>
        void operator()(EVT const& ,FSM&,SourceState& ,TargetState& )
        {
        }
    };

    // The list of FSM states
    typedef BOOST_TYPEOF(build_state()) Empty;

    typedef BOOST_TYPEOF(build_state( )) Open;

    typedef BOOST_TYPEOF(build_state(  )) Stopped;

    typedef BOOST_TYPEOF(build_state(  )) Playing;

    typedef BOOST_TYPEOF(build_state( )) Paused;

    // replaces the old transition table
    typedef BOOST_TYPEOF(build_stt
        ((Stopped() + play()        == Playing()  / start_playback() ,
          Stopped() + open_close()  == Open()     / open_drawer(),
          Stopped() + stop()        == Stopped()  / stopped_again(),
          //  +------------------------------------------------------------------------------+
          Open()    + open_close()  == Empty()    / close_drawer(),
          //  +------------------------------------------------------------------------------+
          Empty()   + open_close()  == Open()     / open_drawer(),
          Empty()   + cd_detected() == Stopped()  / store_cd_info(),
         //  +------------------------------------------------------------------------------+
          Playing() + stop()        == Stopped()  / stop_playback(),
          Playing() + pause()       == Paused()   / pause_playback(),
          Playing() + open_close()  == Open()     / stop_and_open(),
          //  +------------------------------------------------------------------------------+
          Paused()  + end_pause()   == Playing()  / resume_playback(),
          Paused()  + stop()        == Stopped()  / stop_playback(),
          Paused()  + open_close()  == Open()     / stop_and_open()
          //  +------------------------------------------------------------------------------+
                    ) ) ) transition_table;


    struct Log_No_Transition : euml_action<Log_No_Transition>
    {
        template <class FSM,class Event>
        void operator()(Event const& e,FSM&,int state)
        {
            std::cout << "no transition from state " << state
                << " on event " << typeid(e).name() << std::endl;
        }
    };

    // create a state machine "on the fly"
    // VC9 sometimes cannot compile the typedef with build_sm
#ifndef BOOST_MSVC
    // create a state machine "on the fly" for player
    typedef BOOST_TYPEOF(build_sm(  transition_table(), //STT
                                    init_ << Empty(), // Init State
                                    NoAction(), // Entry
                                    NoAction(), // Exit
                                    attributes_ << no_attributes_, // Attributes
                                    configure_ << no_exception() << no_msg_queue(), 
                                    Log_No_Transition() // no_transition handler
                                    )) player_;
#else
    // but this definition is ok
    struct player_ : public BOOST_TYPEOF(build_sm(  transition_table(), //STT
                                                    init_ << Empty(), // Init State
                                                    NoAction(), // Entry
                                                    NoAction(), // Exit
                                                    attributes_ << no_attributes_, // Attributes
                                                    configure_ << no_exception() << no_msg_queue(), // configuration
                                                    Log_No_Transition() // no_transition handler
                                                    ))
    {
    };
#endif
    typedef msm::back::state_machine<player_> player;

    //
    // Testing utilities.
    //
    static char const* const state_names[] = { "Stopped", "Open", "Empty", "Playing", "Paused" };

    void pstate(player const& p)
    {
        std::cout << " -> " << state_names[p.current_state()[0]] << std::endl;
    }

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
        p2.process_event(test_fsm::open_close());
        p2.process_event(test_fsm::open_close()); 
        p2.process_event(test_fsm::cd_detected());
        p2.process_event(test_fsm::play());      
        p2.process_event(test_fsm::pause()); 
        // go back to Playing
        p2.process_event(test_fsm::end_pause()); 
        p2.process_event(test_fsm::pause()); 
        p2.process_event(test_fsm::stop());  
        // event leading to the same state
        p2.process_event(test_fsm::stop());
        p2.process_event(test_fsm::open_close());
        p2.process_event(test_fsm::open_close());
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

