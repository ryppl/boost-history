/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_MPL_MPL_ITERATOR_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_MPL_MPL_ITERATOR_HPP

#include <boost/fusion/iterator/iterator_facade.hpp>
#include <boost/fusion/support/category_of.hpp>

//TODO assoc iterator!

namespace boost { namespace fusion
{
    struct mpl_iterator_tag;
    struct mpl_sequence_tag;

    namespace extension
    {
        template <typename Tag>
        struct value_of_impl;

        template <typename Tag>
        struct equal_to_impl;

        template <typename Tag>
        struct deref_impl;

        template <typename Tag>
        struct next_impl;

        template <typename Tag>
        struct prior_impl;

        template <typename Tag>
        struct advance_impl;

        template <typename Tag>
        struct distance_impl;
    }

    namespace detail
    {
        template<typename>
        struct get_mpl_iterator_tag
        {
            typedef mpl_iterator_tag type;
        };
    }

    template <typename It>
    struct mpl_iterator
      : iterator_facade<
            mpl_iterator<It>
          , typename traits::category_of<It>::type
        >
    {
        //cschmidt: void typedef to enable fast SFINAE in get_mpl_it.hpp!
        typedef void void_;
        typedef It it_type;

        template <typename ItRef>
        struct value_of
          : extension::value_of_impl<
                typename detail::get_mpl_iterator_tag<ItRef>::type
            >::template apply<It&>
        {};

        template <typename It1Ref, typename It2Ref>
        struct equal_to
          : extension::equal_to_impl<
                typename detail::get_mpl_iterator_tag<It1Ref>::type
            >:: template apply<It&,It2Ref>
        {};

        template <typename ItRef>
        struct deref
          : extension::deref_impl<
                typename detail::get_mpl_iterator_tag<ItRef>::type
            >::template apply<It&>
        {};

        template <typename ItRef>
        struct next
          : extension::next_impl<
                typename detail::get_mpl_iterator_tag<ItRef>::type
            >::template apply<It&>
        {};

        template <typename ItRef>
        struct prior
          : extension::prior_impl<
                typename detail::get_mpl_iterator_tag<ItRef>::type
            >::template apply<It&>
        {};

        template <typename ItRef, typename N>
        struct advance
          : extension::advance_impl<
                typename detail::get_mpl_iterator_tag<ItRef>::type
            >::template apply<It&,N>
        {};

        template <typename It1Ref, typename It2Ref>
        struct distance
          : extension::distance_impl<
                typename detail::get_mpl_iterator_tag<It1Ref>::type
            >::template apply<It&,It2Ref>
        {};
    };
}}

#endif
