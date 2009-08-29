/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_GENERATION_DETAIL_VARIADIC_TEMPLATES_MAP_TIE_HPP
#define BOOST_FUSION_CONTAINER_GENERATION_DETAIL_VARIADIC_TEMPLATES_MAP_TIE_HPP

#include <boost/fusion/container/map/map.hpp>
#include <boost/fusion/support/deduce.hpp>
#include <boost/fusion/support/internal/variadic_templates/variadic_arguments_to_vector.hpp>
#include <boost/fusion/support/internal/variadic_templates/variadic_quote.hpp>

#include <boost/mpl/int.hpp>
#include <boost/mpl/divides.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/advance.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/unpack_args.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/mpl/zip_view.hpp>
#include <boost/mpl/transform_view.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template <typename... KeysTypes>
        struct map_tie
        {
            typedef typename
                detail::variadic_arguments_to_vector<KeysTypes...>::type
            args;
            typedef typename
                mpl::iterator_range<
                    typename mpl::begin<args>::type
                  , typename mpl::advance<
                        typename mpl::begin<args>::type
                      , mpl::divides<mpl::size<args>, mpl::int_<2> >
                    >::type
                >
            keys;
            typedef typename
                mpl::iterator_range<
                    typename mpl::advance<
                        typename mpl::begin<args>::type
                      , mpl::divides<mpl::size<args>, mpl::int_<2> >
                    >::type
                  , typename mpl::end<args>::type
                >
            types;

            typedef
                mpl::transform_view<
                    mpl::zip_view<mpl::vector2<keys,types> >
                  , mpl::unpack_args<
                        pair<mpl::_1, detail::deduce_ref<mpl::_2> >
                    >
                >
            map_args;

            typedef typename
                mpl::apply1<
                    mpl::unpack_args<detail::variadic_quote<map> >
                  , map_args
                >::type
            type;
        };
    }

    //TODO cschmidt: ?!
    template <typename... Keys, typename... Types>
    inline typename
        result_of::map_tie<Keys...,BOOST_FUSION_R_ELSE_CLREF(Types)...>::type
    map_tie(BOOST_FUSION_R_ELSE_CLREF(Types)... types)
    {
        typedef typename
            result_of::map_tie<Keys...,BOOST_FUSION_R_ELSE_CLREF(Types)...>::type
        type;
        return type(BOOST_FUSION_FORWARD(Types, types)...);
    }
}}

#endif
