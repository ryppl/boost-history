/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef BOOST_FUSION_ADAPTED_DETAIL_MPL_TAG_OF_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_MPL_TAG_OF_HPP

#include <boost/fusion/support/tag_of.hpp>

#include <boost/mpl/apply.hpp>
#include <boost/mpl/iterator_tags.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/always.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost
{
    namespace fusion
    {
        struct mpl_sequence_tag;
        struct mpl_iterator_tag;
        struct non_fusion_tag;

        namespace detail
        {
            BOOST_MPL_HAS_XXX_TRAIT_DEF(category);

            template<typename T, typename Category>
            struct category_is
              : mpl::equal_to<typename T::category, Category>
            {};

            template <typename T, typename Enable>
            struct tag_of_fallback;

            template<typename T>
            struct tag_of_fallback<
                T
              , typename enable_if<
                    typename mpl::apply1<mpl::always<mpl::true_>,T>::type
                >::type
            >
            {
                typedef typename detail::identity<T>::type identity_t;

                typedef typename
                    mpl::eval_if<
                        mpl::is_sequence<identity_t>
                      , mpl::identity<mpl_sequence_tag>
                      , mpl::eval_if<
                            mpl::and_<
                                detail::has_category<identity_t>
                              , mpl::or_<
                                    detail::category_is<
                                        identity_t
                                      , mpl::forward_iterator_tag
                                    >
                                  , detail::category_is<
                                        identity_t
                                      , mpl::bidirectional_iterator_tag
                                    >
                                  , detail::category_is<
                                        identity_t
                                      , mpl::random_access_iterator_tag
                                    >
                                >
                            >
                          , mpl::identity<mpl_iterator_tag>
                          , mpl::identity<non_fusion_tag>
                        >
                    >::type
                type;
            };
        }
    }
}

#endif
