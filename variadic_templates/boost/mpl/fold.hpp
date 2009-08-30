#ifndef BOOST_MPL_FOLD_HPP_VARIADIC_TEMPLATES
#define BOOST_MPL_FOLD_HPP_VARIADIC_TEMPLATES
#include <boost/mpl/foldl_iter_if.hpp>
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
fold
{
        typedef
      typename foldl_iter_if
      < typename begin<Sequence>::type
      , typename end<Sequence>::type
      , State
      , typename lambda<OpStateVal>::type::template
        apply
        < arg<1>
        , deref<arg<2> >
        > //Changes OpStateVal to OpStateIter: (State,Iterator) -> State
          //where Iterator is the type of iterator over Value's.
      >::type::fstate
    type
    ;
};

}}//exit boost::mpl namespace

#endif //include guard
