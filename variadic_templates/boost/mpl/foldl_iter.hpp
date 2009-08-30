#ifndef BOOST_MPL_FOLD_ITER_HPP_VARIADIC_TEMPLATES
#define BOOST_MPL_FOLDL_ITER_HPP_VARIADIC_TEMPLATES
#include <boost/mpl/next.hpp>
#include <boost/mpl/apply.hpp>
namespace boost { namespace mpl {

  template
  < typename IterNow//start Iterator on Value's, i.e. deref<IterNow>::type is a "Value".
  , typename IterEnd//end ValueIterator.
  , typename State0 //intial State
  , typename OpStateIter//Operator: (State,ValueIterator) -> State
  >
  struct
foldl_iter
/**@brief
 *  Apply OpStateIter for each iterator instance between IterNow and just before
 *  InterEnd starting with intial State, State0.  Associate the applications
 *  to the left.
 *
 *  For example, the analogous operation on run-time-value's is:
 *
 *    Assuming:
 *       z == the run-time-value analogue of State0.
 *       xI == the analogue of deref of the I-th iterator in IterNow...IterEnd.
 *       F == the analogue of OpStateIter but where 2nd arg is a Value instead of Value iterator.
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
: foldl_iter
  < typename next<IterNow>::type
  , IterEnd
  , typename apply<OpStateIter,State0,IterNow>::type
  , OpStateIter
  >
{  
};

  template
  < typename IterEnd
  , typename State0
  , typename OpStateIter
  >
  struct
foldl_iter
  < IterEnd
  , IterEnd
  , State0
  , OpStateIter
  >
{
        typedef
      State0
    type
    ;    
};

}}//exit boost::mpl namespace

#endif //include guard
