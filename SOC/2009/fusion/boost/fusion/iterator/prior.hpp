/*==============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ITERATOR_PRIOR_HPP
#define BOOST_FUSION_ITERATOR_PRIOR_HPP

#include <boost/fusion/support/internal/base.hpp>
#include <boost/fusion/support/tag_of.hpp>

namespace boost { namespace fusion
{
    namespace extension
    {
        template<typename>
        struct prior_impl;
    }

    namespace result_of
    {
        template<typename It>
        struct prior
          : extension::prior_impl<typename traits::tag_of<It>::type>::
                template apply<It>
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_iterator<It>))
        };
    }

    template<typename It>
    typename result_of::prior<It const&>::type
    prior(It const& it)
    {
        return result_of::prior<It const&>::call(it);
    }
}}

#endif
