//-----------------------------------------------------------------------------
// boost mpl/sort.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2002-2003
// Eric Friedman
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_SORT_HPP_INCLUDED
#define BOOST_MPL_SORT_HPP_INCLUDED

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
