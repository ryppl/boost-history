
#ifndef BOOST_MPL_SORT_HPP_INCLUDED
#define BOOST_MPL_SORT_HPP_INCLUDED

// Copyright (c) Eric Friedman 2002-2003
//
// Use, modification and distribution are subject to the Boost Software 
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy 
// at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source$
// $Date$
// $Revision$

#include <boost/mpl/sort_fwd.hpp>
#include <boost/mpl/sequence_tag.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/aux_/sort_impl.hpp>
#include <boost/mpl/aux_/na_spec.hpp>
#include <boost/mpl/aux_/lambda_support.hpp>

namespace boost { namespace mpl {

BOOST_MPL_AUX_AGLORITHM_NAMESPACE_BEGIN

template < typename Sequence, typename Predicate >  // see sort_fwd.hpp
struct sort
    : sort_impl< typename sequence_tag<Sequence>::type >
        ::template apply<Sequence, typename lambda<Predicate>::type >
{
    BOOST_MPL_AUX_LAMBDA_SUPPORT(2,sort,(Sequence,Predicate))
};

BOOST_MPL_AUX_AGLORITHM_NAMESPACE_END

}}

#endif // BOOST_MPL_SORT_HPP_INCLUDED
