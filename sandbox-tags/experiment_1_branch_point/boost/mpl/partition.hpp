//-----------------------------------------------------------------------------
// boost mpl/partition.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2002
// Eric Friedman
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_PARTITION_HPP_INCLUDED
#define BOOST_MPL_PARTITION_HPP_INCLUDED

#include "boost/mpl/bind.hpp"
#include "boost/mpl/lambda.hpp"
#include "boost/mpl/logical/not.hpp"
#include "boost/mpl/pair.hpp"
#include "boost/mpl/remove_if.hpp"
#include "boost/mpl/sequence_tag.hpp"
#include "boost/mpl/aux_/void_spec.hpp"
#include "boost/mpl/aux_/lambda_support.hpp"

namespace boost {
namespace mpl {

template <
      typename BOOST_MPL_AUX_VOID_SPEC_PARAM(Sequence)
    , typename BOOST_MPL_AUX_VOID_SPEC_PARAM(Predicate)
    >
struct partition
{
private:
    typedef typename lambda<Predicate>::type pred_;

    typedef typename remove_if<
          Sequence
        , bind1< logical_not<>, pred_ >
        >::type first_part;
    typedef typename remove_if<
          Sequence
        , pred_
        >::type second_part;

public:
    typedef pair<first_part, second_part>
        type;

    BOOST_MPL_AUX_LAMBDA_SUPPORT(2,partition,(Sequence,Predicate))
};

BOOST_MPL_AUX_VOID_SPEC(2, partition)

} // namespace mpl
} // namespace boost

#endif // BOOST_MPL_PARTITION_HPP_INCLUDED
