
#ifndef BOOST_MPL_AUX_PUSH_BACK_IMPL_HPP_INCLUDED
#define BOOST_MPL_AUX_PUSH_BACK_IMPL_HPP_INCLUDED

// + file: boost/mpl/aux_/push_back_impl.hpp
// + last modified: 10/jun/03

// Copyright (c) 2000-03
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

#include "boost/mpl/push_back_fwd.hpp"
#include "boost/mpl/aux_/has_type.hpp"
#include "boost/mpl/aux_/void_spec.hpp"

namespace boost {
namespace mpl {

// no default implementation; the definition is needed to make happy the deficient
// compilers

template< typename Tag >
struct push_back_impl
{
    template< typename Seq, typename T > struct apply;
};


template< typename Tag >
struct has_push_back_impl
{
    // temporary implementation; ideally, should be able to check without causing
    // 'push_back_impl<Tag>::apply' instantiation
    template< typename Seq > struct apply
        : aux::has_type< typename push_back_impl<Tag>::template apply<Seq,int> >
    {
    };
};

BOOST_MPL_AUX_IMPL_SPEC(2, push_back_impl)
BOOST_MPL_AUX_IMPL_SPEC(1, has_push_back_impl)

} // namespace mpl
} // namespace boost

#endif // BOOST_MPL_AUX_PUSH_BACK_IMPL_HPP_INCLUDED
