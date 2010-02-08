#include <vector>
#include <set>
#include <string>
#include <iostream>
#define FUSION_MAX_VECTOR_SIZE 20

#include "boost/mpl/vector/vector50.hpp"
#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/front/state_machine_def.hpp>

using namespace std;
namespace msm = boost::msm;

namespace  // Concrete FSM implementation
{
    //flags 
    struct MenuActive{};
    // hardware-generated events
    struct Hold {};
    struct NoHold {};
    struct SouthPressed {};
    struct SouthReleased {};
    struct MiddleButton {};
    struct EastPressed{};
    struct EastReleased{};
    struct Off {};
    struct MenuButton {};

    // internally used events
    struct PlayPause {};
    struct EndPlay {};
    struct CloseMenu 
    {
        template<class EVENT>
        CloseMenu(EVENT const &) {}
    };
    struct OnOffTimer {};
    struct MenuMiddleButton {};
    struct SelectSong {};
    struct SongFinished {};
    struct StartSong 
    {
        StartSong (int song_index):m_Selected(song_index){}
        int m_Selected;
    };
    struct PreviousSong{};
    struct NextSong{};
    struct ForwardTimer{};
    struct PlayingMiddleButton{};

    // Concrete FSM implementation 
    struct iPod_ : public msm::front::state_machine_def<iPod_>
    {
        typedef msm::back::state_machine<iPod_> iPod;
        // The list of FSM states
        struct NotHolding : public msm::front::state<>
        {
            template <class Event,class FSM>
            void on_entry(Event const&,FSM& ) {std::cout << "starting: NotHolding" << std::endl;}
            template <class Event,class FSM>
            void on_exit(Event const&,FSM& ) {std::cout << "finishing: NotHolding" << std::endl;}
        };
        struct Holding : public msm::front::interrupt_state<NoHold>
        {
            template <class Event,class FSM>
            void on_entry(Event const&,FSM& ) {std::cout << "starting: Holding" << std::endl;}
            template <class Event,class FSM>
            void on_exit(Event const&,FSM& ) {std::cout << "finishing: Holding" << std::endl;}
        };
        struct NotPlaying : public msm::front::state<>
        {
            template <class Event,class FSM>
            void on_entry(Event const&,FSM& ) {std::cout << "starting: NotPlaying" << std::endl;}
            template <class Event,class FSM>
            void on_exit(Event const&,FSM& ) {std::cout << "finishing: NotPlaying" << std::endl;}
        };
        struct NoMenuMode : public msm::front::state<>
        {
            template <class Event,class FSM>
            void on_entry(Event const&,FSM& ) {std::cout << "starting: NoMenuMode" << std::endl;}
            template <class Event,class FSM>
            void on_exit(Event const&,FSM& ) {std::cout << "finishing: NoMenuMode" << std::endl;}
        };
        struct NoOnOffButton : public msm::front::state<>
        {
            template <class Event,class FSM>
            void on_entry(Event const&,FSM& ) {std::cout << "starting: NoOnOffButton" << std::endl;}
            template <class Event,class FSM>
            void on_exit(Event const&,FSM& ) {std::cout << "finishing: NoOnOffButton" << std::endl;}
        };
        struct OffDown : public msm::front::state<>
        {
            template <class Event,class FSM>
            void on_entry(Event const&,FSM& ) {std::cout << "starting: OffDown, start timer" << std::endl;}
            template <class Event,class FSM>
            void on_exit(Event const&,FSM& ) {std::cout << "finishing: OffDown, end timer" << std::endl;}
        };
        struct PlayerOff : public msm::front::state<>
        {
            template <class Event,class FSM>
            void on_entry(Event const&,FSM& ) {std::cout << "starting: PlayerOff" << std::endl;}
            template <class Event,class FSM>
            void on_exit(Event const&,FSM& ) {std::cout << "finishing: PlayerOff" << std::endl;}
        };
        struct CheckMiddleButton : public msm::front::state<>
        {
            template <class Event,class FSM>
            void on_entry(Event const&,FSM& ) {std::cout << "starting: CheckMiddleButton" << std::endl;}
            template <class Event,class FSM>
            void on_exit(Event const&,FSM& ) {std::cout << "finishing: CheckMiddleButton" << std::endl;}
        };
        struct PlayingMode_ : public msm::front::state_machine_def<PlayingMode_>
        {
            //flags 
            struct NoFastFwd{};

            struct Playing : public msm::front::state<default_base_state,msm::front::sm_ptr>
            {
                template <class Event,class FSM>
                void on_entry(Event const&,FSM& ) 
                {
                    std::cout << "starting: PlayingMode::Playing" << std::endl;
                    std::cout << "playing song:" << m_fsm->get_current_song() << std::endl;
                }
                template <class Event,class FSM>
                void on_exit(Event const&,FSM& ) {std::cout << "finishing: PlayingMode::Playing" << std::endl;}
                void set_sm_ptr(PlayingMode_* pl)
                {
                    m_fsm = pl;
                }
            private:
                PlayingMode_* m_fsm;
            };
            struct Paused : public msm::front::state<>
            {
                template <class Event,class FSM>
                void on_entry(Event const&,FSM& ) {std::cout << "starting: PlayingMode::Paused" << std::endl;}
                template <class Event,class FSM>
                void on_exit(Event const&,FSM& ) {std::cout << "finishing: PlayingMode::Paused" << std::endl;}
            };
            struct WaitingForNextPrev : public msm::front::state<>
            {
                template <class Event,class FSM>
                void on_entry(Event const&,FSM& ) {std::cout << "starting: PlayingMode::WaitingForNextPrev" << std::endl;}
                template <class Event,class FSM>
                void on_exit(Event const&,FSM& ) {std::cout << "finishing: PlayingMode::WaitingForNextPrev" << std::endl;}
            };
            struct WaitingForEnd : public msm::front::state<>
            {
                template <class Event,class FSM>
                void on_entry(Event const&,FSM& ) {std::cout << "starting: PlayingMode::WaitingForEnd" << std::endl;}
                template <class Event,class FSM>
                void on_exit(Event const&,FSM& ) {std::cout << "finishing: PlayingMode::WaitingForEnd" << std::endl;}
            };
            struct NoForward : public msm::front::state<>
            {
                template <class Event,class FSM>
                void on_entry(Event const&,FSM& ) {std::cout << "starting: PlayingMode::NoForward" << std::endl;}
                template <class Event,class FSM>
                void on_exit(Event const&,FSM& ) {std::cout << "finishing: PlayingMode::NoForward" << std::endl;}
            };
            struct ForwardPressed : public msm::front::state<>
            {
                template <class Event,class FSM>
                void on_entry(Event const&,FSM& ) 
                {
                    std::cout << "starting: PlayingMode::ForwardPressed," << "start timer" << std::endl;
                }
                template <class Event,class FSM>
                void on_exit(Event const&,FSM& ) 
                {
                    std::cout << "finishing: PlayingMode::ForwardPressed," << "stop timer" << std::endl;
                }
            };
            struct FastForward : public msm::front::state<>
            {
                template <class Event,class FSM>
                void on_entry(Event const&,FSM& ) 
                {
                    std::cout << "starting: PlayingMode::FastForward," << "start timer" << std::endl;
                }
                template <class Event,class FSM>
                void on_exit(Event const&,FSM& ) 
                {
                    std::cout << "finishing: PlayingMode::FastForward," << "stop timer" << std::endl;
                }
            };
            struct StdDisplay : public msm::front::state<>
            {
                template <class Event,class FSM>
                void on_entry(Event const&,FSM& ) {std::cout << "starting: PlayingMode::StdDisplay" << std::endl;}
                template <class Event,class FSM>
                void on_exit(Event const&,FSM& ) {std::cout << "finishing: PlayingMode::StdDisplay" << std::endl;}
            };
            struct SetPosition : public msm::front::state<>
            {
                typedef mpl::vector1<NoFastFwd>		flag_list;
                template <class Event,class FSM>
                void on_entry(Event const&,FSM& ) {std::cout << "starting: PlayingMode::SetPosition" << std::endl;}
                template <class Event,class FSM>
                void on_exit(Event const&,FSM& ) {std::cout << "finishing: PlayingMode::SetPosition" << std::endl;}
            };
            struct SetMark : public msm::front::state<>
            {
                template <class Event,class FSM>
                void on_entry(Event const&,FSM& ) {std::cout << "starting: PlayingMode::SetMark" << std::endl;}
                template <class Event,class FSM>
                void on_exit(Event const&,FSM& ) {std::cout << "finishing: PlayingMode::SetMark" << std::endl;}
            };
            struct PlayingExit : public msm::front::exit_pseudo_state<EndPlay>
            {
                template <class Event,class FSM>
                void on_entry(Event const&,FSM& ) {std::cout << "starting: PlayingMode::PlayingExit" << std::endl;}
                template <class Event,class FSM>
                void on_exit(Event const&,FSM& ) {std::cout << "finishing: PlayingMode::PlayingExit" << std::endl;}
            };
            // transition action methods
            template <class EVT>
            void inc_song_counter(EVT const&)
            {
                if (++m_SongIndex <= m_NumberOfSongs )
                {
                    std::cout << "playing song:" << m_SongIndex << std::endl;
                }
                else
                {
                    // last song => end playing, next play will start at the beginning
                    m_SongIndex = 1;
                    (static_cast<PlayingMode*>(this))->process_event(EndPlay());
                }
            }
            void select_song(StartSong const& evt)
            {
                if ((evt.m_Selected>0) && (evt.m_Selected<=m_NumberOfSongs))
                {
                    m_SongIndex = evt.m_Selected;
                    std::cout << "selecting song:" << m_SongIndex << std::endl;
                }
                else
                {
                    // play current song
                    std::cout << "selecting song:" << m_SongIndex << std::endl;
                }
            }
            void dec_song_counter(PreviousSong const&)
            {
                if (--m_SongIndex >0 )
                {
                    std::cout << "playing song:" << m_SongIndex << std::endl;
                }
                else
                {
                    // before first song => end playing
                    m_SongIndex = 1;
                    (static_cast<PlayingMode*>(this))->process_event(EndPlay());
                }
            }
            void send_NextSong(EastReleased const&)
            {
                // event internal to PlayingMode
                (static_cast<PlayingMode*>(this))->process_event(NextSong());
            }
            void do_fast_forward(ForwardTimer const&)
            {
                std::cout << "moving song forward..." << std::endl;
            }
            /*void stop_timer_100ms(EastReleased const&)
            {

            }*/
            // transition guard methods
            bool fast_fwd_ok(EastPressed const&)
            {
                // guard accepts only if fast forward is possible (No SetPosition mode) 
                return !(static_cast<PlayingMode*>(this))->is_flag_active<NoFastFwd>();
            }
            // initial states / orthogonal zones
            typedef mpl::vector5<Playing,WaitingForNextPrev,WaitingForEnd,NoForward,StdDisplay> 
                                    initial_state;
            typedef PlayingMode_ fsm; // makes transition table cleaner
            // Transition table for player
            struct transition_table : mpl::vector19<
            //    Start                 Event                Next                 Action				      Guard
            //   +--------------------+---------------------+--------------------+--------------------------+----------------------+
            _row < Playing            , PlayPause           , Paused                                                               >,
            _row < Playing            , Off                 , Paused                                                               >,
           a_row < Playing            , StartSong           , Playing            , &fsm::select_song                               >,
            _row < Paused             , PlayPause           , Playing                                                              >,
           a_row < Playing            , SongFinished        , Playing            , &fsm::inc_song_counter                          >,
           a_row < Paused             , StartSong           , Playing            , &fsm::select_song                               >,
            //   +--------------------+---------------------+--------------------+--------------------------+----------------------+
           a_row < WaitingForNextPrev , PreviousSong        , WaitingForNextPrev , &fsm::dec_song_counter                          >,
           a_row < WaitingForNextPrev , NextSong            , WaitingForNextPrev , &fsm::inc_song_counter                          >,
            //   +--------------------+---------------------+--------------------+--------------------------+----------------------+
            _row < WaitingForEnd      , EndPlay             , PlayingExit                                                          >,
            //   +--------------------+---------------------+--------------------+--------------------------+----------------------+
           g_row < NoForward          , EastPressed         , ForwardPressed                                , &fsm::fast_fwd_ok    >,
           a_row < ForwardPressed     , EastReleased        , NoForward           , &fsm::send_NextSong                            >,
           a_row < ForwardPressed     , ForwardTimer        , FastForward         , &fsm::do_fast_forward                          >,
           a_row < FastForward        , ForwardTimer        , FastForward         , &fsm::do_fast_forward                          >,
            _row < FastForward        , EastReleased        , NoForward                                                            >,
            //   +--------------------+---------------------+---------------------+--------------------------+----------------------+
            _row < StdDisplay         , PlayingMiddleButton , SetPosition                                                          >,
            _row < SetPosition        , StartSong           , StdDisplay                                                           >,
            _row < SetPosition        , PlayingMiddleButton , SetMark                                                              >,
            _row < SetMark            , PlayingMiddleButton , StdDisplay                                                           >,
            _row < SetMark            , StartSong           , StdDisplay                                                           >
            //   +--------------------+---------------------+---------------------+--------------------------+----------------------+
            > {};
            //a fsm having exit pseudo states need a template constructor calling the one of the basic class
            //template <class ContainingSM>
            //PlayingMode_(ContainingSM* sm): 
            //    state_machine_<PlayingMode> (sm),
            //    m_SongIndex(1),
            //    // for simplicity we decide there are 5 songs
            //    m_NumberOfSongs(5){}

            PlayingMode_():
            m_SongIndex(1),
            // for simplicity we decide there are 5 songs
            m_NumberOfSongs(5){}

                int get_current_song(){return m_SongIndex;}
            private:
                int m_SongIndex;
                int m_NumberOfSongs;

        };
        typedef msm::back::state_machine<PlayingMode_> PlayingMode;

        struct MenuMode_ : public msm::front::state_machine_def<MenuMode_>
        {
            typedef mpl::vector1<MenuActive>		flag_list;
            struct WaitingForSongChoice : public msm::front::state<>
            {
                template <class Event,class FSM>
                void on_entry(Event const&,FSM& ) {std::cout << "starting: MenuMode::WaitingForSongChoice" << std::endl;}
                template <class Event,class FSM>
                void on_exit(Event const&,FSM& ) {std::cout << "finishing: MenuMode::WaitingForSongChoice" << std::endl;}
            };
            struct StartCurrentSong : public msm::front::state<>
            {
                template <class Event,class FSM>
                void on_entry(Event const&,FSM& ) {std::cout << "starting: MenuMode::StartCurrentSong" << std::endl;}
                template <class Event,class FSM>
                void on_exit(Event const&,FSM& ) {std::cout << "finishing: MenuMode::StartCurrentSong" << std::endl;}
            };
            struct MenuExit : public msm::front::exit_pseudo_state<CloseMenu>
            {
                template <class Event,class FSM>
                void on_entry(Event const&,FSM& ) {std::cout << "starting: MenuMode::WaitingForSongChoice" << std::endl;}
                template <class Event,class FSM>
                void on_exit(Event const&,FSM& ) {std::cout << "finishing: MenuMode::WaitingForSongChoice" << std::endl;}
            };
            typedef WaitingForSongChoice initial_state;
            typedef MenuMode_ fsm; // makes transition table cleaner
            // Transition table for player
            struct transition_table : mpl::vector2<
            //     Start                 Event           Next                Action				   Guard
            //    +---------------------+------------------+-------------------+---------------------+----------------------+
            _row < WaitingForSongChoice , MenuMiddleButton , StartCurrentSong                                               >,
            _row < StartCurrentSong     , SelectSong       , MenuExit                                                       >
            //    +---------------------+------------------+-------------------+---------------------+----------------------+
            > {};
            //a fsm having exit pseudo states need a template constructor calling the one of the basic class
            //template <class ContainingSM>
            //MenuMode_(ContainingSM* sm): state_machine_<MenuMode> (sm){}
            //MenuMode_(){}
        };
        typedef msm::back::state_machine<MenuMode_> MenuMode;

        // the initial state of the player SM. Must be defined
        typedef mpl::vector5<NotHolding,NotPlaying,NoMenuMode,NoOnOffButton,CheckMiddleButton> 
                                    initial_state;
        // transition actions
        void send_ActivateMenu(EndPlay const&)
        {
            // we need to activate the menu and simulate its button
            (static_cast<iPod*>(this))->process_event(MenuButton());
        }
        void send_StartSong(CloseMenu const&)
        {
            // we suppose the 5th song was selected
           (static_cast<iPod*>(this))->process_event(StartSong(5));
        }
        void send_PlayPause(SouthReleased const&)
        {
            // action using the message queue to generate another event
            (static_cast<iPod*>(this))->process_event(PlayPause());
        }
        void send_Off(OnOffTimer const&)
        {
            std::cout << "turning player off" << std::endl;
            (static_cast<iPod*>(this))->process_event(Off());
        }
        void send_PlayingMiddleButton(MiddleButton const&)
        {
            (static_cast<iPod*>(this))->process_event(PlayingMiddleButton());
        }
        void send_MenuMiddleButton(MiddleButton const&)
        {
            (static_cast<iPod*>(this))->process_event(MenuMiddleButton());
        }
        // guard conditions
        bool is_menu(MiddleButton const&)
        {
            return (static_cast<iPod*>(this))->is_flag_active<MenuActive>();
        }
        bool is_no_menu(MiddleButton const& evt)
        {
            return !is_menu(evt);
        }
        template <class EVENT>
        void switch_on(EVENT const&)
        {
            std::cout << "turning player on" << std::endl;
        }
        typedef iPod_ fsm; // makes transition table cleaner

        // Transition table for player
        struct transition_table : mpl::vector13<
        //     Start               Event           Next                Action				            Guard
        //    +-------------------+---------------+-------------------+--------------------------------+----------------------+
        _row < NotHolding         , Hold          , Holding                                                                   >,
        _row < Holding            , NoHold        , NotHolding                                                                >,
        //    +-------------------+---------------+-------------------+--------------------------------+----------------------+
        _row < NotPlaying         , PlayPause     , PlayingMode                                                               >,
       a_row < PlayingMode::exit_pt<PlayingMode_::
                PlayingExit>       , EndPlay       , NotPlaying        , &fsm::send_ActivateMenu                               >,
        //    +-------------------+---------------+-------------------+--------------------------------+----------------------+
        _row < NoMenuMode         , MenuButton    , MenuMode                                                                  >,
       a_row < MenuMode::exit_pt<MenuMode_::
                MenuExit>          , CloseMenu     , NoMenuMode        , &fsm::send_StartSong                                  >,
        //    +-------------------+---------------+-------------------+--------------------------------+----------------------+
        _row < NoOnOffButton      , SouthPressed  , OffDown                                                                   >,
       a_row < OffDown            , SouthReleased , NoOnOffButton     , &fsm::send_PlayPause                                  >,
       a_row < OffDown            , OnOffTimer    , PlayerOff         , &fsm::send_Off                                        >,
       a_row < PlayerOff          , SouthPressed  , NoOnOffButton     , &fsm::switch_on                                       >,
       a_row < PlayerOff          , NoHold        , NoOnOffButton     , &fsm::switch_on                                       >,
       //     +-------------------+---------------+--------------------+--------------------------------+----------------------+
         row < CheckMiddleButton  , MiddleButton  , CheckMiddleButton , &fsm::send_PlayingMiddleButton  , &fsm::is_menu       >,
         row < CheckMiddleButton  , MiddleButton  , CheckMiddleButton , &fsm::send_MenuMiddleButton     , &fsm::is_no_menu    >
        //    +-------------------+---------------+--------------------+--------------------------------+----------------------+
        > {};
       
        // Replaces the default no-transition response.
        template <class FSM,class Event>
        void no_transition(Event const& e, FSM&,int state)
        {
            std::cout << "no transition from state " << state
                << " on event " << typeid(e).name() << std::endl;
        }
    };
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
