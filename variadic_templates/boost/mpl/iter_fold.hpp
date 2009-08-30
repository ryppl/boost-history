#ifndef BOOST_MPL_ITER_FOLD_HPP_VARIADIC_TEMPLATES
#define BOOST_MPL_ITER_FOLD_HPP_VARIADIC_TEMPLATES
#include <boost/mpl/foldl_iter.hpp>
#include <boost/mpl/begin_end.hpp>

namespace boost { namespace mpl {

  template
  < typename Sequence
  , typename State
  , typename StateIterOp
  >
  struct
iter_fold
: foldl_iter
  < typename begin<Sequence>::type
  , typename end<Sequence>::type
  , State
  , StateIterOp
  >
{
};

}}//exit boost::mpl namespace

#endif //include guard
