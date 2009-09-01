//variadic template version of vector.hpp
#ifndef BOOST_MPL_VECTOR_HPP_VARIADIC_TEMPLATES
#define BOOST_MPL_VECTOR_HPP_VARIADIC_TEMPLATES
//!
//!@nv-mpl_diff:
//!  The following include 
//!  indirectly #includes vector_fwd.hpp.
//!
#include <boost/mpl/vector/vector0.hpp>
#include <boost/mpl/foldr_pkg.hpp>

namespace boost
{
namespace mpl
{

  template
  < typename... Values
  >
  struct
vector
  : foldr_pkg
    < vector0
    , v_item_fold<arg<1>,arg<2> >
    , Values...
    >::type
//!
//!@nv-mpl_repl:
//!  This specialization corresponds to all of the vectorI
//!  for I=1..., which are defined in non-variadic mpl in 
//!  one of the files:
//!    boost/mpl/vector/aux_/preprocessed/typeof_based/vectorN.hpp
//!  for some N in 10,20,...
//!
//!@nv-mpl_diff:
//!  WHAT:
//!    1) Instead of adding items from the tail, items are added from
//!       head.
//!    2) foldr_pkg is used instead of recursive call of vector.
//!  WHY:
//!    1) The variadic template compiler doesn't allow parameter
//!       packs to be followed by anything else (designated here as the
//!       [PACKS@END_CONSTRAINT]).  IOW:
//!         vector<typename Head..., typename Tail>
//!       is not allowed because paramerter pack, Head..., is followed
//!       by non-pack Tail.
//!    2) Try and generalize the way sequences are constructed.
//!
{
};

}//exit mpl namespace
}//exit boost namespace
#endif
