
#ifndef BOOST_MPL_SORT_FWD_HPP_INCLUDED
#define BOOST_MPL_SORT_FWD_HPP_INCLUDED

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

#include <boost/mpl/less.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/aux_/na_spec.hpp>

namespace boost {
namespace mpl {

template< typename Tag > struct sort_impl;

BOOST_MPL_AUX_AGLORITHM_NAMESPACE_BEGIN

template <
      typename BOOST_MPL_AUX_NA_PARAM(Sequence)
    , typename Predicate = less<_,_>
    >
struct sort;

BOOST_MPL_AUX_AGLORITHM_NAMESPACE_END

BOOST_MPL_AUX_NA_ALGORITHM_SPEC(1, sort)

} // namespace mpl
} // namespace boost

#endif // BOOST_MPL_SORT_FWD_HPP_INCLUDED
