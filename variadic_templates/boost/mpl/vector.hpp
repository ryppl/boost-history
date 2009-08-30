//variadic template version of vector.hpp
#ifndef BOOST_MPL_VECTOR_HPP_VARIADIC_TEMPLATES
#define BOOST_MPL_VECTOR_HPP_VARIADIC_TEMPLATES
//!
//!@nv-mpl_diff:
//!  The following include 
//!  indirectly #includes vector_fwd.hpp.
//!
#include <boost/mpl/vector/vector0.hpp>

namespace boost
{
namespace mpl
{

  template
  < typename Head
  , typename... Tail
  >
  struct
vector
  < Head
  , Tail...
  >
  : v_item
    < Head
    , vector<Tail...>
    >
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
//!    Instead of adding items from the tail, items are added from
//!    head.
//!  WHY:
//!    The variadic template compiler doesn't allow parameter
//!    packs to be followed by anything else (designated here as the
//!    [PACKS@END_CONSTRAINT]).  IOW:
//!      vector<typename Head..., typename Tail>
//!    is not allowed because paramerter pack, Head..., is followed
//!    by non-pack Tail.
//!
{
    typedef vector type;
};

}//exit mpl namespace
}//exit boost namespace
#endif
