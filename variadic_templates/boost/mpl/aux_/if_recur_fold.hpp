#ifndef BOOST_MPL_AUX_IF_RECUR_FOLD_HPP_INCLUDED
#define BOOST_MPL_AUX_IF_RECUR_FOLD_HPP_INCLUDED
#include <boost/mpl/and.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/push_front.hpp>
#include <boost/mpl/apply.hpp>
namespace boost{namespace mpl{namespace aux{
namespace if_recur_fold
/**@brief
 *  Provide helper templates to adapt if_recur template
 *  for use in folding a binary operator over elements
 *  in a sequence.
 */
{

  template
  < typename FoldState
  >
struct names_fstate
{
    typedef FoldState fstate;
    typedef names_fstate type;
};
  template
  < typename Stack
  , typename FoldState
  >
struct names_stack_fstate
: names_fstate<FoldState>
{
    typedef Stack stack;
    typedef names_stack_fstate type;
};
  template
  < typename State
  >
struct make_state
/**@brief
 *  This and specializations, assure
 *  the result type has nested
 *  'fstate' typedef.
 */
{
    typedef names_fstate<State> type;
};  
  template
  < typename State
  >
struct make_state
  < names_fstate<State>
  >
{
    typedef names_fstate<State> type;
};  
  template
  < typename Stack
  , typename State
  >
struct make_state
  < names_stack_fstate<Stack,State>
  >
{
    typedef names_stack_fstate<Stack,State> type;
};  
  template
  < typename State
  , typename Iter
  >
struct names_state_iter
: State //either names_fstate or names_stack_fstate
{
    typedef Iter iter;
    typedef names_state_iter type;
};
  template
  < typename State
  , typename Iter
  >
struct make_state_iter
{
        typedef
      typename make_state<State>::type
    named_state
    ;
        typedef 
      names_state_iter<named_state,Iter> 
    type
    ;
};
//-{--trivial metafunctions--
//  where "trivial metafunction" is defined here:
//    http://www.boost.org/doc/libs/1_38_0/libs/mpl/doc/refmanual/trivial-metafunction.html
//
  template
  < typename StateIter
  >
struct iter
{
    typedef typename StateIter::iter type;
};
  template
  < typename StateIter
  >
struct fstate
{
    typedef typename StateIter::fstate type;
};
//-}--trivial metafunctions--

  template
  < typename StateIter
  >
struct op_state_iter
/**@brief
 *  Returns state with incremented iter.
 */
{
        typedef
      mpl::aux::if_recur_fold::names_state_iter
      < mpl::aux::if_recur_fold::names_fstate<typename StateIter::fstate>
      , typename next<typename StateIter::iter>::type
      >
    type
    ;
};

  template
  < typename StateIter
  , typename OpStateIter
  >
struct op_next_fstate_iter
/**@brief
 *  typedef's {iter,fstate}_{now,next}
 *    where X_next is update of X_now, for X in {iter,fstate}.
 */
{
        typedef
      typename StateIter::iter
    iter_now
    ;
        typedef
      typename next<iter_now>::type
    iter_next
    ;
        typedef
      typename StateIter::fstate
    fstate_now
    ;
        typedef
      typename apply<OpStateIter,fstate_now,iter_now>::type
    fstate_next
    ;
};    
  template
  < typename StackStateIter
  , typename OpStateIter
  >
struct op_state_stack_fstate_iter
/**@brief
 *  Updates stack as well as fstate and iter.
 */
: op_next_fstate_iter
  < StackStateIter
  , OpStateIter
  >
{
        typedef
      op_next_fstate_iter
      < StackStateIter
      , OpStateIter
      >
    super_op
    ;
        typedef
      typename StackStateIter::stack
    stack_now
    ;
        typedef
      typename push_front
      < stack_now
      , typename deref<typename super_op::iter_now>::type
      >::type
    stack_next
    ;
        typedef
      names_state_iter
      < names_stack_fstate
        < stack_next
        , typename super_op::fstate_next
        >
      , typename super_op::iter_next
      >
    type
    ;
    
};

  template
  < typename StateIter
  , typename OpStateIter
  >
struct op_state_fstate_iter
: op_next_fstate_iter
  < StateIter
  , OpStateIter
  >
{
        typedef
      op_next_fstate_iter
      < StateIter
      , OpStateIter
      >
    super_op
    ;
        typedef
      names_state_iter
      < names_fstate
        < typename super_op::fstate_next
        >
      , typename super_op::iter_next
      >
    type
    ;
    
};

  template
  < typename State
  , typename StateIterOp
  >
struct op_state_general
;

  template
  < typename State
  , typename Iter
  , typename StateIterOp
  >
struct op_state_general
  < names_state_iter
    < names_fstate<State>
    , Iter
    >
  , StateIterOp
  >
: op_state_fstate_iter
  < names_state_iter
    < names_fstate<State>
    , Iter
    >
  , StateIterOp
  >
{  
};

  template
  < typename Stack
  , typename State
  , typename Iter
  , typename StateIterOp
  >
struct op_state_general
  < names_state_iter
    < names_stack_fstate<Stack,State>
    , Iter
    >
  , StateIterOp
  >
: op_state_stack_fstate_iter
  < names_state_iter
    < names_stack_fstate<Stack,State>
    , Iter
    >
  , StateIterOp
  >
{  
};

  template
  < typename StateNow
  , typename IterLast
  >
struct not_iter_last
: not_
  < is_same
    < typename StateNow::iter
    , IterLast
    >
  >
{
};

  template
  < typename F
  , typename A1NullaryMetaFun
  , typename A2
  >
struct apply2_lazy1
{
        typedef 
      typename apply
      < F
      , typename A1NullaryMetaFun::type
      , A2
      >::type 
    type
    ;
};
  
  template
  < typename StateNow
  , typename IterLast
  , typename Predicate=always<true_>
  >
struct not_iter_last_pred
: and_
  < not_iter_last<StateNow,IterLast>
  , apply2_lazy1
    < Predicate
    , deref<typename StateNow::iter>//lazy eval to avoid possible deref<IterLast>::type
    , typename StateNow::fstate
    >
  >
{
};

}//exit if_recur_fold namespace

}}}//exit boost::mpl::aux namespace
#endif
