#include <vector>
#include <iostream>
#include <string>
#include <boost/mpl/vector/vector50.hpp>
#include <boost/msm/state_machine.hpp>
#include <boost/msm/tools.hpp>

using namespace boost::msm;
using namespace std;

namespace  // Concrete FSM implementation
{
    // events
    struct play {};
    struct end_pause {};
    struct stop {};
    struct pause {};
    struct open_close {};
    struct NextSong {};
    struct PreviousSong {};
    struct ThreeSec {};
    struct TenSec {};
    struct go_sleep {};
    struct error_found {};
    struct end_error {};

    // Flags. Allow information about a property of the current state
    struct PlayingPaused{};
    struct CDLoaded {};
    struct FirstSongPlaying {};

    // A "complicated" event type that carries some data.
    // forward
    struct player;
    struct cd_detected
    {
        cd_detected(std::string name, player& p)
            : name(name)
            , m_player(p)
        {}

        std::string name;
        player& m_player;
    };

    // an easy visitor
    struct SomeVisitor
    {
        template <class T>
        void visit_state(T* astate)
        {
            std::cout << "visiting state:" << typeid(*astate).name() << std::endl;
        }
    };
    // overwrite of the base state (not default)
    struct my_visitable_state
    {
        // signature of the accept function
        typedef args<void,SomeVisitor&> accept_sig;

        // we also want polymorphic states
        virtual ~my_visitable_state() {}
        // default implementation for states who do not need to be visited
        void accept(SomeVisitor&) const {}
    };

    // Concrete FSM implementation 
    struct player : public state_machine<player,NoHistory,my_visitable_state>
    {
        // The list of FSM states
        struct Empty : public state<my_visitable_state> 
        {
            typedef mpl::vector<play> deferred_events;
            // every (optional) entry/exit methods get the event packed as boost::any. Not useful very often.
            template <class Event>
            void on_entry(Event const& ) {std::cout << "entering: Empty" << std::endl;}
            template <class Event>
            void on_exit(Event const& ) {std::cout << "leaving: Empty" << std::endl;}
            void accept(SomeVisitor& vis) const
            {
                vis.visit_state(this);
            }
        };
        struct Open : public state<my_visitable_state> 
        {	 
            typedef mpl::vector1<CDLoaded>		flag_list;
            typedef mpl::vector<play> deferred_events;
            template <class Event>
            void on_entry(Event const& ) {std::cout << "entering: Open" << std::endl;}
            template <class Event>
            void on_exit(Event const& ) {std::cout << "leaving: Open" << std::endl;}
            void accept(SomeVisitor& vis) const
            {
                vis.visit_state(this);
            }
        };
        // a state needing a pointer to the containing state machine
        // and using for this the non-default policy
        // if policy used, set_sm_ptr is needed
        struct Stopped : public state<my_visitable_state,SMPtr> 
        {	 
            // when stopped, the CD is loaded
            typedef mpl::vector1<CDLoaded>		flag_list;
            template <class Event>
            void on_entry(Event const& ) {std::cout << "entering: Stopped" << std::endl;}
            template <class Event>
            void on_exit(Event const& ) {std::cout << "leaving: Stopped" << std::endl;}
            void set_sm_ptr(player* pl){m_player=pl;}
            player* m_player;
        };
        // the player state machine contains a state which is himself a state machine
        // it demonstrates Shallow History: if the state gets activated with end_pause
        // then it will remember the last active state and reactivate it
        // also possible: AlwaysHistory, the last active state will always be reactivated
        // or NoHistory, always restart from the initial state
        struct Playing : public state_machine<Playing,ShallowHistory<mpl::vector<end_pause> >,my_visitable_state >
        {
            // when playing, the CD is loaded and we are in either pause or playing (duh)
            typedef mpl::vector2<PlayingPaused,CDLoaded>		flag_list;
            template <class Event>
            void on_entry(Event const& ) {std::cout << "entering: Playing" << std::endl;}
            template <class Event>
            void on_exit(Event const& ) {std::cout << "leaving: Playing" << std::endl;}
            void accept(SomeVisitor& vis)
            {
                vis.visit_state(this);
                // visit substates
                visit_current_states(vis);
            }
            // The list of FSM states
            // the Playing state machine contains a state which is himself a state machine
            // so we have a SM containing a SM containing a SM
            struct Song1 : public state_machine<Song1,NoHistory,my_visitable_state>
            {
                typedef mpl::vector1<FirstSongPlaying>		flag_list;
                template <class Event>
                void on_entry(Event const& ) {std::cout << "starting: First song" << std::endl;}
                template <class Event>
                void on_exit(Event const& ) {std::cout << "finishing: First Song" << std::endl;}
                void accept(SomeVisitor& vis)
                {
                    vis.visit_state(this);
                }
                struct LightOn : public state<my_visitable_state>
                {	 
                    template <class Event>
                    void on_entry(Event const& ) {std::cout << "starting: LightOn" << std::endl;}
                    template <class Event>
                    void on_exit(Event const& ) {std::cout << "finishing: LightOn" << std::endl;}
                };
                struct LightOff : public state<my_visitable_state>
                {	 
                    template <class Event>
                    void on_entry(Event const& ) {std::cout << "starting: LightOff" << std::endl;}
                    template <class Event>
                    void on_exit(Event const& ) {std::cout << "finishing: LightOff" << std::endl;}
                };
                // the initial state. Must be defined
                typedef LightOn initial_state;
                // transition actions
                void turn_light_off(ThreeSec const&)       { std::cout << "3s off::turn light off\n"; }
                // guard conditions

                typedef Song1 s; // makes transition table cleaner
                // Transition table for Song1
                struct transition_table : mpl::vector1<
                    //    Start     Event         Next      Action				Guard
                    //  +---------+-------------+---------+---------------------+----------------------+
                    a_row < LightOn , ThreeSec    , LightOff, &s::turn_light_off                        >
                    //  +---------+-------------+---------+---------------------+----------------------+
                > {};
                // Replaces the default no-transition response.
                template <class Event>
                int no_transition(int state, Event const& )
                {
                    std::cout << "no transition from state " << state
                        << " on event " << typeid(Event).name() << std::endl;
                    return state;
                }

            };
            struct Song2 : public state<my_visitable_state>
            {	 
                template <class Event>
                void on_entry(Event const& ) {std::cout << "starting: Second song" << std::endl;}
                template <class Event>
                void on_exit(Event const& ) {std::cout << "finishing: Second Song" << std::endl;}
            };
            struct Song3 : public state<my_visitable_state>
            {	 
                template <class Event>
                void on_entry(Event const& ) {std::cout << "starting: Third song" << std::endl;}
                template <class Event>
                void on_exit(Event const& ) {std::cout << "finishing: Third Song" << std::endl;}
            };
            // the initial state. Must be defined
            typedef Song1 initial_state;
            // transition actions
            void start_next_song(NextSong const&)       { std::cout << "Playing::start_next_song\n"; }
            void start_prev_song(PreviousSong const&)       { std::cout << "Playing::start_prev_song\n"; }
            // guard conditions

            typedef Playing pl; // makes transition table cleaner
            // Transition table for Playing
            struct transition_table : mpl::vector4<
                //    Start     Event         Next      Action				Guard
                //  +---------+-------------+---------+---------------------+----------------------+
                a_row < Song1   , NextSong    , Song2   , &pl::start_next_song                      >,
                a_row < Song2   , PreviousSong, Song1   , &pl::start_prev_song                      >,
                a_row < Song2   , NextSong    , Song3   , &pl::start_next_song                      >,
                a_row < Song3   , PreviousSong, Song2   , &pl::start_prev_song                      >
                //  +---------+-------------+---------+---------------------+----------------------+
            > {};
            // Replaces the default no-transition response.
            template <class Event>
            int no_transition(int state, Event const&)
            {
                std::cout << "no transition from state " << state
                    << " on event " << typeid(Event).name() << std::endl;
                return state;
            }
        };
        // the player state machine contains a state which is himself a state machine (2 of them, Playing and Paused)
        struct Paused : public state_machine<Paused,NoHistory,my_visitable_state>
        {
            typedef mpl::vector2<PlayingPaused,CDLoaded>		flag_list;
            template <class Event>
            void on_entry(Event const& ) {std::cout << "entering: Paused" << std::endl;}
            template <class Event>
            void on_exit(Event const& ) {std::cout << "leaving: Paused" << std::endl;}

            // The list of FSM states
            struct StartBlinking : public state<my_visitable_state>
            {	 
                template <class Event>
                void on_entry(Event const& ) {std::cout << "starting: StartBlinking" << std::endl;}
                template <class Event>
                void on_exit(Event const& ) {std::cout << "finishing: StartBlinking" << std::endl;}
            };
            struct StopBlinking : public state<my_visitable_state>
            {	 
                template <class Event>
                void on_entry(Event const& ) {std::cout << "starting: StopBlinking" << std::endl;}
                template <class Event>
                void on_exit(Event const& ) {std::cout << "finishing: StopBlinking" << std::endl;}
            };
            // the initial state. Must be defined
            typedef StartBlinking initial_state;
            // transition actions
            void start_blinking(TenSec const&)       { std::cout << "Paused::start_blinking\n"; }
            void stop_blinking(TenSec const&)       { std::cout << "Paused::stop_blinking\n"; }
            // guard conditions

            typedef Paused pa; // makes transition table cleaner
            // Transition table
            struct transition_table : mpl::vector2<
                //    Start			Event         Next				Action				Guard
                //  +---------------+-------------+--------------+---------------------+----------------------+
                a_row < StartBlinking , TenSec      , StopBlinking  , &pa::stop_blinking                        >,
                a_row < StopBlinking  , TenSec      , StartBlinking , &pa::start_blinking                       >
                //  +---------------+-------------+---------------+--------------------+----------------------+
            > {};
            // Replaces the default no-transition response.
            template <class Event>
            int no_transition(int state, Event const& )
            {
                std::cout << "no transition from state " << state
                    << " on event " << typeid(Event).name() << std::endl;
                return state;
            }
        };
        struct SleepMode : public state<my_visitable_state> {}; // dumy state just to test the automatic id generation

        struct AllOk : public state<my_visitable_state>
        {
            template <class Event>
            void on_entry(Event const& ) {std::cout << "starting: AllOk" << std::endl;}
            template <class Event>
            void on_exit(Event const& ) {std::cout << "finishing: AllOk" << std::endl;}
        };
        struct ErrorMode : //public terminate_state<>
            public interrupt_state<end_error,my_visitable_state>
        {
            template <class Event>
            void on_entry(Event const& ) {std::cout << "starting: ErrorMode" << std::endl;}
            template <class Event>
            void on_exit(Event const& ) {std::cout << "finishing: ErrorMode" << std::endl;}
        };

        // the initial state of the player SM. Must be defined
        typedef mpl::vector<Empty,AllOk> initial_state;
        //typedef Empty initial_state; // this is to have only one active state

#ifdef __MWERKS__
    public: // Codewarrior bug workaround.  Tested at 0x3202
#endif 
        // transition actions
        void start_playback(play const&)       { std::cout << "player::start_playback\n"; }
        void open_drawer(open_close const&)    { std::cout << "player::open_drawer\n"; }
        void close_drawer(open_close const&)   { std::cout << "player::close_drawer\n"; }
        void store_cd_info(cd_detected const&) 
        { 
            std::cout << "player::store_cd_info\n"; 
            // generate another event to test the queue
            //cd.m_player.process_event(play());
        }
        void stop_playback(stop const&)        { std::cout << "player::stop_playback\n"; }
        void pause_playback(pause const&)      { std::cout << "player::pause_playback\n"; }
        void resume_playback(end_pause const&)      { std::cout << "player::resume_playback\n"; }
        void stop_and_open(open_close const&)  { std::cout << "player::stop_and_open\n"; }
        void stopped_again(stop const&)	{std::cout << "player::stopped_again\n";}
        void start_sleep(go_sleep const&)  {  }
        void report_error(error_found const&) {std::cout << "player::report_error\n";}
        void report_end_error(end_error const&) {std::cout << "player::report_end_error\n";}
        // guard conditions


#ifdef __MWERKS__
    private:
#endif 
        typedef player p; // makes transition table cleaner

        // Transition table for player
        struct transition_table : mpl::vector<
            //    Start     Event         Next      Action				Guard
            //  +---------+-------------+---------+---------------------+----------------------+
            a_row < Stopped , play        , Playing , &p::start_playback                        >,
            a_row < Stopped , open_close  , Open    , &p::open_drawer                           >,
            a_row < Stopped , stop        , Stopped , &p::stopped_again                         >,
            //  +---------+-------------+---------+---------------------+----------------------+
            a_row < Open    , open_close  , Empty   , &p::close_drawer                          >,
            //  +---------+-------------+---------+---------------------+----------------------+
            a_row < Empty   , open_close  , Open    , &p::open_drawer                           >,
            a_row < Empty   , cd_detected , Stopped , &p::store_cd_info                         >,
            //  +---------+-------------+---------+---------------------+----------------------+
            a_row < Playing , stop        , Stopped , &p::stop_playback                         >,
            a_row < Playing , pause       , Paused  , &p::pause_playback                        >,
            a_row < Playing , open_close  , Open    , &p::stop_and_open                         >,
            //  +---------+-------------+---------+---------------------+----------------------+
            a_row < Paused  , end_pause   , Playing , &p::resume_playback                       >,
            a_row < Paused  , stop        , Stopped , &p::stop_playback                         >,
            a_row < Paused  , open_close  , Open    , &p::stop_and_open                         >,
            a_row < Paused  , go_sleep    ,SleepMode, &p::start_sleep                           >,
            //  +---------+-------------+---------+---------------------+----------------------+
            a_row < AllOk   , error_found ,ErrorMode, &p::report_error                          >,
            a_row <ErrorMode,end_error    ,AllOk    , &p::report_end_error                      >
            //  +---------+-------------+---------+---------------------+----------------------+
        > {};

        // Replaces the default no-transition response.
        template <class Event>
        int no_transition(int state, Event const& )
        {
            std::cout << "no transition from state " << state
                << " on event " << typeid(Event).name() << std::endl;
            return state;
        }
    };

    //
    // Testing utilities.
    //

    void pstate(player const& p)
    {
        // instead of the hard-coded array we can use fill_state_names:
        //static char const* const state_names[] = { "Stopped", "Open", "Empty", "Playing", "Paused","AllOk","ErrorMode","SleepMode" };
        typedef player::transition_table stt;	
        typedef generate_state_set<stt>::type all_states;
        static char const* state_names[mpl::size<all_states>::value];
        // fill the names of the states defined in the state machine
        mpl::for_each<all_states,boost::msm::wrap<mpl::placeholders::_1> >(fill_state_names<stt>(state_names));

        for (unsigned int i=0;i<p.current_state().size();++i)
        {
            std::cout << " -> " << state_names[p.current_state()[i]] << std::endl;
        }
    }
    void test()
    {
        player p;
        // get a pointer to a state
        // just to show how to do it as Stopped gets a pointer to player anyway
        player::Stopped* tempstate = p.get_state<player::Stopped*>();
        tempstate->m_player = &p;
        // get a reference to a state
        player::Stopped& tempstate2 = p.get_state<player::Stopped&>();
        tempstate2.m_player = &p;
        // make a copy of a state
        player::Stopped tempstate3 = p.get_state<player::Stopped&>();

        // needed to start the highest-level SM. This will call on_entry and mark the start of the SM
        p.start(); 
        std::cout << "CDLoaded active:" << std::boolalpha << p.is_flag_active<CDLoaded>() << std::endl; //=> false (no CD yet)

        // test deferred event
        // deferred in Empty and Open, will be handled only after event cd_detected
        p.process_event(play());

        // go to Open, call on_exit on Empty, then action, then on_entry on Open
        p.process_event(open_close()); pstate(p);
        // visiting Paused and AllOk, but only Paused cares
        SomeVisitor vis;
        p.visit_current_states(boost::ref(vis));
        p.process_event(open_close()); pstate(p);
        // visiting Empty and AllOk, but only Empty cares
        p.visit_current_states(boost::ref(vis));


        p.process_event(cd_detected("louie, louie",p));
        // no need to call play() as the previous event does it in its action method
        //p.process_event(play());  
        // at this point, Play is active, along FirstSong and LightOn
        pstate(p);
        // visiting Playing+Song1 and AllOk, but only Playing+Song1 care
        p.visit_current_states(boost::ref(vis));

        std::cout << "PlayingPaused active:" << std::boolalpha << p.is_flag_active<PlayingPaused>() << std::endl;//=> true
        // call on_exit on LightOn,FirstSong,Play like stated in the UML spec.
        // and of course on_entry on Paused and StartBlinking
        p.process_event(pause()); pstate(p);
        std::cout << "PlayingPaused active:" << std::boolalpha << p.is_flag_active<PlayingPaused>() << std::endl;//=> true
        // forward events to Paused
        p.process_event(TenSec());
        p.process_event(TenSec());
        // go back to Playing
        p.process_event(end_pause());  pstate(p);
        std::cout << "FirstSong active:" << std::boolalpha << p.is_flag_active<FirstSongPlaying>() << std::endl; //=> true
        p.process_event(ThreeSec());  pstate(p);
        p.process_event(NextSong());pstate(p);
        // We are now in second song, Flag inactive
        std::cout << "FirstSong active:" << std::boolalpha << p.is_flag_active<FirstSongPlaying>() << std::endl;//=> false
        // visiting Playing+Song2 and AllOk, but only Playing cares
        p.visit_current_states(vis);

        p.process_event(NextSong());pstate(p);
        // 2nd song active
        p.process_event(PreviousSong());pstate(p);
        // Pause
        p.process_event(pause()); pstate(p);
        // go back to Playing
        // but end_pause is an event activating the History
        // => keep the last active State (SecondSong)
        p.process_event(end_pause());  pstate(p);
        // test of an event from a state to itself. According to UML spec, call again exit/entry from Stopped
        p.process_event(stop());  pstate(p);
        p.process_event(stop());  pstate(p);
        std::cout << "CDLoaded active:" << std::boolalpha << p.is_flag_active<CDLoaded>() << std::endl;//=> true
        std::cout << "PlayingPaused active:" << std::boolalpha << p.is_flag_active<PlayingPaused>() << std::endl;//=> false
        std::cout << "FirstSong active:" << std::boolalpha << p.is_flag_active<FirstSongPlaying>() << std::endl;//=> false
        std::cout << "CDLoaded active with AND:" << std::boolalpha << p.is_flag_active<CDLoaded,player::Flag_AND>() << std::endl;//=> false
        std::cout << "CDLoaded active with OR:" << std::boolalpha << p.is_flag_active<CDLoaded,player::Flag_OR>() << std::endl;//=> true

        // go back to Playing
        // but play is not leading to Shallow History => do not remember the last active State (SecondSong)
        // and activate again FirstSong and LightOn
        p.process_event(play());  pstate(p);
        p.process_event(error_found());  pstate(p);

        // try generating more events
        std::cout << "Trying to generate another event" << std::endl; // will not work, fsm is terminated or interrupted
        p.process_event(NextSong());pstate(p);

        std::cout << "Trying to end the error" << std::endl; // will work only if ErrorMode is interrupt state
        p.process_event(end_error());pstate(p);
        std::cout << "Trying to generate another event" << std::endl; // will work only if ErrorMode is interrupt state
        p.process_event(NextSong());pstate(p);

        std::cout << "Simulate error. Event play is not valid" << std::endl;
        p.process_event(play()); pstate(p);
    }
}

int main()
{
    test();
    return 0;
}


