#ifndef BOOST_MPL_FOLDR_PKG_HPP_VARIADIC_TEMPLATES
#define BOOST_MPL_FOLDR_PKG_HPP_VARIADIC_TEMPLATES
#include <boost/mpl/apply.hpp>

namespace boost { namespace mpl {

  template
  < typename State0 //initial State
  , typename OpValueState_State //Operator: (Value,State) -> State
  , typename... Values
  >
  struct
foldr_pkg
/**@brief
 *  Apply OpValueState_State to each element in Values...
 *  starting with intial State, State0.  Associate the applications
 *  to the right.
 *
 *  For example, the analogous operation on run-time-value's is:
 *
 *    Assuming:
 *       z == the run-time-value analogue of State0.
 *       xI == the analogue of I-th element in Values...
 *       F == the analogue of OpValueState_State.
 *    Then the analogue of result would be:
 *      F(x1,F(x2,F(x3,z)))
 *
 *  This template is similar to the haskell foldr described on p. 117 of:
 *
 *    http://haskell.org/definition/haskell98-report.pdf
 *
 *  where:
 *     haskell these_comments
 *     ------- --------------
 *     f       F
 *     a       typename
 *     b       State
 *     z       initial State
 */
;
  template
  < typename State0 //initial State
  , typename OpValueState_State //Operator: (Value,State) -> State
  , typename Head
  , typename... Tail
  >
  struct
foldr_pkg
  < State0
  , OpValueState_State
  , Head
  , Tail...
  >
: apply
  < OpValueState_State
  , Head
  , typename foldr_pkg
    < State0
    , OpValueState_State
    , Tail...
    >::type
  >::type
{};
  template
  < typename State0 //initial State
  , typename OpValueState_State //Operator: (Value,State) -> State
  >
  struct
foldr_pkg
  < State0
  , OpValueState_State
  >
{
    typedef State0 type;
};

}}//exit boost::mpl namespace

#endif //include guard
