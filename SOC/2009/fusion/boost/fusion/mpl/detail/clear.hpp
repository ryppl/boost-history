/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_MPL_DETAIL_CLEAR_HPP
#define BOOST_FUSION_MPL_DETAIL_CLEAR_HPP

#include <boost/fusion/container/vector/vector_fwd.hpp>
#include <boost/fusion/container/list/list_fwd.hpp>
#include <boost/fusion/container/map/map_fwd.hpp>
#include <boost/fusion/container/set/set_fwd.hpp>

namespace boost { namespace fusion
{
    struct cons_tag;
    struct map_tag;
    struct set_tag;
    struct vector_tag;

    namespace detail
    {
        template <typename Tag>
        struct clear;

        template <>
        struct clear<cons_tag>
          : mpl::identity<list<> >
        {};

        template <>
        struct clear<map_tag>
          : mpl::identity<map<> >
        {};

        template <>
        struct clear<set_tag>
          : mpl::identity<set<> >
        {};

        template <>
        struct clear<vector_tag>
          : mpl::identity<vector<> >
        {};
    }
}}

#endif
