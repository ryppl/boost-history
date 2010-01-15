// Copyright 2008 Christophe Henry
// henry UNDERSCORE christophe AT hotmail DOT com
// This is an extended version of the state machine available in the boost::mpl library
// Distributed under the same license as the original.
// Copyright for the original version:
// Copyright 2005 David Abrahams and Aleksey Gurtovoy. Distributed
// under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MSM_ROW2_HPP
#define BOOST_MSM_ROW2_HPP

#include <boost/type_traits/is_base_of.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/fusion/include/at_key.hpp>
#include <boost/msm/row_tags.hpp>

namespace boost { namespace msm { namespace front
{
    namespace detail
    {
        template<
            typename CalledForAction
            , typename Event
            , void (CalledForAction::*action)(Event const&)
        >
        struct row2_action_helper
        {
            template <class FSM,class Event,class SourceState,class TargetState, class AllStates>
            static void call_helper(FSM&,Event const& evt,SourceState&,TargetState&,
                                    AllStates& all_states,::boost::mpl::false_ const &)
            {
                // in this front-end, we don't need to know source and target states
                ( ::boost::fusion::at_key<CalledForAction>(all_states).*action)(evt);
            }
            template <class FSM,class Event,class SourceState,class TargetState, class AllStates>
            static void call_helper(FSM& fsm,Event const& evt,SourceState&,TargetState&,AllStates&,
                                    ::boost::mpl::true_ const &)
            {
                // in this front-end, we don't need to know source and target states
                (fsm.*action)(evt);
            }
        };

        template<
            typename CalledForGuard
            , typename Event
            , bool (CalledForGuard::*guard)(Event const&)
        >
        struct row2_guard_helper
        {
            template <class FSM,class Event,class SourceState,class TargetState,class AllStates>
            static bool call_helper(FSM&,Event const& evt,SourceState&,TargetState&,
                                    AllStates& all_states, ::boost::mpl::false_ const &)
            {
                // in this front-end, we don't need to know source and target states
                return ( ::boost::fusion::at_key<CalledForGuard>(all_states).*guard)(evt);
            }
            template <class FSM,class Event,class SourceState,class TargetState,class AllStates>
            static bool call_helper(FSM& fsm,Event const& evt,SourceState&,TargetState&,
                                    AllStates&,::boost::mpl::true_ const &)
            {
                // in this front-end, we don't need to know source and target states
                return (fsm.*guard)(evt);
            }
        };
    }

	template<
		typename T1
		, class Event
		, typename T2
        , typename CalledForAction
		, void (CalledForAction::*action)(Event const&)
	>
	struct a_row2
	{
        typedef a_row_tag row_type_tag;
		typedef T1 Source;
		typedef T2 Target;
		typedef Event Evt;
        template <class FSM,class SourceState,class TargetState,class AllStates>
        static void action_call(FSM& fsm,Event const& evt,SourceState& src,TargetState& tgt, 
                                AllStates& all_states)
        {
            // in this front-end, we don't need to know source and target states
            detail::row2_action_helper<CalledForAction,Event,action>::call_helper
                (fsm,evt,src,tgt,all_states,
                ::boost::mpl::bool_< ::boost::is_base_of<CalledForAction,FSM>::type::value>());
        }
	};

	template<
		typename T1
		, class Event
		, typename T2
        , typename CalledForAction
        , void (CalledForAction::*action)(Event const&)
        , typename CalledForGuard
        , bool (CalledForGuard::*guard)(Event const&)
	>
	struct row2
	{
        typedef row_tag row_type_tag;
		typedef T1 Source;
		typedef T2 Target;
		typedef Event Evt;
        template <class FSM,class SourceState,class TargetState, class AllStates>
        static void action_call(FSM& fsm,Event const& evt,SourceState& src,TargetState& tgt, 
                                AllStates& all_states)
        {
            // in this front-end, we don't need to know source and target states
            detail::row2_action_helper<CalledForAction,Event,action>::call_helper
                (fsm,evt,src,tgt,all_states,
                ::boost::mpl::bool_< ::boost::is_base_of<CalledForAction,FSM>::type::value>());
        }
        template <class FSM,class SourceState,class TargetState,class AllStates>
        static bool guard_call(FSM& fsm,Event const& evt,SourceState& src,TargetState& tgt,
                               AllStates& all_states)
        {
            // in this front-end, we don't need to know source and target states
            return detail::row2_guard_helper<CalledForGuard,Event,guard>::call_helper
                (fsm,evt,src,tgt,all_states,
                ::boost::mpl::bool_< ::boost::is_base_of<CalledForGuard,FSM>::type::value>());
        }
	};
	template<
		typename T1
		, class Event
		, typename T2
        , typename CalledForGuard
        , bool (CalledForGuard::*guard)(Event const&)
	>
	struct g_row2
	{
        typedef g_row_tag row_type_tag;
		typedef T1 Source;
		typedef T2 Target;
		typedef Event Evt;
        template <class FSM,class SourceState,class TargetState,class AllStates>
        static bool guard_call(FSM& fsm,Event const& evt,SourceState& src,TargetState& tgt,
                               AllStates& all_states)
        {
            // in this front-end, we don't need to know source and target states
            return detail::row2_guard_helper<CalledForGuard,Event,guard>::call_helper
                (fsm,evt,src,tgt,all_states,
                ::boost::mpl::bool_< ::boost::is_base_of<CalledForGuard,FSM>::type::value>());
        }
    };
    // internal transitions
	template<
		typename T1
		, class Event
        , typename CalledForAction
        , void (CalledForAction::*action)(Event const&)
	>
	struct a_irow2
	{
        typedef a_irow_tag row_type_tag;
		typedef T1 Source;
		typedef T1 Target;
		typedef Event Evt;
        template <class FSM,class SourceState,class TargetState,class AllStates>
        static void action_call(FSM& fsm,Event const& evt,SourceState& src,TargetState& tgt, 
                                AllStates& all_states)
        {
            // in this front-end, we don't need to know source and target states
            detail::row2_action_helper<CalledForAction,Event,action>::call_helper
                (fsm,evt,src,tgt,all_states,
                ::boost::mpl::bool_< ::boost::is_base_of<CalledForAction,FSM>::type::value>());
        }
	};

	template<
		typename T1
		, class Event
        , typename CalledForAction
        , void (CalledForAction::*action)(Event const&)
        , typename CalledForGuard
        , bool (CalledForGuard::*guard)(Event const&)
	>
	struct irow2
	{
        typedef irow_tag row_type_tag;
		typedef T1 Source;
		typedef T1 Target;
		typedef Event Evt;
        template <class FSM,class SourceState,class TargetState,class AllStates>
        static void action_call(FSM& fsm,Event const& evt,SourceState& src,TargetState& tgt, 
                                AllStates& all_states)
        {
            // in this front-end, we don't need to know source and target states
            detail::row2_action_helper<CalledForAction,Event,action>::call_helper
                (fsm,evt,src,tgt,all_states,
                ::boost::mpl::bool_< ::boost::is_base_of<CalledForAction,FSM>::type::value>());
        }
        template <class FSM,class SourceState,class TargetState,class AllStates>
        static bool guard_call(FSM& fsm,Event const& evt,SourceState& src,TargetState& tgt,
                               AllStates& all_states)
        {
            // in this front-end, we don't need to know source and target states
            return detail::row2_guard_helper<CalledForGuard,Event,guard>::call_helper
                (fsm,evt,src,tgt,all_states,
                ::boost::mpl::bool_< ::boost::is_base_of<CalledForGuard,FSM>::type::value>());
        }
	};
	template<
		typename T1
		, class Event
        , typename CalledForGuard
        , bool (CalledForGuard::*guard)(Event const&)
	>
	struct g_irow2
	{
        typedef g_irow_tag row_type_tag;
		typedef T1 Source;
		typedef T1 Target;
		typedef Event Evt;
        template <class FSM,class SourceState,class TargetState,class AllStates>
        static bool guard_call(FSM& fsm,Event const& evt,SourceState& src,TargetState& tgt,
                               AllStates& all_states)
        {
            // in this front-end, we don't need to know source and target states
            return detail::row2_guard_helper<CalledForGuard,Event,guard>::call_helper
                (fsm,evt,src,tgt,all_states,
                ::boost::mpl::bool_< ::boost::is_base_of<CalledForGuard,FSM>::type::value>());
        }
    };

}}}

#endif //BOOST_MSM_ROW2_HPP

