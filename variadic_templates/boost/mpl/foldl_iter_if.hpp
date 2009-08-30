#ifndef BOOST_MPL_FOLDL_ITER_IF_HPP_VARIADIC_TEMPLATES
#define BOOST_MPL_FOLDL_ITER_IF_HPP_VARIADIC_TEMPLATES
#include <boost/mpl/next.hpp>
#include <boost/mpl/if_recur.hpp>
#include <boost/mpl/aux_/if_recur_fold.hpp>
namespace boost { namespace mpl {

  template
  < typename IterNow //start ValueIterator.
  , typename IterEnd //end ValueIterator.
  , typename State0 //initial State
  , typename OpStateIterState //Operator: (State,ValueIterator) -> State
  , typename OpIterBool=always<true_> //Operator: ValueIterator -> mpl::bool_
  >
  struct
foldl_iter_if
/**@brief
 *  Starting from State0, apply OpStateIterState for each iterator instance
 *  between IterNow and just before IterLast (expressed as `[IterNow...IterLast)`)
 *  where IterLast is the 1st iterator in [IterNow...IterEnd) that fails
 *  the OpIterBool test.
 *
 *  Associate the applications to the left.
 *
 *  For example, the analogous operation on run-time-value's is:
 *
 *    Assuming:
 *       z == the run-time-value analogue of State0.
 *       xI == the analogue of deref of the I-th iterator in IterNow...IterEnd.
 *       F == the analogue of OpStateIterState but where 2nd arg is 
 *            a Value instead of Value iterator.
 *       3 == distance<IterNow,IterEnd>::value
 *    Then the analogue of result would be:
 *       F(F(F(z,x1),x2),x3)
 *
 *  This template is similar to the haskell foldl described on p. 116 of:
 *
 *    http://haskell.org/definition/haskell98-report.pdf
 *
 *  where:
 *     haskell these_comments
 *     ------- --------------
 *     f       F
 *     a       State
 *     b       Value
 *     z       initial State
 */
: if_recur
  < typename aux::if_recur_fold::make_state_iter
    < State0
    , IterNow
    >::type
  , aux::if_recur_fold::not_iter_last_pred
    < arg<1>
    , IterEnd
    , typename lambda<OpIterBool>::type
    >
  , aux::if_recur_fold::op_state_general
    < arg<1> 
    , typename lambda<OpStateIterState>::type
    >
  , arg<2>
  , arg<1>
  >
{  
};

}}//exit boost::mpl namespace

#endif //include guard
