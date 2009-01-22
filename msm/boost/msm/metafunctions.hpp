#ifndef METAFUNCTIONS_H
#define METAFUNCTIONS_H

// Copyright 2008 Christophe Henry
// henry UNDERSCORE christophe AT hotmail DOT com
// This is an extended version of the state machine available in the boost::mpl library
// Distributed under the same license as the original.
// Copyright for the original version:
// Copyright 2005 David Abrahams and Aleksey Gurtovoy. Distributed
// under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/mpl/copy_if.hpp>
#include <boost/mpl/unique.hpp>
#include <boost/mpl/set.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/count_if.hpp>

#include <boost/type_traits/is_same.hpp>

namespace mpl = boost::mpl;
BOOST_MPL_HAS_XXX_TRAIT_DEF(explicit_creation)
BOOST_MPL_HAS_XXX_TRAIT_DEF(pseudo_entry)
BOOST_MPL_HAS_XXX_TRAIT_DEF(pseudo_exit)
BOOST_MPL_HAS_XXX_TRAIT_DEF(concrete_exit_state)

namespace boost { namespace msm
{

// returns the current state type of a transition
template <class Transition>
struct transition_source_type
{
    typedef typename Transition::current_state_type type;
};

// returns the target state type of a transition
template <class Transition>
struct transition_target_type
{
    typedef typename Transition::next_state_type type;
};

// helper functions for generate_state_ids
// create a pair of a state and a passed id for source and target states
template <class Id,class Transition>
struct make_pair_source_state_id
{
    typedef typename mpl::pair<typename Transition::current_state_type,Id> type;
};
template <class Id,class Transition>
struct make_pair_target_state_id
{
    typedef typename mpl::pair<typename Transition::next_state_type,Id> type;
};

// iterates through a transition table and automatically generates ids starting at 0
// first the source states, transition up to down
// then the target states, up to down
template <class stt>
struct generate_state_ids
{
    typedef typename 
	    mpl::fold<
	    stt,mpl::pair<mpl::map< >,mpl::int_<0> >,
	    mpl::pair<
		    mpl::if_<
				     mpl::has_key<mpl::first<mpl::placeholders::_1>,transition_source_type<mpl::placeholders::_2> >,
				     mpl::first<mpl::placeholders::_1>,
				     mpl::insert<mpl::first<mpl::placeholders::_1>,
							    make_pair_source_state_id<mpl::second<mpl::placeholders::_1 >,
							    mpl::placeholders::_2> >
				      >,
		    mpl::if_<
				    mpl::has_key<mpl::first<mpl::placeholders::_1>,transition_source_type<mpl::placeholders::_2> >,
				    mpl::second<mpl::placeholders::_1 >,
				    mpl::next<mpl::second<mpl::placeholders::_1 > >
				    >
	    > //pair
	    >::type source_state_ids;
    typedef typename mpl::first<source_state_ids>::type source_state_map;
    typedef typename mpl::second<source_state_ids>::type highest_state_id;


    typedef typename 
	    mpl::fold<
	    stt,mpl::pair<source_state_map,highest_state_id >,
	    mpl::pair<
		    mpl::if_<
				     mpl::has_key<mpl::first<mpl::placeholders::_1>,transition_target_type<mpl::placeholders::_2> >,
				     mpl::first<mpl::placeholders::_1>,
				     mpl::insert<mpl::first<mpl::placeholders::_1>,
							    make_pair_target_state_id<mpl::second<mpl::placeholders::_1 >,
							    mpl::placeholders::_2> >
				     >,
		    mpl::if_<
				    mpl::has_key<mpl::first<mpl::placeholders::_1>,transition_target_type<mpl::placeholders::_2> >,
				    mpl::second<mpl::placeholders::_1 >,
				    mpl::next<mpl::second<mpl::placeholders::_1 > >
				    >
	    > //pair
	    >::type all_state_ids;
    typedef typename mpl::first<all_state_ids>::type type;
};

// returns the id of a given state
template <class stt,class State>
struct get_state_id
{
    typedef typename mpl::at<typename generate_state_ids<stt>::type,State>::type type;
    enum {value = type::value};
};

// returns a mpl::vector containing the init states of a state machine
template <class States>
struct get_initial_states 
{
    typedef typename mpl::if_<
	    mpl::is_sequence<States>,
	    States,
		typename mpl::push_back<mpl::vector<>,States>::type >::type type;
};

// returns a mpl::int_ containing the size of a region. If the argument is not a sequence, returns 1
template <class region>
struct get_number_of_regions 
{
    typedef typename mpl::if_<
	    mpl::is_sequence<region>,
	    mpl::size<region>,
	    mpl::int_<1> >::type type;
};

// builds a mpl::vector of initial states
template <class region>
struct get_regions_as_sequence 
{
    typedef typename mpl::if_<
	    mpl::is_sequence<region>,
	    region,
	    typename mpl::push_back<mpl::vector<>,region>::type >::type type;
};

template <class ToCreateSeq>
struct get_explicit_creation_as_sequence 
{
    typedef typename mpl::if_<
        mpl::is_sequence<ToCreateSeq>,
        ToCreateSeq,
        typename mpl::push_back<mpl::vector<>,ToCreateSeq>::type >::type type;
};

// returns true if 2 transitions have the same source (used to remove duplicates in search of composite states)
template <class stt,class Transition1,class Transition2>
struct have_same_source
{
    enum {current_state1 = get_state_id<stt,typename Transition1::current_state_type >::type::value};
    enum {current_state2 = get_state_id<stt,typename Transition2::current_state_type >::type::value};
    enum {value = ((int)current_state1 == (int)current_state2) };
};


// A metafunction that returns the Event associated with a transition.
template <class Transition>
struct transition_event
{
    typedef typename Transition::event type;
};

// returns true for composite states
template <class State>
struct is_composite_state
{
    enum {value = State::is_composite_tag::value};
    typedef typename State::is_composite_tag type;
};

// transform a transition table in a container of source states
template <class stt>
struct keep_source_names
{
    // instead of the rows we want only the names of the states (from source)
    typedef typename 
	    mpl::transform<
	    stt,transition_source_type<mpl::placeholders::_1> >::type type;
};

// transform a transition table in a container of target states
template <class stt>
struct keep_target_names
{
    // instead of the rows we want only the names of the states (from source)
    typedef typename 
	    mpl::transform<
	    stt,transition_target_type<mpl::placeholders::_1> >::type type;
};

template <class stt>
struct generate_state_set
{
    // keep in the original transition table only the source/target state types
    typedef typename keep_source_names<stt>::type sources;
    typedef typename keep_target_names<stt>::type targets;
    typedef typename 
	    mpl::fold<
	    sources,mpl::set<>,
	    mpl::insert<mpl::placeholders::_1,mpl::placeholders::_2>
	    >::type source_set;
    typedef typename 
	    mpl::fold<
	    targets,source_set,
	    mpl::insert<mpl::placeholders::_1,mpl::placeholders::_2>
	    >::type type;
};

// iterates through the transition table and generate a mpl::set<> containing all the events
template <class stt>
struct generate_event_set
{
    typedef typename 
        mpl::fold<
            stt,mpl::set<>,
            mpl::if_<
                mpl::has_key<mpl::placeholders::_1, transition_event<mpl::placeholders::_2> >,
                mpl::placeholders::_1,
                mpl::insert<mpl::placeholders::_1,transition_event<mpl::placeholders::_2> > >
        >::type type;
};

// returns a mpl::bool_<true> if State has Event as delayed event
template <class State, class Event>
struct has_state_delayed_event  
{
    typedef typename mpl::find<typename State::deferred_events,Event>::type found;
    typedef typename mpl::if_<
	    boost::is_same<found,typename mpl::end<typename State::deferred_events>::type >,
	    mpl::bool_<false>,
	    mpl::bool_<true> >::type type;
};

// Template used to create dummy entries for initial states not found in the stt.
template< typename T1 >
struct not_a_row
{
    struct dummy_event 
    {
    };
    typedef T1                  current_state_type;
    typedef T1                  next_state_type;
    typedef dummy_event         event;
};

template <class StateType>
struct get_explicit_creation 
{
    typedef typename StateType::explicit_creation type;
};

// add to the stt the initial states which could be missing (if not being involved in a transition)
template <class Derived>
struct create_stt 
{
    typedef typename Derived::transition_table stt;
    // get the state set
    typedef typename generate_state_set<stt>::type states;
    // transform the initial region(s) in a sequence
    typedef typename get_regions_as_sequence<typename Derived::initial_state>::type init_states;
    // iterate through the initial states and add them in the stt if not already there
    typedef typename 
	    mpl::fold<
	    init_states,stt,
	    mpl::if_<
			     mpl::has_key<states,mpl::placeholders::_2>,
			     mpl::placeholders::_1,
			     mpl::insert<mpl::placeholders::_1,mpl::end<mpl::placeholders::_1>,
						     not_a_row<mpl::placeholders::_2 > > 
			      >
	    >::type with_init;
    // do the same for states marked as explicitly created
    typedef typename get_explicit_creation_as_sequence<
       typename mpl::eval_if<
            typename has_explicit_creation<Derived>::type,
            get_explicit_creation<Derived>,
            mpl::vector<> >::type
        >::type explicit_created;

    typedef typename 
	    mpl::fold<
	    explicit_created,with_init,
	    mpl::if_<
			     mpl::has_key<states,mpl::placeholders::_2>,
			     mpl::placeholders::_1,
			     mpl::insert<mpl::placeholders::_1,mpl::end<mpl::placeholders::_1>,
						     not_a_row<mpl::placeholders::_2 > > 
			      >
	    >::type type;
};

// returns the transition table of a Composite state
template <class Composite>
struct get_transition_table
{
    typedef typename create_stt<Composite>::type type;
};
// recursively get a transition table for a given composite state.
// returns the transition table for this state + the tables of all composite sub states recursively
template <class Composite>
struct recursive_get_transition_table
{
    // get the transition table of the state if it's a state machine
    typedef typename mpl::eval_if<typename is_composite_state<Composite>::type,
	    get_transition_table<Composite>,
	    mpl::vector0<> >::type org_table;

    typedef typename generate_state_set<org_table>::type states;

    // and for every substate, recursively get the transition table if it's a state machine
    typedef typename mpl::fold<
	    states,org_table,
	    mpl::insert_range<mpl::placeholders::_1,mpl::end<mpl::placeholders::_1>,
	    recursive_get_transition_table<mpl::placeholders::_2 > >
    >::type type;

};

template <class Transition>
struct make_vector 
{
    typedef mpl::vector<Transition> type;
};
template< typename Entry > 
struct get_first_element_pair_second
{ 
    typedef typename mpl::front<typename Entry::second>::type type;
}; 

// returns the owner of an explicit_entry state
// which is the containing SM if the transition originates from outside the containing SM
// or else the explicit_entry state itself
template <class State,class ContainingSM>
struct get_owner 
{
    typedef typename mpl::if_<
                typename mpl::not_<typename boost::is_same<typename State::owner,ContainingSM>::type>::type,
                typename State::owner, State >::type type;
};
template <class Sequence,class ContainingSM>
struct get_fork_owner 
{
    typedef typename mpl::front<Sequence>::type seq_front;
    typedef typename mpl::if_<
                typename mpl::not_<typename boost::is_same<typename seq_front::owner,ContainingSM>::type>::type,
                typename seq_front::owner, seq_front >::type type;
};

// metafunctions used to find out if a state is entry, exit or something else
template <class State>
struct is_pseudo_entry 
{
    typedef typename mpl::if_< typename has_pseudo_entry<State>::type,
        mpl::bool_<true>,mpl::bool_<false> 
    >::type type;
};
// says if a state is an exit pseudo state
template <class State>
struct is_pseudo_exit 
{
    typedef typename mpl::if_< typename has_pseudo_exit<State>::type,
        mpl::bool_<true>,mpl::bool_<false> 
    >::type type;
};

// metafunction used to say if a SM has pseudo exit states
template <class StateType>
struct has_exit_pseudo_states_helper 
{
    typedef typename create_stt<StateType>::type Stt;
    typedef typename generate_state_set<Stt>::type state_list;

    typedef mpl::bool_<mpl::count_if<state_list,is_pseudo_exit<mpl::placeholders::_1> >::value != 0> type;
};
template <class StateType>
struct has_exit_pseudo_states 
{
    typedef typename mpl::eval_if<typename is_composite_state<StateType>::type,
        has_exit_pseudo_states_helper<StateType>,
        mpl::bool_<false> >::type type;
};


// helper to find out if a SM has an active exit state and is therefore waiting for exiting
template <class StateType,class OwnerFct,class FSM>
inline
typename boost::enable_if<typename mpl::and_<typename is_composite_state<FSM>::type,
                                             typename is_pseudo_exit<StateType>::type>,bool >::type
is_exit_state_active(FSM& fsm)
{
    typedef typename OwnerFct::type Composite;
    typedef typename create_stt<Composite>::type stt;
    int state_id = get_state_id<stt,StateType>::type::value;
    Composite& comp = fsm.template get_state<Composite&>();
    return (std::find(comp.current_state().begin(),comp.current_state().end(),state_id)!=comp.current_state().end());
}
template <class StateType,class OwnerFct,class FSM>
inline
typename boost::disable_if<typename mpl::and_<typename is_composite_state<FSM>::type,
                                             typename is_pseudo_exit<StateType>::type>,bool >::type
is_exit_state_active(FSM& fsm)
{
    return false;
}

} } //boost::msm

#endif // METAFUNCTIONS_H
