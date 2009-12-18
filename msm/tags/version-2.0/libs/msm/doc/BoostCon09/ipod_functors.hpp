#ifndef IPOD_FUNCTORS_HPP
#define IPOD_FUNCTORS_HPP
#include <boost/msm/front/euml/euml.hpp>

struct NotHolding_Entry : euml_action<NotHolding_Entry>
{
    template <class Event,class FSM,class STATE>
    void operator()(Event const&,FSM&,STATE& )
    {
        std::cout << "entering: NotHolding" << std::endl;
    }
};
struct Holding_Entry : euml_action<Holding_Entry>
{
    template <class Event,class FSM,class STATE>
    void operator()(Event const&,FSM&,STATE& )
    {
        std::cout << "entering: Holding" << std::endl;
    }
};
struct NotPlaying_Entry : euml_action<NotPlaying_Entry>
{
    template <class Event,class FSM,class STATE>
    void operator()(Event const&,FSM&,STATE& )
    {
        std::cout << "entering: NotPlaying" << std::endl;
    }
};
struct NoMenuMode_Entry : euml_action<NoMenuMode_Entry>
{
    template <class Event,class FSM,class STATE>
    void operator()(Event const&,FSM&,STATE& )
    {
        std::cout << "entering: NoMenuMode" << std::endl;
    }
};
struct NoOnOffButton_Entry : euml_action<NoOnOffButton_Entry>
{
    template <class Event,class FSM,class STATE>
    void operator()(Event const&,FSM&,STATE& )
    {
        std::cout << "entering: NoOnOffButton" << std::endl;
    }
};
struct OffDown_Entry : euml_action<OffDown_Entry>
{
    template <class Event,class FSM,class STATE>
    void operator()(Event const&,FSM&,STATE& )
    {
        std::cout << "entering: OffDown" << std::endl;
    }
};
struct PlayerOff_Entry : euml_action<PlayerOff_Entry>
{
    template <class Event,class FSM,class STATE>
    void operator()(Event const&,FSM&,STATE& )
    {
        std::cout << "entering: PlayerOff" << std::endl;
    }
};
struct CheckMiddleButton_Entry : euml_action<CheckMiddleButton_Entry>
{
    template <class Event,class FSM,class STATE>
    void operator()(Event const&,FSM&,STATE& )
    {
        std::cout << "entering: CheckMiddleButton" << std::endl;
    }
};
struct Playing_Entry : euml_action<Playing_Entry>
{
    template <class Event,class FSM,class STATE>
    void operator()(Event const&,FSM& fsm,STATE& )
    {
        std::cout << "entering: Playing" << std::endl;
        std::cout << "playing song:" << fsm.template get_attribute<m_SongIndex>() << std::endl;
    }
};
struct WaitingForNextPrev_Entry : euml_action<WaitingForNextPrev_Entry>
{
    template <class Event,class FSM,class STATE>
    void operator()(Event const&,FSM&,STATE& )
    {
        std::cout << "entering: WaitingForNextPrev" << std::endl;
    }
};
struct Paused_Entry : euml_action<Paused_Entry>
{
    template <class Event,class FSM,class STATE>
    void operator()(Event const&,FSM&,STATE& )
    {
        std::cout << "entering: Paused" << std::endl;
    }
};
struct WaitingForEnd_Entry : euml_action<WaitingForEnd_Entry>
{
    template <class Event,class FSM,class STATE>
    void operator()(Event const&,FSM&,STATE& )
    {
        std::cout << "entering: OffDown" << std::endl;
    }
};
struct NoForward_Entry : euml_action<NoForward_Entry>
{
    template <class Event,class FSM,class STATE>
    void operator()(Event const&,FSM&,STATE& )
    {
        std::cout << "entering: NoForward" << std::endl;
    }
};
struct ForwardPressed_Entry : euml_action<ForwardPressed_Entry>
{
    template <class Event,class FSM,class STATE>
    void operator()(Event const&,FSM&,STATE& )
    {
        std::cout << "entering: ForwardPressed" << std::endl;
    }
};
struct ForwardPressed_Exit : euml_action<ForwardPressed_Exit>
{
    template <class Event,class FSM,class STATE>
    void operator()(Event const&,FSM&,STATE& )
    {
        std::cout << "leaving: ForwardPressed" << std::endl;
    }
};
struct FastForward_Entry : euml_action<FastForward_Entry>
{
    template <class Event,class FSM,class STATE>
    void operator()(Event const&,FSM&,STATE& )
    {
        std::cout << "entering: FastForward" << std::endl;
    }
};
struct FastForward_Exit : euml_action<FastForward_Exit>
{
    template <class Event,class FSM,class STATE>
    void operator()(Event const&,FSM&,STATE& )
    {
        std::cout << "leaving: FastForward" << std::endl;
    }
};
struct StdDisplay_Entry : euml_action<StdDisplay_Entry>
{
    template <class Event,class FSM,class STATE>
    void operator()(Event const&,FSM&,STATE& )
    {
        std::cout << "entering: StdDisplay" << std::endl;
    }
};
struct SetPosition_Entry : euml_action<SetPosition_Entry>
{
    template <class Event,class FSM,class STATE>
    void operator()(Event const&,FSM&,STATE& )
    {
        std::cout << "entering: SetPosition" << std::endl;
    }
};
struct SetMark_Entry : euml_action<SetMark_Entry>
{
    template <class Event,class FSM,class STATE>
    void operator()(Event const&,FSM&,STATE& )
    {
        std::cout << "entering: SetMark" << std::endl;
    }
};
struct PlayingExit_Entry : euml_action<PlayingExit_Entry>
{
    template <class Event,class FSM,class STATE>
    void operator()(Event const&,FSM&,STATE& )
    {
        std::cout << "entering: PlayingExit" << std::endl;
    }
};
struct WaitingForSongChoice_Entry : euml_action<WaitingForSongChoice_Entry>
{
    template <class Event,class FSM,class STATE>
    void operator()(Event const&,FSM&,STATE& )
    {
        std::cout << "entering: WaitingForSongChoice" << std::endl;
    }
};
struct StartCurrentSong_Entry : euml_action<StartCurrentSong_Entry>
{
    template <class Event,class FSM,class STATE>
    void operator()(Event const&,FSM&,STATE& )
    {
        std::cout << "entering: StartCurrentSong" << std::endl;
    }
};
struct MenuExit_Entry : euml_action<MenuExit_Entry>
{
    template <class Event,class FSM,class STATE>
    void operator()(Event const&,FSM&,STATE& )
    {
        std::cout << "entering: MenuExit" << std::endl;
    }
};
struct show_selected_song : euml_action<show_selected_song>
{
    template <class FSM,class EVT,class SourceState,class TargetState>
    void operator()(EVT const& ,FSM& fsm,SourceState& ,TargetState& )
    {
        std::cout << "selecting song:" << fsm.template get_attribute<m_SongIndex>() << std::endl;
    }
};
struct do_fast_forward : euml_action<do_fast_forward>
{
    template <class FSM,class EVT,class SourceState,class TargetState>
    void operator()(EVT const& ,FSM& fsm,SourceState& ,TargetState& )
    {
        std::cout << "moving song forward..." << std::endl;
    }
};
struct show_playing_song : euml_action<show_playing_song>
{
    template <class FSM,class EVT,class SourceState,class TargetState>
    void operator()(EVT const& ,FSM& fsm,SourceState& ,TargetState& )
    {
        std::cout << "playing song:" << fsm.template get_attribute<m_SongIndex>() << std::endl;
    }
};
struct show_player_off : euml_action<show_player_off>
{
    template <class FSM,class EVT,class SourceState,class TargetState>
    void operator()(EVT const& ,FSM& fsm,SourceState& ,TargetState& )
    {
        std::cout << "turning player off" << std::endl;
    }
};
struct show_player_on : euml_action<show_player_on>
{
    template <class FSM,class EVT,class SourceState,class TargetState>
    void operator()(EVT const& ,FSM& fsm,SourceState& ,TargetState& )
    {
        std::cout << "turning player on" << std::endl;
    }
};
#endif