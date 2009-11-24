#ifndef BOOST_MPL_FOLD_ASSOC_HPP_VARIADIC_TEMPLATES
#define BOOST_MPL_FOLD_ASSOC_HPP_VARIADIC_TEMPLATES
#include <boost/mpl/fold_assoc_iter.hpp>
#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/lambda.hpp>

namespace boost { namespace mpl {

  template
  < op_assoc OpAssoc
  , typename OpBinary
  , typename State
  , typename Sequence
  >
  struct
fold_assoc_seq
;
  template
  < typename OpBinary
  , typename State
  , typename Sequence
  >
  struct
fold_assoc_seq
  < assoc_left
  , OpBinary
  , State
  , Sequence
  >
: fold_assoc_iter
  < assoc_left
  , typename lambda<OpBinary>::type::template
    apply
    < arg<1>
    , deref<arg<2> >
    > //Changes OpStateVal to OpStateIter: (State,Iterator) -> State
      //where Iterator is the type of iterator over Sequence.
  , State
  , typename begin<Sequence>::type
  , typename end<Sequence>::type
  >
{
};
  template
  < typename OpBinary
  , typename State
  , typename Sequence
  >
  struct
fold_assoc_seq
  < assoc_right
  , OpBinary
  , State
  , Sequence
  >
: fold_assoc_iter
  < assoc_right
  , typename lambda<OpBinary>::type::template
    apply
    < deref<arg<1> >
    , arg<2>
    > //Changes OpValState to OpIterState: (Iterator,State) -> State
      //where Iterator is the type of iterator over Sequence.
  , State
  , typename begin<Sequence>::type
  , typename end<Sequence>::type
  >
{
};

}}//exit boost::mpl namespace

#endif //include guard
