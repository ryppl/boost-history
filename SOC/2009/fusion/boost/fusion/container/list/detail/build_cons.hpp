/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_LIST_DETAIL_BUILD_CONS_HPP
#define BOOST_FUSION_CONTAINER_LIST_DETAIL_BUILD_CONS_HPP

#include <boost/fusion/iterator/equal_to.hpp>
#include <boost/fusion/iterator/next.hpp>
#include <boost/fusion/iterator/value_of.hpp>
#include <boost/fusion/iterator/deref.hpp>

namespace boost { namespace fusion { namespace detail
{
    template <
        typename Begin
      , typename End
      , bool is_empty = result_of::equal_to<Begin, End>::value
    >
    struct build_cons;

    template <typename Begin, typename End>
    struct build_cons<Begin, End, true>
    {
        typedef nil type;

        static nil
        call(Begin const&, End const&)
        {
            return nil();
        }
    };

    template <typename Begin, typename End>
    struct build_cons<Begin, End, false>
    {
        typedef
            build_cons<typename result_of::next<Begin>::type, End>
        next_build_cons;

        typedef
            cons<
                typename result_of::value_of<Begin>::type
              , typename next_build_cons::type
            >
        type;

        static type
        call(Begin const& f, End const& l)
        {
            return type(
                    fusion::deref(f),
                    next_build_cons::call(fusion::next(f), l));
        }
    };

}}}

#endif
