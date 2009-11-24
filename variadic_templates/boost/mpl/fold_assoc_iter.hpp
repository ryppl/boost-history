#ifndef BOOST_MPL_FOLD_ASSOC_ITER_HPP_VARIADIC_TEMPLATES
#define BOOST_MPL_FOLD_ASSOC_ITER_HPP_VARIADIC_TEMPLATES
#include <boost/mpl/next.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/op_assoc.hpp>
namespace boost { namespace mpl {

  template
  < op_assoc OpAssoc
  , typename OpBinary//Binary Operator
  , typename State0 //intial State
  , typename IterNow//start Iterator on Value's, i.e. deref<IterNow>::type is a "Value".
  , typename IterEnd//end ValueIterator.
  >
  struct
fold_assoc_iter
/**@brief
 *  The same as:
 *    fold_assoc_pack<OpAssoc,OpBinaryTmpl,State0,Values...>
 *    (See fold_assoc_pack.hpp)
 *  where:
 *    Values... is the sequences of values obtained from
 *      deref'ing iterators in range, [InterNow,IterEnd), in order.
 *    OpBinaryTmpl is lambda<OpBinary>::type::apply
 */
;

//Base Cases:
//  MAINTENANCE_NOTE:2009-11-12:L. Evans
//    It might seem there's no need for two specializations 
//    of the base case since they both do the same thing; however,
//    without two specializations, g++ complains:
//      error: ambiguous class template instantiation for 
//      'boost::mpl::fold_assoc_iter<(boost::mpl::op_assoc)0u,...'
//
  template
  < typename OpBinary
  , typename State0
  , typename IterEnd
  >
  struct
fold_assoc_iter
  < assoc_left
  , OpBinary
  , State0
  , IterEnd
  , IterEnd
  >
{
        typedef
      State0
    type
    ;    
};
  template
  < typename OpBinary
  , typename State0
  , typename IterEnd
  >
  struct
fold_assoc_iter
  < assoc_right
  , OpBinary
  , State0
  , IterEnd
  , IterEnd
  >
{
        typedef
      State0
    type
    ;    
};

//Induction Cases:

  template
  < typename OpStateIter_State//metafun:(State,Iter) -> State
  , typename State0 
  , typename IterNow
  , typename IterEnd
  >
  struct
fold_assoc_iter
  < assoc_left
  , OpStateIter_State
  , State0
  , IterNow
  , IterEnd
  >
: fold_assoc_iter
  < assoc_left
  , OpStateIter_State
  , typename apply
    < OpStateIter_State
    , State0
    , IterNow
    >::type
  , typename next<IterNow>::type
  , IterEnd
  >
{  
};

  template
  < typename OpIterState_State//metafun:(Iter,State) -> State
  , typename State0
  , typename IterNow
  , typename IterEnd
  >
  struct
fold_assoc_iter
  < assoc_right
  , OpIterState_State
  , State0
  , IterNow
  , IterEnd
  >
: apply
  < OpIterState_State
  , IterNow
  , typename fold_assoc_iter
    < assoc_right
    , OpIterState_State
    , State0
    , typename next<IterNow>::type
    , IterEnd
    >::type
  >
{  
};

}}//exit boost::mpl namespace

#endif //include guard
