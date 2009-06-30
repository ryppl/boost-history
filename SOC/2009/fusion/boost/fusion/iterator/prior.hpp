/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ITERATOR_PRIOR_HPP
#define BOOST_FUSION_ITERATOR_PRIOR_HPP

#include <boost/fusion/support/tag_of.hpp>

namespace boost { namespace fusion
{
    // Special tags:
    struct iterator_facade_tag; // iterator facade tag

    namespace extension
    {
        template <typename Tag>
        struct prior_impl;

        template <>
        struct prior_impl<iterator_facade_tag>
        {
            template <typename Iterator>
            struct apply
              : detail::remove_reference<Iterator>::type::template prior<Iterator>
            {};
        };
    }

    namespace result_of
    {
        template <typename Iterator>
        struct prior
          : extension::prior_impl<
                typename traits::tag_of<Iterator>::type
            >::template apply<typename detail::add_lref<Iterator>::type>
        {};
    }

    template <typename Iterator>
    typename result_of::prior<Iterator const&>::type const
    prior(Iterator const& i)
    {
        return result_of::prior<Iterator const&>::call(i);
    }
}}

#endif
