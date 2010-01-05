#include <vector>
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
    struct end_error : euml_event<end_error> {};
    struct error_found : euml_event<error_found> {};

    // A "complicated" event type that carries some data.
    typedef BOOST_TYPEOF(build_attributes(attributes_ << std::string() << DiskTypeEnum() )) cd_detected_attr;
    struct cd_detected : euml_event<cd_detected>,cd_detected_attr
        
    {
        cd_detected(){}
        cd_detected(std::string name, DiskTypeEnum diskType)
        {
            get_attribute<0>()=name;
            get_attribute<1>()=diskType;
        }
    };

    // Flags. Allow information about a property of the current state
    struct PlayingPaused: euml_flag<PlayingPaused>{};
    struct CDLoaded : euml_flag<CDLoaded>{};
    struct FirstSongPlaying : euml_flag<FirstSongPlaying>{};

    // Concrete FSM implementation 

    // The list of FSM states

    typedef BOOST_TYPEOF(build_state(Empty_Entry(),Empty_Exit(),
                                     attributes_ << no_attributes_,
                                     configure_ << play() )) Empty; // defer play

    typedef BOOST_TYPEOF(build_state( Open_Entry(),Open_Exit(),
                                      attributes_ << no_attributes_,
                                      configure_<< CDLoaded()<< play() // defer play, flag state with CDLoaded
                                      )) Open;

    typedef BOOST_TYPEOF(build_state( Stopped_Entry(),Stopped_Exit(),
                                      attributes_ << no_attributes_,
                                      configure_<< CDLoaded() )) Stopped; // flag state with CDLoaded

    // state not defining any entry or exit
    typedef BOOST_TYPEOF(build_state(NoAction(),NoAction(),
                                     attributes_ << no_attributes_,
                                     // defer play, flag state with CDLoaded and PlayingPaused
                                     configure_<< PlayingPaused() << CDLoaded() )) Paused;

    typedef BOOST_TYPEOF(build_state( AllOk_Entry(),AllOk_Exit() )) AllOk;

    // a terminate state 
    //typedef BOOST_TYPEOF(build_terminate_state( ErrorMode_Entry(),ErrorMode_Exit() )) ErrorMode;
    // or as an interrupt state
    typedef BOOST_TYPEOF(build_interrupt_state( end_error(),ErrorMode_Entry(),ErrorMode_Exit() )) ErrorMode;


    // Playing is now a state machine itself.

    // It has 3 substates
    typedef BOOST_TYPEOF(build_state( Song1_Entry(),Song1_Exit(),
                                      attributes_ << no_attributes_,
                                      configure_<< FirstSongPlaying() )) Song1;

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
                                                     init_ << Song1(), // Init State
                                                     NoAction(), // entry
                                                     NoAction(), // exit
                                                     attributes_ << no_attributes_, //attributes
                                                     configure_<< PlayingPaused() << CDLoaded() //flags
                                                   )) 
    {
    };
#endif
    // choice of back-end
    typedef msm::back::state_machine<Playing_> Playing;

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
          Stopped()   == Empty()    + cd_detected() [good_disk_format()&&(Event_<1>()==Int_<DISK_CD>())] 
                                                    / (store_cd_info(),process_(play())),
          Stopped()   == Stopped()  + stop()                            
          //  +------------------------------------------------------------------------------+
                    ) ) ) transition_table;

    // create a state machine "on the fly"
    typedef BOOST_TYPEOF(build_sm(  transition_table(), //STT
                                    init_ << Empty() << AllOk(), // Init States
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
        // we have now several active states, which we show
        for (unsigned int i=0;i<player::nr_regions::value;++i)
        {
            std::cout << " -> " << state_names[p.current_state()[i]] << std::endl;
        }
    }

    void test()
    {        
		player p;
        // needed to start the highest-level SM. This will call on_entry and mark the start of the SM
        p.start(); 
        // test deferred event
        // deferred in Empty and Open, will be handled only after event cd_detected
        p.process_event(play());

        // tests some flags
        std::cout << "CDLoaded active:" << std::boolalpha << p.is_flag_active<CDLoaded>() << std::endl; //=> false (no CD yet)
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

        // at this point, Play is active 
        std::cout << "PlayingPaused active:" << std::boolalpha << p.is_flag_active<PlayingPaused>() << std::endl;//=> true
        std::cout << "FirstSong active:" << std::boolalpha << p.is_flag_active<FirstSongPlaying>() << std::endl;//=> true

        // make transition happen inside it. Player has no idea about this event but it's ok.
        p.process_event(next_song());pstate(p); //2nd song active
        p.process_event(next_song());pstate(p);//3rd song active
        p.process_event(previous_song());pstate(p);//2nd song active
        std::cout << "FirstSong active:" << std::boolalpha << p.is_flag_active<FirstSongPlaying>() << std::endl;//=> false
        std::cout << "PlayingPaused active:" << std::boolalpha << p.is_flag_active<PlayingPaused>() << std::endl;//=> true

        // at this point, Play is active      
        p.process_event(pause()); pstate(p);
        std::cout << "PlayingPaused active:" << std::boolalpha << p.is_flag_active<PlayingPaused>() << std::endl;//=> true

        // go back to Playing
        p.process_event(end_pause());  pstate(p);
        p.process_event(pause()); pstate(p);
        p.process_event(stop());  pstate(p);
        std::cout << "PlayingPaused active:" << std::boolalpha << p.is_flag_active<PlayingPaused>() << std::endl;//=> false
        std::cout << "CDLoaded active:" << std::boolalpha << p.is_flag_active<CDLoaded>() << std::endl;//=> true
        // by default, the flags are OR'ed but you can also use AND. Then the flag must be present in 
        // all of the active states
        std::cout << "CDLoaded active with AND:" << std::boolalpha << p.is_flag_active<CDLoaded,player::Flag_AND>() << std::endl;//=> false

        // event leading to the same state
        // no action method called as none is defined in the transition table
        p.process_event(stop());  pstate(p);

        // event leading to a terminal/interrupt state
        p.process_event(error_found());  pstate(p);
        // try generating more events
        std::cout << "Trying to generate another event" << std::endl; // will not work, fsm is terminated or interrupted
        p.process_event(play());pstate(p);
        std::cout << "Trying to end the error" << std::endl; // will work only if ErrorMode is interrupt state
        p.process_event(end_error());pstate(p);
        std::cout << "Trying to generate another event" << std::endl; // will work only if ErrorMode is interrupt state
        p.process_event(play());pstate(p);

    }
}

int main()
{
    test();
    return 0;
}
