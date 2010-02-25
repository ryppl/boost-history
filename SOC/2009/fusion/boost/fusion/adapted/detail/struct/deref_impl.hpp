/*=============================================================================
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_STRUCT_DEREF_IMPL_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_STRUCT_DEREF_IMPL_HPP

namespace boost { namespace fusion { namespace extension
{
    template <typename>
    struct deref_impl;

    template <>
    struct deref_impl<struct_iterator_tag>
    {
        template <typename It>
        struct apply
        {
           typedef typename detail::remove_reference<It>::type it;
           typedef typename
               extension::struct_member<
                   typename detail::identity<typename it::seq_type>::type
                 , it::index::value
               >::template apply<typename it::seq_type>
           impl;

           typedef typename impl::type type;

           static
           type
           call(It it)
           {
               return impl::call(*it.seq);
           }
        };
    };
}}}

#endif
