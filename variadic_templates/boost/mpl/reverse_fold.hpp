#ifndef BOOST_MPL_REVERSE_FOLD_HPP_VARIADIC_TEMPLATES
#define BOOST_MPL_REVERSE_FOLD_HPP_VARIADIC_TEMPLATES
#include <boost/mpl/fold_assoc_iter.hpp>
#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/lambda.hpp>

namespace boost { namespace mpl {

  template
  < typename Sequence //sequence of Value's
  , typename State //initial State
  , typename OpStateVal //Operator: (State,Value) -> State
  >
  struct
reverse_fold
{
        typedef
      typename fold_assoc_iter
      < assoc_right
      , typename lambda<OpStateVal>::type::template
        apply
        < arg<2>
        , deref<arg<1> >
        > //Changes OpStateVal to OpIterState: (Iterator,State) -> State
          //where Iterator is the type of iterator over Value's.
      , State
      , typename begin<Sequence>::type
      , typename end<Sequence>::type
      >::type
    type
    ;
};

}}//exit boost::mpl namespace

#endif //include guard