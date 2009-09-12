#ifndef BOOST_MPL_FOLDR_ITER_HPP_VARIADIC_TEMPLATES
#define BOOST_MPL_FOLDR_ITER_HPP_VARIADIC_TEMPLATES
#include <boost/mpl/next.hpp>
#include <boost/mpl/if_recur.hpp>
#include <boost/mpl/aux_/if_recur_fold.hpp>
namespace boost { namespace mpl {

  template
  < typename IterNow //start ValueIterator.
  , typename IterEnd //end ValueIterator.
  , typename State0 //initial State
  , typename OpIterState //Operator: (ValueIterator,State) -> State
  >
  struct
foldr_iter
/**@brief
 *  Same as foldl_iter except association is to the right,
 *  and Operator arguments are swapped.
 *  IOW, the analogue result (see comments to foldl_iter.hpp) is:
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
: apply
  < OpIterState
  , IterNow
  , typename foldr_iter
    < typename next<IterNow>::type
    , IterEnd
    , State0
    , OpIterState
    >::type
  >
{  
};

  template
  < typename IterEnd
  , typename State0
  , typename OpIterState
  >
  struct
foldr_iter
  < IterEnd
  , IterEnd
  , State0
  , OpIterState
  >
{
        typedef
      State0
    type
    ;    
};

}}//exit boost::mpl namespace

#endif //include guard
