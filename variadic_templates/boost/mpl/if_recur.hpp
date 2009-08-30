#ifndef BOOST_MPL_IF_RECUR_HPP_INCLUDED
#define BOOST_MPL_IF_RECUR_HPP_INCLUDED
// Copyright Larry Evans 2009
//

// $Id$
// $Date: 2009/04/25 03:10:01 $
// $Revision: 1.1 $

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/arg.hpp>
#include <boost/mpl/always.hpp>

namespace boost
{
namespace mpl
{
namespace aux
{
namespace if_recur
{
      template
      < class F
      , class A1
      , class A2NullaryMetaFun
      >
    struct apply2_lazy2
    {
            typedef
          typename apply
          < F
          , A1
          , typename A2NullaryMetaFun::type
          >::type
        type
        ;
    };
}//exit if_recur namespace    
}//exit aux namespace    
    
    //Notation:
    //  MFC_or_LE = Metafunction class or Lambda Expression
    //  (See Description on libs/mpl/doc/refmanual/apply.html)
    //
    //  X -> Y = metafunction from "Kind" X to "Kind" Y.
    //
    //  State = some Kind of "state"
    //Purpose:
    //  Almost the same as section 12.5 equation 1) of:
    //    http://www.thocp.net/biographies/papers/backus_turingaward_lecture.pdf
    //  The correspondences between this and that equaton is:
    //
    //    this              equation 1)
    //    ----              -----------
    //    if_recur          f
    //    mpl::not_<Recur>  p
    //    ThenDown          j
    //    NowUp             h
    //    mpl::arg<1>       i
    //    Else              g
    //
    
      template
      < class StateNow//nullary metafunction of Kind: -> State.
      , class Recur=always<false_>//MFC_or_LE of Kind: State -> bool_.
      , class ThenDown=arg<1>//MFC_or_LE of Kind: State -> State.
      , class NowUp=arg<2>//MFC_or_LE of Kind: (State,State) -> State.
      , class Else=arg<1>//MFC_or_LE of Kind: State -> State.
      >
    struct if_recur
    {
            typedef
          typename StateNow::type
        state_now
        ;
            typedef        
          typename eval_if
          < typename apply<Recur,state_now>::type
          , aux::if_recur::apply2_lazy2
            < NowUp
            , state_now
            , if_recur
              < apply<ThenDown,state_now>
              , Recur
              , ThenDown
              , NowUp
              , Else
              >
            >
          , apply<Else,state_now>
          >::type
        type
        ;
    };
    
}//exit mpl namespace
}//exit boost namespace
#endif
