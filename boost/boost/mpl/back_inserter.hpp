
#ifndef BOOST_MPL_BACK_INSERTER_HPP_INCLUDED
#define BOOST_MPL_BACK_INSERTER_HPP_INCLUDED

// + file: boost/mpl/back_inserter.hpp
// + last modified: 10/jun/03

// Copyright (c) 2002-03
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.
//
// See http://www.boost.org/libs/mpl for documentation.

#include "boost/mpl/push_back.hpp"
#include "boost/mpl/pair.hpp"
#include "boost/mpl/aux_/void_spec.hpp"
#include "boost/mpl/aux_/lambda_support.hpp"

namespace boost {
namespace mpl {

template<
      typename BOOST_MPL_AUX_VOID_SPEC_PARAM(Seq)
    >
struct back_inserter
    : pair< Seq, push_back<> >
{
    BOOST_MPL_AUX_LAMBDA_SUPPORT(1,back_inserter,(Seq))
};

BOOST_MPL_AUX_VOID_SPEC(1, back_inserter)

} // namespace mpl
} // namespace boost

#endif // BOOST_MPL_BACK_INSERTER_HPP_INCLUDED
