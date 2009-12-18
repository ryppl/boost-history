#include <vector>
#include <iostream>
#include "boost/mpl/vector/vector50.hpp"
#include <boost/msm/state_machine.hpp>

using namespace boost::msm;

namespace  // Concrete FSM implementation
{
    // events
    struct play {};
    struct end_pause {};
    struct stop {};
    struct pause {};
    struct open_close {};

    // A "complicated" event type that carries some data.
    // forward
    struct player;
    enum DiskTypeEnum
    {
        DISK_CD=0,
        DISK_DVD=1
    };
    struct cd_detected
    {
        cd_detected(std::string name, DiskTypeEnum diskType,player& p)
            : name(name),
            disc_type(diskType),
            m_player(p)
        {}

        std::string name;
        DiskTypeEnum disc_type;
        player& m_player;
    };

    // Concrete FSM implementation 
    struct player : public state_machine<player>
    {
        // The list of FSM states
        struct Empty : public state<> 
        {
            // every (optional) entry/exit methods get the event passed.
            template <class Event>
            void on_entry(Event const& ) {std::cout << "entering: Empty" << std::endl;}
            template <class Event>
            void on_exit(Event const& ) {std::cout << "leaving: Empty" << std::endl;}
        };
        struct Open : public state<> 
        {	 
            template <class Event>
            void on_entry(Event const& ) {std::cout << "entering: Open" << std::endl;}
            template <class Event>
            void on_exit(Event const& ) {std::cout << "leaving: Open" << std::endl;}
        };

        struct Stopped : public state<> 
        {	 
            // when stopped, the CD is loaded
            template <class Event>
            void on_entry(Event const& ) {std::cout << "entering: Stopped" << std::endl;}
            template <class Event>
            void on_exit(Event const& ) {std::cout << "leaving: Stopped" << std::endl;}
        };

        struct Playing : public state<>
        {
            template <class Event>
            void on_entry(Event const& ) {std::cout << "entering: Playing" << std::endl;}
            template <class Event>
            void on_exit(Event const& ) {std::cout << "leaving: Playing" << std::endl;}
        };

        // state not defining any entry or exit
        struct Paused : public state<>
        {
            ~Paused()
            {
                std::cout << "Paused: destruct" << std::endl;
            }
        };

        // the initial state of the player SM. Must be defined
        typedef Empty initial_state;

#ifdef __MWERKS__
    public: // Codewarrior bug workaround.  Tested at 0x3202
#endif 
        // transition actions
        void start_playback(play const&)       { std::cout << "player::start_playback\n"; }
        void open_drawer(open_close const&)    { std::cout << "player::open_drawer\n"; }
        void close_drawer(open_close const&)   { std::cout << "player::close_drawer\n"; }
        void store_cd_info(cd_detected const& cd) 
        { 
            std::cout << "player::store_cd_info\n"; 
            // generate another event to test the queue
            cd.m_player.process_event(play());
        }
        void stop_playback(stop const&)        { std::cout << "player::stop_playback\n"; }
        void pause_playback(pause const&)      { std::cout << "player::pause_playback\n"; }
        void resume_playback(end_pause const&)      { std::cout << "player::resume_playback\n"; }
        void stop_and_open(open_close const&)  { std::cout << "player::stop_and_open\n"; }
        void stopped_again(stop const&)	{std::cout << "player::stopped_again\n";}
        // guard conditions
        bool good_disk_format(cd_detected const& evt)
        {
            // to test a guard condition, let's say we understand only CDs, not DVD
            if (evt.disc_type != DISK_CD)
            {
                std::cout << "wrong disk, sorry" << std::endl;
                return false;
            }
            return true;
        }
        bool auto_start(cd_detected const& evt)
        {
            return false;
        }

#ifdef __MWERKS__
    private:
#endif 
        typedef player p; // makes transition table cleaner

        // Transition table for player
        struct transition_table : mpl::vector<
            //    Start     Event         Next      Action				Guard
            //  +---------+-------------+---------+---------------------+----------------------+
            a_row < Stopped , play        , Playing , &p::start_playback                       >,
            a_row < Stopped , open_close  , Open    , &p::open_drawer                          >,
            _row < Stopped , stop        , Stopped                                            >,
            //  +---------+-------------+---------+---------------------+----------------------+
            a_row < Open    , open_close  , Empty   , &p::close_drawer                         >,
            //  +---------+-------------+---------+---------------------+----------------------+
            a_row < Empty   , open_close  , Open    , &p::open_drawer                          >,
            row < Empty   , cd_detected , Stopped , &p::store_cd_info   ,&p::good_disk_format>,
            row < Empty   , cd_detected , Playing , &p::store_cd_info   ,&p::auto_start      >,
            //  +---------+-------------+---------+---------------------+----------------------+
            a_row < Playing , stop        , Stopped , &p::stop_playback                        >,
            a_row < Playing , pause       , Paused  , &p::pause_playback                       >,
            a_row < Playing , open_close  , Open    , &p::stop_and_open                        >,
            //  +---------+-------------+---------+---------------------+----------------------+
            a_row < Paused  , end_pause   , Playing , &p::resume_playback                      >,
            a_row < Paused  , stop        , Stopped , &p::stop_playback                        >,
            a_row < Paused  , open_close  , Open    , &p::stop_and_open                        >
            //  +---------+-------------+---------+---------------------+----------------------+
        > {};

        // Replaces the default no-transition response.
        template <class Event>
        int no_transition(int state, Event const& e)
        {
            std::cout << "no transition from state " << state
                << " on event " << typeid(e).name() << std::endl;
            return state;
        }
    };

    //
    // Testing utilities.
    //
    static char const* const state_names[] = { "Stopped", "Open", "Empty", "Playing", "Paused" };
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
            cd_detected("louie, louie",DISK_DVD,p)); pstate(p);
        p.process_event(
            cd_detected("louie, louie",DISK_CD,p)); pstate(p);
        // no need to call play() as the previous event does it in its action method

        // at this point, Play is active      
        p.process_event(pause()); pstate(p);
        // go back to Playing
        p.process_event(end_pause());  pstate(p);
        p.process_event(pause()); pstate(p);
        p.process_event(stop());  pstate(p);
        // event leading to the same state
        // no action method called as it is not present in the transition table
        p.process_event(stop());  pstate(p);
    }
}

int main()
{
    test();
    return 0;
}
