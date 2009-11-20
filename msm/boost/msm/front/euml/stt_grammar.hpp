// Copyright 2008 Christophe Henry
// henry UNDERSCORE christophe AT hotmail DOT com
// This is an extended version of the state machine available in the boost::mpl library
// Distributed under the same license as the original.
// Copyright for the original version:
// Copyright 2005 David Abrahams and Aleksey Gurtovoy. Distributed
// under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MSM_FRONT_EUML_STT_GRAMMAR_H
#define BOOST_MSM_FRONT_EUML_STT_GRAMMAR_H

#include <boost/msm/front/euml/common.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/eval_if.hpp>

#include <boost/msm/front/euml/operator.hpp>
#include <boost/msm/front/euml/guard_grammar.hpp>
#include <boost/msm/front/euml/state_grammar.hpp>

namespace proto = boost::proto;

namespace boost { namespace msm { namespace front { namespace euml
{
template <class Left,class Right>
struct fusion_left_right 
{
    typedef Row<typename Left::Source,typename Left::Evt,typename Right::Target
               ,typename Right::Action,typename Right::Guard> type;
};
template <class TargetGuard,class ActionClass>
struct fusion_target_action_guard 
{
    typedef Row<none,none,typename TargetGuard::Target,typename ActionClass::Action,typename TargetGuard::Guard> type;
};

// row grammar
struct BuildSourcePlusEvent
    : proto::or_<
            proto::when<
                proto::plus<proto::terminal<state_tag>,proto::terminal<event_tag> >,
                Row<proto::_left,proto::_right,none>()
            >,
            proto::when<
                proto::terminal<state_tag>,
                Row<proto::_,none,none>()
            >
        >
{};
struct BuildActionPlusGuard
    : proto::when<
            proto::subscript<BuildActionSequence,BuildGuards >,
            Row<none,none,none,
			BuildActionSequence(proto::_left),
			BuildGuards(proto::_right)>()
        >
{};

struct BuildNextStates
   : proto::or_<
        proto::when<
                    proto::terminal<state_tag>,
                    proto::_
        >,
        proto::when <
                    proto::comma<proto::terminal<state_tag>,proto::terminal<state_tag> >,
                    ::boost::mpl::push_back<
                        make_vector_one_row<proto::_left>(),
                        proto::_right>()
		>,
        proto::when <
                    proto::comma<BuildNextStates,proto::terminal<state_tag> >,
                    ::boost::mpl::push_back<
                                BuildNextStates(proto::_left),
						        proto::_right >()                
		>
   >
{};

struct BuildTargetPlusGuard
    : proto::when<
            proto::subscript<BuildNextStates,BuildGuards >,
            Row<none,none,BuildNextStates(proto::_left),none,BuildGuards(proto::_right)>()
        >
{};


struct BuildRight
    : proto::or_<
        // after == if just state without guard/action
         proto::when<
                BuildNextStates,
                Row<none,none,BuildNextStates(proto::_),none>() >
        // == target / action
       , proto::when<
                proto::divides<BuildNextStates,BuildActionSequence >,
                Row<none,none,BuildNextStates(proto::_left),
					BuildActionSequence(proto::_right) >() >
        // == target [ guard ]
       , proto::when<
                proto::subscript<BuildNextStates,BuildGuards >,
                Row<none,none,BuildNextStates(proto::_left),none,BuildGuards(proto::_right)>() >
        // == target [ guard ] / action 
       , proto::when<
                proto::divides<BuildTargetPlusGuard,
                               BuildActionSequence >,
                fusion_target_action_guard<BuildTargetPlusGuard(proto::_left),
                                           Row<none,none,none,BuildActionSequence(proto::_right)>()
                                           >() 
                >
        >
{};

struct BuildRow
    : proto::or_<     
        proto::when<
            proto::equal_to<BuildSourcePlusEvent,BuildRight >,
            fusion_left_right<BuildSourcePlusEvent(proto::_left),BuildRight(proto::_right)>()
        >
    >
{};

// stt grammar
struct BuildStt
   : proto::or_<
        proto::when<
                    proto::comma<BuildStt,BuildStt>,
                    boost::mpl::push_back<BuildStt(proto::_left),BuildRow(proto::_right)>()
                >,
        proto::when <
                    BuildRow,
                    make_vector_one_row<BuildRow(proto::_)>()
        >
   >
{};


template <class Expr>
typename ::boost::mpl::eval_if<
    typename proto::matches<Expr,BuildStt>::type,
    boost::result_of<BuildStt(Expr)>,
    make_invalid_type>::type
build_stt(Expr const& expr)
{
    return typename boost::result_of<BuildStt(Expr)>::type();
}


}}}}
#endif //BOOST_MSM_FRONT_EUML_STT_GRAMMAR_H
