// Copyright 2008 Christophe Henry
// henry UNDERSCORE christophe AT hotmail DOT com
// This is an extended version of the state machine available in the boost::mpl library
// Distributed under the same license as the original.
// Copyright for the original version:
// Copyright 2005 David Abrahams and Aleksey Gurtovoy. Distributed
// under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MSM_FRONT_EUML_COMMON_H
#define BOOST_MSM_FRONT_EUML_COMMON_H

#ifdef BOOST_MPL_LIMIT_METAFUNCTION_ARITY
#undef BOOST_MPL_LIMIT_METAFUNCTION_ARITY
#endif

#ifdef BOOST_PROTO_MAX_ARITY
#undef BOOST_PROTO_MAX_ARITY
#endif

#ifndef BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#endif

#ifdef BOOST_MSVC 
#define BOOST_MPL_LIMIT_METAFUNCTION_ARITY 7
#define BOOST_PROTO_MAX_ARITY 7
#else
#define BOOST_MPL_LIMIT_METAFUNCTION_ARITY 6
#define BOOST_PROTO_MAX_ARITY 6
#endif

#include <iterator>
#include <utility>

#include <boost/proto/core.hpp>
#include <boost/proto/transform.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/add_const.hpp>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/set.hpp>
#include <boost/mpl/has_key.hpp>
#include <boost/mpl/int.hpp>

#if BOOST_VERSION >= 104000
#include <boost/mpl/string.hpp>
#endif

#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/include/at_c.hpp>

#include <boost/msm/front/functor_row.hpp>

namespace proto = boost::proto;

BOOST_MPL_HAS_XXX_TRAIT_DEF(tag_type)

namespace boost { namespace msm { namespace front { namespace euml
{
template <class T>
struct get_iterator 
{
    typedef typename T::iterator type;
};
template <class T>
struct get_reverse_iterator 
{
    typedef typename T::reverse_iterator type;
};
template <class T>
struct get_reference 
{
    typedef typename T::reference type;
};
template <class T>
struct get_size_type 
{
    typedef typename T::size_type type;
};
template <class T>
struct get_value_type 
{
    typedef typename T::value_type type;
};
template <class T>
struct get_first_type 
{
    typedef typename T::first_type type;
};
template <class T>
struct get_second_type 
{
    typedef typename T::second_type type;
};
template <class T>
struct get_action_tag_type 
{
    typedef typename ::boost::mpl::has_key<
        typename T::tag_type,action_tag>::type type;
};
template <class T>
struct get_state_action_tag_type 
{
    typedef typename ::boost::mpl::has_key<
        typename T::tag_type,state_action_tag>::type type;
};

template <class T,class EVT,class FSM,class SourceState,class TargetState>
struct get_result_type 
{
	typedef typename T::template transition_action_result<EVT,FSM,SourceState,TargetState>::type type;
};
template <class T,class Event,class FSM,class STATE>
struct get_result_type2 
{
	typedef typename T::template state_action_result<Event,FSM,STATE>::type type;
};
template<class SEQ>
struct get_sequence
{
    typedef typename SEQ::sequence type;
};

template <class T>
struct get_attributes_type
{
    typedef typename T::attributes_type type;
};

template <class T>
struct get_euml_tag_type
{
    typedef typename T::euml_tag_type type;
};

template <class T,class Arg1=void,class Arg2=void,class Arg3=void,class Arg4=void,class Arg5=void
#ifdef BOOST_MSVC
,class Arg6=void
#endif
>
struct get_fct 
{
    typedef typename T::template In<Arg1,Arg2,Arg3,Arg4,Arg5
#ifdef BOOST_MSVC
,Arg6
#endif
>::type type;
};


// grammar forbidding address of for terminals
struct terminal_grammar : proto::not_<proto::address_of<proto::_> >
{};

// Forward-declare an expression wrapper
template<typename Expr>
struct euml_terminal;

struct sm_domain
    : proto::domain< proto::generator<euml_terminal>, terminal_grammar >
{};

template<typename Expr>
struct euml_terminal
    : proto::extends<Expr, euml_terminal<Expr>, sm_domain>
{
    typedef
        proto::extends<Expr, euml_terminal<Expr>, sm_domain>
        base_type;
    // Needs a constructor
    euml_terminal(Expr const &e = Expr())
        : base_type(e)
    {}
    // Unhide Proto's overloaded assignment operator
    using base_type::operator=;
};

template <class EVT>
struct euml_event: proto::extends<typename proto::terminal<event_tag>::type, EVT, sm_domain>
{
    typedef event_tag euml_tag_type;
    using proto::extends<typename proto::terminal<event_tag>::type, EVT, sm_domain>::operator=;
    template <class Arg1,class Arg2,class Arg3,class Arg4,class Arg5 
#ifdef BOOST_MSVC 
 ,class Arg6 
#endif
>
    struct In
    {
        typedef EVT type;
    };
};
template <class STATE>
struct euml_state: proto::extends<typename proto::terminal<state_tag>::type, STATE, sm_domain>
{
    typedef state_tag euml_tag_type;
    using proto::extends<typename proto::terminal<state_tag>::type, STATE, sm_domain>::operator=;
    template <class Arg1,class Arg2,class Arg3,class Arg4,class Arg5 
#ifdef BOOST_MSVC 
 ,class Arg6 
#endif
>
    struct In
    {
        typedef STATE type;
    };
};
template <class ACTION>
struct euml_action: proto::extends<typename proto::terminal<action_tag>::type, ACTION, sm_domain>
{
    typedef action_tag euml_tag_type;
    using proto::extends<typename proto::terminal<action_tag>::type, ACTION, sm_domain>::operator=;
    template <class Arg1,class Arg2,class Arg3,class Arg4,class Arg5 
#ifdef BOOST_MSVC 
 ,class Arg6 
#endif
>
    struct In
    {
        typedef ACTION type;
    };
};
template <class FLAG>
struct euml_flag: proto::extends<typename proto::terminal<flag_tag>::type, FLAG, sm_domain>
{
    typedef flag_tag euml_tag_type;
    using proto::extends<typename proto::terminal<flag_tag>::type, FLAG, sm_domain>::operator=;
    template <class Arg1,class Arg2,class Arg3,class Arg4,class Arg5 
#ifdef BOOST_MSVC 
 ,class Arg6 
#endif
>
    struct In
    {
        typedef FLAG type;
    };
};

template <class CONFIG>
struct euml_config: proto::extends<typename proto::terminal<config_tag>::type, CONFIG, sm_domain>
{
    typedef config_tag euml_tag_type;
    using proto::extends<typename proto::terminal<config_tag>::type, CONFIG, sm_domain>::operator=;
    template <class Arg1,class Arg2,class Arg3,class Arg4,class Arg5 
#ifdef BOOST_MSVC 
 ,class Arg6 
#endif
>
    struct In
    {
        typedef CONFIG type;
    };
};

struct no_exception : euml_config<no_exception>
{
    typedef int no_exception_thrown;
};
struct no_msg_queue : euml_config<no_msg_queue>
{
    typedef int no_message_queue;
};

struct invalid_type{};
struct make_invalid_type
{
    typedef invalid_type type;
};

template <class ROW>
struct make_vector_one_row 
{
	typedef boost::mpl::vector<ROW> type;
};
template <class T>
T make_T(T t) {return t;}

struct make_vector_no_row 
{
    typedef boost::mpl::vector0<> type;
};

struct NoAction : euml_action<NoAction>
{
    // return value if used inside a state action (entry/exit)
    template <class Event,class FSM,class STATE >
    struct state_action_result
    {
        typedef bool type;
    };
    // return value if used inside a transition (action/guard)
    template <class EVT,class FSM,class SourceState,class TargetState>
    struct transition_action_result
    {
        typedef bool type;
    };
    // this functor can be used in both modes, state action and transition action
    typedef ::boost::mpl::set<state_action_tag,action_tag> tag_type;

    template <class Event,class FSM,class STATE>
    bool operator()(Event const&,FSM&,STATE& )
    {
        // does nothing
        return true;
    }
    template <class EVT,class FSM,class SourceState,class TargetState>
    bool operator()(EVT const& evt ,FSM& ,SourceState& ,TargetState&)const
    {
        // does nothing
        return true;
    }
};

struct GetSource_ : euml_action<GetSource_ >
{
    using euml_action<GetSource_ >::operator=;
	template <class EVT,class FSM,class SourceState,class TargetState>
	struct transition_action_result 
	{
		typedef SourceState& type;
	};
	typedef ::boost::mpl::set<action_tag> tag_type;

	template <class EVT,class FSM,class SourceState,class TargetState>
	typename transition_action_result<EVT,FSM,SourceState,TargetState>::type 
	 operator()(EVT const& , FSM&,SourceState& src,TargetState& )const
	{
        return src;
	}
};
struct get_source_tag {};
struct GetSource_Helper: proto::extends< proto::terminal<get_source_tag>::type, GetSource_Helper, sm_domain>
{
    template <class Arg1,class Arg2,class Arg3,class Arg4,class Arg5 
#ifdef BOOST_MSVC 
 ,class Arg6 
#endif
>
    struct In
    {
        typedef GetSource_ type;
    };
};
GetSource_Helper const source_;

struct GetTarget_ : euml_action<GetTarget_ >
{
    using euml_action<GetTarget_ >::operator=;
	template <class EVT,class FSM,class SourceState,class TargetState>
	struct transition_action_result 
	{
		typedef TargetState& type;
	};
	typedef ::boost::mpl::set<action_tag> tag_type;

	template <class EVT,class FSM,class SourceState,class TargetState>
	typename transition_action_result<EVT,FSM,SourceState,TargetState>::type 
	 operator()(EVT const& , FSM&,SourceState& ,TargetState& tgt)const
	{
        return tgt;
	}
};
struct get_target_tag {};
struct GetTarget_Helper: proto::extends< proto::terminal<get_target_tag>::type, GetTarget_Helper, sm_domain>
{
    template <class Arg1,class Arg2,class Arg3,class Arg4,class Arg5 
#ifdef BOOST_MSVC 
 ,class Arg6 
#endif
>
    struct In
    {
        typedef GetTarget_ type;
    };
};
GetTarget_Helper const target_;

struct GetState_ : euml_action<GetState_ >
{
    using euml_action<GetState_ >::operator=;

	template <class Event,class FSM,class STATE >
	struct state_action_result 
	{
		typedef STATE& type;
	};
	typedef ::boost::mpl::set<state_action_tag> tag_type;

	template <class Event,class FSM,class STATE>
    typename state_action_result<Event,FSM,STATE>::type
     operator()(Event const&,FSM& ,STATE& state )
	{
        return state;
	}
};
struct get_state_tag {};
struct GetState_Helper: proto::extends< proto::terminal<get_state_tag>::type, GetState_Helper, sm_domain>
{
    template <class Arg1,class Arg2,class Arg3,class Arg4,class Arg5 
#ifdef BOOST_MSVC 
 ,class Arg6 
#endif
>
    struct In
    {
        typedef GetState_ type;
    };
};
GetState_Helper const state_;

struct GetEvent_ : euml_action<GetEvent_ >
{
    using euml_action<GetEvent_ >::operator=;

	template <class Event,class FSM,class STATE >
	struct state_action_result 
	{
		typedef Event const& type;
	};
	template <class EVT,class FSM,class SourceState,class TargetState>
	struct transition_action_result 
	{
		typedef EVT const& type;
	};
    typedef ::boost::mpl::set<state_action_tag,action_tag> tag_type;

	template <class Event,class FSM,class STATE>
	typename state_action_result<Event,FSM,STATE>::type
		operator()(Event const& evt,FSM& ,STATE& )
	{
		return evt;
	}
	template <class EVT,class FSM,class SourceState,class TargetState>
	typename transition_action_result<EVT,FSM,SourceState,TargetState>::type 
		operator()(EVT const& evt ,FSM& ,SourceState& ,TargetState&)const
	{
		return evt;
	}
};
struct get_event_tag {};
struct GetEvent_Helper: proto::extends< proto::terminal<get_event_tag>::type, GetEvent_Helper, sm_domain>
{
    template <class Arg1,class Arg2,class Arg3,class Arg4,class Arg5 
#ifdef BOOST_MSVC 
 ,class Arg6 
#endif
>
    struct In
    {
        typedef GetEvent_ type;
    };
};
GetEvent_Helper const event_;

struct GetFsm_ : euml_action<GetFsm_>
{
    using euml_action<GetFsm_>::operator=;

    template <class Event,class FSM,class STATE >
    struct state_action_result 
    {
        typedef FSM& type;
    };
    template <class EVT,class FSM,class SourceState,class TargetState>
    struct transition_action_result 
    {
        typedef FSM& type;
    };
    typedef ::boost::mpl::set<state_action_tag,action_tag> tag_type;

    template <class Event,class FSM,class STATE>
    typename state_action_result<Event,FSM,STATE>::type
        operator()(Event const&,FSM& fsm,STATE& )
    {
        return fsm;
    }
    template <class EVT,class FSM,class SourceState,class TargetState>
    typename transition_action_result<EVT,FSM,SourceState,TargetState>::type 
        operator()(EVT const& evt ,FSM& fsm,SourceState& ,TargetState&)const
    {
        return fsm;
    }
};
struct get_fsm_tag {};
struct GetFsm_Helper: proto::extends< proto::terminal<get_fsm_tag>::type, GetFsm_Helper, sm_domain>
{
    template <class Arg1,class Arg2,class Arg3,class Arg4,class Arg5 
#ifdef BOOST_MSVC 
 ,class Arg6 
#endif
>
    struct In
    {
        typedef GetFsm_ type;
    };
};
GetFsm_Helper const fsm_;

template <class StateName,class Param1=void, class Enable=void >                                             
struct SubState_ : euml_action<SubState_<StateName,Param1,Enable> > {};        

template <class StateName,class Param1>
struct SubState_ <StateName,Param1
    , typename ::boost::enable_if<typename ::boost::is_same<Param1,void>::type >::type>
    : euml_action<SubState_<StateName, Param1 > >
{
    template <class Event,class FSM,class STATE >
    struct state_action_result 
    {
        typedef StateName& type;
    };
    template <class EVT,class FSM,class SourceState,class TargetState>
    struct transition_action_result 
    {
        typedef StateName& type;
    };

    typedef ::boost::mpl::set<state_action_tag,action_tag> tag_type;

    template <class EVT,class FSM,class SourceState,class TargetState>
    StateName& operator()(EVT const&,FSM& fsm,SourceState& ,TargetState& )const
    {
        return fsm.template get_state<StateName&>();
    }
    template <class Event,class FSM,class STATE>
    StateName& operator()(Event const& ,FSM& fsm,STATE&  )const
    {
        return fsm.template get_state<StateName&>();
    }
};

template <class StateName,class Param1>
struct SubState_ <StateName,Param1
    , typename ::boost::disable_if<
            typename ::boost::is_same<Param1,void>::type
            >::type>
    : euml_action<SubState_<StateName, Param1> >
{
    template <class Event,class FSM,class STATE >
    struct state_action_result 
    {
        typedef StateName& type;
    };
    template <class EVT,class FSM,class SourceState,class TargetState>
    struct transition_action_result 
    {
        typedef StateName& type;
    };

    typedef ::boost::mpl::set<state_action_tag,action_tag> tag_type;

    template <class EVT,class FSM,class SourceState,class TargetState>
    StateName& operator()(EVT const& evt,FSM& fsm,SourceState& src,TargetState& tgt)const
    {
        return (Param1()(evt,fsm,src,tgt)).template get_state<StateName&>();
    }
    template <class Event,class FSM,class STATE>
    StateName& operator()(Event const& evt,FSM& fsm,STATE&  state)const
    {
        return (Param1()(evt,fsm,state)).template get_state<StateName&>();
    }
};
struct substate_tag {};
struct SubState_Helper: proto::extends< proto::terminal<substate_tag>::type, SubState_Helper, sm_domain>
{
    using proto::extends< proto::terminal<substate_tag>::type, SubState_Helper, sm_domain>::operator=;
    template <class Arg1,class Arg2,class Arg3,class Arg4,class Arg5 
#ifdef BOOST_MSVC 
 ,class Arg6 
#endif
>
    struct In
    {
        typedef SubState_<Arg1,Arg2> type;
    };
};
SubState_Helper const substate_;

template <class Target,class Index>
struct GetAttribute_  : euml_action<GetAttribute_<Target, Index> >
{
    using euml_action<GetAttribute_<Target,Index> >::operator=;
    template <class Event,class FSM,class STATE >
    struct state_action_result 
    {
		typedef typename 
            ::boost::fusion::result_of::at<
            typename get_attributes_type<
            typename ::boost::remove_reference<
                        typename get_result_type2<Target,Event,FSM,STATE>::type>::type>::type,
                typename Index::value_type >::type type;
    };
    template <class EVT,class FSM,class SourceState,class TargetState>
    struct transition_action_result 
    {
		typedef typename 
            ::boost::fusion::result_of::at<
            typename get_attributes_type<
                    typename ::boost::remove_reference<
                        typename get_result_type<Target,EVT,FSM,SourceState,TargetState>::type>::type>::type,
                typename Index::value_type >::type type;
    };

    typedef ::boost::mpl::set<state_action_tag,action_tag> tag_type;

    template <class EVT,class FSM,class SourceState,class TargetState>
	typename ::boost::enable_if<
		typename ::boost::mpl::has_key<
			typename Target::tag_type,action_tag>::type,
			typename transition_action_result<EVT,FSM,SourceState,TargetState>::type >::type 
     operator()(EVT const& evt, FSM& fsm,SourceState& src,TargetState& tgt)const
    {
        return (Target()(evt,fsm,src,tgt)).template get_attribute<Index::value_type::value>();
    }
    template <class Event,class FSM,class STATE>
	typename ::boost::enable_if<
		typename ::boost::mpl::has_key<
			typename Target::tag_type,state_action_tag>::type,
			typename state_action_result<Event,FSM,STATE>::type >::type 
     operator()(Event const& evt,FSM& fsm,STATE& state )const
    {
        return (Target()(evt,fsm,state)).template get_attribute<Index::value_type::value>();
    }
};

struct get_attribute_tag 
{
};
struct GetAttribute_Helper: proto::extends< proto::terminal<get_attribute_tag>::type, GetAttribute_Helper, sm_domain>
{
    template <class Arg1,class Arg2,class Arg3,class Arg4,class Arg5 
#ifdef BOOST_MSVC 
 ,class Arg6 
#endif
>
    struct In
    {
        typedef GetAttribute_<Arg1,Arg2> type;
    };
};
GetAttribute_Helper const attribute_;

template <int Index>
struct Source_ : euml_action<Source_<Index> >
{
    using euml_action<Source_<Index> >::operator=;
	template <class EVT,class FSM,class SourceState,class TargetState>
	struct transition_action_result 
	{
		typedef typename 
			::boost::fusion::result_of::at<typename SourceState::attributes_type,
			::boost::mpl::int_<Index> >::type type;
	};
	typedef ::boost::mpl::set<action_tag> tag_type;

	template <class EVT,class FSM,class SourceState,class TargetState>
	typename transition_action_result<EVT,FSM,SourceState,TargetState>::type 
		operator()(EVT const& , FSM&,SourceState& src,TargetState& )const
	{
        return src.template get_attribute<Index>();
	}
};
template <int Index>
struct Target_ : euml_action<Target_<Index> >
{
    using euml_action<Target_<Index> >::operator=;
	template <class EVT,class FSM,class SourceState,class TargetState>
	struct transition_action_result 
	{
		typedef typename 
			::boost::fusion::result_of::at<typename TargetState::attributes_type,
			::boost::mpl::int_<Index> >::type type;
	};
	typedef ::boost::mpl::set<action_tag> tag_type;

	template <class EVT,class FSM,class SourceState,class TargetState>
	typename transition_action_result<EVT,FSM,SourceState,TargetState>::type 
		operator()(EVT const& ,FSM& ,SourceState& ,TargetState& tgt)const
	{
        return tgt.template get_attribute<Index>();
	}
};
template <int Index>
struct State_ : euml_action<State_<Index> >
{
    using euml_action<State_<Index> >::operator=;

	template <class Event,class FSM,class STATE >
	struct state_action_result 
	{
		typedef typename 
			::boost::fusion::result_of::at<typename STATE::attributes_type,
			::boost::mpl::int_<Index> >::type type;
	};
	typedef ::boost::mpl::set<state_action_tag> tag_type;

	template <class Event,class FSM,class STATE>
	typename state_action_result<Event,FSM,STATE>::type
		operator()(Event const&,FSM& ,STATE& state )
	{
        return state.template get_attribute<Index>();
	}
};
template <int Index>
struct Event_ : euml_action<Event_<Index> >
{
    using euml_action<Event_<Index> >::operator=;

	template <class Event,class FSM,class STATE >
	struct state_action_result 
	{
		typedef typename ::boost::add_const<
			typename ::boost::fusion::result_of::at<typename Event::attributes_type,
                                                    ::boost::mpl::int_<Index> >::type>::type type;
	};
	template <class EVT,class FSM,class SourceState,class TargetState>
	struct transition_action_result 
	{
		typedef typename ::boost::add_const<
            typename ::boost::fusion::result_of::at<typename EVT::attributes_type,
                                                    ::boost::mpl::int_<Index> >::type>::type type;
	};
    typedef ::boost::mpl::set<state_action_tag,action_tag> tag_type;

	template <class Event,class FSM,class STATE>
	typename state_action_result<Event,FSM,STATE>::type
		operator()(Event const& evt,FSM& ,STATE& )
	{
		return evt.template get_attribute<Index>();
	}
	template <class EVT,class FSM,class SourceState,class TargetState>
	typename transition_action_result<EVT,FSM,SourceState,TargetState>::type 
		operator()(EVT const& evt ,FSM& ,SourceState& ,TargetState&)const
	{
		return evt.template get_attribute<Index>();
	}
};
template <class StateType,int Index>
struct State_Attribute_ : euml_action<State_Attribute_<StateType,Index> >
{
    using euml_action<State_Attribute_<StateType,Index> >::operator=;

	template <class Event,class FSM,class STATE >
	struct state_action_result 
	{
		typedef typename 
			::boost::fusion::result_of::at<typename StateType::attributes_type,
			::boost::mpl::int_<Index> >::type type;
	};
    typedef ::boost::mpl::set<state_action_tag> tag_type;

	template <class Event,class FSM,class STATE>
	typename state_action_result<Event,FSM,STATE>::type
		operator()(Event const&,FSM& fsm,STATE& )
	{
        return fsm.template get_state<StateType&>().template get_attribute<Index>();
	}
};

template <int Index>
struct Fsm_ : euml_action<Fsm_<Index> >
{
    using euml_action<Fsm_<Index> >::operator=;

    template <class Event,class FSM,class STATE >
    struct state_action_result 
    {
        typedef typename 
            ::boost::fusion::result_of::at<typename FSM::attributes_type,
            ::boost::mpl::int_<Index> >::type type;
    };
    template <class EVT,class FSM,class SourceState,class TargetState>
    struct transition_action_result 
    {
        typedef typename 
            ::boost::fusion::result_of::at<typename FSM::attributes_type,
            ::boost::mpl::int_<Index> >::type type;
    };
    typedef ::boost::mpl::set<state_action_tag,action_tag> tag_type;

    template <class Event,class FSM,class STATE>
    typename state_action_result<Event,FSM,STATE>::type
        operator()(Event const&,FSM& fsm,STATE& )
    {
        return fsm.template get_attribute<Index>();;
    }
    template <class EVT,class FSM,class SourceState,class TargetState>
    typename transition_action_result<EVT,FSM,SourceState,TargetState>::type 
        operator()(EVT const& evt ,FSM& fsm,SourceState& ,TargetState&)const
    {
        return fsm.template get_attribute<Index>();
    }
};

struct True_ : euml::euml_action<True_>
{
    using euml_action<True_>::operator=;

    template <class Event,class FSM,class STATE >
    struct state_action_result 
    {
        typedef bool type;
    };
    template <class EVT,class FSM,class SourceState,class TargetState>
    struct transition_action_result 
    {
        typedef bool type;
    };
    typedef ::boost::mpl::set<state_action_tag,action_tag> tag_type;

    template <class EVT,class FSM,class SourceState,class TargetState>
    bool operator()(EVT const& evt,FSM&,SourceState& ,TargetState& )
    {
        return true;
    }
    template <class Event,class FSM,class STATE>
    bool operator()(Event const&,FSM&,STATE& )
    {
        return true;
    }
};	
struct False_ : euml::euml_action<False_>
{
    using euml_action<False_>::operator=;

    template <class Event,class FSM,class STATE >
    struct state_action_result 
    {
        typedef bool type;
    };
    template <class EVT,class FSM,class SourceState,class TargetState>
    struct transition_action_result 
    {
        typedef bool type;
    };
    typedef ::boost::mpl::set<state_action_tag,action_tag> tag_type;

    template <class EVT,class FSM,class SourceState,class TargetState>
    bool operator()(EVT const& evt,FSM&,SourceState& ,TargetState& )
    {
        return false;
    }
    template <class Event,class FSM,class STATE>
    bool operator()(Event const&,FSM&,STATE& )
    {
        return false;
    }
};	
template <int Val>
struct Int_ : euml_action<Int_<Val> >
{
    using euml_action<Int_<Val> >::operator=;
    typedef ::boost::mpl::int_<Val> value_type;
    enum {value = Val};

    template <class Event,class FSM,class STATE >
    struct state_action_result 
    {
        typedef int type;
    };
    template <class EVT,class FSM,class SourceState,class TargetState>
    struct transition_action_result 
    {
        typedef int type;
    };
    typedef ::boost::mpl::set<state_action_tag,action_tag> tag_type;

    template <class EVT,class FSM,class SourceState,class TargetState>
    int operator()(EVT const& , FSM& ,SourceState& ,TargetState& )
    {
        return Val;
    }
    template <class Event,class FSM,class STATE>
    int operator()(Event const& ,FSM& ,STATE& )
    {
        return Val;
    }
};

template <char Val>
struct Char_ : euml_action<Char_<Val> >
{
    using euml_action<Char_<Val> >::operator=;

    template <class Event,class FSM,class STATE >
    struct state_action_result 
    {
        typedef char type;
    };
    template <class EVT,class FSM,class SourceState,class TargetState>
    struct transition_action_result 
    {
        typedef char type;
    };
    typedef ::boost::mpl::set<state_action_tag,action_tag> tag_type;

    template <class EVT,class FSM,class SourceState,class TargetState>
    char operator()(EVT const& , FSM& ,SourceState& ,TargetState& )
    {
        return Val;
    }
    template <class Event,class FSM,class STATE>
    char operator()(Event const& ,FSM& ,STATE& )
    {
        return Val;
    }
};

template <size_t Val>
struct Size_t_ : euml_action<Size_t_<Val> >
{
    using euml_action<Size_t_<Val> >::operator=;

    template <class Event,class FSM,class STATE >
    struct state_action_result 
    {
        typedef size_t type;
    };
    template <class EVT,class FSM,class SourceState,class TargetState>
    struct transition_action_result 
    {
        typedef size_t type;
    };
    typedef ::boost::mpl::set<state_action_tag,action_tag> tag_type;

    template <class EVT,class FSM,class SourceState,class TargetState>
    size_t operator()(EVT const& , FSM& ,SourceState& ,TargetState& )
    {
        return Val;
    }
    template <class Event,class FSM,class STATE>
    size_t operator()(Event const& ,FSM& ,STATE& )
    {
        return Val;
    }
};

#if BOOST_VERSION >= 104000

template <class T>
struct String_ : euml_action<String_<T> >
{
    using euml_action<String_<T> >::operator=;

    template <class Event,class FSM,class STATE >
    struct state_action_result 
    {
        typedef char const* type;
    };
    template <class EVT,class FSM,class SourceState,class TargetState>
    struct transition_action_result 
    {
        typedef char const* type;
    };
    typedef ::boost::mpl::set<state_action_tag,action_tag> tag_type;

    template <class EVT,class FSM,class SourceState,class TargetState>
    char const* operator()(EVT const& , FSM& ,SourceState& ,TargetState& )
    {
        return ::boost::mpl::c_str<T>::value;
    }
    template <class Event,class FSM,class STATE>
    char const* operator()(Event const& ,FSM& ,STATE& )
    {
        return ::boost::mpl::c_str<T>::value;
    }
};
#endif


template <class T>
struct Predicate_ : euml_action<Predicate_<T> >
{
    using euml_action<Predicate_<T> >::operator=;

    template <class Event,class FSM,class STATE >
    struct state_action_result 
    {
        typedef T type;
    };
    template <class EVT,class FSM,class SourceState,class TargetState>
    struct transition_action_result 
    {
        typedef T type;
    };
    typedef ::boost::mpl::set<state_action_tag,action_tag> tag_type;

    template <class EVT,class FSM,class SourceState,class TargetState>
    T operator()(EVT const& , FSM& ,SourceState& ,TargetState& )
    {
        return T();
    }
    template <class Event,class FSM,class STATE>
    T operator()(Event const& ,FSM& ,STATE& )
    {
        return T();
    }
};

template <class ToProcessEvt,class Param1=void, class Param2=void, class Param3=void, class Param4=void, class Enable=void >                                             
struct Process_ : euml_action<Process_<ToProcessEvt,Param1,Param2,Param3,Param4,Enable> > {};        

template <class ToProcessEvt,class Param1, class Param2, class Param3, class Param4>
struct Process_ <ToProcessEvt,Param1,Param2,Param3,Param4
    , typename ::boost::enable_if<typename ::boost::is_same<Param1,void>::type >::type>
    : euml_action<Process_<ToProcessEvt, Param1, Param2, Param3, Param4 > >
{
    template <class Event,class FSM,class STATE >
    struct state_action_result 
    {
        typedef void type;
    };
    template <class EVT,class FSM,class SourceState,class TargetState>
    struct transition_action_result 
    {
        typedef void type;
    };

    typedef ::boost::mpl::set<state_action_tag,action_tag> tag_type;

    template <class EVT,class FSM,class SourceState,class TargetState>
    void operator()(EVT const&,FSM& fsm,SourceState& ,TargetState& )const
    {
        fsm.process_event(ToProcessEvt());
    }
    template <class Event,class FSM,class STATE>
    void operator()(Event const& ,FSM& fsm,STATE&  )const
    {
        fsm.process_event(ToProcessEvt());
    }
};

template <class ToProcessEvt,class Param1, class Param2, class Param3, class Param4>
struct Process_ <ToProcessEvt,Param1,Param2,Param3,Param4
    , typename ::boost::disable_if<
        typename ::boost::mpl::or_<
            typename ::boost::is_same<Param1,void>::type,
            typename ::boost::mpl::not_<typename ::boost::is_same<Param2,void>::type>::type
            >::type
    >::type>
    : euml_action<Process_<ToProcessEvt, Param1, Param2, Param3, Param4> >
{
    template <class Event,class FSM,class STATE >
    struct state_action_result 
    {
        typedef void type;
    };
    template <class EVT,class FSM,class SourceState,class TargetState>
    struct transition_action_result 
    {
        typedef void type;
    };

    typedef ::boost::mpl::set<state_action_tag,action_tag> tag_type;

    template <class EVT,class FSM,class SourceState,class TargetState>
    void operator()(EVT const& evt,FSM& fsm,SourceState& src,TargetState& tgt)const
    {
        (Param1()(evt,fsm,src,tgt)).process_event(ToProcessEvt());
    }
    template <class Event,class FSM,class STATE>
    void operator()(Event const& evt,FSM& fsm,STATE&  state)const
    {
        (Param1()(evt,fsm,state)).process_event(ToProcessEvt());
    }
};

template <class ToProcessEvt,class Param1, class Param2, class Param3, class Param4>
struct Process_ <ToProcessEvt,Param1,Param2,Param3,Param4
    , typename ::boost::disable_if<
        typename ::boost::mpl::or_<
            typename ::boost::is_same<Param2,void>::type,
            typename ::boost::mpl::not_<typename ::boost::is_same<Param3,void>::type>::type
            >::type
    >::type>
    : euml_action<Process_<ToProcessEvt, Param1, Param2, Param3, Param4> >
{
    template <class Event,class FSM,class STATE >
    struct state_action_result 
    {
        typedef void type;
    };
    template <class EVT,class FSM,class SourceState,class TargetState>
    struct transition_action_result 
    {
        typedef void type;
    };

    typedef ::boost::mpl::set<state_action_tag,action_tag> tag_type;

    template <class EVT,class FSM,class SourceState,class TargetState>
    void operator()(EVT const& evt,FSM& fsm,SourceState& src,TargetState& tgt)const
    {
        (Param1()(evt,fsm,src,tgt)).process_event(ToProcessEvt());
        (Param2()(evt,fsm,src,tgt)).process_event(ToProcessEvt());
    }
    template <class Event,class FSM,class STATE>
    void operator()(Event const& evt,FSM& fsm,STATE&  state)const
    {
        (Param1()(evt,fsm,state)).process_event(ToProcessEvt());
        (Param2()(evt,fsm,state)).process_event(ToProcessEvt());
    }
};

template <class ToProcessEvt,class Param1, class Param2, class Param3, class Param4>
struct Process_ <ToProcessEvt,Param1,Param2,Param3,Param4
    , typename ::boost::disable_if<
        typename ::boost::mpl::or_<
            typename ::boost::is_same<Param3,void>::type,
            typename ::boost::mpl::not_<typename ::boost::is_same<Param4,void>::type>::type
            >::type
    >::type>
    : euml_action<Process_<ToProcessEvt, Param1, Param2, Param3, Param4> >
{
    template <class Event,class FSM,class STATE >
    struct state_action_result 
    {
        typedef void type;
    };
    template <class EVT,class FSM,class SourceState,class TargetState>
    struct transition_action_result 
    {
        typedef void type;
    };

    typedef ::boost::mpl::set<state_action_tag,action_tag> tag_type;

    template <class EVT,class FSM,class SourceState,class TargetState>
    void operator()(EVT const& evt,FSM& fsm,SourceState& src,TargetState& tgt)const
    {
        (Param1()(evt,fsm,src,tgt)).process_event(ToProcessEvt());
        (Param2()(evt,fsm,src,tgt)).process_event(ToProcessEvt());
        (Param3()(evt,fsm,src,tgt)).process_event(ToProcessEvt());
    }
    template <class Event,class FSM,class STATE>
    void operator()(Event const& evt,FSM& fsm,STATE&  state)const
    {
        (Param1()(evt,fsm,state)).process_event(ToProcessEvt());
        (Param2()(evt,fsm,state)).process_event(ToProcessEvt());
        (Param3()(evt,fsm,state)).process_event(ToProcessEvt());
    }
};

template <class ToProcessEvt,class Param1, class Param2, class Param3, class Param4>
struct Process_ <ToProcessEvt,Param1,Param2,Param3,Param4
    , typename ::boost::disable_if<
            typename ::boost::is_same<Param4,void>::type
            >::type>
    : euml_action<Process_<ToProcessEvt, Param1, Param2, Param3, Param4> >
{
    template <class Event,class FSM,class STATE >
    struct state_action_result 
    {
        typedef void type;
    };
    template <class EVT,class FSM,class SourceState,class TargetState>
    struct transition_action_result 
    {
        typedef void type;
    };

    typedef ::boost::mpl::set<state_action_tag,action_tag> tag_type;

    template <class EVT,class FSM,class SourceState,class TargetState>
    void operator()(EVT const& evt,FSM& fsm,SourceState& src,TargetState& tgt)const
    {
        (Param1()(evt,fsm,src,tgt)).process_event(ToProcessEvt());
        (Param2()(evt,fsm,src,tgt)).process_event(ToProcessEvt());
        (Param3()(evt,fsm,src,tgt)).process_event(ToProcessEvt());
        (Param4()(evt,fsm,src,tgt)).process_event(ToProcessEvt());
    }
    template <class Event,class FSM,class STATE>
    void operator()(Event const& evt,FSM& fsm,STATE&  state)const
    {
        (Param1()(evt,fsm,state)).process_event(ToProcessEvt());
        (Param2()(evt,fsm,state)).process_event(ToProcessEvt());
        (Param3()(evt,fsm,state)).process_event(ToProcessEvt());
        (Param4()(evt,fsm,state)).process_event(ToProcessEvt());
    }
};
struct process_tag {};
struct Process_Helper: proto::extends< proto::terminal<process_tag>::type, Process_Helper, sm_domain>
{
    using proto::extends< proto::terminal<process_tag>::type, Process_Helper, sm_domain>::operator=;
    template <class Arg1,class Arg2,class Arg3,class Arg4,class Arg5 
#ifdef BOOST_MSVC 
 ,class Arg6 
#endif
>
    struct In
    {
        typedef Process_<Arg1,Arg2,Arg3,Arg4,Arg5> type;
    };
};
Process_Helper const process_;

template <class ToProcessEvt,class Value,class Param1=void, class Param2=void, class Param3=void, class Enable=void >                                             
struct Process2_ : euml_action<Process2_<ToProcessEvt,Value,Param1,Param2,Param3,Enable> > {};        

template <class ToProcessEvt,class Value,class Param1, class Param2, class Param3>
struct Process2_ <ToProcessEvt,Value,Param1,Param2,Param3
    , typename ::boost::enable_if<typename ::boost::is_same<Param1,void>::type >::type>
    : euml_action<Process2_<ToProcessEvt,Value, Param1, Param2, Param3 > >
{
    template <class Event,class FSM,class STATE >
    struct state_action_result 
    {
        typedef void type;
    };
    template <class EVT,class FSM,class SourceState,class TargetState>
    struct transition_action_result 
    {
        typedef void type;
    };

    typedef ::boost::mpl::set<state_action_tag,action_tag> tag_type;

    template <class EVT,class FSM,class SourceState,class TargetState>
    void operator()(EVT const& evt,FSM& fsm,SourceState& src,TargetState& tgt)const
    {
        fsm.process_event(ToProcessEvt(Value()(evt,fsm,src,tgt)));
    }
    template <class Event,class FSM,class STATE>
    void operator()(Event const& evt,FSM& fsm,STATE&  state)const
    {
        fsm.process_event(ToProcessEvt(Value()(evt,fsm,state)));
    }
};

template <class ToProcessEvt,class Value,class Param1, class Param2, class Param3>
struct Process2_ <ToProcessEvt,Value,Param1,Param2,Param3
    , typename ::boost::disable_if<
        typename ::boost::mpl::or_<
            typename ::boost::is_same<Param1,void>::type,
            typename ::boost::mpl::not_<typename ::boost::is_same<Param2,void>::type>::type
            >::type
    >::type>
    : euml_action<Process2_<ToProcessEvt,Value, Param1, Param2, Param3> >
{
    template <class Event,class FSM,class STATE >
    struct state_action_result 
    {
        typedef void type;
    };
    template <class EVT,class FSM,class SourceState,class TargetState>
    struct transition_action_result 
    {
        typedef void type;
    };

    typedef ::boost::mpl::set<state_action_tag,action_tag> tag_type;

    template <class EVT,class FSM,class SourceState,class TargetState>
    void operator()(EVT const& evt,FSM& fsm,SourceState& src,TargetState& tgt)const
    {
        (Param1()(evt,fsm,src,tgt)).process_event(ToProcessEvt(Value()(evt,fsm,src,tgt)));
    }
    template <class Event,class FSM,class STATE>
    void operator()(Event const& evt,FSM& fsm,STATE&  state)const
    {
        (Param1()(evt,fsm,state)).process_event(ToProcessEvt(Value()(evt,fsm,state)));
    }
};

template <class ToProcessEvt,class Value,class Param1, class Param2, class Param3>
struct Process2_ <ToProcessEvt,Value,Param1,Param2,Param3
    , typename ::boost::disable_if<
        typename ::boost::mpl::or_<
            typename ::boost::is_same<Param2,void>::type,
            typename ::boost::mpl::not_<typename ::boost::is_same<Param3,void>::type>::type
            >::type
    >::type>
    : euml_action<Process2_<ToProcessEvt,Value, Param1, Param2, Param3> >
{
    template <class Event,class FSM,class STATE >
    struct state_action_result 
    {
        typedef void type;
    };
    template <class EVT,class FSM,class SourceState,class TargetState>
    struct transition_action_result 
    {
        typedef void type;
    };

    typedef ::boost::mpl::set<state_action_tag,action_tag> tag_type;

    template <class EVT,class FSM,class SourceState,class TargetState>
    void operator()(EVT const& evt,FSM& fsm,SourceState& src,TargetState& tgt)const
    {
        (Param1()(evt,fsm,src,tgt)).process_event(ToProcessEvt(Value()(evt,fsm,src,tgt)));
        (Param2()(evt,fsm,src,tgt)).process_event(ToProcessEvt(Value()(evt,fsm,src,tgt)));
    }
    template <class Event,class FSM,class STATE>
    void operator()(Event const& evt,FSM& fsm,STATE&  state)const
    {
        (Param1()(evt,fsm,state)).process_event(ToProcessEvt(Value()(evt,fsm,state)));
        (Param2()(evt,fsm,state)).process_event(ToProcessEvt(Value()(evt,fsm,state)));
    }
};


template <class ToProcessEvt,class Value,class Param1, class Param2, class Param3>
struct Process2_ <ToProcessEvt,Value,Param1,Param2,Param3
    , typename ::boost::disable_if<
            typename ::boost::is_same<Param3,void>::type
            >::type>
    : euml_action<Process2_<ToProcessEvt,Value, Param1, Param2, Param3> >
{
    template <class Event,class FSM,class STATE >
    struct state_action_result 
    {
        typedef void type;
    };
    template <class EVT,class FSM,class SourceState,class TargetState>
    struct transition_action_result 
    {
        typedef void type;
    };

    typedef ::boost::mpl::set<state_action_tag,action_tag> tag_type;

    template <class EVT,class FSM,class SourceState,class TargetState>
    void operator()(EVT const& evt,FSM& fsm,SourceState& src,TargetState& tgt)const
    {
        (Param1()(evt,fsm,src,tgt)).process_event(ToProcessEvt(Value()(evt,fsm,src,tgt)));
        (Param2()(evt,fsm,src,tgt)).process_event(ToProcessEvt(Value()(evt,fsm,src,tgt)));
        (Param3()(evt,fsm,src,tgt)).process_event(ToProcessEvt(Value()(evt,fsm,src,tgt)));
    }
    template <class Event,class FSM,class STATE>
    void operator()(Event const& evt,FSM& fsm,STATE&  state)const
    {
        (Param1()(evt,fsm,state)).process_event(ToProcessEvt(Value()(evt,fsm,state)));
        (Param2()(evt,fsm,state)).process_event(ToProcessEvt(Value()(evt,fsm,state)));
        (Param3()(evt,fsm,state)).process_event(ToProcessEvt(Value()(evt,fsm,state)));
    }
};

struct process2_tag {};
struct Process2_Helper : proto::extends< proto::terminal<process2_tag>::type, Process2_Helper, sm_domain>
{
    using proto::extends< proto::terminal<process2_tag>::type, Process2_Helper, sm_domain>::operator=;
    template <class Arg1,class Arg2,class Arg3,class Arg4,class Arg5 
#ifdef BOOST_MSVC 
 ,class Arg6 
#endif
>
    struct In
    {
        typedef Process2_<Arg1,Arg2,Arg3,Arg4,Arg5> type;
    };
};
Process2_Helper const process2_;

template <class Flag,class Param1=void, class Enable=void >                                             
struct Get_Flag_ : euml_action<Get_Flag_<Flag,Param1,Enable> > {};        

template <class Flag,class Param1>
struct Get_Flag_ <Flag,Param1
    , typename ::boost::enable_if<typename ::boost::is_same<Param1,void>::type >::type>
    : euml_action<Get_Flag_<Flag, Param1> >
{
    template <class Event,class FSM,class STATE >
    struct state_action_result 
    {
        typedef bool type;
    };
    template <class EVT,class FSM,class SourceState,class TargetState>
    struct transition_action_result 
    {
        typedef bool type;
    };

    typedef ::boost::mpl::set<state_action_tag,action_tag> tag_type;

    template <class EVT,class FSM,class SourceState,class TargetState>
    bool operator()(EVT const&,FSM& fsm,SourceState& ,TargetState& )const
    {
        return fsm.template is_flag_active<Flag>();
    }
    template <class Event,class FSM,class STATE>
    bool operator()(Event const& ,FSM& fsm,STATE&  )const
    {
        return fsm.template is_flag_active<Flag>();
    }
};

template <class Flag,class Param1>
struct Get_Flag_ <Flag,Param1
    , typename ::boost::disable_if<
            typename ::boost::is_same<Param1,void>::type
            >::type>
    : euml_action<Get_Flag_<Flag, Param1> >
{
    template <class Event,class FSM,class STATE >
    struct state_action_result 
    {
        typedef bool type;
    };
    template <class EVT,class FSM,class SourceState,class TargetState>
    struct transition_action_result 
    {
        typedef bool type;
    };

    typedef ::boost::mpl::set<state_action_tag,action_tag> tag_type;

    template <class EVT,class FSM,class SourceState,class TargetState>
    bool operator()(EVT const& evt,FSM& fsm,SourceState& src,TargetState& tgt)const
    {
        return (Param1()(evt,fsm,src,tgt)).template is_flag_active<Flag>();
    }
    template <class Event,class FSM,class STATE>
    bool operator()(Event const& evt,FSM& fsm,STATE&  state)const
    {
        return (Param1()(evt,fsm,state)).template is_flag_active<Flag>();
    }
};

struct get_flag_tag 
{
};
struct Get_Flag_Helper: proto::extends< proto::terminal<get_flag_tag>::type, Get_Flag_Helper, sm_domain>
{
    template <class Arg1,class Arg2,class Arg3,class Arg4,class Arg5 
#ifdef BOOST_MSVC 
 ,class Arg6 
#endif
>
    struct In
    {
        typedef Get_Flag_<Arg1,Arg2> type;
    };
};
Get_Flag_Helper const is_flag_;

#ifdef BOOST_MSVC
#define MSM_EUML_FUNCTION(functor,function,function_name,result_trans,result_state)                     \
    template <class Param1=void , class Param2=void , class Param3=void , class Param4=void,            \
    class Param5=void,class Param6=void,class Enable=void >                                             \
    struct functor : euml_action<functor<Param1,Param2,Param3,Param4,Param5,Param6,Enable> > {};        \
    template <class Param1, class Param2, class Param3, class Param4, class Param5, class Param6>       \
    struct functor<Param1,Param2,Param3,Param4,Param5,Param6,                                           \
        typename ::boost::enable_if<typename ::boost::is_same<Param1,void>::type>::type>                \
        : euml_action<functor<Param1,Param2,Param3,Param4,Param5,Param6> > {                            \
        template <class Event,class FSM,class STATE > struct state_action_result {                      \
        typedef result_state type;} ;                                                                   \
        template <class EVT,class FSM,class SourceState,class TargetState>                              \
        struct transition_action_result { typedef result_trans type;};                                  \
        typedef ::boost::mpl::set<boost::msm::front::state_action_tag,boost::msm::front::action_tag> tag_type; \
        template <class EVT,class FSM,class SourceState,class TargetState>                              \
        typename transition_action_result<EVT,FSM,SourceState,TargetState>::type                        \
        operator()(EVT const& , FSM& ,SourceState& ,TargetState& )const {                               \
        return function ();}                                                                            \
        template <class Event,class FSM,class STATE>                                                    \
        typename state_action_result<Event,FSM,STATE>::type                                             \
        operator()(Event const& ,FSM& ,STATE&  )const {                                                 \
        return function ();} };                                                                         \
    template <class Param1, class Param2, class Param3, class Param4, class Param5,class Param6>        \
    struct functor<Param1,Param2,Param3,Param4,Param5,Param6,                                           \
        typename ::boost::disable_if<typename ::boost::mpl::or_<                                        \
        typename ::boost::is_same<Param1,void>::type,typename ::boost::mpl::not_<                       \
        typename ::boost::is_same<Param2,void>::type>::type>::type >::type>                             \
        : euml_action<functor<Param1,Param2,Param3,Param4,Param5,Param6> > {                            \
        template <class Event,class FSM,class STATE > struct state_action_result {                      \
        typedef result_state type;} ;                                                                   \
        template <class EVT,class FSM,class SourceState,class TargetState>                              \
        struct transition_action_result { typedef result_trans type;};                                  \
        typedef ::boost::mpl::set<boost::msm::front::state_action_tag,boost::msm::front::action_tag> tag_type; \
        template <class EVT,class FSM,class SourceState,class TargetState>                              \
        typename ::boost::enable_if<typename ::boost::mpl::has_key<                                     \
        typename Param1::tag_type,boost::msm::front::action_tag>::type,                                 \
        typename transition_action_result<EVT,FSM,SourceState,TargetState>::type >::type                \
        operator()(EVT const& evt, FSM& fsm,SourceState& src,TargetState& tgt)const {                   \
        return function (Param1()(evt,fsm,src,tgt));}                                                   \
        template <class Event,class FSM,class STATE>                                                    \
        typename ::boost::enable_if<typename ::boost::mpl::has_key<                                     \
        typename Param1::tag_type,boost::msm::front::state_action_tag>::type,                           \
        typename state_action_result<Event,FSM,STATE>::type >::type                                     \
        operator()(Event const& evt,FSM& fsm,STATE& state )const {                                      \
        return function (Param1()(evt,fsm,state));} };                                                  \
    template <class Param1, class Param2, class Param3, class Param4, class Param5,class Param6>        \
    struct functor<Param1,Param2,Param3,Param4,Param5,Param6,                                           \
        typename ::boost::disable_if<typename ::boost::mpl::or_<                                        \
        typename ::boost::is_same<Param2,void>::type,typename ::boost::mpl::not_<                       \
        typename ::boost::is_same<Param3,void>::type>::type>::type >::type>                             \
        : euml_action<functor<Param1,Param2,Param3,Param4,Param5,Param6> > {                            \
        template <class Event,class FSM,class STATE > struct state_action_result {                      \
        typedef result_state type;} ;                                                                   \
        template <class EVT,class FSM,class SourceState,class TargetState>                              \
        struct transition_action_result { typedef result_trans type;};                                  \
        typedef ::boost::mpl::set<boost::msm::front::state_action_tag,boost::msm::front::action_tag> tag_type; \
        template <class EVT,class FSM,class SourceState,class TargetState>                              \
        typename ::boost::enable_if<typename ::boost::mpl::has_key<                                     \
        typename Param1::tag_type,boost::msm::front::action_tag>::type,                                 \
        typename transition_action_result<EVT,FSM,SourceState,TargetState>::type >::type                \
        operator()(EVT const& evt, FSM& fsm,SourceState& src,TargetState& tgt)const {                   \
        return function (Param1()(evt,fsm,src,tgt),Param2()(evt,fsm,src,tgt));}                         \
        template <class Event,class FSM,class STATE>                                                    \
        typename ::boost::enable_if<typename ::boost::mpl::has_key<                                     \
        typename Param1::tag_type,boost::msm::front::state_action_tag>::type,                           \
        typename state_action_result<Event,FSM,STATE>::type >::type                                     \
        operator()(Event const& evt,FSM& fsm,STATE& state )const {                                      \
        return function (Param1()(evt,fsm,state),Param2()(evt,fsm,state));} };                          \
    template <class Param1, class Param2, class Param3, class Param4, class Param5,class Param6>        \
    struct functor<Param1,Param2,Param3,Param4,Param5,Param6,                                           \
        typename ::boost::disable_if<typename ::boost::mpl::or_<                                        \
        typename ::boost::is_same<Param3,void>::type,typename ::boost::mpl::not_<                       \
        typename ::boost::is_same<Param4,void>::type>::type>::type >::type>                             \
        : euml_action<functor<Param1,Param2,Param3,Param4,Param5,Param6> > {                            \
        template <class Event,class FSM,class STATE > struct state_action_result {                      \
        typedef result_state type;} ;                                                                   \
        template <class EVT,class FSM,class SourceState,class TargetState>                              \
        struct transition_action_result { typedef result_trans type;};                                  \
        typedef ::boost::mpl::set<boost::msm::front::state_action_tag,boost::msm::front::action_tag> tag_type; \
        template <class EVT,class FSM,class SourceState,class TargetState>                              \
        typename ::boost::enable_if<typename ::boost::mpl::has_key<                                     \
        typename Param1::tag_type,boost::msm::front::action_tag>::type,                                 \
        typename transition_action_result<EVT,FSM,SourceState,TargetState>::type >::type                \
        operator()(EVT const& evt, FSM& fsm,SourceState& src,TargetState& tgt)const {                   \
        return function (Param1()(evt,fsm,src,tgt),Param2()(evt,fsm,src,tgt),Param3()(evt,fsm,src,tgt));} \
        template <class Event,class FSM,class STATE>                                                    \
        typename ::boost::enable_if<typename ::boost::mpl::has_key<                                     \
        typename Param1::tag_type,boost::msm::front::state_action_tag>::type,                           \
        typename state_action_result<Event,FSM,STATE>::type >::type                                     \
        operator()(Event const& evt,FSM& fsm,STATE& state )const {                                      \
        return function (Param1()(evt,fsm,state),Param2()(evt,fsm,state),Param3()(evt,fsm,state));} };  \
    template <class Param1, class Param2, class Param3, class Param4, class Param5,class Param6>        \
    struct functor<Param1,Param2,Param3,Param4,Param5,Param6,                                           \
        typename ::boost::disable_if<typename ::boost::mpl::or_<                                        \
        typename ::boost::is_same<Param4,void>::type,typename ::boost::mpl::not_<                       \
        typename ::boost::is_same<Param5,void>::type>::type>::type >::type>                             \
        : euml_action<functor<Param1,Param2,Param3,Param4,Param5,Param6> > {                            \
        template <class Event,class FSM,class STATE > struct state_action_result {                      \
        typedef result_state type;} ;                                                                   \
        template <class EVT,class FSM,class SourceState,class TargetState>                              \
        struct transition_action_result { typedef result_trans type;};                                  \
        typedef ::boost::mpl::set<boost::msm::front::state_action_tag,boost::msm::front::action_tag> tag_type; \
        template <class EVT,class FSM,class SourceState,class TargetState>                              \
        typename ::boost::enable_if<typename ::boost::mpl::has_key<                                     \
        typename Param1::tag_type,boost::msm::front::action_tag>::type,                                 \
        typename transition_action_result<EVT,FSM,SourceState,TargetState>::type >::type                \
        operator()(EVT const& evt, FSM& fsm,SourceState& src,TargetState& tgt)const {                   \
        return function (Param1()(evt,fsm,src,tgt),Param2()(evt,fsm,src,tgt),Param3()(evt,fsm,src,tgt)  \
        ,Param4()(evt,fsm,src,tgt));}                                                                   \
        template <class Event,class FSM,class STATE>                                                    \
        typename ::boost::enable_if<typename ::boost::mpl::has_key<                                     \
        typename Param1::tag_type,boost::msm::front::state_action_tag>::type,                           \
        typename state_action_result<Event,FSM,STATE>::type >::type                                     \
        operator()(Event const& evt,FSM& fsm,STATE& state )const {                                      \
        return function (Param1()(evt,fsm,state),Param2()(evt,fsm,state),Param3()(evt,fsm,state)        \
        ,Param4()(evt,fsm,state));} };                                                                  \
    template <class Param1, class Param2, class Param3, class Param4, class Param5,class Param6>        \
    struct functor<Param1,Param2,Param3,Param4,Param5,Param6,                                           \
        typename ::boost::disable_if<typename ::boost::mpl::or_<                                        \
        typename ::boost::is_same<Param5,void>::type,typename ::boost::mpl::not_<                       \
        typename ::boost::is_same<Param6,void>::type>::type>::type >::type>                             \
        : euml_action<functor<Param1,Param2,Param3,Param4,Param5,Param6> > {                            \
        template <class Event,class FSM,class STATE > struct state_action_result {                      \
        typedef result_state type;} ;                                                                   \
        template <class EVT,class FSM,class SourceState,class TargetState>                              \
        struct transition_action_result { typedef result_trans type;};                                  \
        typedef ::boost::mpl::set<boost::msm::front::state_action_tag,boost::msm::front::action_tag> tag_type; \
        template <class EVT,class FSM,class SourceState,class TargetState>                              \
        typename ::boost::enable_if<typename ::boost::mpl::has_key<                                     \
        typename Param1::tag_type,boost::msm::front::action_tag>::type,                                 \
        typename transition_action_result<EVT,FSM,SourceState,TargetState>::type >::type                \
        operator()(EVT const& evt, FSM& fsm,SourceState& src,TargetState& tgt)const {                   \
        return function (Param1()(evt,fsm,src,tgt),Param2()(evt,fsm,src,tgt),Param3()(evt,fsm,src,tgt)  \
        ,Param4()(evt,fsm,src,tgt),Param5()(evt,fsm,src,tgt));}                                         \
        template <class Event,class FSM,class STATE>                                                    \
        typename ::boost::enable_if<typename ::boost::mpl::has_key<                                     \
        typename Param1::tag_type,boost::msm::front::state_action_tag>::type,                           \
        typename state_action_result<Event,FSM,STATE>::type >::type                                     \
        operator()(Event const& evt,FSM& fsm,STATE& state )const {                                      \
        return function (Param1()(evt,fsm,state),Param2()(evt,fsm,state),Param3()(evt,fsm,state)        \
        ,Param4()(evt,fsm,state),Param5()(evt,fsm,state));} };                                          \
    template <class Param1, class Param2, class Param3, class Param4, class Param5,class Param6>        \
    struct functor<Param1,Param2,Param3,Param4,Param5,Param6,                                           \
        typename ::boost::disable_if<typename ::boost::is_same<Param6,void>::type>::type>               \
        : euml_action<functor<Param1,Param2,Param3,Param4,Param5,Param6> > {                            \
        template <class Event,class FSM,class STATE > struct state_action_result {                      \
        typedef result_state type;} ;                                                                   \
        template <class EVT,class FSM,class SourceState,class TargetState>                              \
        struct transition_action_result { typedef result_trans type;};                                  \
        typedef ::boost::mpl::set<boost::msm::front::state_action_tag,boost::msm::front::action_tag> tag_type; \
        template <class EVT,class FSM,class SourceState,class TargetState>                              \
        typename ::boost::enable_if<typename ::boost::mpl::has_key<                                     \
        typename Param1::tag_type,boost::msm::front::action_tag>::type,                                 \
        typename transition_action_result<EVT,FSM,SourceState,TargetState>::type >::type                \
        operator()(EVT const& evt, FSM& fsm,SourceState& src,TargetState& tgt)const {                   \
        return function (Param1()(evt,fsm,src,tgt),Param2()(evt,fsm,src,tgt),Param3()(evt,fsm,src,tgt)  \
        ,Param4()(evt,fsm,src,tgt),Param5()(evt,fsm,src,tgt),Param6()(evt,fsm,src,tgt));}               \
        template <class Event,class FSM,class STATE>                                                    \
        typename ::boost::enable_if<typename ::boost::mpl::has_key<                                     \
        typename Param1::tag_type,boost::msm::front::state_action_tag>::type,                           \
        typename state_action_result<Event,FSM,STATE>::type >::type                                     \
        operator()(Event const& evt,FSM& fsm,STATE& state )const {                                      \
        return function (Param1()(evt,fsm,state),Param2()(evt,fsm,state),Param3()(evt,fsm,state)        \
        ,Param4()(evt,fsm,state),Param5()(evt,fsm,state),Param6()(evt,fsm,state));} };                  \
        struct function_name ## tag{};                                                                  \
        struct functor ## Helper : proto::extends< proto::terminal< function_name ## tag >::type,       \
        functor ## Helper , sm_domain> { template <class Arg1,class Arg2,class Arg3,class Arg4,class Arg5,class Arg6> \
        struct In {typedef functor <Arg1,Arg2,Arg3,Arg4,Arg5,Arg6> type;}; };                           \
        functor ## Helper const function_name ;

#define MSM_EUML_METHOD(functor,function,function_name,result_trans,result_state)                     \
    template <class Param1=void , class Param2=void , class Param3=void , class Param4=void,            \
    class Param5=void,class Param6=void,class Enable=void >                                             \
    struct functor : euml_action<functor<Param1,Param2,Param3,Param4,Param5,Param6,Enable> > {};        \
    template <class Param1, class Param2, class Param3, class Param4, class Param5, class Param6>       \
    struct functor<Param1,Param2,Param3,Param4,Param5,Param6,                                           \
        typename ::boost::enable_if<typename ::boost::is_same<Param2,void>::type>::type>                \
        : euml_action<functor<Param1,Param2,Param3,Param4,Param5,Param6> > {                            \
        template <class Event,class FSM,class STATE > struct state_action_result {                      \
        typedef result_state type;} ;                                                                   \
        template <class EVT,class FSM,class SourceState,class TargetState>                              \
        struct transition_action_result { typedef result_trans type;};                                  \
        typedef ::boost::mpl::set<boost::msm::front::state_action_tag,boost::msm::front::action_tag> tag_type; \
        template <class EVT,class FSM,class SourceState,class TargetState>                              \
        typename ::boost::enable_if<typename ::boost::mpl::has_key<                                     \
        typename Param1::tag_type,boost::msm::front::action_tag>::type,                                 \
        typename transition_action_result<EVT,FSM,SourceState,TargetState>::type >::type                \
        operator()(EVT const& evt, FSM& fsm,SourceState& src,TargetState& tgt)const {                   \
        return (Param1()(evt,fsm,src,tgt)).function();}                                                 \
        template <class Event,class FSM,class STATE>                                                    \
        typename ::boost::enable_if<typename ::boost::mpl::has_key<                                     \
        typename Param1::tag_type,boost::msm::front::state_action_tag>::type,                           \
        typename state_action_result<Event,FSM,STATE>::type >::type                                     \
        operator()(Event const& evt,FSM& fsm,STATE& state )const {                                      \
        return (Param1()(evt,fsm,state)).function();} };                                                \
    template <class Param1, class Param2, class Param3, class Param4, class Param5,class Param6>        \
    struct functor<Param1,Param2,Param3,Param4,Param5,Param6,                                           \
        typename ::boost::disable_if<typename ::boost::mpl::or_<                                        \
        typename ::boost::is_same<Param2,void>::type,typename ::boost::mpl::not_<                       \
        typename ::boost::is_same<Param3,void>::type>::type>::type >::type>                             \
        : euml_action<functor<Param1,Param2,Param3,Param4,Param5,Param6> > {                            \
        template <class Event,class FSM,class STATE > struct state_action_result {                      \
        typedef result_state type;} ;                                                                   \
        template <class EVT,class FSM,class SourceState,class TargetState>                              \
        struct transition_action_result { typedef result_trans type;};                                  \
        typedef ::boost::mpl::set<boost::msm::front::state_action_tag,boost::msm::front::action_tag> tag_type; \
        template <class EVT,class FSM,class SourceState,class TargetState>                              \
        typename ::boost::enable_if<typename ::boost::mpl::has_key<                                     \
        typename Param1::tag_type,boost::msm::front::action_tag>::type,                                 \
        typename transition_action_result<EVT,FSM,SourceState,TargetState>::type >::type                \
        operator()(EVT const& evt, FSM& fsm,SourceState& src,TargetState& tgt)const {                   \
        return (Param1()(evt,fsm,src,tgt)).function(Param2()(evt,fsm,src,tgt));}                        \
        template <class Event,class FSM,class STATE>                                                    \
        typename ::boost::enable_if<typename ::boost::mpl::has_key<                                     \
        typename Param1::tag_type,boost::msm::front::state_action_tag>::type,                           \
        typename state_action_result<Event,FSM,STATE>::type >::type                                     \
        operator()(Event const& evt,FSM& fsm,STATE& state )const {                                      \
        return (Param1()(evt,fsm,state)).function(Param2()(evt,fsm,state));} };                         \
    template <class Param1, class Param2, class Param3, class Param4, class Param5,class Param6>        \
    struct functor<Param1,Param2,Param3,Param4,Param5,Param6,                                           \
        typename ::boost::disable_if<typename ::boost::mpl::or_<                                        \
        typename ::boost::is_same<Param3,void>::type,typename ::boost::mpl::not_<                       \
        typename ::boost::is_same<Param4,void>::type>::type>::type >::type>                             \
        : euml_action<functor<Param1,Param2,Param3,Param4,Param5,Param6> > {                            \
        template <class Event,class FSM,class STATE > struct state_action_result {                      \
        typedef result_state type;} ;                                                                   \
        template <class EVT,class FSM,class SourceState,class TargetState>                              \
        struct transition_action_result { typedef result_trans type;};                                  \
        typedef ::boost::mpl::set<boost::msm::front::state_action_tag,boost::msm::front::action_tag> tag_type; \
        template <class EVT,class FSM,class SourceState,class TargetState>                              \
        typename ::boost::enable_if<typename ::boost::mpl::has_key<                                     \
        typename Param1::tag_type,boost::msm::front::action_tag>::type,                                 \
        typename transition_action_result<EVT,FSM,SourceState,TargetState>::type >::type                \
        operator()(EVT const& evt, FSM& fsm,SourceState& src,TargetState& tgt)const {                   \
        return (Param1()(evt,fsm,src,tgt)).function(Param2()(evt,fsm,src,tgt),Param3()(evt,fsm,src,tgt));} \
        template <class Event,class FSM,class STATE>                                                    \
        typename ::boost::enable_if<typename ::boost::mpl::has_key<                                     \
        typename Param1::tag_type,boost::msm::front::state_action_tag>::type,                           \
        typename state_action_result<Event,FSM,STATE>::type >::type                                     \
        operator()(Event const& evt,FSM& fsm,STATE& state )const {                                      \
        return (Param1()(evt,fsm,state)).function(Param2()(evt,fsm,state),Param3()(evt,fsm,state));} }; \
    template <class Param1, class Param2, class Param3, class Param4, class Param5,class Param6>        \
    struct functor<Param1,Param2,Param3,Param4,Param5,Param6,                                           \
        typename ::boost::disable_if<typename ::boost::mpl::or_<                                        \
        typename ::boost::is_same<Param4,void>::type,typename ::boost::mpl::not_<                       \
        typename ::boost::is_same<Param5,void>::type>::type>::type >::type>                             \
        : euml_action<functor<Param1,Param2,Param3,Param4,Param5,Param6> > {                            \
        template <class Event,class FSM,class STATE > struct state_action_result {                      \
        typedef result_state type;} ;                                                                   \
        template <class EVT,class FSM,class SourceState,class TargetState>                              \
        struct transition_action_result { typedef result_trans type;};                                  \
        typedef ::boost::mpl::set<boost::msm::front::state_action_tag,boost::msm::front::action_tag> tag_type; \
        template <class EVT,class FSM,class SourceState,class TargetState>                              \
        typename ::boost::enable_if<typename ::boost::mpl::has_key<                                     \
        typename Param1::tag_type,boost::msm::front::action_tag>::type,                                 \
        typename transition_action_result<EVT,FSM,SourceState,TargetState>::type >::type                \
        operator()(EVT const& evt, FSM& fsm,SourceState& src,TargetState& tgt)const {                   \
        return (Param1()(evt,fsm,src,tgt)).function(Param2()(evt,fsm,src,tgt),Param3()(evt,fsm,src,tgt) \
        ,Param4()(evt,fsm,src,tgt));}                                                                   \
        template <class Event,class FSM,class STATE>                                                    \
        typename ::boost::enable_if<typename ::boost::mpl::has_key<                                     \
        typename Param1::tag_type,boost::msm::front::state_action_tag>::type,                           \
        typename state_action_result<Event,FSM,STATE>::type >::type                                     \
        operator()(Event const& evt,FSM& fsm,STATE& state )const {                                      \
        return (Param1()(evt,fsm,state)).function(Param2()(evt,fsm,state),Param3()(evt,fsm,state)       \
        ,Param4()(evt,fsm,state));} };                                                                  \
    template <class Param1, class Param2, class Param3, class Param4, class Param5,class Param6>        \
    struct functor<Param1,Param2,Param3,Param4,Param5,Param6,                                           \
        typename ::boost::disable_if<typename ::boost::mpl::or_<                                        \
        typename ::boost::is_same<Param5,void>::type,typename ::boost::mpl::not_<                       \
        typename ::boost::is_same<Param6,void>::type>::type>::type >::type>                             \
        : euml_action<functor<Param1,Param2,Param3,Param4,Param5,Param6> > {                            \
        template <class Event,class FSM,class STATE > struct state_action_result {                      \
        typedef result_state type;} ;                                                                   \
        template <class EVT,class FSM,class SourceState,class TargetState>                              \
        struct transition_action_result { typedef result_trans type;};                                  \
        typedef ::boost::mpl::set<boost::msm::front::state_action_tag,boost::msm::front::action_tag> tag_type; \
        template <class EVT,class FSM,class SourceState,class TargetState>                              \
        typename ::boost::enable_if<typename ::boost::mpl::has_key<                                     \
        typename Param1::tag_type,boost::msm::front::action_tag>::type,                                 \
        typename transition_action_result<EVT,FSM,SourceState,TargetState>::type >::type                \
        operator()(EVT const& evt, FSM& fsm,SourceState& src,TargetState& tgt)const {                   \
        return (Param1()(evt,fsm,src,tgt)).function(Param2()(evt,fsm,src,tgt),Param3()(evt,fsm,src,tgt) \
        ,Param4()(evt,fsm,src,tgt),Param5()(evt,fsm,src,tgt));}                                         \
        template <class Event,class FSM,class STATE>                                                    \
        typename ::boost::enable_if<typename ::boost::mpl::has_key<                                     \
        typename Param1::tag_type,boost::msm::front::state_action_tag>::type,                           \
        typename state_action_result<Event,FSM,STATE>::type >::type                                     \
        operator()(Event const& evt,FSM& fsm,STATE& state )const {                                      \
        return (Param1()(evt,fsm,state)).function(Param2()(evt,fsm,state),Param3()(evt,fsm,state)       \
        ,Param4()(evt,fsm,state),Param5()(evt,fsm,state));} };                                          \
    template <class Param1, class Param2, class Param3, class Param4, class Param5,class Param6>        \
    struct functor<Param1,Param2,Param3,Param4,Param5,Param6,                                           \
        typename ::boost::disable_if<typename ::boost::is_same<Param6,void>::type>::type>               \
        : euml_action<functor<Param1,Param2,Param3,Param4,Param5,Param6> > {                            \
        template <class Event,class FSM,class STATE > struct state_action_result {                      \
        typedef result_state type;} ;                                                                   \
        template <class EVT,class FSM,class SourceState,class TargetState>                              \
        struct transition_action_result { typedef result_trans type;};                                  \
        typedef ::boost::mpl::set<boost::msm::front::state_action_tag,boost::msm::front::action_tag> tag_type; \
        template <class EVT,class FSM,class SourceState,class TargetState>                              \
        typename ::boost::enable_if<typename ::boost::mpl::has_key<                                     \
        typename Param1::tag_type,boost::msm::front::action_tag>::type,                                 \
        typename transition_action_result<EVT,FSM,SourceState,TargetState>::type >::type                \
        operator()(EVT const& evt, FSM& fsm,SourceState& src,TargetState& tgt)const {                   \
        return (Param1()(evt,fsm,src,tgt)).function(Param2()(evt,fsm,src,tgt),Param3()(evt,fsm,src,tgt) \
        ,Param4()(evt,fsm,src,tgt),Param5()(evt,fsm,src,tgt),Param6()(evt,fsm,src,tgt));}               \
        template <class Event,class FSM,class STATE>                                                    \
        typename ::boost::enable_if<typename ::boost::mpl::has_key<                                     \
        typename Param1::tag_type,boost::msm::front::state_action_tag>::type,                                              \
        typename state_action_result<Event,FSM,STATE>::type >::type                                     \
        operator()(Event const& evt,FSM& fsm,STATE& state )const {                                      \
        return (Param1()(evt,fsm,state)).function(Param2()(evt,fsm,state),Param3()(evt,fsm,state)       \
        ,Param4()(evt,fsm,state),Param5()(evt,fsm,state),Param6()(evt,fsm,state));} };                  \
        struct function_name ## tag{};                                                                  \
        struct functor ## Helper : proto::extends< proto::terminal< function_name ## tag >::type,       \
        functor ## Helper , sm_domain> { template <class Arg1,class Arg2,class Arg3,class Arg4,class Arg5,class Arg6> \
        struct In {typedef functor <Arg1,Arg2,Arg3,Arg4,Arg5,Arg6> type;}; };                           \
        functor ## Helper const function_name ;

#else

#define MSM_EUML_FUNCTION(functor,function,function_name,result_trans,result_state)                     \
    template <class Param1=void , class Param2=void , class Param3=void , class Param4=void,            \
    class Param5=void,class Enable=void >                                                               \
    struct functor : euml_action<functor<Param1,Param2,Param3,Param4,Param5,Enable> > {};               \
    template <class Param1, class Param2, class Param3, class Param4, class Param5>                     \
    struct functor<Param1,Param2,Param3,Param4,Param5,                                                  \
        typename ::boost::disable_if<typename ::boost::mpl::or_<                                        \
        typename ::boost::is_same<Param1,void>::type,typename ::boost::mpl::not_<                       \
        typename ::boost::is_same<Param2,void>::type>::type>::type >::type>                             \
        : euml_action<functor<Param1,Param2,Param3,Param4,Param5> > {                                   \
        template <class Event,class FSM,class STATE > struct state_action_result {                      \
        typedef result_state type;} ;                                                                   \
        template <class EVT,class FSM,class SourceState,class TargetState>                              \
        struct transition_action_result { typedef result_trans type;};                                  \
        typedef ::boost::mpl::set<boost::msm::front::state_action_tag,boost::msm::front::action_tag> tag_type; \
        template <class EVT,class FSM,class SourceState,class TargetState>                              \
        typename ::boost::enable_if<typename ::boost::mpl::has_key<                                     \
        typename Param1::tag_type,boost::msm::front::action_tag>::type,                                 \
        typename transition_action_result<EVT,FSM,SourceState,TargetState>::type >::type                \
        operator()(EVT const& evt, FSM& fsm,SourceState& src,TargetState& tgt)const {                   \
        return function (Param1()(evt,fsm,src,tgt));}                                                   \
        template <class Event,class FSM,class STATE>                                                    \
        typename ::boost::enable_if<typename ::boost::mpl::has_key<                                     \
        typename Param1::tag_type,boost::msm::front::state_action_tag>::type,                           \
        typename state_action_result<Event,FSM,STATE>::type >::type                                     \
        operator()(Event const& evt,FSM& fsm,STATE& state )const {                                      \
        return function (Param1()(evt,fsm,state));} };                                                  \
    template <class Param1, class Param2, class Param3, class Param4, class Param5>                     \
    struct functor<Param1,Param2,Param3,Param4,Param5,                                                  \
        typename ::boost::disable_if<typename ::boost::mpl::or_<                                        \
        typename ::boost::is_same<Param2,void>::type,typename ::boost::mpl::not_<                       \
        typename ::boost::is_same<Param3,void>::type>::type>::type >::type>                             \
        : euml_action<functor<Param1,Param2,Param3,Param4,Param5> > {                                   \
        template <class Event,class FSM,class STATE > struct state_action_result {                      \
        typedef result_state type;} ;                                                                   \
        template <class EVT,class FSM,class SourceState,class TargetState>                              \
        struct transition_action_result { typedef result_trans type;};                                  \
        typedef ::boost::mpl::set<boost::msm::front::state_action_tag,boost::msm::front::action_tag> tag_type; \
        template <class EVT,class FSM,class SourceState,class TargetState>                              \
        typename ::boost::enable_if<typename ::boost::mpl::has_key<                                     \
        typename Param1::tag_type,boost::msm::front::action_tag>::type,                                 \
        typename transition_action_result<EVT,FSM,SourceState,TargetState>::type >::type                \
        operator()(EVT const& evt, FSM& fsm,SourceState& src,TargetState& tgt)const {                   \
        return function (Param1()(evt,fsm,src,tgt),Param2()(evt,fsm,src,tgt));}                         \
        template <class Event,class FSM,class STATE>                                                    \
        typename ::boost::enable_if<typename ::boost::mpl::has_key<                                     \
        typename Param1::tag_type,boost::msm::front::state_action_tag>::type,                           \
        typename state_action_result<Event,FSM,STATE>::type >::type                                     \
        operator()(Event const& evt,FSM& fsm,STATE& state )const {                                      \
        return function (Param1()(evt,fsm,state),Param2()(evt,fsm,state));} };                          \
    template <class Param1, class Param2, class Param3, class Param4, class Param5>                     \
    struct functor<Param1,Param2,Param3,Param4,Param5,                                                  \
        typename ::boost::disable_if<typename ::boost::mpl::or_<                                        \
        typename ::boost::is_same<Param3,void>::type,typename ::boost::mpl::not_<                       \
        typename ::boost::is_same<Param4,void>::type>::type>::type >::type>                             \
        : euml_action<functor<Param1,Param2,Param3,Param4,Param5> > {                                   \
        template <class Event,class FSM,class STATE > struct state_action_result {                      \
        typedef result_state type;} ;                                                                   \
        template <class EVT,class FSM,class SourceState,class TargetState>                              \
        struct transition_action_result { typedef result_trans type;};                                  \
        typedef ::boost::mpl::set<boost::msm::front::state_action_tag,boost::msm::front::action_tag> tag_type; \
        template <class EVT,class FSM,class SourceState,class TargetState>                              \
        typename ::boost::enable_if<typename ::boost::mpl::has_key<                                     \
        typename Param1::tag_type,boost::msm::front::action_tag>::type,                                 \
        typename transition_action_result<EVT,FSM,SourceState,TargetState>::type >::type                \
        operator()(EVT const& evt, FSM& fsm,SourceState& src,TargetState& tgt)const {                   \
        return function (Param1()(evt,fsm,src,tgt),Param2()(evt,fsm,src,tgt),Param3()(evt,fsm,src,tgt));} \
        template <class Event,class FSM,class STATE>                                                    \
        typename ::boost::enable_if<typename ::boost::mpl::has_key<                                     \
        typename Param1::tag_type,boost::msm::front::state_action_tag>::type,                           \
        typename state_action_result<Event,FSM,STATE>::type >::type                                     \
        operator()(Event const& evt,FSM& fsm,STATE& state )const {                                      \
        return function (Param1()(evt,fsm,state),Param2()(evt,fsm,state),Param3()(evt,fsm,state));} };  \
    template <class Param1, class Param2, class Param3, class Param4, class Param5>                     \
    struct functor<Param1,Param2,Param3,Param4,Param5,                                                  \
        typename ::boost::disable_if<typename ::boost::mpl::or_<                                        \
        typename ::boost::is_same<Param4,void>::type,typename ::boost::mpl::not_<                       \
        typename ::boost::is_same<Param5,void>::type>::type>::type >::type>                             \
        : euml_action<functor<Param1,Param2,Param3,Param4,Param5> > {                                   \
        template <class Event,class FSM,class STATE > struct state_action_result {                      \
        typedef result_state type;} ;                                                                   \
        template <class EVT,class FSM,class SourceState,class TargetState>                              \
        struct transition_action_result { typedef result_trans type;};                                  \
        typedef ::boost::mpl::set<boost::msm::front::state_action_tag,boost::msm::front::action_tag> tag_type; \
        template <class EVT,class FSM,class SourceState,class TargetState>                              \
        typename ::boost::enable_if<typename ::boost::mpl::has_key<                                     \
        typename Param1::tag_type,boost::msm::front::action_tag>::type,                                 \
        typename transition_action_result<EVT,FSM,SourceState,TargetState>::type >::type                \
        operator()(EVT const& evt, FSM& fsm,SourceState& src,TargetState& tgt)const {                   \
        return function (Param1()(evt,fsm,src,tgt),Param2()(evt,fsm,src,tgt),Param3()(evt,fsm,src,tgt)  \
        ,Param4()(evt,fsm,src,tgt));}                                                                   \
        template <class Event,class FSM,class STATE>                                                    \
        typename ::boost::enable_if<typename ::boost::mpl::has_key<                                     \
        typename Param1::tag_type,boost::msm::front::state_action_tag>::type,                           \
        typename state_action_result<Event,FSM,STATE>::type >::type                                     \
        operator()(Event const& evt,FSM& fsm,STATE& state )const {                                      \
        return function (Param1()(evt,fsm,state),Param2()(evt,fsm,state),Param3()(evt,fsm,state)        \
        ,Param4()(evt,fsm,state));} };                                                                  \
    template <class Param1, class Param2, class Param3, class Param4, class Param5>                     \
    struct functor<Param1,Param2,Param3,Param4,Param5,                                                  \
        typename ::boost::disable_if<typename ::boost::is_same<Param5,void>::type>::type>               \
        : euml_action<functor<Param1,Param2,Param3,Param4,Param5> > {                                   \
        template <class Event,class FSM,class STATE > struct state_action_result {                      \
        typedef result_state type;} ;                                                                   \
        template <class EVT,class FSM,class SourceState,class TargetState>                              \
        struct transition_action_result { typedef result_trans type;};                                  \
        typedef ::boost::mpl::set<boost::msm::front::state_action_tag,boost::msm::front::action_tag> tag_type; \
        template <class EVT,class FSM,class SourceState,class TargetState>                              \
        typename ::boost::enable_if<typename ::boost::mpl::has_key<                                     \
        typename Param1::tag_type,boost::msm::front::action_tag>::type,                                 \
        typename transition_action_result<EVT,FSM,SourceState,TargetState>::type >::type                \
        operator()(EVT const& evt, FSM& fsm,SourceState& src,TargetState& tgt)const {                   \
        return function (Param1()(evt,fsm,src,tgt),Param2()(evt,fsm,src,tgt),Param3()(evt,fsm,src,tgt)  \
        ,Param4()(evt,fsm,src,tgt),Param5()(evt,fsm,src,tgt));}                                         \
        template <class Event,class FSM,class STATE>                                                    \
        typename ::boost::enable_if<typename ::boost::mpl::has_key<                                     \
        typename Param1::tag_type,boost::msm::front::state_action_tag>::type,                           \
        typename state_action_result<Event,FSM,STATE>::type >::type                                     \
        operator()(Event const& evt,FSM& fsm,STATE& state )const {                                      \
        return function (Param1()(evt,fsm,state),Param2()(evt,fsm,state),Param3()(evt,fsm,state)        \
        ,Param4()(evt,fsm,state),Param5()(evt,fsm,state));} };                                          \
        struct function_name ## tag{};                                                                  \
        struct functor ## Helper : proto::extends< proto::terminal< function_name ## tag >::type,       \
        functor ## Helper , sm_domain> { template <class Arg1,class Arg2,class Arg3,class Arg4,class Arg5> \
        struct In {typedef functor <Arg1,Arg2,Arg3,Arg4,Arg5> type;}; };                           		\
        functor ## Helper const function_name ;

#define MSM_EUML_METHOD(functor,function,function_name,result_trans,result_state)                     \
    template <class Param1=void , class Param2=void , class Param3=void , class Param4=void,            \
    class Param5=void,class Enable=void >                                                               \
    struct functor : euml_action<functor<Param1,Param2,Param3,Param4,Param5,Enable> > {};               \
    template <class Param1, class Param2, class Param3, class Param4, class Param5>                     \
    struct functor<Param1,Param2,Param3,Param4,Param5,                                                  \
        typename ::boost::enable_if<typename ::boost::is_same<Param2,void>::type>::type>                \
        : euml_action<functor<Param1,Param2,Param3,Param4,Param5> > {                                   \
        template <class Event,class FSM,class STATE > struct state_action_result {                      \
        typedef result_state type;} ;                                                                   \
        template <class EVT,class FSM,class SourceState,class TargetState>                              \
        struct transition_action_result { typedef result_trans type;};                                  \
        typedef ::boost::mpl::set<boost::msm::front::state_action_tag,boost::msm::front::action_tag> tag_type; \
        template <class EVT,class FSM,class SourceState,class TargetState>                              \
        typename ::boost::enable_if<typename ::boost::mpl::has_key<                                     \
        typename Param1::tag_type,boost::msm::front::action_tag>::type,                                 \
        typename transition_action_result<EVT,FSM,SourceState,TargetState>::type >::type                \
        operator()(EVT const& evt, FSM& fsm,SourceState& src,TargetState& tgt)const {                   \
        return (Param1()(evt,fsm,src,tgt)).function();}                                                 \
        template <class Event,class FSM,class STATE>                                                    \
        typename ::boost::enable_if<typename ::boost::mpl::has_key<                                     \
        typename Param1::tag_type,boost::msm::front::state_action_tag>::type,                           \
        typename state_action_result<Event,FSM,STATE>::type >::type                                     \
        operator()(Event const& evt,FSM& fsm,STATE& state )const {                                      \
        return (Param1()(evt,fsm,state)).function();} };                                                \
    template <class Param1, class Param2, class Param3, class Param4, class Param5>                     \
    struct functor<Param1,Param2,Param3,Param4,Param5,                                                  \
        typename ::boost::disable_if<typename ::boost::mpl::or_<                                        \
        typename ::boost::is_same<Param2,void>::type,typename ::boost::mpl::not_<                       \
        typename ::boost::is_same<Param3,void>::type>::type>::type >::type>                             \
        : euml_action<functor<Param1,Param2,Param3,Param4,Param5> > {                                   \
        template <class Event,class FSM,class STATE > struct state_action_result {                      \
        typedef result_state type;} ;                                                                   \
        template <class EVT,class FSM,class SourceState,class TargetState>                              \
        struct transition_action_result { typedef result_trans type;};                                  \
        typedef ::boost::mpl::set<boost::msm::front::state_action_tag,boost::msm::front::action_tag> tag_type; \
        template <class EVT,class FSM,class SourceState,class TargetState>                              \
        typename ::boost::enable_if<typename ::boost::mpl::has_key<                                     \
        typename Param1::tag_type,boost::msm::front::action_tag>::type,                                 \
        typename transition_action_result<EVT,FSM,SourceState,TargetState>::type >::type                \
        operator()(EVT const& evt, FSM& fsm,SourceState& src,TargetState& tgt)const {                   \
        return (Param1()(evt,fsm,src,tgt)).function(Param2()(evt,fsm,src,tgt));}                        \
        template <class Event,class FSM,class STATE>                                                    \
        typename ::boost::enable_if<typename ::boost::mpl::has_key<                                     \
        typename Param1::tag_type,boost::msm::front::state_action_tag>::type,                           \
        typename state_action_result<Event,FSM,STATE>::type >::type                                     \
        operator()(Event const& evt,FSM& fsm,STATE& state )const {                                      \
        return (Param1()(evt,fsm,state)).function(Param2()(evt,fsm,state));} };                         \
    template <class Param1, class Param2, class Param3, class Param4, class Param5>                     \
    struct functor<Param1,Param2,Param3,Param4,Param5,                                                  \
        typename ::boost::disable_if<typename ::boost::mpl::or_<                                        \
        typename ::boost::is_same<Param3,void>::type,typename ::boost::mpl::not_<                       \
        typename ::boost::is_same<Param4,void>::type>::type>::type >::type>                             \
        : euml_action<functor<Param1,Param2,Param3,Param4,Param5> > {                                   \
        template <class Event,class FSM,class STATE > struct state_action_result {                      \
        typedef result_state type;} ;                                                                   \
        template <class EVT,class FSM,class SourceState,class TargetState>                              \
        struct transition_action_result { typedef result_trans type;};                                  \
        typedef ::boost::mpl::set<boost::msm::front::state_action_tag,boost::msm::front::action_tag> tag_type; \
        template <class EVT,class FSM,class SourceState,class TargetState>                              \
        typename ::boost::enable_if<typename ::boost::mpl::has_key<                                     \
        typename Param1::tag_type,boost::msm::front::action_tag>::type,                                 \
        typename transition_action_result<EVT,FSM,SourceState,TargetState>::type >::type                \
        operator()(EVT const& evt, FSM& fsm,SourceState& src,TargetState& tgt)const {                   \
        return (Param1()(evt,fsm,src,tgt)).function(Param2()(evt,fsm,src,tgt),Param3()(evt,fsm,src,tgt));} \
        template <class Event,class FSM,class STATE>                                                    \
        typename ::boost::enable_if<typename ::boost::mpl::has_key<                                     \
        typename Param1::tag_type,boost::msm::front::state_action_tag>::type,                           \
        typename state_action_result<Event,FSM,STATE>::type >::type                                     \
        operator()(Event const& evt,FSM& fsm,STATE& state )const {                                      \
        return (Param1()(evt,fsm,state)).function(Param2()(evt,fsm,state),Param3()(evt,fsm,state));} }; \
    template <class Param1, class Param2, class Param3, class Param4, class Param5>                     \
    struct functor<Param1,Param2,Param3,Param4,Param5,                                                  \
        typename ::boost::disable_if<typename ::boost::mpl::or_<                                        \
        typename ::boost::is_same<Param4,void>::type,typename ::boost::mpl::not_<                       \
        typename ::boost::is_same<Param5,void>::type>::type>::type >::type>                             \
        : euml_action<functor<Param1,Param2,Param3,Param4,Param5> > {                                   \
        template <class Event,class FSM,class STATE > struct state_action_result {                      \
        typedef result_state type;} ;                                                                   \
        template <class EVT,class FSM,class SourceState,class TargetState>                              \
        struct transition_action_result { typedef result_trans type;};                                  \
        typedef ::boost::mpl::set<boost::msm::front::state_action_tag,boost::msm::front::action_tag> tag_type; \
        template <class EVT,class FSM,class SourceState,class TargetState>                              \
        typename ::boost::enable_if<typename ::boost::mpl::has_key<                                     \
        typename Param1::tag_type,boost::msm::front::action_tag>::type,                                 \
        typename transition_action_result<EVT,FSM,SourceState,TargetState>::type >::type                \
        operator()(EVT const& evt, FSM& fsm,SourceState& src,TargetState& tgt)const {                   \
        return (Param1()(evt,fsm,src,tgt)).function(Param2()(evt,fsm,src,tgt),Param3()(evt,fsm,src,tgt) \
        ,Param4()(evt,fsm,src,tgt));}                                                                   \
        template <class Event,class FSM,class STATE>                                                    \
        typename ::boost::enable_if<typename ::boost::mpl::has_key<                                     \
        typename Param1::tag_type,boost::msm::front::state_action_tag>::type,                           \
        typename state_action_result<Event,FSM,STATE>::type >::type                                     \
        operator()(Event const& evt,FSM& fsm,STATE& state )const {                                      \
        return (Param1()(evt,fsm,state)).function(Param2()(evt,fsm,state),Param3()(evt,fsm,state)       \
        ,Param4()(evt,fsm,state));} };                                                                  \
    template <class Param1, class Param2, class Param3, class Param4, class Param5>                     \
    struct functor<Param1,Param2,Param3,Param4,Param5,                                                  \
        typename ::boost::disable_if<typename ::boost::is_same<Param5,void>::type>::type>               \
        : euml_action<functor<Param1,Param2,Param3,Param4,Param5> > {                                   \
        template <class Event,class FSM,class STATE > struct state_action_result {                      \
        typedef result_state type;} ;                                                                   \
        template <class EVT,class FSM,class SourceState,class TargetState>                              \
        struct transition_action_result { typedef result_trans type;};                                  \
        typedef ::boost::mpl::set<boost::msm::front::state_action_tag,boost::msm::front::action_tag> tag_type; \
        template <class EVT,class FSM,class SourceState,class TargetState>                              \
        typename ::boost::enable_if<typename ::boost::mpl::has_key<                                     \
        typename Param1::tag_type,boost::msm::front::action_tag>::type,                                 \
        typename transition_action_result<EVT,FSM,SourceState,TargetState>::type >::type                \
        operator()(EVT const& evt, FSM& fsm,SourceState& src,TargetState& tgt)const {                   \
        return (Param1()(evt,fsm,src,tgt)).function(Param2()(evt,fsm,src,tgt),Param3()(evt,fsm,src,tgt) \
        ,Param4()(evt,fsm,src,tgt),Param5()(evt,fsm,src,tgt));}                                         \
        template <class Event,class FSM,class STATE>                                                    \
        typename ::boost::enable_if<typename ::boost::mpl::has_key<                                     \
        typename Param1::tag_type,boost::msm::front::state_action_tag>::type,                           \
        typename state_action_result<Event,FSM,STATE>::type >::type                                     \
        operator()(Event const& evt,FSM& fsm,STATE& state )const {                                      \
        return (Param1()(evt,fsm,state)).function(Param2()(evt,fsm,state),Param3()(evt,fsm,state)       \
        ,Param4()(evt,fsm,state),Param5()(evt,fsm,state));} };                                          \
        struct function_name ## tag{};                                                                  \
        struct functor ## Helper : proto::extends< proto::terminal< function_name ## tag >::type,       \
        functor ## Helper , sm_domain> { template <class Arg1,class Arg2,class Arg3,class Arg4,class Arg5> \
        struct In {typedef functor <Arg1,Arg2,Arg3,Arg4,Arg5> type;}; };                           		\
        functor ## Helper const function_name ;

#endif

#define RESULT_TYPE2_PARAM1 typename get_result_type2<Param1,Event,FSM,STATE>::type
#define RESULT_TYPE_PARAM1 typename get_result_type<Param1,EVT,FSM,SourceState,TargetState>::type 
#define RESULT_TYPE2_PARAM2 typename get_result_type2<Param2,Event,FSM,STATE>::type
#define RESULT_TYPE_PARAM2 typename get_result_type<Param2,EVT,FSM,SourceState,TargetState>::type 
#define RESULT_TYPE2_PARAM3 typename get_result_type2<Param3,Event,FSM,STATE>::type
#define RESULT_TYPE_PARAM3 typename get_result_type<Param3,EVT,FSM,SourceState,TargetState>::type 
#define RESULT_TYPE2_PARAM4 typename get_result_type2<Param4,Event,FSM,STATE>::type
#define RESULT_TYPE_PARAM4 typename get_result_type<Param4,EVT,FSM,SourceState,TargetState>::type 
#define RESULT_TYPE2_PARAM5 typename get_result_type2<Param5,Event,FSM,STATE>::type
#define RESULT_TYPE_PARAM5 typename get_result_type<Param5,EVT,FSM,SourceState,TargetState>::type 
#define RESULT_TYPE2_PARAM6 typename get_result_type2<Param6,Event,FSM,STATE>::type
#define RESULT_TYPE_PARAM6 typename get_result_type<Param6,EVT,FSM,SourceState,TargetState>::type 


#define RESULT_TYPE2_DIFF_TYPE_ITER_TRAITS_PARAM1 typename std::iterator_traits<typename get_result_type2<Param1,Event,FSM,STATE>::type>::difference_type
#define RESULT_TYPE_DIFF_TYPE_ITER_TRAITS_PARAM1 typename std::iterator_traits<typename get_result_type<Param1,EVT,FSM,SourceState,TargetState>::type>::difference_type 

#define RESULT_TYPE2_REMOVE_REF_PARAM1 typename ::boost::remove_reference<typename get_result_type2<Param1,Event,FSM,STATE>::type>::type
#define RESULT_TYPE_REMOVE_REF_PARAM1 typename ::boost::remove_reference<typename get_result_type<Param1,EVT,FSM,SourceState,TargetState>::type>::type 

#define RESULT_TYPE2_PAIR_REMOVE_REF_PARAM1 std::pair<RESULT_TYPE2_REMOVE_REF_PARAM1,RESULT_TYPE2_REMOVE_REF_PARAM1>
#define RESULT_TYPE_PAIR_REMOVE_REF_PARAM1 std::pair<RESULT_TYPE_REMOVE_REF_PARAM1,RESULT_TYPE_REMOVE_REF_PARAM1>

#define RESULT_TYPE2_GET_REF_REMOVE_REF_PARAM1 typename get_reference<typename ::boost::remove_reference<typename get_result_type2<Param1,Event,FSM,STATE>::type>::type>::type
#define RESULT_TYPE_GET_REF_REMOVE_REF_PARAM1 typename get_reference<typename ::boost::remove_reference<typename get_result_type<Param1,EVT,FSM,SourceState,TargetState>::type>::type>::type 

#define RESULT_TYPE2_GET_ITERATOR_REMOVE_REF_PARAM1 typename get_iterator<typename ::boost::remove_reference<typename get_result_type2<Param1,Event,FSM,STATE>::type>::type>::type
#define RESULT_TYPE_GET_ITERATOR_REMOVE_REF_PARAM1 typename get_iterator<typename ::boost::remove_reference<typename get_result_type<Param1,EVT,FSM,SourceState,TargetState>::type>::type>::type 

#define RESULT_TYPE2_GET_REV_ITERATOR_REMOVE_REF_PARAM1 typename get_reverse_iterator<typename ::boost::remove_reference<typename get_result_type2<Param1,Event,FSM,STATE>::type>::type>::type
#define RESULT_TYPE_GET_REV_ITERATOR_REMOVE_REF_PARAM1 typename get_reverse_iterator<typename ::boost::remove_reference<typename get_result_type<Param1,EVT,FSM,SourceState,TargetState>::type>::type>::type 

#define RESULT_TYPE2_GET_SIZE_TYPE_REMOVE_REF_PARAM1 typename get_size_type<typename ::boost::remove_reference<typename get_result_type2<Param1,Event,FSM,STATE>::type>::type>::type
#define RESULT_TYPE_GET_SIZE_TYPE_REMOVE_REF_PARAM1 typename get_size_type<typename ::boost::remove_reference<typename get_result_type<Param1,EVT,FSM,SourceState,TargetState>::type>::type>::type 

}}}} // boost::msm::front::euml

#endif // BOOST_MSM_FRONT_EUML_COMMON_H
