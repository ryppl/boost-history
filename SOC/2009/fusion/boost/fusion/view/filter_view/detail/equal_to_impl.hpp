/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_FILTER_VIEW_DETAIL_EQUAL_TO_IMPL_HPP
#define BOOST_FUSION_VIEW_FILTER_VIEW_DETAIL_EQUAL_TO_IMPL_HPP

namespace boost { namespace fusion { namespace extension
{
    template<typename>
    struct equal_to_impl;

    template<>
    struct equal_to_impl<filter_view_iterator_tag>
    {
        template<typename It1, typename It2>
        struct apply
          : result_of::equal_to<
                typename detail::remove_reference<It1>::type::begin_type
              , typename detail::remove_reference<It2>::type::begin_type
            >
        {};
    };
}}}

#endif
