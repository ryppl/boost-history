//-----------------------------------------------------------------------------
// boost mpl/list/aux_/pop_front.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2000-02
// Aleksey Gurtovoy
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MPL_LIST_AUX_POP_FRONT_HPP_INCLUDED
#define BOOST_MPL_LIST_AUX_POP_FRONT_HPP_INCLUDED

#include "boost/mpl/pop_front_fwd.hpp"
#include "boost/mpl/list/aux_/tag.hpp"

namespace boost {
namespace mpl {

template<>
struct pop_front_traits< aux::list_tag >
{
    template< typename List > struct algorithm
    {
        typedef typename List::next type;
    };
};

} // namespace mpl
} // namespace boost

#endif // BOOST_MPL_LIST_AUX_POP_FRONT_HPP_INCLUDED
