#ifndef BOOST_MPL_ITER_FOLD_HPP_VARIADIC_TEMPLATES
#define BOOST_MPL_ITER_FOLD_HPP_VARIADIC_TEMPLATES
#include <boost/mpl/fold_assoc_iter.hpp>
#include <boost/mpl/begin_end.hpp>

namespace boost { namespace mpl {

  template
  < typename Sequence
  , typename State
  , typename OpStateIter
  >
  struct
iter_fold
: fold_assoc_iter
  < assoc_left
  , OpStateIter
  , State
  , typename begin<Sequence>::type
  , typename end<Sequence>::type
  >
{
};

}}//exit boost::mpl namespace

#endif //include guard
