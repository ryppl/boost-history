/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_STD_PAIR_DETAIL_AT_IMPL_HPP
#define BOOST_FUSION_ADAPTED_STD_PAIR_DETAIL_AT_IMPL_HPP

#include <boost/fusion/support/detail/access.hpp>
#include <boost/fusion/support/assert.hpp>

#include <boost/mpl/if.hpp>
#include <boost/mpl/int.hpp>

namespace boost { namespace fusion {

    struct std_pair_tag;

    namespace extension
    {
        template<typename T>
        struct at_impl;

        template <>
        struct at_impl<std_pair_tag>
        {
            template <typename Sequence, typename N>
            struct apply
            {
                static int const n_value = N::value;
                BOOST_FUSION_INDEX_CHECK(n_value, 2);

                typedef typename
                    mpl::if_c<
                        (n_value == 0)
                      , typename Sequence::first_type
                      , typename Sequence::second_type
                    >
                element;

                typedef typename
                    mpl::eval_if<
                        is_const<Sequence>
                      , detail::cref_result<element>
                      , detail::ref_result<element>
                    >::type
                type;

                template <typename RT>
                static RT get(Sequence& p, mpl::int_<0>)
                {
                    return  p.first;
                }

                template <typename RT>
                static RT get(Sequence& p, mpl::int_<1>)
                {
                    return  p.second;
                }

                static type
                call(Sequence& p)
                {
                    return get<type>(p, N());
                }
            };
        };
    }
}}

#endif
