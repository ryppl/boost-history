//-----------------------------------------------------------------------------
// boost mpl/vector/aux_/empty.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2000-02
// Aleksey Gurtovoy
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MPL_VECTOR_AUX_EMPTY_HPP_INCLUDED
#define BOOST_MPL_VECTOR_AUX_EMPTY_HPP_INCLUDED

#include "boost/mpl/empty_fwd.hpp"
#include "boost/mpl/not.hpp"
#include "boost/mpl/bool.hpp"
#include "boost/mpl/vector/aux_/tag.hpp"
#include "boost/mpl/aux_/config/vector.hpp"
#include "boost/config.hpp"

namespace boost {
namespace mpl {

#if defined(BOOST_MPL_TYPEOF_BASED_VECTOR_IMPL)

template<>
struct empty_traits< aux::vector_tag >
{
    template< typename Vector > struct algorithm
        : not_< typename Vector::size >
    {
    };
};

#else

template<>
struct empty_traits< aux::vector_tag<0> >
{
    template< typename Vector > struct algorithm
        : true_
    {
    };
};

#if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

template< long N >
struct empty_traits< aux::vector_tag<N> >
{
    template< typename Vector > struct algorithm
        : false_
    {
    };
};

#endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#endif // BOOST_MPL_TYPEOF_BASED_VECTOR_IMPL

} // namespace mpl
} // namespace boost

#endif // BOOST_MPL_VECTOR_AUX_EMPTY_HPP_INCLUDED
