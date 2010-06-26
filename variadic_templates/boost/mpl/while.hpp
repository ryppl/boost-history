#ifndef BOOST_MPL_WHILE_HPP_INCLUDED
#define BOOST_MPL_WHILE_HPP_INCLUDED
// Copyright Larry Evans 2009
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
#include <boost/mpl/eval_if.hpp>

namespace boost
{
namespace mpl
{
      template
      < class State //nullary metafunction returning current state.
      , class IfOps //contains nested unary metafunctions, if_, then_.
      >
    struct while_
    : eval_if
      < typename IfOps::template if_<typename State::type>::type
      , while_
        < typename IfOps::template then_<typename State::type>
        , IfOps
        >
      , State
      >
    {
    };
    
}//exit mpl namespace
}//exit boost namespace
#endif
