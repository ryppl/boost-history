//-----------------------------------------------------------------------------
// boost mpl/front.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2000-01
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_FRONT_HPP
#define BOOST_MPL_FRONT_HPP

#include "boost/mpl/begin_end.hpp"
#include "boost/mpl/sequence_traits.hpp"

namespace boost {
namespace mpl {

namespace detail {
// workaround of MSVC early template instantiation bug
struct front_algorithm_traits_base
{
    template<class Sequence> struct algorithm
    {
        typedef typename mpl::begin<Sequence>::iterator type;
    };
};

} // front_algorithm_traits_base

template<class SequenceTag>
struct front_algorithm_traits
    : mpl::detail::front_algorithm_traits_base
{
};

template<class Sequence>
struct front
    : front_algorithm_traits< 
          typename mpl::sequence_traits<Sequence>::sequence_category
          >::template algorithm<Sequence>::type
{
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_FRONT_HPP
