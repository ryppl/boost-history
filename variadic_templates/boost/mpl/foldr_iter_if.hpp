#ifndef BOOST_MPL_FOLDR_ITER_IF_HPP_VARIADIC_TEMPLATES
#define BOOST_MPL_FOLDR_ITER_IF_HPP_VARIADIC_TEMPLATES
#include <boost/mpl/next.hpp>
#include <boost/mpl/if_recur.hpp>
#include <boost/mpl/aux_/if_recur_fold.hpp>
namespace boost { namespace mpl {

  template
  < typename IterNow //start ValueIterator.
  , typename IterEnd //end ValueIterator.
  , typename State0 //initial State
  , typename OpIterStateState //Operator: (ValueIterator,State) -> State
  , typename OpIterBool=always<true_> //Operator: ValueIterator -> mpl::bool_
  >
  struct
foldr_iter_if
/**@brief
 *  Same as foldl_iter_if except association is to the right,
 *  and Operator arguments are swapped.
 *  IOW, the analogue result (see comments to foldl_iter_if.hpp) is:
 *    F(x1,F(x2,F(x3,z)))
 *
 *  This template is similar to the haskell foldr described on p. 117 of:
 *
 *    http://haskell.org/definition/haskell98-report.pdf
 *
 *  where:
 *     haskell these_comments
 *     ------- --------------
 *     f       F
 *     a       Value
 *     b       State
 *     z       initial State
 */
: if_recur
  < aux::if_recur_fold::names_state_iter
    < aux::if_recur_fold::names_fstate<State0>
    , IterNow
    >
  , aux::if_recur_fold::not_iter_last_pred
    < arg<1>
    , IterEnd
    , typename lambda<OpIterBool>::type
    >
  , aux::if_recur_fold::op_state_iter<arg<1> >
  , typename lambda<OpIterStateState>::type::template
    apply
    < aux::if_recur_fold::iter<arg<1> >
    , arg<2>
    >
  , aux::if_recur_fold::fstate<arg<1> >
  >
{  
};

}}//exit boost::mpl namespace

#endif //include guard
