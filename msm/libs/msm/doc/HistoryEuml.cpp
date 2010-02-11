#include <iostream>

#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/front/euml/euml.hpp>

using namespace std;
using namespace boost::msm::front::euml;
namespace msm = boost::msm;

// entry/exit/action/guard logging functors
#include "logging_functors.h"

namespace  // Concrete FSM implementation
{
    // events
    struct play : euml_event<play>{};
    struct end_pause : euml_event<end_pause>{};
    struct stop : euml_event<stop>{};
    struct pause : euml_event<pause>{};
    struct open_close : euml_event<open_close>{};
    struct next_song : euml_event<next_song>{};
    struct previous_song : euml_event<previous_song>{};

    // A "complicated" event type that carries some data.
    BOOST_MSM_EUML_DECLARE_ATTRIBUTE(std::string,cd_name)
    BOOST_MSM_EUML_DECLARE_ATTRIBUTE(DiskTypeEnum,cd_type)
    typedef BOOST_TYPEOF(build_attributes(attributes_ << cd_name << cd_type )) cd_detected_attributes;
    struct cd_detected : euml_event<cd_detected>,cd_detected_attributes
    {
        cd_detected(){}
        cd_detected(std::string name, DiskTypeEnum diskType)
        {
            get_attribute(cd_name)=name;
            get_attribute(cd_type)=diskType;
        }
    };


    // Seems to correct random VC9 bug
    struct PlayingPaused: euml_flag<PlayingPaused>{};

    // Concrete FSM implementation 

    // The list of FSM states
    // state not defining any entry or exit
    typedef BOOST_TYPEOF(build_state( )) Paused;

    typedef BOOST_TYPEOF(build_state(Empty_Entry(),Empty_Exit())) Empty;

    typedef BOOST_TYPEOF(build_state( Open_Entry(),Open_Exit() )) Open;

    typedef BOOST_TYPEOF(build_state( Stopped_Entry(),Stopped_Exit() )) Stopped;


    // Playing is now a state machine itself.

    // It has 3 substates
    typedef BOOST_TYPEOF(build_state( Song1_Entry(),Song1_Exit() )) Song1;

    typedef BOOST_TYPEOF(build_state( Song2_Entry(),Song2_Exit() )) Song2;

    typedef BOOST_TYPEOF(build_state( Song3_Entry(),Song3_Exit() )) Song3;


    // Playing has a transition table 
    typedef BOOST_TYPEOF(build_stt((
        //  +------------------------------------------------------------------------------+
        Song2()  == Song1() + next_song()       / start_next_song(),
        Song1()  == Song2() + previous_song()   / start_prev_song(),
        Song3()  == Song2() + next_song()       / start_next_song(),
        Song2()  == Song3() + previous_song()   / start_prev_song()
        //  +------------------------------------------------------------------------------+
        ) ) ) playing_transition_table;

    // VC9 cannot compile the typedef with build_sm if one is also used for player
#ifndef BOOST_MSVC
    // create a state machine "on the fly" for Playing
    typedef BOOST_TYPEOF(build_sm(  playing_transition_table(), //STT
                                    init_ << Song1() // Init State
                                    )) Playing_;
#else
    // but this definition is ok
    struct Playing_ : public BOOST_TYPEOF(build_sm(  playing_transition_table(), //STT
                                                     init_ << Song1() // Init State
                                                   )) 
    {
    };
#endif
    // choice of back-end
    typedef msm::back::state_machine<Playing_,msm::back::ShallowHistory<mpl::vector<end_pause> > > Playing;

    // guard conditions
    struct good_disk_format : euml_action<good_disk_format>
    {
        template <class FSM,class EVT,class SourceState,class TargetState>
        bool operator()(EVT const& evt,FSM&,SourceState& ,TargetState& )
        {
            // to test a guard condition, let's say we understand only CDs, not DVD
            if (evt.get_attribute(cd_type)!=DISK_CD)
            {
                std::cout << "wrong disk, sorry" << std::endl;
                // just for logging, does not block any transition
                return true;
            }
            std::cout << "good disk" << std::endl;
            return true;
        }
    };
    // replaces the old transition table
    typedef BOOST_TYPEOF(build_stt((
          Playing()   == Stopped()  + play()        / start_playback() ,
          Playing()   == Paused()   + end_pause()   / resume_playback(),
          //  +------------------------------------------------------------------------------+
          Empty()     == Open()     + open_close()  / close_drawer(),
          //  +------------------------------------------------------------------------------+
          Open()      == Empty()    + open_close()  / open_drawer(),
          Open()      == Paused()   + open_close()  / stop_and_open(),
          Open()      == Stopped()  + open_close()  / open_drawer(),
          Open()      == Playing()  + open_close()  / stop_and_open(),
          //  +------------------------------------------------------------------------------+
          Paused()    == Playing()  + pause()       / pause_playback(),
          //  +------------------------------------------------------------------------------+
          Stopped()   == Playing()  + stop()        / stop_playback(),
          Stopped()   == Paused()   + stop()        / stop_playback(),
          Stopped()   == Empty()    + cd_detected() [good_disk_format()&&
                                                     (event_(cd_type)==Int_<DISK_CD>())] 
                                                    / (store_cd_info(),process_(play())),
          Stopped()   == Stopped()  + stop()                            
          //  +------------------------------------------------------------------------------+
                    ) ) ) transition_table;

    // create a state machine "on the fly"
    typedef BOOST_TYPEOF(build_sm(  transition_table(), //STT
                                    init_ << Empty(), // Init State
                                    NoAction(), // Entry
                                    NoAction(), // Exit
                                    attributes_ << no_attributes_, // Attributes
                                    configure_<< no_configure_, // Flags, Deferred events, configuration
                                    Log_No_Transition() // no_transition handler
                                    )) player_;
    // or simply, if no no_transition handler needed:
    //typedef BOOST_TYPEOF(build_sm(  transition_table(), //STT
    //                                Empty(), // Init State
    //                                )) player_;

    // choice of back-end
    typedef msm::back::state_machine<player_> player;

    //
    // Testing utilities.
    //
    static char const* const state_names[] = { "Stopped", "Paused", "Open", "Empty", "Playing" };
    void pstate(player const& p)
    {
        std::cout << " -> " << state_names[p.current_state()[0]] << std::endl;
    }

    void test()
    {        
		player p;
        // needed to start the highest-level SM. This will call on_entry and mark the start of the SM
        p.start(); 
        // go to Open, call on_exit on Empty, then action, then on_entry on Open
        p.process_event(open_close()); pstate(p);
        p.process_event(open_close()); pstate(p);
        // will be rejected, wrong disk type
        p.process_event(
            cd_detected("louie, louie",DISK_DVD)); pstate(p);
        p.process_event(
            cd_detected("louie, louie",DISK_CD)); pstate(p);
        // no need to call play() as the previous event does it in its action method
		//p.process_event(play());

        // make transition happen inside it. Player has no idea about this event but it's ok.
        p.process_event(next_song());pstate(p); //2nd song active
        p.process_event(next_song());pstate(p);//3rd song active
        p.process_event(previous_song());pstate(p);//2nd song active

        // at this point, Play is active      
        p.process_event(pause()); pstate(p);
        // go back to Playing
        // as you see, remembers the original state as end_pause is an history trigger
        p.process_event(end_pause());  pstate(p);
        p.process_event(pause()); pstate(p);
        p.process_event(stop());  pstate(p);
        // event leading to the same state
        // no action method called as none is defined in the transition table
        p.process_event(stop());  pstate(p);
        // test call to no_transition
        p.process_event(play()); pstate(p);
    }
}

int main()
{
    test();
    return 0;
}
