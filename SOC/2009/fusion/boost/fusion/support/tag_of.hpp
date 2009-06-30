/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SUPPORT_TAG_OF_HPP
#define BOOST_FUSION_SUPPORT_TAG_OF_HPP

#include <boost/fusion/support/tag_of_fwd.hpp>
#include <boost/fusion/support/detail/is_mpl_sequence.hpp>
#include <boost/fusion/support/ref.hpp>

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/if.hpp>

namespace boost { namespace fusion
{
    struct mpl_sequence_tag;
    struct mpl_iterator_tag;

    namespace detail
    {
        BOOST_MPL_HAS_XXX_TRAIT_DEF(fusion_tag)

        template <typename Seq>
        struct get_fusion_tag
        {
            typedef typename Seq::fusion_tag type;
        };
    }

    namespace traits
    {
        template <typename Seq>
        struct tag_of
        {
            typedef typename detail::identity<Seq>::type seq;

            typedef typename
                mpl::eval_if<
                    detail::has_fusion_tag<seq>
                  , detail::get_fusion_tag<seq>
                  , mpl::if_<
                        //???
                        detail::is_mpl_sequence<seq>
                      , mpl_sequence_tag
                      , mpl_iterator_tag
                    >
               >::type
            type;
        };
    }
}}
#endif
