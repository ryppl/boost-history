// Copyright 2008 Christophe Henry
// henry UNDERSCORE christophe AT hotmail DOT com
// This is an extended version of the state machine available in the boost::mpl library
// Distributed under the same license as the original.
// Copyright for the original version:
// Copyright 2005 David Abrahams and Aleksey Gurtovoy. Distributed
// under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MSM_FRONT_STATEMACHINE_DEF_H
#define BOOST_MSM_FRONT_STATEMACHINE_DEF_H

#include <boost/mpl/vector.hpp>

#include <boost/msm/row_tags.hpp>
#include <boost/msm/front/states.hpp>
#include <boost/msm/front/automatic_event.hpp>
#include <boost/msm/front/common_states.hpp>

namespace boost { namespace msm { namespace front
{

template<class Derived,class BaseState = default_base_state>
struct state_machine_def :  public state_base<BaseState>
{
	// tags
	// default: no flag
	typedef ::boost::mpl::vector0<>               flag_list;
	//default: no deferred events
	typedef ::boost::mpl::vector0<>               deferred_events;
    // customization (message queue, exceptions)
	typedef ::boost::mpl::vector0<>               configuration;

    typedef BaseState                             BaseAllStates;
	template<
		typename T1
		, class Event
		, typename T2
		, void (Derived::*action)(Event const&)
	>
	struct a_row
	{
        typedef a_row_tag row_type_tag;
		typedef T1 Source;
		typedef T2 Target;
		typedef Event Evt;
        template <class FSM,class SourceState,class TargetState>
        static void action_call(FSM& fsm,Event const& evt,SourceState&,TargetState&)
        {
            // in this front-end, we don't need to know source and target states
            (fsm.*action)(evt);
        }
	};

	template<
		typename T1
		, class Event
		, typename T2
	>
	struct _row
	{
        typedef _row_tag row_type_tag;
		typedef T1 Source;
		typedef T2 Target;
		typedef Event Evt;
	};

	template<
		typename T1
		, class Event
		, typename T2
		, void (Derived::*action)(Event const&)
		, bool (Derived::*guard)(Event const&) 
	>
	struct row
	{
        typedef row_tag row_type_tag;
		typedef T1 Source;
		typedef T2 Target;
		typedef Event Evt;
        template <class FSM,class SourceState,class TargetState>
        static void action_call(FSM& fsm,Event const& evt,SourceState&,TargetState&)
        {
            // in this front-end, we don't need to know source and target states
            (fsm.*action)(evt);
        }
        template <class FSM,class SourceState,class TargetState>
        static bool guard_call(FSM& fsm,Event const& evt,SourceState&,TargetState&)
        {
            // in this front-end, we don't need to know source and target states
            return (fsm.*guard)(evt);
        }
	};
	template<
		typename T1
		, class Event
		, typename T2
		, bool (Derived::*guard)(Event const&) 
	>
	struct g_row
	{
        typedef g_row_tag row_type_tag;
		typedef T1 Source;
		typedef T2 Target;
		typedef Event Evt;
        template <class FSM,class SourceState,class TargetState>
        static bool guard_call(FSM& fsm,Event const& evt,SourceState&,TargetState&)
        {
            // in this front-end, we don't need to know source and target states
            return (fsm.*guard)(evt);
        }
    };
protected:
    // Default no-transition handler. Can be replaced in the Derived SM class.
    template <class FSM,class Event>
    void no_transition(Event const& ,FSM&, int state)
    {
        BOOST_ASSERT(false);
    }
    // default exception handler. Can be replaced in the Derived SM class.
    template <class FSM,class Event>
    void exception_caught (Event const&,FSM&,std::exception& )
    {
        BOOST_ASSERT(false);
    }
};


} } }// boost::msm::front
#endif //BOOST_MSM_FRONT_STATEMACHINE_DEF_H
