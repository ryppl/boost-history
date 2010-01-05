#ifndef LOGGING_FUNCTORS
#define LOGGING_FUNCTORS

struct Empty_Entry : euml_action<Empty_Entry>
{
    template <class Event,class FSM,class STATE>
    void operator()(Event const&,FSM&,STATE& )
    {
        std::cout << "entering: Empty" << std::endl;
    }
};
struct Empty_Exit : euml_action<Empty_Exit>
{
    template <class Event,class FSM,class STATE>
    void operator()(Event const&,FSM&,STATE& )
    {
        std::cout << "leaving: Empty" << std::endl;
    }
};

struct Open_Entry : euml_action<Open_Entry>
{
    template <class Event,class FSM,class STATE>
    void operator()(Event const&,FSM&,STATE& )
    {
        std::cout << "entering: Open" << std::endl;
    }
};
struct Open_Exit : euml_action<Open_Exit>
{
    template <class Event,class FSM,class STATE>
    void operator()(Event const&,FSM&,STATE& )
    {
        std::cout << "leaving: Open" << std::endl;
    }
};

struct Stopped_Entry : euml_action<Stopped_Entry>
{
    template <class Event,class FSM,class STATE>
    void operator()(Event const&,FSM&,STATE& )
    {
        std::cout << "entering: Stopped" << std::endl;
    }
};
struct Stopped_Exit : euml_action<Stopped_Exit>
{
    template <class Event,class FSM,class STATE>
    void operator()(Event const&,FSM&,STATE& )
    {
        std::cout << "leaving: Stopped" << std::endl;
    }
};

struct AllOk_Entry : euml_action<AllOk_Entry>
{
    template <class Event,class FSM,class STATE>
    void operator()(Event const&,FSM&,STATE& )
    {
        std::cout << "starting: AllOk" << std::endl;
    }
};
struct AllOk_Exit : euml_action<AllOk_Exit>
{
    template <class Event,class FSM,class STATE>
    void operator()(Event const&,FSM&,STATE& )
    {
        std::cout << "finishing: AllOk" << std::endl;
    }
};

struct ErrorMode_Entry : euml_action<ErrorMode_Entry>
{
    template <class Event,class FSM,class STATE>
    void operator()(Event const&,FSM&,STATE& )
    {
        std::cout << "starting: ErrorMode" << std::endl;
    }
};
struct ErrorMode_Exit : euml_action<ErrorMode_Exit>
{
    template <class Event,class FSM,class STATE>
    void operator()(Event const&,FSM&,STATE& )
    {
        std::cout << "finishing: ErrorMode" << std::endl;
    }
};

struct Playing_Entry : euml_action<Playing_Entry>
{
    template <class Event,class FSM,class STATE>
    void operator()(Event const&,FSM&,STATE& )
    {
        std::cout << "entering: Playing" << std::endl;
    }
};
struct Playing_Exit : euml_action<Playing_Exit>
{
    template <class Event,class FSM,class STATE>
    void operator()(Event const&,FSM&,STATE& )
    {
        std::cout << "leaving: Playing" << std::endl;
    }
};

struct Song1_Entry : euml_action<Song1_Entry>
{
    template <class Event,class FSM,class STATE>
    void operator()(Event const&,FSM&,STATE& )
    {
        std::cout << "starting: First song" << std::endl;
    }
};
struct Song1_Exit : euml_action<Song1_Exit>
{
    template <class Event,class FSM,class STATE>
    void operator()(Event const&,FSM&,STATE& )
    {
        std::cout << "finishing: First Song" << std::endl;
    }
};

struct Song2_Entry : euml_action<Song2_Entry>
{
    template <class Event,class FSM,class STATE>
    void operator()(Event const&,FSM&,STATE& )
    {
        std::cout << "starting: Second song" << std::endl;
    }
};
struct Song2_Exit : euml_action<Song2_Exit>
{
    template <class Event,class FSM,class STATE>
    void operator()(Event const&,FSM&,STATE& )
    {
        std::cout << "finishing: Second Song" << std::endl;
    }
};

struct Song3_Entry : euml_action<Song3_Entry>
{
    template <class Event,class FSM,class STATE>
    void operator()(Event const&,FSM&,STATE& )
    {
        std::cout << "starting: Third song" << std::endl;
    }
};
struct Song3_Exit : euml_action<Song3_Exit>
{
    template <class Event,class FSM,class STATE>
    void operator()(Event const&,FSM&,STATE& )
    {
        std::cout << "finishing: Third Song" << std::endl;
    }
};

struct Region2State1_Entry : euml_action<Region2State1_Entry>
{
    template <class Event,class FSM,class STATE>
    void operator()(Event const&,FSM&,STATE& )
    {
        std::cout << "starting: Region2State1" << std::endl;
    }
};
struct Region2State1_Exit : euml_action<Region2State1_Exit>
{
    template <class Event,class FSM,class STATE>
    void operator()(Event const&,FSM&,STATE& )
    {
        std::cout << "finishing: Region2State1" << std::endl;
    }
};
struct Region2State2_Entry : euml_action<Region2State2_Entry>
{
    template <class Event,class FSM,class STATE>
    void operator()(Event const&,FSM&,STATE& )
    {
        std::cout << "starting: Region2State2" << std::endl;
    }
};
struct Region2State2_Exit : euml_action<Region2State2_Exit>
{
    template <class Event,class FSM,class STATE>
    void operator()(Event const&,FSM&,STATE& )
    {
        std::cout << "finishing: Region2State2" << std::endl;
    }
};
// transition actions for Playing
struct start_next_song : euml_action<start_next_song>
{
    template <class FSM,class EVT,class SourceState,class TargetState>
    void operator()(EVT const& ,FSM&,SourceState& ,TargetState& )
    {
        std::cout << "Playing::start_next_song" << endl;
    }
};
struct start_prev_song : euml_action<start_prev_song>
{
    template <class FSM,class EVT,class SourceState,class TargetState>
    void operator()(EVT const& ,FSM&,SourceState& ,TargetState& )
    {
        std::cout << "Playing::start_prev_song" << endl;
    }
};

// transition actions
struct start_playback : euml_action<start_playback>
{
    template <class FSM,class EVT,class SourceState,class TargetState>
    void operator()(EVT const& ,FSM&,SourceState& ,TargetState& )
    {
        cout << "player::start_playback" << endl;
    }
};
struct open_drawer : euml_action<open_drawer>
{
    template <class FSM,class EVT,class SourceState,class TargetState>
    void operator()(EVT const& ,FSM&,SourceState& ,TargetState& )
    {
        cout << "player::open_drawer" << endl;
    }
};
struct close_drawer : euml_action<close_drawer>
{
    template <class FSM,class EVT,class SourceState,class TargetState>
    void operator()(EVT const& ,FSM&,SourceState& ,TargetState& )
    {
        cout << "player::close_drawer" << endl;
    }
};
struct store_cd_info : euml_action<store_cd_info>
{
    template <class FSM,class EVT,class SourceState,class TargetState>
    void operator()(EVT const&, FSM& ,SourceState& ,TargetState& )
    {
        cout << "player::store_cd_info" << endl;
        // it is now easy to use the message queue. 
        // alternatively to the proces_ in the transition table, we could write:
        // fsm.process_event(play());
    }
};
struct stop_playback : euml_action<stop_playback>
{
    template <class FSM,class EVT,class SourceState,class TargetState>
    void operator()(EVT const& ,FSM&,SourceState& ,TargetState& )
    {
        cout << "player::stop_playback" << endl;
    }
};
struct pause_playback : euml_action<pause_playback>
{
    template <class FSM,class EVT,class SourceState,class TargetState>
    void operator()(EVT const& ,FSM&,SourceState& ,TargetState& )
    {
        cout << "player::pause_playback" << endl;
    }
};
struct resume_playback : euml_action<resume_playback>
{
    template <class FSM,class EVT,class SourceState,class TargetState>
    void operator()(EVT const& ,FSM&,SourceState& ,TargetState& )
    {
        cout << "player::resume_playback" << endl;
    }
};
struct stop_and_open : euml_action<stop_and_open>
{
    template <class FSM,class EVT,class SourceState,class TargetState>
    void operator()(EVT const& ,FSM&,SourceState& ,TargetState& )
    {
        cout << "player::stop_and_open" << endl;
    }
};
struct stopped_again : euml_action<stopped_again>
{
    template <class FSM,class EVT,class SourceState,class TargetState>
    void operator()(EVT const& ,FSM&,SourceState& ,TargetState& )
    {
        cout << "player::stopped_again" << endl;
    }
};

struct report_error : euml_action<report_error>
{
    template <class FSM,class EVT,class SourceState,class TargetState>
    void operator()(EVT const& ,FSM&,SourceState& ,TargetState& )
    {
        cout << "player::report_error" << endl;
    }
};

struct report_end_error : euml_action<report_end_error>
{
    template <class FSM,class EVT,class SourceState,class TargetState>
    void operator()(EVT const& ,FSM&,SourceState& ,TargetState& )
    {
        cout << "player::report_end_error" << endl;
    }
};
struct internal_action1 : euml_action<internal_action1>
{
    template <class FSM,class EVT,class SourceState,class TargetState>
    void operator()(EVT const&, FSM& ,SourceState& ,TargetState& )
    {
        cout << "Open::internal action1" << endl;
    }
};
struct internal_action2 : euml_action<internal_action2>
{
    template <class FSM,class EVT,class SourceState,class TargetState>
    void operator()(EVT const&, FSM& ,SourceState& ,TargetState& )
    {
        cout << "Open::internal action2" << endl;
    }
};
struct internal_action : euml_action<internal_action2>
{
    template <class FSM,class EVT,class SourceState,class TargetState>
    void operator()(EVT const&, FSM& ,SourceState& ,TargetState& )
    {
        cout << "Open::internal action" << endl;
    }
};
enum DiskTypeEnum
    {
        DISK_CD=0,
        DISK_DVD=1
    };
// guard conditions
struct good_disk_format : euml_action<good_disk_format>
{
    template <class FSM,class EVT,class SourceState,class TargetState>
    bool operator()(EVT const& evt,FSM&,SourceState& ,TargetState& )
    {
        // to test a guard condition, let's say we understand only CDs, not DVD
        if (evt.template get_attribute<1>()!=DISK_CD)
        {
            std::cout << "wrong disk, sorry" << std::endl;
            // just for logging, does not block any transition
            return true;
        }
        std::cout << "good disk" << std::endl;
        return true;
    }
};

// Handler called when no_transition detected
struct Log_No_Transition : euml_action<Log_No_Transition>
{
    template <class FSM,class Event>
    void operator()(Event const& e,FSM&,int state)
    {
        std::cout << "no transition from state " << state
            << " on event " << typeid(e).name() << std::endl;
    }
};

struct internal_guard1 : euml_action<internal_guard1>
{
    template <class FSM,class EVT,class SourceState,class TargetState>
    bool operator()(EVT const&, FSM& ,SourceState& ,TargetState& )
    {
        cout << "Open::internal guard1" << endl;
        return false;
    }
};
struct internal_guard2 : euml_action<internal_guard2>
{
    template <class FSM,class EVT,class SourceState,class TargetState>
    bool operator()(EVT const&, FSM& ,SourceState& ,TargetState& )
    {
        cout << "Open::internal guard2" << endl;
        return false;
    }
};
#endif // LOGGING_FUNCTORS
