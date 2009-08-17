/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_LIST_DETAIL_CONS_BUILD_CONS_HPP
#define BOOST_FUSION_CONTAINER_LIST_DETAIL_CONS_BUILD_CONS_HPP

#include <boost/fusion/iterator/equal_to.hpp>
#include <boost/fusion/iterator/next.hpp>
#include <boost/fusion/iterator/value_of.hpp>
#include <boost/fusion/iterator/deref.hpp>

namespace boost { namespace fusion { namespace detail
{
    template <
        typename First
      , typename Last
      , bool is_empty = result_of::equal_to<First, Last>::value>
    struct build_cons;

    template <typename First, typename Last>
    struct build_cons<First, Last, true>
    {
        typedef nil type;

        static nil
        call(First const&, Last const&)
        {
            return nil();
        }
    };

    template <typename First, typename Last>
    struct build_cons<First, Last, false>
    {
        typedef
            build_cons<typename result_of::next<First>::type, Last>
        next_build_cons;

        typedef
            cons<
                typename result_of::value_of<First>::type
              , typename next_build_cons::type
            >
        type;

        static type
        call(First const& f, Last const& l)
        {
            return type(
                    fusion::deref(f),
                    next_build_cons::call(fusion::next(f), l));
        }
    };

}}}

#endif
