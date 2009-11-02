/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SUPPORT_TAG_OF_HPP
#define BOOST_FUSION_SUPPORT_TAG_OF_HPP

#include <boost/fusion/support/tag_of_fwd.hpp>
#include <boost/fusion/support/internal/ref.hpp>

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/has_xxx.hpp>

namespace boost { namespace fusion
{
    struct non_fusion_tag;

    namespace detail
    {
        BOOST_MPL_HAS_XXX_TRAIT_DEF(fusion_tag);

        template<typename IdentityT>
        struct get_fusion_tag
        {
            typedef typename IdentityT::fusion_tag type;
        };

        template <typename T, typename Enable=void>
        struct tag_of_fallback
        {
            typedef non_fusion_tag type;
        };
    }

    namespace traits
    {
        template <typename T, typename Enable>
        struct tag_of
        {
            typedef typename detail::identity<T>::type identity_t;

            typedef typename
                mpl::eval_if<
                    detail::has_fusion_tag<identity_t>
                  , detail::get_fusion_tag<identity_t>
                  , detail::tag_of_fallback<T>
                >::type
            type;
        };
    }
}}
#endif
