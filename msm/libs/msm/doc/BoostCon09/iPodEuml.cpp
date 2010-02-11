#include <vector>
#include <set>
#include <string>
#include <iostream>
//TODO
#define FUSION_MAX_VECTOR_SIZE 20

#include "boost/mpl/vector/vector50.hpp"
#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/front/euml/euml.hpp>


using namespace std;
using namespace boost::msm::front::euml;
namespace msm = boost::msm;

// attribute names and types
BOOST_MSM_EUML_DECLARE_ATTRIBUTE(int,m_Selected)
BOOST_MSM_EUML_DECLARE_ATTRIBUTE(int,m_SongIndex)
BOOST_MSM_EUML_DECLARE_ATTRIBUTE(int,m_NumberOfSongs)
#include "ipod_functors.hpp"


namespace  // Concrete FSM implementation
{
    //flags 
    struct MenuActive : euml_flag<MenuActive> {};
    struct NoFastFwd : euml_flag<NoFastFwd> {};
    // hardware-generated events
    struct Hold : euml_event<Hold> {};
    struct NoHold : euml_event<NoHold> {};
    struct SouthPressed : euml_event<SouthPressed> {};
    struct SouthReleased : euml_event<SouthReleased> {};
    struct MiddleButton : euml_event<MiddleButton> {};
    struct EastPressed : euml_event<EastPressed> {};
    struct EastReleased : euml_event<EastReleased>{};
    struct Off : euml_event<Off> {};
    struct MenuButton : euml_event<MenuButton> {};

    // internally defined events
    struct PlayPause : euml_event<PlayPause> {};
    struct EndPlay : euml_event<EndPlay> {};
    struct CloseMenu : euml_event<CloseMenu>
    {
        CloseMenu(){}//defined only for stt
        template<class EVENT>
        CloseMenu(EVENT const &) {}
    };
    struct OnOffTimer : euml_event<OnOffTimer> {};
    struct MenuMiddleButton : euml_event<MenuMiddleButton> {};
    struct SelectSong : euml_event<SelectSong> {};
    struct SongFinished : euml_event<SongFinished> {};

    typedef BOOST_TYPEOF(build_attributes(attributes_ << m_Selected )) StartSongAttributes;
    struct StartSong : euml_event<StartSong> ,StartSongAttributes
    {
        StartSong(){}//defined only for stt
        StartSong (int song_index)
        {
            get_attribute(m_Selected)=song_index;
        }
    };
    struct PreviousSong : euml_event<PreviousSong> {};
    struct NextSong : euml_event<NextSong>{};
    struct ForwardTimer : euml_event<ForwardTimer>{};
    struct PlayingMiddleButton : euml_event<PlayingMiddleButton>{};

    // Concrete iPod implementation 
    // The list of iPod states
    typedef BOOST_TYPEOF(build_state(NotHolding_Entry() )) NotHolding;
    typedef BOOST_TYPEOF(build_interrupt_state(NoHold(),Holding_Entry() )) Holding;
    typedef BOOST_TYPEOF(build_state(NotPlaying_Entry() )) NotPlaying;
    typedef BOOST_TYPEOF(build_state(NoMenuMode_Entry() )) NoMenuMode;
    typedef BOOST_TYPEOF(build_state(NoOnOffButton_Entry() )) NoOnOffButton;
    typedef BOOST_TYPEOF(build_state(OffDown_Entry() )) OffDown;
    typedef BOOST_TYPEOF(build_state(PlayerOff_Entry() )) PlayerOff;
    typedef BOOST_TYPEOF(build_state(CheckMiddleButton_Entry() )) CheckMiddleButton;
    // Concrete PlayingMode_ implementation 
    // The list of PlayingMode_ states
    typedef BOOST_TYPEOF(build_state((Playing_Entry()/*,*/ ) )) Playing;
    typedef BOOST_TYPEOF(build_state(WaitingForNextPrev_Entry() )) WaitingForNextPrev;
    typedef BOOST_TYPEOF(build_state(Paused_Entry() )) Paused;
    typedef BOOST_TYPEOF(build_state(WaitingForEnd_Entry() )) WaitingForEnd;
    typedef BOOST_TYPEOF(build_state(NoForward_Entry() )) NoForward;
    typedef BOOST_TYPEOF(build_state(ForwardPressed_Entry(),ForwardPressed_Exit() )) ForwardPressed;
    typedef BOOST_TYPEOF(build_state(FastForward_Entry(),FastForward_Exit() )) FastForward;
    typedef BOOST_TYPEOF(build_state(StdDisplay_Entry() )) StdDisplay;
    typedef BOOST_TYPEOF(build_state(SetPosition_Entry() )) SetPosition;
    typedef BOOST_TYPEOF(build_state(SetMark_Entry() )) SetMark;
    typedef BOOST_TYPEOF(build_exit_state(EndPlay(),PlayingExit_Entry() )) PlayingExit;

    //stt
    typedef BOOST_TYPEOF(build_stt((
        //  +------------------------------------------------------------------------------+
        Paused()             == Playing()           + PlayPause()                                            ,
        Paused()             == Playing()           + Off()                                                  ,
        Playing()            == Playing()           + StartSong() 
                             / (if_then_(event_(m_Selected) > Int_<0>() && 
                                         event_(m_Selected) < fsm_(m_NumberOfSongs),
                                         fsm_(m_SongIndex) = event_(m_Selected) ),show_selected_song())   ,
        Playing()            == Playing() + SongFinished()
                             / (if_then_else_(++fsm_(m_SongIndex) <= fsm_(m_NumberOfSongs),  /*if*/
                                             show_playing_song(),                               /*then*/
                                             (fsm_(m_SongIndex)=Int_<1>(),process_(EndPlay()))/*else*/ ) )  ,
        Playing()            == Paused()            + PlayPause()                                             ,
        Playing()            == Paused()            + StartSong()
                             / (if_then_(event_(m_Selected) > Int_<0>() && 
                                         event_(m_Selected) < fsm_(m_NumberOfSongs),
                                         fsm_(m_SongIndex) = event_(m_Selected) ),show_selected_song())    ,
        WaitingForNextPrev() == WaitingForNextPrev()+ PreviousSong()                     
                             /( if_then_else_(--fsm_(m_SongIndex) > Int_<0>(),                  /*if*/
                                              show_playing_song(),                                /*then*/
                                              (fsm_(m_SongIndex)=Int_<1>(),process_(EndPlay())) /*else*/ ) ) ,
        WaitingForNextPrev() == WaitingForNextPrev()+ NextSong()
                               / (if_then_else_(++fsm_(m_SongIndex) <= fsm_(m_NumberOfSongs),      /*if*/
                                                show_playing_song(),                               /*then*/
                                                (fsm_(m_SongIndex)=Int_<1>(),process_(EndPlay()))  /*else*/ ) ),

        PlayingExit()        == WaitingForEnd()     + EndPlay()                                                ,
        ForwardPressed()     == NoForward()         + EastPressed() [!is_flag_(NoFastFwd())]                   ,
        NoForward()          == ForwardPressed()    + EastReleased()      / process_(NextSong())               ,
        FastForward()        == ForwardPressed()    + ForwardTimer()    / do_fast_forward()                    ,
        FastForward()        == FastForward()       + ForwardTimer()    / do_fast_forward()                    ,
        FastForward()        == NoForward()         + EastReleased()                                           ,
        SetPosition()        == StdDisplay()        + PlayingMiddleButton()                                    ,
        StdDisplay()         == SetPosition()       + StartSong()                                              ,
        SetMark()            == SetPosition()       + PlayingMiddleButton()                                    ,
        StdDisplay()         == SetMark()           + PlayingMiddleButton()                                    ,
        StdDisplay()         == SetMark()           + StartSong()  
        //  +------------------------------------------------------------------------------+
        ) ) ) playingmode_transition_table;

    // VC9 cannot compile the typedef with build_sm if one is also used for player
#ifndef BOOST_MSVC
    // create a state machine "on the fly" for Playing
    typedef BOOST_TYPEOF(build_sm(  playingmode_transition_table(), //STT
        init_ << Playing() << WaitingForNextPrev() << WaitingForEnd() 
              << NoForward() << StdDisplay(), // Init States
        fsm_(m_NumberOfSongs)=Int_<5>(), // entry
        NoAction(), // exit
        attributes_ << m_SongIndex << m_NumberOfSongs, //attributes
        configure_<< NoFastFwd() // Flags, Deferred events, configuration

        )) PlayingMode_;
#else
    // but this definition is ok
    struct PlayingMode_ : public BOOST_TYPEOF(build_sm(  playingmode_transition_table(), //STT
        init_ << Playing() << WaitingForNextPrev() << WaitingForEnd() 
              << NoForward() << StdDisplay(), // Init States
        fsm_(m_NumberOfSongs)=Int_<5>(), // entry
        NoAction(), // exit
        attributes_ << m_SongIndex << m_NumberOfSongs, //attributes
        configure_<< NoFastFwd() // Flags, Deferred events, configuration
        )) 
    {
    };
#endif
    // choice of back-end
    typedef msm::back::state_machine<PlayingMode_> PlayingMode;

    // Concrete MenuMode_ implementation 
    // The list of MenuMode_ states
    typedef BOOST_TYPEOF(build_state(WaitingForSongChoice_Entry() )) WaitingForSongChoice;
    typedef BOOST_TYPEOF(build_state(StartCurrentSong_Entry() )) StartCurrentSong;
    typedef BOOST_TYPEOF(build_exit_state(CloseMenu(),MenuExit_Entry() )) MenuExit;
    //stt
    typedef BOOST_TYPEOF(build_stt((
        //  +------------------------------------------------------------------------------+
        StartCurrentSong()   == WaitingForSongChoice()  + MenuMiddleButton()  ,
        MenuExit()           == StartCurrentSong()      + SelectSong() 
        //  +------------------------------------------------------------------------------+
        ) ) ) menumode_transition_table;

    // VC9 cannot compile the typedef with build_sm if one is also used for player
#ifndef BOOST_MSVC
    // create a state machine "on the fly" for Playing
    typedef BOOST_TYPEOF(build_sm(  menumode_transition_table(), //STT
        init_ << WaitingForSongChoice(), // Init States
        NoAction(), // entry
        NoAction(), // exit
        attributes_ << no_attributes_, //attributes
        configure_<< MenuActive() // Flags, Deferred events, configuration

        )) MenuMode_;
#else
    // but this definition is ok
    struct MenuMode_ : public BOOST_TYPEOF(build_sm(  menumode_transition_table(), //STT
        init_ << WaitingForSongChoice(), // Init States
        NoAction(), // entry
        NoAction(), // exit
        attributes_ << no_attributes_, //attributes
        configure_<< MenuActive() // Flags, Deferred events, configuration
        )) 
    {
    };
#endif
    typedef msm::back::state_machine<MenuMode_> MenuMode;

    // iPod stt
    typedef BOOST_TYPEOF(build_stt((
        //  +------------------------------------------------------------------------------+
        Holding()           == NotHolding()                 + Hold()                        ,
        NotHolding()        == Holding()                    + NoHold()                      ,
        PlayingMode()       == NotPlaying()                 + PlayPause()                   ,
        NotPlaying()        == PlayingMode::exit_pt<PlayingExit>()  + EndPlay()    
                                / process_(MenuButton())                                    ,
        MenuMode()          == NoMenuMode()                 + MenuButton()                  ,
        NoMenuMode()        == MenuMode::exit_pt<MenuExit>()+ CloseMenu()    
                                / process2_(StartSong(),Int_<5>())                          ,
        OffDown()           == NoOnOffButton()              + SouthPressed()                ,
        NoOnOffButton()     == OffDown()                    + SouthReleased() 
                                / process_(PlayPause())                                     ,
        PlayerOff()         == OffDown()                    + OnOffTimer()     
                                / (show_player_off(),process_(Off()))                       ,
        NoOnOffButton()     == PlayerOff()                  + SouthPressed() / show_player_on() ,
        NoOnOffButton()     == PlayerOff()                  + NoHold() / show_player_on()   ,
        CheckMiddleButton() == CheckMiddleButton()          + MiddleButton() 
                               [is_flag_(MenuActive())] / process_(PlayingMiddleButton())   ,
        CheckMiddleButton() == CheckMiddleButton() + MiddleButton()
                               [!is_flag_(MenuActive())] / process_(PlayingMiddleButton()) 
        //  +------------------------------------------------------------------------------+
        ) ) ) ipod_transition_table;
    // VC9 cannot compile the typedef with build_sm if one is also used for player
#ifndef BOOST_MSVC
    // create a state machine "on the fly" for Playing
    typedef BOOST_TYPEOF(build_sm(  ipod_transition_table(), //STT
        init_ << NotHolding() << NotPlaying() << NoMenuMode() 
              << NoOnOffButton() << CheckMiddleButton() // Init States
        )) iPod_;
#else
    // but this definition is ok
    struct iPod_ : public BOOST_TYPEOF(build_sm(  ipod_transition_table(), //STT
        init_ << NotHolding() << NotPlaying() << NoMenuMode() 
              << NoOnOffButton() << CheckMiddleButton() // Init States
        )) 
    {
    };
#endif
    typedef msm::back::state_machine<iPod_> iPod;
   
    void test()
    {
        iPod sm;
        sm.start();
        // we first press Hold
        std::cout << "pressing hold" << std::endl;
        sm.process_event(Hold());
        // pressing a button is now ignored
        std::cout << "pressing a button" << std::endl;
        sm.process_event(SouthPressed());
        // or even one contained in a submachine
        sm.process_event(EastPressed());
        // no more holding
        std::cout << "no more holding, end interrupt event sent" << std::endl;
        sm.process_event(NoHold());
        std::cout << "pressing South button a short time" << std::endl;
        sm.process_event(SouthPressed());
        // we suppose a short pressing leading to playing a song
        sm.process_event(SouthReleased());
        // we move to the next song
        std::cout << "we move to the next song" << std::endl;
        sm.process_event(NextSong());
        // then back to no song => exit from playing, menu active
        std::cout << "we press twice the West button (simulated)=> end of playing" << std::endl;
        sm.process_event(PreviousSong());
        sm.process_event(PreviousSong());
        // even in menu mode, pressing play will start playing the first song
        std::cout << "pressing play/pause" << std::endl;
        sm.process_event(SouthPressed());
        sm.process_event(SouthReleased());
        // of course pausing must be possible
        std::cout << "pressing play/pause" << std::endl;
        sm.process_event(SouthPressed());
        sm.process_event(SouthReleased());
        std::cout << "pressing play/pause" << std::endl;
        sm.process_event(SouthPressed());
        sm.process_event(SouthReleased());
        // while playing, you can fast forward
        std::cout << "pressing East button a long time" << std::endl;
        sm.process_event(EastPressed());
        // let's suppose the timer just fired
        sm.process_event(ForwardTimer());
        sm.process_event(ForwardTimer());
        // end of fast forwarding
        std::cout << "releasing East button" << std::endl;
        sm.process_event(EastReleased());
        // we now press the middle button to set playing at a given position
        std::cout << "pressing Middle button, fast forwarding disabled" << std::endl;
        sm.process_event(MiddleButton());
        std::cout <<"pressing East button to fast forward" << std::endl;
        sm.process_event(EastPressed());
        // we switch off and on
        std::cout <<"switch off player" << std::endl;
        sm.process_event(SouthPressed());
        sm.process_event(OnOffTimer());
        std::cout <<"switch on player" << std::endl;    
        sm.process_event(SouthPressed());
    }
}

int main()
{
    test();
    return 0;
}
