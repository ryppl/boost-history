//-----------------------------------------------------------------------------
// boost mpl/aux_/clear_impl.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2000-02
// Aleksey Gurtovoy
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MPL_AUX_CLEAR_IMPL_HPP_INCLUDED
#define BOOST_MPL_AUX_CLEAR_IMPL_HPP_INCLUDED

#include "boost/mpl/clear_fwd.hpp"
#include "boost/mpl/aux_/traits_lambda_spec.hpp"
#include "boost/mpl/aux_/config/eti.hpp"

namespace boost {
namespace mpl {

// no default implementation; the definition is needed to make MSVC happy

template< typename Tag >
struct clear_traits
{
    template< typename Sequence > struct algorithm;
};

BOOST_MPL_ALGORITM_TRAITS_LAMBDA_SPEC(1,clear_traits)

} // namespace mpl
} // namespace boost

#endif // BOOST_MPL_AUX_CLEAR_IMPL_HPP_INCLUDED
