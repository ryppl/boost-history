// Copyright 2008 Christophe Henry
// henry UNDERSCORE christophe AT hotmail DOT com
// This is an extended version of the state machine available in the boost::mpl library
// Distributed under the same license as the original.
// Copyright for the original version:
// Copyright 2005 David Abrahams and Aleksey Gurtovoy. Distributed
// under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MSM_STATES_H
#define BOOST_MSM_STATES_H

#include <boost/mpl/bool.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/function.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/msm/common_types.hpp>


// traits used for entry/exit states
BOOST_MPL_HAS_XXX_TRAIT_DEF(no_automatic_create)
BOOST_MPL_HAS_XXX_TRAIT_DEF(direct_entry)
BOOST_MPL_HAS_XXX_TRAIT_DEF(explicit_entry_state)
BOOST_MPL_HAS_XXX_TRAIT_DEF(non_forwarding_flag)

namespace boost { namespace msm
{
// default base: non-polymorphic, not visitable
struct default_base_state 
{
    ~default_base_state(){}
};
// default polymorphic base state. Derive all states from it to get polymorphic behavior
struct polymorphic_state
{
    virtual ~polymorphic_state() {}
};

// the interface for all states. Defines entry and exit functions. Overwrite to implement for any state needing it.
template<class USERBASE>
struct state_base : USERBASE
{
    typedef USERBASE user_state_base;

    // empty implementation for the states not wishing to define an entry condition
    // will not be called polymorphic way
    template <class Event>
    void on_entry(Event const& ){}
    template <class Event>
    void on_exit(Event const& ){}
};
struct no_sm_ptr 
{
    // tags
    typedef ::boost::mpl::bool_<false>   needs_sm;
};
struct sm_ptr 
{
    // tags
    typedef ::boost::mpl::bool_<true>    needs_sm;
};
// kept for backward compatibility
struct NoSMPtr 
{
    // tags
    typedef ::boost::mpl::bool_<false>   needs_sm;
};
struct SMPtr 
{
    // tags
    typedef ::boost::mpl::bool_<true>    needs_sm;
};

// provides the typedefs and interface. Concrete states derive from it.
// template argument: pointer-to-fsm policy
template<class BASE = default_base_state,class SMPtrPolicy = no_sm_ptr>
struct state :  public state_base<BASE>, SMPtrPolicy
{
    // tags
    // default: no flag
    typedef ::boost::mpl::vector<>       flag_list;
    //default: no deferred events
    typedef ::boost::mpl::vector<>       deferred_events;
};

// flags used internally to handle terminate / interrupt states
struct TerminateFlag 
{
    typedef int non_forwarding_flag;
    typedef int event_blocking_flag;
};
struct InterruptedFlag
{
    typedef int non_forwarding_flag;
    typedef int event_blocking_flag;
};
template <class EndEvent>
struct EndInterruptFlag
{
    typedef int non_forwarding_flag;
};

// terminate state simply defines the TerminateFlag flag
// template argument: pointer-to-fsm policy
template<class BASE = default_base_state,class SMPtrPolicy = no_sm_ptr>
struct terminate_state : public state_base<BASE>, SMPtrPolicy
{
    // tags
    typedef ::boost::mpl::vector<boost::msm::TerminateFlag>      flag_list;
    //default: no deferred events
    typedef ::boost::mpl::vector<>                               deferred_events;
};

// terminate state simply defines the InterruptedFlag and EndInterruptFlag<EndInterruptEvent> flags
// template argument: event which ends the interrupt
// template argument: pointer-to-fsm policy
template <class EndInterruptEvent,class BASE = default_base_state,class SMPtrPolicy = no_sm_ptr>
struct interrupt_state : public state_base<BASE>, SMPtrPolicy
{
    // tags
    typedef ::boost::mpl::vector<boost::msm::InterruptedFlag,
        boost::msm::EndInterruptFlag<EndInterruptEvent> >       
        flag_list;
    //default: no deferred events
    typedef ::boost::mpl::vector<>                           deferred_events;
};

// not a state but a bunch of extra typedefs to handle direct entry into a composite state. To be derived from
// template argument: containing composite
// template argument: zone index of this state
template <class Composite,int ZoneIndex=-1>
struct explicit_entry 
{
    typedef int no_automatic_create;
    typedef int explicit_entry_state;
    typedef Composite owner;
    enum {zone_index=ZoneIndex};
    template <class ToTest>
    // metafunction helping determine who needs to create this state
    struct is_owning_composite 
    {
        typedef typename ::boost::is_same<Composite,ToTest>::type type;
    };
};

// to be derived from. Makes a type an entry (pseudo) state. Actually an almost full-fledged state
// template argument: containing composite
// template argument: zone index of this state
// template argument: pointer-to-fsm policy
template<class Composite,int ZoneIndex=-1,class BASE = default_base_state,class SMPtrPolicy = no_sm_ptr>
struct entry_pseudo_state
    :  public state_base<BASE>, explicit_entry<Composite,ZoneIndex> ,SMPtrPolicy
{
    // tags
    typedef int                          pseudo_entry;
    // default: no flag
    typedef ::boost::mpl::vector<>       flag_list;
    //default: no deferred events
    typedef ::boost::mpl::vector<>       deferred_events;
};

// to be derived from. Makes a state an exit (pseudo) state. Actually an almost full-fledged state
// template argument: containing composite
// template argument: event to forward
// template argument: pointer-to-fsm policy
template<class Composite,class Event,class BASE = default_base_state,class SMPtrPolicy = no_sm_ptr>
struct exit_pseudo_state : public state_base<BASE> , SMPtrPolicy
{
    // tags
    typedef int                 pseudo_exit;
    typedef int                 no_automatic_create;
    typedef Composite           owner;
    typedef Event               event;

    // metafunction helping determine who needs to create this state
    template <class ToTest>
    struct is_owning_composite 
    {
        typedef typename boost::is_same<Composite,ToTest>::type type;
    };
    // default: no flag
    typedef ::boost::mpl::vector< > flag_list;
    //default: no deferred events
    typedef ::boost::mpl::vector<>  deferred_events;

    // forward event to the higher-level FSM
    template <class ForwardEvent>
    void forward_event(ForwardEvent const& incomingEvent)
    {
        // use helper to forward or not
        helper(incomingEvent);
    }
    void set_forward_fct(::boost::function<execute_return (Event const&)> fct)
    {
        m_forward = fct;
    }    
    exit_pseudo_state():m_forward(){}
    // by assignments, we keep our forwarding functor unchanged as our containing SM did not change
    exit_pseudo_state(exit_pseudo_state<Composite,Event,BASE,SMPtrPolicy>& rhs){}
    exit_pseudo_state<Composite,Event,BASE,SMPtrPolicy>& operator= (const exit_pseudo_state<Composite,Event,BASE,SMPtrPolicy>& ) 
    { 
        return *this; 
    } 
private:
    ::boost::function<execute_return (Event const&)>           m_forward;

    // helper used to forward an event if it is the one we are supposed to
    template <class ForwardEvent>
    typename ::boost::enable_if<typename ::boost::is_convertible<ForwardEvent,Event>::type,void >::type
        helper(ForwardEvent const& incomingEvent,boost::msm::dummy<0> = 0)
    {
        // call if handler set, if not, this state is simply a terminate state
        if (m_forward)
            m_forward(incomingEvent);
    }
    template <class ForwardEvent>
    typename ::boost::disable_if<typename ::boost::is_convertible<ForwardEvent,Event>::type,void >::type
        helper(ForwardEvent const& incomingEvent,boost::msm::dummy<1> = 0)
    {
        // Not our event, ignore
    }
};

// event used internally for wrapping a direct entry
template <class StateType,class Event>
struct direct_entry_event 
{
    typedef int direct_entry;
    typedef StateType active_state;

    direct_entry_event(Event const& evt):m_event(evt){}
    Event const& m_event;
};

}}

#endif //BOOST_MSM_STATES_H

