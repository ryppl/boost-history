/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

namespace boost { namespace fusion { namespace extension
{
    template <typename>
    struct value_of_impl;

    template <>
    struct value_of_impl<BOOST_FUSION_ADAPTED_TUPLE_ITERATOR_TAG>
    {
        template <typename It>
        struct apply
        {
            typedef typename detail::remove_reference<It>::type it;

            typedef typename
                BOOST_FUSION_ADAPTED_TUPLE_NAMESPACE(element)<
                    it::index::value
                  , typename detail::remove_reference<
                        typename it::seq_type
                    >::type
                 >::type
            type;
        };
    };
}}}
