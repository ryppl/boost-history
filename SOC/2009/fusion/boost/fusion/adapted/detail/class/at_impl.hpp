/*=============================================================================
    Copyright (c) 2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_CLASS_AT_IMPL_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_CLASS_AT_IMPL_HPP

#include <boost/fusion/support/internal/ref.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/remove_const.hpp>

namespace boost { namespace fusion { namespace extension
{
    template<typename>
    struct at_impl;

    template <>
    struct at_impl<class_tag>
    {
        template<typename Seq, typename N>
        struct apply
        {
            typedef typename detail::remove_reference<Seq>::type seq;
            typedef
                extension::struct_member<
                    typename remove_const<seq>::type
                  , N::value
                >
            element;

            typedef typename
                mpl::if_<
                    is_const<seq>
                  , typename element::get_type
                  , typename element::proxy
                >::type
            type;

            static type
            call(Seq seq)
            {
                return element::call(seq);
            }
        };
    };

    template <>
    struct at_impl<assoc_class_tag>
      : at_impl<class_tag>
    {};
}}}

#endif
